//
//  WDAttributedString+Additions.h
//  WDUIKit
//
//  Created by Clark Jackson on 6/14/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface NSAttributedString (WDAttributedStringCategory)

+(id)hyperlinkFromString:(NSString*)inString withURL:(NSURL*)aURL;


@end
