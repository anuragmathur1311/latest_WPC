//
//  WDMonitorFile.h
//  WDUtility
//
//  Created by bob hahn on 7/1/11.
//  Copyright © 2014 Western Digital Technologies, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol WDMonitorFileDelegate <NSObject>
@optional
- (void) fileModified;
- (void) fileAttributesChanged;
- (void) fileRenamed;
- (void) fileDeleted;
@end

@interface WDMonitorFile : NSObject
{
	NSFileManager *fileManager;
	NSThread * FileMonitorThread;
    id <WDMonitorFileDelegate>delegate;
}

- (id) initWithFileAtPath: (NSString *) fFilePath;
- (void) setDelegate: (id<WDMonitorFileDelegate>) object;
- (void) monitorFileState: (id) filePath;

@end