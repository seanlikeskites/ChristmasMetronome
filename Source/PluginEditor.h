#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "LookAndFeel.h"
#include "ParameterSlider.h"

class ChristmasMetronomeAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    ChristmasMetronomeAudioProcessorEditor (ChristmasMetronomeAudioProcessor& owner);
    ~ChristmasMetronomeAudioProcessorEditor();

    void paint (Graphics& g) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ChristmasMetronomeAudioProcessor& processor;
    
    ScopedPointer <Drawable> background;
    ComponentBoundsConstrainer constrainer;
    int width, height;
    
    ScopedPointer <ParameterSlider> gainSliders [ChristmasMetronomeAudioProcessor::numVoices];
    ScopedPointer <ParameterSlider> lpfSliders [ChristmasMetronomeAudioProcessor::numVoices];
    ScopedPointer <ParameterSlider> hpfSliders [ChristmasMetronomeAudioProcessor::numVoices];
    ChristmasLookAndFeel gainLook, lpfLook, hpfLook;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChristmasMetronomeAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED