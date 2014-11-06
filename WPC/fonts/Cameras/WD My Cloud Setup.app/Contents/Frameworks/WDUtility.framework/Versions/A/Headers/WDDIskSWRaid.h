//
//  WDDIskSWRaid.h
//  WDUtility
//
//  Created by Robert (bobhahn@mac.com) Hahn on 12/13/13.
//
//

#import <CoreFoundation/CoreFoundation.h>
#import <Foundation/Foundation.h>
#import "WDDisk.h"

@interface WDDiskSWRaid : WDDisk
{
    NSString *		mPortName;
    io_service_t	mIOAHCIDevice;
    UInt64			mTunnelEndpointGUID;
    UInt8			mSlotNumber;
    Boolean			mRAIDMember;
    NSString *		mRAIDBSDName;
    UInt64			mRAIDSize;
    NSString *		mRAIDLevel;
    NSString *		mRAIDSetName;
    NSString *		mRAIDStatus;
    NSString *		mRAIDFormat;
    NSString *		mRAIDMemberStatus;
    NSString *		mRAIDMemberUUID;
    NSNumber *		mRAIDRebuildProgress;
    NSString *		mRAIDSetUUID;
    NSArray *		mRAIDSetMembers;
    //	Boolean			mTestInProgress;
    //	Boolean			mEraseInProgress;
    //	double			mEraseProgressPercentage;
}

@property (readwrite,copy) NSString *		mPortName;
@property (readwrite,assign) io_service_t	mIOAHCIDevice;
@property (readwrite,assign) UInt8			mSlotNumber;
@property (readwrite,assign) Boolean		mRAIDMember;
@property (readwrite,copy) NSString *		mRAIDBSDName;
@property (readwrite,assign) UInt64			mRAIDSize;
@property (readwrite,copy) NSString *		mRAIDLevel;
@property (readwrite,copy) NSString *		mRAIDSetName;
@property (readwrite,copy) NSString *		mRAIDStatus;
@property (readwrite,copy) NSString *		mRAIDFormat;
@property (readwrite,copy) NSString *		mRAIDMemberStatus;
@property (readwrite,copy) NSString *		mRAIDMemberUUID;
@property (readwrite,copy) NSNumber *		mRAIDRebuildProgress;
@property (readwrite,copy) NSString *		mRAIDSetUUID;
@property (readwrite,copy) NSArray *		mRAIDSetMembers;
//@property (readwrite,assign) Boolean		mTestInProgress;
//@property (readwrite,assign) Boolean		mEraseInProgress;
//@property (readwrite,assign) double		mEraseProgressPercentage;
@property (readwrite,assign) UInt64			mTunnelEndpointGUID;

- (id) initWithServiceObject: (io_service_t) diskServiceObject;

- (int) getSmartData:(NSMutableDictionary **)smartDataDict error:(NSMutableDictionary **)errorDict;
- (int) getRawSmartData:(ATASMARTData *)smartDataBuffer;
- (int) getSmartStatus:(NSMutableDictionary **)smartStatusDict error:(NSMutableDictionary **)errorDict;
- (int) getSmartReturnStatus: (Boolean *)exceededCondition;

-(IOReturn) startShortTest;
-(IOReturn) startExtendedTest;
-(IOReturn) abortTest;

-(IOReturn) openSMARTInterface;
-(IOReturn) closeSMARTInterface;
-(IOReturn) getDiagnosticStatus:(NSMutableDictionary **)diagStatusData error:(NSMutableDictionary **)errorDict;

@end
