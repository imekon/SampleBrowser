#include "MainWindow.h"

MainWindow::MainWindow() : DocumentWindow("Sample Browser",
	Colours::lightgrey,
	DocumentWindow::allButtons)
{
	setContentOwned(new MainComponent(), true);
	setResizable(true, true);
	centreWithSize(getWidth(), getHeight());
	setVisible(true);
}

void MainWindow::closeButtonPressed()
{
	// This is called when the user tries to close this window. Here, we'll just
	// ask the app to quit when this happens, but you can change this to do
	// whatever you need.
	JUCEApplication::getInstance()->systemRequestedQuit();
}

