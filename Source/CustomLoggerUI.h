/*
  ==============================================================================

    CustomLoggerUI.h
    Created: 6 May 2016 2:13:35pm
    Author:  Martin Hermant

  ==============================================================================
*/

#ifndef CustomLoggerUI_H_INCLUDED
#define CustomLoggerUI_H_INCLUDED

#include "CustomLogger.h"
#include "ShapeShifterContent.h"
#include "DebugHelpers.h"

class CustomLoggerUI : 
	public ShapeShifterContentComponent,
	public CustomLogger::Listener,
	public TextButton::Listener,
public AsyncUpdater
{
public:
	CustomLoggerUI(const String &contentName, CustomLogger * l);
	~CustomLoggerUI();

    class LogList : public juce::TableListBoxModel
	{
    public:
		LogList(CustomLoggerUI * o);

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


        CustomLoggerUI * owner;
    };

	CustomLogger * logger;

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



#endif  // CustomLoggerUI_H_INCLUDED
