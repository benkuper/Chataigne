#import <Foundation/Foundation.h>
#import <objc/runtime.h>
#import "os_mac.h"
// Include discovery to test its exact Nintendo name filter and capacity bounds.
#import "../../../Source/Module/modules/controller/wiimote/lib/wiiuse/os_mac/os_mac_find.m"

static int notificationsReleased = 0, notificationsUnregistered = 0, delegatesCleared = 0;
static BOOL noAdapter = NO, neverComplete = NO;
static NSArray* discoveryDevices;
@interface FakeNotification : NSObject
- (void)unregister;
@end
@implementation FakeNotification
- (void)unregister { ++notificationsUnregistered; }
- (void)dealloc { ++notificationsReleased; [super dealloc]; }
@end
@interface FakeChannel : NSObject
- (BluetoothL2CAPPSM)PSM;
- (IOReturn)setDelegate:(id)delegate;
- (IOReturn)closeChannel;
- (IOReturn)writeSync:(void*)bytes length:(UInt16)length;
@end
@implementation FakeChannel
- (BluetoothL2CAPPSM)PSM { return kBluetoothL2CAPPSMHIDInterrupt; }
- (IOReturn)setDelegate:(id)delegate { if (!delegate) ++delegatesCleared; return kIOReturnSuccess; }
- (IOReturn)closeChannel { return kIOReturnSuccess; }
- (IOReturn)writeSync:(void*)bytes length:(UInt16)length { return kIOReturnSuccess; }
@end
@interface FakeDevice : NSObject { NSString* deviceName; }
- (id)initWithName:(NSString*)name;
- (NSString*)name;
@end
@implementation FakeDevice
- (id)initWithName:(NSString*)name { if ((self = [super init])) deviceName = [name copy]; return self; }
- (NSString*)name { return deviceName; }
- (IOReturn)openL2CAPChannelSync:(IOBluetoothL2CAPChannel**)channel withPSM:(BluetoothL2CAPPSM)psm delegate:(id)delegate {
    *channel = (id)[[[FakeChannel alloc] init] autorelease]; return kIOReturnSuccess;
}
- (id)registerForDisconnectNotification:(id)observer selector:(SEL)selector { return [[[FakeNotification alloc] init] autorelease]; }
- (IOReturn)closeConnection { return kIOReturnSuccess; }
- (void)dealloc { [deviceName release]; [super dealloc]; }
@end
@interface FakeHost : NSObject
@end
@implementation FakeHost
- (BluetoothHCIPowerState)powerState { return kBluetoothHCIPowerStateON; }
@end
@interface FakeInquiry : NSObject { id inquiryDelegate; }
- (id)initWithDelegate:(id)delegate;
@end
@implementation FakeInquiry
- (id)initWithDelegate:(id)delegate { if ((self = [super init])) inquiryDelegate = delegate; return self; }
- (void)setSearchCriteria:(BluetoothServiceClassMajor)service majorDeviceClass:(BluetoothDeviceClassMajor)major minorDeviceClass:(BluetoothDeviceClassMinor)minor {}
- (void)setUpdateNewDeviceNames:(BOOL)value {}
- (void)setInquiryLength:(uint8_t)value {}
- (IOReturn)start {
    if (!neverComplete) [inquiryDelegate deviceInquiryComplete:(id)self error:kIOReturnSuccess aborted:NO];
    return kIOReturnSuccess;
}
- (IOReturn)stop { return kIOReturnSuccess; }
- (void)setDelegate:(id)delegate { inquiryDelegate = delegate; }
- (NSArray*)foundDevices { return discoveryDevices; }
@end
static id fakeDefaultController(id object, SEL selector) { return noAdapter ? nil : [[[FakeHost alloc] init] autorelease]; }
static id fakeInquiry(id object, SEL selector, id delegate) { return [[[FakeInquiry alloc] initWithDelegate:delegate] autorelease]; }
@interface WiiuseWiimote (TestQueue)
- (int)checkForAvailableDataForBuffer:(byte*)buffer length:(NSUInteger)length;
@end
static void check(BOOL condition, const char* message) { if (!condition) { fprintf(stderr, "%s\n", message); abort(); } }
int main(void) {
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    method_setImplementation(class_getClassMethod([IOBluetoothHostController class], @selector(defaultController)), (IMP)fakeDefaultController);
    method_setImplementation(class_getClassMethod([IOBluetoothDeviceInquiry class], @selector(inquiryWithDelegate:)), (IMP)fakeInquiry);
    NSMutableArray* devices = [NSMutableArray array];
    for (NSString* name in @[@"Keyboard", @"Nintendo RVL-CNT-01", @"Mouse", @"Nintendo RVL-CNT-01-TR", @"Nintendo RVL-CNT-01", @"Nintendo RVL-CNT-01", @"Nintendo RVL-CNT-01"])
        [devices addObject:[[[FakeDevice alloc] initWithName:name] autorelease]];
    discoveryDevices = devices;
    check(!isWiiRemote((id)[devices objectAtIndex:0]), "Non-Wii HID device passed filter");
    wiimote** wm = wiiuse_init(4);
    wiiuse_os_reset_discovery();
    check(wiiuse_os_find(wm, 4, 1) == 4, "Discovery must filter and cap results at allocated slots");
    check(wm[0]->type == WIIUSE_WIIMOTE_REGULAR && wm[1]->type == WIIUSE_WIIMOTE_MOTION_PLUS_INSIDE, "Original/Plus Remote identification failed");
    wiiuse_cleanup(wm, 4);
    noAdapter = YES; wm = wiiuse_init(1);
    check(wiiuse_os_find(wm, 1, 1) == 0, "No adapter must return safely");
    noAdapter = NO; wiiuse_os_cancel_discovery();
    check(wiiuse_os_find(wm, 1, 1) == 0, "Cancelled discovery continued");
    wiiuse_os_reset_discovery(); neverComplete = YES;
    NSDate* start = [NSDate date];
    check(wiiuse_os_find(wm, 1, 1) == 1, "Bounded inquiry should collect its available result");
    check(-[start timeIntervalSinceNow] < 4, "Discovery without completion callback hung");
    wiiuse_cleanup(wm, 1);
    neverComplete = NO;
    for (int iteration = 0; iteration < 100; ++iteration) {
        wm = wiiuse_init(1);
        FakeDevice* device = [[FakeDevice alloc] initWithName:@"Nintendo RVL-CNT-01"];
        WiiuseWiimote* native = [[WiiuseWiimote alloc] initWithPtr:wm[0] device:(id)device];
        NSAutoreleasePool* connectionPool = [[NSAutoreleasePool alloc] init];
        check([native connect] == kIOReturnSuccess, "Fake L2CAP connect failed");
        [connectionPool drain]; // Notification must still be retained by native.
        byte buffer[MAX_PAYLOAD] = {0};
        byte packet[MAX_PAYLOAD + 2] = {0xA1, 0x30, 0x01};
        FakeChannel* channel = [[[FakeChannel alloc] init] autorelease];
        [native l2capChannelData:(id)channel data:packet length:0];
        [native l2capChannelData:(id)channel data:packet length:1];
        [native l2capChannelData:(id)channel data:packet length:sizeof(packet)];
        check([native checkForAvailableDataForBuffer:buffer length:sizeof(buffer)] == -1, "Invalid packet entered queue");
        [native l2capChannelData:(id)channel data:packet length:3];
        check([native checkForAvailableDataForBuffer:buffer length:sizeof(buffer)] == 2 && buffer[0] == 0x30, "Valid packet lost");
        for (int i = 0; i < 1000; ++i) [native l2capChannelData:(id)channel data:packet length:3];
        int queued = 0;
        while ([native checkForAvailableDataForBuffer:buffer length:sizeof(buffer)] >= 0) ++queued;
        check(queued == 256, "Input queue must be bounded");
        check([native writeReport:0x12 buffer:packet length:MAX_PAYLOAD] == 0, "Oversized write accepted");
        [native disconnect]; [native disconnect]; [native release]; [device release];
        wiiuse_cleanup(wm, 1);
    }
    check(notificationsReleased == 100 && notificationsUnregistered == 100 && delegatesCleared == 200, "L2CAP delegates/notifications leaked or released twice");
    [pool drain];
    puts("Wii discovery filtering/bounds/cancellation, no adapter, packet validation, queue bound and native teardown passed");
    return 0;
}
