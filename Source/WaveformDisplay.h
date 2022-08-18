/**
* WaveformDisplay is the header file for displaying audio waves module.
@required:
    1. JuceHeader file.
    2. Pragma the h.file once.
**/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class WaveformDisplay    : public Component, 
                           public ChangeListener
{
public:
    /// <summary>
    /// Constructor for taking the Audio Format and Cache References
    /// </summary>
    /// <param name="formatManagerToUse"></param>
    /// <param name="cacheToUse"></param>
    WaveformDisplay( AudioFormatManager & 	formatManagerToUse,
                    AudioThumbnailCache & 	cacheToUse );
    
    ///De-Constructor
    ~WaveformDisplay();


    /// <summary>
    /// Painting the UI of the function itself, to display the waveform of the audio frequency.
    /// </summary>
    /// <param name="Graphics">g by reference</param>
    void paint (Graphics&) override;

    /// <summary>
    /// Resizing the UI
    /// </summary>
    void resized() override;

    /// <summary>
    /// Holds a list of ChangeListeners, and sends messages to them when instructed.
    /// </summary>
    /// <param name="source">Takes a pointer of the Broadcaster</param>
    void changeListenerCallback (ChangeBroadcaster *source) override;

    /// <summary>
    /// Loading the Path of the audio file
    /// </summary>
    /// <param name="audioURL">Taking a File Path</param>
    void loadURL(URL audioURL);

    /// <summary>
    /// Set the relative position of he play-head
    /// </summary>
    /// <param name="pos">Double parameter</param>
    void setPositionRelative(double pos);

private:
    AudioThumbnail audioThumb;
    bool fileLoaded; 
    double position;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
