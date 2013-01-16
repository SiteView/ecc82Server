/*	ODS storage of a notes resource */


#ifndef RSRCODS_DEFS
#define RSRCODS_DEFS

#ifdef __cplusplus
extern "C" {
#endif

#include "ods.h"

typedef struct {
	WSIG	Header;
	DWORD 	Flags;				/* one of CDRESOURCE_FLAGS_ */
	WORD 	Type;				/* one of CDRESOURCE_TYPE_ */
	WORD	ResourceClass;		/* one of CDRESOURCE_CLASS_ */
	WORD	Length1;			/* meaning depends on Type */
	WORD	ServerHintLength;		/* length of the server hint */
	WORD	FileHintLength;		/* length of the file hint */
	BYTE 	Reserved[8];
	/*	Variable length follows:
	* 	String of size ServerHintLength, a hint as to the resource's server
	* 	String of size FileHintLength, a hint as to the resource's file
	*	- if CDRESOURCE_TYPE_URL : 
	*		string of size Length1 follows, the URL.
	*	- if CDRESOURCE_TYPE_NOTELINK : 
	*		if CDRESOURCE_FLAGS_NOTELINKINLINE is NOT set in Flags then this data is
	*			WORD LinkID, index into $Links
	*			string of size Length1 follows, the anchor name (optional)
	*		if CDRESOURCE_FLAGS_NOTELINKINLINE is set in Flags then this data is
	*			NOTELINK NoteLink
	*			string of size Length1 follows, the anchor name (optional)
	*	- if CDRESOURCE_TYPE_NAMEDELEMENT :
	*		TIMEDATE ReplicaID;		if element is in a different database - zero if current db
	*		string of size Length1 follows, the name of element
	*/
} CDRESOURCE;

/*	Flags
*/
#define CDRESOURCE_FLAGS_FORMULA			0x00000001	/* the type's data is a formula 
												*  valid for _TYPE_URL and
												*			_TYPE_NAMEDELEMENT
												*/

#define CDRESOURCE_FLAGS_NOTELINKINLINE 	0x00000002 /* the notelink variable length data
													* contains the notelink itself not
												 	* an index into a $Links items
													*/
#define CDRESOURCE_FLAGS_ABSOLUTE			0x00000004 /* If specified, the link
														is to an absolute 
														database or thing.
														Used to make a hard
														link to a specific DB. */
#define CDRESOURCE_FLAGS_USEHINTFIRST		0x00000008 /* If specified, the server
														and file hint are filled
														in and should be 
														attempted before trying
														other copies. */

#define CDRESOURCE_FLAGS_CANNEDIMAGE		0x00000010	/* the type's data is a canned image file (data/domino/icons/[*].gif)
														*  valid for _TYPE_URL && _CLASS_IMAGE only
														*/
/*	NOTE: _PRIVATE_DATABASE and _PRIVATE_DESKTOP are mutually exclusive. */
#define CDRESOURCE_FLAGS_PRIVATE_DATABASE	0x00000020	/* the object is private in its database */
#define CDRESOURCE_FLAGS_PRIVATE_DESKTOP 	0x00000040	/* the object is private in the desktop database */

#define CDRESOURCE_FLAGS_REPLICA_WILDCARD 	0x00000080	/* the replica in the CD resource
															needs to be obtained
															via RLGetReplicaID
															to handle special
															replica IDs like
															'current' mail file. */
#define CDRESOURCE_FLAGS_SIMPLE				0x00000100	/* used with class view and folder to mean "Simple View" */
#define CDRESOURCE_FLAGS_DESIGN_MODE		0x00000200	/* open this up in design mode */

#define CDRESOURCE_FLAGS_RESERVED1			0x10000000 /* reserved meaning for each resource link class */
#define CDRESOURCE_FLAGS_RESERVED2			0x20000000 /* reserved meaning for each resource link class */
#define CDRESOURCE_FLAGS_RESERVED3			0x40000000 /* reserved meaning for each resource link class */
#define CDRESOURCE_FLAGS_RESERVED4			0x80000000 /* reserved meaning for each resource link class */


/* Types of CDRESOURCE
*/
#define CDRESOURCE_TYPE_EMPTY			0
#define CDRESOURCE_TYPE_URL				1
#define CDRESOURCE_TYPE_NOTELINK		2
#define CDRESOURCE_TYPE_NAMEDELEMENT	3
#define CDRESOURCE_TYPE_NOTEIDLINK		4	/* Currently not written to disk only used in RESOURCELINK */
#define CDRESOURCE_TYPE_ACTION			5		/* This would be used in 
													conjunction with the 
													formula flag.  The formula
													is an @Command that would
													perform some action, typically
													it would also switch to a 
													Notes UI element.
												   This will be used to reference
													the replicator page and other
													UI elements. */
#define CDRESOURCE_TYPE_NAMEDITEMELEMENT 6 	/* Currently not written to disk only used in RESOURCELINK */

#define CDRESOURCE_TYPE_RESERVERS	 32000	/* And above...  See comment below. */

/*	Sitemaps/Outlines use the same type identifiers as resource links.
	However, there are some types that are special to an outline, and
		we want to reserve an upper range for thos special types.

	For now, reserve the entire upper range 32,000 and up for them.
	The IDs are started at MAXWORD and work their way down.   
*/


/* Classes of resource linked to by CDRESOURCE
*/
#define CDRESOURCE_CLASS_UNKNOWN		0
#define CDRESOURCE_CLASS_DOCUMENT		1
#define CDRESOURCE_CLASS_VIEW			2
#define CDRESOURCE_CLASS_FORM			3
#define CDRESOURCE_CLASS_NAVIGATOR		4
#define CDRESOURCE_CLASS_DATABASE		5
#define CDRESOURCE_CLASS_FRAMESET		6
#define CDRESOURCE_CLASS_PAGE			7
#define CDRESOURCE_CLASS_IMAGE		8
#define CDRESOURCE_CLASS_ICON			9
#define CDRESOURCE_CLASS_HELPABOUT		10
#define CDRESOURCE_CLASS_HELPUSING		11
#define CDRESOURCE_CLASS_SERVER		12
#define CDRESOURCE_CLASS_APPLET		13
#define CDRESOURCE_CLASS_FORMULA		14	/* A compiled formula someplace */
#define CDRESOURCE_CLASS_AGENT		15
#define CDRESOURCE_CLASS_FILE			16	/* a file on disk (file:) */
#define CDRESOURCE_CLASS_FILEATTACHMENT	17	/* A file attached to a note */
#define CDRESOURCE_CLASS_OLEEMBEDDING	18
#define CDRESOURCE_CLASS_SHAREDIMAGE	19	/* A shared image resource */
#define CDRESOURCE_CLASS_FOLDER			20
#define CDRESOURCE_CLASS_PORTFOLIO		21	/* An old (4.6) or new style portfolio.
												Which gets incorporated into
												the bookmark bar as a tab, rather
												than getting opened as a database. */
#define CDRESOURCE_CLASS_OUTLINE		22

#ifdef __cplusplus
}
#endif

#endif

