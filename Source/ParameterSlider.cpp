#include "ParameterSlider.h"

ParameterSlider::ParameterSlider (AudioProcessorParameter& param, LookAndFeel *lookAndFeel)
        : Slider (Slider::RotaryHorizontalVerticalDrag, Slider::NoTextBox), 
          parameter (param)
{
    setRange (0.0, 1.0, 0.0);
    setLookAndFeel (lookAndFeel);
    startTimerHz (30);
}

ParameterSlider::~ParameterSlider()
{
}

void ParameterSlider::valueChanged()
{
    if (isMouseButtonDown())
        parameter.setValueNotifyingHost (getValue());
    else
        parameter.setValue (getValue());
}

void ParameterSlider::timerCallback()
{
    const float newValue = parameter.getValue();

    if (newValue != getValue() && ! isMouseButtonDown())
        setValue (newValue);
}