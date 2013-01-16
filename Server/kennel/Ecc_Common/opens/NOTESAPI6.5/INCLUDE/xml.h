
#if !defined (XML_H)
#define XML_H

#ifndef GLOBAL_DEFS
#include "global.h"
#endif

#ifndef NSF_DATA_DEFS
#include "nsfdata.h"
#endif

#ifndef NAME_DEFS
#include "names.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef MEMHANDLE DXLEXPORTHANDLE;
typedef MEMHANDLE DXLIMPORTHANDLE;
typedef MEMHANDLE XSLTRANSFORMHANDLE;


/*
 *--------------------------------------------------------------------------------------
 *	XML COMMON DEFINITIONS (SHARED BY TWO OR MORE XML PROCESSORS)
 *--------------------------------------------------------------------------------------
 *
 */

/* XML validation options */
typedef enum
{
	Xml_Validate_Never = 0,
	Xml_Validate_Always = 1,
	Xml_Validate_Auto = 2

} Xml_Validation_Option;

typedef DWORD (LNCALLBACKPTR XML_READ_FUNCTION) (BYTE *pBuffer, DWORD length, void far *pAction);
typedef void (LNCALLBACKPTR XML_WRITE_FUNCTION) (const BYTE *pBuffer, DWORD length, void far *pAction );


/*
 *--------------------------------------------------------------------------------------
 *	DXL IMPORT API
 *--------------------------------------------------------------------------------------
 *
 *	DXL importer options for ACL, design elements, and documents (each can be set independently).
 *
 *	Notes:
 *
 *		1) The REPLACE options remove matching data completely before importing new data from DXL
 *
 *			* For documents and design elements, matching is done by UNID.  When a DXL document
 *			  or design element matches one in the the database, then all items in the database's
 *			  document or design element will be replace with the DXL content. When a DXL document
 *			  or design element does not match one in the database, then the DXL content will be
 *			  ignored if the DXLIMPORTOPTION_REPLACE_ELSE_IGNORE. If, instead, the option is
 *			  DXLIMPORTOPTION_REPLACE_ELSE_CREATE, then an entirely new document or design element
 *			  will be created from the DXL.
 *
 *			* For the ACL, the only valid REPLACE option is DXLIMPORTOPTION_REPLACE_ELSE_IGNORE.
 *			  If the DXL contains ACL info and the database has an ACL (almost always true), then
 *			  the database's ACL will be depopulated and replaced by the ACL entries from the DXL.
 *
 *		2) The UPDATE options work at the item level for documents and the entry level for the ACL,
 *		   and preexisting non-matching items/entries in the database will be preserved.
 *
 *			* For documents, matching is done first by UNID and then by item name. When a DXL item
 *			  matches a document item, the document item is replaced by the DXL content. When a DXL
 *			  item does not have a match in the document, it is added to the document if the
 *			  DXLIMPORTOPTION_UPDATE_ELSE_CREATE option is specified and ignored if the DXLIMPORTOPTION-
 *			  _UPDATE_ELSE_IGNORE option is specified.
 *
 *			* UPDATE options are not applicable to design elements.
 *
 *			* For the ACL, if a DXL ACL entry matches an existing database ACL entry by name, then the
 *			  entry in the database is replaced with the DXL entry. When a DXL ACL entry does not match
 *			  anything already in the database ACL, it is added to the ACL if the DXLIMPORTOPTION-
 *			  _UPDATE_ELSE_CREATE option is specified and ignored if the DXLIMPORTOPTION_UPDATE_ELSE_IGNORE
 *			  option is specified.
 *
 */ 

typedef enum
{
	DXLIMPORTOPTION_IGNORE=1,				/* ignore imported data */

	DXLIMPORTOPTION_CREATE=2,				/* create new data from imported data */
	DXLIMPORTOPTION_CREATE_RESERVED1=3,		/* do not used - reserved for future variation of create option */
	DXLIMPORTOPTION_CREATE_RESERVED2=4,		/* do not used - reserved for future variation of create option */

	DXLIMPORTOPTION_REPLACE_ELSE_IGNORE=5,	/* if imported data matches existing data, then replace existing */
											/* ... data with imported data, else ignore imported data. */
	DXLIMPORTOPTION_REPLACE_ELSE_CREATE=6,	/* if imported data matches existing data, then replace existing */
											/* ... data with imported data, else create new data from imported data */
	DXLIMPORTOPTION_REPLACE_RESERVED1=7,	/* do not used - reserved for future variation of replace option */
	DXLIMPORTOPTION_REPLACE_RESERVED2=8,	/* do not used - reserved for future variation of replace option */

	DXLIMPORTOPTION_UPDATE_ELSE_IGNORE=9,	/* if imported  data matches existing data, then update existing */
											/* ... data with imported data, else ignore imported data. */
	DXLIMPORTOPTION_UPDATE_ELSE_CREATE=10,	/* if imported data matches existing data, then update existing */
											/* ... data with imported data, else create new data from imported data */
	DXLIMPORTOPTION_UPDATE_RESERVED1=11,	/* do not used - reserved for future variation of update option */
	DXLIMPORTOPTION_UPDATE_RESERVED2=12		/* do not used - reserved for future variation of update option */

} DXLIMPORTOPTION;



/*
 *	Options for logging various types of DXL import error situations.
 *	(For example, unknown elements and attributes in the DXL input stream).
 */

typedef enum
{
	DXLLOGOPTION_IGNORE=1,		/* ignore the action.  don't log anything and just continue */
	DXLLOGOPTION_WARNING=2,		/* log the problem as a warning */
	DXLLOGOPTION_ERROR=3,		/* log the problem as an error */
	DXLLOGOPTION_FATALERROR=4	/* log the problem as a fatal error */

} DXLLOGOPTION; 


/*
 *	DXL_IMPORT_PROPERTY default values are set as follows:
 *
 *			Note:	(i) = can input new value into the importer.
 *					(o) = can get current value out of importer.
 *					(io) = can do both. 
 *
 *		iACLImportOption					= (io) DXLIMPORTOPTION_IGNORE
 *		iDesignImportOption					= (io) DXLIMPORTOPTION_IGNORE
 *		iDocumentsImportOption				= (io) DXLIMPOROPTION_CREATE
 *		iCreateFullTextIndex				= (io) FALSE
 *		iReplaceDbProperties				= (io) FALSE
 *		iInputValidationOption				= (io) Xml_Validate_Auto
 *		iReplicaRequiredForReplaceOrUpdate	= (io) TRUE
 *		iExitOnFirstFatalError				= (io) TRUE
 *		iUnknownTokenLogOption				= (io) DXLLOGOPTION_FATALERROR
 *		iResultLogComment					= (io) NULLMEMHANDLE
 *		iResultLog							= (o)  NULLMEMHANDLE
 */

typedef enum
{
	iACLImportOption=1,			/* WORD, Assign to value defined in DXLIMPORTOPTION */
	iDesignImportOption=2,		/* WORD, Assign to value defined in DXLIMPORTOPTION */
	iDocumentsImportOption=3,	/* WORD, Assign to value defined in DXLIMPORTOPTION */
	iCreateFullTextIndex=4,		/* BOOL, TRUE = create full text index, FALSE Do NOT create full text index */
	iReplaceDbProperties=5,		/* BOOL, TRUE = replace database properties, FALSE Do NOT replace database properties */
	iInputValidationOption=6,	/* Xml_Validation_Option, Values defined in Xml_Validation_Option, ...Validate_Never, ...Validate_Always, ...Validate_Auto */
	iReplicaRequiredForReplaceOrUpdate=7,	/* BOOL, TRUE = skip replace/update ops if target DB and import DXL do not have same replicaid's */
											/* ... FALSE = allow replace/update ops even if target DB and import DXL do not have same replicaid's */
	iExitOnFirstFatalError=8,	/* BOOL, TRUE = importer exits on first fatal error, FALSE = importer continues even if fatal error found */
	iUnknownTokenLogOption=9,	/* WORD, Assign to value defined in DXLLOGOPTION. Specifies what to do if DXL contains an unknown element or attribute */
	iResultLogComment=10,		/* char*(i)/MEMHANDLE(o)  LMBCS string to be added as comment to top of result log */  
	iResultLog=11				/* MEMHANDLE, (readonly) The result log from the last import */

} DXL_IMPORT_PROPERTY;

/* Import Functions */
STATUS LNPUBLIC DXLCreateImporter(DXLIMPORTHANDLE *prethDXLImport);
STATUS LNPUBLIC DXLSetImporterProperty(DXLIMPORTHANDLE hDXLImport, DXL_IMPORT_PROPERTY prop, void *propValue);
STATUS LNPUBLIC DXLGetImporterProperty(DXLIMPORTHANDLE hDXLImporter, DXL_IMPORT_PROPERTY prop, void *retPropValue);
STATUS LNPUBLIC DXLImport(DXLIMPORTHANDLE hDXLImport, XML_READ_FUNCTION pDXLReaderFunc, DBHANDLE hDB, void far *pImAction);
BOOL   LNPUBLIC DXLImportWasErrorLogged (DXLIMPORTHANDLE hDXLImport);
void   LNPUBLIC DXLDeleteImporter(DXLIMPORTHANDLE hDXLImport);


/*
 *--------------------------------------------------------------------------------------
 *	DXL EXPORT API
 *--------------------------------------------------------------------------------------
 */

/*
 * DXL export charsets
 */

typedef enum
{
	eDxlExportUtf8,			/* (default) "encoding =" attribute is set to utf8 and output charset is utf8 */
	eDxlExportUtf16			/* "encoding =" attribute is set to utf16 and charset is utf16 */

} DXL_EXPORT_CHARSET;

/*
 *	DXL export richtext options
 */

typedef enum
{
	eRichtextAsDxl,			/* (default) output richtext as dxl with warning comments if uninterpretable CD records */
	eRichtextAsItemdata		/* output richtext as uninterpretted (base64'ed) item data */

} DXL_RICHTEXT_OPTION;

/*
 *	DXL_EXPORT_PROPERTY default values are set as follows:
 *
 *			Note:	(i) = can input new value into the exporter.
 *					(o) = can get current value out of exporter.
 *					(io) = can do both. 
 *
 *		eDxlExportResultLog			= (o)	NULLMEMHANDLE
 *		eDefaultDoctypeSYSTEM		= (o)	filename of dtd or schema keyed to current version of DXL exporter.
 *		eDoctypeSYSTEM				= (io)	filename of dtd or schema keyed to current version of DXL exporter.
 *		eDXLBannerComments			= (io)	NULLMEMHANDLE
 *		eDxlExportCharset			= (io)	eDxlExportUtf8
 *		eDxlRichtextOption			= (io)	eRichtextAsDxl
 *		eDxlExportResultLogComment	= (io)	NULLMEMHANDLE
 *		eForceNoteFormat			= (io)	FALSE
 *		eExitOnFirstFatalError		= (io)	TRUE
 *		eOutputRootAttrs			= (io)	TRUE
 *		eOutputXmlDecl				= (io)	TRUE
 *		eOutputDOCTYPE				= (io)	TRUE
 *		eConvertNotesbitmapsToGIF	= (io)	FALSE
 *
 */

typedef enum
{
		/* non-boolean export properties */

	eDxlExportResultLog=1,		/* MEMHANDLE,				Readonly - the result log from the last export. */
	eDefaultDoctypeSYSTEM=2,	/* MEMHANDLE,				Readonly - filename of dtd/schema keyed to current version of exporter */
	eDoctypeSYSTEM=3,			/* char*(i)/MEMHANDLE(o),	What to use for the DOCTYPE SYSTEM value (if emitted) */
								/*								NULL or "" 	= DOCTYPE should contain no SYSTEM info */
								/*								"filename"	= filename of dtd or schema used as DOCTYPE SYSTEM value */
	eDXLBannerComments=4,		/* char*(i)/MEMHANDLE(o),	One or more XML comments to output at top of the DXL */
								/*								NULL or ""	= no dxl banner comments */
								/*								"whatever"	= zero or more nul-terminated strings capped by extra empty string */
	eDxlExportCharset=5,		/* DXL_EXPORT_CHARSET,		Specifies output charset. */
	eDxlRichtextOption=6,		/* DXL_RICHTEXT_OPTION,		Specifies rule for exporting richtext. */
	eDxlExportResultLogComment=7, /* char*(i)/MEMHANDLE(o),	LMBCS string to be added as comment to top of result log */

		/* boolean properties (gap allows for future definitions of other non-boolean export properties) */

	eForceNoteFormat=30,		/* BOOL, TRUE = Export data as notes containing items, FALSE = export using a high level of abstraction, */
	eExitOnFirstFatalError=31,	/* BOOL, TRUE = Abort on first fatal error, FALSE = try to continue to export */
	eOutputRootAttrs=32,		/* BOOL, TRUE = Root needs xmlns, version, and other common root attrs */
	eOutputXmlDecl=33,			/* BOOL, TRUE = Emit a leading xml declaration statement (<?xml ...?>) */
	eOutputDOCTYPE=34,			/* BOOL, TRUE = Emit a DOCTYPE statement */
	eConvertNotesbitmapsToGIF=35	/* BOOL, TRUE = Convert Notesbitmaps embedded in richtext to GIFs, FALSE = blob the Notesbitmap CD records */

} DXL_EXPORT_PROPERTY;

/* Export Functions */
STATUS LNPUBLIC DXLCreateExporter(DXLEXPORTHANDLE *prethDXLExport);
STATUS LNPUBLIC DXLSetExporterProperty(DXLEXPORTHANDLE hDXLExport, DXL_EXPORT_PROPERTY prop, void *propValue);
STATUS LNPUBLIC DXLGetExporterProperty(DXLEXPORTHANDLE hDXLExport, DXL_EXPORT_PROPERTY prop, void *retPropValue);
STATUS LNPUBLIC DXLExportACL (DXLEXPORTHANDLE hDXLExport, XML_WRITE_FUNCTION pDXLWriteFunc, DBHANDLE hDB, void far *pExAction);
STATUS LNPUBLIC DXLExportNote (DXLEXPORTHANDLE hDXLExport, XML_WRITE_FUNCTION pDXLWriteFunc, NOTEHANDLE hNote, void far *pExAction);
STATUS LNPUBLIC DXLExportDatabase (DXLEXPORTHANDLE hDXLExport, XML_WRITE_FUNCTION pDXLWriteFunc, DBHANDLE hDB, void far *pExAction);
STATUS LNPUBLIC DXLExportIDTable (DXLEXPORTHANDLE hDXLExport, XML_WRITE_FUNCTION pDXLWriteFunc, DBHANDLE hDB, HANDLE hIDTable, void far *pExAction);
BOOL   LNPUBLIC DXLExportWasErrorLogged (DXLEXPORTHANDLE hDXLExport);
void   LNPUBLIC DXLDeleteExporter(DXLEXPORTHANDLE hDXLExport);


/*
 *--------------------------------------------------------------------------------------
 *	XSL TRANSFORMER API
 *--------------------------------------------------------------------------------------
 */

/*
 * XSLT output charsets
 */

typedef enum
{
	xsltOutputUtf8,			/* (default) output charset is utf8 */
	xsltOutputUtf16			/* output charset is utf16 */

} XSLT_OUTPUT_CHARSET;

/*
 *	XSLT_PROPERTY default values are set as follows:
 *
 *			Note:	(i) = can input new value into the exporter.
 *					(o) = can get current value out of exporter.
 *					(io) = can do both. 
 *
 *		xsltResultLog				= (o)	NULLMEMHANDLE
 *		xsltResultLogComment		= (io)	NULLMEMHANDLE
 *		xsltExitOnFirstFatalError	= (io)	TRUE
 *		xsltInputValidationOption	= (io)	Xml_Validate_Auto
 *		xsltOutputCharset			= (io)	xsltOutputUtf8
 *
 */

typedef enum
{
	xsltResultLog=1,			/* MEMHANDLE,				Readonly - the result log from the last transform. */
	xsltResultLogComment=2,		/* char*(i)/MEMHANDLE(o),	LMBCS string to be added as comment to top of result log */
	xsltExitOnFirstFatalError=3,/* BOOL, 					TRUE = importer exits on first fatal error, */
								/*							FALSE = importer continues even if fatal error found */
	xsltInputValidationOption=4,/* Xml_Validation_Option,	Values defined in Xml_Validation_Option: ...Validate_Never, ...Validate_Always, ...Validate_Auto */
	xsltOutputCharset=5			/* XSLT_OUTPUT_CHARSET,		xsltOutputUtf8 or xsltOutputUtf16 */

} XSLT_PROPERTY;

/* XSL Transform Functions */
STATUS LNPUBLIC XSLTCreateTransform(XSLTRANSFORMHANDLE *prethXSLTransform);
STATUS LNPUBLIC XSLTSetTransformProperty(XSLTRANSFORMHANDLE hXSLTransform, XSLT_PROPERTY prop, void *propValue);
STATUS LNPUBLIC XSLTGetTransformProperty(XSLTRANSFORMHANDLE hXSLTransform, XSLT_PROPERTY prop, void *retPropValue);
STATUS LNPUBLIC XSLTAddParameter(XSLTRANSFORMHANDLE hXSLTransform, const char *szParmName, const char *szParmValue);
STATUS LNPUBLIC XSLTTransform(XSLTRANSFORMHANDLE hXSLTransform,
                              XML_READ_FUNCTION pXSL_XMLInputFunc,			void far *pXSL_XMLInputAction,
                              XML_READ_FUNCTION pXSL_StylesheetInputFunc,	void far *pXSL_StylesheetInputAction,
                              XML_WRITE_FUNCTION pXSL_TransformOutputFunc,	void far *pXSL_TransformOutputAction);
BOOL   LNPUBLIC XSLTTransformWasErrorLogged(XSLTRANSFORMHANDLE hXSLTransform);
void   LNPUBLIC XSLTTransformDeleteTransform(XSLTRANSFORMHANDLE hXSLTransform);

#ifdef __cplusplus
}
#endif

#endif	/* XML_H */
