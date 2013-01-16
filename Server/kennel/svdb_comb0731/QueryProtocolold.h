#ifndef	SVDB_QUERYPROTOCOL_
#define	SVDB_QUERYPROTOCOL_

#ifdef WIN32
#define	USEPIPE
#endif

//////////Query string head//////////////


#define	QUERY_GET		0x100	//"SVDB_GET:"
#define	QUERY_UPDATE	0x200	//"SVDB_UPDATE:"
#define	QUERY_DELETE	0x400	//"SVDB_DELETE:"
#define	QUERY_INFO  	0x800 
#define QUERY_ADDNEW	0x600
#define QUERY_EDIT		0x700
#define QUERY_INFOBYSE	0x801
#define QUERY_CREATIDC	0x601
#define	QUERY_FAST_GET	0x101
#define	QUERY_COPY_INI	0x102
#define	QUERY_GET_LABELS 0x103
#define	QUERY_RCD_COUNT 0x104
#define QUERY_LATEST	0x105

#define	QUERY_GETDYN	0x900
#define	QUERY_DELETETABLE	0x901
#define	QUERY_APPENDRECORD	0x902
#define	QUERY_INSERTTABLE	0x903
#define QUERY_GETDYNNOSTR	0x904
#define	QUERY_GETTABLENAMES 0x905
#define QUERY_DELETERECORDS 0x906
#define QUERY_GETDATABYCOUNT 0x907
#define	QUERY_APPENDMASSRECORD	0x908
#define	QUERY_MASSDYN		0x909
#define	QUERY_MASSMONITOR	0x910
#define	QUERY_MASSENTITY	0x911

#define QUERY_SENDMESSAGE	0x930
#define QUERY_POPMESSAGE	0x931
#define QUERY_DELETEQUEUE	0x932
#define QUERY_CLEARQUEUE	0x933
#define QUERY_CREATEQUEUE	0x934
#define QUERY_GETRECCOUNT	0x935
#define QUERY_GETQUEUENAMES 0x936
#define	QUERY_BLOCKPOP		0x937
#define QUERY_PEEKMESSAGE	0x938
#define QUERY_BLOCKPEEK		0x939

/////////////////////////////////////////////

#define MAXHEADLEN		100

/////////Data type falg/////////////////

#define	S_MONITORTEMPLET		0x01
#define S_MONITOR				0x02
#define S_GROUP					0x03
#define S_ENTITY				0x04
#define S_RESOURCE				0x05
#define S_SVSE					0x06
#define S_TASK					0x07
#define S_INIFILE				0x08
#define S_ENTITYGROUP			0x09
#define S_ENTITYTEMPLET			0x10
#define S_DB					0x0A
#define S_QUEUE					0x0B

#define S_ENTITYEX				0x0C
#define S_TOPOLOGYCHART			0x0D
#define S_VIRTUALGROUP			0x0E


//////////////operate result/////////////////////

#define	SVDB_OK		0
#define SVDB_FAILED -1

#define INFOLEN		256

struct svdbresult{
	int	cblen;
	int state;
	char info[INFOLEN];
};

typedef struct svdbresult	SVDBRESULT,*LPSVDBRESULT;

////////////PIPE NAME/////////////////
#ifdef	USEPIPE

//const char* SVDB_PIPENAME="\\\\.\\pipe\\DragonFlow_Sitview7.0_SVDB";
//const char* SVDB_REMOETPIPENAME="\\\\%s\\pipe\\DragonFlow_Sitview7.0_SVDB";

#define SVDB_PIPENAME		"\\\\.\\pipe\\DragonFlow_Sitview7.0_SVDB"
#define SVDB_REMOETPIPENAME	"\\\\%s\\pipe\\DragonFlow_Sitview7.0_SVDB"

#define	MAXPIPENAMESIZE	260
#define OUTBUFFERSIZE	4096
#define INBUFFERSIZE	4096
#define MAXSAMEPIPECOUNT	PIPE_UNLIMITED_INSTANCES
#endif

#define	MAXQUEREYSTRINGLEN		200
#define MAXUSERLEN				100

struct svdbquery
{
	int		len;
	int		querytype;
	int		datatype;
	int		datalen;
	char	qstr[MAXQUEREYSTRINGLEN];
	char	idcuser[MAXUSERLEN];
};

typedef struct svdbquery	SVDBQUERY, *LPSVDBQUERY;

///////////////////////////////////////////////inifile////////////////////////////

#define	D_STRING	0x01
#define D_INT		0x02
#define D_BINARY	0x03
#define D_BINARYLEN	0x04
#define D_SECTIONCOUNT 0x05
#define D_KEYCOUNT	0x06
#define D_VALUETYPE	0x07
#define D_SECTION	0x08
#define D_KEY		0x09

#define MAXSECTIONLEN		200
#define MAXKEYLEN			200
#define MAXDEFAULT			50

struct inifilequery
{
	int		len;
	char	section[MAXSECTIONLEN];
	char	key[MAXKEYLEN];
	int		datalen;
	int		datatype;
	char	defaultret[MAXDEFAULT];

};

typedef struct inifilequery	INIQUERY,*LPINIQUERY;

/////////////////////////////////////////////////db////////////////////////////////////

#define	B_DYN		0x01
#define B_RECENTSPAN	0x02
#define B_SPAN		0x03

#include "libutil/time.h"

struct dbquery
{
	int len;
	int type;
	TTime begin;
	TTime end;
	TTimeSpan span;
};

typedef struct dbquery	DBQUERY,*LPDBQUERY;

#endif