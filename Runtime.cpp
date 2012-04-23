// ============================================================================
//
// This file contains routines that are handled during the Runtime.
//
// Including creating, display, and handling your object.
// 
// ============================================================================

// Common Include
#include	"common.h"


/*short WINAPI GetZoneInfo(LPEDATA edPtr)
{
	int x = 1+2;
	return 0;
}*/


// --------------------
// GetRunObjectDataSize
// --------------------
// Returns the size of the runtime datazone of the object
// 
ushort WINAPI DLLExport GetRunObjectDataSize(fprh rhPtr, LPEDATA edPtr)
{
	return(sizeof(RUNDATA));
}


// ---------------
// CreateRunObject
// ---------------
// The routine where the object is actually created
// 
short WINAPI DLLExport CreateRunObject(LPRDATA rdPtr, LPEDATA edPtr, fpcob cobPtr)
{
	fprh rhPtr = rdPtr->rHo.hoAdRunHeader;
	LPSURFACE ps = WinGetSurface((int)rhPtr->rhIdEditWin);
	LPSURFACE pProto = NULL;

	rdPtr->frames = new vector<ParallaxFrame>();
	rdPtr->aObjects = new vector<AttachedObject>();

	rdPtr->offset =	edPtr->offset;
	rdPtr->roc.rcChanged = true;

	rdPtr->rHo.hoImgWidth = rdPtr->oldWidth = edPtr->swidth;
	rdPtr->rHo.hoImgHeight = rdPtr->oldHeight = edPtr->sheight;

	rdPtr->currentFrame = 0;
	rdPtr->finalImage = NULL;
	rdPtr->angle = 0.0f;

	rdPtr->resample = edPtr->resample;
	rdPtr->zLength = edPtr->zLength;
	rdPtr->direction = edPtr->direction;

	rdPtr->autoscroll = edPtr->autoscroll;
	rdPtr->hasLowerLimit = edPtr->hasLowerLimit;
	rdPtr->hasUpperLimit = edPtr->hasUpperLimit;
	rdPtr->lowerLimit = edPtr->lowerLimit;
	rdPtr->upperLimit = edPtr->upperLimit;
	rdPtr->stationary = edPtr->stationatory;
	rdPtr->smoothLines = edPtr->smoothLines;
	rdPtr->smoothAmount = edPtr->smoothAmount;

	//Aquire the edittime images into the animation list
	for( int i = 0; i < edPtr->numFrames; i++ )
	{
		cSurface imageSurface;
		cSurface * parallaxImage = new cSurface();
		::LockImageSurface( rhPtr->rhIdAppli, edPtr->imageFrames[i], imageSurface, LOCKIMAGE_READBLITONLY );
			parallaxImage->Clone( imageSurface );
		::UnlockImageSurface( imageSurface );

		ParallaxFrame animationFrame = AnalyzeImage( parallaxImage, rdPtr->direction, rdPtr->smoothLines, rdPtr->smoothAmount );
		rdPtr->frames->push_back( animationFrame );
	}

	return 0;
}


// ----------------
// DestroyRunObject
// ----------------
// Destroys the run-time object
// 
short WINAPI DLLExport DestroyRunObject(LPRDATA rdPtr, long fast)
{

	ClearAnimList( rdPtr->frames );

	rdPtr->aObjects->clear();
	delete rdPtr->aObjects;
	
	if( rdPtr->finalImage != NULL ){
		rdPtr->finalImage->Delete();
		delete rdPtr->finalImage;
	}

	return 0;
}


// ----------------
// HandleRunObject
// ----------------
// Called (if you want) each loop, this routine makes the object live
// 
short WINAPI DLLExport HandleRunObject(LPRDATA rdPtr)
{
	if (rdPtr->roc.rcChanged)
		return REFLAG_DISPLAY;
	
	return 0;
}

// ----------------
// DisplayRunObject
// ----------------
// Draw the object in the application screen.
// 
short WINAPI DLLExport DisplayRunObject(LPRDATA rdPtr)
{
	fprh rhPtr = rdPtr->rHo.hoAdRunHeader;
	LPSURFACE ps = WinGetSurface((int)rhPtr->rhIdEditWin);
	LPSURFACE pProto = NULL;

	int wrappedFrameNum = AnimFrameWrap( rdPtr );
	rdPtr->currentPframe = &rdPtr->frames->at( wrappedFrameNum );
	ParallaxFrame current = *rdPtr->currentPframe;

	CheckSurfaceRecreation(rdPtr);

	if( current.parallaxImage == NULL )
		return 0;

	if( rdPtr->autoscroll )
	{
		rdPtr->offset = ( rdPtr->direction == HORIZONTAL ) ? -rdPtr->rHo.hoAdRunHeader->rh3.rh3DisplayX : -rdPtr->rHo.hoAdRunHeader->rh3.rh3DisplayY;
		HandleAttachedObjects(rdPtr);
	}

	int			pWidth			= current.parallaxImage->GetWidth();
	int			pHeight			= current.parallaxImage->GetHeight();

	int			posX			= rdPtr->rHo.hoRect.left;
	int			posY			= rdPtr->rHo.hoRect.top;

	float		offset			= (float)rdPtr->offset;
	float		stationary		= (rdPtr->stationary) ? offset : 0;

	float		zLength			= (float)rdPtr->zLength;

	bool		frameAlpha = current.parallaxImage->HasAlpha() == 1;
	bool		finalAlpha = rdPtr->finalImage->HasAlpha() == 1;

	DWORD		drawMode		= (rdPtr->resample) ? STRF_RESAMPLE_TRANSP : 0;

	//Clear the image and alpha-channels
	rdPtr->finalImage->Fill( (COLORREF)0 );
	if(finalAlpha)
	{
		//I have Yves' word that GetAlphaPitch is > 0
		LPBYTE ptr = rdPtr->finalImage->LockAlpha();
		memset(ptr, 0, rdPtr->finalImage->GetAlphaPitch()*rdPtr->finalImage->GetHeight());
		rdPtr->finalImage->UnlockAlpha();
	}

	
	//Remove the alphachannel of the final image if the parallaximage doesn't have one:
	if(!frameAlpha && finalAlpha)
	{
		long newPitch = 0;
		rdPtr->finalImage->DetachAlpha((LPLONG)newPitch);
	}
	//Create the alpha if needed.
	else if(frameAlpha && !finalAlpha)
		rdPtr->finalImage->CreateAlpha();
	

	int	oWidth		= rdPtr->finalImage->GetWidth();
	int	oHeight		= rdPtr->finalImage->GetHeight();

	int	firstSliceStart	= 0;
	int	upperLimit	= (rdPtr->direction == HORIZONTAL) ? oWidth : oHeight;

	float blurred;
	int jumpAmount, start, length, scrollAmount, firstSlicePos, lastSlicePos;

	//Horizontal scrolling
	if( rdPtr->direction == HORIZONTAL )
	{
		jumpAmount = 1;
		for( int y = 0; y < pHeight; y += jumpAmount )
		{
			start = current.startArray[y];
			length = current.lengthArray[y];
			blurred = current.blurredLength[y];
			jumpAmount = current.blockArray[y];
			
			firstSliceStart = (oWidth - length)/2 + floor(rdPtr->centerOffset*(blurred/zLength));
			lastSlicePos = upperLimit;
			scrollAmount = floor( offset * (blurred/zLength) - stationary);

			if( !rdPtr->hasLowerLimit )
				firstSlicePos = -length + ((firstSliceStart+scrollAmount) % length);
			else
			{
				firstSlicePos = firstSliceStart + rdPtr->lowerLimit * length + scrollAmount;

				if(firstSlicePos < 0)
					firstSlicePos %= length;
			}

			if( rdPtr->hasUpperLimit )
			{
				lastSlicePos = firstSliceStart + rdPtr->upperLimit * length + scrollAmount;
				lastSlicePos = min( lastSlicePos, upperLimit );
			}

			//Repetition count starts at the first visible image-slice to lower the amount of repetitions.
			if(frameAlpha)
				for( int i = firstSlicePos; i < lastSlicePos; i+= length )
					current.parallaxImage->Blit(*rdPtr->finalImage,i,y,start,y,length,jumpAmount,BMODE_OPAQUE,BOP_COPY,0,BLTF_COPYALPHA);
			else
				for( int i = firstSlicePos; i < lastSlicePos; i+= length )
					current.parallaxImage->Blit(*rdPtr->finalImage,i,y,start,y,length,jumpAmount,BMODE_OPAQUE,BOP_COPY);
		}
	}
	//Vertical scrolling
	else
	{
		jumpAmount = 1;
		for( int x = 0; x < pWidth; x += jumpAmount )
		{
			start = current.startArray[x];
			length = current.lengthArray[x];
			blurred = current.blurredLength[x];
			jumpAmount = current.blockArray[x];

			firstSliceStart = (oHeight - length)/2 + floor(rdPtr->centerOffset*(blurred/zLength));
			lastSlicePos = upperLimit;
			scrollAmount = floor( offset * (blurred/zLength) - stationary);

			if( !rdPtr->hasLowerLimit )
				firstSlicePos = -length + ( (firstSliceStart+scrollAmount) % length);
			else
			{
				firstSlicePos = firstSliceStart + rdPtr->lowerLimit * length + scrollAmount;

				if(firstSlicePos < 0)
					firstSlicePos %= length;
			}

			if( rdPtr->hasUpperLimit )
			{
				lastSlicePos = firstSliceStart + rdPtr->upperLimit * length + scrollAmount;
				lastSlicePos = min( lastSlicePos, upperLimit );
			}

			if(frameAlpha)
				for( int i = firstSlicePos; i < lastSlicePos; i+= length )
					current.parallaxImage->Blit(*rdPtr->finalImage,x,i,x,start,jumpAmount,length,BMODE_OPAQUE,BOP_COPY,0,BLTF_COPYALPHA);
			else
				for( int i = firstSlicePos; i < lastSlicePos; i+= length )
					current.parallaxImage->Blit(*rdPtr->finalImage,x,i,x,start,jumpAmount,length,BMODE_OPAQUE,BOP_COPY);
		}
	}


	//-------------------
	// BLIT
	//-------------------

	rdPtr->finalImage->Stretch(
		*ps,
		rdPtr->rHo.hoRect.left,
		rdPtr->rHo.hoRect.top,
		rdPtr->rHo.hoImgWidth,
		rdPtr->rHo.hoImgHeight,
		0,
		0,
		rdPtr->finalImage->GetWidth(),
		rdPtr->finalImage->GetHeight(),
		( (rdPtr->rs.rsEffect & EFFECTFLAG_TRANSPARENT) ? BMODE_TRANSP : BMODE_OPAQUE ),
		(BlitOp)( rdPtr->rs.rsEffect & EFFECT_MASK ),
		rdPtr->rs.rsEffectParam,
		drawMode
	);

	WinAddZone(rhPtr->rhIdEditWin, &rdPtr->rHo.hoRect);

	return 0;
}

// -------------------
// GetRunObjectSurface
// -------------------
// Implement this function instead of DisplayRunObject if your extension
// supports ink effects and transitions. Note: you can support ink effects
// in DisplayRunObject too, but this is automatically done if you implement
// GetRunObjectSurface (MMF applies the ink effect to the transition).
//
// Note: do not forget to enable the function in the .def file 
// if you remove the comments below.

cSurface* WINAPI DLLExport GetRunObjectSurface(LPRDATA rdPtr)
{

	return NULL;
}


// -------------------------
// GetRunObjectCollisionMask
// -------------------------
// Implement this function if your extension supports fine collision mode (OEPREFS_FINECOLLISIONS),
// Or if it's a background object and you want Obstacle properties for this object.
//
// Should return NULL if the object is not transparent.
//
// Note: do not forget to enable the function in the .def file 
// if you remove the comments below.
//
/*
cSurface* WINAPI DLLExport GetRunObjectCollisionMask(LPRDATA rdPtr, LPARAM lParam)
{
	// Typical example for active objects
	// ----------------------------------
	// Opaque? collide with box
	if ( (rdPtr->rs.rsEffect & EFFECTFLAG_TRANSPARENT) == 0 )	// Note: only if your object has the OEPREFS_INKEFFECTS option
		return NULL;

	// Transparent? Create mask
	LPSMASK pMask = rdPtr->m_pColMask;
	if ( pMask == NULL )
	{
		if ( rdPtr->m_pSurface != NULL )
		{
			DWORD dwMaskSize = rdPtr->m_pSurface->CreateMask(NULL, lParam);
			if ( dwMaskSize != 0 )
			{
				pMask = (LPSMASK)calloc(dwMaskSize, 1);
				if ( pMask != NULL )
				{
					rdPtr->m_pSurface->CreateMask(pMask, lParam);
					rdPtr->m_pColMask = pMask;
				}
			}
		}
	}

	// Note: for active objects, lParam is always the same.
	// For background objects (OEFLAG_BACKGROUND), lParam maybe be different if the user uses your object
	// as obstacle and as platform. In this case, you should store 2 collision masks
	// in your data: one if lParam is 0 and another one if lParam is different from 0.

	return pMask;
}
*/

// ----------------
// PauseRunObject
// ----------------
// Enters the pause mode
// 
short WINAPI DLLExport PauseRunObject(LPRDATA rdPtr)
{
	// Ok
	return 0;
}


// -----------------
// ContinueRunObject
// -----------------
// Quits the pause mode
//
short WINAPI DLLExport ContinueRunObject(LPRDATA rdPtr)
{
	// Ok
	return 0;
}


// ============================================================================
//
// START APP / END APP / START FRAME / END FRAME routines
// 
// ============================================================================

// -------------------
// StartApp
// -------------------
// Called when the application starts or restarts.
// Useful for storing global data
// 
void WINAPI DLLExport StartApp(mv _far *mV, CRunApp* pApp)
{
	OutputDebugString("Start app\n");

	// Example
	// -------
	// Delete global data (if restarts application)
//	CMyData* pData = (CMyData*)mV->mvGetExtUserData(pApp, hInstLib);
//	if ( pData != NULL )
//	{
//		delete pData;
//		mV->mvSetExtUserData(pApp, hInstLib, NULL);
//	}
}

// -------------------
// EndApp
// -------------------
// Called when the application ends.
// 
void WINAPI DLLExport EndApp(mv _far *mV, CRunApp* pApp)
{
	OutputDebugString("End app\n");

	// Example
	// -------
	// Delete global data
//	CMyData* pData = (CMyData*)mV->mvGetExtUserData(pApp, hInstLib);
//	if ( pData != NULL )
//	{
//		delete pData;
//		mV->mvSetExtUserData(pApp, hInstLib, NULL);
//	}
}

// -------------------
// StartFrame
// -------------------
// Called when the frame starts or restarts.
// 
void WINAPI DLLExport StartFrame(mv _far *mV, DWORD dwReserved, int nFrameIndex)
{
	char tutu[100];
	wsprintf(tutu, "Start Frame %d\n", nFrameIndex);

	OutputDebugString(tutu);
}

// -------------------
// EndFrame
// -------------------
// Called when the frame ends.
// 
void WINAPI DLLExport EndFrame(mv _far *mV, DWORD dwReserved, int nFrameIndex)
{
	char tutu[100];
	wsprintf(tutu, "End Frame %d\n", nFrameIndex);

	OutputDebugString(tutu);
}

// ============================================================================
//
// TEXT ROUTINES (if OEFLAG_TEXT)
// 
// ============================================================================

// -------------------
// GetRunObjectFont
// -------------------
// Return the font used by the object.
// 
/*

  // Note: do not forget to enable the functions in the .def file 
  // if you remove the comments below.

void WINAPI GetRunObjectFont(LPRDATA rdPtr, LOGFONT* pLf)
{
	// Example
	// -------
	// GetObject(rdPtr->m_hFont, sizeof(LOGFONT), pLf);
}

// -------------------
// SetRunObjectFont
// -------------------
// Change the font used by the object.
// 
void WINAPI SetRunObjectFont(LPRDATA rdPtr, LOGFONT* pLf, RECT* pRc)
{
	// Example
	// -------
//	HFONT hFont = CreateFontIndirect(pLf);
//	if ( hFont != NULL )
//	{
//		if (rdPtr->m_hFont!=0)
//			DeleteObject(rdPtr->m_hFont);
//		rdPtr->m_hFont = hFont;
//		SendMessage(rdPtr->m_hWnd, WM_SETFONT, (WPARAM)rdPtr->m_hFont, FALSE);
//	}

}

// ---------------------
// GetRunObjectTextColor
// ---------------------
// Return the text color of the object.
// 
COLORREF WINAPI GetRunObjectTextColor(LPRDATA rdPtr)
{
	// Example
	// -------
	return 0;	// rdPtr->m_dwColor;
}

// ---------------------
// SetRunObjectTextColor
// ---------------------
// Change the text color of the object.
// 
void WINAPI SetRunObjectTextColor(LPRDATA rdPtr, COLORREF rgb)
{
	// Example
	// -------
	rdPtr->m_dwColor = rgb;
	InvalidateRect(rdPtr->m_hWnd, NULL, TRUE);
}
*/


// ============================================================================
//
// DEBUGGER ROUTINES
// 
// ============================================================================

// DEBUGGER /////////////////////////////////////////////////////////////////

#if !defined(RUN_ONLY)
// Identifiers of items displayed in the debugger
enum
{
	DB_OFFSET,
	DB_LOWERBOUND,
	DB_UPPERBOUND,
	DB_NUMOBJATTACHED,
	DB_CURRENTFRAMENUM,
	DB_FRAMEWIDTH,
	DB_FRAMEHEIGT,
	DB_INTERNALWIDTH,
	DB_INTERNALHEIGHT,
	DB_DEBUGC
};

// Items displayed in the debugger
WORD DebugTree[]=
{
	DB_OFFSET|DB_EDITABLE,
	DB_LOWERBOUND|DB_EDITABLE,
	DB_UPPERBOUND|DB_EDITABLE,
	DB_NUMOBJATTACHED,
	DB_CURRENTFRAMENUM|DB_EDITABLE,
	DB_FRAMEWIDTH,
	DB_FRAMEHEIGT,
	DB_INTERNALWIDTH,
	DB_INTERNALHEIGHT,
	DB_DEBUGC,
	DB_END
};
#endif // !defined(RUN_ONLY)


// -----------------
// GetDebugTree
// -----------------
// This routine returns the address of the debugger tree
//
LPWORD WINAPI DLLExport GetDebugTree(LPRDATA rdPtr)
{
#if !defined(RUN_ONLY)
	return DebugTree;
#else
	return NULL;
#endif // !defined(RUN_ONLY)
}

// -----------------
// GetDebugItem
// -----------------
// This routine returns the text of a given item.
//
void WINAPI DLLExport GetDebugItem(LPSTR pBuffer, LPRDATA rdPtr, int id)
{
#if !defined(RUN_ONLY)

	switch (id)
	{
	case DB_OFFSET:
		wsprintf(pBuffer, "Offset: %i", rdPtr->offset);
		break;
	case DB_LOWERBOUND:
		if(!rdPtr->hasLowerLimit)
			wsprintf(pBuffer, "Lower bound: (off)");
		else
			wsprintf(pBuffer, "Lower bound: %i", rdPtr->lowerLimit);
		break;
	case DB_UPPERBOUND:
		if(!rdPtr->hasUpperLimit)
			wsprintf(pBuffer, "Lower bound: (off)");
		else
			wsprintf(pBuffer, "Upper bound: %i", rdPtr->upperLimit );
		break;
	case DB_NUMOBJATTACHED:
			wsprintf(pBuffer, "Attached objects: %i", rdPtr->aObjects->size() );
		break;
	case DB_CURRENTFRAMENUM:
		wsprintf(pBuffer, "Current frame: %i", AnimFrameWrap(rdPtr) );
		break;
	case DB_FRAMEWIDTH:
		{
			int wrappedFrameNum = AnimFrameWrap( rdPtr );
			ParallaxFrame current = rdPtr->frames->at( wrappedFrameNum );
			wsprintf(pBuffer, "Frame width: %i", current.parallaxImage->GetWidth());
			break;
		}
	case DB_FRAMEHEIGT:
		{
			int wrappedFrameNum = AnimFrameWrap( rdPtr );
			ParallaxFrame current = rdPtr->frames->at( wrappedFrameNum );
			wsprintf(pBuffer, "Frame height: %i", current.parallaxImage->GetHeight());
			break;
		}
	case DB_INTERNALWIDTH:
		wsprintf(pBuffer, "Internal width: %i", rdPtr->oldWidth);
		break;
	case DB_INTERNALHEIGHT:
		wsprintf(pBuffer, "Internal height: %i", rdPtr->oldHeight);
		break;
	}
#endif // !defined(RUN_ONLY)
}

// -----------------
// EditDebugItem
// -----------------
// This routine allows to edit editable items.
//
void WINAPI DLLExport EditDebugItem(LPRDATA rdPtr, int id)
{
#if !defined(RUN_ONLY)

	// Example
	// -------
/*
	switch (id)
	{
	case DB_CURRENTSTRING:
		{
			EditDebugInfo dbi;
			char buffer[256];

			dbi.pText=buffer;
			dbi.lText=TEXT_MAX;
			dbi.pTitle=NULL;

			strcpy(buffer, rdPtr->text);
			long ret=callRunTimeFunction(rdPtr, RFUNCTION_EDITTEXT, 0, (LPARAM)&dbi);
			if (ret)
				strcpy(rdPtr->text, dbi.pText);
		}
		break;
	case DB_CURRENTVALUE:
		{
			EditDebugInfo dbi;

			dbi.value=rdPtr->value;
			dbi.pTitle=NULL;

			long ret=callRunTimeFunction(rdPtr, RFUNCTION_EDITINT, 0, (LPARAM)&dbi);
			if (ret)
				rdPtr->value=dbi.value;
		}
		break;
	}
*/
#endif // !defined(RUN_ONLY)
}


