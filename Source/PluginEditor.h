/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/AdsrComponent.h"
#include "UI/OscComponent.h"
#include "UI/FilterComponent.h"
#include "ThumbnailComponent.h"


//==============================================================================
/**
*/
class FirstSynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FirstSynthAudioProcessorEditor (FirstSynthAudioProcessor&);
    ~FirstSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //void setSliderParams(juce::Slider& slider);

    /*juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;*/

    //juce::ComboBox oscSelector;
    //std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscSelAttachment;
    
    /*using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> decayAttachment;
    std::unique_ptr<SliderAttachment> sustainAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscSelAttachment;*/
    
    FirstSynthAudioProcessor& audioProcessor;

    OscComponent osc;
    AdsrComponent adsr;
    FilterComponent filter;
    AdsrComponent modAdsr;
    //ThumbnailComponent thumbnail1;
    //ThumbnailComponent thumbnail2;
    //ThumbnailComponent thumbnail3;
    //ThumbnailComponent thumbnail4;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FirstSynthAudioProcessorEditor)
};
