# Microsoft Developer Studio Project File - Name="It2K" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=It2K - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "It2K.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "It2K.mak" CFG="It2K - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "It2K - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "It2K - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "It2K - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "It2K___W"
# PROP BASE Intermediate_Dir "It2K___W"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Lib"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "C:\testobj" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "It2K - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "It2K - Win32 Release"
# Name "It2K - Win32 Debug"
# Begin Source File

SOURCE=.\Include\jctypes.h
# End Source File
# Begin Source File

SOURCE=.\Source\PiDircos.c
# End Source File
# Begin Source File

SOURCE=.\Include\PiDircos.h
# End Source File
# Begin Source File

SOURCE=.\Source\PiEuler.c
# End Source File
# Begin Source File

SOURCE=.\Include\PiEuler.h
# End Source File
# Begin Source File

SOURCE=.\Source\PiIT2K.c
# End Source File
# Begin Source File

SOURCE=.\Include\PiIT2k.h
# End Source File
# Begin Source File

SOURCE=.\Source\PiMath.c
# End Source File
# Begin Source File

SOURCE=.\Include\PiMath.h
# End Source File
# Begin Source File

SOURCE=.\Source\PiOri.c
# End Source File
# Begin Source File

SOURCE=.\Include\PiOri.h
# End Source File
# Begin Source File

SOURCE=.\Source\PiPos.c
# End Source File
# Begin Source File

SOURCE=.\Include\PiPos.h
# End Source File
# Begin Source File

SOURCE=.\Source\PiQtrn.c
# End Source File
# Begin Source File

SOURCE=.\Include\PiQtrn.h
# End Source File
# End Target
# End Project
