// NTPerfTest.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "selffunction.h"

#include "NTPerfTest.h"
#include <WinSvc.h>
#include "io.h"
#include "stdlib.h"
#include "stdio.h"

#include "string.h"
#include <cstring>

#include "ServiceFunc.h"
#include "regperf.h"
#include "prj_WMI.h"
#include "SV_WMI.h"
//#include "../../base/splitquery.h"
//#include "../../kennel/svdb/svapi/svdbapi.h"
#include <scsvapi/svdbapi.h>
#include <basetype.h>

#include <string>
#include <list>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define ALL_USE_WMI    2
//#define LOGIN_SLEEP			1000
//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CNTPerfTestApp

BEGIN_MESSAGE_MAP(CNTPerfTestApp, CWinApp)
	//{{AFX_MSG_MAP(CNTPerfTestApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNTPerfTestApp construction

CNTPerfTestApp::CNTPerfTestApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////

//wmi com
CString GetAllDiskbyCom(CString strServer, CString strUser,CString strPwd );
CString GetDiskInfobyCom(CString strServer, CString strUser,CString strPwd, CString strDisk);
CString GetCPUInfobyCom(CString strServer, CString strUser,CString strPwd);
CString GetCPUInfobyScript(CString strCustomer, CString strGroupID, CString strMonitorID, CString strServer, CString strUser,CString strPwd);
CString GetServicesbyCom(CString strServer, CString strUser,CString strPwd);
CString GetMemoryInfobyCom(CString strServer, CString strUser,CString strPwd );
CString GetServiceInfobyCom(CString strServer, CString strUser,CString strPwd, CString strDisk);
CString GetNetWorkInfobyCom(CString strServer, CString strUser,CString strPwd, CString strDisk);
CString GetIISInfobyCom(CString strServer, CString strUser,CString strPwd );
CString GetWebInfobyCom(CString strServer, CString strUser,CString strPwd );
CString GetAllNetWorkbyCom(CString strServer, CString strUser,CString strPwd );
CString GetAllNetWorkbyCom(CString strServer, CString strUser,CString strPwd );
CString GetAllProcessbyCom(CString strServer, CString strUser,CString strPwd );
CString GetProcessInfobyCom(CString strServer, CString strUser,CString strPwd, CString strDisk);
CString GetASPInfobyCom(CString strServer, CString strUser,CString strPwd );
CString GetSybaseInfobyCom(CString strDBName, CString strUser,CString strPwd);
CString GetPhysicsMemoryInfobyCom(CString strServer, CString strUser,CString strPwd );

CString FilterNTEventLogByCom(CString strServer,CString strUser,CString strPwd,
                              CString strLogName,CString strType, CString strCodeList,
                              CString strSourceList, CString strEventMachine,
                              CString strCustomer, CString strGroupID,
                              CString strMonitorID);
CString GetNTEventLogInfobyCom(CString strServer, CString strUser,CString strPwd, CString strLogName, CString strEventType, CString strAlert, CString strEventKey, CString strNotEventKey, CString strPosMatch, CString strNegMatch, CString strEventCategory, CString strEventMachine, CString strInterval, CString strGroupID, CString strMonitorID, CString strCustomer);
//CString GetNTEventLogInfobyExe(CString strServer, CString strUser,CString strPwd, CString strLogName, CString strEventType, CString strAlert, CString strEventKey, CString strNotEventKey, CString strPosMatch, CString strNegMatch, CString strEventCategory, CString strEventMachine, CString strInterval, CString strGroupID, CString strMonitorID, CString strCustomer);

CString GetSQLServerInfobyCom(CString strServer, CString strUser,CString strPwd );
CString GetExchangeInfobyCom(CString strServer, CString strUser,CString strPwd );
CString GetAspNetInfoByCom(CString strServer,CString strUser,CString strPwd);//------2003.3.10 13:37 by Eason-------------------
CString GetAspNetApplicationInfoByCom(CString strServer,CString strUser,CString strPwd);//------2003.3.10 15:09 by Eason-------------------


BOOL GetMachineInfo(CStringList& lstParas,CString &strMachineName,
					CString& strUserAccount,CString& strPassWord);
void DebugInfo(const char * szMsg);
//得到远程主机版本号
int GetVersionFromCom(CString strServer, CString strUser,CString strPwd);
// The one and only CNTPerfTestApp object
CString GetFileInfobyCom(CString strFilePath);
CString GetDirInfobyCom(CString strDirPath, CString strMatch);

////for nt4.0
CString GetAllProcessbyComNT40(CString strServer, CString strUser,CString strPwd );
CString GetProcessInfobyComNT40(CString strServer, CString strUser,CString strPwd, CString strDisk);
CString GetCPUInfobyComNT40(CString strServer, CString strUser,CString strPwd);

CNTPerfTestApp theApp;

BOOL AddToExceptionLog(const char* szMsg);


bool MakeStringListByChar(std::list<string>& pList, const char * pInput )
{
	const char * p;
	int nSize;
	p=pInput;
	while(*p!='\0')
	{
		nSize = static_cast<int>(strlen(p));
		if( nSize>0 )
		{	
			pList.push_back((string)p);
		//	printf(p);
		}
		p=p+nSize+1;
	}

	return true;
}

////////////////////////////////
//HRESULT ThreeCreateSVCom( _clsWMI * myRef)
_clsWMI * ThreeCreateSVCom( )
{
		CString szMutex=_T("AIM4.0-NTPERFTEST-DLL-RUN");
		int nInstance =6;

		HANDLE hMutex = NULL;

		hMutex = ::CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, 10, szMutex);
			
		if (hMutex == NULL) 
			return NULL;

		DWORD dwLastError =GetLastError();

		
		int nTemp=0;
		int i=0;
		_clsWMI * myRef=NULL;
		HRESULT hr;
		div_t div_result;

		srand(1);


		for(int lv=0 ;lv<6 ;lv++)
		{
		
			int* lpvFile = (int*)::MapViewOfFile(hMutex, FILE_MAP_WRITE, 0, 0, 0);

			nTemp = lpvFile[0];

			if (nTemp > (nInstance - 1)) {
				Sleep(5000);
				continue;	
			}
			else {
				lpvFile[0] = nTemp+1;
				::UnmapViewOfFile(lpvFile);
				break;
			}
		}
		if (nTemp > (nInstance - 1)) {			
			goto Fin;
		}





	for (i=0;i<6;i++)
	{
		 hr=CoInitialize(NULL);
		 
		 if (SUCCEEDED(hr))
			 break;
		 div_result=div(rand() , 3);
		 Sleep((5+div_result.rem)*1000);

		 
	}

	if (FAILED(hr))
	{
		CString str;
		CTime tnNow =CTime::GetCurrentTime();
		str.Format(" CoInitialize failed %s",TimeToString(tnNow) );
		AddToExceptionLog(str);	

		if (hMutex) {

			if (nInstance > 1) {
				int* lpvFile = (int*)::MapViewOfFile(hMutex, FILE_MAP_WRITE, 0, 0, 0);
				int nTemp = lpvFile[0];
				lpvFile[0] = nTemp - 1;
				::UnmapViewOfFile(lpvFile);
			}

			CloseHandle(hMutex); 
			hMutex = NULL; 
		}

		return NULL;
	}

	int sleeptime,totalsleeptime;
	totalsleeptime=0;
	for ( i=0;i<6;i++)
	{
		 hr=CoCreateInstance(CLSID_clsWMI,NULL,
							CLSCTX_ALL,
							IID__clsWMI,(void **)&myRef);
		 if (SUCCEEDED(hr))
			 break;
		 
		 div_result=div(rand() , 3);
		 sleeptime=(5+div_result.rem)*1000;
		 totalsleeptime+=sleeptime;
		 Sleep(sleeptime);
		 
	}

	//return hr;
	if (FAILED(hr))
	{
		CString str;
		CTime tnNow =CTime::GetCurrentTime();
		str.Format("use sv com failed sleeptime =%d reason = %d ,time =%s",totalsleeptime ,hr ,TimeToString(tnNow ));
		AddToExceptionLog(str);
		myRef=NULL;
	}

Fin:

	if (hMutex) {
		if (nInstance > 1) {
			int* lpvFile = (int*)::MapViewOfFile(hMutex, FILE_MAP_WRITE, 0, 0, 0);
			int nTemp = lpvFile[0];
			lpvFile[0] = nTemp - 1;
			::UnmapViewOfFile(lpvFile);
		}

		CloseHandle(hMutex); 
		hMutex = NULL; 
	}
	return myRef;

}
BOOL GetDyn(std::string strMid , int& nState, std::string&  strDyn  )
{
	nState=0;
	strDyn ="";

	sv_dyn dyn;                
	if(GetSVDYN(strMid, dyn ))
    {
		nState= dyn.m_state;
		if(dyn.m_displaystr!=NULL)
			strDyn = dyn.m_displaystr;
     return TRUE;   
    }
	return FALSE;
}

/*
_sv_xfdl * ThreeCreateXFDLCom()
{
	HRESULT hr;
	HANDLE hMutex = NULL;
	div_t div_result;
	_sv_xfdl *myRef = NULL;
	int i=0;
	int nInstance =6;
	int nTemp=0;
	CString szMutex=_T("AIM4.0-NTPERFTEST-DLL-RUN");

	srand(1);	
	hMutex = ::CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, 10, szMutex);		
	if (hMutex == NULL) 
		return NULL;
	DWORD dwLastError =GetLastError();
	srand(1);
	for(int lv=0 ;lv<6 ;lv++)
	{	
		int* lpvFile = (int*)::MapViewOfFile(hMutex, FILE_MAP_WRITE, 0, 0, 0);
		nTemp = lpvFile[0];
		if (nTemp > (nInstance - 1))
		{
			Sleep(5000);
			continue;	
		}
		else 
		{
			lpvFile[0] = nTemp+1;
			::UnmapViewOfFile(lpvFile);
			break;
		}
	}
	if (nTemp > (nInstance - 1)) 
	{			
		goto Fin;
	}
	for ( i=0;i<6;i++)
	{
		 hr=CoInitialize(NULL);		 
		 if (SUCCEEDED(hr))
			 break;
		 div_result=div(rand() , 3);
		 Sleep((5+div_result.rem)*1000);		 
	}
	if (FAILED(hr))
	{
		CString str;
		CTime tnNow =CTime::GetCurrentTime();
		str.Format(" CoInitialize failed %s",TimeToString(tnNow) );
		AddToExceptionLog(str);			
		if (hMutex) 
		{
			if (nInstance > 1)
			{
				int* lpvFile = (int*)::MapViewOfFile(hMutex, FILE_MAP_WRITE, 0, 0, 0);
				int nTemp = lpvFile[0];
				lpvFile[0] = nTemp - 1;
				::UnmapViewOfFile(lpvFile);
			}

			CloseHandle(hMutex); 
			hMutex = NULL; 
		}
		return NULL;
	}
	for ( i=0;i<6;i++)
	{
		 hr=CoCreateInstance(CLSID_sv_xfdl,NULL,
							CLSCTX_ALL,
							IID__sv_xfdl,(void **)&myRef);
		 
		 if (SUCCEEDED(hr))
			 break;
		 div_result=div(rand() , 3);
		 Sleep((5+div_result.rem)*1000);
	}
	if (FAILED(hr)) 
	{
		CString str;
		CTime tnNow =CTime::GetCurrentTime();
		str.Format("use com failed %s",TimeToString(tnNow) );
		AddToExceptionLog(str);			
		myRef= NULL;
	}
Fin:
	if (hMutex) 
	{
		if (nInstance > 1) 
		{
			int* lpvFile = (int*)::MapViewOfFile(hMutex, FILE_MAP_WRITE, 0, 0, 0);
			int nTemp = lpvFile[0];
			lpvFile[0] = nTemp - 1;
			::UnmapViewOfFile(lpvFile);
		}
		CloseHandle(hMutex); 
		hMutex = NULL; 
	}
	return myRef;
}
*/
_dragonflow * ThreeCreateDragonCom()
{
	//-----------------------------------------------------------------------

	HRESULT hr;
	_dragonflow *myRef=NULL;
	div_t div_result;
	srand(1);
	int i=0;
	

		CString szMutex=_T("AIM4.0-NTPERFTEST-DLL-RUN");
		int nInstance =6;

		HANDLE hMutex = NULL;

		hMutex = ::CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, 10, szMutex);
			
		if (hMutex == NULL) 
			return NULL;

		DWORD dwLastError =GetLastError();

		
		int nTemp=0;
		


		srand(1);


		for(int lv=0 ;lv<6 ;lv++)
		{
		
			int* lpvFile = (int*)::MapViewOfFile(hMutex, FILE_MAP_WRITE, 0, 0, 0);

			nTemp = lpvFile[0];

			if (nTemp > (nInstance - 1)) {
				Sleep(5000);
				continue;	
			}
			else {
				lpvFile[0] = nTemp+1;
				::UnmapViewOfFile(lpvFile);
				break;
			}
		}
		if (nTemp > (nInstance - 1)) {			
			goto Fin;
		}





	for ( i=0;i<6;i++)
	{
		 hr=CoInitialize(NULL);
		 
		 if (SUCCEEDED(hr))
			 break;
		 div_result=div(rand() , 3);
		 Sleep((5+div_result.rem)*1000);

		 
	}

	if (FAILED(hr))
	{
		CString str;
		CTime tnNow =CTime::GetCurrentTime();
		str.Format(" CoInitialize failed %s",TimeToString(tnNow) );
		AddToExceptionLog(str);	
		
		if (hMutex) {

			if (nInstance > 1) {
				int* lpvFile = (int*)::MapViewOfFile(hMutex, FILE_MAP_WRITE, 0, 0, 0);
				int nTemp = lpvFile[0];
				lpvFile[0] = nTemp - 1;
				::UnmapViewOfFile(lpvFile);
			}

			CloseHandle(hMutex); 
			hMutex = NULL; 
		}

		return NULL;
	}




	for ( i=0;i<6;i++)
	{
		 hr=CoCreateInstance(CLSID_dragonflow,NULL,
							CLSCTX_ALL,
							IID__dragonflow,(void **)&myRef);
		 
		 if (SUCCEEDED(hr))
			 break;
		 div_result=div(rand() , 3);
		 Sleep((5+div_result.rem)*1000);
	}

	if (FAILED(hr)) 
	{

		CString str;
		CTime tnNow =CTime::GetCurrentTime();
		str.Format("use com failed %s",TimeToString(tnNow) );
		AddToExceptionLog(str);	

		
		myRef= NULL;
	}

Fin:

	if (hMutex) {
		if (nInstance > 1) {
			int* lpvFile = (int*)::MapViewOfFile(hMutex, FILE_MAP_WRITE, 0, 0, 0);
			int nTemp = lpvFile[0];
			lpvFile[0] = nTemp - 1;
			::UnmapViewOfFile(lpvFile);
		}

		CloseHandle(hMutex); 
		hMutex = NULL; 
	}

	return myRef;

}

int IfGetFromRegsitry(CStringList& lstParas)
{
	/*
	CString strFileName = _T("");
	CString strInstallPath = _T("");
	strInstallPath = FuncGetInstallPath();
	strFileName.Format("%s\\groups\\setting.config", strInstallPath);

	int nValidAlert = 0;
	nValidAlert = ::DFN_GetPrivateProfileInt("Alert", "_register", 0, strFileName);

	//return nValidAlert;
	return 1;
	*/
	CString strMachineName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");

	GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);
	int nVersion =GetVersionFromCom(strMachineName,strUserAccount,strPassWord);
	return 0;
	if (nVersion >=4) 
		return 0;
	else if(nVersion == 2)
		return nVersion;
	else return 1;
}

CString GetStringListIndex(int nIndex ,CStringList& strList)
{
	POSITION pos;
	pos=strList.GetHeadPosition();
	for(int i=0;i<nIndex;i++)
			strList.GetNext(pos);
	return strList.GetAt(pos);

}

///////////////////////////////////////////////////////////////////////////////
void GetErrorText(int nRetCode ,char * szRet);
CString executeExeFile(CString m_DosCommandLine);
/*
CString GetValueFromList(const CString strName,CStringList& strList)
{
	POSITION pos=NULL;
	pos=strList.GetHeadPosition();
	int nIndex=0;
	CString strItem=_T("");
	CString strValue=_T("");
	while(pos!=NULL)
	{
		strItem=strList.GetNext(pos);
		nIndex=strItem.Find("=");
		strValue=strItem.Left(nIndex);
		if (strValue==strName)
		{
			strValue=strItem.Right(strItem.GetLength()-nIndex-1);
			return strValue;
		}
	}
	return "";
	
}
*/
BOOL LoginMachine(CStringList& lstParas,CString &strMachineName )
{
	int nCount=0;
	nCount=lstParas.GetCount();
	CString strMachineType=_T("");
	
	CString strUserAccount=_T("");
	CString strPassWord=_T("");

	int nRet=-1;
	
	strMachineType=GetValueFromList("_MachineType",lstParas);
	
	
		//GetErrorText(nRet,szRet);
	if(strMachineType==_T(""))
		return FALSE;
	
	if (atoi(strMachineType)==0) //remote
	{
		strMachineName=GetValueFromList("_MachineName",lstParas);
		if (strMachineName=="")
			return FALSE;
		
		strUserAccount=GetValueFromList("_UserAccount",lstParas);
		//if (strUserAccount=="")			return FALSE;
		
		
		strPassWord=GetValueFromList("_PassWord",lstParas);

    	BOOL bReturn = FALSE;
		bReturn = LoginRemoteMachine(strMachineName.GetBuffer(strMachineName.GetLength()),
			strUserAccount.GetBuffer(strUserAccount.GetLength()),
			strPassWord.GetBuffer(strPassWord.GetLength())
			);

		if(!bReturn)
			return FALSE;		
	}
	return TRUE;
}
BOOL GetMachineInfo(CStringList& lstParas,CString &strMachineName,
					CString& strUserAccount,CString& strPassWord)
{
	CString strMachineType=_T("");
	strMachineType=GetValueFromList("_MachineType",lstParas);
	//if(strMachineType==_T(""))
	//	return FALSE;
	//if (atoi(strMachineType)==0) //remote
	{
		strMachineName=GetValueFromList("_MachineName",lstParas);
		if (strMachineName=="")
			return FALSE;
		
		strUserAccount=GetValueFromList("_UserAccount",lstParas);
		//if (strUserAccount=="")		return FALSE;
		
		
		strPassWord=GetValueFromList("_PassWord",lstParas);
		
	}
	if((strMachineName.CompareNoCase("localhost")==0)||
		(strMachineName.CompareNoCase("127.0.0.1")==0))
	{
		strMachineName="";
		strUserAccount="";
		strPassWord="";
	}
	return TRUE;
}
/////////////////////////////////////////////////////////////
///下面是所有接口
//磁盘参数
extern "C" _declspec(dllexport) BOOL GetDiskInfo(const char * strParas, char * szRet, int& nSize)
//(CStringList& lstParas, char* szRet)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CStringList lstParas;
	MakeStringListByChar(lstParas,strParas);


	int nFlag = 0;
	nFlag = IfGetFromRegsitry(lstParas);

	if(nFlag == 0)
	{
		CString strMachineName=_T("");
		CString strUserAccount=_T("");
		CString strPassWord=_T("");
		GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);
		CString m_DosCommandLine=_T("");
		CString strDisk=_T("");
		strDisk=GetValueFromList("_Disk",lstParas);
		if (strDisk=="")
		{
			sprintf(szRet,"error=%s",FuncGetStringFromIDS("SV_NT_SERVER",
                "NT_SERVER_INVALID_PARAMETER"));//<%IDS_NtPerfTest_01%>
			return FALSE;
		}
		/*
		m_DosCommandLine.Format("cscript   //B //nologo //T:15 disk.vbs \"%s\" \"%s\" \"%s\" \"%s\" ",
		strMachineName,strUserAccount,strPassWord,strDisk);
		CString strRet=_T("");
		strRet=executeExeFile(m_DosCommandLine);
		*/
		CString strRet=_T("");
		strRet=GetDiskInfobyCom(strMachineName,strUserAccount,strPassWord,strDisk);
		
		if (strRet=="")
		{
			//strRet="error=不能采集数据$";
			sprintf(szRet,"error=%s",FuncGetStringFromIDS("SV_NT_DISK",
                "NT_DISK_COLLET_DATA_FAILE"));//<%IDS_NtPerfTest_02%>
			return FALSE;
		}
		else{
			sprintf(szRet,"%s",strRet);
			CString strInput;
			strInput =szRet;
			MakeCharByString(szRet,nSize,strInput);
			return TRUE;
		}
	}
	else if(nFlag == 2)
	{
		sprintf(szRet,"error=%s",FuncGetStringFromIDS("SV_NT_DISK",
                "NT_DISK_COM_INIT_FAILED"));//<%IDS_NtPerfTest_41%>
		return FALSE;
	}
	else
	{
		CString strMachineName=_T("");
		CString strUserAccount=_T("");
		CString strPassWord=_T("");		
		BOOL ret=TRUE; 
		
		ret=LoginMachine(lstParas,strMachineName);
		if (ret==FALSE){
			GetErrorText(-1 , szRet);
			return ret;
		}
		
		int nRet=-1;
		CString strDisk=_T("");
		
		strDisk=GetValueFromList("_Disk",lstParas);
		if (strDisk=="")
			return FALSE;
		
		GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);

		int nVersion =GetVersionFromCom(strMachineName,strUserAccount,strPassWord);
		if (nVersion>=5)
		{
			if (!strMachineName.IsEmpty())	strMachineName="\\\\"+strMachineName;
			
			nRet=GetDiskInfo1(strMachineName.GetBuffer(strMachineName.GetLength()),  szRet
				,strDisk);
			
			if (nRet<0)
			{
				GetErrorText(nRet,szRet);
				return FALSE;
			}
			else	return TRUE;
		}else{
			CString strMachineType=GetValueFromList("_MachineType",lstParas);//atoi(strMachineType)
			typedef BOOL(GetDiskV3)(int ,CString,CString,CString,CString,char *);
			GetDiskV3 *pDisk= NULL;
			BOOL bRet=FALSE;
			HINSTANCE hInstance = ::LoadLibrary("Disk.dll");
			if(hInstance)
			{
				pDisk=(GetDiskV3 *)::GetProcAddress(hInstance,"GetDiskData");

				if(pDisk)
					bRet=(*pDisk)(atoi(strMachineType), strMachineName,strUserAccount
					,strPassWord,strDisk,szRet);
				else	
                    sprintf(szRet,"error=%s",FuncGetStringFromIDS("SV_BASIC",
                    "BASIC_GET_PROC_ADDRESS_FAILED"));//<%IDS_NtPerfTest_03%>

				::FreeLibrary(hInstance);
			}else{
				sprintf(szRet,"error=%s",FuncGetStringFromIDS("SV_BASIC",
                    "BASIC_LOAD_DLL_FAILED"));//<%IDS_NtPerfTest_04%>
			}

			return bRet;
		}
	}
}

extern "C" _declspec(dllexport) BOOL GetPhysicsMemoryInfo(const char * strParas, char * szRet, int& nSize)
//(CStringList& lstParas, char* szRet)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	int nFlag = 0;

	CStringList lstParas;
	MakeStringListByChar(lstParas,strParas);

	nFlag = IfGetFromRegsitry(lstParas);

	if(nFlag == 0)
	{
		CString strMachineName=_T("");
		CString strUserAccount=_T("");
		CString strPassWord=_T("");
		GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);
		
		CString strRet=_T("");
		strRet=GetPhysicsMemoryInfobyCom(strMachineName,strUserAccount,strPassWord);
		
		if (strRet=="")
		{
			//strRet="error=不能采集数据$";
			sprintf(szRet,"error=%s",FuncGetStringFromIDS("SV_NT_MEMORY",
                "NT_MEMORY_COLLET_DATA_FAILE"));//<%IDS_NtPerfTest_05%>
			return FALSE;
		}
		else{
			sprintf(szRet,"%s",strRet);
			CString strInput;
			strInput =szRet;
			MakeCharByString(szRet,nSize,strInput);
			return TRUE;
		}
	}
	else if(nFlag == 2)
	{
		sprintf(szRet,"error=%s",FuncGetStringFromIDS("SV_NT_MEMORY",
            "NT_MEMORY_COM_INIT_FAILED"));//<%IDS_NtPerfTest_42%>
		return FALSE;
	}
	else
	{
		CString strMachineName=_T("");
		
		BOOL ret=TRUE; 
		
		ret=LoginMachine(lstParas,strMachineName);
		if (ret==FALSE){
			GetErrorText(-1 , szRet);
			return ret;
		}
		
		int nRet=-1;
		
		if (!strMachineName.IsEmpty())	strMachineName="\\\\"+strMachineName;
		nRet=GetMemoryInfo1(strMachineName.GetBuffer(strMachineName.GetLength()), szRet);
		
		if (nRet<0)
		{
			GetErrorText(nRet,szRet);
			return FALSE;
		}
		else	return TRUE;
	}
}

extern "C" _declspec(dllexport) bool GetMemoryInfo(const char * strParas, char * szRet, int& nSize)
//(CStringList& lstParas, char* szRet)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	int nFlag = 0;
	CStringList lstParas;
	MakeStringListByChar(lstParas,strParas);

	nFlag = IfGetFromRegsitry(lstParas);

	if(nFlag == 0)
	{
		CString strMachineName=_T("");
		CString strUserAccount=_T("");
		CString strPassWord=_T("");
		GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);
		
		CString strRet=_T("");
		strRet=GetMemoryInfobyCom(strMachineName,strUserAccount,strPassWord);
		
		if (strRet=="")
		{
			//strRet="error=不能采集数据$";
			sprintf(szRet,"error=%s",FuncGetStringFromIDS("SV_NT_MEMORY",
                "NT_MEMORY_COLLET_DATA_FAILE"));//<%IDS_NtPerfTest_05%>
			return false;
		}
		else{
			sprintf(szRet,"%s",strRet);
			CString strInput;
			strInput =szRet;
			MakeCharByString(szRet,nSize,strInput);
			return true;
		}
	}
	else if(nFlag == 2)
	{
		sprintf(szRet,"error=%s",FuncGetStringFromIDS("SV_NT_MEMORY",
            "NT_MEMORY_COM_INIT_FAILED"));//<%IDS_NtPerfTest_42%>
		return false;
	}
	else
	{
		CString strMachineName=_T("");
		
		bool ret= true; 
		
		ret=LoginMachine(lstParas,strMachineName);
		if (!ret){
			GetErrorText(-1 , szRet);
			return ret;
		}
		
		int nRet=-1;
		
		if (!strMachineName.IsEmpty())	strMachineName="\\\\"+strMachineName;
		nRet=GetMemoryInfo1(strMachineName.GetBuffer(strMachineName.GetLength()), szRet);
		
		if (nRet<0)
		{
			GetErrorText(nRet,szRet);
			return false;
		}
		else	return true;
	}
}

extern "C" _declspec (dllexport) BOOL GetAllCPURateByScript(CStringList& lstParas, char* szRet)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	int nFlag = 0;
	nFlag = IfGetFromRegsitry(lstParas);
	
	CString strGroupID = _T(""), strMonitorID = _T("");
	strGroupID = GetValueFromList("_GroupID",lstParas);
	strMonitorID = GetValueFromList("_MonitorID",lstParas);
	CString strCustomer = _T("");
	strCustomer = GetValueFromList("_CustomerPath",lstParas);

	if(nFlag == 0)
	{
		CString strMachineName=_T("");
		CString strUserAccount=_T("");
		CString strPassWord=_T("");
		GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);
		
		CString strRet=_T("");
		strRet=GetCPUInfobyScript(strCustomer, strGroupID, strMonitorID, strMachineName,strUserAccount,strPassWord);
		
		if (strRet=="")
		{
			//strRet="error=不能采集数据$";
			sprintf(szRet,"error=%s",FuncGetStringFromIDS("SV_NT_CPU",
                "NT_CPU_COLLET_DATA_FAILED"));//<%IDS_NtPerfTest_06%>
			return FALSE;
		}
		else{
			sprintf(szRet,"%s",strRet);
			return TRUE;
		}
	}
	else
	{
		CString strMachineName=_T("");
		CString strUserAccount=_T("");
		CString strPassWord=_T("");
		
		BOOL ret=TRUE; 
		
		ret=LoginMachine(lstParas,strMachineName);
		if (ret==FALSE){
			GetErrorText(-1 , szRet);
			return ret;
		}
		
		int nRet=-1;
		GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);
		int nVersion =GetVersionFromCom(strMachineName,strUserAccount,strPassWord);
		nRet=GetCPURate(strMachineName.GetBuffer(strMachineName.GetLength()), szRet);//,nVersion);

		CTime MyTime;
		MyTime = CTime::GetCurrentTime();
		CString strCurrentDate = _T("");			
		strCurrentDate = MyTime.Format("%Y-%m-%d %H:%M:%S");
		CString strAllOutPut = _T("");
		strAllOutPut.Format("%s\t%s", strCurrentDate, szRet);
//		DebugInfo(strAllOutPut);

		if (nRet<0)
		{
			GetErrorText(nRet,szRet);
			return FALSE;
		}
		else	return TRUE;
	}
}

extern "C" _declspec (dllexport) BOOL GetSybaseInfo(CStringList& lstParas, char* szRet)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CString strDatabaseName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	
	strDatabaseName = GetValueFromList("_DsnName",lstParas);
	strUserAccount = GetValueFromList("_UserAccount",lstParas);	
	strPassWord = GetValueFromList("_PassWord",lstParas);	
	
	CString strRet=_T("");
	strRet=GetSybaseInfobyCom(strDatabaseName,strUserAccount,strPassWord);

	if (strRet=="")
	{
		//strRet="error=不能采集数据$";
		sprintf(szRet,"error=%s",FuncGetStringFromIDS("<%IDS_NtPerfTest_46%>"));//<%IDS_NtPerfTest_46%>
		return FALSE;
	}
	else{
		sprintf(szRet,"%s",strRet);
		return TRUE;
	}

}

extern "C" _declspec(dllexport) BOOL GetAllCPURate(const char * strParas, char * szRet, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	/*for debug zjw
	CStringList strList;
	MakeStringListByChar(strList, strParas);
	POSITION pos=NULL;
	pos=strList.GetHeadPosition();
	CString strItem = "";

	while(pos != NULL)
	{
		strItem=strList.GetNext(pos);
		strcpy(szRet, strItem);
		strcat(szRet, " for debug zjw");
		OutputDebugString(szRet);
	}
	 return true;
	*/

	CStringList lstParas ;
	MakeStringListByChar(lstParas,strParas);
	//printf("size is %d\n",lstParas.GetCount());

	int nFlag = 0;
	nFlag = IfGetFromRegsitry(lstParas);
	
	CString strGroupID = _T(""), strMonitorID = _T("");
	strGroupID = GetValueFromList("_GroupID",lstParas);
	strMonitorID = GetValueFromList("_MonitorID",lstParas);
	CString strCustomer = _T("");
	strCustomer = GetValueFromList("_CustomerPath",lstParas);

	if(nFlag == 0)
	{
		CString strMachineName=_T("");
		CString strUserAccount=_T("");
		CString strPassWord=_T("");
		GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);
		
		CString strRet=_T("");
		strRet=GetCPUInfobyCom(strMachineName,strUserAccount,strPassWord);

		if (strRet=="")
		{
			
			sprintf(szRet,"error=%s",FuncGetStringFromIDS("SV_NT_CPU",
                "NT_CPU_COLLECT_DATA_FAILED"));//<%IDS_NtPerfTest_06%>
			return FALSE;
		}
		else{
			sprintf(szRet,"%s",strRet);
			CString strInput;
			strInput =szRet;
			MakeCharByString(szRet,nSize,strInput);
			return TRUE;
		}
	}
	else if(nFlag == 2)
	{
		sprintf(szRet,"error=%s",FuncGetStringFromIDS("SV_NT_CPU",
            "NT_CPU_COM_INIT_FAILED"));//<%IDS_NtPerfTest_36%>
		return FALSE;
	}
	else
	{
		CString strMachineName=_T("");
		CString strUserAccount=_T("");
		CString strPassWord=_T("");
		
		BOOL ret=TRUE; 
		
		ret=LoginMachine(lstParas,strMachineName);
		if (ret==FALSE){
			GetErrorText(-1 , szRet);
			return ret;
		}
		
		int nRet=-1;
		GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);
//		int nVersion =GetVersionFromCom(strMachineName,strUserAccount,strPassWord);
		nRet=GetCPURate(strMachineName.GetBuffer(strMachineName.GetLength()), szRet);//,nVersion);

		CTime MyTime;
		MyTime = CTime::GetCurrentTime();
		CString strCurrentDate = _T("");			
		strCurrentDate = MyTime.Format("%Y-%m-%d %H:%M:%S");
		CString strAllOutPut = _T("");
		strAllOutPut.Format("%s\t%s", strCurrentDate, szRet);
//		DebugInfo(strAllOutPut);

		if (nRet<0)
		{
			GetErrorText(nRet,szRet);
			return FALSE;
		}
		else
			return TRUE;
	}
}
/////////////2003.3.10 13:49 by Eason/////////////////////////////////////////////////////////
extern "C" _declspec(dllexport) BOOL GetAspNetInfo(const char * strParas, char * szRet, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strMachineName=_T(""),
		    strUserAccount=_T(""),
			strPassWord=_T(""),
			strRet = _T("");
	CStringList lstParas ;
	MakeStringListByChar(lstParas,strParas);

	GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);
	strRet=GetAspNetInfoByCom(strMachineName,strUserAccount,strPassWord);
	if (strRet=="")
	{
		//strRet="error=不能采集数据$";
		sprintf(szRet,"error=%s",FuncGetStringFromIDS("<%NtPerfTest_06%>"));
		return FALSE;
	}
	else{
		sprintf(szRet,"%s",strRet);
		CString strInput;
		strInput =szRet;
		MakeCharByString(szRet,nSize,strInput);
		return TRUE;
	}


}
////////////2003.3.10 15:34 by Eason//////////////////////////////////////////////////////////////////////////////////
extern "C" _declspec(dllexport) BOOL GetAspNetApplicationInfo(CStringList& lstParas, char* szRet)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strMachineName=_T(""),
		    strUserAccount=_T(""),
			strPassWord=_T(""),
			strRet = _T("");
	GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);
	strRet=GetAspNetApplicationInfoByCom(strMachineName,strUserAccount,strPassWord);
	if (strRet=="")
	{
		//strRet="error=不能采集数据$";
		sprintf(szRet,"error=%s",FuncGetStringFromIDS("<%IDS_NtPerfTest_06%>"));
		return FALSE;
	}
	else{
		sprintf(szRet,"%s",strRet);
		return TRUE;
	}


}
//////////////////////////////////////////////////////////////////////////////////////////////

CString GetSQLServer7InfobyCom(CString strServer, CString strUser,CString strPwd)
{
	CoInitialize(NULL);
	_dragonflow *myRef=NULL;

	HRESULT hr =0;		
	myRef  =ThreeCreateDragonCom();
	if (NULL==myRef  )  hr=-200;

	if (SUCCEEDED(hr))
	{

		VARIANT  a,b,c,vResult;
		BSTR bstrTmp;
		VariantInit(&a);
		VariantInit(&b);
		VariantInit(&c);
		VariantInit(&vResult);
		
		bstrTmp=strServer.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;
		
		bstrTmp=strUser.AllocSysString();
		b.bstrVal=bstrTmp;
		b.vt=VT_BSTR;

		bstrTmp=strPwd.AllocSysString();
		c.bstrVal=bstrTmp;
		c.vt=VT_BSTR;

		myRef->getSQLServer7(&a,&b,&c, &vResult);

		CString strResult(vResult.bstrVal);

		myRef->Release();
		myRef = NULL;


		CoUninitialize();
		return strResult;
	}

	CoUninitialize();
	return FuncGetStringFromIDS("<%IDS_NtPerfTest_43%>");//<%IDS_NtPerfTest_43%>
}

extern "C" __declspec(dllexport) BOOL GetSQLServer7Info(CStringList& lstParas, char* szRet)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strMachineName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);
	CString strRet=_T("");
	strRet=GetSQLServer7InfobyCom(strMachineName,strUserAccount,strPassWord);

	if (strRet=="")
	{
		//strRet="error=不能采集数据$";
		sprintf(szRet,"error=%s",FuncGetStringFromIDS("<%IDS_NtPerfTest_44%>"));//<%IDS_NtPerfTest_44%>
		return FALSE;
	}
	else{
		sprintf(szRet,"%s",strRet);
		return TRUE;
	}
}

extern "C" _declspec(dllexport) BOOL GetServiceInfo(const char * strParas, char * szRet, int& nSize)
//(CStringList& lstParas, char* szRet)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CStringList lstParas;
	MakeStringListByChar(lstParas,strParas);
	int nFlag = 0;
	nFlag = IfGetFromRegsitry(lstParas);

	if(nFlag == 0)
	{
		CString strMachineName=_T("");
		CString strUserAccount=_T("");
		CString strPassWord=_T("");
		GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);
		CString m_DosCommandLine=_T("");
		CString strDisk=_T("");
		strDisk=GetValueFromList("_Service",lstParas);
		if (strDisk=="")
		{
			sprintf(szRet,"error=%s",FuncGetStringFromIDS("SV_NT_SERVICE","NT_SERVICE_NAME_NULL"));//<%IDS_NtPerfTest_07%>
			return FALSE;
		}
		CString strRet=_T("");
		strRet=GetServiceInfobyCom(strMachineName,strUserAccount,strPassWord,strDisk);
		
		if (strRet=="")
		{
			//strRet="error=不能采集数据$";
			sprintf(szRet,"error=%s",FuncGetStringFromIDS("SV_NT_SERVICE",
                "NT_SERVICE_COLLECT_DATA_FAILED"));//<%IDS_NtPerfTest_08%>
			return FALSE;
		}
		else{
			sprintf(szRet,"%s",strRet);
			CString strInput;
			strInput =szRet;
			MakeCharByString(szRet,nSize,strInput);

			return TRUE;
		}
	}
	else if(nFlag == 2)
	{
		sprintf(szRet,"error=%s",FuncGetStringFromIDS("SV_NT_SERVICE",
            "NT_SERVICE_COM_INIT_FAILED"));//<%IDS_NtPerfTest_37%>
		return FALSE;
	}
	else
	{
		CString strMachineName=_T("");
		BOOL ret=TRUE; 
		ret=LoginMachine(lstParas,strMachineName);
		if (ret==FALSE){
			GetErrorText(-1 , szRet);
			return ret;
		}
		
		int nRet=-1;
		
		CString strDisk=_T("");
		
		strDisk=GetValueFromList("_Service",lstParas);
		if (strDisk=="")
			return FALSE;
		
		if (!strMachineName.IsEmpty())	strMachineName="\\\\"+strMachineName;
		nRet=GetServiceInfo1(strMachineName.GetBuffer(strMachineName.GetLength()), 
			strDisk.GetBuffer(strDisk.GetLength())
			,szRet);
		
		if (nRet<0)
		{
			GetErrorText(nRet,szRet);
			return FALSE;
		}
		else	return TRUE;
	}
}


void GetErrorText(int nRetCode ,char * szRet)
{
	switch(nRetCode)
	{
		case -1:
			sprintf(szRet,"error=%s",FuncGetStringFromIDS("SV_NT_SERVER",
                "NT_SERVER_INVALID_PARAMETER"));//<%IDS_NtPerfTest_09%>
			break;
		case -2:
			sprintf(szRet,"error=%s",FuncGetStringFromIDS("SV_NT_SERVER",
                "NT_SERVER_READ_DATA_FAILED"));//<%IDS_NtPerfTest_10%>
			break;
		case -3:
			sprintf(szRet,"error=%s",FuncGetStringFromIDS("SV_NT_SERVER",
                "NT_SERVER_CONNECT_REG_FAILED"));//<%IDS_NtPerfTest_11%>
			break;
		case -4:
			sprintf(szRet,"error=%s",FuncGetStringFromIDS("SV_NT_SERVER",
                "NT_SERVER_READ_COUNTER_FAILED"));//<%IDS_NtPerfTest_12%>
			break;
		case -5:
			sprintf(szRet,"error=%s",FuncGetStringFromIDS("SV_NT_SERVER",
                "NT_SERVER_READ_DATA_FAILED"));//<%IDS_NtPerfTest_13%>
			break;
		case -6:
			sprintf(szRet,"error=%s",FuncGetStringFromIDS("SV_NT_SERVER",
                "NT_SERVER_READ_DATA_FAILED"));//<%IDS_NtPerfTest_14%>
			break;
		case -7:
			sprintf(szRet,"error=%s",FuncGetStringFromIDS("SV_NT_SERVER",
                "NT_SERVER_COUNTER_NOT_STARTERUP"));//<%IDS_NtPerfTest_15%>
			break;

	}
}

///////////////////////////////////////////////////////////
//enume all device
//////////////////////////////////////////////////////////
extern "C" _declspec(dllexport) int GetWebServerInfo(const char * strParas, char * szRet, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	int nFlag = 0;
	CStringList lstParas ;
	MakeStringListByChar(lstParas,strParas);
	nFlag = IfGetFromRegsitry(lstParas);

	if(nFlag == 0)
	{
		CString strMachineName=_T("");
		CString strUserAccount=_T("");
		CString strPassWord=_T("");
		

		GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);
		
		CString strRet=_T("");
		strRet=GetWebInfobyCom(strMachineName,strUserAccount,strPassWord);
		
		if (strRet=="")
		{
			//strRet="error=不能采集数据$";
			sprintf(szRet,FuncGetStringFromIDS("SV_NT_SERVICE",
                "NT_SERVICE_COLLET_DATA_FAILED"));//<%IDS_NtPerfTest_16%>
			return FALSE;
		}
		else{
			sprintf(szRet,"%s",strRet);
			CString strInput;
			strInput =szRet;
			MakeCharByString(szRet,nSize,strInput);
			return TRUE;
		}
	}
	else if(nFlag == 2)
	{
		sprintf(szRet,"error=%s",FuncGetStringFromIDS("SV_NT_SERVICE",
            "NT_SERVICE_COM_INIT_FAILED"));//<%IDS_NtPerfTest_38%>
		return FALSE;
	}
	else
	{
		CString strMachineName=_T("");
		BOOL ret=TRUE; 
		//连接服务器
		ret=LoginMachine(lstParas,strMachineName);
		if (ret==FALSE){
			GetErrorText(-1 , szRet);
			return ret;
		}
		int nRet=-1;
		if (!strMachineName.IsEmpty())	strMachineName="\\\\"+strMachineName;
		//get iis info
		nRet=GetWebServerInfo1(strMachineName.GetBuffer(strMachineName.GetLength()), szRet);
		if (nRet<0)
		{
			GetErrorText(nRet,szRet);
			return FALSE;
		}
		else	return TRUE;
	}
}
extern "C" _declspec(dllexport) int GetIISInfo(const char * strParas, char * szRet, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	int nFlag = 0;
	CStringList lstParas ;
	MakeStringListByChar(lstParas,strParas);
	nFlag = IfGetFromRegsitry(lstParas);

	if(nFlag == 0)
	{
		CString strMachineName=_T("");
		CString strUserAccount=_T("");
		CString strPassWord=_T("");
		GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);
		
		CString strRet=_T("");
		strRet=GetIISInfobyCom(strMachineName,strUserAccount,strPassWord);
		
		if (strRet=="")
		{
			//strRet="error=不能采集数据$";
			sprintf(szRet,"error=%s",FuncGetStringFromIDS("<%IDS_NtPerfTest_17%>"));//<%IDS_NtPerfTest_17%>
			return FALSE;
		}
		else{
			sprintf(szRet,"%s",strRet);
			CString strInput;
			strInput =szRet;
			MakeCharByString(szRet,nSize,strInput);
			return TRUE;
		}
	}
	else if(nFlag == 2)
	{
		sprintf(szRet,"error=%s",FuncGetStringFromIDS("<%IDS_NtPerfTest_39%>"));//<%IDS_NtPerfTest_39%>
		return FALSE;
	}
	else
	{
		CString strMachineName=_T("");
		BOOL ret=TRUE; 
		//连接服务器
		ret=LoginMachine(lstParas,strMachineName);
		if (ret==FALSE){
			GetErrorText(-1 , szRet);
			return ret;
		}
		int nRet=-1;
		if (!strMachineName.IsEmpty())	strMachineName="\\\\"+strMachineName;
		//get iis info
		nRet=GetIISInfo1(strMachineName.GetBuffer(strMachineName.GetLength()), szRet);
		if (nRet<0)
		{
			GetErrorText(nRet,szRet);
			return FALSE;
		}
		else	return TRUE;
	}
}
BOOL CreateServicesList(CString strSource, CStringList& lstText, CStringList& lstValue)
{
	int nEmail=FuncGetCountByMark( strSource,"$");
	CString strOneSrv=_T("");
	CString strSrvValue=_T("");
	int nIndex=-1;
	for(int i = 1; i <= nEmail; i++)
	{
		strOneSrv=FuncGetLeftStringByMark(strSource,"$",i);
		nIndex=strOneSrv.Find("#");
		if (nIndex>0) 
		{
			strSrvValue=strOneSrv.Left(nIndex);
			lstValue.AddTail(strSrvValue);
			strSrvValue=strOneSrv.Right(strOneSrv.GetLength()-nIndex-1);
			lstText.AddTail(strSrvValue);
		}

		//strDesList.AddTail( );
	}
	return TRUE;
}

extern "C" _declspec(dllexport) int GetNTServices(const char * strParas, char * strReturn, int & nSize)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    
    
    std::list<string> liInput;
    std::list<string> liOut;

     CStringList lstParas;

	 MakeStringListByChar(lstParas,strParas);
   
    CString strMachineName=_T("");
    CString strUserAccount=_T("");
    CString strPassWord=_T("");
    GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);

    CString strRet=_T("");
    strRet = GetServicesbyCom(strMachineName,strUserAccount,strPassWord);
    strRet.Replace('#','=');

    if (strRet=="")
    {
        return false;
    }
    else 
    {
		return  MakeCharByString(strReturn,nSize,strRet);
    }

    return true;

}





///////////////////////////////////////////////
//列举服务器所有逻辑磁盘名称

extern "C" _declspec(dllexport) bool  EnumDisksInfo(const char * strParas, char * strReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());


	std::string strInput;
	std::list<string> liInput;
	std::list<string> liOut;

	CStringList lstParas;

	MakeStringListByChar(lstParas,strParas);

	CString strMachineName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);
	
	CString strRet=_T("");
	strRet=GetAllDiskbyCom(strMachineName,strUserAccount,strPassWord);
	
	if (strRet=="")
	{
		return false;
	}
	else 
	{
		strcpy(strReturn,strRet);
		CString strOutRet;
		strOutRet = strReturn;
		nSize = 2048;
		MakeCharByString(strReturn,nSize,strOutRet);	
		MakeStringListByChar(liOut,strReturn);
	
		CString strOut = "";
		for(std::list<string>::iterator It=liOut.begin();It!=liOut.end();It++)
		{
			strOut+= It->c_str();
			strOut+= "=";
			strOut+= It->c_str();
			strOut+= "$";
		}
		nSize = 2048;
		MakeCharByString(strReturn,nSize,strOut);
	}
	
	return true;
	
}

extern "C" _declspec(dllexport) BOOL  EnumProcessInfo(const char * strParas, char * strReturn, int& nSize)
//(CStringList& lstParas, CStringList& lstText, CStringList& lstValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CStringList lstParas;

	MakeStringListByChar(lstParas,strParas);

	CString strMachineName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);
	CString strRet=_T("");
	strRet=GetAllProcessbyCom(strMachineName,strUserAccount,strPassWord);

	if (strRet=="")
	{
	}
	else {
		/*
		CreateListByStringandMark(strRet,lstValue,"?");
		//because process no name
		POSITION pos=NULL;
		pos=lstValue.GetHeadPosition();
		while(pos!=NULL)
		{
			lstText.AddTail(lstValue.GetNext(pos));
		}*/

//		std::list<string> liOut;

		//去掉名称(#号前面的部分)相同的进程(名称相同的进程中的第一个留下，其余的去掉)
		sprintf(strReturn,"%s",strRet.GetBuffer(strRet.GetLength()));
		char *ptr;
		ptr=strReturn;

		std::list<string> valueList;
		std::list<string>::iterator valueItem;
		
		string strTemp="";
		while(*ptr!='\0')
		{		
			if(*ptr == '?')
			{	
				valueList.push_back(strTemp);
				strTemp="";
			}
			else
			{
				if(*ptr == '\0')
				{
					break;
				}
				strTemp += *ptr;
			}
			ptr++;
		}

		std::list<string> valueList1;
		std::list<string>::iterator valueItem1;
		string strTemp1, strTemp2, strTemp3;
		bool bExist = false;
		for(valueItem=valueList.begin();valueItem!=valueList.end();valueItem++)
		{
			strTemp = *valueItem;
			size_t pos = strTemp.find("#") - 1;
			if(pos != string::npos)
			{
				strTemp1 = strTemp.substr(0, pos);
			}
			bExist = false;
			if(valueList1.size() > 0)
			{
				for(valueItem1=valueList1.begin();valueItem1!=valueList1.end();valueItem1++)
				{
                    strTemp2 = *valueItem1;
					pos = strTemp2.find("#") - 1;
					if(pos != string::npos)
					{
						strTemp3 = strTemp2.substr(0, pos);
					}
					if(strTemp1 == strTemp3)
					{
						bExist = true;
						break;
					}
				}
				strTemp1 = "";
				strTemp2 = "";
				strTemp3 = "";
			}
			if(bExist == false)
			{
				valueList1.push_back(strTemp);
			}
			strTemp = "";
		}

		strRet = "";

		for(valueItem1=valueList1.begin();valueItem1!=valueList1.end();valueItem1++)
		{
            strTemp = *valueItem1;
			strRet += strTemp.c_str();
			strRet += "?";
		}

		valueList.clear();
		valueList1.clear();

		strRet.Replace("#","=");
		strRet.Replace("?","\v");
	
		int lsize= strRet.GetLength();

		if(nSize>lsize)
		{
			sprintf(strReturn,"%s",strRet.GetBuffer(strRet.GetLength()));
			char *p;
			p=strReturn;
			while(*p!='\0')
			{
				if(*p=='\v') *p='\0';
				p++;
			
			}
			nSize =lsize;
			return true;

		}else return false;


	}

	return TRUE;
	
}

extern "C" _declspec(dllexport) BOOL FilterEventLogInfo(const char * strParas, char * szRet, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strMachineName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	CStringList lstParas ;
	MakeStringListByChar(lstParas,strParas);

	GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);

    CString strLogName = _T(""), strCodeList = _T(""), strSourceList = _T(""),
        strEventMachine = _T(""), strGroupID = _T(""), strMonitorID = _T(""),
        strCustomer = _T(""), strType = _T(""),strOnce=_T("");
    
	strLogName = GetValueFromList("_logName",lstParas);
	strType = GetValueFromList("_eventType",lstParas);
	strCodeList = GetValueFromList("_codeList",lstParas);
	strSourceList = GetValueFromList("_sourceList",lstParas);
	strEventMachine = GetValueFromList("_eventMachine",lstParas);
	strGroupID = GetValueFromList("_GroupID",lstParas);
	strMonitorID = GetValueFromList("_MonitorID",lstParas);
	strCustomer = GetValueFromList("_CustomerPath",lstParas);
	strOnce  = GetValueFromList("_monitorcondition",lstParas);

    CString strRet = FilterNTEventLogByCom(strMachineName,strUserAccount,strPassWord,
        strLogName,strType,strCodeList,strSourceList,strEventMachine,
        strCustomer,strGroupID,strMonitorID);
	
    int nType = 1;
	if (strRet=="")
	{
		//sprintf(szRet,"error=%s",FuncGetStringFromIDS("<%IDS_NtPerfTest_18%>"));//<%IDS_NtPerfTest_18%>

		sprintf(szRet,"error=%s",FuncGetStringFromIDS("SV_NT_SERVER",
                "NT_SERVER_READ_DATA_FAILED"));
		return FALSE;
	}
	else
    {
		//检查之前是否报错事件
		char buffer [256] = {0};
		char tmp [256] = {0};
		int nOnce;
		int nState;  //  for new logic   stop monitor as soon as error state
		std::string strDyn;
		sprintf(buffer, "logfile.ini", FuncGetInstallRootPath());


		nOnce =atoi(strOnce.GetBuffer(strOnce.GetLength()));
		
		BOOL bError; 
		if(nOnce==AlAWYSMONITOR)
		bError =FALSE;
		else
		{
			GetDyn(strMonitorID.GetBuffer(strMonitorID.GetLength()),nState,strDyn);
			if( (nState==dyn_error )||(nState==dyn_bad) )
				bError =TRUE;
			else bError =FALSE;
			
		}

		//BOOL bChange = DFN_GetPrivateProfileInt(strMonitorID, "needconfirm", FALSE, buffer);

		//解析出过滤后剩余事件数和检查的事件总数
		int nFilterCount = 0;
		int nCheckCount = 0;
		int nBeginIndex = 0;
		int nEndIndex = 0;
		nBeginIndex = strRet.Find("checkcount", 0);
		nEndIndex = strRet.Find("$", 0);
		CString strFilterCount = _T("");
		CString strCheckCount = _T("");
		strCheckCount = strRet.Mid(nBeginIndex+strlen("checkcount="), nEndIndex-nBeginIndex-strlen("checkcount="));
		nCheckCount = atoi(strCheckCount.GetBuffer(strCheckCount.GetLength()));
		nBeginIndex = strRet.Find("filtercount");
		nEndIndex = strRet.Find("$", nBeginIndex+1);
		strFilterCount = strRet.Mid(nBeginIndex+strlen("filtercount="), nEndIndex-nBeginIndex-strlen("filtercount="));
		nFilterCount = atoi(strFilterCount.GetBuffer(strFilterCount.GetLength()));
		
		//wangpeng 
		//bChange=FALSE;
		if (bError)
		{
			sprintf(szRet,"error=%s",strDyn.c_str());

			/*int nTmp = DFN_GetPrivateProfileInt(strMonitorID, "FilterCount", 0, buffer);
			nFilterCount += nTmp;
			nTmp = DFN_GetPrivateProfileInt(strMonitorID, "CheckCount", 0, buffer);
			nCheckCount += nTmp;
			sprintf(szRet, "checkcount=%d$filtercount=%d$", nCheckCount, nFilterCount);
			CString strInput;
			strInput =szRet;
			MakeCharByString(szRet,nSize,strInput);*/
		}
		else
		{
			//if (nFilterCount>0)
			//{
			//	//char buffer [256] = {0};
			//	char tmp [256] = {0};
			//	

			//	DFN_WritePrivateProfileString(strMonitorID, "needconfirm", _ltoa(TRUE, tmp, 10), buffer);
			//	DFN_WritePrivateProfileString(strMonitorID, "FilterCount", _ltoa(nFilterCount, tmp, 10), buffer);
			//	DFN_WritePrivateProfileString(strMonitorID, "CheckCount", _ltoa(nCheckCount, tmp, 10), buffer);
			//}
			sprintf(szRet,"%s",strRet);
			CString strInput;
			strInput =szRet;
			MakeCharByString(szRet,nSize,strInput);
		}
		return TRUE;
	}
}

extern "C" _declspec(dllexport) BOOL  GetNTEventLogInfo(const char * strParas, char * szRet, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CStringList lstParas ;
	MakeStringListByChar(lstParas,strParas);
	CString strMachineName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);

	CString strLogName=_T(""), strEventType=_T(""), strAlert=_T(""), strEventKey=_T(""),  strNotEventKey=_T(""), strPosMatch=_T(""), strNegMatch=_T(""), strEventCategory=_T(""), strEventMachine=_T(""), strInterval=_T("");
    POSITION pos = lstParas.GetHeadPosition();
    while(pos)
    {
        CString strTemp = lstParas.GetNext(pos);
        printf("List Item is:%s\n", strTemp);
    }
	strLogName = GetValueFromList("_logName",lstParas);
	strEventType = GetValueFromList("_eventType",lstParas);
	//strAlert = GetValueFromList("_alerting",lstParas);	
	strEventKey = GetValueFromList("_eventKey",lstParas);
	//strNotEventKey = GetValueFromList("_notMatchEventKey",lstParas);
	strPosMatch = GetValueFromList("_positiveMatch",lstParas);
	strNegMatch = GetValueFromList("_negativeMatch",lstParas);
	//strEventCategory = GetValueFromList("_eventCategory",lstParas);
	strEventMachine = GetValueFromList("_eventMachine",lstParas);
	strInterval = GetValueFromList("_interval",lstParas);

	CString strGroupID = _T(""), strMonitorID = _T("");
	strGroupID = GetValueFromList("_GroupID",lstParas);
	strMonitorID = GetValueFromList("_MonitorID",lstParas);

	CString strCustomer = _T("");
	strCustomer = GetValueFromList("_CustomerPath",lstParas);

	CString strRet=_T("");

	strRet = GetNTEventLogInfobyCom(strMachineName,strUserAccount,strPassWord, strLogName, strEventType, strAlert, strEventKey, strNotEventKey, strPosMatch, strNegMatch, strEventCategory, strEventMachine, "", strGroupID, strMonitorID, "");//strIntervalstr,Customer
	
	if (strRet=="")
	{
		sprintf(szRet,"error=%s",FuncGetStringFromIDS("<%IDS_NtPerfTest_18%>"));//<%IDS_NtPerfTest_18%>
		return FALSE;
	}
	else{
		sprintf(szRet,"%s",strRet);
		CString strInput;
		strInput =szRet;
		MakeCharByString(szRet,nSize,strInput);
		return TRUE;
	}
}

extern "C" __declspec(dllexport) BOOL  GetExchangeInfo(const char * strParas, char * szRet, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strMachineName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	CStringList lstParas ;
	MakeStringListByChar(lstParas,strParas);
	GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);

	strMachineName=GetValueFromList("_Server",lstParas);

	CString strRet=_T("");

	strRet = GetExchangeInfobyCom(strMachineName,strUserAccount,strPassWord);

	if (strRet=="")
	{
		sprintf(szRet,"error=%s",FuncGetStringFromIDS("<%IDS_NtPerfTest_19%>"));//<%IDS_NtPerfTest_19%>
		return FALSE;
	}
	else{
		sprintf(szRet,"%s",strRet);
		CString strInput;
		strInput =szRet;
		MakeCharByString(szRet,nSize,strInput);
		return TRUE;
	}
}

extern "C" _declspec(dllexport) BOOL  GetASPInfo(CStringList& lstParas,char * szRet )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strMachineName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);

	CString strRet=_T("");

	strRet = GetASPInfobyCom(strMachineName,strUserAccount,strPassWord);

	if (strRet=="")
	{
		sprintf(szRet,"error=%s",FuncGetStringFromIDS("<%IDS_NtPerfTest_20%>"));//<%IDS_NtPerfTest_20%>
		return FALSE;
	}
	else{
		sprintf(szRet,"%s",strRet);
		return TRUE;
	}	
}

extern "C" _declspec(dllexport) BOOL  GetSQLServerInfo(CStringList& lstParas,char * szRet )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strMachineName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);

	CString strRet=_T("");

	strRet = GetSQLServerInfobyCom(strMachineName,strUserAccount,strPassWord);

	if (strRet=="")
	{
		sprintf(szRet,"error=%s",FuncGetStringFromIDS("<%IDS_NtPerfTest_21%>"));//<%IDS_NtPerfTest_21%>
		return FALSE;
	}
	else{
		sprintf(szRet,"%s",strRet);
		return TRUE;
	}	
}

extern "C" _declspec(dllexport) BOOL  GetProcessInfo(const char * strParas, char * szRet, int& nSize)
//(CStringList& lstParas,char * szRet )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strMachineName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");

	CStringList lstParas;
	MakeStringListByChar(lstParas,strParas);

	GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);
	CString m_DosCommandLine=_T("");
	CString strDisk=_T("");
	strDisk=GetValueFromList("_monitorProcessList",lstParas);
	if (strDisk=="")
	{
		sprintf(szRet,"error=%s",FuncGetStringFromIDS("<%IDS_NtPerfTest_22%>"));//<%IDS_NtPerfTest_22%>
		return FALSE;
	}
    CString strTemp = strDisk;
    strDisk.MakeLower();
    int nIndex = strDisk.ReverseFind('.');
    if(nIndex >-1)
    {
        if(strDisk.Right(3) == "exe")
            strDisk = strTemp.Left(nIndex);
    }

	CString strRet=_T("");
	strRet=GetProcessInfobyCom(strMachineName,strUserAccount,strPassWord,strDisk);

	if (strRet=="")
	{
		sprintf(szRet,"error=%s",FuncGetStringFromIDS("<%IDS_NtPerfTest_23%>"));
		//<%IDS_NtPerfTest_23%>
		return FALSE;
	}
	else{
		sprintf(szRet,"%s",strRet);
		CString strInput;
		strInput =szRet;
		MakeCharByString(szRet,nSize,strInput);

		return TRUE;
	}

}
extern "C" _declspec(dllexport) BOOL  EnumAllNetWork(const char * strParas, char * strReturn, int& nSize)
//(CStringList& lstParas, CStringList& lstText, CStringList& lstValue)
{
	CStringList lstText;
	CStringList lstValue;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CStringList lstParas;
	MakeStringListByChar(lstParas,strParas);


	int nFlag = 0;
	nFlag = IfGetFromRegsitry(lstParas);

	if(nFlag == 0)
	{
		CString strMachineName=_T("");
		CString strUserAccount=_T("");
		CString strPassWord=_T("");
		GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);
		CString strRet=_T("");
		strRet=GetAllNetWorkbyCom(strMachineName,strUserAccount,strPassWord);
		
		if (strRet=="")
		{
		}
		else {
			strRet.Replace("#","=");
			strRet.Replace("?","\v");

			sprintf(strReturn,strRet.GetBuffer(strRet.GetLength()));
			int lsize= strRet.GetLength();

			if(nSize>lsize)
			{
				sprintf(strReturn,"%s",strRet.GetBuffer(strRet.GetLength()));
				char *p;
				p=strReturn;
				while(*p!='\0')
				{
					if(*p=='\v') *p='\0';
					p++;
				
				}
				nSize =lsize;
				return true;

			}else return false;			
		}
		
		return TRUE;
	}
	else if(nFlag == 2)
	{
		return FALSE;
	}
	else
	{
		CString strMachineName=_T("");
		BOOL ret=TRUE; 
		ret=LoginMachine(lstParas,strMachineName);
		if (ret==FALSE){
			return ret;
		}
		int nRet=-1;
		if (!strMachineName.IsEmpty())	strMachineName="\\\\"+strMachineName;
		nRet=EnumAllNetWork1(strMachineName.GetBuffer(strMachineName.GetLength()),lstText,lstValue);
		if (nRet<0)		return FALSE;
		else	return TRUE;
	}
}

extern "C" _declspec(dllexport) BOOL  GetNetWorkInfo(CStringList& lstParas,char * szRet )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	int nFlag = 0;
	nFlag = IfGetFromRegsitry(lstParas);


//	printf("ddddddddddddddddddddddddddddddddddddddd\n!");

//	FILE*pf=fopen("c:\\testtt.txt","a+");
//	fputs("hello",pf);

	if(nFlag == 0)
	{
        POSITION pos=lstParas.GetHeadPosition();
	    CString strItem=_T("");
	    while(pos!=NULL)
	    {
		    strItem=lstParas.GetNext(pos);
		    printf("List Item:%s\n", strItem);
	    }
		CString strMachineName=_T("");
		CString strUserAccount=_T("");
		CString strPassWord=_T("");
		GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);
		CString m_DosCommandLine=_T("");
		CString strDisk=_T("");
		strDisk=GetValueFromList("_NetWorkName",lstParas);
		if (strDisk=="")
		{
			sprintf(szRet,"error=%s",FuncGetStringFromIDS("<%IDS_NtPerfTest_24%>"));
			//<%IDS_NtPerfTest_24%>
			return FALSE;
		}

		//printf("Network Card:%s\n", strDisk);
        //printf("Machine is:%s\n", strMachineName);
        //printf("User is:%s\n", strUserAccount);
		CString strRet=_T("");
		strRet=GetNetWorkInfobyCom(strMachineName,strUserAccount,strPassWord,strDisk);
		if (strRet=="")
		{
			sprintf(szRet,"error=%s",FuncGetStringFromIDS("<%IDS_NtPerfTest_25%>"));
			//<%IDS_NtPerfTest_25%>
			return FALSE;
		}
		else{
			sprintf(szRet,"%s",strRet);

/*			fputs("ver=0\n",pf);
			fputs(szRet,pf);
			fclose(pf);*/
			return TRUE;
		}
	}
	else if(nFlag == 2)
	{
		sprintf(szRet,"error=%s",FuncGetStringFromIDS("<%IDS_NtPerfTest_40%>"));//<%IDS_NtPerfTest_40%>
		return FALSE;
	}
	else
	{
		CString strMachineName=_T("");
		CString strUserAccount=_T("");
		CString strPassWord=_T("");

		BOOL ret=TRUE; 
		ret=LoginMachine(lstParas,strMachineName);
		if (ret==FALSE){
			GetErrorText(-1 , szRet);
			return ret;
		}
		
		int nRet=-1;
		CString strNetWork=_T("");
		
		strNetWork=GetValueFromList("_NetWorkName",lstParas);
		if (strNetWork=="")
			return FALSE;
		
		if (!strMachineName.IsEmpty())	strMachineName="\\\\"+strMachineName;
		
		GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);

		nRet=GetNetWorkInfo1(strMachineName.GetBuffer(strMachineName.GetLength()), 
				szRet,strNetWork);

/*			fputs("ver=4\n",pf);
			fputs(szRet,pf);
			fclose(pf);*/


		
		if (nRet<0)
		{
			GetErrorText(nRet,szRet);
			return FALSE;
		}
		else	return TRUE;
	}

}

CString executeExeFile(CString m_DosCommandLine)
{
	CString strContent = _T("");
	
	HANDLE hOutputRead, hOutputWrite;
	
	SECURITY_ATTRIBUTES sa;
	
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;
	
	if (CreatePipe(&hOutputRead, &hOutputWrite, &sa, 0)/* && CreatePipe(&hErrorRead, &hErrorWrite, &sa, 0) */) {
		STARTUPINFO si;
		memset(&si, 0, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESTDHANDLES;
		si.hStdOutput = hOutputWrite;
		si.hStdError = hOutputWrite;
		
		PROCESS_INFORMATION pi;
		memset(&pi, 0, sizeof(PROCESS_INFORMATION));
		//puts((LPCTSTR)m_DosCommandLine);
		if (CreateProcess(NULL, m_DosCommandLine.GetBuffer(m_DosCommandLine.GetLength()),  NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
			char buffer[1024] = {0};
			DWORD bytesRead = 0;
			
			DWORD dwRet = 0;
			dwRet = WaitForSingleObject(pi.hProcess, 30000);// INFINITE
			if(dwRet == WAIT_OBJECT_0)
			{	
				if (ReadFile(hOutputRead, buffer, sizeof(buffer), &bytesRead, NULL))
					strContent = buffer;
			}
			else if(dwRet == WAIT_TIMEOUT)
			{
				strContent = "error=timeout$";
				::TerminateProcess(pi.hProcess, -1);
			}
			else
			{
				strContent = "error=unknown$";
			}
			
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
		
		CloseHandle(hOutputRead);
		CloseHandle(hOutputWrite);
	}
	
	return strContent;
}


CString GetDiskInfobyCom(CString strServer, CString strUser,CString strPwd, CString strDisk)
{
		CoInitialize(NULL);
	_dragonflow *myRef=NULL;

	HRESULT hr =0;		
	myRef  =ThreeCreateDragonCom();
	if (NULL==myRef  )  hr=-200;

	if (SUCCEEDED(hr))
	{

		VARIANT  a,b,c,d,vResult;
		BSTR bstrTmp;
		VariantInit(&a);
		VariantInit(&b);
		VariantInit(&c);
		VariantInit(&vResult);
		
		
		bstrTmp=strServer.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;
		
		
		bstrTmp=strUser.AllocSysString();
		b.bstrVal=bstrTmp;
		b.vt=VT_BSTR;

		//strServer=_T("dragonxu");
		bstrTmp=strPwd.AllocSysString();
		c.bstrVal=bstrTmp;
		c.vt=VT_BSTR;

		bstrTmp=strDisk.AllocSysString();
		d.bstrVal=bstrTmp;
		d.vt=VT_BSTR;


		myRef->getDiskInfo(&a,&b,&c,&d,&vResult);
		CString strResult(vResult.bstrVal);

		/*
		VARIANT  vResult;
		VariantInit(&vResult);
		BSTR	a,b,c,d;
		a=strServer.AllocSysString();
		b=strUser.AllocSysString();
		c=strPwd.AllocSysString();
		d=strDisk.AllocSysString();
		myRef->getDiskInfo(&a,&b,&c,&d,&vResult);
		CString strResult(vResult.bstrVal);
*/
		myRef->Release();
		myRef = NULL;

		
		VariantClear(&a);
		VariantClear(&b);
		VariantClear(&c);
		VariantClear(&d);

		VariantClear(&vResult);

		CoUninitialize();
		return strResult;
	}
	CoUninitialize();
	return "";
}

CString GetAllDiskbyCom(CString strServer, CString strUser,CString strPwd )
{

	CoInitialize(NULL);
	_dragonflow *myRef=NULL;
	HRESULT hr =0;
	myRef  =ThreeCreateDragonCom();
	if (NULL==myRef  )  hr=-200;


	if (SUCCEEDED(hr))
	{
		VARIANT  a,b,c,vResult;
		BSTR bstrTmp;
		VariantInit(&a);
		VariantInit(&b);
		VariantInit(&c);
		VariantInit(&vResult);
		
		//CString strServer=_T("dragonxu");
		bstrTmp=strServer.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;
		
		//strServer=_T("administrator");
		bstrTmp=strUser.AllocSysString();
		b.bstrVal=bstrTmp;
		b.vt=VT_BSTR;

		//strServer=_T("dragonxu");
		bstrTmp=strPwd.AllocSysString();
		c.bstrVal=bstrTmp;
		c.vt=VT_BSTR;

		myRef->getDisks(&a,&b,&c,&vResult);
		CString strResult(vResult.bstrVal);
		
		myRef->Release();
		myRef = NULL;
		VariantClear(&a);
		VariantClear(&b);
		VariantClear(&c);
		VariantClear(&vResult);

		CoUninitialize();
		return strResult;
	}
	
	CoUninitialize();
	return "";
}

CString GetCPUInfobyCom(CString strServer, CString strUser,CString strPwd )
{
	CoInitialize(NULL);
	_dragonflow *myRef=NULL;

	HRESULT hr =0;		
	myRef  =ThreeCreateDragonCom();
	if (NULL==myRef  )  hr=-200;



	if (SUCCEEDED(hr))
	{
		VARIANT  a,b,c,vResult;
		BSTR bstrTmp;
		VariantInit(&a);
		VariantInit(&b);
		VariantInit(&c);
		VariantInit(&vResult);
		
		//CString strServer=_T("dragonxu");
		bstrTmp=strServer.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;
		
		//strServer=_T("administrator");
		bstrTmp=strUser.AllocSysString();
		b.bstrVal=bstrTmp;
		b.vt=VT_BSTR;

		//strServer=_T("dragonxu");
		bstrTmp=strPwd.AllocSysString();
		c.bstrVal=bstrTmp;
		c.vt=VT_BSTR;

		myRef->getCPU(&a,&b,&c,&vResult);
		CString strResult(vResult.bstrVal);
		//wangpeng  2005-6-8  for 6.2  kill utilization2....
		int nUindex=strResult.Find("utilization1");
		if(nUindex>0)
		{
			strResult=strResult.Left(nUindex);
		}
		myRef->Release();

		VariantClear(&a);
		VariantClear(&b);
		VariantClear(&c);
		VariantClear(&vResult);

		CoUninitialize();
		return strResult;
	}

	CString strResult=_T("");
	strResult.Format("error=%s", FuncGetStringFromIDS("SV_NT_CPU",
        "NT_CPU_COM_INIT_FAILED"));
	//<%IDS_NtPerfTest_26%>

	CoUninitialize();
	return strResult;
}

CString GetSybaseInfobyCom(CString strDBName, CString strUser,CString strPwd)
{
	CoInitialize(NULL);
	_dragonflow *myRef=NULL;

	HRESULT hr =0;		
	myRef  =ThreeCreateDragonCom();
	if (NULL==myRef  )  hr=-200;

	if (SUCCEEDED(hr))
	{
		VARIANT  a,b,c,vResult;
		BSTR bstrTmp;
		VariantInit(&a);
		VariantInit(&b);
		VariantInit(&c);
		VariantInit(&vResult);
		
		//CString strServer=_T("dragonxu");
		bstrTmp=strDBName.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;
		
		//strServer=_T("administrator");
		bstrTmp=strUser.AllocSysString();
		b.bstrVal=bstrTmp;
		b.vt=VT_BSTR;

		//strServer=_T("dragonxu");
		bstrTmp=strPwd.AllocSysString();
		c.bstrVal=bstrTmp;
		c.vt=VT_BSTR;

		myRef->getSybase(&a,&b,&c,&vResult);
		CString strResult(vResult.bstrVal);

		myRef->Release();
		

		CoUninitialize();
		return strResult;
	}

	CString strResult=_T("");
	strResult.Format("error=%s", FuncGetStringFromIDS("<%IDS_NtPerfTest_45%>"));
	//<%IDS_NtPerfTest_45%>
	
	CoUninitialize();
	return strResult;
}

CString GetCPUInfobyScript(CString strCustomer, CString strGroupID, CString strMonitorID, CString strServer, CString strUser,CString strPwd )
{
	CString strTempFile = _T("");
	CString strCmdLine = _T("");
	CString strResult = _T("");
	CString strPath = ::FuncGetInstallRootPath();
	strCmdLine.Format("cscript //nologo //T:60 cpu2.vbs %s %s %s", strServer, strUser, strPwd);
//	strCmdLine.Format("cscript //nologo //T:60 cpu2.vbs \"%s\" \"%s\" \"%s\"", strServer, strUser, strPwd);
//	DebugInfo(strCmdLine);
	STARTUPINFO si = {0};
	si.cb = sizeof(STARTUPINFO);
	
	PROCESS_INFORMATION pi = {0};
	
	int nOldOutput = _dup(1);
//	puts(strTempFile);
	
	if (strCustomer.IsEmpty()) 
    {
#if _DEBUG
		strTempFile.Format("%s\\temp\\cpu-%s-%s.txt", (LPCTSTR)strPath, (LPCTSTR)strGroupID, (LPCTSTR)strMonitorID);
#else
		strTempFile.Format("%s\\MonitorManager\\temp\\cpu-%s-%s.txt", (LPCTSTR)strPath, (LPCTSTR)strGroupID, (LPCTSTR)strMonitorID);
#endif
	}
	else 
    {
#if _DEBUG
		strTempFile.Format("%s\\%s\\temp\\cpu-%s-%s.txt", (LPCTSTR)strPath, strCustomer, (LPCTSTR)strGroupID, (LPCTSTR)strMonitorID);
#else
		strTempFile.Format("%s\\MonitorManager\\%s\\temp\\cpu-%s-%s.txt", (LPCTSTR)strPath, strCustomer, (LPCTSTR)strGroupID, (LPCTSTR)strMonitorID);
#endif
	}

	FILE *stream = freopen((LPCTSTR)strTempFile, "w", stdout);
	if (stream) {
		HANDLE hOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);

		if (hOutHandle) {
			SECURITY_ATTRIBUTES sa; 
			sa.nLength = sizeof(SECURITY_ATTRIBUTES);
			sa.bInheritHandle = TRUE;
			sa.lpSecurityDescriptor = NULL;
			
			STARTUPINFO si;
			memset(&si, 0, sizeof(STARTUPINFO));
			si.cb = sizeof(STARTUPINFO);
			si.dwFlags = STARTF_USESTDHANDLES;
			si.hStdOutput = hOutHandle;
			si.hStdError = hOutHandle;
			
			PROCESS_INFORMATION pi;
			memset(&pi, 0, sizeof(PROCESS_INFORMATION));

			if (CreateProcess(NULL, strCmdLine.GetBuffer(strCmdLine.GetLength()),  NULL, NULL, TRUE, CREATE_NEW_CONSOLE/*CREATE_NO_WINDOW*/, NULL, NULL, &si, &pi)) {
				BOOL bRet = TRUE;
			
				DWORD dwResult = WaitForSingleObject(pi.hProcess, 20*1000);
				if (dwResult == WAIT_OBJECT_0) {
					CFile myFile((LPCTSTR)strTempFile, CFile::shareDenyRead);
					int nFileLength = myFile.GetLength();
					myFile.Close();
					
					
					if (nFileLength > 0) {
						char * buffer = NULL;
						buffer = (char*)malloc(nFileLength+1);
						if (buffer) {
							memset(buffer, 0, nFileLength+1);
							FILE * fp = fopen((LPCTSTR)strTempFile, "r");
							if (fp) {
								fread(buffer, sizeof(char), nFileLength+1, fp);
								fclose(fp);
								//puts(buffer);
								strResult.Format("%s", buffer);
							}
							free(buffer);
						}
					}     
				}
				else if (dwResult == WAIT_TIMEOUT){
					strResult = "error=timeout$";	
					::TerminateProcess(pi.hProcess, -1);
				}
				else {
					strResult = "error=unknown$";
				}
			}
		}
		SetStdHandle(STD_OUTPUT_HANDLE, hOutHandle);
	}

	return strResult;
//	DebugInfo(strResult);
}


CString GetPhysicsMemoryInfobyCom(CString strServer, CString strUser,CString strPwd )
{
	CoInitialize(NULL);
	_dragonflow *myRef=NULL;

	HRESULT hr =0;		
	myRef  =ThreeCreateDragonCom();
	if (NULL==myRef  )  hr=-200;

	if (SUCCEEDED(hr))
	{
		VARIANT  a,b,c,vResult;
		BSTR bstrTmp;
		VariantInit(&a);
		VariantInit(&b);
		VariantInit(&c);
		VariantInit(&vResult);
		
		//CString strServer=_T("dragonxu");
		bstrTmp=strServer.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;
		
		//strServer=_T("administrator");
		bstrTmp=strUser.AllocSysString();
		b.bstrVal=bstrTmp;
		b.vt=VT_BSTR;

		//strServer=_T("dragonxu");
		bstrTmp=strPwd.AllocSysString();
		c.bstrVal=bstrTmp;
		c.vt=VT_BSTR;

		myRef->getPhysicsMemory(&a,&b,&c,&vResult);
		CString strResult(vResult.bstrVal);
		
		myRef->Release();
		
		VariantClear(&a);
		VariantClear(&b);
		VariantClear(&c);
		VariantClear(&vResult);

		CoUninitialize();
		return strResult;
	}
	
	CoUninitialize();
	CString strResult=_T("");
	strResult.Format("%s","error=%s",FuncGetStringFromIDS("SV_NT_MEMORY",
        "NT_MEMORY_COM_INIT_FAILED"));
	//<%IDS_NtPerfTest_27%>
	return strResult;

}

CString GetMemoryInfobyCom(CString strServer, CString strUser,CString strPwd )
{
	CoInitialize(NULL);
	_dragonflow *myRef=NULL;

	HRESULT hr =0;		
	myRef  =ThreeCreateDragonCom();
	if (NULL==myRef  )  hr=-200;

	if (SUCCEEDED(hr))
	{
		VARIANT  a,b,c,vResult;
		BSTR bstrTmp;
		VariantInit(&a);
		VariantInit(&b);
		VariantInit(&c);
		VariantInit(&vResult);
		
		//CString strServer=_T("dragonxu");
		bstrTmp=strServer.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;
		
		//strServer=_T("administrator");
		bstrTmp=strUser.AllocSysString();
		b.bstrVal=bstrTmp;
		b.vt=VT_BSTR;

		//strServer=_T("dragonxu");
		bstrTmp=strPwd.AllocSysString();
		c.bstrVal=bstrTmp;
		c.vt=VT_BSTR;

		myRef->getMemory(&a,&b,&c,&vResult);
		CString strResult(vResult.bstrVal);
		
		myRef->Release();
		
		VariantClear(&a);
		VariantClear(&b);
		VariantClear(&c);
		VariantClear(&vResult);

		CoUninitialize();
		return strResult;
	}
	
	CoUninitialize();
	CString strResult=_T("");
	strResult.Format("%s","error=%s",FuncGetStringFromIDS("SV_NT_MEMORY",
        "NT_MEMORY_COM_INIT_FAILED"));
	//<%IDS_NtPerfTest_27%>
	return strResult;

}

CString GetServiceInfobyCom(CString strServer, CString strUser,CString strPwd, CString strDisk)
{
		CoInitialize(NULL);
	_dragonflow *myRef=NULL;
	
	HRESULT hr =0;		
	myRef  =ThreeCreateDragonCom();
	if (NULL==myRef  )  hr=-200;

	if (SUCCEEDED(hr))
	{

		VARIANT  a,b,c,d,vResult;
		BSTR bstrTmp;
		VariantInit(&a);
		VariantInit(&b);
		VariantInit(&c);
		VariantInit(&vResult);
		
		
		bstrTmp=strServer.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;
		
		
		bstrTmp=strUser.AllocSysString();
		b.bstrVal=bstrTmp;
		b.vt=VT_BSTR;

		//strServer=_T("dragonxu");
		bstrTmp=strPwd.AllocSysString();
		c.bstrVal=bstrTmp;
		c.vt=VT_BSTR;

		bstrTmp=strDisk.AllocSysString();
		d.bstrVal=bstrTmp;
		d.vt=VT_BSTR;

		myRef->getServiceStatus(&a,&b,&c,&d,&vResult);
		CString strResult(vResult.bstrVal);

		myRef->Release();
		myRef = NULL;

		VariantClear(&a);
		VariantClear(&b);
		VariantClear(&c);
		VariantClear(&d);
		VariantClear(&vResult);

		CoUninitialize();
		return strResult;
	}

	CoUninitialize();
	CString strResult=_T("");
	strResult.Format("%s","error=%s",FuncGetStringFromIDS("SV_NT_SERVICE",
        "NT_SERVICE_COM_INIT_FAILED"));//<%IDS_NtPerfTest_28%>
	return strResult;
}


CString GetServicesbyCom(CString strServer, CString strUser,CString strPwd)
{
		CoInitialize(NULL);
	_dragonflow *myRef=NULL;
	/*
	HRESULT hr=CoCreateInstance(CLSID_dragonflow,NULL,
							CLSCTX_ALL,
							IID__dragonflow,(void **)&myRef);
							*/
	HRESULT hr =0;		
	myRef  =ThreeCreateDragonCom();
	if (NULL==myRef  )  hr=-200;

	if (SUCCEEDED(hr))
	{

		VARIANT  a,b,c,vResult;
		BSTR bstrTmp;
		VariantInit(&a);
		VariantInit(&b);
		VariantInit(&c);
		VariantInit(&vResult);
		
		
		bstrTmp=strServer.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;
		
		
		bstrTmp=strUser.AllocSysString();
		b.bstrVal=bstrTmp;
		b.vt=VT_BSTR;

		//strServer=_T("dragonxu");
		bstrTmp=strPwd.AllocSysString();
		c.bstrVal=bstrTmp;
		c.vt=VT_BSTR;

		//myRef->getServiceStatus(&a,&b,&c,&d,&vResult);
		myRef->getServices(&a,&b,&c,&vResult);
		CString strResult(vResult.bstrVal);

		VariantClear(&a);
		VariantClear(&b);
		VariantClear(&c);
		VariantClear(&vResult);
		myRef->Release();
		myRef = NULL;

		CoUninitialize();
		return strResult;
	}
	CoUninitialize();
	CString strResult=_T("");
	strResult.Format("%s","error=%s",FuncGetStringFromIDS("<%IDS_NtPerfTest_29%>"));//<%IDS_NtPerfTest_29%>
	return strResult;
}


CString GetIISInfobyCom(CString strServer, CString strUser,CString strPwd )
{
	CoInitialize(NULL);
	_dragonflow *myRef=NULL;
	
	HRESULT hr =0;		
	myRef  =ThreeCreateDragonCom();
	if (NULL==myRef  )  hr=-200;

	if (SUCCEEDED(hr))
	{
		VARIANT  a,b,c,vResult;
		BSTR bstrTmp;
		VariantInit(&a);
		VariantInit(&b);
		VariantInit(&c);
		VariantInit(&vResult);
		
		//CString strServer=_T("dragonxu");
		bstrTmp=strServer.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;
		
		//strServer=_T("administrator");
		bstrTmp=strUser.AllocSysString();
		b.bstrVal=bstrTmp;
		b.vt=VT_BSTR;

		//strServer=_T("dragonxu");
		bstrTmp=strPwd.AllocSysString();
		c.bstrVal=bstrTmp;
		c.vt=VT_BSTR;

		myRef->getIIS(&a,&b,&c,&vResult);
		CString strResult(vResult.bstrVal);
		
		if(""==strResult)
		{
			AddToExceptionLog("com return null");
		}
		myRef->Release();
		

		CoUninitialize();
		return strResult;
	}
	AddToExceptionLog(" use com failed");	

	CoUninitialize();
	return FuncGetStringFromIDS("<%IDS_NtPerfTest_30%>");//<%IDS_NtPerfTest_30%>

}

CString GetWebInfobyCom(CString strServer, CString strUser,CString strPwd )
{
	CoInitialize(NULL);
	_dragonflow *myRef=NULL;

	HRESULT hr =0;		
	myRef  =ThreeCreateDragonCom();
	if (NULL==myRef  )  hr=-200;

	if (SUCCEEDED(hr))
	{
		VARIANT  a,b,c,vResult;
		BSTR bstrTmp;
		VariantInit(&a);
		VariantInit(&b);
		VariantInit(&c);
		VariantInit(&vResult);
		
		//CString strServer=_T("dragonxu");
		bstrTmp=strServer.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;
		
		//strServer=_T("administrator");
		bstrTmp=strUser.AllocSysString();
		b.bstrVal=bstrTmp;
		b.vt=VT_BSTR;

		//strServer=_T("dragonxu");
		bstrTmp=strPwd.AllocSysString();
		c.bstrVal=bstrTmp;
		c.vt=VT_BSTR;

		//myRef->getWebServer(&a,&b,&c,&vResult);
		myRef->getWebserver(&a,&b,&c,&vResult);
		CString strResult(vResult.bstrVal);
		
		myRef->Release();
		

		CoUninitialize();
		return strResult;
	}
	
	CoUninitialize();
	return FuncGetStringFromIDS("<%IDS_NtPerfTest_31%>");//<%IDS_NtPerfTest_31%>

}



CString GetNetWorkInfobyCom(CString strServer, CString strUser,CString strPwd, CString strDisk)
{
	CoInitialize(NULL);
	_dragonflow *myRef=NULL;


	HRESULT hr =0;		
	myRef  =ThreeCreateDragonCom();
    printf("Create Prjwmi COM object\n");
	if (NULL==myRef  )  hr=-200;

	if (SUCCEEDED(hr))
	{

		VARIANT  a,b,c,d,vResult;
		BSTR bstrTmp;
		VariantInit(&a);
		VariantInit(&b);
		VariantInit(&c);
		VariantInit(&vResult);
		
		
		bstrTmp=strServer.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;
		
		
		bstrTmp=strUser.AllocSysString();
		b.bstrVal=bstrTmp;
		b.vt=VT_BSTR;

		//strServer=_T("dragonxu");
		bstrTmp=strPwd.AllocSysString();
		c.bstrVal=bstrTmp;
		c.vt=VT_BSTR;

		bstrTmp=strDisk.AllocSysString();
		d.bstrVal=bstrTmp;
		d.vt=VT_BSTR;

		//myRef->getServiceStatus(&a,&b,&c,&d,&vResult);
        printf("Server is :%s\n", (CString)a.bstrVal);
        printf("User is: %s\n", (CString)b.bstrVal);
        printf("Password is: %s\n", (CString)c.bstrVal);
        printf("Network card:%s\n", (CString)d.bstrVal);
        //printf("Password is %s\n", strPwd);
        //printf("Get Infomation of Network\n");
		myRef->getNetworkInterface(&a,&b,&c,&d,&vResult);
        //printf("Return Value Type:%d\n", vResult.vt);
		CString strResult(vResult.bstrVal);
        //strResult = vResult.bstrVal;
        printf("Return String:%s\n", strResult);
		myRef->Release();
		myRef = NULL;

		CoUninitialize();
		return strResult;
	}
	CoUninitialize();
	return FuncGetStringFromIDS("<%IDS_NtPerfTest_32%>");//<%IDS_NtPerfTest_32%>
}
CString GetAllNetWorkbyCom(CString strServer, CString strUser,CString strPwd )
{
	CoInitialize(NULL);
	_dragonflow *myRef=NULL;

	HRESULT hr =0;		
	myRef  =ThreeCreateDragonCom();
	if (NULL==myRef  )  hr=-200;

	if (SUCCEEDED(hr))
	{

		VARIANT  a,b,c,vResult;
		BSTR bstrTmp;
		VariantInit(&a);
		VariantInit(&b);
		VariantInit(&c);
		VariantInit(&vResult);
		
		
		bstrTmp=strServer.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;
		
		
		bstrTmp=strUser.AllocSysString();
		b.bstrVal=bstrTmp;
		b.vt=VT_BSTR;

		
		bstrTmp=strPwd.AllocSysString();
		c.bstrVal=bstrTmp;
		c.vt=VT_BSTR;

		
		myRef->getNetworkInterfaceNames(&a,&b,&c,&vResult);
		CString strResult(vResult.bstrVal);

		myRef->Release();
		myRef = NULL;

		CoUninitialize();
		return strResult;
	}
	CoUninitialize();
	return FuncGetStringFromIDS("<%IDS_NtPerfTest_33%>");//<%IDS_NtPerfTest_33%>
}


CString GetAllProcessbyCom(CString strServer, CString strUser,CString strPwd )
{
	CoInitialize(NULL);
	_dragonflow *myRef=NULL;

	HRESULT hr =0;		
	myRef  =ThreeCreateDragonCom();
	if (NULL==myRef  )  hr=-200;

	if (SUCCEEDED(hr))
	{
		VARIANT  a,b,c,vResult;
		BSTR bstrTmp;
		VariantInit(&a);
		VariantInit(&b);
		VariantInit(&c);
		VariantInit(&vResult);
		
		
		bstrTmp=strServer.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;
		
		
		bstrTmp=strUser.AllocSysString();
		b.bstrVal=bstrTmp;
		b.vt=VT_BSTR;

		
		bstrTmp=strPwd.AllocSysString();
		c.bstrVal=bstrTmp;
		c.vt=VT_BSTR;
		
		myRef->getProcessNamesNew(&a,&b,&c,&vResult);
		CString strResult(vResult.bstrVal);

		VariantClear(&a);
		VariantClear(&b);
		VariantClear(&c);
		myRef->Release();
		myRef = NULL;

		CoUninitialize();
		return strResult;
	}
	CoUninitialize();
	return FuncGetStringFromIDS("<%IDS_NtPerfTest_34%>");//<%IDS_NtPerfTest_34%>
}

/////////////////////////////////////////////////////////////////////////////
// 参数：
//      strServer，服务器IP或者域名
//      strUser，服务器的访问用户
//      strPwd，用户的密码
//      strLogName，预备查询的日志文件名称
//      strType，查询的何种类型的日志(信息，警告，错误)
//      strCodeList，反过滤事件ID的列表，以英文分号分割
//      strSourceList，反过滤事件来源的列表，以英文分号分割
//      strEventMachine，产生事件的机器名称
//      strCustomer，IDC版本的用户名
/////////////////////////////////////////////////////////////////////////////

CString FilterNTEventLogByCom(CString strServer,CString strUser,CString strPwd,
                              CString strLogName,CString strType, CString strCodeList,
                              CString strSourceList, CString strEventMachine,
                              CString strCustomer,CString strGroupID,
                              CString strMonitorID)
{
		CoInitialize(NULL);//初始化COM

	CString strResult = _T("");//结果
	_dragonflow *myRef=NULL;
    HRESULT hr = NULL;
    myRef = ThreeCreateDragonCom();//创建Project1.DragonFlow Com对象
    if(myRef == NULL)//创建失败
        hr = -200;
    if(SUCCEEDED(hr))//成功创建
    {
        //主机，用户名，密码，日志类型，监测事件类型
        //
        VARIANT HostIP, UserName, Pwd , LogFile, type,
            EventMachine, codelist, sourceList, startTime, vResult;
        //初始化变量
        VariantInit(&HostIP);VariantInit(&UserName);VariantInit(&Pwd);
        VariantInit(&LogFile);VariantInit(&type);VariantInit(&EventMachine);
        VariantInit(&codelist);VariantInit(&sourceList);VariantInit(&startTime);
        VariantInit(&vResult);

        CString strStartTime = _T(""), strTempFile = _T("");
        CString strPath = FuncGetInstallRootPath();
		CString strKey;
		if (strCustomer.IsEmpty())
        {
			strTempFile ="nteventlog.ini";
		}
		

		char buffer[512] = {0};
		::DFN_GetPrivateProfileString(strMonitorID.GetBuffer(strMonitorID.GetLength()), "lasttime", "", buffer, sizeof(buffer) - 1, (LPCTSTR)strTempFile);


		
		CTime curTime = CTime::GetCurrentTime();
		CString strMsg = curTime.Format("%Y%m%d%H%M%S");
		
		if (strlen(buffer) == 0) 
        {		
			curTime -= 60*60*24; // 10 minutes ago
			strStartTime = curTime.Format("%Y%m%d%H%M%S");			
		}
		else
        {
			strStartTime = buffer;
		}

		strcpy(buffer, (LPCTSTR)strMsg);
		::DFN_WritePrivateProfileString(strMonitorID.GetBuffer(strMonitorID.GetLength()), "lasttime", buffer, (LPCTSTR)strTempFile);
		
        BSTR bstrTmp;
        //HostIP
		bstrTmp=strServer.AllocSysString();
		HostIP.vt=VT_BSTR;
		HostIP.bstrVal=bstrTmp; 
        //UserName
		bstrTmp=strUser.AllocSysString();
		UserName.vt=VT_BSTR;
		UserName.bstrVal=bstrTmp;  
        //Password
		bstrTmp=strPwd.AllocSysString();
		Pwd.vt=VT_BSTR;
		Pwd.bstrVal=bstrTmp; 
        //LogFile
		bstrTmp=strLogName.AllocSysString();
		LogFile.vt=VT_BSTR;
		LogFile.bstrVal=bstrTmp;   
        //type
		bstrTmp=strType.AllocSysString();
		type.vt=VT_BSTR;
		type.bstrVal=bstrTmp; 
        //Event Machine
		bstrTmp=strEventMachine.AllocSysString();
		EventMachine.vt=VT_BSTR;
		EventMachine.bstrVal=bstrTmp; 
        //Filter Code List
		bstrTmp=strCodeList.AllocSysString();
		codelist.vt=VT_BSTR;
		codelist.bstrVal=bstrTmp;   
        //Filter Source List
		bstrTmp=strSourceList.AllocSysString();
		sourceList.vt=VT_BSTR;
		sourceList.bstrVal=bstrTmp;  
        //start time
		bstrTmp=strStartTime.AllocSysString();
		startTime.vt=VT_BSTR;
		startTime.bstrVal=bstrTmp;

        hr = myRef->FilterNTEnventLog(&HostIP,&UserName,&Pwd,&LogFile,
            &type,&codelist,&sourceList,&EventMachine,&startTime,&vResult);
        if(SUCCEEDED(hr))
        {
            CString strTempResult(vResult.bstrVal);
			strResult = strTempResult;
        }
		VariantClear(&HostIP);
		VariantClear(&UserName);
		VariantClear(&Pwd);
		VariantClear(&LogFile);
		VariantClear(&type);
		VariantClear(&codelist);
		VariantClear(&sourceList);
		VariantClear(&EventMachine);
		VariantClear(&startTime);
		VariantClear(&vResult);
		

        myRef->Release();
        myRef = NULL;
    }
	CoUninitialize();
	return strResult;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
CString GetNTEventLogInfobyCom(CString strServer, CString strUser,CString strPwd,
                               CString strLogName, CString strEventType, 
                               CString strAlert, CString strEventKey, 
                               CString strNotEventKey, CString strPosMatch,
                               CString strNegMatch, CString strEventCategory, 
                               CString strEventMachine, CString strInterval, 
                               CString strGroupID, CString strMonitorID, 
                               CString strCustomer)
{
	CoInitialize(NULL);

	CString strResult = _T("");
	_dragonflow *myRef=NULL;

	HRESULT hr =0;		
	myRef  =ThreeCreateDragonCom();
	if (NULL==myRef  )  hr=-200;

	if (SUCCEEDED(hr)) 	{
		//
		VARIANT  a,b,c,vResult;
		VARIANT logfilename, strstarttime, eventtype, sourceidmatch, sourceidnotmatch, desmatch, desnotmatch, eventcategory, eventmachine, interval;

		BSTR bstrTmp;

		VariantInit(&a);
		bstrTmp=strServer.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;

		VariantInit(&b);
		bstrTmp=strUser.AllocSysString();
		b.bstrVal=bstrTmp;
		b.vt=VT_BSTR;

		VariantInit(&c);
		bstrTmp=strPwd.AllocSysString();
		c.bstrVal=bstrTmp;
		c.vt=VT_BSTR;		

		VariantInit(&logfilename);
		bstrTmp=strLogName.AllocSysString();
		logfilename.bstrVal=bstrTmp;
		logfilename.vt=VT_BSTR;

		VariantInit(&strstarttime);
		CString strPath = ::FuncGetInstallRootPath();
		CString strTempFile = _T("");

		if (strCustomer.IsEmpty()) {
#if _DEBUG
			strTempFile.Format("%s\\temp\\nteventlog-%s-%s.txt", (LPCTSTR)strPath, 
                (LPCTSTR)strGroupID, (LPCTSTR)strMonitorID);
#else
			strTempFile.Format("%s\\MonitorManager\\temp\\nteventlog-%s-%s.txt", 
                (LPCTSTR)strPath, (LPCTSTR)strGroupID, (LPCTSTR)strMonitorID);
#endif
		}
		else {
			strTempFile.Format("%s\\%s\\temp\\nteventlog-%s-%s.txt", (LPCTSTR)strPath, strCustomer, (LPCTSTR)strGroupID, (LPCTSTR)strMonitorID);
		}

		char buffer[512] = {0};
		::DFN_GetPrivateProfileString("NTLOG", "lasttime", "", buffer, sizeof(buffer) - 1, (LPCTSTR)strTempFile);
		CString strStartTime = _T("");
		
		CTime curTime = CTime::GetCurrentTime();
		CString strMsg = curTime.Format("%Y%m%d%H%M%S");
		
		if (strlen(buffer) == 0) {		
			curTime -= 60*60*24; // 10 minutes ago
			strStartTime = curTime.Format("%Y%m%d%H%M%S");			
		}
		else {
			strStartTime = buffer;
		}

		strcpy(buffer, (LPCTSTR)strMsg);
		::DFN_WritePrivateProfileString("NTLOG", "lasttime", buffer, (LPCTSTR)strTempFile);
		

		bstrTmp=strStartTime.AllocSysString();
		strstarttime.bstrVal=bstrTmp;
		strstarttime.vt=VT_BSTR;

		VariantInit(&eventtype);
		bstrTmp=strEventType.AllocSysString();
		eventtype.bstrVal=bstrTmp;
		eventtype.vt=VT_BSTR;

		VariantInit(&sourceidmatch);		
		bstrTmp=strEventKey.AllocSysString();
		sourceidmatch.bstrVal=bstrTmp;
		sourceidmatch.vt=VT_BSTR;

		VariantInit(&sourceidnotmatch);
		bstrTmp=strNotEventKey.AllocSysString();
		sourceidnotmatch.bstrVal=bstrTmp;
		sourceidnotmatch.vt=VT_BSTR;

		VariantInit(&desmatch);
		bstrTmp=strPosMatch.AllocSysString();
		desmatch.bstrVal=bstrTmp;
		desmatch.vt=VT_BSTR;

		VariantInit(&desnotmatch);
		bstrTmp=strNegMatch.AllocSysString();
		desnotmatch.bstrVal=bstrTmp;
		desnotmatch.vt=VT_BSTR;

		VariantInit(&eventcategory);
		bstrTmp=strEventCategory.AllocSysString();
		eventcategory.bstrVal=bstrTmp;
		eventcategory.vt=VT_BSTR;

		VariantInit(&eventmachine);
		bstrTmp=strEventMachine.AllocSysString();
		eventmachine.bstrVal=bstrTmp;
		eventmachine.vt=VT_BSTR;

		VariantInit(&interval);
		bstrTmp=strInterval.AllocSysString();
		interval.bstrVal=bstrTmp;
		interval.vt=VT_BSTR;

		VariantInit(&vResult);
///////////////////////////////////////////////////

		bstrTmp = a.bstrVal;
		CString strMsg1(bstrTmp);

		bstrTmp = b.bstrVal;
		CString strMsg2(bstrTmp);

		bstrTmp = c.bstrVal;
		CString strMsg3(bstrTmp);

		bstrTmp = logfilename.bstrVal;
		CString strMsg4(bstrTmp);
		DebugInfo(strMsg4);

		bstrTmp = strstarttime.bstrVal;
		CString strMsg5(bstrTmp);

		bstrTmp = eventtype.bstrVal;
		CString strMsg6(bstrTmp);

		bstrTmp = sourceidmatch.bstrVal;
		CString strMsg7(bstrTmp);
		DebugInfo(strMsg7);

		bstrTmp = sourceidnotmatch.bstrVal;
		CString strMsg8(bstrTmp);

		bstrTmp = desmatch.bstrVal;
		CString strMsg9(bstrTmp);

		bstrTmp = desnotmatch.bstrVal;
		CString strMsg10(bstrTmp);

		bstrTmp = eventcategory.bstrVal;
		CString strMsg11(bstrTmp);

		bstrTmp = eventmachine.bstrVal;
		CString strMsg12(bstrTmp);

		bstrTmp = interval.bstrVal;
		CString strMsg13(bstrTmp);

///////////////////////////////////////////////////
		hr = myRef->getNTEventLog(&a, &b, &c, &logfilename, &strstarttime, &eventtype, &sourceidmatch, &sourceidnotmatch, &desmatch, &desnotmatch, &eventcategory, &eventmachine, &interval, &vResult);

		if (SUCCEEDED(hr)) {
			CString strTempResult(vResult.bstrVal);
			strResult = strTempResult;			
		}		
		//
		myRef->Release();
		myRef = NULL;
	}

	CoUninitialize();
	return strResult;
}

CString GetExchangeInfobyCom(CString strServer, CString strUser,CString strPwd )
{
	CoInitialize(NULL);

	CString strResult = _T("");

	_dragonflow *myRef=NULL;
	HRESULT hr =0;		
	myRef  =ThreeCreateDragonCom();
	if (NULL==myRef  )  hr=-200;

	if (SUCCEEDED(hr)) 	{
		VARIANT  a,b,c,vResult;
		BSTR bstrTmp;
		VariantInit(&a);
		VariantInit(&b);
		VariantInit(&c);
		VariantInit(&vResult);

		bstrTmp=strServer.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;
		
		bstrTmp=strUser.AllocSysString();
		b.bstrVal=bstrTmp;
		b.vt=VT_BSTR;
		
		bstrTmp=strPwd.AllocSysString();
		c.bstrVal=bstrTmp;
		c.vt=VT_BSTR;
	
		hr = myRef->getExchange(&a,&b,&c,&vResult);

		if (SUCCEEDED(hr)) {
			CString strTempResult(vResult.bstrVal);
			strResult = strTempResult;
		}

		myRef->Release();
		myRef = NULL;
	}
	CoUninitialize();
	return strResult;
}

CString GetASPInfobyCom(CString strServer, CString strUser,CString strPwd )
{
	CoInitialize(NULL);

	CString strResult = _T("");

	_dragonflow *myRef=NULL;
	HRESULT hr =0;		
	myRef  =ThreeCreateDragonCom();
	if (NULL==myRef  )  hr=-200;

	if (SUCCEEDED(hr)) 	{
		VARIANT  a,b,c,vResult;
		BSTR bstrTmp;
		VariantInit(&a);
		VariantInit(&b);
		VariantInit(&c);
		VariantInit(&vResult);

		bstrTmp=strServer.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;
		
		bstrTmp=strUser.AllocSysString();
		b.bstrVal=bstrTmp;
		b.vt=VT_BSTR;
		
		bstrTmp=strPwd.AllocSysString();
		c.bstrVal=bstrTmp;
		c.vt=VT_BSTR;
	
		hr = myRef->getASPServer(&a,&b,&c,&vResult);

		if (SUCCEEDED(hr)) {
			CString strTempResult(vResult.bstrVal);
			//strResult = getRequestReturnValues(strTempResult);
			strResult = strTempResult;
		}

		myRef->Release();
		myRef = NULL;
	}
	CoUninitialize();
	return strResult;
}

CString GetSQLServerInfobyCom(CString strServer, CString strUser,CString strPwd )
{
	CoInitialize(NULL);

	CString strResult = _T("");

	_dragonflow *myRef=NULL;
	HRESULT hr =0;		
	myRef  =ThreeCreateDragonCom();
	if (NULL==myRef  )  hr=-200;

	if (SUCCEEDED(hr)) 	{
		VARIANT  a,b,c,vResult;
		BSTR bstrTmp;
		VariantInit(&a);
		VariantInit(&b);
		VariantInit(&c);
		VariantInit(&vResult);

		bstrTmp=strServer.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;
		
		bstrTmp=strUser.AllocSysString();
		b.bstrVal=bstrTmp;
		b.vt=VT_BSTR;
		
		bstrTmp=strPwd.AllocSysString();
		c.bstrVal=bstrTmp;
		c.vt=VT_BSTR;
	
		hr = myRef->getSQLServer(&a,&b,&c,&vResult);

		if (SUCCEEDED(hr)) {
			CString strTempResult(vResult.bstrVal);
			strResult = strTempResult;
		}

		myRef->Release();
		myRef = NULL;
	}
	CoUninitialize();
	return strResult;
}

CString GetProcessInfobyCom(CString strServer, CString strUser,CString strPwd, CString strDisk)
{
	CoInitialize(NULL);
	_dragonflow *myRef=NULL;

	HRESULT hr =0;		
	myRef  =ThreeCreateDragonCom();
	if (NULL==myRef  )  hr=-200;

	if (SUCCEEDED(hr))
	{

		VARIANT  a,b,c,d,vResult;
		BSTR bstrTmp;
		VariantInit(&a);
		VariantInit(&b);
		VariantInit(&c);
		VariantInit(&vResult);
		
		bstrTmp=strServer.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;
		
		bstrTmp=strUser.AllocSysString();
		b.bstrVal=bstrTmp;
		b.vt=VT_BSTR;

		//strServer=_T("dragonxu");
		bstrTmp=strPwd.AllocSysString();
		c.bstrVal=bstrTmp;
		c.vt=VT_BSTR;

		bstrTmp=strDisk.AllocSysString();
		d.bstrVal=bstrTmp;
		d.vt=VT_BSTR;

		//myRef->getServiceStatus(&a,&b,&c,&d,&vResult);

		myRef->getProcessInfoNew(&a,&b,&c,&d,&vResult);

		CString strResult(vResult.bstrVal);

		myRef->Release();
		myRef = NULL;
		VariantClear(&a);
		VariantClear(&b);
		VariantClear(&c);
		VariantClear(&d);

		CoUninitialize();
		return strResult;
	}
	CoUninitialize();
	return FuncGetStringFromIDS("<%IDS_NtPerfTest_35%>");//<%IDS_NtPerfTest_35%>
}

void DebugInfo(const char * szMsg)
{
	FILE * fp = fopen("c:\\ntpert.txt", "a+");
	if (fp) {
		fputs(szMsg, fp);
		fputs("\r\n", fp);
		fclose(fp);
	}
}

int GetVersionFromCom(CString strServer, CString strUser,CString strPwd)
{
	
	CoInitialize(NULL);
	_dragonflow *myRef=NULL;

	HRESULT hr =0;		
	myRef  =ThreeCreateDragonCom();
	if (NULL==myRef  )  hr=-200;

	if (SUCCEEDED(hr))
	{

		VARIANT  a,b,c,d,vResult;
		BSTR bstrTmp;
		VariantInit(&a);
		VariantInit(&b);
		VariantInit(&c);
		VariantInit(&vResult);
		
		bstrTmp=strServer.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;
		
		bstrTmp=strUser.AllocSysString();
		b.bstrVal=bstrTmp;
		b.vt=VT_BSTR;

		//strServer=_T("dragonxu");
		bstrTmp=strPwd.AllocSysString();
		c.bstrVal=bstrTmp;
		c.vt=VT_BSTR;

		CString strDisk=_T(" Win32_OperatingSystem where Primary=true");
		
		bstrTmp=strDisk.AllocSysString();
		d.bstrVal=bstrTmp;
		d.vt=VT_BSTR;

		//myRef->getServiceStatus(&a,&b,&c,&d,&vResult);

		myRef->getSystemInfo(&a,&b,&c,&d,&vResult);

		CString strResult(vResult.bstrVal);

		myRef->Release();
		myRef = NULL;

		CoUninitialize();
		int nVersion=0;
		nVersion=atoi(strResult);
		if(nVersion == 0) 
			return 2;
		if(nVersion==4) nVersion=0;
		return nVersion;
	}
	CoUninitialize();
	return 5;
}

CString GetFileInfobyCom(CString strFilePath)
{
	CoInitialize(NULL);
	_dragonflow *myRef=NULL;

	HRESULT hr =0;		
	myRef  =ThreeCreateDragonCom();
	if (NULL==myRef  )  hr=-200;

	if (SUCCEEDED(hr))
	{
		VARIANT  a,vResult;
		BSTR bstrTmp;
		VariantInit(&a);
		VariantInit(&vResult);
		
		bstrTmp=strFilePath.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;
		
		myRef->getFileInfo(&a,&vResult);
		CString strResult(vResult.bstrVal);
		
//		strResult = "+++" + strResult + "+++";
//		DebugInfo(strResult);
		
		myRef->Release();
		myRef = NULL;
		
		CoUninitialize();
		return strResult;
	}

	CoUninitialize();
	return FuncGetStringFromIDS("<%IDS_NtPerfTest_34%>");//<%IDS_NtPerfTest_34%>
}

extern "C" __declspec(dllexport) BOOL  GetFileInfo(CStringList& lstParas,char * szRet )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strFilePath=_T("");
	strFilePath=GetValueFromList("_filepath",lstParas);
//	DebugInfo(strFilePath);

	CString strtmp=_T("");
	strtmp = GetFileInfobyCom(strFilePath);

	//printf("Likeham1:%s\n",strtmp);

	if (strtmp=="")
	{
		sprintf(szRet,"error=%s",FuncGetStringFromIDS("<%IDS_NtPerfTest_136%>"));//<%IDS_NtPerfTest_36%>
		return FALSE;
	}
	else{
		//printf("Likeham2:%s\n",strtmp);
		//sprintf(szRet,"%s",strtmp);

		sprintf(szRet,"file:%s",strtmp);

		//printf("Likeham3:%s end\n",szRet);

		return TRUE;
	}

	//CString strRet=_T("");
	//strRet = GetFileInfobyCom(strFilePath);
	
	//printf("Likeham2:%s",strRet);
/*
	if (strRet=="")
	{
		sprintf(szRet,FuncGetStringFromIDS("<%IDS_NtPerfTest_136%>"));//<%IDS_NtPerfTest_36%>
		return FALSE;
	}
	else{
		//printf("Likeham1:%s\n",szRet);

		sprintf(szRet,"file:%s",strRet);

		//sprintf(szRet,"%s",strRet);

		//printf("Likeham2:%s @\n",szRet);

		return TRUE;
	}
	*/
}

CString GetDirInfobyCom(CString strDirPath, CString strMatch)
{
	CoInitialize(NULL);
	_dragonflow *myRef=NULL;

	HRESULT hr =0;		
	myRef  =ThreeCreateDragonCom();
	if (NULL==myRef  )  hr=-200;

	if (SUCCEEDED(hr))
	{
		VARIANT  a,b,vResult;
		BSTR bstrTmp;
		VariantInit(&a);
		VariantInit(&b);
		VariantInit(&vResult);
		
		bstrTmp=strDirPath.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;
		
		bstrTmp=strMatch.AllocSysString();
		b.vt=VT_BSTR;
		b.bstrVal=bstrTmp;

		myRef->getDirInfo(&a,&b,&vResult);
		CString strResult(vResult.bstrVal);
		
		myRef->Release();
		myRef = NULL;
		
		CoUninitialize();
		return strResult;
	}

	CoUninitialize();
	return FuncGetStringFromIDS("<%IDS_NtPerfTest_35%>");//<%IDS_NtPerfTest_35%>
}
extern "C" __declspec(dllexport) BOOL  GetDirInfo(CStringList& lstParas,char * szRet )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CString strDirPath=_T("");
	strDirPath=GetValueFromList("_dirpath",lstParas);
	
	CString strMatch=_T("");
	strMatch=GetValueFromList("_match",lstParas);
	
	CString strRet=_T("");
	strRet = GetDirInfobyCom(strDirPath, strMatch);

//	DebugInfo(strRet);
	if (strRet=="")
	{
		sprintf(szRet,"error=%s",FuncGetStringFromIDS("<%IDS_NtPerfTest_37%>"));//<%IDS_NtPerfTest_37%>
		return FALSE;
	}
	else{
		sprintf(szRet,"dir:%s",strRet);

		return TRUE;
	}
}

extern "C" __declspec(dllexport) BOOL SYSTEMDSN(CStringList &paramList, CStringList &lstTexts, CStringList &lstValues)
{	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HKEY hKey = NULL;
	LONG lRet = NULL;	
	lRet = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\ODBC\\ODBC.INI\\ODBC Data Sources", 0, KEY_QUERY_VALUE , &hKey);
	if (lRet == ERROR_SUCCESS) {
		// TODO: 
		DWORD dwIndex = 0;
		while (1) {
			char szValueName[512] = {0};
			DWORD dwSizeValue = sizeof(szValueName) - 1;
			
			lRet = RegEnumValue(hKey, dwIndex, szValueName, &dwSizeValue, NULL, NULL, NULL, NULL);				
			if (lRet != ERROR_SUCCESS)
				break;

			lstTexts.AddTail(szValueName);
			lstValues.AddTail(szValueName);
			dwIndex++;
		}
		RegCloseKey(hKey);
	}

	return TRUE;
	
}
/////////////////////////////////////////////////////////////////////////////////////////////////
//for NT4.0
extern "C" _declspec(dllexport) BOOL  GetProcessInfoNT40(CStringList& lstParas,char * szRet )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strMachineName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);
	CString m_DosCommandLine=_T("");
	CString strDisk=_T("");
	strDisk=GetValueFromList("_monitorProcessList",lstParas);
	if (strDisk=="")
	{
		sprintf(szRet,"error=%s",FuncGetStringFromIDS("<%IDS_NtPerfTest_22%>"));//<%IDS_NtPerfTest_22%>
		return FALSE;
	}

	CString strRet=_T("");
	strRet=GetProcessInfobyComNT40(strMachineName,strUserAccount,strPassWord,strDisk);

	if (strRet=="")
	{
		sprintf(szRet,"error=%s",FuncGetStringFromIDS("<%IDS_NtPerfTest_23%>"));
		//<%IDS_NtPerfTest_23%>
		return FALSE;
	}
	else{
		sprintf(szRet,"%s",strRet);
		return TRUE;
	}

}

CString GetProcessInfobyComNT40(CString strServer, CString strUser,CString strPwd, CString strDisk)
{
	CoInitialize(NULL);
	_dragonflow *myRef=NULL;

	HRESULT hr =0;		
	myRef  =ThreeCreateDragonCom();
	if (NULL==myRef  )  hr=-200;

	if (SUCCEEDED(hr))
	{

		VARIANT  a,b,c,d,vResult;
		BSTR bstrTmp;
		VariantInit(&a);
		VariantInit(&b);
		VariantInit(&c);
		VariantInit(&vResult);
		
		bstrTmp=strServer.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;
		
		bstrTmp=strUser.AllocSysString();
		b.bstrVal=bstrTmp;
		b.vt=VT_BSTR;

		//strServer=_T("dragonxu");
		bstrTmp=strPwd.AllocSysString();
		c.bstrVal=bstrTmp;
		c.vt=VT_BSTR;

		bstrTmp=strDisk.AllocSysString();
		d.bstrVal=bstrTmp;
		d.vt=VT_BSTR;

		//myRef->getServiceStatus(&a,&b,&c,&d,&vResult);

		myRef->getProcessInfo(&a,&b,&c,&d,&vResult);

		CString strResult(vResult.bstrVal);

		myRef->Release();
		myRef = NULL;

		CoUninitialize();
		return strResult;
	}

	CoUninitialize();
	return FuncGetStringFromIDS("<%IDS_NtPerfTest_35%>");//<%IDS_NtPerfTest_35%>
}

extern "C" _declspec(dllexport) BOOL  EnumProcessInfoNT40(CStringList& lstParas, CStringList& lstText, CStringList& lstValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strMachineName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);
	CString strRet=_T("");
	strRet=GetAllProcessbyComNT40(strMachineName,strUserAccount,strPassWord);

	if (strRet=="")
	{
	}
	else {
		CreateListByStringandMark(strRet,lstValue,"?");
		//because process no name
		POSITION pos=NULL;
		pos=lstValue.GetHeadPosition();
		while(pos!=NULL)
		{
			lstText.AddTail(lstValue.GetNext(pos));
		}

	}

	return TRUE;
	
}

CString GetAllProcessbyComNT40(CString strServer, CString strUser,CString strPwd )
{
	CoInitialize(NULL);
	_dragonflow *myRef=NULL;

	HRESULT hr =0;		
	myRef  =ThreeCreateDragonCom();
	if (NULL==myRef  )  hr=-200;

	if (SUCCEEDED(hr))
	{
		VARIANT  a,b,c,vResult;
		BSTR bstrTmp;
		VariantInit(&a);
		VariantInit(&b);
		VariantInit(&c);
		VariantInit(&vResult);
		
		
		bstrTmp=strServer.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;
		
		
		bstrTmp=strUser.AllocSysString();
		b.bstrVal=bstrTmp;
		b.vt=VT_BSTR;

		
		bstrTmp=strPwd.AllocSysString();
		c.bstrVal=bstrTmp;
		c.vt=VT_BSTR;
		
		myRef->getProcessNames(&a,&b,&c,&vResult);
		CString strResult(vResult.bstrVal);

		myRef->Release();
		myRef = NULL;

		CoUninitialize();
		return strResult;
	}

	CoUninitialize();
	return FuncGetStringFromIDS("<%IDS_NtPerfTest_34%>");//<%IDS_NtPerfTest_34%>
}

extern "C" _declspec(dllexport) BOOL GetAllCPURateNT40(CStringList& lstParas, char* szRet)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	int nFlag = 0;
	nFlag = IfGetFromRegsitry(lstParas);
	
	CString strGroupID = _T(""), strMonitorID = _T("");
	strGroupID = GetValueFromList("_GroupID",lstParas);
	strMonitorID = GetValueFromList("_MonitorID",lstParas);
	CString strCustomer = _T("");
	strCustomer = GetValueFromList("_CustomerPath",lstParas);

	if(nFlag == 0)
	{
		CString strMachineName=_T("");
		CString strUserAccount=_T("");
		CString strPassWord=_T("");
		GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);
		
		CString strRet=_T("");
		strRet=GetCPUInfobyComNT40(strMachineName,strUserAccount,strPassWord);

		if (strRet=="")
		{
			//strRet="error=不能采集数据$";
			sprintf(szRet,"error=%s",FuncGetStringFromIDS("SV_NT_CPU",
                "NT_CPU_COLLET_DATA_FAILED"));//<%IDS_NtPerfTest_06%>
			return FALSE;
		}
		else{
			sprintf(szRet,"%s",strRet);
			return TRUE;
		}
	}
	else if(nFlag == 2)
	{
		sprintf(szRet, "error=%s", FuncGetStringFromIDS("SV_NT_CPU",
            "NT_CPU_COM_INIT_FAILED"));//<%IDS_NtPerfTest_36%>
		return FALSE;
	}
	else
	{
		CString strMachineName=_T("");
		CString strUserAccount=_T("");
		CString strPassWord=_T("");
		
		BOOL ret=TRUE; 
		
		ret=LoginMachine(lstParas,strMachineName);
		if (ret==FALSE){
			GetErrorText(-1 , szRet);
			return ret;
		}
		
		int nRet=-1;
		GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);
		nRet=GetCPURate(strMachineName.GetBuffer(strMachineName.GetLength()), szRet);//,nVersion);

		CTime MyTime;
		MyTime = CTime::GetCurrentTime();
		CString strCurrentDate = _T("");			
		strCurrentDate = MyTime.Format("%Y-%m-%d %H:%M:%S");
		CString strAllOutPut = _T("");
		strAllOutPut.Format("%s\t%s", strCurrentDate, szRet);

		if (nRet<0)
		{
			GetErrorText(nRet,szRet);
			return FALSE;
		}
		else
			return TRUE;
	}
}

CString GetCPUInfobyComNT40(CString strServer, CString strUser,CString strPwd )
{
	CoInitialize(NULL);
	_dragonflow *myRef=NULL;

	HRESULT hr =0;		
	myRef  =ThreeCreateDragonCom();
	if (NULL==myRef  )  hr=-200;

	if (SUCCEEDED(hr))
	{
		VARIANT  a,b,c,vResult;
		BSTR bstrTmp;
		VariantInit(&a);
		VariantInit(&b);
		VariantInit(&c);
		VariantInit(&vResult);
		
		//CString strServer=_T("dragonxu");
		bstrTmp=strServer.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;
		
		//strServer=_T("administrator");
		bstrTmp=strUser.AllocSysString();
		b.bstrVal=bstrTmp;
		b.vt=VT_BSTR;

		//strServer=_T("dragonxu");
		bstrTmp=strPwd.AllocSysString();
		c.bstrVal=bstrTmp;
		c.vt=VT_BSTR;

		myRef->getCPUNT(&a,&b,&c,&vResult);
		CString strResult(vResult.bstrVal);
		
		myRef->Release();

		CoUninitialize();
		return strResult;
	}

	CString strResult=_T("");
	strResult.Format("error=%s",FuncGetStringFromIDS("NT_CPU","NT_CPU_COM_INIT_FAILED"));
	//<%IDS_NtPerfTest_26%>

	CoUninitialize();
	return strResult;
}

CString getASPNetRequestReturnValues(CString str)
{
	if(str.Find("error=") != -1)
	{
		CString s1 = str;
		return s1;
	}

	CString sss = "$";
	CString ss[4] = {"Caption","Description","Name","RequestsCurrent"};
	

	CString str1,str2,strtmp;
	CString smid = "";

	int n = 4;
	int m,l;
	strtmp = str;
	for(int i=0;i<n;i++)
	{
		smid = strtmp;

		m = strtmp.Find(ss[i]);
		if(m != -1)
		{
			str1 = strtmp.Left(m);

			l = strtmp.Find(sss,m);
			str2 = strtmp.Mid(l+1);

			strtmp = str1 + str2;
		}
		else
		{
			strtmp = smid;
		}

		//l = strtmp.Find(sss,m);
		//str2 = strtmp.Mid(l+1);
	}

	return strtmp;
}


//////////////////////////////////////////////////////////////////////////////////////////////
CString GetAspNetInfoByCom(CString strServer,CString strUser,CString strPwd)
{
	CoInitialize(NULL);
	_dragonflow *myRef=NULL;
		
	HRESULT hr =0;		
	myRef  =ThreeCreateDragonCom();
	if (NULL==myRef  )  hr=-200;

	if (SUCCEEDED(hr))
	{
		VARIANT  a,b,c,vResult;
		BSTR bstrTmp;
		VariantInit(&a);
		VariantInit(&b);
		VariantInit(&c);
		VariantInit(&vResult);
		
		//CString strServer=_T("dragonxu");
		bstrTmp=strServer.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;
		
		//strServer=_T("administrator");
		bstrTmp=strUser.AllocSysString();
		b.bstrVal=bstrTmp;
		b.vt=VT_BSTR;

		//strServer=_T("dragonxu");
		bstrTmp=strPwd.AllocSysString();
		c.bstrVal=bstrTmp;
		c.vt=VT_BSTR;

		myRef->getASPNET(&a,&b,&c,&vResult);
		CString strTmpResult(vResult.bstrVal);
		//CString strResult(vResult.bstrVal);

		//printf("Likeham in1:%s\n",strTmpResult);

		//new
		CString strResult = getASPNetRequestReturnValues(strTmpResult);
		
		//printf("Likeham in2:%s\n",strResult);

		myRef->Release();
		

		CoUninitialize();

		//if(strResult.Find("$") == -1)
		//	strResult += "$";

		//strResult += "$";
		//printf("Likeham in:%s\n",strResult);

		//strResult += "$";

		return strResult;
	}
	
	CoUninitialize();
	CString strResult=_T("");
	strResult.Format("error=%s",FuncGetStringFromIDS("<%IDS_NtPerfTest_27%>"));
	//strResult.Format("%s$",FuncGetStringFromIDS("<%IDS_NtPerfTest_27%>"));

	//printf("Likeham out:%s\n",strResult);

	//if(strResult.Find("$") == -1)
	//		strResult += "$";

	return strResult;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
CString GetAspNetApplicationInfoByCom(CString strServer,CString strUser,CString strPwd)
{
	CoInitialize(NULL);
	_dragonflow *myRef=NULL;

	HRESULT hr =0;		
	myRef  =ThreeCreateDragonCom();
	if (NULL==myRef  )  hr=-200;

	if (SUCCEEDED(hr))
	{
		VARIANT  a,b,c,vResult;
		BSTR bstrTmp;
		VariantInit(&a);
		VariantInit(&b);
		VariantInit(&c);
		VariantInit(&vResult);
		
		//CString strServer=_T("dragonxu");
		bstrTmp=strServer.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;
		
		//strServer=_T("administrator");
		bstrTmp=strUser.AllocSysString();
		b.bstrVal=bstrTmp;
		b.vt=VT_BSTR;

		//strServer=_T("dragonxu");
		bstrTmp=strPwd.AllocSysString();
		c.bstrVal=bstrTmp;
		c.vt=VT_BSTR;

		myRef->getASPnetApplication(&a,&b,&c,&vResult);
		CString strResult(vResult.bstrVal);
		
		myRef->Release();
		

		CoUninitialize();
		return strResult;
	}
	
	CoUninitialize();
	CString strResult=_T("");
	strResult.Format("error=%s",FuncGetStringFromIDS("<%IDS_NtPerfTest_27%>"));

	return strResult;
}


BOOL AddToExceptionLog(const char* szMsg)
{
	FILE *logfile = NULL;

	CString strFileName = _T("");
	strFileName.Format("d:\\ntperf-exception.xlog");
	
	logfile=fopen((LPCTSTR)strFileName,"a+");

	if (!logfile)
		return FALSE;

	fprintf(logfile,"%s\r\n",szMsg);

	fclose(logfile);
	return TRUE;
}

CString GetCPUMemoryDiskNetworkByCom(CString strServer,CString strUser,CString strPwd)
{
		CoInitialize(NULL);
	_clsWMI *myRef=NULL;
	HRESULT hr =0;
	myRef  =ThreeCreateSVCom();
	if (NULL==myRef  )  hr=-200;
	//hr=-200;
	//HRESULT hr=CoCreateInstance(CLSID_clsWMI,NULL,
//							CLSCTX_ALL,
//							IID__clsWMI,(void **)&myRef);
	if (SUCCEEDED(hr))
	{
		VARIANT  a,b,c,vResult;
		BSTR bstrTmp;
		VariantInit(&a);
		VariantInit(&b);
		VariantInit(&c);
		VariantInit(&vResult);
		
		//CString strServer=_T("dragonxu");
		bstrTmp=strServer.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;
		
		//strServer=_T("administrator");
		bstrTmp=strUser.AllocSysString();
		b.bstrVal=bstrTmp;
		b.vt=VT_BSTR;

		//strServer=_T("dragonxu");
		bstrTmp=strPwd.AllocSysString();
		c.bstrVal=bstrTmp;
		c.vt=VT_BSTR;

		//AddToExceptionLog("com ok");

		myRef->getCPUMemoryDiskNetwork(&a,&b,&c,&vResult);
		CString strResult(vResult.bstrVal);
	
	
		if(strResult == "")
		{
			AddToExceptionLog("com return null");

			srand(1);
			div_t div_result;
			div_result=div(rand() , 10);
			Sleep((5+div_result.rem)*1000);
			myRef->getCPUMemoryDiskNetwork(&a,&b,&c,&vResult);
			CString strstrResult(vResult.bstrVal);
			strResult=strstrResult;

			
		}


		myRef->Release();
		CoUninitialize();

		return strResult;
	}
	
	CoUninitialize();
	CString strResult=_T("");
	strResult.Format("error=%s",FuncGetStringFromIDS("SV_NT_CPU",
        "NT_CPU_COM_INIT_FAILED"));

	return strResult;
}

extern "C" _declspec(dllexport) BOOL GetCPUMemoryDiskNetworkInfo(CStringList& lstParas, char* szRet)
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strMachineName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);
	CString m_DosCommandLine=_T("");

	CString strRet=_T("");
	strRet=GetCPUMemoryDiskNetworkByCom(strMachineName,strUserAccount,strPassWord);
	
	if (strRet=="")
	{
		//sprintf(szRet,FuncGetStringFromIDS("<%IDS_NtPerfTest_02%>"));//<%IDS_NtPerfTest_02%>
		return FALSE;
	}
	else{
		sprintf(szRet,"%s",strRet);
		return TRUE;
	}

		
}
// Add by juwu.zhang 2007-07-25
CString GetProcessesbyCom(CString strServer, CString strUser,CString strPwd )
{
	CoInitialize(NULL);
	_dragonflow *myRef=NULL;

	HRESULT hr =0;		
	myRef  =ThreeCreateDragonCom();
	if (NULL==myRef  )  hr=-200;



	if (SUCCEEDED(hr))
	{
		VARIANT  a,b,c,vResult;
		BSTR bstrTmp;
		VariantInit(&a);
		VariantInit(&b);
		VariantInit(&c);
		VariantInit(&vResult);
		
		//CString strServer=_T("dragonxu");
		bstrTmp=strServer.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;
		
		//strServer=_T("administrator");
		bstrTmp=strUser.AllocSysString();
		b.bstrVal=bstrTmp;
		b.vt=VT_BSTR;

		//strServer=_T("dragonxu");
		bstrTmp=strPwd.AllocSysString();
		c.bstrVal=bstrTmp;
		c.vt=VT_BSTR;

		myRef->getProcesses(&a,&b,&c,&vResult);
		CString strResult(vResult.bstrVal);
		
		myRef->Release();

		VariantClear(&a);
		VariantClear(&b);
		VariantClear(&c);
		VariantClear(&vResult);

		CoUninitialize();
		return strResult;
	}

	CString strResult=_T("error=Create com failed!");
	return strResult;
}

// Add by juwu.zhang 2007-07-25
extern "C" _declspec(dllexport) BOOL GetProcesses(const char * strParas, char * szRet, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strMachineName=_T(""),
		    strUserAccount=_T(""),
			strPassWord=_T(""),
			strRet = _T("");
	CStringList lstParas ;
	MakeStringListByChar(lstParas,strParas);

	GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);
	strRet=GetProcessesbyCom(strMachineName,strUserAccount,strPassWord);
	if (strRet=="")
	{
		//strRet="error=不能采集数据$";
		sprintf(szRet,"error=%s",FuncGetStringFromIDS("<%NtPerfTest_06%>"));
		return FALSE;
	}
	else{
		sprintf(szRet,"%s",strRet);
		CString strInput;
		strInput =szRet;
		MakeCharByString(szRet,nSize,strInput);
		return TRUE;
	}
}
/*
extern "C" __declspec(dllexport)
BOOL MonitorFile(CStringList &lstParas, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	BOOL bRet = TRUE;
	CString strMachineName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);
	CString strFilePath=_T("");
	strFilePath=GetValueFromList("_filepath",lstParas);
	if(strFilePath.IsEmpty())
	{
		sprintf(szReturn, "error=File name is empty$");
		bRet = TRUE;
	}
	strFilePath.Replace("\\", "\\\\");

	_sv_xfdl *myRef = ThreeCreateXFDLCom();
	if(myRef != NULL)
	{
		BSTR  a,b,c,d,vResult;
		
		a=strMachineName.AllocSysString();
		b=strUserAccount.AllocSysString();
		c=strPassWord.AllocSysString();
		d=strFilePath.AllocSysString();
		if(strMachineName.CompareNoCase("localhost") == 0)
			myRef->MonitorFile(NULL, NULL, NULL, &d, &vResult);
		else
			myRef->MonitorFile(&a, &b, &c, &d, &vResult);
		CString strResult(vResult);
		if(strResult.IsEmpty())
		{
			sprintf(szReturn, "error=Unknown error(COM)$");
			bRet = FALSE;
		}
		else
		{
			sprintf(szReturn , "%s", strResult.GetBuffer(strResult.GetLength()));
			bRet = TRUE;
		}
		myRef->Release();
		CoUninitialize();
	}
	return bRet;
}

extern "C" __declspec(dllexport)
BOOL MonitorDirectory(CStringList &lstParas, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	BOOL bRet = TRUE;
	CString strMachineName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	GetMachineInfo( lstParas,strMachineName,strUserAccount,strPassWord);
	CString strFilePath=_T("");
	strFilePath=GetValueFromList("_dirpath",lstParas);
	if(strFilePath.IsEmpty())
	{
		sprintf(szReturn, "error=Directory is NULL$");
		bRet = TRUE;
	}
	strFilePath.Replace("\\", "\\\\");

	_sv_xfdl *myRef = ThreeCreateXFDLCom();
	if(myRef != NULL)
	{
		BSTR  a,b,c,d,vResult;
	
		a=strMachineName.AllocSysString();
		b=strUserAccount.AllocSysString();
		c=strPassWord.AllocSysString();
		d=strFilePath.AllocSysString();

		if(strMachineName.CompareNoCase("localhost") == 0)
			myRef->MonitorDirectory(NULL, NULL, NULL, &d, &vResult);
		else
			myRef->MonitorDirectory(&a, &b, &c, &d, &vResult);
		CString strResult(vResult);
		if(strResult.IsEmpty())
		{
			sprintf(szReturn, "error=Unknown error(COM)$");
			bRet = FALSE;
		}		
		else
		{
			sprintf(szReturn , "%s", strResult.GetBuffer(strResult.GetLength()));
			bRet = TRUE;
		}
		myRef->Release();
		CoUninitialize();
	}
	return bRet;
}
*/