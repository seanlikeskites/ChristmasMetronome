#include "PluginProcessor.h"
#include "PluginEditor.h"

ChristmasMetronomeAudioProcessorEditor::ChristmasMetronomeAudioProcessorEditor (ChristmasMetronomeAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

ChristmasMetronomeAudioProcessorEditor::~ChristmasMetronomeAudioProcessorEditor()
{
}

//==============================================================================
void ChristmasMetronomeAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::white);

    g.setColour (Colours::black);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void ChristmasMetronomeAudioProcessorEditor::resized()
{
}