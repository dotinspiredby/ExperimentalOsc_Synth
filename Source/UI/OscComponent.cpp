/*
  ==============================================================================

    OscComponent.cpp
    Created: 17 Aug 2021 12:04:02am
    Author:  garfi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscComponent.h"

//==============================================================================
OscComponent::OscComponent(juce::AudioProcessorValueTreeState& apvts, 
    juce::String waveSelectorId, 
    juce::String wave2SelectorId,
    juce::String wave3SelectorId,
    juce::String wave4SelectorId,
    juce::String gainId,
    juce::String pitchId,
    juce::String fmFreqId, 
    juce::String fmDepthId)
{
    juce::StringArray choices = { "sin(x)", "Saw Wave (x/Pi)", "Square Wave (if x<0 then -1, else x)",
                                  "cos(x - sin(x))", "cos(x - sin(5x))",
                                  "sin(7x + cos(3x))", "sin(3x)-cos(x/3)",
                                  "sin(3x) - cos(x/9)", "sin(x-cos(3x)) - cos(x-sin(3x))", "sin(x) + 2cos(x / 2)" };
    juce::StringArray choices_others = { "sin(x)", "Saw Wave (x/Pi)", "Square Wave (if x<0 then -1, else x)",
                                  "cos(x - sin(x))", "cos(x - sin(5x))",
                                  "sin(7x + cos(3x))", "sin(3x)-cos(x/3)",
                                  "sin(3x) - cos(x/9)", "sin(x-cos(3x)) - cos(x-sin(3x))", "sin(x) + 2cos(x / 2)", "N/A" };


    oscWaveSelector.addItemList(choices,1);
    osc2WaveSelector.addItemList(choices_others, 1);
    osc3WaveSelector.addItemList(choices_others, 1);
    osc4WaveSelector.addItemList(choices_others, 1);

    addAndMakeVisible(oscWaveSelector);
    addAndMakeVisible(osc2WaveSelector);
    addAndMakeVisible(osc3WaveSelector);
    addAndMakeVisible(osc4WaveSelector);
    
    oscWaveSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, waveSelectorId, oscWaveSelector);
    oscWave2SelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, wave2SelectorId, osc2WaveSelector);
    oscWave3SelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, wave3SelectorId, osc3WaveSelector);
    oscWave4SelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, wave4SelectorId, osc4WaveSelector);

    setSliderParamsWithLabel(gainSlider, gainLabel, apvts, gainId, gainAttachment);
    setSliderParamsWithLabel(pitchWheelSlider, PitchWheelLabel, apvts, pitchId, pitchWheelAttachment);
    setSliderParamsWithLabel(fmFreqSlider, fmFreqLabel, apvts, fmFreqId, fmFreqAttachment);
    setSliderParamsWithLabel(fmDepthSlider, fmDepthLabel, apvts, fmDepthId, fmDepthAttachment);
}

OscComponent::~OscComponent()
{
}

void OscComponent::paint (juce::Graphics& g)
{
    //auto bounds = juce::Rectangle<int>{ 380, 200 };
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.drawRoundedRectangle(juce::Rectangle<int>{ 380, 240 }.toFloat(), 6.0f, 2.0f);

    //g.drawRect(juce::Rectangle<int>(130, 90, 90, 60));
}

void OscComponent::resized()
{
    const auto startY = 190;
    const auto sliderWidth = 150;
    const auto sliderHeight = 40;
    const auto labelOffset = 20;
    const auto labelHeight = 20;

    oscWaveSelector.setBounds(10, 10, 90, 20);
    osc2WaveSelector.setBounds(10, 40, 90, 20);
    osc3WaveSelector.setBounds(10, 70, 90, 20);
    osc4WaveSelector.setBounds(10, 100, 90, 20);

    gainSlider.setBounds(oscWaveSelector.getRight() + labelOffset, 10 + labelOffset, sliderWidth - 20, sliderHeight);
    gainLabel.setBounds(oscWaveSelector.getRight() + labelOffset, gainSlider.getY() - labelOffset, gainSlider.getWidth(), labelHeight);

    fmFreqSlider.setBounds(gainSlider.getX(), 100, sliderWidth - 20, sliderHeight);
    //fmFreqSlider.setBounds(30, startY, sliderWidth, sliderHeight);
    fmFreqLabel.setBounds(fmFreqSlider.getX(), fmFreqSlider.getY() - labelOffset, fmFreqSlider.getWidth(), labelHeight);

    fmDepthSlider.setBounds(fmFreqSlider.getRight(), 100, sliderWidth - 20, sliderHeight);
    //fmDepthSlider.setBounds(fmFreqSlider.getRight() + labelOffset, startY, sliderWidth, sliderHeight);
    fmDepthLabel.setBounds(fmDepthSlider.getX(), fmDepthSlider.getY() - labelOffset, fmDepthSlider.getWidth(), labelHeight);

    pitchWheelSlider.setBounds(fmDepthSlider.getX(), gainSlider.getY(), sliderWidth - 20, sliderHeight);
    //pitchWheelSlider.setBounds(fmFreqSlider.getRight() + 2 * labelOffset, 10 + labelOffset, sliderWidth, sliderHeight);
    //PitchWheelLabel.setBounds(fmFreqSlider.getRight() + 2 * labelOffset, pitchWheelSlider.getY() - labelOffset, pitchWheelSlider.getWidth(), labelHeight);
    PitchWheelLabel.setBounds(fmDepthSlider.getX(), pitchWheelSlider.getY() - labelOffset, pitchWheelSlider.getWidth(), labelHeight);
}

void OscComponent::setSliderParamsWithLabel(juce::Slider& slider, juce::Label& label,
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
