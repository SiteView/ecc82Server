// LinuxStation.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include <AFXINET.H>
#include "LinuxStation.h"
#include "SiteViewService.h"
#include <comutil.h>
#include <iostream>
#include <list>
#include <string>
#include <comdef.h>
#include <fstream>
using namespace std;
//#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  SPANTIME  5*60; //监测时间间隔5分钟

//
//	注意！
//
//		如果此 DLL 动态链接到 MFC
//		DLL，从此 DLL 导出并
//		调入 MFC 的任何函数在函数的最前面
//		都必须添加 AFX_MANAGE_STATE 宏。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CLinuxStationApp

BEGIN_MESSAGE_MAP(CLinuxStationApp, CWinApp)
END_MESSAGE_MAP()


// CLinuxStationApp 构造

CLinuxStationApp::CLinuxStationApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CLinuxStationApp 对象

CLinuxStationApp theApp;


// CLinuxStationApp 初始化

BOOL CLinuxStationApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}



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
	char szFileDirectory[MAX_PATH]={};
	GetCurrentDirectory(MAX_PATH, szFileDirectory);
	szFileName.Format("%s\\TestLog",szFileDirectory);
	//CreateDirectory(szFileName,NULL);

	try
	{		
		DWORD dwAttr = GetFileAttributes(szFileName);
		if ( (dwAttr != -1) && (dwAttr & FILE_ATTRIBUTE_DIRECTORY) )
		{
			//cout << "文件夹已经存在" << endl;
		}
		else
		{
			BOOL bRet =	CreateDirectory(szFileName,NULL);
			if ( !bRet)
			{
				//cout << "创建目录失败" << endl;
				return false;
			}
			//cout << "创建目录成功" << endl;
		}
	}
	catch(...)
	{
		return false;
	}

	szFileName.Format("%s\\%s.log",szFileName,strFileName);
	//szFileName.Format("%s.log",strFileName);

	ofstream stFileStream;
	stFileStream.open(szFileName, ios::app);
	stFileStream<<szTempBuf<<"\t"<<strLog<<endl;
	stFileStream.close();
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

bool MakeStringListByChar(CStringList& pList, const char * pInput )
{
	const char * p;
	int nSize;
	p=pInput;
	while(*p!='\0')
	{
		nSize = static_cast<int>(strlen(p));
		if( nSize>0 )
		{	
			pList.AddHead(p);
			//	printf(p);
		}
		p=p+nSize+1;
	}

	return true;
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

//把输入时间字符串 如（2009-08-03 08:31:12）返回一个CTime
//如果输入字符串有误，返回当前时间

CTime StrToDate(CString sDateTime)
{
	if (sDateTime.IsEmpty()||(sDateTime.GetLength()<19))
	{
		CTime curTime = CTime::GetCurrentTime();
		return NULL;
	}
	int iYear = 0, iMonth = 0, iDay =0;
	int iHour = 0, iMinute = 0, iSecond =0;

	CString sYear,sMonth,sDay;
	CString sHour,sMinute,sSecond;

	//int nPos = 0;
	sYear	= sDateTime.Mid(0,4);
	sMonth  = sDateTime.Mid(5,2);
	sDay	= sDateTime.Mid(8,2);
	sHour	= sDateTime.Mid(11,2);
	sMinute = sDateTime.Mid(14,2);
	sSecond = sDateTime.Mid(17,2);

	iYear	= atoi(sYear);
	iMonth	= atoi(sMonth);
	iDay	= atoi(sDay);
	iHour	= atoi(sHour);
	iMinute	= atoi(sMinute);
	iSecond	= atoi(sSecond);

	if (iMonth>12 || iDay>31 || iHour>24 || iMinute>60 || iSecond>60)
	{
		return NULL;
	}

	CTime tTime(iYear, iMonth, iDay, iHour, iMinute, iSecond);
	return tTime;

}

extern "C" _declspec(dllexport) 
BOOL GetCheckInfo( const char * strParas, char * strReturn, int& nSize )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CStringList paramList;
	MakeStringListByChar(paramList,strParas);
	POSITION pos = paramList.FindIndex(0);

	CString strURL	= _T("");
	CString strIP	= _T("");
	CString strID	= _T("");

	CString strFile = _T("GetCheckInfo");
	CString strLog	= _T("");

	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if ( strTemp.Find("_URL=",0) == 0 )
		{
			strURL = strTemp.Right(strTemp.GetLength() - strlen("_URL="));
		}
		else if ( strTemp.Find("_IP=",0) == 0 )
		{
			strIP = strTemp.Right(strTemp.GetLength() - strlen("_IP="));
		}
		else if ( strTemp.Find( "_MonitorID=", 0 ) == 0 )
		{
			strID = strTemp.Right(strTemp.GetLength() - strlen("_MonitorID="));
		}
	}

	cout << "strURL:" << strURL << endl;
	cout << "strIP:	" << strIP << endl;
	cout << "strID:	" << strID << endl;	

	CString strServer, strObject;
	INTERNET_PORT nPort;
	DWORD dwServerType;
	BOOL bRet;

	WriteLog (strFile, "------------ GetCheckInfo Start -----------------");
	strLog.Format("URL:%s; IP:%s; MonID:%s",strURL,strIP,strID);
	WriteLog(strFile, strLog);
	
	if (strURL.IsEmpty())
	{
		sprintf(strReturn,"error=URL is null");
		WriteLog(strFile,"-------------- URL 没找到 ----------------");
		return FALSE;
	}

	bRet = AfxParseURL(strURL.GetBuffer(), dwServerType, strServer, strObject, nPort);
	if( !bRet )
	{
		sprintf(strReturn, "error=URL is error");
		WriteLog(strFile,"-------------- URL 设置不正确 ----------------");
		return FALSE;
	}

	CTime curTime = CTime::GetCurrentTime();
	CTime preTime ;
	CString strCurTime = _T(""), strPreTime = _T("");
	CString strIniFile = _T("");	
	strIniFile.Format("..\\data\\TmpIniFile\\LinuxStation_%s.ini",strID);
    strCurTime = curTime.Format("%Y-%m-%dT%H:%M:%S");

	cout << "strCurTime:" << strCurTime << endl;	

	char szTemp[64]={0};
	GetPrivateProfileString(strID,"Time","",szTemp,63,strIniFile);
	WritePrivateProfileString(strID,"Time",strCurTime, strIniFile);


	//
	strPreTime = szTemp;
	if (strPreTime.IsEmpty()) //第一次监测
	{
		ULONG64 nTime = curTime.GetTime();

		cout << "当前时间：" << nTime << endl;
        
		//监测时间当前时间往前5分钟
		nTime -= (60 *5 );
		cout << "改变时间：" << nTime <<endl;

		CTime ctSpan(nTime);
		strPreTime = ctSpan.Format("%Y-%m-%dT%H:%M:%S");
	}

	cout << "strPreTime:" << strPreTime <<endl;

	strLog.Format("strCurTime:%s; strPreTime:%s",strCurTime,strPreTime);
	WriteLog(strFile, strLog);

	CoInitialize(NULL);
	{
		try
		{		
			HRESULT hr = S_OK;
			SiteViewService::CSiteViewService* ws = new SiteViewService::CSiteViewService();
			hr = ws->SetUrl(strURL.GetBuffer());
			if (FAILED(hr))
			{
				cout << "SetUrl 出错" << endl;
				delete ws;
				sprintf(strReturn,"error=Set url is failed");
				CoUninitialize();

				WriteLog(strFile,"---------------- SetUrl 出错 ----------------" );
				return FALSE;
			}
			
			CComBSTR ip(strIP);
			CComBSTR d1(strPreTime);
			CComBSTR d2(strCurTime);
			SiteViewService::LinuxStationCheckInfo* checkinfo;
			int nLen = 0;

			//调用WebServer
			hr = ws->get_LinuxStationCheckInfo(ip, d1, d2, &checkinfo, &nLen);
			if (FAILED(hr))
			{
				cout << "get_LinuxStationCheckInfo 出错" <<endl;
				delete ws;
				sprintf(strReturn,"error=get_LinuxStationCheckInfo is failed");
				CoUninitialize();
				WriteLog(strFile, "--------------- get_LinuxStationCheckInfo 出错 -----------");
				return FALSE;
			}

			CString strReturnIP = _T("");
			CString strDate = _T("");
			CString strValue = _T("");
			int nCpuUtilization = 0;
			int nMemorySum = 0;
			int nMemoryUse = 0;
			int nMemoryRelax = 0;

			//printf("ip: %s, cpu: %s, mem_use: %s, mem_sum: %s\n", (*checkinfo).CS_IP, 
				//(*checkinfo).CPU_SCALE, (*checkinfo).MEMORY_USE, (*checkinfo).MEMORY_SUM);

			ULONG64 nTime=0, nTimeMax=0;
			for (int i=0; i<nLen; i++)
			{
				strValue = (*checkinfo).CS_DATE;
				cout << strValue << endl;
				
				CTime ct = StrToDate(strValue);
				if (ct == NULL)
				{
					strReturnIP = (* checkinfo).CS_IP;
					strDate = (* checkinfo).CS_DATE;
					strValue = (* checkinfo).CPU_SCALE;
					nCpuUtilization = atoi( strValue );
					strValue = (* checkinfo).MEMORY_RELAX;
					nMemoryRelax = atoi( strValue );
					strValue = (* checkinfo).MEMORY_SUM ;
					nMemorySum = atoi( strValue);
					strValue = (* checkinfo).MEMORY_USE;
					nMemoryUse = atoi( strValue );
					strLog.Format("strDate: ",strDate);
					WriteLog(strFile, strLog);
					WriteLog(strFile, "-------------  时间转换失败 ------------------");
					break;
					
				}
				nTime = ct.GetTime();
				if (nTime > nTimeMax)
				{
					nTimeMax = nTime;
					strReturnIP = (* checkinfo).CS_IP;
					strDate = (* checkinfo).CS_DATE;
					strValue = (* checkinfo).CPU_SCALE;
					nCpuUtilization = atoi( strValue );
					strValue = (* checkinfo).MEMORY_RELAX;
					nMemoryRelax = atoi( strValue );
					strValue = (* checkinfo).MEMORY_SUM ;
					nMemorySum = atoi( strValue);
					strValue = (* checkinfo).MEMORY_USE;
					nMemoryUse = atoi( strValue );
				}

				checkinfo ++;
			}
			cout << "---------------  获取数据 -------------------" << endl;

			//strValue = (*checkinfo).CS_IP ;
			//cout << strValue << endl;
			//strLog.Format("CS_IP:		%s",strValue);
			//WriteLog(strFile, strLog);
			//strValue = (*checkinfo).CS_DATE;
			//cout << strValue << endl;
			//strLog.Format("CS_DATE:	%s",strValue);
			//WriteLog(strFile, strLog);
			//strValue = (*checkinfo).CPU_SCALE;
			//cout << strValue << endl;
			//strLog.Format("CPU_SCALE:	%s",strValue);
			//WriteLog(strFile, strLog);
			//strValue = (*checkinfo).MEMORY_USE;
			//cout << strValue << endl;
			//strLog.Format("MEMORY_USE:	%s",strValue);
			//WriteLog(strFile, strLog);
			//strValue = (*checkinfo).MEMORY_SUM;
			//cout << strValue << endl;
			//strLog.Format("MEMORY_SUM:	%s",strValue);
			//WriteLog(strFile, strLog);
			//strValue = (*checkinfo).MEMORY_RELAX;
			//cout << strValue << endl;
			//strLog.Format("MEMORY_RELAX:	%s",strValue);
			//WriteLog(strFile, strLog);			

			/*strReturnIP = (* checkinfo).CS_IP;
			strValue = (* checkinfo).CPU_SCALE;
			nCpuUtilization = atoi( strValue );
			strValue = (* checkinfo).MEMORY_RELAX;
			nMemoryRelax = atoi( strValue );
			strValue = (* checkinfo).MEMORY_SUM ;
			nMemorySum = atoi( strValue);
			strValue = (* checkinfo).MEMORY_USE;
			nMemoryUse = atoi( strValue );	*/

			cout << "nLen:" << nLen << endl;
			cout << "strReturnIP:" << strReturnIP <<endl;
			cout << "strDate:" << strDate << endl;
			cout << "nCpuUtilization:" << nCpuUtilization <<endl;
			cout << "nMemorySum:" << nMemorySum <<endl;
			cout << "nMemoryUse:" << nMemoryUse << endl;
			cout << "nMemoryRelax:" << nMemoryRelax << endl;
			
			cout << "-----------------------------------------------" << endl;

			sprintf(strReturn ,"checkDate=%s$cpuUtilization=%d$memorySum=%d$memoryUse=%d$memoryRelax=%d$$",
					strDate,nCpuUtilization,nMemorySum,nMemoryUse,nMemoryRelax);
			
		} 
		catch(_com_error err)
		{
			cout << " _com_error 异常" << endl; 
			char buf[200] = {0};
			IErrorInfo * ei = err.ErrorInfo();
			BSTR strDesEI;
			ei->GetDescription(&strDesEI);
			sprintf(strReturn, "error=Error ocured:%x: %s", (unsigned)err.Error(),_com_util::ConvertBSTRToString(strDesEI));

			cout << strReturn << endl;
			CoUninitialize();
			WriteLog(strFile, "----------------  _com_error 异常 ------------");
			return FALSE;
		}
		catch(...)
		{
			cout << " 程序异常" << endl;
			sprintf(strReturn, "error=Error ocured: %d",::GetLastError());
			cout << strReturn << endl;
			CoUninitialize();
			WriteLog(strFile,"----------------- 程序异常 ------------------");
			return FALSE;
		}


		CString strOutRet;
		strOutRet =strReturn;
		//nSize = 2048;
		MakeCharByString(strReturn,nSize,strOutRet);
	}	
	
	CoUninitialize();
	
WriteLog(strFile, "------------------- GetCheckInfo End -----------------");
	
	return TRUE;
}


