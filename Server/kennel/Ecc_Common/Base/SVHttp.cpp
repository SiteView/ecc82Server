// SVHttp.cpp: implementation of the SVHttp class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include  <wininet.h>
#include <AfxInet.h>
#include "SVHttp.h"
#include "Base64.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SVHttp::SVHttp()
{
	//SetHttpHeader("Content-Type", "application/x-www-form-urlencoded");
}

SVHttp::~SVHttp()
{
}

BOOL SVHttp::SendRequest(CString strUrl)
{
	SetUrl(strUrl);
	return SendRequest();
}

BOOL SVHttp::SendRequest()
{
	if(m_strUrl.IsEmpty()) 
	{//Url is Empty
		m_strError = "The URL is empty";
		m_nErrorCode = ::GetLastError();//Get Last Error
		return FALSE;//Return False
	}

	//DWORD dwError=0;
	//CString strError=_T("");
	CString strTmpErr = _T("");
	DWORD dwStart = ::GetTickCount();

	int	nTimeout = 60;

	CString strServerName;
	CString strObject;
	INTERNET_PORT nPort;
	DWORD dwServiceType;

	HINTERNET	hInet;
	HINTERNET	hConn;//Http Connect Handle
	HINTERNET	hHttpReq;//Http Request Handle

	int ret = 0;

	//分割URL
	if (!AfxParseURL(m_strUrl, dwServiceType, strServerName, strObject, nPort) ||
		dwServiceType != INTERNET_SERVICE_HTTP)
	{//分割失败
		m_strError.Format("Parsered URL failed,URL=%s.", m_strUrl);
		m_nErrorCode = ::GetLastError();//Get Last Error
		return FALSE;
	}

	//代理服务器Ip与端口
	CString strProxy = _T("");
	if (!m_strProxyIP.IsEmpty())
		if (!m_strProxyPort.IsEmpty())
			strProxy.Format("%s:%s", m_strProxyIP, m_strProxyPort);
	

	if(!strProxy.IsEmpty())
		hInet = InternetOpen("http generic", INTERNET_OPEN_TYPE_PROXY , strProxy, 
							"<local>", 1);
	else
		hInet = InternetOpen("http generic", 0, NULL, NULL, 1);

	if(!hInet)
	{//Open Internet Faild
		m_strError = "Open Internet Faild";
		m_nErrorCode = ::GetLastError();//Get Last Error
		return FALSE;//Return Faild
	}
	
	int ms = 10000; // in milliseconds
	DWORD mslength;
	ms =5000;
	mslength=4;
/*
	::InternetQueryOption(hInet,INTERNET_OPTION_CONNECT_TIMEOUT ,&ms,&mslength);
	ms =5000;
	::InternetSetOption(hInet,INTERNET_OPTION_CONNECT_TIMEOUT ,&ms,sizeof(ms));
	::InternetQueryOption(hInet,INTERNET_OPTION_CONNECT_TIMEOUT ,&ms,&mslength);
*/
	if(!(m_strUserName.IsEmpty()))
		hConn = InternetConnect(hInet, strServerName, nPort, m_strUserName, m_strUserPwd, 
								INTERNET_SERVICE_HTTP , 0, 0);
	else
		hConn = InternetConnect(hInet, strServerName, nPort, NULL, NULL, 
								INTERNET_SERVICE_HTTP , 0, 0);

	if(!hConn)
	{//连接失败
		m_nErrorCode=::GetLastError();//得到最后一次错误
		m_strError.Format("Connect to Internet failed, errorcode = %d, URL=%s", m_nErrorCode, m_strUrl);
		InternetCloseHandle(hInet);//关闭句柄
		return FALSE;//Faild
	}

	if(m_strPost.IsEmpty())//Post Data is Empty, Open Request By Get Mode
		hHttpReq = HttpOpenRequest(hConn, "GET", strObject, "HTTP/1.0", NULL, 0, 
								INTERNET_FLAG_RELOAD|INTERNET_FLAG_NO_AUTO_REDIRECT 
								, 0);
	else//Post Data is not Empty , Open Request By POST Mode
		hHttpReq = HttpOpenRequest(hConn, "POST", strObject, "HTTP/1.0", NULL, 0, 
								INTERNET_FLAG_RELOAD|INTERNET_FLAG_NO_AUTO_REDIRECT 
								, 0);

	if(hHttpReq)
	{//Open HTTP Request Successed
		ms =5000;
		mslength=4;
		::InternetQueryOption(hHttpReq,INTERNET_OPTION_RECEIVE_TIMEOUT,&ms,&mslength);
		//ms =180000;
		ms =nTimeout*1000;
		::InternetSetOption(hHttpReq,INTERNET_OPTION_RECEIVE_TIMEOUT,&ms,sizeof(ms));
		::InternetQueryOption(hHttpReq,INTERNET_OPTION_RECEIVE_TIMEOUT,&ms,&mslength);

		::InternetQueryOption(hHttpReq,INTERNET_OPTION_SEND_TIMEOUT,&ms,&mslength);
		//ms =180000;
		ms =nTimeout*1000;
		
		::InternetSetOption(hHttpReq,INTERNET_OPTION_SEND_TIMEOUT,&ms,sizeof(ms));
		::InternetQueryOption(hHttpReq,INTERNET_OPTION_SEND_TIMEOUT,&ms,&mslength);
	//	printf("http timeout =%d\r\n ",ms);

		BOOL bResult = FALSE;
		
		//代理服务器验证
		if(!m_strUserName.IsEmpty())
		{
			CString	strUserPass = _T("");
			strUserPass.Format("%s:%s", m_strUserName, m_strUserPwd);

			CString	strOutput = _T("Proxy-Authorization: Basic ");

			CBase64	*pBase64 = new CBase64();
			strOutput.Insert(strOutput.GetLength(), pBase64->Encode(strUserPass, strUserPass.GetLength()));
			delete pBase64;
			pBase64 = NULL;
			
			strOutput += _T("\r\n");

			if(!HttpAddRequestHeaders(hHttpReq, strOutput, -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE))
			{
				m_nErrorCode = ::GetLastError();//Get Last Error				
				m_strError.Format("InternetConnect failed, errorcode=%d, URL=%s", m_nErrorCode, m_strUrl);
//				strError=::GetErrorMessage(dwError);
//				sprintf(szReturn,"error=InternetConnect:%s$",strError);
				InternetCloseHandle(hHttpReq);
				InternetCloseHandle(hConn);
				InternetCloseHandle(hInet);
				return FALSE;
			}
		}
		
		//设定http header
//		CString strContentType = _T("");
//		strContentType.Format("User-Agent: Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)\r\nContent-Type: application/x-www-form-urlencoded");
//		WriteLog(strContentType, 201, "c:\\headlog.txt");
		int nTime=0;
LOOP://循环调用
		if(m_strPost.IsEmpty())//Post Data Is Empty
		{
			bResult = HttpSendRequest(hHttpReq, m_strHeader, m_strHeader.GetLength(),
									NULL, 0);
//			bResult = HttpSendRequest(hHttpReq, NULL, 0, NULL, 0);
		}

		else
			bResult = HttpSendRequest(hHttpReq, m_strHeader, m_strHeader.GetLength(), 
									(void *)(LPCTSTR)m_strPost, m_strPost.GetLength());
		
		if(bResult)
		{
			char	sBuffer[1024]={0};
			DWORD	lBuffer = 1024;
			int status = 0;
			int nPageSize = 0;
            //Query Request Information(状态码)
			if(!HttpQueryInfo(hHttpReq, HTTP_QUERY_STATUS_CODE, &sBuffer, &lBuffer, 
							NULL))
			{//Failed
				m_nErrorCode = ::GetLastError();//Get Last Error				
				m_strError.Format("Query info failed, errorcode=%d, URL=%s", m_nErrorCode, m_strUrl);
				InternetCloseHandle(hHttpReq);//Close Request Handle
				InternetCloseHandle(hConn);//Close Connection Handle
				InternetCloseHandle(hInet);//Close Internet handle
				return FALSE;//Failed
			}
			else
			{
				m_nStatusCode = atoi(sBuffer);//状态码
				
			}
			memset(sBuffer, 0, 1024);//Set Memory Data	
			lBuffer=1024;
			if(HttpQueryInfo(hHttpReq, HTTP_QUERY_RAW_HEADERS_CRLF , &sBuffer, &lBuffer, 
							NULL))
			{
				m_strResponsHeader.Format("%s",sBuffer);
			}

			memset(sBuffer, 0, 1024);//Set Memory Data
			lBuffer = 1023;
			BOOL bRead = FALSE;
			DWORD lbegin = 0;
            //Read Http Content Form Request
			bRead = InternetReadFile(hHttpReq,  sBuffer, lBuffer, &lbegin);
			while(bRead) 
            {//Read All Http Request File
				if((0 == lbegin)&&(TRUE == bRead)) 
					break;
				sBuffer[lbegin] = '\0';
                m_strResponsText += sBuffer;
				bRead = InternetReadFile(hHttpReq, sBuffer, lBuffer, &lbegin);
			}
			m_nPageSize = m_strResponsText.GetLength();			
		}
		else
		{
			m_nErrorCode = ::GetLastError();//Get Last Error
			if(nTime < 1)
			{//循环次数小于2
				nTime++;//增加次数
                Sleep(5000);//休眠1秒
				goto LOOP;//循环
			}
			m_strError.Format("errorcode=%d,URL=%s", m_nErrorCode, m_strUrl);
			InternetCloseHandle(hHttpReq);//Close Requst Handle
			InternetCloseHandle(hConn);//Close Connection Handle
			InternetCloseHandle(hInet);//Close Internet Handle
			hHttpReq = NULL;//Set Request Handle as NULL
			hConn = NULL;//Set Connction Handle as NULL
			hInet = NULL;//Set Internet Handle as NULL
			return FALSE;// Return Failed
		}
	}
	else
	{
		m_nErrorCode = ::GetLastError();
		m_strError.Format("errorcode=%d,URL=%s", m_nErrorCode, m_strUrl);
		InternetCloseHandle(hConn);//Close Connection Handle
		InternetCloseHandle(hInet);//Close Internet Handle
		return FALSE;
	}
	InternetCloseHandle(hHttpReq);//Close Requst Handle
	InternetCloseHandle(hConn);//Close Connection Handle
	InternetCloseHandle(hInet);//Close Internet Handle
	
	DWORD dwEnd = ::GetTickCount();
	m_dwResponseTime = (dwEnd - dwStart)*1.0/1000;
    
	return TRUE;
}