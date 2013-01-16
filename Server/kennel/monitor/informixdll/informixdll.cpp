// InformixDll.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "InformixDll.h"
//#include <it.h>
//#include <iostream.h>
//#include "..\..\base\funcGeneral.h"
#include "base\funcGeneral.h"
#include "ado.h"

#include <fstream>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
// CInformixDllApp

BEGIN_MESSAGE_MAP(CInformixDllApp, CWinApp)
	//{{AFX_MSG_MAP(CInformixDllApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInformixDllApp construction

CInformixDllApp::CInformixDllApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CInformixDllApp object

CInformixDllApp theApp;

CTime StrToDate(CString sDateTime);

int PrintLog(const char * strPrint)
{
//#ifndef __DEBUG
//	return 1;
//#endif
	char timebuf[128]={0},datebuf[128]={0},tempbuf[10240]={0};
	_strtime(timebuf);
	_strdate(datebuf);
	sprintf(tempbuf,"%s-%s",datebuf,timebuf);
	ofstream FileStream;
	FileStream.open("informixmonitor.log",ios::app);
	FileStream<<tempbuf<<"\t"<<strPrint<<endl;
	FileStream.close();
	return 0;
}

//���� lish
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
	szFileName.Format("%s.log",strFileName);

	ofstream stFileStream;
	stFileStream.open(szFileName, ios::app);
	stFileStream<<szTempBuf<<"\t"<<strLog<<endl;
	stFileStream.close();
	return true;
}

//----------------------------------------------��д���ܼ��-----------------------------------------------
extern "C" __declspec(dllexport)
BOOL GetInformixInfo(const char * strParas, char * szReturn, int& nSize)
//(CStringList& paramList, char* szReturn)
{

	//////////////////////////////////////////////////////////////////////////
	//lish
	//int nCount = 0;
	//CString strFileName = _T("InformixInfo");
	//CString strLog=_T("Call GetInformixInfo! Input params:");
	//MakeStringByChar(strLog, strParas);
	//WriteLog(strFileName,strLog);
	//////////////////////////////////////////////////////////////////////////

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strDSN = _T("");
	CString strUID = _T("");
	CString strPWD = _T("");
	CString strmid = _T("");
	

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);

	POSITION pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);

		if(strTemp.Find("_DSN=") == 0)
		{
			strDSN = strTemp.Right(strTemp.GetLength() - (int)strlen("_DSN="));
		}

		else if(strTemp.Find("_MonitorID=") == 0)
		{
			strmid = strTemp.Right(strTemp.GetLength() - (int)strlen("_MonitorID="));
		}
		else if(strTemp.Find("_UID=") == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - (int)strlen("_UID="));
		}

		else if(strTemp.Find("_PassWord=") == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - (int)strlen("_PassWord="));
		}
	}

	if (strDSN == _T(""))
    {
        sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
            "INFORMIX_DSN_IS_NULL"));
		return FALSE;
    }

    if (strUID == _T(""))
	{
	    sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
            "INFORMIX_UID_IS_NULL"));
		return FALSE;
	}


	CADODatabase db;
	CString strConn = _T("");
	CString strSql = _T("");
	
	long reads = 0;//������
	long pagesread = 0;//��ҳ��
	long writes = 0;//д����
	long pageswritten = 0;//дҳ��

	long Totalreads = 0;//������
	long Totalpagesread = 0;//��ҳ��
	long Totalwrites = 0;//д����
	long Totalpageswritten = 0;//дҳ��

	
	try{
		strConn.Format("DSN=%s;UID=%s;PWD=%s", strDSN, strUID, strPWD);
		db.Open(strConn);
		if (!db.IsOpen())
		{
			sprintf(szReturn, "error=cannot connect!$");
			return FALSE;
		}

		CADORecordset rs(&db);

		strSql.Format("select * from sysmaster:syschkio;");
		rs.Open(strSql.GetBuffer(0));
		
		/* Remark By zjw 2007.8.16
		if (rs.IsEof())
		{
            sprintf(szReturn, "error=%s(%S)$", FuncGetStringFromIDS("SV_INFORMIX",
                "INFORMIX_EXECUTE_FAILED"),strSql);
			return FALSE;
		}
		*/
		
		while(!rs.IsEof())
		{ 
		
			if (!rs.GetFieldValue(_T("reads"), reads))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}

			if (!rs.GetFieldValue(_T("pagesread"), pagesread))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}

			if (!rs.GetFieldValue(_T("writes"), writes))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}

			if (!rs.GetFieldValue(_T("pageswritten"), pageswritten))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}
			
			/*if (!rs.GetFieldValue(0,reads));
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
					"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}

			if (!rs.GetFieldValue(1, pagesread))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
					"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}

			if (!rs.GetFieldValue(2, writes))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
					"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}

			if (!rs.GetFieldValue(3, pageswritten))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
					"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}*/

			rs.MoveNext();

			Totalreads += reads;
			Totalpagesread += pagesread;
			Totalwrites += writes;
			Totalpageswritten += pageswritten;
			
			
			//////////////////////////////////////////////////////////////////////////
			//lish
			//nCount ++;
			//strLog.Format("reads=%d, pagesread =%d, writes=%d, pageswritten=%d, nCount=%d",reads,pagesread,writes,pageswritten,nCount );
			//printf("%s\n",strLog);
			//////////////////////////////////////////////////////////////////////////
		}

		reads = Totalreads;//������
		pagesread = Totalpagesread;//��ҳ��
		writes = Totalwrites;//д����
		pageswritten = Totalpageswritten;//дҳ��
		
		//////////////////////////////////////////////////////////////////////////
		//lish
		//strLog.Format("reads=%d, pagesread =%d, writes=%d, pageswritten=%d",reads,pagesread,writes,pageswritten);
		//WriteLog(strFileName,strLog);
		//////////////////////////////////////////////////////////////////////////

		rs.Close();
		db.Close();

		

		char	filename[256] = {0};
		char	tmp[256] = {0};
		sprintf(filename, "informixfile.ini");

		long reads0 = 0;//������
		long pagesread0 = 0;//��ҳ��
		long writes0 = 0;//д����
		long pageswritten0 = 0;//дҳ��

		CTime curTime = CTime::GetCurrentTime();
		CString strCurTime = curTime.Format("%Y-%m-%d %H:%M:%S");

		reads0 = DFN_GetPrivateProfileInt(strmid, "reads", -1, filename);
		pagesread0 = DFN_GetPrivateProfileInt(strmid, "pagesread", -1, filename);
		writes0 = DFN_GetPrivateProfileInt(strmid, "writes", -1, filename);
		pageswritten0 = DFN_GetPrivateProfileInt(strmid, "pageswritten", -1, filename);

		char oldTime[512] = {0};
		::DFN_GetPrivateProfileString(strmid, "lasttime", "", oldTime, sizeof(oldTime) - 1, filename);

		DFN_WritePrivateProfileString(strmid, "reads", _ltoa(reads, tmp, 10), filename);
		DFN_WritePrivateProfileString(strmid, "pagesread", _ltoa(pagesread, tmp, 10), filename);
		DFN_WritePrivateProfileString(strmid, "writes", _ltoa(writes, tmp, 10), filename);
		DFN_WritePrivateProfileString(strmid, "pageswritten", _ltoa(pageswritten, tmp, 10), filename);
		DFN_WritePrivateProfileString(strmid, "lasttime", strCurTime, filename);

		CString strOldTime = oldTime;

		CTime tOldTime = CTime::GetCurrentTime();
		
		if(strOldTime.GetLength() > 5)
			tOldTime = StrToDate(strOldTime);

		CTimeSpan tTimeSpan = curTime- tOldTime;
		int nMinutes = tTimeSpan.GetMinutes();

		//////////////////////////////////////////////////////////////////////////
		//lish
		//strLog.Format("reads0=%d, pagesread0 =%d, writes0=%d, pageswritten0=%d, nMinutes=%d, nCount=%d",reads0,pagesread0,writes0,pageswritten0,nMinutes,nCount);
		//WriteLog(strFileName,strLog);
		//////////////////////////////////////////////////////////////////////////

		if(nMinutes<1)
			nMinutes = 1;

		//�����ֵ 
		if(reads0 > 0)
			reads = reads - reads0;
		else
			reads = 0;
		if(pagesread0 > 0)
			pagesread = pagesread - pagesread0;
		else
			pagesread = 0;
		if(writes0 > 0)
			writes = writes - writes0;
		else
			writes = 0;
		if(pageswritten0>0)
			pageswritten = pageswritten - pageswritten0;
		else
			pageswritten = 0;

		//��ƽ����ֵ
		reads = reads/nMinutes;
		pagesread = pagesread/nMinutes;
		writes = writes/nMinutes;
		pageswritten = pageswritten/nMinutes;


		sprintf(szReturn, "reads=%d$pagesread=%d$writes=%d$pageswritten=%d$", reads,pagesread,writes, pageswritten);
		CString strInput;
		strInput =szReturn;
		MakeCharByString(szReturn,nSize,strInput);
		return TRUE;
	}
	catch (_com_error e) 
	{
		sprintf(szReturn, "error=%s$", (LPCTSTR)e.Description());
		return FALSE;
	}
}

//---------------------------------------------����������------------------------------------------------------
extern "C" __declspec(dllexport)
BOOL GetConnect(const char * strParas, char * szReturn, int& nSize)
//(CStringList& paramList, char* szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strDSN = _T("");
	CString strUID = _T("");
	CString strPWD = _T("");
	CString strmid = _T("");

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);
	POSITION pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);

		if(strTemp.Find("_DSN=") == 0)
		{
			strDSN = strTemp.Right(strTemp.GetLength() - (int)strlen("_DSN="));
		}

		else if(strTemp.Find("_MonitorID=") == 0)
		{
			strmid = strTemp.Right(strTemp.GetLength() - (int)strlen("_MonitorID="));
		}
		else if(strTemp.Find("_UID=") == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - (int)strlen("_UID="));
		}

		else if(strTemp.Find("_PassWord=") == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - (int)strlen("_PassWord="));
		}
	}

	if (strDSN == _T(""))
    {
        sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
            "INFORMIX_DSN_IS_NULL"));
		return FALSE;
    }

    if (strUID == _T(""))
	{
	    sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
            "INFORMIX_UID_IS_NULL"));
		return FALSE;
	}

	CADODatabase db;
	CString strConn = _T("");
	CString strSql = _T("");
	
	long sid;//�Ի���
	CString username  = "";//�û���
	long uid;//�û�unix�� 
	long pid;//�û����̺�
	CString hostname;//������
	CString tty;//tty�˿�
	long connected  = 0;//�û�����ʱ��
	CString feprogram;//������
	long pooladdr  = 0;//ר�жԻ���ָ��

	long is_wlatch = 0;//��־1=YES��0=NO���ȴ�����
	long is_wlock = 0;//��־1=YES��0=NO���ȴ���
	long is_wbuff = 0;//��־1=YES��0=NO���ȴ�������
	long is_wckpt = 0;//��־1=YES��0=NO���ȴ�У���
	long is_wlogbuf = 0;//��־1=YES��0=NO���ȴ���־������
	long is_wtrans = 0;//��־1=YES��0=NO���ȴ�����
	long is_monitor = 0;//��־1=YES��0=NO�����ӽ���
	long is_incrit = 0;//��־1=YES��0=NO���ڹؼ�����

	long state  = 0;//��־ 

	//����ͳ��
	int nPerNormal = 0;//ͨ����
	int nPerWait = 0;//�ȴ���

	int	nconnect = 0;//������
	int nwlatch = 0;//�ȴ�������
	int	nwlock = 0;//�ȴ�����
	int nwbuff = 0;//�ȴ���������
	int nwckpt = 0;//�ȴ�У�����
	int nwlogbuf = 0;//�ȴ���־��������
	int nwtrans = 0;//�ȴ�������
	int nmonitor = 0;//���ӽ�����
	int nincrit = 0;//�ڹؼ�������

	try
	{

		strConn.Format("DSN=%s;UID=%s;PWD=%s", strDSN, strUID, strPWD);
		db.Open(strConn);
		if (!db.IsOpen())
		{
			sprintf(szReturn, "error=cannot connect!$");
			return FALSE;
		}

		CADORecordset rs(&db);

		//syseswts �ȴ�ʱ��
		//dbsname, tabname,rowidlk,keynum,type,owner,waiter
		strSql.Format("select * from sysmaster:syssessions;");
		rs.Open(strSql.GetBuffer(0));

		/* Remark By zjw 2007.8.16
		if (rs.IsEof())
		{
            sprintf(szReturn, "error=%s(%S)$", FuncGetStringFromIDS("SV_INFORMIX",
                "INFORMIX_EXECUTE_FAILED"),strSql);
			return FALSE;
		}
		*/

		while(!rs.IsEof())
		{
			if (!rs.GetFieldValue(0, sid))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}

			if (!rs.GetFieldValue(1, username))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}

			if (!rs.GetFieldValue(2, uid))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}

			if (!rs.GetFieldValue(3, pid))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}

			if (!rs.GetFieldValue(4, hostname))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}
			if (!rs.GetFieldValue(5, tty))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}
			if (!rs.GetFieldValue(6, connected))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}
			if (!rs.GetFieldValue(7, feprogram))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}

			/*if (!rs.GetFieldValue(8, pooladdr))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}
			if (!rs.GetFieldValue(8,dPool));
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
					"INFORMIX_GET_LOG_FAILED"));
				sprintf(szReturn,"error=�޷���ȡpooladdr��ֵ");
				return FALSE;
			}*/

			if (!rs.GetFieldValue(9, is_wlatch))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}
			if (!rs.GetFieldValue(10, is_wlock))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}
			if (!rs.GetFieldValue(11, is_wbuff))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}
			if (!rs.GetFieldValue(12, is_wckpt))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}
			if (!rs.GetFieldValue(13, is_wlogbuf))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}
			if (!rs.GetFieldValue(14, is_wtrans))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}
			if (!rs.GetFieldValue(15, is_monitor))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}
			if (!rs.GetFieldValue(16, is_incrit))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}
			if (!rs.GetFieldValue(17, state))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}


			/*if (!rs.GetFieldValue(_T("sid"), sid))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
					"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}

			if (!rs.GetFieldValue(_T("username"), username))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
					"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}

			if (!rs.GetFieldValue(_T("uid"), uid))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
					"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}

			if (!rs.GetFieldValue(_T("pid"), pid))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
					"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}

			if (!rs.GetFieldValue(_T("hostname"), hostname))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
					"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}
			if (!rs.GetFieldValue(_T("tty"), tty))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
					"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}
			if (!rs.GetFieldValue(_T("connected"), connected))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
					"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}
			if (!rs.GetFieldValue(_T("feprogram"), feprogram))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
					"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}
			if (!rs.GetFieldValue(_T("pooladdr"), pooladdr))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
					"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}

			if (!rs.GetFieldValue(_T("is_wlatch"), is_wlatch))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
					"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}
			if (!rs.GetFieldValue(_T("is_wlock"), is_wlock))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
					"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}
			if (!rs.GetFieldValue(_T("is_wbuff"), is_wbuff))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
					"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}
			if (!rs.GetFieldValue(_T("is_wckpt"), is_wckpt))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
					"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}
			if (!rs.GetFieldValue(_T("is_wlogbuf"), is_wlogbuf))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
					"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}
			if (!rs.GetFieldValue(_T("is_wtrans"), is_wtrans))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
					"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}
			if (!rs.GetFieldValue(_T("is_monitor"), is_monitor))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
					"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}
			if (!rs.GetFieldValue(_T("is_incrit"), is_incrit))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
					"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}
			if (!rs.GetFieldValue(_T("state"), state))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
					"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}*/
			
			//ͳ����
			//�ȴ�������
			if(is_wlatch)
				nwlatch++;
			//�ȴ�����
			if(is_wlock)
				nwlock++;
			//�ȴ�������
			if(is_wbuff)
				nwbuff++;
			//�ȴ�У���
			if(is_wckpt)
				nwckpt++;
			//�ȴ���־������
			if(is_wlogbuf)
				nwlogbuf++;
			//�ȴ�����
			if(is_wtrans)
				nwtrans++;
			//���ӽ���
			if(is_monitor)
				nmonitor++;
			//�ڹؼ�����
			if(is_incrit)
				nincrit++;

			//������
			nconnect++;
			
			//////////////////////////////////////////////////////////////////////////
			//lish
			printf("nwlatch=%d, nwlock=%d, nwbuff=%d, nwckpt=%d, nwlogbuf=%d, nwtrans=%d, nconnect=%d\n",
				nwlatch,nwlock,nwbuff,nwckpt,nwlogbuf,nwtrans,nconnect);
			//////////////////////////////////////////////////////////////////////////
			rs.MoveNext();
		}

		//����ͨ���ʺ͵ȴ���
		if(nconnect > 0)
		{
			//�ȴ���
			nPerWait = (nwlatch+nwlock+nwbuff+nwckpt+nwlogbuf+nwtrans);
			nPerWait = (nPerWait*100)/nconnect;
			
			//ͨ����
			nPerNormal = 100 - nPerWait;
		}
		else
		{
			nPerWait = 0;
			nPerNormal = 100;
		}
		

		rs.Close();
		db.Close();

		sprintf(szReturn, "nPerWait=%d$nPerNormal=%d$nconnect=%d$nwlatch=%d$nwlock=%d$nwbuff=%d$nwckpt=%d$nwlogbuf=%d$nwtrans=%d$nmonitor=%d$nincrit=%d$"
			, nPerWait,nPerNormal,nconnect,nwlatch	,nwlock, nwbuff, nwckpt, nwlogbuf, nwtrans,nmonitor,nincrit);
		CString strInput;
		strInput =szReturn;
		MakeCharByString(szReturn,nSize,strInput);
	}
	catch (_com_error e) 
	{
		sprintf(szReturn, "error=%s$", (LPCTSTR)e.Description());
		return FALSE;
	}

	return TRUE;
}

//--------------------------------------�����(�����)-------------------------------------------------
extern "C" __declspec(dllexport)
BOOL GetLock(const char * strParas, char * szReturn, int& nSize)
//(CStringList& paramList, char* szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strDSN = _T("");
	CString strUID = _T("");
	CString strPWD = _T("");
	CString strmid = _T("");

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);
	POSITION pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);

		if(strTemp.Find("_DSN=") == 0)
		{
			strDSN = strTemp.Right(strTemp.GetLength() - (int)strlen("_DSN="));
		}

		else if(strTemp.Find("_MonitorID=") == 0)
		{
			strmid = strTemp.Right(strTemp.GetLength() - (int)strlen("_MonitorID="));
		}
		else if(strTemp.Find("_UID=") == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - (int)strlen("_UID="));
		}

		else if(strTemp.Find("_PassWord=") == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - (int)strlen("_PassWord="));
		}
	}

	if (strDSN == _T(""))
    {
        sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
            "INFORMIX_DSN_IS_NULL"));
		return FALSE;
    }

    if (strUID == _T(""))
	{
	    sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
            "INFORMIX_UID_IS_NULL"));
		return FALSE;
	}

	CADODatabase db;
	CString strConn = _T("");
	CString strSql = _T("");
	
	//������
	int nLock = 0;
	//������
	int nDeadLock = 0;
	//������
	int nPerDeadLock = 0;

	int B_Lock   = 0;// ������������>byte lock(�ֽ���) 
	int IS_Lock  = 0;// ������������>intent shared lock(��ͼ������) 
	int S_Lock   = 0;// ������������>shared lock(������) 
	int XS_Lock  = 0;// ������������>repeatable read shared key(���ظ���������) 
	int U_Lock   = 0;// ������������>update lock(������) 
	int IX_Lock  = 0;// ������������>intent exclusive lock(��ͼ��ռ��) 
	int SIX_Lock = 0;// ������������>shared intent exclusive(������ͼ��ռ��) 
	int X_Lock   = 0;// ������������>exclusive lock(��ռ��) 
	int XR_Lock  = 0;// ������������>repreatable read exclusive(���ظ�����ռ��) 

	CString dbsname = "";//���ݿ�
	CString tabname = "";//����
	long rowidlk    = 0;//�����ؼ��������к�
	long keynum     = 0;//�����ؼ������Ĺؼ��ֺ�
	CString type    = "";//������
	long owner      = 0;//��ӵ���߶Ի�ID
	CString waiter  = "";//��һ���ȴ��߶Ի�ID

	try{

		strConn.Format("DSN=%s;UID=%s;PWD=%s", strDSN, strUID, strPWD);
		db.Open(strConn);
		if (!db.IsOpen())
		{
			sprintf(szReturn, "error=cannot connect!$");
			return FALSE;
		}

		CADORecordset rs(&db);

		//syseswts �ȴ�ʱ��
		//dbsname, tabname,rowidlk,keynum,type,owner,waiter
		strSql.Format("select * from sysmaster:syslocks;");
		rs.Open(strSql.GetBuffer(0));
		
		/* Remark By zjw 2007.8.16
		if (rs.IsEof())
		{
            sprintf(szReturn, "error=%s(%S)$", FuncGetStringFromIDS("SV_INFORMIX",
                "INFORMIX_EXECUTE_FAILED"),strSql);
			return FALSE;
		}
		*/

		while(!rs.IsEof())
		{
			if (!rs.GetFieldValue(0, dbsname))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}

			if (!rs.GetFieldValue(1, tabname))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}

			if (!rs.GetFieldValue(2, rowidlk))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}

			if (!rs.GetFieldValue(3, keynum))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}

			if (!rs.GetFieldValue(4, type))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}
			if (!rs.GetFieldValue(5, owner))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}
			if (!rs.GetFieldValue(6, waiter))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}
			
			rs.MoveNext();

			nLock++;

			//������
			if(waiter != "")
				nDeadLock++;

			//������
			type.Replace(" ","");
			if(type == "B")
				B_Lock++;
			else if(type == "IS")
				IS_Lock++;
			else if(type == "S")
				S_Lock++;
			else if(type == "XS")
				XS_Lock++;

			//������
			else if(type == "U")
				U_Lock++;

			//��ռ��
			else if(type == "X")
				X_Lock++;
			else if(type == "IX")
				IX_Lock++;
			else if(type == "SIX")
				SIX_Lock++;
			else if(type == "XR")
				XR_Lock++;

		}

		rs.Close();
		db.Close();

		//���ֹ������Ͷ�ռ�����
		B_Lock = B_Lock+IS_Lock+S_Lock+XS_Lock;
		X_Lock = X_Lock + IX_Lock + SIX_Lock + XR_Lock;

		//������
		if(nLock>0)
			nPerDeadLock = (100*nDeadLock)/nLock;

		sprintf(szReturn, "nPerDeadLock=%d$nLock=%d$B_Lock=%d$U_Lock=%d$X_Lock=%d$nDeadLock=%d$", 
			nPerDeadLock,nLock,B_Lock,U_Lock,X_Lock,nDeadLock);
		CString strInput;
		strInput =szReturn;
		MakeCharByString(szReturn,nSize,strInput);
	}
	catch (_com_error e) 
	{
		sprintf(szReturn, "error=%s$", (LPCTSTR)e.Description());
		CString strInput;
		strInput =szReturn;
		MakeCharByString(szReturn,nSize,strInput);
		return FALSE;
	}

	return TRUE;
}

//----------------------------------------------������ȴ�-----------------------------------------------
extern "C" __declspec(dllexport)
BOOL GetWait(const char * strParas, char * szReturn, int& nSize)
//(CStringList& paramList, char* szReturn)
{
AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strDSN = _T("");
	CString strUID = _T("");
	CString strPWD = _T("");
	CString strmid = _T("");
	CStringList paramList;
	MakeStringListByChar(paramList,strParas);

	POSITION pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);

		if(strTemp.Find("_DSN=") == 0)
		{
			strDSN = strTemp.Right(strTemp.GetLength() - (int)strlen("_DSN="));
		}

		else if(strTemp.Find("_MonitorID=") == 0)
		{
			strmid = strTemp.Right(strTemp.GetLength() - (int)strlen("_MonitorID="));
		}
		else if(strTemp.Find("_UID=") == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - (int)strlen("_UID="));
		}

		else if(strTemp.Find("_PassWord=") == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - (int)strlen("_PassWord="));
		}
	}

	if (strDSN == _T(""))
    {
        sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
            "INFORMIX_DSN_IS_NULL"));
		return FALSE;
    }

    if (strUID == _T(""))
	{
	    sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
            "INFORMIX_UID_IS_NULL"));
		return FALSE;
	}

	CADODatabase db;
	CString strConn = _T("");
	CString strSql = _T("");
	
	long sid = 0;//�Ի�ID
	CString reason = "";//�Ի�ID
	long numwaits = 0;//�ȴ�ԭ��˵��
	long cumtime = 0;//���ԭ��ĵȴ��ۼ�ʱ��
	long maxtime = 0;//���ԭ��ĵȴ��ʱ��

	try{
		strConn.Format("DSN=%s;UID=%s;PWD=%s", strDSN, strUID, strPWD);
		db.Open(strConn);
		if (!db.IsOpen())
		{
			sprintf(szReturn, "error=cannot connect!$");
			return FALSE;
		}

		CADORecordset rs(&db);

		//syseswts �ȴ�ʱ��
		//sid, reason,numwaits,cumtime,maxtime
		strSql.Format("select * from sysmaster:sysseswts;");

		//strSql.Format("select tabname from sysmaster:sysextents;"); //test

		rs.Open(strSql.GetBuffer(0));
		/* Remark By zjw 2007.8.16
		if (rs.IsEof())
		{
            sprintf(szReturn, "error=%s(%S)$", FuncGetStringFromIDS("SV_INFORMIX",
                "INFORMIX_EXECUTE_FAILED"),strSql);
			return FALSE;
		}
		*/

		while(!rs.IsEof())
		{
		
			if (!rs.GetFieldValue(0, sid))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}

			if (!rs.GetFieldValue(1, reason))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}

			if (!rs.GetFieldValue(2, numwaits))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}

			if (!rs.GetFieldValue(3, cumtime))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}

			if (!rs.GetFieldValue(4, maxtime))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
				return FALSE;
			}

			rs.MoveNext();
		}

		rs.Close();
		db.Close();


		
		sprintf(szReturn, "sid=%d$reason=%s$numwaits=%d$cumtime=%d$maxtime=%d$", sid,reason,numwaits, cumtime, maxtime);
		CString strInput;
		strInput =szReturn;
		MakeCharByString(szReturn,nSize,strInput);
		return TRUE;
	}
	catch (_com_error e) 
	{
		sprintf(szReturn, "error=%s$", (LPCTSTR)e.Description());
		return FALSE;
	}
}

//--------------------------------------���ݿ�ռ���---------------------------------------------------------
extern "C" __declspec(dllexport)
BOOL	GetSpace(const char * strParas, char * szReturn, int& nSize)
//(CStringList& paramList, char* szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	/*
	CStringList paramList1;
	MakeStringListByChar(paramList1, strParas);
	POSITION pos1 = paramList1.GetHeadPosition();
	CString str;

	 while(pos1)
	{
		str = paramList1.GetNext(pos1);
		str += " for debug zjw";
		OutputDebugString(str);
	}
	 return true;
	*/

	CString strDSN = _T("");
	CString strUID = _T("");
	CString strPWD = _T("");
	CString strmid = _T("");
	CStringList paramList;
	MakeStringListByChar(paramList,strParas);


	POSITION pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);

		if(strTemp.Find("_DSN=") == 0)
		{
			strDSN = strTemp.Right(strTemp.GetLength() - (int)strlen("_DSN="));
		}

		else if(strTemp.Find("_MonitorID=") == 0)
		{
			strmid = strTemp.Right(strTemp.GetLength() - (int)strlen("_MonitorID="));
		}
		else if(strTemp.Find("_UID=") == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - (int)strlen("_UID="));
		}

		else if(strTemp.Find("_PassWord=") == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - (int)strlen("_PassWord="));
		}
	}

	if (strDSN == _T(""))
    {
        sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
            "INFORMIX_DSN_IS_NULL"));
		return FALSE;
    }

    if (strUID == _T(""))
	{
	    sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
            "INFORMIX_UID_IS_NULL"));
		return FALSE;
	}

	CADODatabase db;
	CString strConn = _T("");
	CString strSql = _T("");
	
	int  nPerDbspace = 0;
	long nLogFileSum = 0;
	long nLogFileSize = 0;
	long nChunks = 0;
	long nChunkTotalSize = 0;
	long nChunkFreeSize = 0;
	long nTabSum = 0;
	long nTabPageSum = 0;

	try{
		strConn.Format("DSN=%s;UID=%s;PWD=%s", strDSN, strUID, strPWD);
		db.Open(strConn);
		if (!db.IsOpen())
		{
			sprintf(szReturn, "error=cannot connect!$");
			return FALSE;
		}

		CADORecordset rs(&db);

		//syslogs �߼���־��ռ�ÿռ�	�ļ�����
		strSql.Format("select count(number), sum(used) from sysmaster:syslogs;");
		rs.Open(strSql.GetBuffer(0));
		if (rs.IsEof())
		{
            sprintf(szReturn, "error=%s(%S)$", FuncGetStringFromIDS("SV_INFORMIX",
                "INFORMIX_EXECUTE_FAILED"),strSql);
			return FALSE;
		}
		if (!rs.GetFieldValue(0, nLogFileSum))
		{
			sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
            "INFORMIX_GET_LOG_FAILED"));
			return FALSE;
		}

		//�߼���־�ļ�������
		if (!rs.GetFieldValue(1, nLogFileSize))
		{
			sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
            "INFORMIX_GET_LOG_FAILED"));
			return FALSE;
		}

		//syschunks	chunks����	chunks�ܴ�С	chunksʣ���ܴ�С		
		strSql.Format("select count(chknum),sum(chksize),sum(nfree) from sysmaster:syschunks;");
		rs.Open(strSql.GetBuffer(0));
		if (rs.IsEof())
		{
			sprintf(szReturn, "error=%s(%S)$", FuncGetStringFromIDS("SV_INFORMIX",
                "INFORMIX_EXECUTE_FAILED"),strSql);
			return FALSE;
		}
		if (!rs.GetFieldValue(0, nChunks))
		{
			sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
            "INFORMIX_GET_SERVER_COUNT_FAILED"));
			return FALSE;
		}

		if (!rs.GetFieldValue(1, nChunkTotalSize))
		{
			sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
            "INFORMIX_GET_SERVER_CONTENT_FAILED"));
			return FALSE;
		}

		if (!rs.GetFieldValue(2, nChunkFreeSize))
		{
			sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
            "INFORMIX_TRANS_SERVER_CONTENT_FAILED"));
			return FALSE;
		}

		//sysextents	���ݿ������ ҳ���� 
		strSql.Format("select count(tabname),sum(size) from sysmaster:sysextents;");
		rs.Open(strSql.GetBuffer(0));
		if (rs.IsEof())
		{
			sprintf(szReturn, "error=%s(%S)$", FuncGetStringFromIDS("SV_INFORMIX",
                "INFORMIX_EXECUTE_FAILED"),strSql);
			return FALSE;
		}
		if (!rs.GetFieldValue(0, nTabSum))
		{
			sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
                "INFORMIX_GET_TABLE_COUNT_FAILED"));
			return FALSE;
		}

		if (!rs.GetFieldValue(1, nTabPageSum))
		{
			sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
                "INFORMIX_TRANS_TABLE_COUNT_FAILED"));
			return FALSE;
		}

		rs.Close();
		db.Close();

		if(nChunkTotalSize>0)
		{
			nPerDbspace = (nChunkTotalSize-nChunkFreeSize)*100;
			nPerDbspace = nPerDbspace/nChunkTotalSize;
		}

		sprintf(szReturn, 
		"PerDbspaceUse=%d$LogFileSum=%d$LogFileSize=%d$ChunkSum=%d$ChunkTotalSize=%d$ChunkFreeSize=%d$TabSum=%d$TabPageSum=%d$", 
		nPerDbspace,nLogFileSum, nLogFileSize, nChunks, nChunkTotalSize, nChunkFreeSize, nTabSum, nTabPageSum);

		CString strInput;
		strInput =szReturn;
		MakeCharByString(szReturn,nSize,strInput);
		return TRUE;
	}
	catch (_com_error e) {
		sprintf(szReturn, "error=%s$", (LPCTSTR)e.Description());
		return FALSE;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

//�Ӻ���
CTime StrToDate(CString sDateTime)
{
	int iYear = 0, iMonth = 0, iDay =0;
	int iHour = 0, iMinute = 0, iSecond =0;
	
	CString sYear,sMonth,sDay;
	CString sHour,sMinute,sSecond;

	int nEnd = 0;

	//Get Year
	nEnd = sDateTime.Find("-");
	sYear = sDateTime.Left(nEnd);
	sDateTime.Delete(0,nEnd+1);

	//Get Month
	nEnd = sDateTime.Find("-");
	sMonth = sDateTime.Left(nEnd);
	sDateTime.Delete(0,nEnd+1);

	//Get Day
	sDay = sDateTime;
	
	//Time
	nEnd = sDateTime.Find(" ");
	sDateTime.Delete(0,nEnd+1);

	//Get Hour
	nEnd = sDateTime.Find(":");
	sHour = sDateTime.Left(nEnd);
	sDateTime.Delete(0,nEnd+1);

	//Get Minute
	nEnd = sDateTime.Find(":");
	sMinute = sDateTime.Left(nEnd);
	sDateTime.Delete(0,nEnd+1);

	//Get Second
	sSecond = sDateTime;

	iYear = atoi(sYear);
	iMonth = atoi(sMonth);
	iDay = atoi(sDay);

	iHour = atoi(sHour);
	iMinute = atoi(sMinute);
	iSecond = atoi(sSecond);

	//Get time
	CTime tTime(iYear,iMonth,iDay,iHour,iMinute,iSecond);

	return tTime;
}

extern "C" __declspec(dllexport)
BOOL TestInformixConn(const char * strParas, char * szReturn , int &nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet=FALSE;

	CString strDSN = _T("");
	CString strUID = _T("");
	CString strPWD = _T("");
	CString strmid = _T("");
	CStringList paramList;
	MakeStringListByChar(paramList,strParas);

	POSITION pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);

		if(strTemp.Find("_DSN=") == 0)
		{
			strDSN = strTemp.Right(strTemp.GetLength() - (int)strlen("_DSN="));
		}

		else if(strTemp.Find("_MonitorID=") == 0)
		{
			strmid = strTemp.Right(strTemp.GetLength() - (int)strlen("_MonitorID="));
		}
		else if(strTemp.Find("_UID=") == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - (int)strlen("_UID="));
		}

		else if(strTemp.Find("_PassWord=") == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - (int)strlen("_PassWord="));
		}
	}

	if (strDSN == _T(""))
    {
        sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
            "INFORMIX_DSN_IS_NULL"));
		return FALSE;
    }

    if (strUID == _T(""))
	{
	    sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
            "INFORMIX_UID_IS_NULL"));
		return FALSE;
	}

	CADODatabase db;
	CString strConn = _T("");
	CString strSql = _T("");
	
	int BeginTime=0,EndTime=0;

	try{
		strConn.Format("DSN=%s;UID=%s;PWD=%s", strDSN, strUID, strPWD);
		BeginTime=GetTickCount();
		db.Open(strConn);
		if (!db.IsOpen())
		{
			sprintf(szReturn, "error=cannot connect!$");
			return FALSE;
		}

		db.Close();
		EndTime=GetTickCount();

		sprintf(szReturn,"status=OK$time(����)=%d$",EndTime-BeginTime);
		bRet = TRUE;

		
	}
	catch (_com_error e) {
		sprintf(szReturn, "error=%s$", (LPCTSTR)e.Description());
		bRet = FALSE;
	}
	catch(...)
	{
		sprintf(szReturn,"error=%s",GetLastError());
		bRet = FALSE;
	}

	CString strInput;
	strInput =szReturn;
	MakeCharByString(szReturn,nSize,strInput);

	return bRet;
}

//yi.duan 2010-12-24 ������
extern "C" __declspec(dllexport)
BOOL	GetTableName(const char * strParas, char * szReturn, int& nSize)
//(CStringList& paramList, char* szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strDSN = _T("");
	CString strUID = _T("");
	CString strPWD = _T("");
	CString strmid = _T("");
	CStringList paramList;
	MakeStringListByChar(paramList,strParas);


	POSITION pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);

		if(strTemp.Find("_DSN=") == 0)
		{
			strDSN = strTemp.Right(strTemp.GetLength() - (int)strlen("_DSN="));
		}

		else if(strTemp.Find("_MonitorID=") == 0)
		{
			strmid = strTemp.Right(strTemp.GetLength() - (int)strlen("_MonitorID="));
		}
		else if(strTemp.Find("_UID=") == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - (int)strlen("_UID="));
		}

		else if(strTemp.Find("_PassWord=") == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - (int)strlen("_PassWord="));
		}
	}

	if (strDSN == _T(""))
	{
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
			"INFORMIX_DSN_IS_NULL"));
		return FALSE;
	}

	if (strUID == _T(""))
	{
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
			"INFORMIX_UID_IS_NULL"));
		return FALSE;
	}

	CADODatabase db;
	CString strConn = _T("");
	CString strSql = _T("");

	
	long    nTables = 0;
	CString strTableName = _T("");

	try{
		strConn.Format("DSN=%s;UID=%s;PWD=%s", strDSN, strUID, strPWD);
		db.Open(strConn);
		if (!db.IsOpen())
		{
			sprintf(szReturn, "error=cannot connect!$");
			return FALSE;
		}

		CADORecordset rs(&db);

		//select tabname from systables where tabid >99 and tabtype="T" �鿴���ݱ����ƣ��û��Խ���
		//�����������ϵͳ��
		strSql.Format("select count(tabname) from systables;");
		rs.Open(strSql.GetBuffer(0));
		if (rs.IsEof())
		{
			sprintf(szReturn, "error=%s(%S)$", FuncGetStringFromIDS("SV_INFORMIX",
				"INFORMIX_EXECUTE_FAILED"),strSql);
			return FALSE;
		}
		if (!rs.GetFieldValue(0, nTables))
		{
			sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_LOG_FAILED"));
			return FALSE;
		}

		if (nTables <= 0 )
		{
			sprintf(szReturn, "���ݿ�û�б�������ָ�����ݿ�");
			return FALSE;
		}

		strSql.Format("select tabname from systables;");
		rs.Open(strSql.GetBuffer(0));
		if (rs.IsEof())
		{
			sprintf(szReturn, "error=%s(%S)$", FuncGetStringFromIDS("SV_INFORMIX",
				"INFORMIX_EXECUTE_FAILED"),strSql);
			return FALSE;
		}

		char *p = szReturn;
		for(int i=0; i<nTables; i++)
		{
			
			if (!rs.GetFieldValue(0,strTableName))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
					"INFORMIX_GET_TABLE_COUNT_FAILED"));
				return FALSE;
			}
		
			printf("strTableName = %s\n", strTableName.GetBuffer(strTableName.GetLength()));

			sprintf(p,"%s=%s",strTableName,strTableName);
			p+= 2* strlen(strTableName)+2;
			rs.MoveNext();
		}
	
		rs.Close();
		db.Close();

		return TRUE;
	}
	catch (_com_error e) {
		sprintf(szReturn, "error=%s$", (LPCTSTR)e.Description());
		return FALSE;
	}
}

//yi.duan 2010-12-24 ��ռ�
extern "C" __declspec(dllexport)
BOOL	GetTableNameSpace(const char * strParas, char * szReturn, int& nSize)
//(CStringList& paramList, char* szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strDSN = _T("");
	CString strUID = _T("");
	CString strPWD = _T("");
	CString strmid = _T("");
	CString strTableName = _T("");

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);


	POSITION pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);

		if(strTemp.Find("_DSN=") == 0)
		{
			strDSN = strTemp.Right(strTemp.GetLength() - (int)strlen("_DSN="));
		}

		else if(strTemp.Find("_MonitorID=") == 0)
		{
			strmid = strTemp.Right(strTemp.GetLength() - (int)strlen("_MonitorID="));
		}
		else if(strTemp.Find("_UID=") == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - (int)strlen("_UID="));
		}
		else if(strTemp.Find("_PassWord=") == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - (int)strlen("_PassWord="));
		}	
		else if(strTemp.Find("_TableNames=") == 0)
		{
			strTableName = strTemp.Right(strTemp.GetLength() - (int)strlen("_TableNames="));
		}
		
	}

	if (strDSN == _T(""))
	{
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
			"INFORMIX_DSN_IS_NULL"));
		return FALSE;
	}

	if (strUID == _T(""))
	{
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
			"INFORMIX_UID_IS_NULL"));
		return FALSE;
	}

	CADODatabase db;
	CString strConn = _T("");
	CString strSql = _T("");


	long    nChunk  = 0;
	long    nOffset = 0;
	long    nChunkSize   = 0;

	try{
		strConn.Format("DSN=%s;UID=%s;PWD=%s", strDSN, strUID, strPWD);
		db.Open(strConn);
		if (!db.IsOpen())
		{
			sprintf(szReturn, "error=cannot connect!$");
			return FALSE;
		}

		CADORecordset rs(&db);

		strSql.Format("select chunk,offset,size from sysmaster:sysextents where tabname='%s';", strTableName);
		rs.Open(strSql.GetBuffer(0));
		if (rs.IsEof())
		{
			sprintf(szReturn, "error=%s(%S)$", FuncGetStringFromIDS("SV_INFORMIX",
				"INFORMIX_EXECUTE_FAILED"),strSql);
			return FALSE;
		}

		if (!rs.GetFieldValue(0, nChunk))
		{
			sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_TABLE_COUNT_FAILED"));
			return FALSE;
		}

		if (!rs.GetFieldValue(1, nOffset))
		{
			sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_TABLE_COUNT_FAILED"));
			return FALSE;
		}

		if (!rs.GetFieldValue(2, nChunkSize))
		{
			sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_INFORMIX", 
				"INFORMIX_GET_TABLE_COUNT_FAILED"));
			return FALSE;
		}

		rs.Close();
		db.Close();

		printf("nChunk=%d,nOffset=%d,nChunkSize=%d\n", nChunk,nOffset,nChunkSize);

		sprintf(szReturn, "nChunk=%d$nOffset=%d$nChunkSize=%d$", nChunk,nOffset,nChunkSize);
	
		CString strInput;
		strInput =szReturn;
		MakeCharByString(szReturn,nSize,strInput);
		
		return TRUE;
	}
	catch (_com_error e) {
		sprintf(szReturn, "error=%s$", (LPCTSTR)e.Description());
		return FALSE;
	}
}

