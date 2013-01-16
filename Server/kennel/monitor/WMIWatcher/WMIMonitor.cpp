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

#include <fstream>
using namespace std;
//typedef struct std::list<char*> StringList;
typedef class std::list<char*> StringList;


//调试 lish
bool MakeStringByChar(CString& strOut , const char * pInput )
{
	if (pInput == NULL)
	{
		return false;
	}
	const char * p;
	int nSize;
	p=pInput;
	while(*p!='\0')
	{
		nSize =(int)strlen(p);
		if( nSize>0 )
		{
			strOut += p;
			strOut += "; ";
		}
		p=p+nSize+1;
	}

	return true;
}// end of MakeStringByChar

//调试 lish
bool WriteLog(CString strFileName,const CString strLog)
{
	char szDataBuf[128]		= _T("");
	char szTimeBuf[128]		= _T("");
	char szTempBuf[1024*10] = _T("");
	_strdate(szDataBuf);
	_strtime(szTimeBuf);
	sprintf(szTempBuf,"%s-%s",szDataBuf,szTimeBuf);

	CString szFileName="";
	szFileName.Format("%s.log",strFileName);

	ofstream stFileStream;
	stFileStream.open(szFileName, ios::app);
	stFileStream<<szTempBuf<<"\t"<<strLog<<endl;
	stFileStream.close();
	return true;
}
//dy 获取ecc路径，不调用svapi
string getECCPath()
{
	char    CurrPath[128] = {0};

	GetCurrentDirectory(sizeof(CurrPath)-1, CurrPath);

	std::string::size_type spos;
	string input = CurrPath;
	string pathReturn = "";
	spos=input.find("SiteView ECC");
	if (spos!=-1)
	{
		pathReturn = input.substr(0,spos+12); //12:"SiteView ECC"的长度
	}
	return pathReturn;
}

int PrintLog(const char * strReceive)
{
//#ifndef __DEBUG
//	return 0;
//#endif
	char datebuf[128]=_T("");
	char timebuf[128]=_T("");
	char tempbuf[1024*10]=_T("");

	_strdate(datebuf);
	_strtime(timebuf);
	sprintf(tempbuf,"%s-%s",datebuf,timebuf);
	ofstream filestream;
	filestream.open("WMIMonitor.log",ios::app);
	filestream<<tempbuf<<"\t"<<strReceive<<endl;
	filestream.close();
	filestream.close();
	return 0;
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


//此函数存在内存泄露问题
//char *FindStrValue(const char *strParas, CString str)
//{
//	char *pValue = NULL;
//	string m_TempStr;
//
//	std::list<char*> strList;
//	MakeStringListByChar(strList, strParas);
//	std::list<char *>::iterator pos = strList.begin();
//
//	 while(pos != strList.end())
//	{
//		//CString strTemp = strList.GetNext(pos);
//		char * strTemp = *pos;
//		std::string strTemp1 = *pos;
//		int m_Fpos = 0;
//		
//		if((m_Fpos = static_cast<int>(strTemp1.find(str, 0))) >= 0)
//		{
//			m_TempStr = strTemp1.substr( m_Fpos + strlen(str)+1, strTemp1.size() - strlen(str) - 1); 
//			pValue=(char*)malloc(m_TempStr.size()+1);
//			strcpy(pValue, m_TempStr.c_str());
//			
//		}
//		pos++;
//	}
//
//	return pValue;
//	
//}


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

//段毅 dos命令返回的结果输入到fileName里面，再查看里面是否有和matchStr匹配的字段，有就是成功
bool isCmdSuccess(const char *fileName, char *matchStr)
{
	printf("isCmdSuccess fileName = %s\n", fileName);
	FILE *fp = NULL;
	char errorMes[100] = {0};

	fp = fopen(fileName, "r");
	if (fp==NULL)
	{
		printf("file isn't exist\n");
		return false;
	}
	fread(errorMes, 1, 100, fp);
	fclose(fp);

	printf("errorMes = %s\n", errorMes);

	char *p = NULL;
	p = strstr(errorMes, matchStr);

	if (p==NULL)
	{
		printf("cmd error\n");
		DeleteFile(fileName);
		return false;
	}
	else
	{
		printf("cmd ok\n");
	}

	if (DeleteFile(fileName))
	{
		cout << "DeleteFile " << fileName << " OK" << endl;
	}
	else
	{
		cout << "DeleteFile " << fileName << " faild" << endl;
		return false;
	}

	return true;
}

int executeExeFile(CString strCmdLine, CString &strContent)
{
	HANDLE hOutputRead, hOutputWrite;
	SECURITY_ATTRIBUTES sa;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	PROCESS_INFORMATION pi;  //dy
	DWORD dwResult;			 //dy

	try
	{
		if (CreatePipe(&hOutputRead, &hOutputWrite, &sa, 1024*50)) 
		{

			STARTUPINFO si;
			memset(&si, 0, sizeof(STARTUPINFO));
			si.cb = sizeof(STARTUPINFO);
			si.dwFlags = STARTF_USESTDHANDLES;
			si.hStdOutput = hOutputWrite;
			si.hStdError = hOutputWrite;

			memset(&pi, 0, sizeof(PROCESS_INFORMATION));

			if (CreateProcess(NULL, strCmdLine.GetBuffer(strCmdLine.GetLength()),  NULL, NULL, TRUE, /*CREATE_NEW_CONSOLE*/CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
			{

				if(WaitForSingleObject(pi.hProcess, 180000) == WAIT_OBJECT_0 )
				{
					//关闭写句柄
					if (!CloseHandle(hOutputWrite)) 
					{
						OutputDebugString("关闭写句柄失败！");
					}


					char buffer[1024*10] = {0};
					DWORD bytesToRead = sizeof(buffer)-1;//保留一位放字符串结束符
					DWORD bytesRead = 0;
					int index = 0;

					if(GetFileSize(hOutputRead,NULL) > 0)//判断管道中是否有数据，必须有数据，方可执行ReadFile
					{
						while(ReadFile(hOutputRead, buffer, bytesToRead, &bytesRead, NULL))//用循环确保管道中的数据被全部读完
						{

							buffer[bytesRead] = 0;

							strContent += buffer;

							if(bytesToRead > bytesRead)//当读完管道中所有数据时，结束读数
							{
								break;
							}
						}
					}
				}
				else
				{
					strContent="Timeout";
					//即使时间超时，也不结束子进程cmd.exe，而是让其自己结束！
				}

				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
			}

			CloseHandle(hOutputRead);
			GetExitCodeThread(pi.hProcess, &dwResult);

		}
	}
	catch(...)
	{
		CloseHandle(hOutputRead);
		CloseHandle(hOutputWrite);
	}
	strCmdLine.ReleaseBuffer();//dy2010-11-02 GetBuffer 最好有ReleaseBuffer，免得内存泄露
	return 1;
}

//CString FindStrValue( const list< string > paramList, CString str)
//{
//	//char *pValue = NULL;
//	CString strValue="";
//	string m_TempStr;
//
//	list< string > strList = paramList;
//	list< string >::iterator pos = strList.begin();
//	while( pos != strList.end())
//	{
//		string strTemp = * pos;
//		int nPos = 0;
//		if ( ( nPos = static_cast<int>(strTemp.find(str,0))) >=0 )
//		{
//			m_TempStr = strTemp.substr( nPos + strlen(str) + 1 , strTemp.size() - strlen(str) - 1 );
//			/*pValue = (char*)malloc( m_TempStr.size() + 1 );
//			strcpy( pValue, m_TempStr.c_str() );*/
//			strValue = m_TempStr.c_str();
//		}
//		pos++;
//	}
//
//	//return pValue;
//	return strValue;
//
//}


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

	PrintLog(strMachine);
	PrintLog(strUser);
	PrintLog(strPSWD);

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
				printf("WMI:是本机\n\n");

				services = locator->ConnectServer(".","root\\cimv2","","","","",0,NULL);
			}
			else
			{
				printf("WMI:不是本机\n\n");
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

BOOL ConnectADServer(const char * strParas, char * szReturn, int& nSize,WbemScripting::ISWbemServicesPtr &services)
{

	char *machinename=NULL, *user=NULL, *pswd=NULL;

	/*machinename = FindStrValue(strParas, "_MachineName");
	user = FindStrValue(strParas, "_UserAccount");
	pswd = FindStrValue(strParas, "_PassWord");*/

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
				services = locator->ConnectServer(".","root\\MicrosoftActiveDirectory","","","","",0,NULL);
			}
			else
			{
				//services = locator->ConnectServer(machinename,"root\\MicrosoftActiveDirectory",user,pswd,"","",0,NULL);
				services = locator->ConnectServer(strMachine.GetBuffer(),"root\\MicrosoftActiveDirectory",
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
BOOL GetCPURate(const char * strParas, char * szReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	char strTime[128]={0};
	_strtime(strTime);
	cout<<"Begin Time = "<<strTime<<endl;
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
		if(!ConnectServer(strParas, szReturn, nSize, services))
		{
			sprintf(szReturn, "error=%s","连接目标机器失败！");
			CoUninitialize();
			return FALSE;
		}
		try
		{
			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(LPCTSTR(strWQL),"WQL",0x10,NULL);
			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
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
			}
			if(nRoundNum==0)
			{
				sprintf(szReturn,"error=%s","WMI库缺少Win32_PerfRawData_PerfProc_Process类");
				return false;
			}
			Sleep(1000);
/************************第二次取进程CPU占用率数据************************/
			objects = services->ExecQuery(LPCTSTR(strWQL),"WQL",0x10,NULL); 		
			obj_enum = objects->Get_NewEnum(); 
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
						cout<<"新增进程："<<strProName<<"CPU占用时间为(单位：秒/100000)："<<fPercentProcessorTime2<<endl;
					}
				}				
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
				cout<<ptrTestMap->second<<"的CPU利用率是："<<(100*(ptrTestMap->first)/fTotalTime)<<endl;
				ptrTestMap++;
			}
			strResult.Delete(strResult.GetLength()-1,1);		//去除末尾多余的“,”
			strResult = "PerProUse="+strResult;
			cout<<strResult.GetBuffer(1)<<endl;
			strReturn.Format("utilization=%0.2f$",iTotalRate*100/fTotalTime);
			strReturn+=strResult;
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
		CString strInput;
		strInput =strReturn;
		MakeCharByString(szReturn,nSize,strInput);	 
	}
	CoUninitialize();
	_strtime(strTime);
	cout<<"End Time="<<strTime<<endl;
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
			
				//PrintLog(strTemp.GetBuffer(strTemp.GetLength()));
				strReturn += strTemp;
				nTotalRate += nCpuRate;
				nCount++;
			}
			//duanyi 2010.05.25湖北电力
///////////////////////增加显示CPU个数////////////////////////////////////////
		/*	CString cpuNum = "";
			cpuNum.Format("CPU个数:%d,", nCount);
			strReturn.Insert(sizeof("detailutilization=")-1, cpuNum);*/
//////////////////////////////////////////////////////////////////////////
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
			sprintf(szReturn, "error=Error ocured...: %d",::GetLastError());
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
				strReturn.Format("percentFull=%.2f$Mbfree=%.2f$TotalSize=%.2f$",
					((nSize - nFreeSize) / nSize) * 100 , nFreeSize/1048576, nSize/1048576);
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
BOOL GetMemoryInfo(const char * strParas, char * szReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CoInitialize(NULL);
	{
		char strTMP[1024]=_T("");
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(strParas, szReturn, nSize, services))
			return FALSE;

		CString strFile = _T("");
		CString strLog = _T("");
		strFile.Format("GetMemoryInfo");
		WriteLog(strFile,"------------- GetMemoryInfo ----------------------");
		try
		{
			WbemScripting::ISWbemObjectSetPtr objects = services->InstancesOf("CIM_OperatingSystem", 0x10, NULL);
			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
			ULONG fetched; 
			VARIANT var; 
			double fTotalPhyMem=0, fFreePhyMem=0, fTotalVirMem =0, fFreeVirMem=0,fVirPercent=0,fPhypercent=0,fTemp;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("TotalVisibleMemorySize",0);
				fTotalPhyMem = ((double)prop->GetValue())/1024;
				prop = properties->Item("TotalVirtualMemorySize",0);
				fTotalVirMem = ((double)prop->GetValue())/1024;
				prop = properties->Item("FreePhysicalMemory",0);
				fFreePhyMem = ((double)prop->GetValue())/1024;
				prop = properties->Item("FreeVirtualMemory",0);
				fFreeVirMem = ((double)prop->GetValue())/1024;
				strLog.Format("\nfTotalPhyMem=%.2f\nfTotalVirMem=%.2f\nfFreePhyMem=%.2f\nfFreeVirMem=%.2f\n",fTotalPhyMem,fTotalVirMem,fFreePhyMem,fFreeVirMem);
				WriteLog(strFile,strLog);
				if (fFreePhyMem > fTotalPhyMem)
				{
					fTemp			= fTotalPhyMem;
					fTotalPhyMem	= fFreePhyMem;
					fFreePhyMem		= fTemp;
				}
				if (fFreeVirMem > fTotalVirMem)
				{
					fTemp			= fTotalVirMem;
					fTotalVirMem	= fFreeVirMem;
					fFreeVirMem		= fTemp;
				}
				fVirPercent = (fTotalVirMem - fFreeVirMem)/fTotalVirMem*100;
				fPhypercent = (fTotalPhyMem - fFreePhyMem)/fTotalPhyMem*100;
                
				cout << "fTotalPhyMem="<<fTotalPhyMem<<endl;
				cout << "fTotalVirMem="<<fTotalVirMem<<endl;
				cout << "fFreePhyMem="<<fFreePhyMem<<endl;
				cout << "fFreeVirMem="<<fFreeVirMem<<endl;
				cout << "fVirPercent ="<<fVirPercent<<endl;
				cout << "fPhypercent ="<<fPhypercent<<endl;
				
				strReturn.Format("percentUsed=%.2f$Mbfree=%.2f$TotalMemory=%.2f$PhyMemUsage=%.2f$FreePhyMem=%.2f$TotalPhyMem=%.2f$", 
					fVirPercent, fFreeVirMem,fTotalVirMem,fPhypercent,fFreePhyMem,fTotalPhyMem);
				WriteLog(strFile,strReturn);
				
			}
			//取第一次错误页面处理数据
			objects = services->InstancesOf("Win32_PerfRawData_PerfOS_Memory", 0x10, NULL);
			obj_enum = objects->Get_NewEnum(); 
			double fPagesPersec1=0, fTimestamp_PerfTime1=0, fPagesPersec2=0, fTimestamp_PerfTime2=0, fFrequency_PerfTime=0;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("PagesPersec",0);
				fPagesPersec1 = (double)prop->GetValue();

				prop = properties->Item("Timestamp_PerfTime",0);
				fTimestamp_PerfTime1 = (double)prop->GetValue();
			}

			//取第二次错误页面处理数据
			objects = services->InstancesOf("Win32_PerfRawData_PerfOS_Memory", 0x10, NULL);
			obj_enum = objects->Get_NewEnum(); 
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("PagesPersec",0);
				fPagesPersec2 = (double)prop->GetValue();

				prop = properties->Item("Timestamp_PerfTime",0);
				fTimestamp_PerfTime2 = (double)prop->GetValue();

				prop = properties->Item("Frequency_PerfTime",0);
				fFrequency_PerfTime = (double)prop->GetValue();
			}
			double fPagesPersec = (fPagesPersec2-fPagesPersec1)/((fTimestamp_PerfTime2-fTimestamp_PerfTime1)/fFrequency_PerfTime);
			CString strTemp;
			strTemp.Format("pagesPerSec=%.2f$", fPagesPersec);
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
		nSize = 1024*10;
		MakeCharByString(szReturn,nSize,strOutRet);	 
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
			return FALSE;
		try
		{
			WbemScripting::ISWbemObjectSetPtr objects = services->InstancesOf("Win32_Service", 0x10, NULL);
			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
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
		nSize = 1048576;
		MakeCharByString(szReturn,nSize,strOutRet);	 
	}
	CoUninitialize();

	return TRUE;
}

extern "C" _declspec(dllexport) 
BOOL GetServiceInfo(const char * strParas, char * szReturn, int& nSize)
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
			//char *servicename=NULL;
			CString servicename = _T("");
			servicename = FindStrValue(strParas, "_Service");
			CString strCom;
			strCom.Format("SELECT * FROM Win32_Service where DisplayName='%s'", servicename);
			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(LPCTSTR(strCom),"WQL",0x10,NULL); 
			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
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
				if(nProecssID == 0)
					strReturn = "Processes=0$Started=False$ProcessName=NA$";
				else
				{
					strCom.Format("SELECT * FROM Win32_PerfRawData_PerfProc_Process where IDProcess=%d", nProecssID);
					objects = services->ExecQuery(LPCTSTR(strCom),"WQL",0x10,NULL); 
					obj_enum = objects->Get_NewEnum(); 
					while (obj_enum->Next(1,&var,&fetched) == S_OK) 
					{ 
						WbemScripting::ISWbemObjectPtr object = var;
						WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
						WbemScripting::ISWbemPropertyPtr prop = properties->Item("Name",0);
						strTemp.Format("ProcessName=%s$", (const char*)_bstr_t(prop->GetValue()));
						strReturn = "Processes=1$Started=True$";
						strReturn += strTemp;
					}
				}
				prop = properties->Item("State",0);
				strTemp.Format("State=%s$", (const char*)_bstr_t(prop->GetValue()));
				strReturn += strTemp;
				prop = properties->Item("Status",0);
				strTemp.Format("Status=%s$", (const char*)_bstr_t(prop->GetValue()));
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

extern "C" _declspec(dllexport) 
BOOL FilterEventLogInfo(const char * strParas, char * szReturn, int& nSize)
{
	//////////////////////////////////////////////////////////////////////////
	//lish
	CString strLog = "";
	CString strFilename = "FilterEventLogInfo";
	MakeStringByChar(strLog, strParas );
    WriteLog(strFilename,strLog);
	//////////////////////////////////////////////////////////////////////////
	CoInitialize(NULL);
	{
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(strParas, szReturn, nSize, services))
		{
			sprintf(szReturn, "error=%s","连接目标机器失败！");
			CoUninitialize();
			return FALSE;
		}
		try
		{
			CString strLogName="", strType="", strCodeList="", strSourceList="", strEventMachine="", strOnce="", strMonitorID="";
			strLogName = FindStrValue(strParas, "_logName");
			strLogName.TrimRight();
			strType = FindStrValue(strParas, "_eventType");
			strType.TrimRight();
			strCodeList = FindStrValue(strParas, "_codeList");
			strCodeList.TrimRight();
			strSourceList = FindStrValue(strParas, "_sourceList");
			strSourceList.TrimRight();
			strEventMachine = FindStrValue(strParas, "_eventMachine");
			strEventMachine.TrimRight();
			strMonitorID  = FindStrValue(strParas, "_MonitorID");
			strMonitorID.TrimRight();
			strOnce  = FindStrValue(strParas, "_monitorcondition");
			strOnce.TrimRight();
			
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
			printf("第一次取值 strCom=%s\n",strCom);
			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(LPCTSTR(strCom),"WQL",0x10,NULL); 
			int nTotalEventCount = objects->Count;
			printf("nTotalEventCount=%d\n",nTotalEventCount);
			

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
			
			printf("第二次取值(过滤) strCom=%s\n",strCom);
			objects = services->ExecQuery(LPCTSTR(strCom),"WQL",0x10,NULL); 
			int nFilterEventCount = objects->Count;
			//printf("nFilterEventCount=%d\n",nFilterEventCount);

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
BOOL FilterEventLogInfoAdv(const char * strParas, char * szReturn, int& nSize)
{
	//////////////////////////////////////////////////////////////////////////
	//lish
	CString strLog = "";
	CString strFilename = "FilterEventLogInfo";
	MakeStringByChar(strLog, strParas );
	WriteLog(strFilename,strLog);
	//////////////////////////////////////////////////////////////////////////
	CoInitialize(NULL);
	{
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(strParas, szReturn, nSize, services))
		{
			sprintf(szReturn, "error=%s","连接目标机器失败！");
			CoUninitialize();
			return FALSE;
		}
		try
		{
			CString strLogName="", strType="", strCodeList="", strSourceList="", strEventMachine="", strOnce="", strMonitorID="", strIsAlertConfirm="";
			strLogName = FindStrValue(strParas, "_logName");
			strLogName.TrimRight();
			strType = FindStrValue(strParas, "_eventType");
			strType.TrimRight();
			strCodeList = FindStrValue(strParas, "_codeList");
			strCodeList.TrimRight();
			strSourceList = FindStrValue(strParas, "_sourceList");
			strSourceList.TrimRight();
			strEventMachine = FindStrValue(strParas, "_eventMachine");
			strEventMachine.TrimRight();
			strMonitorID  = FindStrValue(strParas, "_MonitorID");
			strMonitorID.TrimRight();
			strOnce  = FindStrValue(strParas, "_monitorcondition");
			strOnce.TrimRight();
			strIsAlertConfirm  = FindStrValue(strParas, "_IsAlertConfirm");
			strIsAlertConfirm.TrimRight();
			
			CString strCom, strTemp;
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
			CString strTime,strTime2;

			strTime.Format("and TimeWritten>='%s.000000+480'", curTime.Format("%Y%m%d000000"));
			strTime2.Format("and TimeWritten>='%s.000000+480'", curTime.Format("%Y%m%d%H%M%S"));

			strCom += strTime;
			printf("第一次取值 strCom=%s\n",strCom);
			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(LPCTSTR(strCom),"WQL",0x10,NULL); 
			int nTotalEventCount = objects->Count;
			printf("nTotalEventCount=%d\n",nTotalEventCount);

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


//  [4/21/2011 Administrator]
			char  iniPath[256]  = {0}; 
			char  CurrPath[128] = {0};
			char  tempValue[128] = {0};

			GetCurrentDirectory(sizeof(CurrPath)-1, CurrPath);
			std::string::size_type spos;
			string input = CurrPath;
			spos=input.find("SiteView ECC");
			string fPath = input.substr(0,spos+12); //12:"SiteView ECC"的长度

			sprintf(iniPath, "%s\\data\\tmpinifile\\EventLogInfo.ini", fPath.c_str());
			printf("iniPath = %s\n", iniPath);

			char *charMonitorID = strMonitorID.GetBuffer(strMonitorID.GetLength());

			//判断是否确认告警
			if(strIsAlertConfirm.Compare("True")==0)
			{
				UINT IsAlertConfirm = GetPrivateProfileInt(charMonitorID,"IsAlertConfirm",0,iniPath);
				//如果已经确认告警，就只读时间
				printf("IsAlertConfirm = %d\n", IsAlertConfirm);

				if (IsAlertConfirm==1)
				{
					GetPrivateProfileString(charMonitorID, "CurrentTime", "", tempValue, sizeof(tempValue)-1,iniPath);
					strCom.Replace(strTime, tempValue);
				}
				else if(IsAlertConfirm==0)
				{
					WritePrivateProfileString(charMonitorID, "IsAlertConfirm", "1", iniPath);
					WritePrivateProfileString(charMonitorID, "CurrentTime", strTime2, iniPath);
					strCom.Replace(strTime, strTime2);
				}
				else
				{
					//error
					sprintf(szReturn, "error=Read IsAlertConfirmIni Fail!");
					return false;
				}
			}
			else
			{
				WritePrivateProfileString(charMonitorID,"IsAlertConfirm", "0", iniPath);
			}
//  [4/21/2011 Administrator]	

			printf("第二次取值(过滤) strCom=%s\n",strCom);
			objects = services->ExecQuery(LPCTSTR(strCom),"WQL",0x10,NULL); 
			int nFilterEventCount = objects->Count;
			//printf("nFilterEventCount=%d\n",nFilterEventCount);

			int nConirms = GetConfirms(strMonitorID);
			if(nFilterEventCount < nConirms)
				nFilterEventCount = 0;
			else
				nFilterEventCount -= nConirms;

			strReturn.Format("checkcount=%d$filtercount=%d$", nTotalEventCount, nFilterEventCount);
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

CString ParseProcessID(CString strInput)
{
	int nFirst = strInput.Find('(');
	if (nFirst == -1)
		return "";

	int nLast = strInput.Find(')');
	if (nLast == -1)
		return "";

	return strInput.Mid(nFirst+1, nLast-nFirst-1);
}

extern "C" _declspec(dllexport) 
BOOL GetProcessInfo(const char * strParas, char * szReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CoInitialize(NULL);
	{
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(strParas, szReturn, nSize, services))
		{
			return FALSE;
		}

		try
		{
			CString strProcessName="";
			strProcessName = FindStrValue(strParas, "_monitorProcessList");

			//取第一次进程CPU占用率数据
			CString strCom;
			strCom.Format("SELECT * FROM Win32_PerfRawData_PerfProc_Process where Name='%s'", strProcessName); 
			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(LPCTSTR(strCom),"WQL",0x10,NULL); 		
			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
			ULONG fetched; 
			VARIANT var; 
			CString strTemp;
			int nThreadCount = 0;
			int nProcess = 0;  //dy2010-09-07 进程数
			float fPercentProcessorTime=0, fWorkingsetsize=0, fPrivateBytes=0, fPercentProcessorTime1=0, fPercentProcessorTime2=0;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("PercentProcessorTime",0);
				fPercentProcessorTime1 += (float)prop->GetValue()/100000;
				nProcess++; //dy2010-09-07 进程数
				//cout << "strProcessName :"<< nProcess<< endl;
			}
			if(objects->Count == 0)
			{
				sprintf(szReturn, "error=The Process %s is not available!", strProcessName);
				return FALSE;
			}

			Sleep(1000);
			//取第二次进程CPU占用率数据
			objects = services->ExecQuery(LPCTSTR(strCom),"WQL",0x10,NULL); 		
			obj_enum = objects->Get_NewEnum(); 
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("ThreadCount",0);
				nThreadCount += (int)prop->GetValue(); 
				prop = properties->Item("PercentProcessorTime",0);
				fPercentProcessorTime2 += (float)prop->GetValue()/100000;
				prop = properties->Item("WorkingSet",0);
				fWorkingsetsize += (float)prop->GetValue()/1024;
				prop = properties->Item("PrivateBytes",0);
				fPrivateBytes += (float)prop->GetValue()/1024;
			}

			fPercentProcessorTime = fPercentProcessorTime2 - fPercentProcessorTime1;
			if(fPercentProcessorTime < 0)
				fPercentProcessorTime = 0;
			if(fPercentProcessorTime > 100)
				fPercentProcessorTime = 100;
			
			float fTotalPhyMem=0, fTotalVirMem=0;
			objects = services->InstancesOf("CIM_OperatingSystem", 0x10, NULL);
			obj_enum = objects->Get_NewEnum(); 
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("TotalVisibleMemorySize",0);
				fTotalPhyMem = (float)prop->GetValue();
				prop = properties->Item("TotalVirtualMemorySize",0);
				fTotalVirMem = (float)prop->GetValue();
			}
			//strReturn.Format("ProcessCount=%d$ThreadCount=%d$PercentProcessorTime=%.2f$WorkingSet=%.2f$\
			//				MemUtilization=%.2f$PrivateBytes=%.2f$VirUtilization=%.2f$",
			//	objects->Count, nThreadCount, fPercentProcessorTime, fWorkingsetsize, fWorkingsetsize/fTotalPhyMem*100,
			//	fPrivateBytes, fPrivateBytes/fTotalVirMem*100);
			strReturn.Format("ProcessCount=%d$ThreadCount=%d$PercentProcessorTime=%.2f$WorkingSet=%.2f$\
							MemUtilization=%.2f$PrivateBytes=%.2f$VirUtilization=%.2f$",
				nProcess, nThreadCount, fPercentProcessorTime, fWorkingsetsize, fWorkingsetsize/fTotalPhyMem*100,
				fPrivateBytes, fPrivateBytes/fTotalVirMem*100);  //dy2010-09-07 进程数
			strReturn.Replace("\t", NULL);
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
		nSize = 1048576;
		MakeCharByString(szReturn,nSize,strOutRet);	 
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
			return FALSE;
		try
		{
			//取第一次IIS服务器数据
			_bstr_t com; 
			com="SELECT * FROM Win32_PerfRawData_W3SVC_WebService where Name='_Total'"; 
			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(com,"WQL",0x10,NULL); 
			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
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
			}

			printf("第一次取值:\n");
			printf("PostRequestsPersec=%f\nGetRequestsPersec=%f\nBytesSentPerSec=%f\nBytesReceivedPerSec=%f\nBytesTotalPersec=%f\nTimestamp_PerfTime=%f\n",
				fPostRequestsPersec1,fGetRequestsPersec1,
				fBytesSentPerSec1,fBytesReceivedPerSec1,
				fBytesTotalPersec1,fTimestamp_PerfTime1);
			
			Sleep(1000);
			//取第二次IIS服务器数据
			com="SELECT * FROM Win32_PerfRawData_W3SVC_WebService where Name='_Total'"; 
			objects = services->ExecQuery(com,"WQL",0x10,NULL); 
			obj_enum = objects->Get_NewEnum(); 
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
			}
			float fInterval = (fTimestamp_PerfTime2-fTimestamp_PerfTime1)/fFrequency_PerfTime;
			strReturn.Format("maxconnections=%d$currentnonanonymoususers=%d$currentconnections=%d$totalnotfounderrors=%d$\
				postrequestspersec=%.2f$getrequestspersec=%.2f$bytessentpersec=%.2f$bytesreceivedpersec=%.2f$\
				bytestotalpersec=%.2f$", nMaximumConnections, nCurrentAnonymousUsers, nCurrentConnections, nTotalNotFoundErrors,
				(fPostRequestsPersec2-fPostRequestsPersec1)/fInterval, (fGetRequestsPersec2-fGetRequestsPersec1)/fInterval,
				(fBytesSentPerSec2-fBytesSentPerSec1)/fInterval, (fBytesReceivedPerSec2-fBytesReceivedPerSec1)/fInterval,
				(fBytesTotalPersec2-fBytesTotalPersec1)/fInterval);
			strReturn.Replace("\t", NULL);

			printf("第二次取值:\n");
			printf("PostRequestsPersec=%f\nGetRequestsPersec=%f\nBytesSentPerSec=%f\nBytesReceivedPerSec=%f\nBytesTotalPersec=%f\nTimestamp_PerfTime=%f\n",
				fPostRequestsPersec2,fGetRequestsPersec2,
				fBytesSentPerSec2,fBytesReceivedPerSec2,
				fBytesTotalPersec2,fTimestamp_PerfTime2);

			printf("MaximumConnections=%d\nCurrentAnonymousUsers=%d\nCurrentConnections=%dTotalNotFoundErrors=%d\nTimestamp_PerfTime2=%f\nFrequency_PerfTime=%f\n",
			nMaximumConnections,nCurrentAnonymousUsers,nCurrentConnections,nTotalNotFoundErrors,fTimestamp_PerfTime2,fFrequency_PerfTime);
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
BOOL GetASPInfo(const char * strParas, char * szReturn, int& nSize)
{
		CoInitialize(NULL);
	{
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(strParas, szReturn, nSize, services))
			return FALSE;
		try
		{
			//取第一次ASP数据
			WbemScripting::ISWbemObjectSetPtr objects = services->InstancesOf("Win32_PerfRawData_ASP_ActiveServerPages", 0x10, NULL);
			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
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
			}

			//取第二次ASP数据
			objects = services->InstancesOf("Win32_PerfRawData_ASP_ActiveServerPages", 0x10, NULL);
			obj_enum = objects->Get_NewEnum(); 
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
			}
			float fInterval = (fTimestamp_PerfTime2-fTimestamp_PerfTime1)/fFrequency_PerfTime;
			strReturn.Format("ErrorsDuringScriptRuntime=%d$ErrorsFromASPPreprocessor=%d$ErrorsFromScriptCompilers=%d$\
				ErrorsPerSec=%.2f$RequestBytesInTotal=%.2f$RequestsFailedTotal=%d$RequestsTotal=%d$\
				SessionsCurrent=%d$RequestsSucceeded=%d$TransactionsTotal=%d$", 
				nErrorsDuringScriptRuntime, nErrorsFromASPPreprocessor, nErrorsFromScriptCompilers,
				(fErrorsPerSec2-fErrorsPerSec1)/fInterval, (fRequestBytesInTotal2-fRequestBytesInTotal1)/fInterval,
				nRequestsFailedTotal, nRequestsTotal,nSessionsCurrent, nRequestsSucceeded,nTransactionsTotal);
			strReturn.Replace("\t", NULL);
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
BOOL GetAspNetApplicationInfo(const char * strParas, char * szReturn, int& nSize)
{
		CoInitialize(NULL);
	{
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(strParas, szReturn, nSize, services))
			return FALSE;
		try
		{
			//取第一次AspNet数据
			_bstr_t com; 
			com="SELECT * FROM Win32_PerfRawData_ASPNET_ASPNETApplications where Name='__Total__'"; 
			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(com,"WQL",0x10,NULL); 	
			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
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
			}

			//取第二次AspNet数据
			com="SELECT * FROM Win32_PerfRawData_ASPNET_ASPNETApplications where Name='__Total__'"; 
			objects = services->ExecQuery(com,"WQL",0x10,NULL); 
			obj_enum = objects->Get_NewEnum(); 
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
BOOL GetAspNetInfo(const char * strParas, char * szReturn, int& nSize)
{
		CoInitialize(NULL);
	{
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(strParas, szReturn, nSize, services))
			return FALSE;
		try
		{
			WbemScripting::ISWbemObjectSetPtr objects = services->InstancesOf("Win32_PerfRawData_ASPNET_ASPNET", 0x10, NULL);
			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
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
			OutputDebugString(szReturn);
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

extern "C" _declspec(dllexport) 
BOOL GetADInfo(const char * strParas, char * szReturn, int& nSize)
{	
	CoInitialize(NULL);
	{
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectADServer(strParas, szReturn, nSize, services))
			return FALSE;
		try
		{
			WbemScripting::ISWbemObjectSetPtr objects = services->InstancesOf("MSAD_DomainController", 0x10, NULL);
			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
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
		nSize = 4096;
		MakeCharByString(szReturn,nSize,strOutRet);	 
	}
	CoUninitialize();

	return TRUE;
}


extern "C" _declspec(dllexport) 
BOOL GetEventLogInfo(const char * strParas, char * szReturn, int& nSize)
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
			/*CString strLogName="", strType="", strCodeList="", strSourceList="", strEventMachine="", strOnce="", strMonitorID="";
			strLogName = FindStrValue(strParas, "_logName");
			strLogName.TrimRight();
			strType = FindStrValue(strParas, "_eventType");
			strType.TrimRight();
			strCodeList = FindStrValue(strParas, "_codeList");
			strCodeList.TrimRight();
			strSourceList = FindStrValue(strParas, "_sourceList");
			strSourceList.TrimRight();
			strEventMachine = FindStrValue(strParas, "_eventMachine");
			strEventMachine.TrimRight();
			strMonitorID  = FindStrValue(strParas, "_MonitorID");
			strMonitorID.TrimRight();
			strOnce  = FindStrValue(strParas, "_monitorcondition");
			strOnce.TrimRight();*/

			CString strCom, strTemp;
			//strCom.Format("SELECT * FROM Win32_NTLogEvent where Logfile='%s'", strLogName);
			strCom.Format("SELECT * FROM Win32_NTLogEvent where (Type='error' or Type='错误')");
			/*switch(atoi(strType))
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
			}*/

			CTime curTime = CTime::GetCurrentTime();
			CTimeSpan preTime =  CTimeSpan(1,0,0,0);
			//curTime -= preTime ;
			//curTime -= 60*60*24; //one day ago
			strTemp.Format("and TimeWritten>='%s.000000+480'", curTime.Format("%Y%m%d000000"));
			strCom += strTemp;

			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(LPCTSTR(strCom),"WQL",0x10,NULL); 
			int nTotalErrorCount = objects->Count;

			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
			ULONG fetched; 
			VARIANT var; 
			//			CString strTemp;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("SourceName",0);

				//strTemp.Format("%s=%s$", (const char*)_bstr_t(prop->GetValue()), (const char*)_bstr_t(prop->GetValue()));
				strTemp = prop->GetValue();
				if (strReturn.Find(strTemp)== -1)
				{
					strReturn += strTemp;
					strReturn +=",";
				}

			}
			
			strReturn.TrimRight(",");
			
			CString strComOld = strCom;
			strTemp.Format(" and Logfile='System'");
			strCom += strTemp;
			objects = services->ExecQuery(LPCTSTR(strCom),"WQL",0x10,NULL);		
			int nTotalSystemCount = objects->Count;

			strCom = strComOld;
			strTemp.Format(" and Logfile='Application'");
			strCom += strTemp;
			objects = services->ExecQuery(LPCTSTR(strCom),"WQL",0x10,NULL);		
			int nTotalApplicationCount = objects->Count;

			strCom = strComOld;
			strTemp.Format(" and Logfile='DNS'");
			strCom += strTemp;
			objects = services->ExecQuery(LPCTSTR(strCom),"WQL",0x10,NULL);		
			int nTotalDNSCount = objects->Count;

			strCom = strComOld;
			strTemp.Format(" and Logfile='Security'");
			strCom += strTemp;
			objects = services->ExecQuery(LPCTSTR(strCom),"WQL",0x10,NULL);		
			int nTotalSecurityCount = objects->Count;

			strTemp = strReturn;			
			strReturn.Format("TotalErrorCount=%d$TotalSystemCount=%d$TotalApplicationCount=%d$TotalDNSCount=%d$TotalSecurityCount=%d$SourceName=%s$",
				nTotalErrorCount,nTotalSystemCount,nTotalApplicationCount,nTotalDNSCount,nTotalSecurityCount,strTemp);

			//strReturn.Format("checkcount=%d$filtercount=%d$", nTotalEventCount, nFilterEventCount);

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
		//nSize = 2048;
		MakeCharByString(szReturn,nSize,strOutRet);	 

	}
	CoUninitialize();
	return TRUE;
}

extern "C" _declspec(dllexport) 
BOOL GetPhysicsMemoryInfo(const char * strParas, char * szReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CoInitialize(NULL);
	{
		char strTMP[1024]=_T("");
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(strParas, szReturn, nSize, services))
			return FALSE;

		CString strFile = _T("");
		CString strLog = _T("");
		strFile.Format("GetPhysicsMemoryInfo");
		WriteLog(strFile,"------------- GetPhysicsMemoryInfo ----------------------");
		try
		{
			WbemScripting::ISWbemObjectSetPtr objects = services->InstancesOf("CIM_OperatingSystem", 0x10, NULL);
			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
			ULONG fetched; 
			VARIANT var; 
			double fTotalPhysicalMemory=0, fFreePhysicalMemory=0,fpercentUsedPhysicalMemory=0,fTemp;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("TotalVisibleMemorySize",0);
				
				fTotalPhysicalMemory = ((double)prop->GetValue())/1024;
				prop = properties->Item("FreePhysicalMemory",0);
				fFreePhysicalMemory = ((double)prop->GetValue())/1024;
				
				if (fFreePhysicalMemory > fTotalPhysicalMemory)
				{
					fTemp			= fTotalPhysicalMemory;
					fTotalPhysicalMemory	= fFreePhysicalMemory;
					fFreePhysicalMemory		= fTemp;
				}
		
				fpercentUsedPhysicalMemory = (fTotalPhysicalMemory - fFreePhysicalMemory)/fTotalPhysicalMemory*100;

				strReturn.Format("percentUsedPhysicalMemory=%.2f$FreePhysicalMemory=%.2f$TotalPhysicalMemory=%.2f$", 
					fpercentUsedPhysicalMemory,fFreePhysicalMemory,fTotalPhysicalMemory);
				WriteLog(strFile,strReturn);

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
		nSize = 1024*10;
		MakeCharByString(szReturn,nSize,strOutRet);	 
	}
	CoUninitialize();

	return TRUE;
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

bool ConnectServer(const list< string > paramList, char * szReturn, int& nSize,WbemScripting::ISWbemServicesPtr &services)
{
	char *machinename=NULL, *user=NULL, *pswd=NULL;
	CString strMachineName="",strUser = "", strPSWD = "";

	//machinename = FindStrValue(strParas, "_MachineName");
	//user = FindStrValue(strParas, "_UserAccount");
	//pswd = FindStrValue(strParas, "_PassWord");

	strMachineName = FindStrValue(paramList, "_MachineName");
	strUser = FindStrValue(paramList, "_UserAccount");
	strPSWD = FindStrValue(paramList, "_PassWord");

	//machinename = strMachineName.GetBuffer();
	//user = strUser.GetBuffer();
	//pswd = strPSWD.GetBuffer();

	//strMachineName.ReleaseBuffer();
	//strUser.ReleaseBuffer();
	//strPSWD.ReleaseBuffer();

	//CoInitialize(NULL);

	try 
	{ 
		WbemScripting::ISWbemLocatorPtr locator; 
		locator.CreateInstance(WbemScripting::CLSID_SWbemLocator);

		if (locator != NULL) 
		{ 
			if(IsLocalHost(strMachineName))
			{
				services = locator->ConnectServer(".","root\\cimv2","","","","",0,NULL);
			}
			else
			{
				services = locator->ConnectServer(strMachineName.GetBuffer() , "root\\cimv2",
					strUser.GetBuffer() , strPSWD.GetBuffer() , "" , "" , 0 , NULL);
				strMachineName.ReleaseBuffer();
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

		return false;
	} 
	catch(...) 
	{ 
		sprintf(szReturn, "error=Error ocured: %d",::GetLastError());
		return false;
	} 
	return true;
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
		if(!ConnectServer(paramList, szReturn, nSize, services))
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
				sprintf(szReturn, "error=Error ocured: path is error");
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
			CString strModifiedTime="";
			CString strInstallDate="";
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
					nLiveDate = preTime.GetTotalSeconds();

					strReturn.Format("file_Size=%d$live_date=%d$Modify_Date=%s$",nSize,nLiveDate,strModifiedTime);
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

//bin.liu
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int GetPrivateProfileString1(char * m_AppName, char * m_KeyName, char * m_default, char * m_Ret, int size, char * m_FileName)
{
	std::string  gRoot_path=GetSiteViewRootPath();
		//gRoot_path = "D:\\SiteView\\SiteView ECC";  //zjw

	char szPath[1024]={0};
	sprintf(szPath,"%s\\data\\TmpIniFile\\%s",gRoot_path.c_str(),m_FileName);
	return GetPrivateProfileString(m_AppName,m_KeyName,m_default,m_Ret,size,szPath);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int GetPrivateProfileInt1(char * m_AppName, char * m_KeyName, int k, char * m_FileName )
{
	std::string  gRoot_path=GetSiteViewRootPath();
		//gRoot_path = "D:\\SiteView\\SiteView ECC";  //zjw

	char szPath[1024]={0};
	sprintf(szPath,"%s\\data\\TmpIniFile\\%s",gRoot_path.c_str(),m_FileName);
	return GetPrivateProfileInt(m_AppName,m_KeyName,k,szPath);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int WritePrivateProfileString1( char * m_AppName, char * m_KeyName, char * m_Value, char * m_FileName)
{
	string strInt;
	std::string  gRoot_path=GetSiteViewRootPath();

	char szPath[1024]={0};
	sprintf(szPath,"%s\\data\\TmpIniFile\\%s",gRoot_path.c_str(),m_FileName);
	return WritePrivateProfileString(m_AppName,m_KeyName,m_Value,szPath);
}
extern "C" _declspec(dllexport) 
BOOL GetDirInfo(const char * strParas, char * szReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	list<string> paramList;
	MakeStringListByChar(paramList,strParas); //将输入参数拆入一个 list<string> 中	

	CoInitialize(NULL);
	{
		CString strReturn = "";
		WbemScripting::ISWbemServicesPtr services;
		if(!ConnectServer(paramList, szReturn, nSize, services))
			return false;
		try
		{
			CString filename ="";
			filename = FindStrValue(paramList,"_dirpath");
			if(filename.IsEmpty())
			{
				//strReturn.Format("error=Error ocured: File path is NULL");
				sprintf(szReturn, "error=Error ocured: path is not find ");
				return false;
			}
			CString strMonitorID = FindStrValue(paramList, "_MonitorID");
			//bool bRet = FilePathParser(filename);
			//if ( !bRet )
			//{
			//	//strReturn.Format("error=Error ocured: File path is error");
			//	sprintf(szReturn, "error=Error ocured: File path is error");
			//	return false;
			//}
            CString csDrive=filename.Mid(0,2);
			//printf(csDrive);
			CString csPath=filename.Mid(2,filename.GetLength()-2)+"\\";
			csPath.Replace("\\","\\\\");
			//printf(csPath);
			//Replace("\","\\\\");
			CString strCom="";
			//strCom.Format("SELECT * FROM CIM_LogicalFile where Name='%s'",filename);
			strCom.Format("SELECT * FROM CIM_DataFile where (Drive='%s') and (Path = '%s') ",csDrive,csPath);
			
			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(LPCTSTR(strCom),"WQL",0x10,NULL);
			//printf(LPCTSTR(strCom));
			printf(strCom);
			//WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery("SELECT * FROM CIM_DataFile where (Drive='D:') and (Path = '\\\\backup\\\\Outlook\\\\') ","WQL",0x10,NULL);
			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
			ULONG fetched; 
			VARIANT var; 
			//CString strSize;
			int nSize=0;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
            { 
				nSize=nSize+1;
			}
			char chTempFile[1024] = {0};
			sprintf(chTempFile, "wmidir_%s.ini", strMonitorID);
			
			//GetPrivateProfileInt1("wmidir", "HistoryValue",Hnsize, chTempFile);
			char tempbb[10]={0} ;
			GetPrivateProfileString1("wmidir", "HistoryValue", "0",tempbb , 1024, chTempFile);
			int Hnsize=atoi(tempbb);
			char tempaa[10]={0} ;
			sprintf(tempaa, "%d", nSize); 
			WritePrivateProfileString1( "wmidir", "HistoryValue",tempaa, chTempFile );
			printf(tempaa);
			printf(" and ");
			printf(tempbb);
			if(nSize>Hnsize)
			{
				nSize=nSize-Hnsize;
			}else
			{
				nSize=0;
			}
			strReturn.Format("add_fileCount=%d$",nSize);

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


extern "C" _declspec(dllexport)  //段毅 模板1336，复制远程机器的文件到本地，并判断内容 2010-11-02 WMI权限有限制，用IPC需要135,445端口
BOOL GetRemoteFile(const char * strParas, char * szReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	try
	{
		CString strMachine = "";
		CString strUser = "";
		CString strPsword = "";
		CString strMonitorID = "";
		CString strMatchStr = "";
		CString remoteFileName ="";

		strMachine = FindStrValue(strParas, "_MachineName");
		strUser  = FindStrValue(strParas, "_UserAccount");
		strPsword  = FindStrValue(strParas, "_PassWord");
		strMonitorID = FindStrValue(strParas, "_MonitorID");
		strMatchStr = FindStrValue(strParas, "_MatchStr");
		remoteFileName = FindStrValue(strParas, "_remoteDataFilePath");
		
		char  strErrorFile1[128] = {0};
		sprintf(strErrorFile1,"C:\\%s_Error1.log",strMonitorID.GetBuffer(strMonitorID.GetLength()));

		char cmdLine[256] = {0};
		sprintf(cmdLine, "cmd.exe /k \"net use \\\\%s\\c$ %s /user:%s > %s && exit\"",
			strMachine.GetBuffer(strMachine.GetLength()),strPsword.GetBuffer(strPsword.GetLength()),
			strUser.GetBuffer(strUser.GetLength()),strErrorFile1);
		
		//cout << "cmdline1 = " << cmdLine << endl;
		CString content = "";
		executeExeFile(cmdLine, content);

		//if (!content.IsEmpty())
		//{
		//	cout << "cmd faild"<< endl;
		//	return FALSE;
		//}
		cout << "content = " << content << endl;

		if (!isCmdSuccess(strErrorFile1, "成功"))
		{
			printf("exe cmd error\n");
			sprintf(szReturn, "error=连接失败");
			return FALSE;
		}

		char  localFileName[128] = {0};
		char  strErrorFile2[128] = {0};


		sprintf(strErrorFile2,"C:\\%s_Error2.log",strMonitorID.GetBuffer(strMonitorID.GetLength()));
		sprintf(localFileName,"C:\\%s_dataFile.log",strMonitorID.GetBuffer(strMonitorID.GetLength()));


		strMachine.ReleaseBuffer();
		
		remoteFileName = remoteFileName.Mid(2,remoteFileName.GetLength()-2);
		//cout << "remoteFileName = " << remoteFileName;
		memset(cmdLine, 0, sizeof(cmdLine));
		sprintf(cmdLine, "cmd.exe /k \"copy \\\\%s\\c$\\%s %s > %s && exit\"",
			strMachine.GetBuffer(strMachine.GetLength()),
			remoteFileName.GetBuffer(remoteFileName.GetLength()),
			localFileName,strErrorFile2);

	//	cout << "cmdline = " << cmdLine << endl;

		CString content2 = "";
		executeExeFile(cmdLine, content2);

		//if (!content.IsEmpty())
		//{
		//	cout << "cmd2 faild"<< endl;
		//	return FALSE;
		//}
		cout << "content2 = " << content2 << endl;

		if (!isCmdSuccess(strErrorFile2, "已复制"))
		{
			printf("exe cmd error\n");
			sprintf(szReturn, "error=找不到网络路径");

			return FALSE;
		}

//结果是否匹配字符
		if (!isCmdSuccess(localFileName, strMatchStr.GetBuffer(strMatchStr.GetLength())))
		{
			printf("exe cmd error\n");
			strcpy(szReturn, "isFind=0$");	
		}
		else
		{
			printf("CHECK OK OK\n");
			strcpy(szReturn, "isFind=1$");
		}
	}
	catch(...) 
	{ 
		sprintf(szReturn, "error=Error ocured: %d",::GetLastError());
		return FALSE;
	} 

	//strcpy(szReturn, strReturn);
	//CString strOutRet;
	//strOutRet =szReturn;
	//nSize = 2048;
	//MakeCharByString(szReturn,nSize,strOutRet);	 

	return TRUE;
}

//dy2010-10-29修正IIS取值方式
extern "C" _declspec(dllexport) 
BOOL GetIISInformation(const char * strParas, char * szReturn, int& nSize)
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
			int nMaximumConnections = 0,nCurrentAnonymousUsers = 0,nCurrentConnections = 0, nTotalNotFoundErrors = 0;
			float fPostRequestsPersec1 = 0, fGetRequestsPersec1 = 0, fBytesSentPerSec1 = 0, fBytesReceivedPerSec1 = 0,
				fBytesTotalPersec1 = 0, fTimestamp_PerfTime1 = 0, fFrequency_PerfTime = 0;

			CString strMachine=_T("");
			CString strMonitorID=_T("");
			char  iniPath[256]  = {0}; 
			char  CurrPath[128] = {0};

			GetCurrentDirectory(sizeof(CurrPath)-1, CurrPath);
			strMachine = FindStrValue(strParas, "_MachineName");
			strMonitorID = FindStrValue(strParas, "_MonitorID");

			std::string::size_type spos;
			string input = CurrPath;
			spos=input.find("SiteView ECC");
			string fPath = input.substr(0,spos+12); //12:"SiteView ECC"的长度

			sprintf(iniPath, "%s\\data\\tmpinifile\\IIS_%s_%s.ini", fPath.c_str(), strMachine.GetBuffer(strMachine.GetLength()), strMonitorID.GetBuffer(strMonitorID.GetLength()));
			printf("iniPath = %s\n", iniPath);

		

			bool isFirst = false; 
			FILE *fp = NULL;
			fp = fopen(iniPath, "r");
			if (fp==NULL)
			{
				strcpy(szReturn, "error=无历史值,请再次刷新监测器");
				printf("无历史值\n");
				isFirst = true;
			}
			else
			{
				fclose(fp);
				char chValue[100] = {0};
				GetPrivateProfileString("PostRequestsPersec", "HistoryValue", "" ,chValue, sizeof(chValue)-1, iniPath);
				fPostRequestsPersec1 = atof(chValue);

				GetPrivateProfileString("GetRequestsPersec", "HistoryValue", "" ,chValue, sizeof(chValue)-1, iniPath);
				fGetRequestsPersec1 = atof(chValue);

				GetPrivateProfileString("BytesSentPerSec", "HistoryValue", "" ,chValue, sizeof(chValue)-1, iniPath);
				fBytesSentPerSec1 = atof(chValue);

				GetPrivateProfileString("BytesReceivedPerSec", "HistoryValue", "" ,chValue, sizeof(chValue)-1, iniPath);
				fBytesReceivedPerSec1 = atof(chValue);

				GetPrivateProfileString("BytesTotalPersec", "HistoryValue", "" ,chValue, sizeof(chValue)-1, iniPath);
				fBytesTotalPersec1 = atof(chValue);

				GetPrivateProfileString("Timestamp_PerfTime", "HistoryValue", "" ,chValue, sizeof(chValue)-1, iniPath);
				fTimestamp_PerfTime1 = atof(chValue);

				printf("历史值:\n");
				printf("PostRequestsPersec=%0.f\nGetRequestsPersec=%0.f\nBytesSentPerSec=%0.f\nBytesReceivedPerSec=%0.f\nBytesTotalPersec=%0.f\nTimestamp_PerfTime=%0.f\n",
					fPostRequestsPersec1,fGetRequestsPersec1,
					fBytesSentPerSec1,fBytesReceivedPerSec1,
					fBytesTotalPersec1,fTimestamp_PerfTime1);
			}

			float fPostRequestsPersec2 = 0, fGetRequestsPersec2 = 0, fBytesSentPerSec2 = 0, fBytesReceivedPerSec2 =0,
				fBytesTotalPersec2 = 0, fTimestamp_PerfTime2 = 0;

			_bstr_t com; 
			com="SELECT * FROM Win32_PerfRawData_W3SVC_WebService where Name='_Total'"; 
			WbemScripting::ISWbemObjectSetPtr objects = services->ExecQuery(com,"WQL",0x10,NULL); 
			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
			ULONG fetched; 
			VARIANT var; 
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
			}
		
			char result[100];
			sprintf(result, "%.0f", fPostRequestsPersec2);
			WritePrivateProfileString("PostRequestsPersec", "HistoryValue", result, iniPath);

			sprintf(result, "%.0f", fGetRequestsPersec2);
			WritePrivateProfileString("GetRequestsPersec", "HistoryValue", result, iniPath);

			sprintf(result, "%.0f", fBytesSentPerSec2);
			WritePrivateProfileString("BytesSentPerSec", "HistoryValue", result, iniPath);

			sprintf(result, "%.0f", fBytesReceivedPerSec2);
			WritePrivateProfileString("BytesReceivedPerSec", "HistoryValue", result, iniPath);

			sprintf(result, "%.0f", fBytesTotalPersec2);
			WritePrivateProfileString("BytesTotalPersec", "HistoryValue", result, iniPath);

			sprintf(result, "%.0f", fTimestamp_PerfTime2);
			WritePrivateProfileString("Timestamp_PerfTime", "HistoryValue", result, iniPath);

	
			if (!isFirst)
			{
				float fInterval = (fTimestamp_PerfTime2-fTimestamp_PerfTime1)/fFrequency_PerfTime;
				strReturn.Format("maxconnections=%d$currentnonanonymoususers=%d$currentconnections=%d$totalnotfounderrors=%d$postrequestspersec=%.0f$getrequestspersec=%.0f$bytessentpersec=%.0f$bytesreceivedpersec=%.0f$bytestotalpersec=%.0f$", 
					nMaximumConnections, nCurrentAnonymousUsers, nCurrentConnections, nTotalNotFoundErrors,
					(fPostRequestsPersec2-fPostRequestsPersec1)/fInterval, (fGetRequestsPersec2-fGetRequestsPersec1)/fInterval,
					(fBytesSentPerSec2-fBytesSentPerSec1)/fInterval, (fBytesReceivedPerSec2-fBytesReceivedPerSec1)/fInterval,
					(fBytesTotalPersec2-fBytesTotalPersec1)/fInterval);
				strReturn.Replace("\t", NULL);
			}
			else
			{
				return FALSE;
			}
			
			printf("第二次取值:\n");
			printf("PostRequestsPersec=%0.f\nGetRequestsPersec=%0.f\nBytesSentPerSec=%0.f\nBytesReceivedPerSec=%0.f\nBytesTotalPersec=%0.f\nTimestamp_PerfTime=%0.f\n",
				fPostRequestsPersec2,fGetRequestsPersec2,
				fBytesSentPerSec2,fBytesReceivedPerSec2,
				fBytesTotalPersec2,fTimestamp_PerfTime2);

			printf("\nMaximumConnections=%d\nCurrentAnonymousUsers=%d\nCurrentConnections=%d\nTotalNotFoundErrors=%d\nFrequency_PerfTime=%0.f\n",
				nMaximumConnections,nCurrentAnonymousUsers,nCurrentConnections,nTotalNotFoundErrors,fFrequency_PerfTime);
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
			sprintf(szReturn, "error=Error ocured...: %d",::GetLastError());
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

//dy2010-11-18磁盘变化率(一直显示上次的变化值，直到再次改变位置)
extern "C" _declspec(dllexport) 
BOOL diskChangeInfoUntil(const char * strParas, char * szReturn, int& nSize)
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
			CString strMachine=_T("");
			CString strMonitorID=_T("");

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
			} 
//----------------------------------------------------------------
			string fPath1 = GetSiteViewRootPath();
			printf("fPath1 = %s\n", fPath1);
			//string fPath = getECCPath();
			char   iniPath[256]  = {0}; 

			strMachine = FindStrValue(strParas, "_MachineName");
			strMonitorID = FindStrValue(strParas, "_MonitorID");

			sprintf(iniPath, "C:\\SiteView\\SiteView ECC\\data\\tmpinifile\\DISK_%s_%s.ini",  strMachine.GetBuffer(strMachine.GetLength()), strMonitorID.GetBuffer(strMonitorID.GetLength()));
			
			//sprintf(iniPath, "%s\\data\\tmpinifile\\DISK_%s_%s.ini", fPath.c_str(), strMachine.GetBuffer(strMachine.GetLength()), strMonitorID.GetBuffer(strMonitorID.GetLength()));
			printf("iniPath = %s\n", iniPath);

			FILE *fp = NULL;
			fp = fopen(iniPath, "r");
			if (fp==NULL)
			{
				printf("无历史值,请再次刷新监测器\n");

				char temp[100];
				sprintf(temp, "%0.f", nFreeSize);//0.f不保留小数，0.2f保留2位小数
				WritePrivateProfileString("histroyParameterOne", "nFreeSize", temp, iniPath);

				printf("temp = %s\n",temp);
				strcpy(szReturn, "error=无历史值,请再次刷新监测器");
				return FALSE;
			}
			else
			{
				fclose(fp);
				char chValue[100] = {0};
				float hostroyValueOne = 0;
				float hostroyValueTwo = 0;

				DWORD getSize1 = GetPrivateProfileString("histroyParameterOne", "nFreeSize", "" ,chValue, sizeof(chValue)-1, iniPath);
				hostroyValueOne = atof(chValue);
				printf("chValue=%s getSize1=%d\n hostroyValueOne=%0.f\n", chValue, getSize1, hostroyValueOne);//%0.2f保留2位小数，%0.f不保留小数

				DWORD getSize2 = GetPrivateProfileString("histroyParameterTwo", "nFreeSize", "" ,chValue, sizeof(chValue)-1, iniPath);
				hostroyValueTwo = atof(chValue);
				printf("chValue=%s getSize2=%d\n hostroyValueTwo=%0.f\n", chValue, getSize2, hostroyValueTwo);

				printf("nFreeSize = %0.f\n", nFreeSize);
				if (getSize2==0)//还没有2次值
				{
					if (nFreeSize==hostroyValueOne)
					{
						printf("无Parameter2 无变化 hostroyValueOne\n");
						printf("结果是：%0.f\n",hostroyValueOne);

						strReturn.Format("changeSize=0$Mbfree=%0.2f$TotalSize=%0.2f$", 
							 nFreeSize/1048576, nSize/1048576);
						strReturn.Replace("\t", NULL);
					}
					else
					{
						printf("==结果是：%0.f\n",nFreeSize-hostroyValueOne);

						printf("写histroyParameterTwo"); //第二次的值写入
						char temp[100];
						sprintf(temp, "%0.f", nFreeSize);
						WritePrivateProfileString("histroyParameterTwo", "nFreeSize", temp, iniPath);

						strReturn.Format("changeSize=%0.2f$Mbfree=%0.2f$TotalSize=%0.2f$",
							(nFreeSize-hostroyValueOne)/1048576 , nFreeSize/1048576, nSize/1048576);
					}
				}
				else
				{
					if (nFreeSize==hostroyValueTwo)
					{
						printf("有Parameter2 hostroyValueTwo-hostroyValueOne\n");
						printf("结果是：%0.2f\n",hostroyValueTwo-hostroyValueOne);

						strReturn.Format("changeSize=%0.2f$Mbfree=%0.2f$TotalSize=%0.2f$",
							(hostroyValueTwo-hostroyValueOne)/1048576 , nFreeSize/1048576, nSize/1048576);

					}
					else//value2 写到histroyParameterOne，value1Input写到histroyParameterTwo
					{
						printf("！=结果是：%0.2f\n",nFreeSize-hostroyValueTwo);

						char temp[100];
						sprintf(temp, "%0.2f", hostroyValueTwo);
						WritePrivateProfileString("histroyParameterOne", "nFreeSize", temp, iniPath);

						sprintf(temp, "%0.2f", nFreeSize);
						WritePrivateProfileString("histroyParameterTwo", "nFreeSize", temp, iniPath);

						strReturn.Format("changeSize=%0.2f$Mbfree=%0.2f$TotalSize=%0.2f$",
							(nFreeSize-hostroyValueTwo)/1048576 , nFreeSize/1048576, nSize/1048576);
					}
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
			sprintf(szReturn, "error=Error ocured...: %d",::GetLastError());
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


extern "C" _declspec(dllexport) //dy2010-11-19磁盘变化率
BOOL diskChangeInfo(const char * strParas, char * szReturn, int& nSize)
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
			CString strMachine=_T("");
			CString strMonitorID=_T("");

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
			}
			//----------------------------------------------------------------
			//string fPath = getECCPath();

			string fPath = GetSiteViewRootPath();
			char   iniPath[256]  = {0}; 

			strMachine = FindStrValue(strParas, "_MachineName");
			strMonitorID = FindStrValue(strParas, "_MonitorID");

			//sprintf(iniPath, "C:\\SiteView\\SiteView ECC\\data\\tmpinifile\\DISK_%s_%s.ini",  strMachine.GetBuffer(strMachine.GetLength()), strMonitorID.GetBuffer(strMonitorID.GetLength()));
			sprintf(iniPath, "%s\\data\\tmpinifile\\DISK_%s_%s.ini", fPath.c_str(), strMachine.GetBuffer(strMachine.GetLength()), strMonitorID.GetBuffer(strMonitorID.GetLength()));
			printf("iniPath = %s\n", iniPath);

			FILE *fp = NULL;
			fp = fopen(iniPath, "r");
			if (fp==NULL)
			{
				printf("无历史值,请再次刷新监测器\n");

				char temp[100];
				sprintf(temp, "%0.f", nFreeSize);//0.f不保留小数，0.2f保留2位小数
				WritePrivateProfileString("histroyValue", "nFreeSize", temp, iniPath);

				printf("temp = %s\n",temp);
				strcpy(szReturn, "error=无历史值,请再次刷新监测器");
				return FALSE;
			}
			else
			{
				fclose(fp);
				char chValue[100] = {0};
				float hostroyValue = 0;

				DWORD getSize1 = GetPrivateProfileString("histroyValue", "nFreeSize", "" ,chValue, sizeof(chValue)-1, iniPath);
				hostroyValue = atof(chValue);
				printf("chValue=%s getSize1=%d\n hostroyValue=%0.f\n", chValue, getSize1, hostroyValue);//%0.2f保留2位小数，%0.f不保留小数

				char temp[100];
				sprintf(temp, "%0.f", nFreeSize);
				WritePrivateProfileString("histroyValue", "nFreeSize", temp, iniPath);

				float usedValue = 0;
				float hostroyUsedValue = 0;

				hostroyUsedValue = nSize - hostroyValue;
				usedValue        = nSize - nFreeSize;

				if (usedValue < hostroyUsedValue)
				{  
					strReturn.Format("changeSize=%0.2f$Mbfree=%0.2f$TotalSize=%0.2f$ChangeRate=%0.2f$",
						(usedValue-hostroyUsedValue)/1048576 , nFreeSize/1048576, nSize/1048576, (hostroyUsedValue-usedValue)/hostroyUsedValue*100);
				}
				else
				{
					strReturn.Format("changeSize=%0.2f$Mbfree=%0.2f$TotalSize=%0.2f$ChangeRate=%0.2f$",
						(usedValue-hostroyUsedValue)/1048576 , nFreeSize/1048576, nSize/1048576, (usedValue-hostroyUsedValue)/hostroyUsedValue*100);
			
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
			sprintf(szReturn, "error=Error ocured...: %d",::GetLastError());
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



