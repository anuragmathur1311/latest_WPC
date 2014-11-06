//
//  FindFirewireDevice.h
//  DriveUnlock
//
//  Created by Bob on 3/9/09.
//  Copyright © 2014 Western Digital Technologies, Inc. All rights reserved.
//

#import "WDDevices.h"
#import <IOKit/firewire/IOFireWireLib.h>

@interface WDDevices (FindFirewireDevices)

- (id) registerNotificationsForFWPIDS: (NSArray *)supportedPids;

void FWDriveAddedCallback(void* refCon, io_iterator_t iter);
void FWDriveRemovedCallback(void* refCon, io_iterator_t iter);

- (void) FWDeviceAdded: (io_iterator_t) iterator;
- (void) FWDeviceRemoved: (io_iterator_t) iterator;

//void FWDeviceRemovedCallback(void* refCon, io_iterator_t iter);
//- (void) FWDeviceRemoved: (io_iterator_t) iterator;

@end
