/*
  ==============================================================================

    SynthVoice.h
    Created: 10 Aug 2021 1:29:04pm
    Author:  garfi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"

#include "Data/OscData.h"
#include "Data/AdsrData.h"
#include "Data/FilterData.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    bool 	canPlaySound(juce::SynthesiserSound* sound) override;
    void 	startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void 	stopNote(float velocity, bool allowTailOff) override;
    void 	controllerMoved(int controllerNumber, int newControllerValue) override;
    void 	pitchWheelMoved(int newPitchWheelValue) override;
    void    prepareToPlay(double samplerRate, int samplesPerBlock, int outputChannels);
    void 	renderNextBlock(juce::AudioBuffer< float >& outputBuffer, int startSample, int numSamples) override;    
    void    reset();

    std::array<OscData, 2>& getOscillator() { return osc; }
    AdsrData& getAdsr() { return adsr; }
    AdsrData& getFilterAdsr() { return filterAdsr; }
    float getFilterAdsrOutput() {return filterAdsrOutput;}

    void    updateModParams(const int filterType, const float filterCutoff, const float resonance, const float adsrDepth);

    juce::AudioBuffer<float>& getWaveForm() { return synthBuffer; }

private:
    static constexpr int numChannelsToProcess{ 2 };
    std::array<OscData, numChannelsToProcess> osc;
    std::array<FilterData, numChannelsToProcess> filter;
    
    AdsrData adsr;
    AdsrData filterAdsr;

    juce::AudioBuffer<float> synthBuffer;
    float filterAdsrOutput{ 0.0f };

    juce::dsp::Gain<float> gain;
    bool isPrepared{ false };
};