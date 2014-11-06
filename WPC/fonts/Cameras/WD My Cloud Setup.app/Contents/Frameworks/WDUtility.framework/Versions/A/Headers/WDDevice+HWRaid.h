//
//  WDDevice+HWRaid.h
//  WDUtility
//
//  Created by Bob Hahn on 10/21/13.
//
//

#import <Foundation/Foundation.h>
#import "WDDevice+SCSICommands.h"

@interface WDDevice (WDDevice_HWRaid)

//
//  RAID Parameters Metadata Byte Layout
//

typedef struct __attribute__((packed))
{
    UInt32  headerMagic;
    UInt32  totalPayloadLengthl;
    UInt16  totalPayloadCRC;
    UInt16  headerCRC;
    UInt32  virtualCDSize;
    UInt32  reserved1;
    UInt16  configuiredByVID;
    UInt16  configuiredByPID;
    UInt32  RAIDGenerationNumber;
    UInt8   RAIDMode;
    UInt8   RAIDStripeSetExponent;
    UInt8   RAIDDiskNumber;
    UInt8   reserved2;
    UInt8   RAIDDisk0HDDsBusType;
    char    RAIDDisk0HDDsModelNumber[40];
    char    RAIDDisk0HDDsSerialNumber[20];
    UInt8   nullTermForRAIDDisk0IDString;
    UInt8   RAIDDisk0Status;
    UInt8   reserved3;
    UInt64  RAIDDisk0UseableCapacity;
    UInt16  RAIDDisk0BlockLength;
    UInt16  RAIDDisk0MediumRotationRate;
    UInt8   reserved4;
    UInt8   reserved5:4;
    UInt8   RAIDDisk0NominalFormFactor;
    UInt8   reserved6;
    UInt8   reserved7;
    UInt8   RAIDDisk1HDDsBusType;
    char    RAIDDisk1HDDsModelNumber[40];
    char    RAIDDisk1HDDsSerialNumber[20];
    UInt8   nullTermForRAIDDisk1IDString;
    UInt8   RAIDDisk1Status;
    UInt8   reserved8;
    UInt64  RAIDDisk1UseableCapacity;
    UInt16  RAIDDisk1BlockLength;
    UInt16  RAIDDisk1MediumRotationRate;
    UInt8   reserved9;
    UInt8   reserved10:4;
    UInt8   RAIDDisk1NominalFormFactor;
    UInt8   reserved11;
    UInt8   reserved12;
    UInt64  arraySize;
} RAID_PARAMETERS_METADATA_BYTE_LAYOUT;

//
//  RAID Staus and Configuration parameter Data
//

//      Metadata Status Values

enum
{
    kMETADATA_STATUS_GOOD               = 0x00,
    kMETADATA_STATUS_CONFLICTINGARRAYS  = 0x01,
    kMETADATA_STATUS_SOMENOTVALID       = 0x02,
    kMETADATA_STATUS_NOTHINGVALID       = 0x03
};

//      Array Mode Values

enum
{
    kARRAY_MODE_STRIPED             = 0x00,
    kARRAY_MODE_MIRRORED            = 0x01,
    ARRAY_MODE_kJBOD                = 0x0F,
    ARRAY_MODE_kNOTCONFIGURED       = 0x10
};

//      Arrary Status Values

enum
{
    kARRAY_STATUS_GOOD              = 0x00,
    kARRAY_STATUS_DEGRADED          = 0x01,
    kARRAY_STATUS_REBUILDING        = 0x02,
    kARRAY_STATUS_REBUILDFAILED     = 0x03,
    kARRAY_STATUS_DATALOST          = 0x04,
    kARRAY_STATUS_NOTCONFIGURED     = 0x10,
    kARRAY_STATUS_BROKEN            = 0x11,
    kARRAY_STATUS_MIXED             = 0xFF
};

enum
{
    kARRAY_COMMAND_NOOP             = 0x80,
    kARRAY_COMMAND_CREATE           = 0x81,
    kARRAY_COMMAND_REBUILD          = 0x82,
    kARRAY_COMMAND_DELETE           = 0x083
};

enum
{
    kSLOT_STATUS_GOOD               = 0x00,
    kSLOT_STATUS_OBSOLETE           = 0x01,
    kSLOT_STATUS_REBUILDING         = 0x0F,
    kSLOT_STATUS_EMPTY              = 0x10,
    kSLOT_STATUS_MISSING            = 0x11,
    kSLOT_STATUS_BLANK              = 0x12,
    kSLOT_STATUS_FAILED             = 0x13,
    kSLOT_STATUS_SPARE              = 0x14,
    kSLOT_STATUS_NOT_PARTICIPATING  = 0x15,
    kSLOT_STATUS_IDMISMATCH         = 0x16,
    kSLOT_STATUS_HARD_REJECTED      = 0x17,
    kSLOT_STATUS_SOFT_REJECTED      = 0x18,
    kSLOT_STATUS_MIXED              = 0xFF
};

typedef struct __attribute__((packed))
{
    UInt8 slotStatus;
    UInt8 originalSlotOrder;
} SLOT_DESCRIPTOR;

typedef struct __attribute__ ((packed))
{
    union {
        UInt8	status;
        UInt8   command;
    } array;
    UInt8   arrayMode;
    UInt8   stripeSize;
    UInt8   rebuildPercentageComplete;
    UInt64  startLBA;
    UInt64  numberOfBlocks;
    CFUUIDBytes   volumeSetUUID;
    SLOT_DESCRIPTOR slotDescriptor[];
} VOLUME_SET_DESCIPTOR;

typedef struct  __attribute__((aligned (8), packed))
{
    UInt8 signature;
    UInt8 reserved1;
    UInt8 reserved2;
    UInt8 metadataStatus;
    UInt8 reserved3;
    UInt8 numberOfSlots;
    UInt8 maximumVolumesPerHDD;
    UInt8 numberOfVolumeSetDescriptors;
    VOLUME_SET_DESCIPTOR volumeSetDecriptor[];
} RAID_STATUS_AND_CONFIG_DATA;


//
//  RAID Slot Detail Parameter Data
//

typedef struct __attribute__((packed))
{
    char    busType;
#ifdef __LITTLE_ENDIAN__
    UInt8   valid:1;
    UInt8   reserved1:7;
#else
    UInt8   reserved1:7;
    UInt8   valid:1;
#endif
    UInt8   reserved2;
    UInt8   descritorLength;
    char    modelNumber[40];
    char    serialNumber[20];
    UInt64  useableCapacity;
    UInt16  blockLength;
    UInt16  mediumRotationRate;
    UInt8   reserved3;
    UInt8   reserved4:4;
    UInt8   nominalFormFactor:4;
    UInt8   reserved5;
    UInt8   reserved6;
} SLOT_DETAIL_DESCRIPTOR;

typedef struct __attribute__((packed))
{
    UInt8   signature;                  //0x31
    UInt8   reserved1;
    UInt8   reserved2;
    UInt8   numberOfDetailDescriptors;
    SLOT_DETAIL_DESCRIPTOR slot[];
} SLOT_DETAIL_PRAMETER_HEADER;

typedef struct __attribute__((packed))
{
    UInt8   operationCode;      // 0xA3
    UInt8   reserved1:3;
    UInt8   serviceAction:5;    // 0x1F
    UInt8   requestCode;        // 0x01 = Report Slot Details
    UInt8   sourceID;
    UInt8   reserved2;
    UInt8   reserved3;
    UInt8   allocationLength;
    UInt8   reserved4;
    UInt8   control;
} RAID_REPORT_SLOT_DETAILS;

typedef struct __attribute__((packed))
{
    UInt32   numberOfLUNs;
    UInt8   reserved1;
    UInt8   reserved2;
    UInt8   reserved3;
    UInt8   reserved4;
    UInt64   lun1;
    UInt64   lun2;
    UInt64   lun3;
} RAID_REPORT_LUNS_DETAILS;

//typedef struct __attribute__((packed))
//{
//    RAID_STATUS_AND_CONFIG_DATA config;
//    VOLUME_SET_DESCIPTOR volume0;
//    SLOT_DESCRIPTOR slot0;
//    SLOT_DESCRIPTOR slot1;
//} RAID_SET_RAID_CONFIGURATION;
//
//typedef struct __attribute__((packed))
//{
//    RAID_STATUS_AND_CONFIG_DATA config;
//    VOLUME_SET_DESCIPTOR volume0;
//    SLOT_DESCRIPTOR volume0Slot0;
//    SLOT_DESCRIPTOR volume0Slot1;
//    VOLUME_SET_DESCIPTOR volume1;
//    SLOT_DESCRIPTOR volume1Slot0;
//    SLOT_DESCRIPTOR volume1Slot1;
//} RAID_SET_JBOD_CONFIGURATION;

//
//typedef struct __attribute__((packed))
//{
//    UInt8   operationCode;      // 0xA4
//    UInt8   reserved1:3;
//    UInt8   serviceAction:5;    // 0x1F
//    UInt8   requestCode;        // 0x00 = Report Slot Details
//    UInt8   sourceID;
//    UInt8   reserved2;
//    UInt8   reserved3;
//    UInt8   parameterListLength;
//    UInt8   reserved4;
//    UInt8   control;
//} RAID_SET_CONFIGURATION;

//typedef struct __attribute__((packed))
//{
//    UInt8   operationCode;      // 0xA3
//    UInt8   reserved1:3;
//    UInt8   serviceAction:5;    // 0x1F
//    UInt8   requestCode;        // 0x00 Report RAID Status
//    UInt8   sourceID;
//    UInt8   reserved2;
//    UInt8   reserved3;
//    UInt8   allocationLength;
//    UInt8   reserved4;
//    UInt8   control;
//} RAID_STATUS_COMMAND;
//
-(int) getLunReport:(NSMutableArray*) lunArray;
-(int) getMetaDataStatus;
-(int) getNumberOfSlots;
-(int) getMaximumVolumesPerHDD;
-(int) getNumberOfVolumeSetDescriptors;
-(NSDictionary *) getVolumeSetDescriptors;

-(BOOL) getHWRAIDIsDriveInSlot: (int) deviceSlot;
-(uint64) getHWRAIDDeviceCapacity;
-(uint64) HWRAIDDriveCapacityInSlot: (int) deviceSlot;
-(NSString*) HWRAIDDriveSerialNumberInSlot: (int) deviceSlot;
-(NSString *) copyHWRAIDDriveModelInSlot: (int) deviceSlot;
-(NSString *) copyHWRAIDDriveSerialnumberInSlot: (int) deviceSlot;

-(int) getArrayStatusForVolumeSet: (int)volumeSet;
-(int) getArrayModeForVolumeSet: (int)volumeSet;
-(int) getArrayRebuildPercentageCompleteForVolumeSet: (int)volumeSet;

-(CFUUIDBytes) getUUIDForVolumeSet: (int)volumeSet;

- (int)getStatusForSlot: (int) slot inVolumeSet: (int)volumeSet;
- (void *) reEnumerateDevice;
@end
