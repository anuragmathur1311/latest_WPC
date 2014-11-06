//
//  WDStringCategory.h
//  PrivilegedInstaller
//
//  Created by Clark Jackson on 9/18/08.
//  Copyright 2008 Western Digital. All rights reserved.
//

@interface NSString (WDStringCategory)

+ (NSString *) pathSuitableForTool:(NSString *)unmodifiedPathString;
- (NSString *) pathSuitableForTool;
+ (NSMutableString *) fixLocalizedString:(NSString *)localizedString;
- (NSString *) pathToApplication;

+ (NSMutableDictionary *) decomposedBSDNodeName:(NSString *)bsdNode;
- (NSMutableDictionary *) decomposedBSDNodeName;

@end
