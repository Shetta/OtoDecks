#include "../JuceLibraryCode/JuceHeader.h"

///Including the header file of the definitions.
#include "WaveformDisplay.h"

//==============================================================================

/// <summary>
/// Taking an initializers' list by starting with 1000 Thumb and initializing as we did not load the file yet.
/// </summary>
/// <param name="formatManagerToUse">Related to AudioFormatManager Class</param>
/// <param name="cacheToUse">Related to AudioThumbnailCache Class</param>
WaveformDisplay::WaveformDisplay(AudioFormatManager & 	formatManagerToUse,
                                 AudioThumbnailCache & 	cacheToUse) :
                                 audioThumb(1000, formatManagerToUse, cacheToUse), 
                                 fileLoaded(false), 
                                 position(0)
                          
{
    /// <summary>
    /// Adding a Child component as a Listener - You should be able to 
    /// Add any child components, and initialize any special settings that your component needs
    /// </summary>
    /// <param name="formatManagerToUse"></param>
    /// <param name="cacheToUse"></param>
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}


/// <summary>
/// This is to repaint our drawing UI/UX. 
/// We are using the graphics class in order to meld with the look and feel of the UI.
/// </summary>
/// <param name="g"></param>
void WaveformDisplay::paint (Graphics& g)
{
    //Clearing the background.
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    //Setting the main colour and drawing the bounds.
    //Outline around the main component.
    g.setColour (Colours::darkseagreen);
    g.drawRect (getLocalBounds(), 1);
    g.setColour (Colours::turquoise);
    
    //If the file is loaded, we would display the waveform.
    if(fileLoaded)
    {
      audioThumb.drawChannel(g, 
        getLocalBounds(), 
        0, 
        audioThumb.getTotalLength(), 
        0, 
        1.0f
      );
      g.setColour(Colours::antiquewhite);
      g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());
    }
    //Otherwise, we would display that nothing is loaded.
    else 
    {
      g.setFont (20.0f);
      g.drawText ("Nothing is loaded yet.", getLocalBounds(),
                  Justification::centred, true);
      g.setColour(Colours::darkturquoise);
    }
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}


/// <summary>
/// Loading the file path and inputing it for the file bool flag
/// </summary>
/// <param name="audioURL">URL Path for the audio file</param>
void WaveformDisplay::loadURL(URL audioURL)
{
      //Clearing the variable of the Thumbnail
      audioThumb.clear();
  
      //Setting the source of the file
      fileLoaded  = audioThumb.setSource(new URLInputSource(audioURL));

      //Setting the bool flag, if it loaded, we would repaint the UI.
      if (fileLoaded)
      {
        AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "File Loaded Successfully!", "Acknowledged");
        repaint();
      }
      else {
          AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "File is Not Loaded.", "Acknowledged");
      }

}

/// <summary>
/// Detecting the change in frequency
/// </summary>
/// <param name="source"></param>
void WaveformDisplay::changeListenerCallback (ChangeBroadcaster *source)
{
    //If any change in the waveform of an audio file, repaint will incur. 
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{

    //Setting the position of the waveform
    if (pos != position)
    {
        position = pos;
        repaint();
    }
}




