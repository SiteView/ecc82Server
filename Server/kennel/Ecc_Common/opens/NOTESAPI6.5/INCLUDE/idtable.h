#ifdef __cplusplus
extern "C" {
#endif

/*	ID Table Routines
*
*	This package is used to create and manipulate tables that contain
*	compressed double-word values that typically represent IDs.  The
*	primitives allow the caller to create an ID table, add or delete IDs,
*	and query for the presence of an ID.
*
*	Compression of the table is achieved by virtue of the fact that it
*	is assumed that the ID space is relatively "regular", that is, that
*	ID values differ from each other by some regular value, say 4.
*
*	ID tables are always stored in Canonical format.
*
*	(This .H file is global so that the ODS routines can access it; all
*	 access to the following structures should be via the programmatic
*	 interfaces provided.)
*/

#ifndef IDTABLE_DEFS
#define IDTABLE_DEFS


#define IDTABLE_MODIFIED 	0x0001	/* modified - set by Insert/Delete */
									/* and can be cleared by caller if desired */
#define IDTABLE_INVERTED	0x0002	/* sense of list inverted */
									/* (reserved for use by caller only) */

/*	Function Declarations */

STATUS LNPUBLIC IDCreateTable (DWORD Alignment, HANDLE far *rethTable);
STATUS LNPUBLIC IDDestroyTable(HANDLE hTable);
STATUS 	LNPUBLIC IDInsert (HANDLE hTable, DWORD id, BOOL far *retfInserted);
STATUS 	LNPUBLIC IDDelete (HANDLE hTable, DWORD id, BOOL far *retfDeleted);
STATUS 	LNPUBLIC IDDeleteAll (HANDLE hTable);
BOOL	LNPUBLIC IDScan (HANDLE hTable, BOOL fFirst, DWORD far *retID);
typedef STATUS (LNCALLBACKPTR IDENUMERATEPROC)
					(void far *Parameter, DWORD id);
STATUS	LNPUBLIC IDEnumerate (HANDLE hTable,
								IDENUMERATEPROC Routine,
								void far *Parameter);
DWORD	LNPUBLIC IDEntries (HANDLE hTable);
BOOL	LNPUBLIC IDIsPresent (HANDLE hTable, DWORD id);
DWORD	LNPUBLIC IDTableSize (HANDLE hTable);
STATUS	LNPUBLIC IDTableCopy (HANDLE hTable, HANDLE far *rethTable);
DWORD	LNPUBLIC IDTableSizeP (void far *pIDTable);
WORD	LNPUBLIC IDTableFlags (void far *pIDTable);
TIMEDATE LNPUBLIC IDTableTime (void far *pIDTable);
void	LNPUBLIC IDTableSetFlags (void far *pIDTable, WORD Flags);
void	LNPUBLIC IDTableSetTime (void far *pIDTable, TIMEDATE Time);
STATUS  LNPUBLIC IDTableIntersect	(HANDLE hSrc1Table,HANDLE hSrc2Table,HANDLE *rethDstTable);
BOOL	LNPUBLIC IDAreTablesEqual	(HANDLE hSrc1Table, HANDLE hSrc2Table);
STATUS	LNPUBLIC IDDeleteTable  (HANDLE hTable, HANDLE hIDsToDelete);
STATUS	LNPUBLIC IDInsertTable  (HANDLE hTable, HANDLE hIDsToAdd);


#endif


/*	End of ID Table Routines */

#ifdef __cplusplus
}
#endif

