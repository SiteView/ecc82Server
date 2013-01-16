
#include "regperf.h"
#include "selffunction.h"
#include <math.h>

#define PROCESSOR_OBJECT_NAME		"Processor"
#define PROCESSOR_COUNTER_NAME	"% Processor Time"

#define MEMORY_OBJECT_NAME   "Memory"
#define WEB_OBJECT_NAME   "Web Service"

#define CPU_LOOP_NUM			2
#define MEMORY_LOOP_NUM			2
#define NETWORK_LOOP_NUM      2
#define IIS_LOOP_NUM      2
#define WEB_LOOP_NUM      2

#define BYTE_TO_MB    1048576



#define TOTALBYTES    8192
#define BYTEINCREMENT 1024


#define DISK_OBJECT_NAME		"LogicalDisk"
#define NETWORK_OBJECT_NAME		"Network Interface"

#define	IIS_OBJECT_NAME   "Web Service"
//#define OBJECT_NAME_NI   "Network Interface"
#define TCP_OBJECT_NAME  "TCP"
#define COUNTER_NAME_TCP "Connections Established"


/*#define ENCRYPT_NUM			24
#define MAX_BUF_SIZE		512
#define TOTAL_BYTE			8192
#define BYTE_INCREMENT		1024
#define SLEEP_TIME			500
#define MAX_RETRY_NUM       10
#define LOGIN_SLEEP			1500
*/


int GetCPURate(char *pszMachineName,char* szRet ,int nVersion)
{
	HKEY hMainKey = NULL;
	hMainKey=GetRegistryLocalKey(pszMachineName);
	if(hMainKey == NULL)
	{
		printf("ERROR=CONNECT REGISTRY FAILED\r\n");
		return -3;
	}
	char* pszInfoBuf = NULL;
	DWORD dwByte = 0;
	if(!CollectObjectCounterInfor(hMainKey,&pszInfoBuf,dwByte))
	{
		if(pszInfoBuf)
		{
			HeapFree(GetProcessHeap(), 0, (LPVOID)pszInfoBuf);
			pszInfoBuf = NULL;
		}
		DisConnectRegistry(hMainKey);
		hMainKey = NULL;
		return -4;
	}
	int iByte = (int)dwByte;
	//-----------------------------------------
	int iObjectIndex = 0;
	GetIndex(PROCESSOR_OBJECT_NAME,pszInfoBuf,iByte,iObjectIndex);
	char szObj[10] = {0};
	itoa(iObjectIndex,szObj,10);


#ifdef DEBUG
	//printf("%s id=%d\r\n",OBJECT_NAME,iObjectIndex);
#endif


	int iCounterIndex = 0;
	GetIndex(PROCESSOR_COUNTER_NAME , pszInfoBuf, iByte,iCounterIndex);

#ifdef DEBUG
//	printf("%s id=%d\r\n",COUNTER_NAME,iCounterIndex);
#endif
	//free object&counter information buffer
	if(pszInfoBuf)
	{
		HeapFree(GetProcessHeap(), 0, (LPVOID)pszInfoBuf);
		//HeapFree(GetProcessHeap(), 0, (LPVOID)pszInfoBuf);
		pszInfoBuf = NULL;
	}

	DisConnectRegistry(hMainKey);
	hMainKey = NULL;
	//--------------Get objcet index and counter index ok-----------------
	hMainKey = GetRegistryPerfKey(pszMachineName);
	if(!hMainKey)
		return -5;


	double dData = 0;
	LONGLONG llArrayX[2] = {0},
			 llArrayY[2] = {0};

	BOOL bObjFlag = TRUE;
	PPERF_DATA_BLOCK pdbDataBlock = NULL;
	int i = 0;
	for(i = 0;i < CPU_LOOP_NUM;i++)
	{
		if(!CollectRawData(hMainKey,pdbDataBlock,szObj))
		{
			if(pdbDataBlock)
			{
				free(pdbDataBlock);
				pdbDataBlock = NULL;
			}
			DisConnectRegistry(hMainKey);
			return -6;
		}
		llArrayY[i] = pdbDataBlock->PerfTime100nSec.QuadPart;
		LPVOID p = NULL;
		CString strValueData=_T("");
		if (nVersion>4)
			strValueData.Format("_total");
		else
			strValueData.Format("0");
		//if(!AnalysizeRawData(pdbDataBlock,iObjectIndex,"_total",iCounterIndex,p))
		if(!AnalysizeRawData(pdbDataBlock,iObjectIndex,strValueData,iCounterIndex,p))
		
		{
			bObjFlag = FALSE;
			break;
		}
		llArrayX[i] = *(LONGLONG *)p;
		if(pdbDataBlock)
		{
			free(pdbDataBlock);
			pdbDataBlock = NULL;
		}
		Sleep(SLEEP_TIME);
	}
	DisConnectRegistry(hMainKey);
	hMainKey = NULL;

	if(!bObjFlag)
	{
		if(pdbDataBlock)
		{
			free(pdbDataBlock);
			pdbDataBlock = NULL;
		}
		return -7;
	}

	double dValue = 100*(1-1.0*(llArrayX[1]-llArrayX[0])/(llArrayY[1] - llArrayY[0]));
	sprintf(szRet,"utilization=%.2f$",fabs(dValue));
	return 0;
}

/*------------------------------------------------------------------------*/






//////////////////////////////////////////////////////////////////////
////get  memory info


int GetMemoryInfo1(char *pszMachineName,char* szRet)
{
	HKEY hKey = NULL;
	hKey=GetRegistryLocalKey(pszMachineName);
	if(hKey == NULL)
	{
		printf("ERROR=CONNECT REGISTRY FAILED\r\n");
		return -3;
	}

	char* pszInfoBuf = NULL;
	DWORD dwByte = 0;

	//////////////////////////////////////////////////////
	if(!CollectObjectCounterInfor(hKey,&pszInfoBuf,dwByte))
	{
		if(pszInfoBuf)
		{
			HeapFree(GetProcessHeap(), 0, (LPVOID)pszInfoBuf);
			pszInfoBuf = NULL;
		}
		DisConnectRegistry(hKey);
		hKey = NULL;
		//strRet.Format("error=收集计数器原始数据失败$");
		//sprintf("");
		//return strRet;
		return -4;
	}
//Get object index
	int iByte = (int)dwByte,
	iObjectIndex = 0;

	//////////	//以上代码通用
	/////////////////////////////////////////////////////////////////

	GetIndex(MEMORY_OBJECT_NAME,pszInfoBuf,iByte,iObjectIndex);



	char szObj[10] = {0};
	itoa(iObjectIndex,szObj,10);

	//Get counter index
	char szCounter[4][64];
	strcpy(szCounter[0],"% Committed Bytes In Use");
	strcpy(szCounter[1],"Commit Limit");
	strcpy(szCounter[2],"Committed Bytes");
	strcpy(szCounter[3],"Pages/sec");

	int iCounterIndex[4] = {0};
	for(int i = 0;i < 4;i++)
		GetIndex(szCounter[i],pszInfoBuf,iByte,iCounterIndex[i]);
	
	//free object&counter information buffer
	
	if(pszInfoBuf)
	{
		HeapFree(GetProcessHeap(), 0, (LPVOID)pszInfoBuf);
		pszInfoBuf = NULL;
	}
	
	//Disconnect to HKEY_LOCAL_MACHINE
	DisConnectRegistry(hKey);
	hKey = NULL;
	//----Get object and counter title index ok -----------------


	//Prepare performance data
	//hKey = ConnectRegistry(strMachineName,iMachineType,1);
	hKey = GetRegistryPerfKey(pszMachineName);
	if(!hKey)
	{
		//strRet.Format("error=打开注册表失败$");
		//return strRet;
		return -5;
	}

	BOOL bObjFlag = TRUE;
	PPERF_DATA_BLOCK pdbDataBlock = NULL;
	DWORD dwArrayX[2] = {0};
	LONGLONG llArrayY[2] = {0},
		     llFreq = 0;

	double dValue[4] = {0.0};
	for(int j = 0;j < MEMORY_LOOP_NUM;j++)
	{
		if(!CollectRawData(hKey,pdbDataBlock,szObj))
		{
			if(pdbDataBlock)
			{
				free(pdbDataBlock);
				pdbDataBlock = NULL;
			}
			DisConnectRegistry(hKey);
			hKey = NULL;
			//strRet.Format("error=采集计数器信息失败$");
			return -6;
		}
		llArrayY[j] = pdbDataBlock->PerfTime.QuadPart;
		llFreq = pdbDataBlock->PerfFreq.QuadPart;

		if(j == 0)
		{
			int k = 0;
			for(k = 0;k < 3;k++)
			{
				LPVOID p = NULL;
				if(!AnalysizeRawData(pdbDataBlock,iObjectIndex,"",iCounterIndex[k],p))
				{
					bObjFlag = FALSE;
					break;
				}
				if(k == 0)
				{
				
					DWORD x1 = 0,
						  x2 = 0;
					x1 = *(DWORD *)p;
					LPVOID p1 = NULL;
					p1 = (LPVOID)((PBYTE)p+4);
					x2 = *(DWORD *)p1;
					dValue[k] = 100*(1.0*x1/(float)x2);
				}
				else
					dValue[k] = *(DWORD *)p;
			}
			if(!bObjFlag)
				break;
		}
		LPVOID pPageSec = NULL;
		if(!AnalysizeRawData(pdbDataBlock,iObjectIndex,"",iCounterIndex[3],pPageSec))
		{
			bObjFlag = FALSE;
			break;
		}
		dwArrayX[j] =*(DWORD *)pPageSec;

		if(pdbDataBlock)
		{
			free(pdbDataBlock);
			pdbDataBlock = NULL;
		}
		Sleep(SLEEP_TIME);
	}
	DisConnectRegistry(hKey);
	hKey = NULL;
	if(!bObjFlag)
	{
		if(pdbDataBlock)
		{
			free(pdbDataBlock);
			pdbDataBlock = NULL;
		}
		//strRet.Format("error=计数器不存在$");
		//return strRet;
		return -7;
	}
	//Analysize raw data
    LONGLONG y = llArrayY[1] - llArrayY[0];
	if(y != 0)
		dValue[3] = (dwArrayX[1] - dwArrayX[0])/((1.0*y)/llFreq);	
	//----Collect and analysize raw data ok--------------------------------------
	double dFreeMB = (dValue[1] - dValue[2]) / BYTE_TO_MB;
	double d1 = 0.0,
		  d2 = 0.0;
	d1 = dValue[1] / BYTE_TO_MB;
	d2 = dValue[2] / BYTE_TO_MB;

	sprintf(szRet,"percentUsed=%.2f$Mbfree=%.2f$pagesPerSec=%.2f$"
		,dValue[0],dFreeMB,dValue[3]);
//	return strRet;
//	sprintf(szRet,"percentUsed=%.2f$Mbfree=%.2f$pagesPerSec=%.2f$"
//		,d1,d2,dValue[3]);
	return 0;



}

/*------------------------------------------------------------------------*/


int GetDiskInfo1(char *pszMachineName,char* szRet,CString strDisk)
{
	HKEY hKey = NULL;
	hKey=GetRegistryLocalKey(pszMachineName);
	if(hKey == NULL)
	{
		printf("ERROR=CONNECT REGISTRY FAILED\r\n");
		return -3;
	}

	char* pszInfoBuf = NULL;
	DWORD dwByte = 0;

	//////////////////////////////////////////////////////
	if(!CollectObjectCounterInfor(hKey,&pszInfoBuf,dwByte))
	{
		if(pszInfoBuf)
		{
			HeapFree(GetProcessHeap(), 0, (LPVOID)pszInfoBuf);
			pszInfoBuf = NULL;
		}
		DisConnectRegistry(hKey);
		hKey = NULL;
		return -4;
	}
	//Get object index
	int iByte = (int)dwByte,
	iObjectIndex = 0;

	GetIndex(DISK_OBJECT_NAME,pszInfoBuf,iByte,iObjectIndex);
	char szObj[10] = {0};
	itoa(iObjectIndex,szObj,10);	

	//Get counter index
	char szCounter[2][64];
	strcpy(szCounter[0],"% Free Space");
	strcpy(szCounter[1],"Free Megabytes");
	
	int iCounterIndex[2] = {0};
	for(int i = 0;i < 2;i++)
		GetIndex(szCounter[i],pszInfoBuf,iByte,iCounterIndex[i]);

	//free object&counter information buffer
	if(pszInfoBuf)
	{
		HeapFree(GetProcessHeap(), 0, (LPVOID)pszInfoBuf);
		pszInfoBuf = NULL;
	}
	//Disconnect to HKEY_LOCAL_MACHINE
	DisConnectRegistry(hKey);
	hKey = NULL;

	//Get the handle of HKEY_PERFORMANCE_DATA
	hKey = GetRegistryPerfKey(pszMachineName);
	if(!hKey)
		return -5;


	BOOL bObjFlag = TRUE;
	PPERF_DATA_BLOCK pdbDataBlock = NULL;
	DWORD dwArrayX[3] = {0};
	double dValue[2] = {0.0};

	if(!CollectRawData(hKey,pdbDataBlock,szObj))
	{
		if(pdbDataBlock)
		{
			free(pdbDataBlock);
			pdbDataBlock = NULL;
		}
		DisConnectRegistry(hKey);
		hKey = NULL;
		return -6;
	}

	LPCTSTR lpHardDisk = NULL;
	lpHardDisk = strDisk.GetBuffer(strDisk.GetLength());
	for(int j = 0;j < 2;j++)
	{
		LPVOID p = NULL;
		if(!AnalysizeRawData(pdbDataBlock,iObjectIndex,lpHardDisk,iCounterIndex[j],p))
		{
			bObjFlag = FALSE;
			break;
		}
		if(!bObjFlag)
			break;

		if(j == 0)
		{
			DWORD x1 = 0,
				  x2 = 0;
			x1 = *(DWORD *)p;
			LPVOID p1 = NULL;
			p1 = (LPVOID)((PBYTE)p+4);
			x2 = *(DWORD *)p1;
			dValue[0] = 100*(1.0*x1/(float)x2);
		}
		if(j == 1)
		{
			DWORD x3 = 0;
			x3 = *(DWORD *)p;
			dValue[1] = (float)x3;
		}
	}
	if(!bObjFlag)
	{
		if(pdbDataBlock)
		{
			free(pdbDataBlock);
			pdbDataBlock = NULL;
		}
		return -7;
	}
	if(pdbDataBlock)
	{
		free(pdbDataBlock);
		pdbDataBlock = NULL;
	}
	
	sprintf(szRet,"percentFull=%.2f$Mbfree=%.2f$",100 - dValue[0],dValue[1]);

	return 0;
}

///////////////////////////////////////////////////////////////////////////
//////////////

int EnumDisks(char *pszMachineName,CStringList& lstText, CStringList& lstValue)
{
	HKEY hKey = NULL;
	hKey=GetRegistryLocalKey(pszMachineName);
	if(hKey == NULL)
	{
		printf("ERROR=CONNECT REGISTRY FAILED\r\n");
		return -3;
	}

	char* pszInfoBuf = NULL;
	DWORD dwByte = 0;

	//////////////////////////////////////////////////////
	if(!CollectObjectCounterInfor(hKey,&pszInfoBuf,dwByte))
	{
		if(pszInfoBuf)
		{
			HeapFree(GetProcessHeap(), 0, (LPVOID)pszInfoBuf);
			pszInfoBuf = NULL;
		}
		DisConnectRegistry(hKey);
		hKey = NULL;
		return -4;
	}

//Get object index
	int iByte = (int)dwByte,
	iObjectIndex = 0;


	GetIndex(DISK_OBJECT_NAME,pszInfoBuf,iByte,iObjectIndex);
	//Disconnect to HKEY_LOCAL_MACHINE
	DisConnectRegistry(hKey);
	hKey = NULL;

	if(pszInfoBuf)
	{
		HeapFree(GetProcessHeap(), 0, (LPVOID)pszInfoBuf);
		pszInfoBuf = NULL;
	}


	//Prepare performance data
	hKey = GetRegistryPerfKey(pszMachineName);
	if(!hKey)	return -5;
	GetPerfomanceInstance(hKey,iObjectIndex,lstText, lstValue);
	//GetPerfomanceInstance(hKey,238,szRet);
	

	DisConnectRegistry(hKey);
	hKey = NULL;
	return 0;

}



int GetPerfomanceInstance(HKEY hPERFKEY, int nObjIndex,CStringList& lstText, CStringList& lstValue)
{
	PPERF_DATA_BLOCK PerfData = NULL;
	PPERF_OBJECT_TYPE PerfObj;
	PPERF_INSTANCE_DEFINITION PerfInst;
	PPERF_COUNTER_DEFINITION PerfCntr, CurCntr;
	PPERF_COUNTER_BLOCK PtrToCntr;
	DWORD BufferSize = TOTALBYTES;
	DWORD i, k;
	CString strObjIndex=_T("");
	strObjIndex.Format("%d",nObjIndex);
	// Allocate the buffer.
	PerfData = (PPERF_DATA_BLOCK) malloc( BufferSize );
	while( RegQueryValueEx( hPERFKEY,
							   strObjIndex,
							   NULL,
							   NULL,
							   (LPBYTE) PerfData,
							   &BufferSize ) == ERROR_MORE_DATA )
	   {
	   // Get a buffer that is big enough.
		  BufferSize += BYTEINCREMENT;
		  PerfData = (PPERF_DATA_BLOCK) realloc( PerfData, BufferSize );
	   }

	// Get the first object type.
	   PerfObj = (PPERF_OBJECT_TYPE)((PBYTE)PerfData +
		  PerfData->HeaderLength);
	   CString strPrintContent=_T("");


	// Process all objects.
	   for( i=0; i < PerfData->NumObjectTypes; i++ )
	   {

	   // Get the counter block.
		  PerfCntr = (PPERF_COUNTER_DEFINITION) ((PBYTE)PerfObj +
					 PerfObj->HeaderLength);

		  if( PerfObj->NumInstances > 0 )
		  {
		  // Get the first instance.
			 PerfInst = (PPERF_INSTANCE_DEFINITION)((PBYTE)PerfObj +
						PerfObj->DefinitionLength);

		  // Retrieve all instances.
			 for( k=0; k < PerfObj->NumInstances; k++ )
			 {
				//strPrintContent.Format("\nInstance: %S<br>\n", (char *)((PBYTE)PerfInst +
				//		PerfInst->NameOffset) );


				 //set instance name
				 
				 strPrintContent.Format("%S",(char *)((PBYTE)PerfInst +
						PerfInst->NameOffset) );
				 if (strPrintContent.CompareNoCase("_total")==0)
					 return 0;
					 //break;
				 lstValue.AddTail(strPrintContent);
				 lstText.AddTail(strPrintContent);
				 //strcat(szRet,strPrintContent.GetBuffer(strPrintContent.GetLength()));

				CurCntr = PerfCntr;

			 // Get the first counter.
				PtrToCntr = (PPERF_COUNTER_BLOCK)((PBYTE)PerfInst +
							PerfInst->ByteLength);

			 // Get the next instance.
				PerfInst = (PPERF_INSTANCE_DEFINITION)((PBYTE)PtrToCntr +
						   PtrToCntr->ByteLength);
			 }

		  }


		  // Get the next object type.
		  PerfObj = (PPERF_OBJECT_TYPE)((PBYTE)PerfObj +
					PerfObj->TotalByteLength);

	   }
	   
	
	return 0;
}

int EnumAllNetWork1(char *pszMachineName,CStringList& lstText, CStringList& lstValue)
{
	HKEY hKey = NULL;
	hKey=GetRegistryLocalKey(pszMachineName);
	if(hKey == NULL)
	{
		printf("ERROR=CONNECT REGISTRY FAILED\r\n");
		return -3;
	}

	char* pszInfoBuf = NULL;
	DWORD dwByte = 0;

	//////////////////////////////////////////////////////
	if(!CollectObjectCounterInfor(hKey,&pszInfoBuf,dwByte))
	{
		if(pszInfoBuf)
		{
			HeapFree(GetProcessHeap(), 0, (LPVOID)pszInfoBuf);
			pszInfoBuf = NULL;
		}
		DisConnectRegistry(hKey);
		hKey = NULL;
		return -4;
	}

//Get object index
	int iByte = (int)dwByte,
	iObjectIndex = 0;


	GetIndex(NETWORK_OBJECT_NAME,pszInfoBuf,iByte,iObjectIndex);
	//Disconnect to HKEY_LOCAL_MACHINE
	DisConnectRegistry(hKey);
	hKey = NULL;

	if(pszInfoBuf)
	{
		HeapFree(GetProcessHeap(), 0, (LPVOID)pszInfoBuf);
		pszInfoBuf = NULL;
	}


	//Prepare performance data
	hKey = GetRegistryPerfKey(pszMachineName);
	if(!hKey)	return -5;
	GetPerfomanceInstance(hKey,iObjectIndex,lstText, lstValue);
	//GetPerfomanceInstance(hKey,238,szRet);
	

	DisConnectRegistry(hKey);
	hKey = NULL;
	return 0;

}
//like memory
int GetIISInfo1(char *pszMachineName,char* szRet)
{
	HKEY hKey = NULL;
	hKey=GetRegistryLocalKey(pszMachineName);
	if(hKey == NULL)
	{
		printf("ERROR=CONNECT REGISTRY FAILED\r\n");
		return -3;
	}

	char* pszInfoBuf = NULL;
	DWORD dwByte = 0;

	//////////////////////////////////////////////////////
	if(!CollectObjectCounterInfor(hKey,&pszInfoBuf,dwByte))
	{
		if(pszInfoBuf)
		{
			HeapFree(GetProcessHeap(), 0, (LPVOID)pszInfoBuf);
			pszInfoBuf = NULL;
		}
		DisConnectRegistry(hKey);
		hKey = NULL;
		return -4;
	}
	//Get object index
	int iByte = (int)dwByte,
	iObjectIndex = 0;

	
	GetIndex(IIS_OBJECT_NAME,pszInfoBuf,iByte,iObjectIndex);
	char szObj[10] = {0};
	itoa(iObjectIndex,szObj,10);
	DisConnectRegistry(hKey);
	hKey = NULL;

	//Get the handle of HKEY_PERFORMANCE_DATA
	hKey = GetRegistryPerfKey(pszMachineName);

	if(!hKey)
	{
		if(pszInfoBuf)
		{
			HeapFree(GetProcessHeap(), 0, (LPVOID)pszInfoBuf);
			pszInfoBuf = NULL;
		}
		return -5;
	}

	PPERF_DATA_BLOCK pdbBlock = NULL;
	if(!CollectRawData(hKey,pdbBlock,szObj))
	{
		if(pdbBlock)
		{
			free(pdbBlock);
			pdbBlock = NULL;
		}
		if(pszInfoBuf)
		{
			HeapFree(GetProcessHeap(), 0, (LPVOID)pszInfoBuf);
			pszInfoBuf = NULL;
		}
		DisConnectRegistry(hKey);
		hKey = NULL;
		//strRet.Format("error=收集计数器原始数据失败$");
		return -6;
		
	}

	//Initialize counter name
	char szCounterTitle[9][64];
	strcpy(szCounterTitle[0],"Maximum Connections");
	strcpy(szCounterTitle[1],"Current NonAnonymous Users");
	strcpy(szCounterTitle[2],"Current Connections");
	strcpy(szCounterTitle[3],"Total Not Found Errors");
	strcpy(szCounterTitle[4],"Post Requests/sec");
	strcpy(szCounterTitle[5],"Get Requests/sec");
	strcpy(szCounterTitle[6],"Bytes Sent/sec");
	strcpy(szCounterTitle[7],"Bytes Received/sec");
	strcpy(szCounterTitle[8],"Bytes Total/sec");

	int iCounterIndex[9] = {0};
	for(int tmp1 = 0;tmp1 < 9;tmp1++)
	{
		BOOL bFindFlag = FALSE;
		int iRet = -1;
		PPERF_OBJECT_TYPE				potObjTmp = NULL;
	    PPERF_COUNTER_DEFINITION		PerfCntr = NULL;

		potObjTmp = (PPERF_OBJECT_TYPE)((PBYTE)pdbBlock + pdbBlock->HeaderLength);
		for(int tmp = 0; tmp < (int)pdbBlock->NumObjectTypes;tmp++)
		{
			if((int)potObjTmp->ObjectNameTitleIndex == iObjectIndex)
			{
				 PerfCntr = (PPERF_COUNTER_DEFINITION) ((PBYTE)potObjTmp + potObjTmp->HeaderLength);
				 for(int mm = 0; mm < (int)potObjTmp->NumCounters ;mm++)
				 {
					 iRet = GetIndexNew(pszInfoBuf,szCounterTitle[tmp1],PerfCntr->CounterNameTitleIndex,iByte);
					 if(iRet != -1)
					 {
						 bFindFlag = TRUE;
						 break;
					 }
					 PerfCntr = (PPERF_COUNTER_DEFINITION)((PBYTE)PerfCntr + PerfCntr->ByteLength);
				 }
				 if(bFindFlag)
					 break;
			}
			else  //find nexe object 
				potObjTmp = (PPERF_OBJECT_TYPE)((PBYTE)potObjTmp + potObjTmp->TotalByteLength);
		}
		if(bFindFlag)
			iCounterIndex[tmp1] = iRet;
	}
	//Release memory
	if(pszInfoBuf)
	{
		HeapFree(GetProcessHeap(), 0, (LPVOID)pszInfoBuf);
		pszInfoBuf = NULL;
	}
	if(pdbBlock)
	{
		free(pdbBlock);
		pdbBlock = NULL;
	}

	//Prepare to collect data

	PPERF_DATA_BLOCK pdbDataBlock = NULL;
	DWORD dwValue[4] = {0};
	DWORD dwArrayX[2][2];
	LONGLONG llArrayX[3][2];
	LONGLONG llArrayY[2] = {0},
		     llFreq = 0;
	BOOL bObjFlag = TRUE;
	double dValue[9] = {0.0};

	for(int j = 0; j<IIS_LOOP_NUM;j++)
	{
		if(!CollectRawData(hKey,pdbDataBlock,szObj))
		{
			if(pdbDataBlock)
			{
				free(pdbDataBlock);
				pdbDataBlock = NULL;
			}
			DisConnectRegistry(hKey);
			hKey = NULL;
			//strRet.Format("error=收集计数器原始数据失败$");
			//return strRet;
			return -6;
		}
		llArrayY[j] = pdbDataBlock->PerfTime.QuadPart;
		llFreq = pdbDataBlock->PerfFreq.QuadPart;
		if(j == 0)
		{
			for(int k = 0;k < 4;k++)
			{
				LPVOID p = NULL;
				//if(!AnalysizeRawData(pdbDataBlock,iObjectIndex,"webservice",iCounterIndex[k],p))
				if(!AnalysizeRawData(pdbDataBlock,iObjectIndex,"_Total",iCounterIndex[k],p))
				{
					bObjFlag = FALSE;
					break;
				}
				dwValue[k] = *(DWORD *)p;
			}
		}
		for(int l = 0;l < 2;l++)
		{
			LPVOID p1 = NULL;
			//if(!AnalysizeRawData(pdbDataBlock,iObjectIndex,"webservice",iCounterIndex[l + 4],p1))
			if(!AnalysizeRawData(pdbDataBlock,iObjectIndex,"_Total",iCounterIndex[l + 4],p1))
			{
				bObjFlag = FALSE;
				break;
			}
			dwArrayX[l][j] = *(DWORD *)p1;
		}
		for(int m = 0;m < 3;m++)
		{
			LPVOID p2 = NULL;
			//if(!AnalysizeRawData(pdbDataBlock,iObjectIndex,"webservice",iCounterIndex[m + 6],p2))
			if(!AnalysizeRawData(pdbDataBlock,iObjectIndex,"_Total",iCounterIndex[m + 6],p2))
			{
				bObjFlag = FALSE;
				break;
			}
			llArrayX[m][j] = *(LONGLONG *)p2;
		}
		if(pdbDataBlock)
		{
			free(pdbDataBlock);
			pdbDataBlock = NULL;
		}
		Sleep(SLEEP_TIME);
		if(!bObjFlag)
			break;
	}
	DisConnectRegistry(hKey);
	hKey = NULL;

	if(!bObjFlag)
	{
		if(pdbDataBlock)
		{
			free(pdbDataBlock);
			pdbDataBlock = NULL;
		}
		//strRet.Format("error=计数器不存在$");
		//return strRet;
		return -7;
	}

	double dTemp = 1.0*(llArrayY[1] - llArrayY[0])/llFreq;
	dValue[0] = dwValue[0];
	dValue[1] = dwValue[1];
	dValue[2] = dwValue[2];
	dValue[3] = dwValue[3];
	if(dTemp != 0)
	{
		dValue[4] = 1.0*(dwArrayX[0][1] - dwArrayX[0][0])/dTemp;
		dValue[5] = 1.0*(dwArrayX[1][1] - dwArrayX[1][0])/dTemp;
		dValue[6] = 1.0*(llArrayX[0][1] - llArrayX[0][0])/dTemp;
		dValue[7] = 1.0*(llArrayX[1][1] - llArrayX[1][0])/dTemp;
		dValue[8] = 1.0*(llArrayX[2][1] - llArrayX[2][0])/dTemp;
	}

	sprintf(szRet,"maxconnections=%.2f$currentnonanonymoususers=%.2f$currentconnections=%.2f$totalnotfounderrors=%.2f$postrequestspersec=%.2f$getrequestspersec=%.2f$bytessentpersec=%.2f$bytesreceivedpersec=%.2f$bytestotalpersec=%.2f$"
		,dValue[0],dValue[1],dValue[2],dValue[3],dValue[4],dValue[5],dValue[6],dValue[7],dValue[8]);
	return 0;

}

int GetWebServerInfo1(char *pszMachineName,char* szRet)
{
	HKEY hKey = NULL;
	hKey=GetRegistryLocalKey(pszMachineName);
	if(hKey == NULL)
	{
		printf("ERROR=CONNECT REGISTRY FAILED\r\n");
		return -3;
	}

	char* pszInfoBuf = NULL;
	DWORD dwByte = 0;

	//////////////////////////////////////////////////////
	if(!CollectObjectCounterInfor(hKey,&pszInfoBuf,dwByte))
	{
		if(pszInfoBuf)
		{
			HeapFree(GetProcessHeap(), 0, (LPVOID)pszInfoBuf);
			pszInfoBuf = NULL;
		}
		DisConnectRegistry(hKey);
		hKey = NULL;
		return -4;
	}
	//Get object index
	int iByte = (int)dwByte,
	iObjectIndex = 0;
	GetIndex(WEB_OBJECT_NAME,pszInfoBuf,iByte,iObjectIndex);
	char szObj[10] = {0};
	itoa(iObjectIndex,szObj,10);
	//Disconnect to HKEY_LOCAL_MACHINE
	DisConnectRegistry(hKey);
	hKey = NULL;

	hKey = GetRegistryPerfKey(pszMachineName);
	if(!hKey)
	{
		if(pszInfoBuf)
		{
			HeapFree(GetProcessHeap(), 0, (LPVOID)pszInfoBuf);
			pszInfoBuf = NULL;
		}
		return -5;
	}
	PPERF_DATA_BLOCK pdbBlock = NULL;
	if(!CollectRawData(hKey,pdbBlock,szObj))
	{
		if(pdbBlock)
		{
			free(pdbBlock);
			pdbBlock = NULL;
		}
		if(pszInfoBuf)
		{
			HeapFree(GetProcessHeap(), 0, (LPVOID)pszInfoBuf);
			pszInfoBuf = NULL;
		}
		DisConnectRegistry(hKey);
		hKey = NULL;
		return -6;
	}

	char szCounterTitle[2][64];
	strcpy(szCounterTitle[0],"Bytes Total/sec");
	strcpy(szCounterTitle[1],"Total Method Requests/sec");

	int iCounterIndex[2] = {0};
	for(int tmp1 = 0;tmp1 < 2;tmp1++)
	{
		BOOL bFindFlag = FALSE;
		int iRet = -1;
		PPERF_OBJECT_TYPE				potObjTmp = NULL;
	    PPERF_COUNTER_DEFINITION		PerfCntr = NULL;

		potObjTmp = (PPERF_OBJECT_TYPE)((PBYTE)pdbBlock + pdbBlock->HeaderLength);
		for(int tmp = 0; tmp < (int)pdbBlock->NumObjectTypes;tmp++)
		{
			if((int)potObjTmp->ObjectNameTitleIndex == iObjectIndex)
			{
				 PerfCntr = (PPERF_COUNTER_DEFINITION) ((PBYTE)potObjTmp + potObjTmp->HeaderLength);
				 for(int mm = 0; mm < (int)potObjTmp->NumCounters ;mm++)
				 {
					 iRet = GetIndexNew(pszInfoBuf,szCounterTitle[tmp1],PerfCntr->CounterNameTitleIndex,iByte);
					 if(iRet != -1)
					 {
						 bFindFlag = TRUE;
						 break;
					 }
					 PerfCntr = (PPERF_COUNTER_DEFINITION)((PBYTE)PerfCntr + PerfCntr->ByteLength);
				 }
				 if(bFindFlag)
					 break;
			}
			else  //find nexe object 
				potObjTmp = (PPERF_OBJECT_TYPE)((PBYTE)potObjTmp + potObjTmp->TotalByteLength);
		}
		if(bFindFlag)
			iCounterIndex[tmp1] = iRet;
	}

	//Release memory
	if(pszInfoBuf)
	{
			HeapFree(GetProcessHeap(), 0, (LPVOID)pszInfoBuf);
			pszInfoBuf = NULL;
	}
	if(pdbBlock)
	{
		free(pdbBlock);
		pdbBlock = NULL;
	}

	//prepare to collect data
	PPERF_DATA_BLOCK pdbDataBlock = NULL;
	DWORD dwArrayX[2] = {0};
	LONGLONG llArrayX[2] = {0};
	LONGLONG llArrayY[2] = {0},
		     llFreq = 0;
	BOOL bObjFlag = TRUE;
	double dValue[2] = {0.0};

	for(int j = 0; j < WEB_LOOP_NUM;j++)
	{
		if(!CollectRawData(hKey,pdbDataBlock,szObj))
		{
			if(pdbDataBlock)
			{
				free(pdbDataBlock);
				pdbDataBlock = NULL;
			}
			DisConnectRegistry(hKey);
			hKey = NULL;
			return -6;
		}

		llArrayY[j] = pdbDataBlock->PerfTime.QuadPart;
		llFreq = pdbDataBlock->PerfFreq.QuadPart;

		LPVOID p1 = NULL;
		if(!AnalysizeRawData(pdbDataBlock,iObjectIndex,"_Total",iCounterIndex[0],p1))
		{
			bObjFlag = FALSE;
			break;
		}
		llArrayX[j] = *(LONGLONG *)p1;
		LPVOID p2 = NULL;
		if(!AnalysizeRawData(pdbDataBlock,iObjectIndex,"_Total",iCounterIndex[1],p2))
		{
			bObjFlag = FALSE;
			break;
		}

		dwArrayX[j] = *(DWORD *)p2;

		if(pdbDataBlock)
		{
			free(pdbDataBlock);
			pdbDataBlock = NULL;
		}
		Sleep(SLEEP_TIME);
	}
	DisConnectRegistry(hKey);
	hKey = NULL;

	if(!bObjFlag)  //Object doesn't exist
	{
		if(pdbDataBlock)
		{
			free(pdbDataBlock);
			pdbDataBlock = NULL;
		}
		return -7;
	}
	
	double dTemp = 1.0*(llArrayY[1] - llArrayY[0])/llFreq;
	if(dTemp != 0.0)
	{
		dValue[0] = 1.0*(llArrayX[1] - llArrayX[0])/dTemp;
		dValue[1] = 1.0*(dwArrayX[1] - dwArrayX[0])/dTemp;
	}
	sprintf(szRet,"hitsPerSec=%.2f$bytesPerSec=%.2f$",dValue[0],dValue[1]);
	return 0;
}





int GetNetWorkInfo1(char *pszMachineName,char* szRet,CString strNetWork)
{
	HKEY hKey = NULL;
	hKey=GetRegistryLocalKey(pszMachineName);
	if(hKey == NULL)
	{
		printf("ERROR=CONNECT REGISTRY FAILED\r\n");
		return -3;
	}

	char* pszInfoBuf = NULL;
	DWORD dwByte = 0;



	if(!CollectObjectCounterInfor(hKey,&pszInfoBuf,dwByte))
	{
		if(pszInfoBuf)
		{
			HeapFree(GetProcessHeap(), 0, (LPVOID)pszInfoBuf);
			pszInfoBuf = NULL;
		}
		DisConnectRegistry(hKey);
		hKey = NULL;
		//strRet.Format("error=采集计数器信息失败$");
//		return strRet;
		return -6;
	}

	///////////////////////////////////////////////////////////////////
	int iByte = (int)dwByte,
		iObjectIndexNI = 0,
		iObjectIndexTCP = 0;
	//Get object index
	GetIndex(NETWORK_OBJECT_NAME,pszInfoBuf,iByte,iObjectIndexNI);
		char szObjNI[10] = {0};
		itoa(iObjectIndexNI,szObjNI,10);
	

	//Build counter title array
	char szCounter[4][64];
	strcpy(szCounter[0],"Packets Outbound Errors");
	strcpy(szCounter[1],"Packets Received Errors");
	strcpy(szCounter[2],"Bytes Sent/sec");
	strcpy(szCounter[3],"Bytes Received/sec");

	int	iCounterConnect = 0,
		iCounterIndex[4] = {0};
	for(int i = 0;i < 4;i++)
		GetIndex(szCounter[i],pszInfoBuf,iByte,iCounterIndex[i]);

	
	if(pszInfoBuf)
	{
		HeapFree(GetProcessHeap(), 0, (LPVOID)pszInfoBuf);
		pszInfoBuf = NULL;
	}
		//Disconnect to HKEY_LOCAL_MACHINE
	DisConnectRegistry(hKey);
	hKey = NULL;

	//Connect to HKEY_PERFORMACNE_DATA Key
	hKey = GetRegistryPerfKey(pszMachineName);
	if(!hKey)
	{
		//strRet.Format("error=打开注册表失败$");
		//return strRet;

		return -5;
	}
	

	
	//////////////////////////////////////////////////////
	//------------------Connect to HKEY_PERFORMANCE_DATA ok------------------
	//Collect data
	//Get TCP established connection
	PPERF_DATA_BLOCK pdbDataBlock = NULL;
	

	int j = 0;
	BOOL bObjFlag = TRUE;
	LONGLONG llArrayY[2] = {0},
			 llFreq = 0;
	DWORD dwArrayX[2][2] ={0,0},
		  dwErrorPacket[2] ={0};	
	for(j = 0;j < NETWORK_LOOP_NUM;j++)
	{
		if(!CollectRawData(hKey,pdbDataBlock,szObjNI))
		{
			if(pdbDataBlock)
			{
				free(pdbDataBlock);
				pdbDataBlock = NULL;
			}
			DisConnectRegistry(hKey);
			hKey = NULL;
			//strRet.Format("error=收集计数器原始数据失败$");
			//return strRet;
			return -4;
		}
		llArrayY[j] = pdbDataBlock->PerfTime.QuadPart;
		llFreq = pdbDataBlock->PerfFreq.QuadPart;
		//Get the data of counter which type is RAWCOUNTER
		if(j == 0)
		{
			int k = 0;
			for(k = 0;k<2;k++)
			{
				LPVOID p = NULL;
				if(!AnalysizeRawData(pdbDataBlock,iObjectIndexNI,strNetWork,iCounterIndex[k],p))
				{
					bObjFlag = FALSE;
					break;
				}
				dwErrorPacket[k] = *(DWORD *)p;
			}
			if(!bObjFlag)
				break;
		}
		//Get the data of counter which type is COUNTER
		int l = 0;
		for(l = 0;l < 2;l++)
		{
			LPVOID pp =  NULL;
			if(!AnalysizeRawData(pdbDataBlock,iObjectIndexNI,strNetWork,iCounterIndex[l+2],pp))
			{
				bObjFlag = FALSE;
				break;
			}
			dwArrayX[l][j] = *(DWORD *)pp;
		}
		if(!bObjFlag)
			break;

		if(pdbDataBlock)
		{
			free(pdbDataBlock);
			pdbDataBlock = NULL;
		}
		Sleep(SLEEP_TIME);
	}
	
	DisConnectRegistry(hKey);
	hKey = NULL;

	if(!bObjFlag)
	{
		if(pdbDataBlock)
		{
			free(pdbDataBlock);
			pdbDataBlock = NULL;
		}
		//strRet.Format("error=计数器不存在$");
		return -7;
	}
	//Deal with analysize data
	double dValue[4];
	double y = 0;
	y = (1.0*(llArrayY[1] - llArrayY[0]))/llFreq;
	if(y != 0)
	{
		dValue[0] = (double)(dwArrayX[0][1] - dwArrayX[0][0])/y;
		dValue[1] = (double)(dwArrayX[1][1] - dwArrayX[1][0])/y;
	}
	dValue[2] = (double)0;
	dValue[3] = (double)(dwErrorPacket[0] + dwErrorPacket[1]);
	sprintf(szRet, "packetErrorsPerSec=%.2f$bytesPerSecReceived=%.2f$bytesPerSecSent=%.2f$status=200$"
		,dValue[3],dValue[1],dValue[0]);
	return 0;
	
}
