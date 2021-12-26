/*
  ==============================================================================

    SynthVoice.cpp
    Created: 10 Aug 2021 1:29:04pm
    Author:  garfi

  ==============================================================================
*/

#include "SynthVoice.h"

bool 	SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void 	SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) 
{
    
    for (int i = 0; i < 2; i++)
    {
        osc[i].setFreq(midiNoteNumber);
    }

    adsr.noteOn();
    filterAdsr.noteOn();
}

void 	SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    adsr.noteOff();
    filterAdsr.noteOff();
    if (!allowTailOff || !adsr.isActive()) { clearCurrentNote(); }
}

void 	SynthVoice::controllerMoved(int controllerNumber, int newControllerValue){}
void 	SynthVoice::pitchWheelMoved(int newPitchWheelValue){}


void    SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    reset();

    adsr.setSampleRate(sampleRate);
    filterAdsr.setSampleRate(sampleRate);
    
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;

    for (int ch = 0; ch < numChannelsToProcess; ch++)
    {
        osc[ch].prepareToPlay( sampleRate, samplesPerBlock, outputChannels );
        filter[ch].prepareToPlay(sampleRate, samplesPerBlock, outputChannels);

    }
    
    gain.prepare(spec);
    gain.setGainLinear(0.07f); //0-1

    isPrepared = true;
}


void 	SynthVoice::renderNextBlock(juce::AudioBuffer< float >& outputBuffer, int startSample, int numSamples)
{
    jassert(isPrepared);

    if (! isVoiceActive()) { return;}

    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);

    filterAdsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());
    filterAdsrOutput = filterAdsr.getNextSample();

    synthBuffer.clear();

    for (int ch = 0; ch < synthBuffer.getNumChannels(); ++ch)
    {
        auto* buffer = synthBuffer.getWritePointer(ch, 0);

        for (int s = 0; s < synthBuffer.getNumSamples(); ++s)
        {
            buffer[s] = osc[ch].processNextSample(buffer[s]);
        }
    }

    juce::dsp::AudioBlock<float> audioBlock{ synthBuffer }; //aliasing
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());
  
    for (int ch = 0; ch < synthBuffer.getNumChannels(); ++ch)
    {
        auto* buffer = synthBuffer.getWritePointer(ch, 0);
        for (int s = 0; s < synthBuffer.getNumSamples(); ++s)
        {
            buffer[s] = filter[ch].processNextSample(ch, synthBuffer.getSample(ch, s));
        }
    }


    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);
        if (!adsr.isActive()) { clearCurrentNote(); }
    }

}

void SynthVoice::reset()
{
    gain.reset();
    adsr.reset();
    filterAdsr.reset();
}
 
void SynthVoice::updateModParams(const int filterType, const float filterCutoff, const float resonance, const float adsrDepth)
{
    auto cutoff = (adsrDepth * filterAdsrOutput) + filterCutoff;
    cutoff = std::clamp<float>(cutoff, 20.0f, 20'000.0f);

    for (int ch = 0; ch < numChannelsToProcess; ++ch)
    {
        filter[ch].setParams(filterType, cutoff, resonance);
    }
}
