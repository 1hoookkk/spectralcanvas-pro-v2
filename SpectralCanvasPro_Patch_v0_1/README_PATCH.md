# SpectralCanvas Pro — Minimal Paint‑to‑Audio & Spectrogram Patch (v0.1)

This drop‑in patch implements the first "90% critical path":
- **Paint‑to‑Audio**: mouse drags stamp a **gaussian mask** on the spectrum
- **RT‑safe messaging**: UI → Audio via lock‑free **SPSC queue**
- **Spectral processing**: STFT + mask apply + iFFT + overlap‑add
- **Live spectrogram**: audio thread publishes magnitude snapshots to a **triple mailbox**

> These follow the **Paint Accumulation**, **Triple‑Buffer Mailbox**, and **STFT Pipeline** patterns described in your BLUEPRINTS.md. fileciteturn0file0

## Files

```
Source/
  DSP/
    GestureEvent.h
    SpscRingBuffer.h
    TripleMailbox.h
    SpectralEngine.h
  UI/
    CanvasComponent.h
    SpectrogramRenderer.h
README_PATCH.md
```

## Integration (Projucer/CMake)
1. Add the files above to your project. Ensure `juce_dsp` module is enabled.
2. **Member wiring in your AudioProcessor**:
   ```cpp
   // In PluginProcessor.h
   #include "DSP/SpscRingBuffer.h"
   #include "DSP/GestureEvent.h"
   #include "DSP/TripleMailbox.h"
   #include "DSP/SpectralEngine.h"

   class SpectralCanvasProAudioProcessor : public juce::AudioProcessor {
     // ...
     SpscRingBuffer<GestureEvent, 1024> gestureQ_;
     TripleMailbox<SpectralSnapshot> uiMailbox_;
     SpectralEngine engine_;
     // Params (APVTS): BrushGainDb, BrushRadius01, DryWet
   };
   ```

3. **prepareToPlay**:
   ```cpp
   void prepareToPlay (double sampleRate, int samplesPerBlock) override
   {
       SpectralEngine::Settings s;
       s.sampleRate = (float) sampleRate;
       s.fftOrder   = 9;         // 512
       s.hop        = 128;       // 75% overlap
       s.paintDecay = 0.95f;     // from BLUEPRINTS
       engine_.initialize (s, samplesPerBlock);
       engine_.setGestureQueue (&gestureQ_);
       engine_.setUiMailbox (&uiMailbox_);
       engine_.reset();
   }
   ```

4. **processBlock** (mono or first channel to start):
   ```cpp
   void processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override
   {
       juce::ScopedNoDenormals n;
       const int numSamples = buffer.getNumSamples();
       auto* ch0 = buffer.getWritePointer (0);
       // Parameter reads (RT‑safe with JUCE): 
       engine_.setBrushGainDb (apvts.getRawParameterValue ("BrushGainDb")->load());
       engine_.setDryWet       (apvts.getRawParameterValue ("DryWet")->load());
       engine_.processBlock (ch0, numSamples);
       // Copy to other channels for now
       for (int ch=1; ch<buffer.getNumChannels(); ++ch)
           buffer.copyFrom (ch, 0, buffer, 0, 0, numSamples);
   }
   ```

5. **Editor wiring**:
   ```cpp
   // In your Editor .h
   #include "UI/CanvasComponent.h"
   #include "UI/SpectrogramRenderer.h"

   class SpectralCanvasProAudioProcessorEditor : public juce::AudioProcessorEditor {
   public:
     SpectralCanvasProAudioProcessorEditor (SpectralCanvasProAudioProcessor& p, 
                                            SpscRingBuffer<GestureEvent,1024>& q,
                                            TripleMailbox<SpectralSnapshot>& mb,
                                            juce::AudioProcessorValueTreeState& apvts)
       : juce::AudioProcessorEditor (&p), canvas_(q)
     {
         setSize (600, 400);
         addAndMakeVisible (canvas_);
         addAndMakeVisible (spectrogram_);
         spectrogram_.setMailbox (&mb);

         // Parameter attachment example (Slider -> BrushRadius01)
         brushSizeSlider_.setRange (0.01, 0.25, 0.001);
         brushSizeAttachment_ = std::make_unique<juce::SliderParameterAttachment>(
             *apvts.getParameter ("BrushRadius01"), brushSizeSlider_, nullptr);
         brushSizeSlider_.onValueChange = [this]{ canvas_.setBrushRadius01((float)brushSizeSlider_.getValue()); };
         addAndMakeVisible (brushSizeSlider_);
     }
     void resized() override
     {
         auto r = getLocalBounds();
         auto bottom = r.removeFromBottom (40);
         brushSizeSlider_.setBounds (bottom.reduced (8));
         canvas_.setBounds (r.removeFromLeft (r.getWidth()/2));
         spectrogram_.setBounds (r);
     }
   private:
     CanvasComponent canvas_;
     SpectrogramRenderer spectrogram_;
     juce::Slider brushSizeSlider_;
     std::unique_ptr<juce::SliderParameterAttachment> brushSizeAttachment_;
   };
   ```

6. **Parameters (APVTS)**:
   ```cpp
   layout.add (std::make_unique<juce::AudioParameterFloat>("BrushGainDb","Brush Gain (dB)", juce::NormalisableRange<float>(-24.0f, +24.0f, 0.01f), 6.0f));
   layout.add (std::make_unique<juce::AudioParameterFloat>("BrushRadius01","Brush Radius", juce::NormalisableRange<float>(0.01f, 0.25f, 0.001f), 0.08f));
   layout.add (std::make_unique<juce::AudioParameterFloat>("DryWet","Dry/Wet", juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 1.0f));
   ```

## How it works (mapping choices)

- **CanvasComponent::mouseDrag** creates **GestureEvent** with (y, intensity, radius) and pushes into a **SpscRingBuffer**.
- **SpectralEngine** consumes gestures each FFT frame and **stamps a gaussian** over the spectral bins (centered at log‑mapped frequency from *y*). The **mask accumulates with decay** every frame (0.95 by default). *This is the "Paint Accumulation Pattern".* fileciteturn0file0
- The engine applies the mask as **boost or cut** based on `BrushGainDb` and blends with the original signal via `DryWet`.
- Every processed frame, it snapshots magnitudes into a **TripleMailbox** that the **SpectrogramRenderer** reads at ~30 FPS and scrolls as a spectrogram. *This mirrors the "Triple‑Buffer Mailbox" pattern.* fileciteturn0file0

## Next steps (day‑by‑day)

**Day 1**  
- Drop in these files, wire up parameters and editor. Confirm:  
  - Dragging the canvas visibly affects the audio (boost/cut).  
  - Spectrogram scrolls and responds.

**Day 2**  
- Add **file loading** and offline spectrogram in `SpectrogramRenderer` (optional utility).  
- Expose **Fast Mode** toggle (FFT 256 / Hop 64) while dragging; revert on idle. fileciteturn0file0

**Day 3–5**  
- Add sculpt modes (Boost/Cut/Morph), parameter smoothing, and brush shapes.  
- Introduce **Command Queue** for GPU/visual colormap updates when you move to D3D11. fileciteturn0file0

## Notes

- All allocations happen in `initialize()`; **no heap work** in `processBlock()`. fileciteturn0file0
- Dropped gestures are fine; **degrade gracefully** instead of blocking. fileciteturn0file0
- The spectrogram map uses a simple HSV curve (purple→cyan) as a placeholder; replace with your preferred colormap later. fileciteturn0file0
