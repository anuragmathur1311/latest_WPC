#define FORWARD                         1
#define BACKWARD                        0



enum {
    kGRAY_SCHEME = 1,
    kBLUE_SCHEME,
    kTERTIARY_SCHEME,
    kINFO_SCHEME,
    kLIGHTGRAY_SCHEME,
    kGRADIENT_BACKGROUND_SCHEME1,
    kGRADIENT_BACKGROUND_SCHEME2,
    kGRADIENT_BACKGROUND_SCHEME3,
    kGRADIENT_BACKGROUND_SCHEME4,
    kGRADIENT_BACKGROUND_SCHEME5,
    kGRADIENT_BACKGROUND_SCHEME6,
    kGRADIENT_BACKGROUND_SCHEME7, // added to match Flash content on Welcome on-boarding screen
    kGRADIENT_BACKGROUND_SCHEME,
    kSOLID_GRAY_SCHEME,
    
    kGAUGE_SCHEME,
    
    kOTHER_CONTENT_GAUGE_SCHEME,
    kYELLOW_SCHEME,
    kALPHA_SCHEME,
    
    kSEGMENT_GRAY_SCHEME_LEFT, // specifies square left edge
    kSEGMENT_GRAY_SCHEME_RIGHT, // specifies square right edge
    kSEGMENT_LIGHTGRAY_SCHEME_LEFT, // specifies square left edge
    kSEGMENT_LIGHTGRAY_SCHEME_RIGHT, // specifies square right edge
    kBUBBLE_BLUE_SCHEME,
    kBUBBLE_YELLOW_SCHEME,
    kTAB_BACKGROUND_SCHEME,
    kTAB_BUTTONS_SCHEME,
    kSOLID_WHITE_SCHEME,
    kECOSYSTEM_SCHEME,
    kSOLID_GRAY_SCHEME77,
    kFIFTY_PC_WHITE_SCHEME,
    kDARKBLUE_SCHEME,
    kECO_SCROLL_SCHEME,
    kNASBLUE_SCHEME
};

enum {
    kDISABLED_BIT  =  1,   // 2^^0    000...00000001
    kPRESSED_BIT   =  2,   // 2^^1    000...00000010
    kDEFAULT_BIT   =  4,   // 2^^2    000...00000100
    kFOCUS_BIT     =  8,   // 2^^3    000...00001000
    kROLLOVER_BIT  = 16,   // 2^^4    000...00010000
    kNORMAL_BIT    = 32,   // 2^^5    000...00100000
    kSELECTION_BIT = 64,   // 2^^6    000...01000000
    kDEFAULT_FOCUS_BIT = 12 //concat  000...00001100
};

enum{
    kBOTTOM_WEST = 0,
    kBOTTOM_EAST,
    kRIGHT_SOUTH,
    kRIGHT_NORTH,
    kTOP_EAST,
    kTOP_WEST,
    kLEFT_NORTH,
    kLEFT_SOUTH
};	

enum{
    kALL_ROUND = 0,
    kLEFT_SQUARE,
    kBOTTOM_SQUARE,
    kRIGHT_SQUARE,
    kTOP_SQUARE
};	


enum {
    kTOP_HALF = 0,
    kBOTTOM_HALF,
    kWHOLE
};


CGFunctionRef createFunctionForRGBBlending(CGFunctionEvaluateCallback evaluationFunction, const float *componentsFrom, const float *componentsTo, int direction);
void BlendColors(void *info, const float *in, float *out);

void WhiteToBlack(void *info,const float *in, float *out);
void SolidGray(void *info,const float *in, float *out);
void DoAlpha(void *info,const float *in, float *out);

void GaugeInappropriateColorFunc(void *info,const float *in, float *out);
void GaugeUnusedSpaceColorFunc(void *info,const float *in, float *out);

# pragma mark Gradient C CreateFunction Functions
CGFunctionRef createFunctionForRGB(CGFunctionEvaluateCallback evaluationFunction);
CGFunctionRef createFunctionForRGB3(CGFunctionEvaluateCallback evaluationFunction);
CGFunctionRef createFunctionForRGB4(CGFunctionEvaluateCallback evaluationFunction);
CGFunctionRef createFunctionForRGB6(CGFunctionEvaluateCallback evaluationFunction);
CGFunctionRef createFunctionForRGB7(CGFunctionEvaluateCallback evaluationFunction);
CGFunctionRef createFunctionForRGB8(CGFunctionEvaluateCallback evaluationFunction);
CGFunctionRef createFunctionForRGB9(CGFunctionEvaluateCallback evaluationFunction);

# pragma mark Gradient C AxialShading Functions
void doSimpleAxialShading(CGContextRef context,CGPoint startPt, CGPoint endPt, int theType,int theFlag, int theHalf);

# pragma mark C Shadowing Functions
void drawSimpleShadow(CGContextRef theContext);
void drawShadowHighlight(CGContextRef theContext);

void addClippedRoundedRectToPath(CGContextRef context, CGRect rect, float cornerWidth, float cornerHeight, int type);
void addCheckmarkShapeToPath(CGContextRef context, CGRect sizeRect, float cornerWidth);
void addDottedRectToPath(CGContextRef context, CGRect rect, float cornerWidth, float cornerHeight, CGColorRef rgbaRef);
// 0 = all corners rounded, 1 = left edge square, 2 = bottom edge square, 3 = right edge square, 4 = top edge square
void addRoundedRectToPath(CGContextRef context, CGRect rect, float cornerWidth, float cornerHeight, int type);
void addTabShape(CGContextRef context, CGRect comboRect,
				 float cornerWidth, float cornerHeight, NSRect selectedRect);
void addTabShapeToPath(CGContextRef context, CGRect comboRect, CGSize tabSize, float cornerWidth, float cornerHeight, float cornerRadius, int position);
void addBubbleShapeToPath(CGContextRef context, CGRect comboRect, CGSize jutSize, float cornerWidth, float cornerHeight, int position);
void addTriangleShape(CGContextRef context, CGRect containingRect, int facing); // facing left = 0, right = 1
void doGaugeAxialShading(CGContextRef context, CGPoint startPt, CGPoint endPt, int scheme, int state, BOOL isTopHalf);
void addEllipseShape(CGContextRef context, CGRect containingRect);



//  this was the old GradComponent class

void drawDivider(NSRect nsRect);
void draw1LineOfRect(NSRect nsRect); 
void draw2LineOfRect(NSRect nsRect); 
void draw3LineOfRect(NSRect nsRect); 

void drawTopDivider(NSRect nsRect); 
void drawEcoDivider(NSRect nsRect); // makes assumptions about nsRect
void drawEcoDividerInRect(NSRect nsRect); // makes no assumptions about nsRect
void drawDisclosureDivider(NSRect nsRect); 
void drawButtonsRowDivider(NSRect nsRect);  // makes assumptions about nsRect
void drawButtonsRowDividerInRect(NSRect nsRect); // makes no assumptions about nsRect
void drawHorizontalInnerSectionDivider(NSRect nsRect); 
void drawVerticalInnerSectionDivider(NSRect nsRect); 

void drawGuidelineCGRect(CGRect rect, CGColorRef colorRef);
//  void drawGuidelineCGRect(CGRect rect);
void drawColorGuidelineRect(NSRect rect, CGColorRef colorRef);
void drawGuidelineRect(NSRect nsRect); 
void drawGuideline(NSPoint p1, NSPoint p2, NSColor* color);
void clearGuidelineCGRect(CGRect rect);
void clearGuidelineRect(NSRect rect);

//#endif