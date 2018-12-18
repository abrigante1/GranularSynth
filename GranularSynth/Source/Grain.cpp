#include "Grain.h"

Grain::Grain(WaveTableGenerator& newWaveTable, int startingSample_, int endSample_) : 
  waveTable(newWaveTable),
  startingSample(startingSample_),
  endSample(endSample_),
  currentSample(startingSample_)
{}

// Generate Grain Sample
float Grain::operator()(void)
{ 
  // Restart the grain if we are using it after finishing.
  if (isFinished)
  {
    isFinished = false;
    currentSample = startingSample;
  }

  // Get the Current Sample from the WaveTable
  float sample = waveTable[currentSample++];

  // Check if we are finished getting the current grain
  if (currentSample >= endSample)
  {
    isFinished = true;
  }

  return sample;
}