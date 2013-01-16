
/*	Notes Rich Text On-Disk Structure Definitions -
	Record format used in the NSF data type TYPE_COMPOSITE. */



#ifndef EDIT_ODS_DEFS
#define EDIT_ODS_DEFS

#ifndef ODS_DEFS
#include "ods.h"
#endif
	
#ifndef NSF_DATA_DEFS
#include "nsfdata.h"					/* for NOTELINK */
#endif

#ifndef FONTID_DEFS
#include "fontid.h"						/* for FONTID and MAXFACESIZE */
#endif

#ifndef MISC_DEFS
#include "misc.h"						/* for NFMT, TFMT, and NCURFMT */
#endif

#ifndef COLORODS_DEFS
#include "colorods.h"					/* COLOR_VALUE */
#endif


/*	Paragraph Record - Defines the start of a new paragraph */
	
typedef struct {
	BSIG Header;
} CDPARAGRAPH;

/*	Paragraph Attribute Block Definition Record */

#define MAXTABS 20				/* maximum number of stops in tables */
#define MAX_STYLE_NAME 35
#define MAX_STYLE_USERNAME 128

typedef struct {
	WSIG Header;
	WORD PABID;					/* ID of this PAB */
	WORD JustifyMode;			/* paragraph justification type */
	WORD LineSpacing;			/* (2 * (Line Spacing - 1)) (0:1,1:1.5,2:2,etc) */
	WORD ParagraphSpacingBefore; /* no. of LineSpacing units above paragraph */
	WORD ParagraphSpacingAfter;	/* no. of LineSpacing units below paragraph */
	WORD LeftMargin;			/* leftmost margin, twips rel to abs left */
								/* (16 bits = about 44") */
	WORD RightMargin;			/* rightmost margin, twips rel to abs right */
								/* (16 bits = about 44") */
								/* Special value "0" means right margin */
								/* will be placed 1" from right edge of */
								/* paper, regardless of paper size. */
	WORD FirstLineLeftMargin;	/* leftmost margin on first line */
								/* (16 bits = about 44") */
	WORD Tabs;					/* number of tab stops in table */
	SWORD Tab[MAXTABS];			/* table of tab stop positions, negative
								   value means decimal tab */
								/* (15 bits = about 22") */
	WORD Flags;					/* paragraph attribute flags */
	DWORD TabTypes;				/* 2 bits per tab */
	WORD Flags2;				/* extra paragraph attribute flags */

} CDPABDEFINITION;


/*	New PAB record for V4 -hide when formula.	*/

typedef struct
	{
	WSIG	Header;
	WORD	PABID;
	BYTE	Reserverd [8];
	} CDPABHIDE;
	/* Follows is the actual formula.		*/

/*	PAB Reference Record -

	This record is output in two situations:  First, at the start of every
	item of type Composite.  Second, at the start of every paragraph.  If,
	when reading this record during a note READ operation, the paragraph
	in question has NO runs in it, this defines the PAB to use for the
	paragraph.  If the paragraph has runs already in it, ignore this record. */
	
typedef struct {
	BSIG Header;
	WORD PABID;					/* ID number of the PAB being referenced */
} CDPABREFERENCE;


/*	This record is similar to a pab reference record but applies
	only to the pab's hide when formula which is new for V4
	so we make a new type that v3 can safely ignore.
*/

typedef struct 
{
	BSIG Header;
	WORD SourcePABID;				/* ID number of the source PAB 
										containing the formula.	*/
	WORD DestPABID;					/*	ID number of the dest PAB */
} CDPABFORMULAREF;


/*	This DWORD, ExtendPabFlags, extends the PAB structure.   Use the ExtendedPab flags
	to know what to read next */

#define EXTENDEDPABFLAGS3	0x00000001L	/* If True then need make another read for Flags3 */

/* 	This DWORD extends the flags and flags 2 in the CDPABDEFINITION 
	record
*/

#define PABFLAG3_HIDE_EE		0x00000001L	/* True, if Hide when embedded */
#define PABFLAG3_HIDE_MOBILE 	0x00000002L /* True, if hidden from mobile clients */
#define PABFLAG3_LAYER_USES_DRM 0x00000004L /* True if boxes in a layer have set PABFLAG_DISPLAY_RM on pabs */


typedef struct
{
	WSIG Header;					/*	Header Jive					*/
	WORD Datalength;
	DWORD Reserved;
	/* Actual anchor text follows.		*/
} CDANCHOR;


/*	Style Name Record */

#define STYLE_FLAG_FONTID		0x00000001L	/* A FONTID follows the CDSTYLENAME structure.
												The font is included in the style. */
#define STYLE_FLAG_INCYCLE		0x00000002L	/* This style is included in the Cycle Key [F11].
												The style is available when you press F11
												to cyle through the named styles. */
#define STYLE_FLAG_PERMANENT	0x00000004L	/* A user name follows the CDSTYLENAME structure.
												The style is available for all documents. */
typedef struct
	{
	BSIG Header;
	DWORD Flags;				/* Style flags */
	WORD PABID;					/* ID number of the PAB being named */
	char StyleName[MAX_STYLE_NAME+1];	/* The style name. */
	/*	If STYLE_FLAG_FONTID, a FONTID follows this structure. */
	/*	If STYLE_FLAG_PERMANENT, a user name follows. */
	} CDSTYLENAME;


/*  Pre Table Begin Record -

	This record was put in so that we can give table data needed up front
	for Domino in a record that will be ignored in pre 5.0 versions. 5.0
	will take advantage of this record to expand the tablebegin record
	but will ignore some of the record which is filled in just for Domino */

	typedef struct
	{
	WSIG	Header;
#define	CDPRETABLE_AUTO_CELL_WIDTH		0x00000001	/* True if automatic cell width calculation */
#define	CDPRETABLE_DONTWRAP				0x00000002
#define CDPRETABLE_DROPSHADOW			0x00000004
#define CDPRETABLE_FIELDDRIVEN			0x00000008
#define CDPRETABLE_V4SPACING			0x00000010
#define CDPRETABLE_USEBORDERCOLOR		0x00000020
#define CDPRETABLE_WIDTHSAMEASWINDOW	0x00000040  /* True if the table width equal to window width */
#define CDPRETABLE_SHOWTABS				0x00000080  /* True if field driven table should also show tabs */
#define CDPRETABLE_SHOWTABSONLEFT		0x00000100  
#define CDPRETABLE_SHOWTABSONBOTTOM		0x00000200  
#define CDPRETABLE_SHOWTABSONRIGHT		0x00000400  
	DWORD	Flags;
	BYTE	Rows;
	BYTE	Columns;
	DWORD	ColumnSizingBits1;
	DWORD	ColumnSizingBits2;
#define CDTABLEVIEWER_ONCLICK			1	
#define CDTABLEVIEWER_ONLOADTIMER		2
#define CDTABLEVIEWER_ONLOADCYCLEONCE	3
#define CDTABLEVIEWER_TABS				4
#define CDTABLEVIEWER_FIELDDRIVEN		5
#define CDTABLEVIEWER_CYCLEONCE			6
#define CDTABLEVIEWER_CAPTIONS			8
#define CDTABLEVIEWER_LAST				8
	BYTE	ViewerType;
	BYTE	Spare;
	WORD	MinRowHeight;
	WORD	Spares[1];		
	DWORD	StyleColor1;
	DWORD	StyleColor2;
	COLOR_VALUE	InnerBorderColor;
	WORD	NameLength;
	/* NOTE:	The following lengths can not be used.  Previous versions of Notes
				are not setup to deal with any length beyond the name length.
				Hang will occur if they are used and you try to open the document
				on a previous version of Notes.  In the same light, this sturcture's
				size can not be changed, no additional items of variable length
				can be added.

				The words could simply be used as word storage.  They just can
				not be useds as lengths.
	*/
	WORD	ImagePacketLength;	
	WORD	RowLabelDataLength;
	}	CDPRETABLEBEGIN;

/* Table Data Extension Record 

	This record was added because the Pre Table Begin Record can not be
	expanded and R6 required more data to be stored. */

typedef struct
	{
	WSIG	Header;
									/* Fixed section ------------ */
	DWORD	dwColumnSizeToFitBits1;
	DWORD	dwColumnSizeToFitBits2;
	WORD	wEqualSizeTabsWidthX;
	WORD	wTabsIndentWidthX;
									/* Each Available member can be used explicitly for
										data. */
	WORD	wAvailable3;
	WORD	wAvailable4;
	DWORD	dwAvailable5;
	DWORD	dwAvailable6;
	DWORD	dwAvailable7;
	DWORD	dwAvailable8;
	DWORD	dwAvailable9;

	WORD	wcTabLabelFont;			/* Length of Tabs Label Font */

									/* Each AvailableLength member can be used for
										data in the variable section. */
	WORD	wAvailableLength11;
	WORD	wAvailableLength12;

	WORD	wExtension2Length;		/* When all Available members have been used up, this
										should be used to extend to another data extension
										for additional storage. */

									/* Variable section ------------ */
	/* The wcTabLabelFont variable data defined below is actually 
	 *   a FONTID followed by a DWORD followed by a COLOR_VALUE.
	 *   Some Notes6 pre-release versions only wrote the FONTID/DWORD combination 
	 *   without the trailing COLOR_VALUE, so need to verify that 
	 *   wcTabLabelFont is at least large enough to hold FONTID, DWORD, and COLOR_VALUE 
	 *   before trying to read off the COLOR_VALUE.  SDK consumers should
	 *   ignore the value of the DWORD when reading this structure and
	 *   write the DWORD value as 0 when writing this structure
	 */
	/* <wcTabLabelFont variable data> */
	/* <wAvailableLength11 variable data> */
	/* <wAvailableLength12 variable data> */
	/* <wExtension2Length variable data>  */
} CDTABLEDATAEXTENSION;

/*	Begin Table Record -

	This record specifies the beginning of a table.  It contains interesting
	information about the format and size of the table. */

typedef struct
	{
	BSIG Header;
	WORD LeftMargin;					/*	TWIPS */
	WORD HorizInterCellSpace;			/*	TWIPS */
	WORD VertInterCellSpace;			/*	TWIPS */
/* NOTE! all items below this comment are NOT guaranteed to have been zeroed if
	created in V2; all items are zeroed before use in V4 */
	WORD V4HorizInterCellSpace;			/*	TWIPS -- this field was spare in v3 */
	WORD V4VertInterCellSpace;			/*	TWIPS -- this field was spare in v3 */	
	WORD Flags;							/* Flags (CDTABLE_xxx) */
#define	CDTABLE_AUTO_CELL_WIDTH		0x0001	/* True if automatic cell width calculation */
#define CDTABLE_V4_BORDERS			0x0002  /* True if the table was created in v4 */
#define CDTABLE_3D_BORDER_EMBOSS	0x0004  /* True if the table uses embossed borders */
#define CDTABLE_3D_BORDER_EXTRUDE	0x0008  /* True if the table uses extruded borders */
#define CDTABLE_BIDI_RTLTABLE		0x0010  /* True if the table reading order is right to left */
#define CDTABLE_ALIGNED_RIGHT		0x0020  /* True if the table alignment is right */
#define CDTABLE_COLLAPSIBLE			0x0040  /* True if the table is collapsible to one row */
#define CDTABLE_LEFTTOP				0x0080  
#define CDTABLE_TOP					0x0100 
#define CDTABLE_LEFT				0x0200 
#define CDTABLE_ALTERNATINGCOLS		0x0400 
#define CDTABLE_ALTERNATINGROWS		0x0800 
#define CDTABLE_RIGHTTOP			0x2000
#define CDTABLE_RIGHT				0x4000
#define CDTABLE_SOLID				0x6f80	/* all styles on means solid color*/
#define CDTABLE_TEMPLATEBITS		0x6f80
#define CDTABLE_ALIGNED_CENTER		0x1000  /* True if the table alignment is center */
#define CDTABLE_TEXTFLOWS			0x8000  /* True if the table rows text flows cell to cell */
	} CDTABLEBEGIN;

typedef struct
	{
	BSIG Header;
	BYTE Row;						/*	Row number (0 based) */
	BYTE Column;					/*	Column number (0 based) */
	WORD LeftMargin;				/*	Twips */
	WORD RightMargin;				/*	Twips */
	WORD FractionalWidth;			/*	20" (in twips) * CellWidth / TableWidth
										Used only if AutoCellWidth is
										specified in the TABLEBEGIN. */
	BYTE Border;					/*	4 cell borders, each 2 bits wide */
									/*	(see shift and mask CDTC_xxx values) */
									/*	Value of each cell border is one of */
									/*	TABLE_BORDER_xxx. */
#define	CDTC_S_Left		0
#define	CDTC_M_Left		0x0003
#define	CDTC_S_Right	2
#define	CDTC_M_Right	0x000c
#define	CDTC_S_Top		4
#define	CDTC_M_Top		0x0030
#define	CDTC_S_Bottom	6
#define	CDTC_M_Bottom	0x00c0
#define TABLE_BORDER_NONE	0
#define TABLE_BORDER_SINGLE	1
#define TABLE_BORDER_DOUBLE	2

	BYTE Flags;						
#define CDTABLECELL_USE_BKGCOLOR	0x01	/* True if background color */
#define CDTABLECELL_USE_V42BORDERS	0x02	/* True if version 4.2 or after */
#define CDTABLECELL_INVISIBLEH		0x04	/* True if cell is spanned */
#define CDTABLECELL_INVISIBLEV		0x08	/* True if cell is spanned */
#define CDTABLECELL_USE_GRADIENT	0x10	/* True if gradient color */
#define CDTABLECELL_VALIGNCENTER	0x20	/* True if contents centered vertically */
#define CDTABLECELL_GRADIENT_LTR	0x40	/* True if gradient should go left to right */
#define CDTABLECELL_VALIGNBOTTOM	0x80	/* True if contents bottomed vertically */

	WORD v42Border;				/* wider borders */
#define	CDTC_S_V42_Left		0
#define	CDTC_M_V42_Left		0x000f
#define	CDTC_S_V42_Right	4
#define	CDTC_M_V42_Right	0x00f0
#define	CDTC_S_V42_Top		8
#define	CDTC_M_V42_Top		0x0f00
#define	CDTC_S_V42_Bottom	12
#define	CDTC_M_V42_Bottom	0xf000

	BYTE RowSpan;
	BYTE ColumnSpan;
	WORD BackgroundColor;			/* Color of background of cell */
	} CDTABLECELL;

typedef struct
	{
	BSIG Header;
	DWORD Spare;
	} CDTABLEEND;

#define MAXREGIONNAME 35

typedef struct {
	WSIG Header;		/* Signature and length of this record */
	WORD Version;		
	WORD Flags;	  
	WORD RegionNum;
	char RegionName[MAXREGIONNAME+1];
} CDREGIONBEGIN;

typedef struct {
	WSIG Header;		/* Signature and length of this record */
	WORD RegionNum;
	char RegionName[MAXREGIONNAME+1];
} CDREGIONEND;

#define LARGEPARAGRAPH_VERSION1	0x0001
#define LARGEPARAGRAPH_CURRENT_VERSION	LARGEPARAGRAPH_VERSION1

typedef struct {
	WSIG Header;		/* Signature and length of this record */
	WORD Version;
	WORD Flags;		
#define CDLARGEPARAGRAPH_BEGIN	0x0001
#define CDLARGEPARAGRAPH_END	0x0002
	DWORD Spare[2];
} CDLARGEPARAGRAPH;

/*	CDFIELD - Field Reference Record, used in forms ($BODY) to define a field. */

typedef struct {
	WSIG Header;
	WORD Flags;					/* Field Flags (see Fxxx flags below) */
	WORD DataType;				/* Alleged NSF Data Type */
	WORD ListDelim;				/* List Delimiters (LDELIM_xxx and LDDELIM_xxx) */
	NFMT NumberFormat;			/* Number format, if applicable */	
	TFMT TimeFormat;			/* Time format, if applicable */
	FONTID FontID;				/* displayed font */
	WORD DVLength;				/* Default Value Formula */
	WORD ITLength;				/* Input Translation Formula */
	WORD TabOrder;				/* Order in tabbing sequence */
	WORD IVLength;				/* Input Validity Check Formula */
	WORD NameLength;			/* NSF Item Name */
	WORD DescLength;			/* Description of the item */
	WORD TextValueLength;		/* (Text List) List of valid text values */
								/* Now comes the variable part of the struct... */
} CDFIELD;

/*	CDFIELD List Delimeters (ListDelim) */

#define	LDELIM_SPACE		0x0001	/* low three nibbles contain delim flags */
#define	LDELIM_COMMA		0x0002
#define	LDELIM_SEMICOLON	0x0004
#define	LDELIM_NEWLINE		0x0008
#define	LDELIM_BLANKLINE	0x0010
#define	LD_MASK				0x0fff

#define	LDDELIM_SPACE		0x1000	/* high nibble contains the display type */
#define	LDDELIM_COMMA		0x2000
#define	LDDELIM_SEMICOLON	0x3000
#define	LDDELIM_NEWLINE		0x4000
#define	LDDELIM_BLANKLINE	0x5000
#define	LDD_MASK			0xf000

/*	CDFIELD Flags Definitions */

#define	V3SPARESTOCLEAR		0x0075	/* Clear these if FOCLEARSPARES is TRUE */

#define	FREADWRITERS		0x0001	/* Field contains read/writers */
#define	FEDITABLE			0x0002	/* Field is editable, not read only */
#define	FNAMES				0x0004	/* Field contains distinguished names */
#define	FSTOREDV			0x0008	/* Store DV, even if not spec'ed by user */
#define	FREADERS			0x0010	/* Field contains document readers */
#define FSECTION			0x0020	/* Field contains a section */
#define	FSPARE3				0x0040	/* can be assumed to be clear in memory, V3 & later */
#define	FV3FAB				0x0080	/* IF CLEAR, CLEAR AS ABOVE */
#define	FCOMPUTED			0x0100	/* Field is a computed field */
#define	FKEYWORDS			0x0200	/* Field is a keywords field */
#define	FPROTECTED			0x0400	/* Field is protected */
#define	FREFERENCE			0x0800	/* Field name is simply a reference to a shared field note */
#define	FSIGN				0x1000	/* sign field */
#define	FSEAL				0x2000	/* seal field */
#define	FKEYWORDS_UI_STANDARD		0x0000	/* standard UI */
#define	FKEYWORDS_UI_CHECKBOX		0x4000	/* checkbox UI */
#define	FKEYWORDS_UI_RADIOBUTTON	0x8000	/* radiobutton UI */
#define	FKEYWORDS_UI_ALLOW_NEW		0xc000	/* allow doc editor to add new values */

#define EC_FLAG_WIDTH_PROPORTIONAL	0x0001	/* field width proportional with font size (const # of chars) (height is part of flags2) */

/*	CDEXT2FIELD - Extended field reference record used for currency, numeric symbol, etc., information */

typedef struct {
	WSIG	Header;
	/* Numeric symbol data */
	BYTE	NumSymPref;			/* NPREF_xxx */
	BYTE	NumSymFlags;		/* NNUMSYM_xxx */
	DWORD	DecimalSymLength;
	DWORD	MilliSepSymLength;
	DWORD	NegativeSymLength;
	WORD	MilliGroupSize;
 	SWORD	VerticalSpacing;	/* extra vertical spacing (%) */
	SWORD	HorizontalSpacing;	/* extra horizontal spacing (%) */
	WORD	Unused2;
	WORD	FirstFieldLimitType;
	/* Currency data */
	BYTE	CurrencyPref;		/* NPREF_xxx */
	BYTE	CurrencyType;		/* NCURFMT_xxx */
	BYTE	CurrencyFlags;		/* NCURFMT_xxx */
	DWORD	CurrencySymLength;
	DWORD	ISOCountry;	
	DWORD	Unused3;
	DWORD	Unused4;
	/* Date/time formatting data */
	BYTE	DTPref;				/* NPREF_xxx */
	DWORD	DTFlags;			/* DT_xxx */
	DWORD	DTFlags2;			/* DT_xxx */
	BYTE	DTDOWFmt;			/* DT_WFMT_xxx */
	BYTE	DTYearFmt;			/* DT_YFMT_xxx */
	BYTE	DTMonthFmt;			/* DT_MFMT_xxx */
	BYTE	DTDayFmt;			/* DT_DFMT_xxx */
	BYTE	DTDsep1Len;
	BYTE	DTDsep2Len;
	BYTE	DTDsep3Len;
	BYTE	DTTsepLen;
	BYTE	DTDShow;			/* DT_DSHOW_xxx */
	BYTE	DTDSpecial;			/* DT_DSPEC_xxx */
	BYTE	DTTShow;			/* DT_TSHOW_xxx */
	BYTE	DTTZone;			/* TZFMT_xxx */
	DWORD	Unused5;
	/* Proportional font for fields data	*/

	BYTE	ECFlags;
	BYTE	Unused612;
	WORD	wCharacters;	/* Number of characters if proportional width	*/
	WORD	wInputEnabledLen;	/* Input enabled formula. */
	WORD	wIMGroupFormulaLen;	/* Instant messageing buddy list group name. */
							/* Now comes the variable part of the struct... */
} CDEXT2FIELD;

/*	CDEXTFIELD - Extended Field Reference Record, used in forms ($BODY) to define a field. */

typedef struct {
	WSIG Header;
	DWORD Flags1;				/* Field Flags (see FEXT_xxx flags below) */
	DWORD Flags2;
	WORD EntryHelper;			/* Field entry helper type (see FIELD_HELPER_XXX below) */
	WORD EntryDBNameLen;		/* Entry helper DB name length */
	WORD EntryViewNameLen;		/* Entry helper View name length */
	WORD EntryColumnNumber;		/* Entry helper column number */
								/* Now comes the variable part of the struct... */
} CDEXTFIELD;

/*	Flags for CDEXTFIELD Flags1.  Note that the low word in Flags1 is not used. */

#define FEXT_LOOKUP_EACHCHAR		0x00010000L	/* lookup name as each char typed */
#define FEXT_KWSELRECALC			0x00020000L	/* recalc on new keyword selection */
#define FEXT_KWHINKYMINKY			0x00040000L	/* suppress showing field hinky minky */
#define FEXT_AFTERVALIDATION		0x00080000L	/* recalc after validation */
#define FEXT_ACCEPT_CARET			0x00100000L	/* the first field with this bit set will accept the caret */
/*	These bits are in use by the	0x02000000L
	column value.  The result of	0x04000000L
	the shifted bits is (cols - 1)	0x08000000L */
#define FEXT_KEYWORD_COLS_SHIFT		25
#define FEXT_KEYWORD_COLS_MASK		0x0E000000L
#define FEXT_KEYWORD_FRAME_3D		0x00000000L
#define FEXT_KEYWORD_FRAME_STANDARD	0x10000000L
#define FEXT_KEYWORD_FRAME_NONE		0x20000000L
#define FEXT_KEYWORD_FRAME_MASK		0x30000000L
#define FEXT_KEYWORD_FRAME_SHIFT	28
#define FEXT_KEYWORDS_UI_COMBO		0x40000000L
#define FEXT_KEYWORDS_UI_LIST		0x80000000L

/*	Flags for CDEXTFIELD Flags2. */

#define FEXT_KW_CHOICE_RECALC		0x00000001 /* TRUE to recalc the value choices. */
#define FEXT_HTML_IN_FIELDDEF		0x00000002 /* TRUE means we have a CD_EXTHTML field */

#define FEXT_HIDEDELIMITERS			0x00000004 /* TRUE if hiding delimeters */
#define FEXT_KW_RTL_READING_ORDER	0x00000008
#define FEXT_ALLOWTABBINGOUT		0x00000010 /* TRUE if tab will exit field (used for richtext only) */
#define FEXT_PASSWORD				0x00000020 /* TRUE if field is a password field	*/
#define FEXT_USEAPPLETINBROWSER		0x00000040 /* TRUE if an applet should be used for a browser (richtext only)	*/
#define FEXT_CONTROL				0x00000080 /* TRUE if field is a control */

#define FEXT_LITERALIZE				0x00000100 /* TRUE if this is a formula
													field which should have 
													item substitution based on
													on items on the form.  This
													is the counterpart to a
													computed formula which is
													a formula programmatically
													generated through @formulas. */
#define FEXT_CONTROLDYNAMIC			0x00000200 /* TRUE if field is a dynamic control  */
#define FEXT_RUNEXITINGONCHANGE		0x00000400 /* TRUE if should run exiting event
												  when value changes.  Currently only
												  implemented for native date/time */
#define FEXT_TIMEZONE				0x00000800 /* TRUE if this is a time zone field */
#define FEXT_PROPORTIONALHEIGHT		0x00004000 /* TRUE if field has proportional height	*/
#define FEXT_PROPORTIONALWIDTH		0x00008000 /* TRUE if field has proportional width	*/
#define FEXT_SHOWIMSTATUS			0x02000000 /* TRUE if a names type field displays im online status */



/*	The following identifiers indicate the type of helper in use by the 
	Keyword and the Name helper/pickers */
/* these define the VarDataFlags signifying variable length data following struct */
#define	CDEXTFIELD_KEYWORDHELPER	0x0001
#define	CDEXTFIELD_NAMEHELPER		0x0002
#define 	FIELD_HELPER_NONE			0
#define 	FIELD_HELPER_ADDRDLG		1
#define 	FIELD_HELPER_ACLDLG			2
#define 	FIELD_HELPER_VIEWDLG		3
	
/*	(*** OBSOLETE ***) Pre-V1 Field Reference Record, used in Forms (*** OBSOLETE ***) */

typedef struct {
	WSIG Header;
	WORD Flags;					/* Field Flags */
	WORD DataType;				/* Alleged NSF Data Type */
	WORD ListDelim;				/* List Delimiters */
	NFMT NumberFormat;			/* Number format, if applicable */	
	TFMT TimeFormat;			/* Time format, if applicable */
	FONTID FontID;				/* displayed font */
	WORD DVLength;				/* Default Value Formula */
	WORD ITLength;				/* Input Translation Formula */
	WORD Unused1;				/* Unused */
	WORD IVLength;				/* Input Validity Check Formula */
	WORD NameLength;			/* NSF Item Name */
	WORD DescLength;			/* Description of the item */
								/* Now comes the variable part of the struct... */

} CDFIELD_PRE_36;				/* List of Text Values was added... */

#define FIELDHINT_LIMITED		0x0001

/*	CDFIELDHINT	 */
typedef struct {
	WSIG	Header;				/* Tag and length */
	WORD	HintTextLength;
	WORD	Flags;				/* See above */
	WORD	Spare;
	DWORD	Spare2;
								/* The 8-bit text string follows... */
} CDFIELDHINT;

/* CDDECSFIELD */
typedef struct {
	WSIG	Header;
	WORD	Flags;
	WORD	ExternalNameLength;
	WORD	MetadataNameLength;
	WORD	DCRNameLength;
	WORD	Spare[8];
} CDDECSFIELD;

#define FDECS_KEY_FIELD			0x0001
#define FDECS_STORE_LOCALLY	0x0002

/*	CDTEXT - 8-bit text string record */

typedef struct {
	WSIG	Header;				/* Tag and length */
	FONTID	FontID;				/* Font ID */
								/* The 8-bit text string follows... */
} CDTEXT;
typedef struct {
	WSIG	Header;				/* Tag and length */
	FONTID	FontEffectID;				/* Font ID */
} CDTEXTEFFECT;


#define LINK_VERSION1    0

/*	CDLINK2 - Link record */

typedef struct	{
	WSIG Header;
	WORD LinkID;				/* Index into array in $LINKS/$FORMLINKS field of this document */
								/* Now comes the display comment... */
} CDLINK2;


/*	CDLINKEXPORT - This record is used in the case of exporting
	a note to the clipboard, where the NSF item describing the links
	cannot be generated. */


typedef struct	{
	WSIG Header;
	NOTELINK NoteLink;
								/* Now comes the display comment... */
} CDLINKEXPORT2;


/*	CDKEYWORD - Keyword Record */

#define CDKEYWORD_RADIO				0x0001
/*	These bits are in use by the	0x0002
	column value.  The result of	0x0004
	the shifted bits is (cols - 1)	0x0008 */
#define CDKEYWORD_COLS_SHIFT		1
#define CDKEYWORD_COLS_MASK			0x000E
#define CDKEYWORD_FRAME_3D			0x0000
#define CDKEYWORD_FRAME_STANDARD	0x0010
#define CDKEYWORD_FRAME_NONE		0x0020
#define CDKEYWORD_FRAME_MASK		0x0030
#define CDKEYWORD_FRAME_SHIFT		4		
#define CDKEYWORD_KEYWORD_RTL		0x0040
#define CDKEYWORD_RO_ACTIVE			0x0080

typedef struct {
	WSIG	Header;				/* Tag and length */
	FONTID	FontID;				/* Font ID */
	WORD	Keywords;			/* number of keywords */
	WORD	Flags;
/*	char	OnOff[];			   array of '1' or '0' indicating state */
/*	char	TextValues[];		   packed buffer of keyword text, fab->pTextValues format */
} CDKEYWORD;



/*	Here is a description of Notes bitmap encoding.
*
*	COLOR FORMATS:
*
*	Notes displays 3 types of bitmaps: monochrome, color and grey scale. 
*	All monochome bitmaps are one bit per pixel.  Color bitmaps can be either 8 bits per Pel (color
*	mapped) or 16 bits per pel "quasi true" color.  Grey scale bitmaps 
*	are simply treated as "color" bitmaps, using the 8 bits per Pel format
*	with a color table whose RGB tuples range from [0,0,0] through 
*	[255,255,255].
*
*	GEOMETRY:
*
*	All bitmaps are single plane encoding. 8 bit color/grey scale must have
*	color tables provided. Monochome bitmaps and 16 bit "quasi true" color
*	bitmaps don't need a color table.  
*
*	RASTER LINE ENCODING:
*
*	For those programmers using Notes API, raster lines are encoded using
*	a simple run-length encoding format, where each raster line of the 
*	bitmap is encoded separately (i.e. run length won't exceed length of
*	a raster line). Also, each raster line is NOT padded to any particular
*	boundary; each scanline ends on the byte boundary which is defined by the
*	width of the bitmap. The following section describes how each raster line
*	is encoded using a simple run-length encoding scheme.
*
*
*		Notes bitmap compression scheme description
*		--------------------------------------------		
*		We have devised a scheme which does a good job compressing 
*		monochrome, color (both mapped and RGB) and gray scale 
*		bitmaps, and a secondary encoding to allow "raw" uncompressed
*		scanlines for those scanlines which actually "expand" by using
*		the run-length scheme (this happens in dithered images).  
*		In the run length encoding scheme, there an escape codes followed by either 
*		a run length byte which is then followed by the 
*		byte(s) to repeat. Note that the byte(s) to repeat may be either 
*		one or two bytes depending on the color format. For monochrome,
*		8 bit color and 8 bit grey scale, use one byte. For 16 bit color
*		the PEL is two bytes long, so the two byte quantity is repeated. 
*
*			MSB<---------->LSB
*			+-----------------------------------+
*		(1)	| 1 1 c c c c c c | r r r r r r r r |
*			+-----------------------------------+
*				cccccc = six bit repeat count
*				rrrrrrrr = PELS to repeat
*
*
*		In the following non-compressed encoding, the escape code is 
*		followed by a 6 bit repeat count of raw PELs (one or two byte).
*
*
*			MSB<---------->LSB
*			+------------------------------------------------------
*		(2)	| 0 0 c c c c c c | r r r r r r r r |[r r r r r r r r]|...
*			+------------------------------------------------------
*				cccccc = 6 bit repeat count
*				r[cccccc] = 1 or more raw uncompressed PELS							
*
*/


/*	A color table (used in CDBITMAPHEADER) is an array of packed colors.
	Each color is stored in 3 bytes (Red,Green,Blue), packed without any
	intervening pad bytes. */

#define CT_ENTRY_SIZE		3  /* Always 3 bytes, packed */
#define CT_RED_OFFSET		0
#define CT_GREEN_OFFSET		1
#define CT_BLUE_OFFSET		2

#define CT_REDVALUE(x) (x[CT_RED_OFFSET])
#define CT_GREENVALUE(x) (x[CT_GREEN_OFFSET])
#define CT_BLUEVALUE(x) (x[CT_BLUE_OFFSET])

#define CT_NEXT(x) (x+=CT_ENTRY_SIZE)
#define CT_ENTRY_PTR(x,ElmNum) (&x[CT_ENTRY_SIZE*ElmNum])

/*	A pattern table is a fixed-size color table used for patterns by
	CDBITMAPHEADER (patterns are used to compress the bitmap).
	A entry in the pattern table is 8 (PELS_PER_PATTERN) packed colors
	(3 bytes per color as above). */

#define PELS_PER_PATTERN 8

/* Maximum number of patterns we will ever store in a CDBITMAPHEADER. */

#define MAXPATTERNS 64

/* Maximum number of colors in a color table (8 bit mapped color) */

#define MAXCOLORS 256

/*	The CDBITMAPHEADER record must be present for all bitmaps. It must
	follow the CDGRAPHIC record, but come before any of the other bitmap
	CD records. */

typedef struct
	{
	LSIG	Header;				/* Signature and Length */
	RECTSIZE Dest;				/* dest bitmap height and width in PELS */
	RECTSIZE Crop;				/* crop destination dimensions in PELS (UNUSED) */
	WORD  Flags;				/* CDBITMAP_FLAGS Valid only in CDGRAPHIC_VERSION2 and later */
	WORD  wReserved;			/* Reserved for future use */
	DWORD lReserved;			/* Reserved for future use */
	WORD Width;					/* Width of bitmap in PELS */
	WORD Height;				/* Height "				 " */
	WORD BitsPerPixel;			/* Bits per PEL, must be 1,8 or 16 */
	WORD SamplesPerPixel;		/* For 1 or 8 BPP, set to 1. For 16 BBP, set to 3 */
	WORD BitsPerSample;			/* For 1 BPP, set to 1. For 8 BPP, set to 8. For 16 BPP, set to 5 */
	WORD SegmentCount;			/* Number of CDBITMAPSEGMENTS */
	WORD ColorCount;			/* Number of entries in CDCOLORTABLE. 0-256 */
	WORD PatternCount;			/* Number of entries in CDPATTERNTABLE. Set to 0 if using Notes API. */
	} CDBITMAPHEADER;

/* Bitmap Uses > 16 colors or > 4 grey scale levels */

#define CDBITMAP_FLAG_REQUIRES_PALETTE	1	

/* Initialized by import code for "first time" importing of bitmaps
	from clipboard or file, to tell Notes that it should compute whether
	or not to use a color palette or not.  All imports and API programs
	should initially set this bit to let the Editor compute whether it
	needs the palette or not. */

#define CDBITMAP_FLAG_COMPUTE_PALETTE	2	


/* 	Bitmap Transparency Table (optionally one per bitmap).  The colors
	in this table specify the bitmap colors that are "transparent".  The
	pixels in the bitmap whose colors are in this table will not affect
	the background; the background will "bleed through" into the bitmap.
	The entries in the transparency table should be in the same format
	as entries in the color table.  If a transparency table is used
	for a bitmap, it must immediately follow the CDBITMAPHEADER. */

typedef struct
	{
	LSIG Header;				/* Signature and length */
	WORD Reserved;				/* Reserved for future use */
	WORD TransparentCount;		/* Count of entries that follow (0-256) */
	/* One or more transparent color table entries */
	} CDTRANSPARENTTABLE;


/*	Alternate HTML text record (used with graphics and embedded applets) */

/*	CDALTTEXT - Text string used for ALT= within <IMG> and <APPLET> HTML tags */

typedef struct {
	WSIG	Header;				/* Tag and length */
	WORD	wLength;			/* text length */
	WORD	Reserved1;			/* Reserved for future use */
	DWORD	Reserved2;			/* Reserved for future use */
								/* The 8-bit text string follows... */
} CDALTTEXT;

/*	CDHTMLFORMULA - Formula used to compute HTML Attributes and Alternate HTML 
	within <APPLET> HTML tags. May be used elsewhere in future. */

typedef struct {
	WSIG	Header;				/* Tag and length */
	DWORD	dwFlags;			/* Flags - denote what kind of formula this is */
	BYTE	cbLevel;			/* */
	BYTE	cbReserved;			/* Reserved for future use. */
	WORD	Reserved;			/* Reserved for future use */
								/* The formula follows... */
} CDHTMLFORMULA;

/*	Flags for use in the dwFlags field of CDFORMULA */

#define CDHTMLFORMULA_FLAG_ATTR				0x00000001		/* formula is HTML attributes */
#define CDHTMLFORMULA_FLAG_ALT				0x00000002		/* formula is alternate HTML */
#define CDHTMLFORMULA_FLAG_ACTION_LABEL		0x00000004		/* formula is an action label formula. */


/* 	Each of the following CDBITMAP segments contains the compressed raster
	data of the bitmap.  It is recommended that each segment be no larger
	than 10K for optimal use within Notes, but try to keep the segments as
	large as possible to increase painting speed. A scanline must not
	span a segment. A bitmap must contain at least one segment, but may have 
	many segments. */

typedef struct
	{
	LSIG	Header;				/* Signature and Length */
	DWORD Reserved[2];			/* Reserved for future use */
	WORD ScanlineCount;			/* Number of compressed scanlines in seg */
	WORD DataSize;				/* Size, in bytes, of compressed data */
	/* Comressed raster data for the segment follows right here */
	} CDBITMAPSEGMENT;


/* 	Bitmap Color Table. If the bitmap is 8 bit color or grey scale, you
	must have a color table.  However, you only need as many entries as
	you have colors, i.e. if a 16 color bitmap was converted to 8 bit
	form for Notes, the color table would only require 16 entries even
	though 8 bit color implies 256 entries.  The number of entries must
	match that specified in the CDBITMAPHEADER ColorCount. */

typedef struct
	{
	LSIG Header;
	/* One or more color table entries go here */
	} CDCOLORTABLE;

/* Bitmap Pattern Table (optionally one per bitmap) */

typedef struct
	{
	LSIG Header;
	/* One or more pattern table entries */
	} CDPATTERNTABLE;


/*	Crop rectangle used in graphic run */

typedef struct
	{
	WORD left;
	WORD top;
	WORD right;
	WORD bottom;	
	}CROPRECT;

/*	The Graphic combination record is used to store one or more graphic objects. 
	This record marks the beginning of a graphic composite item, and MUST
	be present for any graphic object to be loaded/displayed.  A
	graphic composite item can be one or more of the following CD
	record types: BITMAPHEADER, BITMAPSEGMENT, COLORTABLE, CGMMETA,
	WINMETA,WINMETASEG,PMMETAHEADER,PMMETASEG,MACMETAHEADER,MACMETASEG. If
	there is more than one graphic object, Notes will display only one object 
	using the following order: CGM Metafile, Native Metafile (i.e. Windows,
	PM,Mac),Bitmap. */

typedef struct
	{
	LSIG	Header;		/* Signature and Length */
	RECTSIZE DestSize;	/* Destination Display size in twips (1/1440 inch) */
	RECTSIZE CropSize;	/* Width and Height of crop rect in twips. Currently unused */
	CROPRECT CropOffset;/* Crop rectangle offset from bottom left of Dest (in twips).Currently unused */
	WORD	fResize;	/* Set to true if object has been resized by user. */
	BYTE    Version;	/* CDGRAPHIC_VERSION */
	BYTE	bFlags;		/* Ignored before CDGRAPHIC_VERSION3 */
	WORD	wReserved;
	} CDGRAPHIC;

/* Version control of graphic header */
#define CDGRAPHIC_VERSION1		0		/* Created by Notes version 2 */
#define CDGRAPHIC_VERSION2 		1		/* Created by Notes version 3 */
#define CDGRAPHIC_VERSION3 		2		/* Created by Notes version 4.5 */

/*	The following flag indicates that the DestSize field contains
	pixel values instead of twips. */

#define CDGRAPHIC_FLAG_DESTSIZE_IS_PIXELS	0x01
#define CDGRAPHIC_FLAG_SPANSLINES			0x02

/*	CGM Metafile Record. This record follows the CDGRAPHIC record. It can
	contain the entire contents of a CGM metafile, and must be <= 64K Bytes
	in length. */

typedef struct
	{
	LSIG	Header;				/* Signature and Length */
	SWORD	mm;					/* see above CGM_MAPMODE_??? */
	SWORD	xExt,yExt;			/* Extents of drawing in world coordinates */
	RECTSIZE OriginalSize;		/* Original display size of metafile in twips */
	/*	CGM Metafile Bits Follow, must be <= 64K bytes total */
	} CDCGMMETA;

#define CGM_MAPMODE_ABSTRACT	0	/* Virtual coordinate system. This is default */
#define CGM_MAPMODE_METRIC		1	/*	Currently unsupported */


/*	Windows Metafile Record. This record follows the CDGRAPHIC record and
	contains the entire contents of a Windows GDI metafile.  Since these
	metafiles tend to be large, they may be segmented in chunks of any
	arbitrary size, as long as each segment is <= 64K bytes. */

typedef struct
	{
	LSIG	Header;				/* Signature and Length */
	SWORD	mm;					/* Windows mapping mode */
	SWORD	xExt,yExt;			/* size in mapping mode units */
	RECTSIZE OriginalDisplaySize; /* Original display size of metafile in twips */
	DWORD	MetafileSize;		/* Total size of metafile raw data in bytes */
	WORD 	SegCount;			/*	Number of CDWINMETASEG records */
	/*	Metafile segments Follow */
	} CDWINMETAHEADER;

typedef struct
	{
	LSIG	Header;				/* Signature and Length */
	WORD 	DataSize;			/* Actual Size of metafile bits in bytes, ignoring any filler */
	WORD 	SegSize; 			/*  Size of segment, is equal to or larger than DataSize 
									if filler byte added to maintain word boundary */
	/* Windows Metafile Bits for this segment. Each segment must be
	   <= 64K bytes. */
	} CDWINMETASEG;


/*	PM Metafile Record. This record follows the CDGRAPHIC record and
	contains the entire contents of a PM GPI metafile.  Since these
	metafiles tend to be large, they may be segmented in chunks of any
	arbitrary size, as long as each segment is <= 64K bytes. */

typedef struct
	{
	LSIG	Header;				/* Signature and Length of this record */
	SWORD	mm;					/* PM mapping mode, i.e. PU_??? */
	SWORD	xExt,yExt;			/* size in mapping mode units */
	RECTSIZE OriginalDisplaySize; /* Original display size of metafile in twips */
	DWORD	MetafileSize;		/* Total size of metafile raw data in bytes */
	WORD 	SegCount;			/*	Number of CDPMMETASEG records */
	} CDPMMETAHEADER;

typedef struct
	{
	LSIG	Header;				/* Signature and Length */
	WORD 	DataSize;			/* Actual Size of metafile bits in bytes, ignoring any filler */
	WORD 	SegSize; 			/*  Size of segment, is equal to or larger than DataSize 
									if filler byte added to maintain word boundary */
	/* PM Metafile Bits for this segment. Must be <= 64K bytes. */
	} CDPMMETASEG;

/* MACIntosh Metafile (PICT) */

typedef struct
	{
	LSIG	Header;				/* Signature and Length */
	RECTSIZE OriginalDisplaySize; /* Original display size of metafile in twips */
	DWORD	MetafileSize;		/* Total size of metafile raw data in bytes */
	WORD 	SegCount;			/*	Number of CDMACMETASEG records */
	/*	Metafile segments Follow */
	} CDMACMETAHEADER;

typedef struct
	{
	LSIG	Header;				/* Signature and Length */
	WORD 	DataSize;			/* Actual Size of metafile bits in bytes, ignoring any filler */
	WORD 	SegSize; 			/*  Size of segment, is equal to or larger than DataSize 
									if filler byte added to maintain word boundary */		
	/* Metafile Bits for this segment */
	} CDMACMETASEG;

#define CDIMAGETYPE_GIF		1
#define CDIMAGETYPE_JPEG	2
#define CDIMAGETYPE_BMP		3

typedef struct
	{
	LSIG	Header;				/* Signature and Length */
	WORD 	ImageType;			/* Type of image (e.g., GIF, JPEG) */
	WORD	Width;				/* Width of the image (in pixels) */
	WORD	Height;				/* Height of the image (in pixels) */
	DWORD	ImageDataSize;		/* Size (in bytes) of the image data */
	DWORD	SegCount;			/* Number of CDIMAGESEGMENT records expected to follow */
	DWORD	Flags;				/* Flags (currently unused) */
	DWORD	Reserved;			/* Reserved for future use */
	} CDIMAGEHEADER;

typedef struct
	{
	LSIG	Header;				/* Signature and Length */
	WORD 	DataSize;			/* Actual Size of image bits in bytes, ignoring any filler */
	WORD 	SegSize; 			/*  Size of segment, is equal to or larger than DataSize 
									if filler byte added to maintain word boundary */		
	/* Image bits for this segment */
	} CDIMAGESEGMENT;

typedef struct
	{
	LSIG	Header;				/* Signature and Length */
	WORD 	FileExtLen;			/* Length of file extenstion */
	DWORD	FileDataSize;		/* Size (in bytes) of the file data */
	DWORD	SegCount;			/* Number of CDFILESEGMENT records expected to follow */
	DWORD	Flags;				/* Flags (currently unused) */
	DWORD	Reserved;			/* Reserved for future use */
	/*	Variable length string follows (not null terminated).  This
		string is the file extension for the file. */
	} CDFILEHEADER;

typedef struct
	{
	LSIG	Header;				/* Signature and Length */
	WORD 	DataSize;			/* Actual Size of image bits in bytes, ignoring any filler */
	WORD 	SegSize; 			/*  Size of segment, is equal to or larger than DataSize 
									if filler byte added to maintain word boundary */		
	DWORD	Flags;				/* currently unused, but someday someone will be happy this is here */
	DWORD	Reserved;			/* Reserved for future use */
	/* File bits for this segment */
	} CDFILESEGMENT;


/*	Document Record stored in $INFO field of a document.  This contains
	document-wide attributes. */
	/* for FormFlags */
#define	TPL_FLAG_REFERENCE	0x0001		/* Use Reference Note */
#define	TPL_FLAG_MAIL		0x0002		/* Mail during DocSave */
#define TPL_FLAG_NOTEREF	0x0004		/* Add note ref. to "reference note" */
#define TPL_FLAG_NOTEREF_MAIN 0x0008	/* Add note ref. to main parent of "reference note" */
#define TPL_FLAG_RECALC		0x0010		/* Recalc when leaving fields */
#define TPL_FLAG_BOILERPLATE 0x0020		/* Store form item in with note */
#define	TPL_FLAG_FGCOLOR	0x0040		/* Use foreground color to paint */
#define TPL_FLAG_SPARESOK	0x0080		/* Spare DWORDs have been zeroed */
#define TPL_FLAG_ACTIVATE_OBJECT_COMP	0x0100 /* Activate OLE objects when composing a new doc */	
#define TPL_FLAG_ACTIVATE_OBJECT_EDIT	0x0200 /* Activate OLE objects when editing an existing doc */	
#define TPL_FLAG_ACTIVATE_OBJECT_READ	0x0400 /* Activate OLE objects when reading an existing doc */	
#define TPL_FLAG_SHOW_WINDOW_COMPOSE	0x0800 /* Show Editor window if TPL_FLAG_ACTIVATE_OBJECT_COMPOSE */
#define TPL_FLAG_SHOW_WINDOW_EDIT	0x1000 /* Show Editor window if TPL_FLAG_ACTIVATE_OBJECT_EDIT */
#define TPL_FLAG_SHOW_WINDOW_READ	0x2000 /* Show Editor window if TPL_FLAG_ACTIVATE_OBJECT_READ */
#define	TPL_FLAG_UPDATE_RESPONSE	0x4000	/* V3 Updates become responses */
#define	TPL_FLAG_UPDATE_PARENT		0x8000	/* V3 Updates become parents */
	/* for FormFlags2 */
#define	TPL_FLAG_INCLUDEREF			0x0001	/* insert copy of ref note */
#define	TPL_FLAG_RENDERREF			0x0002	/* render ref (else it's a doclink) */
#define TPL_FLAG_RENDCOLLAPSE		0x0004	/* render it collapsed? */
#define TPL_FLAG_EDITONOPEN			0x0008	/* edit mode on open */
#define TPL_FLAG_OPENCNTXT			0x0010	/* open context panes */
#define TPL_FLAG_CNTXTPARENT		0x0020	/* context pane is parent */
#define TPL_FLAG_MANVCREATE			0x0040	/* manual versioning */
#define TPL_FLAG_UPDATE_SIBLING		0x0080	/* V4 versioning - updates are sibblings */
#define	TPL_FLAG_ANONYMOUS			0x0100	/* V4 Anonymous form */
#define	TPL_FLAG_NAVIG_DOCLINK_IN_PLACE 0x0200	/* Doclink dive into same window */
#define	TPL_FLAG_INTERNOTES			0x0400	/* InterNotes special form */
#define TPL_FLAG_DISABLE_FX			0x0800	/* Disable FX for this doc*/
#define TPL_FLAG_NOMENUS			0x1000	/* Disable menus for this DOC */
#define TPL_FLAG_CHECKDISPLAY		0x2000	/* check display before displaying background */
#define TPL_FLAG_FORMISRTL			0x4000	/* This is a Right To Left Form */
#define TPL_FLAG_HIDEBKGRAPHIC		0x8000	/* hide background graphic in design mode */
	/* for FormFlags3 */
#define	TPL_FLAG_RESIZEHEADER		0x0001	/* editor resizes header area to contents */
#define	TPL_FLAG_NOINITIALFOCUS		0x0002	/* No initial focus to any object on a form or page*/
#define	TPL_FLAG_SIGNWHENSAVED		0x0004	/* Sign this document when it gets saved */
#define	TPL_FLAG_NOFOCUSWHENF6		0x0008	/* No focus when doing F6 or tabbing. */
#define TPL_FLAG_RENDERPASSTHROUGH	0x0010	/* Render pass through HTML in the client. */
#define TPL_FLAG_NOADDFIELDNAMESTOINDEX	0x0020  /* Don't automatically add form fields to field index */

typedef struct {
	BSIG Header;
	WORD PaperColor;			/* Color of the paper being used */
	WORD FormFlags;				/* Form Flags */
	WORD NotePrivileges;		/* Privs for notes created when using form */

	/*	WARNING!!! Fields below this comment were not zeroed in builds
		prior to 100.  A mechanism has been set up to use them however.
		dload checks the TPL_FLAG_SPARESOK bit in the flags word.  If it
		is not set, all of the storage after this comment is zeroed.  On
		save, dsave makes sure the unused storage is zero and sets the bit. */

	WORD FormFlags2;			/* more Form Flags */
	WORD InherFieldNameLength;	/* Length of the name, which follows this struct */

	WORD PaperColorExt;			/* Palette Color of the paper being used. New in V4. */
#define ODS_COLOR_MASK	0x00FF	/* Palette color is an index into a 240 entry table */

	COLOR_VALUE PaperColorValue; /* Paper Color: As of v5.0 stored as RGB, other formats possible */
	WORD FormFlags3;
	WORD Spare[1];
								/* ... now the Inherit Field Name string */
								/* ... now the Text Field Name string indicating
										which field to append version number to */
} CDDOCUMENT;


/*	Header/Footer Record, stored in $HEADER and $FOOTER fields of a
	document.  This contains the header and footer used in the document. */

typedef struct {
	WSIG	Header;
	BYTE	FontPitchAndFamily;
	char	FontName[MAXFACESIZE];
	FONTID	Font;
	WORD	HeadLength;			/* total header string length */
								/* ... now comes the string */
} CDHEADER;

/*	Font Table Record, stored in the $FONTS field of a document.
	This contains the list of "non-standard" fonts used in the
	document. */

typedef struct {
	WSIG	Header;				/* Tag and length */
	WORD	Fonts;				/* Number of CDFACEs following */
} CDFONTTABLE;					/* Now come the CDFACE records... */

typedef struct {
	BYTE Face;					/* ID number of face */
	BYTE Family;				/* Font Family */
	char Name[MAXFACESIZE];
} CDFACE;

typedef struct {
	WSIG	Header;
	WORD	NumberOfEntries;
} CDTEXTPROPERTIESTABLE;

#define MAX_TEXTSTYLE_NAME	128
#define MAX_ISO_LANG_SIZE	8		/*  2-letter ISO Language Code
										'-'
										2-letter ISO Country Code
										'\0'
										+ 1 to make it and even number
									*/

typedef struct {
	WSIG Header;						/* Signature and length of this record */
	char TextStyleName[MAX_TEXTSTYLE_NAME]; /* Text Style Name */
	char LangName[MAX_ISO_LANG_SIZE];					/* Language Tagging information */
	DWORD PropID;
	/* There should be more info here */
	DWORD Flags;
	DWORD Reserved2;
	DWORD Reserved3;
} CDTEXTPROPERTY;


/* Print settings data structure - (stored in desktop file per icon) */

typedef struct {

	WORD Flags;					/* PS_ flags below */

	WORD StartingPageNum;		/* Starting page number */
	WORD TopMargin;				/* Height between main body & top of page (TWIPS) */
	WORD BottomMargin;			/* Height between main body & bottom of page (TWIPS) */
	WORD ExtraLeftMargin;		/* Extra left margin width (TWIPS) */
								/* (beyond whats already specified in document) */
	WORD ExtraRightMargin;		/* Extra right margin width (TWIPS) */
								/* (beyond whats already specified in document) */
	WORD HeaderMargin;			/* Height between header & top of page (TWIPS) */
	WORD FooterMargin;			/* Height between footer & bottom of page (TWIPS) */
	WORD PageWidth;				/* Page width override (TWIPS) */
								/* (0 = "use printer's page width") */
	WORD PageHeight;			/* Page height override (TWIPS) */
								/* (0 = "use printer's page height") */
	WORD BinFirstPage;			/* Index of bin for 1st page */
	WORD BinOtherPage;			/* Index of bin for other pages */
	DWORD spare[3];				/* (spare words) */
	} PRINTNEW_SETTINGS;

typedef struct {

	WORD Flags;
#ifdef LITTLE_ENDIAN_ORDER
	#define PS_Initialized		0x0001		/* Print settings have been initialized */
	#define PS_HeaderFooterOnFirst 0x0002	/* Print header/footer on first page */
	#define PS_CropMarks		0x0004		/* Print crop marks */
	#define PS_ChangeBin		0x0008		/* Paper source should be set for 1st & Other Pg. */
	#define PS_HeaderFooterRTL	0x0010		/* Paper source should be set for 1st & Other Pg. */
	#define PS_ReleaseRightMargin 0x0020	/* Release the right margin when printing (to print into gutter) */
#else
	#define PS_Initialized		0x8000
	#define PS_HeaderFooterOnFirst 0x4000
	#define PS_CropMarks		0x2000
	#define PS_ChangeBin		0x1000
	#define PS_HeaderFooterRTL	0x0800	/* Paper source should be set for 1st & Other Pg. */
	#define PS_ReleaseRightMargin 0x0400	/* Release the right margin when printing (to print into gutter) */
#endif
/*	Note -- The sense of PS_ReleaseRightMargin is inverse so that zero will produce
			R5 behaviour.  PS_ReleaseRightMargin was introduced in R6.  The bit not
			being set means "do release right margin" (like R5 always does).  If it
			is set, we don't release the right margin, ie. we clip to right margin.    */


	WORD StartingPageNum;		/* Starting page number */
	WORD TopMargin;				/* Height between main body & top of page (TWIPS) */
	WORD BottomMargin;			/* Height between main body & bottom of page (TWIPS) */
	WORD ExtraLeftMargin;		/* Extra left margin width (TWIPS) */
								/* (beyond whats already specified in document) */
	WORD ExtraRightMargin;		/* Extra right margin width (TWIPS) */
								/* (beyond whats already specified in document) */
	WORD HeaderMargin;			/* Height between header & top of page (TWIPS) */
	WORD FooterMargin;			/* Height between footer & bottom of page (TWIPS) */
	WORD PageWidth;				/* Page width override (TWIPS) */
								/* (0 = "use printer's page width") */
	WORD PageHeight;			/* Page height override (TWIPS) */
								/* (0 = "use printer's page height") */
	WORD BinFirstPage;
	WORD BinOtherPage;
	WORD PageOrientation;		/* New in R6.  0 = undefined, 1 = Portrait, 2 = Landscape */
#define PSPO_UNDEFINED 0x0000
#define PSPO_PORTRAIT  0x0001
#define PSPO_LANDSCAPE 0x0002
	WORD wSpare;				/* Spare word */
	DWORD spare[2];				/* (spare dwords) */
	} PRINT_SETTINGS;

	
	
/*	Header/Footer data structure - passed into import/export modules */

typedef struct {
	BYTE	FontPitchAndFamily;
	char	FontName[MAXFACESIZE];
	FONTID	Font;
	WORD	HeadLength;			/* string length not including '\0' */
								/* Header string (ASCIIZ) follows */
} HEAD_DESC;

#define	MAXHEADERSTRING 256		/* maximum header string size */
typedef struct {				/* used for stack-local ones */
	HEAD_DESC Desc;
	char String[MAXHEADERSTRING]; /* Must be terminated by '\0' */
} HEAD_DESC_BUFFER;



/*	DDE composite data On Disk structures */

#define DDESERVERNAMEMAX	32
#define DDEITEMNAMEMAX		64
#define DDESERVERCOMMANDMAX	256

typedef struct
	{
	WSIG Header;						/* Signature and length of this record */
	char ServerName[DDESERVERNAMEMAX];  /* Null terminated server name */
	char TopicName[100];				/* Null terminated DDE Topic (usually a file name) */
	char ItemName[DDEITEMNAMEMAX];		/* Null terminated Place reference string */
	DWORD Flags;						/* See DDEFLAGS_xxx flag definitions below */
	char PasteEmbedDocName[80];			/*  only used on when making new link during Paste Special */
	WORD EmbeddedDocCount;				/* Number of embedded docs for this link */
										/* (MUST BE 0 or 1) */
	WORD ClipFormat;					/* Clipboard format with which data should be rendered */
										/* (DDEFORMAT_xxx defined below) */
	/* Null terminated embedded document name which is attached to the note follows.. */
	}CDDDEBEGIN;

/*	CDDDEBEGIN flags */

#define DDEFLAGS_AUTOLINK		0x01	/* Link type == Automatic (hot) */
#define DDEFLAGS_MANUALLINK		0x02	/* Link type == Manual (warm) */
#define DDEFLAGS_EMBEDDED		0x04	/* Embedded document exists */
#define DDEFLAGS_INITIATE		0x08	/* Used on paste to indicate not to
										   prompt user to initiate link */	
#define DDEFLAGS_CDP			0x10	/* Used on paste to indicate that
										   server uses Compound Document
										   protocol */
#define DDEFLAGS_NOTES_LAUNCHED 0x20	/* Used on CDP paste/load to indicate that
											Notes lauched app to establish
											original conversation. */

#define DDEFLAGS_CONV_ACTIVE 	0x40	/* Used on non-CDP paste/load to indicate that
										   conversation is already active 
										   for this embedded doc */
#define DDEFLAGS_EMBEDEXTRACTED	0x80	/* Used on non-CDP paste/load to indicate
											that Notes extracted the embedded
											file so that we may later close
											it */

#define DDEFLAGS_NEWOBJECT		0x100	/*	Set if this DDE Range is a new
											inserted object which contains
											no embedded object yet, i.e. a
											"blank" object */											
		
/*	These remappings of Native clipboard formats are used because we can't
	use Windows or PM constants because they are different */

#define DDEFORMAT_TEXT			0x01	/* CF_TEXT */
#define DDEFORMAT_METAFILE		0x02	/* CF_METAFILE or CF_METAFILEPICT */
#define DDEFORMAT_BITMAP		0x03	/* CF_BITMAP */
#define DDEFORMAT_RTF			0x04	/* Rich Text Format */
#define DDEFORMAT_OWNERLINK		0x06	/* OLE Ownerlink (never saved in CD_DDE or CD_OLE: used at run time) */
#define DDEFORMAT_OBJECTLINK	0x07	/* OLE Objectlink (never saved in CD_DDE or CD_OLE: used at run time) */
#define DDEFORMAT_NATIVE		0x08	/* OLE Native (never saved in CD_DDE or CD_OLE: used at run time) */
#define DDEFORMAT_ICON			0x09	/* Program Icon for embedded object */

/*	Total number of DDE format types supported.  Increment this if 
	one is added above */

#define DDEFORMAT_TYPES			5


typedef struct
	{
	WSIG Header;	/* Signature and length of this record */	
	DWORD Flags;	/* Currently unused, but reserve some flags */
	}CDDDEEND;


/*	On-disk format for an OLE object.  Both Links and
	embedded objects actually have an attached  $FILE "object"
	which is the variable length portion of the data which follows
	the CDOLEBEGIN record. */

typedef struct
	{
	WSIG Header;					/* Signature and length of this record */
	WORD Version;					/* Notes OLE implementation version */
	DWORD Flags;					/* See OLEREC_FLAG_xxx flag definitions below */
	WORD ClipFormat;				/* Clipboard format with which data should be rendered */
									/* (DDEFORMAT_xxx defined above) */
	WORD AttachNameLength;			/* Attached file name length */
	WORD ClassNameLength;			/* Length of Classname */
	WORD TemplateNameLength;		/* User during Insert New Object, but never saved to disk */
	/* The Attachment Name (length "AttachNameLength") always follows... */
	/* The Classname, optional, then follows... */
	/* The Template Name, optional, then follows... */
	} CDOLEBEGIN;

typedef struct
	{
	WSIG Header;	/* Signature and length of this record */	
	DWORD Flags;	/* Currently unused, but reserve some flags */
	}CDOLEEND;

/*	Current OLE Version */

#define NOTES_OLEVERSION1	1
#define NOTES_OLEVERSION2	2

#define	OLEREC_FLAG_OBJECT		0x01	/* The data is an OLE embedded OBJECT */
#define OLEREC_FLAG_LINK		0x02    /* The data is an OLE Link */
#define OLEREC_FLAG_AUTOLINK	0x04	/* If link, Link type == Automatic (hot) */
#define OLEREC_FLAG_MANUALLINK	0x08	/* If link, Link type == Manual (warm) */
#define OLEREC_FLAG_NEWOBJECT	0x10	/* New object, just inserted */
#define OLEREC_FLAG_PASTED		0x20	/* New object, just pasted */
#define OLEREC_FLAG_SAVEOBJWHENCHANGED 0x40 /* Object came from form and should be saved 
											every time it changes in server */
#define OLEREC_FLAG_NOVISUALIZE	0x80	/* Object inherited from form, so don't visualize or
										   object incabable of rendering itself. */ 

/*	On-disk format for an OLE object's rich text marker.  Marker is always
	within an OLE rich text hot spot.  Marker was introduced in Notes 4.6.
	Previous versions of Notes ignor this marker on load and do not write it
	out on save.  The absence of this marker within the hot spot indicates 
	that the OLE rich text was accessed on a system running a pre-4.6 version
	of Notes.  */

typedef struct
	{
	WSIG	Header;				/* Signature and length of this record */
	DWORD	Flags;				/* (reserved) */
	} CDOLERTMARKER;

/*	On-disk format for an OLE object's rich text hot spot data.  This data
	is internal to the hot spot and is treated by the hot spot as a blob of
	data.  It is not a CD record type */

typedef struct
	{
	WSIG	Header;				/* Signature and length of this record */
	DWORD	Flags;				/* (reserved) */
	WORD	cFileObjName;		/* Length of extendable $FILE object name which contains object data */
	WORD	Reserved1;			/* Unused, must be 0 */
	WORD	Reserved2;			/* Unused, must be 0 */
	WORD	Reserved3;			/* Unused, must be 0 */
	DWORD	Reserved4;			/* Unused, must be 0 */
	/* The variable length portions go here in the following order:
	   FileObjectName
	*/
	} HSOLERICHTEXT;

#define OLERT_FLAG_CHANGED	0x01	/* Rich text was changed */
#define OLERT_FLAG_NEW		0x02

/*	On-disk format for HotSpots. */

/*	HOTSPOT_RUN Types */

#define HOTSPOTREC_TYPE_POPUP			1
#define HOTSPOTREC_TYPE_HOTREGION		2
#define HOTSPOTREC_TYPE_BUTTON			3
#define HOTSPOTREC_TYPE_FILE			4
#define HOTSPOTREC_TYPE_SECTION			7
#define HOTSPOTREC_TYPE_ANY				8
#define HOTSPOTREC_TYPE_HOTLINK			11
#define HOTSPOTREC_TYPE_BUNDLE			12
#define HOTSPOTREC_TYPE_V4_SECTION		13
#define HOTSPOTREC_TYPE_SUBFORM			14
#define HOTSPOTREC_TYPE_ACTIVEOBJECT	15
#define HOTSPOTREC_TYPE_OLERICHTEXT		18
#define HOTSPOTREC_TYPE_EMBEDDEDVIEW	19	/* embedded view */
#define HOTSPOTREC_TYPE_EMBEDDEDFPANE	20	/* embedded folder pane */
#define HOTSPOTREC_TYPE_EMBEDDEDNAV		21	/* embedded navigator */
#define HOTSPOTREC_TYPE_MOUSEOVER		22
#define HOTSPOTREC_TYPE_FILEUPLOAD		24	/* file upload placeholder */
#define HOTSPOTREC_TYPE_EMBEDDEDOUTLINE 27	/* embedded outline */
#define HOTSPOTREC_TYPE_EMBEDDEDCTL		28	/* embedded control window */
#define HOTSPOTREC_TYPE_EMBEDDEDCALENDARCTL 30	/* embedded calendar control (date picker) */
#define HOTSPOTREC_TYPE_EMBEDDEDSCHEDCTL 31	/* embedded scheduling control */
#define HOTSPOTREC_TYPE_RCLINK			32	/* Not a new type, but renamed for V5 terms*/
#define HOTSPOTREC_TYPE_EMBEDDEDEDITCTL 34	/* embedded editor control */
#define	HOTSPOTREC_TYPE_CONTACTLISTCTL	36	/* Embeddeble buddy list */

#define HOTSPOTREC_RUNFLAG_BEGIN		0x00000001L
#define HOTSPOTREC_RUNFLAG_END			0x00000002L
#define HOTSPOTREC_RUNFLAG_BOX			0x00000004L
#define HOTSPOTREC_RUNFLAG_NOBORDER		0x00000008L
#define HOTSPOTREC_RUNFLAG_FORMULA		0x00000010L	/*	Popup is a formula, not text. */
#define HOTSPOTREC_RUNFLAG_MOVIE		0x00000020L /*	File is a QuickTime movie. */
#define HOTSPOTREC_RUNFLAG_IGNORE		0x00000040L /*	Run is for backward compatibility
														(i.e. ignore the run)
													*/
#define HOTSPOTREC_RUNFLAG_ACTION		0x00000080L	/*	Hot region executes a canned action	*/
#define HOTSPOTREC_RUNFLAG_SCRIPT		0x00000100L	/*	Hot region executes a script.	*/
#define HOTSPOTREC_RUNFLAG_INOTES  		0x00001000L
#define HOTSPOTREC_RUNFLAG_ISMAP    	0x00002000L
#define HOTSPOTREC_RUNFLAG_INOTES_AUTO	0x00004000L
#define HOTSPOTREC_RUNFLAG_ISMAP_INPUT	0x00008000L

#define HOTSPOTREC_RUNFLAG_SIGNED  		0x00010000L
#define HOTSPOTREC_RUNFLAG_ANCHOR  		0x00020000L
#define HOTSPOTREC_RUNFLAG_COMPUTED		0x00040000L	/*	Used in conjunction
														with computed hotspots.
													*/
#define HOTSPOTREC_RUNFLAG_TEMPLATE		0x00080000L	/*	used in conjunction
														with embedded navigator
														panes.
													*/
#define HOTSPOTREC_RUNFLAG_HIGHLIGHT  	0x00100000L
#define HOTSPOTREC_RUNFLAG_EXTACTION	0x00200000L /*  Hot region executes an extended action */
#define HOTSPOTREC_RUNFLAG_NAMEDELEM	0x00400000L	/*	Hot link to a named element */

/*	Allow R6 dual action type buttons, e.g. client LotusScript, web JS */
#define HOTSPOTREC_RUNFLAG_WEBJAVASCRIPT	0x00800000L

#define HOTSPOTREC_RUNFLAG_ODSMASK		0x00FFFFFCL	/*	Mask for bits stored on disk*/


typedef struct CDHOTSPOTBEGIN
	{
	WSIG Header;	/* Signature and length of this record */	
	WORD Type;
	DWORD Flags;
	WORD DataLength;
	/*	Data Follows. */
	/*  if HOTSPOTREC_RUNFLAG_SIGNED, WORD SigLen then SigData follows. */
	} CDHOTSPOTBEGIN;

typedef struct
	{
	BSIG Header;	/* Signature and length of this record */	
	} CDHOTSPOTEND;

/*	On-disk flags for CDBAR (Collapsible Sections)	*/

#define BARREC_DISABLED_FOR_NON_EDITORS 	1L
#define BARREC_EXPANDED 					2L
#define BARREC_PREVIEW 						4L

#define BARREC_BORDER_INVISIBLE 			0x1000L
#define BARREC_ISFORMULA					0x2000L
#define BARREC_HIDE_EXPANDED				0x4000L

/* Auto expand/collapse properties.	*/

#define BARREC_AUTO_EXP_READ	0x10L
#define BARREC_AUTO_EXP_PRE		0x20L
#define BARREC_AUTO_EXP_EDIT	0x40L
#define BARREC_AUTO_EXP_PRINT	0x80L

#define BARREC_AUTO_EXP_MASK	0xf0L

#define BARREC_AUTO_COL_READ	0x100L
#define BARREC_AUTO_COL_PRE		0x200L
#define BARREC_AUTO_COL_EDIT	0x400L
#define BARREC_AUTO_COL_PRINT	0x800L
#define BARREC_AUTO_COL_MASK	0xF00L

#define BARREC_AUTO_PRE_MASK	(BARREC_AUTO_COL_PRE | BARREC_AUTO_EXP_PRE)
#define BARREC_AUTO_READ_MASK	(BARREC_AUTO_COL_READ | BARREC_AUTO_EXP_READ)
#define BARREC_AUTO_EDIT_MASK	(BARREC_AUTO_COL_EDIT | BARREC_AUTO_EXP_EDIT)
#define BARREC_AUTO_PRINT_MASK	(BARREC_AUTO_COL_PRINT | BARREC_AUTO_EXP_PRINT)

/*	We will make use (in the code) of the fact that the auto expand/collapse
	flags for editors are simply shifted left twelve bits from the normal
	expand/collapse flags.
*/

#define BARREC_AUTO_ED_SHIFT		12

#define BARREC_AUTO_ED_EXP_READ		0x10000L
#define BARREC_AUTO_ED_EXP_PRE		0x20000L
#define BARREC_AUTO_ED_EXP_EDIT		0x40000L
#define BARREC_AUTO_ED_EXP_PRINT	0x80000L

#define BARREC_AUTO_ED_EXP_MASK		0xf00000L

#define BARREC_AUTO_ED_COL_READ		0x100000L
#define BARREC_AUTO_ED_COL_PRE		0x200000L
#define BARREC_AUTO_ED_COL_EDIT		0x400000L
#define BARREC_AUTO_ED_COL_PRINT	0x800000L
#define BARREC_AUTO_ED_COL_MASK		0xF00000L

#define BARREC_AUTO_ED_PRE_MASK		(BARREC_AUTO_ED_COL_PRE | BARREC_AUTO_ED_EXP_PRE)
#define BARREC_AUTO_ED_READ_MASK	(BARREC_AUTO_ED_COL_READ | BARREC_AUTO_ED_EXP_READ)
#define BARREC_AUTO_ED_EDIT_MASK	(BARREC_AUTO_ED_COL_EDIT | BARREC_AUTO_ED_EXP_EDIT)
#define BARREC_AUTO_ED_PRINT_MASK	(BARREC_AUTO_ED_COL_PRINT | BARREC_AUTO_ED_EXP_PRINT)

#define BARREC_INTENDED				0x1000000L
#define BARREC_HAS_COLOR			0x4000000L

#define BARREC_BORDER_MASK			0x70000000L
#define GETBORDERTYPE(a) ((DWORD)((a) & BARREC_BORDER_MASK) >> 28)
#define SETBORDERTYPE(a,b) a = ((DWORD)((a) & ~BARREC_BORDER_MASK) | ((DWORD)(b) << 28))

#define	BARREC_BORDER_SHADOW		0
#define BARREC_BORDER_NONE			1
#define BARREC_BORDER_SINGLE		2
#define	BARREC_BORDER_DOUBLE		3
#define BARREC_BORDER_TRIPLE		4
#define	BARREC_BORDER_TWOLINE		5
#define	BARREC_BORDER_WINDOWCAPTION	6
#define	BARREC_BORDER_OTHER			7
#define	BARREC_BORDER_GRADIENT		7
#define	BARREC_BORDER_TAB			8
#define	BARREC_BORDER_DIAG			9

#define BARREC_INDENTED		0x80000000L

/*	Indicate explicitly those bits that we want to save on-disk
	so that we insure that the others are zero when we save to
	disk so that we can use later.
*/
 
#define BARREC_ODS_MASK 0xF4FF6FF7

/*	On-disk format for Object Bars.	*/

#define BARREC_IS_EXPANDED			0x0001
#define BARREC_IS_DISABLED			0x0002

#define BAR_VERSION1    0

/* Some more border bits put out in extra data thru a CDDATAFLAGS record */

#define BAR_DATA_BORDER_OFFSET		7
#define BARREC_DATA_BORDER_MASK		0x0000007L

#define GETDATABORDERTYPE(a) (((DWORD)((a) & BARREC_DATA_BORDER_MASK )) + BAR_DATA_BORDER_OFFSET)
#define SETDATABORDERTYPE(a,b) a = ((DWORD)((a) & ~BARREC_DATA_BORDER_MASK) | ((DWORD)(b - BAR_DATA_BORDER_OFFSET)))

#define	BARREC_DATA_BORDER_GRADIENT		0	
#define	BARREC_DATA_BORDER_TAB			1	
#define	BARREC_DATA_BORDER_DIAG			2



typedef struct
	{
	WSIG	Header;
	DWORD	Flags;
	FONTID	FontID;
	/*	Caption and name follow	*/
	} CDBAR;

/*	On-disk format for Buttons.	*/

/*	Button-specific run information. */

#define BUTTON_UNUSED			0x0000
#define BUTTON_RUNFLAG_SCRIPT	0x0001
#define BUTTON_RUNFLAG_NOWRAP	0x0002
#define BUTTON_ODS_MASK			0x7F02

#define BUTTON_RUNFLAG_RTL			0x0100
#define BUTTON_RUNFLAG_FIXED		0x0200
#define BUTTON_RUNFLAG_MINIMUM		0x0400
#define BUTTON_RUNFLAG_CONTENT		0x0800
#define BUTTON_RUNFLAG_PROPORTIONAL	0x4000
#define BUTTON_FOCUS_ON				0x8000	/* button has focus	*/

#define BUTTON_RUNFLAG_WIDTH_MASK (BUTTON_RUNFLAG_FIXED | BUTTON_RUNFLAG_MINIMUM | BUTTON_RUNFLAG_CONTENT | BUTTON_RUNFLAG_PROPORTIONAL)

#define BUTTON_EDGE_ROUNDED			0x1000
#define BUTTON_EDGE_SQUARE			0x2000


typedef struct
	{
	WSIG 	Header;	/* Signature and length of this record. */
	WORD 	Flags;	
	WORD 	Width;
	WORD 	Height;
	WORD	Lines;
	FONTID 	FontID;
	/* Button Text Follows	*/
	} CDBUTTON;

#define BUTTON_VERSION1    0

#define BUTTON_TYPE_NORMAL			0x00000000
#define BUTTON_TYPE_OK				0x00000001
#define BUTTON_TYPE_CANCEL			0x00000002
#define BUTTON_TYPE_HELP			0x00000004
#define BUTTON_TYPE_MASK	( BUTTON_TYPE_OK | BUTTON_TYPE_CANCEL | BUTTON_TYPE_HELP)
/* byte 0x000000*0 reserved	- danv	*/
#define BUTTON_SPEC_NORMAL			0x00000000
#define BUTTON_SPEC_DEFAULT			0x00000100	/* button with this attribute is the def push button in a dialog box	*/
#define BUTTON_SPEC_MASK	( BUTTON_SPEC_DEFAULT )


/*	On-disk format for Horizontal Rule objects */

#define HRULE_FLAG_USECOLOR			0x00000001
#define HRULE_FLAG_USEGRADIENT		0x00000002
#define HRULE_FLAG_FITTOWINDOW		0x00000004
#define HRULE_FLAG_NOSHADOW			0x00000008

typedef struct
	{
	WSIG 	Header;	/* Signature and length of this record. */
	DWORD 	Flags;	
	WORD 	Width;
	WORD 	Height;
	WORD 	Color;
	WORD 	GradientColor;
	} CDHRULE;

#define CDHORIZONTALRULE_VERSION1	1	/* create by notes v5.0 */


/*	On-disk format for PlaceHolder objects */

#define PLACEHOLDER_FLAG_FITTOWINDOW		0x00000001
#define PLACEHOLDER_FLAG_DRAWBACKGROUND		0x00000002
#define PLACEHOLDER_FLAG_USEPERCENTAGE		0x00000004
#define PLACEHOLDER_FLAG_SCROLLBARS			0x00000008
#define PLACEHOLDER_FLAG_CONTENTSONLY		0x00000010
#define PLACEHOLDER_FLAG_ALIGNCENTER		0x00000020
#define PLACEHOLDER_FLAG_ALIGNRIGHT			0x00000040
#define PLACEHOLDER_FLAG_FITTOWINDOWHEIGHT	0x00000080
#define PLACEHOLDER_FLAG_TILEIMAGE			0x00000100
#define PLACEHOLDER_FLAG_DISPLAYHORZ		0x00000200
#define PLACEHOLDER_FLAG_DONTEXPANDSELECTIONS	0x00000400
#define PLACEHOLDER_FLAG_EXPANDCURRENT		0x00000800
#define PLACEHOLDER_FLAG_FITCONTENTSWIDTH	0x00001000
#define PLACEHOLDER_FLAG_FIXEDWIDTH			0x00002000
#define PLACEHOLDER_FLAG_FIXEDHEIGHT		0x00004000
#define PLACEHOLDER_FLAG_FITCONTENTS		0x00008000
#define PLACEHOLDER_FLAG_PROP_WIDTH			0x00010000
#define PLACEHOLDER_FLAG_PROP_BOTH			0x00020000
#define PLACEHOLDER_FLAG_SCROLLERS		0x00040000

#define PLACEHOLDER_ALIGN_LEFT		0
#define PLACEHOLDER_ALIGN_CENTER	1
#define PLACEHOLDER_ALIGN_RIGHT		2

typedef struct
	{
	WSIG 	Header;	/* Signature and length of this record. */
	WORD	Type;
	DWORD 	Flags;	
	WORD 	Width;
	WORD 	Height;
	FONTID  FontID;
	WORD	Characters;
	WORD	SpaceBetween;
	WORD	TextAlignment;
	WORD	SpaceWord;
	FONTID	SubFontID[2];
	WORD 	DataLength;
	COLOR_VALUE BackgroundColor;
	COLOR_VALUE ColorRGB;
	WORD	SpareWord;
	DWORD	Spare[3];
	} CDPLACEHOLDER;

#define EMBEDDEDOUTLINE_FLAG_DISPLAYHORZ			0x00000001
#define EMBEDDEDOUTLINE_FLAG_HASIMAGELABEL			0x00000002
#define EMBEDDEDOUTLINE_FLAG_TILEIMAGE				0x00000004
#define EMBEDDEDOUTLINE_FLAG_USEAPPLET_INBROWSER	0x00000008
#define EMBEDDEDOUTLINE_FLAG_TYPE_TITLE				0x00000010
#define EMBEDDEDOUTLINE_FLAG_SHOWTWISTIE			0x00000020
#define EMBEDDEDOUTLINE_FLAG_TITLEFIXED				0x00000040
#define EMBEDDEDOUTLINE_FLAG_TOPLEVELFIXED			0x00000080
#define EMBEDDEDOUTLINE_FLAG_SUBLEVELFIXED			0x00000100
#define EMBEDDEDOUTLINE_FLAG_TREE_STYLE				0x00000200
#define EMBEDDEDOUTLINE_FLAG_HASNAME				0x00000400
#define EMBEDDEDOUTLINE_FLAG_HASTARGETFRAME			0x00000800
#define EMBEDDEDOUTLINE_FLAG_ALLTHESAME				0x00001000
#define EMBEDDEDOUTLINE_FLAG_BACK_ALLTHESAME		0x00002000
#define EMBEDDEDOUTLINE_FLAG_EXPAND_DATA			0x00004000
#define EMBEDDEDOUTLINE_FLAG_EXPAND_ALL				0x00008000
#define EMBEDDEDOUTLINE_FLAG_EXPAND_FIRST			0x00010000
#define EMBEDDEDOUTLINE_FLAG_EXPAND_SAVED			0x00020000
#define EMBEDDEDOUTLINE_FLAG_EXPAND_NONE			0x00040000
#define EMBEDDEDOUTLINE_FLAG_HASROOTNAME			0x00080000
#define EMBEDDEDOUTLINE_FLAG_RTLREADING				0x00100000
#define EMBEDDEDOUTLINE_FLAG_TWISTIEIMAGE			0x00200000
#define EMBEDDEDOUTLINE_FLAG_HANDLEFOLDERUNREAD		0x00400000
#define EMBEDDEDOUTLINE_FLAG_NEWSTYLE_TWISTIE		0x00800000

/*	defines for Background Color Offset for title and each level - 0 relative*/
#define TITLE_BACK_COLOR_OFFSET				0
#define MAINLEVEL_BACK_COLOR_OFFSET			3
#define SUBLEVEL_BACK_COLOR_OFFSET			6

/* 	defines for back color offset for each type */
#define NORMAL_BACK_COLOR					0
#define SELECTED_BACK_COLOR					1
#define MOUSE_OVER_BACK_COLOR				2

/* defines for entry/background image alignment */
#define ALIGNMENT_TOPLEFT		0
#define ALIGNMENT_TOPCENTER		1
#define ALIGNMENT_TOPRIGHT		2
#define ALIGNMENT_MIDDLELEFT	3
#define ALIGNMENT_MIDDLECENTER	4
#define ALIGNMENT_MIDDLERIGHT	5
#define ALIGNMENT_BOTTOMLEFT	6
#define ALIGNMENT_BOTTOMCENTER	7
#define ALIGNMENT_BOTTOMRIGHT	8

/*	CDRESOURCE for background images follow the following cd record */

typedef struct
	{
	WSIG 	Header;	/* Signature and length of this record. */
	DWORD 	Flags;	
	DWORD	Unused[3];
	WORD 	Alignment;
	WORD	SpaceBetweenEntries;
	WORD	LabelLength;	
	WORD	Style;
	WORD	Title_VOffset;
	WORD	Title_HOffset;
	WORD	Title_Height;
	WORD	TopLevel_VOffset;
	WORD	TopLevel_HOffset;
	WORD	TopLevel_Height;
	WORD	SubLevel_VOffset;
	WORD	SubLevel_HOffset;
	WORD	SubLevel_Height;
	WORD	NameLength;
	WORD	TargetFrameLength;
	FONTID	SelectFontID[3];
	FONTID	MouseFontID[3];
	WORD	Font_VOffset[3];
	WORD	Font_HOffset[3];
	WORD	Align[3];
	COLOR_VALUE Control_BackColor;
	COLOR_VALUE	BackColor[9];
	COLOR_VALUE SelectFontColor[3];
	WORD	Repeat[4];
	WORD	Background_Align[4];
	WORD	Background_VOffset[4];
	WORD	Background_HOffset[4];
	WORD	wBackground_Image[4];
	COLOR_VALUE NormalFontColor[3];
	COLOR_VALUE MouseFontColor[3];
	WORD	RootLength;
	WORD	TopLevel_PixelHeight;
	WORD	wColWidth;
	WORD	SpareWord;
	DWORD	Spare[4];
	} CDEMBEDDEDOUTLINE;

/* USE_COLORS1 indicates that color values for Busytime, freetime, nodata,
   datarestricted and gridline contain valid information.  Default color
   values are used if this flag is not set */
#define EMBEDDEDSCHED_FLAG_USE_COLORS1				0x00000001
#define EMBEDDEDSCHED_FLAG_NO_INITFROMITEMS			0x00000002
#define EMBEDDEDSCHED_FLAG_NO_REFRESHFROMITEMS		0x00000004
#define EMBEDDEDSCHED_FLAG_ALLOW_FILTERING			0x00000008
#define EMBEDDEDSCHED_FLAG_USE_COLORS2				0x00000008

#define EMBEDDEDSCHED_FLAG_NO_SHOWLEGEND			0x00000010
#define EMBEDDEDSCHED_FLAG_SHOWINTERVALINDICATOR		0x00000020

#define EMBEDDEDSCHED_FLAG_SHOW_TWISTIES			0x00000040
#define EMBEDDEDSCHED_FLAG_ALLOW_EDIT_INPLACE		0x00000080

#define EMBEDDEDSCHED_FLAG_ATTENDEE_WIDTH_DEFINED 	0x00000100
#define EMBEDDEDSCHED_FLAG_ATTENDEE_WIDTH_FIXED		0x00000200

#define EMBEDDEDSCHED_FLAG_PEOPLE_INVISIBLE			0x00000400
#define EMBEDDEDSCHED_FLAG_ROOMS_VISIBLE			0x00000800
#define EMBEDDEDSCHED_FLAG_RESOURCES_VISIBLE		0x00001000

#define EMBEDDEDSCHED_FLAG_PEOPLE_FIXEDHEIGHT		0x00002000
#define EMBEDDEDSCHED_FLAG_ROOMS_FIXEDHEIGHT		0x00004000
#define EMBEDDEDSCHED_FLAG_RESOURCES_FIXEDHEIGHT	0x00008000
#define EMBEDDEDSCHED_FLAG_ATTENDEE_LINES_DEFINED	0x00010000   /* set to true if PeopleLines, RoomsLines and ResourcesLines
                                                                    values in CDEMBEDDEDSCHEDCTL contain valid data */
#define EMBEDDEDSCHED_FLAG_RTL_READING				0x00020000	/* Display schedule from right to left */
#define EMBEDDEDSCHED_FLAG_NO_LAUNCH				0x00040000  /* if TRUE, don't launch scheduler */

typedef struct
	{
	WSIG 		Header;		/* Signature and length of this record. */
	DWORD 		Flags;		/* EMBEDDEDSCHED_FLAG_xxx from above */
	WORD 		TargetFrameLength;
	WORD		DisplayStartDTItemFormulaLength;
	WORD 		HrsPerDayItemFormulaLength;
	WORD		ReqPeopleItemsFormulaLength;
	COLOR_VALUE	BusyTimeColor;	/* Color used for busy times (due to calendar entry) */
	COLOR_VALUE FreeTimeColor;	/* Color used for non-busy times */
	COLOR_VALUE	NoDataColor;	/* Color used for no busy time data found */
	COLOR_VALUE DataRestrictedColor;	/* Color used for busy time data restricted */
	COLOR_VALUE GridLineColor;	/* Color used to paint grid lines in scheduler */

	WORD		NameColWidth;	/* width of the left side of the control where attendee names are displayed */
	WORD		PeopleLines;	/* Max items in People area if EMBEDDEDSCHED_FLAG_PEOPLE_FIXEDHEIGHT */
	WORD		RoomsLines;		/* Max items in Rooms area if EMBEDDEDSCHED_FLAG_ROOMS_FIXEDHEIGHT */
	WORD		ResourcesLines;	/* Max items in Resources area if EMBEDDEDSCHED_RESOURCES_PEOPLE_FIXEDHEIGHT */

	WORD 		SpareWORD[5];
	DWORD		SpareDWORD[13];

	/* Target Frame Name (if any) */
	/* DisplayStartDTItem formula (if any) */
	/* HrsPerDayItem formula (if any) */
	/* RequiredPeopleItems formula (if any) */
	} CDEMBEDDEDSCHEDCTL;


#define EMBEDDEDSCHEDEXT_FLAG_PEOPLE_TITLE_FORMULA		0x00000001	/* if the people title is a formula, set this flag */
#define EMBEDDEDSCHEDEXT_FLAG_ROOMS_TITLE_FORMULA		0x00000002  /* if the rooms title is a formula, set this flag */
#define EMBEDDEDSCHEDEXT_FLAG_RESOURCES_TITLE_FORMULA	0x00000004  /* if the resources title is a formula, set this flag */
#define EMBEDDEDSCHEDEXT_FLAG_SUGG_COLORS_DEFINED		0x00000008	/* if the designer has specified background colors for the suggestion OD list, set this flag */

typedef struct
	{
	WSIG		Header;
	WORD		FixedPartLength;		/* bytes of this structure */
	DWORD		Flags;					/* EMBEDDEDSCHEDEXT_FLAG_xxx from above */

	/* Colors used for the scheduler ODLists */

	COLOR_VALUE	SchedHdrBkgndColor;		/* Bkgnd color for the Scheduler ODList headers */
	COLOR_VALUE SchedHdrFontColor;		/* Font color for the Scheduler ODList headers */

	COLOR_VALUE	SchedBkgndColor;   		/* Bkgnd color for the scheduler ODList */

	COLOR_VALUE	NameHdrBkgndColor;		/* Bkgnd color for name ODList header area*/
	COLOR_VALUE NameHdrFontColor;		/* Font color for name ODList header area */

	COLOR_VALUE	NameBkgndColor;		/* Normal bkgnd color for attendee names */
	COLOR_VALUE	NameMouseBkgndColor;	/* Mouse over bkgnd color for attendee names */
	COLOR_VALUE	NameSelectBkgndColor;	/* Select bkgnd color for attendee names */

	COLOR_VALUE	NameFontColor;			/* Normal font color for attendee names */
	COLOR_VALUE	NameMouseFontColor;		/* Mouse over font color for attendee names */
	COLOR_VALUE	NameSelectFontColor;	/* Select font color for attendee names */

	/* Formula & text lengths */
	WORD		OptPeopleItemsFormulaLength;
	WORD		ReqRoomsItemsFormulaLength;
	WORD		OptRoomsItemsFormulaLength;
	WORD		ReqResourcesItemsFormulaLength;
	WORD		OptResourcesItemsFormulaLength;
	WORD		IntervalStartDTItemFormulaLength;
	WORD		IntervalEndDTItemFormulaLength;
	WORD		SchedulerNameLength;
	WORD		PeopleTitleLength;
	WORD		RoomsTitleLength;
	WORD		ResourcesTitleLength;
	WORD		IntervalChangeEventFormulaLength;
	COLOR_VALUE ProfileColor;	/* Color used to paint busy times (due to freetime availability from calendar profile) */
	WORD		SchedDetailItemsFormulaLength;

	COLOR_VALUE	SuggBkgndColor;			/* bkgnd colors for suggestions odlist */
	COLOR_VALUE SuggMouseBkgndColor;
	COLOR_VALUE SuggSelectBkgndColor;

	WORD		DetailDisplayFormFormulaLength;

	WORD		wReserved[1];
	DWORD		dwReserved[7];			/* Reserved for future use, must be zero */

	/* Variable length data follows if specified */
	/* OptionalPeopleItems formula (if any) */
	/* RequiredRoomsItems formula (if any) */
	/* OptionalRoomsItems formula (if any) */
	/* RequiredResourcesItems formula (if any) */
	/* OptionalResourcesItems formula (if any) */
	/* IntervalStartDT formula (if any) */
	/* IntervalEndDT formula (if any) */
	/* SchedulerName (if any) */
	/* People Title (if any) */
	/* Rooms Title (if any) */
	/* Resources Title (if any) */
	/* Appointment Interval Change Event Formula (if any) */
	/* SchedDetailItemsFormula (if any) */
	/* DetailDisplayFormFormula (if any) */
	} CDEMBEDDEDSCHEDCTLEXTRA;

#define	EMBEDDEDEDITCTL_FLAG_HASNAME 		0x0001
#define EMBEDDEDEDITCTL_FLAG_HIDE_ACTIONBAR	0x0004

typedef struct
	{
	WSIG 		Header;	/* Signature and length of this record. */
	DWORD 		Flags;
	WORD		NameLength;

	WORD 		SpareWORD[5];
	DWORD		SpareDWORD[11];

	} CDEMBEDDEDEDITCTL;

#define EMBEDDEDCAL_FLAG_NON_TRANSPARENT_BKGND				0x0001
#define EMBEDDEDCAL_FLAG_HASTARGETFRAME						0x0002

typedef struct
	{
	WSIG 		Header;	/* Signature and length of this record. */
	DWORD		Flags;
	COLOR_VALUE	HeaderBkgnd;
	COLOR_VALUE	SelectionColor;
	WORD		TargetFrameLength;
			
	DWORD		Spare[10];	/* Reserved for future use - must be zero */		

	} CDEMBEDDEDCALCTL;

typedef struct
	{
	WSIG 		Header;	/* Signature and length of this record. */
	DWORD		Flags;
	COLOR_VALUE	SelectedBackground;
	COLOR_VALUE	SelectedText;
	COLOR_VALUE	ControlBackground;
			
	DWORD		Spare[10];	/* Reserved for future use - must be zero */		

	} CDEMBEDDEDCONTACTLIST;

#define EMBEDDEDVIEW_FLAG_TRANSPARENT					0x0001
/*	if EMBEDDEDVIEW_FLAG_USEAPPLET_VIEWPROP is set then the view's property 
	should be checked otherwise check EMBEDDEDVIEW_FLAG_USEAPPLET_INBROWSER */
#define EMBEDDEDVIEW_FLAG_USEAPPLET_INBROWSER			0x0002
#define EMBEDDEDVIEW_FLAG_USEAPPLET_VIEWPROP			0x0004
#define EMBEDDEDVIEW_FLAG_USE_WEBLINES					0x0008
#define EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_MOUSE_TRACK_ON	0x0010 /* Line selection in the view follows the mouse movement if this flag is set */
#define EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_HEADER_OFF		0x0020 /* Hide the view headers if this flag is set */
#define EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_SHOW_AS_WEB_LINK	0x0040 /* Line in view shown with underline to makeit look like a web link */
#define EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_SHOW_ACTION_BAR	0x0080 /* Show action bar for view or not */
#define EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_SHOW_SELECTION_MARGIN	0x0100 /* Show selection margin in view or not */
#define EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_SHOW_CURRENT_THREAD	0x0200 /* Show current thread given a noteid */
#define EMBEDDEDVIEW_FLAG_NOT_USE_WEBVIEW_DEFAULT		0x0400 /* on web, use selection margin stuff set here */
#define EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_SHOW_ONLY_SUMMARIZED 0x0800 /* Show only summarized for calendar view */
#define EMBEDDEDVIEW_FLAG_HASNAME						0x1000 /* embedded view has name */
#define EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_SHOW_NEWMAIL_AT_TOP 0x2000 /* Show only summarized for calendar view */

/*Flags above are DWORD,so we have plenty left */

#define EVIsSimpleViewMouseTrackOn(pEView) ( (pEView)->Flags & EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_MOUSE_TRACK_ON )
#define EVIsSimpleViewHeaderOff(pEView) ( (pEView)->Flags & EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_HEADER_OFF )
#define EVSetSimpleViewMouseTrackOn(pEView,fSet) \
	{\
		if ( fSet ) (pEView)->Flags |= EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_MOUSE_TRACK_ON;\
		else		(pEView)->Flags &= ~EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_MOUSE_TRACK_ON;\
	}
#define EVSetSimpleViewHeaderOff(pEView,fSet) \
	{\
		if ( fSet ) (pEView)->Flags |= EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_HEADER_OFF;\
		else		(pEView)->Flags &= ~EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_HEADER_OFF;\
	}
#define EVIsSimpleViewShowAsWebLink(pEView) ( (pEView)->Flags & EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_SHOW_AS_WEB_LINK)
#define EVSetSimpleViewShowAsWebLink(pEView,fSet) \
	{\
		if ( fSet ) (pEView)->Flags |= EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_SHOW_AS_WEB_LINK;\
		else		(pEView)->Flags &= ~EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_SHOW_AS_WEB_LINK;\
	}
#define EVIsSimpleViewShowActionBar(pEView) ( (pEView)->Flags & EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_SHOW_ACTION_BAR)
#define EVSetSimpleViewShowActionBar(pEView,fSet) \
	{\
		if ( fSet ) (pEView)->Flags |= EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_SHOW_ACTION_BAR;\
		else		(pEView)->Flags &= ~EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_SHOW_ACTION_BAR;\
	}

#define EVIsSimpleViewShowSelectionMargin(pEView) ( (pEView)->Flags & EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_SHOW_SELECTION_MARGIN)
#define EVSetSimpleViewShowSelectionMargin(pEView,fSet) \
	{\
		if ( fSet ) (pEView)->Flags |= EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_SHOW_SELECTION_MARGIN;\
		else		(pEView)->Flags &= ~EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_SHOW_SELECTION_MARGIN;\
	}

#define EVIsSimpleViewShowCurrentThread(pEView) ( (pEView)->Flags & EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_SHOW_CURRENT_THREAD)
#define EVSetSimpleViewShowCurrentThread(pEView,fSet) \
	{\
		if ( fSet ) (pEView)->Flags |= EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_SHOW_CURRENT_THREAD;\
		else		(pEView)->Flags &= ~EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_SHOW_CURRENT_THREAD;\
	}

#define EVIsSimpleViewShowOnlySummarized(pEView) ( (pEView)->Flags & EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_SHOW_ONLY_SUMMARIZED)
#define EVSetSimpleViewShowOnlySummarized(pEView,fSet) \
	{\
		if ( fSet ) (pEView)->Flags |= EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_SHOW_ONLY_SUMMARIZED;\
		else		(pEView)->Flags &= ~EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_SHOW_ONLY_SUMMARIZED;\
	}

#define EVIsSimpleViewShowNewMailAtTop(pEView) ( (pEView)->Flags & EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_SHOW_NEWMAIL_AT_TOP)
#define EVSetSimpleViewShowNewMailAtTop(pEView,fSet) \
	{\
		if ( fSet ) (pEView)->Flags |= EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_SHOW_NEWMAIL_AT_TOP;\
		else		(pEView)->Flags &= ~EMBEDDEDVIEW_FLAG_SIMPLE_VIEW_SHOW_NEWMAIL_AT_TOP;\
	}

/* Special string for restrict category formula. If the formula evaluates to this then
	there is no category restriction. */
#define EMBEDDEDVIEW_SHOW_ALL_SYMBOL_STRING		"*"

typedef struct
	{
	WSIG 	Header;	/* Signature and length of this record. */
	DWORD 	Flags;	
	FONTID  SpareFontID;
	WORD	RestrictFormulaLength;
	WORD	WebLines;
	WORD	NameLength;
	WORD	wSpare;
	DWORD	Spare[3];
	} CDEMBEDDEDVIEW;

/* edit control styles */
#define	EC_STYLE_EDITMULTILINE	0x0001
#define EC_STYLE_EDITVSCROLL	0x0002
#define EC_STYLE_EDITPASSWORD	0x0004
/* combobox styles */
#define EC_STYLE_EDITCOMBO		0x0001
/* list box styles */
#define	EC_STYLE_LISTMULTISEL	0x0001
/* time control styles */
#define EC_STYLE_CALENDAR		0x0001
#define EC_STYLE_TIME			0x0002
#define EC_STYLE_DURATION		0x0004
#define EC_STYLE_TIMEZONE		0x0008
/* control style is valid */
#define EC_STYLE_VALID			0x80000000

/* other control flags */
#define EC_FLAG_UNITS			0x000F
#define	EC_FLAG_DIALOGUNITS		0x0001	/* Width/Height are in dialog units, not twips */
#define EC_FLAG_FITTOCONTENTS	0x0002	/* Width/Height should be adjusted to fit contents */
#define EC_FLAG_ALWAYSACTIVE	0x0010	/* this control is active regardless of docs R/W status */
#define	EC_FLAG_FITTOWINDOW		0x0020	/* let placeholder automatically fit to window */
#define EC_FLAG_POSITION_TOP	0x0040	/* position control to top of paragraph */
#define EC_FLAG_POSITION_BOTTOM 0x0080	/* position control to bottom of paragraph */
#define EC_FLAG_POSITION_ASCENT 0x0100	/* position control to ascent of paragraph */
#define EC_FLAG_POSITION_HEIGHT 0x0200	/* position control to height of paragraph */


#define EC_DIALOGUNITS(flags)   ((flags & EC_FLAG_UNITS) == EC_FLAG_DIALOGUNITS)
#define EC_FITTOCONTENTS(flags) ((flags & EC_FLAG_UNITS) == EC_FLAG_FITTOCONTENTS)

#define EMBEDDEDCTL_VERSION1	0

/* embedded control types */

enum {
		EMBEDDEDCTL_EDIT = 0,
		EMBEDDEDCTL_COMBO = 1,
		EMBEDDEDCTL_LIST = 2,
		EMBEDDEDCTL_TIME = 3,
   		EMBEDDEDCTL_KEYGEN = 4,
   		EMBEDDEDCTL_FILE = 5,
		EMBEDDEDCTL_TIMEZONE = 6,
		EMBEDDEDCTL_COLOR = 7
	 };

typedef struct
	{
	WSIG 	Header;	/* Signature and length of this record. */
	DWORD 	CtlStyle;	
	WORD	Flags;
	WORD    Width;
	WORD    Height;
	WORD    Version;
	WORD	CtlType;
	WORD	MaxChars;
	WORD	MaxLines;
	WORD	Percentage;
	DWORD	Spare[3];
	} CDEMBEDDEDCTL;

/*	On-disk format for form layout objects */

#define LAYOUT_FLAG_SHOWBORDER		0x00000001
#define LAYOUT_FLAG_SHOWGRID		0x00000002
#define LAYOUT_FLAG_SNAPTOGRID		0x00000004
#define LAYOUT_FLAG_3DSTYLE			0x00000008
#define LAYOUT_FLAG_RTL				0x00000010
#define LAYOUT_FLAG_DONTWRAP		0x00000020

typedef struct
	{
	BSIG	Header;
	WORD	wLeft;
	WORD	wWidth;
	WORD	wHeight;
	DWORD	Flags;
	WORD	wGridSize;
	BYTE	Reserved[14];
	} CDLAYOUT;

typedef struct
	{
	WORD	wLeft;
	WORD	wTop;
	WORD	wWidth;
	WORD	wHeight;
	FONTID	FontID;				/* Font ID */
	BYTE	byBackColor;		/*	Background color */
	BYTE	bSpare;
	COLOR_VALUE BackgroundColor; /* v5.0 background color */
	} ELEMENTHEADER;

/*	The following flags must be the same as LAYOUT_FIELD_FLAG_ equiv's. */
#define LAYOUT_TEXT_FLAG_TRANS		0x10000000
#define LAYOUT_TEXT_FLAG_LEFT		0x00000000
#define LAYOUT_TEXT_FLAG_CENTER		0x20000000
#define LAYOUT_TEXT_FLAG_RIGHT		0x40000000
#define LAYOUT_TEXT_FLAG_ALIGN_MASK	0x60000000
#define LAYOUT_TEXT_FLAG_VCENTER	0x80000000
#define LAYOUT_TEXT_FLAG_LTR		0x01000000
#define LAYOUT_TEXT_FLAG_RTL		0x02000000
#define LAYOUT_TEXT_FLAG_RO_MASK	0x03000000
#define LAYOUT_TEXT_FLAGS_MASK		0xF0000000

typedef struct
	{
	BSIG			Header;
	ELEMENTHEADER	ElementHeader;
	DWORD			Flags;
	BYTE			Reserved[16];
	/*	For records save with builds prior to 134 the 8-bit text string follows... */
	} CDLAYOUTTEXT;

#define LAYOUT_FIELD_TYPE_TEXT	0
#define LAYOUT_FIELD_TYPE_CHECK	1
#define LAYOUT_FIELD_TYPE_RADIO	2
#define LAYOUT_FIELD_TYPE_LIST	3
#define LAYOUT_FIELD_TYPE_COMBO	4

#define LAYOUT_FIELD_FLAG_SINGLELINE	0x00000001
#define LAYOUT_FIELD_FLAG_VSCROLL		0x00000002
/*	The following flag must not be sampled by any design mode
	code.  It is, in effect, "write only" for design elements.
	Play mode elements, on the other hand, can rely on its value. */
#define LAYOUT_FIELD_FLAG_MULTISEL		0x00000004
#define LAYOUT_FIELD_FLAG_STATIC		0x00000008
#define LAYOUT_FIELD_FLAG_NOBORDER		0x00000010
#define LAYOUT_FIELD_FLAG_IMAGE			0x00000020
/*	The following flags must be the same as LAYOUT_TEXT_FLAG_ equiv's. */
#define LAYOUT_FIELD_FLAG_LTR			0x01000000
#define LAYOUT_FIELD_FLAG_RTL			0x02000000
#define LAYOUT_FIELD_FLAG_TRANS			0x10000000
#define LAYOUT_FIELD_FLAG_LEFT			0x00000000
#define LAYOUT_FIELD_FLAG_CENTER		0x20000000
#define LAYOUT_FIELD_FLAG_RIGHT			0x40000000
#define LAYOUT_FIELD_FLAG_VCENTER		0x80000000

typedef struct
	{
	BSIG			Header;
	ELEMENTHEADER	ElementHeader;
	DWORD			Flags;
	BYTE			bFieldType;
	BYTE			Reserved[15];
	} CDLAYOUTFIELD;

#define LAYOUT_GRAPHIC_FLAG_BUTTON		0x00000001
typedef struct
	{
	BSIG			Header;
	ELEMENTHEADER	ElementHeader;
	DWORD			Flags;
	BYTE			Reserved[16];
	} CDLAYOUTGRAPHIC;

typedef struct
	{
	BSIG			Header;
	ELEMENTHEADER	ElementHeader;
	DWORD			Flags;
	BYTE			Reserved[16];
	} CDLAYOUTBUTTON;

typedef struct
	{
	BSIG	Header;
	BYTE	Reserved[16];
	} CDLAYOUTEND;

/* Structure for LS Object code	*/
typedef struct
	{
	WSIG	Header;
	DWORD   CodeSize;		/* Total code size for multiple code segments, for R6 >64k */
	BYTE	Reserved[4];	/* FOr future types/versioning perhaps...*/
	} CDLSOBJECT;

/* R6 specific type, so ignored by R5 */
typedef struct
	{
	WSIG	Header;
#define CDLSOBJECT_R6_TYPE  01		/* signals multiple code segments for R6 >64k */
	BYTE    Flags;
	BYTE	Reserved[7];	
	} CDLSOBJECT_R6;

/*	CDHTML - Store raw HTML */

typedef struct {
	WSIG Header;
	WORD wSegments;				/* Length of raw HTML */
								/* HTML, the variable part of the struct... */
} CDHTMLHEADER;

typedef struct {
	WSIG Header;
	WORD wHTMLLength;			/* Length of raw HTML */
								/* HTML, the variable part of the struct... */
} CDHTMLSEGMENT;


/* Structures for hotspots which are "active" objects (Java applets, plugins, etc.) */

#define ACTIVEOBJECT_TYPE_JAVA		1
#define ACTIVEOBJECT_TYPE_PLUGIN	2
#define ACTIVEOBJECT_TYPE_OBJECT	3

#define ACTIVEOBJECT_UNIT_PIXELS	0
#define ACTIVEOBJECT_UNIT_HIMETRICS	1
#define ACTIVEOBJECT_UNIT_INCHES	2
#define ACTIVEOBJECT_UNIT_PERCENT	3

#define ACTIVEOBJECT_FLAG_MAYSCRIPT			0x00000001
#define ACTIVEOBJECT_FLAG_CORBA_APPLET		0x00000002	/* Active object is a Java applet that uses CORBA */
#define ACTIVEOBJECT_FLAG_CORBA_SSL			0x00000004	/* This is a CORBA applet that uses SSL */
#define ACTIVEOBJECT_FLAG_MAIL_PLUGIN		0x00000008  /* This object comes from a mime mail message*/
#define ACTIVEOBJECT_FLAG_NOCORBADOWNLOAD	0x00000010 /* don't automatically download the jar stuff for applets */
#define ACTIVEOBJECT_FLAG_DIGESTAPPLETFILES	0x00000020 /* Reserved part of ACTIVEOBJECT sturct contains applet files digested for signature verification */

/* Version control of ACTIVEOBJECT */
#define ACTIVEOBJECT_VERSION1		0		/* Created by Notes build 141 */

typedef struct {
	WORD Version;			/* Version number */
	WORD ObjectType;		/* Type of active object */
	DWORD Flags; 			/* Flags */
	BYTE WidthUnitType;		/* Currently only ACTIVEOBJECT_UNIT_PIXELS and ACTIVEOBJECT_UNIT_PERCENT supported */
	BYTE HeightUnitType;	/* Currently only ACTIVEOBJECT_UNIT_PIXELS and ACTIVEOBJECT_UNIT_PERCENT supported */
	DWORD Width;			/* Width of active object */
	DWORD Height;			/* Height of active object */
	WORD  SpaceUnitType;	/* Currently  not used */
	WORD  HSpace;			/* Currently not used */
	WORD  VSpace;			/* Currently not used */

	WORD DocURLNameLength;	/* Length of document URL string that follows */
	WORD CodebaseLength;	/* Length of codebase string that follows */
	WORD CodeLength;		/* Length of code string that follows */
	WORD ObjectNameLength;	/* Length of object name string that follows */
	WORD StorageLinkCount;	/* Number of ACTIVEOBJECTSTORAGELINK structures that follow */ 
	WORD ParamCount;		/* Number of ACTIVEOBJECTPARAM structures that follow */
	BYTE Used[16];
	/* Variable length data (strings not null-terminated) follows here:
		- Document URL string
		- Codebase string
		- Code string (SRC info for plugins)
		- Object name (for Java applets)
		- ACTIVEOBJECTPARAM info (should be ParamCount number of these -
		  						  Plugin ATTRIBUTE info should be here).
		- ACTIVEOBJECTSTORAGELINK info (should be StorageLinkCount number
										of these).
	 */
} ACTIVEOBJECT;

/*	The data in the ACTIVEOBJECTPARAM structure may be formatted in one
	of two ways: either as a string representing the entire parameter, or
	as a string representing the parameter name, and a formula representing
	the parameter value.

	In the first case, the variable length data is a string of the form
	"ParameterName=ParameterValue".  In this case, "Length" should be the
	number of bytes in this string, and	"FormulaLength" should be zero.

	In the second case, the variable length data consists of a string
	representing the parameter name followed by a formula representing the
	parameter value.  In this case, "Length" should be the number of bytes
	in the parameter name string, and "FormulaLength" should be the length
	of the formula. */

typedef struct {
	WORD Length;				/* Length of the text string that follows */
	WORD FormulaLength;			/* Length of the formula data (if any) that follows */
	WORD Reserved;
	/* Variable length data follows (strings not null terminated) */
} ACTIVEOBJECTPARAM;

typedef struct {
	WORD Length;				/* Length of the attachment name following */
	WORD LinkType;				/* Currently not used */
	DWORD Reserved;
	/* Variable length string follows (not null terminated).  This string
	   is the name of an attachment which contains data (e.g., a .class
	   file) used by the active object. */
} ACTIVEOBJECTSTORAGELINK;

#define STORAGE_LINK_TYPE_OBJECT		1
#define STORAGE_LINK_TYPE_NOTE			2
#define STORAGE_LINK_TYPE_URL_CONVERTED	3
#define STORAGE_LINK_TYPE_URL_MIME		4
#define STORAGE_LINK_TYPE_MIME_PART		5
#define STORAGE_LINK_TYPE_MIME_OBJECT	6

#define STORAGE_LINK_LOAD_DEFERRED	1
#define STORAGE_LINK_LOAD_ON_DEMAND	2

/*	Structure for externally stored objects */

typedef struct {
	WSIG Header;
	WORD StorageType; 			/* Type of object (Object, Note, URL, etc.) */
	WORD LoadType;				/* How to load (deferred, on demand, etc.) */
	WORD Flags;					/* Currently not used */
	WORD DataLength;			/* Length of data following */
	WORD Reserved[6];			/* Currently not used */
								/* Storage data follows... */
} CDSTORAGELINK;

/* Structure for alternate CD records */
/* by definition these are alternates to what immediately preceeds them */
typedef struct
	{
	WSIG Header;			/* Signature and length of this record  */
	WORD Type;				/* Unused at this time */
	DWORD SequenceNumber;	/* ID/Sequence number should match  what's
							 * in some ACTIVEOBJECT in the doc
							 */
	DWORD Flags;			/* Unused at this time 					*/
	WORD DataLength;		/* Unused at this time 					*/
	/*	Data Follows. */
	} CDALTERNATEBEGIN;

typedef struct
	{
	BSIG Header;		/* Signature and length of this record */
	DWORD Flags;		/* Unused at this time				   */
	} CDALTERNATEEND;

/*	BEGIN/END HTML Markers									*/

typedef struct {
	WSIG	Header;
	BYTE	Spares[4];
	} CDHTMLBEGIN;

typedef struct {
	WSIG	Header;
	BYTE	Spares[4];
	} CDHTMLEND;

typedef struct
	{
	BSIG Header;
	DWORD Interval;
	WORD Reserved[4];
	} CDTIMERINFO;	

typedef struct
	{
	BSIG Header;
	WORD RowHeight;
	} CDTABLEROWHEIGHT;	

typedef struct
	{
	WSIG Header;
	char Label[128];
	WORD Reserved[3];
#define CDTABLELABEL_ROWLABEL	0x0001
#define CDTABLELABEL_TABLABEL	0x0002
	WORD Flags;
	} CDTABLELABEL;	

/* Repeat Values */
#define REPEAT_UNKNOWN		0
#define REPEAT_ONCE			1
#define REPEAT_VERT			2
#define REPEAT_HORIZ		3
#define REPEAT_BOTH			4
#define REPEAT_SIZE			5
#define REPEAT_CENTER		6

typedef struct
	{
	WSIG Header;
	BYTE  Repeat;
	BYTE  Spare[1];
	DWORD SpareDWORD;
	} CDCELLBACKGROUNDDATA;	

/* Transition Types */
#define TRANS_LEFTTORIGHT_COLUMN	1
#define TRANS_ROLLING				2
#define TRANS_TOPTOBOTTOM_ROW		3
#define TRANS_WIPE					4
#define TRANS_BOXES_INCREMENT		5
#define TRANS_EXPLODE				6
#define TRANS_DISSOLVE				7

typedef struct
	{
	WSIG Header;
	WORD Type;
	WORD Reserved[4];
	} CDTRANSITION;	

/* Begin and End Records - these should be put around all records that pertain
	to the particular data type */

typedef struct
	{
	BSIG Header;		/* Signature and length of this record */
	WORD Version;		
	WORD Signature;		/* Signature of record begin is for */			
	} CDBEGINRECORD;

typedef struct
	{
	BSIG Header;		/* Signature and length of this record */
	WORD Version;		
	WORD Signature;		/* Signature of record end is for */	
	} CDENDRECORD;

typedef struct
	{
	BSIG Header;		/* Signature and length of this record */
	DWORD PropID;	/* Property Number */
	} CDSPANRECORD;

/* Shared HTML type records shared thru use of Begin and End Records */

typedef struct
	{
	BSIG Header;		/* Signature and length of this record */
#define VERTICAL_ALIGNMENT_BASELINE	0
#define VERTICAL_ALIGNMENT_CENTER	1
#define VERTICAL_ALIGNMENT_TOP		2
/* Don't use 3 and 4 */
#define VERTICAL_ALIGNMENT_BOTTOM	5
	WORD Alignment;			/* Vertical Alignment values				   */
	} CDVERTICALALIGN;

/* Flag in CDTARGET for formula target */
#define FLAG_TARGET_IS_FORMULA	0x0001

typedef struct
	{
	WSIG Header;
	WORD TargetLength;
	WORD Flags;
	DWORD Reserved;
	} CDTARGET;	

/*	Structure to store any extra info about embedded elements */
typedef struct
	{
	WSIG Header;
	WORD NameLength;
	DWORD Flags;
	DWORD Reserved[5];
	} CDEMBEDEXTRAINFO;

#define FLOAT_LEFT		1
#define FLOAT_RIGHT		2

typedef struct
	{
	BSIG Header;		/* Signature and length of this record */
	WORD Float;			/* Vertical Alignment values				   */
	} CDFLOAT;

/* On-Disk structure for Borders - May be used by numerous other types */
#define CDBORDERSTYLE_NONE		0
#define CDBORDERSTYLE_SOLID 	1
#define CDBORDERSTYLE_DOUBLE	2
#define CDBORDERSTYLE_INSET		3
#define CDBORDERSTYLE_OUTSET	4
#define CDBORDERSTYLE_RIDGE		5
#define CDBORDERSTYLE_GROOVE	6
#define CDBORDERSTYLE_DOTTED	7
#define CDBORDERSTYLE_DASHED	8
#define CDBORDERSTYLE_PICTURE	9
#define CDBORDERSTYLE_GRAPHIC	10

#define CDBORDER_FLAGS_DROP_SHADOW	0x0001

typedef struct
	{
	WSIG Header;		/* Signature and length of this record */
	DWORD Flags;
	WORD BorderStyle;
	WORD BorderWidthTop;	
	WORD BorderWidthLeft;	
	WORD BorderWidthBottom;	
	WORD BorderWidthRight;	
	DWORD dwSpare;
	WORD BorderFlags;
	WORD DropShadowWidth;
	WORD InnerWidthTop;	
	WORD InnerWidthLeft;	
	WORD InnerWidthBottom;	
	WORD InnerWidthRight;	
	WORD OuterWidthTop;	
	WORD OuterWidthLeft;	
	WORD OuterWidthBottom;	
	WORD OuterWidthRight;	
	COLOR_VALUE Color;
	WORD wSpares[5];
} CDBORDERINFO;



#define CDMAPELEMENT_VERSION1	1

typedef struct {
	WSIG Header;
	DWORD Flags;		  /* reserved for future use */
	WORD MapNameLength;	  /* length of map name that follows */
	WORD LastDefaultRegionID; /* last number used in auto generating default region names */
	WORD LastRectRegionID; /* last number used in auto generating rect names */
	WORD LastCircleRegionID; /* last number used in auto generating circle names */
	WORD LastPolyRegionID;	/* last number used in auto generating polygon names */
	BYTE Reserved[16];
	/* variable part of map element follows:
	*	MapName (string, not  null terminated)
	*/
} CDMAPELEMENT;

#define CDAREAELEMENT_VERSION1	1

typedef struct {
	WSIG Header;
	DWORD Flags;		/* reserved for future use */
	WORD Shape;			/* one of AREA_SHAPE_ */
	WORD TabIndex;
	WORD AccessKey;
	BYTE Reserved[16];
	/* variable length items follow:
	* If Shape == rect
	*   CDRECT
	* else if Shape == circle   NOTE: points for the rect the circle is drawn in
	*	CDRECT
	* else if Shape == polygon
	*	WORD  numPts
	*	CDPOINT 1
	*	CDPOINT 2
	*
	*	...
	*
	*	CDPOINT n
	* else if Shape == default
	*  	No points
	*/
} CDAREAELEMENT;

#define AREA_SHAPE_RECT		1
#define AREA_SHAPE_CIRCLE   2
#define AREA_SHAPE_POLYGON	3
#define AREA_SHAPE_DEFAULT	4

typedef struct {
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
} CDRECT;

typedef struct {
	LONG x;
	LONG y;
} CDPOINT;

/*	Structure to hold HTML events
*/
typedef struct {
	WSIG	Header;
	DWORD	Flags;
	WORD	EventType;			/* one of HTML_EVENT_ */
	WORD	ActionType;			/* one of ACTION_TYPE_ */
	DWORD	ActionLength;		/* length of actions that follows */
	WORD	SignatureLength;	/* length of signature, if any */
	BYTE	Reserved[14];
	/*	Action follows here */
	/*	Signature follows here */
} CDEVENT;

/* Flags for Flags member of CDEVENT */
#define	EVENT_HAS_LIBRARIES		0x00000001

#define HTML_EVENT_ONCLICK		1
#define HTML_EVENT_ONDBLCLICK	2
#define HTML_EVENT_ONMOUSEDOWN	3
#define HTML_EVENT_ONMOUSEUP	4
#define HTML_EVENT_ONMOUSEOVER	5
#define HTML_EVENT_ONMOUSEMOVE	6
#define HTML_EVENT_ONMOUSEOUT	7
#define HTML_EVENT_ONKEYPRESS	8
#define HTML_EVENT_ONKEYDOWN	9
#define HTML_EVENT_ONKEYUP		10
#define HTML_EVENT_ONFOCUS		11
#define HTML_EVENT_ONBLUR		12
#define	HTML_EVENT_ONLOAD		13
#define	HTML_EVENT_ONUNLOAD		14
#define	HTML_EVENT_HEADER		15
#define	HTML_EVENT_ONSUBMIT		16
#define	HTML_EVENT_ONRESET		17
#define	HTML_EVENT_ONCHANGE		18
#define HTML_EVENT_ONERROR		19
#define HTML_EVENT_ONHELP		20
#define HTML_EVENT_ONSELECT		21
#define HTML_EVENT_LIBRARY		22	/* This isn't really an event*/


#define HTML_EVENT_CLIENT_FORM_QUERYOPEN		0x100
#define HTML_EVENT_CLIENT_FORM_QUERYMODE		0x101
#define HTML_EVENT_CLIENT_FORM_POSTMODE		0x102
#define HTML_EVENT_CLIENT_FORM_POSTRECALC		0x103
#define HTML_EVENT_CLIENT_FORM_POSTSAVE		0x104
#define HTML_EVENT_CLIENT_FORM_POSTSEND		0x105
#define HTML_EVENT_CLIENT_FORM_QUERYRECALC	0x106
#define HTML_EVENT_CLIENT_FORM_QUERYSEND		0x107



#define HTML_EVENT_CLIENT_VIEW_QUERYOPEN		0x108
#define HTML_EVENT_CLIENT_VIEW_POSTOPEN		0x109
#define HTML_EVENT_CLIENT_VIEW_REGIONDBLCLK	0x10a
#define HTML_EVENT_CLIENT_VIEW_QUERYOPENDOC	0x10b
#define HTML_EVENT_CLIENT_VIEW_QUERYRECALC	0x10c
#define HTML_EVENT_CLIENT_VIEW_QUERYADDTOFOLDER	0x10d
#define HTML_EVENT_CLIENT_VIEW_QUERYPASTE		0x10e
#define HTML_EVENT_CLIENT_VIEW_POSTPASTE		0x10f
#define HTML_EVENT_CLIENT_VIEW_QUERYDRAGDROP	0x110
#define HTML_EVENT_CLIENT_VIEW_POSTDRAGDROP	0x111
#define HTML_EVENT_CLIENT_VIEW_QUERYCLOSE		0x112



#define HTML_EVENT_CLIENT_ONOBJECTEXECUTE		0x113

#define HTML_EVENT_CLIENT_DB_QUERYOPEN			0x114
#define HTML_EVENT_CLIENT_DB_POSTOPEN			0x115
#define HTML_EVENT_CLIENT_DB_DOCDELETE			0x116
#define HTML_EVENT_CLIENT_DB_QUERYCLOSE		0x117
#define HTML_EVENT_CLIENT_DB_QUERYDELETE		0x118
#define HTML_EVENT_CLIENT_DB_QUERYUNDELETE	0x119
#define HTML_EVENT_CLIENT_DB_QUERYDRAGDROP	0x11a
#define HTML_EVENT_CLIENT_DB_POSTDRAGDROP		0x11b

#define HTML_EVENT_CLIENT_VIEW_QUERYENTRYRESIZE	0x11c
#define HTML_EVENT_CLIENT_VIEW_POSTENTRYRESIZE	0x11d
#define HTML_EVENT_CLIENT_VIEW_INVIEWEDIT		0x11e

#define HTML_EVENT_CLIENT_SCHED_INTERVALCHANGE		0x11f

#define HTML_EVENT_CLIENT_DB_QUERYARCHIVEDRAGDROP	0x120
#define HTML_EVENT_CLIENT_DB_POSTARCHIVEDRAGDROP	0x121

#define ACTION_TYPE_FORMULA			1
#define ACTION_TYPE_CANNED_ACTION 	2
#define ACTION_TYPE_LOTUS_SCRIPT 	3
#define ACTION_TYPE_JAVASCRIPT	 	4

#define BLOBMAX 20000
typedef struct 
	{
	WSIG	Header;		/* Signature and length of this record) */
	WORD	OwnerSig;	/* Sig of the owner of this data */
	WORD	Length;		/* The length of the data that follows */
	WORD	BlobMax;	/* The block size used by the writer of the blob */
	BYTE	Reserved[8];
	} CDBLOBPART;

typedef struct
	{
	WSIG	Header;			/* Signature and length of this record */
	WORD	Length;			/* Length of ID */
	WORD	wClassLen;		/* Length of CLASS */
	WORD	wStyleLen;		/* Length of STYLE */
	WORD	wTitleLen;		/* Length of TITLE */
	WORD	wExtraLen;		/* Length of extra attribute/value pairs */
	WORD	wNameLen;		/* Length of NAME */
	BYTE	reserved[10];
	/*	id,class,style, title and extra attrib/value pairs of object follows */
	} CDIDNAME;

/*	CDCAPTION - Text to display with an object (e.g., a graphic) */

#define CAPTION_POSITION_BELOW_CENTER	0	/*	Centered below object */
#define CAPTION_POSITION_MIDDLE_CENTER	1	/*	Centered on object */

typedef struct {
	WSIG	Header;				/* Tag and length */
	WORD	wLength;			/* Text length */
	BYTE	Position;			/* One of the position flags above */
	FONTID	FontID;				/* Font to use for the text */
	COLOR_VALUE FontColor;		/* RGB font color info */
	BYTE	Reserved[11];		/* Reserved for future use */
								/* The 8-bit text string follows... */
} CDCAPTION;


#define INLINE_FLAG_UNKNOWN			0x00000000
#define INLINE_FLAG_SCRIPT_LIB		0x00000001
#define INLINE_FLAG_STYLE_SHEET		0x00000002
#define INLINE_FLAG_HTML			0x00000004
#define INLINE_FLAG_HTMLFILERES		0x00000008

#define INLINE_FLAG_TYPES_MASK		0x0000000F

#define INLINE_VERSION1				1

typedef struct
	{
	WSIG Header;
	WORD wDatalength;
	DWORD dwFlags;
	DWORD dwReserved[4];
	} CDINLINE;

/*	Layer CD record signifies the start of a layer.
*/
#define CDLAYER_VERSION1				1
/*	Version 2:	In CDPOSITIONING record added BrowserLeftOffset and
*				BrowserRightOffset in reserved space.
*/
#define CDLAYER_VERSION2				2

typedef struct
	{
	BSIG Header;
	DWORD Reserved[4];
	} CDLAYER;

/*	Store a length to disk.
*/
typedef struct
	{
	WORD Flags;
	ALIGNED_NUMBER Length;
	BYTE Units;
	BYTE Reserved;	/*	reserved for future use */
	} LENGTH_VALUE;

/*	Flags for LENGTH_VALUE
*/
#define CDLENGTH_FLAGS_AUTO			0x0001		/*	corresponds to CSS "auto" value */
#define CDLENGTH_FLAGS_INHERIT		0x0002		/*	corresponds to CSS "inherit" value */

/*	Units of LENGTH_VALUE
*/
#define CDLENGTH_UNITS_UNKNOWN		0		/*	set when "auto" or "inherit" is set */
#define CDLENGTH_UNITS_TWIPS		1
#define CDLENGTH_UNITS_PIXELS		2		/* pixels */
#define CDLENGTH_UNITS_PERCENT		3
#define CDLENGTH_UNITS_EMS			4
#define CDLENGTH_UNITS_EXS			5
#define CDLENGTH_UNITS_CHARS		6		/* average width of a character based on the font */

typedef struct
	{
	BSIG Header;
	BYTE Scheme;
	BYTE bReserved;	/* reserved for future use */
	LONG ZIndex;
	LENGTH_VALUE Top;
	LENGTH_VALUE Left;
	LENGTH_VALUE Bottom;
	LENGTH_VALUE Right;			/*	not implemented */
	ALIGNED_NUMBER BrowserLeftOffset; 	/*	subtract from Top.Length to get left for a browser */
	ALIGNED_NUMBER BrowserRightOffset;	/*  not implemented */
	} CDPOSITIONING;

/*	values for Scheme */
#define CDPOSITIONING_SCHEME_STATIC		0
#define CDPOSITIONING_SCHEME_ABSOLUTE	1
#define CDPOSITIONING_SCHEME_RELATIVE	2
#define CDPOSITIONING_SCHEME_FIXED		3


typedef struct
	{
	BSIG Header;
	LENGTH_VALUE Width;
	LENGTH_VALUE Height;
	LENGTH_VALUE Reserved[4];	/* reserved for future use */
	DWORD dwReserved[4];		/* reserved for future use */
	} CDBOXSIZE;

/*
 * Platform types to be used by wPlatform in CDEVENTENTRY.
 */
#define PLATFORM_TYPE_CLIENT_ODS	1
#define PLATFORM_TYPE_WEB_ODS		2

typedef struct
	{
	WSIG	Header;			/* Signature and length of this record */
	WORD	wPlatform;		/* Platform type */
	WORD	wEventId;		/* Event id. The event that this will run on... OnClick, Exit, etc. */
	WORD	wActionType;	/* Action type (the language... LotusScript, Javascript, Formula, etc. */
	WORD	wReserved;		/* future use */
	DWORD	dwReserved;		/* future use */
	} CDEVENTENTRY;


/*	Background properties
*/
#define CDBACKGROUNDPROPERTIES_VERSION1 1
typedef struct
	{
	BSIG Header;
	BYTE Repeat;		/* 	one of REPEAT_ (see above) */
	BYTE bReserved;
	LENGTH_VALUE lvReservedX;
	LENGTH_VALUE lvReservedY;
	DWORD dwReserved[4];
	} CDBACKGROUNDPROPERTIES;


#define	CD_SECTION_ELEMENT			128
#define	CD_FIELDLIMIT_ELEMENT		129
#define	CD_BUTTONEX_ELEMENT			130

typedef struct
	{
	BSIG	Header;
	WORD	nFlags;			/* number of flags */
	WORD	elemType;		/* element these flags are for, CD_xxx_ELEMENT */
	DWORD	dwReserved;		/* future */
	/* DWORDS of flags follow... */
	} CDDATAFLAGS;


/*
 * CDIGNORE - Used to ignore a block of CD records for a particular version of Notes.
 *			  The block start with a CDIGNORE with a dwFlags that has FLAG_CDIGNORE_BEGIN and
 *			  ends with another CDIGNORE record with a dwFlags that has FLAG_CDIGNORE_END.
 *			  The CD records in between a CDIGNORE begin and a CDINGORE end should be ignored
 *			  when the wNotesVersion is <= CDIGNORE_NOTES_VERSION_CURRENT. These CD records
 *			  may be nested. i.e. CDIGNORE begin CDIGNORE begin CDIGNORE end CDIGNORE end.
 */

typedef struct
	{
	BSIG	Header;
	WORD	wNotesVersion;				/* Version of Notes... See below.  */
	DWORD	dwFlags;					/* See FLAG_CDIGNORE_ below. */
	DWORD	dwUnused[6];				/* Reserved for future use. Should be zeroed out. */
	} CDIGNORE;

#define FLAG_CDIGNORE_BEGIN			0x00000001
#define FLAG_CDIGNORE_END			0x00000002

#define CDIGNORE_NOTES_VERSION_6_0_0			1		/* 6.0.0 */
#define CDIGNORE_NOTES_VERSION_CURRENT			CDIGNORE_NOTES_VERSION_6_0_0


/*	End of Edit Package On Disk Structure Definitions */


#endif /* EDIT_ODS_DEFS */

