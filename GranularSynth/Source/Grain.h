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
   *\param: int StartingSample - Sample to Start the Grain
   *\param: int Duration - Duration of the Grain
   */
  GrainCloud(int startingSample_ = 1, int duration = 0);

  /**
   *\Grain () operator
   *\brief: Plays a sample of the Grains (from WAV form)
   *\param: int channel - Channel of an Audio Buffer to Play Sample From
   *\return: float - Current Sample of the Grain
   */
  float operator()(int channel);

  /**
   *\fn: SetStartingSample
   *\brief: Sets the Starting Sample of a Grain
   *\param: int startingSample - Sample to Start the Grain
   */
  void SetCentroidSample(int startingSample);

  /*
   *\fn: GetStartingSample
   *\brief: Gets the Starting Sample of a Grain
   *\return: int - Sample where the grain starts
   */
  int GetCentroidSample() { return mCentroidSample; }

  /**
   *\fn: SetDuration
   *\brief: Sets the Duration of the Grain, and updates the Sample Delta and Ending Sample
   *\param: int duration - duration (in ms) of the Grain
   */
  void SetDuration(int duration);

  /**
   *\fn: SetAudioSource
   *\brief: Sets the Audio Source of the Grain File and Updates The Grain's Properties
   *\param: AudioFormatReader& audioReader - Reader Containing The New Audio File
   */
  void SetAudioSource(AudioFormatReader& audioReader);

  /**
   *\fn: SetCloudSize
   *\brief: Sets the Cloud Size
   *\param: int size - number of grains in the cloud
   */
  void SetCloudSize(int size);
  

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
   *\return: Boolean - Valid WAV file
   */
  bool HasValidWAVFile()
  {
    return mHasValidWAVFile;
  }

  //! Boolean for if the Grain is currently being played
  bool mIsPlaying = false;

   //! Boolean for Determining if we are Randomly Panning the Granular Synth
  bool mRandomPanning = false;
  
  //! Offset of where a Grain can start from
  int mStartingOffset = 0; 

  //! Pitch Offset (in semitones)
  double mPitchOffsetMin = 0.0;
  double mPitchOffsetMax = 0.0;

  //! Pitch Offset Level is used to Properly Generate Random Doubles in the valid range.
  double mPitchOffsetLevel = 0.0; 

  //! Gain Offset for a Individual Grain (in dB)
  int mGainOffsetDbMin = 0;
  int mGainOffsetDbMax = 0;

  //! Scalar Value for the Global Gain of the Cloud (gain value)
  double mGlobalGain = 1.0;   
  
  //! Scalar Value for the Release Duration of the ADSR (in ms)
  double mEnvelopeRelease = 100.0;

  //! Sampling Rate of the Active Grain
  double mSamplingRate = 0.0;

private:
  //================================VARIABLES=====================================
  

  //! AudioSampleBuffer of the Grain's Audio Source WAV file.
  std::unique_ptr<AudioSampleBuffer> mAudioSourceBuffer; 

  /**
   *\struct: GrainData
   *\author: AnthonyBrigante
   *\brief:
      The GrainData Struct contains data about a specific grain inside
      a grain cloud. GrainData is used and managed by the GrainCloud class.
   */
  struct GrainData
  {
    //! Current Playing Sample of a Grain
    double mCurrentSample[2] = { 0.0, 0.0 }; 

    int mStartingSample = 0;     //!< Actual Starting Sample for a Specific Grain
    int mEndSample = 0;          //!< Ending Sample of a Grain
    
    double mPitchScalar = 1.0f;  //!< Scalar Value for a Randomized Pitch Offset
    double mGainScalar = 1.0f;   //!< Scalar Value for a Randomized Gain Offset (gain value)
    double mPanningValue = 1.0f; //!< Scalar Value for a Random Pan
    
    bool mInRelease = true;     //!< Boolean for whether or not the Grain needs to be replayed.

    juce::ADSR envelope;    //!< ADSR Envelope for a Grain

  };

  //! Vector Containing the Grains in the Grain Cloud
  std::vector<GrainData> grains; 

  int mCloudSize = 0;       //!< Size of the Grain Cloud (Number of Grains to Play)

  int mCentroidSample = 1;  //!< The Centroid Sample for a Grain Cloud
  int mDuration = 0;        //!< Duration (in ms) of a Grain Cloud
  int mSampleDelta = 0;     //!< Delta Between the Starting Sample and the Ending Sample (determined by Duration)


  int mWaveSize = 0;        //!< The Size of the Audio Waveform being used

  bool mHasValidWAVFile;    //!< Boolean for whether or not the Grain has a valid WAV file.

  //================================FUNCTIONS=====================================

  /**
   *\fn: RandomizeGrain
   *\brief: Randomizes a Grain
   *\param: GrainData& grain - Grain Parameters to Randomize
   */
  void RandomizeGrain(GrainData& grain);

  /**
   *\fn: AddGrains
   *\brief: Adds Grains from the GrainCloud
   *\param: int count - number of grains to add (default - 1)
   */
  void AddGrains(int count = 1);

  /**
   *\fn: RemoveGrains
   *\brief: Removes Grains from the GrainCloud
   *\param: int count - number of grains to remove (default - 1)
   */
  void RemoveGrains(int count = 1);

};
