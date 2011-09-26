// Object identifier "pskt"
#define IDENTIFIER	MAKEID(p,a,l,x)

#include <vector>
#include    "ImageFlt.h" 
#include    "ImgFlt.h" 
#include    "CfcFile.h" 
using namespace std;

// ------------------------------
// DEFINITION OF CONDITIONS CODES
// ------------------------------

#define	CND_LAST					1
#define	ACT_LAST					22
#define	EXP_LAST                    11

#define HORIZONTAL 0
#define VERTICAL 1


struct ParallaxFrame{
	cSurface	*	parallaxImage;
	int *			startArray;
	int *			lengthArray;
	int *			blockArray;
	double *		blurredLength;
	int				blurStrength;
};

struct AttachedObject{
	long	fixedValue;
	float	startOffset;
	int		atLine;
};



// --------------------------------
// RUNNING OBJECT DATA STRUCTURE
// --------------------------------
// If you want to store anything between actions/conditions/expressions
// you should store it here. Also, some OEFLAGS require you to add
// structures to this structure.

typedef struct tagRDATA
{
	headerObject	rHo;					// Header
	rCom			roc;
	rMvt			rm;
	rSpr			rs;
	rVal			rv;

	int				oldWidth;
	int				oldHeight;

	vector<ParallaxFrame> * frames;
	vector<AttachedObject> * aObjects;
	cSurface	*	finalImage;

	int				currentFrame;
	ParallaxFrame * currentPframe;
	
	float			offset;
	float			centerOffset;
	float			angle;

	int				zLength;
	int				lowerLimit;
	int				upperLimit;
	int				smoothAmount;

	bool			direction;
	bool			resample;
	bool			autoscroll;
	bool			stationary;
	bool			smoothLines;

	bool			hasLowerLimit;
	bool			hasUpperLimit;

} RUNDATA;
typedef	RUNDATA	_far *			LPRDATA;

// Size when editing the object under level editor
// -----------------------------------------------
#define	MAX_EDITSIZE			sizeof(EDITDATA)

// Default flags
// -------------
#define	OEFLAGS      			OEFLAG_VALUES|OEFLAG_SPRITES|OEFLAG_MOVEMENTS|OEFLAG_BACKSAVE|OEFLAG_SCROLLINGINDEPENDANT|OEFLAG_RUNBEFOREFADEIN|OEFLAG_NEVERKILL|OEFLAG_NEVERSLEEP
#define	OEPREFS      			OEPREFS_BACKSAVE|OEPREFS_SCROLLINGINDEPENDANT|OEPREFS_LOADONCALL|OEPREFS_KILL|OEPREFS_INKEFFECTS

/* See OEFLAGS.txt for more info on these useful things! */


// If to handle message, specify the priority of the handling procedure
// 0= low, 255= very high. You should use 100 as normal.                                                
// --------------------------------------------------------------------
#define	WINDOWPROC_PRIORITY		100




////////////////////////////////////////////////////////////////////////////////////////////////
//Signatures for different functions used in Parallaxer extension (implemented in Runtime.cpp)
////////////////////////////////////////////////////////////////////////////////////////////////
ParallaxFrame	AnalyzeImage( cSurface * parallaxImage, bool direction, bool smoothLines, int smoothAmount );
void			SmoothLine(ParallaxFrame * frame, int arrayLength, int amount);
void			ClearAnimList( vector<ParallaxFrame> * animList );
BOOL			LoadImageFile( LPRDATA rdPtr, LPSURFACE psf, LPSTR pFileName, DWORD dwFlags);
int				AnimFrameWrap( LPRDATA rdPtr );
RECT			FitBox(int width, int height, int maxWidth, int maxheight);
void			HandleAttachedObjects(LPRDATA rdPtr);
void			CheckSurfaceRecreation(LPRDATA rdPtr);
//------------------------------------------------
long			FixedValue( LPRO object );
LPRO			LPROFromFixed( LPRDATA rdPtr, long fixedvalue );
//------------------------------------------------
void			Debug( char * string, int value );
void			Debug( char * string, int valueA, int valueB );
////////////////////////////////////////////////////////////////////////////////////////////////



//Old EditData structures from previous versions
typedef struct tagEDATA_V1
{
	extHeader		eHeader;
	short			sx;
	short			sy;
	short			swidth;
	short			sheight;
	bool			direction;
	int				offset;
	bool			resample;
	WORD			parallaxImage;
	int				zLength;

} OLDEDIT_A;



typedef struct tagEDATA_V3
{
	extHeader		eHeader;
	short			sx;
	short			sy;
	short			swidth;
	short			sheight;

	bool			direction;
	int				offset;

	bool			resample;
	int				zLength;

	int				numFrames;
	WORD			imageFrames[1];
} OLDEDIT_B;


typedef struct tagEDATA_V4
{
	extHeader		eHeader;
	short			sx;
	short			sy;
	short			swidth;
	short			sheight;

	int				offset;	
	int				zLength;
	int				numFrames;
	int				lowerLimit;
	int				upperLimit;

	bool			direction;
	bool			resample;
	bool			autoscroll;
	bool			hasLowerLimit;
	bool			hasUpperLimit;

	WORD			imageFrames[1];

} OLDEDIT_C;



typedef struct tagEDATA_V5
{
	extHeader		eHeader;
	short			sx;
	short			sy;
	short			swidth;
	short			sheight;

	int				offset;	
	int				zLength;
	int				numFrames;
	int				lowerLimit;
	int				upperLimit;

	bool			direction;
	bool			resample;
	bool			autoscroll;
	bool			hasLowerLimit;
	bool			hasUpperLimit;
	bool			stationatory;

	WORD			imageFrames[1];

} OLDEDIT_D;

// --------------------------------
// EDITION OF OBJECT DATA STRUCTURE
// --------------------------------
// These values let you store data in your extension that will be saved in the CCA.
// You would use these with a dialog...

typedef struct tagEDATA_V6
{
	extHeader		eHeader;
	short			sx;
	short			sy;
	short			swidth;
	short			sheight;

	int				offset;	
	int				zLength;
	int				numFrames;
	int				lowerLimit;
	int				upperLimit;
	int				smoothAmount;

	bool			direction;
	bool			resample;
	bool			autoscroll;
	bool			hasLowerLimit;
	bool			hasUpperLimit;
	bool			stationatory;
	bool			smoothLines;

	WORD			imageFrames[1];

} EDITDATA;
typedef EDITDATA _far *			LPEDATA;



// Object versions
#define	KCX_CURRENT_VERSION			6