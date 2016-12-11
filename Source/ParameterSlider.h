#ifndef PARAMETERSLIDER_H_INCLUDED
#define PARAMETERSLIDER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class ParameterSlider : public Slider,
                        private Timer
{
public:
    ParameterSlider (AudioProcessorParameter& param, LookAndFeel *lookAndFeel);
    ~ParameterSlider();

    void valueChanged() override;
    void timerCallback() override;
    
private:
    AudioProcessorParameter& parameter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterSlider)
};


#endif  // PARAMETERSLIDER_H_INCLUDED