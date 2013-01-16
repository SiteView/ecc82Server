
#include "GTJA.h"

#include "base\funcGeneral.h"

BOOL GTJA_MONITOR(CString strURL, CString strPostData, int nTimeout, 
				  CString strMatchContent, int nID, char *szReturn)
{
	DWORD dwBeginTime = 0, dwEndTime = 0;

	CoInitialize(NULL);

	CIEEvents *pIEEvents = new CIEEvents(nID, szReturn, strMatchContent);
	IWebBrowser2 * pIE = NULL;
	HRESULT hr = CoCreateInstance(CLSID_InternetExplorer, 
								NULL, 
								CLSCTX_LOCAL_SERVER, 
								IID_IWebBrowser2, 
								(void**)&pIE);
	if (SUCCEEDED(hr))	
	{
		pIEEvents->m_pIE = pIE;
		pIEEvents->m_pIE->AddRef();
		pIE->Release();

		BOOL bAdvised = AfxConnectionAdvise(pIE, 
											DIID_DWebBrowserEvents2,	
											pIEEvents->GetInterface(&IID_IUnknown), 
											TRUE, 
											&pIEEvents->m_dwCookie);

		strURL += "?" + strPostData;
		VARIANT		vURL;
		vURL.vt = VT_BSTR;
		V_BSTR(&vURL) = strURL.AllocSysString();
		
		//////////////////////////////
		DWORD dwID = GetCurrentThreadId();

		struct tagParam param;
		param.threadId = dwID;
		param.timeOut = nTimeout;

		CWinThread* pThread = AfxBeginThread(AgentThreadProc,(LPVOID)&param,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED,NULL);
		if(pThread == NULL)
		{
			if (pIEEvents->m_dwCookie) 
			{
				BOOL bUnadvised = AfxConnectionUnadvise(pIEEvents->m_pIE, 
														DIID_DWebBrowserEvents2, 
														pIEEvents->GetInterface(&IID_IUnknown), 
														TRUE, pIEEvents->m_dwCookie);
				pIEEvents->m_dwCookie = bUnadvised ? 0: pIEEvents->m_dwCookie;
			}

			pIEEvents->m_pIE->Quit();

			delete pIEEvents;
			pIEEvents = NULL;

			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_GTJA_1%>"));//<%IDS_GTJA_1%>
			return FALSE;
		}

		pThread->m_bAutoDelete= TRUE;
		pThread->ResumeThread();
		Sleep(0);
		/////////////////////////////

		dwBeginTime = ::GetTickCount();

		COleVariant vtEmpty;
		pIEEvents->m_pIE->put_Visible(VARIANT_FALSE);
		pIEEvents->m_pIE->Navigate2(&vURL, &vtEmpty, &vtEmpty, &vtEmpty, &vtEmpty);
	}
	else
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_GTJA_2%>"));//<%IDS_GTJA_2%>
		return FALSE;
	}

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (WM_USER_STARTWALKING == msg.message && NULL == msg.hwnd)
			break;
		else
			DispatchMessage(&msg);
	}

	dwEndTime = ::GetTickCount();

	if (pIEEvents->m_dwCookie) 
	{
		BOOL bUnadvised = AfxConnectionUnadvise(pIEEvents->m_pIE, 
												DIID_DWebBrowserEvents2, 
												pIEEvents->GetInterface(&IID_IUnknown), 
												TRUE, pIEEvents->m_dwCookie);
		pIEEvents->m_dwCookie = bUnadvised ? 0: pIEEvents->m_dwCookie;
	}

	pIEEvents->m_pIE->Quit();

	delete pIEEvents;
	pIEEvents = NULL;

	CoUninitialize();

	sprintf(szReturn, "%sTotalTime=%ld$", szReturn, dwEndTime - dwBeginTime);

	return TRUE;
}

UINT AgentThreadProc( LPVOID pParam )
{
	struct tagParam *param = (struct tagParam *)pParam;

	Sleep(param->timeOut);

	if(!PostThreadMessage(param->threadId , WM_USER_STARTWALKING, (WPARAM)0, (LPARAM)0))
		ExitProcess(1);

	return 0;
}
