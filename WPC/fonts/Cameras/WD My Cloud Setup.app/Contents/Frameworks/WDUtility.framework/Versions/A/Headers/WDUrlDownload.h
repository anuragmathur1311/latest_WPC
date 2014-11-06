//
//  WDUrlDownload.h
//  WD Universal Firmware Updater
//
//  Created by Hugh Dinh on 4/6/11.
//  Copyright © 2014 Western Digital Technologies, Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

enum {
	kNoErr = 0,
	kUrlStrErr = 1,
	kUrlReqErr = 2,
	kDetectedDriveLocked = 3,
	kSystemBootDiskSelected = 4,
	kDetectedUnknownDevice = 5,
	kDetectedBrickedDevice = 6
};

@interface WDUrlDownload : NSDocument // <NSURLDownloadDelegate>
{
    BOOL				downloading;
	BOOL				overrideExistingFile;
    BOOL				downloadIsIndeterminate;
	BOOL				downloadIsFinished;
	BOOL				downloadFailed;
    float				downloadProgress;
    long long			expectedContentLength;
    long long			downloadedSoFar;
    
    NSURLDownload*		download;
	NSString*			downloadFileName;
	
	NSString*			downloadFolder;
	
	NSString*			destinationPath;

 	NSProgressIndicator*	progressIndicator;
	NSWindow*				window;
	NSTextField*			progressMsg;
	NSString*				downloadError;
}

- (void)progressBar;
- (void)startDownload:(id)sender atURL:(NSString *)updaterFileURL toFolder:(NSString *)folder;
- (BOOL)isDownloadFinished;
- (BOOL)isDownloadFailed;
- (NSString*)getDownloadError;
- (NSString *)getFileName;

@end
