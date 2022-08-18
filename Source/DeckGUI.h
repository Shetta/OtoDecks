
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "PlaylistComponent.h"
#include <vector>

//==============================================================================
/*
*/
class DeckGUI    : public Component,
                   public Button::Listener, 
                   public Slider::Listener
{
public:
    /// <summary>
    /// Main Constructor to reference the DJAudioPlayer by a pointer.
    /// Ref. The AudioFormatManager and Cache
    /// </summary>
    /// <param name="player"></param>
    /// <param name="formatManagerToUse"></param>
    /// <param name="cacheToUse"></param>
    DeckGUI(DJAudioPlayer* player, 
           AudioFormatManager & 	formatManagerToUse,
           AudioThumbnailCache & 	cacheToUse );
    //De-Constructor
    ~DeckGUI();

	/// <summary>
	/// Painting the UI of the function itself, to display the waveform of the audio frequency.
	/// </summary>
	/// <param name="Graphics">g by reference</param>
	void paint(Graphics&) override;

	/// <summary>
	/// Resizing the UI
	/// </summary>
	void resized() override;
    
    /// <summary>
    /// Implementing the Button Listeners for our functions by pointers
    /// </summary>
    /// <param name=""></param>
    void buttonClicked (Button *) override;

    /// <summary>
    /// Implementing the sliders listeners for our functions by pointers.
    /// </summary>
    /// <param name="slider"></param>
    void sliderValueChanged (Slider *slider) override;

    /// <summary>
    /// Instead of creating each functions in the constructor and labeling them, I have created the similar ones inside a robust function
    /// For future effects to be added.
    /// </summary>
    /// <param name="slider">For our Slider to be added</param>
    /// <param name="label">The label of our slider</param>
    /// <param name="startRange">The starting range</param>
    /// <param name="endRange">The Ending range</param>
    /// <param name="name">The name of our Slider</param>
    /// <param name="suffix">The symbol of our range (E.g dB)</param>
    void effectsPad(Slider &slider,
        Label &label, 
        double startRange, 
        double endRange, 
        String name, 
        String suffix);
    /// <summary>
    /// The same functionality as the above.
    /// Adding an additional functionality for skewing the range and value.
    /// </summary>
    /// <param name="skew"></param>
    /// <param name="value"></param>
    void freqAndDurPad(Slider& slider, 
        Label& label, 
        double startRange, 
        double endRange, 
        String name, 
        String suffix, 
        double skew, 
        double value);
    
    /// <summary>
    /// Instead of creating buttons manually, this function would make the creation more robust
    /// </summary>
    /// <param name="button"></param>
    void buttonsPad(Button* button);


private:

    TextButton playButton{"PLAY"};
    TextButton stopButton{"STOP"};
    
    Slider frequencySlider;
    Label frequencyLabel;

    Slider durationSlider;
    Label durationLabel;

    Slider volSlider; 
    Label volLabel;

    Slider speedSlider;
    Label speedLabel;

    Slider posSlider;
    Label posLabel;

    WaveformDisplay waveformDisplay;

    DJAudioPlayer* player;
   

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
