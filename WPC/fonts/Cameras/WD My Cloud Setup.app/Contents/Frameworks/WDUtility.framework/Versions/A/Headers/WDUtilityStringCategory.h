//
//  WDUtilityStringCategory.h
//  PrivilegedInstaller
//
//  Created by Clark Jackson on 9/18/08.
//  2008 Western Digital Technologies, Inc. All rights reserved.
//

@interface NSString (WDUtilityStringCategory)

+ (NSString *) pathSuitableForTool:(NSString *)unmodifiedPathString;
- (NSString *) pathSuitableForTool;
+ (NSMutableString *) fixLocalizedString:(NSString *)localizedString;
- (NSString *) pathToApplication;

+ (NSMutableDictionary *) decomposedBSDNodeName:(NSString *)bsdNode;
- (NSMutableDictionary *) decomposedBSDNodeName;
+ (NSString *) stringFromBufferContents:(char *) buffer bufferLength:(UInt32) length;
// only works with 10.6
//- (NSMutableAttributedString*) highlightCharactersInSet:(NSCharacterSet*)charsToHighlight withColor:(NSColor*)highlightColor;

@end
