#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_core/juce_core.h>
#include "../Source/Core/SampleLoaderService.h"
#include "../Source/Core/DiagnosticLogger.h"
#include "../Source/SpectralCanvasProAudioProcessor.h"

/**
 * @file SampleLoadTest.cpp
 * @brief Comprehensive tests for sample loading pipeline
 */

class SampleLoadTest : public juce::UnitTest
{
public:
    SampleLoadTest() : juce::UnitTest("Sample Loading Pipeline", "Core") {}

    void runTest() override
    {
        beginTest("SampleLoaderService Creation");
        testSampleLoaderCreation();
        
        beginTest("Supported Extensions");
        testSupportedExtensions();
        
        beginTest("Error Message Mapping");
        testErrorMessages();
        
        beginTest("File Loading - Positive Cases");
        testPositiveFileLoading();
        
        beginTest("File Loading - Negative Cases");
        testNegativeFileLoading();
        
        beginTest("AudioProcessor Renderer Activation");
        testRendererActivation();
    }

private:
    void testSampleLoaderCreation()
    {
        auto loader = std::make_unique<SampleLoaderService>();
        expect(loader != nullptr, "SampleLoaderService should be creatable");
        
        auto extensions = loader->getSupportedExtensions();
        expect(extensions.size() > 0, "Should have supported extensions");
        
        auto formatsString = loader->getSupportedFormatsString();
        expect(formatsString.isNotEmpty(), "Should have formats string");
        
        LOGI(LOADER, "Supported formats: %s", formatsString.toRawUTF8());
    }
    
    void testSupportedExtensions()
    {
        SampleLoaderService loader;
        
        // Always supported
        expect(loader.isExtensionSupported(".wav"), "WAV should be supported");
        expect(loader.isExtensionSupported(".aiff"), "AIFF should be supported");
        expect(loader.isExtensionSupported(".flac"), "FLAC should be supported");
        
        // Test case sensitivity
        expect(loader.isExtensionSupported(".WAV"), "WAV uppercase should be supported");
        expect(loader.isExtensionSupported("wav"), "WAV without dot should be supported");
        
        // Unsupported
        expect(!loader.isExtensionSupported(".xyz"), "Unknown extension should not be supported");
        
#ifdef SPECTRAL_ENABLE_MP3
        expect(loader.isExtensionSupported(".mp3"), "MP3 should be supported when enabled");
        LOGI(DECODER, "MP3 support verified in test");
#else
        expect(!loader.isExtensionSupported(".mp3"), "MP3 should not be supported when disabled");
        LOGI(DECODER, "MP3 support disabled as expected");
#endif
    }
    
    void testErrorMessages()
    {
        // Test all error codes have messages
        auto codes = {
            SampleLoaderService::ErrorCode::Success,
            SampleLoaderService::ErrorCode::FileDialogCancelled,
            SampleLoaderService::ErrorCode::FileOpenFailed,
            SampleLoaderService::ErrorCode::UnsupportedFormat,
            SampleLoaderService::ErrorCode::DecodeFailed,
            SampleLoaderService::ErrorCode::RendererInitFailed,
            SampleLoaderService::ErrorCode::InvalidSampleData
        };
        
        for (auto code : codes)
        {
            auto message = SampleLoaderService::getErrorMessage(code);
            expect(message.isNotEmpty(), "Error message should not be empty");
            LOGV(LOADER, "Error code %d: %s", static_cast<int>(code), message.toRawUTF8());
        }
    }
    
    void testPositiveFileLoading()
    {
        // Create a test WAV file in memory
        createTestWavFile();
        
        if (!testWavFile_.existsAsFile())
        {
            expect(false, "Test WAV file was not created successfully");
            return;
        }
        
        SampleLoaderService loader;
        bool callbackExecuted = false;
        SampleLoaderService::Result lastResult;
        
        // Test loading the file
        loader.loadFromFile(testWavFile_, [&](SampleLoaderService::Result result)
        {
            callbackExecuted = true;
            lastResult = std::move(result);
        });
        
        // Wait for async operation to complete
        auto startTime = juce::Time::getMillisecondCounter();
        while (!callbackExecuted && (juce::Time::getMillisecondCounter() - startTime) < 5000)
        {
            juce::MessageManager::getInstance()->runDispatchLoopUntil(10);
        }
        
        expect(callbackExecuted, "Load callback should have been called");
        expect(lastResult.isSuccess(), "Test WAV loading should succeed");
        
        if (lastResult.sample)
        {
            auto& sample = *lastResult.sample;
            expect(sample.numSamples > 0, "Loaded sample should have samples");
            expect(sample.numChannels > 0, "Loaded sample should have channels");
            expect(sample.sampleRate > 0, "Loaded sample should have valid sample rate");
            expect(sample.durationSeconds > 0, "Loaded sample should have duration");
            
            LOGI(DECODER, "Test WAV loaded: %d samples, %d channels, %.1fkHz, %.2fs",
                 sample.numSamples, sample.numChannels, sample.sampleRate / 1000.0, sample.durationSeconds);
        }
    }
    
    void testNegativeFileLoading()
    {
        SampleLoaderService loader;
        
        // Test non-existent file
        {
            bool callbackExecuted = false;
            SampleLoaderService::Result lastResult;
            
            juce::File nonExistentFile("nonexistent_file.wav");
            loader.loadFromFile(nonExistentFile, [&](SampleLoaderService::Result result)
            {
                callbackExecuted = true;
                lastResult = std::move(result);
            });
            
            // Wait for callback
            auto startTime = juce::Time::getMillisecondCounter();
            while (!callbackExecuted && (juce::Time::getMillisecondCounter() - startTime) < 2000)
            {
                juce::MessageManager::getInstance()->runDispatchLoopUntil(10);
            }
            
            expect(callbackExecuted, "Error callback should have been called");
            expect(lastResult.hasError(), "Non-existent file should produce error");
            expect(lastResult.code == SampleLoaderService::ErrorCode::FileOpenFailed, 
                   "Should get FileOpenFailed error");
        }
    }
    
    void testRendererActivation()
    {
        // Create a minimal audio processor
        SpectralCanvasProAudioProcessor processor;
        
        // Initially should have no active renderer
        expect(!processor.hasActiveRenderer(), "Should start with no active renderer");
        
        // Create test audio buffer
        juce::AudioBuffer<float> testBuffer(1, 44100); // 1 second mono
        testBuffer.clear();
        
        // Generate a test tone
        for (int i = 0; i < testBuffer.getNumSamples(); ++i)
        {
            float sample = std::sin(2.0f * juce::MathConstants<float>::pi * 440.0f * i / 44100.0f);
            testBuffer.setSample(0, i, sample * 0.5f);
        }
        
        // Activate renderer
        bool activated = processor.activateSampleRenderer(testBuffer, 44100.0);
        expect(activated, "Renderer activation should succeed with valid data");
        expect(processor.hasActiveRenderer(), "Should have active renderer after activation");
        
        LOGI(RENDERER, "Renderer activation test passed");
        
        // Test deactivation
        processor.deactivateRenderer();
        expect(!processor.hasActiveRenderer(), "Should have no active renderer after deactivation");
    }
    
    void createTestWavFile()
    {
        testWavFile_ = juce::File::getSpecialLocation(juce::File::tempDirectory)
                         .getChildFile("test_sample_load.wav");
        
        if (testWavFile_.existsAsFile())
            testWavFile_.deleteFile();
        
        // Create a simple 1-second sine wave
        const int sampleRate = 44100;
        const int numSamples = sampleRate; // 1 second
        const int numChannels = 1;
        
        juce::AudioBuffer<float> buffer(numChannels, numSamples);
        
        // Generate 440Hz sine wave
        for (int i = 0; i < numSamples; ++i)
        {
            float sample = std::sin(2.0f * juce::MathConstants<float>::pi * 440.0f * i / float(sampleRate));
            buffer.setSample(0, i, sample * 0.5f);
        }
        
        // Write to WAV file
        juce::WavAudioFormat wavFormat;
        std::unique_ptr<juce::FileOutputStream> fileStream(testWavFile_.createOutputStream());
        
        if (fileStream)
        {
            std::unique_ptr<juce::AudioFormatWriter> writer(
                wavFormat.createWriterFor(fileStream.get(), sampleRate, numChannels, 16, {}, 0));
            
            if (writer)
            {
                fileStream.release(); // Writer takes ownership
                writer->writeFromAudioBuffer(buffer, 0, numSamples);
                writer.reset(); // Flush and close
                
                LOGI(DECODER, "Test WAV file created: %s", testWavFile_.getFullPathName().toRawUTF8());
            }
        }
    }
    
    juce::File testWavFile_;
};

static SampleLoadTest sampleLoadTest;