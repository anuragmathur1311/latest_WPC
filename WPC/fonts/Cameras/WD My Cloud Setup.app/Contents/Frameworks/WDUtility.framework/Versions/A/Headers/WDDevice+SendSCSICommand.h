//
//  SendSCSICommand.h
//  DriveUnlock
//
//  Created by Bob on 3/11/09.
//  Copyright © 2014 Western Digital Technologies, Inc. All rights reserved.
//

#import <IOKit/IOCFPlugIn.h>
#import <IOKit/scsi/SCSITask.h>

#import "WDDevice.h"
#import "WDDMcommon.h"

//typedef struct {
//	unsigned  char	cdb[16];			// the cdb
//	UInt64			dataSize;			// amount to transfer 
//	unsigned long	direction;			// transfer direction
//	unsigned long	timeOut;			// timeout
//	unsigned long	cdbLength;			// length of cdb
//	void*			senseInfoRef;		// pointer to sense info
//	void*			dataRef;			// pointer to data
//	SCSITaskStatus	status;				// scsi status
//	unsigned char	lun;				// lun
//	unsigned char	targetID;			// currently not used
//	unsigned char	pathID;				// currently unused
//}scsiData, *scsiDataPtr;

@interface WDDevice (SendSCSICommand)

- (bool)openSES;
- (void)closeSES;
- (OSStatus)SendSCSICommand:(scsiData*)scsiCommand;

@end
