//
//  GradButton.h
//  RoundTransparentWindow
//
//  Created by Clark Jackson on 10/19/08.
//  Copyright 2008 Western Digital. All rights reserved.
//
//#import <WebKit/WebView.h>

@class WebView;

@interface GradButton : NSButton
{
	int fType; // refers to overall color but not state
	NSTrackingRectTag fTrackingTag;
	NSString *fFlashFileURL;
	NSMutableDictionary *fIdentifier; // for identifing buttons (drives) in the EcoSystem 
}

#pragma mark button Accessors	

- (NSMutableDictionary *) fIdentifier;
- (void) setFIdentifier:(NSMutableDictionary *) theIdentifier;

#pragma mark cell Accessors

/*!
 @function enabledState
 @abstract returns YES when the inquired bit flag is set. Otherwise, returns NO.
 */
-(BOOL)enabledState:(unsigned short) bit;

/*!
 @function enableState
 @abstract sets the state big flag to 1.  This resets other flags to 0 (zero).
 @discussion to set both Default & Focus flags, use setFDefaultFlag & setFFocusFlag setters.

 Note:
 
 1) Cell State Bit/Flag is used for rendering of a specific button state.  It's used explicitly
 for state drawing/rendering purpose only.  Do not set the state bit/flag directly.
 Instead, use a button flag setter directly. 
 
 Use setFDisabledFlag:(BOOL), setFDefaultFlag:(BOOL), setFFocusFlag:(BOOL), setFRolloverFlag:(BOOL)
 or enableRolloverState:(BOOL).
 
 
 2) Button Flag is used to set the button state.  A button state is Rollover, Disabled,
 Focus, Default and Default+Focus.
 
 Use setFDisabledFlag:(BOOL), setFDefaultFlag:(BOOL), setFFocusFlag:(BOOL), setFRolloverFlag:(BOOL)
 or enableRolloverState:(BOOL).
 */
-(void)enableState:(unsigned short) bit;

/*!
 @function fDisabledFlag
 @abstract returns YES if the disabled flag is set.
 */
-(BOOL)fDisabledFlag;

/*!
 @function setFDisabledFlag
 @abstract toggles the button state to be disabled.
 @discussion A user should disable a button by calling this function.  
 Its cell function setFStateFlag should not be called directly. This cell function is for
 rendering for a specific button state.  A specific cell state uses the button state
 to render a specifc state state as such as Normal, Disabled, Rollover, Default and Focus.

 NOTE: Currently, GradientButtonCell keeps the button state.  Ideally, the button state
 should be kept within GradButton.
 */
-(void)setFDisabledFlag:(BOOL)yesOrNo;

/*!
 @function fDefaultFlag
 @abstract returns YES or NO of fDefaultFlag.
 Note: Both a default flag and a focus flag can be set.
 */
-(BOOL)fDefaultFlag;

/*!
 @function setFDefaultFlag
 @abstract toggles setFDefaultFlag YES or NO. 
 Note: Both a default flag and a focus flag can be set.
 */
-(void)setFDefaultFlag:(BOOL)yesOrNo;

/*!
 @function fFocusFlag
 @abstract returns YES or NO of kFOCUS_BIT.
 Note: Both a default flag and a focus flag can be set.
 */
-(BOOL)fFocusFlag;

/*!
 @function setFFocusFlag
 @abstract toggles FFocusFlag YES or NO.
 Note: Both a default flag and a focus flag can be set.
 */
-(void)setFFocusFlag:(BOOL)yesOrNo; 

/*!
 @function fRolloverFlag
 @abstract returns YES when the button state is kROLLOVER_BIT.  Otherwise, returns NO.
 */
- (BOOL) fRolloverFlag;

/*!
 @function setFRolloverFlag
 @abstract sets the state big flag to 1.  This resets other flags to 0 (zero).
 @discussion with toggle value NO, the button state flag is set to kNORMAL_BIT.
 NOTE: With Rollover state, do not set the state bit directly in GradientButtonCell.
 Invoking fRolloverFlag is all needed.  It enables the mouse tracking within the button cell rect.
 This enables mouseEntered function.  Only the rollover state needs mouseEntered function.
 
 */
- (void) setFRolloverFlag:(BOOL)yesOrNo;
- (void) enableRolloverState:(BOOL)yesOrNo;


/*!
 @function fSelectionFlag
 @abstract returns YES when the button state is kSELECTION_BIT.  Otherwise, returns NO.
 */
-(BOOL)fSelectionFlag;

/*!
 @function setFSelectionFlag
 @abstract sets the state big flag to 1.  This resets other flags to 0 (zero).
 @discussion with toggle value NO, the button state flag is set to kNORMAL_BIT.
 */
-(void) setFSelectionFlag:(BOOL)yesOrNo; 
- (void) enableFSelectionState:(BOOL) yesOrNo;

/*!
 @function fSelectionImage
 @abstract returns NSImage instance when the alternateImage is set within the button cell.  If not set, it returns nil.
 */
-(NSImage*) fSelectionImage;

/*!
 @function setFSelectionImage
 @abstract sets the selection image.   
 @discussion Uses the button cell's alternate image as an image place holder.
 */
- (void) setFSelectionImage:(NSImage*) alternateImage;


/*!
 @function fDisabledPress
 @abstract returns YES when the button has Press state disabled.  Otherwise, returns NO.
 When a button has a press disabled, clicking on a button doesn't register nor doesn't generate a button press event.
 */
- (BOOL) fDisabledPress;

/*!
 @function setFDisablePressFlag
 @abstract disables a button press when set YES.  When a button press is disabled by this function,
 any button click or press is not registered, and it will not generate a mouse click or a mouse press event.
 
 When set NO, a button responds to a button press, normally.
 */
- (void) setFDisablePressFlag:(BOOL)yesOrNo;



-(void)setFType:(int)theType;

/*!
 @function setFFlashFileURL
 @abstract sets the fully qualified URL of a flash file to play when a mouse pointer hovers over the button.
 */
-(void)setFFlashFileURL:(NSString*) url;

/*!
 @function fFlashFileURL
 @abstract  
 */
-(NSString *) fFlashFileURL;



/*!
 @function setFScheme
 @abstract set a button scheme/type.
 e.g. kBLUE_SCHEME, kGRAY_SCHEME, kTERTIARY_SCHEME, kSEGMENT_GRAY_SCHEME_LEFT or kSEGMENT_LIGHTGRAY_SCHEME_LEFT,
 kSEGMENT_LIGHTGRAY_SCHEME_LEFT or kSEGMENT_LIGHTGRAY_SCHEME_RIGHT.
 */
-(void)setFScheme:(int)theScheme;


/*!
 @function setFShadowFlag
 @abstract toggle this flag to show the button shadow.

 */
-(void)setFShadowFlag:(BOOL)drawShadow;

/*!
 @function fShadowFlag
 @abstract returns YES if the button is set to draw its shadow.  Otherwise, returns NO.
 
 */
-(BOOL)fShadowFlag;
-(void)sizeToFitAndAnchorLeft;
-(void)sizeToFitAndAnchorRight;

@end



@protocol WDWebView
-(WebView*)fMovieWebView;
@end

