#include <..\JuceLibraryCode\JuceHeader.h>
#include "PlaylistComponent.h"
#include <algorithm>

//==============================================================================

PlaylistComponent::PlaylistComponent(DJAudioPlayer* _player1, DJAudioPlayer* _player2,
	juce::AudioFormatManager& formatManagerToUse,
	juce::AudioThumbnailCache& cacheToUse,
	juce::AudioFormatManager& formatManagerToUse2,
	juce::AudioThumbnailCache& cacheToUse2)
	: player1{ _player1 }, player2{ _player2 },
	waveformDisplay(formatManagerToUse, cacheToUse),
	waveformDisplay2(formatManagerToUse2, cacheToUse2)
{
	//Load the Library again
    loadTrack();

	// create columns for the library
	tableComponent.getHeader().addColumn("Track Title", 1, 200);
	tableComponent.getHeader().addColumn("Length", 2, 100);
	tableComponent.getHeader().addColumn("Load to Deck1", 3, 100);
	tableComponent.getHeader().addColumn("Load to Deck2", 4, 100);
    tableComponent.getHeader().addColumn("", 5, 50);

	// add table component 
	tableComponent.setModel(this);
	addAndMakeVisible(tableComponent);

	// add ADD button 
	addAndMakeVisible(addButton);
	addButton.addListener(this);

	// add searchBox text editor
	addAndMakeVisible(searchBox);
	searchBox.addListener(this);

	// add waveforms
	addAndMakeVisible(waveformDisplay);
	addAndMakeVisible(waveformDisplay2);
	
	startTimer(100);
    
}

PlaylistComponent::~PlaylistComponent()
{
	//After closing the app, save the current directory
    saveTrack();
}

void PlaylistComponent::resized()
{
    /// <summary>
    /// Setting the bounds for each module.
    /// </summary>
    double rowH = getHeight() / 10;

    // set bounds for widgets
    waveformDisplay.setBounds(0, 0, getWidth() / 2, rowH * 2.5);
    waveformDisplay2.setBounds(getWidth() / 2, 0, getWidth() / 2, rowH * 2.5);
    addButton.setBounds(0, rowH * 9, getWidth(), rowH * 1);
    searchBox.setBounds(0, rowH * 2.5, getWidth(), rowH * 0.9);
    tableComponent.setBounds(0, rowH * 3.5, getWidth(), rowH * 4.8);

}

int PlaylistComponent::getNumRows()
{
    /// <summary>
    /// Return the number of Rows depending on the size of the Vector of Tracks
    /// </summary>
    /// <returns></returns>
	return trackFileSearch.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected)
{
	//If we select a column it will be painted with a specific colour
    if (rowIsSelected) {
        g.fillAll(juce::Colours::aqua); // use same color with waveform
    }
    else {
        g.fillAll(juce::Colours::white);
    }
}

void PlaylistComponent::paint(juce::Graphics& g)
{
    /// <summary>
    /// Painting the GUI of our play list by selecting the colours of our boundaries, buttons, and search box. 
    /// </summary>
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::black);
    g.setFont(14.0f);
    g.drawText("PlaylistComponent", getLocalBounds(),
        juce::Justification::centred, true);   // draw some placeholder text

    // set color for the add button and its text
    addButton.setColour(juce::TextButton::buttonColourId, juce::Colour(255, 53, 90));
    addButton.setColour(juce::TextButton::textColourOffId, juce::Colour(34, 53, 70));

    // set default text to show on a search box as well as set color
    searchBox.setTextToShowWhenEmpty("Search...", juce::Colours::darkgrey);
    searchBox.setColour(juce::TextEditor::backgroundColourId, juce::Colour(255, 255, 255));
    searchBox.setColour(juce::TextEditor::textColourId, juce::Colours::darkgrey);
}

void PlaylistComponent::paintCell(juce::Graphics& g,
	int rowNumber,
	int columnId,
	int width,
	int height,
	bool rowIsSelected)
{
	// display track titles (file names)
	if (columnId == 1) {
		g.drawText(trackTitleSearch[rowNumber], 2, 0, width - 4, height,
			juce::Justification::centredLeft, true); 
	}
	// display track lengths
	else if (columnId == 2) {
		g.drawText(trackLengthSearch[rowNumber], 2, 0, width - 4, height,
			juce::Justification::centredLeft, true);
	}
}

juce::Component* PlaylistComponent::refreshComponentForCell(
	int rowNumber,
	int columnId,
	bool rowIsSelected,
	Component* existingComponentToUpdate)
{
	//Creating a Load to Deck One Button depending on the column ID.
	if (columnId == 3) {
		if (existingComponentToUpdate == nullptr) {
			juce::TextButton* btn = new juce::TextButton("Load to Deck One");
			juce::String id{ std::to_string(rowNumber * 3 + 0) };  // even number of ID
			btn->setComponentID(id);

			btn->addListener(this);
			existingComponentToUpdate = btn;

			btn->setColour(juce::TextButton::buttonColourId, juce::Colour(255, 219, 255));
			btn->setColour(juce::TextButton::textColourOffId, juce::Colour(34, 53, 70));
		}
	}
	//Creating a Load to Deck Two Button depending on the column ID.
	else if (columnId == 4) {
		if (existingComponentToUpdate == nullptr) {
			juce::TextButton* btn = new juce::TextButton("Load to Deck Two");
			juce::String id{ std::to_string(rowNumber * 3 + 1) }; // odd number of ID
			btn->setComponentID(id);

			btn->addListener(this);
			existingComponentToUpdate = btn;

			btn->setColour(juce::TextButton::buttonColourId, juce::Colour(255, 219, 255));
			btn->setColour(juce::TextButton::textColourOffId, juce::Colour(34, 53, 70));
		}
	}
	//Creating a Delete Button to delete one of the selected Tracks.
	else if (columnId == 5) {
		if (existingComponentToUpdate == nullptr)
		{
			juce::TextButton* btn = new juce::TextButton("Delete");
			juce::String id{ std::to_string(rowNumber * 3 + 2) };
			btn->setComponentID(id);
			btn->addListener(this);
			existingComponentToUpdate = btn;
			btn->setColour(juce::TextButton::buttonColourId, juce::Colour(255, 219, 255));
			btn->setColour(juce::TextButton::textColourOffId, juce::Colour(34, 53, 70));
		}
	}
	return existingComponentToUpdate;
}

void PlaylistComponent::timerCallback()
{
	/// <summary>
	/// Displaying our waveform of the loaded sound files.
	/// </summary>
	
	try
	{
		waveformDisplay.setPositionRelative(player1->getPositionRelative());
		waveformDisplay2.setPositionRelative(player2->getPositionRelative());
	}
	catch (int& e)
	{
		AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "waveformDisplay Error: Number ", e + "", "Acknowledged", nullptr);
	}
	

}

bool PlaylistComponent::isInterestedInFileDrag(const StringArray& files)
{
	//Returns true as a threshhold for our playlist drag
    return true;
}

void PlaylistComponent::filesDropped(const StringArray& files, int x, int y)
{
    /// <summary>
    /// Checks if the file is already added or not, however it has a bug and it does not work for some reason.
	/// It creates a new DS of our track and pushes it with all of its meta data.
    /// </summary>
    /// <param name="files"></param>
    /// <param name="x"></param>
    /// <param name="y"></param>
    
	try
	{
		for (juce::String filename : files)
		{
			if (!DoesItExist(filename))
			{
				dataInsert(filename, filename, filename, getTrackLength(filename));
				tableComponent.updateContent();
				tableComponent.repaint();
			}
			else {
				AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "Play-list Error: ", filename + "Is already added", "Acknowledged", nullptr);
			}

		}
	}
	catch (int& e)
	{
		AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "Dropping files into library Error: Number ", e + "", "Acknowledged", nullptr);
	}
	
	
}

void PlaylistComponent::loadFromLibrary(int buttonId)
{
    /// <summary>
    /// Clicks on the designated column ID of ODD or EVEN to determine which is which and load the DJOutput to the GUI.
    /// </summary>
    /// <param name="buttonId"></param>
	try
	{
		if (buttonId % 3 == 0) {  // even ID: load to player 1
			player1->loadURL(juce::URL{ trackFile[buttonId / 3] });
			waveformDisplay.loadURL(juce::URL{ trackFile[buttonId / 3] });
		}
		else if (buttonId % 3 == 1) {  // odd ID: load to player 2
			player2->loadURL(juce::URL{ trackFile[buttonId / 3] });
			waveformDisplay2.loadURL(juce::URL{ trackFile[buttonId / 3] });
		}
	}
	catch (int& e)
	{
		AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "Loading into Decks Error: Number ", e + "", "Acknowledged", nullptr);
	}
	
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
	try
	{
		// ADD button is clicked: add music to the library as well as save it to Tracks folder
		if (button == &addButton) {
			addFileToLibrary();
			tableComponent.updateContent();
			tableComponent.repaint();
		}

		// LOAD1 or LOAD2 button is clicked: load files from the library    
		else {
			int buttonId = std::stoi(button->getComponentID().toStdString());
			loadFromLibrary(buttonId);
			if (buttonId % 3 == 2)
			{
				removeTrack(static_cast<int>(buttonId / 3));
			}
		}
	}
	catch (int& e)
	{
		AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "Button Error: Number ", e + "", "Acknowledged", nullptr);
	}

}

void PlaylistComponent::addFileToLibrary()
{
    /// <summary>
    /// Mainly it was inspired by the Code from UoL and this:
	/// https://forum.juce.com/t/presets-file-creation-loading-saving-editing/43431
	/// 
	/// The idea here is to iterate through the browseForMultipleFilesToOpen and checks if the file path 
	/// already exist or not, if not, it will create a new object and push it to the newTracks.
	/// Else, it will display a prompt on the screen that says it already added.
    /// </summary>
    
	try
	{
		juce::FileChooser chooser{ "Do your bidding, DJ.." };

		if (chooser.browseForMultipleFilesToOpen()) {

			for (const File& file : chooser.getResults())
			{
				String musicFileName{ file.getFileNameWithoutExtension() };
				if (!DoesItExist(musicFileName))
				{
					dataInsert(musicFileName, file, file.getCurrentWorkingDirectory().getFullPathName(), getTrackLength(file));
					tableComponent.updateContent();
					tableComponent.repaint();
				}
				else {
					AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "Play-list Error: ", musicFileName + " Is already added", "Acknowledged", nullptr);
				}

			}

		}
	}
	catch (int& e)
	{
		AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "adding tracks Error: Number ", e + "", "Acknowledged", nullptr);
	}
	
}

bool PlaylistComponent::DoesItExist(String& trackName)
{
	/// <summary>
	///  bool function iterate through the name and the path of the file and return a true or false prompt.
	/// </summary>
	/// <param name="trackName"></param>
	/// <returns></returns>
	try
	{
		return (std::find(trackTitle.begin(), trackTitle.end(), trackName) != trackTitle.end());
	}
	catch (int& e)
	{
		AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "Finding the existence of tracks Error: Number ", e + "", "Acknowledged", nullptr);
	}
}

void PlaylistComponent::removeTrack(int trackId)
{
	/// <summary>
	/// This was inspired by:
	/// https://forum.juce.com/t/iterating-xmlelement-and-calling-removechildelement/36745/2
	/// https://forum.juce.com/t/xmlelemt-removechildelement-usage/4584
	/// 
	/// And methodology was posted by the author in the comments:
	/// https://forum.juce.com/t/add-removetrack-and-inserttrack-to-midifile-class/21078
	/// 
	/// The idea here is to have a path for each track then iterate through the Vector
	/// of our data structure then if the path matches, we would delete it from our Vector
	/// </summary>
	/// <param name="trackId"></param>

	try
	{
		URL removeTrack = trackPath[trackId];

		for (int trackNum = 1; trackNum < trackFile.size(); ++trackNum)
		{

			if (trackFile[trackNum] == removeTrack.getLocalFile())
			{
				dataRemove(trackNum);
			}
		}
		dataRemove(trackId);
		resized();
		tableComponent.resized();

		textEditorTextChanged(searchBox);
	}
	catch (int& e)
	{
		AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "Removing tracks Error: Number ", e + "", "Acknowledged", nullptr);
	}
}

String PlaylistComponent::getTrackLength(juce::File trackFile)
{
	/// <summary>
	/// Getting the track length from the format manager
	/// 
	/// Code was inspired by:
	/// https://forum.juce.com/t/get-track-length-before-it-starts-playing/44838/2
	/// 
	/// After getting the total length, we can return it using a String method
	/// </summary>
	/// <param name="trackFile"></param>
	/// <returns></returns>

	try
	{
		juce::AudioFormatManager formatManager;
		formatManager.registerBasicFormats();   // add basic music file formats

		// create a format reader and calculate the song length
		juce::AudioFormatReader* formatReader = formatManager.createReaderFor(trackFile);
		int totalLength = formatReader->lengthInSamples / formatReader->sampleRate;

		// convert the length to time
		return std::to_string(totalLength / 60) + ":" + std::to_string(totalLength % 60);
	}
	catch (int& err)
	{
		AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "Getting tracks' length Error: Number ", err + "", "Acknowledged", nullptr);
	}

}

void PlaylistComponent::saveTrack()
{
    /// <summary>
    /// The function's aim is to iterate through the XML file that we have, get the attributes of the files
	/// and pushes them back to a File variable so that it can be retrieved later.
	/// This will be saved in the desktop of the user as Music_playlist
	/// 
	/// This was inspired by:
	/// https://stackoverflow.com/questions/5415788/read-write-xml-file-in-c
	/// https://forum.juce.com/t/iterating-xmlelement-and-calling-removechildelement/36745/2
	/// https://forum.juce.com/t/solved-using-the-xmlelement-writeto-method/42948
	/// https://forum.juce.com/t/basic-xml-file-creation-help/24418
	/// https://forum.juce.com/t/directoryiterator/781/13
    /// </summary>
    
	try
	{
		juce::XmlElement trackList("TrackList");

		int trackCount = 0;

		for (auto i = 0; i < trackFile.size(); ++i)
		{
			juce::XmlElement* trackMeta = new juce::XmlElement("musicTracks" + (String)trackCount);

			trackMeta->setAttribute("TrackPath", trackFile[i].getFullPathName());
			trackMeta->setAttribute("TrackLength", trackLength[i]);
			trackMeta->setAttribute("TrackTitle", trackFile[i].getFileNameWithoutExtension());
			trackList.addChildElement(trackMeta);
			trackCount++;
		}

		auto trackFileXML = File::getSpecialLocation(File::userDesktopDirectory).getChildFile("Music_playlist.xml");
		trackList.writeTo(trackFileXML);
	}
	catch (int& err)
	{
		AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "Saving tracks Error: Number ", err + "", "Acknowledged", nullptr);
	}
}

void PlaylistComponent::loadTrack()
{
	/// <summary>
	/// The function's aim is to iterate through the XML file that we have, get the attributes of the files
	/// and pushes them back to a File variable so that it can be retrieved later.
	/// This will be saved in the desktop of the user as Music_playlist
	/// This was inspired by :
	/// https://stackoverflow.com/questions/5415788/read-write-xml-file-in-c
	///https://forum.juce.com/t/iterating-xmlelement-and-calling-removechildelement/36745/2
	///https://forum.juce.com/t/solved-using-the-xmlelement-writeto-method/42948
	///https://forum.juce.com/t/basic-xml-file-creation-help/24418
	///https://forum.juce.com/t/directoryiterator/781/13
	/// </summary>
	
	try
	{
		//Read XML file
		auto trackFilesXML = juce::File::getSpecialLocation(juce::File::userDesktopDirectory).getChildFile("Music_playlist.xml");
		juce::XmlDocument xmlMusic{ trackFilesXML };
		std::unique_ptr<juce::XmlElement> mainElementMusic = xmlMusic.getDocumentElement();
		//Scan XML file to reload previous app status
		if (mainElementMusic == 0)
		{
		}
		else
		{
			if (mainElementMusic->hasTagName(("TrackList")))
			{
				for (auto* element : mainElementMusic->getChildIterator())
				{
					juce::File trackFileXML{};
					juce::String trackLengthXML{};
					juce::URL trackPathXML{};
					juce::String trackPathExtXML{};
					if (element->hasAttribute("TrackPath"))
					{
						trackFileXML = juce::File(element->getStringAttribute("TrackPath"));
					}

					if (element->hasAttribute("TrackLength"))
					{
						trackLengthXML = element->getStringAttribute("TrackLength");
					}
					if (element->hasAttribute("TrackTitle"))
					{
						trackPathExtXML = element->getStringAttribute("TrackTitle");
					}

					dataInsert(trackPathExtXML,
						trackFileXML,
						trackPathXML,
						trackLengthXML);
				}
			}
		}
	}
	catch (int& err)
	{
		AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "Loading tracks Error: Number ", err + "", "Acknowledged", nullptr);
	}
}

void PlaylistComponent::dataInsert(String title, 
	File file, 
	URL path, 
	String length)
{
	try
	{
		trackTitle.add(title);
		trackFile.add(file);
		trackPath.add(path);
		trackLength.add(length);

		trackTitleSearch.add(title);
		trackFileSearch.add(file);
		trackPathSearch.add(path);
		trackLengthSearch.add(length);
	}
	catch (int& err)
	{
		AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "Data Insertion Error: Number ", err + "", "Acknowledged", nullptr);
	}

}

void PlaylistComponent::dataRemove(int index)
{
	try
	{
		trackFile.removeAndReturn(index);
		trackPath.removeAndReturn(index);
		trackTitle.removeAndReturn(index);
		trackLength.removeAndReturn(index);

		trackTitleSearch.removeAndReturn(index);
		trackFileSearch.removeAndReturn(index);
		trackPathSearch.removeAndReturn(index);
		trackLengthSearch.removeAndReturn(index);
	}
	catch (int& err)
	{
		AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "Data Removal Error: Number ", err + "", "Acknowledged", nullptr);
	}

}

void PlaylistComponent::textEditorTextChanged(juce::TextEditor& textEditor)
{
	/// <summary>
	/// This was kinda of inspired by the filter class and a problem occurred on the forum
	/// https://docs.juce.com/master/classFileFilter.html
	/// https://forum.juce.com/t/listbox-deselected-problems/304/4
	/// Also:
	/// https://docs.juce.com/master/classString.html#ac577d2b2fc82516d0a8017e8329ae479
	/// The aim of it is to highlight on the searched string
	/// </summary>
	/// <param name="textEditor"></param>
	/// 
	try
	{
		trackTitleSearch.clear();
		trackFileSearch.clear();
		trackPathSearch.clear();
		trackLengthSearch.clear();

		juce::String keyword = textEditor.getText();

		for (auto& myFile : trackFile)
		{
			if (myFile.exists())
			{
				File FileFound(myFile);
				String title = FileFound.getFileName();
				String length = getTrackLength(FileFound);
				URL trackPath = FileFound.getCurrentWorkingDirectory().getFullPathName();

				if (title.containsIgnoreCase(keyword))
				{
					trackTitleSearch.add(title);
					trackFileSearch.add(FileFound);
					trackPathSearch.add(trackPath);
					trackLengthSearch.add(length);
				}
			}
			tableComponent.updateContent();
			tableComponent.repaint();
		}
	}
	catch (int& err)
	{
		AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "Search Error: Number ", err + "", "Acknowledged", nullptr);
	}
}