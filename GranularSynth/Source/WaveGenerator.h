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


enum Waveform
{
  SINE = 0,
  SQUARE,
  SAW,
  TRIANGLE,
  NUMBER_OF_WAVEFORMS
};

// WaveGenerator class - Generates a Sine Wave with modifiable
// frequency, pitchOffset, and volume.
class WaveGenerator
{
  public:

    // Creates an AudioData Class with an Inital Frequency and Sampling Rate
    WaveGenerator(double initialFrequency_, double samplingRate_, Waveform waveform_) 
    {
      waveform = waveform_;
      initialFrequency = initialFrequency_;
      currentFrequency = initialFrequency_;
      mSamplingRate = samplingRate_;
      increment = 2 * M_PI * currentFrequency / mSamplingRate;
      pitchOffset = 1.0; 
    }

    // Calculates and Returns the next sample in a sine wave
    double operator()(void)
    {
      double output = 0;

      switch (waveform)
      {
        case SINE:
          output = sin(data);
          output *= gain;

          // Clip Check
          if (output > 1.0f)
            output = 1.0f;
          else if (output < -1.0f)
            output = -1.0f;

          data += increment * pitchOffset;
          break;

        case SQUARE:
          output = sign(sin(data));
          data += increment;
          break;

        default:
          break;
      }

      return output;
    }

    // Updates the Frequency of the Sine Wave
    void updateFrequency(double freq)
    {
      currentFrequency = freq;
      increment = 2 * M_PI * currentFrequency / mSamplingRate;
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
    { return mSamplingRate; }

  private:

    double sign(double value)
    {
      if(value >= 0.0f)
        return 1.0f;
      else
        return -1.0f;
    }

    Waveform waveform;

    double initialFrequency,
           currentFrequency,
           pitchOffset = 1.0,
           gain = 1.0,
           mSamplingRate = 44100;
    double data = 0,
           increment = currentFrequency / mSamplingRate;

};


// WaveTableGenerator Class
// Generates a WaveTable from a WaveGenerator
class WaveTableGenerator
{

public:

  // Create a WaveTable
  WaveTableGenerator(Waveform waveform)
  {
    UpdateWaveTable(WaveGenerator(440.0, 48000.0, waveform));
  }

  WaveTableGenerator(WaveGenerator wave)
  {
    UpdateWaveTable(wave);
  }

  // Update the WaveTable with a new Waveform
  void UpdateWaveTable(WaveGenerator wave)
  {
    waveTable.reserve(static_cast<size_t>(wave.getSamplingRate()));
    
    for(size_t i = 0; i < static_cast<size_t>(wave.getSamplingRate()); ++i)
      waveTable.push_back(static_cast<float>(wave()));
  }

  int GetSamplingRate()
  { return static_cast<int>(waveTable.size()); }

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