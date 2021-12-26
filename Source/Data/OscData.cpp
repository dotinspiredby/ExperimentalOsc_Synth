/*
  ==============================================================================

    OscData.cpp
    Created: 16 Aug 2021 11:30:42pm
    Author:  garfi

  ==============================================================================
*/

#include "OscData.h"

void OscData::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    resetAll();
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;

    prepare(spec);
 

    osc2.prepare(spec);
    osc3.prepare(spec);
    osc4.prepare(spec);
    fmOsc.prepare(spec);
    gain.prepare(spec);
}

void OscData::setWaveType(const int choice)
{
    //juce::dsp::Oscillator<float> osc{ [](float x) {return std::sin(x); } };
    //juce::dsp::Oscillator<float> osc{ [](float x) {return x / juce::MathConstants<float>::pi; } };
    //juce::dsp::Oscillator<float> osc{ [](float x) {return x < 0.0f ? -1.0f : 1.0f; } };
    //juce::dsp::Oscillator<float> osc{ [](float x) {return std::sin(x / juce::MathConstants<float>::pi) + std::cos(x / juce::MathConstants<float>::pi); } };

    switch (choice) {
        case 0:
            //sin(x)
            initialise([](float x) {return std::sin(x); });
            break;
        case 1:
            //Saw Wav (x/Pi)
            initialise([](float x) {return x / juce::MathConstants<float>::pi; });
            break;
        case 2:
            //Square Wave (if x<0 then -1, else x)
            initialise([](float x) {return x < 0.0f ? -1.0f : 1.0f; });
            break;
        case 3:
            //cos(x - sin(x))
            initialise([](float x) {return std::cos(x - std::sin(x)); });
            break;
        case 4:
            //cos(x - sin(5x))
            initialise([](float x) {return std::cos(x - std::sin(5 * x)); });
            break;
        case 5:
            //sin(7x + cos(3x))
            initialise([](float x) { return std::sin(7 * x + std::cos(3 * x)); });
            break;
        case 6:
            //sin(3x)-cos(x/3)
            initialise([](float x) {return std::sin(3 * x) - std::cos(x / 3); });
            break;
        case 7:
            //sin(3x) - cos(x/9)
            initialise([](float x) {return std::sin(3 * x) - std::cos(x / 9); });
            break;
        case 8:
            //sin(x-cos(3x)) - cos(x-sin(3x))
            initialise([](float x) {return std::sin(x - std::cos(3 * x)) - std::cos(sin(x - std::sin(3 * x))); });
            break;
        case 9:
            //sin(x) + 2cos(x / 2)
            initialise([](float x) {return std::sin(x) + 2 * std::cos(x / 2); });
            break;

        default:
            jassertfalse;
            break;
    }
}

void OscData::setAdditionalWaveType(juce::dsp::Oscillator<float>& osc_, const int choice)
{
    //juce::dsp::Oscillator<float> osc{ [](float x) {return std::sin(x); } };
    //juce::dsp::Oscillator<float> osc{ [](float x) {return x / juce::MathConstants<float>::pi; } };
    //juce::dsp::Oscillator<float> osc{ [](float x) {return x < 0.0f ? -1.0f : 1.0f; } };
    //juce::dsp::Oscillator<float> osc{ [](float x) {return std::sin(x / juce::MathConstants<float>::pi) + std::cos(x / juce::MathConstants<float>::pi); } };

    switch (choice) {
        case 0:
            osc_.initialise([](float x) {return std::sin(x); });
            break;
        case 1:
            osc_.initialise([](float x) {return x / juce::MathConstants<float>::pi; });
            break;
        case 2:
            osc_.initialise([](float x) {return x < 0.0f ? -1.0f : 1.0f; });
            break;
        case 3:
            osc_.initialise([](float x) {return std::cos(x - std::sin(x)); });
            break;
        case 4:
            osc_.initialise([](float x) {return std::cos(x - std::sin(5 * x)); });
            break;
        case 5:
            osc_.initialise([](float x) { return std::sin(7 * x + std::cos(3 * x)); });
            break;
        case 6:
            osc_.initialise([](float x) {return std::sin(3 * x) - std::cos(x / 3); });
            break;
        case 7:
            osc_.initialise([](float x) {return std::sin(3 * x) - std::cos(x / 9); });
            break;
        case 8:
            osc_.initialise([](float x) {return std::sin(x - std::cos(3 * x)) - std::cos(sin(x - std::sin(3 * x))); });
            break;
        case 9:
            osc_.initialise([](float x) {return std::sin(x) + 2 * std::cos(x / 2); });
            break;
        case 10:
            osc_.initialise([](float x) { return 0.0f; });
        default:
            osc_.initialise([](float x) { return 0.0f; });
            break;
    }
}

void OscData::setGain(const float levelInDecibels)
{
    gain.setGainDecibels(levelInDecibels);
}

void OscData::setOscPitch(const int pitch)
{
    lastPitch = pitch;
    setFrequency(juce::MidiMessage::getMidiNoteInHertz((lastMidiNote + lastPitch) + fmMod));
    osc2.setFrequency(juce::MidiMessage::getMidiNoteInHertz((lastMidiNote + lastPitch) + fmMod));
    osc3.setFrequency(juce::MidiMessage::getMidiNoteInHertz((lastMidiNote + lastPitch) + fmMod));
    osc4.setFrequency(juce::MidiMessage::getMidiNoteInHertz((lastMidiNote + lastPitch) + fmMod));
}

void OscData::setFreq(const int midiNoteNumber)
{
    setFrequency(juce::MidiMessage::getMidiNoteInHertz((midiNoteNumber + lastPitch) + fmMod));
    osc2.setFrequency(juce::MidiMessage::getMidiNoteInHertz((midiNoteNumber + lastPitch) + fmMod));
    osc3.setFrequency(juce::MidiMessage::getMidiNoteInHertz((midiNoteNumber + lastPitch) + fmMod));
    osc4.setFrequency(juce::MidiMessage::getMidiNoteInHertz((midiNoteNumber + lastPitch) + fmMod));
    lastMidiNote = midiNoteNumber;
}

void OscData::setFMOsc(const float freq, const float depth)
{
    fmDepth = depth;
    fmOsc.setFrequency(freq);
    
    auto currentFreq = juce::MidiMessage::getMidiNoteInHertz(lastMidiNote + lastPitch) + fmMod;
    setFrequency(currentFreq >= 0 ? currentFreq : currentFreq * -1.0f);
    osc2.setFrequency(currentFreq >= 0 ? currentFreq : currentFreq * -1.0f);
    osc3.setFrequency(currentFreq >= 0 ? currentFreq : currentFreq * -1.0f);
    osc4.setFrequency(currentFreq >= 0 ? currentFreq : currentFreq * -1.0f);
    //setFrequency (juce::MidiMessage::getMidiNoteInHertz ((lastMidiNote + lastPitch) + fmMod));
}

void OscData::renderNextBlock(juce::dsp::AudioBlock<float>& audioBlock)
{
    jassert(audioBlock.getNumSamples() > 0);
    process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    osc2.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    osc3.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    osc4.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
}

float OscData::processNextSample(float input)
{
    fmMod = fmOsc.processSample(input) * fmDepth;
    return gain.processSample(osc2.processSample(osc3.processSample(osc4.processSample(processSample(input)))));
}

void    OscData::setOtherOscs(const int osc2Choice, const int osc3Choice, const int osc4Choice)
{
    setAdditionalWaveType(osc2, osc2Choice);
    setAdditionalWaveType(osc3, osc3Choice);
    setAdditionalWaveType(osc4, osc4Choice);

}

void OscData::setParams(const int oscChoice, const float oscGain, const int oscPitch, const float fmFreq, const float fmDepth)
{

    setWaveType(oscChoice);
    setGain(oscGain);
    setOscPitch(oscPitch);
    setFMOsc(fmFreq, fmDepth);
}

void OscData::resetAll()
{
    reset();
    osc2.reset();
    osc3.reset();
    osc4.reset();
    fmOsc.reset();
    gain.reset();
}

