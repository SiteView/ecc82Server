#include <string>
#include <list>
#include <iostream>
//#include <windows.h>
#include <afxwin.h>
#include <afxole.h> 

using namespace std;

#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#include <wbemcli.h>
#import "progid:WbemScripting.SWbemLocator" named_guids

#define _CRTDBG_MAP_ALLOC 
#include<stdlib.h> 
#include<crtdbg.h>

typedef class std::list<char*> StringList;


#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

void WriteLog( const char* str )
{
	char timebuf[128],datebuf[128];

	_tzset();
	_strtime( timebuf );
	_strdate( datebuf );

	char szLogFile[] = "wmiWatcher.log";

	/*
	// 判断文件大小：在不打开文件的情况下实现
	struct _stat buf;
	if( _stat( szLogFile, &buf ) == 0 )
	{
		if( buf.st_size > 10*1024 )
		{
			FILE *log = fopen( szLogFile, "w" );
			if( log != NULL )
				fclose( log );
		}
	}
	*/


	FILE *log = fopen( szLogFile,"a+");
	if( log != NULL )
	{
		fprintf( log, "%s %s \t%s\n", datebuf, timebuf, str );
		fclose( log );
	}

}


bool MakeStringListByChar(StringList& pList, const char * pInput )
{
	const char * p;
	int nSize;
	p=pInput;
	while(*p!='\0')
	{
		nSize =static_cast<int>((strlen(p)));
		if( nSize>0 )
		{	
			//pList.AddHead(p);
			pList.push_back((char*)p);

		}
		p=p+nSize+1;
	}

	return true;
}

int GetCharLength(const char * pInput)
{
	const char * p;
	int nSize = 0;
	p=pInput;
	while(*p!='\0')
	{
		nSize += static_cast<int>(strlen(p)) + 1;
		p += strlen(p)+1;
	}

	 return nSize;
}


bool MakeCharByString(char *pOut,int &nOutSize,CString strInput )
{
	 char *p;
	
	int nSize=strInput.GetLength();
	if(nSize+2 <nOutSize)
	{
		strcpy(pOut,strInput.GetBuffer(strInput.GetLength()));
	}else return false;
	p=pOut;
	//printf("%d\n",nSize);23028830 13602067678 王波
	for(int i=0;i<nSize;i++)
	{
		if(*p=='$') 	
			*p='\0';
		p++;
	}
	nOutSize=nSize+1;
	return true;
	
}


char *FindStrValue(const char *strParas, CString str)
{
	char *pValue = NULL;
	string m_TempStr;

	std::list<char*> strList;
	MakeStringListByChar(strList, strParas);
	std::list<char *>::iterator pos = strList.begin();

	 while(pos != strList.end())
	{
		//CString strTemp = strList.GetNext(pos);
		char * strTemp = *pos;
		std::string strTemp1 = *pos;
		int m_Fpos = 0;
		
		if((m_Fpos = static_cast<int>(strTemp1.find(str, 0))) >= 0)
		{
			m_TempStr = strTemp1.substr( m_Fpos + strlen(str)+1, strTemp1.size() - strlen(str) - 1); 
			pValue=(char*)malloc(m_TempStr.size()+1);
			strcpy(pValue, m_TempStr.c_str());
			
		}
		pos++;
	}

	return pValue;
	
}

int GetConfirms(CString strMonitorID)
{
	CString strFileName, strDate;
	strFileName.Format("..\\data\\TmpIniFile\\Confirms_%s.ini", strMonitorID);
	strDate = COleDateTime::GetCurrentTime().Format("%Y%m%d");
	int nRet = GetPrivateProfileInt(strDate, "Times", -1, strFileName);
	if(nRet == -1)
	{
		WritePrivateProfileString(strDate, "Times", "0", strFileName);
		return 0;
	}

	return nRet;
}

BOOL IsLocalHost(CString sHostName)
{
	sHostName.TrimRight();
	sHostName.MakeLower();
	if(sHostName == "127.0.0.1" || sHostName == "localhost")
		return TRUE;

	WSADATA wsaData;
	char name[155];
	char *ip;
	PHOSTENT hostinfo; 
	if ( WSAStartup( MAKEWORD(2,0), &wsaData ) == 0 ) 
	{ 
		if( gethostname ( name, sizeof(name)) == 0) 
		{ 
			if(sHostName == name)
				return TRUE;
			if((hostinfo = gethostbyname(name)) != NULL) 
			{ //这些就是获得IP的函数
				ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list); 
//				 ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list[i]);

				if(sHostName == ip)
				{
					return TRUE;
				}
			} 
		} 
		WSACleanup( );
	}
	return FALSE;
}

BOOL ConnectServer(const char * strParas, char * szReturn, int& nSize,WbemScripting::ISWbemServicesPtr &services)
{
	char *machinename=NULL, *user=NULL, *pswd=NULL;
	char szMachineName[128] = {0};
	char szUser[128] = {0};
	char szPswd[128] = {0};

	char szCommand[256] = {0};
	const char* pszInput = strParas;
	while( *pszInput )
	{
		machinename = strstr( pszInput, "_MachineName" );
		if( machinename != NULL )
		{
			strcpy( szMachineName, machinename+strlen("_MachineName")+1 );
		}

		user = strstr( pszInput, "_UserAccount" );
		if( user != NULL )
		{
			strcpy( szUser, user+strlen("_UserAccount")+1 );
		}

		pswd = strstr( pszInput, "_PassWord" );
		if( pswd != NULL )
		{
			strcpy( szPswd, pswd+strlen("_PassWord")+1 );
		}

		pszInput += strlen( pszInput ) + 1;
	}

	WriteLog(szMachineName);
	WriteLog(szUser);
	WriteLog(szPswd);

	try 
	{ 
		WbemScripting::ISWbemLocatorPtr locator; 
		locator.CreateInstance(WbemScripting::CLSID_SWbemLocator);

		if (locator != NULL) 
		{ 
			if(IsLocalHost(szMachineName))
			{
				services = locator->ConnectServer(".","root\\cimv2","","","","",0x80,NULL);
			}
			else
			{
				services = locator->ConnectServer(szMachineName,"root\\cimv2",szUser,szPswd,"","",0x80,NULL);
			}

			locator.Release();
		}

		if( services != NULL )
		{
			WriteLog("连接成功！");
			return true;
		}
		else
		{
			sprintf(szReturn, "error=connect timeout: %d",::GetLastError() );
			return false;
		}
	}

	catch (_com_error err) 
	{ 
		IErrorInfo * ei = err.ErrorInfo();
		BSTR strDesEI;
		ei->GetDescription(&strDesEI);
		char* pstr = _com_util::ConvertBSTRToString(strDesEI);
		sprintf(szReturn, "error=Error ocured:%x: %s", (unsigned)err.Error(), pstr );

		ei->Release();
		free( pstr );
		::SysFreeString( strDesEI );

		return FALSE;
	} 
	catch(...) 
	{ 
		sprintf(szReturn, "error=Error ocured: %d",::GetLastError());
		return FALSE;
	} 
}

BOOL ConnectADServer(const char * strParas, char * szReturn, int& nSize,WbemScripting::ISWbemServicesPtr &services)
{

	char *machinename=NULL, *user=NULL, *pswd=NULL;

	machinename = FindStrValue(strParas, "_MachineName");
	user = FindStrValue(strParas, "_UserAccount");
	pswd = FindStrValue(strParas, "_PassWord");

	//CoInitialize(NULL);

	try 
	{ 
		WbemScripting::ISWbemLocatorPtr locator; 
		locator.CreateInstance(WbemScripting::CLSID_SWbemLocator);
		
		if (locator != NULL) 
		{ 
			if(IsLocalHost(machinename))
				services = locator->ConnectServer(".","root\\MicrosoftActiveDirectory","","","","",0,NULL);
			else
				services = locator->ConnectServer(machinename,"root\\MicrosoftActiveDirectory",user,pswd,"","",0,NULL);
		}
	}
	catch (_com_error err) 
	{ 
		char buf[200] = {0};
		IErrorInfo * ei = err.ErrorInfo();
		BSTR strDesEI;
		ei->GetDescription(&strDesEI);
		sprintf(szReturn, "error=Error ocured:%x: %s", (unsigned)err.Error(),_com_util::ConvertBSTRToString(strDesEI));
		return FALSE;
	} 
	catch(...) 
	{ 
		sprintf(szReturn, "error=Error ocured: %d",::GetLastError());
		return FALSE;
	} 

	return TRUE;
}


extern "C" _declspec(dllexport) 
BOOL EnumDisksInfo(const char * strParas, char * szReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CoInitialize(NULL);
	{
		//CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(strParas, szReturn, nSize, services))
		{
			CoUninitialize();
			return FALSE;
		}

		try
		{
			_bstr_t com; 
			com="SELECT * FROM Win32_LogicalDisk where MediaType=12"; 
			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(com,"WQL",WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,NULL); 			
			if (NULL == objects) 
			{
				services.Release ();
				CoUninitialize ();
				return FALSE;
			}

			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
			if (NULL == obj_enum) 
			{
				services.Release ();
				objects.Release ();
				CoUninitialize ();
				return FALSE;
			}

			ULONG fetched; 
			VARIANT var; 
			//CString strTemp;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("DeviceID",0);
							
				//strTemp.Format("%s=%s$", (const char*)_bstr_t(prop->GetValue()), (const char*)_bstr_t(prop->GetValue()));
				//strReturn += strTemp;

				sprintf( szReturn, "%s%s=%s$", szReturn, (const char*)_bstr_t(prop->GetValue()), (const char*)_bstr_t(prop->GetValue()) );

				VariantClear (&var);
				object.Release ();
				properties.Release ();
				prop.Release ();

			}

			if( NULL != obj_enum )
				obj_enum.Release ();
			if( NULL != objects ) 
				objects.Release ();
			if( NULL != services)
				services.Release();

		}
		catch (_com_error err) 
		{ 
			IErrorInfo * ei = err.ErrorInfo();
			BSTR strDesEI;
			ei->GetDescription(&strDesEI);
			char* pstr = _com_util::ConvertBSTRToString(strDesEI);
			sprintf(szReturn, "error=Error ocured:%x: %s", (unsigned)err.Error(), pstr );

			ei->Release();
			free( pstr );
			::SysFreeString( strDesEI );

			if( services != NULL )
				services.Release();

			CoUninitialize();
			return FALSE;
		} 
		catch(...) 
		{ 
			sprintf(szReturn, "error=Error ocured: %d",::GetLastError());
			OutputDebugString(szReturn);

			if( services != NULL )
				services.Release();

			CoUninitialize();
			return FALSE;
		} 

		/*
		strcpy(szReturn,strReturn);
		CString strOutRet;
		strOutRet =szReturn;
		nSize = 2048;
		MakeCharByString(szReturn,nSize,strOutRet);	
		*/

		nSize=strlen(szReturn);
		for( int i=0; i!=nSize; i++ )
		{
			if( szReturn[i] == '$' )
				szReturn[i] = '\0';
		}
		
		


	}
	CoUninitialize();

	return TRUE;
}


extern "C" _declspec(dllexport) 
BOOL GetNTServices(const char * strParas, char * szReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CoInitialize(NULL);
	{
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(strParas, szReturn, nSize, services))
		{
			CoUninitialize();
			return FALSE;
		}
		try
		{
			WbemScripting::ISWbemObjectSetPtr objects = services->InstancesOf("Win32_Service", WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL);
			if (NULL == objects) 
			{
				services.Release ();
				CoUninitialize ();
				return FALSE;
			}

			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
			if (NULL == obj_enum) 
			{
				services.Release ();
				objects.Release ();
				CoUninitialize ();
				return FALSE;
			}

			ULONG fetched; 
			VARIANT var; 
			CString strTemp;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("DisplayName",0);
				strTemp.Format("%s=%s$", (const char*)_bstr_t(prop->GetValue()), (const char*)_bstr_t(prop->GetValue()));
				strReturn += strTemp;

				VariantClear (&var);
				object.Release ();
				properties.Release ();
				prop.Release ();

			}

			if( NULL != obj_enum )
				obj_enum.Release ();
			if( NULL != objects ) 
				objects.Release ();
			if( NULL != services)
				services.Release();

		}
		catch (_com_error err) 
		{ 
			IErrorInfo * ei = err.ErrorInfo();
			BSTR strDesEI;
			ei->GetDescription(&strDesEI);
			char* pstr = _com_util::ConvertBSTRToString(strDesEI);
			sprintf(szReturn, "error=Error ocured:%x: %s", (unsigned)err.Error(), pstr );

			ei->Release();
			free( pstr );
			::SysFreeString( strDesEI );

			if( services != NULL )
				services.Release();

			CoUninitialize();
			return FALSE;
		} 
		catch(...) 
		{ 
			sprintf(szReturn, "error=Error ocured: %d",::GetLastError());
			OutputDebugString(szReturn);

			if( services != NULL )
				services.Release();

			CoUninitialize();
			return FALSE;
		} 

		strcpy(szReturn,strReturn);
		CString strOutRet;
		strOutRet =szReturn;
		nSize = 1048576;
		MakeCharByString(szReturn,nSize,strOutRet);	 
	}
	CoUninitialize();

	return TRUE;
}


extern "C" _declspec(dllexport) 
BOOL FilterEventLogInfo(const char * strParas, char * szReturn, int& nSize)
{
	CoInitialize(NULL);
	{
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(strParas, szReturn, nSize, services))
		{
			CoUninitialize();
			return FALSE;
		}
		try
		{
			CString strLogName="", strType="", strCodeList="", strSourceList="", strEventMachine="", strOnce="", strMonitorID="";

			char* pszLogName = FindStrValue(strParas, "_logName");
			if( pszLogName != NULL )
			{
				strLogName = pszLogName;
				strLogName.TrimRight();
				free( pszLogName );
			}
			

			char* pszType = FindStrValue(strParas, "_eventType");
			if( pszType != NULL )
			{
				strType = pszType;
				strType.TrimRight();
				free( pszType );
			}
			
			char* pszCodeList = FindStrValue(strParas, "_codeList");
			if( pszCodeList != NULL )
			{
				strCodeList = pszCodeList;
				strCodeList.TrimRight();
				free( pszCodeList );
			}
			
			char* pszSourceList = FindStrValue(strParas, "_sourceList");
			if( pszSourceList != NULL )
			{
				strSourceList = pszSourceList;
				strSourceList.TrimRight();
				free( pszSourceList );				
			}
			
			char* pszEventMachine = FindStrValue(strParas, "_eventMachine");
			if( pszEventMachine != NULL )
			{
				strEventMachine = pszEventMachine;
				strEventMachine.TrimRight();
				free( pszEventMachine );

			}
			
			char* pszMonitorID = FindStrValue(strParas, "_MonitorID");
			if( pszMonitorID != NULL )
			{
				strMonitorID  = pszMonitorID;
				strMonitorID.TrimRight();
				free( pszMonitorID );
			}
			
			char* pszOnce = FindStrValue(strParas, "_monitorcondition");
			if( pszOnce != NULL )
			{
				strOnce  = pszOnce;
				strOnce.TrimRight();
				free( pszOnce );
			}
			
			
			CString strCom, strTemp;;
			strCom.Format("SELECT * FROM Win32_NTLogEvent where Logfile='%s'", strLogName);
			switch(atoi(strType))
			{
				case 2: 
					strCom += " and (Type='error' or Type='错误') ";
					break;
				case 3: 
					strCom += " and (Type='warning' or Type='警告') ";
					break;
				case 4: 
					strCom += " and (Type='warning' or Type='error' or Type='错误' or Type='警告') ";
					break;
				case 5: 
					strCom += " and (Type='information' or Type='信息') ";
					break;
			}

			CTime curTime = CTime::GetCurrentTime();
			//curTime -= 60*60*24; //one day ago
			strTemp.Format("and TimeWritten>='%s.000000+480'", curTime.Format("%Y%m%d000000"));
			strCom += strTemp;

			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(LPCTSTR(strCom),"WQL",WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,NULL); 
			if (NULL == objects) 
			{
				services.Release ();
				CoUninitialize ();
				return FALSE;
			}

			int nTotalEventCount = objects->Count;

			if(strCodeList != "")
			{
				strTemp.Format(" and EventCode<>'%s'", strCodeList);
				strTemp.Replace(";", "' and EventCode<>'");
				strCom += strTemp;
			}
			if(strSourceList != "")
			{
				strTemp.Format(" and SourceName<>'%s'", strSourceList);
				strTemp.Replace(";", "' and SourceName<>'");
				strCom += strTemp;
			}
			if(strEventMachine != "")
			{
				strTemp.Format(" and ComputerName='%s'", strEventMachine);
				strCom += strTemp;
			}


			if( NULL != objects ) 
			{
				objects.Release ();
				objects = NULL;
			}

			objects = services->ExecQuery(LPCTSTR(strCom),"WQL",WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,NULL); 
			if (NULL == objects) 
			{
				services.Release ();
				CoUninitialize ();
				return FALSE;
			}

			int nFilterEventCount = objects->Count;

			int nConirms = GetConfirms(strMonitorID);
			if(nFilterEventCount < nConirms)
				nFilterEventCount = 0;
			else
				nFilterEventCount -= nConirms;

			strReturn.Format("checkcount=%d$filtercount=%d$", nTotalEventCount, nFilterEventCount);

			//int nState;  //  for new logic   stop monitor as soon as error state
			//std::string strDyn;
			//if(atoi(strOnce) != 3)
			//	GetDyn(LPCTSTR(strMonitorID),nState,strDyn);

			if( NULL != objects ) 
				objects.Release ();
			if( NULL != services)
				services.Release();

		}
		catch (_com_error err) 
		{ 
			IErrorInfo * ei = err.ErrorInfo();
			BSTR strDesEI;
			ei->GetDescription(&strDesEI);
			char* pstr = _com_util::ConvertBSTRToString(strDesEI);
			sprintf(szReturn, "error=Error ocured:%x: %s", (unsigned)err.Error(), pstr );

			ei->Release();
			free( pstr );
			::SysFreeString( strDesEI );

			if( services != NULL )
				services.Release();

			CoUninitialize();
			return FALSE;
		} 
		catch(...) 
		{ 
			sprintf(szReturn, "error=Error ocured: %d",::GetLastError());
			OutputDebugString(szReturn);

			if( services != NULL )
				services.Release();

			CoUninitialize();
			return FALSE;
		} 

		strcpy(szReturn,strReturn);
		CString strOutRet;
		strOutRet =szReturn;
		nSize = 2048;
		MakeCharByString(szReturn,nSize,strOutRet);	 

		
	}
	CoUninitialize();

	return TRUE;
}


extern "C" _declspec(dllexport) 
BOOL EnumProcessInfo(const char * strParas, char * szReturn, int& nSize)
{
	WriteLog("============ EnumProcessInfo ===========");
	CoInitialize(NULL);
	{
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(strParas, szReturn, nSize, services))
		{
			CoUninitialize();
			return FALSE;
		}

		try
		{
			_bstr_t com; 
			com="SELECT * FROM Win32_PerfRawData_PerfProc_Process where Name<>'_Total'"; 
			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(com,"WQL",WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,NULL); 		
			if (NULL == objects) 
			{
				services.Release ();
				CoUninitialize ();
				return FALSE;
			}

			WriteLog("获取进程列表成功！");

			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum();
			if (NULL == obj_enum) 
			{
				services.Release ();
				objects.Release ();
				CoUninitialize ();
				return FALSE;
			}

			ULONG fetched; 
			VARIANT var; 
			CString strTemp;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("Name",0);
				_variant_t value = prop->GetValue();
				
				strTemp.Format("%s=%s$", (const char*)_bstr_t(value), (const char*)_bstr_t(value));
				strReturn += strTemp;

				WriteLog(strReturn.GetBuffer());

				VariantClear (&var);
				object.Release ();
				properties.Release ();
				prop.Release ();
			}

			if( NULL != obj_enum )
				obj_enum.Release ();
			if( NULL != objects ) 
				objects.Release ();
			if( NULL != services)
				services.Release();

		}
		catch (_com_error err) 
		{ 
			IErrorInfo * ei = err.ErrorInfo();
			BSTR strDesEI;
			ei->GetDescription(&strDesEI);
			char* pstr = _com_util::ConvertBSTRToString(strDesEI);
			sprintf(szReturn, "error=Error ocured:%x: %s", (unsigned)err.Error(), pstr );

			ei->Release();
			free( pstr );
			::SysFreeString( strDesEI );

			if( services != NULL )
				services.Release();

			CoUninitialize();
			return FALSE;
		} 
		catch(...) 
		{ 
			sprintf(szReturn, "error=Error ocured: %d",::GetLastError());
			OutputDebugString(szReturn);

			if( services != NULL )
				services.Release();

			CoUninitialize();
			return FALSE;
		} 

		strcpy(szReturn,strReturn);
		CString strOutRet;
		strOutRet =szReturn;
		nSize = 1048576;
		MakeCharByString(szReturn,nSize,strOutRet);	

		WriteLog("监测结果：");
		WriteLog(szReturn);

	}

	CoUninitialize();

	return TRUE;
}


extern "C" _declspec(dllexport) 
BOOL GetIISInfo(const char * strParas, char * szReturn, int& nSize)
{
	CoInitialize(NULL);
	{
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(strParas, szReturn, nSize, services))
		{
			CoUninitialize();
			return FALSE;
		}
		try
		{
			//取第一次IIS服务器数据
			_bstr_t com; 
			com="SELECT * FROM Win32_PerfRawData_W3SVC_WebService where Name='_Total'"; 
			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(com,"WQL",WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,NULL); 
			if (NULL == objects) 
			{
				services.Release ();
				CoUninitialize ();
				return FALSE;
			}

			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum();
			if (NULL == obj_enum) 
			{
				services.Release ();
				objects.Release ();
				CoUninitialize ();
				return FALSE;
			}

			ULONG fetched; 
			VARIANT var; 
			int nMaximumConnections = 0,nCurrentAnonymousUsers = 0,nCurrentConnections = 0, nTotalNotFoundErrors = 0;
			float fPostRequestsPersec1 = 0, fGetRequestsPersec1 = 0, fBytesSentPerSec1 = 0, fBytesReceivedPerSec1 = 0,
				fBytesTotalPersec1 = 0, fTimestamp_PerfTime1 = 0, fFrequency_PerfTime = 0;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("PostRequestsPersec",0);
				fPostRequestsPersec1 = (float)prop->GetValue();
				prop = properties->Item("GetRequestsPersec",0);
				fGetRequestsPersec1 = (float)prop->GetValue();
				prop = properties->Item("BytesSentPerSec",0);
				fBytesSentPerSec1 = (float)prop->GetValue();
				prop = properties->Item("BytesReceivedPerSec",0);
				fBytesReceivedPerSec1 = (float)prop->GetValue();
				prop = properties->Item("BytesTotalPersec",0);
				fBytesTotalPersec1 = (float)prop->GetValue();
				prop = properties->Item("Timestamp_PerfTime",0);
				fTimestamp_PerfTime1 = (float)prop->GetValue();

				VariantClear (&var);
				object.Release ();
				properties.Release ();
				prop.Release ();
			}

			obj_enum.Release();
			obj_enum = NULL;
			objects.Release ();
			objects = NULL;

			//取第二次IIS服务器数据
			com="SELECT * FROM Win32_PerfRawData_W3SVC_WebService where Name='_Total'"; 
			objects = services->ExecQuery(com,"WQL",WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,NULL); 
			if (NULL == objects) 
			{
				services.Release ();
				CoUninitialize ();
				return FALSE;
			}

			obj_enum = objects->Get_NewEnum();
			if (NULL == obj_enum) 
			{
				services.Release ();
				objects.Release ();
				CoUninitialize ();
				return FALSE;
			}

			float fPostRequestsPersec2 = 0, fGetRequestsPersec2 = 0, fBytesSentPerSec2 = 0, fBytesReceivedPerSec2 = 0,
				fBytesTotalPersec2 = 0, fTimestamp_PerfTime2 = 0;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("PostRequestsPersec",0);
				fPostRequestsPersec2 = (float)prop->GetValue();
				prop = properties->Item("GetRequestsPersec",0);
				fGetRequestsPersec2 = (float)prop->GetValue();
				prop = properties->Item("BytesSentPerSec",0);
				fBytesSentPerSec2 = (float)prop->GetValue();
				prop = properties->Item("BytesReceivedPerSec",0);
				fBytesReceivedPerSec2 = (float)prop->GetValue();
				prop = properties->Item("BytesTotalPersec",0);
				fBytesTotalPersec2 = (float)prop->GetValue();
				prop = properties->Item("Timestamp_PerfTime",0);
				fTimestamp_PerfTime2 = (float)prop->GetValue();
				prop = properties->Item("Frequency_PerfTime",0);
				fFrequency_PerfTime = (float)prop->GetValue();
				prop = properties->Item("MaximumConnections",0);
				nMaximumConnections = (int)prop->GetValue();
				prop = properties->Item("CurrentAnonymousUsers",0);
				nCurrentAnonymousUsers = (int)prop->GetValue();
				prop = properties->Item("CurrentConnections",0);
				nCurrentConnections = (int)prop->GetValue();
				prop = properties->Item("TotalNotFoundErrors",0);
				nTotalNotFoundErrors = (int)prop->GetValue();

				VariantClear (&var);
				object.Release ();
				properties.Release ();
				prop.Release ();

			}
			float fInterval = (fTimestamp_PerfTime2-fTimestamp_PerfTime1)/fFrequency_PerfTime;
			strReturn.Format("maxconnections=%d$currentnonanonymoususers=%d$currentconnections=%d$totalnotfounderrors=%d$\
				postrequestspersec=%.2f$getrequestspersec=%.2f$bytessentpersec=%.2f$bytesreceivedpersec=%.2f$\
				bytestotalpersec=%.2f$", nMaximumConnections, nCurrentAnonymousUsers, nCurrentConnections, nTotalNotFoundErrors,
				(fPostRequestsPersec2-fPostRequestsPersec1)/fInterval, (fGetRequestsPersec2-fGetRequestsPersec1)/fInterval,
				(fBytesSentPerSec2-fBytesSentPerSec1)/fInterval, (fBytesReceivedPerSec2-fBytesReceivedPerSec1)/fInterval,
				(fBytesTotalPersec2-fBytesTotalPersec1)/fInterval);
			strReturn.Replace("\t", NULL);

			if( NULL != obj_enum )
				obj_enum.Release ();
			if( NULL != objects ) 
				objects.Release ();
			if( NULL != services)
				services.Release();

		}
		catch (_com_error err) 
		{ 
			IErrorInfo * ei = err.ErrorInfo();
			BSTR strDesEI;
			ei->GetDescription(&strDesEI);
			char* pstr = _com_util::ConvertBSTRToString(strDesEI);
			sprintf(szReturn, "error=Error ocured:%x: %s", (unsigned)err.Error(), pstr );

			ei->Release();
			free( pstr );
			::SysFreeString( strDesEI );

			if( services != NULL )
				services.Release();
			CoUninitialize();
			return FALSE;
		} 
		catch(...) 
		{ 
			sprintf(szReturn, "error=Error ocured: %d",::GetLastError());
			OutputDebugString(szReturn);

			if( services != NULL )
				services.Release();
			CoUninitialize();
			return FALSE;
		} 

		strcpy(szReturn,strReturn);
		CString strOutRet;
		strOutRet =szReturn;
		nSize = 2048;
		MakeCharByString(szReturn,nSize,strOutRet);	 
	}
	CoUninitialize();

	return TRUE;
}


extern "C" _declspec(dllexport) 
BOOL GetASPInfo(const char * strParas, char * szReturn, int& nSize)
{
		CoInitialize(NULL);
	{
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(strParas, szReturn, nSize, services))
		{
			CoUninitialize();
			return FALSE;
		}
		try
		{
			//取第一次ASP数据
			WbemScripting::ISWbemObjectSetPtr objects = services->InstancesOf("Win32_PerfRawData_ASP_ActiveServerPages", WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL);
			if (NULL == objects) 
			{
				services.Release ();
				CoUninitialize ();
				return FALSE;
			}

			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
			if (NULL == obj_enum) 
			{
				services.Release ();
				objects.Release ();
				CoUninitialize ();
				return FALSE;
			}

			ULONG fetched; 
			VARIANT var; 
			int nErrorsDuringScriptRuntime = 0,nErrorsFromASPPreprocessor = 0,nErrorsFromScriptCompilers = 0,
				nSessionsCurrent = 0, nRequestsSucceeded = 0, nTransactionsTotal = 0, nRequestsFailedTotal = 0, 
				nRequestsTotal = 0;
			float fErrorsPerSec1 = 0, fRequestBytesInTotal1 = 0,  fTimestamp_PerfTime1 = 0,
				fFrequency_PerfTime = 0;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("ErrorsPerSec",0);
				fErrorsPerSec1 = (float)prop->GetValue();
				prop = properties->Item("RequestBytesInTotal",0);
				fRequestBytesInTotal1 = (float)prop->GetValue();
				prop = properties->Item("Timestamp_PerfTime",0);
				fTimestamp_PerfTime1 = (float)prop->GetValue();

				VariantClear (&var);
				object.Release ();
				properties.Release ();
				prop.Release ();
			}


			obj_enum.Release();
			obj_enum = NULL;
			objects.Release ();
			objects = NULL;

			//取第二次ASP数据
			objects = services->InstancesOf("Win32_PerfRawData_ASP_ActiveServerPages", WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL);
			if (NULL == objects) 
			{
				services.Release ();
				CoUninitialize ();
				return FALSE;
			}

			obj_enum = objects->Get_NewEnum(); 
			if (NULL == obj_enum) 
			{
				services.Release ();
				objects.Release ();
				CoUninitialize ();
				return FALSE;
			}

			float fErrorsPerSec2 = 0, fRequestBytesInTotal2 = 0, fRequestsFailedTotal2 = 0, fRequestsTotal2 = 0, fTimestamp_PerfTime2 = 0;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("ErrorsPerSec",0);
				fErrorsPerSec2 = (float)prop->GetValue();
				prop = properties->Item("RequestBytesInTotal",0);
				fRequestBytesInTotal2 = (float)prop->GetValue();
				prop = properties->Item("RequestsFailedTotal",0);
				nRequestsFailedTotal = (int)prop->GetValue();
				prop = properties->Item("RequestsTotal",0);
				nRequestsTotal = (int)prop->GetValue();
				prop = properties->Item("SessionsCurrent",0);
				nSessionsCurrent = (int)prop->GetValue();
				prop = properties->Item("RequestsSucceeded",0);
				nRequestsSucceeded = (int)prop->GetValue();
				prop = properties->Item("TransactionsTotal",0);
				nTransactionsTotal = (int)prop->GetValue();
				prop = properties->Item("Timestamp_PerfTime",0);
				fTimestamp_PerfTime2 = (float)prop->GetValue();
				prop = properties->Item("Frequency_PerfTime",0);
				fFrequency_PerfTime = (float)prop->GetValue();
				prop = properties->Item("ErrorsDuringScriptRuntime",0);
				nErrorsDuringScriptRuntime = (int)prop->GetValue();
				prop = properties->Item("ErrorsFromASPPreprocessor",0);
				nErrorsFromASPPreprocessor = (int)prop->GetValue();
				prop = properties->Item("ErrorsFromScriptCompilers",0);
				nErrorsFromScriptCompilers = (int)prop->GetValue();

				VariantClear (&var);
				object.Release ();
				properties.Release ();
				prop.Release ();

			}
			float fInterval = (fTimestamp_PerfTime2-fTimestamp_PerfTime1)/fFrequency_PerfTime;
			strReturn.Format("ErrorsDuringScriptRuntime=%d$ErrorsFromASPPreprocessor=%d$ErrorsFromScriptCompilers=%d$\
				ErrorsPerSec=%.2f$RequestBytesInTotal=%.2f$RequestsFailedTotal=%d$RequestsTotal=%d$\
				SessionsCurrent=%d$RequestsSucceeded=%d$TransactionsTotal=%d$", 
				nErrorsDuringScriptRuntime, nErrorsFromASPPreprocessor, nErrorsFromScriptCompilers,
				(fErrorsPerSec2-fErrorsPerSec1)/fInterval, (fRequestBytesInTotal2-fRequestBytesInTotal1)/fInterval,
				nRequestsFailedTotal, nRequestsTotal,nSessionsCurrent, nRequestsSucceeded,nTransactionsTotal);
			strReturn.Replace("\t", NULL);

			
			if( NULL != obj_enum )
				obj_enum.Release ();
			if( NULL != objects ) 
				objects.Release ();
			if( NULL != services)
				services.Release();

		}
		catch (_com_error err) 
		{ 
			IErrorInfo * ei = err.ErrorInfo();
			BSTR strDesEI;
			ei->GetDescription(&strDesEI);
			char* pstr = _com_util::ConvertBSTRToString(strDesEI);
			sprintf(szReturn, "error=Error ocured:%x: %s", (unsigned)err.Error(), pstr );

			ei->Release();
			free( pstr );
			::SysFreeString( strDesEI );

			
			if( services != NULL )
				services.Release();

			CoUninitialize();
			return FALSE;
		} 
		catch(...) 
		{ 
			sprintf(szReturn, "error=Error ocured: %d",::GetLastError());

			if( services != NULL )
				services.Release();

			OutputDebugString(szReturn);
			CoUninitialize();
			return FALSE;
		} 

		strcpy(szReturn,strReturn);
		CString strOutRet;
		strOutRet =szReturn;
		nSize = 2048;
		MakeCharByString(szReturn,nSize,strOutRet);	 
	}
	CoUninitialize();

	return TRUE;
}


extern "C" _declspec(dllexport) 
BOOL GetAspNetApplicationInfo(const char * strParas, char * szReturn, int& nSize)
{
		CoInitialize(NULL);
	{
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(strParas, szReturn, nSize, services))
		{
			CoUninitialize();
			return FALSE;
		}
		try
		{
			//取第一次AspNet数据
			_bstr_t com; 
			com="SELECT * FROM Win32_PerfRawData_ASPNET_ASPNETApplications where Name='__Total__'"; 

			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(com,"WQL",WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,NULL); 
			if (NULL == objects) 
			{
				services.Release ();
				CoUninitialize ();
				return FALSE;
			}

			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 			
			if (NULL == obj_enum) 
			{
				services.Release ();
				objects.Release ();
				CoUninitialize ();
				return FALSE;
			}

			ULONG fetched; 
			VARIANT var; 
			int nCompilationsTotal = 0, nRequestsFailed = 0,nSessionsTimedOut = 0,
				nSessionsActive = 0, nRequestsSucceeded = 0;
			float fTransactionsPerSec1 = 0, fRequestsPerSec1 = 0,  fErrorsTotalPerSec1 = 0,
				fAnonymousRequestsPerSec1 = 0, fCacheAPITurnoverRate1 = 0, fTimestamp_PerfTime1 = 0,
				fFrequency_PerfTime = 0;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("TransactionsPerSec",0);
				fTransactionsPerSec1 = (float)prop->GetValue();
				prop = properties->Item("RequestsPerSec",0);
				fRequestsPerSec1 = (float)prop->GetValue();
				prop = properties->Item("ErrorsTotalPerSec",0);
				fErrorsTotalPerSec1 = (float)prop->GetValue();
				prop = properties->Item("AnonymousRequestsPerSec",0);
				fAnonymousRequestsPerSec1 = (float)prop->GetValue();
				prop = properties->Item("CacheAPITurnoverRate",0);
				fCacheAPITurnoverRate1 = (float)prop->GetValue();
				prop = properties->Item("Timestamp_PerfTime",0);
				fTimestamp_PerfTime1 = (float)prop->GetValue();

				VariantClear (&var);
				object.Release ();
				properties.Release ();
				prop.Release ();

			}

			obj_enum.Release();
			obj_enum = NULL;
			objects.Release ();
			objects = NULL;


			//取第二次AspNet数据
			com="SELECT * FROM Win32_PerfRawData_ASPNET_ASPNETApplications where Name='__Total__'"; 
			objects = services->ExecQuery(com,"WQL",WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,NULL); 			
			if (NULL == objects) 
			{
				services.Release ();
				CoUninitialize ();
				return FALSE;
			}

			obj_enum = objects->Get_NewEnum(); 
			if (NULL == obj_enum) 
			{
				services.Release ();
				objects.Release ();
				CoUninitialize ();
				return FALSE;
			}

			float fTransactionsPerSec2 = 0, fRequestsPerSec2 = 0,  fErrorsTotalPerSec2 = 0,
				fAnonymousRequestsPerSec2 = 0, fCacheAPITurnoverRate2 = 0, fTimestamp_PerfTime2 = 0;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("TransactionsPerSec",0);
				fTransactionsPerSec2 = (float)prop->GetValue();
				prop = properties->Item("RequestsPerSec",0);
				fRequestsPerSec2 = (float)prop->GetValue();
				prop = properties->Item("ErrorsTotalPerSec",0);
				fErrorsTotalPerSec2 = (float)prop->GetValue();
				prop = properties->Item("AnonymousRequestsPerSec",0);
				fAnonymousRequestsPerSec2 = (float)prop->GetValue();
				prop = properties->Item("CacheAPITurnoverRate",0);
				fCacheAPITurnoverRate2 = (float)prop->GetValue();
				prop = properties->Item("Timestamp_PerfTime",0);
				fTimestamp_PerfTime2 = (float)prop->GetValue();
				prop = properties->Item("CompilationsTotal",0);
				nCompilationsTotal = (int)prop->GetValue();
				prop = properties->Item("RequestsFailed",0);
				nRequestsFailed = (int)prop->GetValue();
				prop = properties->Item("SessionsTimedOut",0);
				nSessionsTimedOut = (int)prop->GetValue();
				prop = properties->Item("SessionsActive",0);
				nSessionsActive = (int)prop->GetValue();
				prop = properties->Item("RequestsSucceeded",0);
				nRequestsSucceeded = (int)prop->GetValue();
				prop = properties->Item("Frequency_PerfTime",0);
				fFrequency_PerfTime = (float)prop->GetValue();

				VariantClear (&var);
				object.Release ();
				properties.Release ();
				prop.Release ();

			}
			float fInterval = (fTimestamp_PerfTime2-fTimestamp_PerfTime1)/fFrequency_PerfTime;
			strReturn.Format("CompilationsTotal=%d$RequestsFailed=%d$SessionsTimedOut=%d$\
				SessionsActive=%d$RequestsSucceeded=%d$TransactionsPerSec=%.2f$RequestsPerSec=%.2f$\
				ErrorsTotalPerSec=%.2f$AnonymousRequestsPerSec=%.2f$CacheAPITurnoverRate=%.2f$", 
				nCompilationsTotal, nRequestsFailed, nSessionsTimedOut, nSessionsActive, nRequestsSucceeded,
				(fTransactionsPerSec2-fTransactionsPerSec1)/fInterval, (fRequestsPerSec2-fRequestsPerSec1)/fInterval,
				(fErrorsTotalPerSec2-fErrorsTotalPerSec1)/fInterval, (fAnonymousRequestsPerSec2-fAnonymousRequestsPerSec1)/fInterval,
				(fCacheAPITurnoverRate2-fCacheAPITurnoverRate1)/fInterval);
			strReturn.Replace("\t", NULL);

			if( NULL != obj_enum )
				obj_enum.Release ();
			if( NULL != objects ) 
				objects.Release ();
			if( NULL != services)
				services.Release();

		}
		catch (_com_error err) 
		{ 
			IErrorInfo * ei = err.ErrorInfo();
			BSTR strDesEI;
			ei->GetDescription(&strDesEI);
			char* pstr = _com_util::ConvertBSTRToString(strDesEI);
			sprintf(szReturn, "error=Error ocured:%x: %s", (unsigned)err.Error(), pstr );

			ei->Release();
			free( pstr );
			::SysFreeString( strDesEI );

			if( services != NULL )
				services.Release();

			CoUninitialize();
			return FALSE;
		} 
		catch(...) 
		{ 
			sprintf(szReturn, "error=Error ocured: %d",::GetLastError());
			OutputDebugString(szReturn);

			if( services != NULL )
				services.Release();

			CoUninitialize();
			return FALSE;
		} 

		strcpy(szReturn,strReturn);
		CString strOutRet;
		strOutRet =szReturn;
		nSize = 2048;
		MakeCharByString(szReturn,nSize,strOutRet);	 
	}
	CoUninitialize();

	return TRUE;
}


extern "C" _declspec(dllexport) 
BOOL GetAspNetInfo(const char * strParas, char * szReturn, int& nSize)
{
		CoInitialize(NULL);
	{
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(strParas, szReturn, nSize, services))
		{
			CoUninitialize();
			return FALSE;
		}
		try
		{
			WbemScripting::ISWbemObjectSetPtr objects = services->InstancesOf("Win32_PerfRawData_ASPNET_ASPNET", WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL);
			if (NULL == objects) 
			{
				services.Release ();
				CoUninitialize ();
				return FALSE;
			}

			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
				if (NULL == obj_enum) 
			{
				services.Release ();
				objects.Release ();
				CoUninitialize ();
				return FALSE;
			}

			ULONG fetched; 
			VARIANT var; 
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				obj_enum = properties->Get_NewEnum(); 
				while (obj_enum->Next(1,&var,&fetched) == S_OK) 
				{
					WbemScripting::ISWbemPropertyPtr prot=var.punkVal; 
					_variant_t value = prot->GetValue();
					if(value.vt != VT_NULL)
					{
						strReturn += (const char*)_bstr_t(prot->GetName()) ;
						strReturn += "=";
						strReturn += (const char*)_bstr_t(value) ;
						strReturn += "$";
					}
					
					prot.Release ();
				}

				VariantClear (&var);
				object.Release ();
				properties.Release ();
				
			}

			if( NULL != obj_enum )
				obj_enum.Release ();
			if( NULL != objects ) 
				objects.Release ();
			if( NULL != services)
				services.Release();


		}
		catch (_com_error err) 
		{ 
			IErrorInfo * ei = err.ErrorInfo();
			BSTR strDesEI;
			ei->GetDescription(&strDesEI);
			char* pstr = _com_util::ConvertBSTRToString(strDesEI);
			sprintf(szReturn, "error=Error ocured:%x: %s", (unsigned)err.Error(), pstr );

			ei->Release();
			free( pstr );
			::SysFreeString( strDesEI );

			if( services != NULL )
				services.Release();

			CoUninitialize();
			return FALSE;
		} 
		catch(...) 
		{ 
			sprintf(szReturn, "error=Error ocured: %d",::GetLastError());
			OutputDebugString(szReturn);

			if( services != NULL )
				services.Release();

			CoUninitialize();
			return FALSE;
		} 

		strcpy(szReturn,strReturn);
		CString strOutRet;
		strOutRet =szReturn;
		nSize = 4096;
		MakeCharByString(szReturn,nSize,strOutRet);	 
	}
	CoUninitialize();

	return TRUE;
}


extern "C" _declspec(dllexport) 
BOOL EnumNetWorks(const char * strParas, char * szReturn, int& nSize)
{
	CoInitialize(NULL);
	{
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(strParas, szReturn, nSize, services))
		{
			CoUninitialize();
			return FALSE;
		}

		try
		{
			WbemScripting::ISWbemObjectSetPtr objects = services->InstancesOf("Win32_PerfRawData_Tcpip_NetworkInterface", WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL);	
			if (NULL == objects) 
			{
				services.Release ();
				CoUninitialize ();
				return FALSE;
			}

			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
			if (NULL == obj_enum) 
			{
				services.Release ();
				objects.Release ();
				CoUninitialize ();
				return FALSE;
			}

			ULONG fetched; 
			VARIANT var; 
			CString strTemp;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("Name",0);
							
				strTemp.Format("%s=%s$", (const char*)_bstr_t(prop->GetValue()), (const char*)_bstr_t(prop->GetValue()));
				strReturn += strTemp;

				VariantClear (&var);
				object.Release ();
				properties.Release ();
				prop.Release ();
			}

			if( NULL != obj_enum )
				obj_enum.Release ();
			if( NULL != objects ) 
				objects.Release ();
			if( NULL != services)
				services.Release();


		}
		catch (_com_error err) 
		{ 
			IErrorInfo * ei = err.ErrorInfo();
			BSTR strDesEI;
			ei->GetDescription(&strDesEI);
			char* pstr = _com_util::ConvertBSTRToString(strDesEI);
			sprintf(szReturn, "error=Error ocured:%x: %s", (unsigned)err.Error(), pstr );

			ei->Release();
			free( pstr );
			::SysFreeString( strDesEI );

			if( services != NULL )
				services.Release();

			CoUninitialize();
			return FALSE;
		} 
		catch(...) 
		{ 
			sprintf(szReturn, "error=Error ocured: %d",::GetLastError());
			OutputDebugString(szReturn);

			if( services != NULL )
				services.Release();

			CoUninitialize();
			return FALSE;
		} 

		strcpy(szReturn,strReturn);
		CString strOutRet;
		strOutRet =szReturn;
		nSize = 2048;
		MakeCharByString(szReturn,nSize,strOutRet);	 
	}
	CoUninitialize();

	return TRUE;
}


extern "C" _declspec(dllexport) 
BOOL GetNetWorkInfo(const char * strParas, char * szReturn, int& nSize)
{
	CoInitialize(NULL);
	{
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(strParas, szReturn, nSize, services))
		{
			CoUninitialize();
			return FALSE;
		}
		try
		{
			char *networkname=NULL;
			networkname = FindStrValue(strParas, "_NetWorkName");

			CString strCom;
			if( networkname != NULL )
			{
				strCom.Format("SELECT * FROM Win32_PerfRawData_Tcpip_NetworkInterface where Name='%s'", networkname);
				free(networkname);
			}
				

			//取第一次网络设备数据			
			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(LPCTSTR(strCom),"WQL",WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,NULL); 
			if (NULL == objects) 
			{
				services.Release ();
				CoUninitialize ();
				return FALSE;
			}

			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum();
			if (NULL == obj_enum) 
			{
				services.Release ();
				objects.Release ();
				CoUninitialize ();
				return FALSE;
			}

			ULONG fetched; 
			VARIANT var; 
			int nPacketsOutboundErrors = 0, nPacketsReceivedErrors = 0; 
			float fBytesReceivedPerSec1 = 0, fBytesSentPerSec1 =0, fTimestamp_PerfTime1 = 0,
				fFrequency_PerfTime = 0;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("BytesReceivedPerSec",0);
				fBytesReceivedPerSec1 = (float)prop->GetValue();				
				prop = properties->Item("BytesSentPerSec",0);
				fBytesSentPerSec1 = (float)prop->GetValue();
				prop = properties->Item("Timestamp_PerfTime",0);
				fTimestamp_PerfTime1 = (float)prop->GetValue();

				VariantClear (&var);
				object.Release ();
				properties.Release ();
				prop.Release ();

			}

			obj_enum.Release();
			obj_enum = NULL;
			objects.Release ();
			objects = NULL;


			//取第二次网络设备数据
			objects = services->ExecQuery(LPCTSTR(strCom),"WQL",WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,NULL); 
			if (NULL == objects) 
			{
				services.Release ();
				CoUninitialize ();
				return FALSE;
			}

			obj_enum = objects->Get_NewEnum();
			if (NULL == obj_enum) 
			{
				services.Release ();
				objects.Release ();
				CoUninitialize ();
				return FALSE;
			}

			float fBytesReceivedPerSec2 = 0, fBytesSentPerSec2 =0, fTimestamp_PerfTime2 = 0;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("BytesReceivedPerSec",0);
				fBytesReceivedPerSec2 = (float)prop->GetValue();				
				prop = properties->Item("BytesSentPerSec",0);
				fBytesSentPerSec2 = (float)prop->GetValue();	
				prop = properties->Item("Timestamp_PerfTime",0);
				fTimestamp_PerfTime2 = (float)prop->GetValue();
				prop = properties->Item("PacketsOutboundErrors",0);
				nPacketsOutboundErrors = (int)prop->GetValue();
				prop = properties->Item("PacketsReceivedErrors",0);
				nPacketsReceivedErrors = (int)prop->GetValue();
				prop = properties->Item("Frequency_PerfTime",0);
				fFrequency_PerfTime = (float)prop->GetValue();

				VariantClear (&var);
				object.Release ();
				properties.Release ();
				prop.Release ();

			}
			float fInterval = (fTimestamp_PerfTime2-fTimestamp_PerfTime1)/fFrequency_PerfTime;
			strReturn.Format("BytesReceivedPerSec=%.2f$BytesSentPerSec=%.2f$PacketsErrors=%d", 
				(fBytesReceivedPerSec2-fBytesReceivedPerSec1)/fInterval, (fBytesSentPerSec2-fBytesSentPerSec1)/fInterval,
				nPacketsOutboundErrors+nPacketsReceivedErrors);

			if( NULL != obj_enum )
				obj_enum.Release ();
			if( NULL != objects ) 
				objects.Release ();
			if( NULL != services)
				services.Release();

		}
		catch (_com_error err) 
		{ 
			IErrorInfo * ei = err.ErrorInfo();
			BSTR strDesEI;
			ei->GetDescription(&strDesEI);
			char* pstr = _com_util::ConvertBSTRToString(strDesEI);
			sprintf(szReturn, "error=Error ocured:%x: %s", (unsigned)err.Error(), pstr );

			ei->Release();
			free( pstr );
			::SysFreeString( strDesEI );

			if( services != NULL )
				services.Release();

			CoUninitialize();
			return FALSE;
		} 
		catch(...) 
		{ 
			sprintf(szReturn, "error=Error ocured: %d",::GetLastError());
			OutputDebugString(szReturn);

			if( services != NULL )
				services.Release();

			CoUninitialize();
			return FALSE;
		} 

		strcpy(szReturn,strReturn);
		CString strOutRet;
		strOutRet =szReturn;
		nSize = 2048;
		MakeCharByString(szReturn,nSize,strOutRet);	 
	}
	CoUninitialize();

	return TRUE;
}


extern "C" _declspec(dllexport) 
BOOL GetADInfo(const char * strParas, char * szReturn, int& nSize)
{	
	CoInitialize(NULL);
	{
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectADServer(strParas, szReturn, nSize, services))
		{
			CoUninitialize();
			return FALSE;
		}
		try
		{
			WbemScripting::ISWbemObjectSetPtr objects = services->InstancesOf("MSAD_DomainController", WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL);
			if (NULL == objects) 
			{
				services.Release ();
				CoUninitialize ();
				return FALSE;
			}

			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
			if (NULL == obj_enum) 
			{
				services.Release ();
				objects.Release ();
				CoUninitialize ();
				return FALSE;
			}


			ULONG fetched; 
			VARIANT var; 
			int nPecentUsage = 0;
			bool bState;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("PercentOfRIDsLeft",0);
				nPecentUsage = 100-(int)prop->GetValue();
				prop = properties->Item("IsAdvertisingToLocator",0);
				bState = (bool)prop->GetValue();
				if(bState)
					strReturn.Format("PercentOfRIDsUsed=%d$ServiceState=True$", nPecentUsage);
				else
					strReturn.Format("PercentOfRIDsUsed=%d$ServiceState=False$", nPecentUsage);

				VariantClear (&var);
				object.Release ();
				properties.Release ();
				prop.Release ();
			}

			if( NULL != obj_enum )
				obj_enum.Release ();
			if( NULL != objects ) 
				objects.Release ();
			if( NULL != services)
				services.Release();


		}
		catch (_com_error err) 
		{ 
			IErrorInfo * ei = err.ErrorInfo();
			BSTR strDesEI;
			ei->GetDescription(&strDesEI);
			char* pstr = _com_util::ConvertBSTRToString(strDesEI);
			sprintf(szReturn, "error=Error ocured:%x: %s", (unsigned)err.Error(), pstr );

			ei->Release();
			free( pstr );
			::SysFreeString( strDesEI );

			if( services != NULL )
				services.Release();

			CoUninitialize();
			return FALSE;
		} 
		catch(...) 
		{ 
			sprintf(szReturn, "error=Error ocured: %d",::GetLastError());
			OutputDebugString(szReturn);

			if( services != NULL )
				services.Release();

			CoUninitialize();
			return FALSE;
		} 

		strcpy(szReturn,strReturn);
		CString strOutRet;
		strOutRet =szReturn;
		nSize = 4096;
		MakeCharByString(szReturn,nSize,strOutRet);	 
	}
	CoUninitialize();

	return TRUE;
}


void InvokeProcess( const char* szCommand, char* szResult )
{

	_CrtMemState s1, s2, s3; 
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	char szTemp[1024] = {0};

	/*
	sprintf( szTemp, "线程：%d, 执行前检查到泄露：", GetCurrentThreadId() );
	cout << szTemp << endl;
	_CrtDumpMemoryLeaks();//Dump从程序开始运行到该时刻点，已分配而未释放的内存

	sprintf( szTemp, "线程：%d, 执行前的快照：", GetCurrentThreadId() );
	cout << szTemp << endl;
	_CrtMemCheckpoint( &s1 );
	_CrtMemDumpStatistics( &s1 );
	*/


	HANDLE hOutputRead, hOutputWrite;
	SECURITY_ATTRIBUTES sa;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	try
	{
	
		if (CreatePipe(&hOutputRead, &hOutputWrite, &sa, 0 ) ) 
		{
			STARTUPINFO si;
			memset(&si, 0, sizeof(STARTUPINFO));
			si.cb = sizeof(STARTUPINFO);
			si.dwFlags = STARTF_USESTDHANDLES;
			si.hStdOutput = hOutputWrite;
			si.hStdError = hOutputWrite;
			
			PROCESS_INFORMATION pi;
			memset(&pi, 0, sizeof(PROCESS_INFORMATION));

			if (CreateProcess(NULL, (LPSTR)szCommand,  NULL, NULL, TRUE, /*CREATE_NEW_CONSOLE*/CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
			{
				cout << "创建进程成功！" << endl;

 				char buffer[1024*10] = {0};
				DWORD bytesRead = 0;

				try
				{

					/*
					while(1)
					{
						char* pIndex = NULL;
						if(ReadFile(hOutputRead, buffer, sizeof(buffer)-1, &bytesRead, NULL))
						{
							buffer[bytesRead] = 0;

							strcat( szResult, buffer );
							pIndex = strstr( szResult, "DragonFlow:" );
							if( pIndex != NULL )
							{
								TerminateProcess(pi.hProcess, 1);

								break;
							}
						}
						else  
						{
							TerminateProcess(pi.hProcess, 1);
							ReadFile(hOutputRead, buffer, sizeof(buffer)-1, &bytesRead, NULL);
							buffer[bytesRead] = 0;
							strcat( szResult, buffer );
							break;
						}
					}
					*/

					if( WaitForSingleObject(pi.hProcess, 60000 ) == WAIT_OBJECT_0 )
					{
						if( (GetFileSize(hOutputRead,NULL) > 0) && 
							ReadFile(hOutputRead, buffer, sizeof(buffer)-1, &bytesRead, NULL) )
						{
							buffer[bytesRead] = 0;

							strcat( szResult, buffer );
													
						}
						else
						{
							sprintf( szResult, "DragonFlow:error=Read data failure!$" );
						}
					
					}
					else
					{
						TerminateProcess(pi.hProcess, 1);
						sprintf( szResult, "DragonFlow:error=Read data timeout!$" );
					}


				}
				catch(...)
				{
					TerminateProcess(pi.hProcess, 1);
					sprintf( szResult, "DragonFlow:error=Read data failure!$" );
				}


				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
			}
			else
			{
				sprintf( szResult, "DragonFlow:error=Read data failure!$" );
			}

			CloseHandle(hOutputRead);
			CloseHandle(hOutputWrite);
			
		}
		else
		{
			sprintf( szResult, "DragonFlow:error=Read data failure!$" );
		}

	}
	catch(...)
	{
		if( hOutputRead != NULL )
		{
			CloseHandle(hOutputRead);
		}

		if( hOutputWrite != NULL )
		{
			CloseHandle(hOutputWrite);
		}		
	}

	cout << "szResult:" << szResult << endl;

	/*
	sprintf( szTemp, "线程：%d, 执行后检查到泄露：", GetCurrentThreadId() );
	cout << szTemp << endl;
	_CrtDumpMemoryLeaks();//Dump从程序开始运行到该时刻点，已分配而未释放的内存

	sprintf( szTemp, "线程：%d, 执行后的快照：", GetCurrentThreadId() );
	cout << szTemp << endl;
	_CrtMemCheckpoint( &s2 );
	_CrtMemDumpStatistics( &s2 );
	*/

	
}

extern "C" _declspec(dllexport) 
BOOL GetServiceInfo( const char* szParas, char* szReturn, int& nSize )
{
	char szFunName[128] = "GetServiceInfo";

	char *machinename=NULL, *user=NULL, *pswd=NULL, *param=NULL;
	char szMachineName[128] = "127.0.0.1";
	char szUser[128] = "administrator";
	char szPswd[128] = "111";
	char szParam[256] = "SiteViewDB";
	

	const char* pszInput = szParas;
	while( *pszInput )
	{
		machinename = strstr( pszInput, "_MachineName" );
		if( machinename != NULL )
		{
			strcpy( szMachineName, machinename+strlen("_MachineName")+1 );
		}

		user = strstr( pszInput, "_UserAccount" );
		if( user != NULL )
		{
			strcpy( szUser, user+strlen("_UserAccount")+1 );
		}

		pswd = strstr( pszInput, "_PassWord" );
		if( pswd != NULL )
		{
			strcpy( szPswd, pswd+strlen("_PassWord")+1 );
		}

		param = strstr( pszInput, "_Service" );
		if( param != NULL )
		{
			strcpy( szParam, param+strlen("_Service")+1 );
		}


		pszInput += strlen( pszInput ) + 1;
	}

	if( szMachineName[0] == '\0' )
	{
		strcpy( szMachineName, "127.0.0.1" );
	}

	if( szUser[0] == '\0' )
	{
		strcpy( szUser, "administrator" );
	}

	if( szPswd[0] == '\0' )
	{
		strcpy( szPswd, "111" );
	}

	if( szParam[0] == '\0' )
	{
		strcpy( szParam, "NULL" );
	}


	char szCommand[1024] = {0};
	sprintf( szCommand, "wmiProcess \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"", szFunName, szMachineName, szUser, szPswd, szParam );

	cout << "command: " << szCommand << endl; 

	char szResult[10*1024] = {0};

	InvokeProcess( szCommand, szResult );



	memset( szReturn, 0, nSize );

	if( strlen(szResult) == 0 )
	{
		sprintf( szReturn, "error=Read data failure!" );
		nSize = strlen( szReturn );
	}
	else if( strlen(szResult) < nSize )
	{
		nSize = 0;

		char* pszBegin = szResult+strlen("DragonFlow:");
		char* pszEnd = NULL;
		char* pszRet = szReturn;

		while(*pszBegin)
		{
			if( ( pszEnd=strchr(pszBegin, '$') ) != NULL )
			{
				strncpy( pszRet, pszBegin, pszEnd-pszBegin );
				nSize += strlen(pszRet) + 1;
				pszRet += strlen(pszRet) + 1;
				pszBegin = pszEnd + 1;
			}
			else
			{
				break;
			}
		}

		if( *szReturn == '\0' )
		{
			sprintf( szReturn, "error=Read data failure!" );
			nSize = strlen( szReturn );
		}

	}
	else
	{
		sprintf( szReturn, "error=return overflow!" );
		nSize = strlen( szReturn );
	}

	if( ( strstr( szReturn, "error" ) != NULL ) || ( nSize == 0 ) )
	{
		return false;
	}
	else
	{
		return true;
	}

}


extern "C" _declspec(dllexport) 
BOOL GetProcessInfo( const char* szParas, char* szReturn, int& nSize )
{
	_CrtMemState s1, s2, s3; 
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	char szTemp[1024] = {0};

	/*
	sprintf( szTemp, "线程：%d, 执行前检查到泄露：", GetCurrentThreadId() );
	cout << szTemp << endl;
	_CrtDumpMemoryLeaks();//Dump从程序开始运行到该时刻点，已分配而未释放的内存

	sprintf( szTemp, "线程：%d, 执行前的快照：", GetCurrentThreadId() );
	cout << szTemp << endl;
	_CrtMemCheckpoint( &s1 );
	_CrtMemDumpStatistics( &s1 );
	*/


	char szFunName[128] = "GetProcessInfo";

	char *machinename=NULL, *user=NULL, *pswd=NULL, *param=NULL;
	char szMachineName[128] = "127.0.0.1";
	char szUser[128] = "administrator";
	char szPswd[128] = "111";
	char szParam[256] = "svdb";
	

	const char* pszInput = szParas;
	while( *pszInput )
	{
		machinename = strstr( pszInput, "_MachineName" );
		if( machinename != NULL )
		{
			strcpy( szMachineName, machinename+strlen("_MachineName")+1 );
		}

		user = strstr( pszInput, "_UserAccount" );
		if( user != NULL )
		{
			strcpy( szUser, user+strlen("_UserAccount")+1 );
		}

		pswd = strstr( pszInput, "_PassWord" );
		if( pswd != NULL )
		{
			strcpy( szPswd, pswd+strlen("_PassWord")+1 );
		}

		param = strstr( pszInput, "_monitorProcessList" );
		if( param != NULL )
		{
			strcpy( szParam, param+strlen("_monitorProcessList")+1 );
		}


		pszInput += strlen( pszInput ) + 1;
	}

	if( szMachineName[0] == '\0' )
	{
		strcpy( szMachineName, "127.0.0.1" );
	}

	if( szUser[0] == '\0' )
	{
		strcpy( szUser, "administrator" );
	}

	if( szPswd[0] == '\0' )
	{
		strcpy( szPswd, "111" );
	}

	if( szParam[0] == '\0' )
	{
		strcpy( szParam, "NULL" );
	}


	char szCommand[1024] = {0};
	sprintf( szCommand, "wmiProcess \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"", szFunName, szMachineName, szUser, szPswd, szParam );

	cout << "command: " << szCommand << endl; 

	char szResult[10*1024] = {0};

	InvokeProcess( szCommand, szResult );



	memset( szReturn, 0, nSize );

	if( strlen(szResult) == 0 )
	{
		sprintf( szReturn, "error=Read data failure!" );
		nSize = strlen( szReturn );
	}
	else if( strlen(szResult) < nSize )
	{
		nSize = 0;

		char* pszBegin = szResult+strlen("DragonFlow:");
		char* pszEnd = NULL;
		char* pszRet = szReturn;

		while(*pszBegin)
		{
			if( ( pszEnd=strchr(pszBegin, '$') ) != NULL )
			{
				strncpy( pszRet, pszBegin, pszEnd-pszBegin );
				nSize += strlen(pszRet) + 1;
				pszRet += strlen(pszRet) + 1;
				pszBegin = pszEnd + 1;
			}
			else
			{
				break;
			}
		}

		if( *szReturn == '\0' )
		{
			sprintf( szReturn, "error=Read data failure!" );
			nSize = strlen( szReturn );
		}

	}
	else
	{
		sprintf( szReturn, "error=No sufficient space!" );
		nSize = strlen( szReturn );
	}

	if( ( strstr( szReturn, "error" ) != NULL ) || ( nSize == 0 ) )
	{
		return false;
	}
	else
	{
		return true;
	}

	/*
	sprintf( szTemp, "线程：%d, 执行后检查到泄露：", GetCurrentThreadId() );
	cout << szTemp << endl;
	_CrtDumpMemoryLeaks();//Dump从程序开始运行到该时刻点，已分配而未释放的内存

	sprintf( szTemp, "线程：%d, 执行后的快照：", GetCurrentThreadId() );
	cout << szTemp << endl;
	_CrtMemCheckpoint( &s2 );
	_CrtMemDumpStatistics( &s2 );
	*/

}


extern "C" _declspec(dllexport) 
BOOL GetAllCPURate( const char* szParas, char* szReturn, int& nSize )
{
	char szFunName[128] = "GetAllCPURate";

	char *machinename=NULL, *user=NULL, *pswd=NULL, *param=NULL;
	char szMachineName[128] = "127.0.0.1";
	char szUser[128] = "administrator";
	char szPswd[128] = "111";
	char szParam[256] = "NULL";
	

	const char* pszInput = szParas;
	while( *pszInput )
	{
		machinename = strstr( pszInput, "_MachineName" );
		if( machinename != NULL )
		{
			strcpy( szMachineName, machinename+strlen("_MachineName")+1 );
		}

		user = strstr( pszInput, "_UserAccount" );
		if( user != NULL )
		{
			strcpy( szUser, user+strlen("_UserAccount")+1 );
		}

		pswd = strstr( pszInput, "_PassWord" );
		if( pswd != NULL )
		{
			strcpy( szPswd, pswd+strlen("_PassWord")+1 );
		}

		pszInput += strlen( pszInput ) + 1;
	}

	if( szMachineName[0] == '\0' )
	{
		strcpy( szMachineName, "127.0.0.1" );
	}

	if( szUser[0] == '\0' )
	{
		strcpy( szUser, "administrator" );
	}

	if( szPswd[0] == '\0' )
	{
		strcpy( szPswd, "111" );
	}

	if( szParam[0] == '\0' )
	{
		strcpy( szParam, "NULL" );
	}


	char szCommand[1024] = {0};
	sprintf( szCommand, "wmiProcess \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"", szFunName, szMachineName, szUser, szPswd, szParam );

	cout << "command: " << szCommand << endl; 

	char szResult[10*1024] = {0};

	InvokeProcess( szCommand, szResult );



	memset( szReturn, 0, nSize );

	if( strlen(szResult) == 0 )
	{
		sprintf( szReturn, "error=Read data failure!" );
		nSize = strlen( szReturn );
	}
	else if( strlen(szResult) < nSize )
	{
		nSize = 0;

		char* pszBegin = szResult+strlen("DragonFlow:");
		char* pszEnd = NULL;
		char* pszRet = szReturn;

		while(*pszBegin)
		{
			if( ( pszEnd=strchr(pszBegin, '$') ) != NULL )
			{
				strncpy( pszRet, pszBegin, pszEnd-pszBegin );
				nSize += strlen(pszRet) + 1;
				pszRet += strlen(pszRet) + 1;
				pszBegin = pszEnd + 1;
			}
			else
			{
				break;
			}
		}

		if( *szReturn == '\0' )
		{
			sprintf( szReturn, "error=Read data failure!" );
			nSize = strlen( szReturn );
		}


	}
	else
	{
		sprintf( szReturn, "error=No sufficient space!" );
		nSize = strlen( szReturn );
	}

	if( ( strstr( szReturn, "error" ) != NULL ) || ( nSize == 0 ) )
	{
		return false;
	}
	else
	{
		return true;
	}

}


extern "C" _declspec(dllexport) 
BOOL GetDiskInfo( const char* szParas, char* szReturn, int& nSize )
{
	char szFunName[128] = "GetDiskInfo";

	char *machinename=NULL, *user=NULL, *pswd=NULL, *param=NULL;
	char szMachineName[128] = "127.0.0.1";
	char szUser[128] = "administrator";
	char szPswd[128] = "111";
	char szParam[256] = "C:";
	

	const char* pszInput = szParas;
	while( *pszInput )
	{
		machinename = strstr( pszInput, "_MachineName" );
		if( machinename != NULL )
		{
			strcpy( szMachineName, machinename+strlen("_MachineName")+1 );
		}

		user = strstr( pszInput, "_UserAccount" );
		if( user != NULL )
		{
			strcpy( szUser, user+strlen("_UserAccount")+1 );
		}

		pswd = strstr( pszInput, "_PassWord" );
		if( pswd != NULL )
		{
			strcpy( szPswd, pswd+strlen("_PassWord")+1 );
		}

		param = strstr( pszInput, "_Disk" );
		if( param != NULL )
		{
			strcpy( szParam, param+strlen("_Disk")+1 );
		}


		pszInput += strlen( pszInput ) + 1;
	}

	if( szMachineName[0] == '\0' )
	{
		strcpy( szMachineName, "127.0.0.1" );
	}

	if( szUser[0] == '\0' )
	{
		strcpy( szUser, "administrator" );
	}

	if( szPswd[0] == '\0' )
	{
		strcpy( szPswd, "111" );
	}

	if( szParam[0] == '\0' )
	{
		strcpy( szParam, "NULL" );
	}


	char szCommand[1024] = {0};
	sprintf( szCommand, "wmiProcess \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"", szFunName, szMachineName, szUser, szPswd, szParam );

	cout << "command: " << szCommand << endl; 

	char szResult[10*1024] = {0};

	InvokeProcess( szCommand, szResult );



	memset( szReturn, 0, nSize );

	if( strlen(szResult) == 0 )
	{
		sprintf( szReturn, "error=Read data failure!" );
		nSize = strlen( szReturn );
	}
	else if( strlen(szResult) < nSize )
	{
		nSize = 0;

		char* pszBegin = szResult+strlen("DragonFlow:");
		char* pszEnd = NULL;
		char* pszRet = szReturn;

		while(*pszBegin)
		{
			if( ( pszEnd=strchr(pszBegin, '$') ) != NULL )
			{
				strncpy( pszRet, pszBegin, pszEnd-pszBegin );
				nSize += strlen(pszRet) + 1;
				pszRet += strlen(pszRet) + 1;
				pszBegin = pszEnd + 1;
			}
			else
			{
				break;
			}
		}

		if( *szReturn == '\0' )
		{
			sprintf( szReturn, "error=Read data failure!" );
			nSize = strlen( szReturn );
		}

	}
	else
	{
		sprintf( szReturn, "error=No sufficient space!" );
		nSize = strlen( szReturn );
	}

	if( ( strstr( szReturn, "error" ) != NULL ) || ( nSize == 0 ) )
	{
		return false;
	}
	else
	{
		return true;
	}

}




extern "C" _declspec(dllexport) 
BOOL GetMemoryInfo( const char* szParas, char* szReturn, int& nSize )
{
	char szFunName[128] = "GetMemoryInfo";

	char *machinename=NULL, *user=NULL, *pswd=NULL, *param=NULL;
	char szMachineName[128] = "127.0.0.1";
	char szUser[128] = "administrator";
	char szPswd[128] = "111";
	char szParam[256] = "NULL";
	

	const char* pszInput = szParas;
	while( *pszInput )
	{
		machinename = strstr( pszInput, "_MachineName" );
		if( machinename != NULL )
		{
			strcpy( szMachineName, machinename+strlen("_MachineName")+1 );
		}

		user = strstr( pszInput, "_UserAccount" );
		if( user != NULL )
		{
			strcpy( szUser, user+strlen("_UserAccount")+1 );
		}

		pswd = strstr( pszInput, "_PassWord" );
		if( pswd != NULL )
		{
			strcpy( szPswd, pswd+strlen("_PassWord")+1 );
		}

		pszInput += strlen( pszInput ) + 1;
	}

	if( szMachineName[0] == '\0' )
	{
		strcpy( szMachineName, "127.0.0.1" );
	}

	if( szUser[0] == '\0' )
	{
		strcpy( szUser, "administrator" );
	}

	if( szPswd[0] == '\0' )
	{
		strcpy( szPswd, "111" );
	}

	if( szParam[0] == '\0' )
	{
		strcpy( szParam, "NULL" );
	}


	char szCommand[1024] = {0};
	sprintf( szCommand, "wmiProcess \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"", szFunName, szMachineName, szUser, szPswd, szParam );

	cout << "command: " << szCommand << endl; 

	char szResult[10*1024] = {0};

	InvokeProcess( szCommand, szResult );



	memset( szReturn, 0, nSize );

	if( strlen(szResult) == 0 )
	{
		sprintf( szReturn, "error=Read data failure!" );
		nSize = strlen( szReturn );
	}
	else if( strlen(szResult) < nSize )
	{
		nSize = 0;

		char* pszBegin = szResult+strlen("DragonFlow:");
		char* pszEnd = NULL;
		char* pszRet = szReturn;

		while(*pszBegin)
		{
			if( ( pszEnd=strchr(pszBegin, '$') ) != NULL )
			{
				strncpy( pszRet, pszBegin, pszEnd-pszBegin );
				nSize += strlen(pszRet) + 1;
				pszRet += strlen(pszRet) + 1;
				pszBegin = pszEnd + 1;
			}
			else
			{
				break;
			}
		}

		if( *szReturn == '\0' )
		{
			sprintf( szReturn, "error=Read data failure!" );
			nSize = strlen( szReturn );
		}
	}
	else
	{
		sprintf( szReturn, "error=No sufficient space!" );
		nSize = strlen( szReturn );
	}

	if( ( strstr( szReturn, "error" ) != NULL ) || ( nSize == 0 ) )
	{
		return false;
	}
	else
	{
		return true;
	}

}

extern "C" _declspec(dllexport) 
BOOL GetCPURate( const char* szParas, char * szReturn, int& nSize )
{
	char szFunName[128] = "GetCPURate";

	char *machinename=NULL, *user=NULL, *pswd=NULL, *param=NULL;
	char szMachineName[128] = "127.0.0.1";
	char szUser[128] = "administrator";
	char szPswd[128] = "111";
	char szParam[256] = "NULL";
	

	const char* pszInput = szParas;
	while( *pszInput )
	{
		machinename = strstr( pszInput, "_MachineName" );
		if( machinename != NULL )
		{
			strcpy( szMachineName, machinename+strlen("_MachineName")+1 );
		}

		user = strstr( pszInput, "_UserAccount" );
		if( user != NULL )
		{
			strcpy( szUser, user+strlen("_UserAccount")+1 );
		}

		pswd = strstr( pszInput, "_PassWord" );
		if( pswd != NULL )
		{
			strcpy( szPswd, pswd+strlen("_PassWord")+1 );
		}

		pszInput += strlen( pszInput ) + 1;
	}

	if( szMachineName[0] == '\0' )
	{
		strcpy( szMachineName, "127.0.0.1" );
	}

	if( szUser[0] == '\0' )
	{
		strcpy( szUser, "administrator" );
	}

	if( szPswd[0] == '\0' )
	{
		strcpy( szPswd, "111" );
	}

	if( szParam[0] == '\0' )
	{
		strcpy( szParam, "NULL" );
	}


	char szCommand[1024] = {0};
	sprintf( szCommand, "wmiProcess \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"", szFunName, szMachineName, szUser, szPswd, szParam );

	cout << "command: " << szCommand << endl; 

	char szResult[10*1024] = {0};

	InvokeProcess( szCommand, szResult );



	memset( szReturn, 0, nSize );

	if( strlen(szResult) == 0 )
	{
		sprintf( szReturn, "error=Read data failure!" );
		nSize = strlen( szReturn );
	}
	else if( strlen(szResult) < nSize )
	{
		nSize = 0;

		char* pszBegin = szResult+strlen("DragonFlow:");
		char* pszEnd = NULL;
		char* pszRet = szReturn;

		while(*pszBegin)
		{
			if( ( pszEnd=strchr(pszBegin, '$') ) != NULL )
			{
				strncpy( pszRet, pszBegin, pszEnd-pszBegin );
				nSize += strlen(pszRet) + 1;
				pszRet += strlen(pszRet) + 1;
				pszBegin = pszEnd + 1;
			}
			else
			{
				break;
			}
		}

		if( *szReturn == '\0' )
		{
			sprintf( szReturn, "error=Read data failure!" );
			nSize = strlen( szReturn );
		}

	}
	else
	{
		sprintf( szReturn, "error=No sufficient space!" );
		nSize = strlen( szReturn );
	}

	if( ( strstr( szReturn, "error" ) != NULL ) || ( nSize == 0 ) )
	{
		return false;
	}
	else
	{
		return true;
	}

}

