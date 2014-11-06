//
//  BonjourDiscovery.h
//  WD Discovery
//
//  Created by Hugh Dinh on 10/11/12.
//  Copyright 2012 Western Digital. All rights reserved.
//

#import "RESTController.h"
#import <Cocoa/Cocoa.h>
#import <Foundation/NSNetServices.h>

#define urlRequestTimeout 30

//*************************************************************************
//  @protocol BonjourDiscoveryDelegate
//*************************************************************************
@protocol BonjourDiscoveryDelegate

@optional
-(void)bonjourDidUpdate;

@end // BonjourDiscoveryDelegate


@interface BonjourDiscovery : NSObject 
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
<NSNetServiceBrowserDelegate>
#endif
{
	BOOL	debug;
	
	NSNetServiceBrowser *   serviceBrowser;
	NSNetService*           service;
	int                     resolveCount;
	NSMutableDictionary	*   nasDevices;
	NSMutableDictionary *   currentNasDevices;
	NSMutableArray*         resolveQueue;
	id                      delegate;
    BOOL                    delegateRespondsToServiceDidUpdate;
	NSTimer*                updateServiceInformationTimer;
	NSLock*                 nasDevicesLock;
	BOOL                    foundWDNasDevice;
	NSDictionary*           PreviousDictionary;
	bool                    informDelegate;
	RESTController*         restAPI;
	NSArray*                modelsOfInterest;
    NSMutableDictionary*    dictOfAllDiscovered;
	NSDictionary*           nasDevicesCopy;
	
}

- (id) initWithModels:(NSArray*)models;
- (NSDictionary*) getCopyOfNasDevicesDictionary;
- (void) setDelegate: (id<BonjourDiscoveryDelegate>) object;
- (void) updateServiceInformation;
- (void) stopBrowseForServices;
- (NSDictionary*) arrayOfAllHostName;

@end
