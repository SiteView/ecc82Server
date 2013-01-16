#include "stdafx.h"
#include <list>
#include <fstream>
//#include "../../kennel/svdb/svapi/svdbapi.h"
#include <scsvapi/svdbapi.h>

#include<stdio.h> 
#include<winsock2.h>
#include <atlstr.h>
#pragma comment(lib,"ws2_32.lib")

#import "progid:WbemScripting.SWbemLocator" named_guids

//#include <fstream>
using namespace std;
//typedef struct std::list<char*> StringList;
typedef class std::list<char*> StringList;


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

//将输入参数拆入一个 list<string> 中
bool MakeStringListByChar(list<string> & pList, const char * pInput )
{
	const char * p;
	int nSize;
	p=pInput;
	while(*p!='\0')
	{
		nSize =(int)strlen(p);
		if( nSize>0 )
			pList.push_back(p);
		p=p+nSize+1;
	}

	return true;
}// end of MakeStringListByChar


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



//使用CString类，无需申请和释放内存
CString FindStrValue(const char *strParas, CString str)
{
	//char *pValue = NULL;
	CString strValue = "";
	string m_TempStr;

	list< string > strList;
	MakeStringListByChar(strList, strParas);
	list< string >::iterator pos = strList.begin();

	while(pos != strList.end())
	{
		//CString strTemp = strList.GetNext(pos);
		//char * strTemp = *pos;
		string strTemp1 = *pos;
		int m_Fpos = 0;

		if((m_Fpos = static_cast<int>(strTemp1.find(str, 0))) >= 0)
		{
			m_TempStr = strTemp1.substr( m_Fpos + strlen(str)+1, strTemp1.size() - strlen(str) - 1); 
			//pValue=(char*)malloc(m_TempStr.size()+1);
			//strcpy(pValue, m_TempStr.c_str());
			strValue = m_TempStr.c_str();

		}
		pos++;
	}

	return strValue;

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
				char strTemp[128] = {0};
				int i = 0;

				while(hostinfo->h_addr_list[i]!=NULL)
				{
					//ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list); 
					ip = inet_ntoa (*(struct in_addr *)hostinfo->h_addr_list[i]);
					if(strcmp(sHostName.GetBuffer(1), ip) == 0)
					{
						printf("IP =%s\n",ip);
						return TRUE;
					}
					i++;
				}

			} 
			//			if((hostinfo = gethostbyname(name)) != NULL) 
			//			{ //这些就是获得IP的函数
			//				ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list); 
			////				 ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list[i]);
			//
			//				if(sHostName == ip)
			//				{
			//					printf("IP =%s\n",ip);
			//					return TRUE;
			//				}
			//			} 
		} 
		WSACleanup( );
	}
	return FALSE;
}


BOOL ConnectServer(const char * strParas, char * szReturn, int& nSize,WbemScripting::ISWbemServicesPtr &services)
{
	char *machinename=NULL, *user=NULL, *pswd=NULL;
	//machinename = FindStrValue(strParas, "_MachineName");
	//user = FindStrValue(strParas, "_UserAccount");
	//pswd = FindStrValue(strParas, "_PassWord");

	CString strMachine=_T("");
	CString strUser = _T("");
	CString strPSWD = _T("");
	strMachine = FindStrValue(strParas, "_MachineName");
	strUser  = FindStrValue(strParas, "_UserAccount");
	strPSWD  = FindStrValue(strParas, "_PassWord");
	//CoInitialize(NULL);
	try 
	{ 
		WbemScripting::ISWbemLocatorPtr locator; 
		locator.CreateInstance(WbemScripting::CLSID_SWbemLocator);

		if (locator != NULL) 
		{ 
			//if(IsLocalHost(machinename))
			if(IsLocalHost(strMachine))
			{
				printf("是本机\n\n");
				services = locator->ConnectServer(".","root\\cimv2","","","","",0,NULL);
			}
			else
			{
				printf("不是本机\n\n");
				services = locator->ConnectServer(strMachine.GetBuffer(),"root\\cimv2",
					strUser.GetBuffer(),strPSWD.GetBuffer(),"","",0,NULL);
				strMachine.ReleaseBuffer();
				strUser.ReleaseBuffer();
				strPSWD.ReleaseBuffer();
			}
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
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(strParas, szReturn, nSize, services))
			return FALSE;

		try
		{
			_bstr_t com; 
			com="SELECT * FROM Win32_LogicalDisk where MediaType=12"; 
			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(com,"WQL",0x10,NULL); 			
			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
			ULONG fetched; 
			VARIANT var; 
			CString strTemp;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("DeviceID",0);

				strTemp.Format("%s=%s$", (const char*)_bstr_t(prop->GetValue()), (const char*)_bstr_t(prop->GetValue()));
				strReturn += strTemp;
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
			OutputDebugString(szReturn);
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
BOOL GetDiskInfo(const char * strParas, char * szReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CoInitialize(NULL);
	{
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(strParas, szReturn, nSize, services))
			return FALSE;
		try
		{
			//char *disk=NULL;
			CString disk = _T("");
			disk = FindStrValue(strParas, "_Disk");
			CString strCom;
			strCom.Format("SELECT * FROM Win32_LogicalDisk where MediaType=12 and DeviceID='%s'", disk);
			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(LPCTSTR(strCom),"WQL",0x10,NULL); 
			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
			ULONG fetched; 
			VARIANT var; 
			float nSize = 0, nFreeSize =0;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("Size",0);
				_variant_t value = prop->GetValue();
				nSize = (float)value;				
				prop = properties->Item("FreeSpace",0);
				value = prop->GetValue();
				nFreeSize = (float)value;	
				strReturn.Format("percentFree=%.2f$Mbfree=%.2f$TotalSize=%.2f$",
					( nFreeSize / nSize) * 100 , nFreeSize/1048576, nSize/1048576);
			}

			//取第一次磁盘设备数据
			strCom.Format("SELECT * FROM Win32_PerfRawData_PerfDisk_LogicalDisk Where Name='%s'",disk);
			double fBytesReadPerSec1=0, fBytesWritePerSec1=0, fBytesTransfersPerSec1=0, fTimestamp_PerfTime1 = 0,
				fFrequency_PerfTime = 0;
			objects = services->ExecQuery(LPCTSTR(strCom), "WQL", 0x10, NULL);
			obj_enum = objects->Get_NewEnum();			
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				cout << "------------- obj_enum->Next == S_OK -----" << endl;
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("DiskReadBytesPersec",0);
				CString str = prop->GetValue();
				cout << "str:" << str << endl;
				fBytesReadPerSec1 = (float)prop->GetValue();				
				prop = properties->Item("DiskWriteBytesPersec",0);
				fBytesWritePerSec1 = (double)prop->GetValue();
				prop = properties->Item("DiskBytesPerSec",0);
				fBytesTransfersPerSec1 = (double)prop->GetValue();
				prop = properties->Item("Timestamp_PerfTime",0);
				fTimestamp_PerfTime1 = (double)prop->GetValue();
				cout << "------- 取第一次磁盘设备数据 --------"<<endl;
				cout <<"fBytesReadPerSec1="<<fBytesReadPerSec1<<endl;
				cout <<"fBytesWritePerSec1="<<fBytesWritePerSec1<<endl;
				cout <<"fBytesTransfersPerSec1="<<fBytesTransfersPerSec1<<endl;
				cout <<"fTimestamp_PerfTime1="<<fTimestamp_PerfTime1<<endl;
			}

			cout << "------- 取第一次磁盘设备数据 --------"<<endl;
			cout <<"fBytesReadPerSec1="<<fBytesReadPerSec1<<endl;
			cout <<"fBytesWritePerSec1="<<fBytesWritePerSec1<<endl;
			cout <<"fBytesTransfersPerSec1="<<fBytesTransfersPerSec1<<endl;
			cout <<"fTimestamp_PerfTime1="<<fTimestamp_PerfTime1<<endl;

			Sleep(2000);

			//取第二次磁盘设备数据
			double fBytesReadPerSec2=0, fBytesWritePerSec2=0, fBytesTransfersPerSec2=0, fTimestamp_PerfTime2 = 0;
			objects = services->ExecQuery(LPCTSTR(strCom), "WQL", 0x10, NULL);
			obj_enum = objects->Get_NewEnum();			
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("DiskReadBytesPersec",0);
				fBytesReadPerSec2 = (double)prop->GetValue();				
				prop = properties->Item("DiskWriteBytesPersec",0);
				fBytesWritePerSec2 = (double)prop->GetValue();
				prop = properties->Item("DiskBytesPerSec",0);
				fBytesTransfersPerSec2 = (double)prop->GetValue();
				prop = properties->Item("Timestamp_PerfTime",0);
				fTimestamp_PerfTime2 = (double)prop->GetValue();
				prop = properties->Item("Frequency_PerfTime",0);
				fFrequency_PerfTime = (double)prop->GetValue();

				cout << "------- 取第二次磁盘设备数据 --------"<<endl;
				cout <<"fBytesReadPerSec2="<<fBytesReadPerSec2<<endl;
				cout <<"fBytesWritePerSec2="<<fBytesWritePerSec2<<endl;
				cout <<"fBytesTransfersPerSec2="<<fBytesTransfersPerSec2<<endl;
				cout <<"fTimestamp_PerfTime2="<<fTimestamp_PerfTime2<<endl;
				cout <<"fFrequency_PerfTime"<<fFrequency_PerfTime<<endl;
			}

			cout << "------- 取第二次磁盘设备数据 --------"<<endl;
			cout <<"fBytesReadPerSec2="<<fBytesReadPerSec2<<endl;
			cout <<"fBytesWritePerSec2="<<fBytesWritePerSec2<<endl;
			cout <<"fBytesTransfersPerSec2="<<fBytesTransfersPerSec2<<endl;
			cout <<"fTimestamp_PerfTime2="<<fTimestamp_PerfTime2<<endl;
			cout <<"fFrequency_PerfTime"<<fFrequency_PerfTime<<endl;


			double fInterval = 1;
			INT64 nFlag = fFrequency_PerfTime;
			if (nFlag != 0)
			{
				fInterval = (fTimestamp_PerfTime2-fTimestamp_PerfTime1)/fFrequency_PerfTime*1024;
			}
			
			
			cout <<"fInterval="<<fInterval<<endl;
			double fReadPerSec=0, fWritePerSec=0, fTransfersPerSec=0;
			fReadPerSec		= (fBytesReadPerSec2 - fBytesReadPerSec1)/fInterval;
			fWritePerSec	= (fBytesWritePerSec2 - fBytesWritePerSec1)/fInterval;
			fTransfersPerSec= (fBytesTransfersPerSec2 - fBytesTransfersPerSec1)/fInterval;
			CString strTemp = _T("");
			strTemp.Format("ReadPerSec=%.2f$WritePerSec=%.2f$TransfersPerSec=%.2f$",fReadPerSec,fWritePerSec, fTransfersPerSec);
			strReturn +=strTemp;
			

		}
		catch (_com_error err) 
		{ 
			char buf[200] = {0};
			IErrorInfo * ei = err.ErrorInfo();
			BSTR strDesEI;
			ei->GetDescription(&strDesEI);
			sprintf(szReturn, "error=Error ocured:%x: %s", (unsigned)err.Error(),_com_util::ConvertBSTRToString(strDesEI));
			CoUninitialize();
			return FALSE;
		} 
		catch(...) 
		{ 
			sprintf(szReturn, "error=Error ocured: %d",::GetLastError());
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
BOOL GetDiskDeviceInfo(const char * strParas, char * strReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CoInitialize(NULL);
	{
		//CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(strParas, strReturn, nSize, services))
		{
			CoUninitialize();
			return FALSE;
		}

		try
		{
			CString strCom;
			strCom.Format("SELECT * FROM Win32_DiskDrive");
			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(LPCTSTR(strCom),"WQL",0x10,NULL); 
			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
			ULONG fetched; 
			VARIANT var; 
			double dSize=0;
			CString strStatus = _T("");
			while(obj_enum->Next(1,&var,&fetched) == S_OK)
			{
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("Size",0);
				dSize = (double)prop->GetValue();

				prop = properties->Item("Status",0);
				strStatus = prop->GetValue();

				cout << "dSize:" << dSize << endl;
				cout << "strStatus:" << strStatus << endl;

			}
			dSize /= (1024*1024);
			char * szStatus = strStatus.GetBuffer();

			sprintf(strReturn,"_DiskSize=%.2f$_DiskStatus=%s$",dSize,szStatus);

		}
		catch (_com_error err)
		{
			char buf[200] = {0};
			IErrorInfo * ei = err.ErrorInfo();
			BSTR strDesEI;
			ei->GetDescription(&strDesEI);
			sprintf(strReturn, "error=Error ocured:%x: %s", (unsigned)err.Error(),_com_util::ConvertBSTRToString(strDesEI));
			CoUninitialize();
			return FALSE;
			
		}
		catch (...)
		{
			sprintf(strReturn, "error=Error ocured: %d",::GetLastError());
			//OutputDebugString(strReturn);
			CoUninitialize();
			return FALSE;
		}
		
		strcpy(strReturn,strReturn);
		CString strOutRet;
		strOutRet =strReturn;
		nSize = 2048;
		MakeCharByString(strReturn,nSize,strOutRet);	 
	}
	CoUninitialize();
	return TRUE;
}



extern "C" _declspec(dllexport) 
BOOL GetAllCPURate(const char * strParas, char * szReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CoInitialize(NULL);
	{
		CString strReturn = "detailutilization=";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(strParas, szReturn, nSize, services))
			return FALSE;
		try
		{
			WbemScripting::ISWbemObjectSetPtr objects = services->InstancesOf("Win32_Processor", 0x10, NULL);
			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
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
				nCpuRate = (int)value;				
				prop = properties->Item("DeviceID",0);

				strTemp.Format("%s:%d,", (const char*)_bstr_t(prop->GetValue()), nCpuRate);
				strReturn += strTemp;
				nTotalRate += nCpuRate;
				nCount++;
			}
			strReturn.TrimRight(',');
			strTemp.Format("$utilization=%d$", nTotalRate/nCount);
			strReturn += strTemp;
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
			OutputDebugString(szReturn);
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


BOOL ConnectServerForTemperature(const char * strParas, char * szReturn, int& nSize,WbemScripting::ISWbemServicesPtr &services)
{
	char *machinename=NULL, *user=NULL, *pswd=NULL;

	//machinename = FindStrValue(strParas, "_MachineName");
	//user = FindStrValue(strParas, "_UserAccount");
	//pswd = FindStrValue(strParas, "_PassWord");

	CString strMachine=_T("");
	CString strUser = _T("");
	CString strPSWD = _T("");
	strMachine = FindStrValue(strParas, "_MachineName");
	strUser  = FindStrValue(strParas, "_UserAccount");
	strPSWD  = FindStrValue(strParas, "_PassWord");

	//CoInitialize(NULL);

	try 
	{ 
		WbemScripting::ISWbemLocatorPtr locator; 
		locator.CreateInstance(WbemScripting::CLSID_SWbemLocator);

		if (locator != NULL) 
		{ 
			if(IsLocalHost(strMachine))
			{
				//services = locator->ConnectServer(".","root\\cimv2","","","","",0,NULL);
				services = locator->ConnectServer(".","root\\wmi","","","","",0,NULL);
			}
			else
			{
				//services = locator->ConnectServer(machinename,"root\\cimv2",user,pswd,"","",0,NULL);
				//services = locator->ConnectServer(strMachine.GetBuffer(),"root\\cimv2",
				//strUser.GetBuffer(),strPSWD.GetBuffer(),"","",0,NULL);
				services = locator->ConnectServer(strMachine.GetBuffer(),"root\\wmi",
					strUser.GetBuffer(),strPSWD.GetBuffer(),"","",0,NULL);
				strMachine.ReleaseBuffer();
				strUser.ReleaseBuffer();
				strPSWD.ReleaseBuffer();
			}
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
BOOL GetCPUTemperature(const char * strParas, char * strReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	cout << "------------------ GetCPUTemperature Start -------------------"<<endl;
	
	CoInitialize(NULL);
	{
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServerForTemperature(strParas, strReturn, nSize, services))
		{
			cout << "---------------- ConnectServerForTemperature  failed --------------" << endl;
			CoUninitialize();
			return FALSE;
		}
			
		cout << "---------------- ConnectServerForTemperature success --------------" << endl;
		try
		{
			CString strCom;
			strCom.Format("SELECT * FROM MSAcpi_ThermalZoneTemperature");
			cout << strCom << endl;
			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(LPCTSTR(strCom),"WQL",0x10,NULL); 
			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
			ULONG fetched; 
			VARIANT var; 
			double dTemperature = 0.0;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("CurrentTemperature",0);
				_variant_t value = prop->GetValue();
				dTemperature = (double)((int)value - 2732)/10.00;

				sprintf(strReturn,"cpuTemperature=%.2f$",dTemperature);
			}
			if(objects->Count == 0)
			{
				sprintf(strReturn, "error=The CPU temperature sensor is not available!");
				return FALSE;
			}

		}
		catch (_com_error err)
		{
			char buf[200] = {0};
			IErrorInfo * ei = err.ErrorInfo();
			BSTR strDesEI;
			ei->GetDescription(&strDesEI);
			sprintf(strReturn, "error=Error ocured:%x: %s", (unsigned)err.Error(),_com_util::ConvertBSTRToString(strDesEI));
			CoUninitialize();
			return FALSE;
			
		}
		catch (...)
		{
			sprintf(strReturn, "error=Error ocured: %d",::GetLastError());
			//OutputDebugString(strReturn);
			CoUninitialize();
			return FALSE;
		}
		CString strOutRet;
		strOutRet =strReturn;
		nSize = 2048;
		MakeCharByString(strReturn,nSize,strOutRet);	 
	}

	CoUninitialize();
	
	return TRUE;
}


bool FilePathParser(CString & strFilePath)
{
	CString strTemp = strFilePath ;
	int nPos1=-1, nPos2 =-1;

	if( strTemp.Find("\\\\\\") != -1 )
	{
		return false;
	}

	for(int i=0; i< strTemp.GetLength();i++)
	{
		nPos1 = strTemp.Find( "\\", i );
		if (nPos1 == -1)
		{
			break;
		}
		nPos2 = strTemp.Find("\\",nPos1+1);
		if ( nPos2 != nPos1+1 )
		{
			strTemp.Insert(nPos1+1,"\\");
			if (nPos2 == -1 )
			{
				break;
			}
		}
		i = nPos2;
	}

	strFilePath = strTemp;
	return true;
}



//把输入时间字符串 如（20090803083112），格式输出为（2009-08-03 08:31:12）
CString StrToDateFormat(CString sDateTime)
{

	CString strTime = _T("NULL");
	if (sDateTime.IsEmpty()||(sDateTime.GetLength()<14))
	{
		return strTime;
	}
	strTime.Format("%s-%s-%s %s:%s:%s",sDateTime.Mid(0,4),sDateTime.Mid(4,2),
		sDateTime.Mid(6,2),sDateTime.Mid(8,2),sDateTime.Mid(10,2),sDateTime.Mid(12,2));

	return strTime ;

}


//把输入时间字符串 如（20090803083112）返回一个CTime
//如果输入字符串有误，返回当前时间
CTime StrToDate(CString sDateTime)
{
	if (sDateTime.IsEmpty()||(sDateTime.GetLength()<14))
	{
		CTime curTime = CTime::GetCurrentTime();
		return curTime;
	}
	int iYear = 0, iMonth = 0, iDay =0;
	int iHour = 0, iMinute = 0, iSecond =0;

	CString sYear,sMonth,sDay;
	CString sHour,sMinute,sSecond;

	//int nPos = 0;
	sYear	= sDateTime.Mid(0,4);
	sMonth  = sDateTime.Mid(4,2);
	sDay	= sDateTime.Mid(6,2);
	sHour	= sDateTime.Mid(8,2);
	sMinute = sDateTime.Mid(10,2);
	sSecond = sDateTime.Mid(12,2);

	iYear	= atoi(sYear);
	iMonth	= atoi(sMonth);
	iDay	= atoi(sDay);
	iHour	= atoi(sHour);
	iMinute	= atoi(sMinute);
	iSecond	= atoi(sSecond);

	CTime tTime(iYear, iMonth, iDay, iHour, iMinute, iSecond);
	return tTime;

}

CString FindStrValue( const list< string > paramList, CString str)
{
	//char *pValue = NULL;
	CString strValue="";
	string m_TempStr;

	list< string > strList = paramList;
	list< string >::iterator pos = strList.begin();
	while( pos != strList.end())
	{
		string strTemp = * pos;
		int nPos = 0;
		if ( ( nPos = static_cast<int>(strTemp.find(str,0))) >=0 )
		{
			m_TempStr = strTemp.substr( nPos + strlen(str) + 1 , strTemp.size() - strlen(str) - 1 );
			/*pValue = (char*)malloc( m_TempStr.size() + 1 );
			strcpy( pValue, m_TempStr.c_str() );*/
			strValue = m_TempStr.c_str();
		}
		pos++;
	}

	//return pValue;
	return strValue;
}

extern "C" _declspec(dllexport) 
BOOL GetFileInfo(const char * strParas, char * szReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	list<string> paramList;
	MakeStringListByChar(paramList,strParas); //将输入参数拆入一个 list<string> 中	

	CoInitialize(NULL);
	{
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(strParas, szReturn, nSize, services))
			return false;
		try
		{
			CString filename ="";
			filename = FindStrValue(paramList,"_filepath");
			if(filename.IsEmpty())
			{
				//strReturn.Format("error=Error ocured: File path is NULL");
				sprintf(szReturn, "error=Error ocured: File path is not find ");
				return false;
			}
			bool bRet = FilePathParser(filename);
			if ( !bRet )
			{
				//strReturn.Format("error=Error ocured: File path is error");
				sprintf(szReturn, "error=Error ocured: File path is error");
				return false;
			}

			CString strCom;
			strCom.Format("SELECT * FROM CIM_LogicalFile where Name='%s'",filename);
			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(LPCTSTR(strCom),"WQL",0x10,NULL);
			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
			ULONG fetched; 
			VARIANT var; 
			//CString strSize;
			int nSize=0;
			CString strModifiedTime	= _T("");
			CString strInstallDate	= _T("");
			CString strFileStatus	= _T("");
			bool bFlag = true;
			CTime curTime = CTime::GetCurrentTime();
			CTime installTime;
			CTimeSpan preTime;
			int nLiveDate = 0;
			while(bFlag)
			{
				if (obj_enum->Next(1,&var,&fetched) == S_OK) 
				{ 
					WbemScripting::ISWbemObjectPtr object = var;
					WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
					WbemScripting::ISWbemPropertyPtr prop = properties->Item("FileSize",0);
					_variant_t value = prop->GetValue();
					nSize = (int)value;
					prop = properties->Item("LastModified",0);
					value = prop->GetValue();
					strModifiedTime = value;
					strModifiedTime = StrToDateFormat(strModifiedTime);

					prop = properties->Item("InstallDate",0);
					value = prop->GetValue();
					strInstallDate = value;
					installTime = StrToDate(strInstallDate);
					preTime =curTime - installTime ;
					nLiveDate =(int)preTime.GetTotalSeconds();

					prop = properties->Item("Status",0);
					value = prop->GetValue();
					strFileStatus = value;


					strReturn.Format("fileSize=%d$liveDate=%d$modifyDate=%s$fileStatus=%s$",
									nSize,nLiveDate,strModifiedTime,strFileStatus);
					bFlag = false;
				} 
				else
				{
					sprintf(szReturn,"error=Error ocured: File is not existing");
					bFlag = false;
					return false;
				}
			}

		}
		catch (_com_error err)
		{
			char buf[200] = {0};
			IErrorInfo * ei = err.ErrorInfo();
			BSTR strDesEI;
			ei->GetDescription(&strDesEI);
			sprintf(szReturn, "error=Error ocured:%x: %s", (unsigned)err.Error(),_com_util::ConvertBSTRToString(strDesEI));
			return false;

		}
		catch (...)
		{
			sprintf(szReturn, "error=Error ocured: %d",::GetLastError());
			OutputDebugString(szReturn);
			return false;
		}

		strcpy(szReturn,strReturn);
		CString strOutRet;
		strOutRet =szReturn;
		nSize = 1000;
		MakeCharByString(szReturn,nSize,strOutRet);	 

	}
	CoUninitialize();

	return TRUE;
}



extern "C" _declspec(dllexport) 
BOOL GetSystemStatu(const char * strParas, char * strReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CoInitialize(NULL);
	{
		//CString strReturn = "detailutilization=";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(strParas, strReturn, nSize, services))
			return FALSE;
		try
		{
			WbemScripting::ISWbemObjectSetPtr objects = services->InstancesOf("Win32_OperatingSystem", 0x10, NULL);
			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
			ULONG fetched; 
			VARIANT var; 
			//int nCpuRate = 0, nCount =0, nTotalRate = 0;
			CString strTemp;
			CString strCaption = _T("");
			CString strStatus = _T("");
			CString strTime = _T("");
			CTime curTime = CTime::GetCurrentTime();
			CTime startTime;
			CTimeSpan spanTime;
			int nRunTime = 0;

			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("Caption",0);
				_variant_t value = prop->GetValue();
				strCaption = value ;
				prop = properties->Item("CSDVersion",0);
				value = prop->GetValue();
				strCaption += " ";
				strCaption += value;
				
				prop = properties->Item("Status", 0);
				value = prop->GetValue();
				strStatus = value;
				
				prop = properties->Item("LastBootUpTime",0);
				value = prop->GetValue();
				strTime = value;
				startTime = StrToDate(strTime);	
				spanTime = curTime - startTime;
				nRunTime = (int) spanTime.GetTotalSeconds();

				sprintf(strReturn, "systemInfo=%s$systemStatus=%s$systemRun=%d$",strCaption, strStatus, nRunTime );

			}
		}
		catch (_com_error err) 
		{ 
			char buf[200] = {0};
			IErrorInfo * ei = err.ErrorInfo();
			BSTR strDesEI;
			ei->GetDescription(&strDesEI);
			sprintf(strReturn, "error=Error ocured:%x: %s", (unsigned)err.Error(),_com_util::ConvertBSTRToString(strDesEI));
			return FALSE;
		} 
		catch(...) 
		{ 
			sprintf(strReturn, "error=Error ocured: %d",::GetLastError());
			OutputDebugString(strReturn);
			return FALSE;
		}
		//strcpy(szReturn,strReturn);
		CString strOutRet;
		strOutRet =strReturn;
		nSize = 2048;
		MakeCharByString(strReturn,nSize,strOutRet);	 
	}
	CoUninitialize();
	return TRUE;
}


extern "C" _declspec(dllexport) 
BOOL EnumProcessInfo(const char * strParas, char * szReturn, int& nSize)
{
	CoInitialize(NULL);
	{
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(strParas, szReturn, nSize, services))
			return FALSE;

		try
		{
			_bstr_t com; 
			com="SELECT * FROM Win32_PerfRawData_PerfProc_Process where Name<>'_Total'"; 
			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(com,"WQL",0x10,NULL); 		
			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
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
			OutputDebugString(szReturn);
			return FALSE;
		} 
		//cout << strReturn <<endl;
		//WriteLog("EnumProcessInfo",strReturn);
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
BOOL GetProcessNum(const char * strParas, char * szReturn, int& nSize)
{
	CoInitialize(NULL);
	{
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(strParas, szReturn, nSize, services))
			return FALSE;

		try
		{
			_bstr_t com; 
			com="SELECT * FROM Win32_PerfRawData_PerfProc_Process where Name<>'_Total'"; 
			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(com,"WQL",0x10,NULL);

			int nProcessNum =0;
			nProcessNum = objects->Count;
			cout << "nProcessNum:" << nProcessNum <<endl;
			sprintf(szReturn,"ProcessNumber=%d$$", nProcessNum);		

			/*IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
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
			}*/
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
			OutputDebugString(szReturn);
			return FALSE;
		} 
		//cout << strReturn <<endl;
		//WriteLog("EnumProcessInfo",strReturn);
		//strcpy(szReturn,strReturn);
		CString strOutRet;
		strOutRet =szReturn;
		nSize = 1048576;
		MakeCharByString(szReturn,nSize,strOutRet);	 
	}
	CoUninitialize();
	return TRUE;
}




/*获取进程信息*/
extern "C" _declspec(dllexport) 
BOOL GetProcessInfo(const char * strParas, char * szReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CoInitialize(NULL);
	{
		CString strReturn = "";     

		int ProcessStatus = 0;          //进程状态
		int UserProcessCount = 0;       //主机当前不同用户的连接进程数量
		int ProcessRunningTime = 0;     //进程运行时长

		float fPhysicalMemory = 0;       //进程占物理内存大小(KB)
		float fVirtualMemory = 0;        //进程占虚拟内存大小(KB)
		float ProcessOccupyCPUTime = 0; //CPU使用率(%)	
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(strParas, szReturn, nSize, services))
		{
			return FALSE;
		}

		try
		{
			CString strProcessName="";
			strProcessName = FindStrValue(strParas, "_monitorProcessList"); //获取进程的名字

			/*第一次获取进程CPU占用率的数据*/
			CString strCom;
			strCom.Format("SELECT * FROM Win32_PerfRawData_PerfProc_Process where Name='%s'", strProcessName); 
			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(LPCTSTR(strCom),"WQL",0x10,NULL); 		
			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
			ULONG fetched; 
			VARIANT var;
			float fPercentProcessorTime1 = 0;//cpu占用率数据
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop ;
				prop = properties->Item("PercentProcessorTime",0); //进程CPU占用率
				fPercentProcessorTime1 += (float)prop->GetValue()/100000;
				cout<< fPercentProcessorTime1 << endl;
			}
			if(objects->Count == 0)
			{
				sprintf(szReturn, "error=The Process %s is not available!", strProcessName);
				return FALSE;
			}
			UserProcessCount = objects->Count;  //用户进程的个数
			Sleep(1000);

			/*第二次次获取进程CPU占用率的数据*/
			objects = services->ExecQuery(LPCTSTR(strCom),"WQL",0x10,NULL); 
			obj_enum = objects->Get_NewEnum(); 
			float fPercentProcessorTime2 = 0;//cpu占用率数据
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop ;
				prop = properties->Item("PercentProcessorTime",0); //进程CPU占用率(%)
				fPercentProcessorTime2 += (float)prop->GetValue()/100000;

				prop = properties->Item("WorkingSet",0);
				fPhysicalMemory += (float)prop->GetValue()/1024;

				prop = properties->Item("PrivateBytes",0);  //进程占虚拟内存大小(KB) 
				fVirtualMemory = (float)prop->GetValue()/1024;
			}
			cout << "fPhysicalMemory: " <<fPhysicalMemory <<endl;
			cout << "fVirtualMemory: " << fVirtualMemory<<endl;

			ProcessOccupyCPUTime = fPercentProcessorTime2 - fPercentProcessorTime1;
			if (ProcessOccupyCPUTime < 0)
			{
				ProcessOccupyCPUTime = 0;
			} 
			if (ProcessOccupyCPUTime > 100)
			{
				ProcessOccupyCPUTime = 100;
			}

			/*获取进程占物理内存大小*/
			//strCom.Format("SELECT * FROM Win32_Process Class where Name='%s'",strProcessName);
			//objects = services->ExecQuery(LPCTSTR(strCom),"WQL",0x10,NULL);
			//obj_enum = objects->Get_NewEnum();
			//while (obj_enum->Next(1,&var,&fetched) == S_OK)
			//{
			//	WbemScripting::ISWbemObjectPtr object = var;
			//	WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
			//	WbemScripting::ISWbemPropertyPtr prop ;
			//	prop = properties->Item("WorkingSetSize",0);//进程占物理内存大小(KB)
			//	PhysicalMemory = (int)prop->GetValue();
			//}

			/*获取进程运行时间*/
			strCom.Format("SELECT * FROM Win32_PerfFormattedData_PerfProc_Process where Name='%s'",strProcessName);
			objects = services->ExecQuery(LPCTSTR(strCom),"WQL",0x10,NULL);
			obj_enum = objects->Get_NewEnum();
			while (obj_enum->Next(1,&var,&fetched) == S_OK)
			{
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop ;
				prop = properties->Item("ElapsedTime",0) ;//进程运行时间(s)
				ProcessRunningTime = (int)prop->GetValue();
			}
			strReturn.Format("UserProcessCount=%d$ProcessRunningTime=%d$PhysicalMemory=%.2f$VirtualMemory=%.2f$ProcessOccupyCPUTime=%.2f$",
				UserProcessCount,ProcessRunningTime,fPhysicalMemory,fVirtualMemory,ProcessOccupyCPUTime);
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
		catch (...)
		{
			sprintf(szReturn, "error=Error ocured: %d",::GetLastError());
			OutputDebugString(szReturn);
			return FALSE;
		}
		strcpy(szReturn,strReturn);
		CString strOutRet;
		strOutRet =szReturn;
		nSize = 1024*10;
		MakeCharByString(szReturn,nSize,strOutRet);
	}
	CoUninitialize();

	return TRUE;
}



/*获取内存信息*/
extern "C" _declspec(dllexport) 
BOOL GetMemoryInfo(const char * strParas, char * szReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CoInitialize(NULL);
	{
		CString strReturn = ""; 
		float fMemoryLoad = 0;    //内存负载
		float pagesPerSec = 0;    //内存换页率
		float PagesInPerSec = 0;  //内存交换页换进率
		float PagesOutPerSec = 0; //内存交换页换出率
		WbemScripting::ISWbemServicesPtr services; //com 服务器
		if(!ConnectServer(strParas, szReturn, nSize, services)) //连接 com 服务器
		{
			return FALSE;
		}

		try
		{
			/*计算内存负载*/
			WbemScripting::ISWbemObjectSetPtr objects = services->InstancesOf("CIM_OperatingSystem", 0x10, NULL);
			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
			ULONG fetched; 
			VARIANT var; 

			float fTotalPhyMemory = 0; 
			float fFreePhyMemory = 0;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop;
				prop = properties->Item("TotalVisibleMemorySize",0); //物理内存大小(KB)
				fTotalPhyMemory = ((float)prop->GetValue()) / 1024;
				prop = properties->Item("FreePhysicalMemory",0); //可用物理内存大小(KB)
				fFreePhyMemory = ((float)prop->GetValue()) / 1024;
				fMemoryLoad = (fTotalPhyMemory - fFreePhyMemory) / fTotalPhyMemory * 100;//物理内存使用率(负载)
			}

			/*计算换页率*/
			/*第一次获取内存换页量*/
			objects = services->InstancesOf("Win32_PerfRawData_PerfOS_Memory ",0x10, NULL);
			obj_enum = objects->Get_NewEnum();
			float fPages1 = 0; 
			float fPagesIn1 = 0;
			float fPagesOut1 = 0;
			float fTimestamp_PerfTime1 = 0;
			while (obj_enum->Next(1,&var,&fetched) == S_OK)
			{
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop;
				prop = properties->Item("PagesPerSec",0);       //内存换页量
				fPages1 = (float)prop->GetValue();
				prop = properties->Item("PagesInputPerSec",0);  //内存交换页换进量
				fPagesIn1 = (float)prop->GetValue();
				prop = properties->Item("PagesOutputPerSec",0); //内存交换页换出量
				fPagesOut1 = (float)prop->GetValue();
				prop = properties->Item("Timestamp_PerfTime",0);//时间信息
				fTimestamp_PerfTime1 = (float)prop->GetValue();

			}

			Sleep(3000);

			/*第二次获取内存换页量*/
			objects = services->InstancesOf("Win32_PerfRawData_PerfOS_Memory ",0x10, NULL);
			obj_enum = objects->Get_NewEnum();
			float fPages2 = 0; 
			float fPagesIn2 = 0;
			float fPagesOut2 = 0;
			float fFrequency_PerfTime = 0;
			float fTimestamp_PerfTime2 = 0;
			while (obj_enum->Next(1,&var,&fetched) == S_OK)
			{
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop;
				prop = properties->Item("PagesPerSec",0);       //内存换页量
				fPages2 = (float)prop->GetValue();
				prop = properties->Item("PagesInputPerSec",0);  //内存交换页换进量
				fPagesIn2 = (float)prop->GetValue();
				prop = properties->Item("PagesOutputPerSec",0); //内存交换页换出量
				fPagesOut2 = (float)prop->GetValue();
				prop = properties->Item("Timestamp_PerfTime",0);//时间信息
				fTimestamp_PerfTime2 = (float)prop->GetValue();
				prop = properties->Item("Frequency_PerfTime",0);//频率
				fFrequency_PerfTime = (float)prop->GetValue();
			}
			pagesPerSec = (fPages2 - fPages1)/((fTimestamp_PerfTime2 - fTimestamp_PerfTime1)/fFrequency_PerfTime);         //内存换页率
			PagesInPerSec = (fPagesIn2 - fPagesIn1)/((fTimestamp_PerfTime2 - fTimestamp_PerfTime1)/fFrequency_PerfTime);   //内存交换页换进率
			PagesOutPerSec = (fPagesOut2 - fPagesOut1)/((fTimestamp_PerfTime2 - fTimestamp_PerfTime1)/fFrequency_PerfTime);//内存交换页换出率

			strReturn.Format("fMemoryLoad=%.2f$pagesPerSec=%.2f$PagesInPerSec=%.2f$PagesOutPerSec=%.2f$fTotalPhyMemory=%.2f$fFreePhyMemory=%.2f$",
				fMemoryLoad,pagesPerSec,PagesInPerSec,PagesOutPerSec,fTotalPhyMemory,fFreePhyMemory);
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
			OutputDebugString(szReturn);
			return FALSE;
		} 
		strcpy(szReturn,strReturn);
		CString strOutRet;
		strOutRet =szReturn;
		nSize = 1024*10;
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
			return FALSE;

		try
		{
			WbemScripting::ISWbemObjectSetPtr objects = services->InstancesOf("Win32_PerfRawData_Tcpip_NetworkInterface", 0x10, NULL);	
			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
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
			OutputDebugString(szReturn);
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

	//////////////////////////////////////////////////////////////////////////
	//lish
	//CString strFileName = "GetNetWorkInfo";
	//CString strLog ="";
	//////////////////////////////////////////////////////////////////////////
	CoInitialize(NULL);
	{
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(strParas, szReturn, nSize, services))
			return FALSE;
		try
		{
			//char *networkname=NULL;
			CString networkname = _T("");
			networkname = FindStrValue(strParas, "_NetWorkName");
			CString strCom;

			//取第一次网络设备数据
			strCom.Format("SELECT * FROM Win32_PerfRawData_Tcpip_NetworkInterface where Name='%s'", networkname);
			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(LPCTSTR(strCom),"WQL",0x10,NULL); 
			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
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
			}

			//////////////////////////////////////////////////////////////////////////
			//lish
			//strLog.Format("取第一次网络设备数据:fBytesReceivedPerSec1=%.2f,fBytesSentPerSec1=%.2f,fTimestamp_PerfTime1=%.2f",
			//	fBytesReceivedPerSec1,fBytesSentPerSec1,fTimestamp_PerfTime1);
			//WriteLog(strFileName,strLog);
			//////////////////////////////////////////////////////////////////////////

			Sleep(1000);
			//取第二次网络设备数据
			objects = services->ExecQuery(LPCTSTR(strCom),"WQL",0x10,NULL); 
			obj_enum = objects->Get_NewEnum(); 
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

			}

			//////////////////////////////////////////////////////////////////////////
			//lish
			//strLog.Format("取第二次网络设备数据:fBytesReceivedPerSec2=%.2f,fBytesSentPerSec2=%.2f,fTimestamp_PerfTime2=%.2f，fFrequency_PerfTime=%.2f",
			//	fBytesReceivedPerSec2,fBytesSentPerSec2,fTimestamp_PerfTime2,fFrequency_PerfTime);
			//WriteLog(strFileName,strLog);
			//////////////////////////////////////////////////////////////////////////

			float fInterval = (fTimestamp_PerfTime2-fTimestamp_PerfTime1)/fFrequency_PerfTime;
			strReturn.Format("BytesReceivedPerSec=%.2f$BytesSentPerSec=%.2f$PacketsErrors=%d", 
				(fBytesReceivedPerSec2-fBytesReceivedPerSec1)/fInterval, (fBytesSentPerSec2-fBytesSentPerSec1)/fInterval,
				nPacketsOutboundErrors+nPacketsReceivedErrors);
			//////////////////////////////////////////////////////////////////////////
			//lish
			//strLog.Format("函数返回值：%s",strReturn);
			//WriteLog(strFileName,strLog);
			//////////////////////////////////////////////////////////////////////////

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
			OutputDebugString(szReturn);
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
