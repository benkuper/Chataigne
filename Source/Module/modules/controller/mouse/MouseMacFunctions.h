/*
  ==============================================================================

    MouseMacFunctions.h
    Created: 13 Mar 2020 12:09:00pm
    Author:  Ben Kuper

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#if JUCE_SUPPORT_CARBON
    #include <CoreGraphics/CGEventSource.h>
    #include <CoreGraphics/CoreGraphics.h>
#endif

namespace mousemac {
    
    void sendMouseEvent(int buttonEvent, float posX, float posY){
 #if JUCE_SUPPORT_CARBON
        CGPoint pt;
        pt.x = posX;
        pt.y = posY;
        
        CGEventRef mouseDownEv = CGEventCreateMouseEvent (NULL,(CGEventType)buttonEvent,pt,kCGMouseButtonLeft);
        CGEventPost (kCGHIDEventTap, mouseDownEv);
#endif
        
    }
    
    void setMousePos(float posX, float posY)
    {
 #if JUCE_SUPPORT_CARBON
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
}
