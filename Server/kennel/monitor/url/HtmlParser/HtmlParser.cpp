// HtmlParser.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "HtmlParser.h"

#include <mshtml.h>
#include <mshtmdid.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_USER_STARTWALKING	((WM_USER) + 1)
#define ODS(x) OutputDebugString(x)

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <scsvapi\svapi.h>
void WriteLog( const char* str )
{
	puts( str );

	char timebuf[128],datebuf[128];

	_tzset();
	_strtime( timebuf );
	_strdate( datebuf );

	char szProgramName[] = "htmlParser.log";
	string	strEccPath = GetSiteViewRootPath();
	char szLogFile[128];
	sprintf( szLogFile, "%s\\itsm\\runtime\\logs\\%s", 
		     strEccPath.c_str(), szProgramName );

	
	// 判断文件大小：在不打开文件的情况下实现
	struct _stat buf;
	if( _stat( szLogFile, &buf ) == 0 )
	{
		if( buf.st_size > 1000*1024 )
		{
			FILE *log = fopen( szLogFile, "w" );
			if( log != NULL )
				fclose( log );
		}
	}
	


	FILE *log = fopen( szLogFile,"a+");
	if( log != NULL )
	{
		fprintf( log, "%s %s \t%s\n", datebuf, timebuf, str );
		fclose( log );
	}

}

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
// CHtmlParserApp

BEGIN_MESSAGE_MAP(CHtmlParserApp, CWinApp)
	//{{AFX_MSG_MAP(CHtmlParserApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHtmlParserApp construction

CHtmlParserApp::CHtmlParserApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CHtmlParserApp object

CHtmlParserApp theApp;

class CApp : public IPropertyNotifySink, IOleClientSite, IDispatch
{
public:
	CApp() : m_pMSHTML(NULL), m_dwRef(1), 
			m_hrConnected(CONNECT_E_CANNOTCONNECT),
			m_dwCookie(0), m_pCP(NULL), 
			m_lReadyState(READYSTATE_UNINITIALIZED)
			{memset(m_szURL, 0, sizeof(m_szURL));}
    ~CApp()
    {
    }

	HRESULT Passivate()	{
		HRESULT hr = NOERROR;

		// Disconnect from property change notifications
		if (m_pCP)	{
			if (m_dwCookie)	{
				hr = m_pCP->Unadvise(m_dwCookie);
				m_dwCookie = 0;
			}

			// Release the connection point
			m_pCP->Release();
			m_pCP = NULL;
		}		

		if (m_pMSHTML)	{
			m_pMSHTML->Release();
			m_pMSHTML = NULL;
		}

		return NOERROR;
	}


	// IUnknown methods
    STDMETHOD(QueryInterface)(REFIID riid, LPVOID* ppv);
    STDMETHOD_(ULONG, AddRef)();
    STDMETHOD_(ULONG, Release)(); 

	// IPropertyNotifySink methods
    STDMETHOD(OnChanged)(DISPID dispID);
    STDMETHOD(OnRequestEdit)(DISPID dispID);

	// IOleClientSite methods
	STDMETHOD(SaveObject)(void) { ODS("SaveObject\n"); return E_NOTIMPL; }

	STDMETHOD(GetMoniker)(DWORD dwAssign,
            DWORD dwWhichMoniker,
            IMoniker** ppmk)
			{ ODS("GetMoniker\n"); return E_NOTIMPL; }

	STDMETHOD(GetContainer)(IOleContainer** ppContainer)
			{ ODS("GetContainer\n"); return E_NOTIMPL; }

	STDMETHOD(ShowObject)(void)
			{ ODS("ShowObject\n"); return E_NOTIMPL; }

    STDMETHOD(OnShowWindow)(BOOL fShow)
			{ ODS("OnShowWindow\n"); return E_NOTIMPL; }

    STDMETHOD(RequestNewObjectLayout)(void)
			{ ODS("RequestNewObjectLayout\n"); return E_NOTIMPL; }

	// IDispatch method
	STDMETHOD(GetTypeInfoCount)(UINT* pctinfo)
			{ ODS("GetTypeInfoCount\n"); return E_NOTIMPL; }

	STDMETHOD(GetTypeInfo)(UINT iTInfo,
            LCID lcid,
            ITypeInfo** ppTInfo)
			{ ODS("GetTypeInfo\n"); return E_NOTIMPL; }

	STDMETHOD(GetIDsOfNames)(REFIID riid,
            LPOLESTR* rgszNames,
            UINT cNames,
            LCID lcid,
            DISPID* rgDispId)
			{ ODS("GetIDsOfNames\n"); return E_NOTIMPL; }
        
	STDMETHOD(Invoke)(DISPID dispIdMember,
            REFIID riid,
            LCID lcid,
            WORD wFlags,
            DISPPARAMS __RPC_FAR *pDispParams,
            VARIANT __RPC_FAR *pVarResult,
            EXCEPINFO __RPC_FAR *pExcepInfo,
            UINT __RPC_FAR *puArgErr);

	// Additional class methods
	HRESULT Init(int argc, CString strFileName);
	HRESULT Run(CString strMonitorId, CString strStepIndex, CStringList& lstLinks, CStringList& lstForms,  CStringList& lstFormDate, CStringList& lstFrames, CStringList& lstRefreshs);
	HRESULT Term();
	HRESULT Walk(CString strMonitorId, CString strStepIndex, CStringList& lstLinks, CStringList& lstForms,  CStringList& lstFormDate, CStringList& lstFrames, CStringList& lstRefreshs);
	HRESULT GetAnchorElementList(CString strMonitorId, CString strStepIndex, CStringList& listAnchorElement);
//	HRESULT GetFormElementList(CString strStepIndex, CStringList& listFormElement);
	HRESULT GetFrameElementList(CString strMonitorId, CString strStepIndex, CStringList& listFrameElement);
	HRESULT GetRefreshElementList(CString strMonitorId, CString strStepIndex, CStringList& listRefreshElement);
	HRESULT GetFormDateList(CString strMonitorId, CString strStepIndex, CStringList& listFormElement, CStringList& listFormDate);
	CString GetElementAttributeValue(CString attributename ,  IHTMLElement* pElem ) ;

public:
	// Persistence helpers
	HRESULT LoadURLFromFile();
	HRESULT LoadURLFromMoniker();

	IHTMLDocument2* m_pMSHTML;
	DWORD m_dwRef;
	DWORD m_dwCookie;
	LPCONNECTIONPOINT m_pCP;
	HRESULT m_hrConnected;
	char m_szURL[512];
	
	READYSTATE m_lReadyState;
};

STDMETHODIMP CApp::QueryInterface(REFIID riid, LPVOID* ppv)
{
	*ppv = NULL;

	if (IID_IUnknown == riid || IID_IPropertyNotifySink == riid)
	{
		*ppv = (LPUNKNOWN)(IPropertyNotifySink*)this;
		AddRef();
		return NOERROR;
	}
	else if (IID_IOleClientSite == riid)
	{
		*ppv = (IOleClientSite*)this;
		AddRef();
		return NOERROR;
	}
	else if (IID_IDispatch == riid)
	{
		*ppv = (IDispatch*)this;
		AddRef();
		return NOERROR;
	}
	else
	{
		OLECHAR wszBuff[39];
		int i = StringFromGUID2(riid, wszBuff, 39);
		TCHAR szBuff[39];
		i = WideCharToMultiByte(CP_ACP, 0, wszBuff, -1, szBuff, 39, NULL, NULL);
		ODS("CApp QI: "); ODS(szBuff); ODS("\n");
		return E_NOTIMPL;
	}
}

STDMETHODIMP_(ULONG) CApp::AddRef()
{
	TCHAR szBuff[255];
	wsprintf(szBuff, "CApp refcount increased to %d\n", m_dwRef+1);
	ODS(szBuff);
	return ++m_dwRef;
}

STDMETHODIMP_(ULONG) CApp::Release()
{
	TCHAR szBuff[255];

	if (--m_dwRef == 0) { 
		ODS("Deleting CApp\n");
		delete this; 
		return 0; 
	}

	wsprintf(szBuff, "CApp refcount reduced to %d\n", m_dwRef);
	ODS(szBuff);
	return m_dwRef;
}

// Fired on change of the value of a 'bindable' property
STDMETHODIMP CApp::OnChanged(DISPID dispID)
{

	WriteLog("============ CApp::OnChanged ===========");

	HRESULT hr;
	TCHAR szBuff[255];

	if (DISPID_READYSTATE == dispID) {
		// check the value of the readystate property

		VARIANT vResult = {0};
		EXCEPINFO excepInfo;
		UINT uArgErr;

		DISPPARAMS dp = {NULL, NULL, 0, 0};
		if (SUCCEEDED(hr = m_pMSHTML->Invoke(DISPID_READYSTATE, IID_NULL, LOCALE_SYSTEM_DEFAULT, 
			DISPATCH_PROPERTYGET, &dp, &vResult, &excepInfo, &uArgErr)))
		{
			TCHAR szReadyState[20];
			ASSERT(VT_I4 == V_VT(&vResult));
			m_lReadyState = (READYSTATE)V_I4(&vResult);
			switch (m_lReadyState)
			{	
			case READYSTATE_UNINITIALIZED:	//= 0,
				lstrcpy(szReadyState, _T("Uninitialized"));
				break;
			case READYSTATE_LOADING: //	= 1,
				lstrcpy(szReadyState, _T("Loading"));
				break;	
			case READYSTATE_LOADED:	// = 2,
				lstrcpy(szReadyState, _T("Loaded"));
				break;
			case READYSTATE_INTERACTIVE: //	= 3,
				lstrcpy(szReadyState, _T("Interactive"));
				break;
			case READYSTATE_COMPLETE: // = 4
				lstrcpy(szReadyState, _T("Complete"));
				WriteLog("READYSTATE_COMPLETE!");
				BOOL fRet = PostThreadMessage(GetCurrentThreadId(),
									WM_USER_STARTWALKING,
									(WPARAM)0,
									(LPARAM)0);
				break;
			}
			wsprintf(szBuff, _T("OnChanged: readyState = %s\n"), szReadyState);
			VariantClear(&vResult);
		}
		else
		{
			lstrcpy(szBuff, _T("Unable to obtain readyState value\n"));
		}
	}
	else
	{
		wsprintf(szBuff, _T("OnChanged: %d\n"), dispID);
	}

	ODS(szBuff);

	return NOERROR;
}

STDMETHODIMP CApp::OnRequestEdit(DISPID dispID)
{
	// Property changes are OK any time as far as this app is concerned
	TCHAR szBuff[255];
	wsprintf(szBuff, _T("OnRequestEdit: %d\n"), dispID);
	ODS(szBuff);

	return NOERROR;
}

STDMETHODIMP CApp::Invoke(DISPID dispIdMember,
            REFIID riid,
            LCID lcid,
            WORD wFlags,
            DISPPARAMS __RPC_FAR *pDispParams,
            VARIANT __RPC_FAR *pVarResult,
            EXCEPINFO __RPC_FAR *pExcepInfo,
            UINT __RPC_FAR *puArgErr)
{
	if (!pVarResult)
	{
		return E_POINTER;
	}

	switch(dispIdMember)
	{
	case DISPID_AMBIENT_DLCONTROL: 
		// respond to this ambient to indicate that we only want to
		// download the page, but we don't want to run scripts,
		// Java applets, or ActiveX controls
		V_VT(pVarResult) = VT_I4;
		V_I4(pVarResult) =  DLCTL_DOWNLOADONLY | 
							DLCTL_NO_FRAMEDOWNLOAD |
							DLCTL_NO_SCRIPTS | 
							DLCTL_NO_JAVA |
							DLCTL_NO_DLACTIVEXCTLS |
							DLCTL_NO_RUNACTIVEXCTLS;
		break;
	case DISPID_AMBIENT_USERMODE:
		// put MSHTML into design mode
		V_VT(pVarResult) = VT_BOOL;
		V_BOOL(pVarResult) = VARIANT_FALSE;
		break;
	default:
		return DISP_E_MEMBERNOTFOUND;
	}

	return NOERROR;
}

HRESULT CApp::Init(int argc, CString strFileName)
{
	HRESULT hr;
	LPCONNECTIONPOINTCONTAINER pCPC = NULL;
	LPOLEOBJECT pOleObject = NULL;
	LPOLECONTROL pOleControl = NULL;

	BSTR bstrUA = NULL;

	if (argc < 2) {
		// If no arguments were passed, load a default URL
		strcpy(m_szURL, strFileName);// = _T("http://www.163.com/");
	}
	else
	{
		// otherwise, check the scheme (protocol)
		// to determine which persistence interface to use
		// see the Run method for where the scheme is used
	
	}

	// Create an instance of an dynamic HTML document
	if (FAILED(hr = CoCreateInstance( CLSID_HTMLDocument, NULL, 
					CLSCTX_INPROC_SERVER, IID_IHTMLDocument2, 
					(LPVOID*)&this->m_pMSHTML )))
	{
		goto Error;
	}

	if (FAILED(hr = m_pMSHTML->QueryInterface(IID_IOleObject, (LPVOID*)&pOleObject)))
	{
		goto Error;
	}
	hr = pOleObject->SetClientSite((IOleClientSite*)this);
	pOleObject->Release();

	if (FAILED(hr = m_pMSHTML->QueryInterface(IID_IOleControl, (LPVOID*)&pOleControl)))
	{
		goto Error;
	}

	hr = pOleControl->OnAmbientPropertyChange(DISPID_AMBIENT_USERMODE);

	hr = pOleControl->OnAmbientPropertyChange(DISPID_AMBIENT_DLCONTROL);

	pOleControl->Release();

	// Hook up sink to catch ready state property change
	if (FAILED(hr = m_pMSHTML->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&pCPC)))
	{
		goto Error;
	}

	if (FAILED(hr = pCPC->FindConnectionPoint(IID_IPropertyNotifySink, &m_pCP)))
	{
		goto Error;
	}

	m_hrConnected = m_pCP->Advise((LPUNKNOWN)(IPropertyNotifySink*)this, &m_dwCookie);

Error:
	if (pCPC) pCPC->Release();

	return hr;
}

// Clean up connection point
HRESULT CApp::Term()
{
	return Passivate();
}

// Load the specified document and start pumping messages
HRESULT CApp::Run(CString strMonitorId, CString strStepIndex, CStringList& lstLinks, CStringList& lstForms,  CStringList& lstFormDate, CStringList& lstFrames, CStringList& lstRefreshs)
{
	WriteLog("============ Run ===========");

	HRESULT hr;
	MSG msg;

	// load URL using IPersistMoniker
	//hr = LoadURLFromMoniker();

	//WriteLog("success load url!");

	// try loading URL using IPersistFile
	hr = LoadURLFromFile();

	WriteLog("success load file!");

	if (SUCCEEDED(hr) || E_PENDING == hr)
	{

		while (GetMessage(&msg, NULL, 0, 0))
		{
			char szTemp[1024] = {0};
			sprintf( szTemp, "message=%d,HWND=%d", msg.message, msg.hwnd );
			WriteLog( szTemp );

			if (WM_USER_STARTWALKING == msg.message /*&& NULL == msg.hwnd*/)
			{
				this->Walk(strMonitorId, strStepIndex, lstLinks, lstForms,  lstFormDate, lstFrames, lstRefreshs);
			}
			else
			{
				DispatchMessage(&msg);
			}
		}
	}

	return hr;
}


CString CApp::GetElementAttributeValue(CString attributename ,  IHTMLElement* pElem ) 
{
	//return "";
	BSTR bstrAt = 0;
	VARIANT bstrValue;

	bstrAt = attributename.AllocSysString();
	pElem->getAttribute(bstrAt,1,&bstrValue );
	
	if(bstrValue.vt == VT_BSTR)
	{
		BSTR reVal = bstrValue.bstrVal;

		CString strCaption(reVal);
		SysFreeString(reVal);
		//printf("%s\n", strCaption)	;
		SysFreeString(bstrAt);
		return strCaption;
	}
	else
		SysFreeString(bstrAt);
	return "";
}

HRESULT CApp::GetAnchorElementList(CString strMonitorId, CString strStepIndex, CStringList& listAnchorElement)
{
	char szTemp[1024] = {0};
	WriteLog("============ GetAnchorElementList ===========");

	HRESULT hr;
	IHTMLElementCollection* pColl = NULL;
	int nIndex = 0;
	CString strPaserStepFileName = _T("");
	CString strKey = _T("");
	CString strSection = _T("Links");
	if(strMonitorId == "")
		strPaserStepFileName.Format("%s\\cgi-bin\\PaserStep%s.info", FuncGetInstallRootPath(), strStepIndex);
	else
		strPaserStepFileName.Format("%s\\cgi-bin\\%s_PaserStep%s.info", FuncGetInstallRootPath(), strMonitorId, strStepIndex);

	WriteLog(strPaserStepFileName.GetBuffer());

	//::DFN_WritePrivateProfileString(strSection, NULL, NULL, strPaserStepFileName);

	ASSERT(m_pMSHTML);
	if (!m_pMSHTML)
	{
		return E_UNEXPECTED;
	}

	if (READYSTATE_COMPLETE != m_lReadyState)
	{
		ODS("Shouldn't get here 'til MSHTML changes readyState to READYSTATE_COMPLETE\n");
		DebugBreak();
		return E_UNEXPECTED;
	}

	if (SUCCEEDED(hr = m_pMSHTML->get_all( &pColl )))
	{
		long cElems;

		ASSERT(pColl);

		// retrieve the count of elements in the collection
		if (SUCCEEDED(hr = pColl->get_length( &cElems )))
		{
			sprintf( szTemp, "length=%d\n", cElems );
			WriteLog( szTemp );
			
			// for each element retrieve properties such as TAGNAME and HREF

			for ( int i=0; i<cElems; i++ )
			{
				VARIANT vIndex;
				vIndex.vt = VT_UINT;
				vIndex.lVal = i;
				VARIANT var2 = { 0 };
				LPDISPATCH pDisp; 

				if (SUCCEEDED(hr = pColl->item( vIndex, var2, &pDisp )))
				{
					IHTMLElement* pElem = NULL;
					if (SUCCEEDED(hr = pDisp->QueryInterface( IID_IHTMLElement, (LPVOID*)&pElem )))
					{
						BSTR bstr;

						IHTMLAnchorElement *pAnchorElem = NULL;
						if (SUCCEEDED(hr = pDisp->QueryInterface( IID_IHTMLAnchorElement, (void **)&pAnchorElem )))
						{
							nIndex ++;
							ASSERT(pAnchorElem);
							
							pElem->get_innerText(&bstr);
							CString str(bstr);

							WriteLog( str.GetBuffer() );

							str.Replace(" ", "");

							if((!bstr)||(str.GetLength() == 0))
								pAnchorElem->get_href(&bstr);

							CString strIndex = _T("");
							strIndex.Format("%d", nIndex);

							strKey = "_linkNum";
							::DFN_WritePrivateProfileString(strSection, strKey, strIndex, strPaserStepFileName);
							strKey.Format("_text%d", nIndex);

							CString strTag(bstr);
							if(strTag.GetLength() > 60)
								strTag = strTag.Left(60);
							::DFN_WritePrivateProfileString(strSection, strKey, strTag, strPaserStepFileName);
						
							pAnchorElem->get_href(&bstr);							
							CString strHerf(bstr);
							strHerf.MakeLower();
							if(strHerf.Find("http://") < 0)
							{
								pAnchorElem->get_nameProp(&bstr);
								CString strFinalHerf(bstr);
								strHerf = strFinalHerf;
							}

							strKey.Format("_herf%d", nIndex);
							::DFN_WritePrivateProfileString(strSection, strKey, strHerf, strPaserStepFileName);

//							CString str1(bstr);
//							pElem->get_innerHTML(&bstr);
							if (bstr)
							{
								listAnchorElement.AddTail(strTag);
								SysFreeString(bstr);
							}

							pAnchorElem->Release();	
						}

						pElem->Release();
					} // QI(IHTMLElement)
					pDisp->Release();
				} // item
			} // for
		} // get_length

		pColl->Release();
	} // get_all

	return hr;
}
/*
HRESULT CApp::GetFormElementList(CString strStepIndex, CStringList& listFormElement)
{
	HRESULT hr;
	IHTMLElementCollection* pColl = NULL;

	ASSERT(m_pMSHTML);
	if (!m_pMSHTML)
	{
		return E_UNEXPECTED;
	}

	if (READYSTATE_COMPLETE != m_lReadyState)
	{
		ODS("Shouldn't get here 'til MSHTML changes readyState to READYSTATE_COMPLETE\n");
		DebugBreak();
		return E_UNEXPECTED;
	}

	if (SUCCEEDED(hr = m_pMSHTML->get_forms( &pColl )))
	{
		long cElems;

		ASSERT(pColl);

		// retrieve the count of elements in the collection
		if (SUCCEEDED(hr = pColl->get_length( &cElems )))
		{
			// for each element retrieve properties such as TAGNAME and HREF
 			for ( int i=0; i<cElems; i++ )
			{
				VARIANT vIndex;
				vIndex.vt = VT_UINT;
				vIndex.lVal = i;
				VARIANT var2 = { 0 };
				LPDISPATCH pDisp; 

				if (SUCCEEDED(hr = pColl->item( vIndex, var2, &pDisp )))
				{
					IHTMLElement* pElem = NULL;
					if (SUCCEEDED(hr = pDisp->QueryInterface( IID_IHTMLElement, (LPVOID*)&pElem )))
					{
						BSTR bstr;

						IHTMLFormElement  *pFormElem = NULL;
						if (SUCCEEDED(hr = pDisp->QueryInterface( IID_IHTMLFormElement, (void **)&pFormElem )))
						{		//Form Element
							ASSERT(pFormElem);
							
							pFormElem->get_name(&bstr);
							//pAnchorElem->get_href(&bstr);
							if (bstr)
							{	
								CString strTag(bstr);
								CString strWhole = _T("");
								strWhole.Format("{[%s]%s}%s", strStepIndex, strTag, GetElementAttributeValue("name", pElem));
								listFormElement.AddTail(strWhole);
								SysFreeString(bstr);
							}

							pFormElem->Release();	
						}

						pElem->Release();
					} // QI(IHTMLElement)
					pDisp->Release();
				} // item
			} // for
		} // get_length

		pColl->Release();
	} // get_all

	return hr;
}
*/
HRESULT CApp::GetFormDateList(CString strMonitorId, CString strStepIndex, CStringList& listFormElement, CStringList& listFormDate)
{
	HRESULT hr;
	IHTMLElementCollection* pColl = NULL;

	int nFormIndex = 0;
	CString strPaserStepFileName = _T("");
	CString strKey = _T("");
	CString strSection = _T("Forms");
	if(strMonitorId == "")
		strPaserStepFileName.Format("%s\\cgi-bin\\PaserStep%s.info", FuncGetInstallRootPath(), strStepIndex);
	else
		strPaserStepFileName.Format("%s\\cgi-bin\\%s_PaserStep%s.info", FuncGetInstallRootPath(), strMonitorId, strStepIndex);

	//::DFN_WritePrivateProfileString(strSection, NULL, NULL, strPaserStepFileName);

	ASSERT(m_pMSHTML);
	if (!m_pMSHTML)
	{
		return E_UNEXPECTED;
	}

	if (READYSTATE_COMPLETE != m_lReadyState)
	{
		ODS("Shouldn't get here 'til MSHTML changes readyState to READYSTATE_COMPLETE\n");
		DebugBreak();
		return E_UNEXPECTED;
	}

	if (SUCCEEDED(hr = m_pMSHTML->get_forms( &pColl )))
	{
		long cElems;

		ASSERT(pColl);

		// retrieve the count of elements in the collection
		if (SUCCEEDED(hr = pColl->get_length( &cElems )))
		{
			// for each element retrieve properties such as TAGNAME and HREF
 			for ( int i=0; i<cElems; i++ )
			{
				VARIANT vIndex;
				vIndex.vt = VT_UINT;
				vIndex.lVal = i;
				VARIANT var2 = { 0 };
				LPDISPATCH pDisp; 

				if (SUCCEEDED(hr = pColl->item( vIndex, var2, &pDisp )))
				{
					IHTMLElement* pElem = NULL;
					if (SUCCEEDED(hr = pDisp->QueryInterface( IID_IHTMLElement, (LPVOID*)&pElem )))
					{
						BSTR bstr;

						IHTMLFormElement  *pFormElem = NULL;
						if (SUCCEEDED(hr = pDisp->QueryInterface( IID_IHTMLFormElement, (void **)&pFormElem )))
						{		//Form Element
							ASSERT(pFormElem);
							nFormIndex ++;
							pFormElem->get_name(&bstr);
							if(!bstr)
							{
								pFormElem->get_action(&bstr);
							}
							
							CString strIndex = _T("");
							strIndex.Format("%d", nFormIndex);

							strKey = "_formNum";
							::DFN_WritePrivateProfileString(strSection, strKey, strIndex, strPaserStepFileName);
							
							CString strTag(bstr);
							
//							pFormElem->get_action(&bstr);
							CString strAction = _T("");
							strAction = GetElementAttributeValue("action", pElem);

							strKey.Format("_action%d", nFormIndex);
							::DFN_WritePrivateProfileString(strSection, strKey, strAction, strPaserStepFileName);

							long FormElemLength;
							pFormElem->get_length(&FormElemLength);

							int nFormDataCount = 0;
							nFormDataCount = FormElemLength;
							BOOL bHasSubmit = FALSE;

							for(int ii = 0; ii < FormElemLength; ii++)
							{
								VARIANT vIndexDate;
								vIndexDate.vt = VT_UINT;
								vIndexDate.lVal = ii;
								VARIANT varDate2 = { 0 };
								LPDISPATCH pDispDate; 
								
								pFormElem->item(vIndexDate, varDate2, &pDispDate); 
								IHTMLElement* pElemDate = NULL;
								if (SUCCEEDED(hr = pDispDate->QueryInterface( IID_IHTMLElement, (LPVOID*)&pElemDate )))
								{
									CString strType = _T("");
									strType = GetElementAttributeValue("type", pElemDate);
									CString strWhole = _T("");

									if (!strType.CompareNoCase("submit"))
									{
										bHasSubmit = TRUE;
									}
									pElemDate->Release();
								}
								pDispDate->Release();								
							}
							
							if (!bHasSubmit)
							{
								CString strWhole = _T("");
								strWhole.Format("{[%d]%s}", nFormIndex, strTag);
								
								strKey.Format("_text%d", nFormIndex);
								::DFN_WritePrivateProfileString(strSection, strKey, strWhole, strPaserStepFileName);
								
								listFormElement.AddTail(strWhole);
							}	

							for(int jj = 0; jj < FormElemLength; jj++)
							{
								VARIANT vIndexDate;
								vIndexDate.vt = VT_UINT;
								vIndexDate.lVal = jj;
								VARIANT varDate2 = { 0 };
								LPDISPATCH pDispDate; 
								
								pFormElem->item(vIndexDate, varDate2, &pDispDate); 
								IHTMLElement* pElemDate = NULL;
								if (SUCCEEDED(hr = pDispDate->QueryInterface( IID_IHTMLElement, (LPVOID*)&pElemDate )))
								{
									CString strNameAndValue = _T("");
									CString strItemName = _T("");
									strItemName = GetElementAttributeValue("name", pElemDate);
									CString strItemValue = _T("");
									strItemValue = GetElementAttributeValue("value", pElemDate);
									CString strType = _T("");
									strType = GetElementAttributeValue("type", pElemDate);
									CString strWhole = _T("");
					
									strKey.Format("_postdataNum%d", i + 1);
									strIndex.Format("%d", FormElemLength);
									::DFN_WritePrivateProfileString(strSection, strKey, strIndex, strPaserStepFileName);

									if (!strType.CompareNoCase("submit"))
									{
										strNameAndValue = GetElementAttributeValue("value", pElemDate);
										if(strNameAndValue == "")
											strNameAndValue.Format("[%d]", nFormIndex);
										strWhole.Format("{[%d]%s}%s", nFormIndex, strTag, strNameAndValue);
										
										strKey.Format("_text%d", nFormIndex);
										::DFN_WritePrivateProfileString(strSection, strKey, strWhole, strPaserStepFileName);
										
										listFormElement.AddTail(strWhole);
									
										if(strItemName == "")
											strNameAndValue.Format("[%d]", nFormIndex);
										else
											strNameAndValue = strItemName + "=" + strItemValue;
							
										strKey.Format("_postdata%d_%d", nFormIndex, jj+1);
										::DFN_WritePrivateProfileString(strSection, strKey, strNameAndValue, strPaserStepFileName);
	
										//										strWhole.Format("{[%s]%s}%s", strStepIndex, strTag, strNameAndValue);
										//										listFormDate.AddTail(strWhole);
										
									}
									else
									{
/*										if(strItemName == "")
											strNameAndValue.Format("[%d]", nFormIndex);
										else
										strNameAndValue = strItemName + "=" + strItemValue;
										*/	
										
										if(strItemName != "")
										{
											strNameAndValue = strItemName + "=" + strItemValue;
											strKey.Format("_postdata%d_%d", nFormIndex, jj+1);
											::DFN_WritePrivateProfileString(strSection, strKey, strNameAndValue, strPaserStepFileName);
											
											CString strAttribute = _T("");
											strAttribute = GetElementAttributeValue("type", pElemDate);
											
											if(strAttribute.CompareNoCase("hidden"))
											{
												strWhole.Format("{[%d]%s}%s", nFormIndex, strTag, strNameAndValue);
												listFormDate.AddTail(strWhole);
											}

										}
									}

									pElemDate->Release();								
								}
								pDispDate->Release();
							}

							SysFreeString(bstr);

							pFormElem->Release();	
						}

						pElem->Release();
					} // QI(IHTMLElement)
					pDisp->Release();
				} // item
			} // for
		} // get_length

		pColl->Release();
	} // get_all

	return hr;
}

HRESULT CApp::GetRefreshElementList(CString strMonitorId, CString strStepIndex, CStringList& listRefreshElement)
{
	HRESULT hr;
	IHTMLElementCollection* pColl = NULL;

	int nIndex = 0;
	CString strPaserStepFileName = _T("");
	CString strKey = _T("");
	CString strSection = _T("Refreshes");
	if(strMonitorId == "")
		strPaserStepFileName.Format("%s\\cgi-bin\\PaserStep%s.info", FuncGetInstallRootPath(), strStepIndex);
	else
		strPaserStepFileName.Format("%s\\cgi-bin\\%s_PaserStep%s.info", FuncGetInstallRootPath(), strMonitorId, strStepIndex);

	//::DFN_WritePrivateProfileString(strSection, NULL, NULL, strPaserStepFileName);

	ASSERT(m_pMSHTML);
	if (!m_pMSHTML)
	{
		return E_UNEXPECTED;
	}

	if (READYSTATE_COMPLETE != m_lReadyState)
	{
		ODS("Shouldn't get here 'til MSHTML changes readyState to READYSTATE_COMPLETE\n");
		DebugBreak();
		return E_UNEXPECTED;
	}

	if (SUCCEEDED(hr = m_pMSHTML->get_all( &pColl )))
	{
		long cElems;

		ASSERT(pColl);

		// retrieve the count of elements in the collection
		if (SUCCEEDED(hr = pColl->get_length( &cElems )))
		{
			// for each element retrieve properties such as TAGNAME and HREF
			for ( int i=0; i<cElems; i++ )
			{
				VARIANT vIndex;
				vIndex.vt = VT_UINT;
				vIndex.lVal = i;
				VARIANT var2 = { 0 };
				LPDISPATCH pDisp; 

				if (SUCCEEDED(hr = pColl->item( vIndex, var2, &pDisp )))
				{
					IHTMLElement* pElem = NULL;
					if (SUCCEEDED(hr = pDisp->QueryInterface( IID_IHTMLElement, (LPVOID*)&pElem )))
					{
						BSTR bstr;
						IHTMLMetaElement  *pMetaElem = NULL;
						if (SUCCEEDED(hr = pDisp->QueryInterface( IID_IHTMLMetaElement, (void **)&pMetaElem)))
						{		//Form Element
							ASSERT(pMetaElem);
							pMetaElem->get_httpEquiv(&bstr);
							CString str(bstr);
							//pAnchorElem->get_href(&bstr);
							if (bstr)
							{
								char charArray[256] = {0};
								sprintf(charArray,"%ls",bstr);
								int i = -1;
								i = strcmp(charArray, "Refresh");
								if(i== 0)
								{
									pMetaElem->get_content(&bstr);
									CString strTag(bstr);
									nIndex ++;
									CString strIndex = _T("");
									strIndex.Format("%d", nIndex);
									
									strKey = "_refreshNum";
									::DFN_WritePrivateProfileString(strSection, strKey, strIndex, strPaserStepFileName);
									strKey.Format("_text%d", nIndex);
									CString strText = _T("");
									strText = GetElementAttributeValue("content", pElem);
									::DFN_WritePrivateProfileString(strSection, strKey, strText, strPaserStepFileName);
									
									strKey.Format("_url%d", nIndex);	
									CString strUrl = _T("");
									strUrl = FuncGetRightString(strText, "=");
									strUrl.Replace(";", "");
	
									::DFN_WritePrivateProfileString(strSection, strKey, strUrl, strPaserStepFileName);
									
									listRefreshElement.AddTail(strText);
								}

								SysFreeString(bstr);
							}

							pMetaElem->Release();	
						}

						pElem->Release();
					} // QI(IHTMLElement)
					pDisp->Release();
				} // item
			} // for
		} // get_length

		pColl->Release();
	} // get_all

	return hr;
}
/*
HRESULT CApp::GetFrameElementList(CString strStepIndex, CStringList& listFrameElement)
{
	IHTMLFramesCollection2 *pFramesSet = NULL;
	HRESULT hr;

	int nIndex = 0;
	CString strPaserStepFileName = _T("");
	CString strKey = _T("");
	CString strSection = _T("Frames");
	strPaserStepFileName.Format("%s\\cgi-bin\\PaserStep%s.info", FuncGetInstallPath(), strStepIndex);

	hr = m_pMSHTML->get_frames(&pFramesSet);
	if(SUCCEEDED(hr))
	{
		LONG celem = 0;	
		pFramesSet->get_length(&celem);
		for(int i = 0;i < celem;i ++)
		{
			VARIANT varIndex;
			varIndex.vt = VT_UINT;
			varIndex.lVal = i;

			VARIANT var2;
			VariantInit( &var2 );

			IDispatch* pDisp;
			hr = pFramesSet->item( &varIndex, &var2);
			if(FAILED(hr))
				return hr;

			pDisp=var2.pdispVal;
			IHTMLWindow2 *pFrameWindow = NULL;
			hr = pDisp->QueryInterface(IID_IHTMLWindow2, (void **)&pFrameWindow);
			if(SUCCEEDED(hr))
			{
				BSTR bstr;
				pFrameWindow->get_name(&bstr);
				CString strTag(bstr);

				nIndex ++;
				CString strIndex = _T("");
				strIndex.Format("%d", nIndex);
				
				strKey = "_frameNum";
				::DFN_WritePrivateProfileString(strSection, strKey, strIndex, strPaserStepFileName);
				strKey.Format("_text%d", nIndex);
				
				::DFN_WritePrivateProfileString(strSection, strKey, strTag, strPaserStepFileName);
				
				IHTMLElement* pElem = NULL;
				CString strAction = _T("");
				if (SUCCEEDED(hr = pDisp->QueryInterface( IID_IHTMLElement, (LPVOID*)&pElem )))
				{
					
				}
				
				strKey.Format("_src%d", nIndex);
				::DFN_WritePrivateProfileString(strSection, strKey, strAction, strPaserStepFileName);

				listFrameElement.AddTail(strTag);
				SysFreeString(bstr);
			}		

			pFrameWindow->Release();
			
		}
	}

	return hr;
}
*/

HRESULT CApp::GetFrameElementList(CString strMonitorId, CString strStepIndex, CStringList& listFrameElement)
{
	WriteLog("============ GetFrameElementList ===========");

	HRESULT hr;
	IHTMLElementCollection* pColl = NULL;
	int nIndex = 0;
	CString strPaserStepFileName = _T("");
	CString strKey = _T("");
	CString strSection = _T("Frames");
	if(strMonitorId == "")
		strPaserStepFileName.Format("%s\\cgi-bin\\PaserStep%s.info", FuncGetInstallRootPath(), strStepIndex);
	else
		strPaserStepFileName.Format("%s\\cgi-bin\\%s_PaserStep%s.info", FuncGetInstallRootPath(), strMonitorId, strStepIndex);

	//::DFN_WritePrivateProfileString(strSection, NULL, NULL, strPaserStepFileName);
	ASSERT(m_pMSHTML);

	if (!m_pMSHTML)
	{
		return E_UNEXPECTED;
	}

	if (READYSTATE_COMPLETE != m_lReadyState)
	{
		ODS("Shouldn't get here 'til MSHTML changes readyState to READYSTATE_COMPLETE\n");
		DebugBreak();
		return E_UNEXPECTED;
	}

	if (SUCCEEDED(hr = m_pMSHTML->get_all( &pColl )))
	{
		long cElems;

		ASSERT(pColl);

		// retrieve the count of elements in the collection
		if (SUCCEEDED(hr = pColl->get_length( &cElems )))
		{
			// for each element retrieve properties such as TAGNAME and HREF

			for ( int i=0; i<cElems; i++ )
			{
				VARIANT vIndex;
				vIndex.vt = VT_UINT;
				vIndex.lVal = i;
				VARIANT var2 = { 0 };
				LPDISPATCH pDisp; 

				if (SUCCEEDED(hr = pColl->item( vIndex, var2, &pDisp )))
				{
					IHTMLElement* pElem = NULL;
					if (SUCCEEDED(hr = pDisp->QueryInterface( IID_IHTMLElement, (LPVOID*)&pElem )))
					{
						BSTR bstr;

						IHTMLFrameElement *pFrameElem = NULL;
						if (SUCCEEDED(hr = pDisp->QueryInterface( IID_IHTMLFrameElement, (void **)&pFrameElem )))
						{
							nIndex ++;
							ASSERT(pFrameElem);
							
							CString strIndex = _T("");
							strIndex.Format("%d", nIndex);

							strKey = "_frameNum";
							::DFN_WritePrivateProfileString(strSection, strKey, strIndex, strPaserStepFileName);
							strKey.Format("_text%d", nIndex);

							CString strTag = _T("");
							strTag = GetElementAttributeValue("name", pElem);
							::DFN_WritePrivateProfileString(strSection, strKey, strTag, strPaserStepFileName);

							CString strSrc = _T("");
							strSrc = GetElementAttributeValue("src", pElem);
							strKey.Format("_src%d", nIndex);
							::DFN_WritePrivateProfileString(strSection, strKey, strSrc, strPaserStepFileName);

//							CString str1(bstr);
//							pElem->get_innerHTML(&bstr);
							if (bstr)
							{
								listFrameElement.AddTail(strTag);
							}

							pFrameElem->Release();	
						}

						pElem->Release();
					} // QI(IHTMLElement)
					pDisp->Release();
				} // item
			} // for
		} // get_length

		pColl->Release();
	} // get_all

	return hr;
}
HRESULT CApp::Walk(CString strMonitorId, CString strStepIndex, CStringList& lstLinks, CStringList& lstForms,  CStringList& lstFormDate, CStringList& lstFrames, CStringList& lstRefreshs)
{
	WriteLog("============ Walk ===========");

	HRESULT hr = 0;

	GetAnchorElementList(strMonitorId, strStepIndex, lstLinks);
	GetFrameElementList(strMonitorId, strStepIndex, lstFrames);
	GetRefreshElementList(strMonitorId, strStepIndex, lstRefreshs);
//	GetFormElementList(strStepIndex, lstForms);
	GetFormDateList(strMonitorId, strStepIndex, lstForms, lstFormDate);

/*
	// retrieve a reference to the ALL collection
	if (SUCCEEDED(hr = m_pMSHTML->get_all( &pColl )))
	{
		long cElems;

		ASSERT(pColl);

		// retrieve the count of elements in the collection
		if (SUCCEEDED(hr = pColl->get_length( &cElems )))
		{
			// for each element retrieve properties such as TAGNAME and HREF
			for ( int i=0; i<cElems; i++ )
			{
				VARIANT vIndex;
				vIndex.vt = VT_UINT;
				vIndex.lVal = i;
				VARIANT var2 = { 0 };
				LPDISPATCH pDisp; 

				if (SUCCEEDED(hr = pColl->item( vIndex, var2, &pDisp )))
				{
					IHTMLElement* pElem = NULL;
					if (SUCCEEDED(hr = pDisp->QueryInterface( IID_IHTMLElement, (LPVOID*)&pElem )))
					{
						//_bstr_t bstrTag;
						BSTR bstr;

						ASSERT(pElem);

						hr = pElem->get_tagName(&bstr);
						if (bstr)
						{
							//bstrTag = bstr;
							SysFreeString(bstr);
						}

						// if the element is an <IMG> get its SRC attribute
						IHTMLImgElement* pImage = NULL;
						if (SUCCEEDED(hr = pDisp->QueryInterface( IID_IHTMLImgElement, (LPVOID*)&pImage )))
						{
							ASSERT(pImage);

							pImage->get_src(&bstr);
							if (bstr)
							{
								//bstrTag += " - ";
								//bstrTag += bstr;
								SysFreeString(bstr);
							}
							pImage->Release();
						}
						
						//PrintBSTR(bstrTag);

						pElem->Release();
					} // QI(IHTMLElement)
					pDisp->Release();
				} // item
			} // for
		} // get_length


		pColl->Release();
	} // get_all
*/
	// We're done so post ourselves a quit to terminate the message pump.
	PostQuitMessage(0);

	return hr;
}

HRESULT CApp::LoadURLFromFile()
{
	HRESULT hr;
	OLECHAR  wszURL[MAX_PATH*sizeof(OLECHAR)];
	if (0 == MultiByteToWideChar(CP_ACP, 0, m_szURL, -1, wszURL, MAX_PATH*sizeof(OLECHAR)))
	{
		return E_FAIL;
	}

	LPPERSISTFILE  pPF;
	// MSHTML supports file persistence for ordinary files.
    if ( SUCCEEDED(hr = m_pMSHTML->QueryInterface(IID_IPersistFile, (LPVOID*) &pPF)))
	{
		TCHAR szBuff[MAX_PATH];
		wsprintf(szBuff, "Loading %s...\n", m_szURL);
		ODS(szBuff);

		hr = pPF->Load(wszURL, 0);
		pPF->Release();
	}

	return hr;
}

void MyDebugFile(const char* szMsg)
{
	FILE * fp = fopen("c:\\transaction.txt", "a+");
	if (fp) {
		fputs(szMsg, fp);
		fputs("\r\n", fp);
		fclose(fp);
	}
}

HRESULT CApp::LoadURLFromMoniker()
{
	HRESULT hr;
	OLECHAR  wszURL[MAX_PATH*sizeof(OLECHAR)];
	if (0 == MultiByteToWideChar(CP_ACP, 0, m_szURL, -1, wszURL, MAX_PATH*sizeof(OLECHAR)))
	{
		return E_FAIL;
	}

	// Ask the system for a URL Moniker
	LPMONIKER pMk = NULL;
	LPBINDCTX pBCtx = NULL;
	LPPERSISTMONIKER pPMk = NULL;

	if (FAILED(hr = CreateURLMoniker(NULL, wszURL, &pMk)))
	{
		return hr;
	}

	if (FAILED(hr = CreateBindCtx(0, &pBCtx)))
	{
		goto Error;
	}

	// Use MSHTML moniker services to load the specified document
	if (SUCCEEDED(hr = m_pMSHTML->QueryInterface(IID_IPersistMoniker,
								(LPVOID*)&pPMk)))
	{
		// Call Load on the IPersistMoniker
		// This may return immediately, but the document isn't loaded until
		// MSHTML reports READYSTATE_COMPLETE. See the implementation of 
		// IPropertyNotifySink::OnChanged above and see how the app waits
		// for this state change before walking the document's object model.
		TCHAR szBuff[MAX_PATH];
		wsprintf(szBuff, "Loading %s...\n", m_szURL);
		ODS(szBuff);

		hr = pPMk->Load(FALSE, pMk, pBCtx, STGM_READ);
	}
		
Error:
	if (pMk) pMk->Release();
	if (pBCtx) pBCtx->Release();
	if (pPMk) pPMk->Release();
	return hr;
}

extern "C" __declspec(dllexport) BOOL HtmlParser(CString strMonitorId , CString strStepIndex, CStringList& lstLinks, CStringList& lstForms,  CStringList& lstFormDate, CStringList& lstFrames, CStringList& lstRefreshs, CString strFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	WriteLog("============ HtmlParser ===========");

	HRESULT hr = NOERROR;
	
	CApp * pApp = new CApp;
	if (!pApp)
		return FALSE;
	
	if (SUCCEEDED(hr = CoInitialize(NULL))) 	{
		MyDebugFile("Inside HTMLPASER.DLL");
		MyDebugFile(strFileName);
		if (SUCCEEDED(pApp->Init(1, strFileName)))	{
			
			WriteLog( strFileName.GetBuffer() );

/*			CString strPaserStepFileName = _T("");
			if(nGroupID ==0)
				strPaserStepFileName.Format("%s\\cgi-bin\\PaserStep%s.info", FuncGetInstallPath(), strStepIndex);
			else
				strPaserStepFileName.Format("%s\\cgi-bin\\%d_%d_PaserStep%s.info", FuncGetInstallPath(), nGroupID, nMonitorID, strStepIndex);

			::DeleteFile(strPaserStepFileName);
*/			
			pApp->Run(strMonitorId, strStepIndex, lstLinks, lstForms,  lstFormDate, lstFrames, lstRefreshs);	
			pApp->Term();			
		}		
	}

	if (pApp)
		pApp->Release();

	CoUninitialize();

	return FALSE;
}