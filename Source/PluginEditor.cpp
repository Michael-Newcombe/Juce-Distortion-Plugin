/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DistortionAudioProcessorEditor::DistortionAudioProcessorEditor (DistortionAudioProcessor& p)
: AudioProcessorEditor (&p), processor (p)

{
    
    //initalizing the gui
    setSize (475, 225);
    //setting up the pre gain gui slider
    addAndMakeVisible(&preGainGui);
    preGainGui.setSliderStyle(Slider::SliderStyle::Rotary);
    preGainGui.setRange(-60.0f, 60.f);
    preGainGui.setValue(30.0f);
    preGainGui.setTextValueSuffix(" db");
    preGainGui.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 25);
    preGainGui.setTextBoxIsEditable(true);
    preGainGui.setLookAndFeel(&myGuiDesign);
    addAndMakeVisible(&preGainGuiLabel);
    preGainGuiLabel.setText("Input Gain", dontSendNotification);
    preGainGuiLabel.setLookAndFeel(&myGuiDesign);
    
    //setting up the post gain gui slider
    addAndMakeVisible(&postGainGui);
    postGainGui.setSliderStyle(Slider::SliderStyle::Rotary);
    postGainGui.setRange(-60.0f, 0.0f);
    postGainGui.setValue(-20.0f);
    postGainGui.setTextValueSuffix(" db");
    postGainGui.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 25);
    postGainGui.setLookAndFeel(&myGuiDesign);
    addAndMakeVisible(&postGainGuiLabel);
    postGainGuiLabel.setText("Post Gain", dontSendNotification);
    postGainGuiLabel.setLookAndFeel(&myGuiDesign);

    //setting up the reverb toggle button
    addAndMakeVisible(&reverbGuiToggle);
    reverbGuiToggle.setToggleState(true, sendNotification);
    reverbGuiToggle.setClickingTogglesState(true);
    reverbGuiToggle.setLookAndFeel(&myGuiDesign);
    addAndMakeVisible(reverbGuiLabel);
    reverbGuiLabel.setText("Bypass Wet Signal", dontSendNotification);
    reverbGuiLabel.attachToComponent(&reverbGuiToggle, true);
    
    //setting up the hard clip toggle button
    addAndMakeVisible(&hardClipGuiToggle);
    hardClipGuiToggle.setClickingTogglesState(true);
    addAndMakeVisible(hardClipGuiLabel);
    hardClipGuiLabel.setText("Enable Hard Clip", dontSendNotification);
    hardClipGuiLabel.attachToComponent(&hardClipGuiToggle, true);
    
    //linking the gui input gain slider to the pluginProcessor preGain parameter
    preSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.sliderTree, "preGainID", preGainGui);
    //linking the gui post gain slider to the pluginProcessor postGain parameter
    postSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.sliderTree, "postGainID", postGainGui);
    //linking the gui reverb toggle button to the pluginProcessor reverb parameter
    reverbToggleValue = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(processor.radioBoxTree, "reverbID", reverbGuiToggle);
    //linking the gui hard clip toggle button to the pluginProcessor hardClip parameter
   hardClipToggleValue = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(processor.radioBoxTree, "hardClipID", hardClipGuiToggle);
    
    //chainng the slider style
    myGuiDesign.setColour (Slider::thumbColourId, Colours::purple);
    myGuiDesign.setColour (Slider::rotarySliderFillColourId, Colours::rebeccapurple);
    
}
DistortionAudioProcessorEditor::~DistortionAudioProcessorEditor()
{
}

//==============================================================t================
void DistortionAudioProcessorEditor::paint (Graphics& g)
{
    //setting the background colour
    g.fillAll (Colours::slategrey);
    //setting the text colour
    g.setColour (Colours::white);
    //setting the font size
    g.setFont (15.0f);
}

void DistortionAudioProcessorEditor::resized()
{
   
    //drawing the sliders
    preGainGui.setBounds(0, 50, 150, 150);
    preGainGuiLabel.setBounds(40, 25, 100, 25);
    
    postGainGui.setBounds(150, 50, 150, 150);
    postGainGuiLabel.setBounds(190, 25, 100, 25);
    
    //drawing the buttons
    reverbGuiToggle.setBounds(425, 75, 25, 25);
    hardClipGuiToggle.setBounds(425, 100, 25, 25);

}

