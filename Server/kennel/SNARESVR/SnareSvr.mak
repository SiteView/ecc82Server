# Microsoft Developer Studio Generated NMAKE File, Based on SnareSvr.dsp
!IF "$(CFG)" == ""
CFG=SnareSvr - Win32 Debug
!MESSAGE No configuration specified. Defaulting to SnareSvr - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "SnareSvr - Win32 Release" && "$(CFG)" != "SnareSvr - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SnareSvr.mak" CFG="SnareSvr - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SnareSvr - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SnareSvr - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "SnareSvr - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\SnareSvr.exe"


CLEAN :
	-@erase "$(INTDIR)\NTServApp.obj"
	-@erase "$(INTDIR)\NTServApp.res"
	-@erase "$(INTDIR)\NTService.obj"
	-@erase "$(INTDIR)\SnareSvr.obj"
	-@erase "$(INTDIR)\support.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WebPages.obj"
	-@erase "$(INTDIR)\webserver.obj"
	-@erase "$(OUTDIR)\SnareSvr.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_CONSOLE" /Fp"$(INTDIR)\SnareSvr.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0xc09 /fo"$(INTDIR)\NTServApp.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SnareSvr.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib netapi32.lib advapi32.lib delayimp.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\SnareSvr.pdb" /machine:I386 /out:"$(OUTDIR)\SnareSvr.exe" /DELAYLOAD:advapi32.dll 
LINK32_OBJS= \
	"$(INTDIR)\NTServApp.obj" \
	"$(INTDIR)\NTService.obj" \
	"$(INTDIR)\SnareSvr.obj" \
	"$(INTDIR)\support.obj" \
	"$(INTDIR)\WebPages.obj" \
	"$(INTDIR)\webserver.obj" \
	"$(INTDIR)\NTServApp.res"

"$(OUTDIR)\SnareSvr.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SnareSvr - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\SnareSvr.exe"


CLEAN :
	-@erase "$(INTDIR)\NTServApp.obj"
	-@erase "$(INTDIR)\NTServApp.res"
	-@erase "$(INTDIR)\NTService.obj"
	-@erase "$(INTDIR)\SnareSvr.obj"
	-@erase "$(INTDIR)\support.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WebPages.obj"
	-@erase "$(INTDIR)\webserver.obj"
	-@erase "$(OUTDIR)\SnareSvr.exe"
	-@erase "$(OUTDIR)\SnareSvr.ilk"
	-@erase "$(OUTDIR)\SnareSvr.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_CONSOLE" /Fp"$(INTDIR)\SnareSvr.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0xc09 /fo"$(INTDIR)\NTServApp.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SnareSvr.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib netapi32.lib advapi32.lib delayimp.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\SnareSvr.pdb" /debug /machine:I386 /out:"$(OUTDIR)\SnareSvr.exe" /pdbtype:sept /DELAYLOAD:advapi32.dll 
LINK32_OBJS= \
	"$(INTDIR)\NTServApp.obj" \
	"$(INTDIR)\NTService.obj" \
	"$(INTDIR)\SnareSvr.obj" \
	"$(INTDIR)\support.obj" \
	"$(INTDIR)\WebPages.obj" \
	"$(INTDIR)\webserver.obj" \
	"$(INTDIR)\NTServApp.res"

"$(OUTDIR)\SnareSvr.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("SnareSvr.dep")
!INCLUDE "SnareSvr.dep"
!ELSE 
!MESSAGE Warning: cannot find "SnareSvr.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "SnareSvr - Win32 Release" || "$(CFG)" == "SnareSvr - Win32 Debug"
SOURCE=.\NTServApp.cpp

"$(INTDIR)\NTServApp.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\NTService.cpp

"$(INTDIR)\NTService.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\NTServMsg.rc
SOURCE=.\SnareSvr.cpp

"$(INTDIR)\SnareSvr.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\support.cpp

"$(INTDIR)\support.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\WebPages.cpp

"$(INTDIR)\WebPages.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\webserver.cpp

"$(INTDIR)\webserver.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\NTServApp.rc

"$(INTDIR)\NTServApp.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

