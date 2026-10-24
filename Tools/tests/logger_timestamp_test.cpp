#include <juce_events/juce_events.h>
#include "juce_organicui/helpers/DebugHelpers.h"
#include "juce_organicui/helpers/QueuedNotifier.h"
#include "juce_organicui/logger/CustomLogger.h"
#include <iostream>
#include <stdexcept>
#include <thread>

using namespace juce;

// The logger only needs the application's name and version from its host.
struct LoggerTestApplication
{
    String getApplicationName() const { return "Chataigne logger timestamp test"; }
};
static LoggerTestApplication& getApp() { static LoggerTestApplication app; return app; }
namespace ProjectInfo { const char* versionString = "test"; }
#include "juce_organicui/logger/CustomLogger.cpp"

static void check(bool condition, const char* message)
{
    if (!condition) throw std::runtime_error(message);
}

int main()
{
    ScopedJuceInitialiser_GUI initialise;
    try
    {
        CustomLogger logger;
        const auto logFile = File::createTempFile(".log");
        CustomLogger::FileWriter writer;
        const auto defaultLogFile = writer.fileLog->getLogFile();
        writer.fileLog.reset(new FileLogger(logFile, ""));
        defaultLogFile.deleteFile();

        struct TestCase { int hour, minute, second, millisecond; const char* timestamp; };
        const TestCase cases[] = {
            { 0, 0, 0, 0, "00:00:00.000" },
            { 1, 2, 3, 7, "01:02:03.007" },
            { 12, 34, 56, 80, "12:34:56.080" },
            { 23, 59, 59, 999, "23:59:59.999" }
        };
        const char* prefixes[] = { "", "!", "!!", "!!!" };
        const char* severities[] = { "info", "debug", "warning", "error" };
        for (const auto& test : cases)
        {
            for (int severity = 0; severity < 4; ++severity)
            {
                LogElement element(String("Show::") + prefixes[severity] + "Cue started\nSecond line");
                element.time = Time(2026, 9, 24, test.hour, test.minute, test.second, test.millisecond);
                const CustomLogger::LogEvent event(&element);
                const auto expected = String(test.timestamp) + " [" + severities[severity]
                    + "] Show : Cue started\nSecond line";
                check(element.getFullLog() == expected, "LogElement timestamp, severity and content");
                check(event.getFullLog() == expected, "LogEvent timestamp, severity and content");
                writer.newMessage(event);
                check(logFile.loadFileAsString().replace("\r\n", "\n").endsWith(expected + "\n"),
                    "saved file contains the original event timestamp with milliseconds");
            }
        }

        // Queued events keep their capture time even if the UI log is cleared
        // before the message thread writes them to disk.
        logger.addLogListener(&writer);
        std::thread producer([&] { logger.logMessage("Worker::Queued cue"); });
        producer.join();
        const auto captured = logger.logElements[0]->getFullLog();
        logger.logElements.clear();
        MessageManager::getInstance()->runDispatchLoopUntil(20);
        check(logFile.loadFileAsString().trimEnd().endsWith(captured), "queued log retains its capture time");
        logger.removeLogListener(&writer);

        const auto beforeEmptyMessage = logFile.getSize();
        LogElement empty("Show::");
        writer.newMessage(CustomLogger::LogEvent(&empty));
        check(logFile.getSize() == beforeEmptyMessage, "empty messages remain skipped");
        writer.fileLog.reset();
        logFile.deleteFile();
        std::cout << "Logger timestamp tests passed\n";
        return 0;
    }
    catch (const std::exception& error)
    {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
