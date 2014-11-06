//
//  WDDevices.h
//  WDUtility
//
//  Created by Bob Hahn on 3/18/11.
//  Copyright © 2014 Western Digital Technologies, Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <DiskArbitration/DiskArbitration.h>

@protocol WDDevicesDelegate;

@interface WDDevices : NSObject
{
    id <WDDevicesDelegate>delegate;
	id pidObject;

    NSMutableDictionary *       mDevices;
    DASessionRef				mDASession;
    DAApprovalSessionRef		mDAApprovalSession;
    

    /////////////////// NAS /////////////////////

    //    NASDiscovery* bjDiscoveryObject;
    NSMutableDictionary* dictOfNASDevices;
    NSArray* sortedKeysOfDictOfNASDevices;
    NSArray* supportedPIDs;
	NSLock *					dasDevicesLock;

    io_iterator_t   usbDeviceAddedNotification;
    io_iterator_t   usbDeviceRemovedNotification;
    io_iterator_t   usbDriveAddedNotification;
    io_iterator_t   usbDriveRemovedNotification;
    io_iterator_t   usbMediaAppearedNotification;

    NSArray * firewireSupportedPIDS;
    io_iterator_t   fwDeviceAddedNotification;
    io_iterator_t   fwDeviceRemovedNotification;
    io_iterator_t   fwDriveAddedNotification;
    io_iterator_t   fwDriveRemovedNotification;

    io_iterator_t   fwMediaAppearedNotification;
}

@property (readwrite, copy) NSMutableDictionary * mDevices;
//@property (readwrite, copy) NSMutableDictionary * dictOfSupportedDevices;
//@property (readwrite, copy) NSMutableDictionary * dictOfUnsupportedDevices;
//@property (readwrite, copy) NSMutableDictionary * dictOfVCDDevices;

@property (readwrite, copy) NSArray* sortedKeysOfDictOfNASDevices, *supportedPIDs;
@property (nonatomic, assign)   id <WDDevicesDelegate>delegate;

@property (nonatomic, assign)   io_iterator_t usbDeviceAddedNotification;
@property (nonatomic, assign)   io_iterator_t usbDeviceRemovedNotification;
@property (nonatomic, assign)   io_iterator_t usbMediaAppearedNotification;

@property (nonatomic, assign)   io_iterator_t fwDeviceAddedNotification;
@property (nonatomic, assign)   io_iterator_t fwDeviceRemovedNotification;
@property (nonatomic, assign)   io_iterator_t fwDriveAddedNotification;
@property (nonatomic, assign)   io_iterator_t fwDriveRemovedNotification;
@property (nonatomic, assign)   io_iterator_t fwMediaAppearedNotification;

+ (WDDevices*) instance;
+ (void)printClassName:(io_object_t)classInstance;

- (NSArray *)	getSupportedPIDSArray;
- (NSArray *)   getSupportedEncryptedPIDSArray;
- (NSArray *)   getSupportedEncryptedOnyxPIDsValue;

- (void) setDiskAppearanceCallbackToMountableVolumes;
- (void) setDiskAppearanceCallbackToWholeMedia;

- (BOOL) findDevices: (NSArray *)devicePids;

- (NSDictionary *) deviceDictionaries;
- (void) setDelegate: (id<WDDevicesDelegate>) object;

- (void) diskClaim:(DADiskRef) disk;
- (void) diskUnclaim:(DADiskRef) disk;
- (bool) isRAIDDevice: (NSString *) bsdName;
- (bool)isMultiDriveDevice: (NSString *) bsdName;

void UnrecognizedDiskPeekCallback(DADiskRef disk, void *context);
void DiskAppearedCallback(DADiskRef disk, void *context);
void DriveDisappearedCallback(DADiskRef disk, void *context);
void diskMountCallback( DADiskRef disk, DADissenterRef dissenter, void *context);
void unmountCallback(DADiskRef disk, DADissenterRef dissenter, void *context);

DADissenterRef mountApprovalCallback(DADiskRef disk, void *context);
void DiskClaimed(DADiskRef disk, DADissenterRef dissenter, void *context );

@end

@protocol WDDevicesDelegate <NSObject>
@optional
- (void)UnrecognizedDiskPeek:(DADiskRef)disk;
- (void)DiskAppeared:(DADiskRef)disk;
- (void)driveAppeared: (DADiskRef) disk;
- (void)MountableVolumeAppeared: (DADiskRef) disk;
- (void)UnrecognizedVolumeAppeared: (DADiskRef) disk;
- (void)VolumeAppeared:(DADiskRef)disk;
- (void)DeviceAppeared:(id) device;
- (void)DeviceDisappeared:(id) device;
- (void)DriveDisappeared:(DADiskRef)disk;
- (void)DiskClaimed:(DADiskRef)disk;
- (DADissenterRef)DiskClaimRelease:(DADiskRef)disk;
- (DADissenterRef)mountApproval: (DADiskRef) disk;
- (void)diskMount:(DADiskRef)disk;
- (void)lockDevices;
- (void)unlockDevices;

@end