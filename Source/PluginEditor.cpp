/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
FirstSynthAudioProcessorEditor::FirstSynthAudioProcessorEditor (FirstSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), 
    audioProcessor (p), 
    osc     (audioProcessor.apvts, "OSC1WAVETYPE", "OSC2WAVETYPE","OSC3WAVETYPE","OSC4WAVETYPE",
                                   "OSC1GAIN", "OSC1PITCH", "FMFREQ", "FMDEPTH"),
    adsr    ("Amp Envelope", audioProcessor.apvts, "ATTACK", "DECAY", "SUSTAIN", "RELEASE"),
    filter  (audioProcessor.apvts, "FILTERTYPE", "FILTERFREQ", 
                                   "FILTERADSRDEPTH", 
                                   "FILTERRES"),
    modAdsr ("Mod Envelope", audioProcessor.apvts, "MODATTACK", "MODDECAY", "MODSUSTAIN", "MODRELEASE")
    //thumbnail1 (audioProcessor.thumbnailOsc1), 
    //thumbnail2 (audioProcessor.thumbnailOsc2),
    //thumbnail3 (audioProcessor.thumbnailOsc3),
    //thumbnail4 (audioProcessor.thumbnailOsc4)
{
    
    setSize (400, 700); //400x300

    addAndMakeVisible(osc);
    addAndMakeVisible(adsr);
    addAndMakeVisible(filter);
    addAndMakeVisible(modAdsr);
    addAndMakeVisible(audioProcessor.thumbnailOsc1);
    addAndMakeVisible(audioProcessor.thumbnailOsc2);
    addAndMakeVisible(audioProcessor.thumbnailOsc3);
    addAndMakeVisible(audioProcessor.thumbnailOsc4);
}


FirstSynthAudioProcessorEditor::~FirstSynthAudioProcessorEditor()
{
}

//==============================================================================
void FirstSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);

    g.drawRect(juce::Rectangle<int>(130, 90, 90, 60));


}

void FirstSynthAudioProcessorEditor::resized()
{
    auto thumbnailWidth = 90;
    auto thumbnailHeight = 60;
    auto thumbnailY = 170;

     
    //set bounds
    osc.setBounds(10, 10, getWidth(), 250);
    //adsr.setBounds(getWidth()/2, 0, getWidth()/2, getHeight());
    adsr.setBounds(10, 260, getWidth(), 130); //left, down, size width, size height
    filter.setBounds(10, 390, getWidth(), 200);
    modAdsr.setBounds(10, 570, getWidth(), 120);
    //thumbnailOsc1.setBounds(140, 90, 60, 40);
    //thumbnailOsc2.setBounds(thumbnailOsc1.getRight(), 90, 60, 40);
    //thumbnailOsc3.setBounds(thumbnailOsc2.getRight(), 90, 60, 40);
    //thumbnailOsc4.setBounds(thumbnailOsc3.getRight(), 90, 60, 40);
    audioProcessor.thumbnailOsc1.setBounds(20, thumbnailY, thumbnailWidth, thumbnailHeight);
    audioProcessor.thumbnailOsc2.setBounds(audioProcessor.thumbnailOsc1.getRight(), thumbnailY, thumbnailWidth, thumbnailHeight);
    audioProcessor.thumbnailOsc3.setBounds(audioProcessor.thumbnailOsc2.getRight(), thumbnailY, thumbnailWidth, thumbnailHeight);
    audioProcessor.thumbnailOsc4.setBounds(audioProcessor.thumbnailOsc3.getRight(), thumbnailY, thumbnailWidth, thumbnailHeight);

}

