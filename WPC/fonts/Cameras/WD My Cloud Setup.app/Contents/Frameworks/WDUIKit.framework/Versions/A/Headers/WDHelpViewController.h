//
//  WDHelpViewController.h
//  RoundTransparentWindow
//
//  Created by Clark Jackson on 10/31/08.
//  Copyright 2008 Western Digital. All rights reserved.
//

@class WebView;
@class LinkButton;

@interface WDHelpViewController : NSWindowController
{
    BOOL                    _isModal;

    NSString                *_flowTag;

    IBOutlet LinkButton* _aboutBtn;
	IBOutlet LinkButton* _supportBtn;

	IBOutlet NSTextField    *_debugText;
    IBOutlet NSTextField    *_headerText;
    IBOutlet NSTextField    *_notificationText;

	IBOutlet WebView        *_webView;
    id deviceObjectModel;
    
    NSModalSession          session;
    BOOL                    keepGoing;

}

@property (readwrite, assign) id deviceObjectModel;

#pragma mark ----------- Accessor Methods  -----------

- (void) setContent:(NSString *)path;
- (NSString*)flowTag;
- (void)setFlowTag: (NSString*) value;
- (void)setNotification: (NSString*) value;

#pragma mark ----------- Methods  -----------
- (id) initWithDeviceObjectModel:(id) theDeviceObjectModel;

- (IBAction) openSupportURL:(id)sender;
- (IBAction) showAboutBox:(id)sender;
- (void) showWindow;
- (void) showWindowModally;
- (void) closeWindow;

@end
