#ifndef LOOKANDFEEL_H_INCLUDED
#define LOOKANDFEEL_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class ChristmasLookAndFeel : public LookAndFeel_V2
{
public:
    struct ImageData
    {
        const void *rotarySliderImageData;
        int rotarySliderImageSize;
        float rotarySliderImageWidth, rotarySliderImageHeight;
    };
    
    ChristmasLookAndFeel (const ImageData& imageData);
    ~ChristmasLookAndFeel();
    
    void drawRotarySlider (Graphics &g,
                           int x, int y, int width, int height,
                           float sliderPosProportional,
                           float rotaryStartAngle,
                           float rotaryEndAngle,
                           Slider &slider) override;
                           
private:
    float rotarySliderWidth, rotarySliderHeight;
    ScopedPointer <Drawable> rotarySliderImage;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChristmasLookAndFeel)
};

#endif  // LOOKANDFEEL_H_INCLUDED
