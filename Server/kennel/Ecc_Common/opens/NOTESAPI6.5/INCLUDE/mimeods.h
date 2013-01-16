#ifndef _MIMEODS_H
#define _MIMEODS_H

#ifdef __cplusplus
extern "C" {
#endif

#define MIME_PART_VERSION				2

typedef struct {
	WORD	wVersion;                 	/* MIME_PART Version */
	DWORD	dwFlags;
	#define MIME_PART_HAS_BOUNDARY			0x00000001
	#define MIME_PART_HAS_HEADERS			0x00000002
	#define MIME_PART_BODY_IN_DBOBJECT		0x00000004
	#define MIME_PART_SHARED_DBOBJECT		0x00000008	/*	Used only with MIME_PART_BODY_IN_DBOBJECT. */
	#define MIME_PART_SKIP_FOR_CONVERSION	0x00000010	/* only used during MIME->CD conversion */
	BYTE	cPartType;                	/* Type of MIME_PART body */
	BYTE	cSpare;
	WORD	wByteCount;               	/* Bytes of variable length part data
										   NOT including data in DB object*/

	WORD	wBoundaryLen;             	/* Length of the boundary string */
	WORD	wHeadersLen;              	/* Length of the headers */
	WORD	wSpare;
	DWORD	dwSpare;

	/*	Followed by the actual part data */

	/*	NOTE:	To modify this structure, take one of the spares.  If there are no
				spares left, increment the version number and be sure to add an even
				number of bytes to the structure. */
} MIME_PART;
typedef enum tagMIMEPartType
	{
	MIME_PART_PROLOG		= 1,
	MIME_PART_BODY			= 2,
	MIME_PART_EPILOG		= 3,
	MIME_PART_RETRIEVE_INFO	= 4,
	MIME_PART_MESSAGE		= 5
	} MIMEPARTTYPE;


/* TYPE_822_TEXT item header information */
typedef struct {
	WORD	wVersion;			/* ODSSizeof this structure for versioning */
	DWORD	dwFlags;			/* TYPE_822_TEXT flags.  The first three bits
								   are reserved for the format mask, the formats
								   defined include: */
			#define RFC822_ITEM_FORMAT_MASK		0x00000007
			#define RFC822_ITEM_FORMAT_ADDR		0x00000000 /* 822-header is an address */
			#define RFC822_ITEM_FORMAT_DATE		0x00000001 /* 822-header is a date */
			#define RFC822_ITEM_FORMAT_TEXT		0x00000002 /* 822-header is text */
								/* the remaining
								   bits are flags which include: */
			#define RFC822_ITEM_STORAGE_STRICT	0x00000008 /* STRICT storage format */
			#define RFC822_ITEM_TEXT_LIST		0x00000010 /* Text item is TEXT_LIST */
			#define RFC822_TEXT_UNUSED			0x00000020 /* First available flag */
	WORD	wNotesNativeLen;	/* Length of the Notes version which is either
								   a LMBCS string or a TIMEDATE. */
	WORD	w822NameLen;		/* Length of the original 822 header name */
	WORD	w822DelimLen;		/* Length of the original 822 header delimiter */
	WORD	w822BodyLen;		/* Length of the original 822 header body in
								   it's native charset and encoding (RFC2047) */
	} RFC822ITEMDESC;

#ifdef __cplusplus
}
#endif

#endif /* _MIMEODS_H */

