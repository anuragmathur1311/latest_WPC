//
//  VCDsupport.h
//  WDUtility
//
//  Created by Bob Hahn on 4/14/11.
//  Copyright © 2014 Western Digital Technologies, Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "WDDevice.h"

enum
{
    DISABLED = 0,
    ENABLED = 1
};

enum
{
    UPDATE_NONE = 0,
    UPDATE_ISO_TO_BOTH = 1,
    UPDATE_0_TO_1 = 2,
    UPDATE_1_TO_0 = 3
};



@interface WDDevice (VCDsupport)

- (int) enableVCD:(NSMutableDictionary **)errorDict;
- (BOOL) disableVCD:(NSMutableDictionary **)errorDict;
- (BOOL) VCDState:(NSMutableDictionary **)errorDict;
- (BOOL) isVCDSupported:(NSMutableDictionary **)errorDict;
- (int) setVCDMediaValid;
- (BOOL) isoImageForVCD;
- (void) updateVCD;
- (int) isVCDValid;
- (void) setVCDUpdateMode:(int)mode;

@end
