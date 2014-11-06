//
//  ARPDiscovery.h
//  WD Discovery
//
//  Created by Hugh Dinh on 10/12/12.
//  Copyright 2012 Western Digital. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <Foundation/NSNetServices.h>
#import "PingService.h"
#import "RESTController.h"

#define urlRequestTimeout 30

@protocol ARPDiscoveryDelegate

@optional
-(void)arpDidUpdate;

@end // NASDiscoveryDelegate


@interface ARPDiscovery : NSObject {
    
	BOOL	debug;
    
	NSNetServiceBrowser *serviceBrowser;
	NSNetService		*service;
	int					resolveCount;
	NSMutableDictionary	*nasDevices;
	NSMutableDictionary *currentNasDevices;
	NSMutableArray		*resolveQueue;
	id                  delegate;
    BOOL                delegateRespondsToServiceDidUpdate;
	NSTimer				*updateServiceInformationTimer;
	NSTimer				*parsingArpCacheTimer;
	//	NSLock				*ServiceInformationLock;
	NSLock				*nasDevicesLock;
	BOOL				foundWDNasDevice;
	NSDictionary*		PreviousDictionary;
    NSArray*            modelsOfInterest;
	RESTController*		restAPI;
    
    //arpTable
	NSMutableArray* ipArray;
	NSMutableArray* hostArray;
    NSMutableDictionary *currentMutableDictionary;
}


- (id)initWithModels:(NSArray*)models;
- (void) getNASInformation: (id) key;
- (NSDictionary*) getCopyOfNasDevicesDictionary;

- (void) setDelegate: (id<ARPDiscoveryDelegate>) object;


@end
