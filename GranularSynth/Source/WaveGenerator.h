/**************************************************************************
filename WaveGenerator.h
author Anthony Brigante
Brief Description:
  Implementation and Description File for a Waveform Generator and a
  WaveTableGenerator
  
  WaveGenerator: 
    Generates a Sine Wave with modifiable frequency, pitch offset
    and volume.

  WaveTableGenerater:
    Given a WaveGenerator, generates and stores a WaveTable of R samples.
    (Where R is the Sampling Rate of the WaveGenerator)
**************************************************************************/

#pragma once
#define _USE_MATH_DEFINES 
#include <math.h>
#include <vector>


// WaveGenerator class - Generates a Sine Wave with modifiable
// frequency, pitchOffset, and volume.
class WaveGenerator
{
  public:

    // Creates an AudioData Class with an Inital Frequency and Sampling Rate
    WaveGenerator(double initialFrequency_, double samplingRate_) 
    {
      initialFrequency = initialFrequency_;
      currentFrequency = initialFrequency_;
      samplingRate = samplingRate_;
      increment = 2 * M_PI * currentFrequency / samplingRate;
      pitchOffset = 1.0; 
    }

    // Calculates and Returns the next sample in a sine wave
    double operator()(void)
    {
      float output = sin(data);
      output *= gain;

      // Clip Check
      if(output > 1.0f)
        output = 1.0f;
      else if(output < -1.0f)
        output = -1.0f;

      data += increment * pitchOffset;
      
      return output;
    }

    // Updates the Frequency of the Sine Wave
    void updateFrequency(double freq)
    {
      currentFrequency = freq;
      increment = 2 * M_PI * currentFrequency / samplingRate;
    }

    // Updates the Pitch Offset of the Sine Wave [-1200, 1200]
    void updatePitchOffset(double cents)
    {
      pitchOffset = pow(2.0, cents / 1200.0);
    }

    // Update Gain [-24, 24]
    void updateGain(double db)
    {
      gain = pow(10.0, db / 20.0);
    }

    double getInitalFrequency() 
    { return initialFrequency; }

    double getCurrentFrequency()
    { return currentFrequency * pitchOffset; }

    double getSamplingRate()
    { return samplingRate; }

  private:

    double initialFrequency,
           currentFrequency,
           pitchOffset = 1.0,
           gain = 1.0,
           samplingRate = 44100;
    double data = 0,
           increment = currentFrequency / samplingRate;

};


// WaveTableGenerator Class
// Generates a WaveTable from a WaveGenerator
class WaveTableGenerator
{

public:

  // Create a WaveTable
  WaveTableGenerator()
  {
    UpdateWaveTable(WaveGenerator(440.0, 48000.0));
  }

  WaveTableGenerator(WaveGenerator& wave)
  {
    UpdateWaveTable(wave);
  }

  // Update the WaveTable with a new Waveform
  void UpdateWaveTable(WaveGenerator& wave)
  {
    waveTable.reserve(wave.getSamplingRate());
    
    for(int i = 0; i < wave.getSamplingRate(); ++i)
      waveTable.push_back(wave());
  }

  // Index into the WaveArray
  // (Using Operator Overload for Read-only Access)
  float operator[](int index)
  {
    return waveTable[index];
  }
  
private:

  // WaveTable Object
  std::vector<float> waveTable;
};