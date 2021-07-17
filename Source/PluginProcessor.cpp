/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "PecoParameters.h"

//==============================================================================
PecoAudioProcessor::PecoAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
        parameters(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    for (int i = 0; i < 96000; i++) {
        DDL[0][i] = 0;
        DDL[1][i] = 0;
    }
}

PecoAudioProcessor::~PecoAudioProcessor()
{
}

//==============================================================================
const juce::String PecoAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PecoAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PecoAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PecoAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PecoAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PecoAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PecoAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PecoAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PecoAudioProcessor::getProgramName (int index)
{
    return {};
}

void PecoAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PecoAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void PecoAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PecoAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void PecoAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    int numSamples = buffer.getNumSamples();
    if (numSamples == 0) {
        return; // no audio to process
    }
    
    int numOutputs = getTotalNumOutputChannels();
    auto channelDataL = buffer.getWritePointer(0);
    auto channelDataR = (numOutputs == 2) ? buffer.getWritePointer(1) : buffer.getWritePointer(0);
    
    for (int n = 0; n < numSamples; n++) {
        // get the user parameters
        float wetL = parameters.getParameter(PecoParameterID[WetDryL])->getValue();
        float dryL = 1.0f - wetL;
        
        float wetR = parameters.getParameter(PecoParameterID[WetDryR])->getValue();
        float dryR = 1.0f - wetR;
        
        float fbL = parameters.getParameter(PecoParameterID[FeedbackL])->getValue();
        float fbR = parameters.getParameter(PecoParameterID[FeedbackR])->getValue();
        
        if (delayChanged) {
            
            for (int i = 0; i < 96000; i++) {
                DDL[0][i] = 0;
                DDL[1][i] = 0;
            }
            
            writeInx = 0;
            
            delayInxLow = 96000 - ((juce::AudioParameterInt *)parameters.getParameter(PecoParameterID[Delay]))->get();
            delayInxMid = 96000 - ((juce::AudioParameterInt *)parameters.getParameter(PecoParameterID[Delay2]))->get();
            delayInxHigh = 96000 - ((juce::AudioParameterInt *)parameters.getParameter(PecoParameterID[Delay3]))->get();
            
            delayChanged = false;
        }
        
        float dLevelL = ((juce::AudioParameterFloat *)parameters.getParameter(PecoParameterID[DistControlL]))->get();
        float dLevelR = ((juce::AudioParameterFloat *)parameters.getParameter(PecoParameterID[DistControlR]))->get();
        
        // get the original input signal
        float inputL = buffer.getSample(0, n);
        float inputR = (numOutputs == 2) ? buffer.getSample(1, n) : inputL;
        
        // get the summed delay
        float summedDelayL = (DDL[0][delayInxLow] + DDL[0][delayInxMid] + DDL[0][delayInxHigh]) / 3.0f;
        float summedDelayR = (DDL[1][delayInxLow] + DDL[1][delayInxMid] + DDL[1][delayInxHigh]) / 3.0f;
        
        // apply distortion
        float distL = hard_limit(summedDelayL * dLevelL);
        float distR = hard_limit(summedDelayR * dLevelR);
        
        // write the wet + dry signal to the output
        channelDataL[n] = (inputL * dryL) + (distL * wetL);
        if (numOutputs == 2) {
            channelDataR[n] = (inputR * dryR) + (distR * wetR);
        }
        
        // update the DDL
        // get the output of the DDL
        float dL = distL;
        float dR = distR;
        
        // add the fed back DDL and the original input
        // be sure to scale them so the value will be between -1 and 1
        DDL[0][writeInx] = (inputR + (dR * fbR)) / 2.0f;
        DDL[1][writeInx] = (inputL + (dL * fbL)) / 2.0f;
        
        // increment the read/write index
        writeInx++;
        if (writeInx >= D) {
            writeInx = 0;
        }
        
        delayInxLow++;
        if (delayInxLow >= D) {
            delayInxLow = 0;
        }
        
        delayInxMid++;
        if (delayInxMid >= D) {
            delayInxMid = 0;
        }
        
        delayInxHigh++;
        if (delayInxHigh >= D) {
            delayInxHigh = 0;
        }
    }
}

//==============================================================================
bool PecoAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PecoAudioProcessor::createEditor()
{
    return new PecoAudioProcessorEditor (*this);
}

//==============================================================================
void PecoAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PecoAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PecoAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout PecoAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;
    
    for (int i = 0; i < TotalNumParameters - 4; i++)
    {
        params.add(std::make_unique<juce::AudioParameterFloat>(PecoParameterID[i],
                                                               PecoParameterID[i],
                                                               juce::NormalisableRange<float>(PecoParameterMin[i], PecoParameterMax[i]),
                                                               PecoParameterDefault[i],
                                                               PecoParameterID[i]));
    }
    
    params.add(std::make_unique<juce::AudioParameterInt>(PecoParameterID[Delay],
                                                         PecoParameterID[Delay],
                                                         MIN_DELAY,
                                                         MAX_DELAY,
                                                         ONE_SECOND,
                                                         PecoParameterID[Delay]));
    params.add(std::make_unique<juce::AudioParameterInt>(PecoParameterID[DistType],
                                                         PecoParameterID[DistType],
                                                         0,
                                                         1,
                                                         0,
                                                         PecoParameterID[DistType]));
    params.add(std::make_unique<juce::AudioParameterInt>(PecoParameterID[Delay2],
                                                         PecoParameterID[Delay2],
                                                         MIN_DELAY,
                                                         MAX_DELAY,
                                                         ONE_SECOND,
                                                         PecoParameterID[Delay2]));
    params.add(std::make_unique<juce::AudioParameterInt>(PecoParameterID[Delay3],
                                                         PecoParameterID[Delay3],
                                                         MIN_DELAY,
                                                         MAX_DELAY,
                                                         ONE_SECOND,
                                                         PecoParameterID[Delay3]));
    
    
    return params;
}

float PecoAudioProcessor::hard_limit(float input)
{
    return std::fminf(std::fmaxf(input, -1), 1);
}
