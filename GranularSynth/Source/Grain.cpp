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
    sample += (mAudioSourceBuffer->getSample(channel, 
                                            static_cast<int>(grain.mCurrentSample[channel])));
   
    grain.mCurrentSample[channel] += grain.mPitchScalar;
    if(grain.mCurrentSample[channel] >= static_cast<double>(mWaveSize))
      grain.mCurrentSample[channel] = (static_cast<double>(mWaveSize) - 1.0f);
    
  }

  if(grains.size() > 1)
    sample *= 0.3f;

  if(sample > 1.0f)
    return 1.0f;
  else if(sample < -1.0f)
    return -1.0f;

  return sample;
}

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
  Random rand = Random();

  // Ensure the Starting Smaple is non-negative
  if (mStartingOffset == 0 || (mCentroidSample - mStartingOffset) <= 0)
    grain.mStartingSample = mCentroidSample;
  else
  {

    // Randomize the Starting Sample
    grain.mStartingSample = rand.nextInt(Range<int>(mCentroidSample - mStartingOffset,
                                                    mCentroidSample + mStartingOffset));

    // Clamp the Starting Sample to be Within the WaveTable Range
    if(grain.mStartingSample < 0)
      grain.mStartingSample = 0;
    else if(grain.mStartingSample >= mWaveSize)
      grain.mStartingSample = (mWaveSize - 1);
  }
    
  // Set the Current Sample to the Starting Sample
  grain.mCurrentSample[LEFT_CHANNEL] = static_cast<double>(grains.back().mStartingSample);
  grain.mCurrentSample[RIGHT_CHANNEL] = static_cast<double>(grains.back().mStartingSample);

  // Clamp the End Sample to be Within the WaveTable Range
  grain.mEndSample = grain.mStartingSample + mSampleDelta;
  if (grain.mEndSample >= mWaveSize)
    grain.mEndSample = (mWaveSize - 1);
  
  // Randomize the Pitch 
  int randomPitch = 0;
  if(mPitchOffset > 0)
    randomPitch = rand.nextInt(Range<int>(-mPitchOffset, mPitchOffset));

  grain.mPitchScalar = std::pow(2.0f, static_cast<double>(randomPitch) / 12.0f);
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
