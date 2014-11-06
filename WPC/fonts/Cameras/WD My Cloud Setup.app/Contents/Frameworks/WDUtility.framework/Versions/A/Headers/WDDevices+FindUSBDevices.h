//
//  FindUSBDevice.h
//  DriveUnlock
//
//  Created by Bob on 3/9/09.
//  Copyright © 2014 Western Digital Technologies, Inc. All rights reserved.
//

#import "WDDevices.h"
#import <IOKit/usb/IOUSBLib.h>


@interface WDDevices (FindUSBDevices)

- (id) registerNotificationsForUSBPIDS: (NSArray *)supportedPids;

void USBDeviceAddedCallback(void* refCon, io_iterator_t iter);
void USBDeviceRemovedCallback(void* refCon, io_iterator_t iter);
//void USBDriveAddedCallback(void* refCon, io_iterator_t iter);
//void USBDriveRemovedCallback(void* refCon, io_iterator_t iter);
void USBMediaAddedCallback(void* refCon, io_iterator_t iterator);

- (void) USBDeviceAdded: (io_iterator_t) iterator;
- (void) USBDeviceRemoved: (io_iterator_t) iterator;
//- (void) USBDriveAdded: (io_iterator_t) iterator;
//- (void) USBDriveRemoved: (io_iterator_t) iterator;

@end
