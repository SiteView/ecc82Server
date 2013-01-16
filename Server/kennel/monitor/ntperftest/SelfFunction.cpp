#include "stdafx.h"
#include "selffunction.h"

void GetIndex(const char* pszCounterOrObjectName,char* pszTitleIndexBuf,int iBufSize,int &iID)
{
	char* pszTemp = NULL;
	pszTemp = pszTitleIndexBuf;
	int i = 0,
		j = 0;
	char szIndex[MAX_BUF_SIZE] = {0},
		 szObject[MAX_BUF_SIZE] = {0}; 
	while(i != iBufSize)
	{
		while(*(pszTemp + i) != '\0')
		{
			szIndex[j] = *(pszTemp+i);
			i++;
			j++;
		}
		szIndex[j] = '\0';
		i++;
		j = 0;
		while (*(pszTemp+i) != '\0')
		{
			szObject[j] = *(pszTemp+i);
			i++;
			j++;
		}
		szObject[j] = '\0';
		i++;
		j = 0;
		if(*(pszTemp+i) == '\0')
			i++;
		if(strcmp(szObject,pszCounterOrObjectName) == 0)
		{
			iID = atoi(szIndex);
			break;
		}
	}
	return;
}
/////////////////////////////////////////////////////////////////////////////////////////
HKEY ConnectRegistry(const char* pszComputerName,int iMachineType,int iKeyType)
{
	HKEY hKey = NULL;
	LONG lRet = 0;
	if(iKeyType == 0)//HKEY_LOCAL_MACHINE
	{
		if(iMachineType == 0)	  //Remote machine
			lRet = RegConnectRegistry(pszComputerName, HKEY_LOCAL_MACHINE, &hKey);
		else    //Local machine
			lRet = RegConnectRegistry(NULL, HKEY_LOCAL_MACHINE, &hKey);
	}
	if(iKeyType == 1) //HKEY_PERFORMANC_DATA
	{
		if(iMachineType == 0)	//Remote machine
			lRet = RegConnectRegistry(pszComputerName, HKEY_PERFORMANCE_DATA, &hKey);
		else //Local machine
			lRet = RegConnectRegistry(NULL, HKEY_PERFORMANCE_DATA, &hKey);
	}
	return hKey;

}

///////////////////////////////////////////////////////////////////////////////////////////
BOOL CollectObjectCounterInfor(HKEY hKey,char** pszInfoBuffer,DWORD &dwByte)
{
	HKEY hSubKey = NULL;
	LONG lRet = 0;
	lRet = RegOpenKeyEx(hKey,KEYEN_PERFLIB_PATH, 0, KEY_READ, &hSubKey);
	if(lRet != ERROR_SUCCESS)
		return FALSE;

	dwByte = 0;
	lRet = RegQueryValueEx(hSubKey, VALUE_NAME, NULL, NULL, NULL, &dwByte);
	if(lRet != ERROR_SUCCESS)
	{
		RegCloseKey(hSubKey);
		return FALSE;
	}
	*pszInfoBuffer = NULL;
	*pszInfoBuffer = (char *) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwByte);
	if(*pszInfoBuffer == NULL)
	{
		RegCloseKey(hSubKey);
		return FALSE;
	}
	lRet = RegQueryValueEx(hSubKey, VALUE_NAME, NULL, NULL, (LPBYTE)(*pszInfoBuffer), &dwByte);
	if(lRet != ERROR_SUCCESS)
	{
		RegCloseKey(hSubKey);
		HeapFree(GetProcessHeap(), 0, (LPVOID)(*pszInfoBuffer));
		*pszInfoBuffer = NULL;
		return FALSE;
	}
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CollectRawData(HKEY hKey,PPERF_DATA_BLOCK &pDataBlock,char* szObjIndex)
{
	DWORD dwBufSize = TOTAL_BYTE;
	pDataBlock = NULL;
	pDataBlock = (PPERF_DATA_BLOCK) malloc(dwBufSize);
	if(!pDataBlock)
		return FALSE;
	while(RegQueryValueEx(hKey,szObjIndex,NULL,NULL,(LPBYTE)pDataBlock,&dwBufSize ) == ERROR_MORE_DATA)
	{
		// Get a buffer that is big enough.
		dwBufSize += BYTE_INCREMENT;
		pDataBlock = (PPERF_DATA_BLOCK) realloc(pDataBlock, dwBufSize);
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL AnalysizeRawData(PPERF_DATA_BLOCK pdbData,int iObjIndex,const char* pszInstanceInfo,int iCounterIndex,LPVOID &pData)
{
	PPERF_OBJECT_TYPE				potObj = NULL;
	PPERF_INSTANCE_DEFINITION		pidInst = NULL ;
	PPERF_COUNTER_DEFINITION		pcdCoun = NULL,
									pcdCurCoun = NULL;
	PPERF_COUNTER_BLOCK				pcbPtrToCntr = NULL;

	//Get first object type
	potObj = (PPERF_OBJECT_TYPE)((PBYTE)pdbData + pdbData->HeaderLength);
	int iRightInstance = 0;
	int i = 0;
	for(i = 0;i < pdbData->NumObjectTypes;i++)
	{
		//Find correct object by object titile indxe
		if(potObj->ObjectNameTitleIndex == iObjIndex)
		{
			//Get the counter block
			pcdCoun = (PPERF_COUNTER_DEFINITION)((PBYTE)potObj + potObj->HeaderLength);
			//This object has one instance only
			CString strPrintContent=_T("");

			if(potObj->NumInstances > 0)
			{
				//iRightInstance = GetRightInstance(pszInstanceInfo,potObj);
				/**********wang peng *****************/
				

				//Get the first instance
				pidInst = (PPERF_INSTANCE_DEFINITION)((PBYTE)potObj + potObj->DefinitionLength);
                int j = 0;
				//Regrive all instance
				for(j = 0;j < potObj->NumInstances;j++)
				{
					pcdCurCoun = pcdCoun;
					pcbPtrToCntr = (PPERF_COUNTER_BLOCK)((PBYTE)pidInst + pidInst->ByteLength);

				/**********wang peng *****************/
					//Get instance name
					strPrintContent.Format("%S",(char *)((PBYTE)pidInst +
						pidInst->NameOffset) );
					//Get the correct instance

					if (strPrintContent.CompareNoCase(pszInstanceInfo)==0)
				/**********wang peng *****************/
					//if(j == iRightInstance)
					{
						//Retrive all counter under current instance
						int k = 0;
						for(k = 0;k < potObj->NumCounters;k++)
						{
							if(pcdCurCoun->CounterNameTitleIndex == iCounterIndex)
							{
								pData = (LPVOID)((PBYTE)pcbPtrToCntr + pcdCurCoun->CounterOffset);
								return TRUE;
							}	
							//Get next counter
							pcdCurCoun =  (PPERF_COUNTER_DEFINITION)((PBYTE)pcdCurCoun +  pcdCurCoun->ByteLength);
						}
					}
					//Get next instance
					pidInst = (PPERF_INSTANCE_DEFINITION)((PBYTE)pcbPtrToCntr + pcbPtrToCntr->ByteLength);      
				}
			}
			else//This object doesn't have instance
			{
				pcbPtrToCntr = (PPERF_COUNTER_BLOCK)((PBYTE)potObj + potObj->DefinitionLength);
				//Retrieve all counters
				int l = 0;
				for(l = 0;l < potObj->NumCounters;l++)
				{
					if(pcdCoun->CounterNameTitleIndex == iCounterIndex)
					{
						pData = (LPVOID)((PBYTE)pcbPtrToCntr + pcdCoun->CounterOffset);
						return TRUE;
					}
					//Get next counter
					pcdCoun = (PPERF_COUNTER_DEFINITION)((PBYTE)pcdCoun +  pcdCoun->ByteLength);
				}
			}
		}
		else  //find nexe object 
			potObj = (PPERF_OBJECT_TYPE)((PBYTE)potObj + potObj->TotalByteLength);
	}
	return FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////
int GetRightInstance(const char* pszInstanceInfo,PPERF_OBJECT_TYPE potObject)
{
	int iRet = 0;
	if(stricmp(pszInstanceInfo,"cpu") == 0)
		iRet = potObject->NumInstances - 1;
	if(stricmp(pszInstanceInfo,"NetInterface") == 0)
		iRet = 0;
	if(stricmp(pszInstanceInfo,"webservice") == 0)
		iRet = 0;
	return iRet;
}
////////////////////////////////////////////////////////////////////////////////////////////
BOOL DisConnectRegistry(HKEY hKey)
{
	LONG lRet = RegCloseKey(hKey);
	if(lRet != ERROR_SUCCESS)
		return FALSE;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////
BOOL LoginRemoteMachine(char* pszMachineName,char* pszUserAccount,char* pszPassWord)
{
	NETRESOURCE ns;
	char szRemoteName[MAX_PATH] = {0};
	ns.dwType = RESOURCETYPE_ANY;
	ns.lpProvider = NULL;
	ns.lpLocalName = "";
	sprintf(szRemoteName,"\\\\%s",pszMachineName);
	ns.lpRemoteName = szRemoteName;
		
	DWORD dwResult = 0;
	for(int i = 0;i<MAX_RETRY_NUM;i++)
	{
		if(strcmp(pszPassWord,"NULL") == 0)
			dwResult = WNetAddConnection2(&ns,"",pszUserAccount,CONNECT_UPDATE_PROFILE);
		else
			dwResult = WNetAddConnection2(&ns,pszPassWord,pszUserAccount,CONNECT_UPDATE_PROFILE);
		if(dwResult == 0)
			break;
		Sleep(LOGIN_SLEEP);
	}
	if(dwResult != NO_ERROR)
	{
		#ifdef _DEBUG
			switch(dwResult)
			{
				case ERROR_ACCESS_DENIED:
					printf("ERROR_ACCESS_DENIED=%ld\r\n",dwResult);
					break;
				case ERROR_ALREADY_ASSIGNED:
					printf("ERROR_ALREADY_ASSIGNED=%ld\r\n",dwResult);
					break;
				case ERROR_BAD_DEV_TYPE:
					printf("ERROR_BAD_DEV_TYPE=%ld\r\n",dwResult);
					break;
				case ERROR_BAD_DEVICE:
					printf("ERROR_BAD_DEVICE=%ld\r\n",dwResult);
					break;
				case ERROR_BAD_NET_NAME:
					printf("ERROR_BAD_NET_NAME=%ld\r\n",dwResult);
					break;
				case ERROR_BAD_PROFILE:
					printf("ERROR_BAD_PROFILE=%ld\r\n",dwResult);
					break;
				case ERROR_BAD_PROVIDER:
					printf("ERROR_BAD_PROVIDER=%ld\r\n",dwResult);
					break;
				case ERROR_BUSY:
					printf("ERROR_BUSY=%ld\r\n",dwResult);
					break;
				case ERROR_CANCELLED:
					printf("ERROR_CANCELLED=%ld\r\n",dwResult);
					break;
				case ERROR_CANNOT_OPEN_PROFILE:
					printf("ERROR_CANNOT_OPEN_PROFILE=%ld\r\n",dwResult);
					break;
				case ERROR_DEVICE_ALREADY_REMEMBERED:
					printf("ERROR_DEVICE_ALREADY_REMEMBERED=%ld\r\n",dwResult);
					break;
				case ERROR_EXTENDED_ERROR:
					printf("ERROR_EXTENDED_ERROR=%ld\r\n",dwResult);
					break;
				case ERROR_INVALID_PASSWORD:
					printf("ERROR_INVALID_PASSWORD=%ld\r\n",dwResult);
					break;
				case ERROR_NO_NET_OR_BAD_PATH:
					printf("ERROR_CANNOT_OPEN_PROFILE=%ld\r\n",dwResult);
					break;
				case ERROR_NO_NETWORK:
					printf("ERROR_CANNOT_OPEN_PROFILE=%ld\r\n",dwResult);
					break;
				default:
					printf("Unknown error=%ld\r\n",dwResult);
					break;
			}
#endif
			return FALSE;
	}
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
int DetectFreeDiskSymbol()
{
	int i = 0,
		iRet = 0;
	DWORD dwLength = 20,
		  dwRet	= 0;
	char szAllLocalDisk[12][3]={"O:","P:","Q:","R:","S:","T:","U:","V:",
								"W:","X:","Y:","Z:"};
	char szRemoteMachine[20] = {0};
	for(i = 0;i < 12;i++)
	{
		dwRet = WNetGetConnection(szAllLocalDisk[i],szRemoteMachine,&dwLength);
		if(!((dwRet == 0 )||(dwRet == 2250)))
		{
			iRet = -1;
			break;
		}
		if(stricmp(szRemoteMachine,"") == 0)
		{
			iRet = i+1;
			break;
		}
		dwLength = 20;
		strcpy(szRemoteMachine,"");
	}
	return iRet;
}
////////////////////////////////////////////////////////////////////////////////
BOOL CancelConnectLocalDevice(char* pszLocalDevice,DWORD dwFlag,BOOL bForce)
{	
	if(WNetCancelConnection2(pszLocalDevice,dwFlag,bForce)!=NO_ERROR)
		return FALSE;
	else
		return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
void GetAvaliableDisk(int iDiskIndex,char* pszAvaliableDisk)
{
	switch(iDiskIndex)
	{
		case 1:
			strcpy(pszAvaliableDisk,"O:");
			break;
		case 2:
			strcpy(pszAvaliableDisk,"P:");
			break;
		case 3:
			strcpy(pszAvaliableDisk,"Q:");
			break;
		case 4:
			strcpy(pszAvaliableDisk,"R:");
			break;
		case 5:
			strcpy(pszAvaliableDisk,"S:");
			break;
		case 6:
			strcpy(pszAvaliableDisk,"T:");
			break;
		case 7:
			strcpy(pszAvaliableDisk,"U:");
			break;
		case 8:
			strcpy(pszAvaliableDisk,"V:");
			break;
		case 9:
			strcpy(pszAvaliableDisk,"W:");
			break;
		case 10:
			strcpy(pszAvaliableDisk,"X:");
			break;
		case 11:
			strcpy(pszAvaliableDisk,"Y:");
			break;
		case 12:
			strcpy(pszAvaliableDisk,"Z:");
			break;
		default:
			break;
	}
}

////////////////////////////////////////////////////////////////////////////
int GetIndexNew(char* pszInfoBuf,char* pszCounterTitle,int iCounterIndex,int iBufSize)
{
	int iRet = -1;
	char* pszTemp = NULL;
	pszTemp = pszInfoBuf;
	char szIndex[MAX_BUF_SIZE] = {0},
		 szObject[MAX_BUF_SIZE] = {0}; 
	int i = 0,
		j = 0;
	while(i < iBufSize)
	{
		while(*(pszTemp + i) != '\0')
		{
			szIndex[j] = *(pszTemp+i);
			i++;
			j++;
		}
		szIndex[j] = '\0';
		i++;
		j = 0;
		while (*(pszTemp+i) != '\0')
		{
			szObject[j] = *(pszTemp+i);
			i++;
			j++;
		}
		szObject[j] = '\0';
		i++;
		j = 0;
		if(*(pszTemp+i) == '\0')
			i++;
		if(atoi(szIndex) == iCounterIndex)
		{
			if(stricmp(szObject,pszCounterTitle) == 0)
			{
				iRet = iCounterIndex;
				break;
			}
		}

	}
	return iRet;

}
//////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CheckCheck()
{
	int iMonitorNum =0;
	//if(!AimCheckUserRight(1,iMonitorNum))
	//	return FALSE;
	return TRUE;
}


HKEY GetRegistryLocalKey(const char* pszComputerName)
{
	HKEY hKey = NULL;
	LONG lRet = 0;
	if(strcmp(pszComputerName,"") != 0)//Local machine
			lRet = RegConnectRegistry(pszComputerName, HKEY_LOCAL_MACHINE, &hKey);
	else
		lRet = RegConnectRegistry(NULL, HKEY_LOCAL_MACHINE, &hKey);

	return hKey;

}

HKEY GetRegistryPerfKey(const char* pszComputerName)
{
	HKEY hKey = NULL;
	LONG lRet = 0;

	if(strcmp(pszComputerName,"") != 0){
			lRet = RegConnectRegistry(pszComputerName, HKEY_PERFORMANCE_DATA, &hKey);
	}
	else

		lRet = RegConnectRegistry(NULL, HKEY_PERFORMANCE_DATA, &hKey);

	return hKey;
}
