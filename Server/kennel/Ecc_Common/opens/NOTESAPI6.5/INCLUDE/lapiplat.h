/****************************************************************************

    FILE:       lapiplat.h

    PURPOSE:    platform-specific macro definitions for toolkit samples

    DESCRIPTION:
    Notes API Toolkit version definitions and macro definitions for Notes API
    Toolkit samples to allow platform-independent sample code.  The following
    macros must be defined for each platform:

        LAPI_MAIN
        LAPI_INIT(status)
        LAPI_INIT_ERROR
        LAPI_RETURN(status)
        LAPI_DUMP_ERROR(error)

****************************************************************************/

#ifndef LAPIPLAT_DEFS
#define LAPIPLAT_DEFS

#ifdef __cplusplus
extern "C" {
#endif

#define NOTESAPI_VERSION   "Notes/Domino 6.5.1"
#define NOTESAPI_MAJOR_VERSION 6
#define NOTESAPI_MINOR_VERSION 5
#define NOTESAPI_MINOR_POINT_VERSION 1

#if defined(EBCDIC_RTL)
#include "ascii_a.h"
#endif

#ifndef GLOBAL_DEFS
#include <global.h>
#endif

#ifndef OSMISC_DEFS
#include <osmisc.h>
#endif

#if defined(MAC) || defined (UNIX)
#define LPSTR char *
#endif

/****************************************************************************
*
*       LAPI_MAIN
*
****************************************************************************/

#if defined(MAC)
/*
 *  Macintosh
 */
#define LAPI_MAIN  int main(int argc, char *argv[])

#else
/*
 *  DEFAULT
 */
#define LAPI_MAIN  STATUS LNPUBLIC NotesMain(int argc, char far *argv[])

#endif

/****************************************************************************
*
*       LAPI_INIT
*
****************************************************************************/

#if defined(MAC)
/*
 *  Macintosh
 *
 *  Initialize Notes
*/
#define LAPI_INIT(x)  {if (NotesInitExtended == NULL)\
                         x = (STATUS)TRUE;\
                       else\
                         x = NotesInitExtended( argc, argv );}
#else
/*
 *  DEFAULT
 */
#define LAPI_INIT(x)

#endif

/****************************************************************************
*
*       LAPI_INIT_ERROR
*
****************************************************************************/

#if defined(MAC)
/*
 *  Macintosh
 *
 *  NotesInitExtended failed 
 *  1) Print error message 
 */
 
#define LAPI_INIT_ERROR {printf("\nUnable to initialize Notes\n");\
                         return(1);}

#else
/*
 *  DEFAULT
 */
#define LAPI_INIT_ERROR

#endif

/****************************************************************************
*
*       LAPI_RETURN
*
****************************************************************************/

#if defined(MAC)
/*
 *  Macintosh
 *
 *  1) Dump error text to stdout.
 *  2) Call NotesTerm().
 */

#define LAPI_RETURN(x)  {if (x) LAPI_DUMP_ERROR(x);\
                         NotesTerm();\
                         return(x);}
#else
/*
 *  DEFAULT
 */
#define LAPI_RETURN(x)   return(x)

#endif

/****************************************************************************
*
*       LAPI_DUMP_ERROR
*
*       Use in LAPI_RETURN or as a debugging tool for dumping the associated
*       error to stdout.
*
****************************************************************************/

#if defined (MAC)
/*
 *  Macintosh
 */
#define LAPI_DUMP_ERROR(x) {char e[128];OSLoadString(0L,ERR(x),e,sizeof(e));\
                            printf("\n%s\n",e);}

#else
/*
 *  DEFAULT
 */
#define LAPI_DUMP_ERROR(x)

#endif

#ifdef __cplusplus
}
#endif

#endif
