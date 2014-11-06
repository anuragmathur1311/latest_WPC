//
//  NASDiscovery.h
//  StatusMenu
//
//  Created by Bob Hahn on 7/14/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <Foundation/NSNetServices.h>

#define urlRequestTimeout 30
#define	DEBUG_BUILD_CONFIGURATION

extern NSString* const RaidUpdateCount;
extern NSString* const ShareUpdateCount;
extern NSString* const UsbUpdateCount;
extern NSString* const AlertUpdateCount;
extern NSString* const SystemStateUpdateCount;
extern NSString* const FirmwareUpdateCount;

//*************************************************************************
//  @protocol NASDiscoveryDelegate
//*************************************************************************
@protocol NASDiscoveryDelegate

@optional
-(void)serviceDidUpdate;

@end // NASDiscoveryDelegate

@interface NASDiscovery : NSObject
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
<NSNetServiceDelegate, NSNetServiceBrowserDelegate>
#endif
{
	BOOL	debug;
	
	NSNetServiceBrowser *serviceBrowser;
	NSNetService		*service;
	int					resolveCount;
	NSMutableDictionary	*nasDevices;
	id                  delegate;
    BOOL                delegateRespondsToServiceDidUpdate;
	NSTimer				*updateServiceInformationTimer;
	NSLock				*nasDevicesLock;
	BOOL				foundWDNasDevice;
	NSDictionary*		PreviousDictionary;
	bool				informDelegate;
	
	int					raidUpdateCounter;
	int					shareUpdateCounter;
	int					usbUpdateCounter;
	int					alertUpdateCounter;
	int					systemStateUpdateCounter;
	int					firmwareUpdateCounter;
	
	BOOL				doRaidUpdate;
	BOOL				doShareUpdate;
	BOOL				doUsbUpdate;
	BOOL				doAlertUpdate;
	BOOL				doSystemStateUpdate;
	BOOL				doFirmwareUpdate;
    
}
//- (id) init;
//- (void) startServerBrowserControllerThread;

- (NSDictionary*) copyOfNasDevicesDictionary;
- (NSError *) systemInformationForNasDevice: (NSMutableDictionary *) deviceName;
- (NSError *) usbDrivesForNasDevice: (NSMutableDictionary *) deviceDictionary;
- (NSError *) userRaidStatusForNasDevice: (NSMutableDictionary *) deviceDictionary;
- (NSError *) sharesForNasDevice: (NSMutableDictionary *) deviceDictionary;
- (NSError *) firmwareInfoForNasDevice: (NSMutableDictionary *) deviceName;
- (NSError *) storageUsageForNasDevice: (NSMutableDictionary *) deviceName;
- (NSError *) systemStateForNasDevice: (NSMutableDictionary *) deviceName;
- (NSError *) firmwareUpdateStatusForNasDevice: (NSMutableDictionary *) deviceName;

+ (NSDictionary*) dictionaryFromXMLNode: (NSXMLNode *)rootNode;

- (void) setDelegate: (id<NASDiscoveryDelegate>) object;
- (void) updateServiceInformation;

@end
