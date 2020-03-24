/*
  ==============================================================================

    KeyboardMacFunctions.h
    Created: 13 Mar 2020 12:09:11pm
    Author:  Ben Kuper

  ==============================================================================
*/

#pragma once

#if JUCE_SUPPORT_CARBON
#include <CoreGraphics/CGEventSource.h>
#include <CoreGraphics/CoreGraphics.h>
#endif

namespace keyboardmac {
    
    void sendMacKeyEvent(int keyID, bool isDown){
#if JUCE_SUPPORT_CARBON
        CGEventSourceRef source = CGEventSourceCreate( kCGEventSourceStateHIDSystemState );

        CGEventRef keyEvent = CGEventCreateKeyboardEvent( source, (CGKeyCode) keyID, isDown );
        CGEventPost( kCGAnnotatedSessionEventTap, keyEvent );
        CFRelease( keyEvent );

        CFRelease( source );
#endif
    }
}
