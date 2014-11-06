//
//  WDStaticMethods.h
//  WesternDigitalS
//
//  Created by Clark Jackson on 11/10/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

//enum {
//	SEC_STAT_EncryptionOff	= 0,		// no password set (encrytion is always on but uses default password)
//	SEC_STAT_Locked			= 1,		// password set, drive locked
//	SEC_STAT_Unlocked		= 2,		// password set, drive unlocked
//	SEC_STAT_UnlocksExceeded	= 6,	// password set, unlock attempts exceeded
//	SEC_STAT_NoDEK			= 7			// no drive encrption key (should not happen in production)
//};

enum {
	kItNotFound
};

//enum {
//	kNotFound,
//	kMyBookEssential,
//	kMyBookElite,
//	kMyBookStudio,
//	kMyBookEssential_1130,
//	kMyBookEssential_1132,
//	kMyBookEssential_111A,
//	kMyBookStudio_111C,
//    kMyBookStudioLX_111D,
//
//    kMyBookEssential_1150,
//	kMyBookEssential_1140,
//	kMyBookEssential_1152,
//	kMyBookEssential_1142,
//
//	kPassportIVEssential,
//	kPassportIVElite,
//	kPassportIVStudio,
//	kPassportIVEssential_071A,
//	kPassportIVEssential_0730,
//    kPassportIVEssential_0732,
//    kPassportIVStudio_071C,
//    
//    kPassportIVEssential_0750,
//	kPassportIVEssential_0740,
//	kPassportIVEssential_0752,
//	kPassportIVEssential_0742
//};

enum {
	kAfter10  =  0,
	kAfter15,
	kAfter30,
	kAfter45,
	kAfter90,
	kNever
};


// make C funtions visible
extern Boolean IsWholeMedia(io_service_t service,CFMutableDictionaryRef unserInfo);
extern void FindWholeMedia(io_service_t service,CFMutableDictionaryRef unserInfo);
extern void FindCDBSDName(io_service_t service,CFMutableDictionaryRef unserInfo);
extern void FindUniqueIdentifier(io_service_t service,CFMutableDictionaryRef unserInfo);


@class GradButton;
@class FramelessWindowPopup;
@class WebView;
//+++ @class HelpViewController;

@interface WDStaticMethods : NSObject { }

#pragma mark ----------- Generic Static Methods  -----------
//  these methods are not associated with the BaseWindowController class

+(NSArray *) languageCodes;
+(NSArray *) languageLabels;
+(NSArray *) isoLanguageCodes;
+(NSDictionary *) localizedNameForLanguageCode;
+(NSString *) threeLetterCodeForISOLanguageCode:(NSString*)langCode;
+ (NSString*) nasLanguageCodeForPreferredLanguage;

+ (NSColor *) gray70Color;
+ (NSColor *) gray77Color;
+ (NSColor *) blueColor;
+ (NSColor *) yellowColor;
+ (NSColor *) gray50Color;
+ (NSColor *) redColor;
+ (NSColor *) redColor50Alpha;
+ (NSColor *) blueColor50Alpha;


+ (FramelessWindowPopup *) framelessWindowPopup;
//+++ + (HelpViewController *) helpViewController;

#pragma mark buttons

+ (void) prepareButton:(GradButton *)button;
+ (void) prepareButtonEco:(GradButton *)button withImage:(NSImage *)img;
+ (void) prepareEcoScrollButton:(GradButton *)button facing:(int)theDirection;
+ (void) prepareButton:(GradButton *)button withImages:(NSArray*)imageName;

#pragma mark paths

+ (NSString *) directoryContainingApp;
//  + (NSString *) volumeFromPath:(NSString *)thePath;

#pragma mark text

+ (NSString *) toSizeStringInBase10:(unsigned long long) bytes;
+ (NSString *) toSizeStringInBase2:(unsigned long long) bytes;

//  + (NSString *) humanReadableFreeSizeOfMountPath:(NSString *)path base10:(BOOL) returnBase10;
+ (NSString *) humanReadableSizeOfMountPath:(NSString *)path base10:(BOOL) returnBase10;
+ (NSString *) humanReadableSizeOfBytes:(unsigned long long)bytes base10:(BOOL) returnBase10;
+ (NSArray *)  humanReadableByteSize:(unsigned long long)bytes base10:(BOOL) returnBase10;

+ (void) buildUpBackgroundInFrame:(NSRect)viewFrame asKey:(BOOL)windowIsKey;
+ (void) buildUpWelcomeBackgroundInFrame:(NSRect)viewFrame asKey:(BOOL)windowIsKey;
+ (void) buildUpBackgroundForWhiteDialog:(NSRect)viewFrame;


+ (void)setMainFrameURL:(NSString *)URLString ofWebView:(WebView *) webView;
+ (void) setFontSizeForTextField:(NSTextField *) textField maxFontSize:(float)fontsize;

#pragma mark OS

+ (NSString *) operatingSystemWithPrefix;
+ (NSString *) operatingSystem;
+ (void)getSystemVersionMajor:(unsigned *)major
                        minor:(unsigned *)minor
                       bugFix:(unsigned *)bugFix;

+ (BOOL) usingTiger;
+ (BOOL) supportedOSVersion;
+ (NSMutableDictionary *) volumeInfoForVolume:(NSString*) theVolume;
+ (NSMutableDictionary *) volumeInfoForUID:(NSString*) theUID;
+ (BOOL) compareInstalledVersion:(NSString*) installedVersionString withUpdateVersion:(NSString*)updateVersionString;

+ (BOOL) volumeIsSoftwareWriteProtected:(NSString *)volumePath; // volume path does not use '/Volumes/' prefix
//+ (BOOL) volumeIsNTFSFormatted:(NSString *)volumePath;

#pragma mark ========================== PIDs ==========================
// number of characters available for e-ink
+ (NSNumber *) availableLabelCharactersByPID:(NSString *)thePID;
+ (int) availableLabelCharactersByProductIndex:(int)theIndex;
// returning product name
+ (NSString *) productNameByPID:(NSString *)pid;
// return pids
//  + (NSString *) pidByProductName:(NSString *)theName;
+ (NSString *) pidByProductIndex:(unsigned)theIndex;
+ (NSArray *) apolloPIDs;
+ (NSArray *) apolloPIDDecStrings;
// key/value = pid/productName
//  + (NSDictionary *) apolloProductInfo;
// product index
//+++ + (unsigned) targetDeviceProductIndex;
+ (unsigned) productIndexByPIDHexString:(NSString *)thePID;
+ (unsigned) productIndexByPIDDec:(int)pid;

+ (NSString *)computerName;
+ (NSString*)shortUserName;
//+++ + (int) encryptionStatusForBDSName:(NSString *)deviceSerialNumber;
//+++ + (NSString *) interpretSecurityState:(int)securityState;


+ (CFStringRef) getLaunchBSDNodeName;

@end
