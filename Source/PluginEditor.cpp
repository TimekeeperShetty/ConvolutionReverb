#include "PluginEditor.h"

//==============================================================================
ConvolutionReverbEditor::ConvolutionReverbEditor (ConvolutionReverbProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);

    // TODO: addAndMakeVisible() your components and create their attachments here.

    setSize (500, 300);
}

void ConvolutionReverbEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (20.0f));
    g.drawFittedText ("Convolution Reverb\n(passthrough - nothing here yet)",
                      getLocalBounds(), juce::Justification::centred, 2);
}

void ConvolutionReverbEditor::resized()
{
    // TODO: lay out your components here with setBounds(),
    // or use juce::FlexBox / juce::Grid.
}
