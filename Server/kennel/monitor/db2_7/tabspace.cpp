#include "stdafx.h"
#include "tabspace.h"
#include <string>

int rc;

using namespace std;

static char sqla_program_id[162] = 
{
 47,0,65,68,65,74,65,72,84,65,66,83,80,65,67,69,88,66,117,81,
 79,90,67,88,48,49,49,49,49,50,32,32,13,0,65,68,77,73,78,73,
 83,84,82,65,84,79,82,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0
};

//#include "sqladef.h"

static struct sqla_runtime_info sqla_rtinfo = 
{{'S','Q','L','A','R','T','I','N'}, sizeof(wchar_t), 0, {' ',' ',' ',' '}};


#line 1 "tabspace.sqc"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sqlutil.h>
#include <sqlenv.h>
#include "utilemb.h"


/*
EXEC SQL INCLUDE SQLCA;
*/

/* SQL Communication Area - SQLCA - structures and constants */
#include "sqlca.h"
struct sqlca sqlca;


#line 73 "tabspace.sqc"


bool MakeCharByString(char *pOut,int &nOutSize,string strInput )
{
	 char *p;
	
	int nSize=strInput.length();
	if(nSize+2 <nOutSize)
	{
		strcpy(pOut,strInput.c_str());
	}else return false;
	p=pOut;
	//printf("%d\n",nSize);
	for(int i=0;i<nSize;i++)
	{
		if(*p=='$') 	*p='\0';
		p++;
	}
	nOutSize=nSize+1;
	return true;
	
}

//void tabspaceInfo (struct SQLB_TBSPQRY_DATA *, sqluint32);

//int main (int argc, char *argv[]) {
bool Call_GetTableSpace(const char *param, char *szReturn, int & nSize1)
{
   struct sqlca sqlca;
   struct SQLB_TBSPQRY_DATA *dataP;
   struct SQLB_TBSPQRY_DATA **dataPP;
   sqluint32 numTS, maxTS, numT, index;
   sqluint32 i;
   
/*
EXEC SQL BEGIN DECLARE SECTION;
*/

#line 84 "tabspace.sqc"

      char dbname[9];
      char userid[9];
      char passwd[19];
   
/*
EXEC SQL END DECLARE SECTION;
*/

#line 88 "tabspace.sqc"


 

   char *p = NULL;
   int nSize = 0;
   int pos = 0;

   p=(char*)param;
	
	while(*p!='\0')
	{
		nSize = static_cast<int>(strlen(p));
		if( nSize>0 )
		{	
			string fstring = p;
			pos = fstring.find("=", 0);

			string strpos = fstring.substr(0, pos);
			string strvalue = fstring.substr(pos + 1, fstring.length() - pos - 1);
			if(pos >= 0)
			{
				if(!strcmp(strpos.c_str(), "_Db2DatabaseName"))
				{
					strcpy(dbname, strvalue.c_str());
					OutputDebugString("----------Database--------\n");
					OutputDebugString(dbname);
					OutputDebugString("\n");
				}
				else if(!strcmp(strpos.c_str(), "_UserAccount"))
				{
					strcpy(userid, strvalue.c_str());
					OutputDebugString("----------UserAccount--------\n");
					OutputDebugString(userid);
					OutputDebugString("\n");
				}
				else if(!strcmp(strpos.c_str(), "_PassWord"))
				{
					strcpy(passwd, strvalue.c_str());
					OutputDebugString("----------PassWord--------\n");
					OutputDebugString(passwd);
					OutputDebugString("\n");
				}
				
			}
			//pList.AddHead(p);
		
		}
		p=p+nSize+1;
	}

 //  strcpy(dbname, argv[1]);
//   strcpy(userid, argv[2]);
 //  strcpy(passwd, argv[3]);

   
/*
EXEC SQL CONNECT TO :dbname USER :userid USING :passwd;
*/

{
#line 102 "tabspace.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 102 "tabspace.sqc"
  sqlaaloc(2,3,1,0L);
    {
      struct sqla_setd_list sql_setdlist[3];
#line 102 "tabspace.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 9;
#line 102 "tabspace.sqc"
      sql_setdlist[0].sqldata = (void*)dbname;
#line 102 "tabspace.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 102 "tabspace.sqc"
      sql_setdlist[1].sqltype = 460; sql_setdlist[1].sqllen = 9;
#line 102 "tabspace.sqc"
      sql_setdlist[1].sqldata = (void*)userid;
#line 102 "tabspace.sqc"
      sql_setdlist[1].sqlind = 0L;
#line 102 "tabspace.sqc"
      sql_setdlist[2].sqltype = 460; sql_setdlist[2].sqllen = 19;
#line 102 "tabspace.sqc"
      sql_setdlist[2].sqldata = (void*)passwd;
#line 102 "tabspace.sqc"
      sql_setdlist[2].sqlind = 0L;
#line 102 "tabspace.sqc"
      sqlasetda(2,0,3,sql_setdlist,NULL,0L);
    }
#line 102 "tabspace.sqc"
  sqlacall((unsigned short)29,5,2,0,0L);
#line 102 "tabspace.sqc"
  sqlastop(0L);
}

#line 102 "tabspace.sqc"

   EMB_SQL_CHECK("CONNECT TO DATABASE");

  
   /**********************************\
   * OPEN TABLESPACE QUERY API called *
   \**********************************/
   sqlbotsq (&sqlca, SQLB_OPEN_TBS_ALL, &numTS);
   EMB_SQL_CHECK("OPEN TABLESPACE QUERY");
   maxTS = numTS;
   dataP = (struct SQLB_TBSPQRY_DATA *) malloc (numTS *
      sizeof (struct SQLB_TBSPQRY_DATA));
   strcpy(dataP->tbspqver,SQLB_TBSPQRY_DATA_ID);

   /***********************************\
   * FETCH TABLESPACE QUERY API called *
   \***********************************/
   sqlbftpq (&sqlca, maxTS, dataP, &numTS);
   EMB_SQL_CHECK("FETCH TABLESPACE QUERY");
   tabspaceInfo (dataP, numTS, szReturn, nSize1);
   /************************************\
   * CLOSE TABLESPACE QUERY API called *
   \************************************/
   sqlbctsq (&sqlca);
   EMB_SQL_CHECK("CLOSE TABLESPACE QUERY");

 
   /************************************\
   * SINGLE TABLESPACE QUERY API called *
   \************************************/
   sqlbstpq (&sqlca, 1, dataP, SQLB_RESERVED1);
   EMB_SQL_CHECK("SINGLE TABLESPACE QUERY");
  // tabspaceInfo (dataP, 1, szReturn, nSize1);

   printf ("Using the TABLESPACE QUERY API\n");
   printf ("==============================\n");
   /*****************************\
   * TABLESPACE QUERY API called *
   \*****************************/
   sqlbmtsq (&sqlca, &numTS, &(dataPP), SQLB_RESERVED1, SQLB_RESERVED2);
   EMB_SQL_CHECK("TABLESPACE QUERY");
   for (i=0; i < numTS; i++ )
 //     tabspaceInfo (dataPP[i], 1, szReturn, nSize1);

   /************************\
   * FREE MEMORY API called *
   \************************/
   sqlefmem (&sqlca, dataPP);
   EMB_SQL_CHECK("FREE MEMORY");

   
/*
EXEC SQL CONNECT RESET;
*/

{
#line 155 "tabspace.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 155 "tabspace.sqc"
  sqlacall((unsigned short)29,3,0,0,0L);
#line 155 "tabspace.sqc"
  sqlastop(0L);
}

#line 155 "tabspace.sqc"

   EMB_SQL_CHECK("CONNECT RESET");

   return 0;
}

void tabspaceInfo (struct SQLB_TBSPQRY_DATA *dataP, sqluint32 num, char *szReturn, int &nSize) {
	int nTotal = 0;
	int nUsed = 0;
	int nFree = 0;

   struct sqlca sqlca;
   struct SQLB_TBS_STATS tbs_stats;
   sqluint32 idx;
   for (idx=0; idx < num; idx++, dataP++) {
      printf ("Tablespace ID                   = %ld\n", dataP->id);
      printf ("Tablespace Name                 = %s\n", dataP->name);

      /* "Type" and "Content" are stored bitwise in the flag field */
      OutputDebugString ("Type                            = ");
      switch (dataP->flags & 0xF ) {
         case SQLB_TBS_SMS:
            OutputDebugString ("System managed space\n");
            break;
         case SQLB_TBS_DMS:
            OutputDebugString ("Database managed space\n");
            break;
         default:
            OutputDebugString ("UNKNOWN\n");
            break;
      } /* endswitch */

      OutputDebugString ("Contents                        = ");
      switch (dataP->flags & 0xF0) {
         case SQLB_TBS_ANY:
            OutputDebugString ("regular contents\n");
            break;
         case SQLB_TBS_LONG:
            OutputDebugString ("long field data\n");
            break;
         case SQLB_TBS_TMP:
            OutputDebugString ("temp data\n");
            break;
         default:
            printf ("UNKNOWN :%d:\n", dataP->flags & 0xF0);
            break;
      } /* endswitch */

    
      /***************************\
      * GET TABLESPACE STATISTICS *
      \***************************/
      sqlbgtss(&sqlca, dataP->id, &tbs_stats);

	  nTotal += tbs_stats.totalPages;
	  nUsed += tbs_stats.usedPages;
	  nFree += tbs_stats.freePages;

      printf ("Tablespace Statistics\n");
      printf ("total pages                     = %ld\n", tbs_stats.totalPages);
      printf ("useable pages                   = %ld\n",
         tbs_stats.useablePages);
      printf ("used pages                      = %ld\n", tbs_stats.usedPages);
      printf ("free pages                      = %ld\n", tbs_stats.freePages);
      printf ("high water mark                 = %ld\n",
         tbs_stats.highWaterMark);

      printf ("\n\n");
   } /* endfor */
	
   string sReturn;
   char strReturn[128] = {0};
	sprintf(strReturn, "TOTALPAGES=%ld$USEDPAGES=%ld$FREEDPAGES=%ld$", nTotal, nUsed, nFree);
	OutputDebugString("---------table space result output----------\n");
	OutputDebugString(strReturn);

	sReturn = strReturn;
	MakeCharByString(szReturn, nSize, sReturn);

}

