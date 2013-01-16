// IEEvents.cpp : implementation file
//

#include "stdafx.h"
#include "monitor.h"
#include "IEEvents.h"

#include "..\..\base\funcGeneral.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIEEvents

IMPLEMENT_DYNCREATE(CIEEvents, CCmdTarget)

CIEEvents::CIEEvents()
{
	EnableAutomation();
}

CIEEvents::CIEEvents(int nID, char *szReturn, CString strMatchContent)
{
	m_nID = nID;
	m_szReturn = szReturn;
	m_strMatchContent = strMatchContent;

	EnableAutomation();
}

CIEEvents::~CIEEvents()
{
}


void CIEEvents::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CIEEvents, CCmdTarget)
	//{{AFX_MSG_MAP(CIEEvents)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CIEEvents, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CIEEvents)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	DISP_FUNCTION_ID(CIEEvents, "BeforeNavigate2", DISPID_BEFORENAVIGATE2,
                    OnBeforeNavigate2, VT_EMPTY, VTS_DISPATCH VTS_PVARIANT
                    VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)

	DISP_FUNCTION_ID(CIEEvents, "CommandStateChange", DISPID_COMMANDSTATECHANGE,
                    OnCommandStateChange, VT_EMPTY, VTS_I4 VTS_BOOL)

	DISP_FUNCTION_ID(CIEEvents, "DocumentComplete", DISPID_DOCUMENTCOMPLETE,
                    OnDocumentComplete, VT_EMPTY, VTS_DISPATCH VTS_PVARIANT)

	DISP_FUNCTION_ID(CIEEvents, "DownloadComplete", DISPID_DOWNLOADCOMPLETE,
                    OnDownloadComplete, VT_EMPTY, VTS_NONE)

	DISP_FUNCTION_ID(CIEEvents, "NavigateComplete2", DISPID_NAVIGATECOMPLETE2,
                    OnNavigateComplete2, VT_EMPTY, VTS_DISPATCH VTS_PVARIANT)

	DISP_FUNCTION_ID(CIEEvents, "NewWindow2", DISPID_NEWWINDOW2, 
                    OnNewWindow2, VT_EMPTY, VTS_DISPATCH VTS_PBOOL)

	DISP_FUNCTION_ID(CIEEvents, "OnQuit", DISPID_ONQUIT, OnQuit, VT_EMPTY, VTS_NONE)

	DISP_FUNCTION_ID(CIEEvents, "ProgressChange", DISPID_PROGRESSCHANGE,
                    OnProgressChange, VT_EMPTY, VTS_I4 VTS_I4)

	DISP_FUNCTION_ID(CIEEvents, "StatusTextChange", DISPID_STATUSTEXTCHANGE,
                    OnStatusTextChange, VT_EMPTY, VTS_BSTR)

	DISP_FUNCTION_ID(CIEEvents, "TitleChange", DISPID_TITLECHANGE,
                    OnTitleChange, VT_EMPTY, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IIEEvents to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {E64FCC7E-00D4-42BA-A31A-23E0813FD733}
static const IID IID_IIEEvents =
{ 0xe64fcc7e, 0xd4, 0x42ba, { 0xa3, 0x1a, 0x23, 0xe0, 0x81, 0x3f, 0xd7, 0x33 } };

BEGIN_INTERFACE_MAP(CIEEvents, CCmdTarget)
	INTERFACE_PART(CIEEvents, DIID_DWebBrowserEvents2, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIEEvents message handlers
void CIEEvents::OnBeforeNavigate2(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel)
{
	TRACE("OnBeforeNavigate2\r\n");
}

void CIEEvents::OnCommandStateChange(long lCommand, BOOL bEnable)
{
	TRACE("OnCommandStateChange\r\n");
}

void CIEEvents::OnDocumentComplete(LPDISPATCH pDisp, VARIANT* URL)
{
	TRACE("OnDocumentComplete\r\n");

	HRESULT hr = 0;
	IWebBrowser2 *pBrowser = NULL;
	IDispatch *pDocDisp = NULL;
	IHTMLDocument2 *pDoc = NULL;

	if(SUCCEEDED(hr = pDisp->QueryInterface(IID_IWebBrowser2, (void **)&pBrowser)))
	{
		if(SUCCEEDED(hr = pBrowser->get_Document(&pDocDisp)))
		{
			if(SUCCEEDED(hr = pDocDisp->QueryInterface(IID_IHTMLDocument2, (void **)&pDoc)))
			{
				IPersistFile * pPF = NULL;
				if(SUCCEEDED(hr = pDoc->QueryInterface(IID_IPersistFile, (void**)&pPF)))
				{
					CString strFile = _T("");
					strFile.Format("%s\\doc\\aim\\exec\\jre\\bin\\%d.html", FuncGetInstallPath(), m_nID);
					if (SUCCEEDED (hr = pPF->Save(strFile.AllocSysString(), FALSE)))
					{
						CStdioFile sFile;
						CFileException ex;
						if(!sFile.Open(strFile, CFile::modeRead, &ex))
							sprintf(m_szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_IEEvents_1%>"));//<%IDS_IEEvents_1%>
						
						char szBuffer[100 * 1024] = {0};
//						sFile.ReadHuge(szBuffer, sizeof(szBuffer));
						if(!strstr(szBuffer, m_strMatchContent))
							sprintf(m_szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_IEEvents_2%>"));//<%IDS_IEEvents_2%>

						sFile.Close();
					}
					else
					{
						sprintf(m_szReturn, "%s", "status=-1$");
					}

					pPF->Release();
				}
				else
				{
					sprintf(m_szReturn, "%s", "status=-1$");
				}

				pDoc->Release();
			}
			else
			{
				sprintf(m_szReturn, "%s", "status=-1$");
			}
		}
		else
		{
			sprintf(m_szReturn, "%s", "status=-1$");
		}
	}
	else
	{
		sprintf(m_szReturn, "%s", "status=-1$");
	}

	PostQuitMessage(1);
}

void CIEEvents::OnDownloadComplete() 
{
	TRACE("OnDownloadComplete\r\n");
}

void CIEEvents::OnNavigateComplete2(LPDISPATCH pDisp, VARIANT* URL)
{
	TRACE("OnNavigateComplete2\r\n");
}

void CIEEvents::OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel)
{
	TRACE("OnNewWindow2\r\n");
}

void CIEEvents::OnProgressChange(long lProgress, long lProgressMax)
{
	TRACE("OnProgressChange\r\n");
}

void CIEEvents::OnStatusTextChange(LPCTSTR lpszText) 
{
	TRACE("OnStatusTextChange\r\n");
}

void CIEEvents::OnTitleChange(LPCTSTR lpszText) 
{
	TRACE("OnTitleChange\r\n");
}

void CIEEvents::OnQuit()
{
	TRACE("OnQuit\r\n");
}

CString CIEEvents::FuncGetInstallPath()
{
	CString strRet = _T("");
	HKEY hKey = NULL;
	LONG lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,KEY_PATH,0,KEY_READ,&hKey);
	if(lRet != ERROR_SUCCESS)
		return strRet;
	
	LPTSTR  lpName = NULL,
 			lpValue = NULL;
	CString strSubKey = _T(""),
 			strValKey = _T("");
	DWORD   dwIndex = 0,
			dwS1 = 255,
			dwS2 = 255,
			dwType = REG_SZ;
	BOOL    bFindFlag = FALSE;
	char szAppRoot[255] = {0};
	while(lRet == ERROR_SUCCESS)
	{
		lpName = strSubKey.GetBuffer(dwS1);
		lpValue = strValKey.GetBuffer(dwS2);
		lRet = RegEnumValue(hKey, dwIndex++, lpName , &dwS1,NULL, &dwType ,(LPBYTE)lpValue, &dwS2);
		dwS1 = 255;
		dwS2 = 255;
		if(lRet == ERROR_NO_MORE_ITEMS)
			break;
		if(strSubKey.CompareNoCase(KEY_NAME) == 0)
		{
			bFindFlag = TRUE;
			strcpy(szAppRoot,strValKey);
			break;
		}
	}
	RegCloseKey(hKey);
	if(bFindFlag)
	{
		strRet.Format("%s",szAppRoot);
		strRet.Replace("/", "\\");
	}
	return strRet;
}