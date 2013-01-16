// LinuxStation.cpp : ���� DLL �ĳ�ʼ�����̡�
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

#define  SPANTIME  5*60; //���ʱ����5����

//
//	ע�⣡
//
//		����� DLL ��̬���ӵ� MFC
//		DLL���Ӵ� DLL ������
//		���� MFC ���κκ����ں�������ǰ��
//		��������� AFX_MANAGE_STATE �ꡣ
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CLinuxStationApp

BEGIN_MESSAGE_MAP(CLinuxStationApp, CWinApp)
END_MESSAGE_MAP()


// CLinuxStationApp ����

CLinuxStationApp::CLinuxStationApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CLinuxStationApp ����

CLinuxStationApp theApp;


// CLinuxStationApp ��ʼ��

BOOL CLinuxStationApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}



//���� lish
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
			//cout << "�ļ����Ѿ�����" << endl;
		}
		else
		{
			BOOL bRet =	CreateDirectory(szFileName,NULL);
			if ( !bRet)
			{
				//cout << "����Ŀ¼ʧ��" << endl;
				return false;
			}
			//cout << "����Ŀ¼�ɹ�" << endl;
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



//�������������һ�� list<string> ��
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
	//printf("%d\n",nSize);23028830 13602067678 ����
	for(int i=0;i<nSize;i++)
	{
		if(*p=='$') 	
			*p='\0';
		p++;
	}
	nOutSize=nSize+1;
	return true;

}

//������ʱ���ַ��� �磨2009-08-03 08:31:12������һ��CTime
//��������ַ������󣬷��ص�ǰʱ��

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
		WriteLog(strFile,"-------------- URL û�ҵ� ----------------");
		return FALSE;
	}

	bRet = AfxParseURL(strURL.GetBuffer(), dwServerType, strServer, strObject, nPort);
	if( !bRet )
	{
		sprintf(strReturn, "error=URL is error");
		WriteLog(strFile,"-------------- URL ���ò���ȷ ----------------");
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
	if (strPreTime.IsEmpty()) //��һ�μ��
	{
		ULONG64 nTime = curTime.GetTime();

		cout << "��ǰʱ�䣺" << nTime << endl;
        
		//���ʱ�䵱ǰʱ����ǰ5����
		nTime -= (60 *5 );
		cout << "�ı�ʱ�䣺" << nTime <<endl;

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
				cout << "SetUrl ����" << endl;
				delete ws;
				sprintf(strReturn,"error=Set url is failed");
				CoUninitialize();

				WriteLog(strFile,"---------------- SetUrl ���� ----------------" );
				return FALSE;
			}
			
			CComBSTR ip(strIP);
			CComBSTR d1(strPreTime);
			CComBSTR d2(strCurTime);
			SiteViewService::LinuxStationCheckInfo* checkinfo;
			int nLen = 0;

			//����WebServer
			hr = ws->get_LinuxStationCheckInfo(ip, d1, d2, &checkinfo, &nLen);
			if (FAILED(hr))
			{
				cout << "get_LinuxStationCheckInfo ����" <<endl;
				delete ws;
				sprintf(strReturn,"error=get_LinuxStationCheckInfo is failed");
				CoUninitialize();
				WriteLog(strFile, "--------------- get_LinuxStationCheckInfo ���� -----------");
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
					WriteLog(strFile, "-------------  ʱ��ת��ʧ�� ------------------");
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
			cout << "---------------  ��ȡ���� -------------------" << endl;

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
			cout << " _com_error �쳣" << endl; 
			char buf[200] = {0};
			IErrorInfo * ei = err.ErrorInfo();
			BSTR strDesEI;
			ei->GetDescription(&strDesEI);
			sprintf(strReturn, "error=Error ocured:%x: %s", (unsigned)err.Error(),_com_util::ConvertBSTRToString(strDesEI));

			cout << strReturn << endl;
			CoUninitialize();
			WriteLog(strFile, "----------------  _com_error �쳣 ------------");
			return FALSE;
		}
		catch(...)
		{
			cout << " �����쳣" << endl;
			sprintf(strReturn, "error=Error ocured: %d",::GetLastError());
			cout << strReturn << endl;
			CoUninitialize();
			WriteLog(strFile,"----------------- �����쳣 ------------------");
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


