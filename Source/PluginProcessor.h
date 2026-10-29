#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

//==============================================================================
// The audio side of the plugin. Everything in processBlock() runs on the
// real-time audio thread: no memory allocation, no file I/O, no locks there.
//
// Right now this is a stereo passthrough. Milestone 2 (see README) is to put
// a convolution engine in here.
//==============================================================================
class ConvolutionReverbProcessor final : public juce::AudioProcessor
{
public:
    ConvolutionReverbProcessor();
    ~ConvolutionReverbProcessor() override = default;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    //==============================================================================
    const juce::String getName() const override { return JucePlugin_Name; }

    bool acceptsMidi() const override  { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }

    // TODO: a reverb has a tail! Hosts use this value to decide how long to keep
    // processing after the input stops (e.g. when bouncing). Return the length
    // of the loaded IR in seconds once you have one.
    double getTailLengthSeconds() const override { return 0.0; }

    //==============================================================================
    int getNumPrograms() override                             { return 1; }
    int getCurrentProgram() override                          { return 0; }
    void setCurrentProgram (int) override                     {}
    const juce::String getProgramName (int) override          { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    // All user-facing parameters live here. The editor attaches sliders to it,
    // and it handles saving/loading parameter values with the DAW session.
    juce::AudioProcessorValueTreeState apvts
    {
            *this, nullptr, "Parameters", createParameterLayout()
    };

private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // TODO (Milestone 2): add your DSP objects here, e.g. a juce::dsp::Convolution.
    // They are prepared in prepareToPlay() and used in processBlock().

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConvolutionReverbProcessor)
};
