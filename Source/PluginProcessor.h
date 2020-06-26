/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#define PRE_ID "pre"
#define PRE_NAME "Pre"

//==============================================================================
/**
*/
class DistortionAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    DistortionAudioProcessor();
    ~DistortionAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //creating a AudioProcessorValueTreeState called sliderTree to manage the different parameters and to connect the parameters inside the pluginProcessor and the parameters of the sliders inside the GUI to enable automation of the parameters when using a DAW.
    AudioProcessorValueTreeState sliderTree;
    //calling the ParameterLayout constructor for the sliderTree so that mutiple ranged audio parameters can be added to tree more easily
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
      //creating another AudioProcessorValueTreeState for the same purpose however this tree is used to connect boolean parameters to radioBoxes inside the GUI.
    AudioProcessorValueTreeState radioBoxTree;
     //creating another
    AudioProcessorValueTreeState::ParameterLayout createRadioBoxLayout();
    //creating a function called updateDistortion which is used to update the paramter values. 
    void updateDistortion();
    

private:
    //==============================================================================
    //creating an enumeration data type so that the index of a juce dsp process can be easily referenced inside the ProcessorChain. Without an enum each process would have to referenced by it's raw index value, for example the first process in the chain would be 0, the second 1 and so on.
    enum{
        filterIndex,
        preGainIndex,
        waveshaperIndex,
        postGainIndex,
        reverbIndex             // [2]
    };
    //creating a juce infinite impulse response filter object and using a typedef so the object can be easily referenced
    using Filter = juce::dsp::IIR::Filter<float>;
    //creating the coefficients for the juce infinite impulse response filter
    using FilterCoefs = juce::dsp::IIR::Coefficients<float>;
    
    //Using the juce dsp module a ProcessorChain is being created which is used to store mutiple dsp classes into a signal process. The dsp processor classes in this chain consist of the following:
    //(1) The first dsp process is a ProcessorDuplicator which convert a mono class into multichannel class which takes care of our left and right output. This process is then being passed the IIR Filter and IIR Coefficients objects
    //(2) The second process is the juce gain class which applies a gain to the audio
    // (3) The third process is the juce WaveShaper which is tree waveshaping to the audio
    //(4) The fourth process is another gain class
    //(5) The last process is reverb class for adding reverb to the audio
    juce::dsp::ProcessorChain<juce::dsp::ProcessorDuplicator<Filter, FilterCoefs>, // [1]
    juce::dsp::Gain<float>, juce::dsp::WaveShaper<float>, juce::dsp::Gain<float>,juce::dsp::ProcessorChain<juce::dsp::Reverb>> processorChain;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionAudioProcessor)

};
