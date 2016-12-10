#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Synthesiser.h"

ChristmasMetronomeAudioProcessor::ChristmasMetronomeAudioProcessor()
    : AudioProcessor (BusesProperties().withOutput ("Output", AudioChannelSet::stereo(), true)),
      fs (44100.0f)
{  
    sleighBellSynth.addVoice (new SleighBellVoice (48));
    sleighBellSynth.addSound (new DownBeatSound); 
    sleighBellSynth.addVoice (new SleighBellVoice (50));
    sleighBellSynth.addSound (new OtherBeatSound);
    
    auto downBeatGainCallback = [this] (float gain) {setDownBeatGain (Decibels::decibelsToGain (gain));};
    addParameter (downBeatGainParam = new ParameterWithCallback ("dbgain", "Down Beat Gain",
                                                                 -80.0f, 3.0f,
                                                                 0.0f, 1.0f,
                                                                 downBeatGainCallback));
                                                         
    auto downBeatLpfCallback = [this] (float frequency) {setDownBeatLpfFrequency (frequency);};
    addParameter (downBeatLpfParam = new ParameterWithCallback ("dblpf", "Down Beat LPF Frequency",
                                                                3000.0f, 20000.0f,
                                                                20000.0f, 1.0f / 3.0f,
                                                                downBeatLpfCallback));
                                                        
    auto downBeatHpfCallback = [this] (float frequency) {setDownBeatHpfFrequency (frequency);};
    addParameter (downBeatHpfParam = new ParameterWithCallback ("dbhpf", "Down Beat HPF Frequency",
                                                                3000.0f, 10000.0f,
                                                                5000.0f, 1.0f / 3.0f,
                                                                downBeatHpfCallback));
                                                                
    auto otherBeatGainCallback = [this] (float gain) {setOtherBeatGain (Decibels::decibelsToGain (gain));};
    addParameter (otherBeatGainParam = new ParameterWithCallback ("obgain", "Other Beat Gain",
                                                                  -80.0f, 3.0f,
                                                                  0.0f, 1.0f,
                                                                  otherBeatGainCallback));
                                                         
    auto otherBeatLpfCallback = [this] (float frequency) {setOtherBeatLpfFrequency (frequency);};
    addParameter (otherBeatLpfParam = new ParameterWithCallback ("oblpf", "Other Beat LPF Frequency",
                                                                 3000.0f, 20000.0f,
                                                                 10000.0f, 1.0f / 3.0f,
                                                                 otherBeatLpfCallback));
                                                        
    auto otherBeatHpfCallback = [this] (float frequency) {setOtherBeatHpfFrequency (frequency);};
    addParameter (otherBeatHpfParam = new ParameterWithCallback ("obhpf", "Other Beat HPF Frequency",
                                                                 3000.0f, 10000.0f,
                                                                 3000.0f, 1.0f / 3.0f,
                                                                 otherBeatHpfCallback));
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
    
    SleighBellVoice *voice = static_cast <SleighBellVoice*> (sleighBellSynth.getVoice (0));
    voice->prepareToPlay (samplesPerBlock, sampleRate);
    voice->setLpfFrequency (*downBeatLpfParam);
    voice->setHpfFrequency (*downBeatHpfParam);
    
    voice = static_cast <SleighBellVoice*> (sleighBellSynth.getVoice (1));
    voice->prepareToPlay (samplesPerBlock, sampleRate);
    voice->setLpfFrequency (*otherBeatLpfParam);
    voice->setHpfFrequency (*otherBeatHpfParam);
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
    
    double bpm = playHead.bpm;
    int64 time = playHead.timeInSamples;
    double samplesPerBeat = fs * 60 / bpm;
    int timeSig = playHead.timeSigNumerator;
    
    const int numSamples = buffer.getNumSamples();
    
    for (int i = 0; i < numSamples; ++i)
    {
        if (round (std::fmod (time, samplesPerBeat * timeSig)) == 0)
            midiMessages.addEvent (MidiMessage::noteOn (1, 48, (uint8) 127), i);
        else if (round (std::fmod (time, samplesPerBeat)) == 0)
            midiMessages.addEvent (MidiMessage::noteOn (1, 50, (uint8) 127), i);
            
        time++;
    }
    
    sleighBellSynth.renderNextBlock (buffer, midiMessages, 0, numSamples);
    
    midiMessages.clear();
}

//==============================================================================
bool ChristmasMetronomeAudioProcessor::hasEditor() const
{
    return false;
}

AudioProcessorEditor* ChristmasMetronomeAudioProcessor::createEditor()
{
    return nullptr; // new ChristmasMetronomeAudioProcessorEditor (*this);
}

//==============================================================================
void ChristmasMetronomeAudioProcessor::getStateInformation (MemoryBlock& destData)
{
}

void ChristmasMetronomeAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

void ChristmasMetronomeAudioProcessor::setDownBeatGain (float gain)
{
    SleighBellVoice *voice = static_cast <SleighBellVoice*> (sleighBellSynth.getVoice (0));
    voice->setGain (gain);
}

void ChristmasMetronomeAudioProcessor::setDownBeatLpfFrequency (float frequency)
{
    SleighBellVoice *voice = static_cast <SleighBellVoice*> (sleighBellSynth.getVoice (0));
    voice->setLpfFrequency (frequency);
}

void ChristmasMetronomeAudioProcessor::setDownBeatHpfFrequency (float frequency)
{
    SleighBellVoice *voice = static_cast <SleighBellVoice*> (sleighBellSynth.getVoice (0));
    voice->setHpfFrequency (frequency);
}
    
void ChristmasMetronomeAudioProcessor::setOtherBeatGain (float gain)
{
    SleighBellVoice *voice = static_cast <SleighBellVoice*> (sleighBellSynth.getVoice (1));
    voice->setGain (gain);
}

void ChristmasMetronomeAudioProcessor::setOtherBeatLpfFrequency (float frequency)
{
    SleighBellVoice *voice = static_cast <SleighBellVoice*> (sleighBellSynth.getVoice (1));
    voice->setLpfFrequency (frequency);
}

void ChristmasMetronomeAudioProcessor::setOtherBeatHpfFrequency (float frequency)
{
    SleighBellVoice *voice = static_cast <SleighBellVoice*> (sleighBellSynth.getVoice (1));
    voice->setHpfFrequency (frequency);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ChristmasMetronomeAudioProcessor();
}