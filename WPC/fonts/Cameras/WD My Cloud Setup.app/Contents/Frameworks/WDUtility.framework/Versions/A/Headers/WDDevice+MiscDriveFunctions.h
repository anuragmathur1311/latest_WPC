//
//  WDDevice+MiscDriveFunctions.h
//  WDUtility
//
//  Created by Bob Hahn on 3/8/13.
//
//

#import <Cocoa/Cocoa.h>
#import "WDDevice.h"

typedef struct __attribute__((packed)){
#ifdef __LITTLE_ENDIAN__
	UInt16			ModeDataLength;
	UInt8			MediumType;
	UInt8			DeviceSpecificParameter;
	UInt8			LongLBA:1;
	UInt8			reserved:7;
	UInt8			reserved1;
	UInt16			BlockDescriptorLength;
#else
	UInt16			ModeDataLength;
	UInt8			MediumType;
	UInt8			DeviceSpecificParameter;
	UInt8			reserved:7;
	UInt8			LongLBA:1;
	UInt8			reserved1;
	UInt16			BlockDescriptorLength;
#endif

} MODE_PARAMERTER_HEADER_10;

typedef struct __attribute__((packed)){
	UInt64			numLogicalBlocks;
	UInt8			reserved[4];
	UInt32			logicalBlockSize;
} MODE_PARAMERTER_BLOCK_DESCRIPTOR_LONG;

typedef struct __attribute__((packed)){
	UInt32			numLogicalBlocks;
	UInt32			logicalBlockSize;
} MODE_PARAMERTER_BLOCK_DESCRIPTOR_SHORT;

@interface WDDevice (MiscDriveFunctions)

- (int) getDeviceSectorSize:(NSMutableDictionary **)errorDict;
- (int) setDeviceSectorSize: (int)sectorSize error:(NSMutableDictionary **)errorDict;
- (BOOL) isU1U2Changable:(NSMutableDictionary **)errorDict;
- (BOOL) isU1U2Disabled:(NSMutableDictionary **)errorDict;
- (int) setU1U2Disabled: (BOOL) enable;
- (BOOL) isUASChangable:(NSMutableDictionary **)errorDict;
- (BOOL) isUASDisabled:(NSMutableDictionary **)errorDict;
- (int) setUASDisabled: (BOOL) enable;
- (BOOL) reEnumerateUSBDevice;

@end
