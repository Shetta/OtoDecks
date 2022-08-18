
#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/

//Inheriting the ListBoxModel, TextEditor, Timer, and Drag and drop classes.
class PlaylistComponent : public juce::Component,
    public juce::TableListBoxModel,
    public juce::Button::Listener,
    public juce::TextEditor::Listener,
    public juce::Timer,
    public juce::FileDragAndDropTarget

{
public:
    /// <summary>
    /// By using pointers of DJAudioPlayer twice for the Deck1&2 in our constructor, we would be able to pass our data through them using
    /// Audio Format and Cache Classes.
    /// </summary>
    /// <param name="player1"></param>
    /// <param name="player2"></param>
    /// <param name="formatManagerToUse"></param>
    /// <param name="cacheToUse"></param>
    /// <param name="formatManagerToUse2"></param>
    /// <param name="cacheToUse2"></param>
    PlaylistComponent(DJAudioPlayer* player1, DJAudioPlayer* player2,
        juce::AudioFormatManager& formatManagerToUse,
        juce::AudioThumbnailCache& cacheToUse,
        juce::AudioFormatManager& formatManagerToUse2,
        juce::AudioThumbnailCache& cacheToUse2);

    //De-Constructor
    ~PlaylistComponent() override;

	/// <summary>
    /// Resizing the UI
    /// </summary>
    void resized() override;

    /// <summary>
    /// Getting the number of tracks from our Data Structure.
    /// </summary>
    /// <returns>Number of tracks</returns>
    int getNumRows() override;

    /// <summary>
    /// Painting the BG of the table component itself.
    /// This is by using the Graphics Class and referencing by the rowNumber of the intended part.
    /// </summary>
    /// <param name=""></param>
    /// <param name="rowNumber"></param>
    /// <param name="width"></param>
    /// <param name="height"></param>
    /// <param name="rowIsSelected"></param>
    void paintRowBackground(juce::Graphics&,
        int rowNumber,
        int width,
        int height,
        bool rowIsSelected)override;

	/// <summary>
    /// Painting the UI of the function itself, to display the play list components.
    /// </summary>
    /// <param name="Graphics">g by reference</param>
    void paint(juce::Graphics&) override;

	/// <summary>
	/// By referencing to Graphics Class, we would be able to change the appearance of each cell.
    /// This is done by selecting the appropriate rowNum and ID
	/// </summary>
	/// <param name=""></param>
	/// <param name="rowNumber"></param>
	/// <param name="columnId"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="rowIsSelected"></param>
	void paintCell(juce::Graphics&,
		int rowNumber,
		int columnId,
		int width,
		int height,
		bool rowIsSelected)override;

	/// <summary>
	/// The Custom components that we have created is being refreshed.
	/// </summary>
	/// <param name="rowNumber"></param>
	/// <param name="columnId"></param>
	/// <param name="isRowSelected"></param>
	/// <param name="existingComponentToUpdate"></param>
	/// <returns></returns>
	Component* refreshComponentForCell(
		int rowNumber,
		int columnId,
		bool isRowSelected,
		Component* existingComponentToUpdate)override;

	/// <summary>
    /// To display our waveformDisplay functionality here instead of using it on DECKGUI
    /// </summary>
	void timerCallback() override;

	/// <summary>
	/// These two functions are responsible for dragging and dropping the intended files.
    /// The Logic was a mix between UoL Lectures melded with the logic of our data structures.
	/// </summary>
	/// <param name="files"></param>
	/// <returns></returns>
	bool isInterestedInFileDrag(const StringArray& files) override;
	void filesDropped(const StringArray& files, int x, int y) override;

    /// <summary>
    /// This would induce if the button ID of our load button is clicked 
    /// and parses it to our DJ player by using the pointers.
    /// </summary>
    /// <param name="ID"></param>
    void loadFromLibrary(int ID);

    /// <summary>
    /// The ButtonClicked logic is used only for the three functionalities we have in our
    /// play list component, whereas it can only LOAD into one of our Decks otherwise, if clicked delete, it 
    /// would delete the song.
    /// </summary>
    /// <param name="button"></param>
    void buttonClicked(juce::Button* button)override;

    /// <summary>
    /// This functionality is based upon the taught method of loading a file
    /// The only difference here is to check if the file's path and name already exists in the 
    /// XMLTree that we have, if it does not exist, it would initialize a new Object out of our new
    /// Data Structure, and parse it to our Vector.
    /// </summary>
    void addFileToLibrary();

    /// <summary>
    /// Checks if the Track Name exists within our XML Tree by iterating through the whole Vector
    /// </summary>
    /// <param name="trackName"></param>
    /// <returns>True/False</returns>
    bool DoesItExist(String& trackName);

    /// <summary>
    /// Remove the track if it exists within our Vector
    /// </summary>
    /// <param name="trackId"></param>
    void removeTrack(int trackId);

    /// <summary>
    /// Gets the track Length from our AudioFormatManager as an output and changes it to string
    /// </summary>
    /// <param name="musicFile"></param>
    /// <returns></returns>
    juce::String getTrackLength(juce::File musicFile);

    /// <summary>
    /// Saving and Loading the Track in XML format
    /// https://docs.juce.com/master/tutorial_table_list_box.html
    /// https://docs.juce.com/master/tutorial_audio_processor_value_tree_state.html
    /// https://docs.juce.com/master/classDirectoryContentsList.html
    /// https://docs.juce.com/master/classXmlElement.html
    /// https://docs.juce.com/master/group__juce__core-xml.html
    /// https://forum.juce.com/t/example-for-creating-a-file-and-doing-something-with-it/31998
    /// </summary>
    void saveTrack();
    void loadTrack();
   
    /// <summary>
    /// Searching for our music file by using the string.
    /// </summary>
    /// <param name="textEditor"></param>
    void textEditorTextChanged(juce::TextEditor& textEditor) override;

    /// <summary>
    /// Inserting Data into our Arrays
    /// </summary>
    /// <param name="title"></param>
    /// <param name="file"></param>
    /// <param name="path"></param>
    /// <param name="length"></param>
    void dataInsert(String title, 
        File file,
        URL path, 
        String length);

    /// <summary>
    /// Deleting Data with the index
    /// </summary>
    /// <param name="index"></param>
    void dataRemove(int index);

private:

    WaveformDisplay waveformDisplay;
    WaveformDisplay waveformDisplay2;

    juce::TextButton addButton{ "ADD" };
    juce::TextEditor searchBox{ "Search for your Song." };

    juce::Array<juce::String> trackTitle;
    juce::Array<juce::File> trackFile;
    juce::Array<juce::String> trackLength;
    juce::Array<URL> trackPath;

    juce::Array<juce::String> trackTitleSearch;
    juce::Array<juce::File> trackFileSearch;
    juce::Array<juce::String> trackLengthSearch;
    juce::Array<URL> trackPathSearch;


    juce::TableListBox tableComponent;

    DJAudioPlayer* player1;
    DJAudioPlayer* player2;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};
