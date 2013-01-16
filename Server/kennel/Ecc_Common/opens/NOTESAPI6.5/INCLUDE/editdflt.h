#ifdef __cplusplus
extern "C" {
#endif


#ifndef EDIT_DEFAULT_DEFS
#define EDIT_DEFAULT_DEFS

/*	Paragraph justification type codes */

#define	JUSTIFY_LEFT		0	/* flush left, ragged right */
#define	JUSTIFY_RIGHT		1	/* flush right, ragged left */
#define	JUSTIFY_BLOCK		2	/* full block justification */
#define JUSTIFY_CENTER		3	/* centered */
#define JUSTIFY_NONE		4	/* no line wrapping AT ALL (except hard CRs) */

/*	Paragraph reading order type codes */

#define	READING_ORDER_LEFT		0	/* left */
#define	READING_ORDER_RIGHT		1	/* right */

/*	Paragraph tab type codes */

#define TAB_LEFT			0	/* default - flush left starting at tab pos. */
#define TAB_RIGHT			1	/* text is right justified before tab pos. */
#define	TAB_DECIMAL			2	/* text is placed so that decimal point is aligned with tab pos. */
#define	TAB_CENTER			3	/* text is centered around tab pos. */
#define TAB_DEFAULT			TAB_LEFT

/*	Editor Field Types.

	These #defines are passed to addin menu API programs as well as being
	used internally by the editor.  They can never be changed.  */

#define FIELD_TYPE_ERROR		0
#define FIELD_TYPE_NUMBER		1
#define FIELD_TYPE_TIME			2
#define FIELD_TYPE_RICH_TEXT	3
#define FIELD_TYPE_AUTHORS		4
#define FIELD_TYPE_READERS		5
#define FIELD_TYPE_NAMES		6
#define FIELD_TYPE_KEYWORDS		7
#define FIELD_TYPE_TEXT	   		8
#define FIELD_TYPE_SECTION		9
#define FIELD_TYPE_PASSWORD		10
#define FIELD_TYPE_FORMULA		11
#define FIELD_TYPE_TIMEZONE		12

/*	using 18 because 13 through 17 are already defined in editres.h. */

#define FIELD_TYPE_COLORCTL		18


/*	Paragraph Flags */

#define	PABFLAG_PAGINATE_BEFORE	0x0001	/* start new page with this par */
#define	PABFLAG_KEEP_WITH_NEXT	0x0002	/* don't separate this and next par */
#define	PABFLAG_KEEP_TOGETHER	0x0004	/* don't split lines in paragraph */
#define	PABFLAG_PROPAGATE		0x0008	/* propagate even PAGINATE_BEFORE and KEEP_WITH_NEXT */
#define	PABFLAG_HIDE_RO			0x0010	/* hide paragraph in R/O mode */
#define	PABFLAG_HIDE_RW			0x0020	/* hide paragraph in R/W mode */
#define	PABFLAG_HIDE_PR			0x0040	/* hide paragraph when printing */
#define	PABFLAG_DISPLAY_RM		0x0080	/* in V4 and below, set if PAB.RightMargin (when nonzero)
											is to have meaning.  Turns out, is set iff para is in
											a table.  Anyway, V5+ no longer use this bit but it
											matters to V4 and below.  V5+ runs with this bit
											zeroed throughout runtime but, for backward
											compatibility, outputs it to disk at Save() time
											per whether paragraph is in a table.  */
#define PABFLAG_HIDE_UNLINK		0x0100	/* the pab was saved in V4.	*/
										/*	set this bit or the Notes client will assume the pab
											was saved pre-V4 and will thus "link" these bit
											definitions (assign the right one to the left one)
											since preview did not exist pre-V4:
												PABFLAG_HIDE_PV = PABFLAG_HIDE_RO
												PABFLAG_HIDE_PVE = PABFLAG_HIDE_RW */
#define	PABFLAG_HIDE_CO			0x0200	/* hide paragraph when copying/forwarding */
#define	PABFLAG_BULLET			0x0400	/* display paragraph with bullet */
#define PABFLAG_HIDE_IF			0x0800	/*  use the hide when formula
										   even if there is one.		*/
#define	PABFLAG_NUMBEREDLIST	0x1000	/* display paragraph with number */
#define PABFLAG_HIDE_PV			0x2000	/* hide paragraph when previewing*/
#define PABFLAG_HIDE_PVE		0x4000	/* hide paragraph when editing
											in the preview pane.		*/
#define PABFLAG_HIDE_NOTES		0x8000	/* hide paragraph from Notes clients */

#define PABFLAG_HIDEBITS (PABFLAG_HIDE_RO | PABFLAG_HIDE_RW | PABFLAG_HIDE_CO | PABFLAG_HIDE_PR | PABFLAG_HIDE_PV | PABFLAG_HIDE_PVE \
						| PABFLAG_HIDE_IF | PABFLAG_HIDE_NOTES)

#define TABLE_PABFLAGS			( PABFLAG_KEEP_TOGETHER \
								| PABFLAG_KEEP_WITH_NEXT)

/* Extra Paragraph Flags (stored in Flags2 field) */

#define PABFLAG2_HIDE_WEB		0x0001
#define PABFLAG2_CHECKEDLIST	0x0002	
#define PABFLAG2_LM_OFFSET		0x0004	/* PAB.LeftMargin is an offset value. */
#define PABFLAG2_LM_PERCENT		0x0008	/* PAB.LeftMargin is a percentage value. */
#define PABFLAG2_FLLM_OFFSET	0x0010	/* PAB.LeftMargin is an offset value. */
#define PABFLAG2_FLLM_PERCENT	0x0020	/* PAB.LeftMargin is a percentage value. */
#define PABFLAG2_RM_OFFSET		0x0040	/* PAB.RightMargin is an offset value.   */
#define PABFLAG2_RM_PERCENT		0x0080	/* PAB.RightMargin is a percentage value.   */
#define PABFLAG2_LM_DEFAULT		0x0100	/* If to use default value instead of PAB.LeftMargin. */
#define PABFLAG2_FLLM_DEFAULT	0x0200	/* If to use default value instead of PAB.FirstLineLeftMargin. */
#define PABFLAG2_RM_DEFAULT		0x0400	/* If to use default value instead of PAB.RightMargin. */
#define PABFLAG2_CIRCLELIST		0x0800	
#define PABFLAG2_SQUARELIST		0x1000	
#define PABFLAG2_UNCHECKEDLIST	0x2000	
#define PABFLAG2_BIDI_RTLREADING 0x4000	/* set if right to left reading order */
#define PABFLAG2_MORE_FLAGS		0x8000	/* TRUE if Pab needs to Read more Flafs*/

#define PABFLAG2_HIDEBITS (PABFLAG2_HIDE_WEB)

#define PABFLAG2_CHECKLIST		(PABFLAG2_UNCHECKEDLIST | PABFLAG2_CHECKEDLIST)	

#define PABFLAG2_MARGIN_DEFAULTS_MASK	( PABFLAG2_LM_DEFAULT  \
										| PABFLAG2_RM_DEFAULT  \
										| PABFLAG2_FLLM_DEFAULT	)

#define PABFLAG2_MARGIN_MASK			( PABFLAG2_MARGIN_STYLES_MASK  \
										| PABFLAG2_MARGIN_DEFAULTS_MASK )

#define PABFLAG2_MARGIN_STYLES_MASK	( PABFLAG2_LM_OFFSET  \
									| PABFLAG2_LM_PERCENT \
									| PABFLAG2_FLLM_OFFSET  \
									| PABFLAG2_FLLM_PERCENT \
									| PABFLAG2_RM_OFFSET  \
									| PABFLAG2_RM_PERCENT	)
#define PABFLAG2_ROMANUPPERLIST (PABFLAG2_CHECKEDLIST | PABFLAG2_CIRCLELIST)
#define PABFLAG2_ROMANLOWERLIST (PABFLAG2_CHECKEDLIST | PABFLAG2_SQUARELIST)
#define PABFLAG2_ALPHAUPPERLIST (PABFLAG2_SQUARELIST | PABFLAG2_CIRCLELIST)
#define PABFLAG2_ALPHALOWERLIST (PABFLAG2_CHECKEDLIST | PABFLAG2_SQUARELIST | PABFLAG2_CIRCLELIST)

/*	Table Flags */
#define	TABFLAG_AUTO_CELL_WIDTH	0x0001	/* Cells grow/shrink to fill window */

/* Cell Flags */

#define CELLFLAG_USE_BKGCOLOR	0x01	/* Cell uses background color */

/*	One Inch */

#ifndef ONEINCH
#define	ONEINCH (20*72)			/* One inch worth of TWIPS */
#endif

/*	Paragraph Attribute Block Default Settings */

#define	DEFAULT_JUSTIFICATION		JUSTIFY_LEFT
#define	DEFAULT_LINE_SPACING		0
#define	DEFAULT_ABOVE_PAR_SPACING	0
#define	DEFAULT_BELOW_PAR_SPACING	0
#define	DEFAULT_LEFT_MARGIN			ONEINCH
#define	DEFAULT_FIRST_LEFT_MARGIN	DEFAULT_LEFT_MARGIN
#define	DEFAULT_RIGHT_MARGIN		0
/* Note: Right Margin = "0" means [DEFAULT_RIGHT_GUTTER] inches from */
/* right edge of paper, regardless of paper width. */
#define	DEFAULT_RIGHT_GUTTER		ONEINCH
#define DEFAULT_PAGINATION			0
#define DEFAULT_FLAGS2				0
#define DEFAULT_MARGIN_STYLE		(PABFLAG2_LM_OFFSET  |  PABFLAG2_RM_OFFSET)
/* Note: tabs are relative to the absolute left edge of the paper. */
#define	DEFAULT_TABS				0
#define	DEFAULT_TAB_INTERVAL		(ONEINCH/2)
#define	DEFAULT_TABLE_HCELLSPACE	0
#define DEFAULT_TABLE_VCELLSPACE	0


#define DEFAULT_LAYOUT_LEFT			DEFAULT_LEFT_MARGIN
#define DEFAULT_LAYOUT_WIDTH		(ONEINCH * 6)
#define DEFAULT_LAYOUT_HEIGHT		(3 * ONEINCH / 2)
#define MIN_LAYOUT_WIDTH			(ONEINCH / 4)
#define MIN_LAYOUT_HEIGHT			(ONEINCH / 4)

#define DEFAULT_LAYOUT_ELEM_WIDTH	(4 * ONEINCH / 3)	/*	1.333 inch */
#define DEFAULT_LAYOUT_ELEM_HEIGHT	(ONEINCH / 5)
#define MIN_ELEMENT_WIDTH			(ONEINCH / 8)
#define MIN_ELEMENT_HEIGHT			(ONEINCH / 8)


/*	Field Attribute Block Default Settings */

#define	DEFAULT_FAB_DATATYPE (TYPE_TEXT)
#define	DEFAULT_FAB_FLAGS (FEDITABLE|FSTOREDV|FV3FAB)
#define	DEFAULT_FAB_LISTDELIM (LDDELIM_SEMICOLON)
#define DEFAULT_FAB_NFMT_DIGITS 2
#define DEFAULT_FAB_NFMT_FORMAT NFMT_GENERAL
#define DEFAULT_FAB_NFMT_ATTRIBUTES 0
#define DEFAULT_FAB_TFMT_TDFMT TDFMT_FULL
#define DEFAULT_FAB_TFMT_TTFMT TTFMT_FULL
#define DEFAULT_FAB_TFMT_TZFMT TZFMT_NEVER
#define DEFAULT_FAB_TFMT_TSFMT TSFMT_DATETIME

/* Horizontal Rule Defaults				*/

#define DEFAULTHRULEHEIGHT	7
#define DEFAULTHRULEWIDTH	720


#define DEFAULTPLACEHOLDERHEIGHT	96
#define DEFAULTPLACEHOLDERWIDTH		720

#endif

#ifdef __cplusplus
}
#endif

