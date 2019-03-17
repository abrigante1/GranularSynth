#include "Grain.h"
#define LEFT_CHANNEL 0
#define RIGHT_CHANNEL 1

// ------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------

GrainCloud::GrainCloud(int startingSample_, int duration) : 
  mCentroidSample(startingSample_)
{
  mCurrentSample[LEFT_CHANNEL] = mCentroidSample;
  mCurrentSample[RIGHT_CHANNEL] = mCentroidSample;

  mSamplingRate = 0;
  SetDuration(duration);
}


// ------------------------------------------------------------------------------------

float GrainCloud::operator()(int channel)
{ 

  // Check if we are finished getting the current grain
  if (mAudioSourceBuffer->getNumChannels() >= 2)
  {
    if ((mCurrentSample[LEFT_CHANNEL] >= mEndSample) && (mCurrentSample[RIGHT_CHANNEL] >= mEndSample))
      mIsFinished = true;
  }
  else if((mCurrentSample[LEFT_CHANNEL] >= mEndSample))
    mIsFinished = true;

    // Restart the grain if we are using it after finishing.
  if (mIsFinished)
  {
    mIsFinished = false;
    mCurrentSample[LEFT_CHANNEL] = mCentroidSample;
    mCurrentSample[RIGHT_CHANNEL] = mCentroidSample;
  }

    // Get the Current Sample From the Audio Buffer
  float sample = mAudioSourceBuffer->getSample(channel, mCurrentSample[channel]);
  ++mCurrentSample[channel];

  return sample;
}

// ------------------------------------------------------------------------------------

/* float Grain::operator()()
{
  // Restart the grain if we are using it after finishing.
  if (mIsFinished)
  {
    mIsFinished = false;
    mCurrentSample[LEFT_CHANNEL] = mStartingSample;
    mCurrentSample[RIGHT_CHANNEL] = mStartingSample;
  }

  float sample = waveTable[mCurrentSample[LEFT_CHANNEL]++];

  // Check if we are finished getting the current grain
  if (mCurrentSample[LEFT_CHANNEL] >= mEndSample)
    mIsFinished = true;

  return sample;
} */
 
// ------------------------------------------------------------------------------------

void GrainCloud::SetCentroidSample(int startingSample)
{
  // Update the Starting Sample, and Reset the Current Sample
  mCentroidSample = startingSample - 1;
  mCurrentSample[LEFT_CHANNEL] = mCentroidSample;
  mCurrentSample[RIGHT_CHANNEL] = mCentroidSample;

  // Clamp the End Sample
  mEndSample = mCentroidSample + mSampleDelta;
  if(mEndSample >= mWaveSize)
    mEndSample = (mWaveSize - 1);
}

// ------------------------------------------------------------------------------------

void GrainCloud::SetDuration(int duration)
{
  // Update Duration and Calculate mSample Delta
  mDuration = duration;
  mSampleDelta = static_cast<int>(mSamplingRate * (static_cast<float>(mDuration) / 1000.0f));
  
  // Clamp the End Sample
  mEndSample = mCentroidSample + mSampleDelta;
  if(mEndSample >= mWaveSize)
    mEndSample = (mWaveSize - 1);
}

// ------------------------------------------------------------------------------------

void GrainCloud::SetAudioSource(AudioFormatReader& newAudioFile)
{
  // Update Grain Parameters
  mWaveSize =  static_cast<int>(newAudioFile.lengthInSamples);
  mSamplingRate = newAudioFile.sampleRate;

  // Clear the Audio Source and Read the New WAV File
  mAudioSourceBuffer.reset(new AudioSampleBuffer(newAudioFile.numChannels, static_cast<int>(newAudioFile.lengthInSamples)));
  newAudioFile.read(mAudioSourceBuffer.get(), 0, static_cast<int>(newAudioFile.lengthInSamples), 0, true, true);

}

// ------------------------------------------------------------------------------------

void GrainCloud::Reset()
{
  mIsFinished = true;
  mIsPlaying = false;
}

// ------------------------------------------------------------------------------------

int GrainCloud::GetRandomStartingSample()
{
  return Random().nextInt(Range<int>(-mStartingOffset, mStartingOffset));
}

// ------------------------------------------------------------------------------------