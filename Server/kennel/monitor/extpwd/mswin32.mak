# 
#  makefile for Notes API sample program extpwd.dll.
#               Windows 32-bit version
#
# This makefile is designed for use under Windows NT and Windows 95
#               using Microsoft 32-bit Visual C++ compiler and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows NT make definitions
!include <ntwin32.mak>

#
# DEC Alpha doesn't want the structures to be packed so we use the CPU
# type to conditionally add it as part of compile string
#
!IF "$(CPU)" == "ALPHA"
cpuflags = 
outfilename = aextpwd
defname = mswin32a
!ELSE
cpuflags = -Zp1
outfilename = nextpwd
defname = mswin32
!ENDIF

.c.obj :
    $(cc) $(cdebug) $(cflags) $(cpuflags) -Ow -DNT $(cvars) $*.c

all : $(outfilename).dll

$(outfilename).dll : extman.obj extpwd.obj
    $(link) $(linkdebug) -base:0x1c000000 \
	-dll -def:$(defname).def \
	 -entry:_DllMainCRTStartup$(DLLENTRY)\
	 -out:$(outfilename).dll \
	extman.obj extpwd.obj \
	$(guilibs) notes.lib

extpwd.obj : extpwd.h extpwd.c

extman.obj : extpwd.h extman.c


