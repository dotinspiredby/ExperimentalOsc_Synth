/*
  ==============================================================================

    FilterData.h
    Created: 20 Aug 2021 3:38:56pm
    Author:  garfi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class FilterData : public juce::dsp::StateVariableTPTFilter<float>
{
public:
    FilterData();
    void    prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels);
    void    setParams(const int filterType, const float filterCutoff, const float filterResonance);
    void    processNextBlock(juce::AudioBuffer<float>& buffer);
    float   processNextSample(int channel, float inputValue);
    void    resetAll();


private:
    void selectFilterType(const int type);
    juce::dsp::StateVariableTPTFilter<float> filter;
    //bool isPrepared{ false };
};