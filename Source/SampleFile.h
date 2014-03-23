#ifndef _SAMPLEFILE_H
#define _SAMPLEFILE_H

#include "../JuceLibraryCode/JuceHeader.h"

class SampleFile
{
public:
	SampleFile(StringRef shortName, StringRef fileName, int sampleRate, int bits, int size)
	{
		this->shortName = shortName.text;
		this->fileName = fileName.text;
		this->sampleRate = sampleRate;
		this->bits = bits;
		this->size = size;
	}

	String getName() const { return shortName; }
	int getSampleRate() const { return sampleRate; }
	int getBits() const { return bits; }
	int getSize() const { return size; }

private:
	String shortName;
	String fileName;
	int sampleRate;
	int bits;
	int size;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleFile)
};

#endif // _SAMPLEFILE_H
