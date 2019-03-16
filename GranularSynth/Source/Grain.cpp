#include "Grain.h"

// ------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------

Grain::Grain(WaveTableGenerator newWaveTable, int startingSample_, int duration) : 
  waveTable(newWaveTable),
  mStartingSample(startingSample_),
  mCurrentSample(startingSample_)
{
  mSamplingRate = waveTable.GetSamplingRate();
  SetDuration(duration);
}


// ------------------------------------------------------------------------------------

float Grain::operator()(void)
{ 
  // Restart the grain if we are using it after finishing.
  if (mIsFinished)
  {
    mIsFinished = false;
    mCurrentSample = mStartingSample;
  }

  // Get the Current Sample from the WaveTable
  float sample = waveTable[mCurrentSample++];

  // Check if we are finished getting the current grain
  if (mCurrentSample >= mEndSample)
  {
    mIsFinished = true;
  }

  return sample;
}

// ------------------------------------------------------------------------------------

void Grain::SetStartingSample(int startingSample)
{
  mStartingSample = startingSample;
  mEndSample = mStartingSample + mSampleDelta;
}

// ------------------------------------------------------------------------------------

void Grain::SetDuration(int duration)
{
  mDuration = duration;
  mSampleDelta = static_cast<int>(mSamplingRate * (static_cast<float>(mDuration) / 1000.0f));
  mEndSample = mStartingSample + mSampleDelta;
}

// ------------------------------------------------------------------------------------