/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ThumbnailComponent.h"

//==============================================================================
FirstSynthAudioProcessor::FirstSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParams()),
                          thumbnailOsc1("OSC 1", 0), 
                          thumbnailOsc2("OSC 2", 10),
                          thumbnailOsc3("OSC 3", 10), 
                          thumbnailOsc4("OSC 4", 10)
#endif
{
    synth.addSound(new SynthSound());
    for (int i = 0; i < 20; i++)
    {
        synth.addVoice(new SynthVoice());
    }
}

FirstSynthAudioProcessor::~FirstSynthAudioProcessor()
{
}

//==============================================================================
const juce::String FirstSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FirstSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FirstSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FirstSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FirstSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FirstSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FirstSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FirstSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FirstSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void FirstSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FirstSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) //returns Syntheseiser Voice, neeeds to be rearranged
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
    //filter.prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();

}

void FirstSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FirstSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void FirstSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    setParams();
    //setWaveform();

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

}

//==============================================================================
bool FirstSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FirstSynthAudioProcessor::createEditor()
{
    return new FirstSynthAudioProcessorEditor (*this);
}

//==============================================================================
void FirstSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FirstSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FirstSynthAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout FirstSynthAudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    //OSC
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC1WAVETYPE", "Osc 1 Wave Type", 
        juce::StringArray{ "sin(x)", "Saw Wave (x/Pi)", "Square Wave (if x<0 then -1, else x)", 
                           "cos(x - sin(x))", "cos(x - sin(5x))", 
                           "sin(7x + cos(3x))", "sin(3x)-cos(x/3)", 
                            "sin(3x) - cos(x/9)", "sin(x-cos(3x)) - cos(x-sin(3x))", "sin(x) + 2cos(x / 2)"}, 6));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC2WAVETYPE", "Osc 2 Wave Type", 
        juce::StringArray{ "sin(x)", "Saw Wave (x/Pi)", "Square Wave (if x<0 then -1, else x)",
                           "cos(x - sin(x))", "cos(x - sin(5x))",
                           "sin(7x + cos(3x))", "sin(3x)-cos(x/3)",
                            "sin(3x) - cos(x/9)", "sin(x-cos(3x)) - cos(x-sin(3x))", "sin(x) + 2cos(x / 2)", "N/A" }, 10));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC3WAVETYPE", "Osc 3 Wave Type", 
        juce::StringArray{ "sin(x)", "Saw Wave (x/Pi)", "Square Wave (if x<0 then -1, else x)",
                           "cos(x - sin(x))", "cos(x - sin(5x))",
                           "sin(7x + cos(3x))", "sin(3x)-cos(x/3)",
                            "sin(3x) - cos(x/9)", "sin(x-cos(3x)) - cos(x-sin(3x))", "sin(x) + 2cos(x / 2)", "N/A" }, 10));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC4WAVETYPE", "Osc 4 Wave Type", 
        juce::StringArray{ "sin(x)", "Saw Wave (x/Pi)", "Square Wave (if x<0 then -1, else x)",
                           "cos(x - sin(x))", "cos(x - sin(5x))",
                           "sin(7x + cos(3x))", "sin(3x)-cos(x/3)",
                            "sin(3x) - cos(x/9)", "sin(x-cos(3x)) - cos(x-sin(3x))", "sin(x) + 2cos(x / 2)", "N/A" }, 10));

    //FM
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FMFREQ", "FM Frequency", juce::NormalisableRange<float>{0.0f, 1000.0f, 0.1f}, 0.0f)); //oscing at frq 5 herz
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FMDEPTH", "FM Depth", juce::NormalisableRange<float>{0.0f, 100.0f, 0.1f}, 0.0f)); //taking main freq and moving up/down at 500 herz

    //Gain
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC1GAIN", "Oscillator 1 Gain", juce::NormalisableRange<float> { -40.0f, 0.2f, 0.001f }, 0.1f, "dB"));

    //OSC Pitch 
    params.push_back(std::make_unique<juce::AudioParameterInt>("OSC1PITCH", "Oscillator 1 Pitch", -48, 48, 0));

    //ADSR
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float>{0.1f, 1.0f, 0.1f}, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float>{0.1f, 1.0f, 0.1f}, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float>{0.1f, 1.0f, 0.1f}, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange<float>{0.1f, 3.0f, 0.1f}, 0.4f));

    //Filter ADSR
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTERADSRDEPTH", "Filter ADSR Depth", juce::NormalisableRange<float> { 0.0f, 10000.0f, 0.1f, 0.3f }, 10000.0f, ""));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MODATTACK", "Mod Attack", juce::NormalisableRange<float>{0.1f, 1.0f, 0.1f}, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MODDECAY", "Mod Decay", juce::NormalisableRange<float>{0.1f, 1.0f, 0.1f}, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MODSUSTAIN", "Mod Sustain", juce::NormalisableRange<float>{0.1f, 1.0f, 0.1f}, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MODRELEASE", "Mod Release", juce::NormalisableRange<float>{0.1f, 3.0f, 0.1f}, 0.4f));

    //Filter
    params.push_back(std::make_unique<juce::AudioParameterChoice>("FILTERTYPE", "Filter Type", juce::StringArray{ "Low-Pass", "Band-Pass", "High-Pass" }, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTERFREQ", "Filter Freq", juce::NormalisableRange<float>{20.1f, 20'000.0f, 0.1f, 0.6f}, 200.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTERRES", "Filter Resonance", juce::NormalisableRange<float>{1.0f, 2.0f, 0.1f}, 1.0f));

    return { params.begin(), params.end() };
}

void FirstSynthAudioProcessor::setParams()
{
    setVoiceParams();
    setFilterParams();
}

void FirstSynthAudioProcessor::setVoiceParams()
{
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            // Osc control
            // ADSR
            // LFO
            auto& attack = *apvts.getRawParameterValue("ATTACK");
            auto& decay = *apvts.getRawParameterValue("DECAY");
            auto& sustain = *apvts.getRawParameterValue("SUSTAIN");
            auto& release = *apvts.getRawParameterValue("RELEASE");
            auto& gain = *apvts.getRawParameterValue("OSC1GAIN");
            auto& pitch = *apvts.getRawParameterValue("OSC1PITCH");

            auto& oscWaveChoice = *apvts.getRawParameterValue("OSC1WAVETYPE");
            auto& osc2WaveChoice = *apvts.getRawParameterValue("OSC2WAVETYPE");
            auto& osc3WaveChoice = *apvts.getRawParameterValue("OSC3WAVETYPE");
            auto& osc4WaveChoice = *apvts.getRawParameterValue("OSC4WAVETYPE");

            auto& fmDepth = *apvts.getRawParameterValue("FMDEPTH");
            auto& fmFreq = *apvts.getRawParameterValue("FMFREQ");

            auto& modattack = *apvts.getRawParameterValue("MODATTACK");
            auto& moddecay = *apvts.getRawParameterValue("MODDECAY");
            auto& modsustain = *apvts.getRawParameterValue("MODSUSTAIN");
            auto& modrelease = *apvts.getRawParameterValue("MODRELEASE");

            auto& osc1 = voice->getOscillator();

            auto& adsr = voice->getAdsr();
            auto& filterAdsr = voice->getFilterAdsr();

            for (int i = 0; i < getTotalNumOutputChannels(); i++)
            {
                osc1[i].setParams(oscWaveChoice, gain, pitch, fmFreq, fmDepth);
                osc1[i].setOtherOscs(osc2WaveChoice, osc3WaveChoice, osc4WaveChoice);
            }

            adsr.update(attack.load(), decay.load(), sustain.load(), release.load());
            filterAdsr.update(modattack, moddecay, modsustain, modrelease);

            thumbnailOsc1.change_graphic_choice(oscWaveChoice);
            thumbnailOsc2.change_graphic_choice(osc2WaveChoice);
            thumbnailOsc3.change_graphic_choice(osc3WaveChoice);
            thumbnailOsc4.change_graphic_choice(osc4WaveChoice);
        }
    }
}

void FirstSynthAudioProcessor::setWaveform()
{
    auto& oscWaveChoice = *apvts.getRawParameterValue("OSC1WAVETYPE");
    auto& osc2WaveChoice = *apvts.getRawParameterValue("OSC2WAVETYPE");
    auto& osc3WaveChoice = *apvts.getRawParameterValue("OSC3WAVETYPE");
    auto& osc4WaveChoice = *apvts.getRawParameterValue("OSC4WAVETYPE");
    thumbnailOsc1.change_graphic_choice(oscWaveChoice);
    thumbnailOsc2.change_graphic_choice(osc2WaveChoice);
    thumbnailOsc3.change_graphic_choice(osc3WaveChoice);
    thumbnailOsc4.change_graphic_choice(osc4WaveChoice);
}


void FirstSynthAudioProcessor::setFilterParams()
{
    auto& filterType = *apvts.getRawParameterValue("FILTERTYPE");
    auto& cutoff = *apvts.getRawParameterValue("FILTERFREQ");
    auto& resonance = *apvts.getRawParameterValue("FILTERRES");
    auto& adsrDepth = *apvts.getRawParameterValue("FILTERADSRDEPTH");

    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->updateModParams(filterType, cutoff, resonance, adsrDepth);
        }
    }
}

juce::AudioBuffer<float>& FirstSynthAudioProcessor::getSynthBuffer()
{
    auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(0));
    return voice->getWaveForm();
}