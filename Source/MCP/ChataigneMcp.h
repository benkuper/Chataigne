#pragma once
#if ORGANICUI_USE_MCP
class ChataigneMcp : public ControllableContainer, public EngineListener,
                     private juce::AsyncUpdater, private CustomLogger::LoggerListener
{
public:
    explicit ChataigneMcp(ChataigneEngine&);
    ~ChataigneMcp() override;
    organic::mcp::Registry registry;
    std::unique_ptr<organic::mcp::Permissions> permissions;
    organic::mcp::OrganicProvider organic;
    organic::mcp::TimelineProvider timeline;
    std::unique_ptr<organic::mcp::Server> server;
    BoolParameter* enabled;
    IntParameter* port;
    StringParameter* status;
    void onContainerParameterChanged(Parameter*) override;
    void startLoadFile() override;
    void engineCleared() override;
    juce::var diagnostics(const juce::var&, organic::mcp::Context&);
    juce::var projectSummary();
private:
    ChataigneEngine& engine;
    juce::Array<juce::var> recentLogs;
    void registerOperations();
    void handleAsyncUpdate() override;
    void newMessage(const CustomLogger::LogEvent&) override;
};
#endif
