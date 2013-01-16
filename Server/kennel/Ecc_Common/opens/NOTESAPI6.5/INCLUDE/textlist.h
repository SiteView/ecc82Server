#ifdef __cplusplus
extern "C" {
#endif

#ifndef TEXTLIST_DEFS
#define TEXTLIST_DEFS


/*	Text list functions. */

#ifndef NSF_DATA_DEFS
#include "nsfdata.h"					/* Get LIST */
#endif

STATUS LNPUBLIC ListAllocate (WORD ListEntries,
								WORD TextSize,
								BOOL fPrefixDataType,
								HANDLE far *rethList,
								void far *retpList,
								WORD far *retListSize);

STATUS LNPUBLIC ListAddText (void far *pList,
								BOOL fPrefixDataType,
								WORD EntryNumber,
								const char far *Text,
								WORD TextSize);


STATUS LNPUBLIC ListGetText (void far *pList,
								BOOL fPrefixDataType,
								WORD EntryNumber,
								char far * far *retTextPointer,
								WORD far *retTextLength);



STATUS LNPUBLIC ListRemoveEntry (HANDLE hList,
								BOOL fPrefixDataType,
								WORD far *pListSize,
								WORD EntryNumber);

STATUS LNPUBLIC ListRemoveAllEntries (HANDLE hList,
								BOOL fPrefixDataType,
								WORD far *pListSize);


STATUS LNPUBLIC ListAddEntry (HANDLE hList,
								BOOL fPrefixDataType,
								WORD far *pListSize,
								WORD EntryNumber,
								const char far *Text,
								WORD TextSize);


WORD   LNPUBLIC ListGetSize (void far *pList,
								BOOL fPrefixDataType);

STATUS LNPUBLIC ListDuplicate(LIST far *pInList,
								BOOL	fNoteItem,
								HANDLE far *phOutList);




								
WORD LNPUBLIC ListGetNumEntries (void far *vList, BOOL NoteItem);

#endif

#ifdef __cplusplus
}
#endif
