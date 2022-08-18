#pragma once
#include <JuceHeader.h>

class DJAudioPlayer : public juce::AudioSource {
public:

    /// <summary>
    /// The Constructor of the DJAudioPlayer is referencing to the AudioFormatManager
    /// </summary>
    /// <param name="_formatManager"></param>
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);
    //De-Constructor
    ~DJAudioPlayer();

    /// <summary>
    /// Inputing the samples per audio blocks as an int and the output will be used to generate the tones.
    /// </summary>
    /// <param name="samplesPerBlockExpected"></param>
    /// <param name="sampleRate"></param>
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    /// <summary>
    /// Referencing the AudioSourceChannelInfo to receive the bits of audio to process it to the source engine.
    /// </summary>
    /// <param name="bufferToFill"></param>
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    /// <summary>
    /// Releasing all the sources when not needed.
    /// </summary>
    void releaseResources() override;
	/// <summary>
	/// Loading the Path of the audio file
	/// </summary>
	/// <param name="audioURL">Taking a File Path</param>
    void loadURL(juce::URL audioURL);
    /// <summary>
    /// Setting the gain for the Audio Output. Its between 0 and 1.
    /// <param name="gain"></param>
    void setGain(double gain);
    /// <summary>
    /// Setting the speed for the Audio Output. being 1.0 is the max.
    /// </summary>
    /// <param name="ratio"></param>
    void setSpeed(double ratio);
    /// <summary>
    /// Setting the position of the current Audio moment. (Which second are we playing ATM).
    /// </summary>
    /// <param name="posInSecs"></param>
    void setPosition(double posInSecs);
    /// <summary>
    /// Setting the relative position of the audio moment.
    /// </summary>
    /// <param name="pos"></param>
    void setPositionRelative(double pos);
    /// <summary>
    /// Setting the frequency of the audio (How much our ears can take).
    /// </summary>
    /// <param name="pass"></param>
    void setFrequency(double pass);
    /// <summary>
    /// Plays the Audio file
    /// </summary>
    void start();
    /// <summary>
    /// Stops the Audio file
    /// </summary>
    void stop();
    /// <summary>
    /// Getting the relative position of the playhead
    /// </summary>
    /// <returns></returns>
    double getPositionRelative();
    /// <summary>
    /// Getting the abs pos of the playhead
    /// </summary>
    /// <returns></returns>
    double getPosition();
    /// <summary>
    /// Getting the track length in Seconds.
    /// </summary>
    /// <returns></returns>
    double getTrackLengthInSeconds();

private:
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
    juce::ChannelRemappingAudioSource frequencySource{ &transportSource, false };
};