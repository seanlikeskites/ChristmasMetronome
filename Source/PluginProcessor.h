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

    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override;

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

    static const int numVoices = 2;
    ParameterWithCallback *gainParams [numVoices], *lpfParams [numVoices], *hpfParams [numVoices];
    
private:
    double fs;
    Synthesiser sleighBellSynth;
    
    int voiceNotes [numVoices];
    float defaultGains [numVoices], defaultLpfFrequencies [numVoices], defaultHpfFrequencies [numVoices];
    String gainIds [numVoices], lpfIds [numVoices], hpfIds [numVoices];
    String gainNames [numVoices], lpfNames [numVoices], hpfNames [numVoices];
    
    void setVoiceGain (int voice, float gain);
    void setVoiceLpfFrequency (int voice, float frequency);
    void setVoiceHpfFrequency (int voice, float frequency);
    void prepareVoices (double sampleRate, int samplesPerBlock);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChristmasMetronomeAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED