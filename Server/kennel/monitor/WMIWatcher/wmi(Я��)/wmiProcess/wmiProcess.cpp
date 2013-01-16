#define _WIN32_DCOM

#include <iostream>
#include <map>
#include <process.h>
#include <afxwin.h>
using namespace std;

#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#include <wbemcli.h>
#import "progid:WbemScripting.SWbemLocator" named_guids

#include <comdef.h>
#include <Wbemidl.h>
# pragma comment(lib, "wbemuuid.lib")



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
			{
				WSACleanup( );
				return TRUE;
			}
			if((hostinfo = gethostbyname(name)) != NULL) 
			{ //这些就是获得IP的函数
				ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list); 
//				 ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list[i]);

				if(sHostName == ip)
				{
					WSACleanup( );
					return TRUE;
				}
			} 
		} 

		WSACleanup( );
	}
	return FALSE;
}

BOOL ConnectServer( const char* szM, const char* szU, const char* szP, char* szReturn, WbemScripting::ISWbemServicesPtr& services )
{
	try 
	{ 
		WbemScripting::ISWbemLocatorPtr locator; 
		locator.CreateInstance(WbemScripting::CLSID_SWbemLocator);

		unsigned long nBeginTime(0), nEndTime(0);
		nBeginTime = ::GetTickCount();
		nEndTime = ::GetTickCount();
		
		if (locator != NULL) 
		{ 
			if(IsLocalHost(szM))
			{
				services = locator->ConnectServer(".","root\\cimv2","","","","",0,NULL);
			}
			else
			{
				services = locator->ConnectServer( szM,"root\\cimv2",szU,szP,"","",0,NULL);
			}

			locator.Release();
		}

		nEndTime = ::GetTickCount();
		//cout << "连接用时：" << (nEndTime-nBeginTime) << endl;
	}
	catch (_com_error err) 
	{ 
		IErrorInfo * ei = err.ErrorInfo();
		BSTR strDesEI;
		ei->GetDescription(&strDesEI);
		char* pstr = _com_util::ConvertBSTRToString(strDesEI);
		sprintf(szReturn, "error=Error ocured:%x: %s$", (unsigned)err.Error(), pstr );

		ei->Release();
		free( pstr );
		::SysFreeString( strDesEI );

		return FALSE;
	} 
	catch(...) 
	{ 
		sprintf(szReturn, "error=Error ocured: %d$",::GetLastError());
		return FALSE;
	} 
	return TRUE;
}


void GetServiceInfo( char* szM, char* szU, char* szP, char* szCommand, char* szReturn )
{
	CoInitialize(NULL);
	{
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer( szM, szU, szP, szReturn, services ) )
		{
			//sprintf( szReturn, "error=%s$","连接目标机器失败！");
			CoUninitialize();
			return;
		}
		try
		{
			CString strCom = szCommand;			
			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(LPCTSTR(strCom),"WQL",WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,NULL); 
			if (NULL == objects) 
			{
				services.Release ();
				CoUninitialize ();
				return;
			}

			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
			if (NULL == obj_enum) 
			{
				services.Release ();
				objects.Release ();
				CoUninitialize ();
				return;
			}

			ULONG fetched; 
			VARIANT var; 
			CString strTemp;
			int nProecssID = 0;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("ProcessId",0);
				nProecssID = (int)prop->GetValue();
				prop.Release();

				if(nProecssID == 0)
					strReturn = "Processes=0$Started=False$ProcessName=NA$";
				else
				{

					strCom.Format("SELECT * FROM Win32_PerfRawData_PerfProc_Process where IDProcess=%d", nProecssID);
					WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(LPCTSTR(strCom),"WQL",WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,NULL); 
					if (NULL == objects) 
					{
						services.Release ();
						CoUninitialize ();
						return;
					}

					IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 					
					if (NULL == obj_enum) 
					{
						services.Release ();
						objects.Release ();
						CoUninitialize ();
						return;
					}

					VARIANT var;

					while (obj_enum->Next(1,&var,&fetched) == S_OK) 
					{ 
						WbemScripting::ISWbemObjectPtr object = var;
						WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
						WbemScripting::ISWbemPropertyPtr prop = properties->Item("Name",0);
						strTemp.Format("ProcessName=%s$", (const char*)_bstr_t(prop->GetValue()));
						strReturn = "Processes=1$Started=True$";
						strReturn += strTemp;

						VariantClear (&var);
						object.Release ();
						properties.Release ();
						prop.Release ();

					}

					obj_enum.Release();
					obj_enum = NULL;
					objects.Release ();
					objects = NULL;

				}
				prop = properties->Item("State",0);
				strTemp.Format("State=%s$", (const char*)_bstr_t(prop->GetValue()));
				prop.Release();
				strReturn += strTemp;
				prop = properties->Item("Status",0);
				strTemp.Format("Status=%s$", (const char*)_bstr_t(prop->GetValue()));
				prop.Release();
				strReturn += strTemp;

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
			sprintf(szReturn, "error=Error ocured:%x: %s$", (unsigned)err.Error(), pstr );

			ei->Release();
			free( pstr );
			::SysFreeString( strDesEI );

			if( services != NULL )
				services.Release();

			CoUninitialize();
			return;
		} 
		catch(...) 
		{ 
			sprintf(szReturn, "error=Error ocured: %d$",::GetLastError());
			OutputDebugString(szReturn);

			if( services != NULL )
				services.Release();

			CoUninitialize();
			return;
		} 

		if( strReturn == "" )
		{
			sprintf( szReturn, "error=no data!$" );
		}
		else
		{
			strcpy(szReturn,strReturn);
		}
	}
	CoUninitialize();

	return;
}


void GetProcessInfo( const char* szM, const char* szU, const char* szP, const char* szCommand, char* szReturn )
{
	CoInitialize(NULL);
	{
		
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;	
		if(!ConnectServer( szM, szU, szP, szReturn, services ) )
		{
			//sprintf( szReturn, "error=%s$","连接目标机器失败！");
			CoUninitialize();
			return;
		}		

		try
		{
			unsigned long nBeginTime(0), nEndTime(0);
			nBeginTime = ::GetTickCount();
			nEndTime = ::GetTickCount();


			//取第一次进程CPU占用率数据
			CString strCom  = szCommand;
			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(LPCTSTR(strCom),"WQL",WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,NULL);
			
			if (NULL == objects) 
			{
				services.Release ();
				CoUninitialize ();
				return;
			}

			/*
			if(objects->GetCount() == 0)
			{
				sprintf(szReturn, "error=The Process %s is not available!$", strProcessName);
				services.Release ();
				objects.Release ();
				CoUninitialize();
				return FALSE;
			}
			*/
			
			
			

			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 

			if (NULL == obj_enum) 
			{
				services.Release ();
				objects.Release ();
				CoUninitialize ();
				return;
			}


			ULONG fetched; 
			VARIANT var; 
			CString strTemp;
			int nThreadCount = 0;	

			int nCount(0);

			float fPercentProcessorTime=0, fWorkingsetsize=0, fPrivateBytes=0, fPercentProcessorTime1=0, fPercentProcessorTime2=0;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				nCount++;

				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("PercentProcessorTime",0);
				fPercentProcessorTime1 += (float)prop->GetValue()/100000;
				prop.Release ();

				VariantClear (&var);
				object.Release ();
				properties.Release ();
				
			}	

			obj_enum.Release();
			obj_enum = NULL;
			objects.Release ();
			objects = NULL;

			Sleep(1000);
			//取第二次进程CPU占用率数据
			objects = services->ExecQuery(LPCTSTR(strCom),"WQL",WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,NULL); 

			if (NULL == objects) 
			{
				services.Release ();
				CoUninitialize ();
				return;
			}

			obj_enum = objects->Get_NewEnum(); 

			if (NULL == obj_enum) 
			{
				services.Release ();
				objects.Release ();
				CoUninitialize ();
				return;
			}

			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("ThreadCount",0);
				nThreadCount += (int)prop->GetValue(); 
				prop.Release ();

				prop = properties->Item("PercentProcessorTime",0);
				fPercentProcessorTime2 += (float)prop->GetValue()/100000;
				prop.Release ();

				prop = properties->Item("WorkingSet",0);
				fWorkingsetsize += (float)prop->GetValue()/1024;
				prop.Release ();

				prop = properties->Item("PrivateBytes",0);
				fPrivateBytes += (float)prop->GetValue()/1024;
				prop.Release ();

				VariantClear (&var);
				object.Release ();
				properties.Release ();
		

			}

			fPercentProcessorTime = fPercentProcessorTime2 - fPercentProcessorTime1;
			if(fPercentProcessorTime < 0)
				fPercentProcessorTime = 0;
			if(fPercentProcessorTime > 100)
				fPercentProcessorTime = 100;


			obj_enum.Release();
			obj_enum = NULL;
			objects.Release ();
			objects = NULL;

			
			float fTotalPhyMem=0, fTotalVirMem=0;
			objects = services->InstancesOf("CIM_OperatingSystem", WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL);

			if (NULL == objects) 
			{
				services.Release ();
				CoUninitialize ();
				return;
			}

			obj_enum = objects->Get_NewEnum(); 

			if (NULL == obj_enum) 
			{
				services.Release ();
				objects.Release ();
				CoUninitialize ();
				return;
			}

			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("TotalVisibleMemorySize",0);
				fTotalPhyMem = (float)prop->GetValue();
				prop.Release ();
				prop = properties->Item("TotalVirtualMemorySize",0);
				fTotalVirMem = (float)prop->GetValue();
				prop.Release ();

				VariantClear (&var);
				object.Release ();
				properties.Release ();
				
			}
			
			strReturn.Format("ProcessCount=%d$ThreadCount=%d$PercentProcessorTime=%.2f$WorkingSet=%.2f$\
							MemUtilization=%.2f$PrivateBytes=%.2f$VirUtilization=%.2f$",
				/*objects->GetCount()*/nCount, nThreadCount, fPercentProcessorTime, fWorkingsetsize, fWorkingsetsize/fTotalPhyMem*100,
				fPrivateBytes, fPrivateBytes/fTotalVirMem*100);
			strReturn.Replace("\t", NULL);


			if( NULL != obj_enum )
				obj_enum.Release ();
			if( NULL != objects ) 
				objects.Release ();
			if( NULL != services)
				services.Release();

			nEndTime = ::GetTickCount();
			//cout << "处理数据用时：" << (nEndTime-nBeginTime) << endl;
			
		}
		catch (_com_error err) 
		{
			IErrorInfo * ei = err.ErrorInfo();
			BSTR strDesEI;
			ei->GetDescription(&strDesEI);
			char* pstr = _com_util::ConvertBSTRToString(strDesEI);
			sprintf(szReturn, "error=Error ocured:%x: %s$", (unsigned)err.Error(), pstr );

			ei->Release();
			free( pstr );
			::SysFreeString( strDesEI );				

			if( services != NULL )
				services.Release();

			CoUninitialize();

			return;
		} 
		catch(...) 
		{ 
			sprintf(szReturn, "error=Error ocured: %d$",::GetLastError());
			OutputDebugString(szReturn);
			
			if( services != NULL )
				services.Release();

			CoUninitialize();
			return;
		} 

		if( strReturn == "" )
		{
			sprintf( szReturn, "error=no data!$" );
		}
		else
		{
			strcpy(szReturn,strReturn);
		}

	}
	CoUninitialize();

	return;
}

void GetAllCPURate( const char* szM, const char* szU, const char* szP, const char* szCommand, char* szReturn )
{
	CoInitialize(NULL);
	{
		CString strReturn = "detailutilization=";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer( szM, szU, szP, szReturn, services ) )
		{
			//sprintf( szReturn, "error=%s$","连接目标机器失败！");
			CoUninitialize();
			return;
		}
		try
		{
			WbemScripting::ISWbemObjectSetPtr objects = services->InstancesOf("Win32_Processor", WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL);
			if (NULL == objects) 
			{
				services.Release ();
				CoUninitialize ();
				return;
			}

			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum();
			if (NULL == obj_enum) 
			{
				services.Release ();
				objects.Release ();
				CoUninitialize ();
				return;
			}

			ULONG fetched; 
			VARIANT var; 
			int nCpuRate = 0, nCount =0, nTotalRate = 0;
			CString strTemp;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("LoadPercentage",0);
				_variant_t value = prop->GetValue();
				prop.Release ();
				nCpuRate = (int)value;				
				prop = properties->Item("DeviceID",0);
				strTemp.Format("%s:%d,", (const char*)_bstr_t(prop->GetValue()), nCpuRate);
				prop.Release ();

				strReturn += strTemp;
				nTotalRate += nCpuRate;
				nCount++;

				VariantClear (&var);
				object->Delete_;
				object.Release ();
				properties.Release ();

			}
			strReturn.TrimRight(',');
			strTemp.Format("$utilization=%d$", nTotalRate/nCount);
			strReturn += strTemp;

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
			sprintf(szReturn, "error=Error ocured:%x: %s$", (unsigned)err.Error(), pstr );

			ei->Release();
			free( pstr );
			::SysFreeString( strDesEI );

			if( services != NULL )
				services.Release();

			CoUninitialize();
			return;
		} 
		catch(...) 
		{ 
			sprintf(szReturn, "error=Error ocured: %d$",::GetLastError());
			OutputDebugString(szReturn);

			if( services != NULL )
				services.Release();

			CoUninitialize();
			return;
		} 

		if( strReturn == "" )
		{
			sprintf( szReturn, "error=no data!$" );
		}
		else
		{
			strcpy(szReturn,strReturn);
		}

	}

	CoUninitialize();

	return ;
}

void GetDiskInfo( const char* szM, const char* szU, const char* szP, const char* szCommand, char* szReturn )
{
	CoInitialize(NULL);
	{
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer( szM, szU, szP, szReturn, services ) )
		{
			//sprintf( szReturn, "error=%s$","连接目标机器失败！");
			CoUninitialize();
			return;
		}
		try
		{
			CString strCom = szCommand;			
			
			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(LPCTSTR(strCom),"WQL",WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,NULL); 
			if (NULL == objects) 
			{
				services.Release ();
				CoUninitialize ();
				return;
			}

			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
			if (NULL == obj_enum) 
			{
				services.Release ();
				objects.Release ();
				CoUninitialize ();
				return;
			}

			ULONG fetched; 
			VARIANT var; 
			float nSize = 0, nFreeSize =0;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("Size",0);
				_variant_t value = prop->GetValue();
				prop.Release ();
				nSize = (float)value;				
				prop = properties->Item("FreeSpace",0);
				value = prop->GetValue();
				prop.Release ();

				nFreeSize = (float)value;	
				strReturn.Format("percentFull=%.2f$Mbfree=%.2f$TotalSize=%.2f$",
					((nSize - nFreeSize) / nSize) * 100 , nFreeSize/1048576, nSize/1048576);

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
			sprintf(szReturn, "error=Error ocured:%x: %s$", (unsigned)err.Error(), pstr );

			ei->Release();
			free( pstr );
			::SysFreeString( strDesEI );

			if( services != NULL )
				services.Release();

			CoUninitialize();
			return;
		} 
		catch(...) 
		{ 
			sprintf(szReturn, "error=Error ocured: %d$",::GetLastError());
			OutputDebugString(szReturn);

			if( services != NULL )
				services.Release();

			CoUninitialize();
			return;
		} 

		if( strReturn == "" )
		{
			sprintf( szReturn, "error=no data!$" );
		}
		else
		{
			strcpy(szReturn,strReturn);
		}

	}
	CoUninitialize();

	return;
}


void GetMemoryInfo( const char* szM, const char* szU, const char* szP, const char* szCommand, char* szReturn )
{
	CoInitialize(NULL);
	{
		char strTMP[1024]=_T("");
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer( szM, szU, szP, szReturn, services ) )
		{
			//sprintf( szReturn, "error=%s$","连接目标机器失败！");
			CoUninitialize();
			return;
		}
		try
		{
			WbemScripting::ISWbemObjectSetPtr objects = services->InstancesOf("CIM_OperatingSystem", WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL);
			if (NULL == objects) 
			{
				services.Release ();
				CoUninitialize ();
				return;
			}

			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
			if (NULL == obj_enum) 
			{
				services.Release ();
				objects.Release ();
				CoUninitialize ();
				return;
			}


			ULONG fetched; 
			VARIANT var; 
			double fTotalPhyMem=0, fFreePhyMem=0, fTotalVirMem =0, fFreeVirMem=0;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("TotalVisibleMemorySize",0);

				fTotalPhyMem = ((double)prop->GetValue())/1024;
				prop.Release ();
				prop = properties->Item("TotalVirtualMemorySize",0);
				fTotalVirMem = ((double)prop->GetValue())/1024;
				prop.Release ();
				prop = properties->Item("FreePhysicalMemory",0);
				fFreePhyMem = ((double)prop->GetValue())/1024;
				prop.Release ();
				prop = properties->Item("FreeVirtualMemory",0);
				fFreeVirMem = ((double)prop->GetValue())/1024;
				prop.Release ();

				strReturn.Format("TotalPhyMem=%.2f$TotalMemory=%.2f$FreePhyMem=%.2f$Mbfree=%.2f$PhyMemUsage=%.2f$percentUsed=%.2f$", 
					fTotalPhyMem, fTotalVirMem, fFreePhyMem, fFreeVirMem, (fTotalPhyMem-fFreePhyMem)/fTotalPhyMem*100, 
					(fTotalVirMem-fFreeVirMem)/fTotalVirMem*100);

				properties.Release();
				object.Release();				
				VariantClear (&var);

			}

			VariantClear (&var);
			obj_enum.Release();
			obj_enum = NULL;
			objects.Release ();
			objects = NULL;

			//取第一次错误页面处理数据
			objects = services->InstancesOf("Win32_PerfRawData_PerfOS_Memory", WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL);
			if (NULL == objects) 
			{
				services.Release ();
				CoUninitialize ();
				return;
			}

			obj_enum = objects->Get_NewEnum(); 
			if (NULL == obj_enum) 
			{
				services.Release ();
				objects.Release ();
				CoUninitialize ();
				return;
			}

			double fPagesPersec1=0, fTimestamp_PerfTime1=0, fPagesPersec2=0, fTimestamp_PerfTime2=0, fFrequency_PerfTime=0;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("PagesPersec",0);
				fPagesPersec1 = (double)prop->GetValue();
				prop.Release ();
				prop = properties->Item("Timestamp_PerfTime",0);
				fTimestamp_PerfTime1 = (double)prop->GetValue();
				prop.Release ();

				VariantClear (&var);
				object.Release ();
				properties.Release ();	

			}

			obj_enum.Release();
			obj_enum = NULL;
			objects.Release ();
			objects = NULL;


			//取第二次错误页面处理数据
			objects = services->InstancesOf("Win32_PerfRawData_PerfOS_Memory", WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL);
			if (NULL == objects) 
			{
				services.Release ();
				CoUninitialize ();
				return;
			}

			obj_enum = objects->Get_NewEnum(); 
			if (NULL == obj_enum) 
			{
				services.Release ();
				objects.Release ();
				CoUninitialize ();
				return;
			}

			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("PagesPersec",0);
				fPagesPersec2 = (double)prop->GetValue();
				prop.Release ();
				prop = properties->Item("Timestamp_PerfTime",0);
				fTimestamp_PerfTime2 = (double)prop->GetValue();
				prop.Release ();
				prop = properties->Item("Frequency_PerfTime",0);
				fFrequency_PerfTime = (double)prop->GetValue();
				prop.Release ();

				VariantClear (&var);
				object.Release ();
				properties.Release ();

			}
			double fPagesPersec = (fPagesPersec2-fPagesPersec1)/((fTimestamp_PerfTime2-fTimestamp_PerfTime1)/fFrequency_PerfTime);
			CString strTemp;
			strTemp.Format("pagesPerSec=%.2f$", fPagesPersec);
			strReturn += strTemp;


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
			sprintf(szReturn, "error=Error ocured:%x: %s$", (unsigned)err.Error(), pstr );

			ei->Release();
			free( pstr );
			::SysFreeString( strDesEI );

			if( services != NULL )
				services.Release();

			CoUninitialize();
			return;
		} 
		catch(...) 
		{ 
			sprintf(szReturn, "error=Error ocured: %d$",::GetLastError());
			OutputDebugString(szReturn);

			if( services != NULL )
				services.Release();

			CoUninitialize();
			return;
		} 

		if( strReturn == "" )
		{
			sprintf( szReturn, "error=no data!$" );
		}
		else
		{
			strcpy(szReturn,strReturn);
		}	

	}
	CoUninitialize();

	return;
}



void GetCPURate( const char* szM, const char* szU, const char* szP, const char* szCommand, char* szReturn )
{
	CoInitialize(NULL);
	{
		map<int, float> lPerProTime1;	//保存第一次获取的PID及其占用CPU时间
		map<CString,float> mPerProTime;
		typedef pair<float,CString> TestPair;
		multimap<float,CString,greater<float> > testmap;
		int iPIDNum1=0,iPIDNum2=0;		//PID号
		CString strProName =_T("");	//获取到的进程名列表
		float iTotalRate=0;			//不包含Idle运行的CPU占用时间
		float fTotalTime=0.0;		//包含Idle运行的CPU占用时间
		CString strResult = _T("");
		CString strReturn = "detailutilization=";
		CString strWQL = _T("");
		strWQL.Format("%s","SELECT * FROM Win32_PerfRawData_PerfProc_Process where Name<>'_Total'");
		WbemScripting::ISWbemServicesPtr services;

		if(!ConnectServer( szM, szU, szP, szReturn, services ) )
		{
			//sprintf( szReturn, "error=%s$","连接目标机器失败！");
			CoUninitialize();
			return;
		}
		try
		{
			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(LPCTSTR(strWQL),"WQL",WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,NULL);
			if (NULL == objects) 
			{
				services.Release ();
				CoUninitialize ();
				return;
			}

			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
			if (NULL == obj_enum) 
			{
				services.Release ();
				objects.Release ();
				CoUninitialize ();
				return;
			}

			ULONG fetched; 
			VARIANT var; 
/************************第一次取进程CPU占用率数据************************/
			int nThreadCount = 0;
			char strTest[1024]={0};
			float fPercentProcessorTime=0, fWorkingsetsize=0, fPrivateBytes=0, fPercentProcessorTime1=0, fPercentProcessorTime2=0;	
			int nRoundNum = 0;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("PercentProcessorTime",0);
				fPercentProcessorTime1 = (float)prop->GetValue()/100000;				
				prop = properties->Item("IDProcess",0);									//第一次获取PID，用于与下一次获取的对比。
				iPIDNum1 = (int)prop->GetValue();
				lPerProTime1[iPIDNum1] = fPercentProcessorTime1;
				nRoundNum++;

				VariantClear (&var);
				object.Release ();
				properties.Release ();
				prop.Release ();

			}
			if(nRoundNum==0)
			{
				//sprintf(szReturn,"error=%s$","WMI库缺少Win32_PerfRawData_PerfProc_Process类");
				sprintf( szReturn, "utilization=-1$PerProUse=$" );
				if( NULL != obj_enum )
					obj_enum.Release ();
				if( NULL != objects ) 
					objects.Release ();
				if( NULL != services)
					services.Release();

				return;
			}

			obj_enum.Release();
			obj_enum = NULL;
			objects.Release ();
			objects = NULL;

			Sleep(1000);
/************************第二次取进程CPU占用率数据************************/
			objects = services->ExecQuery(LPCTSTR(strWQL),"WQL",WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,NULL); 
			if (NULL == objects) 
			{
				services.Release ();
				CoUninitialize ();
				return;
			}

			obj_enum = objects->Get_NewEnum();
			if (NULL == obj_enum) 
			{
				services.Release ();
				objects.Release ();
				CoUninitialize ();
				return;
			}

			float TempValue=0;
			CString strTemp=_T("");
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("PercentProcessorTime",0);
				fPercentProcessorTime2 = (float)prop->GetValue()/100000;
				prop = properties->Item("IDProcess",0);									//第二次获取PID
				iPIDNum2 = prop->GetValue();
				prop = properties->Item("Name",0);										//获取进程名列表
				strProName = prop->GetValue();
/************************对数据进行处理************************/
				if(lPerProTime1.find(iPIDNum2)!=lPerProTime1.end())
				{				
					TempValue=lPerProTime1.find(iPIDNum2)->second;
					TempValue=fPercentProcessorTime2-TempValue;
					if(TempValue!=0)
					{
						if(iPIDNum2!=0)
						{
							iTotalRate+=TempValue;
//							mPerProTime[strProName]=TempValue;
							testmap.insert(TestPair(TempValue , strProName));
						}
						fTotalTime+=TempValue;						
					}					
				}
				else
				{
					if(iPIDNum2!=0)
					{
						iTotalRate+=fPercentProcessorTime2;
						fTotalTime+=fPercentProcessorTime2;
						mPerProTime[strProName]=fPercentProcessorTime2;
						testmap.insert(TestPair(fPercentProcessorTime2,strProName));
						//					testmap[fPercentProcessorTime2] = strProName;
					}
				}

				VariantClear (&var);
				object.Release ();
				properties.Release ();
				prop.Release ();

			}
/*			
			map<CString,float>::iterator ptrMap=mPerProTime.begin();
			while(ptrMap!=mPerProTime.end())
			{
				ptrMap->second=100*(ptrMap->second)/fTotalTime;
				strTemp.Format("%s:%0.2f,",ptrMap->first,ptrMap->second);
				strResult+=strTemp;
				ptrMap++;
			}
*/
			multimap<float,CString,greater<float> >::iterator ptrTestMap = testmap.begin();
			while(ptrTestMap!=testmap.end())
			{
//				ptrTestMap->first = 100*(ptrTestMap->first)/fTotalTime;
				strTemp.Format("%s:%0.2f,",ptrTestMap->second, (100*(ptrTestMap->first)/fTotalTime));
				strResult+=strTemp;
				ptrTestMap++;
			}
			strResult.Delete(strResult.GetLength()-1,1);		//去除末尾多余的“,”
			strResult = "PerProUse="+strResult;
			strReturn.Format("utilization=%0.2f$%s$", iTotalRate*100/fTotalTime, strResult.GetBuffer() );

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
			sprintf(szReturn, "error=Error ocured:%x: %s$", (unsigned)err.Error(), pstr );

			ei->Release();
			free( pstr );
			::SysFreeString( strDesEI );


			if( services != NULL )
				services.Release();

			CoUninitialize();
			return;
		} 
		catch(...) 
		{ 
			sprintf(szReturn, "error=Error ocured: %d$",::GetLastError());
			OutputDebugString(szReturn);

			if( services != NULL )
				services.Release();

			CoUninitialize();
			return;
		} 

		if( strReturn == "" )
		{
			sprintf( szReturn, "error=no data!$" );
		}
		else
		{
			strcpy(szReturn,strReturn);
		}
		
			 
	}
	CoUninitialize();

}




int main( int argc, char **argv )
{

	char szFunName[128] = {0};
	char szMachineName[128] = "192.168.0.51";
	char szUser[128] = "administrator";
	char szPswd[128] = "888888";
	char szCommand[128] = {0};

	char szRet[10240] = {0};
	char szReturn[10240] = {0};


	/*
	char szCommandService[256] = "SELECT * FROM Win32_Service where DisplayName='Java Quick Starter'";
	GetServiceInfo( szMachineName, szUser, szPswd, szCommandService, szReturn );

	cout << szReturn << endl;
	
	
	unsigned long nBeginTime(0), nEndTime(0);
	nBeginTime = ::GetTickCount();
	nEndTime = ::GetTickCount();

	char szCommandProcess[256] = "SELECT * FROM Win32_PerfRawData_PerfProc_Process where Name='services'";
	GetProcessInfo( szMachineName, szUser, szPswd, szCommandProcess, szReturn );

	cout << szReturn << endl;

	nEndTime = ::GetTickCount();
	cout << "获取数据用时：" << (nEndTime-nBeginTime) << endl;
	

	char szCommandDisk[256] = "SELECT * FROM Win32_LogicalDisk where MediaType=12 and DeviceID='C:'";
	GetDiskInfo( szMachineName, szUser, szPswd, szCommandDisk, szReturn );

	cout << szReturn << endl;
	
	
	GetMemoryInfo( szMachineName, szUser, szPswd, "NULL", szReturn );

	cout << szReturn << endl;
	
	

	GetAllCPURate( szMachineName, szUser, szPswd, "NULL", szReturn );

	cout << szReturn << endl;
	

	GetCPURate( szMachineName, szUser, szPswd, "NULL", szReturn );

	cout << szReturn << endl;
	*/



	if( argc < 6 )
	{
		sprintf( szReturn, "error=Lack input parameter!$" );
	}
	else
	{
		strcpy( szFunName, argv[1] );
		strcpy( szMachineName, argv[2] );
		strcpy( szUser, argv[3] );
		strcpy( szPswd, argv[4] );

		if( strcmp( szFunName, "GetServiceInfo") == 0 )
		{
            sprintf( szCommand, "SELECT * FROM Win32_Service where DisplayName='%s'", argv[5] );

			GetServiceInfo( szMachineName, szUser, szPswd, szCommand, szReturn );
		}
		else if( strcmp( szFunName, "GetProcessInfo") == 0 )
		{
			sprintf( szCommand, "SELECT * FROM Win32_PerfRawData_PerfProc_Process where Name='%s'", argv[5] );

			GetProcessInfo( szMachineName, szUser, szPswd, szCommand, szReturn );
		}
		else if( strcmp( szFunName, "GetAllCPURate") == 0 )
		{
			sprintf( szCommand, "%s", argv[5] );

			GetAllCPURate( szMachineName, szUser, szPswd, szCommand, szReturn );
		}
		else if( strcmp( szFunName, "GetDiskInfo") == 0 )
		{
			sprintf( szCommand, "SELECT * FROM Win32_LogicalDisk where MediaType=12 and DeviceID='%s'", argv[5] );

			GetDiskInfo( szMachineName, szUser, szPswd, szCommand, szReturn );
		}
		else if( strcmp( szFunName, "GetMemoryInfo") == 0 )
		{
			sprintf( szCommand, "%s", argv[5] );

			GetMemoryInfo( szMachineName, szUser, szPswd, szCommand, szReturn );
		}
		else if( strcmp( szFunName, "GetCPURate") == 0 )
		{
			sprintf( szCommand, "%s", argv[5] );

			GetCPURate( szMachineName, szUser, szPswd, szCommand, szReturn );
		}
		else 
		{
			sprintf( szReturn, "error=Input parameter is incorrect!$" );
		}

	}
	
	

	sprintf( szRet, "DragonFlow:%s", szReturn );
	//cout << szRet << endl;	
	
	
	
	//获取父进程的标准输出句柄

	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	DWORD  dwWritten;

	WriteFile( hStdout, szRet, strlen(szRet), &dwWritten, NULL );

	CloseHandle(hStdout);

	


}