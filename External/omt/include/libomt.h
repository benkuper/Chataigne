/*
* MIT License
*
* Copyright (c) 2025 Open Media Transport Contributors
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*/

#pragma once
#ifdef _MSC_VER
#pragma comment(lib, "libomt.lib")
#endif

#define OMT_MAX_STRING_LENGTH 1024

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

typedef enum OMTFrameType
{
    OMTFrameType_None = 0,
    OMTFrameType_Metadata = 1,
    OMTFrameType_Video = 2,
    OMTFrameType_Audio = 4,
    OMTFrameType_INT32 = 0x7fffffff //Ensure int type in C
} OMTFrameType;

/**
* Supported Codecs:
*
* VMX1 = Fast video codec
*
* UYVY = 16bpp YUV format
*
* YUY2 = 16bpp YUV format YUYV pixel order
* 
* UYVA = 16pp YUV format immediately followed by an alpha plane
* 
* NV12 = Planar 4:2:0 YUV format. Y plane followed by interleaved half height U/V plane.
* 
* YV12 = Planar 4:2:0 YUV format. Y plane followed by half height U and V planes.
*
* BGRA = 32bpp RGBA format (Same as ARGB32 on Win32)
* 
* P216 = Planar 4:2:2 YUV format. 16bit Y plane followed by interlaved 16bit UV plane.
* 
* PA16 = Same as P216 folowed by an additional 16bit alpha plane.
*
* FPA1 = Floating-point Planar Audio 32bit
*/
typedef enum OMTCodec
{
    OMTCodec_VMX1 = 0x31584D56,
    OMTCodec_FPA1 = 0x31415046, //Planar audio
    OMTCodec_UYVY = 0x59565955,
    OMTCodec_YUY2 = 0x32595559,
    OMTCodec_BGRA = 0x41524742,
    OMTCodec_NV12 = 0x3231564E,
    OMTCodec_YV12 = 0x32315659,
    OMTCodec_UYVA = 0x41565955,
    OMTCodec_P216 = 0x36313250,
    OMTCodec_PA16 = 0x36314150

} OMTCodec;

/**
* Specify the video encoding quality.
* 
* If set to Default, the Sender is configured to allow suggestions from all Receivers.
* 
* The highest suggest amongst all receivers is then selected.
* 
* If a Receiver is set to Default, then it will defer the quality to whatever is set amongst other Receivers.
*/
typedef enum OMTQuality
{
    OMTQuality_Default = 0,
    OMTQuality_Low = 1,
    OMTQuality_Medium = 50,
    OMTQuality_High = 100,
    OMTQuality_INT32 = 0x7fffffff //Ensure int type in C
} OMTQuality;

/**
* Specify the color space of the uncompressed Frame. This is used to determine the color space for YUV<>RGB conversions internally.
* 
* If undefined, the codec will assume BT601 for heights < 720, BT709 for everything else.
* 
*/
typedef enum OMTColorSpace
{
    OMTColorSpace_Undefined = 0,
    OMTColorSpace_BT601 = 601,
    OMTColorSpace_BT709 = 709,
    OMTColorSpace_INT32 = 0x7fffffff //Ensure int type in C
} OMTColorSpace;

/**
* Flags set on video frames:
* 
* Interlaced: Frames are interlaced
* 
* Alpha: Frames contain an alpha channel. If this is not set, BGRA will be encoded as BGRX and UYVA will be encoded as UYVY.
* 
* PreMultiplied: When combined with Alpha, alpha channel is premultiplied, otherwise straight
* 
* Preview: Frame is a special 1/8th preview frame
* 
* HighBitDepth: Sender automatically adds this flag for frames encoded using P216 or PA16 pixel formats.
* 
* Set this manually for VMX1 compressed data where the the frame was originally encoded using P216 or PA16.
* This determines which pixel format is selected on the decode side.
* 
*/
typedef enum OMTVideoFlags
{
    OMTVideoFlags_None = 0,
    OMTVideoFlags_Interlaced = 1,
    OMTVideoFlags_Alpha = 2,
    OMTVideoFlags_PreMultiplied = 4,
    OMTVideoFlags_Preview = 8,
    OMTVideoFlags_HighBitDepth = 16,
    OMTVideoFlags_INT32 = 0x7fffffff //Ensure int type in C
} OMTVideoFlags;

/**
* Specify the preferred uncompressed video format of decoded frames.
* 
* UYVY is always the fastest, if no alpha channel is required.
* 
* UYVYorBGRA will provide BGRA only when alpha channel is present.
* 
* BGRA will always convert back to BGRA
* 
* UYVYorUYVA will provide UYVA only when alpha channel is present.
* 
* UYVYorUYVAorP216orPA16 will provide P216 if sender encoded with high bit depth, or PA16 if sender encoded with high bit depth and alpha. Otherwise same as UYVYorUYVA.
* 
*/
typedef enum OMTPreferredVideoFormat
{
    OMTPreferredVideoFormat_UYVY = 0,
    OMTPreferredVideoFormat_UYVYorBGRA = 1,
    OMTPreferredVideoFormat_BGRA = 2,
    OMTPreferredVideoFormat_UYVYorUYVA = 3,
    OMTPreferredVideoFormat_UYVYorUYVAorP216orPA16 = 4,
    OMTPreferredVideoFormat_P216 = 5,
    OMTPreferredVideoFormat_INT32 = 0x7fffffff //Ensure int type in C
} OMTPreferredVideoFormat;

/**
* Flags to enable certain features on a Receiver:
*
* Preview: Receive only a 1/8th preview of the video.
*
* IncludeCompressed: Include a copy of the compressed VMX1 video frames for further processing or recording.
*
* CompressedOnly: Include only the compressed VMX1 video frame without decoding. In this instance DataLength will always be 0.
*/
typedef enum OMTReceiveFlags
{
    OMTReceiveFlags_None = 0,
    OMTReceiveFlags_Preview = 1,
    OMTReceiveFlags_IncludeCompressed = 2,
    OMTReceiveFlags_CompressedOnly = 4,
    OMTReceiveFlags_INT32 = 0x7fffffff //Ensure int type in C
} OMTReceiveFlags;

/// <summary>
/// Tally where 0 = off, 1 = on.
/// </summary>
typedef struct OMTTally
{
    int preview;
    int program;
} OMTTally;

typedef struct OMTSenderInfo
{
    char ProductName[OMT_MAX_STRING_LENGTH];
    char Manufacturer[OMT_MAX_STRING_LENGTH];
    char Version[OMT_MAX_STRING_LENGTH];
    char Reserved1[OMT_MAX_STRING_LENGTH];
    char Reserved2[OMT_MAX_STRING_LENGTH];
    char Reserved3[OMT_MAX_STRING_LENGTH];
} OMTSenderInfo;

typedef struct OMTStatistics
{
    int64_t BytesSent;
    int64_t BytesReceived;
    int64_t BytesSentSinceLast;
    int64_t BytesReceivedSinceLast;

    int64_t Frames;
    int64_t FramesSinceLast;
    int64_t FramesDropped;

    //Time in milliseconds spent encoding so far this instance. Can be divided by Frames to work out per frame times.
    int64_t CodecTime;
    //Time in milliseconds of the last frame encoded.
    int64_t CodecTimeSinceLast;

    int64_t Reserved1;
    int64_t Reserved2;
    int64_t Reserved3;
    int64_t Reserved4;
    int64_t Reserved5;
    int64_t Reserved6;
    int64_t Reserved7;
} OMTStatistics;

/**
* Media Frame struct for sending receiving.
* 
* IMPORTANT: Zero this struct before use. OMTMediaFrame frame = {}; is sufficient.
*/
typedef struct OMTMediaFrame
{
    //Specify the type of frame. This determines which values of this struct are valid/used.
    enum OMTFrameType Type;

    /**
    * This is a timestamp where 1 second = 10,000,000
    * 
    * This should not be left 0 unless this is the very first frame.
    * 
    * This should represent the accurate time the frame or audio sample was generated at the original source and be used on the receiving end to synchronize
    * and record to file as a presentation timestamp (pts).
    * 
    * A special value of -1 can be specified to tell the Sender to generate timestamps and throttle as required to maintain
    * the specified FrameRate or SampleRate of the frame. 
    */
    int64_t Timestamp;

    /**
    * Sending:
    * 
    * Video: 'UYVY', 'YUY2', 'NV12', 'YV12, 'BGRA', 'UYVA', 'VMX1' are supported (BGRA will be treated as BGRX and UYVA as UYVY where alpha flags are not set)
    * 
    * Audio: Only 'FPA1' is supported (32bit floating point planar audio)
    * 
    * Receiving:
    * 
    * Video: Only 'UYVY', 'UYVA', 'BGRA' and 'BGRX' are supported
    * 
    * Audio: Only 'FPA1' is supported (32bit floating point planar audio)
    * 
    */
    enum OMTCodec Codec;

    //Video Properties
    int Width;
    int Height;

    //Stride in bytes of each row of pixels. Typically width*2 for UYVY, width*4 for BGRA and just width for planar formats.
    int Stride;

    enum OMTVideoFlags Flags;

    // Frame Rate Numerator/Denominator in Frames Per Second, for example Numerator 60 and Denominator 1 is 60 frames per second.
    int FrameRateN;
    int FrameRateD;

    // Display aspect ratio expressed as a ratio of width/height. For example 1.777777777777778 for 16/9
    float AspectRatio;

    enum OMTColorSpace ColorSpace;

    //Audio Properties
    // Sample rate, i.e 48000, 44100 etc
    int SampleRate;
    // Audio Channels. A maximum of 32 channels are supported.
    int Channels;
    // Number of 32bit floating point samples per channel/plane. Each plane should contain SamplesPerChannel*4 bytes.
    int SamplesPerChannel;

    //Data Properties

    /**
    * Video: Uncompressed pixel data (or compressed VMX1 data when sending and Codec set to VMX1)
    * 
    * Audio: Planar 32bit floating point audio
    * 
    * Metadata: UTF-8 encoded XML string with terminating null character
    */
    void* Data;

    /**
    * Video: Number of bytes total including stride
    *
    * Audio: Number of bytes (SamplesPerChannel * Channels * 4)
    *
    * Metadata: Number of bytes in UTF-8 encoded string + 1 for terminating null character.
    */
    int DataLength;

    /**
    * Receive only. Use standard Data/DataLength if sending VMX1 frames with a Sender
    * 
    * If IncludeCompressed or CompressedOnly OMTReceiveFlags is set, this will include the original compressed video frame in VMX1 format.
    * 
    * This could then be muxed into an AVI or MOV file using FFmpeg or similar APIs
    */
    void* CompressedData;
    int CompressedLength;

    //Frame MetaData Properties
    // Per frame metadata as UTF-8 encoded string + 1 for null character. Up to 65536 bytes supported.
    void* FrameMetadata;

    // Length in bytes of per frame metadata including null character
    int FrameMetadataLength;

} OMTMediaFrame;

typedef long long omt_receive_t;
typedef long long omt_send_t;
typedef void (*OMTLoggingCallback)(const char *);

#ifdef __cplusplus
extern "C" {
#endif

    /**
    * =================================================
    * Discovery
    * =================================================
    */

    /**
    * Returns a list of sources currently available on the network.
    * 
    * Return value is an array of UTF-8 char pointers.
    * 
    * This array is valid until the next call to getaddresses.   
    * 
    * @param[in] count Number of entries in the returned array.
    */
    char** omt_discovery_getaddresses(int * count);

    /**
    * =================================================
    * Receive
    * =================================================
    */

    /**
    * Create a new Receiver and begin connecting to the Sender specified by address.
    * @param[in] address Address to connect to, either the full name provided by OMTDiscovery or a URL in the format omt://hostname:port</param>
    * @param[in] frameTypes Specify the types of frames to receive, for example to setup audio only or metadata only feeds
    * @param[in] format Specify the preferred uncompressed video format to receive. UYVYorBGRA will only receive BGRA frames when an alpha channel is present.
    * @param[in] flags Specify optional flags such as requesting a Preview feed only, or including the compressed (VMX) data with each frame for further processing (or recording).
    */
    omt_receive_t* omt_receive_create(const char* address, OMTFrameType frameTypes, OMTPreferredVideoFormat format, OMTReceiveFlags flags);

    /**
    * Destroy instance created with omt_receive_create.
    * 
    * Make sure any threads currently accessing the omt_receive_ functions with this instance are closed before calling.
    */
    void omt_receive_destroy(omt_receive_t* instance);

    /**
    * Receive any available frames in the buffer, or wait for frames if empty
    * 
    * Returns a valid OMTMediaFrame if a frame was found, null if timed out.
    * 
    * The data in this struct is valid for until the next call to omt_receive for this instance and frameType.
    * 
    * Pointers do not need to be freed by the caller.
    * 
    * @param[in] frameTypes The frame types to receive. Set multiple types to receive them all in a single thread. Set individually if using separate threads for audio/video/metadata
    * @param[in] timeoutMilliseconds The maximum time to wait for a new frame if empty
    */
    OMTMediaFrame* omt_receive(omt_receive_t* instance, OMTFrameType frameTypes, int timeoutMilliseconds);

    /**
    * Send a metadata frame to the sender. Does not support other frame types.
    */
    int omt_receive_send(omt_receive_t* instance, OMTMediaFrame* frame);

    void omt_receive_settally(omt_receive_t* instance, OMTTally* tally);

    /**
    * Receives the current tally state across all connections to a Sender, not just this Receiver.
    *
    * If this function times out, the last known tally state will be received.
    *
    * Returns 0 if timed out or tally didn't change. 1 otherwise.
    *
    */
    int omt_receive_gettally(omt_receive_t* instance, int timeoutMilliseconds, OMTTally* tally);

    /**
    * Change the flags on the current receive instance. Will apply from the next frame received.
    * This allows dynamic switching between preview mode.
    */
    void omt_receive_setflags(omt_receive_t* instance, OMTReceiveFlags flags);

    /**
    * Inform the sender of the quality preference for this receiver. See OMTQuality documentation for more information.
    */
    void omt_receive_setsuggestedquality(omt_receive_t* instance, OMTQuality quality);

    /**
    * Retrieve optional information describing the sender. Valid only when connected. Returns null if disconnected or no sender information was provided by sender.
    */
    void omt_receive_getsenderinformation(omt_receive_t* instance, OMTSenderInfo* info);

    /**
    * Retrieve statistics for video and audio.
    *
    */
    void omt_receive_getvideostatistics(omt_receive_t* instance, OMTStatistics* stats);
    void omt_receive_getaudiostatistics(omt_receive_t* instance, OMTStatistics* stats);

    /**
    * =================================================
    * Send
    * =================================================
    */

    /**
    * Create a new instance of the OMT Sender
    * 
    * @param[in] name Specify the name of the source not including hostname
    * @param[in] quality Specify the quality to use for video encoding. If Default, this can be automatically adjusted based on Receiver requirements.
    */
    omt_send_t* omt_send_create(const char* name, OMTQuality quality);

    /**
    * Optionally set information describing the sender.
    */
    void omt_send_setsenderinformation(omt_send_t* instance, OMTSenderInfo* info);

    /**
    * Add to the list of metadata that is sent immediately upon a new connection by a receiver.
    * 
    * This metadata will also be immediately sent to any currently connected receivers.
    * 
    * @param[in] metadata XML metadata in UTF-8 encoding followed by a null terminator.
    * 
    */
    void omt_send_addconnectionmetadata(omt_send_t* instance, const char* metadata);

    /**
    * Clears the list of metadata that is sent immediately upon a new connection by a receiver.
    */
    void omt_send_clearconnectionmetadata(omt_send_t* instance);

    /**
    * Use this to inform receivers to connect to a different address.
    * 
    * This is used to create a "virtual source" that can be dynamically switched as needed.
    * 
    * This is useful for scenarios where receiver needs to be changed remotely.
    * 
    * @param[in] newAddress The new address. Set to null or empty to disable redirect.
    */
    void omt_send_setredirect(omt_send_t* instance, const char* newAddress);

    /**
    * Retrieve the Discovery address in the format HOSTNAME (NAME)
    * 
    * Returns the length in bytes of the UTF-8 encoded value including null terminator.
    * 
    * maxLength specifies the maximum amount of bytes allocated to value by the caller.
    */
    int omt_send_getaddress(omt_send_t* instance, char* address, int maxLength);

    /**
    * Destroy instance created with omt_send_create.
    *
    * Make sure any threads currently accessing the omt_send_ functions with this instance are closed before calling.
    */
    void omt_send_destroy(omt_send_t* instance);

    /**
    * Send a frame to any receivers currently connected. 
    * 
    * Video: 'UYVY', 'YUY2', 'NV12', 'YV12, 'BGRA', 'UYVA', 'VMX1' are supported (BGRA will be treated as BGRX and UYVA as UYVY where alpha flags are not set)
    * 
    * Audio: Supports planar 32bit floating point audio
    * 
    * Metadata: Supports UTF8 encoded XML 
    */
    int omt_send(omt_send_t* instance, OMTMediaFrame* frame);

    /**
    * Total number of connections to this sender. Receivers establish one connection for video/metadata and a second for audio.
    */
    int omt_send_connections(omt_send_t* instance);

    /**
    * Receive any available metadata in the buffer, or wait for metadata if empty
    * 
    * Returns a valid OMTMediaFrame if found, null of timed out
    */
    OMTMediaFrame* omt_send_receive(omt_send_t* instance, int timeoutMilliseconds);

    /**
    * Receives the current tally state across all connections to a Sender.
    * 
    * If this function times out, the last known tally state will be received.
    * 
    * Returns 0 if timed out or tally didn't change. 1 otherwise.
    * 
    */
    int omt_send_gettally(omt_send_t* instance, int timeoutMilliseconds, OMTTally* tally);

    /**
    * Retrieve statistics for video and audio.
    * 
    */
    void omt_send_getvideostatistics(omt_send_t* instance, OMTStatistics* stats);
    void omt_send_getaudiostatistics(omt_send_t* instance, OMTStatistics* stats);

    /**
    * =================================================
    * Logging
    * =================================================
    */

    /**
    * Specify a log file filename, including the full path, or null to disable.
    * 
    * If this function is not called, a log file is created in the ~/.OMT/logs folder for this process on Mac and Linux, C:\ProgramData\OMT\logs on Windows.
    * 
    * To override the default folder used for for logs, set the OMT_STORAGE_PATH environment variable prior to calling any OMT functions.
    */
    void omt_setloggingfilename(const char* filename);

    /**
    * Specify a log line callback, or null to disable.
    */
    void omt_setloggingcallback(OMTLoggingCallback callback);

    /**
    * =================================================
    * Settings
    * 
    * These functions override the default settings which are stored in ~/.OMT/settings.xml on Mac and Linux and C:\ProgramData\OMT\settings.xml on Windows by default.
    * 
    * To override the default folder used for for settings, set the OMT_STORAGE_PATH environment variable prior to calling any OMT functions.
    * 
    * The following settings are currently supported:
    * 
    * DiscoveryServer [string] specify a URL in the format omt://hostname:port to connect to for discovery. If left blank, default DNS-SD discovery behavior is enabled.
    * 
    * NetworkPortStart [integer] specify the first port to create Send instances on. Defaults to 6400
    * 
    * NetworkPortEnd [integer] specify the last port to create Send instances on. Defaults to 6600
    * 
    * Settings changed here will persist only for the currently running process.
    * =================================================
    */

    /**
    * Retrieve the current value of a string setting.
    * Returns the length in bytes of the UTF-8 encoded value including null terminator.
    * maxLength specifies the maximum amount of bytes allocated to value by the caller.
    */
    int omt_settings_get_string(const char* name, char* value, int maxLength);

    /**
    * Set a string setting that will be used for the duration of this process.
    * Value should be a null terminated UTF-8 encoded string.
    */
    void omt_settings_set_string(const char* name, const char* value);

    /**
    * Retrieve the current value of the integer setting.
    */
    int omt_settings_get_integer(const char* name);

    /**
    * Set the specified setting to an integer value.
    */
    void omt_settings_set_integer(const char* name, int value);

    /**
    * Shutdown Logging and Discovery background threads.
    * This should only ever be called once before process exit after all senders and receivers have been destroyed first.
    */
    void omt_shutdown();

#ifdef __cplusplus
}
#endif
