// UrlTransaction.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "UrlTransaction.h"
#include "HttpClient.h"

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
// CUrlTransactionApp

BEGIN_MESSAGE_MAP(CUrlTransactionApp, CWinApp)
	//{{AFX_MSG_MAP(CUrlTransactionApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUrlTransactionApp construction

CUrlTransactionApp::CUrlTransactionApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CUrlTransactionApp object

CUrlTransactionApp theApp;


/////////////////////////////////////////////////////////////////////////////////
// Dll Export Function UrlTransaction
/////////////////////////////////////////////////////////////////////////////////
//extern "C" __declspec(dllexport) BOOL UrlTransaction(CStringList &paramList,
//                                                     char  *szReturn)
extern "C" __declspec(dllexport) BOOL UrlTransaction(const char * strParas, char * szReturn, int& nSize)
{
    //
    CString strFileName;

	CStringList paramList;

	MakeStringListByChar(paramList,strParas);

    POSITION pos = paramList.FindIndex(0);
	while(pos)	
    {
		CString strTemp = paramList.GetNext(pos);
        if(strTemp.Find("_transFile=",0) == 0)
        {
//            strFileName.Format("%s.info",strTemp.Right(strTemp.GetLength()-11));
            strFileName.Format("%s",strTemp.Right(strTemp.GetLength()-11));
            break;
        }
    }

	//CString strPath = ::FuncGetInstallRootPath();//Get Install Root Path
    //CString strFile;
    //strFile.Format("%s\\templates.transaction\\%s",strPath,strFileName);

    CHttpClient Client;
    //CString szMsg = Client.Monitor(strFile);
	CString szMsg = Client.Monitor(strFileName);
    //CString szMsg = Client.Monitor("");
    sprintf(szReturn,"%s", szMsg);

	CString strInput;
	strInput =szReturn;
	MakeCharByString(szReturn,nSize,strInput);

    return TRUE;
}
