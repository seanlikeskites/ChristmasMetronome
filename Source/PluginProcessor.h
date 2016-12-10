#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterWithCallback.h"

class ChristmasMetronomeAudioProcessor  : public AudioProcessor
{
public:
    ChristmasMetronomeAudioProcessor();
    ~ChristmasMetronomeAudioProcessor();

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    double fs;
    Synthesiser sleighBellSynth;
    
    ParameterWithCallback *downBeatGainParam, *downBeatLpfParam, *downBeatHpfParam;
    ParameterWithCallback *otherBeatGainParam, *otherBeatLpfParam, *otherBeatHpfParam;
    
    void setDownBeatGain (float gain);
    void setDownBeatLpfFrequency (float frequency);
    void setDownBeatHpfFrequency (float frequency);
    
    void setOtherBeatGain (float gain);
    void setOtherBeatLpfFrequency (float frequency);
    void setOtherBeatHpfFrequency (float frequency);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChristmasMetronomeAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED