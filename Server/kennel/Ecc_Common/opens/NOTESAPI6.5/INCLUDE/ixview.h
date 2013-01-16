#ifdef __cplusplus
extern "C" {
#endif

#ifndef IX_DEFS
#define IX_DEFS

#ifndef NSF_DATA_DEFS
#include "nsfdata.h"		/* for DBHANDLE */
#endif

#ifndef NIF_DEFS
#include "nif.h"			/* for HCOLLECTION */
#endif

#ifndef NAMES_DEFS
#include "names.h"			/* for DESIGNNAMEMAX */
#endif

#ifndef MISC_DEFS
#include "misc.h"			/* for NFMT, TFMT */
#endif


/* size of array for strings which do load string of ERR_ISTRUCT_UNTITLED_STRING */
#define UNTITLED_SIZE	30

/* size of array for full form names (that include synonyms) */
#define FULLNAME_SIZE	DESIGN_ALL_NAMES_MAX


/* This file contains the data structures for the info passed from */
/* the view setup module to import/export exe's */

typedef struct
	{
	HANDLE hFormNames;					/* handle to array of form names */
	WORD   FormCount;					/* Number of form names */
	WORD   Reserved1;
	DBHANDLE hNoteFile;					/* Notes file handle */
	DBHANDLE hViewFile;					/* View file handle */
	WORD   wFlags;						/* Flags, reserved for internal use */
	NOTEID ViewNoteID;					/* View note ID */
	UNID   ViewNoteUNID;				/* View universal note ID */
	HCOLLECTION hViewCollection;		/* View collection handle */
	WORD   Reserved3;
	HANDLE hUnreadList;					/* Unread note ID list handle */
	HANDLE hCollapsedList;				/* Collapsed note ID list handle */
	HANDLE hSelectedList;				/* Selected note ID list handle */
	WORD   Reserved4;
	FLAG ViewIsPrivate:1;				/* View is private */
	NOTEID CaretNoteID; 				/* Caret note ID */
	WORD ColumnCount;					/* Number of columns in the view */
	HANDLE hColDesc;					/* Column descriptor array (IXCOLDESC) */
	WORD LineArrayEntrySize;			/* Size (in bytes) of a line array entry */
	WORD TextWidth;						/* Width of text */
	WORD NumTextWidth;					/* Width of numeric text characters */
	WORD Categories;					/* Number of category columns in view */
	HANDLE hFormNoteIDs;				/* handle to array of form note IDs */
	HANDLE hFormNamesFull;				/* handle to array of form names that include synonyms */
	/* hFormNamesAll points to a list of form names with their synonyms.  A
	   Synonym is created any time a form name is changed.  All the notes
	   created with the previous incarnation of that form have the synonym
	   in their FORM field.  The XSTF export has to know which form the
	   note came from, in order to correctly map the fields to the
	   Agenda data type chosen by the user.
	*/
	} VIEWIXDATA;


/* Structure of the data block of form names */
typedef struct
	{
	/*	The following array is intended to be the maximum name length
		for a form.  This used to use DESIGN_NAME_MAX (from names.h)
		but the  value of DESIGN_LEVEL_MAX (which DESIGN_NAME_MAX is derived
		from) was doubled from V3.0 to V3.0J to allow for longer Japanese
		design names.  The hardcoded value below is based on the V3.0 value
		and will allow API programs to be backward and forward compatible.
	*/
	char Name[66];
	} FORMNAME_ARRAY;

/* Structure of the data block of complete (synonyms included) form names */
typedef struct
	{
	char FullName[FULLNAME_SIZE];
	} FULLFORMNAME;



/* Structure of an entry in the column descriptor block set up by View */
/* for imports/exports (one entry per view column). */

typedef struct
	{
	FLAG Sort:1;						/* Add column to sort */
	FLAG SortCategorize:1;				/* Make column a category */
	FLAG SortPermute:1;					/* Make column permuted if multi-valued. */
	FLAG SortDescending:1;				/* Sort in descending order (ascending if FALSE) */
	FLAG Hidden:1;						/* Normally hide column */
	FLAG Response:1;					/* Response column */
	FLAG Selected:1;					/* Column is selected */
	FLAG HideDetail:1;					/* Do not show detail on subtotalled columns */
	FLAG Icon:1;						/* Icon column. */
	FLAG spare1:8;						/* spare flags */

	WORD ColumnTitleLength;				/* Column title string length */
	HANDLE hColumnTitle;				/* Column title string handle */
	WORD ItemNameLength;				/* Column item name string length */
	HANDLE hItemName;					/* NSF Item name for column */
	WORD FormulaLength;					/* Formula buffer length */	
	FORMULAHANDLE hFormula;				/* Formula buffer handle */
	WORD ConstantValueLength;			/* Constant value length */
	HANDLE hConstantValue;				/* Constant value handle */
	WORD ColumnWidth;					/* Column width in characters */
	WORD TextMax;						/* Maximum text string size */
	WORD Alignment;						/* Column alignment */
	NFMT NumberFormat;					/* Number format specification */
	TFMT TimeFormat;					/* Time format specification */
	WORD FormatDataType;				/* Last specified format data type */
	WORD ListSep;						/* List Separator */
	WORD SubtotalCode;					/* Subtotalling code */
	} IXCOLDESC;


/* Formatted column text descriptor. This is a data structure used */
/* to store text information extracted from the NIF Summary information */
/* for exports of view data into text. */
typedef struct {
	WORD Width;							/* Column width in 1/8 characters */
	WORD Position;						/* Column position in characters */
	FLAG Alignment:2;					/* Alignment (left, right, etc. ) */
	FLAG LastColumn:1;					/* Last column flag */
	FLAG Category:1;					/* Column is a category flag */
	WORD TextMax;						/* Text space allocated for this column */
	WORD TextLength;					/* Text string length */
/*	char Text[TextMax];  */				/* Text string itself */
	} IXCOLUMNTEXTDESC;		



/* view column alignments */
#define IXVIEW_COL_ALIGN_LEFT 	0
#define IXVIEW_COL_ALIGN_RIGHT	1
#define IXVIEW_COL_ALIGN_CENTER 2


#endif
/* [end of file] */

#ifdef __cplusplus
}
#endif

