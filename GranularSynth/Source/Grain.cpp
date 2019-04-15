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
    // If the Current Grain Isn't Active, Randomize it and Play the Grain
    if (!grain.envelope.isActive())
    {
      RandomizeGrain(grain);
      grain.envelope.noteOn();
      grain.mInRelease = false;
    }
    else if(!grain.mInRelease)
    {
      // Check if we are finished getting the current grain
      if (mAudioSourceBuffer->getNumChannels() >= 2) // Stereo 
      {
        
        if ((grain.mCurrentSample[LEFT_CHANNEL] >= grain.mEndSample) &&
          (grain.mCurrentSample[RIGHT_CHANNEL] >= grain.mEndSample))
        {
          grain.envelope.noteOff();
          grain.mInRelease = true;
        }

      }
      else if (grain.mCurrentSample[LEFT_CHANNEL] >= grain.mEndSample) // Mono
      {
        grain.envelope.noteOff();
        grain.mInRelease = true;
      }
    }

    double randomPanValue = 1.0f;
    if(mRandomPanning)
      if (channel == 0)
        randomPanValue = std::sin(grain.mPanningValue * (M_PI_2));
      else
        randomPanValue = std::sin((1.0 - grain.mPanningValue) * (M_PI_2));

    // Get the Current Sample From the Audio Buffer
    float currentSample = (mAudioSourceBuffer->getSample(channel, static_cast<int>(grain.mCurrentSample[channel])));
    
    //         currentSample             ADSR Grain                         Random Gain                            Random Panning Value
    sample += (currentSample * grain.envelope.getNextSample() * static_cast<float>(grain.mGainScalar) * static_cast<float>(randomPanValue));
   
    grain.mCurrentSample[channel] += grain.mPitchScalar;
    if(grain.mCurrentSample[channel] >= static_cast<double>(mWaveSize))
      grain.mCurrentSample[channel] = (static_cast<double>(mWaveSize) - 1.0f);
    
  }

  // Scale the Sample by the Gain
  sample *= static_cast<float>(mGlobalGain);

  // If Polyphonic, Scale by 0.3f
  if(grains.size() > 1)
    sample *= 0.3f;
  
  // Clip Check
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

  // Set Grain Envelope Parameters
  grain.envelope.setParameters({});
    
  // Set the Current Sample to the Starting Sample
  grain.mCurrentSample[LEFT_CHANNEL] = static_cast<double>(grain.mStartingSample);
  grain.mCurrentSample[RIGHT_CHANNEL] = static_cast<double>(grain.mStartingSample);

  // Clamp the End Sample to be Within the WaveTable Range
  grain.mEndSample = grain.mStartingSample + mSampleDelta;
  if (grain.mEndSample >= mWaveSize)
    grain.mEndSample = (mWaveSize - 1);
  
  // Randomize the Pitch 
  double randomPitch = 0.0;
  if(mPitchOffsetMax != 0.0 || mPitchOffsetMin != 0.0)
    randomPitch = mPitchOffsetMin + (mPitchOffsetMax - mPitchOffsetMin) * rand.nextDouble();
  grain.mPitchScalar = std::pow(2.0f, randomPitch / 12.0f);

  // Randomize the Grain Gain
  double randomGain = 0.0;
  if(mGainOffsetDbMax < 0)
    randomGain = static_cast<double>(rand.nextInt(Range<int>(mGainOffsetDbMin, mGainOffsetDbMax)));
  grain.mGainScalar = Decibels::decibelsToGain<double>(randomGain);

  // Randomize the Pan Value
  if(mRandomPanning)
    grain.mPanningValue = rand.nextDouble();
  else
    grain.mPanningValue = 1.0f;
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
  {
    grain.envelope.noteOff();
    grain.mInRelease = true;
  }
 
  mIsPlaying = false;
}

// ------------------------------------------------------------------------------------
