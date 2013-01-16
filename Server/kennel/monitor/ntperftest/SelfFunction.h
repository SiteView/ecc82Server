#include "Winnetwk.h"
#include "Winperf.h"
//#include "AppCheck.h"
#define KEYCN_PERFLIB_PATH	"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Perflib\\004"
#define KEYEN_PERFLIB_PATH	"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Perflib\\009"
#define VALUE_NAME			"Counter"

#define ENCRYPT_NUM			24
#define MAX_BUF_SIZE		512
#define TOTAL_BYTE			8192
#define BYTE_INCREMENT		1024
#define SLEEP_TIME			500
#define MAX_RETRY_NUM       1
#define LOGIN_SLEEP			1500


//-------------------Functioin declare-----------------------------------
int GetRightInstance(const char* pszInstanceInfo,PPERF_OBJECT_TYPE potObject);
int GetIndexNew(char* pszInfoBuf,char* pszCounterTitle,int iCounterIndex,int iBufSize);
int DetectFreeDiskSymbol();
void GetAvaliableDisk(int iDiskIndex);
void GetIndex(const char* pszCounterOrObjectName,char* pszTitleIndexBuf,int iBufSize,int &iID);
BOOL AnalysizeRawData(PPERF_DATA_BLOCK pdbData,int iObjIndex,const char* pszInstanceInfo,int iCounterIndex,LPVOID &pData);
HKEY ConnectRegistry(const char* pszComputerName,int iMachineType,int iKeyType);
BOOL CollectObjectCounterInfor(HKEY hKey,char** pszInfoBuffer,DWORD &dwByte);
BOOL CollectRawData(HKEY hKey,PPERF_DATA_BLOCK &pDataBlock,char* szObjIndex);
					//,int iCounterIndex,LPVOID pData);
BOOL DisConnectRegistry(HKEY hKey);
BOOL LoginRemoteMachine(char* pszMachineName,char* pszUserAccount,char* pszPassWord);
BOOL CancelConnectLocalDevice(char* pszLocalDevice,DWORD dwFlag,BOOL bForce);
BOOL CheckCheck();

HKEY GetRegistryLocalKey(const char *pszMachineName);

HKEY GetRegistryPerfKey(const char *pszMachineName);

//extern "C" __declspec(dllimport) BOOL AimCheckUserRight(int iType,int &iMonitorNum);
//------------------------------------------------------------------------

/*****************************************************************************
 *Function name:		GetIndex(const char* pszCounterOrObjectName,char* pszTitleIndexBuf,int iBufSize,int &iID)
 *Return   value:		BOOL
 *Aim explain:			Get the index of object or counter which is stored in registry
 *Parameter explain:	pszCounterOrObjectName :[in]object or counter title
 *					    pszTitleIndexBuf:[in] The buf which include title and index of all object and counter
 *						iBufSize:[in]The size of a buffer which pszCounterOrObjcetName point to.
 *                      iID:[out] index of objcet or counter                                   
 *
******************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////
