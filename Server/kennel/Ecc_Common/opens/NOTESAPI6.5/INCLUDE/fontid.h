#ifdef __cplusplus
extern "C" {
#endif


/*	Font ID Definitions */

#ifndef FONTID_DEFS
#define FONTID_DEFS

/*	A font ID is a DWORD which is sub-divided into 4 byte fields.  The
	FONTID datatype is defined by "global.h" so it is more commonly
	available. */


/*	Font ID sub-fields */

typedef struct {
#ifdef LITTLE_ENDIAN_ORDER
	BYTE Face;						/* Font face (FONT_FACE_xxx) */
	BYTE Attrib;					/* Attributes (ISBOLD,etc - see below) */
	BYTE Color;						/* Color index (FONT_COLOR_xxx) */
	BYTE PointSize;					/* Size of font in points */
#else
	BYTE PointSize;					/* Size of font in points */
	BYTE Color;						/* Color index (FONT_COLOR_xxx) */
	BYTE Attrib;					/* Attributes (ISBOLD,etc - see below) */
	BYTE Face;						/* Font face (FONT_FACE_xxx) */
#endif
} FONTIDFIELDS;

/*	Font Union */

typedef union FID {
	FONTIDFIELDS x;
	FONTID FontID;
}FID;

/*	Define the first 5 entries of the font table.  These entries are
	used for two things:  First, they maintain compatibility with previous
	versions, before we had font tables.  Second, they provide a limited
	capability (used in the menu processing) for hard-coded font IDs,
	since these IDs are always the same in a running system.  Note: The
	only one of these definitions that really NEEDS to be external is
	STATIC_FONT_FACES, which is used as an optimization by the editor so
	that it doesn't need to generate a font table entry for these, the
	most frequently used fonts. */

#define	FONT_FACE_ROMAN				0		/* (e.g. Times Roman family) */
#define	FONT_FACE_SWISS				1		/* (e.g. Helv family) */
#define FONT_FACE_UNICODE			2		/* (e.g. Monotype Sans WT) */
#define FONT_FACE_USERINTERFACE		3		/* (e.g. Arial */
#define	FONT_FACE_TYPEWRITER		4		/* (e.g. Courier family) */
#define	STATIC_FONT_FACES			5

#define MAXFACESIZE 32			/* same as LF_FACESIZE in WINDOWS.H */

/*	Font Style Bit Fields */

#define ISBOLD		0x01
#define	ISITALIC	0x02
#define	ISUNDERLINE	0x04
#define	ISSTRIKEOUT	0x08
#define	ISSUPER		0x10
#define	ISSUB		0x20
#define	ISEFFECT	0x80		/* Used for implementation of special effect styles */
#define	ISSHADOW	0x80		/* Used for implementation of special effect styles */
#define	ISEMBOSS	0x90		/* Used for implementation of special effect styles */
#define	ISEXTRUDE	0xa0		/* Used for implementation of special effect styles */

/*	Define static font IDs */

#define NULLFONTID 0L					/* Used to mean "no font selected" */
#define DEFAULT_SMALL_FONT_ID	FontSetFaceID(FontSetSize(NULLFONTID,9),FONT_FACE_SWISS)
#define DEFAULT_FONT_ID			FontSetFaceID(FontSetSize(NULLFONTID,10),FONT_FACE_SWISS)
#define DEFAULT_BOLD_FONT_ID	FontSetBold(DEFAULT_FONT_ID)
#define FOREIGN_FONT_ID			FontSetFaceID(FontSetSize(NULLFONTID,10),FONT_FACE_TYPEWRITER)


/* 	the default permanent pen font ID is a bold, red version of the default font.
	we can't guarantee red, but the color index 2 defaults to a red color so it's a good
	guess. */
	
#define DEFAULT_PPEN_FONT_ID	FontSetColor(FontSetBold(DEFAULT_FONT_ID),(BYTE)NOTES_COLOR_RED)

/*	Define some common functions as macros */

#define BYTEMASK(leftshift) ((DWORD)((DWORD)0x000000ff << (leftshift)))

#define FONT_SIZE_SHIFT		24
#define FONT_COLOR_SHIFT	16
#define FONT_STYLE_SHIFT	8
#define FONT_FACE_SHIFT		0

#define FONT_SHADOW_VALUE	14

#define FontGetSize(fontid) ((BYTE)(((fontid) >> FONT_SIZE_SHIFT) & 0xff))
#define FontSetSize(fontid,size) (((DWORD)(fontid) & ~BYTEMASK(FONT_SIZE_SHIFT)) | ((DWORD)(size)<<FONT_SIZE_SHIFT))
#define FontGetColor(fontid) ((BYTE)(((fontid) >> FONT_COLOR_SHIFT) & 0xff))
#define FontSetColor(fontid,colorid) (((DWORD)(fontid) & ~BYTEMASK(FONT_COLOR_SHIFT)) | ((DWORD)(colorid)<<FONT_COLOR_SHIFT))
#define FontGetStyle(fontid) ((BYTE)(((fontid) >> FONT_STYLE_SHIFT) & 0xff))
#define FontSetStyle(fontid,styleid) (((DWORD)(fontid) & ~BYTEMASK(FONT_STYLE_SHIFT)) | ((DWORD)(styleid)<<FONT_STYLE_SHIFT))
#define FontGetFaceID(fontid) ((BYTE)(((fontid) >> FONT_FACE_SHIFT) & 0xff))
#define FontSetFaceID(fontid,faceid) (((DWORD)(fontid) & ~BYTEMASK(FONT_FACE_SHIFT)) | ((DWORD)(faceid)<<FONT_FACE_SHIFT))

#define FontIsUnderline(fontid) (((fontid) & ((DWORD) ISUNDERLINE << FONT_STYLE_SHIFT)) != 0)
#define FontIsItalic(fontid) (((fontid) & ((DWORD) ISITALIC << FONT_STYLE_SHIFT)) != 0)
#define FontIsBold(fontid) (((fontid) & ((DWORD) ISBOLD << FONT_STYLE_SHIFT)) != 0)

#define FontSetItalic(fontid) ((DWORD)(fontid) | (ISITALIC << FONT_STYLE_SHIFT))
#define FontClearItalic(fontid) ((DWORD)(fontid) & ~(ISITALIC << FONT_STYLE_SHIFT))

#define FontSetBold(fontid) ((DWORD)(fontid) | (ISBOLD << FONT_STYLE_SHIFT))
#define FontClearBold(fontid) ((DWORD)(fontid) & ~(ISBOLD << FONT_STYLE_SHIFT))

#define FontSetUnderline(fontid) ((DWORD)(fontid) | (ISUNDERLINE << FONT_STYLE_SHIFT))
#define FontClearUnderline(fontid) ((DWORD)(fontid) & ~(ISUNDERLINE << FONT_STYLE_SHIFT))

#define FontSetStrikeOut(fontid) ((DWORD)(fontid) | (ISSTRIKEOUT << FONT_STYLE_SHIFT))
#define FontClearStrikeOut(fontid) ((DWORD)(fontid) & ~(ISSTRIKEOUT << FONT_STYLE_SHIFT))

#define FontSetSuperScript(fontid) ((DWORD)(fontid) | (ISSUPER << FONT_STYLE_SHIFT))
#define FontClearSuperScript(fontid) ((DWORD)(fontid) & ~(ISSUPER << FONT_STYLE_SHIFT))

#define FontSetSubScript(fontid) ((DWORD)(fontid) | (ISSUB << FONT_STYLE_SHIFT))
#define FontClearSubScript(fontid) ((DWORD)(fontid) & ~(ISSUB << FONT_STYLE_SHIFT))

#define FontIsStrikeOut(fontid) (((fontid) & ((DWORD) ISSTRIKEOUT << FONT_STYLE_SHIFT)) != 0)
#define FontIsSuperScript(fontid) (((fontid) & ((DWORD)(ISEFFECT|ISSUPER) << FONT_STYLE_SHIFT)) == (DWORD)(ISSUPER << FONT_STYLE_SHIFT))
#define FontIsSubScript(fontid) (((fontid) & ((DWORD)(ISEFFECT|ISSUB) << FONT_STYLE_SHIFT)) == (DWORD)(ISSUB << FONT_STYLE_SHIFT))
#define FontIsPlain(fontid) (FontGetStyle(fontid) == 0)
#define FontSetPlain(fontid) (FontSetStyle(fontid,0))

#define FontIsEffect(fontid) (((fontid) & ((DWORD) ISEFFECT << FONT_STYLE_SHIFT)) != 0)
#define FontIsShadow(fontid) (((fontid) & ((DWORD) (ISSHADOW|ISEMBOSS|ISEXTRUDE) << FONT_STYLE_SHIFT)) == ((DWORD) ISSHADOW << FONT_STYLE_SHIFT))

#define FontIsEmboss(fontid) (((fontid) & ((DWORD) ISEMBOSS << FONT_STYLE_SHIFT)) == ((DWORD) ISEMBOSS << FONT_STYLE_SHIFT))
#define FontIsExtrude(fontid) (((fontid) & ((DWORD) ISEXTRUDE << FONT_STYLE_SHIFT)) == ((DWORD) ISEXTRUDE << FONT_STYLE_SHIFT))

#define FontSetShadow(fontid) ((DWORD)(fontid) | (ISSHADOW << FONT_STYLE_SHIFT))
#define FontClearShadow(fontid) ((DWORD)(fontid) & ~(ISSHADOW << FONT_STYLE_SHIFT))
#define FontSetEmboss(fontid) ((DWORD)(fontid) | (ISEMBOSS << FONT_STYLE_SHIFT))
#define FontClearEmboss(fontid) ((DWORD)(fontid) & ~(ISEMBOSS << FONT_STYLE_SHIFT))
#define FontSetExtrude(fontid) ((DWORD)(fontid) | (ISEXTRUDE << FONT_STYLE_SHIFT))
#define FontClearExtrude(fontid) ((DWORD)(fontid) & ~(ISEXTRUDE << FONT_STYLE_SHIFT))
#define FontClearEffect(fontid) ((DWORD)(fontid) & ~((ISEMBOSS|ISEXTRUDE) << FONT_STYLE_SHIFT))
#define FontGetShadowOffset(fontid) (BYTE)(FontGetSize(fontid)/(BYTE)FONT_SHADOW_VALUE + 1)


#endif


#ifdef __cplusplus
}
#endif


