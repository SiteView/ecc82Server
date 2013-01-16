

/* Domino Web Server - Filter API */


#if !defined(DSAPI_H)
#define DSAPI_H

#ifdef __cplusplus
extern "C" {
#endif


#define kInterfaceVersion	2
#define kMaxFilterDesc	255

typedef unsigned char LMBCS;

typedef enum {
	kFilterNotHandled = 0,
	kFilterHandledRequest = 1,
	kFilterHandledEvent = 2,
	kFilterError = 3
} FilterReturnCode;


/* DSAPI events to register for  */
typedef enum {
	kFilterRawRequest		= 0x01,
	kFilterParsedRequest	= 0x02,
	kFilterAuthUser			= 0x04,
	kFilterUserNameList		= 0x08,
	kFilterMapURL			= 0x10,
	kFilterResponse			= 0x20,
	kFilterRawWrite			= 0x40,
	kFilterEndRequest		= 0x80,
	kFilterStartRequest		= 0x100,
	kFilterPostTranslate	= 0x200,
	kFilterAuthorized		= 0x400,
	kFilterProcessRequest	= 0x800,
	kFilterAuthenticate		= 0x2000,
	kFilterRewriteURL		= 0x4000,

	kFilterAny				= 0x6FFF & ~kFilterAuthUser		/*  */
} EventFlags;

#define kFilterTranslateRequest	0x10	/* Same as kURLMap event. */


#define DSAPI_ERROR_MESSAGES		1000
#define DSAPI_BUFFER_TOO_SMALL		( DSAPI_ERROR_MESSAGES + 1 )
#define DSAPI_INVALID_ARGUMENT		( DSAPI_ERROR_MESSAGES + 2 )
#define DSAPI_MEMORY_ERROR			( DSAPI_ERROR_MESSAGES + 3 )
#define DSAPI_INTERNAL_ERROR		( DSAPI_ERROR_MESSAGES + 4 )
#define DSAPI_REQUEST_ALREADY_OWNED	( DSAPI_ERROR_MESSAGES + 5 )


/* Filter initialization data */

typedef struct {
	unsigned int	serverFilterVersion;
	unsigned int	appFilterVersion;
	unsigned int	eventFlags;
	unsigned int	initFlags;
	char			filterDesc[ kMaxFilterDesc + 1 ];
} FilterInitData;



/* Request line descriptor  */
typedef struct {
	unsigned int	method;
	char*			URL;
	char*			version;
	char*			userName;
	char*			password;
	unsigned char*	clientCert;
	unsigned int	clientCertLen;
	char*			contentRead;
	unsigned int	contentReadLen;
} FilterRequest;



/* Filter context data included in every call to filter  */
typedef struct _FilterContext {
	unsigned int	contextSize;
	unsigned int	revision;
	void*			serverContext;
	unsigned int	serverReserved;
	unsigned int	securePort;
	void*			privateContext;

	int ( *GetRequest )( struct _FilterContext *pContext, 
						 FilterRequest *pRequest,
						 unsigned int *pErrID);

	int ( *GetRequestContents )( struct _FilterContext *pContext,
								 char **pContents, 
								 unsigned int *pErrID );

	int ( *GetServerVariable )( struct _FilterContext *pContext, 
								char *pNname, 
								void *pBuffer,
								unsigned int bufferSize,
								unsigned int *pErrID );

	int ( *WriteClient )( struct _FilterContext *pContext, 
						  char *pBuffer,
						  unsigned int bufferLen,
						  unsigned int reserved,
						  unsigned int *pErrID );

	void* ( *AllocMem )( struct _FilterContext *pContext, 
						 unsigned int size,
						 unsigned int reserved,
						 unsigned int *pErrID );

	int ( *ServerSupport )( struct _FilterContext *pContext, 
							unsigned int funcType,
							void *pData1,
							void *pData2,
							unsigned int other,
							unsigned int *pErrID );
} FilterContext;



/* Request methods   */
typedef enum {
	kRequestNone	= 0,
	kRequestHEAD	= 1,
	kRequestGET		= 2,
	kRequestPOST	= 3,
	kRequestPUT		= 4,
	kRequestDELETE	= 5,
	kRequestTRACE	= 6,
	kRequestCONNECT	= 7,
	kRequestOPTIONS	= 8,
	kRequestUNKNOWN	= 9,
	kRequestBAD		= 10

} RequestMethod;



/* Server support function types */
typedef enum {
	kWriteResponseHeaders		= 1,
	kOwnsRequest				= 2,
	kGetParsedRequest			= 3,
	kWrite102Processing			= 4,
	kGetAuthenticatedUserInfo	= 5
} ServerSupportTypes;


/* 'data1' for server support function 'kWriteResponseHeaders' */
typedef struct {
	unsigned int	responseCode;
	char*			reasonText;
	char*			headerText;
} FilterResponseHeaders;


/* Data1 for server support function kGetParsedRequest */
typedef struct{
	char			*pRawUri;
	char			*pPathUri;
	char			*pQueryUri;
	char			*pFragmentUri;

	char			*pSchemeUri;
	char			*pHostInfoUri;
	char			*pHostName;
	int				hostPort;

	char			*pUserUri;
	char			*pUserPasswordUri;

	int				majorProtocolVersion;
	int				minorProtocolVersion;
}FilterParsedRequestLine;


typedef enum{
	kCannonicalUserName = 0x01,
	kWebUserName		= 0x02,
	kUserPassword		= 0x04,
	kUserGroupList		= 0x08
}FilterAuthenticatedUserFields;

typedef struct {
	FilterAuthenticatedUserFields	fieldFlags;
	char							*pUserCannonicalName;
	char							*pWebUserName;
	char							*pUserPassword;
	char							*pUserGroupList;
}FilterAuthenticatedUser;



/* Raw request (headers not processed yet)   */
typedef struct {
	unsigned int requestMethod;

	int ( *GetAllHeaders )( FilterContext *pContext,
							char** ppHeaders,
							unsigned int *pErrID );

	int ( *GetHeader )( FilterContext *pContext,
						char *pName,
						char *pBuffer,
						unsigned int bufferSize,
						unsigned int *pErrID );

	int ( *SetHeader )( FilterContext *pContext,
						char *pName,
						char *pValue,
						unsigned int *pErrID );

	int ( *AddHeader )( FilterContext *pContext,
						char *pHheader,
						unsigned int *pErrID );

	unsigned int reserved;
} FilterRawRequest;



/* Parsed request */
typedef struct {
	unsigned int requestMethod;

	int ( *GetAllHeaders )( FilterContext *pContext,
							char **ppHeaders,
							unsigned int *pErrID );

	int ( *GetHeader )( FilterContext *pContext,
						char *pName,
						char *pBuffer,
						unsigned int bufferSize,
						unsigned int *pErrID );

	unsigned int reserved;
} FilterParsedRequest;



/*		URL map  */
typedef struct {
	const char*		url;			/* Input. The input URL */
	char*			pathBuffer;		/* Input. The resulting mapping is contained in the supplied buffer */
	unsigned int	bufferSize;		/* Size of the buffer supplied */
	unsigned int	mapType;		/* Mapping type. */
} FilterMapURL;



/*		URL map types  */
typedef enum {
	kURLMapUnknown	= 0,			/* Unknown mapping type. */
	kURLMapPass		= 1,			/* File system mapping rule */
	kURLMapExec		= 2,			/* CGI mapping rule */
	kURLMapRedirect = 3,			/* Redirect mapping rule */
	kURLMapService	= 4,			/* Obsolete. Not used anymore in Rnext. */
	kURLMapDomino	= 5				/* Domino mapping rule */
} FilterURLMapTypes;



/* User authentication  */
typedef struct {
	LMBCS*			userName;
	LMBCS*			password;
	unsigned char*	clientCert;
	unsigned int	clientCertLen;
	unsigned int	authFlags;
	unsigned int	preAuthenticated;
	unsigned int	foundInCache;
	unsigned int	authNameSize;
	LMBCS*			authName;
	unsigned int	authType;

	int ( *GetUserNameList )( FilterContext *pContext, 
							  LMBCS *pBuffer,
							  unsigned int bufferSize,
							  unsigned int *pNumNames,
							  unsigned int reserved,
							  unsigned int *pErrID );

	int ( *GetHeader )( FilterContext *pContext,
						char *pName,
						char *pBuffer,
						unsigned int bufferSize,
						unsigned int *pErrID );

	void ( *GetMappedResource )( FilterContext *pContext,
								 char **ppBuffer,
								 unsigned int *pErrID );

} FilterAuthenticate;


/* User authentication types  */
typedef enum {
	kNotAuthentic			= 0,
	kAuthenticBasic			= 1,
	kAuthenticClientCert	= 2
} FilterAuthenticationTypes;


/*	Authentication configuration flags  */
typedef enum {
	kAuthAllowBasic			= 1,
	kAuthAllowAnonymous		= 2,
	kAuthAllowSSLCert		= 4,
	kAuthAllowSSLBasic		= 8,
	kAuthAllowSSLAnonymous	= 16,
	kAuthRedirectToSSL		= 32
} FilterAuthConfigFlags;


/* User name list */
typedef struct {
	const LMBCS*	userName;

	int ( *GetUserNameList )( FilterContext *pContext, 
							  LMBCS *pBuffer,
							  unsigned int bufferSize,
							  unsigned int *pNumNames,
							  unsigned int reserved,
							  unsigned int *pErrID );

	int ( *PopulateUserNameList )( FilterContext *pContext, 
								   LMBCS *pBuffer,
								   unsigned int bufferSize,
								   unsigned int *pNumNames,
								   unsigned int reserved,
								   unsigned int *pErrID );

	int ( *AddGroupsToList )( FilterContext *pCcontext, 
							  LMBCS *pGroupNames,
							  unsigned int numGroupNames,
							  unsigned int reserved,
							  unsigned int *pErrID );

	int ( *RemoveGroupsFromList )( FilterContext *pContext, 
								   unsigned int reserved,
								   unsigned int *pErrID );

	unsigned int reserved;
} FilterUserNameList;


/* Request response  */
typedef struct {
	unsigned int	responseCode;
	char*			reasonText;

	int ( *GetAllHeaders )( FilterContext *pContext,
							char **ppHeaders,
							unsigned int *pErrID );

	int ( *GetHeader )( FilterContext *pContext,
						char *pName,
						char *pBuffer,
						unsigned int bufferSize,
						unsigned int *pErrID );

	int ( *SetHeader )( FilterContext *pContext,
						char *pName,
						char *pValue,
						unsigned int *pErrID );

	int ( *AddHeader )( FilterContext *pContext,
						char *pHeader,
						unsigned int *pErrID );

	unsigned int	reserved;
	char*			userName;
} FilterResponse;



/*	Write content  */
typedef struct {
	char*			content;
	unsigned int	contentLen;
	unsigned int	reserved;
} FilterRawWrite;


typedef struct _FilterAuthorize{
	const char		*pURL;			/* Input. The input URL */
	char			*pBuffer;		/* Input. The resulting mapping is contained in the supplied buffer */
	unsigned int	bufferSize;		/* Size of the buffer supplied */
	unsigned int	mapType;		/* Mapping type. */
	unsigned int	isAuthorized;	/* Result of operation, 1 if successfull, 0 otherwise. */
} FilterAuthorize;


#ifdef __cplusplus
}
#endif


#endif /* DSAPI_H */

