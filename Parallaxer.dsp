# Microsoft Developer Studio Project File - Name="Parallaxer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Parallaxer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Parallaxer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Parallaxer.mak" CFG="Parallaxer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Parallaxer - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Parallaxer - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Parallaxer - Win32 Run_Only" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Parallaxer - Win32 Release_Small" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Parallaxer - Win32 Run_Only_Small" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Parallaxer - Win32 Vitalize" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Parallaxer - Win32 Vitalize_Small" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Parallaxer - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "Parallaxer_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\Inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "STRICT" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /fo".\Release\Ext.res" /i "..\..\Inc" /d "NDEBUG" /d "EDITOR"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib ..\..\lib\mmfs2.lib /nologo /dll /machine:I386 /def:".\Ext.def" /out:".\Release/Parallaxer.mfx"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy "ToInstall\Files\Data\Runtime\*.*" "%ProgramFiles(x86)%\Multimedia Fusion Developer 2\Data\Runtime\"	copy "ToInstall\Files\Extensions\*.*" "%ProgramFiles(x86)%\Multimedia Fusion Developer 2\Extensions\"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Parallaxer - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "V2Parallaxer32_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\Inc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "STRICT" /Fr /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /fo".\Debug\Ext.res" /i "..\..\Inc" /d "_DEBUG" /d "EDITOR"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib ..\..\lib\mmfs2.lib /nologo /dll /debug /machine:I386 /def:".\Ext.def" /out:".\Debug/Parallaxer.mfx"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy "Debug\Parallaxer.mfx" "%ProgramFiles(x86)%\Multimedia Fusion Developer 2\Extensions\Parallaxer.mfx"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Parallaxer - Win32 Run_Only"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "V2Parallaxer32___Win32_Run_Only"
# PROP BASE Intermediate_Dir "V2Parallaxer32___Win32_Run_Only"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Run_Only"
# PROP Intermediate_Dir ".\Run_Only"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "STRICT" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\Inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "STRICT" /D "RUN_ONLY" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /D "RUN_ONLY" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /fo".\Run_Only\Ext.res" /i "..\..\Inc" /d "NDEBUG" /d "RUN_ONLY"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\lib\cncs32.lib /nologo /dll /machine:I386
# ADD LINK32 gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib ..\..\lib\mmfs2.lib /nologo /dll /machine:I386 /def:".\Ext.def" /out:".\Run_Only/Parallaxer.mfx"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy "ToInstall\Files\Data\Runtime\*.*" "%ProgramFiles(x86)%\Multimedia Fusion Developer 2\Data\Runtime\"	copy "ToInstall\Files\Extensions\*.*" "%ProgramFiles(x86)%\Multimedia Fusion Developer 2\Extensions\"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Parallaxer - Win32 Release_Small"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "V2Parallaxer32___Win32_Release_Small"
# PROP BASE Intermediate_Dir "V2Parallaxer32___Win32_Release_Small"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release_Small"
# PROP Intermediate_Dir ".\Release_Small"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "STRICT" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /I "..\..\Inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "STRICT" /D "COMPILE_SMALL" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /fo".\Release_Small\Ext.res" /i "..\..\Inc" /d "NDEBUG" /d "EDITOR"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\lib\cncs32.lib /nologo /dll /machine:I386 /out:".\Release/Parallaxer.mfx"
# ADD LINK32 gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib ..\..\lib\mmfs2.lib /nologo /dll /machine:I386 /def:".\Ext.def" /out:".\Release_Small/Parallaxer.mfx"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy "Run_Only_Small\Parallaxer.mfx" "%ProgramFiles(x86)%\Multimedia Fusion Developer 2\Data\Runtime\Parallaxer.mfx"	copy "Release_Small\Parallaxer.mfx" "%ProgramFiles(x86)%\Multimedia Fusion Developer 2\Extensions\Parallaxer.mfx"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Parallaxer - Win32 Run_Only_Small"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "V2Parallaxer32___Win32_Run_Only_Small"
# PROP BASE Intermediate_Dir "V2Parallaxer32___Win32_Run_Only_Small"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Run_Only_Small"
# PROP Intermediate_Dir ".\Run_Only_Small"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "STRICT" /D "RUN_ONLY" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /I "..\..\Inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "STRICT" /D "RUN_ONLY" /D "COMPILE_SMALL" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /D "RUN_ONLY" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /D "RUN_ONLY" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "RUN_ONLY"
# ADD RSC /l 0x409 /fo".\Run_Only_Small\Ext.res" /i "..\..\Inc" /d "NDEBUG" /d "RUN_ONLY"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\lib\cncs32.lib /nologo /dll /machine:I386 /out:".\Run_Only/Parallaxer.mfx"
# ADD LINK32 gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib ..\..\lib\mmfs2.lib /nologo /dll /machine:I386 /def:".\Ext.def" /out:".\Run_Only_Small/Parallaxer.mfx"
# SUBTRACT LINK32 /pdb:none /nodefaultlib
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy "Run_Only_Small\Parallaxer.mfx" "%ProgramFiles(x86)%\Multimedia Fusion Developer 2\Data\Runtime\Parallaxer.mfx"	copy "Release_Small\Parallaxer.mfx" "%ProgramFiles(x86)%\Multimedia Fusion Developer 2\Extensions\Parallaxer.mfx"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Parallaxer - Win32 Vitalize"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "V2Parallaxer32___Win32_Vitalize"
# PROP BASE Intermediate_Dir "V2Parallaxer32___Win32_Vitalize"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Vitalize"
# PROP Intermediate_Dir ".\Vitalize"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "STRICT" /D "RUN_ONLY" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\Inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "STRICT" /D "RUN_ONLY" /D "VITALIZE" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /D "RUN_ONLY" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /D "RUN_ONLY" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "RUN_ONLY"
# ADD RSC /l 0x409 /fo".\Vitalize\Ext.res" /i "..\..\Inc" /d "NDEBUG" /d "RUN_ONLY" /d "VITALIZE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\lib\cncs32.lib /nologo /dll /machine:I386 /out:".\Run_Only/Parallaxer.mfx"
# ADD LINK32 gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib ..\..\lib\mmfs2.lib /nologo /dll /machine:I386 /def:".\Ext.def" /out:".\Vitalize\Parallaxer.mox"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy "ToInstall\Files\Data\Runtime\*.*" "%ProgramFiles(x86)%\Multimedia Fusion Developer 2\Data\Runtime\"	copy "ToInstall\Files\Extensions\*.*" "%ProgramFiles(x86)%\Multimedia Fusion Developer 2\Extensions\"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Parallaxer - Win32 Vitalize_Small"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "V2Parallaxer32___Win32_Vitalize_Small"
# PROP BASE Intermediate_Dir "V2Parallaxer32___Win32_Vitalize_Small"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Vitalize_Small"
# PROP Intermediate_Dir ".\Vitalize_Small"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "STRICT" /D "RUN_ONLY" /D "COMPILE_SMALL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /O1 /I "..\..\Inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "STRICT" /D "RUN_ONLY" /D "COMPILE_SMALL" /D "VITALIZE" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /D "RUN_ONLY" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /D "RUN_ONLY" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "RUN_ONLY"
# ADD RSC /l 0x409 /fo".\Vitalize_Small\Ext.res" /i "..\..\Inc" /d "NDEBUG" /d "RUN_ONLY" /d "VITALIZE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib msvcrt.lib ..\lib\cncs32.lib /nologo /dll /machine:I386 /nodefaultlib /out:".\Run_Only_Small/Parallaxer.mfx"
# ADD LINK32 gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib ..\..\lib\mmfs2.lib /nologo /dll /machine:I386 /def:".\Ext.def" /out:".\Vitalize_Small/Parallaxer.mox"
# SUBTRACT LINK32 /pdb:none /nodefaultlib
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy "ToInstall\Files\Data\Runtime\*.*" "%ProgramFiles(x86)%\Multimedia Fusion Developer 2\Data\Runtime\"	copy "ToInstall\Files\Extensions\*.*" "%ProgramFiles(x86)%\Multimedia Fusion Developer 2\Extensions\"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Parallaxer - Win32 Release"
# Name "Parallaxer - Win32 Debug"
# Name "Parallaxer - Win32 Run_Only"
# Name "Parallaxer - Win32 Release_Small"
# Name "Parallaxer - Win32 Run_Only_Small"
# Name "Parallaxer - Win32 Vitalize"
# Name "Parallaxer - Win32 Vitalize_Small"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Edittime.cpp
# End Source File
# Begin Source File

SOURCE=.\Ext.def
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Ext.rc
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\General.cpp
# End Source File
# Begin Source File

SOURCE=.\Main.cpp
# End Source File
# Begin Source File

SOURCE=.\ParallaxerFunctions.cpp
# End Source File
# Begin Source File

SOURCE=.\Runtime.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Common.h
# End Source File
# Begin Source File

SOURCE=.\Main.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\default.bmp

!IF  "$(CFG)" == "Parallaxer - Win32 Release"

!ELSEIF  "$(CFG)" == "Parallaxer - Win32 Debug"

!ELSEIF  "$(CFG)" == "Parallaxer - Win32 Run_Only"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Parallaxer - Win32 Release_Small"

!ELSEIF  "$(CFG)" == "Parallaxer - Win32 Run_Only_Small"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Parallaxer - Win32 Vitalize"

!ELSEIF  "$(CFG)" == "Parallaxer - Win32 Vitalize_Small"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\EXTICON.BMP
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Extimg.bmp
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Information"

# PROP Default_Filter "txt"
# Begin Source File

SOURCE=..\Help\CoxSdk.chm
# End Source File
# End Group
# Begin Group "MMF Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE="C:\Program Files\Microsoft Visual Studio\Vc98\Include\Basetsd.h"
# End Source File
# Begin Source File

SOURCE=..\..\Inc\Ccx.h
# End Source File
# Begin Source File

SOURCE=..\..\Inc\Ccxhdr.h
# End Source File
# Begin Source File

SOURCE=..\..\Inc\CfcError.h
# End Source File
# Begin Source File

SOURCE=..\..\Inc\CfcFile.h
# End Source File
# Begin Source File

SOURCE=..\..\Inc\Cncf.h
# End Source File
# Begin Source File

SOURCE=..\..\Inc\cncr.h
# End Source File
# Begin Source File

SOURCE=..\..\Inc\Cncy.h
# End Source File
# Begin Source File

SOURCE=..\..\Inc\Cnpdll.h
# End Source File
# Begin Source File

SOURCE=..\..\Inc\colors.h
# End Source File
# Begin Source File

SOURCE=..\..\Inc\EVTCCX.H
# End Source File
# Begin Source File

SOURCE=..\..\Inc\Fill.h
# End Source File
# Begin Source File

SOURCE=..\..\Inc\FilterMgr.h
# End Source File
# Begin Source File

SOURCE=..\..\Inc\ImageFlt.h
# End Source File
# Begin Source File

SOURCE=..\..\Inc\ImgFlt.h
# End Source File
# Begin Source File

SOURCE=..\..\Inc\Palet.h
# End Source File
# Begin Source File

SOURCE=..\..\Inc\PictEdDefs.h
# End Source File
# Begin Source File

SOURCE=..\..\Inc\Props.h
# End Source File
# Begin Source File

SOURCE=..\..\Inc\PTYPE.H
# End Source File
# Begin Source File

SOURCE=..\..\Inc\SoundFilter.h
# End Source File
# Begin Source File

SOURCE=..\..\Inc\SoundFilterMgr.h
# End Source File
# Begin Source File

SOURCE=..\..\Inc\Surface.h
# End Source File
# Begin Source File

SOURCE=..\..\Inc\TigsExt.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Inc\WinMacro.h
# End Source File
# End Group
# End Target
# End Project
