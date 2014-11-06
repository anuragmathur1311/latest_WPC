//
//  GradientButtonCell.h
//  RoundTransparentWindow
//
//  Created by Clark Jackson on 11/3/08.
//  Copyright 2008 Western Digital. All rights reserved.
//

@interface GradientButtonCell : NSButtonCell
{
	BOOL            fShadowFlag,
                    fDefaultFlag,
                    fFocusFlag,
                    fRolloverFlag,
                    fSelectionFlag,
                    fDisabledFlag,
                    fDisablePressFlag;

	int             fScheme, // refers to overall color but not state
                    fStateFlag, // refers to state: pressed, normal, roll over etc.
                    facing; // refers to right = 1, left = 0
    float           fFontSize;
    
    NSArray*    imageArray;
    BOOL killShadow;
    
}

/*" Initializers "*/
- (id)initTextCell:(NSString *)aString;

// Accessors
-(void)setFScheme:(int)theScheme;

- (BOOL) killShadow;
- (void) setKillShadow:(BOOL)yesorno;
/*!
 @function fStateFlag
 @discussion all button states are kept within an instance of a button cell.
 */
-(int)fStateFlag;
/*!
 @function setFStateFlag
 @discussion all button states are kept within an instance of a button cell.
 */
-(void)setFStateFlag:(int)theStateFlag;

/*!
 @function fShadowFlag
 @discussion all button states are kept within an instance of a button cell.
 */
-(BOOL)fShadowFlag;
/*!
 @function setFShadowFlag
 @discussion all button states are kept within an instance of a button cell.
 */
-(void)setFShadowFlag:(BOOL)drawShadow;

/*!
 @function fDisabledFlag
 @discussion all button states are kept within an instance of a button cell.
 */
-(BOOL)fDisabledFlag;
/*!
 @function setFDisabledFlag
 @discussion all button states are kept within an instance of a button cell.
 */
-(void)setFDisabledFlag:(BOOL)drawDisabled;

/*!
 @function fDefaultFlag
 @discussion all button states are kept within an instance of a button cell.
 */
-(BOOL)fDefaultFlag;
/*!
 @function setFDefaultFlag
 @discussion all button states are kept within an instance of a button cell.
 */
-(void)setFDefaultFlag:(BOOL)drawDefault;

/*!
 @function fFocusFlag
 @discussion all button states are kept within an instance of a button cell.
 */
-(BOOL)fFocusFlag;
/*!
 @function setFFocusFlag
 @discussion all button states are kept within an instance of a button cell.
 */
-(void)setFFocusFlag:(BOOL)drawFocus;

/*!
 @function fRolloverFlag
 @discussion all button states are kept within an instance of a button cell.
 */
-(BOOL)fRolloverFlag;
/*!
 @function setFRolloverFlag
 @discussion all button states are kept within an instance of a button cell.
 */
-(void)setFRolloverFlag:(BOOL)drawRollover;

/*!
 @function fSelectionFlag
 @discussion all button rendering states are kept within an instance of a button cell.
 */
-(BOOL)fSelectionFlag;
/*!
 @function setFSelectionFlag
 @discussion all button rendering states are kept within an instance of a button cell.
 */
-(void)setFSelectionFlag:(BOOL)drawSelection;

/*!
 @function fDisabledPressFlag
 @discussion all button states are kept within an instance of a button cell.
 */
-(BOOL)fDisabledPressFlag;
/*!
 @function setFDisablePressFlag
 @discussion all button states are kept within an instance of a button cell.
 */
-(void)setFDisablePressFlag:(BOOL)drawRollover;

-(void)setFFontSize:(float)fontSize;
-(int)fScheme;

-(void)setFacing:(int)theFacing;
-(int)facing;

-(NSDictionary*) stringAttributes;

- (NSArray*) imageArray;
- (void) setImageArray:(NSArray*) theImageArray;

@end
