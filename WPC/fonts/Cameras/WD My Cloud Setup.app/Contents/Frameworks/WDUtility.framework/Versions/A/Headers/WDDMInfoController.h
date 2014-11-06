//
//  WDDMInfoController.h
//  RoundTransparentWindow
//
//  Created by Bob Hahn on 1/21/09.
//  Copyright © 2014 Western Digital Technologies, Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <CommonCrypto/CommonDigest.h>
#import "WDDMcommon.h"
//#import "WDDevice+SendSCSICommand.h"

#define MAX_COOKED_PASSWORD_SIZE	32

typedef struct ConnectionState *ConnectStateRef;

typedef bool (*ConnectionCallbackProcPtr) (ConnectStateRef connection, const PacketHeader* packet, void* refCon);
int RegisterListener(ConnectStateRef connection,  CFRunLoopRef runLoop, ConnectionCallbackProcPtr callback, void* refcon);
bool ReceivedInfo(ConnectStateRef connection, const PacketHeader* packet , void* refcon);
void ConnectionHasData(CFSocketRef sock, CFSocketCallBackType type, CFDataRef address, const void*  data, void*  info);
void ShutDownConnection(ConnectStateRef connection);
typedef struct ConnectionState {
	OSType						magic;
	int							sockFD;
	CFSocketRef					sockCF;
	CFRunLoopSourceRef			runLoopSrc;
	CFMutableDataRef			bufferedPackets;
	ConnectionCallbackProcPtr	theCallback;
	void*						callbackRefCon;
} ConnectionState;

@interface WDDMInfoController : NSObject
{
	UInt32					numDisks;
	UInt32					numBadDisks;
	NSMutableArray			*devices;
	ConnectionState*		connection;			// For communicating commands
	ConnectionState*		feedback;			// Feedback portion
	BOOL					connectionOpen;
	NSMutableDictionary		*fAttachedDiskInfo;
}

+ (WDDMInfoController*) instance;

-(int) OpenServerConnection;
-(void) Listen;
-(int) FeedBackRPC:(PacketHeader*) request :(PacketHeader*) reply :(UInt32) replySize;
-(int) Communicate:(void*) dataPacket :(size_t) packetLen :(void*) replyPacket :(size_t) replyLen;
-(void) CloseServerConnection;
-(int) ConnRead:(void*) buffer :(size_t) buffLen;
-(int) ConnWrite:(void*) buffer :(size_t) buffLen;

-(int) GetDiskInquiryDataforBSDName:(NSString *)bsdName deviceinformation:(DeviceInfoBlock*) DevInfo;
-(int) SendSCSICommandUsingWDDM:(NSString *)bsdName scsiCommand:(scsiData *) scsiCommand;
-(void) ShutDownConnection;

@end
