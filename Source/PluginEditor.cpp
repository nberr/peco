/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "PecoParameters.h"

//==============================================================================
PecoAudioProcessorEditor::PecoAudioProcessorEditor (PecoAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    float scale = 1.5;
    setSize (400 * scale, 300 * scale);
    setResizable(false, false);
    
    // for each slider
    // - set the name (for fetching parameter)
    // - set the style
    // - set the textbox style
    // - set the range
    // - set the default value
    // - add the editor as a listener
    // - add to the scene
    
    // wet dry sliders
    mWetDrySliderL.setName(PecoParameterID[WetDryL]);
    mWetDrySliderL.setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    mWetDrySliderL.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 100, 20);
    mWetDrySliderL.setRange(PecoParameterMin[WetDryL], PecoParameterMax[WetDryL], 0.01);
    mWetDrySliderL.setValue(PecoParameterDefault[WetDryL]);
    mWetDrySliderL.addListener(this);
    addAndMakeVisible(&mWetDrySliderL);
    
    mWetDrySliderR.setName(PecoParameterID[WetDryR]);
    mWetDrySliderR.setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    mWetDrySliderR.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 100, 20);
    mWetDrySliderR.setRange(PecoParameterMin[WetDryR], PecoParameterMax[WetDryR], 0.01);
    mWetDrySliderR.setValue(PecoParameterDefault[WetDryR]);
    mWetDrySliderR.addListener(this);
    addAndMakeVisible(&mWetDrySliderR);
    
    // feedback sliders
    mFeedbackL.setName(PecoParameterID[FeedbackL]);
    mFeedbackL.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    mFeedbackL.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 100, 20);
    mFeedbackL.setRange(PecoParameterMin[FeedbackL], PecoParameterMax[FeedbackL], 0.01);
    mFeedbackL.setValue(PecoParameterDefault[FeedbackL]);
    mFeedbackL.addListener(this);
    addAndMakeVisible(&mFeedbackL);
    
    mFeedbackR.setName(PecoParameterID[FeedbackR]);
    mFeedbackR.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    mFeedbackR.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 100, 20);
    mFeedbackR.setRange(PecoParameterMin[FeedbackR], PecoParameterMax[FeedbackR], 0.01);
    mFeedbackR.setValue(PecoParameterDefault[FeedbackR]);
    mFeedbackR.addListener(this);
    addAndMakeVisible(&mFeedbackR);
    
    // distortion control sliders
    mDistControlL.setName(PecoParameterID[DistControlL]);
    mDistControlL.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    mDistControlL.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 100, 20);
    mDistControlL.setRange(PecoParameterMin[DistControlL], PecoParameterMax[DistControlL], 0.01);
    mDistControlL.setValue(PecoParameterDefault[DistControlL]);
    mDistControlL.addListener(this);
    addAndMakeVisible(&mDistControlL);
    
    mDistControlR.setName(PecoParameterID[DistControlR]);
    mDistControlR.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    mDistControlR.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 100, 20);
    mDistControlR.setRange(PecoParameterMin[DistControlR], PecoParameterMax[DistControlR], 0.01);
    mDistControlR.setValue(PecoParameterDefault[DistControlR]);
    mDistControlR.addListener(this);
    addAndMakeVisible(&mDistControlR);
    
    // delay sliders
    mDelaySlider.setName(PecoParameterID[Delay]);
    mDelaySlider.setSliderStyle(juce::Slider::SliderStyle::ThreeValueHorizontal);
    mDelaySlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    mDelaySlider.setRange(MIN_DELAY, MAX_DELAY, 1);
    mDelaySlider.setMinAndMaxValues(MIN_DELAY, MAX_DELAY);
    mDelaySlider.setValue(PecoParameterDefault[Delay]);
    mDelaySlider.addListener(this);
    addAndMakeVisible(&mDelaySlider);

    mDelay1Text.setText(std::to_string(MIN_DELAY), juce::NotificationType::dontSendNotification);
    mDelay1Text.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&mDelay1Text);
    
    mDelay2Text.setText(std::to_string(ONE_SECOND), juce::NotificationType::dontSendNotification);
    mDelay2Text.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&mDelay2Text);
    
    mDelay3Text.setText(std::to_string(MAX_DELAY), juce::NotificationType::dontSendNotification);
    mDelay3Text.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&mDelay3Text);
}

PecoAudioProcessorEditor::~PecoAudioProcessorEditor()
{
}

//==============================================================================
void PecoAudioProcessorEditor::paint (juce::Graphics& g)
{
    juce::Colour black = juce::Colour::fromFloatRGBA (0.08, 0.08, 0.08, 1.0);
    g.fillAll (black);
    
    g.setColour(juce::Colours::white);
    
    g.drawText("Wet/Dry Left", mWetDrySliderL.getX() + 5, mWetDrySliderL.getY() - 20, 100, 20, juce::Justification::centred);
    g.drawText("Wet/Dry Right", mWetDrySliderR.getX() + 5, mWetDrySliderR.getY() - 20, 100, 20, juce::Justification::centred);
    
    g.drawText("Feedback Left", mFeedbackL.getX() + 25, mFeedbackL.getY() - 20, 100, 20, juce::Justification::centred);
    g.drawText("Feedback Right", mFeedbackR.getX() + 25, mFeedbackR.getY() - 20, 100, 20, juce::Justification::centred);
    
    g.drawText("Distortion Left", mDistControlL.getX() + 25, mDistControlL.getY() - 20, 100, 20, juce::Justification::centred);
    g.drawText("Distortion Right", mDistControlR.getX() + 25, mDistControlR.getY() - 20, 100, 20, juce::Justification::centred);
    
    g.drawText("Delay", mDelaySlider.getX()+3, mDelaySlider.getY() - 20, 100, 20, juce::Justification::centred);
    
    g.drawRect(mDelay1Text.getX(), mDelay1Text.getY(), 100, 20);
    g.drawRect(mDelay2Text.getX(), mDelay2Text.getY(), 100, 20);
    g.drawRect(mDelay3Text.getX(), mDelay3Text.getY(), 100, 20);
    
    
}

void PecoAudioProcessorEditor::resized()
{
    float scale = 1.5;
    
    mDistControlL.setSize(100* scale, 20 * scale);
    mDistControlL.setCentrePosition(getWidth() * 0.2f, getHeight() * 0.25f);
    
    mDistControlR.setSize(100* scale, 20 * scale);
    mDistControlR.setCentrePosition(getWidth() * 0.8f, getHeight() * 0.25f);
    
    
    // wet dry sliders
    mWetDrySliderL.setSize(75 * scale, 75 * scale);
    mWetDrySliderL.setCentrePosition(getWidth() * 0.2f, getHeight() * 0.75f);
    
    mWetDrySliderR.setSize(75 * scale, 75 * scale);
    mWetDrySliderR.setCentrePosition(getWidth() * 0.8f, getHeight() * 0.75f);
    
    // feedback sliders
    mFeedbackL.setSize(100 * scale, 20 * scale);
    mFeedbackL.setCentrePosition(getWidth() * 0.2f, getHeight() * 0.5f);
    
    mFeedbackR.setSize(100 * scale, 20 * scale);
    mFeedbackR.setCentrePosition(getWidth() * 0.8f, getHeight() * 0.5f);
    
    // delay slider
    mDelaySlider.setSize(75 * scale, 75 * scale);
    mDelaySlider.setCentrePosition(getWidth() * 0.5f, getHeight() * 0.5f);
    
    mDelay1Text.setSize(100, 20);
    mDelay1Text.setCentrePosition(getWidth() * 0.5f, getHeight() * 0.6f);
    
    mDelay2Text.setSize(100, 20);
    mDelay2Text.setCentrePosition(getWidth() * 0.5f, getHeight() * 0.6f + 20);
    
    mDelay3Text.setSize(100, 20);
    mDelay3Text.setCentrePosition(getWidth() * 0.5f, getHeight() * 0.6f + 40);
}

void PecoAudioProcessorEditor::sliderValueChanged(juce::Slider *s)
{
    if (s == &mWetDrySliderL) {
        juce::AudioParameterFloat *wetDryL = (juce::AudioParameterFloat *)audioProcessor.parameters.getParameter(s->getName());
        *wetDryL = s->getValue();
    }
    else if (s == &mWetDrySliderR) {
        juce::AudioParameterFloat *wetDryR = (juce::AudioParameterFloat *)audioProcessor.parameters.getParameter(s->getName());
        *wetDryR = s->getValue();
    }
    else if (s == &mFeedbackL) {
        juce::AudioParameterFloat *fbL = (juce::AudioParameterFloat *)audioProcessor.parameters.getParameter(s->getName());
        *fbL = s->getValue();
    }
    else if (s == &mFeedbackR) {
        juce::AudioParameterFloat *fbR = (juce::AudioParameterFloat *)audioProcessor.parameters.getParameter(s->getName());
        *fbR = s->getValue();
    }
    else if (s == &mDelaySlider) {
        juce::AudioParameterInt *delay = (juce::AudioParameterInt *)audioProcessor.parameters.getParameter(PecoParameterID[Delay]);
        *delay = (int)(s->getMinValue());
        
        juce::AudioParameterInt *delay2 = (juce::AudioParameterInt *)audioProcessor.parameters.getParameter(PecoParameterID[Delay2]);
        *delay2 = (int)(s->getValue());
        
        juce::AudioParameterInt *delay3= (juce::AudioParameterInt *)audioProcessor.parameters.getParameter(PecoParameterID[Delay3]);
        *delay3 = (int)(s->getMaxValue());
        
        audioProcessor.delayChanged = true;
        
        // update the text boxes
        mDelay1Text.setText(std::to_string((int)mDelaySlider.getMinValue()), juce::NotificationType::dontSendNotification);
        mDelay2Text.setText(std::to_string((int)mDelaySlider.getValue()), juce::NotificationType::dontSendNotification);
        mDelay3Text.setText(std::to_string((int)mDelaySlider.getMaxValue()), juce::NotificationType::dontSendNotification);
    }
    else if (s == &mDistControlL) {
        juce::AudioParameterFloat *dLevelL = (juce::AudioParameterFloat *)audioProcessor.parameters.getParameter(s->getName());
        *dLevelL = s->getValue();
    }
    else if (s == &mDistControlR) {
        juce::AudioParameterFloat *dLevelR = (juce::AudioParameterFloat *)audioProcessor.parameters.getParameter(s->getName());
        *dLevelR = s->getValue();
    }
}
