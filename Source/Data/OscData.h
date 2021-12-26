/*
  ==============================================================================

    OscData.h
    Created: 16 Aug 2021 11:30:42pm
    Author:  garfi

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class OscData : public juce::dsp::Oscillator <float>
{
public:
    void    prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);               //

    void    setWaveType(const int choice);                             //
    void    setAdditionalWaveType(juce::dsp::Oscillator<float>& osc, const int choice);
    void    setGain(const float levelInDeibels);                       //
    void    setOscPitch(const int pitch);                                 //
    void    setFreq(const int midiNoteNumber);                         //
    void    setFMOsc(const float freq, const float depth);             //
    void    setOtherOscs(const int osc2Choice, const int osc3Choice, const int osc4Choice);
    void    renderNextBlock(juce::dsp::AudioBlock<float>& audioBlock); //


    float   processNextSample(float input);                            //
    void    setParams(const int oscChoice, const float oscGain, const int oscPitch, const float fmFreq, const float fmDepth);
    void    resetAll();                                                //

private:
    
    juce::dsp::Oscillator <float> osc2;
    juce::dsp::Oscillator <float> osc3;
    juce::dsp::Oscillator <float> osc4;

    juce::dsp::Oscillator<float> fmOsc{ [](float x) {return std::sin(x); } };

    juce::dsp::Gain<float> gain;
    int     lastPitch{ 0 };
    int     lastMidiNote{ 0 };
    float   fmDepth{ 0.0f };
    float   fmMod{ 0.0f }; //-1--1
    
    
};
