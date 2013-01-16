//////////////////////////////////////////////////////////////////////
// Company: ���������Ƽ�
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
		char szRevMatch[MAX_BUF];//��ƥ���ַ���
        int nType;//��������
    }RequestStep;//������
/////////////////////////////////////////////////////////////////////////////////
    typedef struct _HttpCookie
    {
        char chCookie[MAX_BUF];//Cookie Content
    }HttpCookie;//Cookie
//////////////////////////////////////////////////////////////////////////////////
public:
	CString Monitor(CString strFileName);//���
	CHttpClient();
	virtual ~CHttpClient();

private:
	BOOL ValidHostChar(char ch);
	void MakeReturnString(CString *strReturn,int nStatus);//���������ַ���
	void ParseURL(LPCSTR url,LPSTR protocol,int lprotocol, LPSTR host,
        int lhost,LPSTR request,int lrequest,int *port);//�ָ�URL���õ��������˿ڵ�
	BOOL StepByStep(CString *strMsg);//�ֲ�����
	void ReadConfigFile(LPCSTR szFileName);//��ȡ�����ļ�
	BOOL HttpConnection(LPCSTR szUrl,LPCSTR szPara,int nRequestType,CString strMatch,
        CString strRevMatch, CString *strMsg);//���ӵ�Http����
    //Post ����ģʽ
	BOOL RequestPost(HINTERNET hConnect ,CString strObject ,DWORD dwFlags
                          ,CString strInfo,CString strMatch, CString strRevMatch, 
						  CString *pszReturn);
    //Get ����ģʽ
	BOOL RequestGet(HINTERNET hConnect ,CString strObject ,DWORD dwFlags
                          ,CString strInfo,CString strMatch,CString strRevMatch, 
						  CString *pszReturn);
// Overrides
private:
	BSTR UTF2Unicode(LPCSTR lp, int nLen);
	BOOL IsPrintLog();
	BOOL m_bWriteLog;//�Ƿ�д��־�ļ�
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
    //Cookie List
    queue<HttpCookie>CookieList;
    //Request List
    queue<RequestStep>RequestList;
    int nStep;
};

#endif // !defined(AFX_HTTPCLIENT_H__3ECA8275_BBA6_43B0_A57C_CCB8F884DCC6__INCLUDED_)
