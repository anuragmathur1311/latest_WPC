/*
File:		WDNASFramelessDialogView.h

Description: 	This is the header file for the FramelessDialogViewWBG class, which handles the drawing of the window content.
*/

@class WDNASFramelessDialogController;

@interface WDNASFramelessDialogView : NSView
{
    IBOutlet WDNASFramelessDialogController* delegate;
}

- (WDNASFramelessDialogController*) delegate;

@end
