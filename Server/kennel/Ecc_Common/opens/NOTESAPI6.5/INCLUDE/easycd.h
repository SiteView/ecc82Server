#ifdef __cplusplus
extern "C" {
#endif

#ifndef EASYCD_DEFS
#define EASYCD_DEFS

#ifndef EDIT_ODS_DEFS
#include "editods.h"					/* for MAXTABS */
#endif


#include "nls.h"


/*	Compound style datatype.  Similiar to a CDPABDEFINITION. */

typedef struct
	{
	WORD	JustifyMode;
	WORD	LineSpacing;
	WORD	ParagraphSpacingBefore;
	WORD	ParagraphSpacingAfter;
	WORD	LeftMargin;
	WORD	RightMargin;
	WORD	FirstLineLeftMargin;
	WORD	Tabs;
	SWORD	Tab[MAXTABS];
	WORD	Flags;
	} COMPOUNDSTYLE;


/*	Flags for CompoundTextAddText. */

#define COMP_FROM_FILE			0x00000001
#define	COMP_PRESERVE_LINES		0x00000002
#define	COMP_PARA_LINE			0x00000004
#define COMP_PARA_BLANK_LINE	0x00000008
#define COMP_SERVER_HINT_FOLLOWS 0x00000010



/*	Use this style ID in CompoundTextAddText to continue using the
	same paragraph style as the previous paragraph. */

#define STYLE_ID_SAMEASPREV		0xFFFFFFFF
 	
/*	Function prototypes. */

STATUS LNPUBLIC CompoundTextCreate
			(
			NOTEHANDLE	hNote,
			char	far *pszItemName,
			HANDLE	far *phCompound
			);
STATUS LNPUBLIC CompoundTextClose
			(
			HANDLE		hCompound, 
			HANDLE	far *phReturnBuffer, 
			DWORD	far *pdwReturnBufferSize,
			char	far *pchReturnFile,
			WORD		wReturnFileSize
			);
void LNPUBLIC CompoundTextDiscard
			(
			HANDLE hCompound
			);

STATUS LNPUBLIC CompoundTextDefineStyle
			(
			HANDLE			hCompound, 
			char		far *pszStyleName, 
			COMPOUNDSTYLE far *pDefinition,
			DWORD		far *pdwStyleId
			);


STATUS LNPUBLIC CompoundTextAssimilateItem
			(
			HANDLE			hCompound,
			NOTEHANDLE		hNote,
			char		far *pszItemName,
			DWORD			dwFlags
			);
STATUS LNPUBLIC CompoundTextAssimilateFile
			(
			HANDLE			hCompound,
			char		far *pszFileSpec,
			DWORD			dwFlags
			);
STATUS LNPUBLIC CompoundTextAddParagraphExt
			(
			HANDLE			hCompound,
			DWORD			dwStyleId,
			FONTID			FontID,
			char			*pchText,
			DWORD			dwTextLen,
			NLS_PINFO		pInfo
			);
STATUS LNPUBLIC CompoundTextAddTextExt
			(
			HANDLE			hCompound,
			DWORD			dwStyleId,
			FONTID			FontID,
			char		far *pchText,
			DWORD			dwTextLen,
			char		far *pszLineDelim,
			DWORD			dwFlags,
			NLS_PINFO		pInfo
			);
void LNPUBLIC CompoundTextInitStyle
			(
			COMPOUNDSTYLE far *pStyle
			);


STATUS LNPUBLIC CompoundTextAddDocLink
			(
			HANDLE			hCompound,
			TIMEDATE		DBReplicaID,
			UNID			ViewUNID,
			UNID			NoteUNID,
			char		far *pszComment,
			DWORD			dwFlags
			);

STATUS LNPUBLIC CompoundTextAddRenderedNote 
			(
			HANDLE 			hCompound,
			NOTEHANDLE 		hNote,
			NOTEHANDLE 		hFormNote,
		   	DWORD 			dwFlags
			);

STATUS LNPUBLIC CompoundTextAddCDRecords
			(
			HANDLE	hCompound,
			void *	pvRecord,
			DWORD		dwRecordLength
			);
STATUS LNPUBLIC SubformInsert (DBHANDLE hDB, char *pSubForm, char *pForm, DWORD Flags);
STATUS LNPUBLIC SubformRemove (DBHANDLE hDB, char *pSubForm, char *pForm, DWORD Flags);

#endif




#ifdef __cplusplus
}
#endif

