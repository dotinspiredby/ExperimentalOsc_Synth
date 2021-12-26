/*
  ==============================================================================

    AdsrData.cpp
    Created: 15 Aug 2021 3:04:56pm
    Author:  garfi

  ==============================================================================
*/

#include "AdsrData.h"

void AdsrData::update(const float attack, const float decay, const float sustain, const float release)
{

    adsrParams.attack = attack;
    adsrParams.decay = decay;
    adsrParams.sustain = sustain;
    adsrParams.release = release;

    setParameters(adsrParams);
}