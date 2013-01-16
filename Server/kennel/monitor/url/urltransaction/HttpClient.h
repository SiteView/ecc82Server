//////////////////////////////////////////////////////////////////////
// Company: 北京游龙科技
// HttpClient.h: interface for the CHttpClient class.
// If this file running Without Warnning and Error,
// This File is Coding By KaiYang.If it has bug, I don't 
// know who coding this file.
// date: 2004-11-28
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTTPCLIENT_H__3ECA8275_BBA6_43B0_A57C_CCB8F884DCC6__INCLUDED_)
#define AFX_HTTPCLIENT_H__3ECA8275_BBA6_43B0_A57C_CCB8F884DCC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <iostream>
#include <queue>
#include <functional>

using namespace std;

#include <afxinet.h>
#define MAX_BUF 255

class CHttpClient  
{
    typedef struct _HttpClientConfig
    {
        char chProxyServer[MAX_BUF];//代理服务器名称
        char chProxyUser[MAX_BUF];//代理服务器使用用户名
        char chProxyPwd[MAX_BUF];//代理服务器验证密码
        int nProxyPort;//代理服务器端口
        int nTimeOut;//超时时间
    }HttpConfig;//Http Client Config
/////////////////////////////////////////////////////////////////////////////////
    typedef struct _RequestStep
    {
        char szUrl[MAX_BUF];//Url Address
        char szPara[MAX_BUF];//Post Para
        char szMatch[MAX_BUF];//匹配字符串
		char szRevMatch[MAX_BUF];//反匹配字符串
        int nType;//请求类型
    }RequestStep;//请求步骤
/////////////////////////////////////////////////////////////////////////////////
    typedef struct _HttpCookie
    {
        char chCookie[MAX_BUF];//Cookie Content
    }HttpCookie;//Cookie
//////////////////////////////////////////////////////////////////////////////////
public:
	CString Monitor(CString strFileName);//监测
	CHttpClient();
	virtual ~CHttpClient();

private:
	BOOL ValidHostChar(char ch);
	void MakeReturnString(CString *strReturn,int nStatus);//制作返回字符串
	void ParseURL(LPCSTR url,LPSTR protocol,int lprotocol, LPSTR host,
        int lhost,LPSTR request,int lrequest,int *port);//分割URL，得到主机名端口等
	BOOL StepByStep(CString *strMsg);//分布请求
	void ReadConfigFile(LPCSTR szFileName);//读取配置文件
	BOOL HttpConnection(LPCSTR szUrl,LPCSTR szPara,int nRequestType,CString strMatch,
        CString strRevMatch, CString *strMsg);//连接到Http主机
    //Post 请求模式
	BOOL RequestPost(HINTERNET hConnect ,CString strObject ,DWORD dwFlags
                          ,CString strInfo,CString strMatch, CString strRevMatch, 
						  CString *pszReturn);
    //Get 请求模式
	BOOL RequestGet(HINTERNET hConnect ,CString strObject ,DWORD dwFlags
                          ,CString strInfo,CString strMatch,CString strRevMatch, 
						  CString *pszReturn);
// Overrides
private:
	BSTR UTF2Unicode(LPCSTR lp, int nLen);
	BOOL IsPrintLog();
	BOOL m_bWriteLog;//是否写日志文件
    //得到返回状态码
	int GetStatus(CString strHeaderBuffer);
    //得到错误说明
	CString GetErrorMessage(DWORD dwErrCode);
    //得到请求内容
	CString ReadRequestContent(HINTERNET hRequest,int nContentLen);
    //写接收内容日志文件
	void WriteRecvContent(CString strContent);
    //写接受HTTP Header日至文件
	void WriteRecvHeader(CString strHttpHeader);
    //比较是否匹配
	BOOL ContentMatch(CString strContent ,CString strMatch);
    //得到Http Content Length
	int GetContentLength(CString strHttpHeader);
    //得到Cookie
	void GetCookies(CString strHeaderBuffer);
    //Cookie List
    queue<HttpCookie>CookieList;
    //Request List
    queue<RequestStep>RequestList;
    int nStep;
};

#endif // !defined(AFX_HTTPCLIENT_H__3ECA8275_BBA6_43B0_A57C_CCB8F884DCC6__INCLUDED_)
