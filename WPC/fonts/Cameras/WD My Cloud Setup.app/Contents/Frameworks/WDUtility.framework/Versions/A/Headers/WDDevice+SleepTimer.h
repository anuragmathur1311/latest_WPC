//
//  WDDevice+SleepTimer.h
//  WDUtility
//
//  Created by Bob Hahn on 4/12/11.
//  Copyright © 2014 Western Digital Technologies, Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "WDDevice.h"

enum {
	k_After10  =  0,
	k_After15,
	k_After30,
	k_After45,
	k_After90,
	k_Never
};

@interface WDDevice (SleepTimer)

-(int) sleepTimer:(int*)value error:(NSMutableDictionary **)errorDict;
-(int) setSleepTimer:(int)value error:(NSMutableDictionary **)errorDict;

@end
