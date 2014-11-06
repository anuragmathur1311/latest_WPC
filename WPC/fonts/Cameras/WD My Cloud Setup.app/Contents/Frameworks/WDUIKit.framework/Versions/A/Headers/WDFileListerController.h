//
//  OB03Controller.h
//  RoundTransparentWindow
//
//  Created by Clark Jackson on 10/31/08.
//  Copyright 2008 Western Digital. All rights reserved.
//

@class GradButton;
@class WDFileListerView;
@class WDHelpViewController;

@interface WDFileListerController : NSWindowController
#if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6)
<NSTableViewDataSource,NSTableViewDelegate>
#endif
{
	// general
	IBOutlet WDFileListerView *fileListerView;
	IBOutlet NSTextField	*fWindowTitleTF;
	IBOutlet GradButton		*fButton1;
	IBOutlet GradButton		*fInfoButton;
    
    IBOutlet NSTextField    *_guidanceText;
    IBOutlet NSTextField    *categoryText;
    
    int fButtonValue;
    IBOutlet NSTextField	*pathTF;
    // table view stuff
	IBOutlet NSTableView	*tableView;
    NSMutableDictionary     *dataDictionary;
    NSArray                 *arrayOfKeys;
    NSMutableArray          *dataArray;
}

- (void) setNotification:(NSString *)theString;
- (void) setPathTFStringValue:(NSString *)theString;
- (void) setDataDictionary:(NSMutableDictionary *)theDict;
- (void) setDataArray:(NSMutableArray *)theArray;
- (IBAction) doInfo:(id)sender;
- (IBAction) doClose:(id)sender;

#pragma mark ----------- Generic Static Methods  -----------
+ (WDHelpViewController *) helpViewController;


@end
