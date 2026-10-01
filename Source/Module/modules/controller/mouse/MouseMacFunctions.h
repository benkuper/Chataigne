/*
  ==============================================================================

    MouseMacFunctions.h
    Created: 13 Mar 2020 12:09:00pm
    Author:  Ben Kuper

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#if JUCE_MAC
    #include <CoreGraphics/CGEventSource.h>
    #include <CoreGraphics/CoreGraphics.h>
#endif

namespace mousemac {
    
    void sendMouseEvent(int buttonEvent, float posX, float posY){
 #if JUCE_MAC
        CGPoint pt;
        pt.x = posX;
        pt.y = posY;
        
        const auto button = (buttonEvent == kCGEventRightMouseDown || buttonEvent == kCGEventRightMouseUp)
                            ? kCGMouseButtonRight
                            : (buttonEvent == kCGEventOtherMouseDown || buttonEvent == kCGEventOtherMouseUp)
                              ? kCGMouseButtonCenter : kCGMouseButtonLeft;
        CGEventRef mouseDownEv = CGEventCreateMouseEvent (NULL,(CGEventType)buttonEvent,pt,button);
        if (mouseDownEv != nullptr)
        {
            CGEventPost (kCGHIDEventTap, mouseDownEv);
            CFRelease(mouseDownEv);
        }
#endif
        
    }
    
    void setMousePos(float posX, float posY)
    {
 #if JUCE_MAC
        CGPoint pt;
        pt.x = posX;
        pt.y = posY;
        
        CGEventRef moveEvent = CGEventCreateMouseEvent(
                                                       NULL,               // NULL to create a new event
                                                       kCGEventMouseMoved, // what type of event (move)
                                                       pt,                 // screen coordinate for the event
                                                       kCGMouseButtonLeft  // irrelevant for a move event
                                                       );
        
        // post the event and cleanup
        CGEventPost(kCGSessionEventTap, moveEvent);
        CFRelease(moveEvent);
#endif
        
    }

    void sendScrollWheelEvent(int32 scrollX, int32 scrollY) {
#if JUCE_MAC
        if (scrollX == 0 && scrollY == 0) return;
        CGEventRef scrollEvent = CGEventCreateScrollWheelEvent(nullptr, kCGScrollEventUnitLine, 2, scrollY, scrollX);
        if (scrollEvent != nullptr)
        {
            CGEventPost(kCGHIDEventTap, scrollEvent);
            CFRelease(scrollEvent);
        }
#endif

    }
}
