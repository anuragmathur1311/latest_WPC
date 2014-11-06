//
//  RESTControllerDiag.h
//  WD Discovery
//
//  Created by Hugh Dinh on 10/11/12.
//  Copyright 2012 Western Digital. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#define urlRequestTimeout 30

extern NSString* const SystemInformation;
extern NSString* const FirmwareInfo;
extern NSString* const StorageUsage;
extern NSString* const SystemState;
extern NSString* const FirmwareUpdate;
extern NSString* const USBDrives;
extern NSString* const USBUnlock;
extern NSString* const USBEject;

    //BOOL WDDebug;

@interface RESTControllerDiag : NSObject {

	NSLock				*nasLock;
	NSMutableDictionary *myMutableDictionary;
}
- (id)			initWithDict: (NSMutableDictionary *) deviceDictionary;
- (NSError *)	restCallForNasDevice: (NSString *) restCall;

+ (NSDictionary*) dictionaryFromXMLNode: (NSXMLNode *)rootNode;

@end
