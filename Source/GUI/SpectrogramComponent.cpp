#include "SpectrogramComponent.h"
#include "../Core/TimeUtils.h"
#include <cmath>
#include <algorithm>

SpectrogramComponent::SpectrogramComponent() noexcept
    : currentPage_{}
    , viewportTopLeft_{}
    , frameCounter_(0)
    , lastRenderTimeMs_(0.0f)
{
    setOpaque(false);
    setBufferedToImage(false);
    
    // Phase 2: init CPU column ring + timer
    ring_.init(1u << 12); // 4096 slots
    startTimerHz(60); // UI uploads at ~60 fps
}

SpectrogramComponent::~SpectrogramComponent() = default;

void SpectrogramComponent::setModel(const SpectralModel* m) {
    legacyModel_ = m; 
    if (renderMode_ == RenderMode::Legacy) {
        rebuildLegacyImage();
    }
}

void SpectrogramComponent::setTiledAtlas(std::shared_ptr<TiledAtlas> atlas) noexcept {
    tiledAtlas_ = std::move(atlas);
    if (tiledAtlas_) {
        currentPage_ = tiledAtlas_->getActivePage();
        if (!currentPage_.isValid()) {
            // Allocate and activate a page on UI thread for immediate use
            auto h = tiledAtlas_->allocateFreePage();
            if (h.isValid()) {
                tiledAtlas_->activatePage(h);
                currentPage_ = h;
            }
        }
    }
    repaint();
}

void SpectrogramComponent::setGpuRenderer(std::shared_ptr<GpuRenderer> renderer) noexcept {
    gpuRenderer_ = std::move(renderer);
    repaint();
}

void SpectrogramComponent::setAtlasUpdateQueue(AtlasUpdateQueue* queue) noexcept {
    atlasUpdateQueue_ = queue;
}

void SpectrogramComponent::paint(juce::Graphics& g) {
    const auto startTime = TimeUtils::getCurrentTimeUs();
    
    g.fillAll(juce::Colours::black);
    
    // Phase 2: draw CPU progressive spectrogram first (underlays GPU)
    if (cpuImage_.isValid())
        g.drawImageTransformed(cpuImage_,
                               juce::AffineTransform::scale(
                                  (float)getWidth()  / (float)cpuImage_.getWidth(),
                                  (float)getHeight() / (float)cpuImage_.getHeight()));

    // Minimal overlay: highlight last painted column on top of CPU image
    if (lastPaint_.framesLeft > 0 && lastPaint_.values.size() > 0)
    {
        const int W = getWidth();
        const int H = getHeight();
        const int numBins = (int) lastPaint_.values.size();
        const int x = (int)((int(lastPaint_.colInTile) - (int)viewportTopLeft_.columnInTile) * W / (int)AtlasConfig::TILE_WIDTH);
        const float fade = (float) lastPaint_.framesLeft / 30.0f; // ~0.5s at 60fps
        if (x >= 0 && x < W)
        {
            for (int b = 0; b < numBins; ++b)
            {
                const float v = lastPaint_.values[(size_t)b];
                if (v >= 0.999f) continue; // no attenuation → no overlay
                const int y = H - 1 - (b * H / (int)AtlasConfig::TILE_HEIGHT);
                const float alpha = juce::jlimit(0.0f, 1.0f, (1.0f - v) * 0.45f * fade);
                g.setColour(juce::Colours::red.withAlpha(alpha));
                g.fillRect(x, juce::jmax(0, y-1), 1, 2); // small vertical dashes for visibility
            }
        }
    }
    
    switch (renderMode_) {
        case RenderMode::Legacy:
            renderLegacySpectrogram(g);
            break;
            
        case RenderMode::GpuAtlas:
            renderAtlasSpectrogram(g);
            break;
            
        case RenderMode::Hybrid:
            renderAtlasSpectrogram(g);
            renderLegacySpectrogram(g);
            break;
    }
    
    const auto endTime = TimeUtils::getCurrentTimeUs();
    lastRenderTimeMs_ = static_cast<float>(endTime - startTime) / 1000.0f;
    frameCounter_++;

    // ---- Perf HUD (top-left) ----
    g.setColour(juce::Colours::white.withAlpha(0.9f));
    g.setFont(juce::FontOptions{12.0f});
    juce::String hud;
    hud << "FPS est: ~" << juce::String(1000.0f / juce::jmax(1.0f, lastRenderTimeMs_), 1)
        << "  CPU img: " << cpuImage_.getWidth() << "x" << cpuImage_.getHeight()
        << "  Uploaded/frame: " << uploadedColumns_
        << "  ColIn: "  << (juce::int64)colIn_.load()
        << "  ColOut: " << (juce::int64)colOut_.load() << "\n";
    if (tooShort_.load()) hud << "File shorter than FFT size — no columns.\n";
    if (hudProvider_)
    {
        const auto s = hudProvider_();
        hud << "Mask push: " << (juce::int64)s.maskPushCount
            << "  drop: " << (juce::int64)s.maskDropCount
            << "  badBins: " << (int)s.badBinSkips
            << "  badCols: " << (int)s.badColSkips
            << "  drains/block: " << (int)s.deltaDrainsPerBlock;
    }
    g.drawMultiLineText(hud, 8, 16, juce::jmax(220, getWidth() / 3));
}

void SpectrogramComponent::renderLegacySpectrogram(juce::Graphics& g) {
    if (legacyImage_.isValid()) {
        g.drawImageTransformed(legacyImage_, 
                             juce::AffineTransform::scale(static_cast<float>(getWidth()) / legacyImage_.getWidth(),
                                                        static_cast<float>(getHeight()) / legacyImage_.getHeight()));
    }
    
    if (legacyMask_ && legacyModel_ && legacyModel_->isReady()) {
        const int W = getWidth(), H = getHeight();
        const int F = legacyModel_->numFrames();
        const int B = legacyModel_->numBins();
        
        juce::Image overlay(juce::Image::ARGB, W, H, true);
        for (int x = 0; x < W; ++x) {
            const int f = juce::jlimit(0, F-1, static_cast<int>(std::floor((x / static_cast<float>(W)) * F)));
            for (int y = 0; y < H; ++y) {
                const int b = juce::jlimit(0, B-1, static_cast<int>(std::floor((1.0f - y / static_cast<float>(H)) * B)));
                const float m = legacyMask_->get(f, b);
                if (m < 0.999f) {
                    jassert(x >= 0 && x < overlay.getWidth());
                    jassert(y >= 0 && y < overlay.getHeight());
                    overlay.setPixelAt(x, y, juce::Colour(255, 0, 0).withAlpha((1.0f - m) * 0.5f));
                }
            }
        }
        g.drawImageAt(overlay, 0, 0);
    }
}

void SpectrogramComponent::renderAtlasSpectrogram(juce::Graphics& g) {
    if (!gpuRenderer_ || !tiledAtlas_) {
        g.setColour(juce::Colours::darkgrey);
        g.drawText("GPU Atlas Not Available", getLocalBounds(), juce::Justification::centred);
        return;
    }
    
    updateAtlasFromQueue();
    
    if (tiledAtlas_->hasPageUpdate()) {
        currentPage_ = tiledAtlas_->getActivePage();
        tiledAtlas_->acknowledgePageUpdate();
    }
    
    if (!currentPage_.isValid()) {
        g.setColour(juce::Colours::darkgrey);
        g.drawText("No Atlas Data", getLocalBounds(), juce::Justification::centred);
        return;
    }
    
    const float* atlasData = tiledAtlas_->getPageData(currentPage_);
    if (!atlasData) {
        g.setColour(juce::Colours::darkgrey);
        g.drawText("Atlas Data Invalid", getLocalBounds(), juce::Justification::centred);
        return;
    }
    
    if (gpuRenderer_->isInitialized()) {
        SpectralFrame frame;
        
        const size_t atlasX = viewportTopLeft_.columnInTile;
        const size_t atlasY = viewportTopLeft_.binStart;
        
        for (size_t bin = 0; bin < SpectralFrame::NUM_BINS && bin < AtlasConfig::TILE_HEIGHT; ++bin) {
            const size_t atlasIndex = (atlasY + bin) * AtlasConfig::ATLAS_WIDTH + atlasX;
            if (atlasIndex < AtlasConfig::ATLAS_WIDTH * AtlasConfig::ATLAS_HEIGHT) {
                frame.magnitude[bin] = atlasData[atlasIndex];
                frame.phase[bin] = 0.0f;
            }
        }
        
        frame.sequenceNumber = frameCounter_;
        frame.timestamp = TimeUtils::getCurrentTimeUs();
        
        gpuRenderer_->updateSpectralTexture(frame);
        gpuRenderer_->renderSpectralVisualization();
    } else {
        const int W = getWidth();
        const int H = getHeight();
        
        juce::Image atlasImage(juce::Image::RGB, W, H, false);
        
        for (int x = 0; x < W; ++x) {
            const float atlasX = viewportTopLeft_.columnInTile + (x * AtlasConfig::TILE_WIDTH) / static_cast<float>(W);
            
            for (int y = 0; y < H; ++y) {
                const float atlasY = viewportTopLeft_.binStart + ((H - 1 - y) * AtlasConfig::TILE_HEIGHT) / static_cast<float>(H);
                
                const size_t atlasIndex = static_cast<size_t>(atlasY) * AtlasConfig::ATLAS_WIDTH + static_cast<size_t>(atlasX);
                
                float intensity = 0.0f;
                if (atlasIndex < AtlasConfig::ATLAS_WIDTH * AtlasConfig::ATLAS_HEIGHT) {
                    intensity = atlasData[atlasIndex];
                }
                
                intensity = std::pow(intensity, 1.0f / (1.0f + displayGainDb_ / 20.0f));
                intensity = juce::jlimit(0.0f, 1.0f, intensity);
                
                const auto pixelValue = static_cast<juce::uint8>(intensity * 255.0f);
                jassert(x >= 0 && x < atlasImage.getWidth());
                jassert(y >= 0 && y < atlasImage.getHeight());
                atlasImage.setPixelAt(x, y, juce::Colour(pixelValue, pixelValue, pixelValue));
            }
        }
        
        g.drawImageAt(atlasImage, 0, 0);
    }
}

void SpectrogramComponent::updateAtlasFromQueue() {
    if (!atlasUpdateQueue_) return;
    
    AtlasUpdate update;
    while (atlasUpdateQueue_->pop(update)) {
        if (update.isPageFlip()) {
            currentPage_ = update.pendingPageHandle;
        } else if (update.isColumnUpdate()) {
            repaint();
        }
    }
}

void SpectrogramComponent::resized() {
    if (renderMode_ == RenderMode::Legacy) {
        rebuildLegacyImage();
    }
    
    if (gpuRenderer_) {
        gpuRenderer_->resizeBuffers(getWidth(), getHeight());
    }
}

void SpectrogramComponent::mouseDown(const juce::MouseEvent& e) {
    isDragging_ = true;
    lastDragPos_ = e.getPosition();
    
    if (renderMode_ == RenderMode::Legacy) {
        applyLegacyBrush(e, !e.mods.isAltDown());
    } else {
        applyAtlasBrush(e, !e.mods.isAltDown());
    }
}

void SpectrogramComponent::mouseDrag(const juce::MouseEvent& e) {
    if (!isDragging_) return;
    
    lastDragPos_ = e.getPosition();
    
    if (renderMode_ == RenderMode::Legacy) {
        applyLegacyBrush(e, !e.mods.isAltDown());
    } else {
        applyAtlasBrush(e, !e.mods.isAltDown());
    }
}

void SpectrogramComponent::mouseUp(const juce::MouseEvent& e) {
    juce::ignoreUnused(e);
    isDragging_ = false;
}

AtlasPosition SpectrogramComponent::screenToAtlasPosition(juce::Point<int> screenPos) const noexcept {
    const float normalizedX = static_cast<float>(screenPos.x) / static_cast<float>(getWidth());
    const float normalizedY = static_cast<float>(getHeight() - screenPos.y) / static_cast<float>(getHeight());
    
    AtlasPosition pos = viewportTopLeft_;
    pos.columnInTile += static_cast<uint16_t>(normalizedX * AtlasConfig::TILE_WIDTH);
    pos.binStart += static_cast<uint16_t>(normalizedY * AtlasConfig::TILE_HEIGHT);
    
    pos.columnInTile = std::min(pos.columnInTile, static_cast<uint16_t>(AtlasConfig::TILE_WIDTH - 1));
    pos.binStart = std::min(pos.binStart, static_cast<uint16_t>(AtlasConfig::TILE_HEIGHT - 1));
    
    return pos;
}

juce::Point<int> SpectrogramComponent::atlasToScreenPosition(const AtlasPosition& atlasPos) const noexcept {
    const int deltaX = atlasPos.columnInTile - viewportTopLeft_.columnInTile;
    const int deltaY = atlasPos.binStart - viewportTopLeft_.binStart;
    
    const int screenX = (deltaX * getWidth()) / AtlasConfig::TILE_WIDTH;
    const int screenY = getHeight() - (deltaY * getHeight()) / AtlasConfig::TILE_HEIGHT;
    
    return {screenX, screenY};
}

void SpectrogramComponent::setFrequencyRange(float minHz, float maxHz) noexcept {
    minFreqHz_ = std::max(0.0f, minHz);
    maxFreqHz_ = std::max(minFreqHz_, maxHz);
    repaint();
}

void SpectrogramComponent::setTimeRange(float minSeconds, float maxSeconds) noexcept {
    minTimeSeconds_ = std::max(0.0f, minSeconds);
    maxTimeSeconds_ = std::max(minTimeSeconds_, maxSeconds);
    repaint();
}

void SpectrogramComponent::rebuildLegacyImage() {
    if (!legacyModel_ || !legacyModel_->isReady()) {
        legacyImage_ = {};
        repaint();
        return;
    }
    
    const int W = legacyModel_->numFrames();
    const int H = legacyModel_->numBins();
    legacyImage_ = juce::Image(juce::Image::RGB, juce::jmax(1, W), juce::jmax(1, H), false);

    const float floorDb = -80.0f;
    for (int x = 0; x < W; ++x) {
        for (int y = 0; y < H; ++y) {
            const float mag = legacyModel_->magnitude(x, y);
            const float db = 20.0f * std::log10(mag + 1e-9f);
            const float t = juce::jlimit(0.0f, 1.0f, (db - floorDb) / -floorDb);
            const auto v = static_cast<juce::uint8>(juce::jlimit(0, 255, static_cast<int>(std::round(t * 255.0f))));
            const int px = x;
            const int py = H - 1 - y;
            jassert(px >= 0 && px < legacyImage_.getWidth());
            jassert(py >= 0 && py < legacyImage_.getHeight());
            legacyImage_.setPixelAt(px, py, juce::Colour(v, v, v));
        }
    }
    repaint();
}

void SpectrogramComponent::applyLegacyBrush(const juce::MouseEvent& e, bool additive) {
    if (!(legacyMask_ && legacyModel_ && legacyModel_->isReady())) return;
    
    const int W = getWidth(), H = getHeight();
    const int F = legacyModel_->numFrames(), B = legacyModel_->numBins();

    const int cx = juce::jlimit(0, W-1, static_cast<int>(e.position.x));
    const int cy = juce::jlimit(0, H-1, static_cast<int>(e.position.y));
    const int f0 = juce::jlimit(0, F-1, static_cast<int>(std::floor((cx / static_cast<float>(W)) * F)));
    const int b0 = juce::jlimit(0, B-1, static_cast<int>(std::floor((1.0f - cy / static_cast<float>(H)) * B)));

    const int r = brushRadius_;
    for (int df = -r; df <= r; ++df) {
        for (int db = -r; db <= r; ++db) {
            if (df*df + db*db > r*r) continue;
            
            const int f = juce::jlimit(0, F-1, f0 + df);
            const int b = juce::jlimit(0, B-1, b0 + db);
            const float cur = legacyMask_->get(f, b);
            const float v = additive ? juce::jmin(1.0f, cur + 0.9f) : 0.0f;
            legacyMask_->set(f, b, v);
        }
    }
    repaint();
}

void SpectrogramComponent::applyAtlasBrush(const juce::MouseEvent& e, bool additive) {
    juce::ignoreUnused(additive);
    if (!maskDeltaQueue_) return; // not wired yet — safe no-op
    
    const auto pos = screenToAtlasPosition(e.getPosition());
    const int numBins = AtlasConfig::NUM_BINS;
    
    // Coalesce on this column: last write wins within the frame
    if (!coalesced_.active || coalesced_.colInTile != pos.columnInTile)
    {
        coalesced_.colInTile = pos.columnInTile;
        coalesced_.values.assign((size_t)numBins, 1.0f);
        coalesced_.active = true;
    }
    
    // Simple mapping: mouse Y → center bin
    const int centerBin = juce::jlimit(0, numBins - 1, (int)pos.binStart);
    tools::BrushParams bp; bp.radiusBins = 8.0f; bp.strength = 0.8f;
    tools::GainBrush::stroke(coalesced_.values, centerBin, numBins, bp);
}

void SpectrogramComponent::timerCallback() {
    uploadedColumns_ = 0;
    uploadColumnsBudgeted_(); // ≤ 64 columns
    flushCoalescedDeltas_();  // ≤ 128 deltas
    repaint(); // always repaint to update HUD counters
    
    if (renderMode_ != RenderMode::Legacy) {
        updateAtlasFromQueue();
    }

    // Fade last-paint overlay
    if (lastPaint_.framesLeft > 0) --lastPaint_.framesLeft;
}

// ===== Phase 2 helpers =====
void SpectrogramComponent::beginAnalysis(const juce::AudioBuffer<float>& mono, double sampleRate,
                                         int fftSize, int hop)
{
    DBG("[SCP] beginAnalysis sr=" << sampleRate << " fft=" << fftSize << " hop=" << hop
         << " samples=" << mono.getNumSamples());
    cpuBins_ = fftSize / 2;
    cpuImage_ = juce::Image(juce::Image::RGB, juce::jmax(2048, 1), juce::jmax(cpuBins_, 1), false);
    uploadedColumns_ = 0;
    colIn_ = 0; colOut_ = 0; tooShort_ = false;
    
    // For diagnostic - generate some test data if file is too short
    if (mono.getNumSamples() < fftSize) {
        tooShort_ = true;
        DBG("[SCP] File too short for FFT: " << mono.getNumSamples() << " < " << fftSize);
    } else {
        // TODO: Wire to simplified STFT analyzer when ready
        // For now, generate a few test columns to verify the data path
        generateTestColumns(mono.getNumSamples(), fftSize, hop);
    }
    repaint();
}

void SpectrogramComponent::cancelAnalysis()
{
    // TODO: Wire to analyzer cancellation when ready
}

void SpectrogramComponent::enqueueCpuColumn(int64_t columnIndex, const float* magnitudes, size_t numBins) noexcept
{
    if (magnitudes == nullptr || numBins == 0)
        return;

    // Initialize image height if not yet set
    if (cpuBins_ == 0)
        cpuBins_ = static_cast<int>(numBins);

    ColumnRing::Node n;
    n.col = columnIndex;
    n.mags.assign(magnitudes, magnitudes + numBins);

    // Non-blocking push; if full, drop oldest then retry
    if (!ring_.push(ColumnRing::Node{ n })) {
        ColumnRing::Node throwaway;
        (void) ring_.pop(throwaway);
        (void) ring_.push(std::move(n));
    }
}

void SpectrogramComponent::uploadColumnsBudgeted_()
{
    const int budget = 64;
    int used = 0;
    ColumnRing::Node node;
    while (used < budget && ring_.pop(node))
    {
        if (node.mags.empty()) continue; // nothing to draw
        // Ensure image is wide enough
        growImageIfNeeded_((int)node.col + 1);
        
        const int x = (int)node.col;
        // very simple magnitude → greyscale map (improve later)
        for (int y = 0; y < cpuBins_ && y < cpuImage_.getHeight(); ++y)
        {
            const float v = juce::jlimit(0.0f, 1.0f, node.mags[(size_t)y] * 0.02f);
            const int py = cpuImage_.getHeight() - 1 - y;
            jassert(x >= 0 && x < cpuImage_.getWidth());
            jassert(py >= 0 && py < cpuImage_.getHeight());
            cpuImage_.setPixelAt(x, py, magToGrey_(v));
        }
        ++used; ++uploadedColumns_;
        colOut_.fetch_add(1, std::memory_order_relaxed);
    }
}

void SpectrogramComponent::growImageIfNeeded_(int requiredWidth)
{
    if (requiredWidth <= cpuImage_.getWidth()) return;
    const int newW = juce::jmax(requiredWidth, cpuImage_.getWidth() * 2);
    juce::Image bigger(juce::Image::RGB, newW, cpuImage_.getHeight(), false);
    juce::Graphics gb(bigger);
    gb.drawImageAt(cpuImage_, 0, 0);
    cpuImage_ = std::move(bigger);
}

juce::Colour SpectrogramComponent::magToGrey_(float v)
{
    const auto u = (juce::uint8) juce::jlimit(0, 255, (int)std::round(v * 255.0f));
    return juce::Colour(u, u, u);
}

void SpectrogramComponent::flushCoalescedDeltas_()
{
    if (!maskDeltaQueue_ || !coalesced_.active) return;
    
    MaskColumnDelta d{};
    d.position.tileX = 0; // assuming single tile view for now
    d.position.tileY = 0;
    d.position.columnInTile = coalesced_.colInTile;
    d.metadata.samplePosition = 0; // optional: fill from playhead mapping later
    // Copy NUM_BINS values
    const int nb = AtlasConfig::NUM_BINS;
    for (int i = 0; i < nb; ++i) d.values[(size_t)i] = coalesced_.values[(size_t)i];
    
    (void)maskDeltaQueue_->push(d); // drop when full (bounded)

    coalesced_.active = false;

    // Update last-paint overlay (short-lived)
    lastPaint_.colInTile = d.position.columnInTile;
    lastPaint_.values.assign(std::begin(d.values), std::begin(d.values) + (int)AtlasConfig::NUM_BINS);
    lastPaint_.framesLeft = 30; // ~0.5 seconds
}

void SpectrogramComponent::generateTestColumns(int numSamples, int fftSize, int hop)
{
    // TEMP: Generate synthetic test columns to verify data path
    const int numColumns = (numSamples - fftSize) / hop + 1;
    const int testColumns = juce::jmin(20, numColumns); // Generate up to 20 test columns
    
    DBG("[SCP] Generating " << testColumns << " test columns for diagnostic");
    
    for (int col = 0; col < testColumns; ++col)
    {
        ColumnRing::Node node;
        node.col = col;
        node.mags.resize(cpuBins_);
        
        // Generate simple test pattern: low freq content with some harmonics
        for (int bin = 0; bin < cpuBins_; ++bin)
        {
            const float freq = (float)bin / (float)cpuBins_;
            const float mag = std::exp(-freq * 3.0f) * (0.5f + 0.5f * std::sin(freq * 10.0f + col * 0.1f));
            node.mags[bin] = mag * 50.0f; // Scale up for visibility
        }
        
        if (ring_.push(std::move(node)))
        {
            colIn_.fetch_add(1, std::memory_order_relaxed);
        }
    }
}
