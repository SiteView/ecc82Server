
/*	Navigator on-disk data definitions */

#ifndef VMODS_DEFS
#define VMODS_DEFS

#ifndef ODS_DEFS
#include "ods.h"
#endif
	
#ifndef NSF_DATA_DEFS
#include "nsfdata.h"			/* NOTELINK */
#endif

#ifndef EDIT_ODS_DEFS
#include "editods.h"
#endif
 
/* 
	The VIEWMAP_XXX_DEFAULTS subrecords keep the default style 
	attribute settings for various types of objects.  
	
	NOTE that the VIEWMAP_HIGHLIGHT_DEFAULTS struct must appear FIRST
	in each subrecord so it can be reliably dereferenced via a PVOID
	that points to one of the object-specific subrecords.
*/

typedef struct
	{
	WORD		bHighlightTouch;
	WORD		bHighlightCurrent;
	WORD		HLOutlineColor;
	WORD		HLOutlineWidth;
	WORD		HLOutlineStyle;
	WORD		HLFillColor;	
	}  VIEWMAP_HIGHLIGHT_DEFAULTS;

typedef struct
	{
	VIEWMAP_HIGHLIGHT_DEFAULTS	Highlight;
	WORD		LineColor;
	WORD		FillFGColor;
	WORD		FillBGColor;
	WORD		LineStyle;
	WORD		LineWidth;
	WORD		FillStyle;
	FONTID		FontID;
	} VIEWMAP_SHAPE_DEFAULTS;

typedef struct
	{
	VIEWMAP_HIGHLIGHT_DEFAULTS	Highlight;
	WORD		LineColor;
	WORD		FillFGColor;
	WORD		FillBGColor;
	WORD		LineStyle;
	WORD		LineWidth;
	WORD		FillStyle;
	} VIEWMAP_LINE_DEFAULTS;

typedef struct
	{
	VIEWMAP_HIGHLIGHT_DEFAULTS	Highlight;
	} VIEWMAP_REGION_DEFAULTS;

typedef struct
	{
	VIEWMAP_HIGHLIGHT_DEFAULTS	Highlight;
	WORD		LineColor;
	WORD		FillFGColor;
	WORD		FillBGColor;
	WORD		LineStyle;
	WORD		LineWidth;
	WORD		FillStyle;
	FONTID		FontID;
	} VIEWMAP_BUTTON_DEFAULTS;

typedef struct
	{
	VIEWMAP_HIGHLIGHT_DEFAULTS	Highlight;
	} VIEWMAP_BITMAP_DEFAULTS;

typedef struct
	{
	VIEWMAP_HIGHLIGHT_DEFAULTS	Highlight;
	WORD		LineColor;
	WORD		FillFGColor;
	WORD		FillBGColor;
	WORD		LineStyle;
	WORD		LineWidth;
	WORD		FillStyle;
	FONTID		FontID;
	} VIEWMAP_TEXTBOX_DEFAULTS;

/*	
	The VIEWMAP_STYLE_DEFAULTS record amalgamates all the above subrecords:
*/

typedef struct
	{
 	VIEWMAP_SHAPE_DEFAULTS	Shapes;
 	VIEWMAP_LINE_DEFAULTS	Lines;
 	VIEWMAP_REGION_DEFAULTS	Regions;
 	VIEWMAP_BUTTON_DEFAULTS	Buttons;
 	VIEWMAP_BITMAP_DEFAULTS	Bitmaps;
	VIEWMAP_TEXTBOX_DEFAULTS TextBoxes;
	} VIEWMAP_STYLE_DEFAULTS;

/*
	VIEWMAP_DATASET_RECORD
	(followed by InitialView name, if any)
*/



#define	VIEWMAP_VERSION		8
#define VIEWMAP_DATASET_VERSION 9 /* version of VIEWMAP_DATASET_RECORD */
#define VM_MAX_OBJTYPES		32

/* Navigator Dataset Flags */
#define VM_DSET_SHOW_GRID		0x0001	/* show the grid in design mode, NIY */
#define VM_DSET_SNAPTO_GRID		0x0002	/* snap to grid */
#define VM_DSET_SAVE_IMAGEMAP	0x0004	/* save web imagemap of navigator so it 
										*  looks good on the web */

typedef struct
	{
	WSIG 		Header;
	WORD		Version;
	WORD		ViewNameLen;	/* length of initial view name; 0 if none */
	WORD		Gridsize;		/* (in pixels) */
	WORD		Flags;			/* VM_DSET_XXXXXXXX */
	WORD		bAutoAdjust;
	WORD		BGColor;
	WORD		SeqNums[VM_MAX_OBJTYPES];  /* highest sequence number for */
										/* each type of draw object */
										/* supported (w/extra space for */
										/* future) */
	VIEWMAP_STYLE_DEFAULTS	StyleDefaults;
	WORD		NumPaletteEntries;
	WORD		ViewDesignType;		/* design type of initial view */
	COLOR_VALUE BGColorValue;		/*	BG color stored in some color space */
	DWORD		spare[14];
	} VIEWMAP_DATASET_RECORD;

/*
	Composite record definitions:
*/

/*	Structure to store BoundRect in ODS records: */
typedef struct
	{
	LONG		left;
	LONG		top;
	LONG		right;
	LONG		bottom;
	} VMODSrect;


/*		Component struct common to most ViewMap draw object records: */

#define	VM_DROBJ_FLAGS_VISIBLE			0x0002	/*	Set if obj is visible */
#define	VM_DROBJ_FLAGS_SELECTABLE		0x0004	/*	Set if obj can be select 
													(i.e. is not background) */
#define	VM_DROBJ_FLAGS_LOCKED			0x0008	/*	Set if obj can't be edited */
#define VM_DROBJ_FLAGS_IMAGEMAP_BITMAP	0x0010	/*	Bitmap representing runtime image
												*	of the navigator.  Use to create
												*	imagemaps from navigators.
												*/
#define VM_DSET_READING_ORDER_RTL	0x0020	/* reading order */

typedef struct
	{
	BSIG 		Header;
	VMODSrect	ObjRect;
	WORD		flags;
	WORD		NameLen;
	WORD		LabelLen;
	FONTID		FontID;
	WORD		TextColor;
	WORD		Alignment;
	WORD		bWrap;
	DWORD		spare[4];		/* for future use */
	} VMODSdrobj;

typedef struct
	{
	WSIG 		Header;
	VMODSrect	ObjRect;
	WORD		flags;
	WORD		NameLen;
	WORD		LabelLen;
	FONTID		FontID;
	WORD		TextColor;
	WORD		Alignment;
	WORD		bWrap;
	DWORD		spare[4];		/* for future use */
	} VMODSbigobj;


/*		VIEWMAP_HEADER_RECORD	 	*/

typedef struct
	{
	BSIG 		Header;
	WORD		Version;		/* from VIEWMAP_VERSION def, above */
	WORD		NameLen;		/* always 0 (reserved for future use) */
	} VIEWMAP_HEADER_RECORD;


/*		VIEWMAP_RECT_RECORD */
/*		(followed by Instance Name) */

typedef struct
	{
	VMODSdrobj	DRobj;
	WORD		LineColor;
	WORD		FillFGColor;
	WORD		FillBGColor;
	WORD		LineStyle;
	WORD		LineWidth;
	WORD		FillStyle;
	DWORD		spare[4];		/* for future use */
	} VIEWMAP_RECT_RECORD;


/*		VIEWMAP_TEXTBOX_RECORD */
/*		(followed by Instance Name, ) */

typedef struct
	{
	VMODSbigobj	DRobj;
	WORD		LineColor;
	WORD		FillFGColor;
	WORD		FillBGColor;
	WORD		LineStyle;
	WORD		LineWidth;
	WORD		FillStyle;
	DWORD		spare[4];		/* for future use */
	} VIEWMAP_TEXT_RECORD;


/*		VIEWMAP_BITMAP_RECORD */
/*		(followed by Instance Name and Bitmap data; */
/*		a null background is represented by a bitmap */
/*		record where NameLen = 0 and DataLen = 0, and */
/*		no trailing data is present) */

typedef struct
	{
	VMODSdrobj	DRobj;
	WORD		DataLen;
	WORD		xBytes;		/* width in bytes */
	WORD		yBits;		/* height in bits */
	WORD		zBits;		/* depth in bits */
	DWORD		spare[4];	/* for future use */
	} VIEWMAP_BITMAP_RECORD;

/*		VIEWMAP_REGION_RECORD */
/*		(followed by Instance Name and Label if any) */

typedef struct
	{
	VMODSdrobj	DRobj;
	WORD		LineColor;
	WORD		LineStyle;
	WORD		LineWidth;
	WORD		FillStyle;
	DWORD		spare[4];		/* for future use */
	} VIEWMAP_REGION_RECORD;


/*		VIEWMAP_POLYGON_RECORD */
/*		(followed by Instance Name and points list) */

typedef struct
	{
	VMODSbigobj	DRobj;
	WORD		LineColor;
	WORD		FillFGColor;
	WORD		FillBGColor;
	WORD		LineStyle;
	WORD		LineWidth;
	WORD		FillStyle;
	WORD		nPts;
	DWORD		spare[4];		/* for future use */
	} VIEWMAP_POLYGON_RECORD;


/*		VIEWMAP_POLYLINE_RECORD */
/*		(followed by Instance Name and points list) */

typedef struct
	{
	VMODSbigobj	DRobj;
	WORD		LineColor;
	WORD		LineStyle;
	WORD		LineWidth;
	WORD		nPts;
	DWORD		spare[4];		/* for future use */
	} VIEWMAP_POLYLINE_RECORD;


/*		VIEWMAP_ACTION_RECORD */
/*		(followed by ActionData string if any) */

typedef struct
	{
	WSIG 		Header;
	WORD		bHighlightTouch;
	WORD		bHighlightCurrent;
	WORD		HLOutlineColor;
	WORD		HLFillColor;		
	WORD		ClickAction;
	WORD		ActionStringLen;
	WORD		HLOutlineWidth;
	WORD		HLOutlineStyle;
	NOTELINK	LinkInfo;
	WORD		ExtDataLen;		/* length of extended action data, 	*/
								/* e.g. compiled script 		*/
	WORD		ActionDataDesignType;	/* this is the design type for the named
										   folder or view named in the ActionString */
	DWORD		spare[2];		/* reserved for future use */
	} VIEWMAP_ACTION_RECORD;


/* property values for ACTION: */

#define		VM_ACTION_NONE				0
#define		VM_ACTION_SWITCHVIEW		1
#define		VM_ACTION_SWITCHNAV			2
#define		VM_ACTION_ALIAS_FOLDER		3
#define		VM_ACTION_GOTO_LINK			4
#define		VM_ACTION_RUNSCRIPT			5
#define		VM_ACTION_RUNFORMULA		6
#define		VM_ACTION_GOTO_URL			8

/* property values for line and fill styles: */

#define	VM_LINE_SOLID			0		
#define	VM_FILL_SOLID			1

#endif /* VMODS_DEFS */


