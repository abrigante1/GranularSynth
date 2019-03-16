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
 *\class: Grain
 *\author: Anthony Brigante
 *\brief:
    The Grain Class Describes the Properties of a 'grain' 

    A grain contains the following properties
      - (TO DO) Source Audio File or Source Waveform 
      - Start Location in Source Audio File / Waveform
      - End Location in Source Audio File / Waveform
      - (TO DO) Envelope
      - (TO DO) Grain Weight 
      - Grain Duration
 */
class Grain
{

public:

  /**
	 *\Grain constructor
	 *\brief: Generates a Grain
   *\param: WaveTableGenerator - Wave Table used to Generate a Waveform
   *\param: StartingSample - Sample to Start the Grain 
   *\param: Duration - Duration of the Grain
	 */
  Grain(WaveTableGenerator waveTable, int startingSample_, int duration);

  /** 
   *\Grain () operator
   *\brief: Plays a sample of the Grain
   *\return: Current Sample of the Grain
   */
  float operator()(void);

  /**
   *\fn: SetStartingSample
   *\brief: Sets the Starting Sample of a Grain
   *\param: StartingSample - Sample to Start the Grain
   */
  void SetStartingSample(int startingSample);

  /**
   *\fn: SetDuration
   *\brief: Sets the Duration of the Grain, and updates the Sample Delta and Ending Sample
   *\param: Duration - Duration (in ms) of the Grain
   */
  void SetDuration(int duration);

private:

  //! Pointer to the WaveTable for the Grain (null if not used)
  WaveTableGenerator waveTable;
  
  int mCurrentSample;   //!< Current Playing Sample of a Grain
  int mStartingSample;  //!< Starting Sample of a Grain
  int mEndSample;       //!< Ending Sample of a Grain
  int mDuration;        //!< Duration (in ms) of a Grain
  int mSampleDelta;     //!< Delta Between the Starting Sample and the Ending Sample (determined by Duration)

  int mSamplingRate;    //!< Sampling Rate of the Audio used by the Grain

  bool mIsFinished;     //!< Boolean for whether or not the Grain needs to be replayed.
};
