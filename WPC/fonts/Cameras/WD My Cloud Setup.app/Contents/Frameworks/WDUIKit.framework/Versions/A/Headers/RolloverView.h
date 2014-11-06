//
//  RolloverView.h
//  RoundTransparentWindow
//
//  Created by Clark Jackson on 11/22/08.
//  Copyright 2008 Western Digital. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface RolloverView : NSView
{
	NSTextView	*fTextView;                              //  NSTextView
}

- (NSTextView *) fTextView;
@end
