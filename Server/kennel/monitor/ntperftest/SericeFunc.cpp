
#include "stdafx.h"
#include <WinSvc.h>

void EnumServices(char *pszMachineName, DWORD dwType, DWORD dwState,DWORD *pdwCount ,CStringList& lstText, CStringList& lstValue)
{
  ASSERT(pdwCount != NULL);
  // Maybe check if dwType and dwState have at least one constant specified

  *pdwCount = 0;
  CString  strPrintContent=_T("");
  char szRemoteName[MAX_PATH] = {0};
  //SC_HANDLE scman = ::OpenSCManager(NULL,NULL,SC_MANAGER_ENUMERATE_SERVICE);
  sprintf(szRemoteName,"\\\\%s",pszMachineName);
  //printf("服务器=%s\n",szRemoteName);
  SC_HANDLE scman =NULL;
  if (strcmp(pszMachineName,"")==0)
	scman =::OpenSCManager(NULL,NULL,SC_MANAGER_ENUMERATE_SERVICE);
  else
	scman =::OpenSCManager(szRemoteName,NULL,SC_MANAGER_ENUMERATE_SERVICE);
  
  if (scman) {
	  
    ENUM_SERVICE_STATUS service, *lpservice;
    BOOL rc;
    DWORD bytesNeeded,servicesReturned,resumeHandle = 0;
    rc = ::EnumServicesStatus(scman,dwType,dwState,&service,sizeof(service),
                              &bytesNeeded,&servicesReturned,&resumeHandle);
    if ((rc == FALSE) && (::GetLastError() == ERROR_MORE_DATA)) {
      DWORD bytes = bytesNeeded + sizeof(ENUM_SERVICE_STATUS);
      lpservice = new ENUM_SERVICE_STATUS [bytes];
      ::EnumServicesStatus(scman,dwType,dwState,lpservice,bytes,
                                &bytesNeeded,&servicesReturned,&resumeHandle);
      *pdwCount = servicesReturned;    // Not a chance that 0 services is returned
     
      TCHAR Buffer[1024];         // Should be enough for service info
      QUERY_SERVICE_CONFIG *lpqch = (QUERY_SERVICE_CONFIG*)Buffer;
	  
      for (DWORD ndx = 0; ndx < servicesReturned; ndx++) {
		  lstText.AddTail(lpservice[ndx].lpDisplayName);
		  lstValue.AddTail(lpservice[ndx].lpServiceName);
		 

        //printf ("%s<br>\n",lpservice[ndx].lpDisplayName);
	 	   //strPrintContent.Format("%s<br>\n",lpservice[ndx].lpDisplayName);
		   //PrintItemTR(strPrintContent);

		/*
        SC_HANDLE sh = ::OpenService(scman,lpservice[ndx].lpServiceName,SERVICE_QUERY_CONFIG);
		
        if (::QueryServiceConfig(sh,lpqch,sizeof(Buffer),&bytesNeeded)) {
        }
		
        ::CloseServiceHandle(sh);
		*/

      }
      delete [] lpservice;
    }
    ::CloseServiceHandle(scman);
  }
  else{
  }
  return ;
}



//////////////////////////////////////////////////////////
DWORD GetMachineServiceStatus(char* pszMachineName,char* pszServiceName)
{
	SC_HANDLE m_hSC = NULL;
	DWORD dwServiceStatus=-1;
	m_hSC=OpenSCManager(pszMachineName,NULL,SC_MANAGER_ENUMERATE_SERVICE);
	if(m_hSC==NULL)
		return dwServiceStatus;

	SC_HANDLE sc=NULL;
	sc = OpenService(m_hSC,pszServiceName,GENERIC_READ);

	LPSERVICE_STATUS lpServiceStatus=NULL;
	lpServiceStatus=new SERVICE_STATUS;

	if(sc)
	{
		DWORD dwBytesNeed=0;
		if(QueryServiceStatus(sc,lpServiceStatus))
			dwServiceStatus=lpServiceStatus->dwCurrentState;
	}
	delete lpServiceStatus;
	lpServiceStatus=NULL;

	CloseServiceHandle(sc);
	sc = NULL;
	CloseServiceHandle(m_hSC);
	m_hSC = NULL;
	return dwServiceStatus;
}

int GetServiceInfo1(char* pszMachineName,char* pszServiceName, char* szRet)
{
	DWORD dwServiceStatus = -1;

	if (strcmp(pszMachineName,"")==0)
		dwServiceStatus = GetMachineServiceStatus(NULL,pszServiceName);
	else
		dwServiceStatus = GetMachineServiceStatus(pszMachineName,pszServiceName);

		if(dwServiceStatus == -1)
	{
		//strRet.Format("error=获取指定服务状态失败\r\n");
			sprintf(szRet,"error=%s$",FuncGetStringFromIDS("SV_NT_SERVICE",
                "NT_SERVICE_QUERY_STATUS_FAILED"));//<%IDS_SericeFunc_01%>
		
	}
	if(dwServiceStatus == 4)  //run
		sprintf(szRet,"Processes=1$");

	if(dwServiceStatus == 1)  //stop
		sprintf(szRet,"Processes=0$");
	return 0;
	

}

