//
//  WDDiscovery.h
//  WD Discovery
//
//  Created by Hugh Dinh on 11/14/12.
//  Copyright 2012 Western Digital. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "BonjourDiscovery.h"
#import "ARPDiscovery.h"
#import "PingDiscovery.h"
#import <WDDiagnostics/WDLog.h>

@protocol WDDiscoveryDelegate

@optional
-(void)serviceDidUpdate;

@end // BonjourDiscoveryDelegate

@interface WDDiscovery : NSObject <ARPDiscoveryDelegate, BonjourDiscoveryDelegate>
{
	id                  delegate;
	NSDictionary*       dictionaryByBonjour;
	NSDictionary*       dictionaryByArp;
	BonjourDiscovery*   devicesByBonjour;
	ARPDiscovery*       devicesByArp;
    BOOL                delegateRespondsToServiceDidUpdate;
	NSLock*             nasDevicesLock;
	NSDictionary*       nasDictCopy;
    NSDictionary*            arrayOfHostName;
	WDLog *             myLog;

}
- (id) initWithModels:(NSArray *)modelsOfInterest;
-(void)setDelegate:(id<WDDiscoveryDelegate>)object;
- (NSDictionary*) discoveredDictionary;
- (NSDictionary*) arrayOfHostName;

@end
