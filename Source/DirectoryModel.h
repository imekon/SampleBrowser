#ifndef _DIRECTORYMODEL_H
#define _DIRECTORYMODEL_H

#include "SampleFile.h"

class DirectoryModel : public TableListBoxModel
{
public:
	enum Columns
	{
		name = 1,
		sampleRate,
		bits,
		size
	};

	DirectoryModel();

	int getNumRows() override;
	void paintRowBackground(Graphics &g, int row, int width, int height, bool selected) override;
	void paintCell(Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

	void clear();
	void addSample(const File &file);

private:
	OwnedArray<SampleFile> samples;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DirectoryModel)
};

#endif // _DIRECTORYMODEL_H
