/*
  ==============================================================================

    ThumbnailComponent.cpp
    Created: 8 Oct 2021 8:11:14am
    Author:  garfi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ThumbnailComponent.h"

//==============================================================================
ThumbnailComponent::ThumbnailComponent(juce::String OscNum, int graphic_choice_by_default) : 
 
    text(OscNum), 
    osc_graphic_choice(graphic_choice_by_default)
{
}

/*
ThumbnailComponent::ThumbnailComponent(const ThumbnailComponent&)
{
}
*/

ThumbnailComponent::~ThumbnailComponent()
{
}

void ThumbnailComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.drawRect(getLocalBounds());

    float pseudo_amplitude = 15.0f;
    float pseudo_freq = 0.2f;

    juce::Path wavepath;
    auto centerY = getHeight() / 2.0f;
    wavepath.startNewSubPath(0, centerY);
    for (auto x = 1.0f; x < getWidth(); ++x)
    {
        switch (osc_graphic_choice)
        {
            case 0:
                wavepath.lineTo(x, centerY - pseudo_amplitude * std::sin(pseudo_freq* x));
                break;
            case 1:
                wavepath.lineTo(x, centerY - (x < getWidth()/2 ? x / juce::MathConstants<float>::pi : (x - getWidth()) / juce::MathConstants<float>::pi));
                break;
            case 2:
                wavepath.lineTo(x, centerY - (x < (getWidth()/2) ? 15.0f : -15.0f));
                break;
            case 3:
                wavepath.lineTo(x, centerY - pseudo_amplitude * std::cos(.5f * pseudo_freq*x - std::sin(pseudo_freq*x)));
                break;
            case 4:
                wavepath.lineTo(x, centerY - pseudo_amplitude * std::cos(pseudo_freq*x - std::sin(5.f * pseudo_freq*x)));
                break;
            case 5:
                wavepath.lineTo(x, centerY - pseudo_amplitude * (std::sin(7.f * (pseudo_freq-0.1f)*x + std::cos(3.f * (pseudo_freq-.1f)*x))));
                break;
            case 6:
                wavepath.lineTo(x, centerY - pseudo_amplitude * (std::sin(3.f * pseudo_freq*x) - std::cos(pseudo_freq*x / 3.f)));
                break;
            case 7:
                wavepath.lineTo(x, centerY - pseudo_amplitude * (std::sin(3.f * (pseudo_freq)*x) - std::cos((pseudo_freq)*x / 9.f)));
                break;
            case 8:
                wavepath.lineTo(x, centerY - pseudo_amplitude * std::sin(pseudo_freq * x - std::cos(3.f * pseudo_freq * x)) - std::cos(sin(pseudo_freq * x - std::sin(3.f * pseudo_freq * x))));
                break;
            case 9:
                wavepath.lineTo(x, centerY - pseudo_amplitude * std::sin((pseudo_freq+.2f) * x) + 2.f * std::cos((pseudo_freq+.2f) * x / 2.f));
                break;
            case 10:
                wavepath.lineTo(x, centerY);
                break;
        }
        
        g.setColour(juce::Colours::white);
        g.strokePath(wavepath, juce::PathStrokeType(0.4f));
    }

}

void ThumbnailComponent::change_graphic_choice(const int choice) 
{
    osc_graphic_choice = choice; 
    repaint();
}

void ThumbnailComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
