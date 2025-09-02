#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <vector>
#include <memory>

/**
 * @file ToastManager.h
 * @brief Non-blocking toast notification system
 * 
 * Provides temporary overlay messages for user feedback without blocking the UI.
 * Messages auto-dismiss with fade animations and can be queued.
 */

class ToastView : public juce::Component,
                  private juce::Timer
{
public:
    enum class Type
    {
        Info,
        Success,
        Error
    };

    ToastView(const juce::String& message, Type type, int durationMs);
    ~ToastView() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    
    bool isExpired() const noexcept { return expired_; }
    void startFadeOut();

private:
    void timerCallback() override;
    juce::Colour getBackgroundColour() const;
    juce::Colour getTextColour() const;

    juce::String message_;
    Type type_;
    int durationMs_;
    bool expired_ = false;
    bool fadingOut_ = false;
    float alpha_ = 0.0f;
    
    static constexpr int FADE_IN_MS = 200;
    static constexpr int FADE_OUT_MS = 300;
    static constexpr int TIMER_INTERVAL_MS = 16; // ~60fps
};

class ToastManager : public juce::Component,
                    private juce::Timer
{
public:
    explicit ToastManager(juce::Component& parent);
    ~ToastManager() override;

    void showInfo(const juce::String& message, int durationMs = 2500);
    void showSuccess(const juce::String& message, int durationMs = 2000);
    void showError(const juce::String& message, int durationMs = 4000);
    
    void clear();
    
    void resized() override;

private:
    void timerCallback() override;
    void updateLayout();
    void addToast(const juce::String& message, ToastView::Type type, int durationMs);

    juce::Component& parent_;
    std::vector<std::unique_ptr<ToastView>> toasts_;
    
    static constexpr int MAX_TOASTS = 5;
    static constexpr int TOAST_HEIGHT = 40;
    static constexpr int TOAST_MARGIN = 10;
    static constexpr int CLEANUP_INTERVAL_MS = 100;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ToastManager)
};