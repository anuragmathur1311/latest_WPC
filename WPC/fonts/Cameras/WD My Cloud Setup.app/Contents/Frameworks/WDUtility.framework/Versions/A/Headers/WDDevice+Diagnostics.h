//
//  WDDevice_Diagnostics.h
//  WDUtility
//
//  Created by Bob Hahn on 4/12/11.
//  Copyright © 2014 Western Digital Technologies, Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "WDDevice.h"


@interface WDDevice (Diagnostics)

- (int) startShortTest:(NSMutableDictionary **)errorDict;
- (int) startExtendedTest:(NSMutableDictionary **)errorDict;
- (int) abortTest:(NSMutableDictionary **)errorDict;
- (int) getSmartData:(NSMutableDictionary **)smartDataDict forSlot: (int)slot error:(NSMutableDictionary **)errorDict;
- (int) getSmartStatus:(NSMutableDictionary **)smartStatusDict forSlot: (int)slot error:(NSMutableDictionary **)errorDict;
- (int) getDiagnosticStatus:(NSMutableDictionary **)diagStatusData forSlot: (int)slot error:(NSMutableDictionary **)errorDict;

@end
