//
//  WDDiagnostics.h
//  WD Discovery
//
//  Created by Hugh Dinh on 1/8/13.
//  Copyright 2013 Western Digital. All rights reserved.
//

#import "RESTControllerDiag.h"
#import "WDLog.h"
#import <Cocoa/Cocoa.h>
#import <QTKit/QTKit.h>
#import <QTKit/QTMovie.h>


extern NSString* const RestValidation;
extern NSString* const NasValidation;
extern NSString* const NasState;
extern NSString* const HttpValidation;
extern NSString* const ShareAccess;
extern NSString* const FileCopying;
extern NSString* const DataStreaming;

extern NSString* const NoError;
extern NSString* const Good;
extern NSString* const Bad;

extern NSString* const ResultKey;
extern NSString* const ErrorKey;
extern NSString* const CodeKey;
extern NSString* const MessageKey;
extern NSString* const SystemErrorKey;

extern NSString* const SourcePath;
extern NSString* const DestinationPath;
extern NSString* const Mp3DestinationPath;

extern NSString* const RestValidationFailed;
extern NSString* const RestValidationFailedToComplete;
extern NSString* const NasValidationFailed;
extern NSString* const NasStatusFailed;
extern NSString* const HttpValidationFailed;
extern NSString* const ShareAccessFailed;
extern NSString* const FileCopyFailed;
extern NSString* const DataStreamingFailed;
extern NSString* const UnknownError;

enum {
	kRestValidation = 1,
	kNasValidation = 2,
	kNasState = 3,
	kHttpValidation = 4,
	kShareAccess = 5,
	kFileCopying = 6,
	kDataStreaming = 7,
	kLastTest = 8
};

@class WDNetworkService, WDURLService;

@protocol WDDiagnosticsDelegate

@optional
-(void)diagnosticsDidUpdate;

@end // WDDiagnosticsDelegate

@interface WDDiagnostics : NSObject {
	
	NSString*                   currentIP;
	NSMutableDictionary*		mutableDiagnosticsDict;
	NSMutableDictionary*		wdNasDict;
 	RESTControllerDiag*         restAPI;
    BOOL                        diagnosticUpdate;
	id                          delegate;
	int                         testNumber;
	int                         testCount;
	NSString*                   testName;
	BOOL                        beginTest;
	NSMutableDictionary*		mutableDiagnosticsResults;
    NSString*                   diagnosticsPlist;
    NSString*                   Mp3SourcePath;
	NSTimer *                   reloadTimer; 
	
    WDNetworkService *          myNetworkService;
    WDURLService *              myURLService;
	
	BOOL                        useWDNetworkService;
	NSString *                  urlField;
	char                        randomString[512];
	NSString *                  key;
	NSData *                    keyData;
	NSString *                  keyPathStr;
	BOOL                        abortTest;
	BOOL                        HttpValidating;

	WDLog *                     myLog;
    FSVolumeRefNum              publicVolumeRefNum;
    QTMovie*                    audioPlayer;
	
}

-(id)initWithIp: (NSString *) IP;
- (void) setDelegate: (id<WDDiagnosticsDelegate>) object;
- (void) runTestSuite;
- (NSDictionary *) diagnosticsUpdate;
- (NSDictionary *) diagnosticsResults;
- (void)startDiagnostics;
- (void) stopDiagnostics;

@end
