/**************************************************************************
/*!
\file Grain.h
\author Anthony Brigante
\brief
  Definition for the Grain Struct. 
  The Grain Class describes the properties of a 'grain'.

**************************************************************************/
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveGenerator.h"


/**
 *\class: GrainCloud
 *\author: Anthony Brigante
 *\brief:
    The GrainCloud Class Describes the Properties of a 'GrainCloud'.
    A GrainCloud Manages the Meta Data for a 'Grain'.

    A GrainCloud contains the following properties
      - A Source Audio File
      - Start Location in Source Audio File
      - End Location in Source Audio File
      - Grain Duration
 */
class GrainCloud
{

public:

  /**
	 *\Grain constructor
	 *\brief: Generates a Grain from a waveform generator
   *\param: WaveTable - Wave Table used to Generate a Waveform
   *\param: StartingSample - Sample to Start the Grain 
   *\param: Duration - Duration of the Grain
	 */
  GrainCloud(int startingSample_, int duration);

  /** 
   *\Grain () operator
   *\brief: Plays a sample of the Grains (from WAV form)
   *\param: Channel of an Audio Buffer to Play Sample From
   *\return: Current Sample of the Grain
   */
  float operator()(int channel);

  /** 
   *\Grain () operator
   *\brief: Plays a sample of the Grains (From WaveGenerator)
   *\return: Current Sample of the Grain
   *\DEPRECATED (for now)
   */
  //float operator()();

  /**
   *\fn: SetStartingSample
   *\brief: Sets the Starting Sample of a Grain
   *\param: StartingSample - Sample to Start the Grain
   */
  void SetCentroidSample(int startingSample);

  /**
   *\fn: SetDuration
   *\brief: Sets the Duration of the Grain, and updates the Sample Delta and Ending Sample
   *\param: Duration - Duration (in ms) of the Grain
   */
  void SetDuration(int duration);

  /**
   *\fn: SetAudioSource
   *\brief: Sets the Audio Source of the Grain File and Updates The Grain's Properties
   *\param: audioReader - Reader Containing The New Audio File
   */
  void SetAudioSource(AudioFormatReader& audioReader);

  /**
   *\fn: GetSize
   *\brief: Get's the Size of the Grain in Samples
   *\return: int - Size of the Grain in Samples
   */
  int GetSize() {  return mWaveSize; };

  /**
   *\fn: Reset
   *\brief: Resets the Grain to its inital state
   */
  void Reset();

  /**
   *\fn: HasValidWAVFile
   *\brief: Checks if the Grain Has a Valid WAV File
   *\return: Boolean for Valid WAV file
   */
  bool HasValidWAVFile()
  {
    return mHasValidWAVFile;
  }

  //! Boolean for if the Grain is currently being played
  bool mIsPlaying = false;

  //!< Size of the Grain Cloud (Number of Grains to Play)
  int mCloudSize;      

  //!< Offset of where a Grain can start from
  int mStartingOffset;

private:
  //================================VARIABLES=====================================
  

  //! AudioSampleBuffer of the Grain's Audio Source WAV file.
  std::unique_ptr<AudioSampleBuffer> mAudioSourceBuffer; 

  struct GrainData
  {
  int mCurrentSample[2]; //!< Current Playing Sample of a Grain
  int mStartingSample;   //!< Actual Starting Sample for a Specific Grain
  int mEndSample;        //!< Ending Sample of a Grain
  bool mIsFinished;      //!< Boolean for whether or not the Grain needs to be replayed.
  };

  int mCurrentSample[2]; //!< Current Playing Sample of a Grain
  int mStartingSample;   //!< Actual Starting Sample for a Specific Grain
  int mEndSample;        //!< Ending Sample of a Grain
  bool mIsFinished;      //!< Boolean for whether or not the Grain needs to be replayed.

  //!< Vector Containing the Grains in the Grain Cloud
  std::vector<GrainData> grains; 

  int mCentroidSample;   //!< The Centroid Sample for a Grain Cloud
  int mDuration;         //!< Duration (in ms) of a Grain Cloud
  int mSampleDelta;      //!< Delta Between the Starting Sample and the Ending Sample (determined by Duration)

  int mWaveSize = 0;     //!< The Size of the Audio Waveform being used
  int mSamplingRate;     //!< Sampling Rate of the Audio used by the Grain Cloud

  bool mHasValidWAVFile; //!< Boolean for whether or not the Grain has a valid WAV file.

  //================================FUNCTIONS=====================================

  /**
   *\fn: GetRandomStartingSample
   *\brief: Gets a Random Starting Sample Based off the CloudRange and the Centroid Sample
   *\return: int - random starting sample
   */
  int GetRandomStartingSample();

};


