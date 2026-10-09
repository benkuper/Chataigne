#if CHATAIGNE_VIDEO_RUNTIME_ONLY
#if defined(_WIN32)
#include "AppConfig.h"
#endif
#include "Module/modules/video/VideoNetworkRuntime.h"
#else
#include "MainIncludes.h"
#include "Module/ModuleIncludes.h"
#endif
#include <iostream>
#include <stdexcept>
#include <thread>
#undef main
using namespace juce;

#if ! CHATAIGNE_VIDEO_RUNTIME_ONLY
class VideoNetworkTestApplication : public OrganicApplication
{
public:
    VideoNetworkTestApplication() : OrganicApplication("Video network tests", false) {}
    void initialiseInternal(const String&) override {}
};
#endif

static void check(bool condition, const char* message)
{
    if (!condition) throw std::runtime_error(message);
}

template<class Function>
static Function symbol(DynamicLibrary& library, const char* name)
{
    auto function = reinterpret_cast<Function>(library.getFunction(name));
    check(function != nullptr, name);
    return function;
}

static void checkPicture(const uint8_t* data, int stride)
{
    check(data != nullptr && stride >= 64 * 4, "Received pixels/stride");
    const auto* top = data + 8 * stride + 8 * 4;
    const auto* bottom = data + 24 * stride + 8 * 4;
    check(top[2] > 180 && top[0] < 60, "Red top: BGRA channel order and orientation");
    check(bottom[0] > 180 && bottom[2] < 60, "Blue bottom: BGRA channel order and orientation");
}

static void testNDI(const std::vector<uint8_t>& pixels)
{
    auto& api = VideoNetworkRuntime::NDI::get();
    check(api.load(), "NDI runtime load");
    auto getSource = symbol<decltype(&NDIlib_send_get_source_name)>(api.library, "NDIlib_send_get_source_name");
    auto createReceiver = symbol<decltype(&NDIlib_recv_create_v3)>(api.library, "NDIlib_recv_create_v3");
    auto capture = symbol<decltype(&NDIlib_recv_capture_v2)>(api.library, "NDIlib_recv_capture_v2");
    auto freeVideo = symbol<decltype(&NDIlib_recv_free_video_v2)>(api.library, "NDIlib_recv_free_video_v2");
    auto destroyReceiver = symbol<decltype(&NDIlib_recv_destroy)>(api.library, "NDIlib_recv_destroy");
    NDIlib_send_create_t settings;
    settings.p_ndi_name = "Chataigne NDI loopback test";
    settings.clock_video = false;
    auto sender = api.send_create(&settings);
    check(sender != nullptr, "NDI sender");
    NDIlib_recv_create_v3_t receiverSettings;
    receiverSettings.source_to_connect_to = *getSource(sender);
    receiverSettings.source_to_connect_to.p_url_address = "127.0.0.1:5961";
    receiverSettings.color_format = NDIlib_recv_color_format_BGRX_BGRA;
    auto receiver = createReceiver(&receiverSettings);
    check(receiver != nullptr, "NDI receiver");
    std::atomic<bool> sending { true };
    std::thread worker([&] {
        NDIlib_video_frame_v2_t frame(64, 32, NDIlib_FourCC_video_type_BGRX, 29970, 1000,
            2.0f, NDIlib_frame_format_type_progressive, NDIlib_send_timecode_synthesize,
            const_cast<uint8_t*>(pixels.data()), 64 * 4);
        while (sending) { api.send_send_video_v2(sender, &frame); Thread::sleep(30); }
    });
    NDIlib_video_frame_v2_t received;
    bool found = false;
    for (int i = 0; i < 100 && !found; ++i)
        found = capture(receiver, &received, nullptr, nullptr, 100) == NDIlib_frame_type_video;
    sending = false;
    worker.join();
    if (found)
    {
        check(received.xres == 64 && received.yres == 32, "NDI dimensions");
        check(std::abs(double(received.frame_rate_N) / received.frame_rate_D - 29.97) < .001, "NDI fractional FPS");
        checkPicture(received.p_data, received.line_stride_in_bytes);
        freeVideo(receiver, &received);
    }
    destroyReceiver(receiver);
    api.send_destroy(sender);
    check(found, "NDI loopback frame received");
    std::cerr << "NDI local transmission: pixels, dimensions, fractional FPS passed" << std::endl;
}

static void testOMT(const std::vector<uint8_t>& pixels)
{
    auto& api = VideoNetworkRuntime::OMT::get();
    check(api.load(), "OMT runtime and codec load");
    auto setInteger = symbol<decltype(&omt_settings_set_integer)>(api.library, "omt_settings_set_integer");
    auto createReceiver = symbol<decltype(&omt_receive_create)>(api.library, "omt_receive_create");
    auto receive = symbol<decltype(&omt_receive)>(api.library, "omt_receive");
    auto destroyReceiver = symbol<decltype(&omt_receive_destroy)>(api.library, "omt_receive_destroy");
    setInteger("NetworkPortStart", 26400);
    setInteger("NetworkPortEnd", 26400);
    auto sender = api.send_create("Chataigne OMT loopback test", OMTQuality_High);
    check(sender != nullptr, "OMT sender");
    auto receiver = createReceiver("omt://127.0.0.1:26400", OMTFrameType_Video, OMTPreferredVideoFormat_BGRA, OMTReceiveFlags_None);
    check(receiver != nullptr, "OMT receiver");
    std::atomic<bool> sending { true };
    std::thread worker([&] {
        OMTMediaFrame frame = {};
        frame.Type = OMTFrameType_Video;
        frame.Codec = OMTCodec_BGRA;
        frame.Width = 64; frame.Height = 32; frame.Stride = 64 * 4;
        frame.FrameRateN = 29970; frame.FrameRateD = 1000;
        frame.AspectRatio = 2.0f; frame.ColorSpace = OMTColorSpace_BT601;
        frame.Timestamp = -1;
        frame.Data = const_cast<uint8_t*>(pixels.data()); frame.DataLength = int(pixels.size());
        while (sending) api.send(sender, &frame);
    });
    OMTMediaFrame* received = nullptr;
    for (int i = 0; i < 100 && received == nullptr; ++i) received = receive(receiver, OMTFrameType_Video, 100);
    sending = false;
    worker.join();
    if (received != nullptr)
    {
        check(received->Width == 64 && received->Height == 32, "OMT dimensions");
        check(std::abs(double(received->FrameRateN) / received->FrameRateD - 29.97) < .001, "OMT fractional FPS");
        checkPicture(static_cast<uint8_t*>(received->Data), received->Stride);
    }
    destroyReceiver(receiver);
    api.send_destroy(sender);
    check(received != nullptr, "OMT loopback frame received");
    std::cerr << "OMT local transmission: pixels, dimensions, fractional FPS passed" << std::endl;
}

#if ! CHATAIGNE_VIDEO_RUNTIME_ONLY
static void testModules()
{
    NDIOutModule ndi;
    OMTOutModule omt;
    ndi.fps->setValue(29.97f); omt.fps->setValue(59.94f);
    ndi.width->setValue(64); omt.width->setValue(64);
    ndi.height->setValue(32); omt.height->setValue(32);
    for (int i = 0; i < 20; ++i)
    {
        ndi.enabled->setValue(false); omt.enabled->setValue(false);
        ndi.streamName->setValue("NDI lifecycle " + String(i));
        omt.streamName->setValue("OMT lifecycle " + String(i));
        ndi.enabled->setValue(true); omt.enabled->setValue(true);
        Thread::sleep(10);
    }
    ndi.clearItem(); omt.clearItem();
    std::cerr << "Module reconfiguration and worker shutdown passed" << std::endl;
}
#endif

int main(int argc, char** argv)
{
    if (argc > 1 && String(argv[1]) == "--expect-missing")
    {
        if (VideoNetworkRuntime::NDI::get().load() || VideoNetworkRuntime::OMT::get().load()) return 1;
        std::cerr << "Missing runtimes handled without startup failure" << std::endl;
        return 0;
    }
#if ! CHATAIGNE_VIDEO_RUNTIME_ONLY
    ScopedJuceInitialiser_GUI gui;
    VideoNetworkTestApplication app;
    app.engine.reset(new ChataigneEngine());
#endif
    int result = 0;
    try
    {
        std::vector<uint8_t> pixels(64 * 32 * 4, 255);
        for (int y = 0; y < 32; ++y)
            for (int x = 0; x < 64; ++x)
            {
                auto* pixel = pixels.data() + (y * 64 + x) * 4;
                pixel[0] = y < 16 ? 0 : 255; pixel[1] = 0; pixel[2] = y < 16 ? 255 : 0;
            }
        testNDI(pixels);
#if ! defined(__arm__) || defined(__aarch64__)
        testOMT(pixels);
#endif
#if ! CHATAIGNE_VIDEO_RUNTIME_ONLY
        testModules();
#endif
        std::cerr << "Video network integration tests passed" << std::endl;
    }
    catch (const std::exception& e) { std::cerr << e.what() << '\n'; result = 1; }
#if ! CHATAIGNE_VIDEO_RUNTIME_ONLY
    app.engine.reset();
#endif
    return result;
}
