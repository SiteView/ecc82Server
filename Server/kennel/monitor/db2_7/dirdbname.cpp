#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sqlenv.h>


#include "utilapi.h"

using namespace std;

#if ( defined(DB2OS2) | defined(DB2WIN))
   #define DRIVE_SZ 2
#else
   /* The "drive" field of the SQLEDINFO is truncated to 50 characters for
      display purposes in this program.  The actual length of this field is
      215 */
   #define DRIVE_SZ 50
#endif
#define DB_PATH NULL
#define DB_NODE "db2test"  
#define DB_TYPE SQL_REMOTE 



//int list_db (struct sqlca, char *szReturn, int * nSize);



int list_db (struct sqlca sqlca, char *param, char *szReturn, int * nSize1) {
   unsigned short index;
   unsigned short dbHandle;
   unsigned short dbCount;
   struct sqledinfo *dbBuffer;
   int size = 0;
   char temp[32] = {0};
   char nodename[9] = {0};

   char* p ;
   int nSize;
   int pos;
   string fstring;
   string m_InstanceName;

   /*****************************************\
   * OPEN DATABASE DIRECTORY SCAN API called *
   \*****************************************/
   sqledosd ("\0", &dbHandle, &dbCount, &sqlca);
   if (sqlca.sqlcode == SQLE_RC_NODBDIR) {
   //   printf ("--- Database directory is empty ---\n");
      return 1;
   } else {
  //    API_SQL_CHECK("opening the database directory scan API");
   } /* endif */

   *nSize1 = 0;
   
	p=param;
	
	while(*p!='\0')
	{

		nSize = static_cast<int>(strlen(p));
		if( nSize>0 )
		{	
			fstring = p;
			pos = fstring.find("DatabaseDir=", 0);
			if(pos >= 0)
			{
				m_InstanceName = fstring.substr(pos + 12, fstring.length() - pos -1);
				
				OutputDebugString("---------dir database name--------\n");
				OutputDebugString(m_InstanceName.c_str());
				break;//如果找到实例节点则退出循环
			}
			//pList.AddHead(p);
		
		}
		p=p+nSize+1;
	}
	
	char strdbcount[64] = {0};
	sprintf(strdbcount, "*************----start sqledgne for  count: %d----*********\n", dbCount);
	OutputDebugString(strdbcount);
   for (index = 0; index < dbCount; index++) {
      /************************************\
      * GET NEXT DATABASE ENTRY API called *
      \************************************/
	   OutputDebugString("*************----again start sqledgne for----*********\n");
      sqledgne (dbHandle, &dbBuffer, &sqlca);

	  memset(nodename, 0, 9);
	  sprintf(nodename, "%.8s", dbBuffer->nodename);

	  OutputDebugString("--------nodename output-----------\n");
	  OutputDebugString(nodename);

	  if(!strcmp(nodename,"        "))
	  {
		if(!strcmp(m_InstanceName.c_str(),"LOCAL"))
		{
			
			memset(temp, 0, 32);
			sprintf(temp, "%.8s=%.8s", dbBuffer->alias, dbBuffer->alias);
			
			OutputDebugString("--------LOCAL database--------------\n");
			OutputDebugString(temp);

			memcpy(szReturn + size, temp, 17);
			size += (17 + 1);
		}
	  }
	  else
	  {
		  if(!strcmp(m_InstanceName.c_str(), nodename))
		  {
			memset(temp, 0, 32);
			
			sprintf(temp, "%.8s=%.8s", dbBuffer->alias, dbBuffer->alias);
			
			OutputDebugString("----------database name output----------\n");
			OutputDebugString(temp);

			memcpy(szReturn + size, temp, 17);
			size += (17 + 1);
		  }
	  }
	  
      /* printing out the node information on to the screen */
      /*printf ("alias name :            %.8s\n", dbBuffer->alias);
      printf ("database name :         %.8s\n", dbBuffer->dbname);
      printf ("database drive :        %.*s\n",DRIVE_SZ, dbBuffer->drive);
      printf ("database directory :    %.8s\n", dbBuffer->intname);
      printf ("node name :             %.8s\n", dbBuffer->nodename);
      printf ("database release type : %.20s\n", dbBuffer->dbtype);
      printf ("database comment :      %.30s\n", dbBuffer->comment);
      printf ("database entry type :   %d\n", dbBuffer->type);
	  */
  
   } /* endfor */
   /******************************************\
   * CLOSE DATABASE DIRECTORY SCAN API called *
   \******************************************/
   sqledcls (dbHandle, &sqlca);
//   API_SQL_CHECK("closing the node directory scan");

   *nSize1 = size;
   return 0;
}


