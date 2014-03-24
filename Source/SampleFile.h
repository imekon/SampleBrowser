#ifndef _SAMPLEFILE_H
#define _SAMPLEFILE_H

#include "../JuceLibraryCode/JuceHeader.h"

class DirectoryModel;

class SampleFile
{
public:
	SampleFile(DirectoryModel *model, int row, StringRef shortName, StringRef fileName);
	virtual ~SampleFile() { }

	String getFilename() const { return fileName; }
	String getName() const { return shortName; }
	int getSampleRate() const { return sampleRate; }
	int getChannels() const { return channels; }
	int getBits() const { return bits; }
	int getSize() const { return size; }

	void readContents();

private:
	DirectoryModel *model;
	int row;
	String shortName;
	String fileName;
	int sampleRate;
	int channels;
	int bits;
	int size;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleFile)
};

#endif // _SAMPLEFILE_H
