/*
  ==============================================================================

    FilterComponent.h
    Created: 20 Aug 2021 11:22:26pm
    Author:  garfi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FilterComponent  : public juce::Component
{
public:
    FilterComponent(
        juce::AudioProcessorValueTreeState& apvts,
        juce::String filterTypeId,
        juce::String filterFreqId,
        juce::String filterDepthId,
        juce::String filterResonanceId);
    ~FilterComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::ComboBox filterTypeSelector{ "Filter Type" };
    juce::Slider filterFreqSlider;
    juce::Slider filterResSlider;
    juce::Slider filterDepthSlider;
    

    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeSelectorAttachment;
    std::unique_ptr<Attachment> filterFreqAttachment;
    std::unique_ptr<Attachment> filterResAttachment;
    std::unique_ptr<Attachment> filterDepthAttachment;

    juce::Label filterSelectorLabel { "Filter Type", "Filter Type" };
    juce::Label filterFreqLabel { "Filter Freq", "Filter Freq" };
    juce::Label filterResonanceLabel { "Filter Resonance", "Filter Res" };
    juce::Label filterDepthLabel{ "Filter Depth", "Filter Depth" };

    void setSliderParamsWithLabel(juce::Slider& slider, juce::Label& label,
        juce::AudioProcessorValueTreeState& apvts, juce::String& paramId, std::unique_ptr <Attachment>& attachment);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};
