/*
File:		WDFramelessDialogView.h

Description: 	This is the header file for the FramelessDialogViewWBG class, which handles the drawing of the window content.
*/

@interface WDFramelessDialogView : NSView
{
    BOOL    _drawTopDivider;
    BOOL    _drawBottomDivider;
}

- (void) setDrawTopDivider:(BOOL)yesno;
- (void) setDrawBottomDivider:(BOOL)yesno;

@end
