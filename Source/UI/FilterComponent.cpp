/*
  ==============================================================================

    FilterComponent.cpp
    Created: 20 Aug 2021 11:22:26pm
    Author:  garfi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterComponent.h"

//==============================================================================
FilterComponent::FilterComponent(
    juce::AudioProcessorValueTreeState& apvts,
    juce::String filterTypeId,
    juce::String filterFreqId,
    juce::String filterDepthId,
    juce::String filterResonanceId)
{
    
    setSliderParamsWithLabel(filterFreqSlider, filterFreqLabel, apvts, filterFreqId, filterFreqAttachment);
    setSliderParamsWithLabel(filterResSlider, filterResonanceLabel, apvts, filterResonanceId, filterResAttachment);
    setSliderParamsWithLabel(filterDepthSlider, filterDepthLabel, apvts, filterDepthId, filterDepthAttachment);

    juce::StringArray choices = { "Low-Pass", "Band-Pass", "High-Pass" };
    filterTypeSelector.addItemList(choices, 1);
    addAndMakeVisible(filterTypeSelector);
    filterTypeSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, filterTypeId, filterTypeSelector);
    filterSelectorLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    filterSelectorLabel.setFont(15.0f);
    filterSelectorLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(filterSelectorLabel);

}

FilterComponent::~FilterComponent()
{
}

void FilterComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.drawRoundedRectangle(juce::Rectangle<int>{ 380, 170 }.toFloat(), 6.0f, 2.0f);
}

void FilterComponent::resized()
{
    const auto startY = 100;
    const auto sliderWidth = 150;
    const auto sliderHeight = 40;
    const auto labelOffset = 20;
    const auto labelHeight = 20;

    filterFreqSlider.setBounds(30, 30, sliderWidth, sliderHeight);
    filterFreqLabel.setBounds(filterFreqSlider.getX(), filterFreqSlider.getY() - labelOffset, filterFreqSlider.getWidth(), labelHeight);

    filterResSlider.setBounds(filterFreqLabel.getRight() + labelOffset, 30, sliderWidth, sliderHeight);
    filterResonanceLabel.setBounds(filterResSlider.getX(), filterResSlider.getY() - labelOffset, filterResSlider.getWidth(), labelHeight);

    filterTypeSelector.setBounds(10, filterFreqSlider.getY() + 100, filterResonanceLabel.getWidth(), 20);

    filterDepthSlider.setBounds(filterResSlider.getX(), filterTypeSelector.getY() - 5, sliderWidth, sliderHeight);
    filterDepthLabel.setBounds(filterResSlider.getX(), filterDepthSlider.getY()- 1.5*labelOffset, sliderWidth, sliderHeight);
}

void FilterComponent::setSliderParamsWithLabel(juce::Slider& slider, juce::Label& label,
    juce::AudioProcessorValueTreeState& apvts, juce::String& paramId, std::unique_ptr <Attachment>& attachment)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(slider);

    attachment = std::make_unique<Attachment>(apvts, paramId, slider);

    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}
