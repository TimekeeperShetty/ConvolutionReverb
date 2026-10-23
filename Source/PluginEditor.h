#pragma once

#include "PluginProcessor.h"

//==============================================================================
// The plugin window. Runs on the message (UI) thread, never the audio thread.
//==============================================================================
class ConvolutionReverbEditor final : public juce::AudioProcessorEditor
{
public:
    explicit ConvolutionReverbEditor (ConvolutionReverbProcessor&);
    ~ConvolutionReverbEditor() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    ConvolutionReverbProcessor& processorRef;

    // TODO: add UI components here, e.g.
    //   juce::Slider mixSlider;
    //   std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
    //   juce::TextButton loadIRButton { "Load IR..." };
    //   std::unique_ptr<juce::FileChooser> fileChooser;   // must outlive launchAsync()

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConvolutionReverbEditor)
};
