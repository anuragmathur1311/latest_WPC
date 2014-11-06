//
//  OB03Controller.h
//  RoundTransparentWindow
//
//  Created by Clark Jackson on 10/31/08.
//  Copyright 2008 Western Digital. All rights reserved.
//


enum dialogScreen {
    kPasswordDialog,
    kNoticeDialog, // not used
    kErasingDialog,
    kDiskFullDialog, // not used
    kUnformattedDriveDialog,
    kMinimumRequirementDialog,
    kAboutDialog,
    kConfirmOverwriteDialog,
    kDriveUnpluggedDialog, // not used
    kCancelRetrievingDialog, // not used
    kUpdateAvailableDialog,
    kApplyBackupChangesDialog,
    kStopBackupDialog,
    kSetupDriveBeforeContinuingWithLabelDialog,
    kSetupDriveBeforeContinuingWithoutLabelDialog,
    kStartBackupBeforeContinuingDialog,
    kVisitBackupBeforeContinuingDialog, // not used
    kNoConnectionToInternetDialog,
    kNotWritableDialog,
    kQuitInstall,
    kPerformUpate, // not used
    kReformatDriveDialog, // used by installer only
    kChangeBUScheduleDialog, // SmartWare debugging only
    kSwitchingBackupPlanDialog,
    kChangingPreferredBackupPlanDialog
};

@class WDFramelessDialogView;
@class GradButton;
@class LinkButton;
@class WDHelpViewController;

@interface WDFramelessDialogController : NSWindowController
{
    //  if it's not it's a PassPort
    BOOL                            _isAMyBook;
    BOOL                            keepGoing;
    NSModalSession                  session;
    BOOL                            isAwake;
    IBOutlet WDFramelessDialogView* framelessDialogView;
    
	int                             _state,
                                    _buttonValue;

	NSString                        *_bsdName,
                                    *_driveName,
                                    *_overwriteString;

	IBOutlet NSButton               *fWarningCB;
	IBOutlet NSButton               *fPasswordRevealHintButton;
	IBOutlet NSButton               *fNoticeCB;
	IBOutlet LinkButton             *eulaButton;
	IBOutlet LinkButton             *privacyButton;
	IBOutlet NSButton               *overwriteUseThisChoiceCB;

	IBOutlet NSTextField            *fWindowTitleTF;
	IBOutlet NSTextField            *fPasswordInfoTF;
	IBOutlet NSTextField            *fPasswordEnterTF;
	IBOutlet NSTextField            *fPasswordHintTF;
	IBOutlet NSTextField            *fPasswordHintLabel;
	IBOutlet NSTextField            *fNoticeInfoTF;
	IBOutlet NSTextField            *fWarningInfoTitleTF;
	IBOutlet NSTextField            *fWarningInfoTF;
	IBOutlet NSTextField            *fWarningInfo2TF;
	IBOutlet NSTextField            *fWarningLabel;
	IBOutlet NSTextField            *fWarningIUnderstandLabel;
	IBOutlet NSTextField            *aboutInfo;
   	IBOutlet NSTextField            *overwriteTF;
	IBOutlet NSTextField            *_passwordLabel;
    IBOutlet NSTextField            *_guidanceText;
    IBOutlet NSTextField            *fNotWritableInfoTF;
    IBOutlet NSTextField            *fUpdateInfoTF;

	IBOutlet NSImageView            *fNoticeWarningImage;
	IBOutlet NSImageView            *fWarningWarningImage;
    IBOutlet NSImageView            *iconImageView;
    IBOutlet NSImageView            *overwriteIconImageView; 
    IBOutlet NSImageView            *fNotWritableWarningImage;

	IBOutlet NSProgressIndicator    *_progBarDiag;

	IBOutlet GradButton             *fInfoButton;
	IBOutlet GradButton             *fButton1;
	IBOutlet GradButton             *fButton2;
	IBOutlet GradButton             *fButton3;
    
    // kChangeBUScheduleDialog
    IBOutlet NSTextField            *buIntervalInfoTF;
    IBOutlet NSTextField            *buIntervalEntryTF;
	IBOutlet NSButton               *disableContinuousBUCB;


	IBOutlet NSView                 *_passwordView;
	IBOutlet NSView                 *_noticeView;
	IBOutlet NSView                 *_warningView;
    IBOutlet NSView                 *_aboutView;
    IBOutlet NSView                 *_overwriteView;
    IBOutlet NSView                 *_notWritableView;
    IBOutlet NSView                 *_updateView;
    IBOutlet NSView                 *_buIntervalView;

    id  sharedSWInfoController;
}

#pragma mark ----------- Generic Static Methods  -----------
+ (WDHelpViewController *) helpViewController;


- (IBAction) doInfo:(id)sender;
- (IBAction) doPrivacyPolicy:(id)sender;

- (void) doStateChange:(int)state;
- (IBAction) doButton1:(id)sender;
- (IBAction) doButton2:(id)sender;
- (IBAction) doButton3:(id)sender;
- (void) close; // for scripting support
- (int) fButtonValue;
- (IBAction) doRevealHint:(id)sender;
- (void) showWindow;
- (void) showWindowModally;

-(IBAction) doIUnderstand:(id)sender;

// for handling threaded progress when erasing drive
-(void) setupIndeterminateDiagnosticsProgressIndicator;
- (void) setupDeterminateDiagnosticsProgressIndicator:(NSString *)label;
- (void) setDiagnosticsProgress:(NSNumber *)value;
- (void) takeDownDiagnosticsProgressIndicator;
- (void) presentDiagnosticsProgress:(NSNumber *) startingProgress;
- (void) updateDiagnosticsProgressBarLabel:(NSString *)theString;

#pragma mark Accessors

// general

- (NSTextField *) guidanceText;
- (void) setGuidanceText:(NSString *)gt;
- (void) setAttributedGuidanceText:(NSMutableAttributedString *)at;

- (NSTextField *) fWindowTitleTF;
- (NSButton*) fNoticeCB;

- (GradButton *) fInfoButton;
- (GradButton *) fButton1;
- (GradButton *) fButton2;
- (GradButton *) fButton3;
- (NSProgressIndicator *) fProgressIndicator;
- (int) fButtonValue;
- (void) setFDriveName:(NSString *)driveName;

// type password
- (NSTextField *) fPasswordInfoTF;
- (NSTextField *) fPasswordLabel;

- (void) setPasswordLabel:(NSString *)gt;
- (void) setAttributedPasswordLabel:(NSMutableAttributedString *)at;

- (NSTextField *) fPasswordEnterTF;
- (NSTextField *) fPasswordHintTF;
- (NSTextField *) fPasswordHintLabel;
- (NSButton *) fPasswordRevealHintButton;
- (void) setFBSDName:(NSString *)bsdName;

// type notice
- (NSTextField *) fNoticeInfoTF;
- (NSImageView *) fNoticeWarningImage;

// type notWritable
- (NSTextField *) fNotWritableInfoTF;
- (NSImageView *) fNotWritableWarningImage;

// type warning
- (NSTextField *) fWarningInfoTF;
- (NSTextField *) fWarningInfo2TF;
- (NSImageView *) fWarningWarningImage;
- (NSTextField *) fWarningLabel;
- (NSTextField *) fWarningIUnderstandLabel;
- (NSButton *)    fWarningCB;
- (NSImageView *) fWarningWarningImage;

// type kNoticeDialog, kErasingDialog
- (void) setThisIsAMyBook:(BOOL)bookYesNo;

// about box
- (IBAction) doEULA:(id)sender;

// type confirm overwrite
- (void) setOverwriteDynamicString:(NSString *)str;
- (NSButton *)overwriteUseThisChoiceCB;
- (IBAction) applyDontShowRetrieveDialog:(id)sender; // user clicks on Don't show this dialog again check box

- (int) fState;

// backup interval
- (NSTextField *) buIntervalInfoTF;
- (NSTextField *) buIntervalEntryTF;
- (NSButton *) disableContinuousBUCB;



// for SWInfoController
- (void) setSharedController:(id)theSharedSWInfoController;

@end

@protocol SWInfo

-(int) GetPasswordHintforBSDName:(NSString *)bsdName hint:(NSMutableString *)hintString;

@end
