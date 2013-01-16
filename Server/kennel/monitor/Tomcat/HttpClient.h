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
        char chProxyServer[MAX_BUF];//�������������
        char chProxyUser[MAX_BUF];//���������ʹ���û���
        char chProxyPwd[MAX_BUF];//�����������֤����
        int nProxyPort;//����������˿�
        int nTimeOut;//��ʱʱ��
    }HttpConfig;//Http Client Config
/////////////////////////////////////////////////////////////////////////////////
    typedef struct _RequestStep
    {
        char szUrl[MAX_BUF];//Url Address
        char szPara[MAX_BUF];//Post Para
        char szMatch[MAX_BUF];//ƥ���ַ���
        int nType;//��������
    }RequestStep;//������
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
	void MakeReturnString(CString *strReturn,int nStatus);//���������ַ���
	void ParseURL(LPCSTR url,LPSTR protocol,int lprotocol, LPSTR host,
        int lhost,LPSTR request,int lrequest,int *port);//�ָ�URL���õ��������˿ڵ�
	BOOL HttpConnection(LPCSTR szUrl,LPCSTR szPara,int nRequestType,CString strMatch,
        CString *strMsg);//���ӵ�Http����
    //Post ����ģʽ
	BOOL RequestPost(HINTERNET hConnect ,CString strObject ,DWORD dwFlags
                          ,CString strInfo,CString strMatch, CString *pszReturn);
    //Get ����ģʽ
	BOOL RequestGet(HINTERNET hConnect ,CString strObject ,DWORD dwFlags
                          ,CString strInfo,CString strMatch, CString *pszReturn);
	BOOL IsPrintLog();

    //�õ�����״̬��
	int GetStatus(CString strHeaderBuffer);
    //�õ�����˵��
	CString GetErrorMessage(DWORD dwErrCode);
    //�õ���������
	CString ReadRequestContent(HINTERNET hRequest,int nContentLen);
    //д����������־�ļ�
	void WriteRecvContent(CString strContent);
    //д����HTTP Header�����ļ�
	void WriteRecvHeader(CString strHttpHeader);
    //�Ƚ��Ƿ�ƥ��
	BOOL ContentMatch(CString strContent ,CString strMatch);
    //�õ�Http Content Length
	int GetContentLength(CString strHttpHeader);
    //�õ�Cookie
	void GetCookies(CString strHeaderBuffer);
    BOOL ValidHostChar(char ch);
//////////////////////////////////////////////////////////////////////////////////
    BOOL m_bWriteLog;//�Ƿ�д��־�ļ�
    //Cookie List
    queue<HttpCookie>CookieList;
};

#endif // !defined(AFX_HTTPCLIENT_H__8AA6A003_BB27_45D0_ABD8_DD926E8C8164__INCLUDED_)
