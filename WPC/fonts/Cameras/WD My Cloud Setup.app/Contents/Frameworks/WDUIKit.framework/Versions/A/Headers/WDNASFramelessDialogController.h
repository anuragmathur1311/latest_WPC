//
//  WDNASFramelessDialogView
//
//  Created by Clark Jackson on 10/31/08.
//  Copyright 2008 Western Digital. All rights reserved.
//

enum nasDialogScreen {
    kDetailsDialog,
    kInstallDialog
};

@class WDNASFramelessDialogView;
@class GradButton;
@class LinkButton;

@interface WDNASFramelessDialogController : NSWindowController
{
    BOOL                            keepGoing;
    NSModalSession                  session;

	int state;
    int buttonValue;

	IBOutlet NSTextField*   windowTitleTF;
    IBOutlet NSTextField*   detailsTF;
    IBOutlet NSTextField*   installTF;
    IBOutlet NSView*        detailsView;
    IBOutlet NSView*        installView;
    IBOutlet WDNASFramelessDialogView* dialogContentView;

	IBOutlet NSProgressIndicator*   installProgBar;

	IBOutlet LinkButton*   linkButton;
	IBOutlet GradButton*   fButton1;
	IBOutlet GradButton*   fButton2;
	IBOutlet GradButton*   fButton3;
}

#pragma mark ----------- Generic Methods  -----------

- (void) doStateChange:(int)state;
- (IBAction) doButton1:(id)sender;
- (IBAction) doButton2:(id)sender;
- (IBAction) doButton3:(id)sender;
- (IBAction) doLinkButton:(id)sender;
- (void) showWindow;
- (void) showWindowModally;


// for handling threaded progress when erasing drive
//-(void) setupIndeterminateDiagnosticsProgressIndicator;
//- (void) setupDeterminateDiagnosticsProgressIndicator:(NSString *)label;
//- (void) setDiagnosticsProgress:(NSNumber *)value;
//- (void) takeDownDiagnosticsProgressIndicator;
//- (void) presentDiagnosticsProgress:(NSNumber *) startingProgress;
//- (void) updateDiagnosticsProgressBarLabel:(NSString *)theString;

#pragma mark Accessors

- (WDNASFramelessDialogView *) dialogContentView;
- (NSTextField *) windowTitleTF;
- (NSTextField *) detailsTF;
- (NSTextField *) installTF;
- (GradButton *) fButton1;
- (GradButton *) fButton2;
- (GradButton *) fButton3;
- (LinkButton *) linkButton;
- (NSProgressIndicator *) installProgBar;
- (int) buttonValue;
- (int) state;
- (NSView *) detailsView;
- (NSView *) installView;

+ (void) doEULA:(id)sender;


@end
