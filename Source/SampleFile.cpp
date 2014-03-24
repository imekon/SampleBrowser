#include "SampleFile.h"
#include "DirectoryModel.h"

SampleFile::SampleFile(DirectoryModel *model, int row, StringRef shortName, StringRef fileName)
{
	this->model = model;
	this->row = row;
	this->shortName = shortName.text;
	this->fileName = fileName.text;
	sampleRate = 0;
	channels = 0;
	bits = 0;
	size = 0;
}

void SampleFile::readContents()
{
	File file(fileName);
	FileInputStream *stream = new FileInputStream(file);
	WavAudioFormat format;
	ScopedPointer<AudioFormatReader> reader = format.createReaderFor(stream, true);
	if (reader)
	{
		sampleRate = (int)reader->sampleRate;
		bits = reader->bitsPerSample;
		channels = reader->numChannels;
		size = (int)reader->lengthInSamples;
	}

	const MessageManagerLock lock(Thread::getCurrentThread());
	if (!lock.lockWasGained())
		return;

	model->notifyListeners(row);
}