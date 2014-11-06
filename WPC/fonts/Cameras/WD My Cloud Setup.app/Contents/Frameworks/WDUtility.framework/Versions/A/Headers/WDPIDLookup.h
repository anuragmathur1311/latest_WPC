//
//  WDPIDLookup.h
//  SmartWare
//
//  Created by Hugh Dinh on 12/31/10.
//  Copyright 2013 Western Digital Technologies, Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

enum {
	kBase				= 0,
	kBaseReversed		= 1,
	kBaseLocked			= 2,
	kBaseReversedLocked	= 3,
	kEco				= 4,
	kEcoSelected		= 5
};

enum {
	kNoVCD				= 0,
	kMiniVCD			= 1,
	kLargeVCD			= 2
};

extern NSString* const defaultPlist;

// image string constants
extern NSString * const BASE;
extern NSString * const BASE_REVERSED;
extern NSString * const BASE_LOCKED;
extern NSString * const BASE_REVERSED_LOCKED;
extern NSString * const ECO;
extern NSString * const ECO_SELECTED;
extern NSString * const VOLUME_ICON_NAME;

@interface WDPIDLookup : NSObject {
	
	NSDictionary * wdPidDictAll;
	//NSDictionary * wdPidDict;
	//NSMutableArray * wdPidDictArray;
	//NSString*	pListPath;

}

-(id) init;
-(int) supportedPIDCount;
-(NSString *)productNameByPID:(NSString *) pid;
-(NSString *)multilineProductNameByPID:(NSString *) pid;
-(NSString *)defaultVolumeNameByPID:(NSString *) pidHex;
-(NSString *)shortDefaultVolumeNameByPID:(NSString *) pidHex;

-(NSArray*) supportedPIDsDecimal;
-(NSArray*) supportedPIDsHex;
-(NSArray*) supportedPIDsValue;
-(NSArray*) supportedOnyxPIDsValue;
-(NSArray*) supportedEncryptedOnyxPIDsValue;
-(NSArray*) supportedEncryptedPIDsValue;
-(NSArray*) supportedFormatterPIDsValue;

-(int )availableLabelCharactersByPID:(NSString *)pidHex;
-(NSString *)imageNameByPID:(NSString *) pidHex type:(NSString *) imageType;
-(NSImage *)imageByPID:(NSString *) pidHex type:(NSString *) imageType;
-(BOOL) hasEncryptionByPID: (NSString *) pidHex;
-(BOOL) isOnyxOrLaterDriveByPID: (NSString *) pidHex;
-(BOOL) isHWRAIDSupported: (NSString *) pidHex;

-(int) vcdByPID: (NSString *) pidHex;
-(int) vcdDisableSupportMinFWVerByPID: (NSString *) pidHex;

+(NSDictionary*) dictFromPlist;
+(int) wdSupportedPIDCount;
+(NSArray*) wdSupportedPIDsDecimal;
+(int )wdAvailableLabelCharactersByPID:(NSString *)pidHex;
+(NSString *)wdProductNameByPID:(NSString *) pidHex;
+(NSString *)wdImageNameByPID:(NSString *) pidHex type:(NSString *) imageType;

@end
