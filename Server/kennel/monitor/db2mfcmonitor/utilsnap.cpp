
#include "stdafx.h"
//#include "db2mfcmonitor.cpp"
#include "utilsnap.h"
#if ((__cplusplus >= 199711L) && !defined DB2HP && !defined DB2AIX) || \
    (DB2LINUX && (__LP64__ || (__GNUC__ >= 3)) )
   #include <iomanip>
   #include <iostream>
   using namespace std;
#else
   #include <iomanip.h>
   #include <iostream.h>
#endif

#define SNAPSHOT_BUFFER_UNIT_SZ 1024
#define NUMELEMENTS 790
#define BLANKS "                                                                             "
#define DASHES "\n----------------------------------------------------------------------"


extern char * szReturn1;
static BOOL flag;

int ti = 0;
 int tlen = 0;
char *FindStrValue(const char *strParas, CString str);

/***************************************************************************/
/* GetSnapshot                                                             */
/* This function is called from one of insnap, clisnap, or dbsnap. Here,   */
/* the buffer used to contain the self-describing monitor data is          */
/* initialized (and later freed), the data structures used by the          */
/* db2GetSnapshot API are populated, and the snapshot is captured. The     */
/* snapshot monitor data buffer is then passed to the ParseMonitorStream   */
/* function, which parses and prints the data. Once the ParseMonitorStream */
/* is finished running, the resources used by the snapshot monitor samples */
/* are freed with the FreeMemory function.                                 */
/***************************************************************************/
int Snapshot::GetSnapshot(CStringList &paramList,struct sqlma *ma_ptr,char* szReturn)
{
  int rc = 0;  
  struct sqlm_collected collected;  
  struct sqlca sqlca;
  char *buffer_ptr = NULL;  
  sqluint32 buffer_sz;  
  sqluint32 outputFormat;
  db2GetSnapshotData getSnapshotParam;
  db2GetSnapshotSizeData getSnapshotSizeParam;
  CRITICAL_SECTION m_Lock;			//创建线程锁，保护线程共享资源。

  InitializeCriticalSection(&m_Lock);	//初始化线程锁。

  EnterCriticalSection(&m_Lock);		//启动线程锁
  memset(&collected, '\0', sizeof(struct sqlm_collected));

  getSnapshotSizeParam.piSqlmaData = ma_ptr;
  getSnapshotSizeParam.poBufferSize = &buffer_sz;
  getSnapshotSizeParam.iVersion = SQLM_CURRENT_VERSION;
//  getSnapshotSizeParam.iVersion = SQLM_DBMON_VERSION6;
  getSnapshotSizeParam.iNodeNumber = SQLM_CURRENT_NODE;
  getSnapshotSizeParam.iSnapshotClass = SQLM_CLASS_DEFAULT;
  rc = db2GetSnapshotSize(db2Version810, &getSnapshotSizeParam, &sqlca);

   if (rc != 0)
  {
    FreeMemory(ma_ptr, buffer_ptr);
    return(rc);
  }

   if (sqlca.sqlcode != 0L)
  {
    if (sqlca.sqlcode < 0L)
    {
      FreeMemory(ma_ptr, buffer_ptr);
      return((int)sqlca.sqlcode);
    }
  }

   if (buffer_sz == 0)
  {
    FreeMemory(ma_ptr, buffer_ptr);
    return(99);
  }

  buffer_ptr = (char *) malloc(buffer_sz);
  memset(buffer_ptr,0,buffer_sz);
  if (buffer_ptr == NULL)
  {
    FreeMemory(ma_ptr, buffer_ptr);
    return(99);
  }
  memset(buffer_ptr, '\0', buffer_sz);

  getSnapshotParam.piSqlmaData = ma_ptr;
  getSnapshotParam.poCollectedData = &collected;
  getSnapshotParam.iBufferSize = buffer_sz;
  getSnapshotParam.poBuffer = buffer_ptr;
  getSnapshotParam.iVersion = SQLM_CURRENT_VERSION;
  getSnapshotParam.iStoreResult = 0;
  getSnapshotParam.iNodeNumber = SQLM_CURRENT_NODE;
  getSnapshotParam.poOutputFormat = &outputFormat;
  getSnapshotParam.iSnapshotClass = SQLM_CLASS_DEFAULT;
  //getSnapshotParam.iSnapshotClass = SQLM_CLASS_HEALTH_WITH_DETAIL ;
  cout << DASHES;
  cout << "\nCapturing snapshot using db2GetSnapshot.";
  cout << DASHES;
  
 // memset(szReturn,0,buffer_sz);

  rc = db2GetSnapshot(db2Version810, &getSnapshotParam, &sqlca);

  while (sqlca.sqlcode == 1606)
  {
    FreeMemory(NULL, buffer_ptr);


   buffer_sz = buffer_sz + SNAPSHOT_BUFFER_UNIT_SZ;

    buffer_ptr = (char *) realloc(szReturn,buffer_sz*2);
    if (buffer_ptr == NULL)
    {
      FreeMemory(ma_ptr, buffer_ptr);
      return(99);
    }
    memset(buffer_ptr, '\0', buffer_sz);

    getSnapshotParam.iBufferSize = buffer_sz;
    getSnapshotParam.poBuffer = buffer_ptr;

    rc = db2GetSnapshot(db2Version810, &getSnapshotParam, &sqlca);
  }
  
  if (rc != 0)
  {
    FreeMemory(ma_ptr, buffer_ptr);
    return rc;
  }

  if (sqlca.sqlcode != 0L)
  {
    if (sqlca.sqlcode == 1611)
    {
    }
    if (sqlca.sqlcode < 0L)
    {
      FreeMemory(ma_ptr, buffer_ptr);
      return((int)sqlca.sqlcode);
    }
  }

  CString strReturn="";
  ParseMonitorStream(paramList," ", buffer_ptr, NULL,szReturn);

  FreeMemory(ma_ptr, buffer_ptr);
  LeaveCriticalSection(&m_Lock);

  return rc;
} // GetSnapshot

/***************************************************************************/
/* TurnOnAllMonitorSwitches                                                */
/* Turn on all of the monitor switches.                                    */
/***************************************************************************/
int Snapshot::TurnOnAllMonitorSwitches(void)
{
  int rc = 0;
  struct sqlca sqlca;
  db2MonitorSwitchesData switchesData;
  struct sqlm_recording_group switchesList[SQLM_NUM_GROUPS];
  sqluint32 outputFormat;

  
   switchesList[SQLM_UOW_SW].input_state = SQLM_ON;
   switchesList[SQLM_STATEMENT_SW].input_state = SQLM_ON;
   switchesList[SQLM_TABLE_SW].input_state = SQLM_ON;
   switchesList[SQLM_BUFFER_POOL_SW].input_state = SQLM_ON;
   switchesList[SQLM_LOCK_SW].input_state = SQLM_ON;
   switchesList[SQLM_SORT_SW].input_state = SQLM_ON;
   switchesList[SQLM_TIMESTAMP_SW].input_state = SQLM_ON;
   switchesData.piGroupStates = switchesList;
   switchesData.poBuffer = NULL;
   switchesData.iVersion = SQLM_CURRENT_VERSION;
   switchesData.iBufferSize = 0;
   switchesData.iReturnData = 0;
   switchesData.iNodeNumber = SQLM_CURRENT_NODE;
   switchesData.poOutputFormat = &outputFormat;
   db2MonitorSwitches(db2Version810, &switchesData, &sqlca);

  if (sqlca.sqlcode != 0L)
  {
    if (sqlca.sqlcode < 0L)
    {
      return((int)sqlca.sqlcode);
    }
  }

  return rc;
} //Snapshot::TurnOnAllMonitorSwitches

/***************************************************************************/
/* FreeMemory                                                              */
/* General cleanup routine to release memory buffers.                      */
/***************************************************************************/
int Snapshot::FreeMemory(struct sqlma *ma_ptr, char *buffer_ptr)
{
  int rc = 0;

  if (buffer_ptr != NULL)
    free(buffer_ptr);

  if (ma_ptr != NULL)
    free(ma_ptr);

  return rc;
}  

/***************************************************************************/
/* Snapshot constructor                                                    */
/* Initialize the array of element names based on the defines in sqlmon.h. */
/***************************************************************************/
Snapshot::Snapshot()
{
  int i = 0;
  int arraySize = 0;
  char *c;

  elementName = new char *[NUMELEMENTS];

  arraySize = NUMELEMENTS * sizeof(elementName[0][0]);
  memset(elementName,'\0',arraySize);

 
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DB2"); elementName[1]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_FCM"); elementName[2]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_FCM_NODE"); elementName[3]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_APPL_INFO"); elementName[4]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_APPL"); elementName[5]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DCS_APPL_INFO"); elementName[6]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DCS_APPL"); elementName[7]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DCS_STMT"); elementName[8]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SUBSECTION"); elementName[9]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_AGENT"); elementName[10]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOCK_WAIT"); elementName[11]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DCS_DBASE"); elementName[12]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DBASE"); elementName[13]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_ROLLFORWARD"); elementName[14]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLE"); elementName[15]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOCK"); elementName[16]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE"); elementName[17]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_BUFFERPOOL"); elementName[18]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DYNSQL"); elementName[19]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_COLLECTED"); elementName[20]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SWITCH_LIST"); elementName[21]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_UOW_SW"); elementName[22]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_STATEMENT_SW"); elementName[23]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLE_SW"); elementName[24]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_BUFFPOOL_SW"); elementName[25]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOCK_SW"); elementName[26]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SORT_SW"); elementName[27]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLE_LIST"); elementName[28]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_LIST"); elementName[29]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DYNSQL_LIST"); elementName[30]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_APPL_LOCK_LIST"); elementName[31]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DB_LOCK_LIST"); elementName[32]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_STMT"); elementName[33]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DBASE_REMOTE"); elementName[34]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_APPL_REMOTE"); elementName[35]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_APPL_ID_INFO"); elementName[36]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_STMT_TRANSMISSIONS"); elementName[37]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TIMESTAMP_SW"); elementName[38]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLE_REORG"); elementName[39]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MEMORY_POOL"); elementName[40]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_QUIESCER"); elementName[41]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_CONTAINER"); elementName[42]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_RANGE"); elementName[43]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_RANGE_CONTAINER"); elementName[44]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_NODEINFO"); elementName[45]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_HEALTH_INDICATOR"); elementName[46]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_HEALTH_INDICATOR_HIST"); elementName[47]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_BUFFERPOOL_NODEINFO"); elementName[48]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_UTILITY"); elementName[49]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_EVENT_DB"); elementName[100]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_EVENT_CONN"); elementName[101]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_EVENT_TABLE"); elementName[102]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_EVENT_STMT"); elementName[103]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_EVENT_XACT"); elementName[104]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_EVENT_DEADLOCK"); elementName[105]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_EVENT_DLCONN"); elementName[106]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_EVENT_TABLESPACE"); elementName[107]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_EVENT_DBHEADER"); elementName[108]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_EVENT_START"); elementName[109]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_EVENT_CONNHEADER"); elementName[110]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_EVENT_OVERFLOW"); elementName[111]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_EVENT_BUFFERPOOL"); elementName[112]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_EVENT_SUBSECTION"); elementName[113]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_EVENT_LOG_HEADER"); elementName[114]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_EVENT_CONTROL"); elementName[115]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_EVENT_LOCK_LIST"); elementName[116]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_EVENT_DBMEMUSE"); elementName[117]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_EVENT_CONNMEMUSE"); elementName[118]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_EVENT_DETAILED_DLCONN"); elementName[119]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TIME_STAMP"); elementName[200]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_STATUS_CHANGE_TIME"); elementName[201]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_GW_CON_TIME"); elementName[202]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_PREV_UOW_STOP_TIME"); elementName[203]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_UOW_START_TIME"); elementName[204]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_UOW_STOP_TIME"); elementName[205]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_STMT_START"); elementName[206]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_STMT_STOP"); elementName[207]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LAST_RESET"); elementName[208]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DB2START_TIME"); elementName[209]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DB_CONN_TIME"); elementName[210]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LAST_BACKUP"); elementName[211]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOCK_WAIT_START_TIME"); elementName[212]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_APPL_CON_TIME"); elementName[213]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CONN_COMPLETE_TIME"); elementName[214]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DISCONN_TIME"); elementName[215]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_EVENT_TIME"); elementName[216]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_START_TIME"); elementName[217]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_STOP_TIME"); elementName[218]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_RF_TIMESTAMP"); elementName[219]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CONN_TIME"); elementName[220]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_FIRST_OVERFLOW_TIME"); elementName[221]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LAST_OVERFLOW_TIME"); elementName[222]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_GW_EXEC_TIME"); elementName[223]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_AGENT_USR_CPU_TIME"); elementName[224]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_AGENT_SYS_CPU_TIME"); elementName[225]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SS_USR_CPU_TIME"); elementName[226]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SS_SYS_CPU_TIME"); elementName[227]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_USER_CPU_TIME"); elementName[228]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TOTAL_EXEC_TIME"); elementName[229]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SWITCH_SET_TIME"); elementName[230]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_ELAPSED_EXEC_TIME"); elementName[231]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SELECT_TIME"); elementName[232]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_INSERT_TIME"); elementName[233]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_UPDATE_TIME"); elementName[234]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DELETE_TIME"); elementName[235]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CREATE_NICKNAME_TIME"); elementName[236]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_PASSTHRU_TIME"); elementName[237]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_STORED_PROC_TIME"); elementName[238]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_REMOTE_LOCK_TIME"); elementName[239]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_NETWORK_TIME_TOP"); elementName[240]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_NETWORK_TIME_BOTTOM"); elementName[241]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_REBALANCER_START_TIME"); elementName[242]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_REBALANCER_RESTART_TIME"); elementName[243]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_MIN_RECOVERY_TIME"); elementName[244]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_HI_TIMESTAMP"); elementName[245]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SECONDS"); elementName[300]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MICROSEC"); elementName[301]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_AGENT_ID"); elementName[302]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SERVER_DB2_TYPE"); elementName[303]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SERVER_PRDID"); elementName[304]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SERVER_NNAME"); elementName[305]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SERVER_INSTANCE_NAME"); elementName[306]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_NODE_NUMBER"); elementName[307]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TIME_ZONE_DISP"); elementName[308]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SERVER_VERSION"); elementName[309]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_APPL_STATUS"); elementName[310]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CODEPAGE_ID"); elementName[311]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_STMT_TEXT"); elementName[312]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_APPL_NAME"); elementName[313]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_APPL_ID"); elementName[314]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SEQUENCE_NO"); elementName[315]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_AUTH_ID"); elementName[316]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CLIENT_NNAME"); elementName[317]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CLIENT_PRDID"); elementName[318]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_INPUT_DB_ALIAS"); elementName[319]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CLIENT_DB_ALIAS"); elementName[320]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DB_NAME"); elementName[321]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DB_PATH"); elementName[322]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_NUM_ASSOC_AGENTS"); elementName[323]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_COORD_NODE_NUM"); elementName[324]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_AUTHORITY_LVL"); elementName[325]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_EXECUTION_ID"); elementName[326]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CORR_TOKEN"); elementName[327]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CLIENT_PID"); elementName[328]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CLIENT_PLATFORM"); elementName[329]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CLIENT_PROTOCOL"); elementName[330]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_COUNTRY_CODE"); elementName[331]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_COORD_AGENT_PID"); elementName[332]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_GW_DB_ALIAS"); elementName[333]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_OUTBOUND_COMM_ADDRESS"); elementName[334]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_INBOUND_COMM_ADDRESS"); elementName[335]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_OUTBOUND_COMM_PROTOCOL"); elementName[336]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DCS_DB_NAME"); elementName[337]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_HOST_DB_NAME"); elementName[338]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_HOST_PRDID"); elementName[339]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_OUTBOUND_APPL_ID"); elementName[340]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_OUTBOUND_SEQUENCE_NO"); elementName[341]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DCS_APPL_STATUS"); elementName[342]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_HOST_CCSID"); elementName[343]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_OUTPUT_STATE"); elementName[344]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_COUNT"); elementName[345]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_ROWS_SELECTED"); elementName[346]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SQL_STMTS"); elementName[347]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_FAILED_SQL_STMTS"); elementName[348]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_COMMIT_SQL_STMTS"); elementName[349]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_ROLLBACK_SQL_STMTS"); elementName[350]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_INBOUND_BYTES_RECEIVED"); elementName[351]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_OUTBOUND_BYTES_SENT"); elementName[352]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_OUTBOUND_BYTES_RECEIVED"); elementName[353]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_INBOUND_BYTES_SENT"); elementName[354]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_STMT_OPERATION"); elementName[355]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SECTION_NUMBER"); elementName[356]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOCK_NODE"); elementName[357]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CREATOR"); elementName[358]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_PACKAGE_NAME"); elementName[359]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_APPL_IDLE_TIME"); elementName[360]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_OPEN_CURSORS"); elementName[361]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_UOW_COMP_STATUS"); elementName[362]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SEQUENCE_NO_HOLDING_LK"); elementName[363]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_ROLLED_BACK_AGENT_ID"); elementName[364]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_ROLLED_BACK_APPL_ID"); elementName[365]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_ROLLED_BACK_SEQUENCE_NO"); elementName[366]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_XID"); elementName[367]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TPMON_CLIENT_USERID"); elementName[368]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TPMON_CLIENT_WKSTN"); elementName[369]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TPMON_CLIENT_APP"); elementName[370]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TPMON_ACC_STR"); elementName[371]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_QUERY_COST_ESTIMATE"); elementName[372]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_QUERY_CARD_ESTIMATE"); elementName[373]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_FETCH_COUNT"); elementName[374]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_GW_TOTAL_CONS"); elementName[375]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_GW_CUR_CONS"); elementName[376]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_GW_CONS_WAIT_HOST"); elementName[377]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_GW_CONS_WAIT_CLIENT"); elementName[378]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_GW_CONNECTIONS_TOP"); elementName[379]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SORT_HEAP_ALLOCATED"); elementName[380]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POST_THRESHOLD_SORTS"); elementName[381]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_PIPED_SORTS_REQUESTED"); elementName[382]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_PIPED_SORTS_ACCEPTED"); elementName[383]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DL_CONNS"); elementName[384]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_REM_CONS_IN"); elementName[385]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_REM_CONS_IN_EXEC"); elementName[386]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOCAL_CONS"); elementName[387]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOCAL_CONS_IN_EXEC"); elementName[388]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CON_LOCAL_DBASES"); elementName[389]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_AGENTS_REGISTERED"); elementName[390]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_AGENTS_WAITING_ON_TOKEN"); elementName[391]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DB2_STATUS"); elementName[392]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_AGENTS_REGISTERED_TOP"); elementName[393]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_AGENTS_WAITING_TOP"); elementName[394]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_COMM_PRIVATE_MEM"); elementName[395]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_IDLE_AGENTS"); elementName[396]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_AGENTS_FROM_POOL"); elementName[397]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_AGENTS_CREATED_EMPTY_POOL"); elementName[398]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_AGENTS_TOP"); elementName[399]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_COORD_AGENTS_TOP"); elementName[400]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_AGENT_OVERFLOWS"); elementName[401]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_AGENTS_STOLEN"); elementName[402]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_PRODUCT_NAME"); elementName[403]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_COMPONENT_ID"); elementName[404]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SERVICE_LEVEL"); elementName[405]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POST_THRESHOLD_HASH_JOINS"); elementName[406]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_BUFF_FREE"); elementName[407]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_BUFF_FREE_BOTTOM"); elementName[408]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MA_FREE"); elementName[409]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MA_FREE_BOTTOM"); elementName[410]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CE_FREE"); elementName[411]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CE_FREE_BOTTOM"); elementName[412]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_RB_FREE"); elementName[413]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_RB_FREE_BOTTOM"); elementName[414]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CONNECTION_STATUS"); elementName[416]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TOTAL_BUFFERS_SENT"); elementName[417]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TOTAL_BUFFERS_RCVD"); elementName[418]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOCKS_HELD"); elementName[419]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOCK_WAITS"); elementName[420]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOCK_WAIT_TIME"); elementName[421]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOCK_LIST_IN_USE"); elementName[422]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DEADLOCKS"); elementName[423]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOCK_ESCALS"); elementName[424]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_X_LOCK_ESCALS"); elementName[425]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOCKS_WAITING"); elementName[426]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TOTAL_SORTS"); elementName[427]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TOTAL_SORT_TIME"); elementName[428]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SORT_OVERFLOWS"); elementName[429]=c;		/* number of sort overflows          */
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_ACTIVE_SORTS"); elementName[430]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_DATA_L_READS"); elementName[431]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_DATA_P_READS"); elementName[432]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_DATA_WRITES"); elementName[433]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_INDEX_L_READS"); elementName[434]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_INDEX_P_READS"); elementName[435]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_INDEX_WRITES"); elementName[436]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_READ_TIME"); elementName[437]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_WRITE_TIME"); elementName[438]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_FILES_CLOSED"); elementName[439]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DYNAMIC_SQL_STMTS"); elementName[440]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_STATIC_SQL_STMTS"); elementName[441]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SELECT_SQL_STMTS"); elementName[442]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DDL_SQL_STMTS"); elementName[443]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_UID_SQL_STMTS"); elementName[444]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_INT_AUTO_REBINDS"); elementName[445]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_INT_ROWS_DELETED"); elementName[446]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_INT_ROWS_UPDATED"); elementName[447]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_INT_COMMITS"); elementName[448]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_INT_ROLLBACKS"); elementName[449]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_INT_DEADLOCK_ROLLBACKS"); elementName[450]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_ROWS_DELETED"); elementName[451]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_ROWS_INSERTED"); elementName[452]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_ROWS_UPDATED"); elementName[453]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_BINDS_PRECOMPILES"); elementName[454]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOCKS_HELD_TOP"); elementName[455]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_NUM_NODES_IN_DB2_INSTANCE"); elementName[456]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TOTAL_CONS"); elementName[457]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_APPLS_CUR_CONS"); elementName[458]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_APPLS_IN_DB2"); elementName[459]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SEC_LOG_USED_TOP"); elementName[460]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TOT_LOG_USED_TOP"); elementName[461]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SEC_LOGS_ALLOCATED"); elementName[462]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_ASYNC_INDEX_READS"); elementName[463]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_DATA_TO_ESTORE"); elementName[464]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_INDEX_TO_ESTORE"); elementName[465]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_INDEX_FROM_ESTORE"); elementName[466]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_DATA_FROM_ESTORE"); elementName[467]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DB_STATUS"); elementName[468]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOCK_TIMEOUTS"); elementName[469]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CONNECTIONS_TOP"); elementName[470]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DB_HEAP_TOP"); elementName[471]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_ASYNC_DATA_READS"); elementName[472]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_ASYNC_DATA_WRITES"); elementName[473]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_ASYNC_INDEX_WRITES"); elementName[474]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_ASYNC_READ_TIME"); elementName[475]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_ASYNC_WRITE_TIME"); elementName[476]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_ASYNC_DATA_READ_REQS"); elementName[477]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_LSN_GAP_CLNS"); elementName[478]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_DRTY_PG_STEAL_CLNS"); elementName[479]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_DRTY_PG_THRSH_CLNS"); elementName[480]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DIRECT_READS"); elementName[481]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DIRECT_WRITES"); elementName[482]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DIRECT_READ_REQS"); elementName[483]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DIRECT_WRITE_REQS"); elementName[484]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DIRECT_READ_TIME"); elementName[485]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DIRECT_WRITE_TIME"); elementName[486]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_INT_ROWS_INSERTED"); elementName[487]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOG_READS"); elementName[488]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOG_WRITES"); elementName[489]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_PKG_CACHE_LOOKUPS"); elementName[490]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_PKG_CACHE_INSERTS"); elementName[491]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CAT_CACHE_LOOKUPS"); elementName[492]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CAT_CACHE_INSERTS"); elementName[493]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CAT_CACHE_OVERFLOWS"); elementName[494]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CAT_CACHE_HEAP_FULL"); elementName[495]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CATALOG_NODE"); elementName[496]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TOTAL_SEC_CONS"); elementName[497]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DB_LOCATION"); elementName[498]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SERVER_PLATFORM"); elementName[499]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CATALOG_NODE_NAME"); elementName[500]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_PREFETCH_WAIT_TIME"); elementName[501]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_APPL_SECTION_LOOKUPS"); elementName[502]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_APPL_SECTION_INSERTS"); elementName[503]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TOTAL_HASH_JOINS"); elementName[504]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TOTAL_HASH_LOOPS"); elementName[505]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_HASH_JOIN_OVERFLOWS"); elementName[506]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_HASH_JOIN_SMALL_OVERFLOWS"); elementName[507]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_UOW_LOCK_WAIT_TIME"); elementName[508]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_STMT_TYPE"); elementName[509]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CURSOR_NAME"); elementName[510]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_UOW_LOG_SPACE_USED"); elementName[511]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_OPEN_REM_CURS"); elementName[512]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_OPEN_REM_CURS_BLK"); elementName[513]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_REJ_CURS_BLK"); elementName[514]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_ACC_CURS_BLK"); elementName[515]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_VERSION"); elementName[516]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_EVENT_MONITOR_NAME"); elementName[517]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SQL_REQS_SINCE_COMMIT"); elementName[518]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_BYTE_ORDER"); elementName[520]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_PREP_TIME_WORST"); elementName[521]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_ROWS_READ"); elementName[522]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_ROWS_WRITTEN"); elementName[523]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_OPEN_LOC_CURS"); elementName[524]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_OPEN_LOC_CURS_BLK"); elementName[525]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_COORD_NODE"); elementName[526]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_NUM_AGENTS"); elementName[527]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_ASSOCIATED_AGENTS_TOP"); elementName[528]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_APPL_PRIORITY"); elementName[529]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_APPL_PRIORITY_TYPE"); elementName[530]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DEGREE_PARALLELISM"); elementName[531]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_STMT_SORTS"); elementName[532]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_STMT_USR_CPU_TIME"); elementName[533]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_STMT_SYS_CPU_TIME"); elementName[534]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SS_NUMBER"); elementName[535]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SS_STATUS"); elementName[536]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SS_NODE_NUMBER"); elementName[537]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SS_EXEC_TIME"); elementName[538]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_PREP_TIME_BEST"); elementName[539]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_NUM_COMPILATIONS"); elementName[540]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TQ_NODE_WAITED_FOR"); elementName[541]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TQ_WAIT_FOR_ANY"); elementName[542]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TQ_ID_WAITING_ON"); elementName[543]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TQ_TOT_SEND_SPILLS"); elementName[544]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TQ_CUR_SEND_SPILLS"); elementName[545]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TQ_MAX_SEND_SPILLS"); elementName[546]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TQ_ROWS_READ"); elementName[547]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TQ_ROWS_WRITTEN"); elementName[548]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_AGENT_PID"); elementName[549]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOCK_ESCALATION"); elementName[550]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SUBSECTION_NUMBER"); elementName[551]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOCK_MODE"); elementName[552]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOCK_OBJECT_TYPE"); elementName[553]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_NUM_EXECUTIONS"); elementName[554]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLE_NAME"); elementName[555]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLE_SCHEMA"); elementName[556]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_NAME"); elementName[557]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_AGENT_ID_HOLDING_LK"); elementName[558]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_APPL_ID_HOLDING_LK"); elementName[559]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLE_FILE_ID"); elementName[561]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLE_TYPE"); elementName[562]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_OVERFLOW_ACCESSES"); elementName[563]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_PAGE_REORGS"); elementName[564]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SQLCABC"); elementName[565]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOCK_STATUS"); elementName[566]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOCK_OBJECT_NAME"); elementName[567]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_RF_TYPE"); elementName[568]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_RF_LOG_NUM"); elementName[569]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_RF_STATUS"); elementName[570]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TS_NAME"); elementName[571]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_BP_NAME"); elementName[572]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_STMT_NODE_NUMBER"); elementName[573]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_PARTIAL_RECORD"); elementName[574]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SYSTEM_CPU_TIME"); elementName[575]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SQLCA"); elementName[576]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SQLCODE"); elementName[577]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SQLERRML"); elementName[578]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SQLERRMC"); elementName[579]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SQLERRP"); elementName[580]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SQLERRD"); elementName[581]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SQLWARN"); elementName[582]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SQLSTATE"); elementName[583]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_UOW_STATUS"); elementName[584]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TOTAL_SYS_CPU_TIME"); elementName[585]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TOTAL_USR_CPU_TIME"); elementName[586]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOCK_MODE_REQUESTED"); elementName[587]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_INACTIVE_GW_AGENTS"); elementName[588]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_NUM_GW_CONN_SWITCHES"); elementName[589]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_GW_COMM_ERRORS"); elementName[590]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_GW_COMM_ERROR_TIME"); elementName[591]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_GW_CON_START_TIME"); elementName[592]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CON_RESPONSE_TIME"); elementName[593]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CON_ELAPSED_TIME"); elementName[594]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_HOST_RESPONSE_TIME"); elementName[595]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_PKG_CACHE_NUM_OVERFLOWS"); elementName[596]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_PKG_CACHE_SIZE_TOP"); elementName[597]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_APPL_ID_OLDEST_XACT"); elementName[598]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TOTAL_LOG_USED"); elementName[599]=c;	/* log space used by database        */
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TOTAL_LOG_AVAILABLE"); elementName[600]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_STMT_ELAPSED_TIME"); elementName[601]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_UOW_ELAPSED_TIME"); elementName[602]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SQLCAID"); elementName[603]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SMALLEST_LOG_AVAIL_NODE"); elementName[604]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DISCONNECTS"); elementName[605]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CREATE_NICKNAME"); elementName[606]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_PASSTHRUS"); elementName[607]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_STORED_PROCS"); elementName[608]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SP_ROWS_SELECTED"); elementName[609]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DATASOURCE_NAME"); elementName[610]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_REMOTE_LOCKS"); elementName[611]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_BLOCKING_CURSOR"); elementName[612]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_OUTBOUND_BLOCKING_CURSOR"); elementName[613]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_INSERT_SQL_STMTS"); elementName[614]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_UPDATE_SQL_STMTS"); elementName[615]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DELETE_SQL_STMTS"); elementName[616]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_UNREAD_PREFETCH_PAGES"); elementName[617]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_AGENT_STATUS"); elementName[618]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_NUM_TRANSMISSIONS"); elementName[619]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_OUTBOUND_BYTES_SENT_TOP"); elementName[620]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_OUTBOUND_BYTES_RECEIVED_TOP"); elementName[621]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_OUTBOUND_BYTES_SENT_BOTTOM"); elementName[622]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_OUTBOUND_BYTES_RECEIVED_BOTTOM"); elementName[623]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_DATA_SENT_128"); elementName[624]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_DATA_SENT_256"); elementName[625]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_DATA_SENT_512"); elementName[626]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_DATA_SENT_1024"); elementName[627]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_DATA_SENT_2048"); elementName[628]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_DATA_SENT_4096"); elementName[629]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_DATA_SENT_8192"); elementName[630]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_DATA_SENT_16384"); elementName[631]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_DATA_SENT_31999"); elementName[632]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_DATA_SENT_64000"); elementName[633]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_DATA_SENT_GT64000"); elementName[634]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_DATA_RECEIVED_128"); elementName[635]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_DATA_RECEIVED_256"); elementName[636]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_DATA_RECEIVED_512"); elementName[637]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_DATA_RECEIVED_1024"); elementName[638]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_DATA_RECEIVED_2048"); elementName[639]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_DATA_RECEIVED_4096"); elementName[640]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_DATA_RECEIVED_8192"); elementName[641]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_DATA_RECEIVED_16384"); elementName[642]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_DATA_RECEIVED_31999"); elementName[643]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_DATA_RECEIVED_64000"); elementName[644]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_DATA_RECEIVED_GT64000"); elementName[645]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_TIME_2_MS"); elementName[646]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_TIME_4_MS"); elementName[647]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_TIME_8_MS"); elementName[648]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_TIME_16_MS"); elementName[649]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_TIME_32_MS"); elementName[650]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_TIME_GT32_MS"); elementName[651]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_TIME_GT32_MS"); elementName[651]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DEADLOCK_ID"); elementName[652]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_DEADLOCK_NODE"); elementName[653]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_PARTICIPANT_NO"); elementName[654]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_PARTICIPANT_NO_HOLDING_LK"); elementName[655]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_ROLLED_BACK_PARTICIPANT_NO"); elementName[656]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SQLERRD1"); elementName[657]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SQLERRD2"); elementName[658]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SQLERRD3"); elementName[659]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SQLERRD4"); elementName[660]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SQLERRD5"); elementName[661]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SQLERRD6"); elementName[662]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_EVMON_ACTIVATES"); elementName[663]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_EVMON_FLUSHES"); elementName[664]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SQL_REQ_ID"); elementName[665]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MESSAGE"); elementName[666]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MESSAGE_TIME"); elementName[667]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_VECTORED_IOS"); elementName[668]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_PAGES_FROM_VECTORED_IOS"); elementName[669]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_BLOCK_IOS"); elementName[670]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_PAGES_FROM_BLOCK_IOS"); elementName[671]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_PHYSICAL_PAGE_MAPS"); elementName[672]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOCKS_IN_LIST"); elementName[673]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_REORG_PHASE"); elementName[674]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_REORG_MAX_PHASE"); elementName[675]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_REORG_CURRENT_COUNTER"); elementName[676]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_REORG_MAX_COUNTER"); elementName[677]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_REORG_TYPE"); elementName[678]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_REORG_STATUS"); elementName[679]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_REORG_COMPLETION"); elementName[680]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_REORG_START"); elementName[681]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_REORG_END"); elementName[682]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_REORG_PHASE_START"); elementName[683]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_REORG_INDEX_ID"); elementName[684]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_REORG_TBSPC_ID"); elementName[685]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_ID"); elementName[686]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_CUR_SIZE"); elementName[687]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_MAX_SIZE"); elementName[688]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_WATERMARK"); elementName[689]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_ID"); elementName[690]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_TYPE"); elementName[691]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_CONTENT_TYPE"); elementName[692]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_STATE"); elementName[693]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_PAGE_SIZE"); elementName[694]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_EXTENT_SIZE"); elementName[695]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_PREFETCH_SIZE"); elementName[696]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_CUR_POOL_ID"); elementName[697]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_NEXT_POOL_ID"); elementName[698]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_TOTAL_PAGES"); elementName[699]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_USABLE_PAGES"); elementName[700]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_USED_PAGES"); elementName[701]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_FREE_PAGES"); elementName[702]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_PAGE_TOP"); elementName[703]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_PENDING_FREE_PAGES"); elementName[704]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_REBALANCER_MODE"); elementName[705]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_REBALANCER_EXTENTS_REMAINING"); elementName[706]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_REBALANCER_EXTENTS_PROCESSED"); elementName[707]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_REBALANCER_LAST_EXTENT_MOVED"); elementName[708]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_REBALANCER_PRIORITY"); elementName[709]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_NUM_QUIESCERS"); elementName[710]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_STATE_CHANGE_OBJECT_ID"); elementName[711]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"Monitor Element 712"); elementName[712]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_NUM_CONTAINERS"); elementName[713]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_NUM_RANGES"); elementName[714]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_QUIESCER_STATE"); elementName[715]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_QUIESCER_AGENT_ID"); elementName[716]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_QUIESCER_TS_ID"); elementName[717]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_QUIESCER_OBJ_ID"); elementName[718]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_QUIESCER_AUTH_ID"); elementName[719]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CONTAINER_ID"); elementName[720]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CONTAINER_TYPE"); elementName[721]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CONTAINER_TOTAL_PAGES"); elementName[722]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CONTAINER_USABLE_PAGES"); elementName[723]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CONTAINER_STRIPE_SET"); elementName[724]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CONTAINER_ACCESSIBLE"); elementName[725]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CONTAINER_NAME"); elementName[726]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_RANGE_STRIPE_SET_NUMBER"); elementName[727]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_RANGE_NUMBER"); elementName[728]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_RANGE_OFFSET"); elementName[729]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_RANGE_MAX_PAGE_NUMBER"); elementName[730]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_RANGE_MAX_EXTENT"); elementName[731]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_RANGE_START_STRIPE"); elementName[732]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_RANGE_END_STRIPE"); elementName[733]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_RANGE_ADJUSTMENT"); elementName[734]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_RANGE_NUM_CONTAINERS"); elementName[735]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_RANGE_CONTAINER_ID"); elementName[736]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CONSISTENCY_TOKEN"); elementName[737]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_PACKAGE_VERSION_ID"); elementName[738]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOCK_NAME"); elementName[739]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOCK_COUNT"); elementName[740]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOCK_HOLD_COUNT"); elementName[741]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOCK_ATTRIBUTES"); elementName[742]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOCK_RELEASE_FLAGS"); elementName[743]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOCK_CURRENT_MODE"); elementName[744]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_TABLESPACE_FS_CACHING"); elementName[745]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_BP_TBSP_USE_COUNT"); elementName[751]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_BP_PAGES_LEFT_TO_REMOVE"); elementName[752]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_BP_CUR_BUFFSZ"); elementName[753]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_BP_NEW_BUFFSZ"); elementName[754]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SORT_HEAP_TOP"); elementName[755]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SORT_SHRHEAP_ALLOCATED"); elementName[756]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SORT_SHRHEAP_TOP"); elementName[757]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SHR_WORKSPACE_SIZE_TOP"); elementName[758]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SHR_WORKSPACE_NUM_OVERFLOWS"); elementName[759]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SHR_WORKSPACE_SECTION_LOOKUPS"); elementName[760]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SHR_WORKSPACE_SECTION_INSERTS"); elementName[761]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_PRIV_WORKSPACE_SIZE_TOP"); elementName[762]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_PRIV_WORKSPACE_NUM_OVERFLOWS"); elementName[763]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_PRIV_WORKSPACE_SECTION_LOOKUPS"); elementName[764]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_PRIV_WORKSPACE_SECTION_INSERTS"); elementName[765]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_CAT_CACHE_SIZE_TOP"); elementName[766]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_PARTITION_NUMBER"); elementName[767]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_NUM_TRANSMISSIONS_GROUP"); elementName[768]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_NUM_INDOUBT_TRANS"); elementName[769]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_UTILITY_DBNAME"); elementName[770]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_UTILITY_ID"); elementName[771]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_UTILITY_TYPE"); elementName[772]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_UTILITY_PRIORITY"); elementName[773]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_UTILITY_START_TIME"); elementName[774]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_UTILITY_DESCRIPTION"); elementName[775]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_ASYNC_INDEX_READ_REQS"); elementName[776]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SESSION_AUTH_ID"); elementName[777]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_SQL_CHAINS"); elementName[778]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_TEMP_DATA_L_READS"); elementName[779]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_TEMP_DATA_P_READS"); elementName[780]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_TEMP_INDEX_L_READS"); elementName[781]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_TEMP_INDEX_P_READS"); elementName[782]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_TIME_1_MS"); elementName[783]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_TIME_100_MS"); elementName[784]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_TIME_500_MS"); elementName[785]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_MAX_TIME_GT500_MS"); elementName[786]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOG_TO_REDO_FOR_RECOVERY"); elementName[787]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_POOL_NO_VICTIM_BUFFER"); elementName[788]=c;
  c=new char[50]; memset(c,'\0',50); strcpy(c,"SQLM_ELM_LOG_HELD_BY_DIRTY_PAGES"); elementName[789]=c;
} // Snapshot constructor

sqlm_header_info * Snapshot::ParseMonitorStream(CStringList &paramList,char * prefix,
                                                char * pStart,
                                                char * pEnd,char* szReturn)
{
  char buf[256];

  sqlm_header_info * pHeader = (sqlm_header_info *)pStart;
  char * pData;
  char * pElementName;
  char   elementNameBuffer[24];

  CRITICAL_SECTION m_Lock;

  InitializeCriticalSection(&m_Lock);

  EnterCriticalSection(&m_Lock);
  if (!pEnd)
    pEnd = pStart +                  
           pHeader->size +           
           sizeof(sqlm_header_info); 

  
 
  while ((char*)pHeader < pEnd)
  {
   
    pData = (char*)pHeader + sizeof(sqlm_header_info);
	
	if(pHeader->element == 211)
		pElementName = elementNameBuffer;

    if (pHeader->element >= NUMELEMENTS  ||
        (!(pElementName = elementName[pHeader->element])))
    {
    
      sprintf(elementNameBuffer,"Element Number %d",pHeader->element);
      pElementName = elementNameBuffer;
    }
 
	CString m_str;
	BOOL flag1=FALSE;
	  
	if(!FindStrValue(szReturn, pElementName))
	{
		flag1=TRUE;
	} 
	//in condition scope get db2 parameter
	BOOL flag2=FALSE;  
	POSITION pos=paramList.GetHeadPosition();
	while(pos)
	{
		m_str=paramList.GetNext(pos);
		if(!strcmp(m_str,pElementName))
		{
			flag2=TRUE;
			pos = NULL;
		}
	}

    if (pHeader->type == SQLM_TYPE_HEADER)
    {
      char newPrefix[80];
      char *pNewEnd;
	  POSITION prevpos;

	  CString m_str;
	  
      strncpy(newPrefix, BLANKS, strlen(prefix)+2);
      newPrefix[strlen(prefix)+2] = '\0';
      pNewEnd = pData + (pHeader->size);

	  //CStringList m_CList;
	  if(flag1&&flag2)
	  {
		switch(pHeader->element)
		{
		case SQLM_ELM_DB_CONN_TIME:
		case SQLM_ELM_DB2START_TIME:
		case SQLM_ELM_LAST_BACKUP:
				pHeader = ParseMonitorStream_SubItem(pElementName, "SQLM_ELM_SECONDS", newPrefix, pData, pNewEnd,szReturn);
				break;
		default:
				pHeader = ParseMonitorStream(paramList,newPrefix, pData, pNewEnd,szReturn);
				break;
		}
	  }
	  else
		pHeader = ParseMonitorStream(paramList,newPrefix, pData, pNewEnd,szReturn);
	  flag=FALSE;
    }
    else
    {
	  //get condition db2 parameter end
	  CString szReturn1;

	  if(flag1&&flag2)
	  {
		unsigned int unInt;
		signed int nInt;
		unsigned __int64 unLong;
		signed __int64 nLong;
		char str1[2];
		int i, j;

		szReturn1+=pElementName;
	    szReturn1+="=";
		switch(pHeader->type)
		{
			case SQLM_TYPE_U8BIT:
			case SQLM_TYPE_U16BIT:
			case SQLM_TYPE_U32BIT:
				unInt = *(unsigned int*)pData;
				itoa(unInt,buf,10);
				szReturn1+=buf;
				break;
			case SQLM_TYPE_8BIT:
			case SQLM_TYPE_16BIT:
			case SQLM_TYPE_32BIT:
				nInt = *(signed int*)pData;
				itoa(nInt,buf,10);
				szReturn1+=buf;
				break;
			case SQLM_TYPE_U64BIT:
				unLong = *(unsigned __int64*)pData;
				_ui64toa(unLong,buf,10);
				szReturn1+=buf;
				break;
			case SQLM_TYPE_64BIT:
				nLong = *(signed __int64*)pData;
				_i64toa(nLong,buf,10);
				szReturn1+=buf;
				break;
			case SQLM_TYPE_STRING:
				szReturn1+=pData;
				break;
			default:
				szReturn1+="0x";
				for (i = 0; i<pHeader->size; i++)
				{
			 		j = (char)*(pData + i);
					sprintf(str1,"%.2x",j);
					szReturn1+=str1;
				}
				break;
		}
		if(ti == 0)
		{
			//strcpy(szReturn , szReturn1.GetBuffer());
			memcpy(szReturn, szReturn1.GetBuffer(szReturn1.GetLength()), szReturn1.GetLength());
			tlen = szReturn1.GetLength() + 1;
		}
		else
		{
			memcpy(szReturn + tlen , szReturn1.GetBuffer(), szReturn1.GetLength());
			tlen += (szReturn1.GetLength() + 1);
		}
		ti++;
	  }

	  pHeader = (sqlm_header_info *)(pData + pHeader->size);
    }
  }
  LeaveCriticalSection(&m_Lock);
  return (pHeader);
}

sqlm_header_info * Snapshot::ParseMonitorStream_SubItem(CString strItem, CString strSubItem, char * prefix,
                                                char * pStart,
                                                char * pEnd,char* szReturn)
{
  char buf[256];

  sqlm_header_info * pHeader = (sqlm_header_info *)pStart;
  char * pData;
  char * pElementName;
  char   elementNameBuffer[24];

  CRITICAL_SECTION m_Lock;

  InitializeCriticalSection(&m_Lock);

  EnterCriticalSection(&m_Lock);
  if (!pEnd)
    pEnd = pStart +                  
           pHeader->size +           
           sizeof(sqlm_header_info); 

  
 
  while ((char*)pHeader < pEnd)
  {
   
    pData = (char*)pHeader + sizeof(sqlm_header_info);
	
	if(pHeader->element == 432)
		pElementName = elementNameBuffer;

    if (pHeader->element >= NUMELEMENTS  ||
        (!(pElementName = elementName[pHeader->element])))
    {
    
      sprintf(elementNameBuffer,"Element Number %d",pHeader->element);
      pElementName = elementNameBuffer;
    }
 
	BOOL flag1=FALSE;
	  
	if(!FindStrValue(szReturn, pElementName))
	{
		flag1=TRUE;
	} 
	//in condition scope get db2 parameter
	BOOL flag2=FALSE;  
	
	if(!strcmp(strSubItem,pElementName) || strSubItem == "")
		flag2=TRUE;

    if (pHeader->type == SQLM_TYPE_HEADER)
    {
      char newPrefix[80];
      char *pNewEnd;
	  POSITION prevpos;

	  CString m_str;
	  
      strncpy(newPrefix, BLANKS, strlen(prefix)+2);
      newPrefix[strlen(prefix)+2] = '\0';
      pNewEnd = pData + (pHeader->size);

	  pHeader = ParseMonitorStream_SubItem(strItem, strSubItem, newPrefix, pData, pNewEnd,szReturn);
	  
	  flag=FALSE;
    }
    else
    {
	  //get condition db2 parameter end
	  CString szReturn1;

	  if(flag1&&flag2)
	  {
		unsigned int unInt;
		signed int nInt;
		unsigned __int64 unLong;
		signed __int64 nLong;
		char str1[2];
		int i, j;

		if(strSubItem == "")
			szReturn1+=pElementName;
		else
			szReturn1+=strItem;
	    szReturn1+="=";
		switch(pHeader->type)
		{
			case SQLM_TYPE_U8BIT:
			case SQLM_TYPE_U16BIT:
			case SQLM_TYPE_U32BIT:
				unInt = *(unsigned int*)pData;
				itoa(unInt,buf,10);
				szReturn1+=buf;
				break;
			case SQLM_TYPE_8BIT:
			case SQLM_TYPE_16BIT:
			case SQLM_TYPE_32BIT:
				nInt = *(signed int*)pData;
				itoa(nInt,buf,10);
				szReturn1+=buf;
				break;
			case SQLM_TYPE_U64BIT:
				unLong = *(unsigned __int64*)pData;
				_ui64toa(unLong,buf,10);
				szReturn1+=buf;
				break;
			case SQLM_TYPE_64BIT:
				nLong = *(signed __int64*)pData;
				_i64toa(nLong,buf,10);
				szReturn1+=buf;
				break;
			case SQLM_TYPE_STRING:
				szReturn1+=pData;
				break;
			default:
				szReturn1+="0x";
				for (i = 0; i<pHeader->size; i++)
				{
			 		j = (char)*(pData + i);
					sprintf(str1,"%.2x",j);
					szReturn1+=str1;
				}
				break;
		}
		if(ti == 0)
		{
			//strcpy(szReturn , szReturn1.GetBuffer());
			memcpy(szReturn, szReturn1.GetBuffer(szReturn1.GetLength()), szReturn1.GetLength());
			tlen = szReturn1.GetLength() + 1;
		}
		else
		{
			memcpy(szReturn + tlen , szReturn1.GetBuffer(), szReturn1.GetLength());
			tlen += (szReturn1.GetLength() + 1);
		}
		ti++;
	  }

	  pHeader = (sqlm_header_info *)(pData + pHeader->size);
    }
  }
  LeaveCriticalSection(&m_Lock);
  return (pHeader);
}

/***************************************************************************/
/* UpdateUtilImpactLim                                                     */
/* Throttling is done to regulate the performance impact of online         */
/* utilities on the production workload.                                   */
/* Update UTIL_IMPACT_LIM DBM config parameter to set the percentage of    */
/* throttling intended for the utilities of an instance                    */
/***************************************************************************/
int Snapshot::UpdateUtilImpactLim(void)
{
  int rc = 0;
  struct sqlca sqlca;
  db2CfgParam cfgParameters[1];
  db2Cfg cfgStruct;
  unsigned utillim = 0;

  cfgParameters[0].flags = 0;
  cfgParameters[0].token = SQLF_KTN_UTIL_IMPACT_LIM;
  cfgParameters[0].ptrvalue = (char *)&utillim;

  utillim = 60;
  cfgStruct.numItems = 1;
  cfgStruct.paramArray = cfgParameters;
  cfgStruct.flags = db2CfgDatabaseManager | db2CfgImmediate;
  cfgStruct.dbname = NULL;

  db2CfgSet(db2Version810, (void *)&cfgStruct, &sqlca);

  return rc;
} // Snapshot::UpdateUtilImpactLim
