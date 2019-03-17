#include "Grain.h"
#define LEFT_CHANNEL 0
#define RIGHT_CHANNEL 1

// ------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------

GrainCloud::GrainCloud(int startingSample_, int duration_)
{ 
  mSamplingRate = 0;
  SetDuration(duration_);
  SetCentroidSample(startingSample_);
  AddGrains();
}


// ------------------------------------------------------------------------------------

/*float GrainCloud::operator()(int channel)
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
}*/

float GrainCloud::operator()(int channel)
{
  float sample = 0;

  for (GrainData& grain : grains)
  {
     // Check if we are finished getting the current grain
    if (mAudioSourceBuffer->getNumChannels() >= 2)
    {
      if ((grain.mCurrentSample[LEFT_CHANNEL] >= grain.mEndSample) && 
          (grain.mCurrentSample[RIGHT_CHANNEL] >= grain.mEndSample))
        grain.mIsFinished = true;
    }
    else if((grain.mCurrentSample[LEFT_CHANNEL] >= grain.mEndSample))
      grain.mIsFinished = true;

      // Restart the grain if we are using it after finishing.
    if (grain.mIsFinished)
    {
      grain.mIsFinished = false;

      // Randomize the Grain
      RandomizeGrain(grain);
    }

    // Get the Current Sample From the Audio Buffer
    sample += mAudioSourceBuffer->getSample(channel, grain.mCurrentSample[channel]);
    ++grain.mCurrentSample[channel];
  }

  sample *= 0.3f;
  if(sample > 1.0f)
    return 1.0f;
  else if(sample < -1.0f)
    return -1.0f;

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

  for (GrainData& grain : grains)
  {
    RandomizeGrain(grain);
  }
}

// ------------------------------------------------------------------------------------

void GrainCloud::SetDuration(int duration)
{
  // Update Duration and Calculate mSample Delta
  mDuration = duration;
  mSampleDelta = static_cast<int>(mSamplingRate * (static_cast<float>(mDuration) / 1000.0f));
  
  for (GrainData& grain : grains)
  {
    // Clamp the End Sample
    grain.mEndSample = grain.mStartingSample + mSampleDelta;
    if (grain.mEndSample >= mWaveSize)
      grain.mEndSample = (mWaveSize - 1);
  }
}

// ------------------------------------------------------------------------------------

void GrainCloud::SetAudioSource(AudioFormatReader& newAudioFile)
{
  // Update Grain Parameters
  mWaveSize =  static_cast<int>(newAudioFile.lengthInSamples);
  mSamplingRate = static_cast<int>(newAudioFile.sampleRate);

  // Clear the Audio Source and Read the New WAV File
  mAudioSourceBuffer.reset(new AudioSampleBuffer(newAudioFile.numChannels, static_cast<int>(newAudioFile.lengthInSamples)));
  newAudioFile.read(mAudioSourceBuffer.get(), 0, static_cast<int>(newAudioFile.lengthInSamples), 0, true, true);

}

// ------------------------------------------------------------------------------------

void GrainCloud::SetCloudSize(int size)
{
  if (size > mCloudSize)
    AddGrains(size - mCloudSize);
  else if (size < mCloudSize)
    RemoveGrains(mCloudSize - size);
}

// ------------------------------------------------------------------------------------


void GrainCloud::RandomizeGrain(GrainData& grain)
{
  if (mStartingOffset == 0 || (mCentroidSample - mStartingOffset) <= 0)
    grain.mStartingSample = mCentroidSample;
  else
    grain.mStartingSample = Random().nextInt(Range<int>(mCentroidSample - mStartingOffset,
                                                        mCentroidSample + mStartingOffset));
    
  grain.mCurrentSample[LEFT_CHANNEL] = grains.back().mStartingSample;
  grain.mCurrentSample[RIGHT_CHANNEL] = grains.back().mStartingSample;

  grain.mEndSample = grain.mStartingSample + mSampleDelta;
  if (grain.mEndSample >= mWaveSize)
    grain.mEndSample = (mWaveSize - 1);
}

// ------------------------------------------------------------------------------------

void GrainCloud::AddGrains(int count)
{
  for (int i = 0; i < count; ++i)
  {
    grains.push_back(GrainData());
    RandomizeGrain(grains.back());
  }

  mCloudSize += count;
}

// ------------------------------------------------------------------------------------

void GrainCloud::RemoveGrains(int count)
{
  for(int i = 0; i < count; ++i)
    grains.pop_back(); 

  mCloudSize -= count;
}

// ------------------------------------------------------------------------------------


void GrainCloud::Reset()
{
  for (GrainData& grain : grains)
    grain.mIsFinished = true;
 
  mIsPlaying = false;
}

// ------------------------------------------------------------------------------------
