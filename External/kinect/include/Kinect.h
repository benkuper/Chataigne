

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Sun Oct 19 12:54:08 2014
 */
/* Compiler settings for ..\..\idl\Kinect.COM.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.00.0595 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

/* verify that the <rpcsal.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCSAL_H_VERSION__
#define __REQUIRED_RPCSAL_H_VERSION__ 100
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __Kinect_h__
#define __Kinect_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IKinectSensor_FWD_DEFINED__
#define __IKinectSensor_FWD_DEFINED__
typedef interface IKinectSensor IKinectSensor;

#endif 	/* __IKinectSensor_FWD_DEFINED__ */


#ifndef __IIsAvailableChangedEventArgs_FWD_DEFINED__
#define __IIsAvailableChangedEventArgs_FWD_DEFINED__
typedef interface IIsAvailableChangedEventArgs IIsAvailableChangedEventArgs;

#endif 	/* __IIsAvailableChangedEventArgs_FWD_DEFINED__ */


#ifndef __IFrameDescription_FWD_DEFINED__
#define __IFrameDescription_FWD_DEFINED__
typedef interface IFrameDescription IFrameDescription;

#endif 	/* __IFrameDescription_FWD_DEFINED__ */


#ifndef __IFrameCapturedEventArgs_FWD_DEFINED__
#define __IFrameCapturedEventArgs_FWD_DEFINED__
typedef interface IFrameCapturedEventArgs IFrameCapturedEventArgs;

#endif 	/* __IFrameCapturedEventArgs_FWD_DEFINED__ */


#ifndef __IMultiSourceFrameReader_FWD_DEFINED__
#define __IMultiSourceFrameReader_FWD_DEFINED__
typedef interface IMultiSourceFrameReader IMultiSourceFrameReader;

#endif 	/* __IMultiSourceFrameReader_FWD_DEFINED__ */


#ifndef __IMultiSourceFrameArrivedEventArgs_FWD_DEFINED__
#define __IMultiSourceFrameArrivedEventArgs_FWD_DEFINED__
typedef interface IMultiSourceFrameArrivedEventArgs IMultiSourceFrameArrivedEventArgs;

#endif 	/* __IMultiSourceFrameArrivedEventArgs_FWD_DEFINED__ */


#ifndef __IMultiSourceFrameReference_FWD_DEFINED__
#define __IMultiSourceFrameReference_FWD_DEFINED__
typedef interface IMultiSourceFrameReference IMultiSourceFrameReference;

#endif 	/* __IMultiSourceFrameReference_FWD_DEFINED__ */


#ifndef __IMultiSourceFrame_FWD_DEFINED__
#define __IMultiSourceFrame_FWD_DEFINED__
typedef interface IMultiSourceFrame IMultiSourceFrame;

#endif 	/* __IMultiSourceFrame_FWD_DEFINED__ */


#ifndef __IColorFrameReference_FWD_DEFINED__
#define __IColorFrameReference_FWD_DEFINED__
typedef interface IColorFrameReference IColorFrameReference;

#endif 	/* __IColorFrameReference_FWD_DEFINED__ */


#ifndef __IColorFrameArrivedEventArgs_FWD_DEFINED__
#define __IColorFrameArrivedEventArgs_FWD_DEFINED__
typedef interface IColorFrameArrivedEventArgs IColorFrameArrivedEventArgs;

#endif 	/* __IColorFrameArrivedEventArgs_FWD_DEFINED__ */


#ifndef __IColorFrameSource_FWD_DEFINED__
#define __IColorFrameSource_FWD_DEFINED__
typedef interface IColorFrameSource IColorFrameSource;

#endif 	/* __IColorFrameSource_FWD_DEFINED__ */


#ifndef __IColorFrameReader_FWD_DEFINED__
#define __IColorFrameReader_FWD_DEFINED__
typedef interface IColorFrameReader IColorFrameReader;

#endif 	/* __IColorFrameReader_FWD_DEFINED__ */


#ifndef __IColorFrame_FWD_DEFINED__
#define __IColorFrame_FWD_DEFINED__
typedef interface IColorFrame IColorFrame;

#endif 	/* __IColorFrame_FWD_DEFINED__ */


#ifndef __IDepthFrameReference_FWD_DEFINED__
#define __IDepthFrameReference_FWD_DEFINED__
typedef interface IDepthFrameReference IDepthFrameReference;

#endif 	/* __IDepthFrameReference_FWD_DEFINED__ */


#ifndef __IDepthFrameArrivedEventArgs_FWD_DEFINED__
#define __IDepthFrameArrivedEventArgs_FWD_DEFINED__
typedef interface IDepthFrameArrivedEventArgs IDepthFrameArrivedEventArgs;

#endif 	/* __IDepthFrameArrivedEventArgs_FWD_DEFINED__ */


#ifndef __IDepthFrameSource_FWD_DEFINED__
#define __IDepthFrameSource_FWD_DEFINED__
typedef interface IDepthFrameSource IDepthFrameSource;

#endif 	/* __IDepthFrameSource_FWD_DEFINED__ */


#ifndef __IDepthFrameReader_FWD_DEFINED__
#define __IDepthFrameReader_FWD_DEFINED__
typedef interface IDepthFrameReader IDepthFrameReader;

#endif 	/* __IDepthFrameReader_FWD_DEFINED__ */


#ifndef __IDepthFrame_FWD_DEFINED__
#define __IDepthFrame_FWD_DEFINED__
typedef interface IDepthFrame IDepthFrame;

#endif 	/* __IDepthFrame_FWD_DEFINED__ */


#ifndef __IBodyFrameReference_FWD_DEFINED__
#define __IBodyFrameReference_FWD_DEFINED__
typedef interface IBodyFrameReference IBodyFrameReference;

#endif 	/* __IBodyFrameReference_FWD_DEFINED__ */


#ifndef __IBodyFrameArrivedEventArgs_FWD_DEFINED__
#define __IBodyFrameArrivedEventArgs_FWD_DEFINED__
typedef interface IBodyFrameArrivedEventArgs IBodyFrameArrivedEventArgs;

#endif 	/* __IBodyFrameArrivedEventArgs_FWD_DEFINED__ */


#ifndef __IBodyFrameSource_FWD_DEFINED__
#define __IBodyFrameSource_FWD_DEFINED__
typedef interface IBodyFrameSource IBodyFrameSource;

#endif 	/* __IBodyFrameSource_FWD_DEFINED__ */


#ifndef __IBodyFrameReader_FWD_DEFINED__
#define __IBodyFrameReader_FWD_DEFINED__
typedef interface IBodyFrameReader IBodyFrameReader;

#endif 	/* __IBodyFrameReader_FWD_DEFINED__ */


#ifndef __IBodyFrame_FWD_DEFINED__
#define __IBodyFrame_FWD_DEFINED__
typedef interface IBodyFrame IBodyFrame;

#endif 	/* __IBodyFrame_FWD_DEFINED__ */


#ifndef __IBody_FWD_DEFINED__
#define __IBody_FWD_DEFINED__
typedef interface IBody IBody;

#endif 	/* __IBody_FWD_DEFINED__ */


#ifndef __IBodyIndexFrameReference_FWD_DEFINED__
#define __IBodyIndexFrameReference_FWD_DEFINED__
typedef interface IBodyIndexFrameReference IBodyIndexFrameReference;

#endif 	/* __IBodyIndexFrameReference_FWD_DEFINED__ */


#ifndef __IBodyIndexFrameArrivedEventArgs_FWD_DEFINED__
#define __IBodyIndexFrameArrivedEventArgs_FWD_DEFINED__
typedef interface IBodyIndexFrameArrivedEventArgs IBodyIndexFrameArrivedEventArgs;

#endif 	/* __IBodyIndexFrameArrivedEventArgs_FWD_DEFINED__ */


#ifndef __IBodyIndexFrameSource_FWD_DEFINED__
#define __IBodyIndexFrameSource_FWD_DEFINED__
typedef interface IBodyIndexFrameSource IBodyIndexFrameSource;

#endif 	/* __IBodyIndexFrameSource_FWD_DEFINED__ */


#ifndef __IBodyIndexFrameReader_FWD_DEFINED__
#define __IBodyIndexFrameReader_FWD_DEFINED__
typedef interface IBodyIndexFrameReader IBodyIndexFrameReader;

#endif 	/* __IBodyIndexFrameReader_FWD_DEFINED__ */


#ifndef __IBodyIndexFrame_FWD_DEFINED__
#define __IBodyIndexFrame_FWD_DEFINED__
typedef interface IBodyIndexFrame IBodyIndexFrame;

#endif 	/* __IBodyIndexFrame_FWD_DEFINED__ */


#ifndef __IInfraredFrameReference_FWD_DEFINED__
#define __IInfraredFrameReference_FWD_DEFINED__
typedef interface IInfraredFrameReference IInfraredFrameReference;

#endif 	/* __IInfraredFrameReference_FWD_DEFINED__ */


#ifndef __IInfraredFrameArrivedEventArgs_FWD_DEFINED__
#define __IInfraredFrameArrivedEventArgs_FWD_DEFINED__
typedef interface IInfraredFrameArrivedEventArgs IInfraredFrameArrivedEventArgs;

#endif 	/* __IInfraredFrameArrivedEventArgs_FWD_DEFINED__ */


#ifndef __IInfraredFrameSource_FWD_DEFINED__
#define __IInfraredFrameSource_FWD_DEFINED__
typedef interface IInfraredFrameSource IInfraredFrameSource;

#endif 	/* __IInfraredFrameSource_FWD_DEFINED__ */


#ifndef __IInfraredFrameReader_FWD_DEFINED__
#define __IInfraredFrameReader_FWD_DEFINED__
typedef interface IInfraredFrameReader IInfraredFrameReader;

#endif 	/* __IInfraredFrameReader_FWD_DEFINED__ */


#ifndef __IInfraredFrame_FWD_DEFINED__
#define __IInfraredFrame_FWD_DEFINED__
typedef interface IInfraredFrame IInfraredFrame;

#endif 	/* __IInfraredFrame_FWD_DEFINED__ */


#ifndef __ILongExposureInfraredFrameReference_FWD_DEFINED__
#define __ILongExposureInfraredFrameReference_FWD_DEFINED__
typedef interface ILongExposureInfraredFrameReference ILongExposureInfraredFrameReference;

#endif 	/* __ILongExposureInfraredFrameReference_FWD_DEFINED__ */


#ifndef __ILongExposureInfraredFrameArrivedEventArgs_FWD_DEFINED__
#define __ILongExposureInfraredFrameArrivedEventArgs_FWD_DEFINED__
typedef interface ILongExposureInfraredFrameArrivedEventArgs ILongExposureInfraredFrameArrivedEventArgs;

#endif 	/* __ILongExposureInfraredFrameArrivedEventArgs_FWD_DEFINED__ */


#ifndef __ILongExposureInfraredFrameSource_FWD_DEFINED__
#define __ILongExposureInfraredFrameSource_FWD_DEFINED__
typedef interface ILongExposureInfraredFrameSource ILongExposureInfraredFrameSource;

#endif 	/* __ILongExposureInfraredFrameSource_FWD_DEFINED__ */


#ifndef __ILongExposureInfraredFrameReader_FWD_DEFINED__
#define __ILongExposureInfraredFrameReader_FWD_DEFINED__
typedef interface ILongExposureInfraredFrameReader ILongExposureInfraredFrameReader;

#endif 	/* __ILongExposureInfraredFrameReader_FWD_DEFINED__ */


#ifndef __ILongExposureInfraredFrame_FWD_DEFINED__
#define __ILongExposureInfraredFrame_FWD_DEFINED__
typedef interface ILongExposureInfraredFrame ILongExposureInfraredFrame;

#endif 	/* __ILongExposureInfraredFrame_FWD_DEFINED__ */


#ifndef __IAudioBeam_FWD_DEFINED__
#define __IAudioBeam_FWD_DEFINED__
typedef interface IAudioBeam IAudioBeam;

#endif 	/* __IAudioBeam_FWD_DEFINED__ */


#ifndef __IAudioBeamList_FWD_DEFINED__
#define __IAudioBeamList_FWD_DEFINED__
typedef interface IAudioBeamList IAudioBeamList;

#endif 	/* __IAudioBeamList_FWD_DEFINED__ */


#ifndef __IAudioBeamFrameList_FWD_DEFINED__
#define __IAudioBeamFrameList_FWD_DEFINED__
typedef interface IAudioBeamFrameList IAudioBeamFrameList;

#endif 	/* __IAudioBeamFrameList_FWD_DEFINED__ */


#ifndef __IAudioBeamFrame_FWD_DEFINED__
#define __IAudioBeamFrame_FWD_DEFINED__
typedef interface IAudioBeamFrame IAudioBeamFrame;

#endif 	/* __IAudioBeamFrame_FWD_DEFINED__ */


#ifndef __IAudioBeamSubFrame_FWD_DEFINED__
#define __IAudioBeamSubFrame_FWD_DEFINED__
typedef interface IAudioBeamSubFrame IAudioBeamSubFrame;

#endif 	/* __IAudioBeamSubFrame_FWD_DEFINED__ */


#ifndef __IAudioBeamFrameReference_FWD_DEFINED__
#define __IAudioBeamFrameReference_FWD_DEFINED__
typedef interface IAudioBeamFrameReference IAudioBeamFrameReference;

#endif 	/* __IAudioBeamFrameReference_FWD_DEFINED__ */


#ifndef __IAudioBodyCorrelation_FWD_DEFINED__
#define __IAudioBodyCorrelation_FWD_DEFINED__
typedef interface IAudioBodyCorrelation IAudioBodyCorrelation;

#endif 	/* __IAudioBodyCorrelation_FWD_DEFINED__ */


#ifndef __IAudioBeamFrameArrivedEventArgs_FWD_DEFINED__
#define __IAudioBeamFrameArrivedEventArgs_FWD_DEFINED__
typedef interface IAudioBeamFrameArrivedEventArgs IAudioBeamFrameArrivedEventArgs;

#endif 	/* __IAudioBeamFrameArrivedEventArgs_FWD_DEFINED__ */


#ifndef __IAudioBeamFrameReader_FWD_DEFINED__
#define __IAudioBeamFrameReader_FWD_DEFINED__
typedef interface IAudioBeamFrameReader IAudioBeamFrameReader;

#endif 	/* __IAudioBeamFrameReader_FWD_DEFINED__ */


#ifndef __IAudioSource_FWD_DEFINED__
#define __IAudioSource_FWD_DEFINED__
typedef interface IAudioSource IAudioSource;

#endif 	/* __IAudioSource_FWD_DEFINED__ */


#ifndef __ICoordinateMapper_FWD_DEFINED__
#define __ICoordinateMapper_FWD_DEFINED__
typedef interface ICoordinateMapper ICoordinateMapper;

#endif 	/* __ICoordinateMapper_FWD_DEFINED__ */


#ifndef __ICoordinateMappingChangedEventArgs_FWD_DEFINED__
#define __ICoordinateMappingChangedEventArgs_FWD_DEFINED__
typedef interface ICoordinateMappingChangedEventArgs ICoordinateMappingChangedEventArgs;

#endif 	/* __ICoordinateMappingChangedEventArgs_FWD_DEFINED__ */


#ifndef __IColorCameraSettings_FWD_DEFINED__
#define __IColorCameraSettings_FWD_DEFINED__
typedef interface IColorCameraSettings IColorCameraSettings;

#endif 	/* __IColorCameraSettings_FWD_DEFINED__ */


#ifndef __IBodyHandPair_FWD_DEFINED__
#define __IBodyHandPair_FWD_DEFINED__
typedef interface IBodyHandPair IBodyHandPair;

#endif 	/* __IBodyHandPair_FWD_DEFINED__ */


#ifndef __IKinectCoreWindow_FWD_DEFINED__
#define __IKinectCoreWindow_FWD_DEFINED__
typedef interface IKinectCoreWindow IKinectCoreWindow;

#endif 	/* __IKinectCoreWindow_FWD_DEFINED__ */


#ifndef __IKinectGestureRecognizer_FWD_DEFINED__
#define __IKinectGestureRecognizer_FWD_DEFINED__
typedef interface IKinectGestureRecognizer IKinectGestureRecognizer;

#endif 	/* __IKinectGestureRecognizer_FWD_DEFINED__ */


#ifndef __IKinectGestureRecognizerSelectionHandler_FWD_DEFINED__
#define __IKinectGestureRecognizerSelectionHandler_FWD_DEFINED__
typedef interface IKinectGestureRecognizerSelectionHandler IKinectGestureRecognizerSelectionHandler;

#endif 	/* __IKinectGestureRecognizerSelectionHandler_FWD_DEFINED__ */


#ifndef __IKinectGestureRecognizerManipulationHandler_FWD_DEFINED__
#define __IKinectGestureRecognizerManipulationHandler_FWD_DEFINED__
typedef interface IKinectGestureRecognizerManipulationHandler IKinectGestureRecognizerManipulationHandler;

#endif 	/* __IKinectGestureRecognizerManipulationHandler_FWD_DEFINED__ */


#ifndef __IKinectHoldingEventArgs_FWD_DEFINED__
#define __IKinectHoldingEventArgs_FWD_DEFINED__
typedef interface IKinectHoldingEventArgs IKinectHoldingEventArgs;

#endif 	/* __IKinectHoldingEventArgs_FWD_DEFINED__ */


#ifndef __IKinectManipulationCompletedEventArgs_FWD_DEFINED__
#define __IKinectManipulationCompletedEventArgs_FWD_DEFINED__
typedef interface IKinectManipulationCompletedEventArgs IKinectManipulationCompletedEventArgs;

#endif 	/* __IKinectManipulationCompletedEventArgs_FWD_DEFINED__ */


#ifndef __IKinectManipulationInertiaStartingEventArgs_FWD_DEFINED__
#define __IKinectManipulationInertiaStartingEventArgs_FWD_DEFINED__
typedef interface IKinectManipulationInertiaStartingEventArgs IKinectManipulationInertiaStartingEventArgs;

#endif 	/* __IKinectManipulationInertiaStartingEventArgs_FWD_DEFINED__ */


#ifndef __IKinectManipulationStartedEventArgs_FWD_DEFINED__
#define __IKinectManipulationStartedEventArgs_FWD_DEFINED__
typedef interface IKinectManipulationStartedEventArgs IKinectManipulationStartedEventArgs;

#endif 	/* __IKinectManipulationStartedEventArgs_FWD_DEFINED__ */


#ifndef __IKinectManipulationUpdatedEventArgs_FWD_DEFINED__
#define __IKinectManipulationUpdatedEventArgs_FWD_DEFINED__
typedef interface IKinectManipulationUpdatedEventArgs IKinectManipulationUpdatedEventArgs;

#endif 	/* __IKinectManipulationUpdatedEventArgs_FWD_DEFINED__ */


#ifndef __IKinectPointerDevice_FWD_DEFINED__
#define __IKinectPointerDevice_FWD_DEFINED__
typedef interface IKinectPointerDevice IKinectPointerDevice;

#endif 	/* __IKinectPointerDevice_FWD_DEFINED__ */


#ifndef __IKinectPointerEventArgs_FWD_DEFINED__
#define __IKinectPointerEventArgs_FWD_DEFINED__
typedef interface IKinectPointerEventArgs IKinectPointerEventArgs;

#endif 	/* __IKinectPointerEventArgs_FWD_DEFINED__ */


#ifndef __IKinectPointerPoint_FWD_DEFINED__
#define __IKinectPointerPoint_FWD_DEFINED__
typedef interface IKinectPointerPoint IKinectPointerPoint;

#endif 	/* __IKinectPointerPoint_FWD_DEFINED__ */


#ifndef __IKinectPointerPointProperties_FWD_DEFINED__
#define __IKinectPointerPointProperties_FWD_DEFINED__
typedef interface IKinectPointerPointProperties IKinectPointerPointProperties;

#endif 	/* __IKinectPointerPointProperties_FWD_DEFINED__ */


#ifndef __IKinectPressingCompletedEventArgs_FWD_DEFINED__
#define __IKinectPressingCompletedEventArgs_FWD_DEFINED__
typedef interface IKinectPressingCompletedEventArgs IKinectPressingCompletedEventArgs;

#endif 	/* __IKinectPressingCompletedEventArgs_FWD_DEFINED__ */


#ifndef __IKinectPressingStartedEventArgs_FWD_DEFINED__
#define __IKinectPressingStartedEventArgs_FWD_DEFINED__
typedef interface IKinectPressingStartedEventArgs IKinectPressingStartedEventArgs;

#endif 	/* __IKinectPressingStartedEventArgs_FWD_DEFINED__ */


#ifndef __IKinectPressingUpdatedEventArgs_FWD_DEFINED__
#define __IKinectPressingUpdatedEventArgs_FWD_DEFINED__
typedef interface IKinectPressingUpdatedEventArgs IKinectPressingUpdatedEventArgs;

#endif 	/* __IKinectPressingUpdatedEventArgs_FWD_DEFINED__ */


#ifndef __IKinectTappedEventArgs_FWD_DEFINED__
#define __IKinectTappedEventArgs_FWD_DEFINED__
typedef interface IKinectTappedEventArgs IKinectTappedEventArgs;

#endif 	/* __IKinectTappedEventArgs_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "mmreg.h"
#include "Kinect.INPC.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_Kinect2ECOM_0000_0000 */
/* [local] */ 


typedef INT_PTR WAITABLE_HANDLE;

typedef INT64 TIMESPAN;

#ifndef _KinectCapabilities_
#define _KinectCapabilities_
typedef enum _KinectCapabilities KinectCapabilities;


enum _KinectCapabilities
    {
        KinectCapabilities_None	= 0,
        KinectCapabilities_Vision	= 0x1,
        KinectCapabilities_Audio	= 0x2,
        KinectCapabilities_Face	= 0x4,
        KinectCapabilities_Expressions	= 0x8,
        KinectCapabilities_Gamechat	= 0x10
    } ;
#endif // _KinectCapabilities_
#ifndef _FrameSourceTypes_
#define _FrameSourceTypes_
typedef enum _FrameSourceTypes FrameSourceTypes;


enum _FrameSourceTypes
    {
        FrameSourceTypes_None	= 0,
        FrameSourceTypes_Color	= 0x1,
        FrameSourceTypes_Infrared	= 0x2,
        FrameSourceTypes_LongExposureInfrared	= 0x4,
        FrameSourceTypes_Depth	= 0x8,
        FrameSourceTypes_BodyIndex	= 0x10,
        FrameSourceTypes_Body	= 0x20,
        FrameSourceTypes_Audio	= 0x40
    } ;
#endif // _FrameSourceTypes_
#ifndef _ColorImageFormat_
#define _ColorImageFormat_
typedef enum _ColorImageFormat ColorImageFormat;


enum _ColorImageFormat
    {
        ColorImageFormat_None	= 0,
        ColorImageFormat_Rgba	= 1,
        ColorImageFormat_Yuv	= 2,
        ColorImageFormat_Bgra	= 3,
        ColorImageFormat_Bayer	= 4,
        ColorImageFormat_Yuy2	= 5
    } ;
#endif // _ColorImageFormat_
#ifndef _HandState_
#define _HandState_
typedef enum _HandState HandState;


enum _HandState
    {
        HandState_Unknown	= 0,
        HandState_NotTracked	= 1,
        HandState_Open	= 2,
        HandState_Closed	= 3,
        HandState_Lasso	= 4
    } ;
#endif // _HandState_
#ifndef _Expression_
#define _Expression_
typedef enum _Expression Expression;


enum _Expression
    {
        Expression_Neutral	= 0,
        Expression_Happy	= 1,
        Expression_Count	= ( Expression_Happy + 1 ) 
    } ;
#endif // _Expression_
#ifndef _DetectionResult_
#define _DetectionResult_
typedef enum _DetectionResult DetectionResult;


enum _DetectionResult
    {
        DetectionResult_Unknown	= 0,
        DetectionResult_No	= 1,
        DetectionResult_Maybe	= 2,
        DetectionResult_Yes	= 3
    } ;
#endif // _DetectionResult_
#ifndef _TrackingConfidence_
#define _TrackingConfidence_
typedef enum _TrackingConfidence TrackingConfidence;


enum _TrackingConfidence
    {
        TrackingConfidence_Low	= 0,
        TrackingConfidence_High	= 1
    } ;
#endif // _TrackingConfidence_
#ifndef _Activity_
#define _Activity_
typedef enum _Activity Activity;


enum _Activity
    {
        Activity_EyeLeftClosed	= 0,
        Activity_EyeRightClosed	= 1,
        Activity_MouthOpen	= 2,
        Activity_MouthMoved	= 3,
        Activity_LookingAway	= 4,
        Activity_Count	= ( Activity_LookingAway + 1 ) 
    } ;
#endif // _Activity_
#ifndef _Appearance_
#define _Appearance_
typedef enum _Appearance Appearance;


enum _Appearance
    {
        Appearance_WearingGlasses	= 0,
        Appearance_Count	= ( Appearance_WearingGlasses + 1 ) 
    } ;
#endif // _Appearance_
#ifndef _JointType_
#define _JointType_
typedef enum _JointType JointType;


enum _JointType
    {
        JointType_SpineBase	= 0,
        JointType_SpineMid	= 1,
        JointType_Neck	= 2,
        JointType_Head	= 3,
        JointType_ShoulderLeft	= 4,
        JointType_ElbowLeft	= 5,
        JointType_WristLeft	= 6,
        JointType_HandLeft	= 7,
        JointType_ShoulderRight	= 8,
        JointType_ElbowRight	= 9,
        JointType_WristRight	= 10,
        JointType_HandRight	= 11,
        JointType_HipLeft	= 12,
        JointType_KneeLeft	= 13,
        JointType_AnkleLeft	= 14,
        JointType_FootLeft	= 15,
        JointType_HipRight	= 16,
        JointType_KneeRight	= 17,
        JointType_AnkleRight	= 18,
        JointType_FootRight	= 19,
        JointType_SpineShoulder	= 20,
        JointType_HandTipLeft	= 21,
        JointType_ThumbLeft	= 22,
        JointType_HandTipRight	= 23,
        JointType_ThumbRight	= 24,
        JointType_Count	= ( JointType_ThumbRight + 1 ) 
    } ;
#endif // _JointType_
#ifndef _TrackingState_
#define _TrackingState_
typedef enum _TrackingState TrackingState;


enum _TrackingState
    {
        TrackingState_NotTracked	= 0,
        TrackingState_Inferred	= 1,
        TrackingState_Tracked	= 2
    } ;
#endif // _TrackingState_
#ifndef _FrameEdges_
#define _FrameEdges_
typedef enum _FrameEdges FrameEdges;


enum _FrameEdges
    {
        FrameEdge_None	= 0,
        FrameEdge_Right	= 0x1,
        FrameEdge_Left	= 0x2,
        FrameEdge_Top	= 0x4,
        FrameEdge_Bottom	= 0x8
    } ;
#endif // _FrameEdges_
#ifndef _FrameCapturedStatus_
#define _FrameCapturedStatus_
typedef enum _FrameCapturedStatus FrameCapturedStatus;


enum _FrameCapturedStatus
    {
        FrameCapturedStatus_Unknown	= 0,
        FrameCapturedStatus_Queued	= 1,
        FrameCapturedStatus_Dropped	= 2
    } ;
#endif // _FrameCapturedStatus_
#ifndef _AudioBeamMode_
#define _AudioBeamMode_
typedef enum _AudioBeamMode AudioBeamMode;


enum _AudioBeamMode
    {
        AudioBeamMode_Automatic	= 0,
        AudioBeamMode_Manual	= 1
    } ;
#endif // _AudioBeamMode_
#ifndef _KinectAudioCalibrationState_
#define _KinectAudioCalibrationState_
typedef enum _KinectAudioCalibrationState KinectAudioCalibrationState;


enum _KinectAudioCalibrationState
    {
        KinectAudioCalibrationState_Unknown	= 0,
        KinectAudioCalibrationState_CalibrationRequired	= 1,
        KinectAudioCalibrationState_Calibrated	= 2
    } ;
#endif // _KinectAudioCalibrationState_
#ifndef _Vector4_
#define _Vector4_
typedef struct _Vector4
    {
    float x;
    float y;
    float z;
    float w;
    } 	Vector4;

#endif // _Vector4_
#ifndef _PointF_
#define _PointF_
typedef struct _PointF
    {
    float X;
    float Y;
    } 	PointF;

#endif // _PointF_
#ifndef _RectF_
#define _RectF_
typedef struct _RectF
    {
    float X;
    float Y;
    float Width;
    float Height;
    } 	RectF;

#endif // _RectF_
#ifndef _ColorSpacePoint_
#define _ColorSpacePoint_
typedef struct _ColorSpacePoint
    {
    float X;
    float Y;
    } 	ColorSpacePoint;

#endif // _ColorSpacePoint_
#ifndef _DepthSpacePoint_
#define _DepthSpacePoint_
typedef struct _DepthSpacePoint
    {
    float X;
    float Y;
    } 	DepthSpacePoint;

#endif // _DepthSpacePoint_
#ifndef _CameraSpacePoint_
#define _CameraSpacePoint_
typedef struct _CameraSpacePoint
    {
    float X;
    float Y;
    float Z;
    } 	CameraSpacePoint;

#endif // _CameraSpacePoint_
#ifndef _Joint_
#define _Joint_
typedef struct _Joint
    {
    JointType JointType;
    CameraSpacePoint Position;
    TrackingState TrackingState;
    } 	Joint;

#endif // _Joint_
#ifndef _JointOrientation_
#define _JointOrientation_
typedef struct _JointOrientation
    {
    JointType JointType;
    Vector4 Orientation;
    } 	JointOrientation;

#endif // _JointOrientation_
#ifndef _CameraIntrinsics_
#define _CameraIntrinsics_
typedef struct _CameraIntrinsics
    {
    float FocalLengthX;
    float FocalLengthY;
    float PrincipalPointX;
    float PrincipalPointY;
    float RadialDistortionSecondOrder;
    float RadialDistortionFourthOrder;
    float RadialDistortionSixthOrder;
    } 	CameraIntrinsics;

#endif // _CameraIntrinsics_






















































extern RPC_IF_HANDLE __MIDL_itf_Kinect2ECOM_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Kinect2ECOM_0000_0000_v0_0_s_ifspec;

#ifndef __IKinectSensor_INTERFACE_DEFINED__
#define __IKinectSensor_INTERFACE_DEFINED__

/* interface IKinectSensor */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IKinectSensor;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3C6EBA94-0DE1-4360-B6D4-653A10794C8B")
    IKinectSensor : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SubscribeIsAvailableChanged( 
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnsubscribeIsAvailableChanged( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetIsAvailableChangedEventData( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IIsAvailableChangedEventArgs **eventData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Open( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsOpen( 
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isOpen) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsAvailable( 
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isAvailable) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ColorFrameSource( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IColorFrameSource **colorFrameSource) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_DepthFrameSource( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IDepthFrameSource **depthFrameSource) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_BodyFrameSource( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyFrameSource **bodyFrameSource) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_BodyIndexFrameSource( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyIndexFrameSource **bodyIndexFrameSource) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_InfraredFrameSource( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IInfraredFrameSource **infraredFrameSource) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_LongExposureInfraredFrameSource( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  ILongExposureInfraredFrameSource **longExposureInfraredFrameSource) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_AudioSource( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IAudioSource **audioSource) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OpenMultiSourceFrameReader( 
            DWORD enabledFrameSourceTypes,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IMultiSourceFrameReader **multiSourceFrameReader) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_CoordinateMapper( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  ICoordinateMapper **coordinateMapper) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_UniqueKinectId( 
            UINT bufferSize,
            /* [annotation][out][retval] */ 
            _Out_writes_z_(bufferSize)  WCHAR *uniqueKinectId) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_KinectCapabilities( 
            /* [annotation][out][retval] */ 
            _Out_  DWORD *capabilities) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IKinectSensorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IKinectSensor * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IKinectSensor * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IKinectSensor * This);
        
        HRESULT ( STDMETHODCALLTYPE *SubscribeIsAvailableChanged )( 
            IKinectSensor * This,
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *UnsubscribeIsAvailableChanged )( 
            IKinectSensor * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *GetIsAvailableChangedEventData )( 
            IKinectSensor * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IIsAvailableChangedEventArgs **eventData);
        
        HRESULT ( STDMETHODCALLTYPE *Open )( 
            IKinectSensor * This);
        
        HRESULT ( STDMETHODCALLTYPE *Close )( 
            IKinectSensor * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsOpen )( 
            IKinectSensor * This,
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isOpen);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsAvailable )( 
            IKinectSensor * This,
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isAvailable);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_ColorFrameSource )( 
            IKinectSensor * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IColorFrameSource **colorFrameSource);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_DepthFrameSource )( 
            IKinectSensor * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IDepthFrameSource **depthFrameSource);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_BodyFrameSource )( 
            IKinectSensor * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyFrameSource **bodyFrameSource);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_BodyIndexFrameSource )( 
            IKinectSensor * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyIndexFrameSource **bodyIndexFrameSource);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_InfraredFrameSource )( 
            IKinectSensor * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IInfraredFrameSource **infraredFrameSource);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_LongExposureInfraredFrameSource )( 
            IKinectSensor * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  ILongExposureInfraredFrameSource **longExposureInfraredFrameSource);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_AudioSource )( 
            IKinectSensor * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IAudioSource **audioSource);
        
        HRESULT ( STDMETHODCALLTYPE *OpenMultiSourceFrameReader )( 
            IKinectSensor * This,
            DWORD enabledFrameSourceTypes,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IMultiSourceFrameReader **multiSourceFrameReader);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_CoordinateMapper )( 
            IKinectSensor * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  ICoordinateMapper **coordinateMapper);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_UniqueKinectId )( 
            IKinectSensor * This,
            UINT bufferSize,
            /* [annotation][out][retval] */ 
            _Out_writes_z_(bufferSize)  WCHAR *uniqueKinectId);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_KinectCapabilities )( 
            IKinectSensor * This,
            /* [annotation][out][retval] */ 
            _Out_  DWORD *capabilities);
        
        END_INTERFACE
    } IKinectSensorVtbl;

    interface IKinectSensor
    {
        CONST_VTBL struct IKinectSensorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IKinectSensor_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IKinectSensor_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IKinectSensor_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IKinectSensor_SubscribeIsAvailableChanged(This,waitableHandle)	\
    ( (This)->lpVtbl -> SubscribeIsAvailableChanged(This,waitableHandle) ) 

#define IKinectSensor_UnsubscribeIsAvailableChanged(This,waitableHandle)	\
    ( (This)->lpVtbl -> UnsubscribeIsAvailableChanged(This,waitableHandle) ) 

#define IKinectSensor_GetIsAvailableChangedEventData(This,waitableHandle,eventData)	\
    ( (This)->lpVtbl -> GetIsAvailableChangedEventData(This,waitableHandle,eventData) ) 

#define IKinectSensor_Open(This)	\
    ( (This)->lpVtbl -> Open(This) ) 

#define IKinectSensor_Close(This)	\
    ( (This)->lpVtbl -> Close(This) ) 

#define IKinectSensor_get_IsOpen(This,isOpen)	\
    ( (This)->lpVtbl -> get_IsOpen(This,isOpen) ) 

#define IKinectSensor_get_IsAvailable(This,isAvailable)	\
    ( (This)->lpVtbl -> get_IsAvailable(This,isAvailable) ) 

#define IKinectSensor_get_ColorFrameSource(This,colorFrameSource)	\
    ( (This)->lpVtbl -> get_ColorFrameSource(This,colorFrameSource) ) 

#define IKinectSensor_get_DepthFrameSource(This,depthFrameSource)	\
    ( (This)->lpVtbl -> get_DepthFrameSource(This,depthFrameSource) ) 

#define IKinectSensor_get_BodyFrameSource(This,bodyFrameSource)	\
    ( (This)->lpVtbl -> get_BodyFrameSource(This,bodyFrameSource) ) 

#define IKinectSensor_get_BodyIndexFrameSource(This,bodyIndexFrameSource)	\
    ( (This)->lpVtbl -> get_BodyIndexFrameSource(This,bodyIndexFrameSource) ) 

#define IKinectSensor_get_InfraredFrameSource(This,infraredFrameSource)	\
    ( (This)->lpVtbl -> get_InfraredFrameSource(This,infraredFrameSource) ) 

#define IKinectSensor_get_LongExposureInfraredFrameSource(This,longExposureInfraredFrameSource)	\
    ( (This)->lpVtbl -> get_LongExposureInfraredFrameSource(This,longExposureInfraredFrameSource) ) 

#define IKinectSensor_get_AudioSource(This,audioSource)	\
    ( (This)->lpVtbl -> get_AudioSource(This,audioSource) ) 

#define IKinectSensor_OpenMultiSourceFrameReader(This,enabledFrameSourceTypes,multiSourceFrameReader)	\
    ( (This)->lpVtbl -> OpenMultiSourceFrameReader(This,enabledFrameSourceTypes,multiSourceFrameReader) ) 

#define IKinectSensor_get_CoordinateMapper(This,coordinateMapper)	\
    ( (This)->lpVtbl -> get_CoordinateMapper(This,coordinateMapper) ) 

#define IKinectSensor_get_UniqueKinectId(This,bufferSize,uniqueKinectId)	\
    ( (This)->lpVtbl -> get_UniqueKinectId(This,bufferSize,uniqueKinectId) ) 

#define IKinectSensor_get_KinectCapabilities(This,capabilities)	\
    ( (This)->lpVtbl -> get_KinectCapabilities(This,capabilities) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IKinectSensor_INTERFACE_DEFINED__ */


#ifndef __IIsAvailableChangedEventArgs_INTERFACE_DEFINED__
#define __IIsAvailableChangedEventArgs_INTERFACE_DEFINED__

/* interface IIsAvailableChangedEventArgs */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IIsAvailableChangedEventArgs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3A6DD52E-967F-4982-B3D9-74B9E1A044C9")
    IIsAvailableChangedEventArgs : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsAvailable( 
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isAvailable) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IIsAvailableChangedEventArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IIsAvailableChangedEventArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IIsAvailableChangedEventArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IIsAvailableChangedEventArgs * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsAvailable )( 
            IIsAvailableChangedEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isAvailable);
        
        END_INTERFACE
    } IIsAvailableChangedEventArgsVtbl;

    interface IIsAvailableChangedEventArgs
    {
        CONST_VTBL struct IIsAvailableChangedEventArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IIsAvailableChangedEventArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IIsAvailableChangedEventArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IIsAvailableChangedEventArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IIsAvailableChangedEventArgs_get_IsAvailable(This,isAvailable)	\
    ( (This)->lpVtbl -> get_IsAvailable(This,isAvailable) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IIsAvailableChangedEventArgs_INTERFACE_DEFINED__ */


#ifndef __IFrameDescription_INTERFACE_DEFINED__
#define __IFrameDescription_INTERFACE_DEFINED__

/* interface IFrameDescription */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IFrameDescription;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("21F6EFB7-EB6D-48F4-9C08-181A87BF0C98")
    IFrameDescription : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Width( 
            /* [annotation][out][retval] */ 
            _Out_range_(512, 1920)  int *width) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Height( 
            /* [annotation][out][retval] */ 
            _Out_range_(424, 1080)  int *height) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_HorizontalFieldOfView( 
            /* [annotation][out][retval] */ 
            _Out_  float *horizontalFieldOfView) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_VerticalFieldOfView( 
            /* [annotation][out][retval] */ 
            _Out_  float *verticalFieldOfView) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_DiagonalFieldOfView( 
            /* [annotation][out][retval] */ 
            _Out_  float *diagonalFieldOfView) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_LengthInPixels( 
            /* [annotation][out][retval] */ 
            _Out_range_(512*424, 1920*1080)  unsigned int *lengthInPixels) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_BytesPerPixel( 
            /* [annotation][out][retval] */ 
            _Out_range_(1, 4)  unsigned int *bytesPerPixel) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IFrameDescriptionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IFrameDescription * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IFrameDescription * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IFrameDescription * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IFrameDescription * This,
            /* [annotation][out][retval] */ 
            _Out_range_(512, 1920)  int *width);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IFrameDescription * This,
            /* [annotation][out][retval] */ 
            _Out_range_(424, 1080)  int *height);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_HorizontalFieldOfView )( 
            IFrameDescription * This,
            /* [annotation][out][retval] */ 
            _Out_  float *horizontalFieldOfView);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_VerticalFieldOfView )( 
            IFrameDescription * This,
            /* [annotation][out][retval] */ 
            _Out_  float *verticalFieldOfView);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_DiagonalFieldOfView )( 
            IFrameDescription * This,
            /* [annotation][out][retval] */ 
            _Out_  float *diagonalFieldOfView);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_LengthInPixels )( 
            IFrameDescription * This,
            /* [annotation][out][retval] */ 
            _Out_range_(512*424, 1920*1080)  unsigned int *lengthInPixels);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_BytesPerPixel )( 
            IFrameDescription * This,
            /* [annotation][out][retval] */ 
            _Out_range_(1, 4)  unsigned int *bytesPerPixel);
        
        END_INTERFACE
    } IFrameDescriptionVtbl;

    interface IFrameDescription
    {
        CONST_VTBL struct IFrameDescriptionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFrameDescription_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IFrameDescription_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IFrameDescription_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IFrameDescription_get_Width(This,width)	\
    ( (This)->lpVtbl -> get_Width(This,width) ) 

#define IFrameDescription_get_Height(This,height)	\
    ( (This)->lpVtbl -> get_Height(This,height) ) 

#define IFrameDescription_get_HorizontalFieldOfView(This,horizontalFieldOfView)	\
    ( (This)->lpVtbl -> get_HorizontalFieldOfView(This,horizontalFieldOfView) ) 

#define IFrameDescription_get_VerticalFieldOfView(This,verticalFieldOfView)	\
    ( (This)->lpVtbl -> get_VerticalFieldOfView(This,verticalFieldOfView) ) 

#define IFrameDescription_get_DiagonalFieldOfView(This,diagonalFieldOfView)	\
    ( (This)->lpVtbl -> get_DiagonalFieldOfView(This,diagonalFieldOfView) ) 

#define IFrameDescription_get_LengthInPixels(This,lengthInPixels)	\
    ( (This)->lpVtbl -> get_LengthInPixels(This,lengthInPixels) ) 

#define IFrameDescription_get_BytesPerPixel(This,bytesPerPixel)	\
    ( (This)->lpVtbl -> get_BytesPerPixel(This,bytesPerPixel) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IFrameDescription_INTERFACE_DEFINED__ */


#ifndef __IFrameCapturedEventArgs_INTERFACE_DEFINED__
#define __IFrameCapturedEventArgs_INTERFACE_DEFINED__

/* interface IFrameCapturedEventArgs */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IFrameCapturedEventArgs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("24CBAB8E-DF1A-4FA8-827E-C1B27A44A3A1")
    IFrameCapturedEventArgs : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FrameType( 
            /* [annotation][out][retval] */ 
            _Out_  FrameSourceTypes *frameType) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FrameStatus( 
            /* [annotation][out][retval] */ 
            _Out_  FrameCapturedStatus *frameStatus) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_RelativeTime( 
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IFrameCapturedEventArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IFrameCapturedEventArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IFrameCapturedEventArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IFrameCapturedEventArgs * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FrameType )( 
            IFrameCapturedEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  FrameSourceTypes *frameType);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FrameStatus )( 
            IFrameCapturedEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  FrameCapturedStatus *frameStatus);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_RelativeTime )( 
            IFrameCapturedEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime);
        
        END_INTERFACE
    } IFrameCapturedEventArgsVtbl;

    interface IFrameCapturedEventArgs
    {
        CONST_VTBL struct IFrameCapturedEventArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFrameCapturedEventArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IFrameCapturedEventArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IFrameCapturedEventArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IFrameCapturedEventArgs_get_FrameType(This,frameType)	\
    ( (This)->lpVtbl -> get_FrameType(This,frameType) ) 

#define IFrameCapturedEventArgs_get_FrameStatus(This,frameStatus)	\
    ( (This)->lpVtbl -> get_FrameStatus(This,frameStatus) ) 

#define IFrameCapturedEventArgs_get_RelativeTime(This,relativeTime)	\
    ( (This)->lpVtbl -> get_RelativeTime(This,relativeTime) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IFrameCapturedEventArgs_INTERFACE_DEFINED__ */


#ifndef __IMultiSourceFrameReader_INTERFACE_DEFINED__
#define __IMultiSourceFrameReader_INTERFACE_DEFINED__

/* interface IMultiSourceFrameReader */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IMultiSourceFrameReader;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C0F6432B-9FFE-4AB3-A683-F37C72BBB158")
    IMultiSourceFrameReader : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SubscribeMultiSourceFrameArrived( 
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnsubscribeMultiSourceFrameArrived( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMultiSourceFrameArrivedEventData( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IMultiSourceFrameArrivedEventArgs **eventData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AcquireLatestFrame( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IMultiSourceFrame **multiSourceFrame) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FrameSourceTypes( 
            /* [annotation][out][retval] */ 
            _Out_  DWORD *enabledFrameSourceTypes) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsPaused( 
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isPaused) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_IsPaused( 
            BOOLEAN isPaused) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_KinectSensor( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IKinectSensor **sensor) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IMultiSourceFrameReaderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMultiSourceFrameReader * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMultiSourceFrameReader * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMultiSourceFrameReader * This);
        
        HRESULT ( STDMETHODCALLTYPE *SubscribeMultiSourceFrameArrived )( 
            IMultiSourceFrameReader * This,
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *UnsubscribeMultiSourceFrameArrived )( 
            IMultiSourceFrameReader * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *GetMultiSourceFrameArrivedEventData )( 
            IMultiSourceFrameReader * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IMultiSourceFrameArrivedEventArgs **eventData);
        
        HRESULT ( STDMETHODCALLTYPE *AcquireLatestFrame )( 
            IMultiSourceFrameReader * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IMultiSourceFrame **multiSourceFrame);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FrameSourceTypes )( 
            IMultiSourceFrameReader * This,
            /* [annotation][out][retval] */ 
            _Out_  DWORD *enabledFrameSourceTypes);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsPaused )( 
            IMultiSourceFrameReader * This,
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isPaused);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_IsPaused )( 
            IMultiSourceFrameReader * This,
            BOOLEAN isPaused);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_KinectSensor )( 
            IMultiSourceFrameReader * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IKinectSensor **sensor);
        
        END_INTERFACE
    } IMultiSourceFrameReaderVtbl;

    interface IMultiSourceFrameReader
    {
        CONST_VTBL struct IMultiSourceFrameReaderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMultiSourceFrameReader_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMultiSourceFrameReader_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMultiSourceFrameReader_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMultiSourceFrameReader_SubscribeMultiSourceFrameArrived(This,waitableHandle)	\
    ( (This)->lpVtbl -> SubscribeMultiSourceFrameArrived(This,waitableHandle) ) 

#define IMultiSourceFrameReader_UnsubscribeMultiSourceFrameArrived(This,waitableHandle)	\
    ( (This)->lpVtbl -> UnsubscribeMultiSourceFrameArrived(This,waitableHandle) ) 

#define IMultiSourceFrameReader_GetMultiSourceFrameArrivedEventData(This,waitableHandle,eventData)	\
    ( (This)->lpVtbl -> GetMultiSourceFrameArrivedEventData(This,waitableHandle,eventData) ) 

#define IMultiSourceFrameReader_AcquireLatestFrame(This,multiSourceFrame)	\
    ( (This)->lpVtbl -> AcquireLatestFrame(This,multiSourceFrame) ) 

#define IMultiSourceFrameReader_get_FrameSourceTypes(This,enabledFrameSourceTypes)	\
    ( (This)->lpVtbl -> get_FrameSourceTypes(This,enabledFrameSourceTypes) ) 

#define IMultiSourceFrameReader_get_IsPaused(This,isPaused)	\
    ( (This)->lpVtbl -> get_IsPaused(This,isPaused) ) 

#define IMultiSourceFrameReader_put_IsPaused(This,isPaused)	\
    ( (This)->lpVtbl -> put_IsPaused(This,isPaused) ) 

#define IMultiSourceFrameReader_get_KinectSensor(This,sensor)	\
    ( (This)->lpVtbl -> get_KinectSensor(This,sensor) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMultiSourceFrameReader_INTERFACE_DEFINED__ */


#ifndef __IMultiSourceFrameArrivedEventArgs_INTERFACE_DEFINED__
#define __IMultiSourceFrameArrivedEventArgs_INTERFACE_DEFINED__

/* interface IMultiSourceFrameArrivedEventArgs */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IMultiSourceFrameArrivedEventArgs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3532F40B-D908-451D-BBF4-6CA73B782558")
    IMultiSourceFrameArrivedEventArgs : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FrameReference( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IMultiSourceFrameReference **frames) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IMultiSourceFrameArrivedEventArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMultiSourceFrameArrivedEventArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMultiSourceFrameArrivedEventArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMultiSourceFrameArrivedEventArgs * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FrameReference )( 
            IMultiSourceFrameArrivedEventArgs * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IMultiSourceFrameReference **frames);
        
        END_INTERFACE
    } IMultiSourceFrameArrivedEventArgsVtbl;

    interface IMultiSourceFrameArrivedEventArgs
    {
        CONST_VTBL struct IMultiSourceFrameArrivedEventArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMultiSourceFrameArrivedEventArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMultiSourceFrameArrivedEventArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMultiSourceFrameArrivedEventArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMultiSourceFrameArrivedEventArgs_get_FrameReference(This,frames)	\
    ( (This)->lpVtbl -> get_FrameReference(This,frames) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMultiSourceFrameArrivedEventArgs_INTERFACE_DEFINED__ */


#ifndef __IMultiSourceFrameReference_INTERFACE_DEFINED__
#define __IMultiSourceFrameReference_INTERFACE_DEFINED__

/* interface IMultiSourceFrameReference */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IMultiSourceFrameReference;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("DD70E845-E283-4DD1-8DAF-FC259AC5F9E3")
    IMultiSourceFrameReference : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE AcquireFrame( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IMultiSourceFrame **frame) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IMultiSourceFrameReferenceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMultiSourceFrameReference * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMultiSourceFrameReference * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMultiSourceFrameReference * This);
        
        HRESULT ( STDMETHODCALLTYPE *AcquireFrame )( 
            IMultiSourceFrameReference * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IMultiSourceFrame **frame);
        
        END_INTERFACE
    } IMultiSourceFrameReferenceVtbl;

    interface IMultiSourceFrameReference
    {
        CONST_VTBL struct IMultiSourceFrameReferenceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMultiSourceFrameReference_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMultiSourceFrameReference_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMultiSourceFrameReference_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMultiSourceFrameReference_AcquireFrame(This,frame)	\
    ( (This)->lpVtbl -> AcquireFrame(This,frame) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMultiSourceFrameReference_INTERFACE_DEFINED__ */


#ifndef __IMultiSourceFrame_INTERFACE_DEFINED__
#define __IMultiSourceFrame_INTERFACE_DEFINED__

/* interface IMultiSourceFrame */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IMultiSourceFrame;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("29A63AFB-76CE-4359-895A-997F1E094D1C")
    IMultiSourceFrame : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ColorFrameReference( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IColorFrameReference **colorFrameReference) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_DepthFrameReference( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IDepthFrameReference **depthFrameReference) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_BodyFrameReference( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyFrameReference **bodyFrameReference) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_BodyIndexFrameReference( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyIndexFrameReference **bodyIndexFrameReference) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_InfraredFrameReference( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IInfraredFrameReference **infraredFrameReference) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_LongExposureInfraredFrameReference( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  ILongExposureInfraredFrameReference **longExposureInfraredFrameReference) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IMultiSourceFrameVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMultiSourceFrame * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMultiSourceFrame * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMultiSourceFrame * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_ColorFrameReference )( 
            IMultiSourceFrame * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IColorFrameReference **colorFrameReference);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_DepthFrameReference )( 
            IMultiSourceFrame * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IDepthFrameReference **depthFrameReference);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_BodyFrameReference )( 
            IMultiSourceFrame * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyFrameReference **bodyFrameReference);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_BodyIndexFrameReference )( 
            IMultiSourceFrame * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyIndexFrameReference **bodyIndexFrameReference);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_InfraredFrameReference )( 
            IMultiSourceFrame * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IInfraredFrameReference **infraredFrameReference);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_LongExposureInfraredFrameReference )( 
            IMultiSourceFrame * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  ILongExposureInfraredFrameReference **longExposureInfraredFrameReference);
        
        END_INTERFACE
    } IMultiSourceFrameVtbl;

    interface IMultiSourceFrame
    {
        CONST_VTBL struct IMultiSourceFrameVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMultiSourceFrame_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMultiSourceFrame_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMultiSourceFrame_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMultiSourceFrame_get_ColorFrameReference(This,colorFrameReference)	\
    ( (This)->lpVtbl -> get_ColorFrameReference(This,colorFrameReference) ) 

#define IMultiSourceFrame_get_DepthFrameReference(This,depthFrameReference)	\
    ( (This)->lpVtbl -> get_DepthFrameReference(This,depthFrameReference) ) 

#define IMultiSourceFrame_get_BodyFrameReference(This,bodyFrameReference)	\
    ( (This)->lpVtbl -> get_BodyFrameReference(This,bodyFrameReference) ) 

#define IMultiSourceFrame_get_BodyIndexFrameReference(This,bodyIndexFrameReference)	\
    ( (This)->lpVtbl -> get_BodyIndexFrameReference(This,bodyIndexFrameReference) ) 

#define IMultiSourceFrame_get_InfraredFrameReference(This,infraredFrameReference)	\
    ( (This)->lpVtbl -> get_InfraredFrameReference(This,infraredFrameReference) ) 

#define IMultiSourceFrame_get_LongExposureInfraredFrameReference(This,longExposureInfraredFrameReference)	\
    ( (This)->lpVtbl -> get_LongExposureInfraredFrameReference(This,longExposureInfraredFrameReference) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMultiSourceFrame_INTERFACE_DEFINED__ */


#ifndef __IColorFrameReference_INTERFACE_DEFINED__
#define __IColorFrameReference_INTERFACE_DEFINED__

/* interface IColorFrameReference */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IColorFrameReference;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5CC49E38-9BBD-48BE-A770-FD30EA405247")
    IColorFrameReference : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE AcquireFrame( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IColorFrame **colorFrame) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_RelativeTime( 
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IColorFrameReferenceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IColorFrameReference * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IColorFrameReference * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IColorFrameReference * This);
        
        HRESULT ( STDMETHODCALLTYPE *AcquireFrame )( 
            IColorFrameReference * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IColorFrame **colorFrame);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_RelativeTime )( 
            IColorFrameReference * This,
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime);
        
        END_INTERFACE
    } IColorFrameReferenceVtbl;

    interface IColorFrameReference
    {
        CONST_VTBL struct IColorFrameReferenceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IColorFrameReference_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IColorFrameReference_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IColorFrameReference_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IColorFrameReference_AcquireFrame(This,colorFrame)	\
    ( (This)->lpVtbl -> AcquireFrame(This,colorFrame) ) 

#define IColorFrameReference_get_RelativeTime(This,relativeTime)	\
    ( (This)->lpVtbl -> get_RelativeTime(This,relativeTime) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IColorFrameReference_INTERFACE_DEFINED__ */


#ifndef __IColorFrameArrivedEventArgs_INTERFACE_DEFINED__
#define __IColorFrameArrivedEventArgs_INTERFACE_DEFINED__

/* interface IColorFrameArrivedEventArgs */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IColorFrameArrivedEventArgs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("82A2E32F-4AE5-4614-88BB-DCC5AE0CEAED")
    IColorFrameArrivedEventArgs : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FrameReference( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IColorFrameReference **colorFrameReference) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IColorFrameArrivedEventArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IColorFrameArrivedEventArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IColorFrameArrivedEventArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IColorFrameArrivedEventArgs * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FrameReference )( 
            IColorFrameArrivedEventArgs * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IColorFrameReference **colorFrameReference);
        
        END_INTERFACE
    } IColorFrameArrivedEventArgsVtbl;

    interface IColorFrameArrivedEventArgs
    {
        CONST_VTBL struct IColorFrameArrivedEventArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IColorFrameArrivedEventArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IColorFrameArrivedEventArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IColorFrameArrivedEventArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IColorFrameArrivedEventArgs_get_FrameReference(This,colorFrameReference)	\
    ( (This)->lpVtbl -> get_FrameReference(This,colorFrameReference) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IColorFrameArrivedEventArgs_INTERFACE_DEFINED__ */


#ifndef __IColorFrameSource_INTERFACE_DEFINED__
#define __IColorFrameSource_INTERFACE_DEFINED__

/* interface IColorFrameSource */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IColorFrameSource;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("57621D82-D8EE-4783-B412-F7E019C96CFD")
    IColorFrameSource : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SubscribeFrameCaptured( 
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnsubscribeFrameCaptured( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFrameCapturedEventData( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IFrameCapturedEventArgs **eventData) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsActive( 
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isActive) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OpenReader( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IColorFrameReader **reader) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateFrameDescription( 
            ColorImageFormat format,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IFrameDescription **frameDescription) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FrameDescription( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IFrameDescription **rawFrameDescription) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_KinectSensor( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IKinectSensor **sensor) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IColorFrameSourceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IColorFrameSource * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IColorFrameSource * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IColorFrameSource * This);
        
        HRESULT ( STDMETHODCALLTYPE *SubscribeFrameCaptured )( 
            IColorFrameSource * This,
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *UnsubscribeFrameCaptured )( 
            IColorFrameSource * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *GetFrameCapturedEventData )( 
            IColorFrameSource * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IFrameCapturedEventArgs **eventData);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsActive )( 
            IColorFrameSource * This,
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isActive);
        
        HRESULT ( STDMETHODCALLTYPE *OpenReader )( 
            IColorFrameSource * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IColorFrameReader **reader);
        
        HRESULT ( STDMETHODCALLTYPE *CreateFrameDescription )( 
            IColorFrameSource * This,
            ColorImageFormat format,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IFrameDescription **frameDescription);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FrameDescription )( 
            IColorFrameSource * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IFrameDescription **rawFrameDescription);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_KinectSensor )( 
            IColorFrameSource * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IKinectSensor **sensor);
        
        END_INTERFACE
    } IColorFrameSourceVtbl;

    interface IColorFrameSource
    {
        CONST_VTBL struct IColorFrameSourceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IColorFrameSource_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IColorFrameSource_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IColorFrameSource_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IColorFrameSource_SubscribeFrameCaptured(This,waitableHandle)	\
    ( (This)->lpVtbl -> SubscribeFrameCaptured(This,waitableHandle) ) 

#define IColorFrameSource_UnsubscribeFrameCaptured(This,waitableHandle)	\
    ( (This)->lpVtbl -> UnsubscribeFrameCaptured(This,waitableHandle) ) 

#define IColorFrameSource_GetFrameCapturedEventData(This,waitableHandle,eventData)	\
    ( (This)->lpVtbl -> GetFrameCapturedEventData(This,waitableHandle,eventData) ) 

#define IColorFrameSource_get_IsActive(This,isActive)	\
    ( (This)->lpVtbl -> get_IsActive(This,isActive) ) 

#define IColorFrameSource_OpenReader(This,reader)	\
    ( (This)->lpVtbl -> OpenReader(This,reader) ) 

#define IColorFrameSource_CreateFrameDescription(This,format,frameDescription)	\
    ( (This)->lpVtbl -> CreateFrameDescription(This,format,frameDescription) ) 

#define IColorFrameSource_get_FrameDescription(This,rawFrameDescription)	\
    ( (This)->lpVtbl -> get_FrameDescription(This,rawFrameDescription) ) 

#define IColorFrameSource_get_KinectSensor(This,sensor)	\
    ( (This)->lpVtbl -> get_KinectSensor(This,sensor) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IColorFrameSource_INTERFACE_DEFINED__ */


#ifndef __IColorFrameReader_INTERFACE_DEFINED__
#define __IColorFrameReader_INTERFACE_DEFINED__

/* interface IColorFrameReader */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IColorFrameReader;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9BEA498C-C59C-4653-AAF9-D884BAB7C35B")
    IColorFrameReader : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SubscribeFrameArrived( 
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnsubscribeFrameArrived( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFrameArrivedEventData( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IColorFrameArrivedEventArgs **eventData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AcquireLatestFrame( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IColorFrame **colorFrame) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsPaused( 
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isPaused) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_IsPaused( 
            BOOLEAN isPaused) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ColorFrameSource( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IColorFrameSource **colorFrameSource) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IColorFrameReaderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IColorFrameReader * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IColorFrameReader * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IColorFrameReader * This);
        
        HRESULT ( STDMETHODCALLTYPE *SubscribeFrameArrived )( 
            IColorFrameReader * This,
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *UnsubscribeFrameArrived )( 
            IColorFrameReader * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *GetFrameArrivedEventData )( 
            IColorFrameReader * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IColorFrameArrivedEventArgs **eventData);
        
        HRESULT ( STDMETHODCALLTYPE *AcquireLatestFrame )( 
            IColorFrameReader * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IColorFrame **colorFrame);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsPaused )( 
            IColorFrameReader * This,
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isPaused);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_IsPaused )( 
            IColorFrameReader * This,
            BOOLEAN isPaused);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_ColorFrameSource )( 
            IColorFrameReader * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IColorFrameSource **colorFrameSource);
        
        END_INTERFACE
    } IColorFrameReaderVtbl;

    interface IColorFrameReader
    {
        CONST_VTBL struct IColorFrameReaderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IColorFrameReader_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IColorFrameReader_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IColorFrameReader_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IColorFrameReader_SubscribeFrameArrived(This,waitableHandle)	\
    ( (This)->lpVtbl -> SubscribeFrameArrived(This,waitableHandle) ) 

#define IColorFrameReader_UnsubscribeFrameArrived(This,waitableHandle)	\
    ( (This)->lpVtbl -> UnsubscribeFrameArrived(This,waitableHandle) ) 

#define IColorFrameReader_GetFrameArrivedEventData(This,waitableHandle,eventData)	\
    ( (This)->lpVtbl -> GetFrameArrivedEventData(This,waitableHandle,eventData) ) 

#define IColorFrameReader_AcquireLatestFrame(This,colorFrame)	\
    ( (This)->lpVtbl -> AcquireLatestFrame(This,colorFrame) ) 

#define IColorFrameReader_get_IsPaused(This,isPaused)	\
    ( (This)->lpVtbl -> get_IsPaused(This,isPaused) ) 

#define IColorFrameReader_put_IsPaused(This,isPaused)	\
    ( (This)->lpVtbl -> put_IsPaused(This,isPaused) ) 

#define IColorFrameReader_get_ColorFrameSource(This,colorFrameSource)	\
    ( (This)->lpVtbl -> get_ColorFrameSource(This,colorFrameSource) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IColorFrameReader_INTERFACE_DEFINED__ */


#ifndef __IColorFrame_INTERFACE_DEFINED__
#define __IColorFrame_INTERFACE_DEFINED__

/* interface IColorFrame */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IColorFrame;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("39D05803-8803-4E86-AD9F-13F6954E4ACA")
    IColorFrame : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_RawColorImageFormat( 
            /* [annotation][out][retval] */ 
            _Out_  ColorImageFormat *rawColorImageFormat) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FrameDescription( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IFrameDescription **rawFrameDescription) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CopyRawFrameDataToArray( 
            UINT capacity,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(capacity)  BYTE *frameData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AccessRawUnderlyingBuffer( 
            /* [annotation][out] */ 
            _Out_  UINT *capacity,
            /* [annotation][size_is][size_is][out] */ 
            _Outptr_result_buffer_(*capacity)  BYTE **buffer) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CopyConvertedFrameDataToArray( 
            UINT capacity,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(capacity)  BYTE *frameData,
            ColorImageFormat colorFormat) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateFrameDescription( 
            ColorImageFormat format,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IFrameDescription **frameDescription) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ColorCameraSettings( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IColorCameraSettings **colorCameraSettings) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_RelativeTime( 
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ColorFrameSource( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IColorFrameSource **colorFrameSource) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IColorFrameVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IColorFrame * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IColorFrame * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IColorFrame * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_RawColorImageFormat )( 
            IColorFrame * This,
            /* [annotation][out][retval] */ 
            _Out_  ColorImageFormat *rawColorImageFormat);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FrameDescription )( 
            IColorFrame * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IFrameDescription **rawFrameDescription);
        
        HRESULT ( STDMETHODCALLTYPE *CopyRawFrameDataToArray )( 
            IColorFrame * This,
            UINT capacity,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(capacity)  BYTE *frameData);
        
        HRESULT ( STDMETHODCALLTYPE *AccessRawUnderlyingBuffer )( 
            IColorFrame * This,
            /* [annotation][out] */ 
            _Out_  UINT *capacity,
            /* [annotation][size_is][size_is][out] */ 
            _Outptr_result_buffer_(*capacity)  BYTE **buffer);
        
        HRESULT ( STDMETHODCALLTYPE *CopyConvertedFrameDataToArray )( 
            IColorFrame * This,
            UINT capacity,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(capacity)  BYTE *frameData,
            ColorImageFormat colorFormat);
        
        HRESULT ( STDMETHODCALLTYPE *CreateFrameDescription )( 
            IColorFrame * This,
            ColorImageFormat format,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IFrameDescription **frameDescription);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_ColorCameraSettings )( 
            IColorFrame * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IColorCameraSettings **colorCameraSettings);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_RelativeTime )( 
            IColorFrame * This,
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_ColorFrameSource )( 
            IColorFrame * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IColorFrameSource **colorFrameSource);
        
        END_INTERFACE
    } IColorFrameVtbl;

    interface IColorFrame
    {
        CONST_VTBL struct IColorFrameVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IColorFrame_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IColorFrame_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IColorFrame_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IColorFrame_get_RawColorImageFormat(This,rawColorImageFormat)	\
    ( (This)->lpVtbl -> get_RawColorImageFormat(This,rawColorImageFormat) ) 

#define IColorFrame_get_FrameDescription(This,rawFrameDescription)	\
    ( (This)->lpVtbl -> get_FrameDescription(This,rawFrameDescription) ) 

#define IColorFrame_CopyRawFrameDataToArray(This,capacity,frameData)	\
    ( (This)->lpVtbl -> CopyRawFrameDataToArray(This,capacity,frameData) ) 

#define IColorFrame_AccessRawUnderlyingBuffer(This,capacity,buffer)	\
    ( (This)->lpVtbl -> AccessRawUnderlyingBuffer(This,capacity,buffer) ) 

#define IColorFrame_CopyConvertedFrameDataToArray(This,capacity,frameData,colorFormat)	\
    ( (This)->lpVtbl -> CopyConvertedFrameDataToArray(This,capacity,frameData,colorFormat) ) 

#define IColorFrame_CreateFrameDescription(This,format,frameDescription)	\
    ( (This)->lpVtbl -> CreateFrameDescription(This,format,frameDescription) ) 

#define IColorFrame_get_ColorCameraSettings(This,colorCameraSettings)	\
    ( (This)->lpVtbl -> get_ColorCameraSettings(This,colorCameraSettings) ) 

#define IColorFrame_get_RelativeTime(This,relativeTime)	\
    ( (This)->lpVtbl -> get_RelativeTime(This,relativeTime) ) 

#define IColorFrame_get_ColorFrameSource(This,colorFrameSource)	\
    ( (This)->lpVtbl -> get_ColorFrameSource(This,colorFrameSource) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IColorFrame_INTERFACE_DEFINED__ */


#ifndef __IDepthFrameReference_INTERFACE_DEFINED__
#define __IDepthFrameReference_INTERFACE_DEFINED__

/* interface IDepthFrameReference */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IDepthFrameReference;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("20621E5E-ABC9-4EBD-A7EE-4C77EDD0152A")
    IDepthFrameReference : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE AcquireFrame( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IDepthFrame **depthFrame) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_RelativeTime( 
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IDepthFrameReferenceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDepthFrameReference * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDepthFrameReference * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDepthFrameReference * This);
        
        HRESULT ( STDMETHODCALLTYPE *AcquireFrame )( 
            IDepthFrameReference * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IDepthFrame **depthFrame);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_RelativeTime )( 
            IDepthFrameReference * This,
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime);
        
        END_INTERFACE
    } IDepthFrameReferenceVtbl;

    interface IDepthFrameReference
    {
        CONST_VTBL struct IDepthFrameReferenceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDepthFrameReference_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDepthFrameReference_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDepthFrameReference_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDepthFrameReference_AcquireFrame(This,depthFrame)	\
    ( (This)->lpVtbl -> AcquireFrame(This,depthFrame) ) 

#define IDepthFrameReference_get_RelativeTime(This,relativeTime)	\
    ( (This)->lpVtbl -> get_RelativeTime(This,relativeTime) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDepthFrameReference_INTERFACE_DEFINED__ */


#ifndef __IDepthFrameArrivedEventArgs_INTERFACE_DEFINED__
#define __IDepthFrameArrivedEventArgs_INTERFACE_DEFINED__

/* interface IDepthFrameArrivedEventArgs */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IDepthFrameArrivedEventArgs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2B01BCB8-29D7-4726-860C-6DA56664AA81")
    IDepthFrameArrivedEventArgs : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FrameReference( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IDepthFrameReference **depthFrameReference) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IDepthFrameArrivedEventArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDepthFrameArrivedEventArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDepthFrameArrivedEventArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDepthFrameArrivedEventArgs * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FrameReference )( 
            IDepthFrameArrivedEventArgs * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IDepthFrameReference **depthFrameReference);
        
        END_INTERFACE
    } IDepthFrameArrivedEventArgsVtbl;

    interface IDepthFrameArrivedEventArgs
    {
        CONST_VTBL struct IDepthFrameArrivedEventArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDepthFrameArrivedEventArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDepthFrameArrivedEventArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDepthFrameArrivedEventArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDepthFrameArrivedEventArgs_get_FrameReference(This,depthFrameReference)	\
    ( (This)->lpVtbl -> get_FrameReference(This,depthFrameReference) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDepthFrameArrivedEventArgs_INTERFACE_DEFINED__ */


#ifndef __IDepthFrameSource_INTERFACE_DEFINED__
#define __IDepthFrameSource_INTERFACE_DEFINED__

/* interface IDepthFrameSource */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IDepthFrameSource;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C428D558-5E46-490A-B699-D1DDDAA24150")
    IDepthFrameSource : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SubscribeFrameCaptured( 
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnsubscribeFrameCaptured( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFrameCapturedEventData( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IFrameCapturedEventArgs **eventData) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsActive( 
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isActive) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OpenReader( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IDepthFrameReader **reader) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_DepthMinReliableDistance( 
            /* [annotation][out][retval] */ 
            _Out_  UINT16 *depthMinReliableDistance) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_DepthMaxReliableDistance( 
            /* [annotation][out][retval] */ 
            _Out_  UINT16 *depthMaxReliableDistance) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FrameDescription( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IFrameDescription **frameDescription) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_KinectSensor( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IKinectSensor **sensor) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IDepthFrameSourceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDepthFrameSource * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDepthFrameSource * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDepthFrameSource * This);
        
        HRESULT ( STDMETHODCALLTYPE *SubscribeFrameCaptured )( 
            IDepthFrameSource * This,
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *UnsubscribeFrameCaptured )( 
            IDepthFrameSource * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *GetFrameCapturedEventData )( 
            IDepthFrameSource * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IFrameCapturedEventArgs **eventData);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsActive )( 
            IDepthFrameSource * This,
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isActive);
        
        HRESULT ( STDMETHODCALLTYPE *OpenReader )( 
            IDepthFrameSource * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IDepthFrameReader **reader);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_DepthMinReliableDistance )( 
            IDepthFrameSource * This,
            /* [annotation][out][retval] */ 
            _Out_  UINT16 *depthMinReliableDistance);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_DepthMaxReliableDistance )( 
            IDepthFrameSource * This,
            /* [annotation][out][retval] */ 
            _Out_  UINT16 *depthMaxReliableDistance);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FrameDescription )( 
            IDepthFrameSource * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IFrameDescription **frameDescription);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_KinectSensor )( 
            IDepthFrameSource * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IKinectSensor **sensor);
        
        END_INTERFACE
    } IDepthFrameSourceVtbl;

    interface IDepthFrameSource
    {
        CONST_VTBL struct IDepthFrameSourceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDepthFrameSource_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDepthFrameSource_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDepthFrameSource_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDepthFrameSource_SubscribeFrameCaptured(This,waitableHandle)	\
    ( (This)->lpVtbl -> SubscribeFrameCaptured(This,waitableHandle) ) 

#define IDepthFrameSource_UnsubscribeFrameCaptured(This,waitableHandle)	\
    ( (This)->lpVtbl -> UnsubscribeFrameCaptured(This,waitableHandle) ) 

#define IDepthFrameSource_GetFrameCapturedEventData(This,waitableHandle,eventData)	\
    ( (This)->lpVtbl -> GetFrameCapturedEventData(This,waitableHandle,eventData) ) 

#define IDepthFrameSource_get_IsActive(This,isActive)	\
    ( (This)->lpVtbl -> get_IsActive(This,isActive) ) 

#define IDepthFrameSource_OpenReader(This,reader)	\
    ( (This)->lpVtbl -> OpenReader(This,reader) ) 

#define IDepthFrameSource_get_DepthMinReliableDistance(This,depthMinReliableDistance)	\
    ( (This)->lpVtbl -> get_DepthMinReliableDistance(This,depthMinReliableDistance) ) 

#define IDepthFrameSource_get_DepthMaxReliableDistance(This,depthMaxReliableDistance)	\
    ( (This)->lpVtbl -> get_DepthMaxReliableDistance(This,depthMaxReliableDistance) ) 

#define IDepthFrameSource_get_FrameDescription(This,frameDescription)	\
    ( (This)->lpVtbl -> get_FrameDescription(This,frameDescription) ) 

#define IDepthFrameSource_get_KinectSensor(This,sensor)	\
    ( (This)->lpVtbl -> get_KinectSensor(This,sensor) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDepthFrameSource_INTERFACE_DEFINED__ */


#ifndef __IDepthFrameReader_INTERFACE_DEFINED__
#define __IDepthFrameReader_INTERFACE_DEFINED__

/* interface IDepthFrameReader */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IDepthFrameReader;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("81C0C0AB-6E6C-45CB-8625-A5F4D38759A4")
    IDepthFrameReader : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SubscribeFrameArrived( 
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnsubscribeFrameArrived( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFrameArrivedEventData( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IDepthFrameArrivedEventArgs **eventData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AcquireLatestFrame( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IDepthFrame **depthFrame) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsPaused( 
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isPaused) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_IsPaused( 
            BOOLEAN isPaused) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_DepthFrameSource( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IDepthFrameSource **depthFrameSource) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IDepthFrameReaderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDepthFrameReader * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDepthFrameReader * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDepthFrameReader * This);
        
        HRESULT ( STDMETHODCALLTYPE *SubscribeFrameArrived )( 
            IDepthFrameReader * This,
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *UnsubscribeFrameArrived )( 
            IDepthFrameReader * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *GetFrameArrivedEventData )( 
            IDepthFrameReader * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IDepthFrameArrivedEventArgs **eventData);
        
        HRESULT ( STDMETHODCALLTYPE *AcquireLatestFrame )( 
            IDepthFrameReader * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IDepthFrame **depthFrame);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsPaused )( 
            IDepthFrameReader * This,
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isPaused);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_IsPaused )( 
            IDepthFrameReader * This,
            BOOLEAN isPaused);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_DepthFrameSource )( 
            IDepthFrameReader * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IDepthFrameSource **depthFrameSource);
        
        END_INTERFACE
    } IDepthFrameReaderVtbl;

    interface IDepthFrameReader
    {
        CONST_VTBL struct IDepthFrameReaderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDepthFrameReader_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDepthFrameReader_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDepthFrameReader_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDepthFrameReader_SubscribeFrameArrived(This,waitableHandle)	\
    ( (This)->lpVtbl -> SubscribeFrameArrived(This,waitableHandle) ) 

#define IDepthFrameReader_UnsubscribeFrameArrived(This,waitableHandle)	\
    ( (This)->lpVtbl -> UnsubscribeFrameArrived(This,waitableHandle) ) 

#define IDepthFrameReader_GetFrameArrivedEventData(This,waitableHandle,eventData)	\
    ( (This)->lpVtbl -> GetFrameArrivedEventData(This,waitableHandle,eventData) ) 

#define IDepthFrameReader_AcquireLatestFrame(This,depthFrame)	\
    ( (This)->lpVtbl -> AcquireLatestFrame(This,depthFrame) ) 

#define IDepthFrameReader_get_IsPaused(This,isPaused)	\
    ( (This)->lpVtbl -> get_IsPaused(This,isPaused) ) 

#define IDepthFrameReader_put_IsPaused(This,isPaused)	\
    ( (This)->lpVtbl -> put_IsPaused(This,isPaused) ) 

#define IDepthFrameReader_get_DepthFrameSource(This,depthFrameSource)	\
    ( (This)->lpVtbl -> get_DepthFrameSource(This,depthFrameSource) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDepthFrameReader_INTERFACE_DEFINED__ */


#ifndef __IDepthFrame_INTERFACE_DEFINED__
#define __IDepthFrame_INTERFACE_DEFINED__

/* interface IDepthFrame */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IDepthFrame;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D8600853-8835-44F9-84A7-E617CDD7DFDD")
    IDepthFrame : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CopyFrameDataToArray( 
            UINT capacity,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(capacity)  UINT16 *frameData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AccessUnderlyingBuffer( 
            /* [annotation][out] */ 
            _Out_  UINT *capacity,
            /* [annotation][size_is][size_is][out] */ 
            _Outptr_result_buffer_(*capacity)  UINT16 **buffer) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FrameDescription( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IFrameDescription **frameDescription) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_RelativeTime( 
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_DepthFrameSource( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IDepthFrameSource **depthFrameSource) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_DepthMinReliableDistance( 
            /* [annotation][out][retval] */ 
            _Out_  USHORT *depthMinReliableDistance) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_DepthMaxReliableDistance( 
            /* [annotation][out][retval] */ 
            _Out_  USHORT *depthMaxReliableDistance) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IDepthFrameVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDepthFrame * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDepthFrame * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDepthFrame * This);
        
        HRESULT ( STDMETHODCALLTYPE *CopyFrameDataToArray )( 
            IDepthFrame * This,
            UINT capacity,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(capacity)  UINT16 *frameData);
        
        HRESULT ( STDMETHODCALLTYPE *AccessUnderlyingBuffer )( 
            IDepthFrame * This,
            /* [annotation][out] */ 
            _Out_  UINT *capacity,
            /* [annotation][size_is][size_is][out] */ 
            _Outptr_result_buffer_(*capacity)  UINT16 **buffer);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FrameDescription )( 
            IDepthFrame * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IFrameDescription **frameDescription);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_RelativeTime )( 
            IDepthFrame * This,
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_DepthFrameSource )( 
            IDepthFrame * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IDepthFrameSource **depthFrameSource);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_DepthMinReliableDistance )( 
            IDepthFrame * This,
            /* [annotation][out][retval] */ 
            _Out_  USHORT *depthMinReliableDistance);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_DepthMaxReliableDistance )( 
            IDepthFrame * This,
            /* [annotation][out][retval] */ 
            _Out_  USHORT *depthMaxReliableDistance);
        
        END_INTERFACE
    } IDepthFrameVtbl;

    interface IDepthFrame
    {
        CONST_VTBL struct IDepthFrameVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDepthFrame_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDepthFrame_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDepthFrame_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDepthFrame_CopyFrameDataToArray(This,capacity,frameData)	\
    ( (This)->lpVtbl -> CopyFrameDataToArray(This,capacity,frameData) ) 

#define IDepthFrame_AccessUnderlyingBuffer(This,capacity,buffer)	\
    ( (This)->lpVtbl -> AccessUnderlyingBuffer(This,capacity,buffer) ) 

#define IDepthFrame_get_FrameDescription(This,frameDescription)	\
    ( (This)->lpVtbl -> get_FrameDescription(This,frameDescription) ) 

#define IDepthFrame_get_RelativeTime(This,relativeTime)	\
    ( (This)->lpVtbl -> get_RelativeTime(This,relativeTime) ) 

#define IDepthFrame_get_DepthFrameSource(This,depthFrameSource)	\
    ( (This)->lpVtbl -> get_DepthFrameSource(This,depthFrameSource) ) 

#define IDepthFrame_get_DepthMinReliableDistance(This,depthMinReliableDistance)	\
    ( (This)->lpVtbl -> get_DepthMinReliableDistance(This,depthMinReliableDistance) ) 

#define IDepthFrame_get_DepthMaxReliableDistance(This,depthMaxReliableDistance)	\
    ( (This)->lpVtbl -> get_DepthMaxReliableDistance(This,depthMaxReliableDistance) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDepthFrame_INTERFACE_DEFINED__ */


#ifndef __IBodyFrameReference_INTERFACE_DEFINED__
#define __IBodyFrameReference_INTERFACE_DEFINED__

/* interface IBodyFrameReference */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IBodyFrameReference;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C3A1733C-5F84-443B-9659-2F2BE250C97D")
    IBodyFrameReference : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE AcquireFrame( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyFrame **bodyFrame) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_RelativeTime( 
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IBodyFrameReferenceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBodyFrameReference * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBodyFrameReference * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBodyFrameReference * This);
        
        HRESULT ( STDMETHODCALLTYPE *AcquireFrame )( 
            IBodyFrameReference * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyFrame **bodyFrame);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_RelativeTime )( 
            IBodyFrameReference * This,
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime);
        
        END_INTERFACE
    } IBodyFrameReferenceVtbl;

    interface IBodyFrameReference
    {
        CONST_VTBL struct IBodyFrameReferenceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBodyFrameReference_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBodyFrameReference_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBodyFrameReference_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBodyFrameReference_AcquireFrame(This,bodyFrame)	\
    ( (This)->lpVtbl -> AcquireFrame(This,bodyFrame) ) 

#define IBodyFrameReference_get_RelativeTime(This,relativeTime)	\
    ( (This)->lpVtbl -> get_RelativeTime(This,relativeTime) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBodyFrameReference_INTERFACE_DEFINED__ */


#ifndef __IBodyFrameArrivedEventArgs_INTERFACE_DEFINED__
#define __IBodyFrameArrivedEventArgs_INTERFACE_DEFINED__

/* interface IBodyFrameArrivedEventArgs */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IBodyFrameArrivedEventArgs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BF5CCA0E-00C1-4D48-837F-AB921E6AEE01")
    IBodyFrameArrivedEventArgs : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FrameReference( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyFrameReference **bodyFrameReference) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IBodyFrameArrivedEventArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBodyFrameArrivedEventArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBodyFrameArrivedEventArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBodyFrameArrivedEventArgs * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FrameReference )( 
            IBodyFrameArrivedEventArgs * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyFrameReference **bodyFrameReference);
        
        END_INTERFACE
    } IBodyFrameArrivedEventArgsVtbl;

    interface IBodyFrameArrivedEventArgs
    {
        CONST_VTBL struct IBodyFrameArrivedEventArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBodyFrameArrivedEventArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBodyFrameArrivedEventArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBodyFrameArrivedEventArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBodyFrameArrivedEventArgs_get_FrameReference(This,bodyFrameReference)	\
    ( (This)->lpVtbl -> get_FrameReference(This,bodyFrameReference) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBodyFrameArrivedEventArgs_INTERFACE_DEFINED__ */


#ifndef __IBodyFrameSource_INTERFACE_DEFINED__
#define __IBodyFrameSource_INTERFACE_DEFINED__

/* interface IBodyFrameSource */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IBodyFrameSource;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BB94A78A-458C-4608-AC69-34FEAD1E3BAE")
    IBodyFrameSource : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SubscribeFrameCaptured( 
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnsubscribeFrameCaptured( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFrameCapturedEventData( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IFrameCapturedEventArgs **eventData) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsActive( 
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isActive) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_BodyCount( 
            /* [annotation][out][retval] */ 
            _Out_  INT32 *bodyCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OpenReader( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyFrameReader **reader) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_KinectSensor( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IKinectSensor **sensor) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OverrideHandTracking( 
            UINT64 trackingId) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OverrideAndReplaceHandTracking( 
            UINT64 oldTrackingId,
            UINT64 newTrackingId) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IBodyFrameSourceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBodyFrameSource * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBodyFrameSource * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBodyFrameSource * This);
        
        HRESULT ( STDMETHODCALLTYPE *SubscribeFrameCaptured )( 
            IBodyFrameSource * This,
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *UnsubscribeFrameCaptured )( 
            IBodyFrameSource * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *GetFrameCapturedEventData )( 
            IBodyFrameSource * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IFrameCapturedEventArgs **eventData);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsActive )( 
            IBodyFrameSource * This,
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isActive);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_BodyCount )( 
            IBodyFrameSource * This,
            /* [annotation][out][retval] */ 
            _Out_  INT32 *bodyCount);
        
        HRESULT ( STDMETHODCALLTYPE *OpenReader )( 
            IBodyFrameSource * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyFrameReader **reader);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_KinectSensor )( 
            IBodyFrameSource * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IKinectSensor **sensor);
        
        HRESULT ( STDMETHODCALLTYPE *OverrideHandTracking )( 
            IBodyFrameSource * This,
            UINT64 trackingId);
        
        HRESULT ( STDMETHODCALLTYPE *OverrideAndReplaceHandTracking )( 
            IBodyFrameSource * This,
            UINT64 oldTrackingId,
            UINT64 newTrackingId);
        
        END_INTERFACE
    } IBodyFrameSourceVtbl;

    interface IBodyFrameSource
    {
        CONST_VTBL struct IBodyFrameSourceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBodyFrameSource_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBodyFrameSource_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBodyFrameSource_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBodyFrameSource_SubscribeFrameCaptured(This,waitableHandle)	\
    ( (This)->lpVtbl -> SubscribeFrameCaptured(This,waitableHandle) ) 

#define IBodyFrameSource_UnsubscribeFrameCaptured(This,waitableHandle)	\
    ( (This)->lpVtbl -> UnsubscribeFrameCaptured(This,waitableHandle) ) 

#define IBodyFrameSource_GetFrameCapturedEventData(This,waitableHandle,eventData)	\
    ( (This)->lpVtbl -> GetFrameCapturedEventData(This,waitableHandle,eventData) ) 

#define IBodyFrameSource_get_IsActive(This,isActive)	\
    ( (This)->lpVtbl -> get_IsActive(This,isActive) ) 

#define IBodyFrameSource_get_BodyCount(This,bodyCount)	\
    ( (This)->lpVtbl -> get_BodyCount(This,bodyCount) ) 

#define IBodyFrameSource_OpenReader(This,reader)	\
    ( (This)->lpVtbl -> OpenReader(This,reader) ) 

#define IBodyFrameSource_get_KinectSensor(This,sensor)	\
    ( (This)->lpVtbl -> get_KinectSensor(This,sensor) ) 

#define IBodyFrameSource_OverrideHandTracking(This,trackingId)	\
    ( (This)->lpVtbl -> OverrideHandTracking(This,trackingId) ) 

#define IBodyFrameSource_OverrideAndReplaceHandTracking(This,oldTrackingId,newTrackingId)	\
    ( (This)->lpVtbl -> OverrideAndReplaceHandTracking(This,oldTrackingId,newTrackingId) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBodyFrameSource_INTERFACE_DEFINED__ */


#ifndef __IBodyFrameReader_INTERFACE_DEFINED__
#define __IBodyFrameReader_INTERFACE_DEFINED__

/* interface IBodyFrameReader */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IBodyFrameReader;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("45532DF5-A63C-418F-A39F-C567936BC051")
    IBodyFrameReader : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SubscribeFrameArrived( 
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnsubscribeFrameArrived( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFrameArrivedEventData( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IBodyFrameArrivedEventArgs **eventData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AcquireLatestFrame( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyFrame **bodyFrame) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsPaused( 
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isPaused) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_IsPaused( 
            BOOLEAN isPaused) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_BodyFrameSource( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyFrameSource **bodyFrameSource) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IBodyFrameReaderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBodyFrameReader * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBodyFrameReader * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBodyFrameReader * This);
        
        HRESULT ( STDMETHODCALLTYPE *SubscribeFrameArrived )( 
            IBodyFrameReader * This,
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *UnsubscribeFrameArrived )( 
            IBodyFrameReader * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *GetFrameArrivedEventData )( 
            IBodyFrameReader * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IBodyFrameArrivedEventArgs **eventData);
        
        HRESULT ( STDMETHODCALLTYPE *AcquireLatestFrame )( 
            IBodyFrameReader * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyFrame **bodyFrame);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsPaused )( 
            IBodyFrameReader * This,
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isPaused);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_IsPaused )( 
            IBodyFrameReader * This,
            BOOLEAN isPaused);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_BodyFrameSource )( 
            IBodyFrameReader * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyFrameSource **bodyFrameSource);
        
        END_INTERFACE
    } IBodyFrameReaderVtbl;

    interface IBodyFrameReader
    {
        CONST_VTBL struct IBodyFrameReaderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBodyFrameReader_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBodyFrameReader_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBodyFrameReader_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBodyFrameReader_SubscribeFrameArrived(This,waitableHandle)	\
    ( (This)->lpVtbl -> SubscribeFrameArrived(This,waitableHandle) ) 

#define IBodyFrameReader_UnsubscribeFrameArrived(This,waitableHandle)	\
    ( (This)->lpVtbl -> UnsubscribeFrameArrived(This,waitableHandle) ) 

#define IBodyFrameReader_GetFrameArrivedEventData(This,waitableHandle,eventData)	\
    ( (This)->lpVtbl -> GetFrameArrivedEventData(This,waitableHandle,eventData) ) 

#define IBodyFrameReader_AcquireLatestFrame(This,bodyFrame)	\
    ( (This)->lpVtbl -> AcquireLatestFrame(This,bodyFrame) ) 

#define IBodyFrameReader_get_IsPaused(This,isPaused)	\
    ( (This)->lpVtbl -> get_IsPaused(This,isPaused) ) 

#define IBodyFrameReader_put_IsPaused(This,isPaused)	\
    ( (This)->lpVtbl -> put_IsPaused(This,isPaused) ) 

#define IBodyFrameReader_get_BodyFrameSource(This,bodyFrameSource)	\
    ( (This)->lpVtbl -> get_BodyFrameSource(This,bodyFrameSource) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBodyFrameReader_INTERFACE_DEFINED__ */


#ifndef __IBodyFrame_INTERFACE_DEFINED__
#define __IBodyFrame_INTERFACE_DEFINED__

/* interface IBodyFrame */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IBodyFrame;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("52884F1F-94D7-4B57-BF87-9226950980D5")
    IBodyFrame : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetAndRefreshBodyData( 
            UINT capacity,
            /* [annotation][size_is][out][in] */ 
            _Inout_updates_all_(capacity)  IBody **bodies) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FloorClipPlane( 
            /* [annotation][out][retval] */ 
            _Out_  Vector4 *floorClipPlane) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_RelativeTime( 
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_BodyFrameSource( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyFrameSource **bodyFrameSource) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IBodyFrameVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBodyFrame * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBodyFrame * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBodyFrame * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetAndRefreshBodyData )( 
            IBodyFrame * This,
            UINT capacity,
            /* [annotation][size_is][out][in] */ 
            _Inout_updates_all_(capacity)  IBody **bodies);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FloorClipPlane )( 
            IBodyFrame * This,
            /* [annotation][out][retval] */ 
            _Out_  Vector4 *floorClipPlane);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_RelativeTime )( 
            IBodyFrame * This,
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_BodyFrameSource )( 
            IBodyFrame * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyFrameSource **bodyFrameSource);
        
        END_INTERFACE
    } IBodyFrameVtbl;

    interface IBodyFrame
    {
        CONST_VTBL struct IBodyFrameVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBodyFrame_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBodyFrame_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBodyFrame_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBodyFrame_GetAndRefreshBodyData(This,capacity,bodies)	\
    ( (This)->lpVtbl -> GetAndRefreshBodyData(This,capacity,bodies) ) 

#define IBodyFrame_get_FloorClipPlane(This,floorClipPlane)	\
    ( (This)->lpVtbl -> get_FloorClipPlane(This,floorClipPlane) ) 

#define IBodyFrame_get_RelativeTime(This,relativeTime)	\
    ( (This)->lpVtbl -> get_RelativeTime(This,relativeTime) ) 

#define IBodyFrame_get_BodyFrameSource(This,bodyFrameSource)	\
    ( (This)->lpVtbl -> get_BodyFrameSource(This,bodyFrameSource) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBodyFrame_INTERFACE_DEFINED__ */


#ifndef __IBody_INTERFACE_DEFINED__
#define __IBody_INTERFACE_DEFINED__

/* interface IBody */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IBody;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("46AEF731-98B0-4D18-827B-933758678F4A")
    IBody : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetJoints( 
            /* [annotation] */ 
            _Pre_equal_to_(JointType_Count)  UINT capacity,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(capacity)  Joint *joints) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetJointOrientations( 
            /* [annotation] */ 
            _Pre_equal_to_(JointType_Count)  UINT capacity,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(capacity)  JointOrientation *jointOrientations) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Engaged( 
            /* [annotation][out][retval] */ 
            _Out_  DetectionResult *detectionResult) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetExpressionDetectionResults( 
            /* [annotation] */ 
            _Pre_equal_to_(Expression_Count)  UINT capacity,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(capacity)  DetectionResult *detectionResults) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetActivityDetectionResults( 
            /* [annotation] */ 
            _Pre_equal_to_(Activity_Count)  UINT capacity,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(capacity)  DetectionResult *detectionResults) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetAppearanceDetectionResults( 
            /* [annotation] */ 
            _Pre_equal_to_(Appearance_Count)  UINT capacity,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(capacity)  DetectionResult *detectionResults) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_HandLeftState( 
            /* [annotation][out][retval] */ 
            _Out_  HandState *handState) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_HandLeftConfidence( 
            /* [annotation][out][retval] */ 
            _Out_  TrackingConfidence *confidence) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_HandRightState( 
            /* [annotation][out][retval] */ 
            _Out_  HandState *handState) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_HandRightConfidence( 
            /* [annotation][out][retval] */ 
            _Out_  TrackingConfidence *confidence) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ClippedEdges( 
            /* [annotation][out][retval] */ 
            _Out_  DWORD *clippedEdges) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_TrackingId( 
            /* [annotation][out][retval] */ 
            _Out_  UINT64 *trackingId) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsTracked( 
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *tracked) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsRestricted( 
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isRestricted) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Lean( 
            /* [annotation][out][retval] */ 
            _Out_  PointF *amount) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_LeanTrackingState( 
            /* [annotation][out][retval] */ 
            _Out_  TrackingState *trackingState) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IBodyVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBody * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBody * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBody * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetJoints )( 
            IBody * This,
            /* [annotation] */ 
            _Pre_equal_to_(JointType_Count)  UINT capacity,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(capacity)  Joint *joints);
        
        HRESULT ( STDMETHODCALLTYPE *GetJointOrientations )( 
            IBody * This,
            /* [annotation] */ 
            _Pre_equal_to_(JointType_Count)  UINT capacity,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(capacity)  JointOrientation *jointOrientations);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Engaged )( 
            IBody * This,
            /* [annotation][out][retval] */ 
            _Out_  DetectionResult *detectionResult);
        
        HRESULT ( STDMETHODCALLTYPE *GetExpressionDetectionResults )( 
            IBody * This,
            /* [annotation] */ 
            _Pre_equal_to_(Expression_Count)  UINT capacity,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(capacity)  DetectionResult *detectionResults);
        
        HRESULT ( STDMETHODCALLTYPE *GetActivityDetectionResults )( 
            IBody * This,
            /* [annotation] */ 
            _Pre_equal_to_(Activity_Count)  UINT capacity,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(capacity)  DetectionResult *detectionResults);
        
        HRESULT ( STDMETHODCALLTYPE *GetAppearanceDetectionResults )( 
            IBody * This,
            /* [annotation] */ 
            _Pre_equal_to_(Appearance_Count)  UINT capacity,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(capacity)  DetectionResult *detectionResults);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_HandLeftState )( 
            IBody * This,
            /* [annotation][out][retval] */ 
            _Out_  HandState *handState);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_HandLeftConfidence )( 
            IBody * This,
            /* [annotation][out][retval] */ 
            _Out_  TrackingConfidence *confidence);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_HandRightState )( 
            IBody * This,
            /* [annotation][out][retval] */ 
            _Out_  HandState *handState);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_HandRightConfidence )( 
            IBody * This,
            /* [annotation][out][retval] */ 
            _Out_  TrackingConfidence *confidence);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_ClippedEdges )( 
            IBody * This,
            /* [annotation][out][retval] */ 
            _Out_  DWORD *clippedEdges);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_TrackingId )( 
            IBody * This,
            /* [annotation][out][retval] */ 
            _Out_  UINT64 *trackingId);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsTracked )( 
            IBody * This,
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *tracked);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsRestricted )( 
            IBody * This,
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isRestricted);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Lean )( 
            IBody * This,
            /* [annotation][out][retval] */ 
            _Out_  PointF *amount);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_LeanTrackingState )( 
            IBody * This,
            /* [annotation][out][retval] */ 
            _Out_  TrackingState *trackingState);
        
        END_INTERFACE
    } IBodyVtbl;

    interface IBody
    {
        CONST_VTBL struct IBodyVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBody_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBody_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBody_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBody_GetJoints(This,capacity,joints)	\
    ( (This)->lpVtbl -> GetJoints(This,capacity,joints) ) 

#define IBody_GetJointOrientations(This,capacity,jointOrientations)	\
    ( (This)->lpVtbl -> GetJointOrientations(This,capacity,jointOrientations) ) 

#define IBody_get_Engaged(This,detectionResult)	\
    ( (This)->lpVtbl -> get_Engaged(This,detectionResult) ) 

#define IBody_GetExpressionDetectionResults(This,capacity,detectionResults)	\
    ( (This)->lpVtbl -> GetExpressionDetectionResults(This,capacity,detectionResults) ) 

#define IBody_GetActivityDetectionResults(This,capacity,detectionResults)	\
    ( (This)->lpVtbl -> GetActivityDetectionResults(This,capacity,detectionResults) ) 

#define IBody_GetAppearanceDetectionResults(This,capacity,detectionResults)	\
    ( (This)->lpVtbl -> GetAppearanceDetectionResults(This,capacity,detectionResults) ) 

#define IBody_get_HandLeftState(This,handState)	\
    ( (This)->lpVtbl -> get_HandLeftState(This,handState) ) 

#define IBody_get_HandLeftConfidence(This,confidence)	\
    ( (This)->lpVtbl -> get_HandLeftConfidence(This,confidence) ) 

#define IBody_get_HandRightState(This,handState)	\
    ( (This)->lpVtbl -> get_HandRightState(This,handState) ) 

#define IBody_get_HandRightConfidence(This,confidence)	\
    ( (This)->lpVtbl -> get_HandRightConfidence(This,confidence) ) 

#define IBody_get_ClippedEdges(This,clippedEdges)	\
    ( (This)->lpVtbl -> get_ClippedEdges(This,clippedEdges) ) 

#define IBody_get_TrackingId(This,trackingId)	\
    ( (This)->lpVtbl -> get_TrackingId(This,trackingId) ) 

#define IBody_get_IsTracked(This,tracked)	\
    ( (This)->lpVtbl -> get_IsTracked(This,tracked) ) 

#define IBody_get_IsRestricted(This,isRestricted)	\
    ( (This)->lpVtbl -> get_IsRestricted(This,isRestricted) ) 

#define IBody_get_Lean(This,amount)	\
    ( (This)->lpVtbl -> get_Lean(This,amount) ) 

#define IBody_get_LeanTrackingState(This,trackingState)	\
    ( (This)->lpVtbl -> get_LeanTrackingState(This,trackingState) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBody_INTERFACE_DEFINED__ */


#ifndef __IBodyIndexFrameReference_INTERFACE_DEFINED__
#define __IBodyIndexFrameReference_INTERFACE_DEFINED__

/* interface IBodyIndexFrameReference */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IBodyIndexFrameReference;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D0EA0519-F7E7-4B1E-B3D8-03B3C002795F")
    IBodyIndexFrameReference : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE AcquireFrame( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyIndexFrame **bodyIndexFrame) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_RelativeTime( 
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IBodyIndexFrameReferenceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBodyIndexFrameReference * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBodyIndexFrameReference * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBodyIndexFrameReference * This);
        
        HRESULT ( STDMETHODCALLTYPE *AcquireFrame )( 
            IBodyIndexFrameReference * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyIndexFrame **bodyIndexFrame);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_RelativeTime )( 
            IBodyIndexFrameReference * This,
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime);
        
        END_INTERFACE
    } IBodyIndexFrameReferenceVtbl;

    interface IBodyIndexFrameReference
    {
        CONST_VTBL struct IBodyIndexFrameReferenceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBodyIndexFrameReference_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBodyIndexFrameReference_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBodyIndexFrameReference_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBodyIndexFrameReference_AcquireFrame(This,bodyIndexFrame)	\
    ( (This)->lpVtbl -> AcquireFrame(This,bodyIndexFrame) ) 

#define IBodyIndexFrameReference_get_RelativeTime(This,relativeTime)	\
    ( (This)->lpVtbl -> get_RelativeTime(This,relativeTime) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBodyIndexFrameReference_INTERFACE_DEFINED__ */


#ifndef __IBodyIndexFrameArrivedEventArgs_INTERFACE_DEFINED__
#define __IBodyIndexFrameArrivedEventArgs_INTERFACE_DEFINED__

/* interface IBodyIndexFrameArrivedEventArgs */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IBodyIndexFrameArrivedEventArgs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("10B7E92E-B4F2-4A36-A459-06B2A4B249DF")
    IBodyIndexFrameArrivedEventArgs : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FrameReference( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyIndexFrameReference **bodyIndexFrameReference) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IBodyIndexFrameArrivedEventArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBodyIndexFrameArrivedEventArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBodyIndexFrameArrivedEventArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBodyIndexFrameArrivedEventArgs * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FrameReference )( 
            IBodyIndexFrameArrivedEventArgs * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyIndexFrameReference **bodyIndexFrameReference);
        
        END_INTERFACE
    } IBodyIndexFrameArrivedEventArgsVtbl;

    interface IBodyIndexFrameArrivedEventArgs
    {
        CONST_VTBL struct IBodyIndexFrameArrivedEventArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBodyIndexFrameArrivedEventArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBodyIndexFrameArrivedEventArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBodyIndexFrameArrivedEventArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBodyIndexFrameArrivedEventArgs_get_FrameReference(This,bodyIndexFrameReference)	\
    ( (This)->lpVtbl -> get_FrameReference(This,bodyIndexFrameReference) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBodyIndexFrameArrivedEventArgs_INTERFACE_DEFINED__ */


#ifndef __IBodyIndexFrameSource_INTERFACE_DEFINED__
#define __IBodyIndexFrameSource_INTERFACE_DEFINED__

/* interface IBodyIndexFrameSource */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IBodyIndexFrameSource;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("010F2A40-DC58-44A5-8E57-329A583FEC08")
    IBodyIndexFrameSource : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SubscribeFrameCaptured( 
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnsubscribeFrameCaptured( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFrameCapturedEventData( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IFrameCapturedEventArgs **eventData) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsActive( 
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isActive) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OpenReader( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyIndexFrameReader **reader) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FrameDescription( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IFrameDescription **frameDescription) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_KinectSensor( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IKinectSensor **sensor) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IBodyIndexFrameSourceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBodyIndexFrameSource * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBodyIndexFrameSource * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBodyIndexFrameSource * This);
        
        HRESULT ( STDMETHODCALLTYPE *SubscribeFrameCaptured )( 
            IBodyIndexFrameSource * This,
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *UnsubscribeFrameCaptured )( 
            IBodyIndexFrameSource * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *GetFrameCapturedEventData )( 
            IBodyIndexFrameSource * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IFrameCapturedEventArgs **eventData);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsActive )( 
            IBodyIndexFrameSource * This,
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isActive);
        
        HRESULT ( STDMETHODCALLTYPE *OpenReader )( 
            IBodyIndexFrameSource * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyIndexFrameReader **reader);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FrameDescription )( 
            IBodyIndexFrameSource * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IFrameDescription **frameDescription);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_KinectSensor )( 
            IBodyIndexFrameSource * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IKinectSensor **sensor);
        
        END_INTERFACE
    } IBodyIndexFrameSourceVtbl;

    interface IBodyIndexFrameSource
    {
        CONST_VTBL struct IBodyIndexFrameSourceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBodyIndexFrameSource_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBodyIndexFrameSource_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBodyIndexFrameSource_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBodyIndexFrameSource_SubscribeFrameCaptured(This,waitableHandle)	\
    ( (This)->lpVtbl -> SubscribeFrameCaptured(This,waitableHandle) ) 

#define IBodyIndexFrameSource_UnsubscribeFrameCaptured(This,waitableHandle)	\
    ( (This)->lpVtbl -> UnsubscribeFrameCaptured(This,waitableHandle) ) 

#define IBodyIndexFrameSource_GetFrameCapturedEventData(This,waitableHandle,eventData)	\
    ( (This)->lpVtbl -> GetFrameCapturedEventData(This,waitableHandle,eventData) ) 

#define IBodyIndexFrameSource_get_IsActive(This,isActive)	\
    ( (This)->lpVtbl -> get_IsActive(This,isActive) ) 

#define IBodyIndexFrameSource_OpenReader(This,reader)	\
    ( (This)->lpVtbl -> OpenReader(This,reader) ) 

#define IBodyIndexFrameSource_get_FrameDescription(This,frameDescription)	\
    ( (This)->lpVtbl -> get_FrameDescription(This,frameDescription) ) 

#define IBodyIndexFrameSource_get_KinectSensor(This,sensor)	\
    ( (This)->lpVtbl -> get_KinectSensor(This,sensor) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBodyIndexFrameSource_INTERFACE_DEFINED__ */


#ifndef __IBodyIndexFrameReader_INTERFACE_DEFINED__
#define __IBodyIndexFrameReader_INTERFACE_DEFINED__

/* interface IBodyIndexFrameReader */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IBodyIndexFrameReader;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E9724AA1-EBFA-48F8-9044-E0BE33383B8B")
    IBodyIndexFrameReader : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SubscribeFrameArrived( 
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnsubscribeFrameArrived( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFrameArrivedEventData( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IBodyIndexFrameArrivedEventArgs **eventData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AcquireLatestFrame( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyIndexFrame **bodyIndexFrame) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsPaused( 
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isPaused) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_IsPaused( 
            BOOLEAN isPaused) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_BodyIndexFrameSource( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyIndexFrameSource **bodyIndexFrameSource) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IBodyIndexFrameReaderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBodyIndexFrameReader * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBodyIndexFrameReader * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBodyIndexFrameReader * This);
        
        HRESULT ( STDMETHODCALLTYPE *SubscribeFrameArrived )( 
            IBodyIndexFrameReader * This,
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *UnsubscribeFrameArrived )( 
            IBodyIndexFrameReader * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *GetFrameArrivedEventData )( 
            IBodyIndexFrameReader * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IBodyIndexFrameArrivedEventArgs **eventData);
        
        HRESULT ( STDMETHODCALLTYPE *AcquireLatestFrame )( 
            IBodyIndexFrameReader * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyIndexFrame **bodyIndexFrame);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsPaused )( 
            IBodyIndexFrameReader * This,
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isPaused);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_IsPaused )( 
            IBodyIndexFrameReader * This,
            BOOLEAN isPaused);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_BodyIndexFrameSource )( 
            IBodyIndexFrameReader * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyIndexFrameSource **bodyIndexFrameSource);
        
        END_INTERFACE
    } IBodyIndexFrameReaderVtbl;

    interface IBodyIndexFrameReader
    {
        CONST_VTBL struct IBodyIndexFrameReaderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBodyIndexFrameReader_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBodyIndexFrameReader_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBodyIndexFrameReader_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBodyIndexFrameReader_SubscribeFrameArrived(This,waitableHandle)	\
    ( (This)->lpVtbl -> SubscribeFrameArrived(This,waitableHandle) ) 

#define IBodyIndexFrameReader_UnsubscribeFrameArrived(This,waitableHandle)	\
    ( (This)->lpVtbl -> UnsubscribeFrameArrived(This,waitableHandle) ) 

#define IBodyIndexFrameReader_GetFrameArrivedEventData(This,waitableHandle,eventData)	\
    ( (This)->lpVtbl -> GetFrameArrivedEventData(This,waitableHandle,eventData) ) 

#define IBodyIndexFrameReader_AcquireLatestFrame(This,bodyIndexFrame)	\
    ( (This)->lpVtbl -> AcquireLatestFrame(This,bodyIndexFrame) ) 

#define IBodyIndexFrameReader_get_IsPaused(This,isPaused)	\
    ( (This)->lpVtbl -> get_IsPaused(This,isPaused) ) 

#define IBodyIndexFrameReader_put_IsPaused(This,isPaused)	\
    ( (This)->lpVtbl -> put_IsPaused(This,isPaused) ) 

#define IBodyIndexFrameReader_get_BodyIndexFrameSource(This,bodyIndexFrameSource)	\
    ( (This)->lpVtbl -> get_BodyIndexFrameSource(This,bodyIndexFrameSource) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBodyIndexFrameReader_INTERFACE_DEFINED__ */


#ifndef __IBodyIndexFrame_INTERFACE_DEFINED__
#define __IBodyIndexFrame_INTERFACE_DEFINED__

/* interface IBodyIndexFrame */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IBodyIndexFrame;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2CEA0C07-F90C-44DF-A18C-F4D18075EA6B")
    IBodyIndexFrame : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CopyFrameDataToArray( 
            UINT capacity,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(capacity)  BYTE *frameData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AccessUnderlyingBuffer( 
            /* [annotation][out] */ 
            _Out_  UINT *capacity,
            /* [annotation][size_is][size_is][out] */ 
            _Outptr_result_buffer_(*capacity)  BYTE **buffer) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FrameDescription( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IFrameDescription **frameDescription) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_RelativeTime( 
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_BodyIndexFrameSource( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyIndexFrameSource **bodyIndexFrameSource) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IBodyIndexFrameVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBodyIndexFrame * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBodyIndexFrame * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBodyIndexFrame * This);
        
        HRESULT ( STDMETHODCALLTYPE *CopyFrameDataToArray )( 
            IBodyIndexFrame * This,
            UINT capacity,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(capacity)  BYTE *frameData);
        
        HRESULT ( STDMETHODCALLTYPE *AccessUnderlyingBuffer )( 
            IBodyIndexFrame * This,
            /* [annotation][out] */ 
            _Out_  UINT *capacity,
            /* [annotation][size_is][size_is][out] */ 
            _Outptr_result_buffer_(*capacity)  BYTE **buffer);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FrameDescription )( 
            IBodyIndexFrame * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IFrameDescription **frameDescription);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_RelativeTime )( 
            IBodyIndexFrame * This,
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_BodyIndexFrameSource )( 
            IBodyIndexFrame * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IBodyIndexFrameSource **bodyIndexFrameSource);
        
        END_INTERFACE
    } IBodyIndexFrameVtbl;

    interface IBodyIndexFrame
    {
        CONST_VTBL struct IBodyIndexFrameVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBodyIndexFrame_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBodyIndexFrame_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBodyIndexFrame_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBodyIndexFrame_CopyFrameDataToArray(This,capacity,frameData)	\
    ( (This)->lpVtbl -> CopyFrameDataToArray(This,capacity,frameData) ) 

#define IBodyIndexFrame_AccessUnderlyingBuffer(This,capacity,buffer)	\
    ( (This)->lpVtbl -> AccessUnderlyingBuffer(This,capacity,buffer) ) 

#define IBodyIndexFrame_get_FrameDescription(This,frameDescription)	\
    ( (This)->lpVtbl -> get_FrameDescription(This,frameDescription) ) 

#define IBodyIndexFrame_get_RelativeTime(This,relativeTime)	\
    ( (This)->lpVtbl -> get_RelativeTime(This,relativeTime) ) 

#define IBodyIndexFrame_get_BodyIndexFrameSource(This,bodyIndexFrameSource)	\
    ( (This)->lpVtbl -> get_BodyIndexFrameSource(This,bodyIndexFrameSource) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBodyIndexFrame_INTERFACE_DEFINED__ */


#ifndef __IInfraredFrameReference_INTERFACE_DEFINED__
#define __IInfraredFrameReference_INTERFACE_DEFINED__

/* interface IInfraredFrameReference */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IInfraredFrameReference;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("60183D5B-DED5-4D5C-AE59-64C7724FE5FE")
    IInfraredFrameReference : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE AcquireFrame( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IInfraredFrame **infraredFrame) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_RelativeTime( 
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IInfraredFrameReferenceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IInfraredFrameReference * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IInfraredFrameReference * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IInfraredFrameReference * This);
        
        HRESULT ( STDMETHODCALLTYPE *AcquireFrame )( 
            IInfraredFrameReference * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IInfraredFrame **infraredFrame);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_RelativeTime )( 
            IInfraredFrameReference * This,
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime);
        
        END_INTERFACE
    } IInfraredFrameReferenceVtbl;

    interface IInfraredFrameReference
    {
        CONST_VTBL struct IInfraredFrameReferenceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IInfraredFrameReference_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IInfraredFrameReference_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IInfraredFrameReference_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IInfraredFrameReference_AcquireFrame(This,infraredFrame)	\
    ( (This)->lpVtbl -> AcquireFrame(This,infraredFrame) ) 

#define IInfraredFrameReference_get_RelativeTime(This,relativeTime)	\
    ( (This)->lpVtbl -> get_RelativeTime(This,relativeTime) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IInfraredFrameReference_INTERFACE_DEFINED__ */


#ifndef __IInfraredFrameArrivedEventArgs_INTERFACE_DEFINED__
#define __IInfraredFrameArrivedEventArgs_INTERFACE_DEFINED__

/* interface IInfraredFrameArrivedEventArgs */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IInfraredFrameArrivedEventArgs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7E17F78E-D9D1-4448-90C2-4E50EC4ECEE9")
    IInfraredFrameArrivedEventArgs : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FrameReference( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IInfraredFrameReference **infraredFrameReference) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IInfraredFrameArrivedEventArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IInfraredFrameArrivedEventArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IInfraredFrameArrivedEventArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IInfraredFrameArrivedEventArgs * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FrameReference )( 
            IInfraredFrameArrivedEventArgs * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IInfraredFrameReference **infraredFrameReference);
        
        END_INTERFACE
    } IInfraredFrameArrivedEventArgsVtbl;

    interface IInfraredFrameArrivedEventArgs
    {
        CONST_VTBL struct IInfraredFrameArrivedEventArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IInfraredFrameArrivedEventArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IInfraredFrameArrivedEventArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IInfraredFrameArrivedEventArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IInfraredFrameArrivedEventArgs_get_FrameReference(This,infraredFrameReference)	\
    ( (This)->lpVtbl -> get_FrameReference(This,infraredFrameReference) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IInfraredFrameArrivedEventArgs_INTERFACE_DEFINED__ */


#ifndef __IInfraredFrameSource_INTERFACE_DEFINED__
#define __IInfraredFrameSource_INTERFACE_DEFINED__

/* interface IInfraredFrameSource */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IInfraredFrameSource;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4C299EC6-CA45-4AFF-87AD-DF5762C49BE7")
    IInfraredFrameSource : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SubscribeFrameCaptured( 
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnsubscribeFrameCaptured( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFrameCapturedEventData( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IFrameCapturedEventArgs **eventData) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsActive( 
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isActive) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OpenReader( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IInfraredFrameReader **reader) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FrameDescription( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IFrameDescription **frameDescription) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_KinectSensor( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IKinectSensor **sensor) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IInfraredFrameSourceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IInfraredFrameSource * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IInfraredFrameSource * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IInfraredFrameSource * This);
        
        HRESULT ( STDMETHODCALLTYPE *SubscribeFrameCaptured )( 
            IInfraredFrameSource * This,
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *UnsubscribeFrameCaptured )( 
            IInfraredFrameSource * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *GetFrameCapturedEventData )( 
            IInfraredFrameSource * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IFrameCapturedEventArgs **eventData);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsActive )( 
            IInfraredFrameSource * This,
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isActive);
        
        HRESULT ( STDMETHODCALLTYPE *OpenReader )( 
            IInfraredFrameSource * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IInfraredFrameReader **reader);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FrameDescription )( 
            IInfraredFrameSource * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IFrameDescription **frameDescription);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_KinectSensor )( 
            IInfraredFrameSource * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IKinectSensor **sensor);
        
        END_INTERFACE
    } IInfraredFrameSourceVtbl;

    interface IInfraredFrameSource
    {
        CONST_VTBL struct IInfraredFrameSourceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IInfraredFrameSource_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IInfraredFrameSource_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IInfraredFrameSource_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IInfraredFrameSource_SubscribeFrameCaptured(This,waitableHandle)	\
    ( (This)->lpVtbl -> SubscribeFrameCaptured(This,waitableHandle) ) 

#define IInfraredFrameSource_UnsubscribeFrameCaptured(This,waitableHandle)	\
    ( (This)->lpVtbl -> UnsubscribeFrameCaptured(This,waitableHandle) ) 

#define IInfraredFrameSource_GetFrameCapturedEventData(This,waitableHandle,eventData)	\
    ( (This)->lpVtbl -> GetFrameCapturedEventData(This,waitableHandle,eventData) ) 

#define IInfraredFrameSource_get_IsActive(This,isActive)	\
    ( (This)->lpVtbl -> get_IsActive(This,isActive) ) 

#define IInfraredFrameSource_OpenReader(This,reader)	\
    ( (This)->lpVtbl -> OpenReader(This,reader) ) 

#define IInfraredFrameSource_get_FrameDescription(This,frameDescription)	\
    ( (This)->lpVtbl -> get_FrameDescription(This,frameDescription) ) 

#define IInfraredFrameSource_get_KinectSensor(This,sensor)	\
    ( (This)->lpVtbl -> get_KinectSensor(This,sensor) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IInfraredFrameSource_INTERFACE_DEFINED__ */


#ifndef __IInfraredFrameReader_INTERFACE_DEFINED__
#define __IInfraredFrameReader_INTERFACE_DEFINED__

/* interface IInfraredFrameReader */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IInfraredFrameReader;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("059A049D-A0AC-481E-B342-483EE94A028B")
    IInfraredFrameReader : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SubscribeFrameArrived( 
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnsubscribeFrameArrived( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFrameArrivedEventData( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IInfraredFrameArrivedEventArgs **eventData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AcquireLatestFrame( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IInfraredFrame **infraredFrame) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsPaused( 
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isPaused) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_IsPaused( 
            BOOLEAN isPaused) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_InfraredFrameSource( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IInfraredFrameSource **infraredFrameSource) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IInfraredFrameReaderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IInfraredFrameReader * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IInfraredFrameReader * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IInfraredFrameReader * This);
        
        HRESULT ( STDMETHODCALLTYPE *SubscribeFrameArrived )( 
            IInfraredFrameReader * This,
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *UnsubscribeFrameArrived )( 
            IInfraredFrameReader * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *GetFrameArrivedEventData )( 
            IInfraredFrameReader * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IInfraredFrameArrivedEventArgs **eventData);
        
        HRESULT ( STDMETHODCALLTYPE *AcquireLatestFrame )( 
            IInfraredFrameReader * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IInfraredFrame **infraredFrame);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsPaused )( 
            IInfraredFrameReader * This,
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isPaused);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_IsPaused )( 
            IInfraredFrameReader * This,
            BOOLEAN isPaused);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_InfraredFrameSource )( 
            IInfraredFrameReader * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IInfraredFrameSource **infraredFrameSource);
        
        END_INTERFACE
    } IInfraredFrameReaderVtbl;

    interface IInfraredFrameReader
    {
        CONST_VTBL struct IInfraredFrameReaderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IInfraredFrameReader_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IInfraredFrameReader_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IInfraredFrameReader_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IInfraredFrameReader_SubscribeFrameArrived(This,waitableHandle)	\
    ( (This)->lpVtbl -> SubscribeFrameArrived(This,waitableHandle) ) 

#define IInfraredFrameReader_UnsubscribeFrameArrived(This,waitableHandle)	\
    ( (This)->lpVtbl -> UnsubscribeFrameArrived(This,waitableHandle) ) 

#define IInfraredFrameReader_GetFrameArrivedEventData(This,waitableHandle,eventData)	\
    ( (This)->lpVtbl -> GetFrameArrivedEventData(This,waitableHandle,eventData) ) 

#define IInfraredFrameReader_AcquireLatestFrame(This,infraredFrame)	\
    ( (This)->lpVtbl -> AcquireLatestFrame(This,infraredFrame) ) 

#define IInfraredFrameReader_get_IsPaused(This,isPaused)	\
    ( (This)->lpVtbl -> get_IsPaused(This,isPaused) ) 

#define IInfraredFrameReader_put_IsPaused(This,isPaused)	\
    ( (This)->lpVtbl -> put_IsPaused(This,isPaused) ) 

#define IInfraredFrameReader_get_InfraredFrameSource(This,infraredFrameSource)	\
    ( (This)->lpVtbl -> get_InfraredFrameSource(This,infraredFrameSource) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IInfraredFrameReader_INTERFACE_DEFINED__ */


#ifndef __IInfraredFrame_INTERFACE_DEFINED__
#define __IInfraredFrame_INTERFACE_DEFINED__

/* interface IInfraredFrame */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IInfraredFrame;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("EA83823C-7613-4F29-BD51-4A9678A52C7E")
    IInfraredFrame : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CopyFrameDataToArray( 
            UINT capacity,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(capacity)  UINT16 *frameData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AccessUnderlyingBuffer( 
            /* [annotation][out] */ 
            _Out_  UINT *capacity,
            /* [annotation][size_is][size_is][out] */ 
            _Outptr_result_buffer_(*capacity)  UINT16 **buffer) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FrameDescription( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IFrameDescription **frameDescription) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_RelativeTime( 
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_InfraredFrameSource( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IInfraredFrameSource **infraredFrameSource) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IInfraredFrameVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IInfraredFrame * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IInfraredFrame * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IInfraredFrame * This);
        
        HRESULT ( STDMETHODCALLTYPE *CopyFrameDataToArray )( 
            IInfraredFrame * This,
            UINT capacity,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(capacity)  UINT16 *frameData);
        
        HRESULT ( STDMETHODCALLTYPE *AccessUnderlyingBuffer )( 
            IInfraredFrame * This,
            /* [annotation][out] */ 
            _Out_  UINT *capacity,
            /* [annotation][size_is][size_is][out] */ 
            _Outptr_result_buffer_(*capacity)  UINT16 **buffer);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FrameDescription )( 
            IInfraredFrame * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IFrameDescription **frameDescription);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_RelativeTime )( 
            IInfraredFrame * This,
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_InfraredFrameSource )( 
            IInfraredFrame * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IInfraredFrameSource **infraredFrameSource);
        
        END_INTERFACE
    } IInfraredFrameVtbl;

    interface IInfraredFrame
    {
        CONST_VTBL struct IInfraredFrameVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IInfraredFrame_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IInfraredFrame_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IInfraredFrame_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IInfraredFrame_CopyFrameDataToArray(This,capacity,frameData)	\
    ( (This)->lpVtbl -> CopyFrameDataToArray(This,capacity,frameData) ) 

#define IInfraredFrame_AccessUnderlyingBuffer(This,capacity,buffer)	\
    ( (This)->lpVtbl -> AccessUnderlyingBuffer(This,capacity,buffer) ) 

#define IInfraredFrame_get_FrameDescription(This,frameDescription)	\
    ( (This)->lpVtbl -> get_FrameDescription(This,frameDescription) ) 

#define IInfraredFrame_get_RelativeTime(This,relativeTime)	\
    ( (This)->lpVtbl -> get_RelativeTime(This,relativeTime) ) 

#define IInfraredFrame_get_InfraredFrameSource(This,infraredFrameSource)	\
    ( (This)->lpVtbl -> get_InfraredFrameSource(This,infraredFrameSource) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IInfraredFrame_INTERFACE_DEFINED__ */


#ifndef __ILongExposureInfraredFrameReference_INTERFACE_DEFINED__
#define __ILongExposureInfraredFrameReference_INTERFACE_DEFINED__

/* interface ILongExposureInfraredFrameReference */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_ILongExposureInfraredFrameReference;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("10043A3E-0DAA-409C-9944-A6FC66C85AF7")
    ILongExposureInfraredFrameReference : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE AcquireFrame( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  ILongExposureInfraredFrame **longExposureInfraredFrame) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_RelativeTime( 
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ILongExposureInfraredFrameReferenceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILongExposureInfraredFrameReference * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILongExposureInfraredFrameReference * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILongExposureInfraredFrameReference * This);
        
        HRESULT ( STDMETHODCALLTYPE *AcquireFrame )( 
            ILongExposureInfraredFrameReference * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  ILongExposureInfraredFrame **longExposureInfraredFrame);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_RelativeTime )( 
            ILongExposureInfraredFrameReference * This,
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime);
        
        END_INTERFACE
    } ILongExposureInfraredFrameReferenceVtbl;

    interface ILongExposureInfraredFrameReference
    {
        CONST_VTBL struct ILongExposureInfraredFrameReferenceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILongExposureInfraredFrameReference_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ILongExposureInfraredFrameReference_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ILongExposureInfraredFrameReference_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ILongExposureInfraredFrameReference_AcquireFrame(This,longExposureInfraredFrame)	\
    ( (This)->lpVtbl -> AcquireFrame(This,longExposureInfraredFrame) ) 

#define ILongExposureInfraredFrameReference_get_RelativeTime(This,relativeTime)	\
    ( (This)->lpVtbl -> get_RelativeTime(This,relativeTime) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ILongExposureInfraredFrameReference_INTERFACE_DEFINED__ */


#ifndef __ILongExposureInfraredFrameArrivedEventArgs_INTERFACE_DEFINED__
#define __ILongExposureInfraredFrameArrivedEventArgs_INTERFACE_DEFINED__

/* interface ILongExposureInfraredFrameArrivedEventArgs */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_ILongExposureInfraredFrameArrivedEventArgs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D73D4B5E-E329-4F04-894C-0C97482690D4")
    ILongExposureInfraredFrameArrivedEventArgs : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FrameReference( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  ILongExposureInfraredFrameReference **longExposureInfraredFrameReference) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ILongExposureInfraredFrameArrivedEventArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILongExposureInfraredFrameArrivedEventArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILongExposureInfraredFrameArrivedEventArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILongExposureInfraredFrameArrivedEventArgs * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FrameReference )( 
            ILongExposureInfraredFrameArrivedEventArgs * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  ILongExposureInfraredFrameReference **longExposureInfraredFrameReference);
        
        END_INTERFACE
    } ILongExposureInfraredFrameArrivedEventArgsVtbl;

    interface ILongExposureInfraredFrameArrivedEventArgs
    {
        CONST_VTBL struct ILongExposureInfraredFrameArrivedEventArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILongExposureInfraredFrameArrivedEventArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ILongExposureInfraredFrameArrivedEventArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ILongExposureInfraredFrameArrivedEventArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ILongExposureInfraredFrameArrivedEventArgs_get_FrameReference(This,longExposureInfraredFrameReference)	\
    ( (This)->lpVtbl -> get_FrameReference(This,longExposureInfraredFrameReference) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ILongExposureInfraredFrameArrivedEventArgs_INTERFACE_DEFINED__ */


#ifndef __ILongExposureInfraredFrameSource_INTERFACE_DEFINED__
#define __ILongExposureInfraredFrameSource_INTERFACE_DEFINED__

/* interface ILongExposureInfraredFrameSource */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_ILongExposureInfraredFrameSource;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D7150EDA-EDA2-4673-B4F8-E3C76D1F402B")
    ILongExposureInfraredFrameSource : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SubscribeFrameCaptured( 
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnsubscribeFrameCaptured( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFrameCapturedEventData( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IFrameCapturedEventArgs **eventData) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsActive( 
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isActive) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OpenReader( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  ILongExposureInfraredFrameReader **reader) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FrameDescription( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IFrameDescription **frameDescription) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_KinectSensor( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IKinectSensor **sensor) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ILongExposureInfraredFrameSourceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILongExposureInfraredFrameSource * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILongExposureInfraredFrameSource * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILongExposureInfraredFrameSource * This);
        
        HRESULT ( STDMETHODCALLTYPE *SubscribeFrameCaptured )( 
            ILongExposureInfraredFrameSource * This,
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *UnsubscribeFrameCaptured )( 
            ILongExposureInfraredFrameSource * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *GetFrameCapturedEventData )( 
            ILongExposureInfraredFrameSource * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IFrameCapturedEventArgs **eventData);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsActive )( 
            ILongExposureInfraredFrameSource * This,
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isActive);
        
        HRESULT ( STDMETHODCALLTYPE *OpenReader )( 
            ILongExposureInfraredFrameSource * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  ILongExposureInfraredFrameReader **reader);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FrameDescription )( 
            ILongExposureInfraredFrameSource * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IFrameDescription **frameDescription);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_KinectSensor )( 
            ILongExposureInfraredFrameSource * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IKinectSensor **sensor);
        
        END_INTERFACE
    } ILongExposureInfraredFrameSourceVtbl;

    interface ILongExposureInfraredFrameSource
    {
        CONST_VTBL struct ILongExposureInfraredFrameSourceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILongExposureInfraredFrameSource_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ILongExposureInfraredFrameSource_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ILongExposureInfraredFrameSource_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ILongExposureInfraredFrameSource_SubscribeFrameCaptured(This,waitableHandle)	\
    ( (This)->lpVtbl -> SubscribeFrameCaptured(This,waitableHandle) ) 

#define ILongExposureInfraredFrameSource_UnsubscribeFrameCaptured(This,waitableHandle)	\
    ( (This)->lpVtbl -> UnsubscribeFrameCaptured(This,waitableHandle) ) 

#define ILongExposureInfraredFrameSource_GetFrameCapturedEventData(This,waitableHandle,eventData)	\
    ( (This)->lpVtbl -> GetFrameCapturedEventData(This,waitableHandle,eventData) ) 

#define ILongExposureInfraredFrameSource_get_IsActive(This,isActive)	\
    ( (This)->lpVtbl -> get_IsActive(This,isActive) ) 

#define ILongExposureInfraredFrameSource_OpenReader(This,reader)	\
    ( (This)->lpVtbl -> OpenReader(This,reader) ) 

#define ILongExposureInfraredFrameSource_get_FrameDescription(This,frameDescription)	\
    ( (This)->lpVtbl -> get_FrameDescription(This,frameDescription) ) 

#define ILongExposureInfraredFrameSource_get_KinectSensor(This,sensor)	\
    ( (This)->lpVtbl -> get_KinectSensor(This,sensor) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ILongExposureInfraredFrameSource_INTERFACE_DEFINED__ */


#ifndef __ILongExposureInfraredFrameReader_INTERFACE_DEFINED__
#define __ILongExposureInfraredFrameReader_INTERFACE_DEFINED__

/* interface ILongExposureInfraredFrameReader */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_ILongExposureInfraredFrameReader;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2AF23594-0115-417B-859F-A0E3FFB690D2")
    ILongExposureInfraredFrameReader : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SubscribeFrameArrived( 
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnsubscribeFrameArrived( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFrameArrivedEventData( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  ILongExposureInfraredFrameArrivedEventArgs **eventData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AcquireLatestFrame( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  ILongExposureInfraredFrame **longExposureInfraredFrame) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsPaused( 
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isPaused) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_IsPaused( 
            BOOLEAN isPaused) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_LongExposureInfraredFrameSource( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  ILongExposureInfraredFrameSource **LongExposureInfraredFrameSource) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ILongExposureInfraredFrameReaderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILongExposureInfraredFrameReader * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILongExposureInfraredFrameReader * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILongExposureInfraredFrameReader * This);
        
        HRESULT ( STDMETHODCALLTYPE *SubscribeFrameArrived )( 
            ILongExposureInfraredFrameReader * This,
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *UnsubscribeFrameArrived )( 
            ILongExposureInfraredFrameReader * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *GetFrameArrivedEventData )( 
            ILongExposureInfraredFrameReader * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  ILongExposureInfraredFrameArrivedEventArgs **eventData);
        
        HRESULT ( STDMETHODCALLTYPE *AcquireLatestFrame )( 
            ILongExposureInfraredFrameReader * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  ILongExposureInfraredFrame **longExposureInfraredFrame);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsPaused )( 
            ILongExposureInfraredFrameReader * This,
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isPaused);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_IsPaused )( 
            ILongExposureInfraredFrameReader * This,
            BOOLEAN isPaused);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_LongExposureInfraredFrameSource )( 
            ILongExposureInfraredFrameReader * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  ILongExposureInfraredFrameSource **LongExposureInfraredFrameSource);
        
        END_INTERFACE
    } ILongExposureInfraredFrameReaderVtbl;

    interface ILongExposureInfraredFrameReader
    {
        CONST_VTBL struct ILongExposureInfraredFrameReaderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILongExposureInfraredFrameReader_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ILongExposureInfraredFrameReader_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ILongExposureInfraredFrameReader_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ILongExposureInfraredFrameReader_SubscribeFrameArrived(This,waitableHandle)	\
    ( (This)->lpVtbl -> SubscribeFrameArrived(This,waitableHandle) ) 

#define ILongExposureInfraredFrameReader_UnsubscribeFrameArrived(This,waitableHandle)	\
    ( (This)->lpVtbl -> UnsubscribeFrameArrived(This,waitableHandle) ) 

#define ILongExposureInfraredFrameReader_GetFrameArrivedEventData(This,waitableHandle,eventData)	\
    ( (This)->lpVtbl -> GetFrameArrivedEventData(This,waitableHandle,eventData) ) 

#define ILongExposureInfraredFrameReader_AcquireLatestFrame(This,longExposureInfraredFrame)	\
    ( (This)->lpVtbl -> AcquireLatestFrame(This,longExposureInfraredFrame) ) 

#define ILongExposureInfraredFrameReader_get_IsPaused(This,isPaused)	\
    ( (This)->lpVtbl -> get_IsPaused(This,isPaused) ) 

#define ILongExposureInfraredFrameReader_put_IsPaused(This,isPaused)	\
    ( (This)->lpVtbl -> put_IsPaused(This,isPaused) ) 

#define ILongExposureInfraredFrameReader_get_LongExposureInfraredFrameSource(This,LongExposureInfraredFrameSource)	\
    ( (This)->lpVtbl -> get_LongExposureInfraredFrameSource(This,LongExposureInfraredFrameSource) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ILongExposureInfraredFrameReader_INTERFACE_DEFINED__ */


#ifndef __ILongExposureInfraredFrame_INTERFACE_DEFINED__
#define __ILongExposureInfraredFrame_INTERFACE_DEFINED__

/* interface ILongExposureInfraredFrame */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_ILongExposureInfraredFrame;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D1199394-9A42-4577-BE12-90A38B72282C")
    ILongExposureInfraredFrame : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CopyFrameDataToArray( 
            UINT capacity,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(capacity)  UINT16 *frameData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AccessUnderlyingBuffer( 
            /* [annotation][out] */ 
            _Out_  UINT *capacity,
            /* [annotation][size_is][size_is][out] */ 
            _Outptr_result_buffer_(*capacity)  UINT16 **buffer) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FrameDescription( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IFrameDescription **frameDescription) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_RelativeTime( 
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_LongExposureInfraredFrameSource( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  ILongExposureInfraredFrameSource **longExposureInfraredFrameSource) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ILongExposureInfraredFrameVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILongExposureInfraredFrame * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILongExposureInfraredFrame * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILongExposureInfraredFrame * This);
        
        HRESULT ( STDMETHODCALLTYPE *CopyFrameDataToArray )( 
            ILongExposureInfraredFrame * This,
            UINT capacity,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(capacity)  UINT16 *frameData);
        
        HRESULT ( STDMETHODCALLTYPE *AccessUnderlyingBuffer )( 
            ILongExposureInfraredFrame * This,
            /* [annotation][out] */ 
            _Out_  UINT *capacity,
            /* [annotation][size_is][size_is][out] */ 
            _Outptr_result_buffer_(*capacity)  UINT16 **buffer);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FrameDescription )( 
            ILongExposureInfraredFrame * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IFrameDescription **frameDescription);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_RelativeTime )( 
            ILongExposureInfraredFrame * This,
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_LongExposureInfraredFrameSource )( 
            ILongExposureInfraredFrame * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  ILongExposureInfraredFrameSource **longExposureInfraredFrameSource);
        
        END_INTERFACE
    } ILongExposureInfraredFrameVtbl;

    interface ILongExposureInfraredFrame
    {
        CONST_VTBL struct ILongExposureInfraredFrameVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILongExposureInfraredFrame_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ILongExposureInfraredFrame_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ILongExposureInfraredFrame_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ILongExposureInfraredFrame_CopyFrameDataToArray(This,capacity,frameData)	\
    ( (This)->lpVtbl -> CopyFrameDataToArray(This,capacity,frameData) ) 

#define ILongExposureInfraredFrame_AccessUnderlyingBuffer(This,capacity,buffer)	\
    ( (This)->lpVtbl -> AccessUnderlyingBuffer(This,capacity,buffer) ) 

#define ILongExposureInfraredFrame_get_FrameDescription(This,frameDescription)	\
    ( (This)->lpVtbl -> get_FrameDescription(This,frameDescription) ) 

#define ILongExposureInfraredFrame_get_RelativeTime(This,relativeTime)	\
    ( (This)->lpVtbl -> get_RelativeTime(This,relativeTime) ) 

#define ILongExposureInfraredFrame_get_LongExposureInfraredFrameSource(This,longExposureInfraredFrameSource)	\
    ( (This)->lpVtbl -> get_LongExposureInfraredFrameSource(This,longExposureInfraredFrameSource) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ILongExposureInfraredFrame_INTERFACE_DEFINED__ */


#ifndef __IAudioBeam_INTERFACE_DEFINED__
#define __IAudioBeam_INTERFACE_DEFINED__

/* interface IAudioBeam */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IAudioBeam;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F692D23A-14D0-432D-B802-DD381A45A121")
    IAudioBeam : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_AudioSource( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IAudioSource **audioSource) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_AudioBeamMode( 
            /* [annotation][out][retval] */ 
            _Out_  AudioBeamMode *audioBeamMode) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_AudioBeamMode( 
            AudioBeamMode audioBeamMode) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_BeamAngle( 
            /* [annotation][out][retval] */ 
            _Out_  float *beamAngle) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_BeamAngle( 
            float beamAngle) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_BeamAngleConfidence( 
            /* [annotation][out][retval] */ 
            _Out_  float *beamAngleConfidence) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OpenInputStream( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IStream **stream) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_RelativeTime( 
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IAudioBeamVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAudioBeam * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAudioBeam * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAudioBeam * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_AudioSource )( 
            IAudioBeam * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IAudioSource **audioSource);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_AudioBeamMode )( 
            IAudioBeam * This,
            /* [annotation][out][retval] */ 
            _Out_  AudioBeamMode *audioBeamMode);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_AudioBeamMode )( 
            IAudioBeam * This,
            AudioBeamMode audioBeamMode);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_BeamAngle )( 
            IAudioBeam * This,
            /* [annotation][out][retval] */ 
            _Out_  float *beamAngle);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_BeamAngle )( 
            IAudioBeam * This,
            float beamAngle);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_BeamAngleConfidence )( 
            IAudioBeam * This,
            /* [annotation][out][retval] */ 
            _Out_  float *beamAngleConfidence);
        
        HRESULT ( STDMETHODCALLTYPE *OpenInputStream )( 
            IAudioBeam * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IStream **stream);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_RelativeTime )( 
            IAudioBeam * This,
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime);
        
        END_INTERFACE
    } IAudioBeamVtbl;

    interface IAudioBeam
    {
        CONST_VTBL struct IAudioBeamVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAudioBeam_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAudioBeam_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAudioBeam_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAudioBeam_get_AudioSource(This,audioSource)	\
    ( (This)->lpVtbl -> get_AudioSource(This,audioSource) ) 

#define IAudioBeam_get_AudioBeamMode(This,audioBeamMode)	\
    ( (This)->lpVtbl -> get_AudioBeamMode(This,audioBeamMode) ) 

#define IAudioBeam_put_AudioBeamMode(This,audioBeamMode)	\
    ( (This)->lpVtbl -> put_AudioBeamMode(This,audioBeamMode) ) 

#define IAudioBeam_get_BeamAngle(This,beamAngle)	\
    ( (This)->lpVtbl -> get_BeamAngle(This,beamAngle) ) 

#define IAudioBeam_put_BeamAngle(This,beamAngle)	\
    ( (This)->lpVtbl -> put_BeamAngle(This,beamAngle) ) 

#define IAudioBeam_get_BeamAngleConfidence(This,beamAngleConfidence)	\
    ( (This)->lpVtbl -> get_BeamAngleConfidence(This,beamAngleConfidence) ) 

#define IAudioBeam_OpenInputStream(This,stream)	\
    ( (This)->lpVtbl -> OpenInputStream(This,stream) ) 

#define IAudioBeam_get_RelativeTime(This,relativeTime)	\
    ( (This)->lpVtbl -> get_RelativeTime(This,relativeTime) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAudioBeam_INTERFACE_DEFINED__ */


#ifndef __IAudioBeamList_INTERFACE_DEFINED__
#define __IAudioBeamList_INTERFACE_DEFINED__

/* interface IAudioBeamList */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IAudioBeamList;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3C792C7B-7D95-4C56-9DC7-EF63955781EA")
    IAudioBeamList : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_BeamCount( 
            /* [annotation][out][retval] */ 
            _Out_  UINT *count) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OpenAudioBeam( 
            UINT32 index,
            /* [annotation][out] */ 
            _COM_Outptr_  IAudioBeam **audioBeam) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IAudioBeamListVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAudioBeamList * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAudioBeamList * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAudioBeamList * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_BeamCount )( 
            IAudioBeamList * This,
            /* [annotation][out][retval] */ 
            _Out_  UINT *count);
        
        HRESULT ( STDMETHODCALLTYPE *OpenAudioBeam )( 
            IAudioBeamList * This,
            UINT32 index,
            /* [annotation][out] */ 
            _COM_Outptr_  IAudioBeam **audioBeam);
        
        END_INTERFACE
    } IAudioBeamListVtbl;

    interface IAudioBeamList
    {
        CONST_VTBL struct IAudioBeamListVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAudioBeamList_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAudioBeamList_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAudioBeamList_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAudioBeamList_get_BeamCount(This,count)	\
    ( (This)->lpVtbl -> get_BeamCount(This,count) ) 

#define IAudioBeamList_OpenAudioBeam(This,index,audioBeam)	\
    ( (This)->lpVtbl -> OpenAudioBeam(This,index,audioBeam) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAudioBeamList_INTERFACE_DEFINED__ */


#ifndef __IAudioBeamFrameList_INTERFACE_DEFINED__
#define __IAudioBeamFrameList_INTERFACE_DEFINED__

/* interface IAudioBeamFrameList */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IAudioBeamFrameList;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5393C8B9-C044-49CB-BDD6-23DFFFD7427E")
    IAudioBeamFrameList : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_BeamCount( 
            /* [annotation][out][retval] */ 
            _Out_  UINT *count) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OpenAudioBeamFrame( 
            UINT32 index,
            /* [annotation][out] */ 
            _COM_Outptr_  IAudioBeamFrame **audioBeamFrame) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IAudioBeamFrameListVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAudioBeamFrameList * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAudioBeamFrameList * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAudioBeamFrameList * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_BeamCount )( 
            IAudioBeamFrameList * This,
            /* [annotation][out][retval] */ 
            _Out_  UINT *count);
        
        HRESULT ( STDMETHODCALLTYPE *OpenAudioBeamFrame )( 
            IAudioBeamFrameList * This,
            UINT32 index,
            /* [annotation][out] */ 
            _COM_Outptr_  IAudioBeamFrame **audioBeamFrame);
        
        END_INTERFACE
    } IAudioBeamFrameListVtbl;

    interface IAudioBeamFrameList
    {
        CONST_VTBL struct IAudioBeamFrameListVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAudioBeamFrameList_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAudioBeamFrameList_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAudioBeamFrameList_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAudioBeamFrameList_get_BeamCount(This,count)	\
    ( (This)->lpVtbl -> get_BeamCount(This,count) ) 

#define IAudioBeamFrameList_OpenAudioBeamFrame(This,index,audioBeamFrame)	\
    ( (This)->lpVtbl -> OpenAudioBeamFrame(This,index,audioBeamFrame) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAudioBeamFrameList_INTERFACE_DEFINED__ */


#ifndef __IAudioBeamFrame_INTERFACE_DEFINED__
#define __IAudioBeamFrame_INTERFACE_DEFINED__

/* interface IAudioBeamFrame */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IAudioBeamFrame;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("07AADCC8-EC4A-42F8-90A9-C72ECF0A1D06")
    IAudioBeamFrame : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_AudioSource( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IAudioSource **audioSource) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Duration( 
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *duration) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_AudioBeam( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IAudioBeam **audioBeam) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_SubFrameCount( 
            /* [annotation][out][retval] */ 
            _Out_  UINT32 *pSubFrameCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSubFrame( 
            UINT32 subFrameIndex,
            /* [annotation][out] */ 
            _COM_Outptr_  IAudioBeamSubFrame **audioBeamSubFrame) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_RelativeTimeStart( 
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IAudioBeamFrameVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAudioBeamFrame * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAudioBeamFrame * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAudioBeamFrame * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_AudioSource )( 
            IAudioBeamFrame * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IAudioSource **audioSource);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Duration )( 
            IAudioBeamFrame * This,
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *duration);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_AudioBeam )( 
            IAudioBeamFrame * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IAudioBeam **audioBeam);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_SubFrameCount )( 
            IAudioBeamFrame * This,
            /* [annotation][out][retval] */ 
            _Out_  UINT32 *pSubFrameCount);
        
        HRESULT ( STDMETHODCALLTYPE *GetSubFrame )( 
            IAudioBeamFrame * This,
            UINT32 subFrameIndex,
            /* [annotation][out] */ 
            _COM_Outptr_  IAudioBeamSubFrame **audioBeamSubFrame);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_RelativeTimeStart )( 
            IAudioBeamFrame * This,
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime);
        
        END_INTERFACE
    } IAudioBeamFrameVtbl;

    interface IAudioBeamFrame
    {
        CONST_VTBL struct IAudioBeamFrameVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAudioBeamFrame_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAudioBeamFrame_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAudioBeamFrame_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAudioBeamFrame_get_AudioSource(This,audioSource)	\
    ( (This)->lpVtbl -> get_AudioSource(This,audioSource) ) 

#define IAudioBeamFrame_get_Duration(This,duration)	\
    ( (This)->lpVtbl -> get_Duration(This,duration) ) 

#define IAudioBeamFrame_get_AudioBeam(This,audioBeam)	\
    ( (This)->lpVtbl -> get_AudioBeam(This,audioBeam) ) 

#define IAudioBeamFrame_get_SubFrameCount(This,pSubFrameCount)	\
    ( (This)->lpVtbl -> get_SubFrameCount(This,pSubFrameCount) ) 

#define IAudioBeamFrame_GetSubFrame(This,subFrameIndex,audioBeamSubFrame)	\
    ( (This)->lpVtbl -> GetSubFrame(This,subFrameIndex,audioBeamSubFrame) ) 

#define IAudioBeamFrame_get_RelativeTimeStart(This,relativeTime)	\
    ( (This)->lpVtbl -> get_RelativeTimeStart(This,relativeTime) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAudioBeamFrame_INTERFACE_DEFINED__ */


#ifndef __IAudioBeamSubFrame_INTERFACE_DEFINED__
#define __IAudioBeamSubFrame_INTERFACE_DEFINED__

/* interface IAudioBeamSubFrame */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IAudioBeamSubFrame;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0967DB97-80D1-4BC5-BD2B-4685098D9795")
    IAudioBeamSubFrame : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FrameLengthInBytes( 
            /* [annotation][out][retval] */ 
            _Out_  UINT *length) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Duration( 
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *duration) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_BeamAngle( 
            /* [annotation][out][retval] */ 
            _Out_  float *beamAngle) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_BeamAngleConfidence( 
            /* [annotation][out][retval] */ 
            _Out_  float *beamAngleConfidence) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_AudioBeamMode( 
            /* [annotation][out][retval] */ 
            _Out_  AudioBeamMode *audioBeamMode) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_AudioBodyCorrelationCount( 
            /* [annotation][out][retval] */ 
            _Out_  UINT32 *pCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetAudioBodyCorrelation( 
            /* [annotation][in][in] */ 
            _In_  UINT index,
            /* [annotation][out][out] */ 
            _COM_Outptr_  IAudioBodyCorrelation **ppAudioBodyCorrelation) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CopyFrameDataToArray( 
            /* [annotation] */ 
            _Pre_equal_to_(1024)  UINT capacity,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(capacity)  BYTE *frameData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AccessUnderlyingBuffer( 
            /* [annotation][out] */ 
            _Out_  UINT *capacity,
            /* [annotation][size_is][size_is][out] */ 
            _Outptr_result_buffer_(*capacity)  BYTE **buffer) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_RelativeTime( 
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IAudioBeamSubFrameVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAudioBeamSubFrame * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAudioBeamSubFrame * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAudioBeamSubFrame * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FrameLengthInBytes )( 
            IAudioBeamSubFrame * This,
            /* [annotation][out][retval] */ 
            _Out_  UINT *length);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Duration )( 
            IAudioBeamSubFrame * This,
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *duration);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_BeamAngle )( 
            IAudioBeamSubFrame * This,
            /* [annotation][out][retval] */ 
            _Out_  float *beamAngle);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_BeamAngleConfidence )( 
            IAudioBeamSubFrame * This,
            /* [annotation][out][retval] */ 
            _Out_  float *beamAngleConfidence);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_AudioBeamMode )( 
            IAudioBeamSubFrame * This,
            /* [annotation][out][retval] */ 
            _Out_  AudioBeamMode *audioBeamMode);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_AudioBodyCorrelationCount )( 
            IAudioBeamSubFrame * This,
            /* [annotation][out][retval] */ 
            _Out_  UINT32 *pCount);
        
        HRESULT ( STDMETHODCALLTYPE *GetAudioBodyCorrelation )( 
            IAudioBeamSubFrame * This,
            /* [annotation][in][in] */ 
            _In_  UINT index,
            /* [annotation][out][out] */ 
            _COM_Outptr_  IAudioBodyCorrelation **ppAudioBodyCorrelation);
        
        HRESULT ( STDMETHODCALLTYPE *CopyFrameDataToArray )( 
            IAudioBeamSubFrame * This,
            /* [annotation] */ 
            _Pre_equal_to_(1024)  UINT capacity,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(capacity)  BYTE *frameData);
        
        HRESULT ( STDMETHODCALLTYPE *AccessUnderlyingBuffer )( 
            IAudioBeamSubFrame * This,
            /* [annotation][out] */ 
            _Out_  UINT *capacity,
            /* [annotation][size_is][size_is][out] */ 
            _Outptr_result_buffer_(*capacity)  BYTE **buffer);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_RelativeTime )( 
            IAudioBeamSubFrame * This,
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime);
        
        END_INTERFACE
    } IAudioBeamSubFrameVtbl;

    interface IAudioBeamSubFrame
    {
        CONST_VTBL struct IAudioBeamSubFrameVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAudioBeamSubFrame_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAudioBeamSubFrame_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAudioBeamSubFrame_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAudioBeamSubFrame_get_FrameLengthInBytes(This,length)	\
    ( (This)->lpVtbl -> get_FrameLengthInBytes(This,length) ) 

#define IAudioBeamSubFrame_get_Duration(This,duration)	\
    ( (This)->lpVtbl -> get_Duration(This,duration) ) 

#define IAudioBeamSubFrame_get_BeamAngle(This,beamAngle)	\
    ( (This)->lpVtbl -> get_BeamAngle(This,beamAngle) ) 

#define IAudioBeamSubFrame_get_BeamAngleConfidence(This,beamAngleConfidence)	\
    ( (This)->lpVtbl -> get_BeamAngleConfidence(This,beamAngleConfidence) ) 

#define IAudioBeamSubFrame_get_AudioBeamMode(This,audioBeamMode)	\
    ( (This)->lpVtbl -> get_AudioBeamMode(This,audioBeamMode) ) 

#define IAudioBeamSubFrame_get_AudioBodyCorrelationCount(This,pCount)	\
    ( (This)->lpVtbl -> get_AudioBodyCorrelationCount(This,pCount) ) 

#define IAudioBeamSubFrame_GetAudioBodyCorrelation(This,index,ppAudioBodyCorrelation)	\
    ( (This)->lpVtbl -> GetAudioBodyCorrelation(This,index,ppAudioBodyCorrelation) ) 

#define IAudioBeamSubFrame_CopyFrameDataToArray(This,capacity,frameData)	\
    ( (This)->lpVtbl -> CopyFrameDataToArray(This,capacity,frameData) ) 

#define IAudioBeamSubFrame_AccessUnderlyingBuffer(This,capacity,buffer)	\
    ( (This)->lpVtbl -> AccessUnderlyingBuffer(This,capacity,buffer) ) 

#define IAudioBeamSubFrame_get_RelativeTime(This,relativeTime)	\
    ( (This)->lpVtbl -> get_RelativeTime(This,relativeTime) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAudioBeamSubFrame_INTERFACE_DEFINED__ */


#ifndef __IAudioBeamFrameReference_INTERFACE_DEFINED__
#define __IAudioBeamFrameReference_INTERFACE_DEFINED__

/* interface IAudioBeamFrameReference */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IAudioBeamFrameReference;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1BD29D0E-6304-4AFB-9C85-77CFE3DC4FCE")
    IAudioBeamFrameReference : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE AcquireBeamFrames( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IAudioBeamFrameList **audioBeamFrameList) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_RelativeTime( 
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IAudioBeamFrameReferenceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAudioBeamFrameReference * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAudioBeamFrameReference * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAudioBeamFrameReference * This);
        
        HRESULT ( STDMETHODCALLTYPE *AcquireBeamFrames )( 
            IAudioBeamFrameReference * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IAudioBeamFrameList **audioBeamFrameList);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_RelativeTime )( 
            IAudioBeamFrameReference * This,
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *relativeTime);
        
        END_INTERFACE
    } IAudioBeamFrameReferenceVtbl;

    interface IAudioBeamFrameReference
    {
        CONST_VTBL struct IAudioBeamFrameReferenceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAudioBeamFrameReference_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAudioBeamFrameReference_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAudioBeamFrameReference_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAudioBeamFrameReference_AcquireBeamFrames(This,audioBeamFrameList)	\
    ( (This)->lpVtbl -> AcquireBeamFrames(This,audioBeamFrameList) ) 

#define IAudioBeamFrameReference_get_RelativeTime(This,relativeTime)	\
    ( (This)->lpVtbl -> get_RelativeTime(This,relativeTime) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAudioBeamFrameReference_INTERFACE_DEFINED__ */


#ifndef __IAudioBodyCorrelation_INTERFACE_DEFINED__
#define __IAudioBodyCorrelation_INTERFACE_DEFINED__

/* interface IAudioBodyCorrelation */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IAudioBodyCorrelation;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C5BA2355-07DB-47C3-ABC4-68D24B91DE61")
    IAudioBodyCorrelation : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_BodyTrackingId( 
            /* [annotation][out][retval] */ 
            _Out_  UINT64 *trackingId) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IAudioBodyCorrelationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAudioBodyCorrelation * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAudioBodyCorrelation * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAudioBodyCorrelation * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_BodyTrackingId )( 
            IAudioBodyCorrelation * This,
            /* [annotation][out][retval] */ 
            _Out_  UINT64 *trackingId);
        
        END_INTERFACE
    } IAudioBodyCorrelationVtbl;

    interface IAudioBodyCorrelation
    {
        CONST_VTBL struct IAudioBodyCorrelationVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAudioBodyCorrelation_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAudioBodyCorrelation_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAudioBodyCorrelation_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAudioBodyCorrelation_get_BodyTrackingId(This,trackingId)	\
    ( (This)->lpVtbl -> get_BodyTrackingId(This,trackingId) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAudioBodyCorrelation_INTERFACE_DEFINED__ */


#ifndef __IAudioBeamFrameArrivedEventArgs_INTERFACE_DEFINED__
#define __IAudioBeamFrameArrivedEventArgs_INTERFACE_DEFINED__

/* interface IAudioBeamFrameArrivedEventArgs */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IAudioBeamFrameArrivedEventArgs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E0DBE62D-2045-4571-8D1D-ECF3981E3C3D")
    IAudioBeamFrameArrivedEventArgs : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FrameReference( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IAudioBeamFrameReference **audioBeamFrameReference) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IAudioBeamFrameArrivedEventArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAudioBeamFrameArrivedEventArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAudioBeamFrameArrivedEventArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAudioBeamFrameArrivedEventArgs * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FrameReference )( 
            IAudioBeamFrameArrivedEventArgs * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IAudioBeamFrameReference **audioBeamFrameReference);
        
        END_INTERFACE
    } IAudioBeamFrameArrivedEventArgsVtbl;

    interface IAudioBeamFrameArrivedEventArgs
    {
        CONST_VTBL struct IAudioBeamFrameArrivedEventArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAudioBeamFrameArrivedEventArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAudioBeamFrameArrivedEventArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAudioBeamFrameArrivedEventArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAudioBeamFrameArrivedEventArgs_get_FrameReference(This,audioBeamFrameReference)	\
    ( (This)->lpVtbl -> get_FrameReference(This,audioBeamFrameReference) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAudioBeamFrameArrivedEventArgs_INTERFACE_DEFINED__ */


#ifndef __IAudioBeamFrameReader_INTERFACE_DEFINED__
#define __IAudioBeamFrameReader_INTERFACE_DEFINED__

/* interface IAudioBeamFrameReader */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IAudioBeamFrameReader;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B5733DE9-6ECF-46B2-8B23-A16D71F1A75C")
    IAudioBeamFrameReader : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SubscribeFrameArrived( 
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnsubscribeFrameArrived( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFrameArrivedEventData( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IAudioBeamFrameArrivedEventArgs **eventData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AcquireLatestBeamFrames( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IAudioBeamFrameList **audioBeamFrameList) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsPaused( 
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isPaused) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_IsPaused( 
            BOOLEAN isPaused) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_AudioSource( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IAudioSource **audioSource) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IAudioBeamFrameReaderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAudioBeamFrameReader * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAudioBeamFrameReader * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAudioBeamFrameReader * This);
        
        HRESULT ( STDMETHODCALLTYPE *SubscribeFrameArrived )( 
            IAudioBeamFrameReader * This,
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *UnsubscribeFrameArrived )( 
            IAudioBeamFrameReader * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *GetFrameArrivedEventData )( 
            IAudioBeamFrameReader * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IAudioBeamFrameArrivedEventArgs **eventData);
        
        HRESULT ( STDMETHODCALLTYPE *AcquireLatestBeamFrames )( 
            IAudioBeamFrameReader * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IAudioBeamFrameList **audioBeamFrameList);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsPaused )( 
            IAudioBeamFrameReader * This,
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isPaused);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_IsPaused )( 
            IAudioBeamFrameReader * This,
            BOOLEAN isPaused);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_AudioSource )( 
            IAudioBeamFrameReader * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IAudioSource **audioSource);
        
        END_INTERFACE
    } IAudioBeamFrameReaderVtbl;

    interface IAudioBeamFrameReader
    {
        CONST_VTBL struct IAudioBeamFrameReaderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAudioBeamFrameReader_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAudioBeamFrameReader_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAudioBeamFrameReader_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAudioBeamFrameReader_SubscribeFrameArrived(This,waitableHandle)	\
    ( (This)->lpVtbl -> SubscribeFrameArrived(This,waitableHandle) ) 

#define IAudioBeamFrameReader_UnsubscribeFrameArrived(This,waitableHandle)	\
    ( (This)->lpVtbl -> UnsubscribeFrameArrived(This,waitableHandle) ) 

#define IAudioBeamFrameReader_GetFrameArrivedEventData(This,waitableHandle,eventData)	\
    ( (This)->lpVtbl -> GetFrameArrivedEventData(This,waitableHandle,eventData) ) 

#define IAudioBeamFrameReader_AcquireLatestBeamFrames(This,audioBeamFrameList)	\
    ( (This)->lpVtbl -> AcquireLatestBeamFrames(This,audioBeamFrameList) ) 

#define IAudioBeamFrameReader_get_IsPaused(This,isPaused)	\
    ( (This)->lpVtbl -> get_IsPaused(This,isPaused) ) 

#define IAudioBeamFrameReader_put_IsPaused(This,isPaused)	\
    ( (This)->lpVtbl -> put_IsPaused(This,isPaused) ) 

#define IAudioBeamFrameReader_get_AudioSource(This,audioSource)	\
    ( (This)->lpVtbl -> get_AudioSource(This,audioSource) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAudioBeamFrameReader_INTERFACE_DEFINED__ */


#ifndef __IAudioSource_INTERFACE_DEFINED__
#define __IAudioSource_INTERFACE_DEFINED__

/* interface IAudioSource */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IAudioSource;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("52D1D743-AED1-4E61-8AF8-19EF287A662C")
    IAudioSource : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SubscribeFrameCaptured( 
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnsubscribeFrameCaptured( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFrameCapturedEventData( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IFrameCapturedEventArgs **eventData) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_KinectSensor( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IKinectSensor **sensor) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsActive( 
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isActive) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_SubFrameLengthInBytes( 
            /* [annotation][out][retval] */ 
            _Out_  UINT *length) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_SubFrameDuration( 
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *duration) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_MaxSubFrameCount( 
            /* [annotation][out][retval] */ 
            _Out_  UINT *count) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OpenReader( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IAudioBeamFrameReader **reader) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_AudioBeams( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IAudioBeamList **audioBeamList) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_AudioCalibrationState( 
            /* [annotation][out][retval] */ 
            _Out_  KinectAudioCalibrationState *audioCalibrationState) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IAudioSourceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAudioSource * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAudioSource * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAudioSource * This);
        
        HRESULT ( STDMETHODCALLTYPE *SubscribeFrameCaptured )( 
            IAudioSource * This,
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *UnsubscribeFrameCaptured )( 
            IAudioSource * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *GetFrameCapturedEventData )( 
            IAudioSource * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IFrameCapturedEventArgs **eventData);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_KinectSensor )( 
            IAudioSource * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IKinectSensor **sensor);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsActive )( 
            IAudioSource * This,
            /* [annotation][out][retval] */ 
            _Out_  BOOLEAN *isActive);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_SubFrameLengthInBytes )( 
            IAudioSource * This,
            /* [annotation][out][retval] */ 
            _Out_  UINT *length);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_SubFrameDuration )( 
            IAudioSource * This,
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *duration);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_MaxSubFrameCount )( 
            IAudioSource * This,
            /* [annotation][out][retval] */ 
            _Out_  UINT *count);
        
        HRESULT ( STDMETHODCALLTYPE *OpenReader )( 
            IAudioSource * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IAudioBeamFrameReader **reader);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_AudioBeams )( 
            IAudioSource * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IAudioBeamList **audioBeamList);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_AudioCalibrationState )( 
            IAudioSource * This,
            /* [annotation][out][retval] */ 
            _Out_  KinectAudioCalibrationState *audioCalibrationState);
        
        END_INTERFACE
    } IAudioSourceVtbl;

    interface IAudioSource
    {
        CONST_VTBL struct IAudioSourceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAudioSource_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAudioSource_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAudioSource_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAudioSource_SubscribeFrameCaptured(This,waitableHandle)	\
    ( (This)->lpVtbl -> SubscribeFrameCaptured(This,waitableHandle) ) 

#define IAudioSource_UnsubscribeFrameCaptured(This,waitableHandle)	\
    ( (This)->lpVtbl -> UnsubscribeFrameCaptured(This,waitableHandle) ) 

#define IAudioSource_GetFrameCapturedEventData(This,waitableHandle,eventData)	\
    ( (This)->lpVtbl -> GetFrameCapturedEventData(This,waitableHandle,eventData) ) 

#define IAudioSource_get_KinectSensor(This,sensor)	\
    ( (This)->lpVtbl -> get_KinectSensor(This,sensor) ) 

#define IAudioSource_get_IsActive(This,isActive)	\
    ( (This)->lpVtbl -> get_IsActive(This,isActive) ) 

#define IAudioSource_get_SubFrameLengthInBytes(This,length)	\
    ( (This)->lpVtbl -> get_SubFrameLengthInBytes(This,length) ) 

#define IAudioSource_get_SubFrameDuration(This,duration)	\
    ( (This)->lpVtbl -> get_SubFrameDuration(This,duration) ) 

#define IAudioSource_get_MaxSubFrameCount(This,count)	\
    ( (This)->lpVtbl -> get_MaxSubFrameCount(This,count) ) 

#define IAudioSource_OpenReader(This,reader)	\
    ( (This)->lpVtbl -> OpenReader(This,reader) ) 

#define IAudioSource_get_AudioBeams(This,audioBeamList)	\
    ( (This)->lpVtbl -> get_AudioBeams(This,audioBeamList) ) 

#define IAudioSource_get_AudioCalibrationState(This,audioCalibrationState)	\
    ( (This)->lpVtbl -> get_AudioCalibrationState(This,audioCalibrationState) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAudioSource_INTERFACE_DEFINED__ */


#ifndef __ICoordinateMapper_INTERFACE_DEFINED__
#define __ICoordinateMapper_INTERFACE_DEFINED__

/* interface ICoordinateMapper */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_ICoordinateMapper;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8784DF2D-16B0-481C-A11E-55E70BF25018")
    ICoordinateMapper : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SubscribeCoordinateMappingChanged( 
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnsubscribeCoordinateMappingChanged( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCoordinateMappingChangedEventData( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  ICoordinateMappingChangedEventArgs **eventData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MapCameraPointToDepthSpace( 
            CameraSpacePoint cameraPoint,
            /* [annotation][out][retval] */ 
            _Out_  DepthSpacePoint *depthPoint) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MapCameraPointToColorSpace( 
            CameraSpacePoint cameraPoint,
            /* [annotation][out][retval] */ 
            _Out_  ColorSpacePoint *colorPoint) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MapDepthPointToCameraSpace( 
            DepthSpacePoint depthPoint,
            UINT16 depth,
            /* [annotation][out][retval] */ 
            _Out_  CameraSpacePoint *cameraPoint) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MapDepthPointToColorSpace( 
            DepthSpacePoint depthPoint,
            UINT16 depth,
            /* [annotation][out][retval] */ 
            _Out_  ColorSpacePoint *colorPoint) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MapCameraPointsToDepthSpace( 
            UINT cameraPointCount,
            /* [annotation][size_is][in] */ 
            _In_reads_(cameraPointCount)  const CameraSpacePoint *cameraPoints,
            UINT depthPointCount,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(depthPointCount)  DepthSpacePoint *depthPoints) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MapCameraPointsToColorSpace( 
            UINT cameraPointCount,
            /* [annotation][size_is][in] */ 
            _In_reads_(cameraPointCount)  const CameraSpacePoint *cameraPoints,
            UINT colorPointCount,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(colorPointCount)  ColorSpacePoint *colorPoints) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MapDepthPointsToCameraSpace( 
            UINT depthPointCount,
            /* [annotation][size_is][in] */ 
            _In_reads_(depthPointCount)  const DepthSpacePoint *depthPoints,
            UINT depthCount,
            /* [annotation][size_is][in] */ 
            _In_reads_(depthCount)  const UINT16 *depths,
            UINT cameraPointCount,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(cameraPointCount)  CameraSpacePoint *cameraPoints) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MapDepthPointsToColorSpace( 
            UINT depthPointCount,
            /* [annotation][size_is][in] */ 
            _In_reads_(depthPointCount)  const DepthSpacePoint *depthPoints,
            UINT depthCount,
            /* [annotation][size_is][in] */ 
            _In_reads_(depthCount)  const UINT16 *depths,
            UINT colorPointCount,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(colorPointCount)  ColorSpacePoint *colorPoints) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MapDepthFrameToCameraSpace( 
            UINT depthPointCount,
            /* [annotation][size_is][in] */ 
            _In_reads_(depthPointCount)  const UINT16 *depthFrameData,
            UINT cameraPointCount,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(cameraPointCount)  CameraSpacePoint *cameraSpacePoints) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MapDepthFrameToColorSpace( 
            UINT depthPointCount,
            /* [annotation][size_is][in] */ 
            _In_reads_(depthPointCount)  const UINT16 *depthFrameData,
            UINT colorPointCount,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(colorPointCount)  ColorSpacePoint *colorSpacePoints) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MapColorFrameToDepthSpace( 
            UINT depthDataPointCount,
            /* [annotation][size_is][in] */ 
            _In_reads_(depthDataPointCount)  const UINT16 *depthFrameData,
            UINT depthPointCount,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(depthPointCount)  DepthSpacePoint *depthSpacePoints) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MapColorFrameToCameraSpace( 
            UINT depthDataPointCount,
            /* [annotation][size_is][in] */ 
            _In_reads_(depthDataPointCount)  const UINT16 *depthFrameData,
            UINT cameraPointCount,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(cameraPointCount)  CameraSpacePoint *cameraSpacePoints) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDepthFrameToCameraSpaceTable( 
            /* [annotation][out] */ 
            _Out_  UINT32 *tableEntryCount,
            /* [annotation][size_is][size_is][out][retval] */ 
            _Outptr_result_buffer_(*tableEntryCount)  PointF **tableEntries) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDepthCameraIntrinsics( 
            /* [annotation][out][retval] */ 
            _Out_  CameraIntrinsics *cameraIntrinsics) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICoordinateMapperVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoordinateMapper * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoordinateMapper * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoordinateMapper * This);
        
        HRESULT ( STDMETHODCALLTYPE *SubscribeCoordinateMappingChanged )( 
            ICoordinateMapper * This,
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *UnsubscribeCoordinateMappingChanged )( 
            ICoordinateMapper * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *GetCoordinateMappingChangedEventData )( 
            ICoordinateMapper * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  ICoordinateMappingChangedEventArgs **eventData);
        
        HRESULT ( STDMETHODCALLTYPE *MapCameraPointToDepthSpace )( 
            ICoordinateMapper * This,
            CameraSpacePoint cameraPoint,
            /* [annotation][out][retval] */ 
            _Out_  DepthSpacePoint *depthPoint);
        
        HRESULT ( STDMETHODCALLTYPE *MapCameraPointToColorSpace )( 
            ICoordinateMapper * This,
            CameraSpacePoint cameraPoint,
            /* [annotation][out][retval] */ 
            _Out_  ColorSpacePoint *colorPoint);
        
        HRESULT ( STDMETHODCALLTYPE *MapDepthPointToCameraSpace )( 
            ICoordinateMapper * This,
            DepthSpacePoint depthPoint,
            UINT16 depth,
            /* [annotation][out][retval] */ 
            _Out_  CameraSpacePoint *cameraPoint);
        
        HRESULT ( STDMETHODCALLTYPE *MapDepthPointToColorSpace )( 
            ICoordinateMapper * This,
            DepthSpacePoint depthPoint,
            UINT16 depth,
            /* [annotation][out][retval] */ 
            _Out_  ColorSpacePoint *colorPoint);
        
        HRESULT ( STDMETHODCALLTYPE *MapCameraPointsToDepthSpace )( 
            ICoordinateMapper * This,
            UINT cameraPointCount,
            /* [annotation][size_is][in] */ 
            _In_reads_(cameraPointCount)  const CameraSpacePoint *cameraPoints,
            UINT depthPointCount,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(depthPointCount)  DepthSpacePoint *depthPoints);
        
        HRESULT ( STDMETHODCALLTYPE *MapCameraPointsToColorSpace )( 
            ICoordinateMapper * This,
            UINT cameraPointCount,
            /* [annotation][size_is][in] */ 
            _In_reads_(cameraPointCount)  const CameraSpacePoint *cameraPoints,
            UINT colorPointCount,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(colorPointCount)  ColorSpacePoint *colorPoints);
        
        HRESULT ( STDMETHODCALLTYPE *MapDepthPointsToCameraSpace )( 
            ICoordinateMapper * This,
            UINT depthPointCount,
            /* [annotation][size_is][in] */ 
            _In_reads_(depthPointCount)  const DepthSpacePoint *depthPoints,
            UINT depthCount,
            /* [annotation][size_is][in] */ 
            _In_reads_(depthCount)  const UINT16 *depths,
            UINT cameraPointCount,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(cameraPointCount)  CameraSpacePoint *cameraPoints);
        
        HRESULT ( STDMETHODCALLTYPE *MapDepthPointsToColorSpace )( 
            ICoordinateMapper * This,
            UINT depthPointCount,
            /* [annotation][size_is][in] */ 
            _In_reads_(depthPointCount)  const DepthSpacePoint *depthPoints,
            UINT depthCount,
            /* [annotation][size_is][in] */ 
            _In_reads_(depthCount)  const UINT16 *depths,
            UINT colorPointCount,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(colorPointCount)  ColorSpacePoint *colorPoints);
        
        HRESULT ( STDMETHODCALLTYPE *MapDepthFrameToCameraSpace )( 
            ICoordinateMapper * This,
            UINT depthPointCount,
            /* [annotation][size_is][in] */ 
            _In_reads_(depthPointCount)  const UINT16 *depthFrameData,
            UINT cameraPointCount,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(cameraPointCount)  CameraSpacePoint *cameraSpacePoints);
        
        HRESULT ( STDMETHODCALLTYPE *MapDepthFrameToColorSpace )( 
            ICoordinateMapper * This,
            UINT depthPointCount,
            /* [annotation][size_is][in] */ 
            _In_reads_(depthPointCount)  const UINT16 *depthFrameData,
            UINT colorPointCount,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(colorPointCount)  ColorSpacePoint *colorSpacePoints);
        
        HRESULT ( STDMETHODCALLTYPE *MapColorFrameToDepthSpace )( 
            ICoordinateMapper * This,
            UINT depthDataPointCount,
            /* [annotation][size_is][in] */ 
            _In_reads_(depthDataPointCount)  const UINT16 *depthFrameData,
            UINT depthPointCount,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(depthPointCount)  DepthSpacePoint *depthSpacePoints);
        
        HRESULT ( STDMETHODCALLTYPE *MapColorFrameToCameraSpace )( 
            ICoordinateMapper * This,
            UINT depthDataPointCount,
            /* [annotation][size_is][in] */ 
            _In_reads_(depthDataPointCount)  const UINT16 *depthFrameData,
            UINT cameraPointCount,
            /* [annotation][size_is][out] */ 
            _Out_writes_all_(cameraPointCount)  CameraSpacePoint *cameraSpacePoints);
        
        HRESULT ( STDMETHODCALLTYPE *GetDepthFrameToCameraSpaceTable )( 
            ICoordinateMapper * This,
            /* [annotation][out] */ 
            _Out_  UINT32 *tableEntryCount,
            /* [annotation][size_is][size_is][out][retval] */ 
            _Outptr_result_buffer_(*tableEntryCount)  PointF **tableEntries);
        
        HRESULT ( STDMETHODCALLTYPE *GetDepthCameraIntrinsics )( 
            ICoordinateMapper * This,
            /* [annotation][out][retval] */ 
            _Out_  CameraIntrinsics *cameraIntrinsics);
        
        END_INTERFACE
    } ICoordinateMapperVtbl;

    interface ICoordinateMapper
    {
        CONST_VTBL struct ICoordinateMapperVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoordinateMapper_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoordinateMapper_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoordinateMapper_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICoordinateMapper_SubscribeCoordinateMappingChanged(This,waitableHandle)	\
    ( (This)->lpVtbl -> SubscribeCoordinateMappingChanged(This,waitableHandle) ) 

#define ICoordinateMapper_UnsubscribeCoordinateMappingChanged(This,waitableHandle)	\
    ( (This)->lpVtbl -> UnsubscribeCoordinateMappingChanged(This,waitableHandle) ) 

#define ICoordinateMapper_GetCoordinateMappingChangedEventData(This,waitableHandle,eventData)	\
    ( (This)->lpVtbl -> GetCoordinateMappingChangedEventData(This,waitableHandle,eventData) ) 

#define ICoordinateMapper_MapCameraPointToDepthSpace(This,cameraPoint,depthPoint)	\
    ( (This)->lpVtbl -> MapCameraPointToDepthSpace(This,cameraPoint,depthPoint) ) 

#define ICoordinateMapper_MapCameraPointToColorSpace(This,cameraPoint,colorPoint)	\
    ( (This)->lpVtbl -> MapCameraPointToColorSpace(This,cameraPoint,colorPoint) ) 

#define ICoordinateMapper_MapDepthPointToCameraSpace(This,depthPoint,depth,cameraPoint)	\
    ( (This)->lpVtbl -> MapDepthPointToCameraSpace(This,depthPoint,depth,cameraPoint) ) 

#define ICoordinateMapper_MapDepthPointToColorSpace(This,depthPoint,depth,colorPoint)	\
    ( (This)->lpVtbl -> MapDepthPointToColorSpace(This,depthPoint,depth,colorPoint) ) 

#define ICoordinateMapper_MapCameraPointsToDepthSpace(This,cameraPointCount,cameraPoints,depthPointCount,depthPoints)	\
    ( (This)->lpVtbl -> MapCameraPointsToDepthSpace(This,cameraPointCount,cameraPoints,depthPointCount,depthPoints) ) 

#define ICoordinateMapper_MapCameraPointsToColorSpace(This,cameraPointCount,cameraPoints,colorPointCount,colorPoints)	\
    ( (This)->lpVtbl -> MapCameraPointsToColorSpace(This,cameraPointCount,cameraPoints,colorPointCount,colorPoints) ) 

#define ICoordinateMapper_MapDepthPointsToCameraSpace(This,depthPointCount,depthPoints,depthCount,depths,cameraPointCount,cameraPoints)	\
    ( (This)->lpVtbl -> MapDepthPointsToCameraSpace(This,depthPointCount,depthPoints,depthCount,depths,cameraPointCount,cameraPoints) ) 

#define ICoordinateMapper_MapDepthPointsToColorSpace(This,depthPointCount,depthPoints,depthCount,depths,colorPointCount,colorPoints)	\
    ( (This)->lpVtbl -> MapDepthPointsToColorSpace(This,depthPointCount,depthPoints,depthCount,depths,colorPointCount,colorPoints) ) 

#define ICoordinateMapper_MapDepthFrameToCameraSpace(This,depthPointCount,depthFrameData,cameraPointCount,cameraSpacePoints)	\
    ( (This)->lpVtbl -> MapDepthFrameToCameraSpace(This,depthPointCount,depthFrameData,cameraPointCount,cameraSpacePoints) ) 

#define ICoordinateMapper_MapDepthFrameToColorSpace(This,depthPointCount,depthFrameData,colorPointCount,colorSpacePoints)	\
    ( (This)->lpVtbl -> MapDepthFrameToColorSpace(This,depthPointCount,depthFrameData,colorPointCount,colorSpacePoints) ) 

#define ICoordinateMapper_MapColorFrameToDepthSpace(This,depthDataPointCount,depthFrameData,depthPointCount,depthSpacePoints)	\
    ( (This)->lpVtbl -> MapColorFrameToDepthSpace(This,depthDataPointCount,depthFrameData,depthPointCount,depthSpacePoints) ) 

#define ICoordinateMapper_MapColorFrameToCameraSpace(This,depthDataPointCount,depthFrameData,cameraPointCount,cameraSpacePoints)	\
    ( (This)->lpVtbl -> MapColorFrameToCameraSpace(This,depthDataPointCount,depthFrameData,cameraPointCount,cameraSpacePoints) ) 

#define ICoordinateMapper_GetDepthFrameToCameraSpaceTable(This,tableEntryCount,tableEntries)	\
    ( (This)->lpVtbl -> GetDepthFrameToCameraSpaceTable(This,tableEntryCount,tableEntries) ) 

#define ICoordinateMapper_GetDepthCameraIntrinsics(This,cameraIntrinsics)	\
    ( (This)->lpVtbl -> GetDepthCameraIntrinsics(This,cameraIntrinsics) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoordinateMapper_INTERFACE_DEFINED__ */


#ifndef __ICoordinateMappingChangedEventArgs_INTERFACE_DEFINED__
#define __ICoordinateMappingChangedEventArgs_INTERFACE_DEFINED__

/* interface ICoordinateMappingChangedEventArgs */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_ICoordinateMappingChangedEventArgs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E9A2A0BF-13BD-4A53-A157-91FC8BB41F85")
    ICoordinateMappingChangedEventArgs : public IUnknown
    {
    public:
    };
    
    
#else 	/* C style interface */

    typedef struct ICoordinateMappingChangedEventArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoordinateMappingChangedEventArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoordinateMappingChangedEventArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoordinateMappingChangedEventArgs * This);
        
        END_INTERFACE
    } ICoordinateMappingChangedEventArgsVtbl;

    interface ICoordinateMappingChangedEventArgs
    {
        CONST_VTBL struct ICoordinateMappingChangedEventArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoordinateMappingChangedEventArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoordinateMappingChangedEventArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoordinateMappingChangedEventArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoordinateMappingChangedEventArgs_INTERFACE_DEFINED__ */


#ifndef __IColorCameraSettings_INTERFACE_DEFINED__
#define __IColorCameraSettings_INTERFACE_DEFINED__

/* interface IColorCameraSettings */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IColorCameraSettings;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("DBF802AB-0ADF-485A-A844-CF1C7956D039")
    IColorCameraSettings : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ExposureTime( 
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *exposureTime) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FrameInterval( 
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *frameInterval) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Gain( 
            /* [annotation][out][retval] */ 
            _Out_  float *gain) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Gamma( 
            /* [annotation][out][retval] */ 
            _Out_  float *gamma) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IColorCameraSettingsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IColorCameraSettings * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IColorCameraSettings * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IColorCameraSettings * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_ExposureTime )( 
            IColorCameraSettings * This,
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *exposureTime);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FrameInterval )( 
            IColorCameraSettings * This,
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *frameInterval);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Gain )( 
            IColorCameraSettings * This,
            /* [annotation][out][retval] */ 
            _Out_  float *gain);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Gamma )( 
            IColorCameraSettings * This,
            /* [annotation][out][retval] */ 
            _Out_  float *gamma);
        
        END_INTERFACE
    } IColorCameraSettingsVtbl;

    interface IColorCameraSettings
    {
        CONST_VTBL struct IColorCameraSettingsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IColorCameraSettings_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IColorCameraSettings_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IColorCameraSettings_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IColorCameraSettings_get_ExposureTime(This,exposureTime)	\
    ( (This)->lpVtbl -> get_ExposureTime(This,exposureTime) ) 

#define IColorCameraSettings_get_FrameInterval(This,frameInterval)	\
    ( (This)->lpVtbl -> get_FrameInterval(This,frameInterval) ) 

#define IColorCameraSettings_get_Gain(This,gain)	\
    ( (This)->lpVtbl -> get_Gain(This,gain) ) 

#define IColorCameraSettings_get_Gamma(This,gamma)	\
    ( (This)->lpVtbl -> get_Gamma(This,gamma) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IColorCameraSettings_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_Kinect2ECOM_0000_0052 */
/* [local] */ 

HRESULT WINAPI GetDefaultKinectSensor(_COM_Outptr_ IKinectSensor** defaultKinectSensor);
#define BODY_COUNT 6

#ifndef _PointerDeviceType_
#define _PointerDeviceType_
typedef enum _PointerDeviceType PointerDeviceType;


enum _PointerDeviceType
    {
        PointerDeviceType_Touch	= 0,
        PointerDeviceType_Pen	= 1,
        PointerDeviceType_Mouse	= 2,
        PointerDeviceType_Kinect	= 3
    } ;
#endif // _PointerDeviceType_
#ifndef _HandType_
#define _HandType_
typedef enum _HandType HandType;


enum _HandType
    {
        HandType_NONE	= 0,
        HandType_LEFT	= ( HandType_NONE + 1 ) ,
        HandType_RIGHT	= ( HandType_LEFT + 1 ) 
    } ;
#endif // _HandType_
#ifndef _KinectHoldingState_
#define _KinectHoldingState_
typedef enum _KinectHoldingState KinectHoldingState;


enum _KinectHoldingState
    {
        KinectHoldingState_Started	= 0,
        KinectHoldingState_Completed	= 1,
        KinectHoldingState_Canceled	= 2
    } ;
#endif // _KinectHoldingState_
#ifndef _KinectGestureSettings_
#define _KinectGestureSettings_
typedef enum _KinectGestureSettings KinectGestureSettings;


enum _KinectGestureSettings
    {
        KinectGestureSettings_None	= 0,
        KinectGestureSettings_Tap	= 0x1,
        KinectGestureSettings_ManipulationTranslateX	= 0x40,
        KinectGestureSettings_ManipulationTranslateY	= 0x80,
        KinectGestureSettings_ManipulationTranslateRailsX	= 0x100,
        KinectGestureSettings_ManipulationTranslateRailsY	= 0x200,
        KinectGestureSettings_ManipulationScale	= 0x800,
        KinectGestureSettings_ManipulationTranslateInertia	= 0x1000,
        KinectGestureSettings_KinectHold	= 0x10000
    } ;
#endif // _KinectGestureSettings_
#ifndef _KinectInteractionMode_
#define _KinectInteractionMode_
typedef enum _KinectInteractionMode KinectInteractionMode;


enum _KinectInteractionMode
    {
        KinectInteractionMode_Normal	= 0,
        KinectInteractionMode_Off	= 1,
        KinectInteractionMode_Media	= 2
    } ;
#endif // _KinectInteractionMode_
#ifndef _KinectEngagementMode_
#define _KinectEngagementMode_
typedef enum _KinectEngagementMode KinectEngagementMode;


enum _KinectEngagementMode
    {
        KinectEngagementMode_None	= 0,
        KinectEngagementMode_SystemOnePerson	= 1,
        KinectEngagementMode_SystemTwoPerson	= 2,
        KinectEngagementMode_ManualOnePerson	= 3,
        KinectEngagementMode_ManualTwoPerson	= 4
    } ;
#endif // _KinectEngagementMode_
#ifndef _KinectManipulationDelta_
#define _KinectManipulationDelta_
typedef struct _KinectManipulationDelta
    {
    PointF Translation;
    float Scale;
    float Rotation;
    float Expansion;
    } 	KinectManipulationDelta;

#endif // _KinectManipulationDelta_
#ifndef _KinectManipulationVelocities_
#define _KinectManipulationVelocities_
typedef struct _KinectManipulationVelocities
    {
    PointF Linear;
    float Angular;
    float Expansion;
    } 	KinectManipulationVelocities;

#endif // _KinectManipulationVelocities_




















extern RPC_IF_HANDLE __MIDL_itf_Kinect2ECOM_0000_0052_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Kinect2ECOM_0000_0052_v0_0_s_ifspec;

#ifndef __IBodyHandPair_INTERFACE_DEFINED__
#define __IBodyHandPair_INTERFACE_DEFINED__

/* interface IBodyHandPair */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IBodyHandPair;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3F0C2E40-E9DC-4178-A8F5-5600E059C84C")
    IBodyHandPair : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_BodyTrackingId( 
            /* [annotation][out][retval] */ 
            _Out_  UINT64 *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_BodyTrackingId( 
            /* [in] */ UINT64 value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_HandType( 
            /* [annotation][out][retval] */ 
            _Out_  HandType *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_HandType( 
            /* [in] */ HandType value) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IBodyHandPairVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBodyHandPair * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBodyHandPair * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBodyHandPair * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_BodyTrackingId )( 
            IBodyHandPair * This,
            /* [annotation][out][retval] */ 
            _Out_  UINT64 *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_BodyTrackingId )( 
            IBodyHandPair * This,
            /* [in] */ UINT64 value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_HandType )( 
            IBodyHandPair * This,
            /* [annotation][out][retval] */ 
            _Out_  HandType *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_HandType )( 
            IBodyHandPair * This,
            /* [in] */ HandType value);
        
        END_INTERFACE
    } IBodyHandPairVtbl;

    interface IBodyHandPair
    {
        CONST_VTBL struct IBodyHandPairVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBodyHandPair_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBodyHandPair_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBodyHandPair_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBodyHandPair_get_BodyTrackingId(This,value)	\
    ( (This)->lpVtbl -> get_BodyTrackingId(This,value) ) 

#define IBodyHandPair_put_BodyTrackingId(This,value)	\
    ( (This)->lpVtbl -> put_BodyTrackingId(This,value) ) 

#define IBodyHandPair_get_HandType(This,value)	\
    ( (This)->lpVtbl -> get_HandType(This,value) ) 

#define IBodyHandPair_put_HandType(This,value)	\
    ( (This)->lpVtbl -> put_HandType(This,value) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBodyHandPair_INTERFACE_DEFINED__ */


#ifndef __IKinectCoreWindow_INTERFACE_DEFINED__
#define __IKinectCoreWindow_INTERFACE_DEFINED__

/* interface IKinectCoreWindow */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IKinectCoreWindow;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6B5DDAC2-1C7A-4962-937B-A7A527149C64")
    IKinectCoreWindow : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SubscribePointerEntered( 
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnsubscribePointerEntered( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPointerEnteredEventData( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IKinectPointerEventArgs **eventData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SubscribePointerMoved( 
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnsubscribePointerMoved( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPointerMovedEventData( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IKinectPointerEventArgs **eventData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SubscribePointerExited( 
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnsubscribePointerExited( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPointerExitedEventData( 
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IKinectPointerEventArgs **eventData) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IKinectCoreWindowVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IKinectCoreWindow * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IKinectCoreWindow * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IKinectCoreWindow * This);
        
        HRESULT ( STDMETHODCALLTYPE *SubscribePointerEntered )( 
            IKinectCoreWindow * This,
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *UnsubscribePointerEntered )( 
            IKinectCoreWindow * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *GetPointerEnteredEventData )( 
            IKinectCoreWindow * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IKinectPointerEventArgs **eventData);
        
        HRESULT ( STDMETHODCALLTYPE *SubscribePointerMoved )( 
            IKinectCoreWindow * This,
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *UnsubscribePointerMoved )( 
            IKinectCoreWindow * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *GetPointerMovedEventData )( 
            IKinectCoreWindow * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IKinectPointerEventArgs **eventData);
        
        HRESULT ( STDMETHODCALLTYPE *SubscribePointerExited )( 
            IKinectCoreWindow * This,
            /* [annotation][out][retval] */ 
            _Out_  WAITABLE_HANDLE *waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *UnsubscribePointerExited )( 
            IKinectCoreWindow * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle);
        
        HRESULT ( STDMETHODCALLTYPE *GetPointerExitedEventData )( 
            IKinectCoreWindow * This,
            /* [annotation][in] */ 
            _In_  WAITABLE_HANDLE waitableHandle,
            /* [annotation][out][retval] */ 
            _Out_  IKinectPointerEventArgs **eventData);
        
        END_INTERFACE
    } IKinectCoreWindowVtbl;

    interface IKinectCoreWindow
    {
        CONST_VTBL struct IKinectCoreWindowVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IKinectCoreWindow_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IKinectCoreWindow_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IKinectCoreWindow_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IKinectCoreWindow_SubscribePointerEntered(This,waitableHandle)	\
    ( (This)->lpVtbl -> SubscribePointerEntered(This,waitableHandle) ) 

#define IKinectCoreWindow_UnsubscribePointerEntered(This,waitableHandle)	\
    ( (This)->lpVtbl -> UnsubscribePointerEntered(This,waitableHandle) ) 

#define IKinectCoreWindow_GetPointerEnteredEventData(This,waitableHandle,eventData)	\
    ( (This)->lpVtbl -> GetPointerEnteredEventData(This,waitableHandle,eventData) ) 

#define IKinectCoreWindow_SubscribePointerMoved(This,waitableHandle)	\
    ( (This)->lpVtbl -> SubscribePointerMoved(This,waitableHandle) ) 

#define IKinectCoreWindow_UnsubscribePointerMoved(This,waitableHandle)	\
    ( (This)->lpVtbl -> UnsubscribePointerMoved(This,waitableHandle) ) 

#define IKinectCoreWindow_GetPointerMovedEventData(This,waitableHandle,eventData)	\
    ( (This)->lpVtbl -> GetPointerMovedEventData(This,waitableHandle,eventData) ) 

#define IKinectCoreWindow_SubscribePointerExited(This,waitableHandle)	\
    ( (This)->lpVtbl -> SubscribePointerExited(This,waitableHandle) ) 

#define IKinectCoreWindow_UnsubscribePointerExited(This,waitableHandle)	\
    ( (This)->lpVtbl -> UnsubscribePointerExited(This,waitableHandle) ) 

#define IKinectCoreWindow_GetPointerExitedEventData(This,waitableHandle,eventData)	\
    ( (This)->lpVtbl -> GetPointerExitedEventData(This,waitableHandle,eventData) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IKinectCoreWindow_INTERFACE_DEFINED__ */


#ifndef __IKinectGestureRecognizer_INTERFACE_DEFINED__
#define __IKinectGestureRecognizer_INTERFACE_DEFINED__

/* interface IKinectGestureRecognizer */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IKinectGestureRecognizer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5F51A3C4-2400-4F6D-8A6C-25B96715EFF1")
    IKinectGestureRecognizer : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE RegisterSelectionTappedHandler( 
            /* [in] */ IKinectGestureRecognizerSelectionHandler *handler) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnregisterSelectionTappedHandler( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RegisterSelectionHoldingHandler( 
            /* [in] */ IKinectGestureRecognizerSelectionHandler *handler) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnregisterSelectionHoldingHandler( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RegisterSelectionPressingStartedHandler( 
            /* [in] */ IKinectGestureRecognizerSelectionHandler *handler) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnregisterSelectionPressingStartedHandler( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RegisterSelectionPressingUpdatedHandler( 
            /* [in] */ IKinectGestureRecognizerSelectionHandler *handler) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnregisterSelectionPressingUpdatedHandler( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RegisterSelectionPressingCompletedHandler( 
            /* [in] */ IKinectGestureRecognizerSelectionHandler *handler) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnregisterSelectionPressingCompletedHandler( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RegisterManipulationStartedHandler( 
            /* [in] */ IKinectGestureRecognizerManipulationHandler *handler) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnregisterManipulationStartedHandler( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RegisterManipulationUpdatedHandler( 
            /* [in] */ IKinectGestureRecognizerManipulationHandler *handler) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnregisterManipulationUpdatedHandler( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RegisterManipulationInertiaStartingHandler( 
            /* [in] */ IKinectGestureRecognizerManipulationHandler *handler) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnregisterManipulationInertiaStartingHandler( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RegisterManipulationCompletedHandler( 
            /* [in] */ IKinectGestureRecognizerManipulationHandler *handler) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnregisterManipulationCompletedHandler( void) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_GestureSettings( 
            /* [annotation][out][retval] */ 
            _Out_  KinectGestureSettings *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_GestureSettings( 
            /* [in] */ KinectGestureSettings value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsInertial( 
            /* [annotation][out][retval] */ 
            _Out_  boolean *value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsActive( 
            /* [annotation][out][retval] */ 
            _Out_  boolean *value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_InertiaTranslationDeceleration( 
            /* [annotation][out][retval] */ 
            _Out_  float *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_InertiaTranslationDeceleration( 
            /* [in] */ float value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_InertiaTranslationDisplacement( 
            /* [annotation][out][retval] */ 
            _Out_  float *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_InertiaTranslationDisplacement( 
            /* [in] */ float value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_AutoProcessInertia( 
            /* [annotation][out][retval] */ 
            _Out_  boolean *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_AutoProcessInertia( 
            /* [in] */ boolean value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_BoundingRect( 
            /* [annotation][out][retval] */ 
            _Out_  RectF *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_BoundingRect( 
            /* [in] */ RectF value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ProcessDownEvent( 
            /* [annotation][in] */ 
            _In_  IKinectPointerPoint *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ProcessUpEvent( 
            /* [annotation][in] */ 
            _In_  IKinectPointerPoint *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ProcessMoveEvents( 
            /* [annotation][in] */ 
            _In_  IKinectPointerPoint *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ProcessInertia( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CompleteGesture( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IKinectGestureRecognizerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IKinectGestureRecognizer * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IKinectGestureRecognizer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IKinectGestureRecognizer * This);
        
        HRESULT ( STDMETHODCALLTYPE *RegisterSelectionTappedHandler )( 
            IKinectGestureRecognizer * This,
            /* [in] */ IKinectGestureRecognizerSelectionHandler *handler);
        
        HRESULT ( STDMETHODCALLTYPE *UnregisterSelectionTappedHandler )( 
            IKinectGestureRecognizer * This);
        
        HRESULT ( STDMETHODCALLTYPE *RegisterSelectionHoldingHandler )( 
            IKinectGestureRecognizer * This,
            /* [in] */ IKinectGestureRecognizerSelectionHandler *handler);
        
        HRESULT ( STDMETHODCALLTYPE *UnregisterSelectionHoldingHandler )( 
            IKinectGestureRecognizer * This);
        
        HRESULT ( STDMETHODCALLTYPE *RegisterSelectionPressingStartedHandler )( 
            IKinectGestureRecognizer * This,
            /* [in] */ IKinectGestureRecognizerSelectionHandler *handler);
        
        HRESULT ( STDMETHODCALLTYPE *UnregisterSelectionPressingStartedHandler )( 
            IKinectGestureRecognizer * This);
        
        HRESULT ( STDMETHODCALLTYPE *RegisterSelectionPressingUpdatedHandler )( 
            IKinectGestureRecognizer * This,
            /* [in] */ IKinectGestureRecognizerSelectionHandler *handler);
        
        HRESULT ( STDMETHODCALLTYPE *UnregisterSelectionPressingUpdatedHandler )( 
            IKinectGestureRecognizer * This);
        
        HRESULT ( STDMETHODCALLTYPE *RegisterSelectionPressingCompletedHandler )( 
            IKinectGestureRecognizer * This,
            /* [in] */ IKinectGestureRecognizerSelectionHandler *handler);
        
        HRESULT ( STDMETHODCALLTYPE *UnregisterSelectionPressingCompletedHandler )( 
            IKinectGestureRecognizer * This);
        
        HRESULT ( STDMETHODCALLTYPE *RegisterManipulationStartedHandler )( 
            IKinectGestureRecognizer * This,
            /* [in] */ IKinectGestureRecognizerManipulationHandler *handler);
        
        HRESULT ( STDMETHODCALLTYPE *UnregisterManipulationStartedHandler )( 
            IKinectGestureRecognizer * This);
        
        HRESULT ( STDMETHODCALLTYPE *RegisterManipulationUpdatedHandler )( 
            IKinectGestureRecognizer * This,
            /* [in] */ IKinectGestureRecognizerManipulationHandler *handler);
        
        HRESULT ( STDMETHODCALLTYPE *UnregisterManipulationUpdatedHandler )( 
            IKinectGestureRecognizer * This);
        
        HRESULT ( STDMETHODCALLTYPE *RegisterManipulationInertiaStartingHandler )( 
            IKinectGestureRecognizer * This,
            /* [in] */ IKinectGestureRecognizerManipulationHandler *handler);
        
        HRESULT ( STDMETHODCALLTYPE *UnregisterManipulationInertiaStartingHandler )( 
            IKinectGestureRecognizer * This);
        
        HRESULT ( STDMETHODCALLTYPE *RegisterManipulationCompletedHandler )( 
            IKinectGestureRecognizer * This,
            /* [in] */ IKinectGestureRecognizerManipulationHandler *handler);
        
        HRESULT ( STDMETHODCALLTYPE *UnregisterManipulationCompletedHandler )( 
            IKinectGestureRecognizer * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_GestureSettings )( 
            IKinectGestureRecognizer * This,
            /* [annotation][out][retval] */ 
            _Out_  KinectGestureSettings *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_GestureSettings )( 
            IKinectGestureRecognizer * This,
            /* [in] */ KinectGestureSettings value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsInertial )( 
            IKinectGestureRecognizer * This,
            /* [annotation][out][retval] */ 
            _Out_  boolean *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsActive )( 
            IKinectGestureRecognizer * This,
            /* [annotation][out][retval] */ 
            _Out_  boolean *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_InertiaTranslationDeceleration )( 
            IKinectGestureRecognizer * This,
            /* [annotation][out][retval] */ 
            _Out_  float *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_InertiaTranslationDeceleration )( 
            IKinectGestureRecognizer * This,
            /* [in] */ float value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_InertiaTranslationDisplacement )( 
            IKinectGestureRecognizer * This,
            /* [annotation][out][retval] */ 
            _Out_  float *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_InertiaTranslationDisplacement )( 
            IKinectGestureRecognizer * This,
            /* [in] */ float value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_AutoProcessInertia )( 
            IKinectGestureRecognizer * This,
            /* [annotation][out][retval] */ 
            _Out_  boolean *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_AutoProcessInertia )( 
            IKinectGestureRecognizer * This,
            /* [in] */ boolean value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_BoundingRect )( 
            IKinectGestureRecognizer * This,
            /* [annotation][out][retval] */ 
            _Out_  RectF *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_BoundingRect )( 
            IKinectGestureRecognizer * This,
            /* [in] */ RectF value);
        
        HRESULT ( STDMETHODCALLTYPE *ProcessDownEvent )( 
            IKinectGestureRecognizer * This,
            /* [annotation][in] */ 
            _In_  IKinectPointerPoint *value);
        
        HRESULT ( STDMETHODCALLTYPE *ProcessUpEvent )( 
            IKinectGestureRecognizer * This,
            /* [annotation][in] */ 
            _In_  IKinectPointerPoint *value);
        
        HRESULT ( STDMETHODCALLTYPE *ProcessMoveEvents )( 
            IKinectGestureRecognizer * This,
            /* [annotation][in] */ 
            _In_  IKinectPointerPoint *value);
        
        HRESULT ( STDMETHODCALLTYPE *ProcessInertia )( 
            IKinectGestureRecognizer * This);
        
        HRESULT ( STDMETHODCALLTYPE *CompleteGesture )( 
            IKinectGestureRecognizer * This);
        
        END_INTERFACE
    } IKinectGestureRecognizerVtbl;

    interface IKinectGestureRecognizer
    {
        CONST_VTBL struct IKinectGestureRecognizerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IKinectGestureRecognizer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IKinectGestureRecognizer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IKinectGestureRecognizer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IKinectGestureRecognizer_RegisterSelectionTappedHandler(This,handler)	\
    ( (This)->lpVtbl -> RegisterSelectionTappedHandler(This,handler) ) 

#define IKinectGestureRecognizer_UnregisterSelectionTappedHandler(This)	\
    ( (This)->lpVtbl -> UnregisterSelectionTappedHandler(This) ) 

#define IKinectGestureRecognizer_RegisterSelectionHoldingHandler(This,handler)	\
    ( (This)->lpVtbl -> RegisterSelectionHoldingHandler(This,handler) ) 

#define IKinectGestureRecognizer_UnregisterSelectionHoldingHandler(This)	\
    ( (This)->lpVtbl -> UnregisterSelectionHoldingHandler(This) ) 

#define IKinectGestureRecognizer_RegisterSelectionPressingStartedHandler(This,handler)	\
    ( (This)->lpVtbl -> RegisterSelectionPressingStartedHandler(This,handler) ) 

#define IKinectGestureRecognizer_UnregisterSelectionPressingStartedHandler(This)	\
    ( (This)->lpVtbl -> UnregisterSelectionPressingStartedHandler(This) ) 

#define IKinectGestureRecognizer_RegisterSelectionPressingUpdatedHandler(This,handler)	\
    ( (This)->lpVtbl -> RegisterSelectionPressingUpdatedHandler(This,handler) ) 

#define IKinectGestureRecognizer_UnregisterSelectionPressingUpdatedHandler(This)	\
    ( (This)->lpVtbl -> UnregisterSelectionPressingUpdatedHandler(This) ) 

#define IKinectGestureRecognizer_RegisterSelectionPressingCompletedHandler(This,handler)	\
    ( (This)->lpVtbl -> RegisterSelectionPressingCompletedHandler(This,handler) ) 

#define IKinectGestureRecognizer_UnregisterSelectionPressingCompletedHandler(This)	\
    ( (This)->lpVtbl -> UnregisterSelectionPressingCompletedHandler(This) ) 

#define IKinectGestureRecognizer_RegisterManipulationStartedHandler(This,handler)	\
    ( (This)->lpVtbl -> RegisterManipulationStartedHandler(This,handler) ) 

#define IKinectGestureRecognizer_UnregisterManipulationStartedHandler(This)	\
    ( (This)->lpVtbl -> UnregisterManipulationStartedHandler(This) ) 

#define IKinectGestureRecognizer_RegisterManipulationUpdatedHandler(This,handler)	\
    ( (This)->lpVtbl -> RegisterManipulationUpdatedHandler(This,handler) ) 

#define IKinectGestureRecognizer_UnregisterManipulationUpdatedHandler(This)	\
    ( (This)->lpVtbl -> UnregisterManipulationUpdatedHandler(This) ) 

#define IKinectGestureRecognizer_RegisterManipulationInertiaStartingHandler(This,handler)	\
    ( (This)->lpVtbl -> RegisterManipulationInertiaStartingHandler(This,handler) ) 

#define IKinectGestureRecognizer_UnregisterManipulationInertiaStartingHandler(This)	\
    ( (This)->lpVtbl -> UnregisterManipulationInertiaStartingHandler(This) ) 

#define IKinectGestureRecognizer_RegisterManipulationCompletedHandler(This,handler)	\
    ( (This)->lpVtbl -> RegisterManipulationCompletedHandler(This,handler) ) 

#define IKinectGestureRecognizer_UnregisterManipulationCompletedHandler(This)	\
    ( (This)->lpVtbl -> UnregisterManipulationCompletedHandler(This) ) 

#define IKinectGestureRecognizer_get_GestureSettings(This,value)	\
    ( (This)->lpVtbl -> get_GestureSettings(This,value) ) 

#define IKinectGestureRecognizer_put_GestureSettings(This,value)	\
    ( (This)->lpVtbl -> put_GestureSettings(This,value) ) 

#define IKinectGestureRecognizer_get_IsInertial(This,value)	\
    ( (This)->lpVtbl -> get_IsInertial(This,value) ) 

#define IKinectGestureRecognizer_get_IsActive(This,value)	\
    ( (This)->lpVtbl -> get_IsActive(This,value) ) 

#define IKinectGestureRecognizer_get_InertiaTranslationDeceleration(This,value)	\
    ( (This)->lpVtbl -> get_InertiaTranslationDeceleration(This,value) ) 

#define IKinectGestureRecognizer_put_InertiaTranslationDeceleration(This,value)	\
    ( (This)->lpVtbl -> put_InertiaTranslationDeceleration(This,value) ) 

#define IKinectGestureRecognizer_get_InertiaTranslationDisplacement(This,value)	\
    ( (This)->lpVtbl -> get_InertiaTranslationDisplacement(This,value) ) 

#define IKinectGestureRecognizer_put_InertiaTranslationDisplacement(This,value)	\
    ( (This)->lpVtbl -> put_InertiaTranslationDisplacement(This,value) ) 

#define IKinectGestureRecognizer_get_AutoProcessInertia(This,value)	\
    ( (This)->lpVtbl -> get_AutoProcessInertia(This,value) ) 

#define IKinectGestureRecognizer_put_AutoProcessInertia(This,value)	\
    ( (This)->lpVtbl -> put_AutoProcessInertia(This,value) ) 

#define IKinectGestureRecognizer_get_BoundingRect(This,value)	\
    ( (This)->lpVtbl -> get_BoundingRect(This,value) ) 

#define IKinectGestureRecognizer_put_BoundingRect(This,value)	\
    ( (This)->lpVtbl -> put_BoundingRect(This,value) ) 

#define IKinectGestureRecognizer_ProcessDownEvent(This,value)	\
    ( (This)->lpVtbl -> ProcessDownEvent(This,value) ) 

#define IKinectGestureRecognizer_ProcessUpEvent(This,value)	\
    ( (This)->lpVtbl -> ProcessUpEvent(This,value) ) 

#define IKinectGestureRecognizer_ProcessMoveEvents(This,value)	\
    ( (This)->lpVtbl -> ProcessMoveEvents(This,value) ) 

#define IKinectGestureRecognizer_ProcessInertia(This)	\
    ( (This)->lpVtbl -> ProcessInertia(This) ) 

#define IKinectGestureRecognizer_CompleteGesture(This)	\
    ( (This)->lpVtbl -> CompleteGesture(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IKinectGestureRecognizer_INTERFACE_DEFINED__ */


#ifndef __IKinectGestureRecognizerSelectionHandler_INTERFACE_DEFINED__
#define __IKinectGestureRecognizerSelectionHandler_INTERFACE_DEFINED__

/* interface IKinectGestureRecognizerSelectionHandler */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IKinectGestureRecognizerSelectionHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9F3D5C22-158F-4A4E-AD4E-023FAE1F4BC9")
    IKinectGestureRecognizerSelectionHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnTapped( 
            /* [in] */ IKinectTappedEventArgs *args) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnHolding( 
            /* [in] */ IKinectHoldingEventArgs *args) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnPressingStarted( 
            /* [in] */ IKinectPressingStartedEventArgs *args) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnPressingUpdated( 
            /* [in] */ IKinectPressingUpdatedEventArgs *args) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnPressingCompleted( 
            /* [in] */ IKinectPressingCompletedEventArgs *args) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IKinectGestureRecognizerSelectionHandlerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IKinectGestureRecognizerSelectionHandler * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IKinectGestureRecognizerSelectionHandler * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IKinectGestureRecognizerSelectionHandler * This);
        
        HRESULT ( STDMETHODCALLTYPE *OnTapped )( 
            IKinectGestureRecognizerSelectionHandler * This,
            /* [in] */ IKinectTappedEventArgs *args);
        
        HRESULT ( STDMETHODCALLTYPE *OnHolding )( 
            IKinectGestureRecognizerSelectionHandler * This,
            /* [in] */ IKinectHoldingEventArgs *args);
        
        HRESULT ( STDMETHODCALLTYPE *OnPressingStarted )( 
            IKinectGestureRecognizerSelectionHandler * This,
            /* [in] */ IKinectPressingStartedEventArgs *args);
        
        HRESULT ( STDMETHODCALLTYPE *OnPressingUpdated )( 
            IKinectGestureRecognizerSelectionHandler * This,
            /* [in] */ IKinectPressingUpdatedEventArgs *args);
        
        HRESULT ( STDMETHODCALLTYPE *OnPressingCompleted )( 
            IKinectGestureRecognizerSelectionHandler * This,
            /* [in] */ IKinectPressingCompletedEventArgs *args);
        
        END_INTERFACE
    } IKinectGestureRecognizerSelectionHandlerVtbl;

    interface IKinectGestureRecognizerSelectionHandler
    {
        CONST_VTBL struct IKinectGestureRecognizerSelectionHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IKinectGestureRecognizerSelectionHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IKinectGestureRecognizerSelectionHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IKinectGestureRecognizerSelectionHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IKinectGestureRecognizerSelectionHandler_OnTapped(This,args)	\
    ( (This)->lpVtbl -> OnTapped(This,args) ) 

#define IKinectGestureRecognizerSelectionHandler_OnHolding(This,args)	\
    ( (This)->lpVtbl -> OnHolding(This,args) ) 

#define IKinectGestureRecognizerSelectionHandler_OnPressingStarted(This,args)	\
    ( (This)->lpVtbl -> OnPressingStarted(This,args) ) 

#define IKinectGestureRecognizerSelectionHandler_OnPressingUpdated(This,args)	\
    ( (This)->lpVtbl -> OnPressingUpdated(This,args) ) 

#define IKinectGestureRecognizerSelectionHandler_OnPressingCompleted(This,args)	\
    ( (This)->lpVtbl -> OnPressingCompleted(This,args) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IKinectGestureRecognizerSelectionHandler_INTERFACE_DEFINED__ */


#ifndef __IKinectGestureRecognizerManipulationHandler_INTERFACE_DEFINED__
#define __IKinectGestureRecognizerManipulationHandler_INTERFACE_DEFINED__

/* interface IKinectGestureRecognizerManipulationHandler */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IKinectGestureRecognizerManipulationHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("398CF7E0-4FD4-4336-BAAA-B6708B4C437B")
    IKinectGestureRecognizerManipulationHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnManipulationStarted( 
            /* [in] */ IKinectManipulationStartedEventArgs *args) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnManipulationUpdated( 
            /* [in] */ IKinectManipulationUpdatedEventArgs *args) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnManipulationInertiaStarting( 
            /* [in] */ IKinectManipulationInertiaStartingEventArgs *args) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnManipulationCompleted( 
            /* [in] */ IKinectManipulationCompletedEventArgs *args) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IKinectGestureRecognizerManipulationHandlerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IKinectGestureRecognizerManipulationHandler * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IKinectGestureRecognizerManipulationHandler * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IKinectGestureRecognizerManipulationHandler * This);
        
        HRESULT ( STDMETHODCALLTYPE *OnManipulationStarted )( 
            IKinectGestureRecognizerManipulationHandler * This,
            /* [in] */ IKinectManipulationStartedEventArgs *args);
        
        HRESULT ( STDMETHODCALLTYPE *OnManipulationUpdated )( 
            IKinectGestureRecognizerManipulationHandler * This,
            /* [in] */ IKinectManipulationUpdatedEventArgs *args);
        
        HRESULT ( STDMETHODCALLTYPE *OnManipulationInertiaStarting )( 
            IKinectGestureRecognizerManipulationHandler * This,
            /* [in] */ IKinectManipulationInertiaStartingEventArgs *args);
        
        HRESULT ( STDMETHODCALLTYPE *OnManipulationCompleted )( 
            IKinectGestureRecognizerManipulationHandler * This,
            /* [in] */ IKinectManipulationCompletedEventArgs *args);
        
        END_INTERFACE
    } IKinectGestureRecognizerManipulationHandlerVtbl;

    interface IKinectGestureRecognizerManipulationHandler
    {
        CONST_VTBL struct IKinectGestureRecognizerManipulationHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IKinectGestureRecognizerManipulationHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IKinectGestureRecognizerManipulationHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IKinectGestureRecognizerManipulationHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IKinectGestureRecognizerManipulationHandler_OnManipulationStarted(This,args)	\
    ( (This)->lpVtbl -> OnManipulationStarted(This,args) ) 

#define IKinectGestureRecognizerManipulationHandler_OnManipulationUpdated(This,args)	\
    ( (This)->lpVtbl -> OnManipulationUpdated(This,args) ) 

#define IKinectGestureRecognizerManipulationHandler_OnManipulationInertiaStarting(This,args)	\
    ( (This)->lpVtbl -> OnManipulationInertiaStarting(This,args) ) 

#define IKinectGestureRecognizerManipulationHandler_OnManipulationCompleted(This,args)	\
    ( (This)->lpVtbl -> OnManipulationCompleted(This,args) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IKinectGestureRecognizerManipulationHandler_INTERFACE_DEFINED__ */


#ifndef __IKinectHoldingEventArgs_INTERFACE_DEFINED__
#define __IKinectHoldingEventArgs_INTERFACE_DEFINED__

/* interface IKinectHoldingEventArgs */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IKinectHoldingEventArgs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A7B4B8F4-55B5-4FB2-B7C4-83B7700748DF")
    IKinectHoldingEventArgs : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_PointerDeviceType( 
            /* [annotation][out][retval] */ 
            _Out_  PointerDeviceType *value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Position( 
            /* [annotation][out][retval] */ 
            _Out_  PointF *value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_HoldingState( 
            /* [annotation][out][retval] */ 
            _Out_  KinectHoldingState *value) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IKinectHoldingEventArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IKinectHoldingEventArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IKinectHoldingEventArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IKinectHoldingEventArgs * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_PointerDeviceType )( 
            IKinectHoldingEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  PointerDeviceType *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Position )( 
            IKinectHoldingEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  PointF *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_HoldingState )( 
            IKinectHoldingEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  KinectHoldingState *value);
        
        END_INTERFACE
    } IKinectHoldingEventArgsVtbl;

    interface IKinectHoldingEventArgs
    {
        CONST_VTBL struct IKinectHoldingEventArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IKinectHoldingEventArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IKinectHoldingEventArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IKinectHoldingEventArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IKinectHoldingEventArgs_get_PointerDeviceType(This,value)	\
    ( (This)->lpVtbl -> get_PointerDeviceType(This,value) ) 

#define IKinectHoldingEventArgs_get_Position(This,value)	\
    ( (This)->lpVtbl -> get_Position(This,value) ) 

#define IKinectHoldingEventArgs_get_HoldingState(This,value)	\
    ( (This)->lpVtbl -> get_HoldingState(This,value) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IKinectHoldingEventArgs_INTERFACE_DEFINED__ */


#ifndef __IKinectManipulationCompletedEventArgs_INTERFACE_DEFINED__
#define __IKinectManipulationCompletedEventArgs_INTERFACE_DEFINED__

/* interface IKinectManipulationCompletedEventArgs */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IKinectManipulationCompletedEventArgs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("98AF1727-D511-459C-BE14-16CA8230871B")
    IKinectManipulationCompletedEventArgs : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_PointerDeviceType( 
            /* [annotation][out][retval] */ 
            _Out_  PointerDeviceType *value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Position( 
            /* [annotation][out][retval] */ 
            _Out_  PointF *value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Cumulative( 
            /* [annotation][out][retval] */ 
            _Out_  KinectManipulationDelta *value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Velocities( 
            /* [annotation][out][retval] */ 
            _Out_  KinectManipulationVelocities *value) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IKinectManipulationCompletedEventArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IKinectManipulationCompletedEventArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IKinectManipulationCompletedEventArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IKinectManipulationCompletedEventArgs * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_PointerDeviceType )( 
            IKinectManipulationCompletedEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  PointerDeviceType *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Position )( 
            IKinectManipulationCompletedEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  PointF *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Cumulative )( 
            IKinectManipulationCompletedEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  KinectManipulationDelta *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Velocities )( 
            IKinectManipulationCompletedEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  KinectManipulationVelocities *value);
        
        END_INTERFACE
    } IKinectManipulationCompletedEventArgsVtbl;

    interface IKinectManipulationCompletedEventArgs
    {
        CONST_VTBL struct IKinectManipulationCompletedEventArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IKinectManipulationCompletedEventArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IKinectManipulationCompletedEventArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IKinectManipulationCompletedEventArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IKinectManipulationCompletedEventArgs_get_PointerDeviceType(This,value)	\
    ( (This)->lpVtbl -> get_PointerDeviceType(This,value) ) 

#define IKinectManipulationCompletedEventArgs_get_Position(This,value)	\
    ( (This)->lpVtbl -> get_Position(This,value) ) 

#define IKinectManipulationCompletedEventArgs_get_Cumulative(This,value)	\
    ( (This)->lpVtbl -> get_Cumulative(This,value) ) 

#define IKinectManipulationCompletedEventArgs_get_Velocities(This,value)	\
    ( (This)->lpVtbl -> get_Velocities(This,value) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IKinectManipulationCompletedEventArgs_INTERFACE_DEFINED__ */


#ifndef __IKinectManipulationInertiaStartingEventArgs_INTERFACE_DEFINED__
#define __IKinectManipulationInertiaStartingEventArgs_INTERFACE_DEFINED__

/* interface IKinectManipulationInertiaStartingEventArgs */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IKinectManipulationInertiaStartingEventArgs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("90ACC90A-4573-406B-9715-20171D8A3875")
    IKinectManipulationInertiaStartingEventArgs : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_PointerDeviceType( 
            /* [annotation][out][retval] */ 
            _Out_  PointerDeviceType *value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Position( 
            /* [annotation][out][retval] */ 
            _Out_  PointF *value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Delta( 
            /* [annotation][out][retval] */ 
            _Out_  KinectManipulationDelta *value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Cumulative( 
            /* [annotation][out][retval] */ 
            _Out_  KinectManipulationDelta *value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Velocities( 
            /* [annotation][out][retval] */ 
            _Out_  KinectManipulationVelocities *value) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IKinectManipulationInertiaStartingEventArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IKinectManipulationInertiaStartingEventArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IKinectManipulationInertiaStartingEventArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IKinectManipulationInertiaStartingEventArgs * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_PointerDeviceType )( 
            IKinectManipulationInertiaStartingEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  PointerDeviceType *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Position )( 
            IKinectManipulationInertiaStartingEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  PointF *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Delta )( 
            IKinectManipulationInertiaStartingEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  KinectManipulationDelta *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Cumulative )( 
            IKinectManipulationInertiaStartingEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  KinectManipulationDelta *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Velocities )( 
            IKinectManipulationInertiaStartingEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  KinectManipulationVelocities *value);
        
        END_INTERFACE
    } IKinectManipulationInertiaStartingEventArgsVtbl;

    interface IKinectManipulationInertiaStartingEventArgs
    {
        CONST_VTBL struct IKinectManipulationInertiaStartingEventArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IKinectManipulationInertiaStartingEventArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IKinectManipulationInertiaStartingEventArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IKinectManipulationInertiaStartingEventArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IKinectManipulationInertiaStartingEventArgs_get_PointerDeviceType(This,value)	\
    ( (This)->lpVtbl -> get_PointerDeviceType(This,value) ) 

#define IKinectManipulationInertiaStartingEventArgs_get_Position(This,value)	\
    ( (This)->lpVtbl -> get_Position(This,value) ) 

#define IKinectManipulationInertiaStartingEventArgs_get_Delta(This,value)	\
    ( (This)->lpVtbl -> get_Delta(This,value) ) 

#define IKinectManipulationInertiaStartingEventArgs_get_Cumulative(This,value)	\
    ( (This)->lpVtbl -> get_Cumulative(This,value) ) 

#define IKinectManipulationInertiaStartingEventArgs_get_Velocities(This,value)	\
    ( (This)->lpVtbl -> get_Velocities(This,value) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IKinectManipulationInertiaStartingEventArgs_INTERFACE_DEFINED__ */


#ifndef __IKinectManipulationStartedEventArgs_INTERFACE_DEFINED__
#define __IKinectManipulationStartedEventArgs_INTERFACE_DEFINED__

/* interface IKinectManipulationStartedEventArgs */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IKinectManipulationStartedEventArgs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2EC8A1BA-96EC-49F7-BD41-D5FE791F0D46")
    IKinectManipulationStartedEventArgs : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_PointerDeviceType( 
            /* [annotation][out][retval] */ 
            _Out_  PointerDeviceType *value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Position( 
            /* [annotation][out][retval] */ 
            _Out_  PointF *value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Cumulative( 
            /* [annotation][out][retval] */ 
            _Out_  KinectManipulationDelta *value) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IKinectManipulationStartedEventArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IKinectManipulationStartedEventArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IKinectManipulationStartedEventArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IKinectManipulationStartedEventArgs * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_PointerDeviceType )( 
            IKinectManipulationStartedEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  PointerDeviceType *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Position )( 
            IKinectManipulationStartedEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  PointF *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Cumulative )( 
            IKinectManipulationStartedEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  KinectManipulationDelta *value);
        
        END_INTERFACE
    } IKinectManipulationStartedEventArgsVtbl;

    interface IKinectManipulationStartedEventArgs
    {
        CONST_VTBL struct IKinectManipulationStartedEventArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IKinectManipulationStartedEventArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IKinectManipulationStartedEventArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IKinectManipulationStartedEventArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IKinectManipulationStartedEventArgs_get_PointerDeviceType(This,value)	\
    ( (This)->lpVtbl -> get_PointerDeviceType(This,value) ) 

#define IKinectManipulationStartedEventArgs_get_Position(This,value)	\
    ( (This)->lpVtbl -> get_Position(This,value) ) 

#define IKinectManipulationStartedEventArgs_get_Cumulative(This,value)	\
    ( (This)->lpVtbl -> get_Cumulative(This,value) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IKinectManipulationStartedEventArgs_INTERFACE_DEFINED__ */


#ifndef __IKinectManipulationUpdatedEventArgs_INTERFACE_DEFINED__
#define __IKinectManipulationUpdatedEventArgs_INTERFACE_DEFINED__

/* interface IKinectManipulationUpdatedEventArgs */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IKinectManipulationUpdatedEventArgs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1097A513-F25E-4B48-84F2-2D3B0B3FC78C")
    IKinectManipulationUpdatedEventArgs : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_PointerDeviceType( 
            /* [annotation][out][retval] */ 
            _Out_  PointerDeviceType *value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Position( 
            /* [annotation][out][retval] */ 
            _Out_  PointF *value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Delta( 
            /* [annotation][out][retval] */ 
            _Out_  KinectManipulationDelta *value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Cumulative( 
            /* [annotation][out][retval] */ 
            _Out_  KinectManipulationDelta *value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Velocities( 
            /* [annotation][out][retval] */ 
            _Out_  KinectManipulationVelocities *value) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IKinectManipulationUpdatedEventArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IKinectManipulationUpdatedEventArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IKinectManipulationUpdatedEventArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IKinectManipulationUpdatedEventArgs * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_PointerDeviceType )( 
            IKinectManipulationUpdatedEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  PointerDeviceType *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Position )( 
            IKinectManipulationUpdatedEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  PointF *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Delta )( 
            IKinectManipulationUpdatedEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  KinectManipulationDelta *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Cumulative )( 
            IKinectManipulationUpdatedEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  KinectManipulationDelta *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Velocities )( 
            IKinectManipulationUpdatedEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  KinectManipulationVelocities *value);
        
        END_INTERFACE
    } IKinectManipulationUpdatedEventArgsVtbl;

    interface IKinectManipulationUpdatedEventArgs
    {
        CONST_VTBL struct IKinectManipulationUpdatedEventArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IKinectManipulationUpdatedEventArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IKinectManipulationUpdatedEventArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IKinectManipulationUpdatedEventArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IKinectManipulationUpdatedEventArgs_get_PointerDeviceType(This,value)	\
    ( (This)->lpVtbl -> get_PointerDeviceType(This,value) ) 

#define IKinectManipulationUpdatedEventArgs_get_Position(This,value)	\
    ( (This)->lpVtbl -> get_Position(This,value) ) 

#define IKinectManipulationUpdatedEventArgs_get_Delta(This,value)	\
    ( (This)->lpVtbl -> get_Delta(This,value) ) 

#define IKinectManipulationUpdatedEventArgs_get_Cumulative(This,value)	\
    ( (This)->lpVtbl -> get_Cumulative(This,value) ) 

#define IKinectManipulationUpdatedEventArgs_get_Velocities(This,value)	\
    ( (This)->lpVtbl -> get_Velocities(This,value) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IKinectManipulationUpdatedEventArgs_INTERFACE_DEFINED__ */


#ifndef __IKinectPointerDevice_INTERFACE_DEFINED__
#define __IKinectPointerDevice_INTERFACE_DEFINED__

/* interface IKinectPointerDevice */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IKinectPointerDevice;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C6F3A13D-16DA-4556-AD25-C4CD73A2D625")
    IKinectPointerDevice : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_PointerDeviceType( 
            /* [annotation][out][retval] */ 
            _Out_  PointerDeviceType *value) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IKinectPointerDeviceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IKinectPointerDevice * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IKinectPointerDevice * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IKinectPointerDevice * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_PointerDeviceType )( 
            IKinectPointerDevice * This,
            /* [annotation][out][retval] */ 
            _Out_  PointerDeviceType *value);
        
        END_INTERFACE
    } IKinectPointerDeviceVtbl;

    interface IKinectPointerDevice
    {
        CONST_VTBL struct IKinectPointerDeviceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IKinectPointerDevice_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IKinectPointerDevice_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IKinectPointerDevice_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IKinectPointerDevice_get_PointerDeviceType(This,value)	\
    ( (This)->lpVtbl -> get_PointerDeviceType(This,value) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IKinectPointerDevice_INTERFACE_DEFINED__ */


#ifndef __IKinectPointerEventArgs_INTERFACE_DEFINED__
#define __IKinectPointerEventArgs_INTERFACE_DEFINED__

/* interface IKinectPointerEventArgs */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IKinectPointerEventArgs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("65635AE1-3D28-4D5A-AA09-825AAE07C1B1")
    IKinectPointerEventArgs : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Handled( 
            /* [annotation][out][retval] */ 
            _Out_  boolean *handled) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Handled( 
            /* [in] */ boolean handled) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_CurrentPoint( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IKinectPointerPoint **pointer) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IKinectPointerEventArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IKinectPointerEventArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IKinectPointerEventArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IKinectPointerEventArgs * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Handled )( 
            IKinectPointerEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  boolean *handled);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Handled )( 
            IKinectPointerEventArgs * This,
            /* [in] */ boolean handled);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_CurrentPoint )( 
            IKinectPointerEventArgs * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IKinectPointerPoint **pointer);
        
        END_INTERFACE
    } IKinectPointerEventArgsVtbl;

    interface IKinectPointerEventArgs
    {
        CONST_VTBL struct IKinectPointerEventArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IKinectPointerEventArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IKinectPointerEventArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IKinectPointerEventArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IKinectPointerEventArgs_get_Handled(This,handled)	\
    ( (This)->lpVtbl -> get_Handled(This,handled) ) 

#define IKinectPointerEventArgs_put_Handled(This,handled)	\
    ( (This)->lpVtbl -> put_Handled(This,handled) ) 

#define IKinectPointerEventArgs_get_CurrentPoint(This,pointer)	\
    ( (This)->lpVtbl -> get_CurrentPoint(This,pointer) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IKinectPointerEventArgs_INTERFACE_DEFINED__ */


#ifndef __IKinectPointerPoint_INTERFACE_DEFINED__
#define __IKinectPointerPoint_INTERFACE_DEFINED__

/* interface IKinectPointerPoint */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IKinectPointerPoint;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D2F1E5B8-9DAD-4933-BCD2-90F4C64E17AC")
    IKinectPointerPoint : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_PointerDevice( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IKinectPointerDevice **ppPointerDevice) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_PointerId( 
            /* [annotation][out][retval] */ 
            _Out_  UINT32 *pointerId) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Position( 
            /* [annotation][out][retval] */ 
            _Out_  PointF *position) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_RawPosition( 
            /* [annotation][out][retval] */ 
            _Out_  PointF *rawPosition) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Timestamp( 
            /* [annotation][out][retval] */ 
            _Out_  UINT64 *timestamp) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Properties( 
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IKinectPointerPointProperties **ppProperties) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IKinectPointerPointVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IKinectPointerPoint * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IKinectPointerPoint * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IKinectPointerPoint * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_PointerDevice )( 
            IKinectPointerPoint * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IKinectPointerDevice **ppPointerDevice);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_PointerId )( 
            IKinectPointerPoint * This,
            /* [annotation][out][retval] */ 
            _Out_  UINT32 *pointerId);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Position )( 
            IKinectPointerPoint * This,
            /* [annotation][out][retval] */ 
            _Out_  PointF *position);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_RawPosition )( 
            IKinectPointerPoint * This,
            /* [annotation][out][retval] */ 
            _Out_  PointF *rawPosition);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Timestamp )( 
            IKinectPointerPoint * This,
            /* [annotation][out][retval] */ 
            _Out_  UINT64 *timestamp);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Properties )( 
            IKinectPointerPoint * This,
            /* [annotation][out][retval] */ 
            _COM_Outptr_  IKinectPointerPointProperties **ppProperties);
        
        END_INTERFACE
    } IKinectPointerPointVtbl;

    interface IKinectPointerPoint
    {
        CONST_VTBL struct IKinectPointerPointVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IKinectPointerPoint_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IKinectPointerPoint_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IKinectPointerPoint_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IKinectPointerPoint_get_PointerDevice(This,ppPointerDevice)	\
    ( (This)->lpVtbl -> get_PointerDevice(This,ppPointerDevice) ) 

#define IKinectPointerPoint_get_PointerId(This,pointerId)	\
    ( (This)->lpVtbl -> get_PointerId(This,pointerId) ) 

#define IKinectPointerPoint_get_Position(This,position)	\
    ( (This)->lpVtbl -> get_Position(This,position) ) 

#define IKinectPointerPoint_get_RawPosition(This,rawPosition)	\
    ( (This)->lpVtbl -> get_RawPosition(This,rawPosition) ) 

#define IKinectPointerPoint_get_Timestamp(This,timestamp)	\
    ( (This)->lpVtbl -> get_Timestamp(This,timestamp) ) 

#define IKinectPointerPoint_get_Properties(This,ppProperties)	\
    ( (This)->lpVtbl -> get_Properties(This,ppProperties) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IKinectPointerPoint_INTERFACE_DEFINED__ */


#ifndef __IKinectPointerPointProperties_INTERFACE_DEFINED__
#define __IKinectPointerPointProperties_INTERFACE_DEFINED__

/* interface IKinectPointerPointProperties */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IKinectPointerPointProperties;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5527A776-CA76-47BA-95B4-A2DB2BD32207")
    IKinectPointerPointProperties : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsPrimary( 
            /* [annotation][out][retval] */ 
            _Out_  boolean *isPrimary) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsInRange( 
            /* [annotation][out][retval] */ 
            _Out_  boolean *isInRange) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsEngaged( 
            /* [annotation][out][retval] */ 
            _Out_  boolean *isEngaged) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_BodyTrackingId( 
            /* [annotation][out][retval] */ 
            _Out_  UINT64 *bodyTrackingId) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_HandType( 
            /* [annotation][out][retval] */ 
            _Out_  HandType *handType) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_HandReachExtent( 
            /* [annotation][out][retval] */ 
            _Out_  float *handReachExtent) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_BodyTimeCounter( 
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *bodyTimeCounter) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_HandRotation( 
            /* [annotation][out][retval] */ 
            _Out_  float *handRotation) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_PressExtent( 
            /* [annotation][out][retval] */ 
            _Out_  float *pressExtent) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_UnclampedPosition( 
            /* [annotation][out][retval] */ 
            _Out_  PointF *unclampedPosition) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IKinectPointerPointPropertiesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IKinectPointerPointProperties * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IKinectPointerPointProperties * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IKinectPointerPointProperties * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsPrimary )( 
            IKinectPointerPointProperties * This,
            /* [annotation][out][retval] */ 
            _Out_  boolean *isPrimary);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsInRange )( 
            IKinectPointerPointProperties * This,
            /* [annotation][out][retval] */ 
            _Out_  boolean *isInRange);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsEngaged )( 
            IKinectPointerPointProperties * This,
            /* [annotation][out][retval] */ 
            _Out_  boolean *isEngaged);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_BodyTrackingId )( 
            IKinectPointerPointProperties * This,
            /* [annotation][out][retval] */ 
            _Out_  UINT64 *bodyTrackingId);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_HandType )( 
            IKinectPointerPointProperties * This,
            /* [annotation][out][retval] */ 
            _Out_  HandType *handType);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_HandReachExtent )( 
            IKinectPointerPointProperties * This,
            /* [annotation][out][retval] */ 
            _Out_  float *handReachExtent);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_BodyTimeCounter )( 
            IKinectPointerPointProperties * This,
            /* [annotation][out][retval] */ 
            _Out_  TIMESPAN *bodyTimeCounter);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_HandRotation )( 
            IKinectPointerPointProperties * This,
            /* [annotation][out][retval] */ 
            _Out_  float *handRotation);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_PressExtent )( 
            IKinectPointerPointProperties * This,
            /* [annotation][out][retval] */ 
            _Out_  float *pressExtent);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_UnclampedPosition )( 
            IKinectPointerPointProperties * This,
            /* [annotation][out][retval] */ 
            _Out_  PointF *unclampedPosition);
        
        END_INTERFACE
    } IKinectPointerPointPropertiesVtbl;

    interface IKinectPointerPointProperties
    {
        CONST_VTBL struct IKinectPointerPointPropertiesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IKinectPointerPointProperties_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IKinectPointerPointProperties_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IKinectPointerPointProperties_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IKinectPointerPointProperties_get_IsPrimary(This,isPrimary)	\
    ( (This)->lpVtbl -> get_IsPrimary(This,isPrimary) ) 

#define IKinectPointerPointProperties_get_IsInRange(This,isInRange)	\
    ( (This)->lpVtbl -> get_IsInRange(This,isInRange) ) 

#define IKinectPointerPointProperties_get_IsEngaged(This,isEngaged)	\
    ( (This)->lpVtbl -> get_IsEngaged(This,isEngaged) ) 

#define IKinectPointerPointProperties_get_BodyTrackingId(This,bodyTrackingId)	\
    ( (This)->lpVtbl -> get_BodyTrackingId(This,bodyTrackingId) ) 

#define IKinectPointerPointProperties_get_HandType(This,handType)	\
    ( (This)->lpVtbl -> get_HandType(This,handType) ) 

#define IKinectPointerPointProperties_get_HandReachExtent(This,handReachExtent)	\
    ( (This)->lpVtbl -> get_HandReachExtent(This,handReachExtent) ) 

#define IKinectPointerPointProperties_get_BodyTimeCounter(This,bodyTimeCounter)	\
    ( (This)->lpVtbl -> get_BodyTimeCounter(This,bodyTimeCounter) ) 

#define IKinectPointerPointProperties_get_HandRotation(This,handRotation)	\
    ( (This)->lpVtbl -> get_HandRotation(This,handRotation) ) 

#define IKinectPointerPointProperties_get_PressExtent(This,pressExtent)	\
    ( (This)->lpVtbl -> get_PressExtent(This,pressExtent) ) 

#define IKinectPointerPointProperties_get_UnclampedPosition(This,unclampedPosition)	\
    ( (This)->lpVtbl -> get_UnclampedPosition(This,unclampedPosition) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IKinectPointerPointProperties_INTERFACE_DEFINED__ */


#ifndef __IKinectPressingCompletedEventArgs_INTERFACE_DEFINED__
#define __IKinectPressingCompletedEventArgs_INTERFACE_DEFINED__

/* interface IKinectPressingCompletedEventArgs */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IKinectPressingCompletedEventArgs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7B9DAD43-0DFA-48DC-A2E3-CDBA3B3E251F")
    IKinectPressingCompletedEventArgs : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Position( 
            /* [annotation][out][retval] */ 
            _Out_  PointF *value) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IKinectPressingCompletedEventArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IKinectPressingCompletedEventArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IKinectPressingCompletedEventArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IKinectPressingCompletedEventArgs * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Position )( 
            IKinectPressingCompletedEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  PointF *value);
        
        END_INTERFACE
    } IKinectPressingCompletedEventArgsVtbl;

    interface IKinectPressingCompletedEventArgs
    {
        CONST_VTBL struct IKinectPressingCompletedEventArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IKinectPressingCompletedEventArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IKinectPressingCompletedEventArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IKinectPressingCompletedEventArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IKinectPressingCompletedEventArgs_get_Position(This,value)	\
    ( (This)->lpVtbl -> get_Position(This,value) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IKinectPressingCompletedEventArgs_INTERFACE_DEFINED__ */


#ifndef __IKinectPressingStartedEventArgs_INTERFACE_DEFINED__
#define __IKinectPressingStartedEventArgs_INTERFACE_DEFINED__

/* interface IKinectPressingStartedEventArgs */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IKinectPressingStartedEventArgs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C01511F4-C7B3-4BA1-A15F-09DC854962AB")
    IKinectPressingStartedEventArgs : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Position( 
            /* [annotation][out][retval] */ 
            _Out_  PointF *value) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IKinectPressingStartedEventArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IKinectPressingStartedEventArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IKinectPressingStartedEventArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IKinectPressingStartedEventArgs * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Position )( 
            IKinectPressingStartedEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  PointF *value);
        
        END_INTERFACE
    } IKinectPressingStartedEventArgsVtbl;

    interface IKinectPressingStartedEventArgs
    {
        CONST_VTBL struct IKinectPressingStartedEventArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IKinectPressingStartedEventArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IKinectPressingStartedEventArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IKinectPressingStartedEventArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IKinectPressingStartedEventArgs_get_Position(This,value)	\
    ( (This)->lpVtbl -> get_Position(This,value) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IKinectPressingStartedEventArgs_INTERFACE_DEFINED__ */


#ifndef __IKinectPressingUpdatedEventArgs_INTERFACE_DEFINED__
#define __IKinectPressingUpdatedEventArgs_INTERFACE_DEFINED__

/* interface IKinectPressingUpdatedEventArgs */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IKinectPressingUpdatedEventArgs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D7D93D29-228E-4EBD-A3B9-E1B916B9BAEF")
    IKinectPressingUpdatedEventArgs : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Position( 
            /* [annotation][out][retval] */ 
            _Out_  PointF *value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_PressExtent( 
            /* [annotation][out][retval] */ 
            _Out_  float *value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_HoldProgress( 
            /* [annotation][out][retval] */ 
            _Out_  float *value) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IKinectPressingUpdatedEventArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IKinectPressingUpdatedEventArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IKinectPressingUpdatedEventArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IKinectPressingUpdatedEventArgs * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Position )( 
            IKinectPressingUpdatedEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  PointF *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_PressExtent )( 
            IKinectPressingUpdatedEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  float *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_HoldProgress )( 
            IKinectPressingUpdatedEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  float *value);
        
        END_INTERFACE
    } IKinectPressingUpdatedEventArgsVtbl;

    interface IKinectPressingUpdatedEventArgs
    {
        CONST_VTBL struct IKinectPressingUpdatedEventArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IKinectPressingUpdatedEventArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IKinectPressingUpdatedEventArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IKinectPressingUpdatedEventArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IKinectPressingUpdatedEventArgs_get_Position(This,value)	\
    ( (This)->lpVtbl -> get_Position(This,value) ) 

#define IKinectPressingUpdatedEventArgs_get_PressExtent(This,value)	\
    ( (This)->lpVtbl -> get_PressExtent(This,value) ) 

#define IKinectPressingUpdatedEventArgs_get_HoldProgress(This,value)	\
    ( (This)->lpVtbl -> get_HoldProgress(This,value) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IKinectPressingUpdatedEventArgs_INTERFACE_DEFINED__ */


#ifndef __IKinectTappedEventArgs_INTERFACE_DEFINED__
#define __IKinectTappedEventArgs_INTERFACE_DEFINED__

/* interface IKinectTappedEventArgs */
/* [object][local][uuid] */ 


EXTERN_C const IID IID_IKinectTappedEventArgs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C5B870C3-C819-4FF9-9F11-B5334D98761E")
    IKinectTappedEventArgs : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_PointerDeviceType( 
            /* [annotation][out][retval] */ 
            _Out_  PointerDeviceType *value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Position( 
            /* [annotation][out][retval] */ 
            _Out_  PointF *value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_TapCount( 
            /* [annotation][out][retval] */ 
            _Out_  UINT32 *value) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IKinectTappedEventArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IKinectTappedEventArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IKinectTappedEventArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IKinectTappedEventArgs * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_PointerDeviceType )( 
            IKinectTappedEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  PointerDeviceType *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Position )( 
            IKinectTappedEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  PointF *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_TapCount )( 
            IKinectTappedEventArgs * This,
            /* [annotation][out][retval] */ 
            _Out_  UINT32 *value);
        
        END_INTERFACE
    } IKinectTappedEventArgsVtbl;

    interface IKinectTappedEventArgs
    {
        CONST_VTBL struct IKinectTappedEventArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IKinectTappedEventArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IKinectTappedEventArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IKinectTappedEventArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IKinectTappedEventArgs_get_PointerDeviceType(This,value)	\
    ( (This)->lpVtbl -> get_PointerDeviceType(This,value) ) 

#define IKinectTappedEventArgs_get_Position(This,value)	\
    ( (This)->lpVtbl -> get_Position(This,value) ) 

#define IKinectTappedEventArgs_get_TapCount(This,value)	\
    ( (This)->lpVtbl -> get_TapCount(This,value) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IKinectTappedEventArgs_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_Kinect2ECOM_0000_0070 */
/* [local] */ 

HRESULT WINAPI GetKinectCoreWindowForCurrentThread(_COM_Outptr_ IKinectCoreWindow** ppKinectCoreWindow);
HRESULT WINAPI CreateKinectGestureRecognizer(_COM_Outptr_ IKinectGestureRecognizer** ppKinectGestureRecognizer);
HRESULT WINAPI OverrideKinectInteractionMode(KinectInteractionMode mode);
HRESULT WINAPI SetKinectOnePersonSystemEngagement();
HRESULT WINAPI SetKinectTwoPersonSystemEngagement();
HRESULT WINAPI SetKinectOnePersonManualEngagement(_In_ IBodyHandPair* pBodyHandPair);
HRESULT WINAPI SetKinectTwoPersonManualEngagement(_In_ IBodyHandPair* pBodyHandPair1, _In_ IBodyHandPair* pBodyHandPair2);
HRESULT WINAPI GetKinectEngagementMode(_Out_ KinectEngagementMode* pMode);
HRESULT WINAPI GetKinectManualEngagedHandCount(_Out_ UINT32* pManualEngagedHandCount);
HRESULT WINAPI GetKinectManualEngagedHand(UINT32 manualEngagedHandIndex, _COM_Outptr_ IBodyHandPair** ppManualEngagedHand);
HRESULT WINAPI GetMaximumKinectEngagedPersonCount(_Out_ UINT32* pMaximumKinectEngagedPersonCount);
HRESULT WINAPI CreateBodyHandPair(UINT64 bodyTrackingId, HandType handType, _COM_Outptr_ IBodyHandPair** ppBodyHandPair);


extern RPC_IF_HANDLE __MIDL_itf_Kinect2ECOM_0000_0070_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Kinect2ECOM_0000_0070_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


