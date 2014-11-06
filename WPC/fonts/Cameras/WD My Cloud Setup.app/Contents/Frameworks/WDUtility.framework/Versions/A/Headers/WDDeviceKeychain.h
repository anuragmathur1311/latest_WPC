//
//  WDDeviceKeychain.h
//  WDUtility
//
//  Created by Bob Hahn on 4/19/11.
//  Copyright © 2014 Western Digital Technologies, Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface WDDeviceKeychain : NSObject
{
//    SecKeychainItemRef		itemRef;
//    NSString *				accountName;    
}

//@property (readwrite,copy)	NSString * accountName;

-(NSString *) passwordForSerialNumber: (NSString *) serialNumber;
-(OSStatus) setPassword: (NSString *) password forSerialNumber: (NSString *) serialNumber;
-(OSStatus) setPasswordWithFullAccess: (NSString *) password forSerialNumber: (NSString *) serialNumber;
-(OSStatus) deletePasswordForSerialNumber: (NSString *) serialNumber;

@end
