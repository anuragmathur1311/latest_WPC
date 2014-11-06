//
//  findThunderboltDevices.h
//  DriveUnlock
//
//  Created by Bob on 3/9/09.
//  Copyright © 2014 Western Digital Technologies, Inc. All rights reserved.
//

#import "WDDevices.h"

@interface WDDevices (findThunderboltDevices)

- (bool)findThunderboltDevicesWithPIDS: (NSArray *) supportedPIDS;

@end
