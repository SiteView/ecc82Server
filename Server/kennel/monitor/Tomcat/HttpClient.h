// HttpClient.h: interface for the CHttpClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTTPCLIENT_H__8AA6A003_BB27_45D0_ABD8_DD926E8C8164__INCLUDED_)
#define AFX_HTTPCLIENT_H__8AA6A003_BB27_45D0_ABD8_DD926E8C8164__INCLUDED_

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
//////////////////////////////////////////////////////////////////////////////////
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
        int nType;//请求类型
    }RequestStep;//请求步骤
/////////////////////////////////////////////////////////////////////////////////
    typedef struct _HttpCookie
    {
        char chCookie[MAX_BUF];//Cookie Content
    }HttpCookie;//Cookie
//////////////////////////////////////////////////////////////////////////////////
public:
	CHttpClient();
	virtual ~CHttpClient();
public:
    CString Monitor(CStringList &paramList);
private:
	void MakeReturnString(CString *strReturn,int nStatus);//制作返回字符串
	void ParseURL(LPCSTR url,LPSTR protocol,int lprotocol, LPSTR host,
        int lhost,LPSTR request,int lrequest,int *port);//分割URL，得到主机名端口等
	BOOL HttpConnection(LPCSTR szUrl,LPCSTR szPara,int nRequestType,CString strMatch,
        CString *strMsg);//连接到Http主机
    //Post 请求模式
	BOOL RequestPost(HINTERNET hConnect ,CString strObject ,DWORD dwFlags
                          ,CString strInfo,CString strMatch, CString *pszReturn);
    //Get 请求模式
	BOOL RequestGet(HINTERNET hConnect ,CString strObject ,DWORD dwFlags
                          ,CString strInfo,CString strMatch, CString *pszReturn);
	BOOL IsPrintLog();

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
    BOOL ValidHostChar(char ch);
//////////////////////////////////////////////////////////////////////////////////
    BOOL m_bWriteLog;//是否写日志文件
    //Cookie List
    queue<HttpCookie>CookieList;
};

#endif // !defined(AFX_HTTPCLIENT_H__8AA6A003_BB27_45D0_ABD8_DD926E8C8164__INCLUDED_)
