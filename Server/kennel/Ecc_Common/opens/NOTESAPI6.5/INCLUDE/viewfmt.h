#ifdef __cplusplus
extern "C" {
#endif

#ifndef VIEWFMT_DEFS
#define VIEWFMT_DEFS

/*	This file contains the view format definitions which are used to read and
	write the format of a view in the View note.
	
	The structure of this data is as follows:

	[ VIEW_FORMAT_HEADER ]		

	[ VIEW_TABLE_FORMAT ]

	[ VIEW_COLUMN_FORMAT #0 ]	n columns, as specified in VIEW_TABLE_FORMAT.
	[ VIEW_COLUMN_FORMAT #1 ]
	...
	[ VIEW_COLUMN_FORMAT #n ]

	[ COLUMN DATA #0 ]			n columns, as specified in VIEW_TABLE_FORMAT.
	[ COLUMN DATA #1 ]
	...
	[ COLUMN DATA #n ]

	[VIEW_TABLE_FORMAT2]

	[ VIEW_COLUMN_FORMAT2 #0 ]	n columns, as specified in VIEW_TABLE_FORMAT.
	[ VIEW_COLUMN_FORMAT2 #1 ]
	...
	[ VIEW_COLUMN_FORMAT2 #n ]

	[ VIEW_TABLE_FORMAT3 ]

	[ COLUMN DATA #0 ]			n column variable data added for R6
	[ COLUMN DATA #1 ]
	...
	[ COLUMN DATA #n ]

	[ VIEW_TABLE_FORMAT4 ]
	
	[ BACKGROUND RESOURCE LINK ]

	[ VIEW_COLUMN_FORMAT3 ]

	[ VIEW_COLUMN_FORMAT4 ]

*/
	


#ifndef FONTID_DEFS
#include "fontid.h"
#endif

#ifndef MISC_DEFS
#include "misc.h"
#endif

#ifndef NSF_DATA_DEFS
#include "nsfdata.h"
#endif

#ifndef COLORODS_DEFS
#include "colorods.h"
#endif 

#ifndef RSRCODS_DEFS
#include "rsrcods.h"
#endif
/* View on-disk format definitions */

#define VIEW_FORMAT_VERSION 1

#define VIEW_COLUMN_FORMAT_SIGNATURE	0x4356
#define VIEW_COLUMN_FORMAT_SIGNATURE2	0x4357
#define VIEW_COLUMN_FORMAT_SIGNATURE3	0x4358
#define VIEW_COLUMN_FORMAT_SIGNATURE4	0x4359
#define VIEW_COLUMN_FORMAT_SIGNATURE5	0x4360

#define VIEW_CLASS_TABLE	(0 << 4)
#define VIEW_CLASS_CALENDAR	(1 << 4)
#define VIEW_CLASS_MASK		0xF0

#define CALENDAR_TYPE_DAY	0
#define CALENDAR_TYPE_WEEK	1
#define CALENDAR_TYPE_MONTH	2

#define VIEW_STYLE_TABLE	VIEW_CLASS_TABLE
#define VIEW_STYLE_DAY		(VIEW_CLASS_CALENDAR + 0)
#define VIEW_STYLE_WEEK		(VIEW_CLASS_CALENDAR + 1)
#define VIEW_STYLE_MONTH	(VIEW_CLASS_CALENDAR + 2)

/* View table format descriptor.  Followed by VIEW_COLUMN_FORMAT */
/* descriptors; one per column.  The column format descriptors are followed */
/* by the packed item name, title, formula, and constant values.  */
/* All of this is followed by a VIEW_TABLE_FORMAT2 data structure that */
/* is only present in views saved in V2 or later. */
/* All descriptors and values are packed into one item named $VIEWFORMAT. */

typedef struct
	{
	BYTE	Version;						/* Version number */
	BYTE	ViewStyle;						/* View Style - Table,Calendar */
	} VIEW_FORMAT_HEADER;

typedef struct
	{
	VIEW_FORMAT_HEADER Header;
	WORD 	Columns;								/* Number of columns */
	WORD	ItemSequenceNumber;						/* Sequence number for unique item names */
	WORD 	Flags;									/* Flags */
#define VIEW_TABLE_FLAG_COLLAPSED			0x0001	/* Default to fully collapsed */
#define VIEW_TABLE_FLAG_FLATINDEX			0x0002	/* Do not index hierarchically */
													/* If FALSE, MUST have */
													/* NSFFormulaSummaryItem($REF) */
													/* as LAST item! */
#define VIEW_TABLE_FLAG_DISP_ALLUNREAD		0x0004	/* Display unread flags in margin at ALL levels */
#define VIEW_TABLE_FLAG_CONFLICT			0x0008	/* Display replication conflicts */
													/* If TRUE, MUST have */
													/* NSFFormulaSummaryItem($Conflict) */
													/* as SECOND-TO-LAST item! */
#define VIEW_TABLE_FLAG_DISP_UNREADDOCS		0x0010	/* Display unread flags in margin for documents only */
#define VIEW_TABLE_GOTO_TOP_ON_OPEN			0x0020	/* Position to top when view is opened. */
#define VIEW_TABLE_GOTO_BOTTOM_ON_OPEN		0x0040	/* Position to bottom when view is opened. */
#define VIEW_TABLE_ALTERNATE_ROW_COLORING	0x0080	/* Color alternate rows. */
#define VIEW_TABLE_HIDE_HEADINGS			0x0100	/* Hide headings. */
#define VIEW_TABLE_HIDE_LEFT_MARGIN			0x0200	/* Hide left margin. */
#define VIEW_TABLE_SIMPLE_HEADINGS			0x0400	/* Show simple (background color) headings. */
#define VIEW_TABLE_VARIABLE_LINE_COUNT		0x0800	/* TRUE if LineCount is variable (can be reduced as needed). */

/*	Refresh flags.

	When both flags are clear, automatic refresh of display on update
	notification is disabled.  In this case, the refresh indicator will
	be displayed.
	
	When VIEW_TABLE_GOTO_TOP_ON_REFRESH is set, the view will fe refreshed from
	the top row of the collection (as if the user pressed F9 and Ctrl-Home).
	
	When VIEW_TABLE_GOTO_BOTTOM_ON_REFRESH is set, the view will be refreshed
	so the bottom row of the collection is visible (as if the user pressed F9
	and Ctrl-End).
	
	When BOTH flags are set (done to avoid using another bit in the flags),
	the view will be refreshed from the current top row (as if the user
	pressed F9). */

#define VIEW_TABLE_GOTO_TOP_ON_REFRESH		0x1000	/* Position to top when view is refreshed. */
#define VIEW_TABLE_GOTO_BOTTOM_ON_REFRESH	0x2000	/* Position to bottom when view is refreshed. */

/*	More flag(s). */

#define VIEW_TABLE_EXTEND_LAST_COLUMN		0x4000	/* TRUE if last column should be extended to fit the window width. */
#define	VIEW_TABLE_RTLVIEW					0x8000	/* TRUE if the View indexing should work from the Right most column */

	WORD 	Flags2;									/* Flags */
#define VIEW_TABLE_FLAT_HEADINGS			0x0001	/* TRUE if we should display no-borders at all on the header */
#define	VIEW_TABLE_COLORIZE_ICONS			0x0002	/* TRUE if the icons displayed inthe view should be colorized */
#define VIEW_TABLE_HIDE_SB					0x0004	/* TRUE if we should not display a search bar for this view */
#define VIEW_TABLE_HIDE_CAL_HEADER			0x0008	/* TRUE if we should hide the calendar header */
#define VIEW_TABLE_NOT_CUSTOMIZED			0x0010	/* TRUE if view has not been customized (i.e. not saved by Designer) */
	} VIEW_TABLE_FORMAT;

/*  Additional (since V2) format info.  This structure follows the
	variable length strings that follow the VIEW_COLUMN_FORMAT structres */

typedef struct
	{
	WORD	Length;							/* Length of this structure */
	WORD	BackgroundColor;				/* Color of view's background. Pre-V4 compatible */
	WORD	V2BorderColor;					/* Archaic! Color of view's border lines. */
	FONTID	TitleFont;						/* Title and borders */
	FONTID	UnreadFont;						/* Unread lines */
	FONTID	TotalsFont;						/* Totals/Statistics */
	WORD	AutoUpdateSeconds;				/* interval b/w auto updates (zero for no autoupdate) */
	WORD	AlternateBackgroundColor;		/* Color of view's background for alternate rows. */

	/*	When wSig == VALID_VIEW_FORMAT_SIG, rest of struct is safe to use.  Bug
		in versions prior to V4 caused spare space in this structure to contain
		random stuff. */

	WORD	wSig;							
#define VALID_VIEW_FORMAT_SIG				0x2BAD
	BYTE	LineCount;						/* Number of lines per row.  1, 2, etc. */
#define VIEW_TABLE_MAX_LINE_COUNT			10
	BYTE	Spacing;						/* Spacing.  VIEW_TABLE_XXX_SPACE. */
#define VIEW_TABLE_SINGLE_SPACE				0
#define VIEW_TABLE_ONE_POINT_25_SPACE		1
#define VIEW_TABLE_ONE_POINT_50_SPACE		2
#define VIEW_TABLE_ONE_POINT_75_SPACE		3
#define VIEW_TABLE_DOUBLE_SPACE				4
	WORD	BackgroundColorExt;				/* Palette Color of view's background. */
#define VIEW_TABLE_COLOR_MASK	0x00FF		/* color is index into 240 element array */
	BYTE	HeaderLineCount;				/* Lines per header. */
	BYTE	Flags1;
#define VIEW_TABLE_HAS_LINK_COLUMN 	0x01	/* TRUE if a link column has been specified for a web browser. */
#define VIEW_TABLE_HTML_PASSTHRU	0x02	/* TRUE if line entry text should be treated as HTML by a web browser. */

	/* Spares.  Will be zero when wSig == VALID_VIEW_FORMAT_SIG. */

	WORD	Spare[4];						
	} VIEW_TABLE_FORMAT2;

typedef struct
	{
	VIEW_FORMAT_HEADER Header;
	} VIEW_DAY_FORMAT;

typedef struct
	{
	VIEW_FORMAT_HEADER Header;
	} VIEW_WEEK_FORMAT;

typedef struct
	{
	VIEW_FORMAT_HEADER Header;
	} VIEW_MONTH_FORMAT;


typedef struct
	{
	WORD Length;							/*	Length of this structure */
	DWORD Flags;						  	/*	Reserved for future use */
#define VTF3_S_GridStyleSolid				0		/* V6 view grid lines use solid lines */
#define VTF3_M_GridStyleSolid				0x00000001
#define VTF3_S_GridStyleDash				1		/* V6 view grid lines use dashes */
#define VTF3_M_GridStyleDash				0x00000002
#define VTF3_S_GridStyleDot					2		/* V6 view grid lines use dots */
#define VTF3_M_GridStyleDot					0x00000004
#define VTF3_S_GridStyleDashDot				3		/* V6 view grid lines use dashes and dots */
#define VTF3_M_GridStyleDashDot				0x00000008
#define VTF3_S_AllowCustomizations			4		/* V6 view customizations on/off */
#define VTF3_M_AllowCustomizations			0x00000010
#define VTF3_S_EvaluateActionsHideWhen		5		/* V6 view actions evaluate hidewhen */
#define VTF3_M_EvaluateActionsHideWhen		0x00000020
#define VTF3_S_HideLeftMarginBorder			6
#define VTF3_M_HideLeftMarginBorder			0x00000040	/* V6 - Hide border after left margin */
#define VTF3_S_BoldUnreadRows				7
#define VTF3_M_BoldUnreadRows				0x00000080	/* V6 - bold the unread rows. */
#define VTF3_S_AllowCreateNewDoc			8
#define VTF3_M_AllowCreateNewDoc			0x00000100	/* V6 - inviewedit-newdocs in view */
#define VTF3_S_HasBackgroundImage			9
#define VTF3_M_HasBackgroundImage			0x00000200	/* V6 - View has background image. */
	COLOR_VALUE BackgroundColor;		  	/*	V5 background color */
	COLOR_VALUE AlternateBackgroundColor; 	/*	V5 alternate row color */
	COLOR_VALUE GridColorValue;				/*  V6 View's grid color (3 words)*/
	WORD wViewMarginTop;					/*  V6 View Margin top */
	WORD wViewMarginLeft;					/*  V6 View Margin left */
	WORD wViewMarginRight;					/*  V6 View Margin right */
	WORD wViewMarginBottom;					/*  V6 View Margin bottom */
	COLOR_VALUE MarginBackgroundColor;		/*  V6 View Margin Background Color */
	COLOR_VALUE HeaderBackgroundColor;		/* 	V6 View column header background color */
	WORD wViewMarginTopUnder;				/*  V6 View Margin top -- under header */
	COLOR_VALUE UnreadColor;				/*  V6 unread row color */
	COLOR_VALUE TotalsColor;				/*  V6 Column Total Color */
	DWORD dwReserved[2];					/*	Reserved for future use */
	} VIEW_TABLE_FORMAT3;

typedef struct
	{
	WORD 		Length;					/*	Length of this structure */
	DWORD 		Flags;				  	/*	Reserved for future use */
	WORD		RepeatType;				/*  see viewprop.h - way to repeat image*/
	} VIEW_TABLE_FORMAT4;

typedef struct
	{
	WORD 		Length;					/*	Length of this structure */
	DWORD 		Flags;				  	/*	Reserved for future use */
	WORD		RepeatType;				/*  see viewprop.h - way to repeat image*/
	} VIEW_TABLE_FORMAT5;

/* 	Calendar View Format Information.  Introduced in build 141 (for 4.2).
	This is in Calendar Style Views only. */

#define VIEW_CALENDAR_FORMAT_VERSION	1

typedef struct
	{
	BYTE	Version;						/* Version Number */	
	BYTE	Formats;						/* Formats supported by this view VIEW_CAL_FORMAT_XXX.*/
#define VIEW_CAL_FORMAT_TWO_DAY		0x01
#define VIEW_CAL_FORMAT_ONE_WEEK	0x02
#define VIEW_CAL_FORMAT_TWO_WEEKS	0x04	
#define VIEW_CAL_FORMAT_ONE_MONTH	0x08	
#define VIEW_CAL_FORMAT_ONE_YEAR	0x10  	
#define VIEW_CAL_FORMAT_ONE_DAY		0x20
#define VIEW_CAL_FORMAT_WORK_WEEK	0x40  	
#define VIEW_CAL_FORMAT_ALL			0xff

#define VIEW_CAL_VALID_PRE_503_FORMATS		0x3f
#define VIEW_CAL_VALID_503_FORMATS			0x7f


	FONTID	DayDateFont;					/* Day and Date display */
	FONTID	TimeSlotFont;					/* Time Slot display */
	FONTID	HeaderFont;					   	/* Month Headers */

	WORD	DaySeparatorsColor;				/* Lines separating days */
	WORD    TodayColor;						/* Color Today is displayed in */

	WORD	wFlags;							/* Misc Flags */
#define CAL_DISPLAY_CONFLICTS		0x0001	/* Display Conflict marks */
#define CAL_ENABLE_TIMESLOTS		0x0002	/* Disable Time Slots */
#define CAL_DISPLAY_TIMESLOT_BMPS	0x0004	/* Show Time Slot Bitmaps */
#define CAL_ENABLE_TIMEGROUPING		0x0008	/* Enable Timegrouping */
#define CAL_TIMESLOT_OVERRIDE		0x0010	/* Allow user to override time slots */
#define CAL_HIDE_MONTH_HEADER		0x0020	/* Don't show the month header in the view (i.e. January 2001) */
#define CAL_HIDE_GOTOTODAY			0x0040	/* Don't show the GoToToday button in the view */
#define CAL_SHOW_TRASHVIEW			0x0080	/* Don't show the trash view in the header */
#define CAL_SHOW_ALLDOCSVIEW		0x0100	/* Don't show the all docs view in the header */
#define CAL_HIDE_FORMATBTN			0x0200	/* Don't show the formatting button */
#define CAL_HIDE_DAYTAB				0x0400	/* Don't show the day tab */
#define CAL_HIDE_WEEKTAB			0x0800	/* Don't show the week tab */
#define CAL_HIDE_MONTHTAB			0x1000	/* Don't show the month tab */
#define CAL_SHOW_DAYPLANNER			0x2000	/* show the header as dayplanner */
#define CAL_HIDE_OWNERNAME			0x4000  /* show the owner name */
#define	VIEW_CALENDAR_RTLVIEW		0x8000	/* TRUE if RTL Calendar, Note: same as VIEW_TABLE_RTLVIEW */
	
	WORD    BusyColor;						/* Color busy times are displayed in */

	WORD    wTimeSlotStart;				 	/* TimeSlot start time (in minutes from midnight) */
	WORD    wTimeSlotEnd;				 	/* TimeSlot end time (in minutes from midnight) */
	WORD    wTimeSlotDuration;			 	/* TimeSlot duration (in minutes) */

	COLOR_VALUE DaySeparatorsColorExt;		/* written by v5; Same meaning as DaySeparatorsColor, but can be any color */
	COLOR_VALUE BusyColorExt;				/* written by v5; Same meaning as BusyColor, but can be any color */

	BYTE	MinorVersion;					/* Minor version */
#define VIEW_CAL_FORMAT_MINOR_V4x	0			/* V4.5, V4.6 has minor version of 0 */
#define VIEW_CAL_FORMAT_MINOR_1		1			/* V5 */
#define VIEW_CAL_FORMAT_MINOR_2		2			/* V5.03 and up  - added custom work week format */
#define VIEW_CAL_FORMAT_MINOR_3		3			/* Calendar Grid Color */
#define VIEW_CAL_FORMAT_MINOR_4		4			/* more damn colors */

	BYTE	InitialFormat;					/* Initial format to display.  0=last viewed by user */
	
	DWORD	CalGridBkColor;					/* Background color of Calendar Grid */
	DWORD	WorkHoursColor;					/* Background color for the work hours in the calendar grid */ 
	DWORD	ToDoBkColor;					/* Background color for the ToDo entry region */
	DWORD	HeaderBkColor;					/* Background color for calendar view's header background */

	} VIEW_CALENDAR_FORMAT;

typedef VIEW_CALENDAR_FORMAT *PVIEW_CALENDAR_FORMAT;


/* 	Secondary Calendar View Format.  Introduced in build 161 (for v5). */

#define VIEW_CALENDAR_FORMAT2_SIGNATURE	0x0323
	
typedef struct
	{
	WORD		Signature;			   		/* VIEW_CALENDAR_FORMAT2_SIGNATURE */

											/* Attributes initialized when MinorVersion = 1 */
	COLOR_VALUE DayDateBkColor;				/* Color for day/date background area */							
	COLOR_VALUE NonMonthBkColor;	 		/* Color for non-month date background area */							
	COLOR_VALUE NonMonthTextColor;	 		/* Color for non-month font */							

	COLOR_VALUE	DayDateColor;				/* V6 - Day and Date display */
	COLOR_VALUE	TimeSlotColor;				/* V6 - Time Slot display */
	COLOR_VALUE	HeaderColor;				/* V6 - Month Headers */
	COLOR_VALUE TodayRGBColor;				/* V6 - Today color */

	FONTID	WeekDayMonthFont;				/* One week view - Day and Month display */

	DWORD		Spare[3];
	} VIEW_CALENDAR_FORMAT2;



/* View column format descriptor.  One per column. */

typedef struct tagVIEW_COLUMN_FORMAT
	{
	WORD	Signature;							/* VIEW_COLUMN_FORMAT_SIGNATURE */
	WORD	Flags1;
#define	VCF1_S_Sort						0		/* Add column to sort */
#define	VCF1_M_Sort						0x0001
#define	VCF1_S_SortCategorize			1		/* Make column a category */
#define	VCF1_M_SortCategorize			0x0002
#define	VCF1_S_SortDescending			2		/* Sort in descending order (ascending if FALSE) */
#define	VCF1_M_SortDescending			0x0004
#define	VCF1_S_Hidden					3		/* Hidden column */
#define	VCF1_M_Hidden					0x0008
#define	VCF1_S_Response					4		/* Response column */
#define	VCF1_M_Response					0x0010
#define	VCF1_S_HideDetail				5		/* Do not show detail on subtotalled columns */
#define	VCF1_M_HideDetail				0x0020
#define	VCF1_S_Icon						6		/* Display icon instead of text */
#define	VCF1_M_Icon						0x0040
#define	VCF1_S_NoResize					7		/* Resizable at run time. */
#define	VCF1_M_NoResize					0x0080
#define	VCF1_S_ResortAscending			8		/* Resortable in ascending order. */
#define	VCF1_M_ResortAscending			0x0100
#define	VCF1_S_ResortDescending			9		/* Resortable in descending order. */
#define	VCF1_M_ResortDescending			0x0200
#define	VCF1_S_Twistie					10		/* Show twistie if expandable. */
#define	VCF1_M_Twistie					0x0400
#define	VCF1_S_ResortToView				11		/* Resort to a view. */
#define	VCF1_M_ResortToView				0x0800
#define	VCF1_S_SecondResort				12		/* Secondary resort column set. */
#define	VCF1_M_SecondResort				0x1000
#define	VCF1_S_SecondResortDescending	13		/* Secondary column resort descending (ascending if clear). */
#define	VCF1_M_SecondResortDescending	0x2000
/* The following 4 constants are obsolete - see new VCF3_ constants below. */
#define	VCF1_S_CaseInsensitiveSort		14		/* Case insensitive sorting. */
#define	VCF1_M_CaseInsensitiveSort		0x4000
#define	VCF1_S_AccentInsensitiveSort	15		/* Accent insensitive sorting. */
#define	VCF1_M_AccentInsensitiveSort	0x8000

	WORD	ItemNameSize;						/* Item name string size */
	WORD	TitleSize;							/* Title string size */
	WORD	FormulaSize;						/* Compiled formula size */
	WORD	ConstantValueSize;					/* Constant value size */
	WORD	DisplayWidth;						/* Display width - 1/8 ave. char width units */
	FONTID	FontID;								/* Display font ID */
	WORD	Flags2;
#define	VCF2_S_DisplayAlignment			0		/* Display alignment - VIEW_COL_ALIGN_XXX */
#define	VCF2_M_DisplayAlignment			0x0003
#define	VCF2_S_SubtotalCode				2		/* Subtotal code (NIF_STAT_xxx) */
#define	VCF2_M_SubtotalCode				0x003c
#define	VCF2_S_HeaderAlignment			6		/* Header alignment - VIEW_COL_ALIGN_XXX */
#define	VCF2_M_HeaderAlignment			0x00c0
#define	VCF2_S_SortPermute				8		/* Make column permuted if multi-valued */
#define	VCF2_M_SortPermute				0x0100
#define	VCF2_S_SecondResortUniqueSort	9		/* Secondary resort column props different from column def.*/
#define	VCF2_M_SecondResortUniqueSort	0x0200
#define	VCF2_S_SecondResortCategorized	10		/* Secondary resort column categorized. */
#define	VCF2_M_SecondResortCategorized	0x0400
#define	VCF2_S_SecondResortPermute		11		/* Secondary resort column permuted. */
#define	VCF2_M_SecondResortPermute		0x0800
#define	VCF2_S_SecondResortPermutePair 	12		/* Secondary resort column pairwise permuted. */
#define	VCF2_M_SecondResortPermutePair	0x1000
#define	VCF2_S_ShowValuesAsLinks		13		/* Show values as links when viewed by web browsers. */
#define	VCF2_M_ShowValuesAsLinks		0x2000
#define	VCF2_S_DisplayReadingOrder		14		/* Display Reading order - VIEW_COL_XXX */
#define	VCF2_M_DisplayReadingOrder		0x4000
#define	VCF2_S_HeaderReadingOrder		15		/* Header Reading order - VIEW_COL_XXX */
#define	VCF2_M_HeaderReadingOrder		0x8000

	NFMT	NumberFormat;						/* Number format specification */
	TFMT	TimeFormat;							/* Time format specification */
	WORD	FormatDataType;						/* Last format data type */
	WORD	ListSep;							/* List Separator */
	} VIEW_COLUMN_FORMAT;	

/* View column display alignment.  */
/*		Note: order and values are assumed in VIEW_ALIGN_XXX_ID's. */

#define VIEW_COL_ALIGN_LEFT		0				/* Left justified */
#define VIEW_COL_ALIGN_RIGHT	1				/* Right justified */
#define VIEW_COL_ALIGN_CENTER	2				/* Centered */

/* View column display reading order.  */

#define VIEW_COL_LTR	0						/* Left To Right reading order */
#define VIEW_COL_RTL	1						/* Right To Left reading order */

/* Simple format data types, used to initialize dialog box to last "mode". */

#define VIEW_COL_NUMBER 0
#define VIEW_COL_TIMEDATE 1
#define VIEW_COL_TEXT 2

/*	Extended View column format descriptor.  One per column as of Notes V4.
	NOTE:  If you add variable data to this structure, store the packed,
	variable data AFTER the array of structures. */

typedef struct
	{
	WORD	Signature;								/* VIEW_COLUMN_FORMAT_SIGNATURE2 */
	FONTID	HeaderFontID;							/* FontID of column header. */
	UNID	ResortToViewUNID;						/* UNID of view to switch to. */
	WORD	wSecondResortColumnIndex;				/* 0 based index of secondary resort column. */
	WORD	Flags3;						
#define	VCF3_S_FlatInV5 					0		/* View is flat in V5 or greater */
#define	VCF3_M_FlatInV5 					0x0001
#define	VCF3_S_CaseSensitiveSortInV5		1		/* Case Sensitive sorting. */
#define	VCF3_M_CaseSensitiveSortInV5		0x0002
#define	VCF3_S_AccentSensitiveSortInV5		2		/* Accent Sensitive sorting. */
#define	VCF3_M_AccentSensitiveSortInV5		0x0004
#define VCF3_S_HideWhenFormula				3		/* Column has hide/when formula set */
#define VCF3_M_HideWhenFormula				0x0008	
#define VCF3_S_TwistieResource				4		
#define VCF3_M_TwistieResource				0x0010	
#define VCF3_S_Color						5		/* Column value to be used as a color for this entry. */
#define VCF3_M_Color						0x0020	
											/* 6 */
#define VCF3_ExtDate						0x0040	/*column has extended date info*/
											/* 7 */
#define VCF3_NumberFormat					0x0080	/*column has extended number format*/
#define VCF3_S_IsColumnEditable				8		/* v6 - can this column be edited */
#define VCF3_M_IsColumnEditable				0x0100	/* V6 - color col and user definable color */
#define VCF3_S_UserDefinableColor			9
#define VCF3_M_UserDefinableColor			0x0200	
#define VCF3_S_HideInR5						10		/* Column has hide/when formula set and needs to be hidden in R5 or before*/
#define VCF3_M_HideInR5						0x0400
#define VCF3_S_NamesFormat					11		/* Column has extended names format */
#define VCF3_M_NamesFormat					0x0800
#define VCF3_S_HideColumnTitle				12		/* Hide column title from display, but not from customization */
#define VCF3_M_HideColumnTitle				0x1000	

	WORD	wHideWhenFormulaSize;
	WORD	wTwistieResourceSize;
	WORD    wCustomOrder;					/* V6 View Customization support */
	WORD	wCustomHiddenFlags;				/* V6 View Customization support */
#define	VCF_HIDE_S_NormalView 					0
#define	VCF_HIDE_M_NormalView 					0x0001
#define	VCF_HIDE_S_CalFormatTwoDay				1
#define	VCF_HIDE_M_CalFormatTwoDay				0x0002
#define	VCF_HIDE_S_CalFormatOneWeek 			2
#define	VCF_HIDE_M_CalFormatOneWeek				0x0004
#define	VCF_HIDE_S_CalFormatTwoWeeks 			3
#define	VCF_HIDE_M_CalFormatTwoWeeks			0x0008
#define	VCF_HIDE_S_CalFormatOneMonth 			4
#define	VCF_HIDE_M_CalFormatOneMonth			0x0010
#define	VCF_HIDE_S_CalFormatOneYear 			5
#define	VCF_HIDE_M_CalFormatOneYear				0x0020
#define	VCF_HIDE_S_CalFormatOneDay 				6
#define	VCF_HIDE_M_CalFormatOneDay				0x0040
#define	VCF_HIDE_S_CalFormatWorkWeek 			7
#define	VCF_HIDE_M_CalFormatWorkWeek			0x0080
/* #define	VCF_HIDE_S_spare 						8 - 15 */
/* #define	VCF_HIDE_M_spare						0x0100 - 0x8000 */

	COLOR_VALUE	ColumnColor;				/* V6 - Column Text Color */
	COLOR_VALUE HeaderFontColor;			/* V6 - column header color */
	} VIEW_COLUMN_FORMAT2;

typedef struct
	{
	WORD	Signature;								/* VIEW_COLUMN_FORMAT_SIGNATURE3 */
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
	WORD	DatePreference;
	BYTE	bUnused;
	DWORD	Unused;
	} VIEW_COLUMN_FORMAT3;

typedef VIEW_COLUMN_FORMAT3 *PVIEW_COLUMN_FORMAT3;

typedef struct
	{
	WORD	Signature;								/* VIEW_COLUMN_FORMAT_SIGNATURE4 */
	/* Numeric symbol data */
	NFMT	NumberFormat;
	BYTE	NumSymPref;			/* NPREF_xxx */
	BYTE	NumSymFlags;		/* NNUMSYM_xxx */
	DWORD	DecimalSymLength;
	DWORD	MilliSepSymLength;
	DWORD	NegativeSymLength;
	WORD	MilliGroupSize;
	DWORD	Unused1;
	DWORD	Unused2;
	/* Currency data */
	BYTE	CurrencyPref;		/* NPREF_xxx */
	BYTE	CurrencyType;		/* NCURFMT_xxx */
	BYTE	CurrencyFlags;		/* NCURFMT_xxx */
	DWORD	CurrencySymLength;
	DWORD	ISOCountry;
	WORD	NumberPreference;	
	BYTE	bUnused;
	DWORD	Unused3;
	DWORD	Unused4;
	} VIEW_COLUMN_FORMAT4;

typedef VIEW_COLUMN_FORMAT4 *PVIEW_COLUMN_FORMAT4;


typedef struct
	{
	WORD	Signature;								/* VIEW_COLUMN_FORMAT_SIGNATURE5 */
	WORD	dwLength;								/* sizeof this structure + any extra data. */
	/* Names style formatting data. */
	DWORD	dwFlags;						
#define VCF5_S_IS_NAME				0				/* Column contains a name. */
#define VCF5_M_IS_NAME				0x00000001
#define VCF5_S_SHOW_IM_STATUS		1				/* Show IM online status in this column. */
#define VCF5_M_SHOW_IM_STATUS		0x00000002

	WORD	wDistNameColLen;						/* Length of programatic name of column that contains distiguished name. */
	WORD	wReserved;								/* Reserved for future use. */
	DWORD	dwReserved[4];							/* Reserved for future use. */
	} VIEW_COLUMN_FORMAT5;


#endif /* VIEWFMT_DEFS */


#ifdef __cplusplus
}
#endif

