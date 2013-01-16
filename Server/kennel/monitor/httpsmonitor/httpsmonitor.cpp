// HttpsMonitor.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "HttpsMonitor.h"
#include "W3c.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include ".\base\funcGeneral.h"

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
// CHttpsMonitorApp

BEGIN_MESSAGE_MAP(CHttpsMonitorApp, CWinApp)
	//{{AFX_MSG_MAP(CHttpsMonitorApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHttpsMonitorApp construction

CHttpsMonitorApp::CHttpsMonitorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CHttpsMonitorApp object

CHttpsMonitorApp theApp;

CString QueryRawHeaderStatus(CString strRawHeader);
BSTR UTF2Unicode(LPCSTR lp, int nLen);
void MatchContent(CString strContent, CString strFilePath, char *szReturn);


extern "C" __declspec(dllexport) BOOL HttpsMonitor(const char * strParas, char * szReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	BOOL bRet = FALSE;

	CString strPath = ::FuncGetInstallRootPath();
	if (strPath.IsEmpty())//�õ�Siteview��װ·��ʧ��
		return bRet;

	CString	strURL = _T(""),				// URL
			strPostData = _T(""),			// Post Data
			strUsername = _T(""),			// Authorization User Name
			strPassword = _T(""),			// Authorization Password
			strProxyServer = _T(""),		// HTTP Proxy
			strProxyUsername = _T(""),		// Proxy Server User Name
			strProxyPassword = _T(""),		// Proxy Server Password
			strMatchContent = _T("");		// Match Content

	CString strProxyPort = _T("");
	CString strCustomer = _T("");

	DWORD dwError=0;
	CString strError=_T("");

	DWORD dwStart = ::GetTickCount();

	CString strGID;
	CString strMid;
	//int		nTimeout = 60, 
	//		nGID = 0, 
	//		nMID = 0;
	int		nTimeout = 60;
	CStringList paramList;

	MakeStringListByChar(paramList,strParas);

	POSITION pos = paramList.FindIndex(0);//�õ�StringList�еĵ�һ��Ԫ��
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__URL__, 0) == 0)
		{//URL��ַ
			strURL = strTemp.Right(strTemp.GetLength() - strlen(__URL__));
		}
		else if(strTemp.Find(__POSTDATA__, 0) == 0)
		{//Post Data
			strPostData = strTemp.Right(strTemp.GetLength() - strlen(__POSTDATA__));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{//�û���
			strUsername = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{//����
			strPassword = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{//����������Ͷ˿�
			strTemp.Delete(0, strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{//�����ʹ��ȱʡ�Ķ˿�
				strProxyServer = strTemp.Left(index);//���������
				strTemp.Delete(0, index + 1);
				strProxyPort = strTemp;//����������˿�				
			}
			else
			{//���������
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{//��������������û�
			strProxyUsername = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{//�����������������
			strProxyPassword = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{//��ʱʱ��
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__MATCHSTR__, 0) == 0)
		{//ƥ���ַ���
			strMatchContent = strTemp.Right(strTemp.GetLength() - strlen(__MATCHSTR__));
		}
		else if(strTemp.Find(__GROUPID__, 0) == 0)
		{//Group ID

			//nGID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__GROUPID__)));
			strGID=strTemp.Right(strTemp.GetLength() - strlen(__GROUPID__));
		}
		else if(strTemp.Find(__MONITORID__, 0) == 0)
		{//Monitor ID
			//nMID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__MONITORID__)));
			strMid=strTemp.Right(strTemp.GetLength() - strlen(__MONITORID__));
		}
		else if(strTemp.Find(__CUSTOMER__, 0) == 0)
		{//IDC�û�
			strCustomer = strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMER__));
		}
	}

	if(strURL.IsEmpty()) 
	{//����ҳ��Ϊ��
		sprintf(szReturn, "error=%s",FuncGetStringFromIDS("<%IDS_UrlMonitor_1%>"));	//<%IDS_UrlMonitor_1%>"error=ȱ��URL"
		return bRet;
	}

	CString strTempFile = _T("");//ҳ�����ݱ����ڱ��ص��ļ�·��
	if (strCustomer.IsEmpty())
	{//�������IDC�û�

		//strTempFile.Format("%s\\MonitorManager\\temp\\https-%d-%d.html", (LPCTSTR)strPath, 
          //  nGID, nMID);
		strTempFile.Format("%s\\temp\\https-%s.html", (LPCTSTR)strPath, 
             strMid);
		puts(strTempFile);

	}
	else
	{//IDC�û�
		strTempFile.Format("%s\\%s\\temp\\https-%s-%s.html", (LPCTSTR)strPath, strCustomer, strGID, strMid);
	}
	W3Client *_pclient = new W3Client();//�����µĿͻ���
	
	WriteLog(strTempFile,300,"wp.ini");

	if(_pclient->Connect((const char*)strURL.GetBuffer(0)))
	{//���ӳɹ�		
		if(_pclient->Request(_pclient->GetURI()))
		{//����ҳ��ɹ�
			FILE * fp = NULL;
			//�����ļ������ԭ�ļ��Ѿ�����������
			fp = fopen(strTempFile, "w");
			if (fp) 
			{//�����ļ��ɹ�
				unsigned char buf[1024]="\0";
				//��ȡ����ͷ
				_pclient->QueryRawHeader(buf, 1024);
				//�õ�������
				CString strStatusCode = QueryRawHeaderStatus(buf);
				memset(buf, 0x00, 1024);//��������
				unsigned long ulRead = 1,//��ȡ�ĳ���
					nPageSize =0;//ҳ��Ĵ�С(��λΪbyte)
				while(ulRead)
				{
					ulRead = _pclient->Response(buf, 1024);//��ȡ��������
					nPageSize += ulRead;//���ش�С�ۼ�
					UTF2Unicode((const char*)buf,ulRead);
					fwrite(buf,ulRead,sizeof(char),fp);//д���ݵ��ļ���
					memset(buf, 0x00, 1024);//��������
				}	
				fclose(fp);//�ر��ļ�
				//�õ���ǰʱ��
				DWORD dwEnd = ::GetTickCount();
				sprintf(szReturn,"status=%s$totaltime=%.2f$downloadsize=%d$", 
					strStatusCode, (dwEnd - dwStart)*1.0/1000, nPageSize); 

				if(!strMatchContent.IsEmpty())
				{//���ƥ���ַ�����Ϊ��
					MatchContent(strMatchContent, strTempFile, szReturn);
				}
				CString strResult= szReturn;
				MakeCharByString(szReturn,nSize,strResult);


			}
			else
			{//�����ļ�ʧ��
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("IDS_OpenFileFailed"));//���ļ�ʧ��");
				return FALSE;
			}
		}
		else
		{//��ҳ��ʧ��	
			sprintf(szReturn, "error=%s", _pclient->GetErrorMsg().c_str());
			//sprintf(szReturn, "error=��ҳ��ʧ��$");
			return FALSE;
		}
	}
	else
	{//����ʧ��
		//sprintf(szReturn, "error=%s$", _pclient->GetErrorMsg().c_str());
		sprintf(szReturn, "error=%s",FuncGetStringFromIDS("<%IDS_UrlMonitor_3%>"));//sprintf(szReturn, "error=����ʧ��");

		return FALSE;
	}
	return TRUE;
}
CString QueryRawHeaderStatus(CString strRawHeader)
{
	CString strStatus = _T("");
	strStatus = strRawHeader.Left(strRawHeader.Find("\r\n"));
	strStatus = strStatus.Right(strStatus.GetLength() - 
		strStatus.Find(" ") - 1);
	strStatus = strStatus.Left(strStatus.Find(" "));
	return strStatus;
}

BSTR UTF2Unicode(LPCSTR lp, int nLen)
{
   BSTR str = NULL;
   int nConvertedLen = MultiByteToWideChar(CP_UTF8, 0, lp,
     nLen, NULL, NULL);
 
   // BUG FIX #1 (from Q241857): only subtract 1 from 
   // the length if the source data is nul-terminated
   if (nLen == -1)
      nConvertedLen--;
 
   str = ::SysAllocStringLen(NULL, nConvertedLen);
   if (str != NULL)
   {
     MultiByteToWideChar(CP_UTF8, 0, lp, nLen, str, nConvertedLen);
   }
   return str;
}

void MatchContent(CString strContent, CString strFilePath, char *szReturn)
{
	CFile fileTmp;
	CFileException fileError;
	if(!fileTmp.Open(strFilePath, CFile::modeRead , &fileError))
	{
		sprintf(szReturn, "error=%s",FuncGetStringFromIDS("IDS_OpenFileFailed"));//���ļ�ʧ��");
		return ;
	}
	DWORD dwLength = fileTmp.GetLength();
    if(dwLength == 0)
    {
		//strcpy(szReturn, "error=���ļ�ʧ��");
		return ;
    }
	char *chContent;
	chContent = (char*)malloc(dwLength);
	CString strRead;
	fileTmp.Read(chContent, dwLength);
	fileTmp.Close();
	if (!strstr(chContent, strContent.GetBuffer(strContent.GetLength())))
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_UrlMonitor_2%>"));//<%IDS_UrlMonitor_2%>"error=ƥ��ʧ��"
	delete chContent;
}
