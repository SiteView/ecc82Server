#ifdef __cplusplus
extern "C" {
#endif


/*	Notes Editor-to-bitmap Definitions */

#ifndef EDITFAX_DEFS
#define EDITFAX_DEFS

#ifndef NSF_DATA_DEFS
#include "nsfdata.h"						/* NOTEID, DBHANDLE, NOTEHANDLE */
#endif

/*	Data structures used when printing to a bitmap */

typedef BOOL (FAR PASCAL * EPBPROC)(void *EPBContext, WORD PageNumber);

#ifdef W32
#ifndef HPAINT
#define HPAINT 		HDC
#endif
#endif

typedef struct
	{
	EPBPROC Proc;			/* Callback procedure */
	HPAINT hPaint;			/* hPS that the bitmap can be selected into */
	HBITMAP hBitmap;		/* Bitmap to paint into */
	RECTSIZE BitmapSize;	/* Size of the bitmap */
	RECT PaintRect;			/* Rectangle within the bitmap into which to paint */
	WORD cxPaperTwips;		/* Width of the paper, in TWIPS */
	WORD cyPaperTwips;		/* Height of the paper, in TWIPS */
	} EPBCONTEXT;

STATUS far PASCAL EditorPrintNoteToBitmap (
											DBHANDLE hDB,
											NOTEID NoteID,
											void *EPBContext);

STATUS far PASCAL EditorAppendBitmapToNote (
											HBITMAP hBitmap,
											WORD cxPaperTwips, WORD cyPaperTwips,
											WORD DisplayScalingPercent,
											NOTEHANDLE hNote,
											WORD ItemFlags,
											char *ItemName,
											WORD ItemNameLength,
											void *ParagraphAttributes);


#endif



#ifdef __cplusplus
}
#endif

