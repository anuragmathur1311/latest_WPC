//
//  XMLparser.h
//  WD Universal Firmware Updater
//
//  Created by Hugh Dinh on 4/6/10.
//  Copyright 2013 Western Digital Technologies, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

@protocol NSXMLParserDelegate;


@interface XMLparser : NSObject <NSXMLParserDelegate> 
{
	NSString *currentStringValue;
	NSString *updaterSize;
	NSString *httpStatus;
	NSString *updaterVersion;
	NSString *updaterURL;
	NSString *updaterAlert;
	NSString *updaterReadme;
	
	BOOL	debug;
}

@property (readwrite,retain) NSString* currentStringValue, *updaterSize, *httpStatus, *updaterVersion, *updaterURL, *updaterAlert, *updaterReadme;

- (BOOL)parseXMLData:(NSData*)xmlData;
- (BOOL)parseXMLFile:(NSString *)pathToFile;

@end

