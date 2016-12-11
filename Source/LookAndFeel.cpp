#include "LookAndFeel.h"

ChristmasLookAndFeel::ChristmasLookAndFeel (const ImageData& imageData)
    : rotarySliderWidth (imageData.rotarySliderImageWidth), 
      rotarySliderHeight (imageData.rotarySliderImageHeight),
      rotarySliderImage (Drawable::createFromImageData (imageData.rotarySliderImageData,
                                                        imageData.rotarySliderImageSize))
{
}

ChristmasLookAndFeel::~ChristmasLookAndFeel()
{
}
    
void ChristmasLookAndFeel::drawRotarySlider (Graphics &g,
                                             int x, int y, int width, int height,
                                             float sliderPosProportional,
                                             float rotaryStartAngle,
                                             float rotaryEndAngle,
                                             Slider &slider)
{
    float angleRange = rotaryEndAngle - rotaryStartAngle;
    float angle = sliderPosProportional * angleRange + rotaryStartAngle;
    
    float centreX = width / 2.0f;
    float centreY = height / 2.0f;
    
    AffineTransform transform = AffineTransform::scale (width / rotarySliderWidth, 
                                                        height / rotarySliderHeight);
    transform = transform.rotated (angle, centreX, centreY);
        
    rotarySliderImage->draw (g, 1.0f, transform);
}