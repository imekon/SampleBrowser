#include "DirectoryModel.h"

DirectoryModel::DirectoryModel()
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

void DirectoryModel::paintCell(Graphics &g, int rowNumber, int columnId, int width, int height, bool selected)
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

	case bits:
		text = String(sample->getBits());
		break;

	case size:
		text = String(sample->getSize());
		break;
	}

	g.setColour(Colours::black);
	g.drawText(text, 2, 2, width - 2, height - 2, Justification::centred, true);
}

void DirectoryModel::clear()
{
	samples.clear();
}

void DirectoryModel::addSample(const File &file)
{
	SampleFile *sample = new SampleFile(file.getFileNameWithoutExtension(), file.getFullPathName(), 0, 0, 0);
	samples.add(sample);
}