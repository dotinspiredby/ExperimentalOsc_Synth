/*
  ==============================================================================

    OscComponent.h
    Created: 17 Aug 2021 12:04:02am
    Author:  garfi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>



//==============================================================================
/*
*/
class OscComponent  : public juce::Component
{
public:
    OscComponent(
        juce::AudioProcessorValueTreeState& apvts, 
        juce::String waveSelectorId, 
        juce::String wave2SelectorId,
        juce::String wave3SelectorId,
        juce::String wave4SelectorId,
        juce::String gainId,
        juce::String pitchId,
        juce::String fmFreqId, 
        juce::String fmDepthId
        );
    ~OscComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::ComboBox oscWaveSelector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscWaveSelectorAttachment;

    juce::ComboBox osc2WaveSelector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscWave2SelectorAttachment;

    juce::ComboBox osc3WaveSelector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscWave3SelectorAttachment;

    juce::ComboBox osc4WaveSelector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscWave4SelectorAttachment;


    juce::Slider gainSlider;
    juce::Slider pitchWheelSlider;
    juce::Slider fmFreqSlider;
    juce::Slider fmDepthSlider;

    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<Attachment> gainAttachment;
    std::unique_ptr<Attachment> pitchWheelAttachment;
    std::unique_ptr<Attachment> fmFreqAttachment;
    std::unique_ptr<Attachment> fmDepthAttachment;
    
    juce::Label gainLabel{ "Gain", "Gain Level" };
    juce::Label PitchWheelLabel{ "Pitch", "Pitch Level" };
    juce::Label fmFreqLabel{ "FM Freq", "FM Freq" };
    juce::Label fmDepthLabel{ "FM Depth", "FM Depth" };
    

    void setSliderParamsWithLabel(juce::Slider& slider, juce::Label& label,
        juce::AudioProcessorValueTreeState& apvts, juce::String& paramId, std::unique_ptr <Attachment>& attachment);



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscComponent)
};
