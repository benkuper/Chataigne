/*
  ==============================================================================

    FlapLoggerUI.h
    Created: 6 May 2016 2:13:35pm
    Author:  Martin Hermant

  ==============================================================================
*/

#ifndef FlapLoggerUI_H_INCLUDED
#define FlapLoggerUI_H_INCLUDED

#include "FlapLogger.h"
#include "ShapeShifterContent.h"
#include "DebugHelpers.h"

class FlapLoggerUI : public ShapeShifterContent,
	public FlapLogger::Listener,
	public TextButton::Listener,
public AsyncUpdater
{
public:
	FlapLoggerUI(const String &contentName, FlapLogger * l);
	~FlapLoggerUI();

    class LogList : public juce::TableListBoxModel
	{
    public:
		LogList(FlapLoggerUI * o);

        int getNumRows() override;

        void paintRowBackground (Graphics&,
                                         int rowNumber,
                                         int width, int height,
                                         bool rowIsSelected) override;

         void paintCell (Graphics&,
                                int rowNumber,
                                int columnId,
                                int width, int height,
                                bool rowIsSelected) override;

    private:


        FlapLoggerUI * owner;
    };

	FlapLogger * logger;

    void resized()override;
    LogList logList;
    TextButton clearB;
    ScopedPointer<TableListBox> logListComponent;
    int maxNumElement;

    void buttonClicked (Button*) override;


    OwnedArray<LogElement> logElements;
    void newMessage(const String & ) override;
private:
    int totalLogRow;
    void updateTotalLogRow();
    const String & getSourceForRow(int r);
    const String & getContentForRow(int r);
	String getTimeStringForRow(int r);
    friend class LogList;

    void handleAsyncUpdate()override;

};



#endif  // FlapLoggerUI_H_INCLUDED
