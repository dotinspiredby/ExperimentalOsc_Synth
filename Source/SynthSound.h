/*
  ==============================================================================

    SynthSound.h
    Created: 10 Aug 2021 1:29:27pm
    Author:  garfi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public: 
    /*Virtual functions, which need to be accomplished*/
    virtual bool    appliesToNote(int midiNoteNumber) override { return true; }
    virtual bool 	appliesToChannel(int midiChannel) override { return true; }

};