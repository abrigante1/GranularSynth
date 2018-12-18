/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "GranularSynthComponent.h"
#include "Grain.h"

//==============================================================================
GranularSynthComponent::GranularSynthComponent() : waveGenerator(440.0, 48000.0), grainTest(WaveTableGenerator(), 0, 48000)
{
    // Setup the Frequency Slider and Enable it.
    startingSample.setRange (0, 48000.0);
    startingSample.setTextValueSuffix (" Sample");
    startingSample.setNumDecimalPlacesToDisplay(0);
    addAndMakeVisible (startingSample);

    endingSample.setRange (0, 48000.0);
    endingSample.setTextValueSuffix (" Sample");
    endingSample.setNumDecimalPlacesToDisplay(0);
    addAndMakeVisible (endingSample);
    
    // set size of the component
    setSize (800, 600);

    // specify the number of input and output channels that we want to open
    setAudioChannels (2, 2);
}

GranularSynthComponent::~GranularSynthComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void GranularSynthComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void GranularSynthComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
   grainTest.SetStartingSample(static_cast<int>(startingSample.getValue()));
   grainTest.SetEndSample(static_cast<int>(endingSample.getValue()));

    {
        auto* buffer1 = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
        auto* buffer2 = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);
     
        for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
          float waveSamp = grainTest();
          buffer1[sample] = waveSamp;
          buffer2[sample] = waveSamp;
        }
    }

    //bufferToFill.clearActiveBufferRegion();
}

void GranularSynthComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================

void GranularSynthComponent::resized()
{
    startingSample.setBounds (100, 10, getWidth() - 110, 20);
    endingSample.setBounds (100, 40, getWidth() - 110, 20);

}
