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
                                 public ChangeListener
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

    //==============================================================================
    void resized() override;

    void changeListenerCallback(ChangeBroadcaster* source) override;

private:
    //================================VARIABLES=====================================

    //----- GUI Objects -----//
    
    TextButton mOpenFileButton;   //<! Button Used to Open an Audio File
    TextButton mPlayButton;       //<! Button Used to Play an opened Audio File
    TextButton mStopButton;       //<! Button Used to Stop an Opened Audio File

    Slider mStartingSample;       //<! Slider Designating the Starting Sample of a Grain
    Slider mGrainDuration;        //<! Slider Designating the Duration of a Grain

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

    AudioFormatManager mFormatManager;                      //<! Manages Valid Audio Formats
    std::unique_ptr<AudioFormatReaderSource> mReaderSource; //<! Reads an Audio Source File
    AudioTransportSource transportSource;                   //<! Controls Playback of the Audio Source File
    TransportState state;                                   //<! Determines the Current Audio Playback State

    std::vector<Grain> mGrains;   //<! Vector of Grains 
    Grain grainExample;           //<! Used to Test the features of a single grain. 

    WaveGenerator mWaveGenerator; //<! Generates Primative Waveforms

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
