#include "ToastManager.h"
#include "../Core/DiagnosticLogger.h"

//==============================================================================
ToastView::ToastView(const juce::String& message, Type type, int durationMs)
    : message_(message), type_(type), durationMs_(durationMs)
{
    setSize(300, 40);
    startTimer(TIMER_INTERVAL_MS);
    
    LOGV(UI, "Toast created: %s (type=%d, duration=%dms)", 
         message.toRawUTF8(), static_cast<int>(type), durationMs);
}

ToastView::~ToastView()
{
    stopTimer();
}

void ToastView::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    
    // Background with rounded corners
    g.setColour(getBackgroundColour().withAlpha(alpha_));
    g.fillRoundedRectangle(bounds, 6.0f);
    
    // Border
    g.setColour(getTextColour().withAlpha(alpha_ * 0.8f));
    g.drawRoundedRectangle(bounds, 6.0f, 1.0f);
    
    // Text
    g.setColour(getTextColour().withAlpha(alpha_));
    g.setFont(juce::FontOptions().withPointHeight(13.0f));
    g.drawText(message_, bounds.reduced(12, 8), juce::Justification::centredLeft);
}

void ToastView::resized()
{
    // Fixed size component
}

void ToastView::startFadeOut()
{
    if (!fadingOut_)
    {
        fadingOut_ = true;
        LOGV(UI, "Toast fade out started: %s", message_.toRawUTF8());
    }
}

void ToastView::timerCallback()
{
    if (fadingOut_)
    {
        // Fade out
        alpha_ = juce::jmax(0.0f, alpha_ - (1.0f / (FADE_OUT_MS / TIMER_INTERVAL_MS)));
        if (alpha_ <= 0.0f)
        {
            expired_ = true;
            stopTimer();
            LOGV(UI, "Toast expired: %s", message_.toRawUTF8());
        }
    }
    else
    {
        // Fade in and hold
        if (alpha_ < 1.0f)
        {
            alpha_ = juce::jmin(1.0f, alpha_ + (1.0f / (FADE_IN_MS / TIMER_INTERVAL_MS)));
        }
        else
        {
            // Check if duration expired
            static auto startTime = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - startTime).count();
            
            if (elapsed >= durationMs_)
            {
                startFadeOut();
            }
        }
    }
    
    repaint();
}

juce::Colour ToastView::getBackgroundColour() const
{
    switch (type_)
    {
        case Type::Success: return juce::Colours::darkgreen;
        case Type::Error:   return juce::Colours::darkred;
        case Type::Info:
        default:            return juce::Colours::darkblue;
    }
}

juce::Colour ToastView::getTextColour() const
{
    switch (type_)
    {
        case Type::Success: return juce::Colours::lightgreen;
        case Type::Error:   return juce::Colours::lightcoral;
        case Type::Info:
        default:            return juce::Colours::lightblue;
    }
}

//==============================================================================
ToastManager::ToastManager(juce::Component& parent)
    : parent_(parent)
{
    setSize(parent.getWidth(), parent.getHeight());
    startTimer(CLEANUP_INTERVAL_MS);
    
    LOGI(UI, "ToastManager initialized with parent size %dx%d", 
         parent.getWidth(), parent.getHeight());
}

ToastManager::~ToastManager()
{
    stopTimer();
    clear();
}

void ToastManager::showInfo(const juce::String& message, int durationMs)
{
    addToast(message, ToastView::Type::Info, durationMs);
    LOGI(UI, "Info toast: %s", message.toRawUTF8());
}

void ToastManager::showSuccess(const juce::String& message, int durationMs)
{
    addToast(message, ToastView::Type::Success, durationMs);
    LOGI(UI, "Success toast: %s", message.toRawUTF8());
}

void ToastManager::showError(const juce::String& message, int durationMs)
{
    addToast(message, ToastView::Type::Error, durationMs);
    LOGE(UI, "Error toast: %s", message.toRawUTF8());
}

void ToastManager::clear()
{
    for (auto& toast : toasts_)
    {
        if (toast)
        {
            removeChildComponent(toast.get());
        }
    }
    toasts_.clear();
    LOGV(UI, "All toasts cleared");
}

void ToastManager::resized()
{
    setSize(parent_.getWidth(), parent_.getHeight());
    updateLayout();
}

void ToastManager::timerCallback()
{
    // Remove expired toasts
    bool hasExpired = false;
    for (auto it = toasts_.begin(); it != toasts_.end();)
    {
        if ((*it)->isExpired())
        {
            removeChildComponent(it->get());
            it = toasts_.erase(it);
            hasExpired = true;
        }
        else
        {
            ++it;
        }
    }
    
    if (hasExpired)
    {
        updateLayout();
    }
}

void ToastManager::updateLayout()
{
    int y = TOAST_MARGIN;
    
    for (auto& toast : toasts_)
    {
        if (toast)
        {
            int x = getWidth() - toast->getWidth() - TOAST_MARGIN;
            toast->setBounds(x, y, toast->getWidth(), TOAST_HEIGHT);
            y += TOAST_HEIGHT + TOAST_MARGIN;
        }
    }
}

void ToastManager::addToast(const juce::String& message, ToastView::Type type, int durationMs)
{
    // Remove oldest if at capacity
    while (toasts_.size() >= MAX_TOASTS)
    {
        auto& oldest = toasts_.front();
        if (oldest)
        {
            oldest->startFadeOut();
        }
        removeChildComponent(oldest.get());
        toasts_.erase(toasts_.begin());
    }
    
    // Create new toast
    auto toast = std::make_unique<ToastView>(message, type, durationMs);
    addAndMakeVisible(*toast);
    toasts_.push_back(std::move(toast));
    
    updateLayout();
    
    LOGV(UI, "Toast added, total count: %d", static_cast<int>(toasts_.size()));
}