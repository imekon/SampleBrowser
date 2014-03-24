#include "DirectoryModel.h"

DirectoryModel::DirectoryModel() : Thread("sample file reader")
{

}

int DirectoryModel::getNumRows()
{
	return samples.size();
}

void DirectoryModel::paintRowBackground(Graphics &g, int row, int width, int height, bool selected)
{
	if (selected)
	{
		g.setColour(Colours::lightblue);
	}
	else
	{
		if (row % 2 == 1)
		{
			g.setColour(Colours::aliceblue);
		}
		else
		{
			g.setColour(Colours::white);
		}
	}

	g.fillRect(0, 0, width, height);
}

void DirectoryModel::paintCell(Graphics &g, int rowNumber, int columnId, int width, int height, bool)
{
	SampleFile *sample = samples[rowNumber];

	String text;

	switch (columnId)
	{
	case name:
		text = sample->getName();
		break;

	case sampleRate:
		text = String(sample->getSampleRate());
		break;

	case channels:
		text = String(sample->getChannels());
		break;

	case bits:
		text = String(sample->getBits());
		break;

	case size:
		text = String(sample->getSize());
		break;
	}

	g.setColour(Colours::black);
	g.drawText(text, 2, 2, width - 2, height - 2, Justification::right, true);
}

void DirectoryModel::clear()
{
	stopThread(300);
	samples.clear();
}

void DirectoryModel::addSample(const File &file)
{
	int row = samples.size();
	SampleFile *sample = new SampleFile(this, row, file.getFileNameWithoutExtension(), file.getFullPathName());
	samples.add(sample);
}

void DirectoryModel::startReadingFiles()
{
	startThread(3);
}

void DirectoryModel::run()
{
	for (int i = 0; i < samples.size(); i++)
	{
		if (threadShouldExit())
			return;

		SampleFile *sample = samples[i];
		sample->readContents();
	}
}

void DirectoryModel::notifyListeners(int row)
{
	for (int i = 0; i < listeners.size(); i++)
	{
		listeners[i]->updatedSample(row);
	}
}

void DirectoryModel::selectedRowsChanged(int row)
{
	for (int i = 0; i < listeners.size(); i++)
	{
		listeners[i]->selectSample(row != -1 ? samples[row] : nullptr);
	}
}
