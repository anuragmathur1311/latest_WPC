/*
File:		FramelessDialogViewWBG.h

Description: 	This is the header file for the FramelessDialogViewWBG class, which handles the drawing of the window content.
*/


@interface WDFileListerView : NSView
{
    BOOL                    _drawTopDivider;
    IBOutlet NSTableView    *table; // for positioning the text below the table
    IBOutlet NSTextField    *categoryText;
    IBOutlet NSTextField	*pathTF;
}

- (void) setDrawTopDivider:(BOOL)yesno;



@end
