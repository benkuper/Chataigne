/*
 *  wiiuse
 *
 *  Written By:
 *    Michael Laforest  < para >
 *    Email: < thepara (--AT--) g m a i l [--DOT--] com >
 *
 *  Copyright 2006-2007
 *
 *  This file is part of wiiuse.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  $Header$
 *
 */

/**
 *  @file
 *  @brief Handles device I/O for Mac OS X.
 */

#ifdef __APPLE__

#import "os_mac.h"

#import "io.h"
#import "events.h"
//#import "os.h"

#import <IOBluetooth/IOBluetoothUtilities.h>
#import <IOBluetooth/objc/IOBluetoothDevice.h>
#import <IOBluetooth/objc/IOBluetoothHostController.h>
#import <IOBluetooth/objc/IOBluetoothDeviceInquiry.h>
#if !WIIUSE_MAC_OS_X_VERSION_10_7_OR_ABOVE
#import <IOBluetooth/IOBluetoothUserLib.h> // IOBluetoothLocalDeviceGetPowerState
#endif


// Discovery and all native delegates stay on the WiiUse worker run loop.
#include <stdbool.h>
#include <stdatomic.h>
static atomic_bool discoveryCancelled = false;
void wiiuse_os_cancel_discovery(void) { atomic_store(&discoveryCancelled, true); }
void wiiuse_os_reset_discovery(void) { atomic_store(&discoveryCancelled, false); }

static BOOL isWiiRemote(IOBluetoothDevice* device) {
    NSString* name = [device name];
    return [name isEqualToString:@"Nintendo RVL-CNT-01"] ||
           [name isEqualToString:@"Nintendo RVL-CNT-01-TR"];
}

@interface WiiuseDeviceInquiry : NSObject<IOBluetoothDeviceInquiryDelegate> {
    BOOL complete;
    NSUInteger found;
    NSUInteger capacity;
}
- (int) find:(wiimote**)wiimotes capacity:(int)count timeout:(int)seconds;
@end

@implementation WiiuseDeviceInquiry
- (int) find:(wiimote**)wiimotes capacity:(int)count timeout:(int)seconds {
    if (!wiimotes || count <= 0 || atomic_load(&discoveryCancelled)) return 0;
    IOBluetoothHostController* controller = [IOBluetoothHostController defaultController];
    if (!controller || [controller powerState] != kBluetoothHCIPowerStateON) {
        WIIUSE_WARNING("Bluetooth is unavailable or switched off.");
        return 0;
    }
    capacity = count;
    found = 0;
    complete = NO;
    IOBluetoothDeviceInquiry* inquiry = [[IOBluetoothDeviceInquiry inquiryWithDelegate:self] retain];
    [inquiry setSearchCriteria:kBluetoothServiceClassMajorAny
             majorDeviceClass:WM_DEV_MAJOR_CLASS minorDeviceClass:kBluetoothDeviceClassMinorAny];
    // Names are needed to distinguish Wii Remotes from keyboards and other HID devices.
    [inquiry setUpdateNewDeviceNames:YES];
    [inquiry setInquiryLength:MAX(1, MIN(30, seconds))];
    int result = 0;
    IOReturn status = [inquiry start];
    if (status == kIOReturnSuccess) {
        NSDate* deadline = [NSDate dateWithTimeIntervalSinceNow:MAX(1, MIN(30, seconds)) + 2];
        while (!complete && !atomic_load(&discoveryCancelled) && [deadline timeIntervalSinceNow] > 0) {
            if (![[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode
                                         beforeDate:[NSDate dateWithTimeIntervalSinceNow:.05]])
                [NSThread sleepForTimeInterval:.01];
        }
        [inquiry stop];
        // Stop callbacks before this delegate and the destination array go away.
        [inquiry setDelegate:nil];
        if (!atomic_load(&discoveryCancelled)) {
            for (IOBluetoothDevice* device in [inquiry foundDevices]) {
                if (result >= count) break;
                if (!isWiiRemote(device) || !wiimotes[result]) continue;
                wiimote* wm = wiimotes[result];
                wm->type = [[device name] hasSuffix:@"-TR"] ? WIIUSE_WIIMOTE_MOTION_PLUS_INSIDE : WIIUSE_WIIMOTE_REGULAR;
                wm->objc_wm = [[WiiuseWiimote alloc] initWithPtr:wm device:device];
                WIIMOTE_ENABLE_STATE(wm, WIIMOTE_STATE_DEV_FOUND);
                ++result;
            }
        }
    } else {
        WIIUSE_ERROR("Unable to start Bluetooth discovery (%d). Check Bluetooth permission in System Settings.", status);
        [inquiry setDelegate:nil];
    }
    [inquiry release];
    return result;
}
- (void) deviceInquiryDeviceFound:(IOBluetoothDeviceInquiry*)inquiry device:(IOBluetoothDevice*)device {
    if (isWiiRemote(device) && ++found >= capacity) complete = YES;
}
- (void) deviceInquiryComplete:(IOBluetoothDeviceInquiry*)inquiry error:(IOReturn)error aborted:(BOOL)aborted {
    complete = YES;
    if (error != kIOReturnSuccess && !aborted)
        WIIUSE_WARNING("Bluetooth inquiry failed (%d).", error);
}
@end

int wiiuse_os_find(struct wiimote_t** wm, int count, int timeout) {
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    WiiuseDeviceInquiry* inquiry = [[WiiuseDeviceInquiry alloc] init];
    int result = [inquiry find:wm capacity:count timeout:timeout];
    [inquiry release];
    [pool drain];
    return result;
}
#endif // __APPLE__
