/*
  ==============================================================================

    AdsrData.h
    Created: 15 Aug 2021 3:04:56pm
    Author:  garfi

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class AdsrData : public juce::ADSR
{
public:
    void update(const float attack, const float decay, const float sustain, const float release);
private:
    juce::ADSR::Parameters adsrParams;
};