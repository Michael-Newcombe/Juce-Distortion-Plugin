/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
//constructor
DistortionAudioProcessor::DistortionAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                    ),
//calling the contructor for the sliderTree which is used set up the parameters which control the dsp processes by connecting to the current audio processor, setting no undo manager, the tree ID and the createParameterLayout function which contains mutiple ranged audio parameters.
sliderTree(*this,nullptr,"treeParameters",createParameterLayout()),
//calling the contructor for the radioBoxTree which has the same functionality as the sliderTree however this tree is using AudioParameterBool parameters instead of ranged audio parameters.
radioBoxTree(*this,nullptr,"treeRadioBoxes",createRadioBoxLayout())

#endif
{
    //accessing the dsp Waveshaper processer inside the processorChain.
    auto& waveshaper = processorChain.template get<waveshaperIndex>();
    //accessing the value of the hardClip parameter
    auto hardClipping = radioBoxTree.getParameterAsValue("hardClipID").getValue();
    //if the hardClip parameter boolean value equals true then a hard clipping distortion is applied to the sound.
    if (hardClipping.equals(true)) {
        //defining the transfer function the dsp waveshaper function uses using a lamda function.
        waveshaper.functionToUse = [] (float x)
        {
            //creating a hard clipping transfer function by returning a signum transfer function by contraining the signal between the ranges of -0.5 and 0.5 using the juce jlimit function. The value being contrained is x which is the incoming audio signal, which is also being passed into a hyperbolic tangent function.
            return jlimit (float (-0.5), float (0.5), std::tanh(x));
        };
    }
    //if the hardClip parameter boolean value equals false then a soft clipping distortion is applied to the sound.
    else if(hardClipping.equals(false)) {
        waveshaper.functionToUse = [] (float x)
        {
            //creating a soft clipping transfer function by returning just a hyperbolic tangent function as it uses a more smooth curve.
            return std::tanh(x);
        };
    }
   
    //accessing the current parameter float value from the preGain parameter using the getRawParameterValue function and passing in the ID of the parameter.
    auto initalPreValue = sliderTree.getRawParameterValue("preGainID");
    //same as above however this variable is getting the parameter value from the postGain parameter.
    auto initalPostValue = sliderTree.getRawParameterValue("postGainID");
    
    //accessing the dsp gain processor inside the processorChain which is being used as the preGain for the distortion. This is to boost or decrease the signal going into the waveshaper to change the intensity of the distortion. This is applied before the waveshaper process in the processorChain as it initalized before the processorChain.
    auto& preGain = processorChain.template get<preGainIndex>();
    //setting the gain processer to use decibel values and passing in the current preGain parameter value.
    preGain.setGainDecibels (*initalPreValue);
    
     //accessing the second dsp gain processor inside the processorChain which gets called after the waveshaper, this gain processor is being used as the postGain for the distortion to reduce the amplidute of the signal coming from the waveshaper as the signal that is going into the waveshaper is being increased the signal which comes out of the waveshaper has to be reduced
    auto& postGain = processorChain.template get<postGainIndex>();
    //setting the second gain processer to use decible values and passing in the current postGain parameter value.
    postGain.setGainDecibels (*initalPostValue);
        
}

//destructor
DistortionAudioProcessor::~DistortionAudioProcessor()
{
}

//this function contains the AudioParamters used in the sliderTree 
AudioProcessorValueTreeState::ParameterLayout DistortionAudioProcessor::createParameterLayout()
{
    //creating a vector of RangedAudioParameters
    std::vector <std::unique_ptr<RangedAudioParameter>> parameters;
    
    //adding a AudioParamterFloat to the parameters vector, this parameter is used to control the distortion preGain value. The min value is -60.0f, the max value is 60.0f and the intial value is 30.0f.
    parameters.push_back(std::make_unique<AudioParameterFloat>("preGainID","preGain",-60.0f,60.0f,30.0f));
    //adding a AudioParamterFloat to the parameters vector, this parameter is used to control the distortion postGain value. The min value is -60.0f, the max value is 0.0f and the intial value is -20.0f.
    parameters.push_back(std::make_unique<AudioParameterFloat>("postGainID","postGain",-60.0f,0.0f,-20.f));
//    NormalisableRange<float> cutoffRange (100.0f, 2000.0f);
//    params.push_back(std::make_unique<AudioParameterFloat>("hpFilterID","hpFilter",cutoffRange,100.0f));

    //returning the parameters
    return { parameters.begin(), parameters.end()};
    
}

//this function contains the AudioParamters used in the radioBoxTree
AudioProcessorValueTreeState::ParameterLayout DistortionAudioProcessor::createRadioBoxLayout()
{
    //creating a vector of AudioParameterBool parameters
    std::vector <std::unique_ptr<AudioParameterBool>> radios;
    
    //adding a AudioParameterBool to the radios vector, this parameter is used to toggle the reverb on and off with an inital value of true
    radios.push_back(std::make_unique<AudioParameterBool>("reverbID","reverb",true));
     //adding a AudioParameterBool to the radios vector, this parameter is used to toggle the distortion transfer function type, the inital value is set to false
    radios.push_back(std::make_unique<AudioParameterBool>("hardClipID","hardClip",false));
    
    //returning the parameters
    return { radios.begin(), radios.end()};
    
}
//==============================================================================
const String DistortionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DistortionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DistortionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DistortionAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DistortionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DistortionAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DistortionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DistortionAudioProcessor::setCurrentProgram (int index)
{
}

const String DistortionAudioProcessor::getProgramName (int index)
{
    return {};
}

void DistortionAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DistortionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    //initalizing a juce dsp ProcessSpec structure
    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getMainBusNumInputChannels();
    
    //calling reset on processor chain to reset the processing pipeline
    processorChain.reset();
    //calling prepare on the processor chain to set up the sample rate and block size and passing it the spec object which contains our sample rate and our samples per block
    processorChain.prepare (spec);
    
    //accessing the dsp Filter processer inside the processorChain.
    auto& filter = processorChain.template get<filterIndex>();
    //assigning the filter state to use a first order high pass filter also known as a single pole filter with a cutoff of 300 Hz. This is to make the distorted sound have a more clean output by removing some of the low frequencies from the sound as when a sound with low frequecny content gets distored the sound can become very muddy
    filter.state = FilterCoefs::makeFirstOrderHighPass (spec.sampleRate, 300.0f);
    
    //accessing the dsp reverb processer inside the processorChain.
    auto& reverb = processorChain.template get<reverbIndex>();
    //accessing the value of the reverb parameter
    auto reverbParam = radioBoxTree.getParameterAsValue("reverbID");
    //
   //accesing the reverb is by passed function and assigning it to the reverbParam value which is set to true, this means the reverb will not be active when the plugin fists loads
    reverb.isBypassed = reverbParam.getValue();

    
}

void DistortionAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

//this function is used to get the updated values from the parameters
void DistortionAudioProcessor::updateDistortion()
{
    //the same method that was used in the contructor is being used to set up the pre gain prosessor.
    //accessing the dsp gain processor.
    auto& preGain = processorChain.template get<preGainIndex>();
    //accessing the preGain parameter value.
    auto updatedPreValue = sliderTree.getRawParameterValue("preGainID");
    //setting the gain processer to use decibels and passing in the current preGain parameter value.
    preGain.setGainDecibels (*updatedPreValue);
    
    //the same method that was used in the contructor is being used to set the post gain value
    //accessing the dsp gain processor.
    auto& postGain = processorChain.template get<postGainIndex>();
    //accessing the postGain parameter value.
    auto updatedPostValue = sliderTree.getRawParameterValue("postGainID");
    //setting the gain processer to use decibels and passing in the current postGain parameter value.
    postGain.setGainDecibels (*updatedPostValue);
    
    //the same method that was used in the prepareToPlay function is being used to set up the reverb processor
    //accessing the dsp reverb processor.
    auto& reverb = processorChain.template get<reverbIndex>();
    //accessing the reverb parameter value
    auto reverbParam = radioBoxTree.getParameterAsValue("reverbID");
    //accesing the reverb is by passed function and assigning it to the reverbParam value
    reverb.isBypassed = reverbParam.getValue();
    
    //the same method used in the contructor is also used to set up the waveshaper and to determine which transfer function is used.
    //accessing the dsp Waveshaper processer.
    auto& waveshaper = processorChain.template get<waveshaperIndex>();
    //accessing the value of the hardClip parameter
    auto hardClipping = radioBoxTree.getParameterAsValue("hardClipID").getValue();
    //checking if hardClipping equals true
    if (hardClipping.equals(true)) {
        //defining the transfer function the dsp waveshaper function uses.
        waveshaper.functionToUse = [] (float x)
        {
            //returning the transfer function
            return jlimit (float (-0.5), float (0.5), std::tanh(x));
        };
    }
    //checking if hardClipping equals false
    else if(hardClipping.equals(false)) {
        //defining the transfer function the dsp waveshaper function uses.
        waveshaper.functionToUse = [] (float x)
        {
            //returning the transfer function.
            return std::tanh(x);
        };
    }
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DistortionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

//this is where the main audio processing happens where we take our input singal, manipulate the signal which in this case is by adding distortion, filtering and reverb, then we output the resut of this giving us a different sound.
void DistortionAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

  
    //looping through the total number of output channels, this for loop checks if there is more output channels than input channels, if this is true then any output channels that didn't contain input data are cleared.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        //clearing our audio buffer for specified region by passing in our channel, start samples and the number of samples allocated inside the buffers channel
        buffer.clear (i, 0, buffer.getNumSamples());
        //this is where we manipulate the signal
    {
        //calling the dsp audio block which contains a list of pointers
        dsp::AudioBlock<float> block (buffer);
        //calling process on the processorChain which processes our input and output buffers from the ProcessContextReplacing
        processorChain.process(dsp::ProcessContextReplacing<float> (block));
        //calling the updateDistortion function to get the updated parameter values
        updateDistortion();
    }
}

//==============================================================================
bool DistortionAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DistortionAudioProcessor::createEditor()
{
    return new DistortionAudioProcessorEditor (*this);
}

//==============================================================================
void DistortionAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DistortionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DistortionAudioProcessor();
}
