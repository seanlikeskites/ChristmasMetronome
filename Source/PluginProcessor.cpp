#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Synthesiser.h"

ChristmasMetronomeAudioProcessor::ChristmasMetronomeAudioProcessor()
    : AudioProcessor (BusesProperties().withOutput ("Output", AudioChannelSet::stereo(), true)),
      fs (44100.0f),
      voiceNotes {SleighBellVoice::DownBeatNote, SleighBellVoice::OtherBeatNote},
      defaultGains {0.0f, 0.0f},
      defaultLpfFrequencies {20000.0f, 10000.0f},
      defaultHpfFrequencies {5000.0f, 3000.0f},
      gainIds {"dbgain", "obgain"},
      lpfIds {"dblpf", "oblpf"},
      hpfIds {"dbhpf", "obhpf"},
      gainNames {"Down Beat Gain", "Other Beat Gain"},
      lpfNames {"Down Beat LPF Frequency", "Other Beat LPF Frequency"},
      hpfNames {"Down Beat HPF Frequency", "Other Beat HPF Frequency"}
{  
    for (int i = 0; i < numVoices; ++i)
    {
        sleighBellSynth.addVoice (new SleighBellVoice (voiceNotes [i]));
        
        auto gainCallback = [this, i] (float gain) {setVoiceGain (i, Decibels::decibelsToGain (gain));};
        addParameter (gainParams [i] = new ParameterWithCallback (gainIds [i], gainNames [i],
                                                                  -20.0f, 3.0f,
                                                                  defaultGains [i], 1.0f,
                                                                  gainCallback));
                                                         
        auto lpfCallback = [this, i] (float frequency) {setVoiceLpfFrequency (i, frequency);};
        addParameter (lpfParams [i] = new ParameterWithCallback (lpfIds [i], lpfNames [i],
                                                                 3000.0f, 20000.0f,
                                                                 defaultLpfFrequencies [i], 1.0f / 3.0f,
                                                                 lpfCallback));
                                                        
        auto hpfCallback = [this, i] (float frequency) {setVoiceHpfFrequency (i, frequency);};
        addParameter (hpfParams [i] = new ParameterWithCallback (hpfIds [i], hpfNames [i],
                                                                 3000.0f, 10000.0f,
                                                                 defaultHpfFrequencies [i], 1.0f / 3.0f,
                                                                 hpfCallback));
    }
       
    sleighBellSynth.addSound (new DownBeatSound); 
    sleighBellSynth.addSound (new OtherBeatSound);                                                    
}

ChristmasMetronomeAudioProcessor::~ChristmasMetronomeAudioProcessor()
{
}

const String ChristmasMetronomeAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ChristmasMetronomeAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ChristmasMetronomeAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double ChristmasMetronomeAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ChristmasMetronomeAudioProcessor::getNumPrograms()
{
    return 1;           
}

int ChristmasMetronomeAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ChristmasMetronomeAudioProcessor::setCurrentProgram (int index)
{
}

const String ChristmasMetronomeAudioProcessor::getProgramName (int index)
{
    return String();
}

void ChristmasMetronomeAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void ChristmasMetronomeAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    fs = sampleRate;
    
    sleighBellSynth.setCurrentPlaybackSampleRate (sampleRate);
    prepareVoices (sampleRate, samplesPerBlock);
}

void ChristmasMetronomeAudioProcessor::releaseResources()
{
}

bool ChristmasMetronomeAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const AudioChannelSet& mainOutput = layouts.getMainOutputChannelSet();

    // only allow stereo and mono
    if (mainOutput.size() > 2) 
        return false;
        
    return true;
}

void ChristmasMetronomeAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{   
    AudioPlayHead::CurrentPositionInfo playHead;
    getPlayHead()->getCurrentPosition (playHead);
    
    if (! playHead.isPlaying)
        return;
        
    midiMessages.clear();
    
    double crotchetsPerMinute = playHead.bpm;
    double samplesPerCrotchet = fs * 60.0 / crotchetsPerMinute;
    double crotchetsPerBeat = 4.0 / playHead.timeSigDenominator;
    double samplesPerBeat = samplesPerCrotchet * crotchetsPerBeat;
    
    int timeSig = playHead.timeSigNumerator;
    
    double beatInBar = (playHead.ppqPosition - playHead.ppqPositionOfLastBarStart) / crotchetsPerBeat;
    int beatInteger = floor (beatInBar);
    double beatFraction = beatInBar - beatInteger;
    double beatSample = - samplesPerBeat * beatFraction;
    
    while (beatSample < 0)
    {
        beatSample += samplesPerBeat;
        ++beatInteger;
    }
    
    const int numSamples = buffer.getNumSamples();
    
    for (; beatSample < numSamples; beatSample += samplesPerBeat, ++beatInteger)
    {
        if (beatInteger % timeSig == 0)
            midiMessages.addEvent (MidiMessage::noteOn (1, SleighBellVoice::DownBeatNote, (uint8) 127),
                                   round (beatSample));
        else
            midiMessages.addEvent (MidiMessage::noteOn (1, SleighBellVoice::OtherBeatNote, (uint8) 127),
                                   round (beatSample));
    }
    
    sleighBellSynth.renderNextBlock (buffer, midiMessages, 0, numSamples);
    
    midiMessages.clear();
}

//==============================================================================
bool ChristmasMetronomeAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* ChristmasMetronomeAudioProcessor::createEditor()
{
    return new ChristmasMetronomeAudioProcessorEditor (*this);
}

//==============================================================================
void ChristmasMetronomeAudioProcessor::getStateInformation (MemoryBlock& destData)
{
}

void ChristmasMetronomeAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

void ChristmasMetronomeAudioProcessor::setVoiceGain (int voice, float gain)
{
    SleighBellVoice *v = static_cast <SleighBellVoice*> (sleighBellSynth.getVoice (voice));
    v->setGain (gain);
}

void ChristmasMetronomeAudioProcessor::setVoiceLpfFrequency (int voice, float frequency)
{
    SleighBellVoice *v = static_cast <SleighBellVoice*> (sleighBellSynth.getVoice (voice));
    v->setLpfFrequency (frequency);
}

void ChristmasMetronomeAudioProcessor::setVoiceHpfFrequency (int voice, float frequency)
{
    SleighBellVoice *v = static_cast <SleighBellVoice*> (sleighBellSynth.getVoice (0));
    v->setHpfFrequency (frequency);
}

void ChristmasMetronomeAudioProcessor::prepareVoices (double sampleRate, int samplesPerBlock)
{       
    for (int i = 0; i < numVoices; ++i)
    {
        SleighBellVoice *voice = static_cast <SleighBellVoice*> (sleighBellSynth.getVoice (i));
        voice->prepareToPlay (samplesPerBlock, sampleRate);
        voice->setLpfFrequency (*lpfParams [i]);
        voice->setHpfFrequency (*hpfParams [i]);
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ChristmasMetronomeAudioProcessor();
}