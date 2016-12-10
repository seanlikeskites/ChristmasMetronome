#include "Synthesiser.h"

DownBeatSound::DownBeatSound()
{
}

DownBeatSound::~DownBeatSound()
{
}

bool DownBeatSound::appliesToNote (int midiNoteNumber)
{        
    if (midiNoteNumber == 48)
        return true;
    else
        return false;
}

bool DownBeatSound::appliesToChannel (int /*midiChannel*/)
{
    return true;
}

OtherBeatSound::OtherBeatSound()
{
}

OtherBeatSound::~OtherBeatSound()
{
}

bool OtherBeatSound::appliesToNote (int midiNoteNumber)
{    
    if (midiNoteNumber == 50)
        return true;
    else
        return false;
}

bool OtherBeatSound::appliesToChannel (int /*midiChannel*/)
{
    return true;
}

SleighBellVoice::SleighBellVoice (int midiNoteInit)
    : midiNote (midiNoteInit),
      gain (0.0f), gainValue (0.0f),
      fs (44100.0),
      lpfFrequency (20000.0f), hpfFrequency (1.0f)
{
    readerSource = new AudioFormatReaderSource (sleighBellReader->reader, false);
    readerSource->setLooping (false);
    
    audioSource = new ResamplingAudioSource (readerSource, false, numChannels);
}

SleighBellVoice::~SleighBellVoice()
{
}
    
bool SleighBellVoice::canPlaySound (SynthesiserSound *sound)
{
    return sound->appliesToNote (midiNote);
}

void SleighBellVoice::startNote (int midiNoteNumber, float velocity, 
                                 SynthesiserSound *sound, int currentPitchWheelPosition)
{
    gain = gainValue;
    resetFilters();
    readerSource->setNextReadPosition (0);
}

void SleighBellVoice::stopNote (float velocity, bool allowTailOff)
{
    gain = 0.0f;
    clearCurrentNote();
}

void SleighBellVoice::pitchWheelMoved (int /*newPitchWheelValue*/)
{
}

void SleighBellVoice::controllerMoved (int /*controllerNumber*/, int /*newControllerValue*/)
{
}

void SleighBellVoice::renderNextBlock (AudioSampleBuffer &outputBuffer, int startSample, int numSamples)
{
    audioSource->getNextAudioBlock (AudioSourceChannelInfo (&currentBuffer, 0, numSamples));
    
    const int numOutputChannels = outputBuffer.getNumChannels();
    
    for (int i = 0; i < jmin (numChannels, numOutputChannels); ++i)
    {
        float *channelData = currentBuffer.getWritePointer (i, startSample);
        lpf [i].processSamples (channelData, numSamples);
        hpf [i].processSamples (channelData, numSamples);
        
        outputBuffer.addFrom (i, startSample, currentBuffer, i, 0, numSamples, gain);
    }
}

void SleighBellVoice::prepareToPlay (int samplesPerBlock, double sampleRate)
{
    fs = sampleRate;
    
    updateLpf();
    updateHpf();
    resetFilters();
    
    double sourceSampleRate = sleighBellReader->reader->sampleRate;
    readerSource->prepareToPlay (samplesPerBlock, sourceSampleRate);
    audioSource->setResamplingRatio (sourceSampleRate / sampleRate);
    audioSource->prepareToPlay (samplesPerBlock, sampleRate);
    currentBuffer.setSize (numChannels, samplesPerBlock * 4);
    
    clearCurrentNote();
}

void SleighBellVoice::releaseResources()
{
    audioSource->releaseResources();
    readerSource->releaseResources();
}

void SleighBellVoice::setGain (float newGain)
{
    gainValue = newGain;
}

void SleighBellVoice::setLpfFrequency (float newFrequency)
{
    lpfFrequency = newFrequency;
    updateLpf();
}

void SleighBellVoice::setHpfFrequency (float newFrequency)
{
    hpfFrequency = newFrequency;
    updateHpf();
}

void SleighBellVoice::updateLpf()
{
    for (int i = 0; i < numChannels; ++i)
        lpf [i].setCoefficients (IIRCoefficients::makeLowPass (fs, lpfFrequency));
}

void SleighBellVoice::updateHpf()
{
    for (int i = 0; i < numChannels; ++i)
        hpf [i].setCoefficients (IIRCoefficients::makeHighPass (fs, hpfFrequency));
}

void SleighBellVoice::resetFilters()
{
    for (int i = 0; i < numChannels; ++i)
    {
        lpf [i].reset();
        hpf [i].reset();
    }
}

SleighBellVoice::AudioLoader::AudioLoader()
{
    formatManager.registerFormat (new WavAudioFormat, true);
    reader = formatManager.createReaderFor (new MemoryInputStream (BinaryData::sleighbell_wav,
                                                                   BinaryData::sleighbell_wavSize,
                                                                   false));
}

SleighBellVoice::AudioLoader::~AudioLoader()
{
}