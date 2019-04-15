/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

// async vs sync granular synthesis

#include "GranularSynthComponent.h"
#include "Grain.h"

//==============================================================================
GranularSynthComponent::GranularSynthComponent() : activeGrain()
{

        //------ CENTROID SAMPLE -------//
    
    // Label
    addAndMakeVisible(mCentroidSampleLabel);
    mCentroidSampleLabel.setText("Centroid Sample Location", dontSendNotification);
    mCentroidSampleLabel.attachToComponent(&mCentroidSampleSlider, false);
    mCentroidSampleLabel.setJustificationType(Justification::centred);

    // Slider
    mCentroidSampleSlider.setRange (1, 2);
    mCentroidSampleSlider.setTextValueSuffix (" Sample");
    mCentroidSampleSlider.setNumDecimalPlacesToDisplay(0);
    mCentroidSampleSlider.addListener(this);
    addAndMakeVisible (mCentroidSampleSlider);
  
        //------ GRAIN DURATION -------//

    // Label
    addAndMakeVisible(mGrainDurationLabel);
    mGrainDurationLabel.setText("Grain Duration", dontSendNotification);
    mGrainDurationLabel.attachToComponent(&mGrainDurationSlider, false);
    mGrainDurationLabel.setJustificationType(Justification::centred);

    // Slider
    mGrainDurationSlider.setRange (1, 1000);
    mGrainDurationSlider.setTextValueSuffix (" ms");
    mGrainDurationSlider.setNumDecimalPlacesToDisplay(0);
    mGrainDurationSlider.addListener(this);
    addAndMakeVisible (mGrainDurationSlider);

        //------ CLOUD SIZE -------//

    // Label
    addAndMakeVisible(mCloudSizeLabel);
    mCloudSizeLabel.setText("Cloud Size", dontSendNotification);
    mCloudSizeLabel.attachToComponent(&mCloudSizeSlider, false);
    mCloudSizeLabel.setJustificationType(Justification::centred);

    // Slider
    mCloudSizeSlider.setRange (1, 10);
    mCloudSizeSlider.setTextValueSuffix (" grains");
    mCloudSizeSlider.setNumDecimalPlacesToDisplay(0);
    mCloudSizeSlider.addListener(this);
    addAndMakeVisible (mCloudSizeSlider);

        //------ STARTING OFFSET -------//

    // Label
    addAndMakeVisible(mStartingOffsetLabel);
    mStartingOffsetLabel.setText("Random Sample Offset", dontSendNotification);
    mStartingOffsetLabel.attachToComponent(&mStartingOffsetSlider, false);
    mStartingOffsetLabel.setJustificationType(Justification::centred);

    // Slider
    mStartingOffsetSlider.setRange (0, 10000);
    mStartingOffsetSlider.setTextValueSuffix (" samples");
    mStartingOffsetSlider.setNumDecimalPlacesToDisplay(0);
    mStartingOffsetSlider.addListener(this);
    addAndMakeVisible (mStartingOffsetSlider);

        //------ PITCH OFFSET -------//

    // Min Label
    addAndMakeVisible(mPitchOffsetMinLabel);
    mPitchOffsetMinLabel.setText("Minimum Pitch Offset", dontSendNotification);
    mPitchOffsetMinLabel.attachToComponent(&mPitchOffsetMinSlider, false);
    mPitchOffsetMinLabel.setJustificationType(Justification::centred);

    // Min Slider
    mPitchOffsetMinSlider.setRange (-24, 24);
    mPitchOffsetMinSlider.setTextValueSuffix (" semitones");
    mPitchOffsetMinSlider.setNumDecimalPlacesToDisplay(2);
    mPitchOffsetMinSlider.addListener(this);
    addAndMakeVisible (mPitchOffsetMinSlider);

    // Max Label
    addAndMakeVisible(mPitchOffsetMaxLabel);
    mPitchOffsetMaxLabel.setText("Maximum Pitch Offset", dontSendNotification);
    mPitchOffsetMaxLabel.attachToComponent(&mPitchOffsetMaxSlider, false);
    mPitchOffsetMaxLabel.setJustificationType(Justification::centred);

    // Max Slider
    mPitchOffsetMaxSlider.setRange (-24, 24);
    mPitchOffsetMaxSlider.setTextValueSuffix (" semitones");
    mPitchOffsetMaxSlider.setNumDecimalPlacesToDisplay(2);
    mPitchOffsetMaxSlider.addListener(this);
    addAndMakeVisible (mPitchOffsetMaxSlider);

        //------ GLOBAL GAIN -------//

    // Label
    addAndMakeVisible(mGlobalGainLabel);
    mGlobalGainLabel.setText("Global Gain", dontSendNotification);
    mGlobalGainLabel.attachToComponent(&mGrainCloudGainSlider, false);
    mGlobalGainLabel.setJustificationType(Justification::centred);

    // Slider
    mGrainCloudGainSlider.setRange (-60, 0);
    mGrainCloudGainSlider.setTextValueSuffix (" dB");
    mGrainCloudGainSlider.setNumDecimalPlacesToDisplay(0);
    mGrainCloudGainSlider.addListener(this);
    addAndMakeVisible (mGrainCloudGainSlider);

        //------ GRAIN GAIN OFFSET -------//


    // Min Label
    addAndMakeVisible(mGrainGainMinLabel);
    mGrainGainMinLabel.setText("Grain Gain Minimum", dontSendNotification);
    mGrainGainMinLabel.attachToComponent(&mGrainGainMinSlider, false);
    mGrainGainMinLabel.setJustificationType(Justification::centred);

    // Min Slider
    mGrainGainMinSlider.setRange (-60, 0);
    mGrainGainMinSlider.setTextValueSuffix (" dB");
    mGrainGainMinSlider.setNumDecimalPlacesToDisplay(0);
    mGrainGainMinSlider.addListener(this);
    addAndMakeVisible (mGrainGainMinSlider);

    // Max Label
    addAndMakeVisible(mGrainGainMaxLabel);
    mGrainGainMaxLabel.setText("Grain Gain Maximum", dontSendNotification);
    mGrainGainMaxLabel.attachToComponent(&mGrainGainMaxSlider, false);
    mGrainGainMaxLabel.setJustificationType(Justification::centred);

    // Max Slider
    mGrainGainMaxSlider.setRange (-60, 0);
    mGrainGainMaxSlider.setTextValueSuffix (" dB");
    mGrainGainMaxSlider.setNumDecimalPlacesToDisplay(0);
    mGrainGainMaxSlider.addListener(this);
    addAndMakeVisible (mGrainGainMaxSlider);

    // Release Value of the Grain ADSR
    /*mGrainReleaseSlider.setRange(0, 100);
    mGrainReleaseSlider.setTextValueSuffix( " ms");
    mGrainReleaseSlider.setNumDecimalPlacesToDisplay(0);
    mGrainReleaseSlider.addListener(this);
    addAndMakeVisible (mGrainReleaseSlider);*/

    // Random Panning Boolean
    addAndMakeVisible(mRandomPanning);
    mRandomPanning.onClick = [this] { updateToggleValue(&mRandomPanning); };
    mRandomPanning.setButtonText("Random Panning?");


    // ----- FILE I/O ----- //

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
  // Centroid Sample
  if (slider == &mCentroidSampleSlider)
    activeGrain.SetCentroidSample(static_cast<int>(mCentroidSampleSlider.getValue()));

  // Gain Duration
  else if (slider == &mGrainDurationSlider)
    activeGrain.SetDuration(static_cast<int>(mGrainDurationSlider.getValue()));

  // Starting Offset
  else if (slider == &mStartingOffsetSlider)
    activeGrain.mStartingOffset = static_cast<int>(mStartingOffsetSlider.getValue());

  // Cloud Size
  else if (slider == &mCloudSizeSlider)
    activeGrain.SetCloudSize(static_cast<int>(mCloudSizeSlider.getValue())); 

  // Pitch Offset Min
  else if (slider == &mPitchOffsetMinSlider)
    activeGrain.mPitchOffsetMin = mPitchOffsetMinSlider.getValue();

  // Pitch Offset Max
  else if (slider == &mPitchOffsetMaxSlider)
    activeGrain.mPitchOffsetMax = mPitchOffsetMaxSlider.getValue();
  
  // Grain Cloud Gain
  else if(slider == &mGrainCloudGainSlider)
    activeGrain.mGlobalGain = Decibels::decibelsToGain<double>(mGrainCloudGainSlider.getValue());
  
  // Grain Gain Min Offset
  else if(slider == &mGrainGainMinSlider)
    activeGrain.mGainOffsetDbMin = static_cast<int>(mGrainGainMinSlider.getValue());

  // Grain Gain Max Offset
  else if(slider == &mGrainGainMaxSlider)
    activeGrain.mGainOffsetDbMax = static_cast<int>(mGrainGainMaxSlider.getValue());

  // Release Slider
  else if(slider == &mGrainReleaseSlider)
    activeGrain.mEnvelopeRelease = (mGrainReleaseSlider.getValue() / 1000.0);

}

void GranularSynthComponent::updateToggleValue(Button* button)
{
  if(button == &mRandomPanning)
    activeGrain.mRandomPanning = button->getToggleState();
}

//==============================================================================

void GranularSynthComponent::resized()
{
    int yValue = 30;
    int yValueOffset = 50;
    auto xValue = 50;
    auto halfWidth = (getWidth() / 2) - 30;
    
    // Centroid Sample
    mCentroidSampleSlider.setBounds (xValue, (yValue), getWidth() - xValue - 10, 20);
    
    // Grain Duration
    mGrainDurationSlider.setBounds  (xValue, (yValue += yValueOffset), getWidth() - xValue - 10, 20);
    
    // Cloud Size
    mCloudSizeSlider.setBounds      (xValue, (yValue += yValueOffset), getWidth() - xValue - 10, 20);
    
    // Offset
    mStartingOffsetSlider.setBounds (xValue, (yValue += yValueOffset), getWidth() - xValue - 10, 20);
    
    // Pitch Offset
    mPitchOffsetMinSlider.setBounds (xValue, (yValue += yValueOffset), halfWidth, 20);
    mPitchOffsetMaxSlider.setBounds (mPitchOffsetMinSlider.getRight(), yValue, halfWidth, 20);
    
    // Grain Cloud
    mGrainCloudGainSlider.setBounds (xValue, (yValue += yValueOffset), getWidth() - xValue - 10, 20);

    // Grain Gain
    mGrainGainMinSlider.setBounds   (xValue, (yValue += yValueOffset), halfWidth, 20);
    mGrainGainMaxSlider.setBounds   (mGrainGainMinSlider.getRight(), yValue, halfWidth, 20);
   
    // Random Panning
    mRandomPanning.setBounds        (xValue, (yValue += yValueOffset),getWidth() - xValue - 10, 20);

    // Buttons
    mOpenFileButton.setBounds (10, (yValue += 40), getWidth() - 20, 20);
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
  // Set the Grain to No Longer be Playing
  activeGrain.mIsPlaying = false;
  
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
      mCentroidSampleSlider.setValue(1);
      mGrainDurationSlider.setValue(1);
      mCloudSizeSlider.setValue(1);
      mStartingOffsetSlider.setValue(0);
      mPitchOffsetMinSlider.setValue(0);
      mPitchOffsetMaxSlider.setValue(0);
      mGrainCloudGainSlider.setValue(0);
      mGrainGainMinSlider.setValue(0);
      mGrainGainMaxSlider.setValue(0);

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
