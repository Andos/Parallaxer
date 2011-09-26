
#include	"common.h"



// -----------------------------------------
// Returns a fixed value from an LPRO value
// -----------------------------------------
long FixedValue( LPRO object )
{
	return (object->roHo.hoCreationId << 16) + object->roHo.hoNumber;
}


// ------------------------------------------------
// Return a valid LPRO pointer from a fixed value,
// NULL if object doesn't exist.
// ------------------------------------------------
LPRO LPROFromFixed( LPRDATA rdPtr, long fixedvalue )
{
	LPOBL objList = rdPtr->rHo.hoAdRunHeader->rhObjectList;
	int index = 0x0000FFFF & fixedvalue;

	if (index < 0 || index >= 10000)
		return NULL;

	LPRO theObject = (LPRO)objList[0x0000FFFF & fixedvalue].oblOffset;

	if (theObject == NULL)
		return NULL;
	else if (FixedValue(theObject) != fixedvalue)
		return NULL;

	return theObject;
}


// -----------------------------------------------------------
// Wraps the current image frame number to an existing image
// -----------------------------------------------------------
int AnimFrameWrap( LPRDATA rdPtr )
{
	int wrappedFrameNum = rdPtr->currentFrame % rdPtr->frames->size();
	while( wrappedFrameNum < 0 ) wrappedFrameNum += rdPtr->frames->size();
	return wrappedFrameNum;
}


void Debug( char * string, int value )
{
	char output [1024];
	sprintf( output, "%s: %i", string, value );
	MessageBox(NULL,output,"",NULL);
}

void Debug( char * string, int valueA, int valueB )
{
	char output [1024];
	sprintf( output, "%s: %i   %i", string, valueA, valueB );
	MessageBox(NULL,output,"",NULL);
}


// -----------------------------------------------------------
// Analyzes the Parallaxer Image and returns a ParallaxFrame
// -----------------------------------------------------------
ParallaxFrame AnalyzeImage( cSurface * parallaxImage, bool direction, bool smoothLines, int smoothAmount ){

	ParallaxFrame frame;
	
	ParallaxFrame nullframe;
	nullframe.startArray = NULL;
	nullframe.lengthArray = NULL;
	nullframe.blockArray = NULL;
	nullframe.parallaxImage = NULL;

	if( parallaxImage == NULL || !parallaxImage->IsValid() )
		return nullframe;

	frame.parallaxImage = parallaxImage;

	//Analyze image for width/height of lines
	int width  = parallaxImage->GetWidth();
	int height = parallaxImage->GetHeight();
	int arrayLength = direction ? width : height;

	if(width == 0 || height == 0)
		return nullframe;

	frame.startArray  = new int [ arrayLength ];
	frame.lengthArray = new int [ arrayLength ];
	frame.blockArray = new int [ arrayLength ];
	frame.blurredLength = new double[arrayLength];

	//Set all blocksizes to 1
	for( int l=0; l < arrayLength; l++)
	{
		frame.startArray[l] = 0;
		frame.blockArray[l] = 1;
		frame.lengthArray[l] = 1;
		frame.blurredLength[l] = 1;
	}

	COLORREF transparent = parallaxImage->GetTransparentColor();
	bool hasAlpha = parallaxImage->HasAlpha() == 1;
	int	pitch = 1;

	//Lock resources
	LPBYTE lock = parallaxImage->LockBuffer();
	LPBYTE aLock = NULL;
	if(hasAlpha)
	{
		aLock = parallaxImage->LockAlpha();
		pitch = parallaxImage->GetAlphaPitch();
		transparent = 0;
	}

	//Horizontal
	if( direction == HORIZONTAL )
	{
		bool metLine;
		for( int y = 0; y < height; y++ )
		{
			metLine = false;

			for( int xa = 0; xa < width; xa++ )
			{
				COLORREF color = (!hasAlpha) ? parallaxImage->GetPixelFast(xa,y) : (COLORREF)aLock[pitch*y+xa];
				if( color != transparent )
				{
					metLine = true;
					frame.startArray[y] = xa;
					break;
				}
			}

			if( metLine )
				for( int xb = width-1; xb > 0; xb-- )
				{
					COLORREF color = (!hasAlpha) ? parallaxImage->GetPixelFast(xb,y) : (COLORREF)aLock[pitch*y+xb];
					if( color != transparent )
					{
						frame.lengthArray[y] = xb - frame.startArray[y] + 1;
						frame.blurredLength[y] = frame.lengthArray[y];
						break;
					}	
				}
			else
			{
				frame.startArray[y] = 0;
				frame.lengthArray[y] = width;
				frame.blurredLength[y] = width;
			}
		}
	}
	//Vertical
	else
	{
		bool metLine;
		for( int x = 0; x < width; x++ ){
			metLine = false;

			for( int ya = 0; ya < height; ya++ )
			{
				COLORREF color = (!hasAlpha) ? parallaxImage->GetPixelFast(x,ya) : (COLORREF)aLock[width*ya+x];
				if( color != transparent )
				{
					metLine = true;
					frame.startArray[x] = ya;
					break;
				}
			}
			if( metLine )
				for( int yb = height-1; yb > 0; yb-- )
				{
					COLORREF color = (!hasAlpha) ? parallaxImage->GetPixelFast(x,yb) : (COLORREF)aLock[width*yb+x];
					if( color != transparent ){
						frame.lengthArray[x] = yb - frame.startArray[x] + 1;
						frame.blurredLength[x] = frame.lengthArray[x];
						break;
					}
				}
			else{
				frame.startArray[x] = 0;
				frame.lengthArray[x] = height;
				frame.blurredLength[x] = height;
			}
		}
	}
	//Unlock resources
	parallaxImage->UnlockBuffer(lock);
	if(hasAlpha)
		parallaxImage->UnlockAlpha();

	if(smoothLines)
		SmoothLine(&frame, arrayLength, smoothAmount);


	int blockStart = 0;
	for( int i = 0; i < arrayLength; i++ )
	{
		//Block copy optimization
		//If previous line is equal (start/length), increase blocksize
		if( frame.startArray[ blockStart ] == frame.startArray[ i ] && frame.blurredLength[ blockStart ] == frame.blurredLength[ i ] && blockStart != 0)
			frame.blockArray[ blockStart ] += 1;
		else
			blockStart = i;
	}

	return frame;
}

void SmoothLine(ParallaxFrame * frame, int arrayLength, int amount)
{
	double * window = new double[arrayLength];

	for(int i = 0; i<arrayLength; i++)
	{
		int x = floor(i - arrayLength/2);
		window[i] = 1.0/(sqrt(2*PI)*amount)  *  pow( 2.71828183, -( pow(x,2)/(  2*(pow(amount,2))  ) ) );
	}

	double total = 0.0;
	for(int w = 0; w<arrayLength; w++)
		total += window[w];

	for(int t = 0; t<arrayLength; t++)
		window[t] = window[t] / total;

	for(int u = 0; u<arrayLength; u++)
	{
		double finalValue = 0;
		for(int v = -arrayLength/2; v<arrayLength/2; v++)
		{
			int length = frame->lengthArray[max(min(v+u,arrayLength-1),0)];
			finalValue += window[v+arrayLength/2] * length;
		}
		frame->blurredLength[u] = finalValue;
	}

	delete [] window;
}




void HandleAttachedObjects(LPRDATA rdPtr)
{
	int wrappedFrameNum = AnimFrameWrap( rdPtr );
	rdPtr->currentPframe = &rdPtr->frames->at( wrappedFrameNum );
	ParallaxFrame current = *rdPtr->currentPframe;

	//Handle attached objects
	for( int i = 0; i < rdPtr->aObjects->size(); i++ )
	{
		AttachedObject objStruct = rdPtr->aObjects->at( i );

		LPRO object = LPROFromFixed( rdPtr, objStruct.fixedValue );
		if( object == NULL )
		{
			rdPtr->aObjects->erase( rdPtr->aObjects->begin() + i );
			i--;
			break;
		}
		else
		{
			if( current.parallaxImage == NULL )
				break;
			double length = current.blurredLength[ objStruct.atLine ];

			if( rdPtr->direction == HORIZONTAL )
			{
				int center = rdPtr->rHo.hoX + rdPtr->rHo.hoImgWidth/2;
				object->roHo.hoX = int(center + objStruct.startOffset + rdPtr->offset * ( length / (float)rdPtr->zLength ));
				object->roHo.hoY = int(rdPtr->rHo.hoY + objStruct.atLine / ( current.parallaxImage->GetHeight() / (float)rdPtr->rHo.hoImgHeight ));
			}
			
			if( rdPtr->direction == VERTICAL )
			{
				int center = rdPtr->rHo.hoY + rdPtr->rHo.hoImgHeight/2;
				object->roHo.hoY = int(center + objStruct.startOffset +  rdPtr->offset * ( length / (float)rdPtr->zLength ));
				object->roHo.hoX = int(rdPtr->rHo.hoX + objStruct.atLine / ( current.parallaxImage->GetWidth() / (float)rdPtr->rHo.hoImgWidth ));
			}
			
			//Update the object
			object->roc.rcChanged = true;
		}
	}
}


void CheckSurfaceRecreation(LPRDATA rdPtr)
{

	fprh rhPtr = rdPtr->rHo.hoAdRunHeader;
	LPSURFACE ps = WinGetSurface((int)rhPtr->rhIdEditWin);
	LPSURFACE pProto = NULL;

	ParallaxFrame current = *rdPtr->currentPframe;

	//Recreate surface if nessecary.
	if( (rdPtr->direction == HORIZONTAL && rdPtr->rHo.hoImgWidth != rdPtr->oldWidth) ||
		(rdPtr->direction == VERTICAL && rdPtr->rHo.hoImgHeight != rdPtr->oldHeight) ||
		(rdPtr->finalImage == NULL)
	)
	{
		if( rdPtr->finalImage != NULL )
		{
			rdPtr->finalImage->Delete();
			delete rdPtr->finalImage;
		}
		rdPtr->finalImage = new cSurface();
		if ( GetSurfacePrototype( &pProto, ps->GetDepth(), ST_MEMORY, SD_DIB) )
		{
			int newWidth = (rdPtr->direction == HORIZONTAL) ? rdPtr->rHo.hoImgWidth : current.parallaxImage->GetWidth();
			int newHeight = (rdPtr->direction == HORIZONTAL) ? current.parallaxImage->GetHeight() : rdPtr->rHo.hoImgHeight;

			rdPtr->finalImage->Create(newWidth, newHeight, pProto);

			if( !rdPtr->finalImage->IsValid() )
			{
				rdPtr->finalImage->Delete();
				delete rdPtr->finalImage;
			}
				
		}
	}

	rdPtr->oldWidth = rdPtr->rHo.hoImgWidth;
	rdPtr->oldHeight = rdPtr->rHo.hoImgHeight;
}

// --------------------------------------------------------
// Clears a ParallaxFrame list and releases the resources
// --------------------------------------------------------
void ClearAnimList( vector<ParallaxFrame> * animList ){
	for( int i = 0; i < animList->size(); i++ ){
		ParallaxFrame temp = animList->at( i );
		if(temp.parallaxImage != NULL) temp.parallaxImage->Delete();
		if(temp.startArray    != NULL) delete [] temp.startArray;
		if(temp.lengthArray   != NULL) delete [] temp.lengthArray;
		if(temp.blockArray    != NULL) delete [] temp.blockArray;
		if(temp.blurredLength != NULL) delete [] temp.blurredLength;
	}
	animList->clear();
	delete animList;
}




// ---------------------------------------
// Load Image File function w. filters.
// ---------------------------------------
BOOL LoadImageFile(LPRDATA rdPtr, LPSURFACE psf, LPSTR pFileName, DWORD dwFlags) 
{ 
	BOOL    bOK = FALSE; 
	LPRH    rhPtr = rdPtr->rHo.hoAdRunHeader; 
	HANDLE    hf = INVALID_HANDLE_VALUE; 

	do { 
		if ( pFileName == NULL || *pFileName == 0 ) 
			break; 

		// Copy filename to temporary string 
		char fname[MAX_PATH]; 
		strcpy(fname, pFileName); 

		// Open file 
		// 
		// mvOpenHFile opens the file directly from the CCN/EXE file ifthe file has been included in the embedded binary files of the application. 
		// If the file is not embedded, mvOpenHFile opens it from its current location (and downloads it first in Vitalize mode). 
		// 
		DWORD dwSize;                                    // file size 
		hf = rhPtr->rh4.rh4Mv->mvOpenHFile(fname, &dwSize, 0); 
		if ( hf == INVALID_HANDLE_VALUE ) 
			break; 

		// Create CInputFile 
		DWORD dwOff = File_GetPosition((HFILE)hf);        // position of embedded file in CCN/EXE file (0 if not embedded) 
		CInputBufFile bf; 
		if ( bf.Create((HFILE)hf, dwOff, dwSize) != 0 ) 
			break; 

		// Load picture 
		if ( ImportImageFromInputFile(rhPtr->rh4.rh4Mv->mvImgFilterMgr, &bf, psf, NULL, dwFlags) ) 
			bOK = TRUE; 

	} while(FALSE); 

	if ( hf != INVALID_HANDLE_VALUE ) 
		rhPtr->rh4.rh4Mv->mvCloseHFile(hf); 

	return bOK; 
}



RECT FitBox(int width, int height, int maxWidth, int maxHeight)
{
	int newWidth = width;
	int newHeight = height;
	float aspect = width / (float)height;
	if( newWidth > maxWidth )
	{
		newWidth = maxWidth;
		newHeight = (int)floor(newWidth / aspect);
	}
	if( newHeight > maxHeight )
	{
		newHeight = maxHeight;
		newWidth = (int)floor(newHeight * aspect);
	}

	RECT ret;



	ret.left = maxWidth/2 - newWidth/2;
	ret.top = maxHeight/2 - newHeight/2;

	ret.right = newWidth;
	ret.bottom = newHeight;

	return ret;
}