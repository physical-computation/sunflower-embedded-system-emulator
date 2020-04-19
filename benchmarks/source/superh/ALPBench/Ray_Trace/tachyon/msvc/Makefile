#
# (out of date) Microsoft Developer Studio Makefiles for Win32 builds
#

newdefault:
	@echo "This makefile is here for historic reasons, and does not work."
	@echo "Use the MSVC workspace provided in the tachyon directory instead"

default:
	@echo You must choose one of the following configurations:
	@echo ----------------------------------------------------
	@echo          win32 - MSVC 5, no threads                         
	@echo      win32-ogl - MSVC 5, OpenGL Display, no threads         
	@echo      win32-mpi - MSVC 5, MSTI MPI/Pro, no threads           
	@echo  win32-mpi-ogl - MSVC 5, MSTI MPI/Pro, OpenGL, no threads   
	@echo      win32-via - MSVC 5, MSTI MPI/Pro, VIA, no threads      
	@echo  win32-via-ogl - MSVC 5, MSTI MPI/Pro, VIA, OpenGL, no threads
	@echo          clean - Deletes all object files                   
	@echo ----------------------------------------------------
	@echo on                                                  

win32:
	cd libray
	nmake /f "libray.mak" CFG="libray - Win32 Release"
	nmake /f "libray.mak" CFG="libray - Win32 Debug"
	cd ..
	cd libmgf
	nmake /f "libmgf.mak" CFG="libmgf - Win32 Release"
	nmake /f "libmgf.mak" CFG="libmgf - Win32 Debug"
	cd ..
	cd tacheon
	nmake /f "tacheon.mak" CFG="tacheon - Win32 Release"
	nmake /f "tacheon.mak" CFG="tacheon - Win32 Debug"
	cd ..	
	copy tacheon\Release\tacheon.exe bin\tacheon.exe

win32-debug:
	cd libray
	nmake /f "libray.mak" CFG="libray - Win32 Debug"
	cd ..
	cd libmgf
	nmake /f "libmgf.mak" CFG="libmgf - Win32 Debug"
	cd ..
	cd tacheon
	nmake /f "tacheon.mak" CFG="tacheon - Win32 Debug"
	cd ..	

win32-ogl:
	cd libray
	nmake /f "libray.mak" CFG="libray - Win32 Release"
	cd ..
	cd libmgf
	nmake /f "libmgf.mak" CFG="libmgf - Win32 Release"
	cd ..
	cd tacheon
	nmake /f "tacheon.mak" CFG="tacheon - Win32 OGL Release"
	cd ..	
	copy tacheon\OGLRelease\tacheon.exe bin\ogltacheon.exe

win32-mpi:	
	cd libray
	nmake /f "libray.mak" CFG="libray - Win32 MPI Release"
	cd ..
	cd libmgf
	nmake /f "libmgf.mak" CFG="libmgf - Win32 Release"
	cd ..
	cd tacheon
	nmake /f "tacheon.mak" CFG="tacheon - Win32 MPI Release"
	cd ..	
	copy tacheon\MPIRelease\tacheon.exe bin\mpitacheon.exe

win32-mpi-debug:	
	cd libray
	nmake /f "libray.mak" CFG="libray - Win32 MPI Debug"
	cd ..
	cd libmgf
	nmake /f "libmgf.mak" CFG="libmgf - Win32 Debug"
	cd ..
	cd tacheon
	nmake /f "tacheon.mak" CFG="tacheon - Win32 MPI Debug"
	cd ..	

win32-via:	
	cd libray
	nmake /f "libray.mak" CFG="libray - Win32 VIA Release"
	cd ..
	cd libmgf
	nmake /f "libmgf.mak" CFG="libmgf - Win32 Release"
	cd ..
	cd tacheon
	nmake /f "tacheon.mak" CFG="tacheon - Win32 VIA Release"
	cd ..	
	copy tacheon\VIARelease\tacheon.exe bin\viatacheon.exe


win32-mpi-ogl:	
	cd libray
	nmake /f "libray.mak" CFG="libray - Win32 MPI Release"
	cd ..
	cd libmgf
	nmake /f "libmgf.mak" CFG="libmgf - Win32 Release"
	cd ..
	cd tacheon
	nmake /f "tacheon.mak" CFG="tacheon - Win32 MPI OGL Release"
	cd ..	
	copy tacheon\MPIOGLRelease\tacheon.exe bin\mpiogltacheon.exe

win32-via-ogl:	
	cd libray
	nmake /f "libray.mak" CFG="libray - Win32 VIA Release"
	cd ..
	cd libmgf
	nmake /f "libmgf.mak" CFG="libmgf - Win32 Release"
	cd ..
	cd tacheon
	nmake /f "tacheon.mak" CFG="tacheon - Win32 VIA OGL Release"
	cd ..	
	copy tacheon\VIAOGLRelease\tacheon.exe bin\viaogltacheon.exe



clean:
	del /Q /F bin\*
	del /Q /F libmgf\Release\*
	del /Q /F libmgf\Debug\*
	del /Q /F libray\Release\*
	del /Q /F libray\Debug\*
	del /Q /F libray\MPIRelease\*
	del /Q /F libray\MPIDebug\*
	del /Q /F tacheon\Release\*
	del /Q /F tacheon\Debug\*
	del /Q /F tacheon\MPIRelease\*
	del /Q /F tacheon\MPIDebug\*
	del /Q /F tacheon\OGLRelease\*
	del /Q /F tacheon\MPIOGLRelease\*
	del /Q /F tacheon\VIARelease\*
	del /Q /F tacheon\VIAOGLRelease\*


