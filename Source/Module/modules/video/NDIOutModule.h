#pragma once

#include "../../Module.h"
#include "TimeMachine/Sequence/layers/video/ui/CompositionRenderer.h"
#include "TimeMachine/Sequence/layers/video/VideoAudioProcessor.h"
#include "VideoNetworkRuntime.h"
#include <atomic>
#include <cstdint>
#include <map>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <vector>

class NDIOutModule;

// One editable output, with its own saved parameters and a unique runtime route.
// Adding/removing/reordering these items does not change any layer's assignment.
class NDIOutput : public BaseItem
{
public:
    NDIOutput();
    ~NDIOutput() override = default;

    String getTypeString() const override { return "NDI Output"; }

    uint64_t routeId = 0;
    NDIOutModule* module = nullptr;
    StringParameter* streamName = nullptr;
    IntParameter* width = nullptr;
    IntParameter* height = nullptr;
    FloatParameter* fps = nullptr;
    BoolParameter* testCard = nullptr;

    void onContainerParameterChangedInternal(Parameter* p) override;
    void onContainerNiceNameChanged() override;
};

class NDIOutputManager : public BaseManager<NDIOutput>
{
public:
    explicit NDIOutputManager(NDIOutModule* owner);
    NDIOutput* createItem() override;
    NDIOutModule* owner = nullptr;
};

class NDIOutModule : public Module,
    private juce::Thread,
    private BaseManagerListener<NDIOutput>
{
public:
    NDIOutModule();
    ~NDIOutModule() override;

    static Module* create() { return new NDIOutModule(); }
    String getTypeString() const override { return "NDI out"; }

    NDIOutputManager outputs;

    void updateOutput();
    void onContainerParameterChangedInternal(Parameter* p) override;
    void loadJSONDataItemInternal(var data) override;
    void clearItem() override;

    // PCM comes from the mpv pipes, independently of the Sound Card audio graph.
    static void publishAudio(const void* source, uint64_t routeId,
        const float* interleavedStereo, int frames, int sampleRate);
    static void removeAudioSource(const void* source);

private:
    struct OutputConfig
    {
        uint64_t id = 0;
        String name;
        int width = 1280;
        int height = 720;
        int frameRate = 30000; // numerator; denominator is 1000
    };

    struct SenderState
    {
        NDIlib_send_instance_t sender = nullptr;
        String name;
        int frameRate = 30000;
        double audioFraction = 0;
        std::chrono::steady_clock::time_point nextDue;
    };

    struct AudioSource
    {
        uint64_t routeId = 0;
        int sampleRate = 0;
        double nextInputPosition = 0;
        float lastLeft = 0, lastRight = 0;
        bool hasPrevious = false;
        std::unique_ptr<AudioFIFO> fifo;
        bool primed = false;
    };

    // Message-thread surfaces; the GL holder writes their latest images.
    std::map<uint64_t, std::unique_ptr<CompositionRenderer::CompositionSurface>> surfaces;
    std::vector<OutputConfig> desiredOutputs;
    std::mutex stateLock;
    std::condition_variable stateSignal;
    bool configDirty = true;

    // Only the worker thread touches these NDI sender handles.
    std::map<uint64_t, SenderState> senders;

    std::mutex audioLock;
    std::map<const void*, AudioSource> audioSources;

    void run() override;
    void configureSenders(const std::vector<OutputConfig>& wanted);
    void destroySender(SenderState& s);
    void sendLatestFrame(uint64_t id, SenderState& s);
    void sendLatestAudio(uint64_t id, SenderState& s);
    void enqueueAudio(const void* source, uint64_t routeId,
        const float* samples, int frames, int sampleRate);
    void forgetAudio(const void* source);

    void itemAdded(NDIOutput*) override;
    void itemsAdded(Array<NDIOutput*>) override;
    void itemRemoved(NDIOutput*) override;
    void itemsRemoved(Array<NDIOutput*>) override;
    void itemsReordered() override;

    bool cleanedUp = false;
    void shutdown();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NDIOutModule)
};
