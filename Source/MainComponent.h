/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "DirectoryModel.h"
#include "ThumbnailComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public Component, 
	public Button::Listener,
	public FileBrowserListener, 
	public Slider::Listener, 
	public ChangeListener,
	public DirectoryModel::Listener
{
public:
    //==============================================================================
    MainComponent();
    virtual ~MainComponent();
    void resized() override;

	void selectionChanged() override;
	void fileClicked(const File &file, const MouseEvent &e) override;
	void fileDoubleClicked(const File &file) override;
	void browserRootChanged(const File &newRoot) override;

	void sliderValueChanged(Slider* slider) override;

	void buttonClicked(Button *button) override;

	void changeListenerCallback(ChangeBroadcaster *source) override;

	void selectSample(SampleFile *sample) override;
	void updatedSample(int row) override;

private:
	AudioDeviceManager deviceManager;
	AudioFormatManager formatManager;

	DirectoryModel model;
	TimeSliceThread thread;
	DirectoryContentsList directoryList;
	FileTreeComponent treeList;
	TableListBox fileList;
	Slider zoomSlider;
	TextButton playButton;
	TextButton stopButton;
	AudioSourcePlayer audioSourcePlayer;
	AudioTransportSource transportSource;
	ScopedPointer<AudioFormatReaderSource> currentAudioFileSource;

	ScopedPointer<ThumbnailComponent> thumbnail;

	void loadFileIntoTransport(const File &file);
	void playSample();
	void stopSample();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
