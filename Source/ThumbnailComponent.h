/*
  ==============================================================================

    ThumbnailComponent.h
    Created: 8 Oct 2021 8:11:14am
    Author:  garfi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


//==============================================================================
/*
*/
class ThumbnailComponent  : public juce::Component
{
public:
    ThumbnailComponent(juce::String OscNum, int graphic_choice_by_default);
    //ThumbnailComponent(const ThumbnailComponent&);
    ~ThumbnailComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void change_graphic_choice(const int choice);

private:
    int osc_graphic_choice;
    std::vector<float> audioPoints;
    juce::String text;

    //SynthVoice& audioProcessor;
    //FirstSynthAudioProcessor& audioProcessor;
    //juce::AudioBuffer<float>& waveForm;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThumbnailComponent)
};
