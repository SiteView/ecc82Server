// Exchange2003.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Exchange2003.h"
#include "ExchangeWmi.h"

#include <list>
#include <string>
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
// CExchange2003App

BEGIN_MESSAGE_MAP(CExchange2003App, CWinApp)
	//{{AFX_MSG_MAP(CExchange2003App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExchange2003App construction


CExchange2003App::CExchange2003App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CExchange2003App object

CExchange2003App theApp;

//com函数
BOOL GetMachineInfo(CStringList& lstParas,CString &strMachineName,CString& strUserAccount,CString& strPassWord);
int GetVersionFromCom(CString strServer, CString strUser,CString strPwd);
_Exchange * ThreeCreateDragonCom();
BOOL AddToExceptionLog(const char* szMsg);

//Exchange

//MailBox
CString GetExchangeMailboxNamebyCom(CString strServer, CString strUser,CString strPwd);
CString GetExchangeMailboxbyCom(CString strServer, CString strUser,CString strPwd,CString strMailboxName);

//SMTP2003
CString GetSMTP2003byCom(CString strServer, CString strUser,CString strPwd);
//MTA2003
CString GetMTA2003byCom(CString strServer, CString strUser,CString strPwd);

//ExchangeConnecto
CString GetExchangeConnectorNamebyCom(CString strServer, CString strUser,CString strPwd);
CString GetExchangeConnectorStatebyCom(CString strServer, CString strUser,CString strPwd,CString strConnectorName);
//ExchangeQueue
CString GetExchangeQueueNamebyCom(CString strServer, CString strUser,CString strPwd);
CString GetExchangeQueuebyCom(CString strServer, CString strUser,CString strPwd,CString strExchangeQueueName);
//ExchangeServerState 
CString GetExchangeServerStatebyCom(CString strServer, CString strUser,CString strPwd);
//ExchangeServer
CString GetExchangeServerbyCom(CString strServer, CString strUser,CString strPwd);


/*****************2000******************/
//GetPOP3byCom
CString GetPOP3byCom(CString strServer, CString strUser,CString strPwd);
//GetSMTPbyCom
CString GetSMTPbyCom(CString strServer, CString strUser,CString strPwd);
//GetWebMailbyCom
CString GetWebMailbyCom(CString strServer, CString strUser,CString strPwd);


//公共函数
BOOL GetList(char *pValue,char *pMark,std::list<std::string> &lstText);
/////////////////////////////////////////////////////////////
///下面是所有接口 

int PrintLog(const char * strInfo, const char * strFileName)
{
//#ifndef _DEBUG
//	return -1;
//#endif
	char timebuf[128] = {0}, datebuf[128] = {0}, tempbuf[1024] = {0};
	char strFile[128] = {0};
	sprintf(strFile,"%s.log",strFileName);
	_strtime(timebuf);
	_strdate(datebuf);
	sprintf(tempbuf,"%s-%s",datebuf,timebuf);
	ofstream FileStream;
	FileStream.open(strFile,ios::app);
	FileStream<<tempbuf<<"\t"<<strInfo<<endl;
	FileStream.close();
	return 0;
}

//ExchangeMailboxName
extern "C" _declspec(dllexport) BOOL ExchangeMailboxName(const char * strParas, char * strReturn, int& nSize)
//(CStringList& lstParas, CStringList& lstText, CStringList& lstValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strMachineName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	CStringList lstParas;
	MakeStringListByChar(lstParas,strParas);
	
	GetMachineInfo(lstParas,strMachineName,strUserAccount,strPassWord);

	CString strRet=_T("");
	strRet = GetExchangeMailboxNamebyCom(strMachineName,strUserAccount,strPassWord);

	if (strRet=="")
	{
		return FALSE;
	}
	else 
	{
		
		std::list<std::string> lstText;
		GetList(strRet.GetBuffer(strRet.GetLength()),"?",lstText);
		std::string strOut;

		for(std::list<string>::iterator It=lstText.begin();It!=lstText.end();It++)
		{
			strOut+= It->c_str();
			strOut+= "=";
			strOut+= It->c_str();
			strOut+= "\v";
		}

		int lsize =static_cast<int>(strOut.size());

		if(nSize>lsize)
		{
			sprintf(strReturn,"%s",strOut.c_str());
			char *p;
			p=strReturn;
			while(*p!='\0')
			{
				if(*p=='\v') *p='\0';
				p++;
			
			}
			nSize =static_cast<int>(strOut.size());
			return true;

		}else return false;

		
	}
	
	return TRUE;

}

//ExchangeMailbox
extern "C" _declspec(dllexport) BOOL ExchangeMailbox(const char * strParas, char * szRet, int& nSize)
//(CStringList& lstParas, char* szRet)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strMachineName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	CStringList lstParas;
	MakeStringListByChar(lstParas,strParas);	 


	CString strMailboxName = GetValueFromList("_MailboxName",lstParas);


	GetMachineInfo(lstParas,strMachineName,strUserAccount,strPassWord);

	CString strRet=_T("");
	strRet = GetExchangeMailboxbyCom(strMachineName,strUserAccount,strPassWord,strMailboxName);
	MakeCharByString(szRet,nSize,strRet);
	//strcpy(szRet,strRet);

	
	return TRUE;
}

//ExchangeConnectorName
extern "C" _declspec(dllexport) BOOL ExchangeConnectorName(const char * strParas, char * strReturn, int& nSize)
//(CStringList& lstParas, CStringList& lstText, CStringList& lstValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strMachineName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	CStringList lstParas;
	MakeStringListByChar(lstParas,strParas);
	GetMachineInfo(lstParas,strMachineName,strUserAccount,strPassWord);

	CString strRet=_T("");
	strRet = GetExchangeConnectorNamebyCom(strMachineName,strUserAccount,strPassWord);
	PrintLog(strRet.GetBuffer(strRet.GetLength()),"TestExchange");

	if (strRet=="")
	{
		return FALSE;
	}
	else 
	{
		
		std::list<std::string> lstText;
		GetList(strRet.GetBuffer(strRet.GetLength()),"?",lstText);
		std::string strOut;

		for(std::list<string>::iterator It=lstText.begin();It!=lstText.end();It++)
		{
			strOut+= It->c_str();
			strOut+= "=";
			strOut+= It->c_str();
			strOut+= "\v";
		}

		int lsize =static_cast<int>(strOut.size());

		if(nSize>lsize)
		{
			sprintf(strReturn,"%s",strOut.c_str());
			char *p;
			p=strReturn;
			while(*p!='\0')
			{
				if(*p=='\v') *p='\0';
				p++;
			
			}
			nSize =static_cast<int>(strOut.size());
			return true;

		}
		else 
		{
			return false;
		}

		
	}
	
	return TRUE;

}

//ExchangeConnectorState
extern "C" _declspec(dllexport) BOOL ExchangeConnectorState(const char * strParas, char * szRet, int& nSize)
//(CStringList& lstParas, char* szRet)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strMachineName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	CStringList lstParas;
	MakeStringListByChar(lstParas,strParas);
	CString strConnectorName = GetValueFromList("_ConnectorName",lstParas);

	GetMachineInfo(lstParas,strMachineName,strUserAccount,strPassWord);

	CString strRet=_T("");
	strRet = GetExchangeConnectorStatebyCom(strMachineName,strUserAccount,strPassWord,strConnectorName);
	MakeCharByString(szRet,nSize,strRet);
	
	return TRUE;
}



//ExchangeQueueName
extern "C" _declspec (dllexport) 
BOOL NumExchangeQueue(const char * strParas, char * strReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	PrintLog("Query quene name!", "ExchangeQueueName");


	
	CString strMachineName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	CStringList paramList;
	MakeStringListByChar(paramList,strParas);
	GetMachineInfo(paramList,strMachineName,strUserAccount,strPassWord);


	CString strRet=_T("");
	strRet = GetExchangeQueueNamebyCom(strMachineName,strUserAccount,strPassWord);
	OutputDebugString(strRet.GetBuffer(strRet.GetLength()));

	if (strRet=="")
	{
		//lstTexts.AddTail("NULL");
		//lstValues.AddTail("NULL");
		return TRUE;
	}
	else 
	{
		std::list<std::string> lstText;
		GetList(strRet.GetBuffer(strRet.GetLength()),"?",lstText);
		std::string strOut;

		for(std::list<string>::iterator It=lstText.begin();It!=lstText.end();It++)
		{
			strOut+= It->c_str();
			strOut+= "=";
			strOut+= It->c_str();
			strOut+= "\v";
		}

		int lsize =static_cast<int>(strOut.size());

		if(nSize>lsize)
		{
			sprintf(strReturn,"%s",strOut.c_str());
			char *p;
			p=strReturn;
			while(*p!='\0')
			{
				if(*p=='\v') *p='\0';
				p++;
			
			}
			nSize =static_cast<int>(strOut.size());
			return true;

		}else return false;

	}

	return TRUE;

}

//ExchangeQueue
extern "C" _declspec(dllexport) BOOL ExchangeQueue(const char * strParas, char * szRet, int& nSize)
//(CStringList& lstParas, char* szRet)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strMachineName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	CStringList lstParas;
	MakeStringListByChar(lstParas,strParas);
	CString strQueueName = GetValueFromList("_QueueName",lstParas);

	GetMachineInfo(lstParas,strMachineName,strUserAccount,strPassWord);

	CString strRet=_T("");
	strRet = GetExchangeQueuebyCom(strMachineName,strUserAccount,strPassWord,strQueueName);
	MakeCharByString(szRet,nSize,strRet);
	
	return TRUE;
}


//ExchangeServerState 
extern "C" _declspec(dllexport) 
BOOL ExchangeServerState(const char * strParas, char * szRet, int& nSize)
//(CStringList& lstParas, char* szRet)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	PrintLog("========ExchangeServerState Start========","ExchangeServerState");

	CString strMachineName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	CStringList lstParas;
	MakeStringListByChar(lstParas,strParas);
	GetMachineInfo(lstParas,strMachineName,strUserAccount,strPassWord);
	
	CString strRet=_T("");
	strRet = GetExchangeServerStatebyCom(strMachineName,strUserAccount,strPassWord);

	if(strRet.Find("OK"))
		strRet.Replace("Unknown", "OK");
	else
		strRet.Replace("Unknown", "Warning");

	MakeCharByString(szRet,nSize,strRet);
	
	PrintLog("========ExchangeServerState End========","ExchangeServerState");
	return TRUE;

}


//ExchangeServer
extern "C" _declspec(dllexport) BOOL ExchangeServer(const char * strParas, char * szRet, int& nSize)
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strMachineName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	CStringList lstParas;
	MakeStringListByChar(lstParas,strParas);
	GetMachineInfo(lstParas,strMachineName,strUserAccount,strPassWord);
	
	CString strRet=_T("");
	strRet = GetExchangeServerbyCom(strMachineName,strUserAccount,strPassWord);

	MakeCharByString(szRet,nSize,strRet);
	
	return TRUE;

}

// SMTP2003
extern "C" _declspec(dllexport) BOOL SMTP2003(const char * strParas, char * szRet, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strMachineName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	CStringList lstParas;
	MakeStringListByChar(lstParas,strParas);
	GetMachineInfo(lstParas,strMachineName,strUserAccount,strPassWord);
	
	CString strRet=_T("");
	strRet = GetSMTP2003byCom(strMachineName,strUserAccount,strPassWord);

	MakeCharByString(szRet,nSize,strRet);
	
	return TRUE;
}

// MTA2003
extern "C" _declspec(dllexport) BOOL MTA2003(const char * strParas, char * szRet, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strMachineName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	CStringList lstParas;
	MakeStringListByChar(lstParas,strParas);
	GetMachineInfo(lstParas,strMachineName,strUserAccount,strPassWord);
	
	CString strRet=_T("");
	strRet = GetMTA2003byCom(strMachineName,strUserAccount,strPassWord);

	MakeCharByString(szRet,nSize,strRet);
	
	return TRUE;
}

// SMTP
extern "C" _declspec(dllexport) BOOL SMTP(const char * strParas, char * szRet, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strMachineName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	CStringList lstParas;
	MakeStringListByChar(lstParas,strParas);
	
	GetMachineInfo(lstParas,strMachineName,strUserAccount,strPassWord);
	
	CString strRet=_T("");
	strRet = GetSMTPbyCom(strMachineName,strUserAccount,strPassWord);

	MakeCharByString(szRet,nSize,strRet);
	
	return TRUE;
}

// WebMail
extern "C" _declspec(dllexport) BOOL WebMail(const char * strParas, char * szRet, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strMachineName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");

	CStringList lstParas;
	MakeStringListByChar(lstParas,strParas);
	GetMachineInfo(lstParas,strMachineName,strUserAccount,strPassWord);
	
	CString strRet=_T("");
	strRet = GetWebMailbyCom(strMachineName,strUserAccount,strPassWord);
	MakeCharByString(szRet,nSize,strRet);

	
	
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//子函数
CString GetExchangeMailboxNamebyCom(CString strServer, CString strUser,CString strPwd)
{
	CoInitialize(NULL);

	_Exchange *myRef = ThreeCreateDragonCom();	

	if (myRef <= 0)  
	{
		CoUninitialize();
		CString strResult=_T("");
		strResult.Format("%s","error=%s$",FuncGetStringFromIDS("<%IDS_NtPerfTest_29%>"));//<%IDS_NtPerfTest_29%>
		return strResult;
	}

	VARIANT  vServer,vUser,vPwd,vResult;
	BSTR bstrTmp;
	VariantInit(&vServer);
	VariantInit(&vUser);
	VariantInit(&vPwd);
	VariantInit(&vResult);
	
	bstrTmp = strServer.AllocSysString();
	vServer.vt = VT_BSTR;
	vServer.bstrVal = bstrTmp;
	
	bstrTmp = strUser.AllocSysString();
	vUser.bstrVal = bstrTmp;
	vUser.vt = VT_BSTR;

	bstrTmp = strPwd.AllocSysString();
	vPwd.bstrVal = bstrTmp;
	vPwd.vt = VT_BSTR;

	myRef->getMailboxName(&vServer,&vUser,&vPwd,&vResult);
	CString strResult(vResult.bstrVal);

	myRef->Release();
	myRef = NULL;
	return strResult;
}

CString GetExchangeMailboxbyCom(CString strServer, CString strUser,CString strPwd,CString strMailboxName)
{
	CoInitialize(NULL);

	_Exchange *myRef = ThreeCreateDragonCom();	

	if (myRef <= 0)  
	{
		CoUninitialize();
		CString strResult=_T("");
		strResult.Format("%s","error=%s$",FuncGetStringFromIDS("<%IDS_NtPerfTest_29%>"));//<%IDS_NtPerfTest_29%>
		return strResult;
	}

	VARIANT  vServer,vUser,vPwd,vMailboxName,vResult;

	BSTR bstrTmp;

	VariantInit(&vServer);
	VariantInit(&vUser);
	VariantInit(&vPwd);
	VariantInit(&vMailboxName);
	VariantInit(&vResult);
	
	bstrTmp = strServer.AllocSysString();
	vServer.vt = VT_BSTR;
	vServer.bstrVal = bstrTmp;
	
	bstrTmp = strUser.AllocSysString();
	vUser.bstrVal = bstrTmp;
	vUser.vt = VT_BSTR;

	bstrTmp = strPwd.AllocSysString();
	vPwd.bstrVal = bstrTmp;
	vPwd.vt = VT_BSTR;

	bstrTmp = strMailboxName.AllocSysString();
	vMailboxName.bstrVal = bstrTmp;
	vMailboxName.vt = VT_BSTR;

	myRef->getMailbox(&vServer,&vUser,&vPwd,&vMailboxName,&vResult);

	CString strResult(vResult.bstrVal);

	myRef->Release();
	myRef = NULL;
	CoUninitialize();

	return strResult;
}

CString GetExchangeConnectorNamebyCom(CString strServer, CString strUser,CString strPwd)
{
	CoInitialize(NULL);

	_Exchange *myRef = ThreeCreateDragonCom();	

	if (myRef <= 0)  
	{
		CoUninitialize();
		CString strResult=_T("");
		strResult.Format("%s","error=%s$",FuncGetStringFromIDS("<%IDS_NtPerfTest_29%>"));//<%IDS_NtPerfTest_29%>
		return strResult;
	}

	VARIANT  vServer,vUser,vPwd,vResult;
	BSTR bstrTmp;
	VariantInit(&vServer);
	VariantInit(&vUser);
	VariantInit(&vPwd);
	VariantInit(&vResult);
	
	bstrTmp = strServer.AllocSysString();
	vServer.vt = VT_BSTR;
	vServer.bstrVal = bstrTmp;
	
	bstrTmp = strUser.AllocSysString();
	vUser.bstrVal = bstrTmp;
	vUser.vt = VT_BSTR;

	bstrTmp = strPwd.AllocSysString();
	vPwd.bstrVal = bstrTmp;
	vPwd.vt = VT_BSTR;

	myRef->getExchangeConnectorName(&vServer,&vUser,&vPwd,&vResult);
	CString strResult(vResult.bstrVal);

	myRef->Release();
	myRef = NULL;
	return strResult;
}

CString GetExchangeConnectorStatebyCom(CString strServer, CString strUser,CString strPwd,CString strConnectorName)
{
	CoInitialize(NULL);

	_Exchange *myRef = ThreeCreateDragonCom();	

	if (myRef <= 0)  
	{
		CoUninitialize();
		CString strResult=_T("");
		strResult.Format("%s","error=%s$",FuncGetStringFromIDS("<%IDS_NtPerfTest_29%>"));//<%IDS_NtPerfTest_29%>
		return strResult;
	}

	VARIANT  vServer,vUser,vPwd,vConnectorName,vResult;

	BSTR bstrTmp;

	VariantInit(&vServer);
	VariantInit(&vUser);
	VariantInit(&vPwd);
	VariantInit(&vConnectorName);
	VariantInit(&vResult);
	
	bstrTmp = strServer.AllocSysString();
	vServer.vt = VT_BSTR;
	vServer.bstrVal = bstrTmp;
	
	bstrTmp = strUser.AllocSysString();
	vUser.bstrVal = bstrTmp;
	vUser.vt = VT_BSTR;

	bstrTmp = strPwd.AllocSysString();
	vPwd.bstrVal = bstrTmp;
	vPwd.vt = VT_BSTR;

	bstrTmp = strConnectorName.AllocSysString();
	vConnectorName.bstrVal = bstrTmp;
	vConnectorName.vt = VT_BSTR;

	myRef->getExchangeConnectorState(&vServer,&vUser,&vPwd,&vConnectorName,&vResult);

	CString strResult(vResult.bstrVal);

	myRef->Release();
	myRef = NULL;
	CoUninitialize();

	return strResult;
}


//ExchangeQueueName
CString GetExchangeQueueNamebyCom(CString strServer, CString strUser,CString strPwd)
{
	CoInitialize(NULL);

	_Exchange *myRef = ThreeCreateDragonCom();	

	if (myRef <= 0)  
	{
		CoUninitialize();
		CString strResult=_T("");
		strResult.Format("%s","error=%s$",FuncGetStringFromIDS("<%IDS_NtPerfTest_29%>"));//<%IDS_NtPerfTest_29%>
		return strResult;
	}

	VARIANT  vServer,vUser,vPwd,vResult;
	BSTR bstrTmp;
	VariantInit(&vServer);
	VariantInit(&vUser);
	VariantInit(&vPwd);
	VariantInit(&vResult);
	
	bstrTmp = strServer.AllocSysString();
	vServer.vt = VT_BSTR;
	vServer.bstrVal = bstrTmp;
	
	bstrTmp = strUser.AllocSysString();
	vUser.bstrVal = bstrTmp;
	vUser.vt = VT_BSTR;

	bstrTmp = strPwd.AllocSysString();
	vPwd.bstrVal = bstrTmp;
	vPwd.vt = VT_BSTR;

	myRef->getExchangeQueueName(&vServer,&vUser,&vPwd,&vResult);
	CString strResult(vResult.bstrVal);

	myRef->Release();
	myRef = NULL;

	return strResult;
}

//ExchangeQueue
CString GetExchangeQueuebyCom(CString strServer, CString strUser,CString strPwd,CString strExchangeQueueName)
{
	CoInitialize(NULL);

	_Exchange *myRef = ThreeCreateDragonCom();	

	if (myRef <= 0)  
	{
		CoUninitialize();
		CString strResult=_T("");
		strResult.Format("%s","error=%s$",FuncGetStringFromIDS("<%IDS_NtPerfTest_29%>"));//<%IDS_NtPerfTest_29%>
		return strResult;
	}

	VARIANT  vServer,vUser,vPwd,vQueueName,vResult;

	BSTR bstrTmp;

	VariantInit(&vServer);
	VariantInit(&vUser);
	VariantInit(&vPwd);
	VariantInit(&vQueueName);
	VariantInit(&vResult);
	
	bstrTmp = strServer.AllocSysString();
	vServer.vt = VT_BSTR;
	vServer.bstrVal = bstrTmp;
	
	bstrTmp = strUser.AllocSysString();
	vUser.bstrVal = bstrTmp;
	vUser.vt = VT_BSTR;

	bstrTmp = strPwd.AllocSysString();
	vPwd.bstrVal = bstrTmp;
	vPwd.vt = VT_BSTR;

	bstrTmp = strExchangeQueueName.AllocSysString();
	vQueueName.bstrVal = bstrTmp;
	vQueueName.vt = VT_BSTR;

	myRef->getExchangeQueue(&vServer,&vUser,&vPwd,&vQueueName,&vResult);

	CString strResult(vResult.bstrVal);

	myRef->Release();
	myRef = NULL;
	CoUninitialize();

	return strResult;
}


//ExchangeServerState
CString GetExchangeServerStatebyCom(CString strServer, CString strUser,CString strPwd)
{
	CoInitialize(NULL);
	PrintLog("********GetExchangeServerStatebyCom Start!********","ExchangeServerState");

	_Exchange *myRef = ThreeCreateDragonCom();	

	if (myRef <= 0)  
	{
		CoUninitialize();
		CString strResult=_T("");
		strResult.Format("%s","error=%s$","myRef is null");//<%IDS_NtPerfTest_29%>
		return strResult;
	}

	VARIANT  vServer,vUser,vPwd,vResult;
	BSTR bstrTmp;
	VariantInit(&vServer);
	VariantInit(&vUser);
	VariantInit(&vPwd);
	VariantInit(&vResult);
	
	bstrTmp = strServer.AllocSysString();
	vServer.vt = VT_BSTR;
	vServer.bstrVal = bstrTmp;
	
	bstrTmp = strUser.AllocSysString();
	vUser.bstrVal = bstrTmp;
	vUser.vt = VT_BSTR;

	bstrTmp = strPwd.AllocSysString();
	vPwd.bstrVal = bstrTmp;
	vPwd.vt = VT_BSTR;


	myRef->getExchangeServerState(&vServer,&vUser,&vPwd,&vResult);
	CString strResult(vResult.bstrVal);

	PrintLog(strResult.GetBuffer(strResult.GetLength()),"ExchangeServerState");

	myRef->Release();
	myRef = NULL;
	PrintLog("********GetExchangeServerStatebyCom End!********","ExchangeServerState");
	return strResult;
}


//ExchangeServer
CString GetExchangeServerbyCom(CString strServer, CString strUser,CString strPwd)
{
	CoInitialize(NULL);

	_Exchange *myRef = ThreeCreateDragonCom();	

	if (myRef <= 0)  
	{
		CoUninitialize();
		CString strResult=_T("");
		strResult.Format("%s","error=%s$","myRef is null");//<%IDS_NtPerfTest_29%>
		return strResult;
	}

	VARIANT  vServer,vUser,vPwd,vResult;
	BSTR bstrTmp;
	VariantInit(&vServer);
	VariantInit(&vUser);
	VariantInit(&vPwd);
	VariantInit(&vResult);
	
	bstrTmp = strServer.AllocSysString();
	vServer.vt = VT_BSTR;
	vServer.bstrVal = bstrTmp;
	
	bstrTmp = strUser.AllocSysString();
	vUser.bstrVal = bstrTmp;
	vUser.vt = VT_BSTR;

	bstrTmp = strPwd.AllocSysString();
	vPwd.bstrVal = bstrTmp;
	vPwd.vt = VT_BSTR;

	myRef->getPOP3(&vServer,&vUser,&vPwd,&vResult);
	CString strResult(vResult.bstrVal);

	myRef->Release();
	myRef = NULL;
	return strResult;

}


// WebMail
CString GetWebMailbyCom(CString strServer, CString strUser,CString strPwd)
{
	CoInitialize(NULL);

	_Exchange *myRef = ThreeCreateDragonCom();	

	if (myRef <= 0)  
	{
		CoUninitialize();
		CString strResult=_T("");
		strResult.Format("%s","error=%s$","myRef is null");//<%IDS_NtPerfTest_29%>
		return strResult;
	}

	VARIANT  vServer,vUser,vPwd,vResult;
	BSTR bstrTmp;
	VariantInit(&vServer);
	VariantInit(&vUser);
	VariantInit(&vPwd);
	VariantInit(&vResult);
	
	bstrTmp = strServer.AllocSysString();
	vServer.vt = VT_BSTR;
	vServer.bstrVal = bstrTmp;
	
	bstrTmp = strUser.AllocSysString();
	vUser.bstrVal = bstrTmp;
	vUser.vt = VT_BSTR;


	bstrTmp = strPwd.AllocSysString();
	vPwd.bstrVal = bstrTmp;
	vPwd.vt = VT_BSTR;

	myRef->getWebMail(&vServer,&vUser,&vPwd,&vResult);
	CString strResult(vResult.bstrVal);

	myRef->Release();
	myRef = NULL;
	return strResult;

}

// SMTP
CString GetSMTPbyCom(CString strServer, CString strUser,CString strPwd)
{
	CoInitialize(NULL);

	_Exchange *myRef = ThreeCreateDragonCom();	

	if (myRef <= 0)  
	{
		CoUninitialize();
		CString strResult=_T("");
		strResult.Format("%s","error=%s$","myRef is null");//<%IDS_NtPerfTest_29%>
		return strResult;
	}

	VARIANT  vServer,vUser,vPwd,vResult;
	BSTR bstrTmp;
	VariantInit(&vServer);
	VariantInit(&vUser);
	VariantInit(&vPwd);
	VariantInit(&vResult);
	
	bstrTmp = strServer.AllocSysString();
	vServer.vt = VT_BSTR;
	vServer.bstrVal = bstrTmp;
	
	bstrTmp = strUser.AllocSysString();
	vUser.bstrVal = bstrTmp;
	vUser.vt = VT_BSTR;

	bstrTmp = strPwd.AllocSysString();
	vPwd.bstrVal = bstrTmp;
	vPwd.vt = VT_BSTR;

	myRef->getSMTP(&vServer,&vUser,&vPwd,&vResult);
	CString strResult(vResult.bstrVal);

	myRef->Release();
	myRef = NULL;
	return strResult;

}

//SMTP2003
CString GetSMTP2003byCom(CString strServer, CString strUser,CString strPwd)
{
	CoInitialize(NULL);

	_Exchange *myRef = ThreeCreateDragonCom();	

	if (myRef <= 0)  
	{
		CoUninitialize();
		CString strResult=_T("");
		strResult.Format("%s","error=%s$","myRef is null");//<%IDS_NtPerfTest_29%>
		return strResult;
	}

	VARIANT  vServer,vUser,vPwd,vResult;
	BSTR bstrTmp;
	VariantInit(&vServer);
	VariantInit(&vUser);
	VariantInit(&vPwd);
	VariantInit(&vResult);
	
	bstrTmp = strServer.AllocSysString();
	vServer.vt = VT_BSTR;
	vServer.bstrVal = bstrTmp;
	
	bstrTmp = strUser.AllocSysString();
	vUser.bstrVal = bstrTmp;
	vUser.vt = VT_BSTR;

	bstrTmp = strPwd.AllocSysString();
	vPwd.bstrVal = bstrTmp;
	vPwd.vt = VT_BSTR;

	myRef->getSMTPServer(&vServer,&vUser,&vPwd,&vResult);
	CString strResult(vResult.bstrVal);

	myRef->Release();
	myRef = NULL;
	return strResult;

}


//MTA2003
CString GetMTA2003byCom(CString strServer, CString strUser,CString strPwd)
{
	CoInitialize(NULL);

	_Exchange *myRef = ThreeCreateDragonCom();	

	if (myRef <= 0)  
	{
		CoUninitialize();
		CString strResult=_T("");
		strResult.Format("%s","error=%s$","myRef is null");//<%IDS_NtPerfTest_29%>
		return strResult;
	}

	VARIANT  vServer,vUser,vPwd,vResult;
	BSTR bstrTmp;
	VariantInit(&vServer);
	VariantInit(&vUser);
	VariantInit(&vPwd);
	VariantInit(&vResult);
	
	bstrTmp = strServer.AllocSysString();
	vServer.vt = VT_BSTR;
	vServer.bstrVal = bstrTmp;
	
	bstrTmp = strUser.AllocSysString();
	vUser.bstrVal = bstrTmp;
	vUser.vt = VT_BSTR;

	bstrTmp = strPwd.AllocSysString();
	vPwd.bstrVal = bstrTmp;
	vPwd.vt = VT_BSTR;

	myRef->getMTA(&vServer,&vUser,&vPwd,&vResult);
	CString strResult(vResult.bstrVal);

	myRef->Release();
	myRef = NULL;
	return strResult;

}

//POP3
CString GetPOP3byCom(CString strServer, CString strUser,CString strPwd)
{
	CoInitialize(NULL);

	_Exchange *myRef = ThreeCreateDragonCom();	

	if (myRef <= 0)  
	{
		CoUninitialize();
		CString strResult=_T("");
		strResult.Format("%s","error=%s$","myRef is null");//<%IDS_NtPerfTest_29%>
		return strResult;
	}

	VARIANT  vServer,vUser,vPwd,vResult;
	BSTR bstrTmp;
	VariantInit(&vServer);
	VariantInit(&vUser);
	VariantInit(&vPwd);
	VariantInit(&vResult);
	
	bstrTmp = strServer.AllocSysString();
	vServer.vt = VT_BSTR;
	vServer.bstrVal = bstrTmp;
	
	bstrTmp = strUser.AllocSysString();
	vUser.bstrVal = bstrTmp;
	vUser.vt = VT_BSTR;

	bstrTmp = strPwd.AllocSysString();
	vPwd.bstrVal = bstrTmp;
	vPwd.vt = VT_BSTR;

	myRef->getPOP3(&vServer,&vUser,&vPwd,&vResult);
	CString strResult(vResult.bstrVal);

	myRef->Release();
	myRef = NULL;
	return strResult;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//公共函数

BOOL GetList(char *pValue,char *pMark,std::list<std::string> &lstText)
{
	char *p = pValue;
	char *p1 = strstr(p,pMark);
	int nLen = 0;
	
	CString sTemp;
	while(p1)
	{
		char temp[128] = {0};
		nLen = static_cast<int>(p1 - p);
		strncpy(temp,p,nLen);

		sTemp = temp;
		lstText.push_back(temp);
		//lstText.AddTail(sTemp);
		//lstValue.AddTail(sTemp);
	
		if(strlen(p1) <=1)
			break;

		p1++;
		p = p1;
		p1 = strstr(p,pMark);
	}

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//COM函数

BOOL GetMachineInfo(CStringList& lstParas,CString &strMachineName,CString& strUserAccount,CString& strPassWord)
{
	CString strMachineType=_T("");

	strMachineType = GetValueFromList("_MachineType",lstParas);
	strMachineName = GetValueFromList("_MachineName",lstParas);
	strUserAccount = GetValueFromList("_UserAccount",lstParas);
	strPassWord = GetValueFromList("_PassWord",lstParas);
		
	if (strMachineName=="")
	{
		strMachineName = GetValueFromList("_Server",lstParas);
		return FALSE;
	}

	if((strMachineName.CompareNoCase("localhost")==0)||(strMachineName.CompareNoCase("127.0.0.1")==0))
	{
		strMachineName="";
		strUserAccount="";
		strPassWord="";
	}

	return TRUE;
}


int GetVersionFromCom(CString strServer, CString strUser,CString strPwd)
{
	
	CoInitialize(NULL);
	_Exchange *myRef = NULL;

	HRESULT hr = 0;	
	
	myRef  = ThreeCreateDragonCom(); 

	if (NULL== myRef  )  
		hr=-200;

	if (SUCCEEDED(hr))
	{

		VARIANT  a,b,c,d,vResult;
		BSTR bstrTmp;
		VariantInit(&a);
		VariantInit(&b);
		VariantInit(&c);
		VariantInit(&vResult);
		
		bstrTmp=strServer.AllocSysString();
		a.vt=VT_BSTR;
		a.bstrVal=bstrTmp;
		
		bstrTmp=strUser.AllocSysString();
		b.bstrVal=bstrTmp;
		b.vt=VT_BSTR;

		bstrTmp=strPwd.AllocSysString();
		c.bstrVal=bstrTmp;
		c.vt=VT_BSTR;

		CString strDisk=_T(" Win32_OperatingSystem where Primary=true");
		
		bstrTmp=strDisk.AllocSysString();
		d.bstrVal=bstrTmp;
		d.vt=VT_BSTR;

		//mtx???
		//myRef->getSystemInfo(&a,&b,&c,&d,&vResult);

		CString strResult(vResult.bstrVal);

		myRef->Release();
		myRef = NULL;

		CoUninitialize();

		int nVersion=0;

		nVersion = atoi(strResult);

		if(nVersion == 0) 
			nVersion = 2;
		else if(nVersion == 4) 
			nVersion=0;

		return nVersion;
	}

	CoUninitialize();

	return 5;
}

_Exchange * ThreeCreateDragonCom()
{
	HRESULT hr;
	_Exchange *myRef = NULL;
	div_t div_result;
	srand(1); 
	int i=0;

	CString szMutex = _T("AIM4.0-Exchange-DLL-RUN");
	int nInstance = 6;
	HANDLE hMutex = NULL;
	hMutex = ::CreateFileMapping((HANDLE)0xFFFFFFFFF, NULL, PAGE_READWRITE, 0, 10, szMutex);
		
	if (hMutex == NULL) 
		return NULL;

	DWORD dwLastError = GetLastError();
	int nTemp=0;
	srand(1);

	for(int lv=0 ;lv<6 ;lv++)
	{
		int* lpvFile = (int*)::MapViewOfFile(hMutex, FILE_MAP_WRITE, 0, 0, 0);

		nTemp = lpvFile[0];
		if (nTemp > (nInstance - 1)) 
		{
			Sleep(5000);
			continue;	
		}
		else 
		{
			lpvFile[0] = nTemp+1;
			::UnmapViewOfFile(lpvFile);
			break;
		}
	}

	if (nTemp > (nInstance - 1)) 
	{			
		goto Fin;
	}


	for ( i=0;i<6;i++)
	{
		 hr=CoInitialize(NULL);
		 
		 if (SUCCEEDED(hr))
			 break;

		 div_result = div(rand(),3);

		 Sleep((5+div_result.rem)*1000);
	}

	if (FAILED(hr))
	{
		CString str;
		CTime tnNow =CTime::GetCurrentTime();
		str.Format(" CoInitialize failed %s",TimeToString(tnNow) );
		AddToExceptionLog(str);	
		
		if (hMutex) {
			if (nInstance > 1) 
			{
				int* lpvFile = (int*)::MapViewOfFile(hMutex, FILE_MAP_WRITE, 0, 0, 0);
				int nTemp = lpvFile[0];
				lpvFile[0] = nTemp - 1;
				::UnmapViewOfFile(lpvFile);
			}
			CloseHandle(hMutex); 
			hMutex = NULL; 
		}
		return NULL;
	}

	for ( i=0;i<6;i++)
	{
		hr=CoCreateInstance(CLSID_Exchange,NULL,
							CLSCTX_ALL,
							IID__Exchange,(void **)&myRef);

		 //hr=CoCreateInstance(CLSID_Exchange,NULL,CLSCTX_ALL,IID__Exchange,(void **)&myRef);
		 
		 if (SUCCEEDED(hr))
			 break;

		 div_result = div(rand(),3);

		 Sleep((5+div_result.rem)*1000);
	}

	if (FAILED(hr)) 
	{
		CString str;
		CTime tnNow =CTime::GetCurrentTime();
		str.Format("use com failed %s",TimeToString(tnNow) );
		AddToExceptionLog(str);	
		myRef= NULL;
	}

Fin:
	if (hMutex) 
	{
		if (nInstance > 1) 
		{
			int* lpvFile = (int*)::MapViewOfFile(hMutex, FILE_MAP_WRITE, 0, 0, 0);
			int nTemp = lpvFile[0];
			lpvFile[0] = nTemp - 1;
			::UnmapViewOfFile(lpvFile);
		}
		CloseHandle(hMutex); 
		hMutex = NULL; 
	}

	return myRef;
}

BOOL AddToExceptionLog(const char* szMsg)
{
	FILE *logfile = NULL;

	CString strFileName = _T("");
	strFileName.Format("d:\\ntperf-exception.xlog");
	
	logfile=fopen((LPCTSTR)strFileName,"a+");

	if (!logfile)
		return FALSE;

	fprintf(logfile,"%s\r\n",szMsg);

	fclose(logfile);
	return TRUE;
}
extern "C" __declspec(dllexport)
BOOL TestExchangeConn(const char * strParas, char * szReturn , int &nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = true;

	CString strMachineName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	CString strServer = _T("");
	CString strUser = _T("");
	CString strPwd = _T("");
	int BeginTime=0,EndTime=0;
	CStringList lstParas;
	MakeStringListByChar(lstParas,strParas);
	GetMachineInfo(lstParas,strMachineName,strUserAccount,strPassWord);

    CoInitialize(NULL);
	try
	{
		BeginTime=::GetTickCount();
		_Exchange *myRef = ThreeCreateDragonCom();	

		if (myRef <= 0)  
		{
			CoUninitialize();
			CString strResult=_T("");
			strResult.Format("%s","error=%s$",FuncGetStringFromIDS("<%IDS_NtPerfTest_29%>"));//<%IDS_NtPerfTest_29%>
			myRef->Release();
			myRef = NULL;
			return false;
		}
		myRef->Release();
		myRef = NULL;
		EndTime=::GetTickCount();
		sprintf(szReturn,"status=OK$ConnectTime=%d",EndTime-BeginTime);
	}
	catch(...)
	{
		sprintf(szReturn, "error=%s$","Error occured in Process!");
		bRet = false;
	}
	CoUninitialize();
	CString strInput = szReturn;
	MakeCharByString(szReturn,nSize, strInput);
	return bRet;
}