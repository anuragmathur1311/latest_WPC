//
//  PrintBuffer.h
//  WDUtility
//
//  Created by Bob Hahn on 3/22/11.
//  Copyright © 2014 Western Digital Technologies, Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface PrintBuffer : NSObject
{
}

+ (int) printBuffer:(NSString *)name buffer:(UInt8 *)buffer bufferLength:(UInt32) length;
+ (int) printBuffer:(NSString *) name buffer:(UInt8 *) inBuffer toBuffer:(char *) outBuffer bufferLength:(UInt32) length;

@end
