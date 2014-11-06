//
//  WDLog.h
//  WDDiagnostics
//
//  Created by hugh dinh on 2/27/13.
//  Copyright (c) 2013 hugh dinh. All rights reserved.
//

#import <Foundation/Foundation.h>

extern NSString * const wdLogPath;
extern NSString* const dividerPattern;

@interface WDLog : NSObject
{
    NSString *      logFilePath;
    NSString *      logFileName;
    NSFileHandle *  logFileHandle;
    
}

- (id) initFor:(NSString *) logName;
- (void) wdLog:(NSString *) logStr;
- (void) wdLogDivider;

@end
