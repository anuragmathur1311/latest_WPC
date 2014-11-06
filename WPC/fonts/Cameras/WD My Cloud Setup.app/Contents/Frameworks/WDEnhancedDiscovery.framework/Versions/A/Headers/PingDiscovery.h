//
//  PingDiscovery.h
//  WD Discovery
//
//  Created by Hugh Dinh on 7/3/12.
//  Copyright (c) 2012 Western Digital. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "PingService.h"

#define urlRequestTimeout 30

@protocol PingDiscoveryDelegate

@optional
-(void)serviceDidUpdate;

@end // PingDiscoveryDelegate

@interface PingDiscovery : NSObject <PingServiceDelegate>
{
    PingService *    _pinger;
    NSTimer *       _sendTimer;
    BOOL            _done;
    PingService* pinger;
	id                  delegate;
	NSMutableDictionary * myMutableDictionary;
	NSMutableDictionary	*nasDevices;
    NSString *stringAddress;
	NSTimer				*updateServiceInformationTimer;
}
- (void)runWithHostName:(NSString *)hostName;
- (void)browseForServices;
- (void) setDelegate: (id<PingDiscoveryDelegate>) object;
- (void) updateServiceInformation;
- (NSArray *) arrayOfDiscoveredDevices;

- (NSError *) systemInformationForNasDevice: (NSMutableDictionary *) deviceDictionary;
+ (NSDictionary*) dictionaryFromXMLNode: (NSXMLNode *)rootNode;

@end
