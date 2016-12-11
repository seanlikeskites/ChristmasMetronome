#include "PluginProcessor.h"
#include "PluginEditor.h"

ChristmasMetronomeAudioProcessorEditor::ChristmasMetronomeAudioProcessorEditor (ChristmasMetronomeAudioProcessor& owner)
    : AudioProcessorEditor (&owner), 
      processor (owner),
      background (Drawable::createFromImageData (BinaryData::BellsBackground_svg,
                                                 BinaryData::BellsBackground_svgSize)),
      width (630), height (333),
      gainLook ({BinaryData::AmplitudeDial_svg,
                 BinaryData::AmplitudeDial_svgSize,
                 80.0f, 80.0f}),
      lpfLook ({BinaryData::LPFDial_svg,
                BinaryData::LPFDial_svgSize,
                80.0f, 80.0f}),
      hpfLook ({BinaryData::HPFDial_svg,
                BinaryData::HPFDial_svgSize,
                80.0f, 80.0f})
{
    setSize (width, height);
    
    int midPoint = width / 2;
    int sliderWidth = 110;
    int sliderHeight = 110;
    
    int titleBottom = 70;
    int topRowY = titleBottom + (height - titleBottom) / 4 - sliderWidth / 2; 
    int bottomRowY = titleBottom + 3 * (height - titleBottom) / 4 - sliderWidth / 2; ;
    
    int gainX = (midPoint - sliderWidth) / 2;
    int lpfX = midPoint / 4 - sliderWidth / 2;
    int hpfX = 3 * midPoint / 4 - sliderWidth / 2;

    for (int i = 0; i < ChristmasMetronomeAudioProcessor::numVoices; ++i)
    {
        int offset = midPoint * i;
        
        gainSliders [i] = new ParameterSlider (*owner.gainParams [i], &gainLook);
        addAndMakeVisible (gainSliders [i]);
        gainSliders [i]->setBounds (offset + gainX, topRowY, sliderWidth, sliderHeight);
        
        lpfSliders [i] = new ParameterSlider (*owner.lpfParams [i], &lpfLook);
        addAndMakeVisible (lpfSliders [i]);
        lpfSliders [i]->setBounds (offset + lpfX, bottomRowY, sliderWidth, sliderHeight);
        
        hpfSliders [i] = new ParameterSlider (*owner.hpfParams [i], &hpfLook);
        addAndMakeVisible (hpfSliders [i]);
        hpfSliders [i]->setBounds (offset + hpfX, bottomRowY, sliderWidth, sliderHeight);
    }
}

ChristmasMetronomeAudioProcessorEditor::~ChristmasMetronomeAudioProcessorEditor()
{ 
}

//==============================================================================
void ChristmasMetronomeAudioProcessorEditor::paint (Graphics& g)
{
     background->drawWithin (g, getBounds().toFloat(),
                            RectanglePlacement::stretchToFit, 1.0f);
}

void ChristmasMetronomeAudioProcessorEditor::resized()
{
}