#import <Foundation/Foundation.h>
#import <objc/runtime.h>
#import "os_mac.h"
// Include discovery to test its exact Nintendo name filter and capacity bounds.
#import "../../../Source/Module/modules/controller/wiimote/lib/wiiuse/os_mac/os_mac_find.m"

static int notificationsReleased = 0, notificationsUnregistered = 0, delegatesCleared = 0;
static BOOL noAdapter = NO, neverComplete = NO, failWrite = NO;
static NSArray* discoveryDevices;
static BOOL delayedInquiry = NO, inquiryCompleted = NO, pairingRequired = NO, pairingAuthorized = NO;
static int channelOpens = 0, channelCloses = 0, deviceCloses = 0, failOpens = 0;
static BluetoothL2CAPPSM failingPSM = 0;
@interface FakeNotification : NSObject
- (void)unregister;
@end
@implementation FakeNotification
- (void)unregister { ++notificationsUnregistered; [self release]; }
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
- (IOReturn)closeChannel { ++channelCloses; return kIOReturnSuccess; }
- (IOReturn)writeSync:(void*)bytes length:(UInt16)length { return failWrite ? kIOReturnError : kIOReturnSuccess; }
@end
@interface FakeDevice : NSObject { NSString* deviceName; }
- (id)initWithName:(NSString*)name;
- (NSString*)name;
@end
@implementation FakeDevice
- (id)initWithName:(NSString*)name { if ((self = [super init])) deviceName = [name copy]; return self; }
- (NSString*)name { return deviceName; }
- (IOReturn)openL2CAPChannelSync:(IOBluetoothL2CAPChannel**)channel withPSM:(BluetoothL2CAPPSM)psm delegate:(id)delegate {
    ++channelOpens;
    if (pairingRequired && !pairingAuthorized) return kIOReturnNotPermitted;
    if (psm == failingPSM && failOpens > 0) { --failOpens; return kIOReturnNotPermitted; }
    *channel = (id)[[[FakeChannel alloc] init] autorelease]; return kIOReturnSuccess;
}
- (id)registerForDisconnectNotification:(id)observer selector:(SEL)selector { return [[FakeNotification alloc] init]; }
- (IOReturn)closeConnection { ++deviceCloses; return kIOReturnSuccess; }
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
- (void)completeInquiry {
    inquiryCompleted = YES;
    [inquiryDelegate deviceInquiryComplete:(id)self error:kIOReturnSuccess aborted:NO];
}
- (IOReturn)start {
    inquiryCompleted = NO;
    if (delayedInquiry) {
        if ([inquiryDelegate respondsToSelector:@selector(deviceInquiryDeviceFound:device:)])
            [inquiryDelegate deviceInquiryDeviceFound:(id)self device:[discoveryDevices objectAtIndex:1]];
        [self performSelector:@selector(completeInquiry) withObject:nil afterDelay:.02];
    } else if (!neverComplete) [self completeInquiry];
    return kIOReturnSuccess;
}
- (IOReturn)stop {
    if (inquiryDelegate) { fprintf(stderr, "Inquiry stopped with a live delegate\n"); abort(); }
    return kIOReturnSuccess;
}
- (void)setDelegate:(id)delegate { inquiryDelegate = delegate; }
- (NSArray*)foundDevices { return discoveryDevices; }
@end
@interface FakePairingActions : NSObject
- (void)authorize;
- (void)cancel;
@end
@implementation FakePairingActions
- (void)authorize { pairingAuthorized = YES; }
- (void)cancel { wiiuse_os_cancel_discovery(); }
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
    delayedInquiry = YES; wm = wiiuse_init(1);
    check(wiiuse_os_find(wm, 1, 1) == 1 && inquiryCompleted,
          "Finding a device ended inquiry before native completion/name updates");
    wiiuse_cleanup(wm, 1); delayedInquiry = NO;
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
    wm = wiiuse_init(1);
    byte result[4] = {0};
    struct read_req_t* request = calloc(1, sizeof(struct read_req_t));
    request->addr = 0x1000; request->size = 4; request->wait = 4; request->buf = result;
    wm[0]->read_req = request;
    byte reply[21] = {0};
    reply[2] = 0xF0; reply[3] = 0x0F; reply[4] = 0xFF; // Before buffer, 16 bytes
    propagate_event(wm[0], WM_RPT_READ, reply);
    check(request->wait == 4 && result[0] == 0, "Invalid read offset modified request");
    reply[3] = 0x10; reply[4] = 0; // Correct offset, oversized response
    propagate_event(wm[0], WM_RPT_READ, reply);
    check(request->wait == 4, "Oversized read reply modified request");
    reply[2] = 0x10; reply[5] = 7; reply[6] = 8; // Valid first two bytes
    propagate_event(wm[0], WM_RPT_READ, reply);
    check(request->wait == 2 && result[0] == 7 && result[1] == 8, "Valid read reply lost");
    propagate_event(wm[0], WM_RPT_READ, reply); // Duplicate must not complete it
    check(request->wait == 2, "Duplicate read reply completed pending request");
    wm[0]->read_req = NULL; free(request); wiiuse_cleanup(wm, 1);
    FakePairingActions* actions = [[[FakePairingActions alloc] init] autorelease];
    wm = wiiuse_init(1);
    FakeDevice* pairingDevice = [[[FakeDevice alloc] initWithName:@"Nintendo RVL-CNT-01"] autorelease];
    WiiuseWiimote* pairingNative = [[WiiuseWiimote alloc] initWithPtr:wm[0] device:(id)pairingDevice];
    pairingRequired = YES; pairingAuthorized = NO;
    int opensBefore = channelOpens, closesBefore = deviceCloses;
    [actions performSelector:@selector(authorize) withObject:nil afterDelay:.02];
    check([pairingNative connect] == kIOReturnSuccess && pairingAuthorized,
          "Connection gave up before macOS pairing authorization");
    check(channelOpens >= opensBefore + 3 && deviceCloses == closesBefore,
          "Connection retry discarded the discovered device during pairing");
    [pairingNative disconnect]; [pairingNative release]; wiiuse_cleanup(wm, 1);
    pairingRequired = NO;

    wm = wiiuse_init(1);
    pairingNative = [[WiiuseWiimote alloc] initWithPtr:wm[0] device:(id)pairingDevice];
    failingPSM = kBluetoothL2CAPPSMHIDInterrupt; failOpens = 1;
    opensBefore = channelOpens; closesBefore = channelCloses;
    check([pairingNative connect] == kIOReturnSuccess && channelOpens == opensBefore + 4,
          "Interrupt failure did not reopen both channels");
    check(channelCloses == closesBefore + 1, "Partial control channel was not closed before retry");
    [pairingNative disconnect]; [pairingNative release]; wiiuse_cleanup(wm, 1);
    failingPSM = 0; failOpens = 0;

    wm = wiiuse_init(1);
    pairingNative = [[WiiuseWiimote alloc] initWithPtr:wm[0] device:(id)pairingDevice];
    pairingRequired = YES; pairingAuthorized = NO;
    opensBefore = channelOpens; start = [NSDate date];
    [actions performSelector:@selector(cancel) withObject:nil afterDelay:.02];
    check([pairingNative connect] == kIOReturnAborted && -[start timeIntervalSinceNow] < 2,
          "Pending pairing did not stop promptly on cancellation");
    check(channelOpens == opensBefore + 1, "Cancelled pairing opened another channel");
    [pairingNative disconnect]; [pairingNative release]; wiiuse_cleanup(wm, 1);
    wm = wiiuse_init(1);
    pairingNative = [[WiiuseWiimote alloc] initWithPtr:wm[0] device:(id)pairingDevice];
    check([pairingNative connect] == kIOReturnAborted && channelOpens == opensBefore + 1,
          "Pre-cancelled connection contacted Bluetooth");
    [pairingNative release]; wiiuse_cleanup(wm, 1);
    pairingRequired = NO; wiiuse_os_reset_discovery();
    check(notificationsReleased == 2 && notificationsUnregistered == 2 && delegatesCleared == 5,
          "Pairing retries leaked notifications or partially opened channels");
    notificationsReleased = notificationsUnregistered = delegatesCleared = 0;
    for (int iteration = 0; iteration < 100; ++iteration) {
        wm = wiiuse_init(1);
        FakeDevice* device = [[FakeDevice alloc] initWithName:@"Nintendo RVL-CNT-01"];
        WiiuseWiimote* native = [[WiiuseWiimote alloc] initWithPtr:wm[0] device:(id)device];
        NSAutoreleasePool* connectionPool = [[NSAutoreleasePool alloc] init];
        check([native connect] == kIOReturnSuccess, "Fake L2CAP connect failed");
        [connectionPool drain]; // Registration owns the notification until unregister; native owns its extra retain.
        byte buffer[MAX_PAYLOAD] = {0};
        byte packet[MAX_PAYLOAD + 2] = {0xA1, 0x30, 0x01};
        FakeChannel* channel = [[[FakeChannel alloc] init] autorelease];
        [native l2capChannelData:(id)channel data:packet length:0];
        [native l2capChannelData:(id)channel data:packet length:1];
        [native l2capChannelData:(id)channel data:packet length:3];
        [native l2capChannelData:(id)channel data:packet length:sizeof(packet)];
        check([native checkForAvailableDataForBuffer:buffer length:sizeof(buffer)] == -1, "Invalid packet entered queue");
        [native l2capChannelData:(id)channel data:packet length:4];
        check([native checkForAvailableDataForBuffer:buffer length:sizeof(buffer)] == 3 && buffer[0] == 0x30, "Valid packet lost");
        for (int i = 0; i < 1000; ++i) [native l2capChannelData:(id)channel data:packet length:4];
        int queued = 0;
        while ([native checkForAvailableDataForBuffer:buffer length:sizeof(buffer)] >= 0) ++queued;
        check(queued == 256, "Input queue must be bounded");
        check([native writeReport:0x12 buffer:packet length:MAX_PAYLOAD] == 0, "Oversized write accepted");
        WIIMOTE_ENABLE_STATE(wm[0], WIIMOTE_STATE_CONNECTED);
        if (iteration == 0) {
            failWrite = YES;
            check([native writeReport:0x12 buffer:packet length:1] == 0 && !WIIMOTE_IS_CONNECTED(wm[0]), "Failed writes left a stale connected state");
            failWrite = NO;
        } else {
            [native l2capChannelClosed:[native valueForKey:@"interruptChannel"]];
            check([native checkForAvailableDataForBuffer:buffer length:sizeof(buffer)] == 0 && !WIIMOTE_IS_CONNECTED(wm[0]), "Lost channel left a stale connected state");
        }
        [native disconnect]; [native disconnect]; [native release]; [device release];
        wiiuse_cleanup(wm, 1);
    }
    check(notificationsReleased == 100 && notificationsUnregistered == 100 && delegatesCleared == 201, "L2CAP delegates/notifications leaked or released twice");
    [pool drain];
    puts("Wii inquiry completion, pairing retries/cancellation, filtering/bounds, packet validation and native teardown passed");
    return 0;
}
