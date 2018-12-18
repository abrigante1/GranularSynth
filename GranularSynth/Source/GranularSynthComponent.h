/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Grain.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class GranularSynthComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    GranularSynthComponent();
    ~GranularSynthComponent();
    
    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
    Slider startingSample;
    Slider endingSample;
    Grain grainTest;
    WaveGenerator waveGenerator;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GranularSynthComponent)
};
