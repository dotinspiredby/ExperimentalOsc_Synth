/*
  ==============================================================================

    AdsrComponent.cpp
    Created: 15 Aug 2021 3:04:00pm
    Author:  garfi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AdsrComponent.h"

//==============================================================================
AdsrComponent::AdsrComponent(juce::String name, juce::AudioProcessorValueTreeState& apvts, juce::String attackId, juce::String decayId, 
    juce::String sustainId, juce::String releaseId)
{
    componentName = name;
    //using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    //attackAttachment = std::make_unique<SliderAttachment>(apvts, attackId, attackSlider);
    //decayAttachment = std::make_unique<SliderAttachment>(apvts, decayId, decaySlider);
    //sustainAttachment = std::make_unique<SliderAttachment>(apvts, sustainId, sustainSlider);
    //releaseAttachment = std::make_unique<SliderAttachment>(apvts, releaseId, releaseSlider);

    setSliderParamsWithLabel(attackSlider, attackLabel, apvts, attackId, attackAttachment);
    setSliderParamsWithLabel(decaySlider, decayLabel, apvts, decayId, decayAttachment);
    setSliderParamsWithLabel(sustainSlider, sustainLabel, apvts, sustainId, sustainAttachment);
    setSliderParamsWithLabel(releaseSlider, releaseLabel, apvts, releaseId, releaseAttachment);

}

AdsrComponent::~AdsrComponent()
{
}

void AdsrComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.drawRoundedRectangle(juce::Rectangle<int>{ 380, 120 }.toFloat(), 6.0f, 2.0f);
}

void AdsrComponent::resized()
{
    const auto bounds = getLocalBounds().reduced(10);
    const auto padding = 10; //dist between sliders
    const auto sliderWidth = bounds.getWidth() / 4 - padding;
    const auto sliderHeight = bounds.getWidth() / 4 - padding;
    const auto sliderStartX = 0;
    const auto Y = 25;

    
    attackSlider.setBounds(sliderStartX, Y, sliderWidth, sliderHeight);
    attackLabel.setBounds(sliderStartX, attackSlider.getY() - 15, sliderWidth, 20);

    decaySlider.setBounds((attackSlider.getRight() + padding), Y, sliderWidth, sliderHeight);
    decayLabel.setBounds(decaySlider.getX(), attackSlider.getY() - 15, sliderWidth, 20);

    sustainSlider.setBounds(decaySlider.getRight() + padding, Y, sliderWidth, sliderHeight);
    sustainLabel.setBounds(sustainSlider.getX(), attackSlider.getY() - 15, sliderWidth, 20);

    releaseSlider.setBounds(sustainSlider.getRight() + padding, Y, sliderWidth, sliderHeight);
    releaseLabel.setBounds(releaseSlider.getX(), attackSlider.getY() - 15, sliderWidth, 20);
}

void AdsrComponent::setSliderParamsWithLabel(juce::Slider& slider, juce::Label& label,
    juce::AudioProcessorValueTreeState& apvts, juce::String& paramId, std::unique_ptr <Attachment>& attachment)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(slider);

    attachment = std::make_unique<Attachment>(apvts, paramId, slider);

    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}