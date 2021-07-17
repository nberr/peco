/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class PecoAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener
{
public:
    PecoAudioProcessorEditor (PecoAudioProcessor&);
    ~PecoAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged(juce::Slider *s) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PecoAudioProcessor& audioProcessor;

    juce::Slider mDelaySlider;
    juce::Label mDelay1Text;
    juce::Label mDelay2Text;
    juce::Label mDelay3Text;
    
    // left channel controls
    juce::Slider mWetDrySliderL;
    juce::Slider mFeedbackL;
    juce::Slider mDistControlL;
    
    // right channel controls
    juce::Slider mWetDrySliderR;
    juce::Slider mFeedbackR;
    juce::Slider mDistControlR;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PecoAudioProcessorEditor)
};
