/**************************************************************************
filename Grain.h
author Anthony Brigante
Brief Description:
  Definition for the Grain Struct. 
  The Grain Class describes the properties of a 'grain'.

  A grain contains the following properties
    - Source Audio File or Source Waveform
    - Start Location in Source Audio File / Waveform
    - End Location in Source Audio File / Waveform
    - Envelope
    - Grain Weight
    - Grain Duration
**************************************************************************/
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveGenerator.h"


class Grain
{

public:

  Grain(WaveTableGenerator& waveTable, int startingSample_, int endSample_);

  // Generate Grain Sample
  float operator()(void);

  void SetStartingSample(int newStartingSample)
  { startingSample = newStartingSample; }

  void SetEndSample(int newEndSample)
  { endSample = newEndSample; }


  float weight;

private:

  // Pointer to the WaveTable for the Grain (null if not used)
  WaveTableGenerator waveTable;
  
  int currentSample;
  int startingSample;
  int endSample;
  int samplingRate;

  bool isFinished;
};
