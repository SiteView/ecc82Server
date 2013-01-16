#include "stdafx.h"
#include <sqlenv.h>
#include "utilapi.h"

#include "sqladef.h"
#include "dirdbname.h"
#include "tabspace.h"

#define SVAPI_API __declspec(dllexport)

//extern "C" int list_db (struct sqlca sqlca, char * param, char *szReturn, int * nSize);
//extern "C" void tabspaceInfo (struct SQLB_TBSPQRY_DATA *dataP, sqluint32 num);

extern "C" SVAPI_API
 bool DatabaseDir(const char *param, char *szReturn, int & nSize)
{
	OutputDebugString("------------DatabaseDir() start call-----------\n");
	struct sqlca sqlca;
	char dbName[32] = {0};
	char dbAlias[32] = {0};
	unsigned char dbType = SQL_REMOTE;
	char nodeName[] = "";
	char *dbPath = NULL;
	char dbComment[] = " ";
	unsigned short dbAuthentication = SQL_AUTHENTICATION_SERVER;	

	sqlecadb(dbName,
           dbAlias,
           dbType,
           nodeName,
           dbPath,
           dbComment,
           dbAuthentication,
           NULL,
           &sqlca);

	/* Ignore warning SQL1100W */
	if (SQLCODE != 1100) ;

	list_db (sqlca, (char*)param, szReturn, &nSize);
	sqleuncd (dbName, &sqlca);
	return true;
}

extern "C" SVAPI_API
 bool GetTableSpace(const char *param, char *szReturn, int & nSize)
{
	OutputDebugString("-----------GetTableSpace() start call-----------------\n");
	Call_GetTableSpace(param, szReturn, nSize);
	return true;
}
