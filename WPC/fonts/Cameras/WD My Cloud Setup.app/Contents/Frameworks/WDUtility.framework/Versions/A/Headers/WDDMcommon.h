/*
	Created by David Tay
	Copyright © 2006 Western Digital Corporation. All rights  reserved.
	File name:		common.h
	Contents:		Common definitions used in the various modules for both the device, server and clients
*/

#import <IOKit/scsi/SCSITaskLib.h>

#ifndef COMMON
#define COMMON

#define kServerSocketPath		"/var/tmp/com.WD.WDDMService/Socket"
//#define kServerSocketPath		"/var/tmp/com.WD.WDDriveManagerService/Socket"
//#define kServerSocketPath     "/var/tmp/com.WD.WDDrvSvc/Socket"
//#define kServerSocketPath		"/tmp/com.WD.WDDriveManagerService/Socket"

#define kReportID1			1
#define kReportID2			2
#define kReportID3			3
#define kReportIDGetSN1		0x10
#define kReportIDGetSN2		0x20

#define ArraySatusMask			0x0F
#define CurRAIDConfigMask		0xC0
#define RAID0Config				0
#define RAID1Config				1

enum {
	kDeviceSNLength	= 17,		// Length of device serial number
	kFRUSNLength	= 21,		// Length of each of the FRU serial numbers
	kTotalSNLength	= 59,		// Length of device and 2 FRU serial numbers 
	kNameLength		= 256		// Name length
};

enum {						// Internal codes used by WDRAIDManager
	kStatusGood		= 0,	// RAID is working fine
	kStatusCaution	= 1,	// RAID is degraded
	kStatusBad		= 2,	// RAID is bad
	kStatusRebuild	= 3,	// Rebuilding RAID
	kStatusUnknown	= 4		// RAID status unknown
};

enum {
    kPacketIDNone  = 0,
    kPacketIDFirst = 1
};

enum {
	kRAID0Setting,	// Fast
	kRAID1Setting	// Safe
};


enum {						// Internal codes used by WDRAIDManager
	kFastHFSSetting		= 0,
	kSafeHFSSetting		= 1,
	kFastFAT32Setting	= 2,
	kSafeFAT32Setting	= 3,
	kUnknownSetting		= 4
};

enum {
	kHFSPPartitionType	= 0,	// Default Partition type - journaled HFS+ 
	kFATPartitionType	= 1,	// FAT 32
};

enum {
	kArrayStatsOK			= 0x0,		// Array properly configured and operating normally
	kArrayNoConfig			= 0x1,		// No valid array configuration for either slot
	kArrayConfigProblem		= 0x2,		// Something wrong with one or both of the slots' array configurations
	kArrayConfigMismatch	= 0x3,		// Valid configuration for both slots, but slots not in agreement
	kArrayAllDisksFaulty	= 0x4,		// Valid configuration, but both slots are faulty
	kArrayDegraded			= 0x5,		// Valid configuration, one faulty slot
	kArrayRebuilding		= 0x6,		// Valid configuration, rebuilding one slot
	kArrayRebuildFailed		= 0x7,		// Valid configuration, but rebuilding failed
	kArrayNoDisks			= 0x8,		// No disks attached.
	kArrayFailed			= 0x9,		// new FireFly 2x error code
	kArrayIOProblem			= 0x10,		// Problem with device from IOKit standpoint
	kCouldNotUnmount		= 0x20,		// Could not unmount the disk
	kUnitIsOverTemp			= 0x30,		// Overtemp condition
	kArrayNoSES				= 0xDE		// First byte of 0xDEADBEEF
};

enum {
	kSlotOk					= 0x0,
	kSlotMissing			= 0x01,
	kSlotNoConfig			= 0x02,
	kSlotWrongSlot			= 0x03,
	kSlotFaulty				= 0x04,
	kSlotRebuilding			= 0x05,
	kSlotDetecting			= 0x06,
	kSlotRejected			= 0x07
};

enum {
	kAStatusDismountFail	= 0x1,		// Could not dismount the drive
	kAStatusBlankingFail	= 0x2,		// Could not blank the disk
	kAStatusNewFSFail		= 0x3,		// Could not create a new FS
	kAStatusMountFail		= 0x4,		// Failed to remount the disk
	kAStatusDisappeared		= 0x5		// Disk disappeared
};

enum {
	kDiskClaim,
	kCheckDiskClaim,
	kDiskUnmount,
	kCheckUnmountDisk,
	kDiskBlank,
	kCheckBlankDisk,
	kConfigureRAID,					// Configure the RAID step
	kCheckRAIDConfig,				// Check the RAID configuration
	kDiskReappear,
	kDiskNewFS,
	kCheckNewFSDisk,
	kDiskRemount,
	kCheckRemountDisk,
	kDiskDone,
	kDiskUnmountFailed,
	kDiskUnmounted,
	kDiskDisappeared
};

enum {
	kDiskCountType	= 1,	// Number of known disks
	kDiskInfoType	= 2,	// High level info for one disk to be displayed in the table view
	kDiskDetailType	= 3,	// Detailed information for one disk to be displayed in the disclosure view
	kSCSIRetType	= 4		// SCSI Return Data
};

enum {
    kPacketMagic = 'WDPM'               // WD Packet Magic
};

enum {
	kUnknownConnectionType,		// Unknown  connection
	kFireWireConnectionType,	// Device connected via the 1394 interface
	kUSBConnectionType			// Device connected via the USB interface
};

enum {
	kPacketMaximumSize = 1024
};

enum {
    kPacketTypeGoodbye	= 'GDBY',            // sent by client to server before signing off
    kPacketTypeNOP		= 'NOOP',            // no operation, test for client/server RPC
    kPacketTypeReply	= 'RPLY',            // all RPC replies are of this type
    kPacketTypeTalk		= 'TALK',            // client/server RPC to print message on server
    kPacketTypeLogout	= 'LGOT',            // sent by client to server, which echoes it to all listening clients
    kPacketTypeListen	= 'LSTN',            // client/server RPC to register for error messages
	kPacketTypeInfo		= 'INFO',			 // Informational messages to the Configurator
    kPacketTypeQuit		= 'QUIT'             // client/server RPC to tell server to quit
};

// messages that the  server will respond to or send

enum {
//	kMsgRAIDFast		= 'FAST',	// tell server to configure device as RAID 0 / fast setting
//	kMsgRAIDSafe		= 'SAFE',	// tell server to configure device as RAID 1 / safe setting
	kMsgRAID0HFS		= 'FHFS',	// Set to RAID 0 and lay down a HFS+ partition
	kMsgRAID1HFS		= 'SHFS',	// Set to RAID 1 and lay down a HFS+ partition
	kMsgRAID0FAT32		= 'FFAT',	// Set to RAID 0 and lay down a FAT32 partition
	kMsgRAID1FAT32		= 'SFAT',	// Set to RAID 1 and lay down a FAT32 partition
	kMsgPartitionHFS	= 'HFS+',	// tell server to lay down a HFS+ partition
	kMsgPartitionFAT32	= 'FAT_',	// tell server to lay down a FAT 32 partition
//	kMsgUnmount			= 'UNMT',	// tell server to unmount volume
//	kMsgUnmountFail		= 'UFAL',	// could not unmount the volume(s)
//	kMsgPartitionFail	= 'PFAL',	// failed  to create the partition
//	kMsgRAIDConfFail	= 'RFAL',	// failed  to configure the RAID setting
	kMsgAllDiskInfo		= 'ADSK',	// get information about all disks
	kMsgOneDiskInfo		= 'ODSK',	// Information for one specific disk
	kMsgDiskCount		= 'DSKC',	// Number of disks we're tracking
	kMsgNewDiskInfo		= 'DNEW',	// New disk was attached to computer
	kMsgRemoveDiskInfo	= 'DRMV',	// Disk was disconnected from computer
	kMsgActionStatus	= 'STAT',	// Operation status
	kMsgUpdateDiskInfo	= 'UDSK',	// Update disk info
	kMsgUpdateDiskStat	= 'UDST',	// Update disk status
	kMsgSendSCSI		= 'SCSI',	// Send a SCSI command to the target.
	kMsgSetDriveLabel	= 'DLBL',	// Set the external label of the drive.
	kHandyStoreWrite	= 'HSWR',	// Write Handy Store information.
	kHandyStoreRead		= 'HSRD',	// Read Handy Store information.
	kHandyStoreCapacity	= 'HSCP',	// Get the Handy Store Capacity.
	kEncryptionStatus	= 'ESTS',	// Encryption Status
	kEncryptionArm		= 'EARM',	// Encryption Arm	
	kEncryptionDisArm	= 'EDAR',	// Encryption Disarm
	kEncryptionChangePassword = 'ECPW', // Encryption Change Password
	kEncryptionUnlock	= 'EULC',	// Encryption Unlock
	kEncryptionResetKey	= 'ERSK',	// Encryption Reset Key
//	kEncryptionReturnStatus = 'ERST', // Return status for Encryption call
	kDiagShortTest		= 'DSST',	//	Diagnostic Short Self Test
	kDiagExtTest		= 'DEST',	// Diagnostic Extended Self Test
	kDiagAbortTest		= 'DABT',	// Diagnostic Abort Test
	kDiagGetSmartData	= 'DGSD',	// Get the Smart  Data
	kDiagGetSmartStatus	= 'DGSS',	// Get the Samrt Status
	kSetSleepTimer		= 'SSLT',	// Set the drive sleep timer
	kGetSleepTimer		= 'GSLT'	// Get the drive sleep timer
};

enum {
	kHandStoreSecurityBlock		=	1,
	kHandStoreUserSettingBlock	=	2
};


//
//	
//

typedef struct {
	OSType		fMagic;
	uid_t		fUserID;
	gid_t		fGroupID;
	OSType		fType;
	int32_t		fID;
	uint32_t	fSize;
} PacketHeader, *PacketHeaderPtr;

typedef struct {
	unsigned  char	cdb[16];			// the cdb
	UInt64			dataSize;			// amount to transfer 
	unsigned long	direction;			// transfer direction
	unsigned long	timeOut;			// timeout
	unsigned long	cdbLength;			// length of cdb
	void*			senseInfoRef;		// pointer to sense info
	void*			dataRef;			// pointer to data
	SCSITaskStatus	status;				// scsi status
	unsigned char	lun;				// lun
	unsigned char	targetID;			// currently not used
	unsigned char	pathID;				// currently unused
}scsiData, *scsiDataPtr;

typedef union {
	OSStatus		ret;
	struct {
		UInt8		status;
		UInt8		sensekey;
		UInt8		ASC;
		UInt8		ASCQ;
	} scsi;
} SCSIStatus;

//
// Data structures returned in response to commands.
//

typedef struct {
	PacketHeader	header;
	UInt32			numDisks;
} PacketDiskCount;

typedef struct {
	char	deviceBSDName[kNameLength];			// Key which used to uniquely id individual devices on a system
	char	deviceCDBSDName[kNameLength];		// Name of the CD associated with this device.
	char	deviceName[kNameLength];			// Device name to be displayed
	char	deviceSerialNumber[kDeviceSNLength];// Serial number to be displayed
	char	devFRU1SN[kFRUSNLength];			// Serial number of the FRU1
	char	devFRU2SN[kFRUSNLength];			// FRU2 serial number
	UInt64	deviceCapacity;						// Total disk capacity in bytes
	char	deviceStatus[4];					// The status bits
	UInt32	deviceConfig;						// Upper bits - USB or 1394, Lower bits - partition
	UInt32	devInterface;						// IO Registry entry for the main device
	SInt16	devProductID;						// WD Product ID
	SInt16	deviceFirmwareVersion;				// Firmware version of bridge
} DeviceInfoBlock;

typedef struct {
	PacketHeader	header;
	DeviceInfoBlock device[];
} PacketDiskInfo;

typedef struct {
	PacketHeader	header;
	CFStringRef		deviceName;
	CFStringRef		partitionType;
	UInt32			connection;		// USB 1394 or unknown
} PacketDiskDetail;

typedef struct {                      // reply: 
    PacketHeader    header;            // fType is kPacketTypeNOP, fID echoed
} PacketNOP;

// Packet for communicating messages to the  Notifier client(s).
typedef struct {
	PacketHeader	header;
	Str255			devIdentifier;		// error title
}PacketTalk;

// Packet for informing the CONFIGURATOR of device changes, command completion
typedef struct {
	PacketHeader	header;
	SInt32			status;
	UInt8			buffer[];
} PacketInfo;

typedef struct {
	PacketHeader	header;
	UInt8			buffer[kNameLength];
} PacketCommand;

// Data structure for use with the Send SCSI Command.

typedef union {
	OSStatus		ret;
	struct {
		UInt8		status;
		UInt8		sensekey;
		UInt8		ASC;
		UInt8		ASCQ;
	} scsi;
} SWStatus;

typedef struct {
	PacketHeader		header;
	SWStatus			status;
} PacketEncryptRetStatus;

typedef struct {
	PacketHeader		header;	
	int					SCSIStatus;
	SCSI_Sense_Data		SCSISenseData;
	int					SCSIReturnDataSize;
	UInt8				SCSIReturnData[];
} PacketSCSIRetData;

typedef struct {
	PacketHeader		header;
	char				deviceBSDName[kNameLength];
	char				cmd[16];
	int					cmdlen;
	int					dir;
	int					datalen;
	unsigned char		data[];
} PacketSCSICommand;

typedef struct {
	PacketHeader		header;
	char				deviceBSDName[kNameLength];
	int					SetmentMapLen;
	unsigned char		SegmentMap[24];
} PacketSetDriveLabelCommand;

typedef struct {
	PacketHeader	header;
	char			deviceBSDName[kNameLength];
	SWStatus		status;
	UInt8			buffer[kNameLength];
} PacketHandyStoreCapacityCommand;

typedef struct {
	UInt8		LastHandyBlockAddress[4];
	UInt8		BlockLength[4];
	UInt8		Reserved1[2];
	UInt8		MaximumTransferLength[2];
} ReadHandyCapacityData;

typedef struct {
	PacketHeader		header;
	SWStatus			status;
	ReadHandyCapacityData	HandyCapacityData;
} PacketHandyStoreCapacityRetStatus;

typedef struct {
	PacketHeader	header;
	char			deviceBSDName[kNameLength];
	SWStatus		status;
	int				blocknumber;
	UInt8			BlockData[512];
} PacketHandyStoreWriteCommand;

typedef struct {
	PacketHeader		header;
	SWStatus			status;
} PacketHandyStoreWriteRetStatus;

typedef struct {
	PacketHeader	header;
	char			deviceBSDName[kNameLength];
	SWStatus		status;
	int				blocknumber;
} PacketHandyStoreReadCommand;

typedef struct {
	PacketHeader		header;
	SWStatus			status;
	UInt8				BlockData[512];
} PacketHandyStoreReadRetStatus;

typedef struct {
	PacketHeader	header;
	char			deviceBSDName[kNameLength];
	SWStatus		status;
} PacketDiagShortTestCommand;

typedef struct {
	PacketHeader		header;
	SWStatus			status;
} PacketDiagShortTestRetStatus;

typedef struct {
	PacketHeader	header;
	char			deviceBSDName[kNameLength];
	SWStatus		status;
} PacketDiagExtTestCommand;

typedef struct {
	PacketHeader		header;
	SWStatus			status;
} PacketDiagExtTestRetStatus;

typedef struct {
	PacketHeader	header;
	char			deviceBSDName[kNameLength];
	SWStatus		status;
} PacketAbortTestCommand;

typedef struct {
	PacketHeader		header;
	SWStatus			status;
} PacketAbortTestRetStatus;

typedef struct {
	PacketHeader	header;
	char			deviceBSDName[kNameLength];
	SWStatus		status;
} PacketDiagGetSmartDataCommand;

typedef struct {
	PacketHeader		header;
	SWStatus			status;
	UInt8				BlockData[512];
} PacketDiagGetSmartDataResponse;

typedef struct {
	PacketHeader	header;
	char			deviceBSDName[kNameLength];
	SWStatus		status;
} PacketDiagGetSmartStatusCommand;

typedef struct {
	PacketHeader		header;
	SWStatus			status;
	UInt16				SmartStatus;
} PacketDiagGetSmartStatusResponse;

typedef struct {
	PacketHeader	header;
	char			deviceBSDName[kNameLength];
	SWStatus		status;
	UInt8			minutes;
} PacketSetSleepTimerCommand;

typedef struct {
	PacketHeader	header;
	SWStatus		status;

} PacketSetSleepTimerStatusResponse;

typedef struct {
	PacketHeader	header;
	char			deviceBSDName[kNameLength];
	SWStatus		status;
} PacketGetSleepTimerCommand;

typedef struct {
	PacketHeader	header;
	SWStatus		status;
	UInt8			minutes;
} PacketGetSleepTimerStatusResponse;

//
//
//
typedef struct {
	PacketHeader		header;
	char				deviceBSDName[kNameLength];
} PacketEncryptGetStatus;
	
typedef struct {
	UInt8		Signature;
	UInt8		reserved1[2];
	UInt8		SecurityState;
	UInt8		CurrentCipherID;
	UInt8		reserved2;
//	UInt8		PasswordLength[2];
    UInt16		PasswordLength;
	UInt8		KeyResetEnabler[4];
	UInt8		reserved3[3];
	UInt8		NumberOfCiphers;
	UInt8		CipherList[16];
} EncryptStatusReturnData;

typedef struct {
	UInt8		PeripheralDeviceType:5;
	UInt8		PeripheralQualifer:3;
	UInt8		PageCode;
//	UInt8		Reserved0;
	UInt16		PageLength;
	UInt8		Signature;
	UInt8		Reserved1;
	UInt8		MaxDisk;
	UInt8		DiskInstalled;
	UInt8		TotalNumberofBlocks[8];
	UInt8		BlockLength[4];
	UInt8		PreferredBlockLength[4];
} RawCapacityPage;

typedef struct {
	PacketHeader				header;
	SWStatus					status;
	EncryptStatusReturnData		EncryptStatusData;
} PacketEncryptGetStatusReturn;

enum {
	SEC_STAT_EncryptionOff	= 0,
	SEC_STAT_Locked			= 1,
	SEC_STAT_Unlocked		= 2,
	SEC_STAT_UnlocksExceeded	= 6,
	SEC_STAT_NoDEK			= 7
};

enum {
	NO_CIPHER		= 0x00,
	AES_128_ECB		= 0x10,
	AES_128_CBC		= 0x12,
	AES_128_XTS		= 0x18,

	AES_256_ECB		= 0x20,
	AES_256_CBC		= 0x22,
	AES_256_XTS		= 0x28,
    FULL_DISK_ENCRYPTION = 0x30
};

//
//
//
typedef struct {
	PacketHeader		header;
	char				deviceBSDName[kNameLength];
	UInt8				PasswordLength;
	char				NewPassword[32];
} PacketEncryptArm;

//
//
//
typedef struct {
	PacketHeader		header;
	char				deviceBSDName[kNameLength];
	UInt8				PasswordLength;
	char				Password[32];
} PacketEncryptDisarm;

//
//
//
typedef struct {
	PacketHeader		header;
	char				deviceBSDName[kNameLength];
	UInt8				PasswordLength;
	char				OldPassword[32];
	char				NewPassword[32];
} PacketEncryptChangePassword;

//
//
//
typedef struct {
	PacketHeader		header;
	char				deviceBSDName[kNameLength];
	UInt8				PasswordLength;
	char				Password[32];
} PacketEncryptUnlock;

//
//
//

typedef struct {
	PacketHeader		header;
	char				deviceBSDName[kNameLength];
	UInt32				newKey;
} PacketResetEncryptionKey;

typedef union{
	UInt8	byte[512];
	struct {
		UInt8	Signature[4];
		UInt8	reserved1[4];
		UInt8	IterationCount[4];
		UInt8	Salt[8];
		UInt8	reserved2[4];
		UInt8	hint[202];
        UInt8   PasswordGUID[82];  // (maximum 40 characters plus NULL ending);
		UInt8	reserved3[203];
		UInt8	checksum;
	}data; 
} SecurityBlock;

#ifdef __cplusplus
extern "C" {
#endif
int UnixErrno(int result);
int IgnoreSigPipe(void);
int UnixRead(int fd, void *buffer, size_t bufSize, size_t *bytesRead);
int UnixWrite(int fd, const void *buffer, size_t buffSize, size_t *bytesWritten);
int UnixSetNonBlocking(int fd);

typedef void (*SignalSocketCallback)(const siginfo_t *sigInfo, void *refCon);

int InstallSignalToSocket(const sigset_t* sigSet, CFRunLoopRef runLoop, CFStringRef runLoopMode, SignalSocketCallback callback, void* refcon);
void InitPacketHeader(PacketHeader *packet, OSType packetType, size_t packetSize, uid_t userID, gid_t groupID, Boolean rpc);
#ifdef __cplusplus
}
#endif
#endif