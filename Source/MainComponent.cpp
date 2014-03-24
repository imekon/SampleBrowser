/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : 
	thread("audio preview thread"),
	directoryList(nullptr, thread), 
	treeList(directoryList),
	playButton("Play"),
	stopButton("Stop")
{
	deviceManager.initialise(2, 2, nullptr, true);
	deviceManager.addAudioCallback(&audioSourcePlayer);
	audioSourcePlayer.setSource(&transportSource);

	directoryList.setDirectory(File::getSpecialLocation(File::userHomeDirectory), true, false);
	thread.startThread(3);
	addAndMakeVisible(treeList);
	TableHeaderComponent *header = new TableHeaderComponent();
	header->addColumn("Name", DirectoryModel::name, 120);
	header->addColumn("Sample Rate", DirectoryModel::sampleRate, 100);
	header->addColumn("Channels", DirectoryModel::channels, 90);
	header->addColumn("Bits", DirectoryModel::bits, 70);
	header->addColumn("Size", DirectoryModel::size, 100);
	fileList.setHeader(header);
	fileList.setModel(&model);
	addAndMakeVisible(fileList);
	treeList.addListener(this);

	formatManager.registerBasicFormats();

	addAndMakeVisible(zoomSlider);
	zoomSlider.setRange(0, 1, 0);
	zoomSlider.setSliderStyle(Slider::LinearHorizontal);
	zoomSlider.setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
	zoomSlider.addListener(this);
	zoomSlider.setSkewFactor(2);

	thumbnail = new ThumbnailComponent(formatManager, transportSource, zoomSlider);
	addAndMakeVisible(thumbnail);

	model.addListener(this);

	addAndMakeVisible(playButton);
	addAndMakeVisible(stopButton);

	playButton.addListener(this);
	stopButton.addListener(this);

    setSize (800, 600);
}

MainComponent::~MainComponent()
{
	deviceManager.removeAudioCallback(&audioSourcePlayer);
	audioSourcePlayer.setSource(nullptr);
	transportSource.stop();
	transportSource.setSource(nullptr);
	currentAudioFileSource = nullptr;
	treeList.removeListener(this);
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
	treeList.setBoundsRelative(0.01f, 0.01f, 0.28f, 0.98f);
	fileList.setBoundsRelative(0.30f, 0.01f, 0.68f, 0.78f);
	thumbnail->setBoundsRelative(0.3f, 0.80f, 0.68f, 0.12f);
	zoomSlider.setBoundsRelative(0.3f, 0.93f, 0.2f, 0.05f);
	playButton.setBoundsRelative(0.6f, 0.93f, 0.15f, 0.05f);
	stopButton.setBoundsRelative(0.76f, 0.93f, 0.15f, 0.05f);
}

void MainComponent::selectionChanged()
{
	model.clear();
	File file = treeList.getSelectedFile();
	if (file.isDirectory())
	{
		Array<File> files;

		int count = file.findChildFiles(files, File::findFiles, false, "*.wav");

		for (int i = 0; i < count; i++)
		{
			model.addSample(files[i]);
		}

		model.startReadingFiles();
	}
	fileList.updateContent();
	fileList.repaint();
}

void MainComponent::fileClicked(const File &file, const MouseEvent &e)
{

}

void MainComponent::fileDoubleClicked(const File &file)
{

}

void MainComponent::browserRootChanged(const File &newRoot)
{

}

void MainComponent::changeListenerCallback(ChangeBroadcaster *source)
{
	if (source == thumbnail)
	{
		//showFile(thumbnail->getLastDroppedFile());
	}
}

void MainComponent::sliderValueChanged(Slider* slider)
{
	if (slider == &zoomSlider)
	{
		thumbnail->setZoomFactor(zoomSlider.getValue());
	}
}

void MainComponent::selectSample(SampleFile *sample)
{
	if (sample)
	{
		File file(sample->getFilename());
		loadFileIntoTransport(file);
		thumbnail->setFile(file);
	}
}

void MainComponent::updatedSample(int row)
{
	fileList.repaintRow(row);
}

void MainComponent::loadFileIntoTransport(const File &file)
{
	transportSource.stop();
	transportSource.setSource(nullptr);
	currentAudioFileSource = nullptr;

	AudioFormatReader *reader = formatManager.createReaderFor(file);
	if (reader)
	{
		currentAudioFileSource = new AudioFormatReaderSource(reader, true);
		transportSource.setSource(currentAudioFileSource, 32768, &thread, reader->sampleRate);
	}
}

void MainComponent::buttonClicked(Button *button)
{
	if (button == &playButton)
	{
		playSample();
	}
	else if (button == &stopButton)
	{
		stopSample();
	}
}

void MainComponent::playSample()
{
	if (!transportSource.isPlaying())
	{
		transportSource.setPosition(0);
		transportSource.start();
	}
}

void MainComponent::stopSample()
{
	if (transportSource.isPlaying())
	{
		transportSource.stop();
	}
}

