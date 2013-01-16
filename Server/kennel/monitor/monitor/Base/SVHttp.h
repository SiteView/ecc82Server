// SVHttp.h: interface for the SVHttp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SVHTTP_H__07B9E642_6CEA_4034_A42B_8F517A321DAC__INCLUDED_)
#define AFX_SVHTTP_H__07B9E642_6CEA_4034_A42B_8F517A321DAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once

#endif // _MSC_VER > 1000

#ifndef _SVHTTP_H
#define _SVHTTP_H
class SVHttp
{
public:
	SVHttp();
	virtual ~SVHttp();

	void Init(){
		m_strHeader = "";
		m_strPost = "";
		m_strProxyIP = "";
		m_strProxyPort = "";
		m_strProxyUser = "";
		m_strProxyPwd = "";
		m_strUserName = "";
		m_strUserPwd = "";
		m_strResponsText = "";
		m_strError = "";
		m_nStatusCode = 0;
		m_strResponsHeader = "";
		m_strUrl = "";
		m_nErrorCode = 0;
		m_nPageSize = 0;
		m_dwResponseTime = 0;
	}

	CString GetHttpHeader(){
		return m_strHeader;
	}
	void SetHttpHeader(CString strKeyName, CString strKeyValue){
		if (strKeyValue == _T(""))
		{
			return;
		}
		CString strTmp = _T("");
		strTmp.Format("%s: %s", strKeyName, strKeyValue);
		m_strHeader += strTmp;
		m_strHeader += "\r\n";
	}
	void ClearHeader(){
		m_strHeader = "";
	}

	CString GetPostData(){
		return m_strPost;
	}
	void SetPostData(CString strPost){
		m_strPost = strPost;
	}

	CString GetHttpUser(){
		return m_strUserName;
	}
	void SetHttpUser(CString strUserName){
		m_strUserName = strUserName;
	}

	CString GetHttpPwd(){
		return m_strUserPwd;
	}
	void SetHttpPwd(CString strUserPwd){
		m_strUserPwd = strUserPwd;
	}

	CString GetResponseText(){
		return m_strResponsText;
	}

	CString GetResponseHeader(){
		return m_strResponsHeader;
	}

	CString GetErrorMessage(){
		return m_strError;
	}

	int GetStatusCode(){
		return m_nStatusCode;
	}

	CString GetUrl(){
		return m_strUrl;
	}
	void SetUrl(CString strUrl){
		m_strUrl = strUrl;
	}

	int GetErrorCode(){
		return m_nErrorCode;
	}

	int GetPageSize(){
		return m_nPageSize;
	}

	double GetResponseTime(){
		return m_dwResponseTime;
	}

	//Cxy 2005.01.25 新增

	void SetProxyUser(CString strIn)
	{
		m_strProxyUser = strIn;
	}

	void SetProxyPwd(CString strIn)
	{
		m_strProxyPwd = strIn;
	}

	void SetProxyIp(CString strIn)
	{
		m_strProxyIP = strIn;
	}

	void SetProxyPort(CString strIn)
	{
		m_strProxyPort = strIn;
	}
	void SetTimeOut(int nTimeOut)
	{
		m_nTimeOut = nTimeOut;
	}


	BOOL SendRequest(CString strUrl);
	BOOL SendRequest();

private:
	CString m_strHeader;
	CString m_strPost;
	CString m_strProxyIP;
	CString m_strProxyPort;
	CString m_strProxyUser;
	CString m_strProxyPwd;
	CString m_strUserName;
	CString m_strUserPwd;
	CString m_strResponsText;
	CString m_strError;
	int m_nStatusCode;
	CString m_strResponsHeader;
	CString m_strUrl;
	int m_nErrorCode;
	int m_nPageSize;
	int m_nTimeOut;// 单位 ：s
	double m_dwResponseTime;
};

#endif // !defined(AFX_SVHTTP_H__07B9E642_6CEA_4034_A42B_8F517A321DAC__INCLUDED_)
#endif