/*
File:		WDFramelessWindow.h


*/

@interface WDFramelessWindow : NSWindow
{
    //This point is used in dragging to mark the initial click location
    NSPoint     _initialLocation;
    BOOL        dragFlag; // this flag keeps track of wheter a drag event was initiated inside the window itself
}

-(id)delegate;
-(void)setDelegate:(id)anObject;


@end
