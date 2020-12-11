# Microsoft Developer Studio Project File - Name="libtachyon" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libtachyon - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libtachyon.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libtachyon.mak" CFG="libtachyon - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libtachyon - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libtachyon - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "libtachyon - Win32 Release"
# Name "libtachyon - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\src\api.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\apigeom.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\apitrigeom.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\box.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\camera.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\coordsys.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\cylinder.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\extvol.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\global.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\grid.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\hash.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\imageio.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\imap.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\intersect.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\jpeg.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\light.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\parallel.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\plane.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\pngfile.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\ppm.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\quadric.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\render.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\ring.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\sgirgb.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\shade.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\sphere.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\texture.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\tgafile.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\threads.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\trace.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\triangle.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\ui.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\util.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\vector.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\vol.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\src\winbmp.c

!IF  "$(CFG)" == "libtachyon - Win32 Release"

# ADD CPP /GB /MD /Ob2 /I "..\..\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "libtachyon - Win32 Debug"

# ADD CPP /MDd /I "..\..\src"
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\src\apitrigeom.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\box.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\camera.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\coordsys.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\cylinder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\extvol.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\global.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\grid.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\image.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\imageio.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\imap.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\intersect.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\jpeg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\light.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\machine.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\macros.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\objbound.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\parallel.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\plane.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\ppm.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\quadric.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\render.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\ring.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\sgirgb.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\shade.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\sphere.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\tachyon.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\texture.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\tgafile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\threads.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\trace.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\triangle.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\types.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\ui.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\util.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\vector.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\video.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\vol.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\winbmp.h
# End Source File
# End Group
# End Target
# End Project
