//
//  WDUtilityStaticMethods.h
//  WesternDigitalS
//
//  Created by Clark Jackson on 11/10/09.
//  Copyright © 2014 Western Digital Technologies, Inc. All rights reserved.
//

//enum {
//	SEC_STAT_EncryptionOff	= 0,		// no password set (encrytion is always on but uses default password)
//	SEC_STAT_Locked			= 1,		// password set, drive locked
//	SEC_STAT_Unlocked		= 2,		// password set, drive unlocked
//	SEC_STAT_UnlocksExceeded	= 6,	// password set, unlock attempts exceeded
//	SEC_STAT_NoDEK			= 7			// no drive encrption key (should not happen in production)
//};

enum {
	kNotFound,
	kMyBookEssential,
	kMyBookElite,
	kMyBookStudio,
	kMyBookEssential_1130,
	kMyBookEssential_1132,
	kMyBookEssential_111A,
	kMyBookStudio_111C,
    kMyBookStudioLX_111D,

    kMyBookEssential_1150,
	kMyBookEssential_1140,
	kMyBookEssential_1152,
	kMyBookEssential_1142,

	kPassportIVEssential,
	kPassportIVElite,
	kPassportIVStudio,
	kPassportIVEssential_071A,
	kPassportIVEssential_0730,
    kPassportIVEssential_0732,
    kPassportIVStudio_071C,
    
    kPassportIVEssential_0750,
	kPassportIVEssential_0740,
	kPassportIVEssential_0752,
	kPassportIVEssential_0742
};

//enum {
//	k_After10  =  0,
//	k_After15,
//	k_After30,
//	k_After45,
//	k_After90,
//	k_Never
//};


// make C funtions visible
extern Boolean IsWholeMedia(io_service_t service,CFMutableDictionaryRef unserInfo);
extern void FindWholeMedia(io_service_t service,CFMutableDictionaryRef unserInfo);
extern void FindCDBSDName(io_service_t service,CFMutableDictionaryRef unserInfo);
extern void FindUniqueIdentifier(io_service_t service,CFMutableDictionaryRef unserInfo);


@class GradButton;
@class FramelessWindowPopup;
@class WebView;
//+++ @class HelpViewController;

@interface WDUtilityStaticMethods : NSObject { }

#pragma mark localization

+ (NSMutableArray*) arrayOfLocalizedTimeStrings;

#pragma mark paths

+ (NSString *) directoryContainingApp;
//  + (NSString *) volumeFromPath:(NSString *)thePath;
+ (void) createAliasToPath:(NSString*)path appendingName:(NSString*)appendedName;
+ (void) setupSidebarFavoriteOfPath:(NSString*)favoritePath iconPath:(NSString*)iconPath;

#pragma mark text

+ (NSString *) toSizeStringInBase10:(unsigned long long) bytes;
+ (NSString *) toSizeStringInBase10:(unsigned long long) bytes withBundle:(NSBundle*)theLocalizationBundle;
+ (NSString *) toSizeStringInBase2:(unsigned long long) bytes;
+ (NSString *) toSizeStringInBase2:(unsigned long long) bytes withBundle:(NSBundle*)theLocalizationBundle;

//  + (NSString *) humanReadableFreeSizeOfMountPath:(NSString *)path base10:(BOOL) returnBase10;
+ (NSString *) humanReadableSizeOfMountPath:(NSString *)path base10:(BOOL) returnBase10;
+ (NSString *) humanReadableSizeOfMountPath:(NSString *)path base10:(BOOL) returnBase10 withBundle:(NSBundle*)theLocalizationBundle;
+ (NSString *) humanReadableSizeOfBytes:(unsigned long long)bytes base10:(BOOL) returnBase10;
+ (NSString *) humanReadableSizeOfBytes:(unsigned long long)bytes base10:(BOOL) returnBase10 withBundle:(NSBundle*)theLocalizationBundle;
+ (NSArray *)  humanReadableByteSize:(unsigned long long)bytes base10:(BOOL) returnBase10;
+ (NSArray *)  humanReadableByteSize:(unsigned long long)bytes base10:(BOOL) returnBase10 withBundle:(NSBundle*)theLocalizationBundle;

//+ (void) buildUpBackgroundInFrame:(NSRect)viewFrame asKey:(BOOL)windowIsKey;
//+ (void) buildUpWelcomeBackgroundInFrame:(NSRect)viewFrame asKey:(BOOL)windowIsKey;
//+ (void) buildUpBackgroundForWhiteDialog:(NSRect)viewFrame;


+ (void)setMainFrameURL:(NSString *)URLString ofWebView:(WebView *) webView;
+ (void) setFontSizeForTextField:(NSTextField *) textField maxFontSize:(float)fontsize;
+ (void) setFontSizeForButton:(NSButton *) button maxFontSize:(int)fontsize;

#pragma mark ----------- Color Methods  -----------
+ (NSColor *) gray70Color;
+ (NSColor *) gray77Color;
+ (NSColor *) blackColor;
+ (NSColor *) blueColor;
+ (NSColor *) yellowColor;
+ (NSColor *) gray50Color;
+ (NSColor *) redColor;
+ (NSColor *) redColor50Alpha;
+ (NSColor *) blueColor50Alpha;


#pragma mark OS

+ (NSString *) operatingSystemWithPrefix;
+ (NSString *) operatingSystem;
+ (void)getSystemVersionMajor:(unsigned *)major
                        minor:(unsigned *)minor
                       bugFix:(unsigned *)bugFix;

+ (BOOL) usingTiger;
+ (BOOL) usingLeopard;
+ (BOOL) usingLion;
+ (BOOL) supportedOSVersion;
+ (NSMutableDictionary *) volumeInfoForVolume:(NSString*) theVolume;
+ (NSMutableDictionary *) volumeInfoForUID:(NSString*) theUID;
+ (BOOL) compareInstalledVersion:(NSString*) installedVersionString withUpdateVersion:(NSString*)updateVersionString;

+ (BOOL) volumeIsSoftwareWriteProtected:(NSString *)volumePath; // volume path does not use '/Volumes/' prefix
//+ (BOOL) volumeIsNTFSFormatted:(NSString *)volumePath;

#pragma mark ----------- Language Methods  -----------
+(NSArray *) languageCodes;
+(NSArray *) languageLabels;
+(NSArray *) isoLanguageCodes;
+(NSDictionary *) localizedNameForLanguageCode;
+(NSString *) threeLetterCodeForISOLanguageCode:(NSString*)langCode;
+(NSString*) nasLanguageCodeForPreferredLanguage;


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

+ (void) shakeWindow:(NSWindow *)window;
+ (int) senddiskutilCommandWithArguments: (NSArray *)arguments;

+ (CFStringRef) newLaunchBSDNodeName;
+ (BOOL) isOSVersionGreaterThan10Dot7;
+ (BOOL) isOSVersionLessThan10Dot7;

@end
