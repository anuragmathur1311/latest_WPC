//
//  WDVolume.h
//  WDUtility
//
//  Created by bob hahn on 3/5/12.
//  Copyright © 2014 Western Digital Technologies, Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "WDDisk.h"

typedef enum 
{
	unknown,
	HFSPlus,
	ExFat,
    NTFS
} volumeType;

@interface WDVolume : NSObject
{
    io_service_t    _service;
    WDDisk *        _disk;                      // Service Object for the disk this volume is on.
}

@property (nonatomic, readonly, assign)     io_service_t service;
@property (nonatomic, readonly, assign)     WDDisk * disk;

- (WDVolume *) initWithVolumeName: (char *) objectName withProperties: (NSDictionary *)objectProperties onDisk: (WDDisk *) diskDevice;

- (NSString *) mediaName;
- (NSString *) volumeName;
- (NSURL *) volumePath;
- (NSString *) bsdName;
- (UInt64) size;
- (NSString *) uuid;
- (NSString *) partType;
- (BOOL) writeable;
- (BOOL) mountable;

+(int) setCustomIcon: (NSString *) iconName OnVolume: (NSString *) bsdName;

@end
