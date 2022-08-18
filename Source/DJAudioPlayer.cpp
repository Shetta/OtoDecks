#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager)
    : formatManager(_formatManager)
{

}
DJAudioPlayer::~DJAudioPlayer()
{

}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    //Setting the transportSource for the playback -- Passing the sample rate and the samples per block    
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    //Setting the resampleSource for the playback -- Passing the sample rate and the samples per block    
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    //Getting the next audio chuncks for the resampleSource
    resampleSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
    //Releasing the sources when not needed.
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    //Loading the file into the playback.
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));

    if (reader != nullptr) {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
}

void DJAudioPlayer::setGain(double gain)
{
    //Setting the gain for the playback
    if (gain < 0 || gain > 1.0)
    {
        DBG("DJAudioPlayer::setGain gain should be between 0 and 1");
    }
    else
    {
        transportSource.setGain(gain);
    }
        
}

void DJAudioPlayer::setSpeed(double ratio)
{
    //Setting the speed for our playback
    if (ratio < 0 || ratio > 100.0)
    {
        DBG("DJAudioPlayer::setSpeed ratio should be between 0 and 100");
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
        
}

void DJAudioPlayer::setPosition(double posInSecs)
{
    //Setting the position for the play head
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    //Setting the relative position for our play head.
    if (pos < 0 || pos > 1.0)
    {
        DBG("DJAudioPlayer::setPositionRelative pos should be between 0 and 1");
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

void DJAudioPlayer::start()
{
    //Starting the audio playback
    transportSource.start();
}

void DJAudioPlayer::stop()
{
    //Stopping the audio playback
    transportSource.stop();
}

double DJAudioPlayer::getPositionRelative()
{
    //Getting the relative position of our playback.
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

double DJAudioPlayer::getPosition()
{
    //Getting the abs position of our playback.
    return  transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

double DJAudioPlayer::getTrackLengthInSeconds()
{
    //Getting the track length in seconds.
    return transportSource.getLengthInSeconds();
}

void DJAudioPlayer::setFrequency(double pass)
{
    //Setting the frequency for our audioplayback.
    if (pass < 50 || pass > 5000)
    {
        DBG("DJAudioPlayer::setFrequency pass should be between 50 and 5000");
    }
    else
    {
        frequencySource.getRemappedInputChannel(pass);
        resampleSource.setResamplingRatio(pass);
        transportSource.setGain(pass);
    }  
}
