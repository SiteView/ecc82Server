/****************************************************************************

    FILE:       lapicinc.h

    PURPOSE:    C runtime includes for lapiplat.h

    DESCRIPTION:
    This file must be included in all Notes API Toolkit samples that also
    include lapiplat.h.  It should be included after the C runtime library
    includes and before the Notes API includes.  This is because the Notes
    API headers redefine some of the C runtime symbols.

****************************************************************************/

#ifndef LAPICINC_DEFS
#define LAPICINC_DEFS

#ifdef __cplusplus
extern "C" {
#endif


#if defined(MAC)
/****************************************************************************

    Macintosh DEFINITIONS

****************************************************************************/

#include <Types.h>
#include <Quickdraw.h>
#define thePort qd.thePort
#include <Dialogs.h>
#include <Menus.h>
#include <Windows.h>
#include <Fonts.h>
#include <MacMemory.h>
#include <SegLoad.h>    /* ExitToShell() */
#include <StdLib.h>     /* malloc, free  */

#ifdef __MWERKS__
#include <stdio.h>
#endif

#elif defined(OS390)
/****************************************************************************

    Expose the OS/390 to/from IEEE floating-point conversion routines
    that are contained in the libnotes library.  

****************************************************************************/
/* Convert OS/390 native floats (single and double) to IEEE */
void ConvertFloatToIEEE(void *source, void *destination);
void ConvertDoubleToIEEE(void *source, void *destination);
/* Convert IEEE floats (single and double) to OS/390 native */
void ConvertIEEEToFloat(void *source, void *destination);
void ConvertIEEEToDouble(void *source, void *destination);

#else
/****************************************************************************
*
*  DEFAULT DEFINITIONS
*
****************************************************************************/

#endif

#ifdef __cplusplus
}
#endif

#endif
