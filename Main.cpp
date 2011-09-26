// ============================================================================
//
// This file are where the Conditions/Actions/Expressions are defined.
// You can manually enter these, or use CICK (recommended)
// See the Extension FAQ in this SDK for more info and where to download it
//
// ============================================================================

// Common Include
#include	"common.h"

// Quick memo: content of the eventInformations arrays
// ---------------------------------------------------
// Menu ID
// String ID
// Code
// Flags
// Number_of_parameters
// Parameter_type [Number_of_parameters]
// Parameter_TitleString [Number_of_parameters]

// Definitions of parameters for each condition
short conditionsInfos[]=
	{ 
	0 };

// Definitions of parameters for each action
short actionsInfos[]=
	{ 
	AID_actSetOffset,		AID_actSetOffset,		0,	0,	1,	PARAM_EXPRESSION,	AP0ID_actSetOffset,
	AID_actSetWidth,		AID_actSetWidth,		1,	0,	1,	PARAM_EXPRESSION,	AP0ID_actSetWidth,
	AID_actSetHeight,		AID_actSetHeight,		2,	0,	1,	PARAM_EXPRESSION,	AP0ID_actSetHeight,
	AID_actSetResampleOn,	AID_actSetResampleOn,	3,	0,	0,
	AID_actSetResampleOff,	AID_actSetResampleOff,	4,	0,	0,
	AID_actSetZLength,		AID_actSetZLength,		5,	0,	1,	PARAM_EXPRESSION,	AP0ID_actSetZLength,
	AID_actLoadImage,		AID_actLoadImage,		6,  0,  2,  PARAM_FILENAME2,	PARAM_EXPRESSION,		AP0ID_actLoadImage,	AP1ID_actLoadImage,
	AID_actSetFrame,		AID_actSetFrame,		7,	0,	1,	PARAM_EXPRESSION,	AP0ID_actSetFrame,
	AID_actAttachObject,	AID_actAttachObject,	8,	0,	1,	PARAM_OBJECT,		AP0ID_actAttachObject,
	AID_actDetachObject,	AID_actDetachObject,	9,	0,	1,	PARAM_OBJECT,		AP0ID_actDetachObject,
	AID_actAutoScrollOn,	AID_actAutoScrollOn,	10,	0,	0,
	AID_actAutoScrollOff,	AID_actAutoScrollOff,	11,	0,	0,
	AID_actLowerSetValue,	AID_actLowerSetValue,	12,	0,	1,	PARAM_EXPRESSION,	AP0ID_actLowerSetValue,
	AID_actUpperSetValue,	AID_actUpperSetValue,	13,	0,	1,	PARAM_EXPRESSION,	AP0ID_actUpperSetValue,
	AID_actLowerOn,			AID_actLowerOn,			14,	0,	0,
	AID_actLowerOff,		AID_actLowerOff,		15,	0,	0,
	AID_actUpperOn,			AID_actUpperOn,			16,	0,	0,
	AID_actUpperOff,		AID_actUpperOff,		17,	0,	0,
	AID_actStationaryOn,	AID_actStationaryOn,	18,	0,	0,
	AID_actStationaryOff,	AID_actStationaryOff,	19,	0,	0,
	AID_actSetCenterOffset,	AID_actSetCenterOffset,	20,	0,	1,	PARAM_EXPRESSION,	AP0ID_actSetCenterOffset,
	AID_actSetAngle,		AID_actSetAngle,		21,	0,	1,	PARAM_EXPRESSION,	AP0ID_actSetAngle,
	0 };


// Definitions of parameters for each expression
short expressionsInfos[]=
	{ 
	EID_expGetOffset,		EID_expGetOffset,		0,	0,	0,
	EID_expGetWidth,		EID_expGetWidth,		1,	0,	0,
	EID_expGetHeight,		EID_expGetHeight,		2,	0,	0,
	EID_expGetZLength,		EID_expGetZLength,		3,	0,	0,
	EID_expGetAniFrame,		EID_expGetAniFrame,		4,	0,	0,
	EID_expGetLineLength,	EID_expGetLineLength,	5,	0,	1,	EXPPARAM_LONG,	EP0ID_expGetLineLength,
	EID_expGetScrollCoeff,	EID_expGetScrollCoeff,	6,	0,	1,	EXPPARAM_LONG,	EP0ID_expGetScrollCoeff,
	EID_expGetLowerValue,	EID_expGetLowerValue,	7,	0,	0,
	EID_expGetUpperValue,	EID_expGetUpperValue,	8,	0,	0,
	EID_expGetCenterOffset,	EID_expGetCenterOffset,	9,	0,	0,
	EID_expGetAngle,		EID_expGetAngle,		10,	0,	0,
	0 };



// ============================================================================
//
// CONDITION ROUTINES
// 
// ============================================================================.


// ============================================================================
//
// ACTIONS ROUTINES
// 
// ============================================================================


short WINAPI DLLExport actSetOffset(LPRDATA rdPtr, long param1, long param2)
{
	float value = -GetFloatParam(rdPtr);
	if(rdPtr->offset != value)
	{
		HandleAttachedObjects(rdPtr);
		rdPtr->roc.rcChanged = true;
		rdPtr->offset = value;
	}
	return 0;
}

short WINAPI DLLExport actSetWidth(LPRDATA rdPtr, long param1, long param2)
{
	rdPtr->rHo.hoImgWidth = param1;
	HandleAttachedObjects(rdPtr);
	rdPtr->roc.rcChanged = true;
	return 0;
}

short WINAPI DLLExport actSetHeight(LPRDATA rdPtr, long param1, long param2)
{
	rdPtr->rHo.hoImgHeight = param1;
	HandleAttachedObjects(rdPtr);
	rdPtr->roc.rcChanged = true;
	return 0;
}


short WINAPI DLLExport actSetResampleOn(LPRDATA rdPtr, long param1, long param2){
	rdPtr->resample = true;
	rdPtr->roc.rcChanged = true;
	return 0;
}

short WINAPI DLLExport actSetResampleOff(LPRDATA rdPtr, long param1, long param2){
	rdPtr->resample = false;
	rdPtr->roc.rcChanged = true;
	return 0;
}

short WINAPI DLLExport actSetZLength(LPRDATA rdPtr, long param1, long param2){
	rdPtr->zLength = param1;
	HandleAttachedObjects(rdPtr);
	rdPtr->roc.rcChanged = true;
	return 0;
}

short WINAPI DLLExport actLoadImage(LPRDATA rdPtr, long param1, long param2){
	
	char * filename = (char *)param1;
	long option = param2;

	cSurface * newImage = new cSurface();

	if( LoadImageFile( rdPtr, newImage, filename, 0 ) )
	{
		ParallaxFrame newFrame = AnalyzeImage( newImage, rdPtr->direction, rdPtr->smoothLines, rdPtr->smoothAmount );

		if( option == 1 ) rdPtr->frames->push_back( newFrame );
		else if( option == 0 )
		{
			ClearAnimList( rdPtr->frames );
			rdPtr->frames->push_back( newFrame );
		}
		rdPtr->roc.rcChanged = true;
	}
	HandleAttachedObjects(rdPtr);
	
	return 0;
}


short WINAPI DLLExport actSetFrame(LPRDATA rdPtr, long param1, long param2){
	rdPtr->currentFrame = param1;
	rdPtr->roc.rcChanged = true;
	HandleAttachedObjects(rdPtr);
	return 0;
}

short WINAPI DLLExport actAttachObject(LPRDATA rdPtr, long param1, long param2){
	LPRO object = (LPRO)param1;
	long fixedValue = FixedValue( object );

	for( int i = 0; i < rdPtr->aObjects->size(); i++ ){
		AttachedObject obj = rdPtr->aObjects->at( i );
		if( obj.fixedValue == fixedValue ){
			rdPtr->aObjects->erase( rdPtr->aObjects->begin() + i );
			break;
		}
	}

	ParallaxFrame current = rdPtr->frames->at( AnimFrameWrap( rdPtr ) );
	if( current.parallaxImage == NULL ) return 0;
	
	if( rdPtr->direction == HORIZONTAL && object->roHo.hoY >= rdPtr->rHo.hoY && object->roHo.hoY < (rdPtr->rHo.hoY+rdPtr->rHo.hoImgHeight) )
	{
		AttachedObject attachment;
		attachment.fixedValue = fixedValue;
		attachment.atLine = int( (object->roHo.hoY - rdPtr->rHo.hoY) * ( current.parallaxImage->GetHeight() / (float)rdPtr->rHo.hoImgHeight ));
		int length = current.lengthArray[ attachment.atLine ];
		float offsetCorrection = rdPtr->offset * ( length / (float)rdPtr->zLength );
		attachment.startOffset = object->roHo.hoX - (rdPtr->rHo.hoX + rdPtr->rHo.hoImgWidth/2) - offsetCorrection;
		rdPtr->aObjects->push_back( attachment );
	}

	if( rdPtr->direction == VERTICAL && object->roHo.hoX >= rdPtr->rHo.hoX && object->roHo.hoX < (rdPtr->rHo.hoX+rdPtr->rHo.hoImgWidth) )
	{
		AttachedObject attachment;
		attachment.fixedValue = fixedValue;
		attachment.atLine = int( (object->roHo.hoX - rdPtr->rHo.hoX) * ( current.parallaxImage->GetWidth() / (float)rdPtr->rHo.hoImgWidth ));
		int length = current.lengthArray[ attachment.atLine ];
		float offsetCorrection = rdPtr->offset * ( length / (float)rdPtr->zLength );
		attachment.startOffset = object->roHo.hoY - (rdPtr->rHo.hoY + rdPtr->rHo.hoImgHeight/2) - offsetCorrection;
		rdPtr->aObjects->push_back( attachment );
	}

	return 0;
}

short WINAPI DLLExport actDetachObject(LPRDATA rdPtr, long param1, long param2){

	LPRO object = (LPRO)param1;
	long fixedValue = FixedValue( object );

	for( int i = 0; i < rdPtr->aObjects->size(); i++ ){
		AttachedObject obj = rdPtr->aObjects->at( i );
		if( obj.fixedValue == fixedValue ){
			rdPtr->aObjects->erase( rdPtr->aObjects->begin() + i );
			break;
		}
	}
	return 0;
}


short WINAPI DLLExport actAutoScrollOn(LPRDATA rdPtr, long param1, long param2){
	rdPtr->autoscroll = true;
	rdPtr->roc.rcChanged = true;
	return 0;
}

short WINAPI DLLExport actAutoScrollOff(LPRDATA rdPtr, long param1, long param2){
	rdPtr->autoscroll = false;
	rdPtr->roc.rcChanged = true;
	return 0;
}

short WINAPI DLLExport actLowerSetValue(LPRDATA rdPtr, long param1, long param2){
	rdPtr->lowerLimit = param1;
	rdPtr->upperLimit = max( rdPtr->lowerLimit, rdPtr->upperLimit );
	rdPtr->roc.rcChanged = true;
	return 0;
}

short WINAPI DLLExport actUpperSetValue(LPRDATA rdPtr, long param1, long param2){
	rdPtr->upperLimit = param1;
	rdPtr->lowerLimit = min( rdPtr->lowerLimit, rdPtr->upperLimit );
	rdPtr->roc.rcChanged = true;
	return 0;
}

short WINAPI DLLExport actLowerOn(LPRDATA rdPtr, long param1, long param2){
	rdPtr->hasLowerLimit = true;
	rdPtr->roc.rcChanged = true;
	return 0;
}

short WINAPI DLLExport actLowerOff(LPRDATA rdPtr, long param1, long param2){
	rdPtr->hasLowerLimit = false;
	rdPtr->roc.rcChanged = true;
	return 0;
}

short WINAPI DLLExport actUpperOn(LPRDATA rdPtr, long param1, long param2){
	rdPtr->hasUpperLimit = true;
	rdPtr->roc.rcChanged = true;
	return 0;
}

short WINAPI DLLExport actUpperOff(LPRDATA rdPtr, long param1, long param2){
	rdPtr->hasUpperLimit = false;
	rdPtr->roc.rcChanged = true;
	return 0;
}

short WINAPI DLLExport actStationaryOn(LPRDATA rdPtr, long param1, long param2){
	rdPtr->stationary = true;
	rdPtr->roc.rcChanged = true;
	return 0;
}

short WINAPI DLLExport actStationaryOff(LPRDATA rdPtr, long param1, long param2){
	rdPtr->stationary = false;
	rdPtr->roc.rcChanged = true;
	return 0;
}

short WINAPI DLLExport actSetCenterOffset(LPRDATA rdPtr, long param1, long param2){
	rdPtr->centerOffset = GetFloatParam(rdPtr);
	rdPtr->roc.rcChanged = true;
	return 0;
}

short WINAPI DLLExport actSetAngle(LPRDATA rdPtr, long param1, long param2){
	rdPtr->angle = GetFloatParam(rdPtr);
	rdPtr->roc.rcChanged = true;
	return 0;
}


// ============================================================================
//
// EXPRESSIONS ROUTINES
// 
// ============================================================================


long WINAPI DLLExport expGetOffset(LPRDATA rdPtr, long param1){
	rdPtr->rHo.hoFlags |= HOF_FLOAT;
	float fResult = -rdPtr->offset;
    return *((long*)&fResult);
}

long WINAPI DLLExport expGetWidth(LPRDATA rdPtr, long param1){
	return rdPtr->rHo.hoImgWidth;
}

long WINAPI DLLExport expGetHeight(LPRDATA rdPtr, long param1){
	return rdPtr->rHo.hoImgHeight;
}

long WINAPI DLLExport expGetZLength(LPRDATA rdPtr, long param1){
	return rdPtr->zLength;
}

long WINAPI DLLExport expGetAniFrame(LPRDATA rdPtr, long param1){
	return rdPtr->currentFrame;
}


long WINAPI DLLExport expGetLineLength(LPRDATA rdPtr, long param1){
	long linePos = CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_INT);

	ParallaxFrame current = rdPtr->frames->at( AnimFrameWrap( rdPtr ) );
	if( current.parallaxImage == NULL ) return 0;

	int pos = (rdPtr->direction == HORIZONTAL) ? rdPtr->rHo.hoY : rdPtr->rHo.hoX;
	int dim = (rdPtr->direction == HORIZONTAL) ? rdPtr->rHo.hoImgHeight : rdPtr->rHo.hoImgWidth;
	int pim = (rdPtr->direction == HORIZONTAL) ? current.parallaxImage->GetHeight() : current.parallaxImage->GetWidth();

	if( linePos >= pos && linePos < ( pos + dim ) )
		return current.lengthArray[ (int)( (linePos - pos) * ( pim /(float)dim )) ];

	return 0;
}

long WINAPI DLLExport expGetScrollCoeff(LPRDATA rdPtr, long param1){
	long linePos = CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_INT);

	ParallaxFrame current = rdPtr->frames->at( AnimFrameWrap( rdPtr ) );
	if( current.parallaxImage == NULL ) return 0;

	float scrollCoeff = 0.0f;

	int pos = (rdPtr->direction == HORIZONTAL) ? rdPtr->rHo.hoY : rdPtr->rHo.hoX;
	int dim = (rdPtr->direction == HORIZONTAL) ? rdPtr->rHo.hoImgHeight : rdPtr->rHo.hoImgWidth;
	int pim = (rdPtr->direction == HORIZONTAL) ? current.parallaxImage->GetHeight() : current.parallaxImage->GetWidth();

	if( linePos >= pos && linePos < ( pos + dim ) )
		scrollCoeff = current.blurredLength[ (int)( (linePos - pos) * ( pim /(float)dim )) ] / (float)rdPtr->zLength;

	rdPtr->rHo.hoFlags |= HOF_FLOAT;
    return *((long*)&scrollCoeff);
}
	

long WINAPI DLLExport expGetLowerValue(LPRDATA rdPtr, long param1){
    return rdPtr->lowerLimit;
}

long WINAPI DLLExport expGetUpperValue(LPRDATA rdPtr, long param1){
    return rdPtr->upperLimit;
}

long WINAPI DLLExport expGetCenterOffset(LPRDATA rdPtr, long param1){
	rdPtr->rHo.hoFlags |= HOF_FLOAT;
	float fResult = rdPtr->centerOffset;
    return *((long*)&fResult);
}

long WINAPI DLLExport expGetAngle(LPRDATA rdPtr, long param1){
	rdPtr->rHo.hoFlags |= HOF_FLOAT;
	float fResult = rdPtr->angle;
    return *((long*)&fResult);
}



// ----------------------------------------------------------
// Condition / Action / Expression jump table
// ----------------------------------------------------------
// Contains the address inside the extension of the different
// routines that handle the action, conditions and expressions.
// Located at the end of the source for convinience
// Must finish with a 0
//
long (WINAPI * ConditionJumps[])(LPRDATA rdPtr, long param1, long param2) = 
	{ 
	0};
	
short (WINAPI * ActionJumps[])(LPRDATA rdPtr, long param1, long param2) =
	{
	actSetOffset,
	actSetWidth,
	actSetHeight,
	actSetResampleOn,
	actSetResampleOff,
	actSetZLength,
	actLoadImage,
	actSetFrame,
	actAttachObject,
	actDetachObject,
	actAutoScrollOn,
	actAutoScrollOff,
	actLowerSetValue,
	actUpperSetValue,
	actLowerOn,
	actLowerOff,
	actUpperOn,
	actUpperOff,
	actStationaryOn,
	actStationaryOff,
	actSetCenterOffset,
	actSetAngle,
	0
	};

long (WINAPI * ExpressionJumps[])(LPRDATA rdPtr, long param) = 
	{
	expGetOffset,
	expGetWidth,
	expGetHeight,
	expGetZLength,
	expGetAniFrame,
	expGetLineLength,
	expGetScrollCoeff,
	expGetLowerValue,
	expGetUpperValue,
	expGetCenterOffset,
	expGetAngle,
	0
	};