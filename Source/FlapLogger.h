/*
 ==============================================================================

 Logger.h
 Created: 6 May 2016 1:37:41pm
 Author:  Martin Hermant

 ==============================================================================
 */

#ifndef FUSIONLOGGER_H_INCLUDED
#define FUSIONLOGGER_H_INCLUDED

#include "QueuedNotifier.h"



class FlapLogger : public Logger {
    public :

    juce_DeclareSingleton(FlapLogger, true);

    FlapLogger():notifier(100){
        addLogListener(&fileWriter);
    }
	
	~FlapLogger() { if (Logger::getCurrentLogger() == this) Logger::setCurrentLogger(nullptr); }

	void logMessage(const String & message) override;


    QueuedNotifier<String> notifier;
    typedef QueuedNotifier<String>::Listener Listener;



    void addLogListener(Listener * l){notifier.addListener(l);}
    void removeLogListener(Listener * l){notifier.removeListener(l);}

    class FileWriter : public Listener{
    public:
        FileWriter(){fileLog = FileLogger::createDefaultAppLogger("F.L.A.P.", "log", "");}

        void newMessage(const String& s) override{if (fileLog) {fileLog->logMessage(s);}}
        String getFilePath(){return fileLog->getLogFile().getFullPathName();}
        ScopedPointer<FileLogger> fileLog;
    };

    FileWriter fileWriter;

};



#endif  // FUSIONLOGGER_H_INCLUDED
