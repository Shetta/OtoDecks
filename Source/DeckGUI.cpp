#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"
#include "DJAudioPlayer.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                AudioFormatManager & 	formatManagerToUse,
                AudioThumbnailCache & 	cacheToUse
           ) : player(_player), 
               waveformDisplay(formatManagerToUse, cacheToUse)
{
    //Reveling the buttons 
    buttonsPad(&playButton);
    buttonsPad(&stopButton);

    //Reveling the sliders
    effectsPad(volSlider, volLabel, 0.0, 1.0, "Volume", " dB");
    effectsPad(speedSlider, speedLabel, 0.0, 100.0, "Speed", " SVC");
    effectsPad(posSlider, posLabel, 0.0, 1.0, "Position", " Seconds");
    freqAndDurPad(frequencySlider,
                  frequencyLabel, 
                  50,
                  5000,
                  "Frequency",
                  " Hz",
                  500,
                  500);
    freqAndDurPad(durationSlider, 
                  durationLabel, 
                  1.0 / frequencySlider.getMaximum(), 
                  1.0 / frequencySlider.getMinimum(),
                  "Duration",
                  " S",
                  0.002,
                  500);

}

DeckGUI::~DeckGUI()
{
    
}

void DeckGUI::paint (Graphics& g)
{
    //Clearing the background
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
    //Setting the GUI colour
    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::black);
    g.setFont (14.0f);
    g.drawText ("", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void DeckGUI::resized()
{

    //Setting the bounds for each module
    double rowH = getHeight() / 10; 
    playButton.setBounds(0, 0, getWidth(), rowH);
    stopButton.setBounds(0, rowH * 1.5, getWidth(), rowH);

    volSlider.setBounds(getWidth() / 5, rowH * 2.5, getWidth() / 2, rowH);
    speedSlider.setBounds(getWidth() / 5, rowH * 3.5, getWidth() / 2, rowH);
    posSlider.setBounds(getWidth() / 5, rowH * 4.5, getWidth() / 2, rowH);

    frequencySlider.setBounds(getWidth()/ 5, rowH * 5.5, getWidth()/2, rowH);
    durationSlider.setBounds(getWidth() / 5, rowH * 6.5, getWidth() / 2, rowH);

    waveformDisplay.setBounds(0, rowH * 7.5, getWidth(), rowH * 2);
    

}

void DeckGUI::buttonClicked(Button* button)
{
    //If the playbutton is pressed, the audio will start, otherwise if the stop button is pressed, it will stop.
    if (button == &playButton)
    {
        player->setPosition(0);
        player->start();
    }
     if (button == &stopButton)
    {
        player->stop();
    }
}

void DeckGUI::sliderValueChanged (Slider *slider)
{
    //The sliders' value would affect each and respective functions in the DJ audioplayer
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }

    if (slider == &frequencySlider)
    {
        player->setFrequency(slider->getValue());
        durationSlider.setValue(1.0 / frequencySlider.getValue());
    }
    else if (slider == &durationSlider)
    {
        player->setFrequency(slider->getValue());
        frequencySlider.setValue(1.0 / durationSlider.getValue());
    }
    
}

void DeckGUI::effectsPad(Slider &slider,
                         Label &label,
                         double startRange,
                         double endRange,
                         String name,
                         String suffix)
{
	//Setting the sliders style and form.
    slider.setSliderStyle(Slider::LinearBar);
	addAndMakeVisible(slider);
    slider.addListener(this);
    slider.setRange(startRange, endRange);
    slider.setTextValueSuffix(suffix);
	addAndMakeVisible(slider);
    label.setText(name, dontSendNotification);
    label.attachToComponent(&slider, true);
}

void DeckGUI::freqAndDurPad(Slider& slider,
                            Label& label,
                            double startRange,
                            double endRange,
                            String name,
                            String suffix,
                            double skew, 
                            double value)
{
    slider.setSliderStyle(Slider::LinearBar);
    slider.addListener(this);
	addAndMakeVisible(slider);
    slider.setRange(startRange, endRange);
    slider.setValue(value);
    slider.setSkewFactorFromMidPoint(skew);
    slider.setTextValueSuffix(suffix);

	addAndMakeVisible(label);
    label.setText(name, dontSendNotification);
    label.attachToComponent(&slider, true);
}

void DeckGUI::buttonsPad(Button* button)
{
    //Buttons and their listners 
    addAndMakeVisible(button);
    button->setColour(juce::TextButton::buttonOnColourId, juce::Colours::aliceblue);
    button->addListener(this);

}



    

