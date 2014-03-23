#ifndef _THUMBNAILCOMPONENT_H
#define _THUMBNAILCOMPONENT_H

#include "../JuceLibraryCode/JuceHeader.h"

class ThumbnailComponent : public Component,
	public ChangeListener,
	public FileDragAndDropTarget,
	public ChangeBroadcaster,
	private ScrollBar::Listener,
	private Timer
{
public:
	ThumbnailComponent(AudioFormatManager& formatManager,
		AudioTransportSource& transportSource_,
		Slider& slider);

	virtual ~ThumbnailComponent();

	void setFile(const File& file);
	File getLastDroppedFile() const noexcept;
	void setZoomFactor(double amount);
	void setRange(Range<double> newRange);
	void setFollowsTransport(bool shouldFollow);

	void paint(Graphics& g) override;
	void resized() override;
	void changeListenerCallback(ChangeBroadcaster*) override;
	bool isInterestedInFileDrag(const StringArray& /*files*/) override;
	void filesDropped(const StringArray& files, int /*x*/, int /*y*/) override;
	void mouseDown(const MouseEvent& e) override;
	void mouseDrag(const MouseEvent& e) override;
	void mouseUp(const MouseEvent&) override;
	void mouseWheelMove(const MouseEvent&, const MouseWheelDetails& wheel) override;

private:
	AudioTransportSource& transportSource;
	Slider& zoomSlider;
	ScrollBar scrollbar;

	AudioThumbnailCache thumbnailCache;
	AudioThumbnail thumbnail;
	Range<double> visibleRange;
	bool isFollowingTransport;
	File lastFileDropped;

	DrawableRectangle currentPositionMarker;

	float timeToX(const double time) const;
	double xToTime(const float x) const;
	bool canMoveTransport() const noexcept;
	void scrollBarMoved(ScrollBar* scrollBarThatHasMoved, double newRangeStart) override;
	void timerCallback() override;
	void updateCursorPosition();
};

#endif // _THUMBNAILCOMPONENT_H
