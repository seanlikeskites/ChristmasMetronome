#ifndef SYNTHESISER_H_INCLUDED
#define SYNTHESISER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class DownBeatSound : public SynthesiserSound
{
public:
    DownBeatSound();
    ~DownBeatSound();

    bool appliesToNote (int midiNoteNumber) override;
    bool appliesToChannel (int midiChannel) override;
};

class OtherBeatSound : public SynthesiserSound
{
public:
    OtherBeatSound();
    ~OtherBeatSound();

    bool appliesToNote (int midiNoteNumber) override;
    bool appliesToChannel (int midiChannel) override;
};

class SleighBellVoice : public SynthesiserVoice
{
public:
    SleighBellVoice (int midiNoteInit);
    ~SleighBellVoice();
    
    bool canPlaySound (SynthesiserSound *sound) override;
    void startNote (int midiNoteNumber, float velocity, 
                    SynthesiserSound *sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void renderNextBlock (AudioSampleBuffer &outputBuffer, int startSample, int numSamples) override;
    
    void prepareToPlay (int samplesPerBlock, double sampleRate);
    void releaseResources();
    
    void setGain (float newGain);
    void setLpfFrequency (float newFrequency);
    void setHpfFrequency (float newFrequency);
    
    enum NoteNumbers
    {
        DownBeatNote = 48,
        OtherBeatNote = 50
    };
    
private:
    static const int numChannels = 2;
    int midiNote;
    float gain, gainValue;
    double fs;
    IIRFilter lpf [numChannels], hpf [numChannels];
    float lpfFrequency, hpfFrequency;
    ScopedPointer <AudioFormatReaderSource> readerSource;
    ScopedPointer <ResamplingAudioSource> audioSource;
    AudioSampleBuffer currentBuffer;
    
    void updateLpf();
    void updateHpf();
    void resetFilters();
    
    struct AudioLoader
    {
        AudioLoader();
        ~AudioLoader();
        
        AudioFormatManager formatManager;
        ScopedPointer <AudioFormatReader> reader;
    };
    
    SharedResourcePointer <AudioLoader> sleighBellReader;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SleighBellVoice)
};

#endif  // SYNTHESISER_H_INCLUDED