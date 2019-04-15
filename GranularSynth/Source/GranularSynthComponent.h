/**************************************************************************
/*!
\file GranularSynthComponent.h
\author Anthony Brigante
\brief
  The Base Synth Component Class. This Describes the Runtime Logic
  of the Base Audio App Component.

**************************************************************************/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Grain.h"

/**
 *\class: GranularSynthComponent
 *\author: Anthony Brigante
 *\brief:
    The GranularSynthComponent is the Core Class that Manages the Entire GranularSynth
 */
class GranularSynthComponent   : public AudioAppComponent,
                                 public Slider::Listener 
{ 
public:
    //==============================================================================
    
    /**
     *\GranularSynthComponent constructor
     *\brief: Constructs the Granular Synth Component
     */
    GranularSynthComponent();
    
    /**
      *\GranularSynthComponent Destructor
      *\brief: Destructs the GranularSynthComponent
      */
    ~GranularSynthComponent();
    
    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void sliderValueChanged(Slider* slider) override;
    void updateToggleValue(Button* button);

    //==============================================================================
    void resized() override;

private:
    //================================VARIABLES=====================================

    //----- GUI Objects -----//
    
    TextButton mOpenFileButton;    //<! Button Used to Open an Audio File
    TextButton mPlayButton;        //<! Button Used to Play an opened Audio File
    TextButton mStopButton;        //<! Button Used to Stop an Opened Audio File

    Label mCentroidSampleLabel;    //<! Label Noting the Centroid Sample Slider
    Slider mCentroidSampleSlider;  //<! Slider Designating the Starting Sample of a Grain
    
    Label mGrainDurationLabel;     //<! Label Noting the Duration Slider
    Slider mGrainDurationSlider;   //<! Slider Designating the Duration of a Grain
    
    Label mStartingOffsetLabel;    //<! Label Noting the Starting Offset Slider
    Slider mStartingOffsetSlider;  //<! Slider Designating the Offset a Grain can use.

    Label mCloudSizeLabel;         //<! Label Noting the Cloud Size Slider
    Slider mCloudSizeSlider;       //<! Slider Designating the Cloud Size of a Grain

    // --- Pitch Offset --- //
    Label mPitchOffsetLabel;       //<! Label Noting the Pitch Offset Sliders
    
    Label mPitchOffsetMinLabel;    //<! Label Noting the Pitch Offset Min Slider
    Slider mPitchOffsetMinSlider;  //<! Slider Designating the Minimum Pitch Offset Value
    
    Label mPitchOffsetMaxLabel;    //<! Label Noting the Pitch Offset Max Slider
    Slider mPitchOffsetMaxSlider;  //<! Slider Designating the Maximium Pitch Offset Value
    
    // --- Global Gain --- //

    Label mGlobalGainLabel;        //<! Label Noting the Global Gain
    Slider mGrainCloudGainSlider;  //<! Slider Designating the Gain for an Entire Cloud

    // --- Random Gain Offset --- //

    Label mGrainGainMinLabel;      //<! Label Noting the Min Grain Gain Slider
    Slider mGrainGainMinSlider;    //<! Slider Noting the Min Grain Gain Offset

    Label mGrainGainMaxLabel;      //<! Label Noting the Max Grain Gain Slider
    Slider mGrainGainMaxSlider;    //<! Slider Noting the Min Grain Gain Offset

    // --- MISC --- //

    Slider mGrainReleaseSlider;    //<! Slider Designating The Release Value of the Grain ADSR
    ToggleButton mRandomPanning;   //<! ToggleButton Determining if we are to Randomly Pan the Grain
   

    //----- Data ------//

    /**
     *\enum TransportState
     *\brief Describes Audio Playback States
     */
    enum TransportState
    {
        STOPPED,
        STARTING,
        PLAYING,
        STOPPING
    };

    AudioFormatManager mFormatManager;  //<! Manages Valid Audio Formats
    TransportState state;               //<! Determines the Current Audio Playback State

    std::vector<GrainCloud> mGrains;   //<! Vector of Grains 
    GrainCloud activeGrain;            //<! Reference to the Current Active grain. 

    //================================FUNCTIONS=====================================
     
    /**
     *\fn: changeState
     *\brief: Changes the Current Playback State
     *\param: State to transition to
     */
    void changeState(TransportState newState);

    /**
     *\fn: openFile
     *\brief: Open and Loads a Selected Audio File
     */
    void openFile();

    /**
     *\fn: playFile
     *\brief: Plays a Selected Audio File
     */
    void playFile();

    /**
     *\fn: stopFile
     *\brief: Stops a Selected Audio File
     */
    void stopFile();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GranularSynthComponent)
};
