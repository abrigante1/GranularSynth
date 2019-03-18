/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "GranularSynthComponent.h"
#include "Grain.h"

//==============================================================================
GranularSynthComponent::GranularSynthComponent() : activeGrain()
{
    // Centroid Sample Slider Setup
    mCentroidSampleSlider.setRange (1, 2);
    mCentroidSampleSlider.setTextValueSuffix (" Sample");
    mCentroidSampleSlider.setNumDecimalPlacesToDisplay(0);
    mCentroidSampleSlider.addListener(this);
    addAndMakeVisible (mCentroidSampleSlider);
  
    // Gain Duration Slider Setup
    mGrainDurationSlider.setRange (1, 1000);
    mGrainDurationSlider.setTextValueSuffix (" ms");
    mGrainDurationSlider.setNumDecimalPlacesToDisplay(0);
    mGrainDurationSlider.addListener(this);
    addAndMakeVisible (mGrainDurationSlider);

    // Cloud Size Slider Setup
    mCloudSizeSlider.setRange (1, 10);
    mCloudSizeSlider.setTextValueSuffix (" grains");
    mCloudSizeSlider.setNumDecimalPlacesToDisplay(0);
    mCloudSizeSlider.addListener(this);
    addAndMakeVisible (mCloudSizeSlider);

    // Starting Offset Slider Setup
    mStartingOffsetSlider.setRange (0, 10000);
    mStartingOffsetSlider.setTextValueSuffix (" samples");
    mStartingOffsetSlider.setNumDecimalPlacesToDisplay(0);
    mStartingOffsetSlider.addListener(this);
    addAndMakeVisible (mStartingOffsetSlider);

    // Pitch Offset Slider
    mPitchOffsetSlider.setRange (0, 24);
    mPitchOffsetSlider.setTextValueSuffix (" semitones");
    mPitchOffsetSlider.setNumDecimalPlacesToDisplay(0);
    mPitchOffsetSlider.addListener(this);
    addAndMakeVisible (mPitchOffsetSlider);

    // Open File Button
    mOpenFileButton.setButtonText("Open File...");
    mOpenFileButton.onClick = [this] { openFile(); };
    addAndMakeVisible(&mOpenFileButton);

    // Play Button
    mPlayButton.setButtonText("Play");
    mPlayButton.onClick = [this] { playFile(); };
    mPlayButton.setColour(TextButton::buttonColourId, Colours::green);
    mPlayButton.setEnabled(false);
    addAndMakeVisible(&mPlayButton);

    // Stop Button
    mStopButton.setButtonText("Stop");
    mStopButton.onClick = [this] { stopFile(); };
    mStopButton.setColour(TextButton::buttonColourId, Colours::red);
    mStopButton.setEnabled(false);
    addAndMakeVisible(&mStopButton);

    // set size of the component
    setSize (800, 600);

    // Register the Audio File Reader
    mFormatManager.registerBasicFormats();

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
   // Get the Next Audio Block if the Grain is Being Played
   if (activeGrain.mIsPlaying)
   {
      for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
      {
        // Get a pointer to the start sample in the buffer for this audio output channel
        auto* buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);

        // Fill the required number of samples with noise between -0.125 and +0.125
        for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
          buffer[sample] = activeGrain(channel);
        }
      }
   }
}

void GranularSynthComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

void GranularSynthComponent::sliderValueChanged(Slider * slider)
{
  if (slider == &mCentroidSampleSlider)
    activeGrain.SetCentroidSample(static_cast<int>(mCentroidSampleSlider.getValue()));
  else if (slider == &mGrainDurationSlider)
    activeGrain.SetDuration(static_cast<int>(mGrainDurationSlider.getValue()));
  else if (slider == &mStartingOffsetSlider)
    activeGrain.mStartingOffset = static_cast<int>(mStartingOffsetSlider.getValue());
  else if (slider == &mCloudSizeSlider)
    activeGrain.SetCloudSize(static_cast<int>(mCloudSizeSlider.getValue())); 
  else if(slider == &mPitchOffsetSlider)
    activeGrain.mPitchOffset = static_cast<int>(mPitchOffsetSlider.getValue());
}

//==============================================================================

void GranularSynthComponent::resized()
{
    int yValue = 10;
    mCentroidSampleSlider.setBounds (100, (yValue), getWidth() - 110, 20);
    mGrainDurationSlider.setBounds (100,  (yValue += 30), getWidth() - 110, 20);
    mCloudSizeSlider.setBounds (100, (yValue += 30), getWidth() - 110, 20);
    mStartingOffsetSlider.setBounds (100, (yValue += 30), getWidth() - 110, 20);
    mPitchOffsetSlider.setBounds(100, (yValue += 30), getWidth() - 110, 20);

    mOpenFileButton.setBounds (10, (yValue += 30), getWidth() - 20, 20);
    mPlayButton.setBounds (10, (yValue += 30), getWidth() - 20, 20);
    mStopButton.setBounds (10, (yValue += 30), getWidth() - 20, 20);
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
        break;

      case TransportState::STARTING:
        mPlayButton.setEnabled (false);
        activeGrain.mIsPlaying = true;
        changeState(TransportState::PLAYING);
        break;

      case TransportState::PLAYING:
        mStopButton.setEnabled (true);
        break;

      case TransportState::STOPPING:
        activeGrain.Reset();
        changeState(TransportState::STOPPED);
        break;
    }
  }
}

void GranularSynthComponent::openFile()
{
  // Close the Audio Thread While Opening A File
  shutdownAudio();
  
  FileChooser chooser("Select a WAV file to play...", {}, "*.wav;*.flac");
  
  // Open File Browser
  if (chooser.browseForFileToOpen())
  {
    auto file = chooser.getResult();
    auto* reader = mFormatManager.createReaderFor(file);

    // Load File into the Grain
    if (reader != nullptr)
    {
      // Enable the PlayButton
      mPlayButton.setEnabled(true);

      // Set the Active Grain's Source Audio File to the New Source
      activeGrain.SetAudioSource(*reader);

      // Update the Starting Sample Slider Range
      mCentroidSampleSlider.setRange (1, activeGrain.GetSize());
      mCentroidSampleSlider.setTextValueSuffix (" Sample");
      mCentroidSampleSlider.setNumDecimalPlacesToDisplay(0);

      // Reset the Values for all Sliders
      //mCentroidSampleSlider.setValue(1, dontSendNotification);
      //mGrainDurationSlider.setValue(1, dontSendNotification);
      //mStartingOffsetSlider.setValue(1, dontSendNotification);
      //mCloudSizeSlider.setValue(0, dontSendNotification);
      //mPitchOffsetSlider.setValue(0, dontSendNotification);

      // Turn Back on the Audio Thread
      setAudioChannels(0, reader->numChannels);

      // Clear the File Reader From Memory
      delete reader;
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
