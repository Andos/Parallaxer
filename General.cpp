
// ============================================================================
//
// The following routines are used internally by MMF, and should not need to
// be modified.
//
// 
// ============================================================================

// Common Include
#include	"common.h"

HINSTANCE hInstLib;

// ============================================================================
//
// LIBRARY ENTRY & QUIT POINTS
// 
// ============================================================================

// -----------------
// Entry points
// -----------------
// Usually you do not need to do any initialization here: you will prefer to 
// do them in "Initialize" found in Edittime.cpp
BOOL WINAPI DllMain(HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
		// DLL is attaching to the address space of the current process.
		case DLL_PROCESS_ATTACH:
			
			hInstLib = hDLL; // Store HINSTANCE
			break;

		// A new thread is being created in the current process.
		case DLL_THREAD_ATTACH:
			break;

		// A thread is exiting cleanly.
		case DLL_THREAD_DETACH:
			break;

		// The calling process is detaching the DLL from its address space.
	    case DLL_PROCESS_DETACH:
			break;
	}
	
	return TRUE;
}

// -----------------
// Initialize
// -----------------
// Where you want to do COLD-START initialization.
// Called when the extension is loaded into memory.
//
extern "C" int WINAPI DLLExport Initialize(mv _far *mV, int quiet)
{
	// No error
	return 0;
}

// -----------------
// Free
// -----------------
// Where you want to kill and initialized data opened in the above routine
// Called just before freeing the DLL.
// 
extern "C" int WINAPI DLLExport Free(mv _far *mV)
{
	// No error
	return 0;
}

// ============================================================================
//
// GENERAL INFO
// 
// ============================================================================

// -----------------
// Get Infos
// -----------------
// 
extern "C" 
{
	DWORD WINAPI DLLExport GetInfos(int info)
	{
		
		switch (info)
		{
			case KGI_VERSION:
				return EXT_VERSION2;				// Do not change
			case KGI_PLUGIN:
				return EXT_PLUGIN_VERSION1;			// Do not change
			case KGI_PRODUCT:
				return PRODUCT_VERSION_STANDARD;	// Works with MMF Standard or above
			case KGI_BUILD:
				return 219;							// Works with build 219 or above
			default:
				return 0;
		}
	}
}

// ----------------------------------------------------------
// GetRunObjectInfos
// ----------------------------------------------------------
// Fills an information structure that tells CC&C everything
// about the object, its actions, conditions and expressions
// 

short WINAPI DLLExport GetRunObjectInfos(mv _far *mV, fpKpxRunInfos infoPtr)
{
	infoPtr->conditions = (LPBYTE)ConditionJumps;
	infoPtr->actions = (LPBYTE)ActionJumps;
	infoPtr->expressions = (LPBYTE)ExpressionJumps;

	infoPtr->numOfConditions = CND_LAST;
	infoPtr->numOfActions = ACT_LAST;
	infoPtr->numOfExpressions = EXP_LAST;

	infoPtr->editDataSize = MAX_EDITSIZE;
	infoPtr->editFlags= OEFLAGS;

	infoPtr->windowProcPriority = WINDOWPROC_PRIORITY;

	// See doc
	infoPtr->editPrefs = OEPREFS;

	// Identifier, for run-time identification
	infoPtr->identifier = IDENTIFIER;
	
	// Current version
	infoPtr->version = KCX_CURRENT_VERSION;
	
	return TRUE;
}

// ----------------------------------------------------------
// GetDependencies
// ----------------------------------------------------------
// Returns the name of the external modules that you wish MMF to include
// with stand-alone applications (these modules must be in the MMF
// Data\Runtime folder).
// 

//LPCSTR szDep[] = {
//	"MyDll.dll",
//	NULL
//};

LPCSTR* WINAPI DLLExport GetDependencies()
{
	return NULL;	// szDep;
}

// -----------------
// LoadObject
// -----------------
// Routine called for each object when the object is read from the MFA file (edit time)
// or from the CCN or EXE file (run time).
// You can load data here, reserve memory etc...
//
int	WINAPI DLLExport LoadObject(mv _far *mV, LPCSTR fileName, LPEDATA edPtr, int reserved)
{
	return 0;
}

// -----------------
// UnloadObject
// -----------------
// The counterpart of the above routine: called just before the object is
// deleted from the frame.
//
void WINAPI DLLExport UnloadObject(mv _far *mV, LPEDATA edPtr, int reserved)
{
}

// --------------------
// UpdateEditStructure
// --------------------
// For you to update your object structure to newer versions
// Called at both edit time and run time
// 
HGLOBAL WINAPI DLLExport UpdateEditStructure(mv __far *mV, void __far * OldEdPtr)
{
	
	HGLOBAL hgNew;
	unsigned long originalVersion = ((tagEDATA_V1*)OldEdPtr)->eHeader.extVersion;
	
	while( ((tagEDATA_V1*)OldEdPtr)->eHeader.extVersion != KCX_CURRENT_VERSION )
	{

		hgNew = NULL;

		switch( ((tagEDATA_V1*)OldEdPtr)->eHeader.extVersion ){

		case 1:{
				tagEDATA_V1 * oldEdPtr = ((tagEDATA_V1*)OldEdPtr);
				tagEDATA_V3 * newEdPtr;

				if ((hgNew = GlobalAlloc(GPTR, sizeof(tagEDATA_V3))) != NULL){

					newEdPtr = (tagEDATA_V3 *)GlobalLock( hgNew );
					memcpy(&newEdPtr->eHeader, &oldEdPtr->eHeader, sizeof(extHeader));
					newEdPtr->eHeader.extVersion = 3;					// Update the version number
					newEdPtr->eHeader.extSize = sizeof(tagEDATA_V3);    // Update the EDITDATA structure size

					newEdPtr->sx = oldEdPtr->sx;
					newEdPtr->sy = oldEdPtr->sy;
					newEdPtr->swidth = oldEdPtr->swidth;
					newEdPtr->sheight = oldEdPtr->sheight;
					newEdPtr->direction = oldEdPtr->direction;
					newEdPtr->offset = oldEdPtr->offset;
					newEdPtr->resample = oldEdPtr->resample;
					newEdPtr->zLength = oldEdPtr->zLength;

					newEdPtr->numFrames = 1;
					newEdPtr->imageFrames[0] = oldEdPtr->parallaxImage;
					
					GlobalUnlock(hgNew);
				}
				break;
				
			}
		case 3:{
				tagEDATA_V3 * oldEdPtr = ((tagEDATA_V3*)OldEdPtr);
				tagEDATA_V4 * newEdPtr;

				unsigned long newsize = sizeof(tagEDATA_V4) + sizeof(WORD) * oldEdPtr->numFrames;
				if ((hgNew = GlobalAlloc(GPTR, newsize )) != NULL){

					newEdPtr = (tagEDATA_V4 *)GlobalLock( hgNew );
					memcpy(&newEdPtr->eHeader, &oldEdPtr->eHeader, sizeof(extHeader));
					newEdPtr->eHeader.extVersion = 4;					// Update the version number
					newEdPtr->eHeader.extSize = newsize;    // Update the EDITDATA structure size

					newEdPtr->sx = oldEdPtr->sx;
					newEdPtr->sy = oldEdPtr->sy;
					newEdPtr->swidth = oldEdPtr->swidth;
					newEdPtr->sheight = oldEdPtr->sheight;
					newEdPtr->direction = oldEdPtr->direction;
					newEdPtr->offset = oldEdPtr->offset;
					newEdPtr->resample = oldEdPtr->resample;
					newEdPtr->zLength = oldEdPtr->zLength;

					newEdPtr->autoscroll = false;
					newEdPtr->hasLowerLimit = false;
					newEdPtr->hasUpperLimit	= false;
					newEdPtr->lowerLimit = 0;
					newEdPtr->upperLimit = 1;				

					newEdPtr->numFrames = oldEdPtr->numFrames;
					for( int i = 0; i < oldEdPtr->numFrames; i++ ){
						newEdPtr->imageFrames[i] = oldEdPtr->imageFrames[i];
					}
					
					GlobalUnlock(hgNew);
			   }
				break;
			}
		case 4:{
				tagEDATA_V4 * oldEdPtr = ((tagEDATA_V4*)OldEdPtr);
				tagEDATA_V5 * newEdPtr;

				unsigned long newsize = sizeof(tagEDATA_V5) + sizeof(WORD) * oldEdPtr->numFrames;
				if ((hgNew = GlobalAlloc(GPTR, newsize )) != NULL){

					newEdPtr = (tagEDATA_V5 *)GlobalLock( hgNew );
					memcpy(&newEdPtr->eHeader, &oldEdPtr->eHeader, sizeof(extHeader));
					newEdPtr->eHeader.extVersion = 5;					// Update the version number
					newEdPtr->eHeader.extSize = newsize;    // Update the EDITDATA structure size

					newEdPtr->sx = oldEdPtr->sx;
					newEdPtr->sy = oldEdPtr->sy;
					newEdPtr->swidth = oldEdPtr->swidth;
					newEdPtr->sheight = oldEdPtr->sheight;
					newEdPtr->direction = oldEdPtr->direction;
					newEdPtr->offset = oldEdPtr->offset;
					newEdPtr->resample = oldEdPtr->resample;
					newEdPtr->zLength = oldEdPtr->zLength;

					newEdPtr->autoscroll = oldEdPtr->autoscroll;
					newEdPtr->hasLowerLimit = oldEdPtr->hasLowerLimit;
					newEdPtr->hasUpperLimit	= oldEdPtr->hasUpperLimit;
					newEdPtr->lowerLimit = oldEdPtr->lowerLimit;
					newEdPtr->upperLimit = oldEdPtr->upperLimit;
					newEdPtr->stationatory = false;

					newEdPtr->numFrames = oldEdPtr->numFrames;
					for( int i = 0; i < oldEdPtr->numFrames; i++ ){
						newEdPtr->imageFrames[i] = oldEdPtr->imageFrames[i];
					}
					
					GlobalUnlock(hgNew);
				}
				break;
			}
		case 5:{
				tagEDATA_V5 * oldEdPtr = ((tagEDATA_V5*)OldEdPtr);
				tagEDATA_V6 * newEdPtr;

				unsigned long newsize = sizeof(tagEDATA_V6) + sizeof(WORD) * oldEdPtr->numFrames;
				if ((hgNew = GlobalAlloc(GPTR, newsize )) != NULL){

					newEdPtr = (tagEDATA_V6 *)GlobalLock( hgNew );
					memcpy(&newEdPtr->eHeader, &oldEdPtr->eHeader, sizeof(extHeader));
					newEdPtr->eHeader.extVersion = 6;					// Update the version number
					newEdPtr->eHeader.extSize = newsize;    // Update the EDITDATA structure size

					newEdPtr->sx = oldEdPtr->sx;
					newEdPtr->sy = oldEdPtr->sy;
					newEdPtr->swidth = oldEdPtr->swidth;
					newEdPtr->sheight = oldEdPtr->sheight;
					newEdPtr->direction = oldEdPtr->direction;
					newEdPtr->offset = oldEdPtr->offset;
					newEdPtr->resample = oldEdPtr->resample;
					newEdPtr->zLength = oldEdPtr->zLength;

					newEdPtr->autoscroll = oldEdPtr->autoscroll;
					newEdPtr->hasLowerLimit = oldEdPtr->hasLowerLimit;
					newEdPtr->hasUpperLimit	= oldEdPtr->hasUpperLimit;
					newEdPtr->lowerLimit = oldEdPtr->lowerLimit;
					newEdPtr->upperLimit = oldEdPtr->upperLimit;
					newEdPtr->stationatory = oldEdPtr->stationatory;

					newEdPtr->smoothLines = false;
					newEdPtr->smoothAmount = 1;

					newEdPtr->numFrames = oldEdPtr->numFrames;
					for( int i = 0; i < oldEdPtr->numFrames; i++ ){
						newEdPtr->imageFrames[i] = oldEdPtr->imageFrames[i];
					}
					
					GlobalUnlock(hgNew);
				}
				break;
			}
		}
		OldEdPtr = hgNew;
	}

	//MessageBox(NULL,"Updated Parallaxer object to build 5","Parallax update",NULL);
	return hgNew;
}

// --------------------
// UpdateFileNames
// --------------------
// If you store file names in your datazone, they have to be relocated when the
// application is moved to a different directory: this routine does it.
// Called at edit time and run time.
//
// Call lpfnUpdate to update your file pathname (refer to the documentation)
// 
void WINAPI DLLExport UpdateFileNames(mv _far *mV, LPSTR appName, LPEDATA edPtr, void (WINAPI * lpfnUpdate)(LPSTR, LPSTR))
{
}

// ---------------------
// EnumElts
// ---------------------
//
// Uncomment this function if you need to store an image in the image bank.
//
// Note: do not forget to enable the function in the .def file 
// if you remove the comments below.
//

int WINAPI  EnumElts (LPMV mV, LPEDATA edPtr, ENUMELTPROC enumProc, ENUMELTPROC undoProc, LPARAM lp1, LPARAM lp2)
{
	int error = 0;
	for ( int i = 0; i < edPtr->numFrames; i++){
		error = enumProc( &edPtr->imageFrames[i], IMG_TAB, lp1, lp2);
		if ( error ){
			
			for ( int j = i-1; j >= 0; j-- )
				undoProc (&edPtr->imageFrames[j], IMG_TAB, lp1, lp2);
		
			break;
		}
	}
	return error;
}


