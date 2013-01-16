#ifdef __cplusplus
extern "C" {
#endif


/*	Definitions available to all add-in menus*/

#ifndef ADDINMEN_DEFS
#define ADDINMEN_DEFS

#ifndef IX_DEFS
#include "ixview.h"
#endif

#ifndef IXEDIT_DEFS
#include "ixedit.h"
#endif

#ifdef UNIX
#define HWND			unsigned short
#define HMENU			HWND
#endif
#ifdef OS2
#define HMENU   		HWND
#endif

#define MAX_ADDIN_MENU_NAME 64

/*	Notes AddIn Menu operations. */

#define NAMM_INIT			0
#define NAMM_INITMENU		1
#define NAMM_COMMAND		2
#define NAMM_TERM			3

/*	Notes AddIn Menu return values. */

#define NAM_NOERROR			0
#define NAM_INIT_CONTINUE	1
#define NAM_INIT_STOP		2

/*	Notes AddIn menu messages. */

#define WM_ADDIN_FIRST		(WM_USER+400)
#define WM_ADDIN_REFRESH	(WM_ADDIN_FIRST+0)
#define WM_ADDIN_IMPORT		(WM_ADDIN_FIRST+1)

/*	Notes context flags */

#define NAM_IN_DESK			0
#define NAM_IN_VIEW			1
#define NAM_IN_VIEW_DESIGN	2
#define NAM_IN_EDIT_RO		3
#define NAM_IN_EDIT_RW		4
#define NAM_IN_EDIT_DESIGN	5


typedef WORD NAMRESULT;
typedef NAMRESULT (LNCALLBACKPTR NAMPROC)(WORD Command, void far *pParam);

typedef struct
	{
	EDITIMPORTDATA Import;
	EDITEXPORTDATA Export;
	/*	The following array is intended to be the maximum name length
		for a field.  This used to use DESIGN_FIELD_MAX+1 (from names.h)
		but the  value of DESIGN_LEVEL_MAX (which DESIGN_FIELD_MAX is derived
		from) was doubled from V3.0 to V3.0J to allow for longer Japanese
		design names.  The hardcoded value below is based on the V3.0 value
		and will allow API programs to be backward and forward compatible.
	*/
	char CaretFieldName[34];
	WORD CaretFieldType;
	} EDITIXDATA;

typedef struct
	{
	FLAG Context:3;		/* Desk, View, Editor R/W, Editor R/O */
	FLAG fCanExport:1;	/* TRUE if the add-in can perform an export. */
	FLAG fCanImport:1;	/* TRUE if the add-in can request an import. */
	union
		{
		EDITIXDATA Edit;
		VIEWIXDATA View;
		} IXData;
	} NAM_CONTEXT_DATA;

typedef struct
	{
	WORD wStartingID;     /* Input: DLL must add this to its ID. */
	WORD wMenuItemNumber; /* Input: Ascending ordinal number for each call. */

	WORD wMenuID;          /* Output: DLL's Menu ID. */
	char MenuItemName[MAX_ADDIN_MENU_NAME]; /* Output: Text of menu item. */
	} NAM_INIT_INFO;

typedef struct
	{
	HWND hNotesWnd;			/* Handle of Notes main window to use as parent. */
	WORD wMenuID;			/* DLL's ID number of selected menu item. */
	NAM_CONTEXT_DATA Data;	/* Context specific data */
	} NAM_COMMAND_INFO;

typedef struct
	{
	HMENU hMenu;			/* Handle of the Add-In PopUp menu. */
	NAM_CONTEXT_DATA Data;	/*	Context specific data. */
	} NAM_INITMENU_INFO;

#endif /* ADDINMEN_DEFS */



#ifdef __cplusplus
}
#endif

