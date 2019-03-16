/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "GranularSynthComponent.h"
#include "Grain.h"

//==============================================================================
GranularSynthComponent::GranularSynthComponent() : mWaveGenerator(440.0, 48000.0, SQUARE), grainExample(WaveTableGenerator(SQUARE), 0, 48000)
{
    // Setup the Frequency Slider and Enable it.
    mStartingSample.setRange (1, mWaveGenerator.getSamplingRate());
    mStartingSample.setTextValueSuffix (" Sample");
    mStartingSample.setNumDecimalPlacesToDisplay(0);
    addAndMakeVisible (mStartingSample);

    mGrainDuration.setRange (0, 100);
    mGrainDuration.setTextValueSuffix (" ms");
    mGrainDuration.setNumDecimalPlacesToDisplay(0);
    addAndMakeVisible (mGrainDuration);

    addAndMakeVisible(&mOpenFileButton);
    mOpenFileButton.setButtonText("Open File...");
    mOpenFileButton.onClick = [this] { openFile(); };

    addAndMakeVisible(&mPlayButton);
    mPlayButton.setButtonText("PLay File");
    mPlayButton.onClick = [this] { playFile(); };
    mPlayButton.setColour(TextButton::buttonColourId, Colours::green);
    mPlayButton.setEnabled(false);

    addAndMakeVisible(&mStopButton);
    mStopButton.setButtonText("Stop File");
    mStopButton.onClick = [this] { stopFile(); };
    mStopButton.setColour(TextButton::buttonColourId, Colours::red);
    mStopButton.setEnabled(false);



    // set size of the component
    setSize (800, 600);

    // Register the Audio File Reader
    mFormatManager.registerBasicFormats();
    transportSource.addChangeListener(this);

    // specify the number of input and output channels that we want to open
    setAudioChannels (0, 2);
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
    sampleRate = 0;
    samplesPerBlockExpected = 0;
}

void GranularSynthComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
   grainExample.SetStartingSample(static_cast<int>(mStartingSample.getValue() - 1));
   grainExample.SetDuration(static_cast<int>(mGrainDuration.getValue()));

    for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
    {
        // Get a pointer to the start sample in the buffer for this audio output channel
        auto* buffer = bufferToFill.buffer->getWritePointer (channel, bufferToFill.startSample);
        // Fill the required number of samples with noise between -0.125 and +0.125
        for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
          float test = grainExample();
          buffer[sample] = test;
        }
    }
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
    mStartingSample.setBounds (100, 10, getWidth() - 110, 20);
    mGrainDuration.setBounds (100, 40, getWidth() - 110, 20);

    mOpenFileButton.setBounds (10, 10, getWidth() - 20, 20);
    mPlayButton.setBounds (10, 40, getWidth() - 20, 20);
    mStopButton.setBounds (10, 70, getWidth() - 20, 20);

}

void GranularSynthComponent::changeListenerCallback(ChangeBroadcaster* source)
{
  // Update the Transport State based on the Transport Source
  if(source == &transportSource)
  {
    if(transportSource.isPlaying())
      changeState(TransportState::PLAYING);
    else
      changeState(TransportState::STOPPED);
  }
}

//==============================================================================

void GranularSynthComponent::changeState(TransportState newState)
{
  if (state != newState)
  {
    state = newState;

    switch (state)
    {
      case TransportState::STOPPED:
        mStopButton.setEnabled (false);
        mPlayButton.setEnabled (true);
        transportSource.setPosition (0.0);
        break;

      case TransportState::STARTING:
        mPlayButton.setEnabled (false);
        transportSource.start();
        break;

      case TransportState::PLAYING:
        mStopButton.setEnabled (true);
        break;

      case TransportState::STOPPING:
        transportSource.stop();
        break;
    }
  }
}

void GranularSynthComponent::openFile()
{
  FileChooser chooser("Select a WAV file to play...", {}, "*.wav");
  
  // Open File Browser
  if (chooser.browseForFileToOpen())
  {
    auto file = chooser.getResult();
    auto* reader = mFormatManager.createReaderFor(file);

    if (reader != nullptr)
    {
      // Create a New AudioFormatReader
      std::unique_ptr<AudioFormatReaderSource> newSource (new AudioFormatReaderSource(reader, true));

      // Load the Source into the Transport
      transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);

      // Enable the PlayButton
      mPlayButton.setEnabled(true);

      // Reset the Main Reader Source
      mReaderSource.reset(newSource.release());
    }

  }
}

void GranularSynthComponent::playFile()
{
  changeState(TransportState::STARTING);
}

void GranularSynthComponent::stopFile()
{
  changeState(TransportState::STOPPING);
}
