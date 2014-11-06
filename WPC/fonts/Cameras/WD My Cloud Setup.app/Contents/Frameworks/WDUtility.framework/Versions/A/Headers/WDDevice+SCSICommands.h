//
//  SCSIInquiryCommand.h
//  DriveUnlock
//
//  Created by Bob on 3/11/09.
//  Copyright © 2014 Western Digital Technologies, Inc. All rights reserved.
//

#include <sys/param.h>

#import <IOKit/IOCFPlugIn.h>
#import <IOKit/scsi/SCSITask.h>
#import <IOKit/scsi/SCSICommandOperationCodes.h>
#import <IOKit/scsi/SCSICmds_MODE_Definitions.h>
#import <IOKit/storage/ata/ATASMARTLib.h>

#import "WDDevice.h"
#import "WDDMcommon.h"
#import "WDDevice+SendSCSICommand.h"
#import "WDDevice+Encryption.h"

#define defaultCipherID			AES_256_ECB	//	Default to AES_256_ECB.
#define defaultKeyLength		256

#define MAX_SCSI_INQUIRY_DATA_SIZE       256

//typedef union {
//	OSStatus		ret;
//	struct {
//		UInt8		status;
//		UInt8		sensekey;
//		UInt8		ASC;
//		UInt8		ASCQ;
//	} scsi;
//} SCSIStatus;

enum {
	// WDC Vendor specific SCSI command codes
	
	kENCRIPTION_STATUS_COMMAND		= 0xC0,
	kCHANGE_ENCRYPTION_PASSPHRASE	= 0xC1,
	KRESET_DATA_ENCRIPTION_KEY		= 0xC1,
	kUNLOCK_ENCRYPTION_COMMAND		= 0xC1,
	kFORMAT_DISK					= 0xC4,
	kREAD_HANDY_CAPACITY			= 0xD5,
	kREAD_HANDY_STORE				= 0xD8,
	kWRITE_HANDY_STORE				= 0xDA,
	kREAD_VITURAL_CD_CAPACITY		= 0xC5,
	kREAD_VIRTUAL_CD				= 0xC8,
	kWRITE_VIRTUAL_CD				= 0xCA,
    kREPORT_LUNS                    = 0xA0,
    kREPORT_RAID_STATUS             = 0xA3,
    kREPORT_SLOT_DETAILS            = 0xA3,
    kSET_RAID_CONFIGURATION         = 0xA4
};

enum {
	kENCRIPTION_STATUS_COMMAND_SIGNATURE	= 0x45
};

enum {
	kENCRIPTION_STATUS_COMMAND_LENGTH	= 10,
	kCHANGE_ENCRYPTION_PASSPHRASE_LENGTH = 10,
	KRESET_DATA_ENCRIPTION_KEY_LENGTH	= 10,
	kUNLOCK_ENCRYPTION_COMMAND_LENGTH	= 10,
	kREAD_HANDY_CAPACITY_LENGTH			= 10,		
	kREAD_HANDY_STORE_LENGTH			= 10,
	kWRITE_HANDY_STORE_LENGTH			= 10,
	kLOG_SENSE_COMMAND_LENGTH			= 10,
	kLOG_SELECT_COMMAND_LENGTH			= 10,
    kREPORT_RAID_STATUS_LENGTH          = 12,
    kREPORT_SLOT_DETAILS_LENGTH         = 12,
    kSET_RAID_CONFIGURATION_LENGTH      = 12
};

enum {
	kUNLOCK_ENCRYPTION_COMMAND_ACTION	= 0xE1,
	kCHANGE_ENCRYPTION_PASSPHRASE_ACTION = 0xE2,
	KRESET_DATA_ENCRIPTION_KEY_ACTION	= 0xE3
};

enum {
	kMODE_SENSE_DBD					= 0x08,
	kMODE_SENSE_LLBAA				= 0x10
};

enum {
	kMODE_SENSE_CURRENT_VALUES		= 0x00,				// 0x00
	kMODE_SENSE_CHANGEABLE_VALUES	= 0x40,				// 0x01
	kMODE_SENSE_DEFAULT_VALUES		= 0x80,				// 0x02	
	kMODE_SENSE_SAVED_VALUES		= 0xC0				// 0x03
};

enum {
	kPOWERCONDITIONPAGE				= 0x1A,
	kMODEDEVICECONFIGURATIONPAGE	= 0x20,
	kMODEOPERATIONSPAGE				= 0x21,
	kMODE_SENSE_ALL_PAGES           = 0x3F
};

typedef struct __attribute__((packed)){
	UInt8			LastCDAreaBlockAddress[4];
	UInt8			BlockLength[4];
	UInt8			reserved[2];
	UInt8			MaximumTransferLength[2];
} READ_VITURAL_CD_CAPACITY_DATA;

typedef struct __attribute__((packed)) {
	UInt8			modeParamHeader[4];
#ifdef __LITTLE_ENDIAN__
	UInt8			pageCode:6;
	UInt8			reserved1:1;
	UInt8			ParamSavable:1;
	UInt8			pageLength:8;
	UInt8			signature:8;
	UInt8			reserved2:8;
	UInt8			eSATA15:1;
	UInt8			LooseSB2:1;
	UInt8			reserved3:6;
	UInt8			enableCDEject:1;
	UInt8			CDMediaVaild:1;
	UInt8			reserved4:6;
	UInt8			reserved5:8;
	UInt8			reserved6:8;
	UInt8			powerLEDBrite:8;
	UInt8			backlightBrite:8;
	UInt8			whiteOnBlack:1;
	UInt8			reserved7:7;
	UInt8			reserved8:8;
#else
	UInt8			reserved8:8;
	UInt8			reserved7:7;
	UInt8			whiteOnBlack:1;
	UInt8			backlightBrite:8;
	UInt8			powerLEDBrite:8;
	UInt8			reserved6:8;
	UInt8			reserved5:8;
	UInt8			reserved4:6;
	UInt8			CDMediaVaild:1;
	UInt8			enableCDEject:1;
	UInt8			reserved3:6;
	UInt8			LooseSB2:1;
	UInt8			eSATA15:1;
	UInt8			reserved2:8;
	UInt8			signature:8;
	UInt8			pageLength:8;
	UInt8			ParamSavable:1;
	UInt8			reserved1:1;
	UInt8			pageCode:6;
#endif
} MODE_PARAMERTER_OPERATIONS_PAGE;

typedef struct __attribute__((packed)){
#ifdef __LITTLE_ENDIAN__
	UInt8			PeripheralDeviceType:5;
	UInt8			PeripheralQualifier:3;
#else
	UInt8			PeripheralQualifier:3;
	UInt8			PeripheralDeviceType:5;
#endif
    
	UInt8			PageCode;
	UInt8			Reserved;
	UInt8			PageLength;
	UInt8			ProductSerialNumber[32];
} SERIAL_NUMBER_PAGE;

typedef struct __attribute__((packed)){
	UInt8			signature;
	UInt8			reserved1[2];
    
#ifdef __LITTLE_ENDIAN__
	UInt8			OldDefault:1;
	UInt8			reserved2:3;
	UInt8			NewDefault:1;
	UInt8			reserved3:3;
#else
	UInt8			reserved3:3;
	UInt8			NewDefault:1;
	UInt8			reserved2:3;
 	UInt8			OldDefault:1;   
#endif
    
	UInt8			reserved4[2];
	UInt8			PasswordLength[2];
	UInt8			OldPassword[32];
	UInt8			NewPassword[32];
} CHANGE_ENCRYPTION_PASSPHRASE_PARAM;

typedef struct __attribute__((packed)){
	UInt8			signature;
	UInt8			reserved[5];
	UInt8			PasswordLength[2];
	UInt8			Password[32];
} CHANGE_ENCRYPTION_UNLOCK_PARAM;

typedef struct __attribute__((packed)){
	UInt8		signature;
	UInt8		reserved1[2];
	
#ifdef __LITTLE_ENDIAN__
	UInt8		combine:1;
	UInt8		reserved2:7;
#else
	UInt8		reserved2:7;
	UInt8		combine:1;
#endif
	
	UInt8		cipherID;
	UInt8		reserved3;
	UInt16		keyLength;
	UInt8		Key[32];
} RESET_DEK_PARAM;

typedef struct __attribute__((packed)){
#ifdef __LITTLE_ENDIAN__
	UInt8	pageCode:6;
	UInt8	reserved1:1;
	UInt8	ParamSavable:1;
#else
	UInt8	ParamSavable:1;
	UInt8	reserved1:1;
	UInt8	pageCode:6;
#endif
	
	UInt8	pageLength:8;
	UInt8	signature:8;
	UInt8	reserved2:8;
	
#ifdef __LITTLE_ENDIAN__
	UInt8	DisableSES:1;
	UInt8	DisableCDROM:1;
	UInt8	Reserved3:2;
    UInt8   DisableUAS:1;
    UInt8   DisableU1U2:1;
    UInt8   Reserved4:1;
	UInt8	DisableAP:1;
#else
	UInt8	DisableAP:1;
    UInt8   Reserved4:1;
    UInt8   DisableU1U2:1;
    UInt8   DisableUAS:1;
	UInt8	Reserved3:2;
	UInt8	DisableCDROM:1;
	UInt8	DisableSES:1;
#endif
	
#ifdef __LITTLE_ENDIAN__
	UInt8	DisableWhiteList:1;
	UInt8	TwoTBLimit:1;
	UInt8	reserved5:6;
#else
	UInt8	reserved5:6;
	UInt8	TwoTBLimit:1;
	UInt8	DisableWhiteList:1;
#endif
	
	UInt8	reserved6:8;
	UInt8	reserved7:8;
} DeviceConfigurationPage;

typedef struct __attribute__((packed)){
    UInt8           PageCode;
    UInt8           reserved1;
    UInt16          PageLength;
#ifdef __LITTLE_ENDIAN__
    UInt8           PowerOff:1;
    UInt8           RestartLights:1;
    UInt8           StopLights:1;
    UInt8           Obsolete:1;
    UInt8           ForceFanOn:1;
    UInt8           ReEnum:1;
    UInt8           reserved2:2;
#else
    UInt8           reserved2:2;
    UInt8           ReEnum:1;
    UInt8           ForceFanOn:1;
    UInt8           Obsolete:1;
    UInt8           StopLights:1;
    UInt8           RestartLights:1;
    UInt8           PowerOff:1;
#endif
} SCSI_Power_Down_page_80;

typedef struct __attribute__((packed)){
	UInt8			PageCode;
	UInt8			reserved1;
	UInt8			PageLength[2];
	UInt8			DiagnosticDriveID;
	UInt8			SmartStatusMSB;
	UInt8			SmartStatusLSB;
} SCSI_Smart_Status_page_84;

typedef struct __attribute__((packed)){
	UInt8			PageCode;
	UInt8			reserved1;
	UInt8			PageLength[2];
	UInt8			DiagnosticDriveID;
	UInt8			reserved2[3];
	UInt8			SmartData[512];
} SCSI_Smart_data_page_85;

typedef struct __attribute__((packed))
{
#ifdef __LITTLE_ENDIAN__
	UInt8	ParameterCode[2];
	
	UInt8	FormatAndLinking:2;
	UInt8	TMC:2;
	UInt8	ETC:1;
	UInt8	TSD:1;
	UInt8	Obsolete:1;
	UInt8	DU:1;
	
	UInt8	ParameterLength;
	
	UInt8	SelfTestResults:4;
	UInt8	Reserved1:1;
	UInt8	SelfTestCode:3;
	
	UInt8	SelfTestNumber;
	UInt8	AccumulatedPowerOnHours[2];
	UInt8	AddressOfFirstFailure[8];
	UInt8	SenseKey:4;
	UInt8	Reserved2:4;
	UInt8	ASC;
	UInt8	ASCQ;
	UInt8	VendorSpecific;
#else
	UInt8	ParameterCode[2];
	
	UInt8	DU:1;
	UInt8	Obsolete:1;
	UInt8	TSD:1;
	UInt8	ETC:1;
	UInt8	TMC:2;
	UInt8	FormatAndLinking:2;
	
	UInt8	ParameterLength;
	
	UInt8	SelfTestCode:3;
	UInt8	Reserved1:1;
	UInt8	SelfTestResults:4;
	
	UInt8	SelfTestNumber;
	UInt8	AccumulatedPowerOnHours[2];
	UInt8	AddressOfFirstFailure[8];
	UInt8	SenseKey:4;
	UInt8	Reserved2:4;
	UInt8	ASC;
	UInt8	ASCQ;
	UInt8	VendorSpecific;
#endif
} SelfTestResultsLogParameter;

typedef struct __attribute__((packed))
{
#ifdef __LITTLE_ENDIAN__
	UInt8	PageCode:6;
	UInt8   SPF:1;
	UInt8	DS:1;
	UInt8	SubPageCode;
	UInt8	PageLength[2];
	SelfTestResultsLogParameter SelfTestLog[20];
#else
	UInt8	DS:1;
	UInt8   SPF:1;
	UInt8	PageCode:6;
	UInt8	SubPageCode;
	UInt8	PageLength[2];
	SelfTestResultsLogParameter SelfTestLog[20];
#endif
} SelfTestResultsData;

typedef struct {
	UInt8	modeParamHeader[4];
    
#ifdef __LITTLE_ENDIAN__
	UInt8	pageCode:6;
	UInt8	reserved1:1;
	UInt8	ParamSavable:1;
#else
	UInt8	ParamSavable:1;
	UInt8	reserved1:1;
	UInt8	pageCode:6;
#endif
	
	UInt8	pageLength:8;
	
	
#ifdef __LITTLE_ENDIAN__
	UInt8	standbyY:1;
	UInt8	reserved2:7;
#else
	UInt8	reserved2:7;
	UInt8	standbyY:1;
#endif
    
#ifdef __LITTLE_ENDIAN__
	UInt8	standbyZ:1;
	UInt8	idleA:1;
	UInt8	idleB:1;
	UInt8	idleC:1;
	UInt8	reserved3:4;
#else
	UInt8	reserved3:4;
	UInt8	idleC:1;
	UInt8	idleB:1;
	UInt8	idleA:1;
	UInt8	standbyZ:1;
#endif
	
	UInt8	idleAContionTimer[4];
	UInt8	standbyZConditionTimer[4];
	UInt8	idleBContionTimer[4];
	UInt8	idleCContionTimer[4];
	UInt8	standbyYConditionTimer[4];
    uint8   reserved4[16];
} PowerConditionPage;

//typedef struct __attribute__((packed)){
//	UInt8		PeripheralDeviceType:5;
//	UInt8		PeripheralQualifer:3;
//	UInt8		PageCode;
//	UInt8		Reserved0;
//	UInt8		PageLength;
//	UInt8		Signature;
//	UInt8		Reserved1;
//	UInt8		MaxDisk;
//	UInt8		DiskInstalled;
//	UInt8		TotalNumberofBlocks[8];
//	UInt8		BlockLength[4];
//	UInt8		PreferredBlockLength[4];
//} RawCapacityPage;

enum {
	kMaxBSDNameLength			=	MAXPATHLEN,										// Max length of the bsd name as a c string 
};

//enum {
//    kSEND_DIAGNOSTIC_COMMAD             = 0x1D,
//	kDIAG_SHORT_TEST_COMMAND			= 0x1D,
//	kDIAG_EXTENDED_TEST_COMMAND			= 0x1D,
//	kDIAG_GET_TEST_STATUS_COMMAND		= 0x1C,
//};
//
//enum {
//    kSEND_DIAGNOSTIC_COMMAND_LENGTH     = 6,
//	kDIAG_SHORT_TEST_COMMAND_LENGTH		= 6,
//	kDIAG_EXTENDED_TEST_COMMAND_LENGTH	= 6,
//	kDIAG_GET_TEST_STATUS_COMMAND_LENGTH = 6
//};

// Moved to WDDMcommon.h
//
//typedef struct {
//	UInt8		LastHandyBlockAddress[4];
//	UInt8		BlockLength[4];
//	UInt8		Reserved1[2];
//	UInt8		MaximumTransferLength[2];
//} ReadHandyCapacityData;

//enum {
//	kHandStoreSecurityBlock		=	1,
//	kHandStoreUserSettingBlock	=	2
//};
//
//typedef union{
//	UInt8	byte[512];
//	struct {
//		UInt8	Signature[4];
//		UInt8	reserved1[4];
//		UInt8	IterationCount[4];
//		UInt8	Salt[8];
//		UInt8	reserved2[4];
//		UInt8	hint[202];
//      UCHAR   PasswordGUID[82];  // (maximum 40 characters plus NULL ending)
//		UInt8	reserved3[203];
//		UInt8	checksum;
//	}data; 
//} SecurityBlock;

typedef union __attribute__((packed)){
	UInt8	byte[512];
	struct __attribute__((packed)){
		UInt8	Signature[4];	//	  4 Bytes
		UInt8	reserved1[4];	//	  4 Bytes
		UInt8	Label[64];		//	 64 Bytes
		UInt8	reserved[439];	//	439 Bytes
		UInt8	checksum;		//	  1 Byte
	}data; 
} HandyStoreUserBlock;


typedef struct __attribute__((packed)){
	UInt8	modeParamHeader[4];
	
#ifdef __LITTLE_ENDIAN__
	UInt8	pageCode:6;
	UInt8	reserved1:1;
	UInt8	ParamSavable:1;
#else
	UInt8	ParamSavable:1;
	UInt8	reserved1:1;
	UInt8	pageCode:6;
#endif
	
	UInt8	pageLength:8;
	UInt8	signature:8;
	UInt8	reserved2:8;
	
#ifdef __LITTLE_ENDIAN__
	UInt8	eSATA15:1;
	UInt8	LooseSB2:1;
	UInt8	reserved3:6;
#else
	UInt8	reserved3:6;
	UInt8	LooseSB2:1;
	UInt8	eSATA15:1;
#endif
	
#ifdef __LITTLE_ENDIAN__
	UInt8	enableCDEject:1;
	UInt8	CDMediaVaild:1;
	UInt8	reserved4:6;
#else
	UInt8	reserved4:6;
	UInt8	CDMediaVaild:1;
	UInt8	enableCDEject:1;
#endif
	
	UInt8	reserved5:8;
	UInt8	reserved6:8;
	UInt8	powerLEDBrite:8;
	UInt8	backlightBrite:8;
	
#ifdef __LITTLE_ENDIAN__
	UInt8	whiteOnBlack:1;
	UInt8	reserved7:7;
#else
	UInt8	reserved7:7;
	UInt8	whiteOnBlack:1;
#endif
	
	UInt8	reserved8:8;
} OperationsPage;

@interface WDDevice (SCSICommands)

- (BOOL) SCSITestUnitReady:(NSMutableDictionary **)errorDict;
- (int) GetSCSIInquiryData: (UInt8 *) buffer buffersize:(int)bufferSize error:(NSMutableDictionary **)errorDict;
- (int) GetSCSISerialNumber: (UInt8 *) buffer buffersize:(int)bufferSize error:(NSMutableDictionary **)errorDict;

//- (int) EncryptArmWithPassword:(UInt8*)  cookedPassword  passwordLength:(UInt8) passwordLength error:(NSMutableDictionary **)errorDict;
//- (int) EncryptDisArmWithPassword:(UInt8*) cookedPassword  passwordLength:(UInt8) passwordLength error:(NSMutableDictionary **)errorDict;
//- (int) EncryptChangePassword: (UInt8)passwordLength oldCookedPassword:(UInt8*) oldCookedPassword newCookedPassword:(UInt8*) newCookedPassword error:(NSMutableDictionary **)errorDict;
//- (int) EncryptGetStatus:(EncryptStatusReturnData *) returnData error:(NSMutableDictionary **)errorDict;
//- (int) EncryptUnlockWithPassword:(UInt8*)password passwordLength:(UInt8) passwordLength  error:(NSMutableDictionary **)errorDict;
//- (int) EncryptResetDEK:(UInt32 *) newKey error:(NSMutableDictionary **)errorDict;
//- (int) ReadHandyStoreBlockNumber:(int) blockNumber blockdata:(UInt8 *) blockData error:(NSMutableDictionary **)errorDict;
//- (int) WriteHandyStoreBlockNumber:(int) blockNumber blockdata:(UInt8 *) blockData error:(NSMutableDictionary **)errorDict;
//- (int) GetHandyStoreCapacity: (ReadHandyCapacityData*) HandyCapacityData error:(NSMutableDictionary **)errorDict;

//
//  Destination Disk LUN compatable commands
//
- (int) EncryptArmWithPassword:(UInt8*)  cookedPassword  passwordLength:(UInt8) passwordLength onLUN: destLUN error:(NSMutableDictionary **)errorDict;
- (int) EncryptDisArmWithPassword:(UInt8*) cookedPassword  passwordLength:(UInt8) passwordLength onLUN: destLUN error:(NSMutableDictionary **)errorDict;
- (int) EncryptChangePassword: (UInt8)passwordLength oldCookedPassword:(UInt8*) oldCookedPassword newCookedPassword:(UInt8*) newCookedPassword onLUN: destLUN error:(NSMutableDictionary **)errorDict;
- (int) EncryptGetStatus:(EncryptStatusReturnData *) ReturnData onLUN: (UInt32)destLUN error:(NSMutableDictionary **)errorDict;
- (int) EncryptUnlockWithPassword:(UInt8*)password passwordLength:(UInt8) passwordLength  onLUN: destLUN error:(NSMutableDictionary **)errorDict;
- (int) EncryptResetDEK:(UInt32 *) newKey onLUN: (UInt32)destLUN error:(NSMutableDictionary **)errorDict;
- (int) ReadHandyStoreBlockNumber:(int) blockNumber blockdata:(UInt8 *) blockData onLUN: destLUN error:(NSMutableDictionary **)errorDict;
- (int) WriteHandyStoreBlockNumber:(int) blockNumber blockdata:(UInt8 *) blockData onLUN: destLUN error:(NSMutableDictionary **)errorDict;
- (int) GetHandyStoreCapacity: (ReadHandyCapacityData*) HandyCapacityData onLUN: destLUN error:(NSMutableDictionary **)errorDict;
//
//
//

- (int) verifyHandyStoreSecurityBlock:(SecurityBlock *)blockData;
- (int) initHandyStoreSecurityBlock:(SecurityBlock*) blockData;
- (int) verifyHandyStoreUserBlock:(HandyStoreUserBlock *)blockData;
- (int) initHandyStoreUserBlock:(HandyStoreUserBlock*) blockData;

- (int) verifyHandyStoreSecurityBlock:(SecurityBlock *)blockData;

- (int) GetRawDeviceCapacity:(UInt64 *)capacity error:(NSMutableDictionary **)errorDict;

- (int) setVCDState: (bool) state error:(NSMutableDictionary **)errorDict;
- (BOOL) vcdStatus:(NSMutableDictionary **)errorDict;
- (bool) VCDSupported:(NSMutableDictionary **)errorDict;
- (int)  performVCDConfiguration;
- (int) ReadVirturalCDCapacity:(READ_VITURAL_CD_CAPACITY_DATA *) VCDCapacityDataPtr;

- (int) WriteVirtualCD:(unsigned char *) DataPtr onSlot: destSlot atBlock:(UInt32)blockAddress length:(UInt64)dataLength;
- (int) ReadVirtualCD:(unsigned char *) DataPtr onSlot: destSlot atBlock:(UInt32)blockAddress length:(UInt64)dataLength;
- (int) ModeSelectCommand:(unsigned char *) PageDataPtr length:(UInt64)PageDataLength savepages:(bool)savePages;
- (int) ModeSenseCommand:(unsigned char *) PageDataPtr length:(UInt64)PageDataLength pagecontrol:(int)pageControl pagecode:(int)pageCode;

- (int) diagStartShortTest:(NSMutableDictionary **)errorDict;
- (int) diagStartExtTest:(NSMutableDictionary **)errorDict;
- (int) diagAbortTest:(NSMutableDictionary **)errorDict;
- (int) diagGetSmartData:(ATASMARTData *) smartData forSlot: (int) slot error:(NSMutableDictionary **)errorDict;
- (int) diagGetSmartStatus:(UInt16 *) smartStatus forSlot: (int)slot error:(NSMutableDictionary **)errorDict;
- (int) getSelfTestData:(SelfTestResultsData *) selfTestData forSlot: (int)slot error:(NSMutableDictionary **)errorDict;
- (int) diagReEnumerate:(NSMutableDictionary **)errorDict;

- (int) ConfigSetDriveSleepTimer:(UInt8) minutes error:(NSMutableDictionary **)errorDict;
- (int) ConfigGetDriveSleepTimer:(UInt8 *) minutes error:(NSMutableDictionary **)errorDict;

- (int)reportRAIDStatus:(UInt8 *) buffer buffersize:(UInt32)bufferSize sourceID: (UInt8) sourceID error:(NSMutableDictionary **)errorDict;
- (int)reportSlotDetails:(UInt8 *) buffer buffersize:(UInt32)bufferSize sourceID: (UInt8) sourceID error:(NSMutableDictionary **)errorDict;
- (int)setHWRAIDConfiguration:(UInt8 *) buffer buffersize:(UInt32)bufferSize error:(NSMutableDictionary **)errorDict;
- (int)reportLUNs:(UInt8 *) buffer buffersize:(UInt32)bufferSize error:(NSMutableDictionary **)errorDict;

@end
