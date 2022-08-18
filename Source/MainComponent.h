#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::AudioAppComponent
{
public:
    //==============================================================================
    //Constructor
    MainComponent();
    //De-Constructor
    ~MainComponent() override;

    //==============================================================================
    
    /// <summary>
    /// Buffering the input number of samples expected. The output will be used to generate tones.
    /// </summary>
    /// <param name="samplesPerBlockExpected"></param>
    /// <param name="sampleRate"></param>
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    
    /// <summary>
    /// Telling the Audio source When to play. Taking the buffer as an input to fetch the blocks of audio
    /// </summary>
    /// <param name="bufferToFill"></param>
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    
    /// <summary>
    /// Releasing all the resources when not needed.
    /// </summary>
    void releaseResources() override;

    //==============================================================================

    /// <summary>
    /// Painting the graphics of the UI.
    /// </summary>
    /// <param name="g">Taking g as a refrence to Graphics moduke</param>
    void paint(juce::Graphics& g) override;

    /// <summary>
    /// Resize the component when needed.
    /// </summary>
    void resized() override;

private:
    //==============================================================================

    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbCache{ 100 };

    DJAudioPlayer player1{ formatManager };
    DeckGUI deckGUI1{ &player1, formatManager, thumbCache };

    DJAudioPlayer player2{ formatManager };
    DeckGUI deckGUI2{ &player2, formatManager, thumbCache };

    DJAudioPlayer playerForParsingMetaData{ formatManager };

    juce::MixerAudioSource mixerSource;

    PlaylistComponent playlistComponent{ &player1, &player2,  formatManager, thumbCache,  formatManager, thumbCache};


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
