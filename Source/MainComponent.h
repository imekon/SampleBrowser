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
class MainComponent : public Component, public FileBrowserListener, public Slider::Listener, public ChangeListener
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

	void changeListenerCallback(ChangeBroadcaster *source) override;

private:
	AudioDeviceManager deviceManager;
	AudioFormatManager formatManager;

	DirectoryModel model;
	TimeSliceThread thread;
	DirectoryContentsList directoryList;
	FileTreeComponent treeList;
	TableListBox fileList;
	Slider zoomSlider;
	AudioSourcePlayer audioSourcePlayer;
	AudioTransportSource transportSource;
	ScopedPointer<AudioFormatReaderSource> currentAudioFileSource;

	ScopedPointer<ThumbnailComponent> thumbnail;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
