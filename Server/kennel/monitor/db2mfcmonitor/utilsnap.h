#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlutil.h>
#include <db2ApiDf.h>
#include <sqlenv.h>

#include <sqlmon.h>
#include <list>
#include <string>

typedef struct std::list<char*> StringList;

//static int item[32];
//static CString itemposstr[32];

class Snapshot
{
  public:
    Snapshot();
    int GetSnapshot(CStringList &paramList,struct sqlma *ma_ptr,char* szReturn);
    static int TurnOnAllMonitorSwitches(void);
    sqlm_header_info *ParseMonitorStream(CStringList &paramList,char *prefix, char *pStart,
                                         char *pEnd,char* szReturn);
	sqlm_header_info *ParseMonitorStream_SubItem(CString strItem,CString strSubItem, char *prefix, char *pStart,
                                         char *pEnd,char* szReturn);
    int UpdateUtilImpactLim(void);

  private:
    int FreeMemory(struct sqlma *ma_ptr, char *buffer_ptr);
     char **elementName;
};
