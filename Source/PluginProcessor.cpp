#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ConvolutionReverbProcessor::ConvolutionReverbProcessor()
    : AudioProcessor (BusesProperties()
                          .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                          .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout ConvolutionReverbProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // TODO: add the plugin's parameters here. Example of the pattern:
    //
    // layout.add (std::make_unique<juce::AudioParameterFloat> (
    //     juce::ParameterID { "example", 1 },   // ID (keep it stable once released)
    //     "Example",                            // name shown in the DAW
    //     0.0f, 1.0f,                           // range
    //     0.5f));                               // default
    //
    // Read it on the audio thread with:
    //     apvts.getRawParameterValue ("example")->load()

    return layout;
}

//==============================================================================
void ConvolutionReverbProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Called before playback starts, and again whenever the sample rate or
    // block size changes. Allocate buffers and prepare DSP objects here.
    //
    // TODO (Milestone 2): build a juce::dsp::ProcessSpec from these values and
    // call prepare() on your DSP objects.
    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void ConvolutionReverbProcessor::releaseResources()
{
    // Called when playback stops. Free anything big you don't need.
}

bool ConvolutionReverbProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // Accept mono or stereo, as long as input and output match.
    const auto out = layouts.getMainOutputChannelSet();

    if (out != juce::AudioChannelSet::mono() && out != juce::AudioChannelSet::stereo())
        return false;

    return out == layouts.getMainInputChannelSet();
}

void ConvolutionReverbProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                               juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);
    juce::ScopedNoDenormals noDenormals;

    // Clear any output channels that have no matching input.
    for (auto ch = getTotalNumInputChannels(); ch < getTotalNumOutputChannels(); ++ch)
        buffer.clear (ch, 0, buffer.getNumSamples());

    // At this point `buffer` holds the input audio. Whatever is in it when
    // this function returns is the output. Doing nothing = passthrough.
    //
    // TODO (Milestone 2): wrap the buffer in a juce::dsp::AudioBlock and run
    // your convolution over it.
}

//==============================================================================
juce::AudioProcessorEditor* ConvolutionReverbProcessor::createEditor()
{
    return new ConvolutionReverbEditor (*this);
}

//==============================================================================
void ConvolutionReverbProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Saves all APVTS parameters with the DAW session.
    // TODO (Milestone 4): also store the path of the loaded IR file, e.g. as a
    // property on `state`, so the session reopens with the same IR.
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void ConvolutionReverbProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml (getXmlFromBinary (data, sizeInBytes));

    if (xml != nullptr && xml->hasTagName (apvts.state.getType()))
        apvts.replaceState (juce::ValueTree::fromXml (*xml));

    // TODO (Milestone 4): read the IR path back and reload the IR.
}

//==============================================================================
// This creates new instances of the plugin.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ConvolutionReverbProcessor();
}
