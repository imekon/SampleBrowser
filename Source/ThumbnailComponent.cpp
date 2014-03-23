#include "ThumbnailComponent.h"

ThumbnailComponent::ThumbnailComponent(AudioFormatManager& formatManager,
	AudioTransportSource& transportSource_,
	Slider& slider) : transportSource(transportSource_),
	zoomSlider(slider),
	scrollbar(false),
	thumbnailCache(5),
	thumbnail(512, formatManager, thumbnailCache),
	isFollowingTransport(false)
{
	thumbnail.addChangeListener(this);

	addAndMakeVisible(scrollbar);
	scrollbar.setRangeLimits(visibleRange);
	scrollbar.setAutoHide(false);
	scrollbar.addListener(this);

	currentPositionMarker.setFill(Colours::white.withAlpha(0.85f));
	addAndMakeVisible(currentPositionMarker);
}

ThumbnailComponent::~ThumbnailComponent()
{
	scrollbar.removeListener(this);
	thumbnail.removeChangeListener(this);
}

void ThumbnailComponent::setFile(const File& file)
{
	if (!file.isDirectory())
	{
		thumbnail.setSource(new FileInputSource(file));
		const Range<double> newRange(0.0, thumbnail.getTotalLength());
		scrollbar.setRangeLimits(newRange);
		setRange(newRange);

		startTimer(1000 / 40);
	}
}

File ThumbnailComponent::getLastDroppedFile() const noexcept
{
	return lastFileDropped;
}

void ThumbnailComponent::setZoomFactor(double amount)
{
	if (thumbnail.getTotalLength() > 0)
	{
		const double newScale = jmax(0.001, thumbnail.getTotalLength() * (1.0 - jlimit(0.0, 0.99, amount)));
		const double timeAtCentre = xToTime(getWidth() / 2.0f);
		setRange(Range<double>(timeAtCentre - newScale * 0.5, timeAtCentre + newScale * 0.5));
	}
}

void ThumbnailComponent::setRange(Range<double> newRange)
{
	visibleRange = newRange;
	scrollbar.setCurrentRange(visibleRange);
	updateCursorPosition();
	repaint();
}

void ThumbnailComponent::setFollowsTransport(bool shouldFollow)
{
	isFollowingTransport = shouldFollow;
}

void ThumbnailComponent::paint(Graphics& g)
{
	g.fillAll(Colours::darkgrey);
	g.setColour(Colours::lightblue);

	if (thumbnail.getTotalLength() > 0.0)
	{
		Rectangle<int> thumbArea(getLocalBounds());
		thumbArea.removeFromBottom(scrollbar.getHeight() + 4);
		thumbnail.drawChannels(g, thumbArea.reduced(2),
			visibleRange.getStart(), visibleRange.getEnd(), 1.0f);
	}
	else
	{
		g.setFont(14.0f);
		g.drawFittedText("(No audio file selected)", getLocalBounds(), Justification::centred, 2);
	}
}

void ThumbnailComponent::resized()
{
	scrollbar.setBounds(getLocalBounds().removeFromBottom(14).reduced(2));
}

void ThumbnailComponent::changeListenerCallback(ChangeBroadcaster*)
{
	// this method is called by the thumbnail when it has changed, so we should repaint it..
	repaint();
}

bool ThumbnailComponent::isInterestedInFileDrag(const StringArray& /*files*/)
{
	return true;
}

void ThumbnailComponent::filesDropped(const StringArray& files, int /*x*/, int /*y*/)
{
	lastFileDropped = File(files[0]);
	sendChangeMessage();
}

void ThumbnailComponent::mouseDown(const MouseEvent& e)
{
	mouseDrag(e);
}

void ThumbnailComponent::mouseDrag(const MouseEvent& e)
{
	if (canMoveTransport())
		transportSource.setPosition(jmax(0.0, xToTime((float)e.x)));
}

void ThumbnailComponent::mouseUp(const MouseEvent&)
{
	transportSource.start();
}

void ThumbnailComponent::mouseWheelMove(const MouseEvent&, const MouseWheelDetails& wheel)
{
	if (thumbnail.getTotalLength() > 0.0)
	{
		double newStart = visibleRange.getStart() - wheel.deltaX * (visibleRange.getLength()) / 10.0;
		newStart = jlimit(0.0, jmax(0.0, thumbnail.getTotalLength() - (visibleRange.getLength())), newStart);

		if (canMoveTransport())
			setRange(Range<double>(newStart, newStart + visibleRange.getLength()));

		if (wheel.deltaY != 0.0f)
			zoomSlider.setValue(zoomSlider.getValue() - wheel.deltaY);

		repaint();
	}
}

float ThumbnailComponent::timeToX(const double time) const
{
	return getWidth() * (float)((time - visibleRange.getStart()) / (visibleRange.getLength()));
}

double ThumbnailComponent::xToTime(const float x) const
{
	return (x / getWidth()) * (visibleRange.getLength()) + visibleRange.getStart();
}

bool ThumbnailComponent::canMoveTransport() const noexcept
{
	return !(isFollowingTransport && transportSource.isPlaying());
}

void ThumbnailComponent::scrollBarMoved(ScrollBar* scrollBarThatHasMoved, double newRangeStart)
{
	if (scrollBarThatHasMoved == &scrollbar)
		if (!(isFollowingTransport && transportSource.isPlaying()))
			setRange(visibleRange.movedToStartAt(newRangeStart));
}

void ThumbnailComponent::timerCallback()
{
	if (canMoveTransport())
		updateCursorPosition();
	else
		setRange(visibleRange.movedToStartAt(transportSource.getCurrentPosition() - (visibleRange.getLength() / 2.0)));
}

void ThumbnailComponent::updateCursorPosition()
{
	currentPositionMarker.setVisible(transportSource.isPlaying() || isMouseButtonDown());

	currentPositionMarker.setRectangle(Rectangle<float>(timeToX(transportSource.getCurrentPosition()) - 0.75f, 0,
		1.5f, (float)(getHeight() - scrollbar.getHeight())));
}
