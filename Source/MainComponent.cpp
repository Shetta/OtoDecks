#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    //Setting the size of the main UI.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }  
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }  

    //Making the Decks visible
    addAndMakeVisible(deckGUI1); 
    addAndMakeVisible(deckGUI2);
    //Making the music library visible 
    addAndMakeVisible(playlistComponent);

    //Registering the music file formats.
    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    //Registering both decks for the playback
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    //Registering the mixer for the playback
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    //Parallel playback
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);

 }
void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    //Passing the audio blocks for the mixer
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
	//Setting the main background for the application
	getLookAndFeel().setColour(juce::ResizableWindow::backgroundColourId, juce::Colours::dimgrey);
}

void MainComponent::resized()
{
    //This methodology makes the bounds of each module expandable (Like the idea of pixels)
    //By storing the total area in a VectorInt of Rectangles, then slicing it apart.

    Rectangle<int> totalArea = getLocalBounds();
    Rectangle<int> sectionAreaOne = totalArea.removeFromTop(totalArea.getHeight() / 2);
    Rectangle<int> sectionAreaTwo = sectionAreaOne.removeFromLeft(sectionAreaOne.getWidth() / 2);
    Rectangle<int> sectionAreaThree = sectionAreaOne.removeFromRight(sectionAreaOne.getWidth());
    Rectangle<int> sectionAreaFour = totalArea.removeFromBottom(totalArea.getHeight());

    deckGUI1.setBounds(sectionAreaTwo);
    deckGUI2.setBounds(sectionAreaThree);
    playlistComponent.setBounds(sectionAreaFour);

    
}

