
/*	inc\ oleods.h - This header file contains typedefs and structures stored on disk
					by Notes and used in Notes OLE.

	NOTE:  This file can have NO dependencies on Microsoft OLE since it must be available
			on ALL platforms.

 */


#ifndef OLE_ODS_DEFS
#define OLE_ODS_DEFS

#ifndef ODS_DEFS
#include "ods.h"
#endif

#ifndef EDIT_ODS_DEFS
#include "editods.h"	/* for DDEFORMAT_??? definitions */
#endif

/*	Name of a form autolaunch item.  This optional item is created when
	designing a Notes form using the auto launch options. */

#define	FORM_AUTOLAUNCH_ITEM "$AUTOLAUNCH"

/*	Name of an OLE object item.  One of these is created for every
	OLE embedded object that exists in a Notes document.  This item
	is used to access OLE objects witout having to parse the
	Rich Text item within the document to find an OLE CD record */

#define OLE_OBJECT_ITEM "$OLEOBJINFO"


/*	On-disk structure of an OLE GUID.  This is taken directly from 
	OLE's compobj.h.  The reason it's copied rather than included
	here is to eliminate inclusion of the OLE2 header files, which
	without great pain, only compile on OLE platforms.  This header file
	is included on ALL Notes platforms, so we don't want to mess with the
	whole of OLE just for the GUID typedef... */

typedef struct 
{
    DWORD Data1;
    WORD  Data2;
    WORD  Data3;
    BYTE  Data4[8];
} OLE_GUID;


/*	Format of an on-disk autolaunch item. Most of the info contained in
	this structure refer to OLE autolaunching behaviors, but there are
	some  */

typedef struct
{
	WSIG		Header;				/* Signature and length of this record */
	DWORD		ObjectType;			/* Type of object to launch, see OBJECT_TYPE_??? */
	DWORD		HideWhenFlags; 		/* HIDE_ flags below */
	DWORD		LaunchWhenFlags;	/* LAUNCH_ flags below */
	DWORD		OleFlags;			/* OLE Flags below */
	DWORD		CopyToFieldFlags;	/* Field create flags below */
	DWORD		Spare1;
	DWORD		Spare2;
	WORD		FieldNameLength;	/* If named field, length of field name */
	OLE_GUID	OleObjClass;		/* ClassID GUID of OLE object, if create new */
	/* Field Name, if used, goes here */	
} CDDOCAUTOLAUNCH;


/* Autolaunch Object type flags */

#define AUTOLAUNCH_OBJTYPE_NONE			0x00000000
#define AUTOLAUNCH_OBJTYPE_OLE_CLASS	0x00000001 /* OLE Class ID (GUID) */
#define AUTOLAUNCH_OBJTYPE_OLEOBJ		0x00000002 /* First OLE Object */
#define AUTOLAUNCH_OBJTYPE_DOCLINK		0x00000004 /* First Notes doclink */
#define AUTOLAUNCH_OBJTYPE_ATTACH		0x00000008 /* First Attachment */
#define AUTOLAUNCH_OBJTYPE_URL			0x00000010 /* AutoLaunch the url in the URL field */


/*	Hide-when flags */

#define HIDE_OPEN_CREATE	0x00000001 /* Hide when opening flags */
#define HIDE_OPEN_EDIT		0x00000002
#define HIDE_OPEN_READ		0x00000004
#define HIDE_CLOSE_CREATE	0x00000008	/* Hide when closing flags */
#define HIDE_CLOSE_EDIT		0x00000010
#define HIDE_CLOSE_READ		0x00000020

/* 	Launch-when flags */

#define LAUNCH_WHEN_CREATE	0x00000001
#define LAUNCH_WHEN_EDIT	0x00000002
#define LAUNCH_WHEN_READ	0x00000004

/* OLE Flags */

#define OLE_EDIT_INPLACE	0x00000001
#define OLE_MODAL_WINDOW	0x00000002
#define OLE_ADV_OPTIONS		0x00000004

/* Field Location Flags */

#define FIELD_COPY_NONE		0x00000001	/* Don't copy obj to any field (V3 compatabile) */
#define FIELD_COPY_FIRST	0x00000002  /* Copy obj to first rich text field */
#define FIELD_COPY_NAMED	0x00000004  /* Copy obj to named rich text field */


typedef struct
{
	WSIG		Header;					/* Signature and length of this record */
	WORD		FileObjNameLength;		/* Length of name of extendable $FILE object containing 
											object data */
	WORD		DescriptionNameLength;	/* Length of description of object */
	WORD		FieldNameLength;		/* Length of field name in which object resides */
	WORD		TextIndexObjNameLength;	/* Length of name of the $FILE object containing LMBCS text 
											for object */
	OLE_GUID	OleObjClass;			/* OLE ClassID GUID of OLE object */
	WORD		StorageFormat;			/* See below OLE_STG_FMT_??? */	
	WORD		DisplayFormat;			/* Object's display format within document, DDEFORMAT_??? */
	DWORD		Flags;					/* Object information flags, see OBJINFO_FLAGS_??? */
	WORD		StorageFormatAppearedIn;/* Version # of Notes, high byte=major, low byte=minor, 
											for display purposes -- not really used */
	WORD		HTMLDataLength;			/* Length of HTML data for object */
	WORD		AssociatedFILEsLength;	/* Length of Associated $FILEs data for object */
	WORD		Reserved3;				/* Unused, must be 0 */
	DWORD		Reserved4;				/* Unused, must be 0 */
	/* The variable length portions go here in the following order:
		FileObjectName
		DescriptionName
		Field Name in Document in which this object resides
		Full Text index $FILE object name
		HTML Data
		Associated $FILEs Data
	*/
}CDOLEOBJ_INFO;

#define OBJINFO_FLAGS_SCRIPTED				0x00000001  /* Object is scripted */
#define OBJINFO_FLAGS_RUNREADONLY			0x00000002	/* Object is run in read-only mode */
#define OBJINFO_FLAGS_CONTROL				0x00000004	/* Object is a control */
#define OBJINFO_FLAGS_FITTOWINDOW			0x00000008	/* Object is sized to fit to window */
#define OBJINFO_FLAGS_FITBELOWFIELDS		0x00000010	/* Object is sized to fit below fields */
#define OBJINFO_FLAGS_UPDATEFROMDOCUMENT	0x00000020	/* Object is to be updated from document */
#define OBJINFO_FLAGS_INCLUDERICHTEXT		0x00000040	/* Object is to be updated from document */
#define OBJINFO_FLAGS_ISTORAGE_ISTREAM      0x00000080	/* Object is stored in IStorage/IStream
															format rather than RootIStorage/
															IStorage/IStream */
#define OBJINFO_FLAGS_HTMLDATA				0x00000100	/* Object has HTML data */

#define OLE_STG_FMT_STRUCT_STORAGE		1	/* OLE "Docfile" structured storage format,
												RootIStorage/IStorage/IStream (Notes format) */
#define OLE_STG_FMT_ISTORAGE_ISTREAM	2	/* OLE IStorage/IStream structured storage format */
#define OLE_STG_FMT_STRUCT_STREAM		3	/* OLE RootIStorage/IStream structured storage format */


/* HTML OBJECT Event Entry ----------------------------------------------------- */
typedef struct
{
	WORD	wLength;			/* Size of this structure including both fixed and
									variable sections */
	WORD	wsNameLength;		/* Length of Name */
	WORD	wsScriptLength;		/* Length of Script */
	WORD	wReserved1;			/* Unused, must be 0 */
	WORD	wReserved2;			/* Unused, must be 0 */
	/* The variable length portions go here in the following order:
		Name
		Script 
	*/
} OLEOBJHTMLEVENT;	

/* HTML OBJECT Param Entry ----------------------------------------------------- */
typedef struct
{
	WORD	wLength;			/* Size of this structure including both fixed and
									variable sections */
	WORD	wsDataFldLength;	/* Length of Data Field */
	WORD	wsDataFmtsLength;	/* Length of Data Formats */
	WORD	wsDataSrcLength;	/* Length of Data Source */
	WORD	wsNameLength;		/* Length of Name */
	WORD	wsTypeLength;		/* Length of Type */
	WORD	wsValueLength;		/* Length of Value */
	WORD	wsValueTypeLength;	/* Length of Value Type */
	WORD	wReserved1;			/* Unused, must be 0 */
	WORD	wReserved2;			/* Unused, must be 0 */
	/* The variable length portions go here in the following order:
		Data Field - column name from the data source object
		Data Formats - indicates whether bound data is plain text or HTML
		Data Source - "#ID" of the data source object
		Name - name of this parameter
		Type - internal media type
		Value - value associated with parameter
		Value Type - type of value (data, ref, object) 
	*/
} OLEOBJHTMLPARAM;

/* OLE Object HTML Data ------------------------------------------------------------- */
typedef struct
{								
	WORD	wLength;				/* Size of this structure including both fixed and
										variable sections */
	WORD	wsURLBaseLength;		/* Length of Base URL */
	WORD	wsURLCodeBaseLength;	/* Length of CodeBase URL */
	WORD	wsMIMETypeCodeLength;	/* Length of MIME CodeType */
	WORD	wsURLDataLength;		/* Length of Data URL */
	WORD	wsDataFldLength;		/* Length of Data Field name */
	WORD	wsDataSrcLength;		/* Length of Data Source ID */
	DWORD	dwFlags;				/* Flags */
	WORD	wsLangLength;			/* Length of Language */
	WORD	wsNameLength;			/* Length of Name */
	WORD	wsMIMETypeDataLength;	/* Length of MIME Type */
	WORD	wcEvents;				/* Number of events */
	WORD	wcParams;				/* Number of params */
	WORD	wHeight;				/* Height of object */
	WORD	wWidth;					/* Width of object */
	WORD	wReserved1;				/* Unused, must be 0 */
	WORD	wReserved2;				/* Unused, must be 0 */
	WORD	wReserved3;				/* Unused, must be 0 */
	WORD	wReserved4;				/* Unused, must be 0 */
	WORD	wReserved5;				/* Unused, must be 0 */
	WORD	wReserved6;				/* Unused, must be 0 */
	/* The variable length portions go here in the following order:
		URLBase - Base URL
		CodeBase - URL that references where to find implementation of the object. 
		CodeType - MIME type of the code referenced by CLSID
		Data - URL of the data to be loaded
		Data Field - column name from the data source object
		Data Source - "#ID" of the data source object
		Lang - ISO standard language abbreviation
		Name - variable name
		Type - MIME type of Data attribute.
		Events - array or list of events (OLEOBJHTMLEVENT structures)
		Params - array or list of params (OLEOBJHTMLPARAM structures)
	*/
} OLEOBJHTMLDATA;


#define OBJINFO_HTMLFLAGS_DECLARE	0x00000001	/* Declare - download and install object's code
													but do not instantiate. */

/* Associated $FILE Object Data --------------------------------------------------------- */
typedef struct 
{
	WORD	wLength;			/* Size of this structure including both fixed and
								variable sections */
	WORD	wcAssociatedFILE;	/* Length of the Associated $FILE name */
	WORD	wLinkType;			/* Unused, must be 0 */
	DWORD	Reserved;			/* Unused, must be 0 */
	/* Variable length portions go here in the following order:
		Associated $FILE name
	*/
} ASSOCIATEDFILE;

/* Associated $FILEs Data --------------------------------------------------------- */
typedef struct
{								
	WORD	wLength;				/* Size of this structure including both fixed and
										variable sections */
	WORD	wcAssociatedFILEs;		/* Number of Associated $FILEs */
	WORD	wReserved1;				/* Unused, must be 0 */
	WORD	wReserved2;				/* Unused, must be 0 */
	WORD	wReserved3;				/* Unused, must be 0 */
	/* The variable length portions go here in the following order:
		Associated $FILEs - array or list of Associated FILES (OLEOBJASSOCIATEDFILE structures)
	*/
} ASSOCIATEDFILESDATA;


#endif	/* OLE_ODS_DEFS */

