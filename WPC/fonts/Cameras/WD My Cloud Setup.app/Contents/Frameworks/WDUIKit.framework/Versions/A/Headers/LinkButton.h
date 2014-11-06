//
//  LinkButton.h
//  NASOnboarding
//
//  Created by Clark Jackson on 7/15/10.
//  Copyright 2010 WD Branded Products. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface LinkButton : NSButton
{
    NSColor* titleColor;
}

- (void) setTitleColor:(NSColor *)theColor;
- (NSColor*) titleColor;
- (void) setUpTitle:(NSString*)theTitle;
- (void) setUpTitle:(NSString*)theTitle withAlignment:(NSTextAlignment)mode;

@end
