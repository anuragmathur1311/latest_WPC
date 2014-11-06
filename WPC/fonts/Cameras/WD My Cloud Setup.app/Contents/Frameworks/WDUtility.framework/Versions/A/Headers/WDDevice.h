//
//  WDDevice.h
//  DriveUnlock
//
//  Created by Bob on 3/11/09.
//  Copyright © 2014 Western Digital Technologies, Inc. All rights reserved.
//

#import <IOKit/IOCFPlugIn.h>
#import <IOKit/scsi/SCSITaskLib.h>
#import "WDDMInfoController.h"
#import <DiskArbitration/DiskArbitration.h>

//enum {
//	kDeviceSNLength	= 17,		// Length of device serial number
//	kFRUSNLength	= 21,		// Length of each of the FRU serial numbers
//	kTotalSNLength	= 59,		// Length of device and 2 FRU serial numbers 
//	kNameLength		= 256		// Name length
//};

#define kInterlakenVCDImage		@"vcdIsoName"

typedef enum  {
    kUSB,
    kFW,
    kTB
}interfaceType;

//typedef struct 
//{
//	bool	eraseInProgress;
//	double	eraseProgressPercentage;
//} statusBlock;

@interface WDDeviceEraseProgress : NSObject {
	bool	eraseInProgress;
	double	percentageComplete;
    bool    cancelEraseDrive;
}

@property (nonatomic, readwrite, assign)	bool	eraseInProgress;
@property (nonatomic, readwrite, assign)	double	percentageComplete;
@property (readwrite)                       bool    cancelEraseDrive;
@end

@class WDDisk;

@interface WDDevice : NSObject {
	io_service_t				IOService;
    io_service_t                mUSBPluginNub;
	io_service_t				mSESPluginNub;              // IOCreatePlugInInterfaceForService
	io_service_t				mCDPluginNub;
	io_service_t				AppleAHCInterface;			// SWRAID - Start the search for disk drives here.
    BOOL                        deviceOpen;                 // Is the device ready to accept a command?
	id                          pidObject;
    
	IOCFPlugInInterface **		mPlugInIterface;            // Variables to talk to the SES Interface.
	SCSITaskDeviceInterface**	mSCSITaskDevInterface;
	SCSITaskInterface**			mSCSITaskInterface;
	SCSITaskInterface**         mSCSITask;

    
    WDDMInfoController *        mWDDMInfoController;        // set if we have a connection to WDDM.
    BOOL                        mWDDMControlled;            // TRUE if WDDM has this device open and therefore controlling the device.
    
	// Drive information

	UInt64                      mDeviceLunCount;            // number of disk LUN.
	UInt64                      mDeviceSize;                // From IOKit IOMedia class Size parameter.
	UInt64                      mCDDeviceSize;              // From IOKit IOCDMedia class Size parameter.
    UInt64                      mRawDeviceCapacity;         // Device Capacity from Inquiry Raw Capacity VPD page.
	NSString *					mSerialNumber;              // For USB device; Initally from IOKit IOUSBDevice USB Serial Number field. Updated
                                                            // later for USB and FW with Inquiry Serial Number Page.
	NSString *					mDeviceBSDName;             // From IOKit IOMedia class BSD Name. May not be there if device is locked. Must be set to "unknown" if not there.
	NSString *					mDeviceCDBSDName;           // From IOKit IOCDMedia class BSD Name. May not be there if VCD is disabled. Must be set to "unknown" if not there.
    NSString *                  mProductName;               // USB Product Name for USB devices, Product ID for 1394
	UInt32                      mProductID;                 // From IOKit IOUSBDevice product ID of the device.
	UInt16                      mFWVersion;                 // From IOKit IOUSBDevice bcdDevice field.
    interfaceType               mInterfaceType;             // Initer face type of device. (defined above)
	NSMutableArray *			mDisks;                     // Array of WDDisk's in a device.
	NSMutableArray *			mLuns;                      // Array of Disk Luns in a device.
    int                         mNumberOfSlots;             // Number of physical slots in a device.
	NSLock *					mdeviceLock;                // Should we use @synchronized instead???
    NSString *                  mPartitionScheme;
    BOOL                        mMultiDriveSupport;         // Are multiple drives supported on this device.
    
    //VCD variables
    NSString *                  mVcdIsoPath;
    NSString *                  mVcdIsoVersion;
    int                         mVcdIsoSize;
    
    NSProgressIndicator*        progressIndicator;
	NSWindow*                   window;
	NSTextField*                progressMsg;
    BOOL                        abortUpdateVCD;
    int                         vcdUpdateMode;

}

@property (readwrite,assign) io_service_t           IOService;
@property (readwrite,assign) io_service_t           mUSBPluginNub;
@property (readwrite,assign) io_service_t           mSESPluginNub;
@property (readwrite,assign) io_service_t           mCDPluginNub;
@property (readwrite,assign) io_service_t			AppleAHCInterface;

@property (readwrite,assign) UInt64                 mDeviceLunCount;
@property (readwrite,assign) UInt64                 mDeviceSize;
@property (readwrite,assign) UInt64                 mCDDeviceSize;
@property (readwrite,assign) UInt64                 mRawDeviceCapacity;
@property (readwrite,copy)	NSString *				mSerialNumber;
@property (readwrite,copy)	NSString *				mDeviceBSDName;
@property (readwrite,copy)	NSString *				mDeviceCDBSDName;
@property (readwrite,copy)  NSString *              mProductName;
@property (readwrite,assign) UInt32                 mProductID;
@property (readwrite,assign) UInt16                 mFWVersion;
@property (readwrite,assign) interfaceType          mInterfaceType;
@property (readwrite,retain) NSMutableArray *		mDisks;
@property (readwrite,retain) NSMutableArray *		mLuns;
@property (readwrite,assign) int                    mNumberOfSlots;
@property (readwrite, copy)  NSString *             mPartitionScheme;
@property (readwrite,assign) WDDMInfoController *   mWDDMInfoController;
@property (readwrite,assign) BOOL                   mWDDMControlled;
@property (readwrite,assign) BOOL                   mMultiDriveSupport;
@property (readwrite,copy)  NSString *              mVcdIsoPath;

+ (void)printClassName:(io_object_t)classInstance;

-(NSDictionary *) deviceDictionary;

- (BOOL) writeHFSFormat: (bycopy NSString*) driveNode volumeName: (bycopy NSString*) volumeName;
- (void) dismount;
- (void) dismountVCD;
- (int) mountVCD;
- (void) eject;
- (void) ejectVCD;
- (void) writeZerosToDisk: (WDDeviceEraseProgress *) progressBlock;
- (int) dismountDisk: (id)sender;
- (UInt8) devicePrimaryLunSelection;
- (UInt8) deviceSecondaryLunSelection;
- (UInt8) deviceSecondaryLunSelectionForSlots;
- (void) deviceLock;
- (void) deviceUnLock;
- (void) enableWDDMSupport;
- (NSString*) isoFileType;

+ (NSString*) volumeFromBSDName:(NSString*)bsdName;
+ (NSArray*) volumesFromBSDName:(NSString*)bsdName;
- (NSArray*) writableMountedVolumeNames;
- (NSArray*) writableMountedVolumePaths;
- (NSArray*) writableMountedVolumePathsForDisk:(WDDisk*)theDisk;
- (NSArray*) writableMountedVolumesForDisk:(WDDisk*)theDisk;
- (NSArray*) writableVolumeNames;
- (NSArray*) writableVolumePaths;
- (NSArray*) writableVolumePathsForDisk:(WDDisk*)theDisk;
- (NSArray*) writableVolumesForDisk:(WDDisk*)theDisk;

- (io_service_t)populateDeviceInformation: (WDDevice *) device;
- (io_service_t)populateDriveInformation;

+(int) setCustomIcon: (NSString *) iconName OnVolume: (NSString *) bsdName;
void SafeRemovalCallback( DADiskRef disk, DADissenterRef dissenter, void * context );

@end
