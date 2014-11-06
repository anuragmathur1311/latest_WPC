//
//  WDDisk.h
//  WDUtility
//
//  Created by bob hahn on 7/26/11.
//  Copyright © 2014 Western Digital Technologies, Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <IOKit/storage/ata/ATASMARTLib.h>
#include <DiskArbitration/DiskArbitration.h>

@interface WDDiskEraseProgress : NSObject {
	bool	eraseInProgress;
	double	percentageComplete;
}

@property (nonatomic, readwrite, assign)	bool	eraseInProgress;
@property (nonatomic, readwrite, assign)	double	percentageComplete;

@end

// void SafeRemovalCallback( DADiskRef disk, DADissenterRef dissenter, void *dismountStatus);

@interface WDDisk : NSObject {
    io_service_t    IOServiceObject;
    io_service_t    IODeviceServiceObject;
    UInt8           _peripheralDeviceType;
	NSString *		_serialNumber;
    NSString *      _modelName;
    NSString *		_bsdName;
    UInt64			_diskSize;
	NSMutableArray *_volumes;
    int             _slotNumber;

    io_service_t				mSmartLibPluginNub;
	IOCFPlugInInterface **		mPlugInIterface;
	IOATASMARTInterface **		mSMARTTaskDevInterface;
	NSLock						*mWDDiskLock;
	
	BOOL						mdiskOpen;
}

@property (readwrite, assign) io_service_t  IOServiceObject;
@property (readwrite, assign) io_service_t  IODeviceServiceObject;
@property (readwrite, assign) UInt8         peripheralDeviceType;
@property (readwrite,copy) NSString *		serialNumber;
@property (readwrite,copy) NSString *		modelName;
@property (readwrite,copy) NSString *		bsdName;
@property (readwrite,assign) UInt64         diskSize;
@property (readwrite,retain) NSMutableArray *	volumes;
@property (readwrite,assign) int            slotNumber;

- (id) initWithServiceObject: (io_service_t) diskServiceObject;
- (void) initalizeDiskWithVolumeName: (NSString *)volumeName format: (NSString *)volumeFormat;
- (void) updateDiskInformation;
- (int) dismountDisk: (id)sender;
- (void) writeZerosToDisk: (WDDiskEraseProgress *) progressBlock;
@end
