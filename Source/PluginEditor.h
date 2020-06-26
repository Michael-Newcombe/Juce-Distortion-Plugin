/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DistortionAudioProcessorEditor  : public AudioProcessorEditor, public LookAndFeel_V4
{
public:
    DistortionAudioProcessorEditor (DistortionAudioProcessor&);
    ~DistortionAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DistortionAudioProcessor& processor;
    Slider preGainGui;
    Label preGainGuiLabel;
    
    Slider postGainGui;
    Label postGainGuiLabel;
    
//    Slider hpFilterGainGui;
//    Label hpFilterGuiLabel;
    
    ToggleButton reverbGuiToggle;
    Label reverbGuiLabel;
    
    ToggleButton hardClipGuiToggle;
    Label hardClipGuiLabel;
    
    LookAndFeel_V4 myGuiDesign;
    
public:
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> preSliderValue;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> postSliderValue;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> reverbToggleValue;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> hardClipToggleValue;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionAudioProcessorEditor)
};

