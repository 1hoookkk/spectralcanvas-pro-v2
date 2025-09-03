#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_opengl/juce_opengl.h>
#include "../DSP/SpectralModel.h"
#include "../DSP/SpectralMask.h"
#include "../Core/TiledAtlas.h"
#include "../Core/MessageBus.h"
#include "../Viz/GpuRenderer.h"
#include "BrushTools.h"
#include <memory>
#include <functional>

/**
 * GPU-accelerated spectrogram component using tiled atlas rendering.
 * Supports real-time updates via atomic page flipping for tear-free visualization.
 */
class SpectrogramComponent : public juce::Component, private juce::Timer
{
public:
    SpectrogramComponent() noexcept;
    ~SpectrogramComponent() override;
    
    // Legacy API compatibility
    void setModel (const SpectralModel* m);
    void setEditableMask (SpectralMask* mk) { legacyMask_ = mk; repaint(); }
    void setBrushRadius (int r) { brushRadius_ = juce::jlimit (2, 64, r); }

    // Tiled atlas integration
    void setTiledAtlas(std::shared_ptr<TiledAtlas> atlas) noexcept;
    void setGpuRenderer(std::shared_ptr<GpuRenderer> renderer) noexcept;
    void setAtlasUpdateQueue(AtlasUpdateQueue* queue) noexcept;
    
    // Phase 2 (SEE): start offline analysis and progressive CPU rendering
    void beginAnalysis(const juce::AudioBuffer<float>& mono, double sampleRate,
                       int fftSize, int hop);
    // Optional: external cancellation (e.g., on new file)
    void cancelAnalysis();
    
    // Phase 3 (EDIT): UI → Audio mask deltas
    void setMaskDeltaQueue(MaskDeltaQueue* q) noexcept { maskDeltaQueue_ = q; }
    
    // ---- Perf HUD provider (Editor supplies counters from Processor) ----
    struct HudSnapshot
    {
        // Editor-provided (from processor)
        uint64_t maskPushCount = 0;
        uint64_t maskDropCount = 0;
        uint32_t badBinSkips   = 0;
        uint32_t badColSkips   = 0;
        uint32_t deltaDrainsPerBlock = 0;
    };
    using HudProvider = std::function<HudSnapshot()>;
    void setHudProvider(HudProvider p) noexcept { hudProvider_ = std::move(p); repaint(); }
    
    // Component interface
    void paint (juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;
    
    // Mouse interaction
    void mouseDown (const juce::MouseEvent& e) override;
    void mouseDrag (const juce::MouseEvent& e) override;
    void mouseUp (const juce::MouseEvent& e) override;
    
    // Display settings
    void setColormap(int colormapType) noexcept { colormapType_ = colormapType; }
    void setGainDb(float gainDb) noexcept { displayGainDb_ = gainDb; }
    void setFrequencyRange(float minHz, float maxHz) noexcept;
    void setTimeRange(float minSeconds, float maxSeconds) noexcept;
    
    // Rendering modes
    enum RenderMode {
        Legacy = 0,     // CPU-based juce::Image rendering
        GpuAtlas = 1,   // GPU texture atlas rendering
        Hybrid = 2      // GPU with CPU overlay
    };
    void setRenderMode(RenderMode mode) noexcept { renderMode_ = mode; repaint(); }

private:
    // Legacy rendering (fallback)
    void renderLegacySpectrogram(juce::Graphics& g);
    void rebuildLegacyImage();
    void applyLegacyBrush(const juce::MouseEvent& e, bool additive);
    
    // GPU atlas rendering
    void renderAtlasSpectrogram(juce::Graphics& g);
    void updateAtlasFromQueue();
    void applyAtlasBrush(const juce::MouseEvent& e, bool additive);
    
    // Coordinate transformations
    AtlasPosition screenToAtlasPosition(juce::Point<int> screenPos) const noexcept;
    juce::Point<int> atlasToScreenPosition(const AtlasPosition& atlasPos) const noexcept;
    
    // Mouse state tracking
    bool isDragging_ = false;
    juce::Point<int> lastDragPos_;
    
    // Legacy data (for compatibility)
    const SpectralModel* legacyModel_ = nullptr;
    SpectralMask* legacyMask_ = nullptr;
    juce::Image legacyImage_;
    
    // Tiled atlas system
    std::shared_ptr<TiledAtlas> tiledAtlas_;
    std::shared_ptr<GpuRenderer> gpuRenderer_;
    AtlasUpdateQueue* atlasUpdateQueue_ = nullptr;
    
    // Current atlas state
    AtlasPageHandle currentPage_;
    AtlasPosition viewportTopLeft_; // Top-left position of current view
    
    // === Phase 2 CPU progressive rendering ===
    struct ColumnRing
    {
        struct Node { int64_t col; std::vector<float> mags; };
        std::vector<Node> q;
        std::atomic<size_t> head{0}, tail{0}, mask{0};
        void init(size_t pow2) { q.resize(pow2); mask = pow2 - 1; head = tail = 0; }
        bool push(Node&& n)
        {
            const size_t h = head.load(std::memory_order_relaxed);
            const size_t nh = (h + 1) & mask.load(std::memory_order_relaxed);
            if (nh == tail.load(std::memory_order_acquire)) return false; // full
            q[h] = std::move(n);
            head.store(nh, std::memory_order_release);
            return true;
        }
        bool pop(Node& out)
        {
            const size_t t = tail.load(std::memory_order_relaxed);
            if (t == head.load(std::memory_order_acquire)) return false; // empty
            out = std::move(q[t]);
            tail.store((t + 1) & mask.load(std::memory_order_relaxed), std::memory_order_release);
            return true;
        }
        size_t size() const { return (head.load() - tail.load()) & mask.load(); }
    } ring_;
    
    juce::Image cpuImage_;           // x = column, y = bin
    int cpuBins_ = 0;
    int uploadedColumns_ = 0;
    void uploadColumnsBudgeted_();   // ≤ 64 / frame
    void growImageIfNeeded_(int requiredWidth);
    static juce::Colour magToGrey_(float v);
    HudProvider hudProvider_;
    
    // === Phase 3 mask delta queue ===
    MaskDeltaQueue*   maskDeltaQueue_   = nullptr; // UI→Audio (optional)
    
    // Phase 3 coalescer (last-write-wins per column per frame)
    struct PendingDelta { uint16_t colInTile=0; std::vector<float> values; bool active=false; };
    PendingDelta coalesced_;
    void flushCoalescedDeltas_(); // push ≤128/frame in timerCallback
    
    // Display parameters
    int brushRadius_ = 8;
    int colormapType_ = 0;
    float displayGainDb_ = 0.0f;
    float minFreqHz_ = 0.0f;
    float maxFreqHz_ = 22050.0f;
    float minTimeSeconds_ = 0.0f;
    float maxTimeSeconds_ = 60.0f;
    
    // Render settings
    RenderMode renderMode_ = RenderMode::GpuAtlas;
    
    // Performance tracking
    uint32_t frameCounter_ = 0;
    float lastRenderTimeMs_ = 0.0f;
};