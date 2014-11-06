//
//  diskRoutines.h
//  WDUtility
//
//  Created by Bob Hahn on 3/17/11.
//  Copyright © 2014 Western Digital Technologies, Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface diskRoutines : NSObject {

}

+ (UInt64) freeSpaceForDeviceWithSerialNumber: (NSString *) serialNumber;
+ (UInt64) freeSpaceForDeviceWithBSDName: (NSString *) bsdName;

@end
