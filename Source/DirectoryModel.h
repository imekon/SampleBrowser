#ifndef _DIRECTORYMODEL_H
#define _DIRECTORYMODEL_H

#include "SampleFile.h"

class DirectoryModel : public TableListBoxModel, public Thread
{
public:
	enum Columns
	{
		name = 1,
		sampleRate,
		channels,
		bits,
		size
	};

	class Listener
	{
	public:
		virtual void selectSample(SampleFile *sample) = 0;
		virtual void updatedSample(int row) = 0;
	};

	DirectoryModel();
	virtual ~DirectoryModel() 
	{
		listeners.clear();
	}

	int getNumRows() override;
	void paintRowBackground(Graphics &g, int row, int width, int height, bool selected) override;
	void paintCell(Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
	void selectedRowsChanged(int lastRowSelected) override;

	void clear();
	void addSample(const File &file);
	void startReadingFiles();

	void addListener(Listener *listener) { listeners.add(listener); }
	void removeListener(Listener *listener)
	{
		int index = listeners.indexOf(listener);
		if (index != -1)
		{
			listeners.remove(index);
		}
	}
	void notifyListeners(int row);

	void run() override;

private:
	OwnedArray<SampleFile> samples;
	Array<Listener *> listeners;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DirectoryModel)
};

#endif // _DIRECTORYMODEL_H
