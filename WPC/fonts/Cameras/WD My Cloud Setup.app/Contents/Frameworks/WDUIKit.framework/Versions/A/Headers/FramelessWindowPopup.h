/*
File:		FramelessWindow.h


*/

@class RolloverView;

@interface FramelessWindowPopup : NSWindow
{
    //This point is used in dragging to mark the initial click location
    NSPoint                 _initialLocation;
	IBOutlet RolloverView   *_rolloverView;  
}

- (RolloverView *)fRolloverView;
- (void) setRolloverView:(RolloverView *)rv;

@end
