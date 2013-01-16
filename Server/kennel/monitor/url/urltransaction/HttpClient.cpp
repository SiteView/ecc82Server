// HttpClient.cpp: implementation of the CHttpClient class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HttpClient.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------written by chenxingang
#define ERROR_WINHTTP_AUTO_PROXY_SERVICE_ERROR					12178
#define ERROR_WINHTTP_AUTODETECTION_FAILED						12180
#define ERROR_WINHTTP_BAD_AUTO_PROXY_SCRIPT						12166
#define ERROR_WINHTTP_CANNOT_CALL_AFTER_OPEN					12103
#define ERROR_WINHTTP_CANNOT_CALL_AFTER_SEND					12102
#define ERROR_WINHTTP_CANNOT_CALL_BEFORE_OPEN					12100
#define ERROR_WINHTTP_CANNOT_CALL_BEFORE_SEND					12101
#define ERROR_WINHTTP_CANNOT_CONNECT							12029
#define ERROR_WINHTTP_CHUNKED_ENCODING_HEADER_SIZE_OVERFLOW		12183
#define ERROR_WINHTTP_CLIENT_AUTH_CERT_NEEDED					12044
#define ERROR_WINHTTP_CONNECTION_ERROR							12030
#define ERROR_WINHTTP_HEADER_ALREADY_EXISTS						12155
#define ERROR_WINHTTP_HEADER_COUNT_EXCEEDED						12181
#define ERROR_WINHTTP_HEADER_NOT_FOUND							12150
#define ERROR_WINHTTP_HEADER_SIZE_OVERFLOW						12182
#define ERROR_WINHTTP_INCORRECT_HANDLE_STATE					12019
#define ERROR_WINHTTP_INCORRECT_HANDLE_TYPE						12018
#define ERROR_WINHTTP_INTERNAL_ERROR							12004
#define ERROR_WINHTTP_INVALID_OPTION							12009
#define ERROR_WINHTTP_INVALID_QUERY_REQUEST						12154
#define ERROR_WINHTTP_INVALID_SERVER_RESPONSE					12152
#define ERROR_WINHTTP_INVALID_URL								12005
#define ERROR_WINHTTP_LOGIN_FAILURE								12015
#define ERROR_WINHTTP_NAME_NOT_RESOLVED							12007
#define ERROR_WINHTTP_NOT_INITIALIZED							12172
#define ERROR_WINHTTP_OPERATION_CANCELLED						12017
#define ERROR_WINHTTP_OPTION_NOT_SETTABLE						12011
#define ERROR_WINHTTP_OUT_OF_HANDLES							12001
#define ERROR_WINHTTP_REDIRECT_FAILED							12156
#define ERROR_WINHTTP_RESEND_REQUEST							12032
#define ERROR_WINHTTP_RESPONSE_DRAIN_OVERFLOW					12184
#define ERROR_WINHTTP_SECURE_CERT_CN_INVALID					12038
#define ERROR_WINHTTP_SECURE_CERT_DATE_INVALID					12037
#define ERROR_WINHTTP_SECURE_CERT_REV_FAILED					12057
#define ERROR_WINHTTP_SECURE_CERT_REVOKED						12170
#define ERROR_WINHTTP_SECURE_CERT_WRONG_USAGE					12179
#define ERROR_WINHTTP_SECURE_CHANNEL_ERROR						12157
#define ERROR_WINHTTP_SECURE_FAILURE							12175
#define ERROR_WINHTTP_SECURE_INVALID_CA							12045
#define ERROR_WINHTTP_SECURE_INVALID_CERT						12169
#define ERROR_WINHTTP_SHUTDOWN									12012
#define ERROR_WINHTTP_TIMEOUT									12002
#define ERROR_WINHTTP_UNABLE_TO_DOWNLOAD_SCRIPT					12167
#define ERROR_WINHTTP_UNRECOGNIZED_SCHEME						12006
#define ERROR_WINHTTP_PROXY_AUTH_REQUIRED                       12185
//--------------------------------------------------written by chenxingang
const LPCSTR AGENT_NAME = _T("CeHttp");
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHttpClient::CHttpClient()
{
    m_bWriteLog = FALSE;
}

CHttpClient::~CHttpClient()
{

}

/////////////////////////////////////////////////////////////////////////////////
// ������
// ˵����
// ������
//
// ����ֵ��
/////////////////////////////////////////////////////////////////////////////////
BOOL CHttpClient::RequestGet(HINTERNET hConnect ,CString strObject ,DWORD dwFlags
                          ,CString strInfo, CString strMatch, CString strRevMatch,
						  CString *pszReturn)
{
    HINTERNET hRequest = NULL;

    const char * chType = "*/*";
    hRequest = HttpOpenRequest (hConnect,  TEXT("GET"), (LPCTSTR)strObject, 
        TEXT("HTTP/1.0"), NULL, &chType, dwFlags, 0);

    if(!hRequest)
    {
        CString strTemp = pszReturn->GetBuffer(0);
        pszReturn->Format("error=Requset Failed!$%s",strTemp);
        return FALSE;
    }
        while(!CookieList.empty())
    {//����Cookie
        char chHeader[200];
        HttpCookie cookie =CookieList.front();
        CookieList.pop();
        sprintf(chHeader,"Cookie: %s",cookie.chCookie);
        //��������ͷ����
        HttpAddRequestHeaders( hRequest, chHeader, 
            _tcslen(chHeader), HTTP_ADDREQ_FLAG_REPLACE); 
    }
	BOOL bSendRequest = FALSE;
	DWORD dwHeadLen = 0 , dwOptionLen = 0;				
	dwOptionLen = strInfo.GetLength();
    //����ʽ
	char* strType = "Content-Type: application/x-www-form-urlencoded"; 
	int nTypeLen = strlen(strType);	
    //��������
	bSendRequest = HttpSendRequest(hRequest, 
        strType,
		nTypeLen,
		NULL,
		NULL);

	if(!bSendRequest)
	{//����ʧ��
        CString strTemp = pszReturn->GetBuffer(0);
        pszReturn->Format("error=Send Requset Failed!$%s",strTemp);
        return FALSE;
    }
	DWORD dwLen = 1024;
    char *chCookie = (char*)malloc(1024);
    memset(chCookie ,0 ,1024);
    //�õ���"\r\n"�ָ��Http���󷵻�ͷ
    if(!HttpQueryInfo(hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, chCookie, &dwLen, NULL))
    {//ʧ��
        DWORD dwErr=GetLastError();//Get Last Error
		CString strError=GetErrorMessage(dwErr);// Get Error Message
        CString strTemp = pszReturn->GetBuffer(0);
		pszReturn->Format("error=HttpQueryInfo:%s$%s",strError,strTemp);
		InternetCloseHandle(hRequest);
		return FALSE;
    }
    GetCookies(chCookie);  //�ӷ���ͷ�еõ�Cookie  
    int nStatus = GetStatus(chCookie);//�õ�����״̬��
    MakeReturnString(pszReturn,nStatus);
    if(nStatus >= 400)
    {
        CString strTemp = pszReturn->GetBuffer(0);
		pszReturn->Format("StepStatusCode=%d$%s",nStatus,strTemp);        
        return FALSE;
    }
    //ReadRequestContent(hRequest, GetContentLength(chCookie));
    BOOL bMatch = TRUE;
    CString strContent = ReadRequestContent(hRequest, GetContentLength(chCookie));
	if(!strRevMatch.IsEmpty())
	{
        bMatch = ContentMatch(strContent,strRevMatch);
        if(bMatch)
        {//ƥ��ʧ��
            CString strTemp = pszReturn->GetBuffer(0);
            pszReturn->Format("error=��ƥ��ɹ�!$%s",strTemp);
            InternetCloseHandle(hRequest);//�ر�����
            delete chCookie;
            return FALSE;
        }
		bMatch = TRUE;
	}
    else if(!strMatch.IsEmpty())
    {//ƥ���ַ�����Ϊ��        
        bMatch = ContentMatch(strContent,strMatch);
        if(!bMatch)
        {//ƥ��ʧ��
            CString strTemp = pszReturn->GetBuffer(0);
            pszReturn->Format("error=ƥ��ʧ��!$%s",strTemp);
            InternetCloseHandle(hRequest);//�ر�����
            delete chCookie;
            return FALSE;
        }
    }
    delete chCookie;
	InternetCloseHandle (hRequest);//�ر�����
    return bMatch;
}

/////////////////////////////////////////////////////////////////////////////////
// ������
// ˵����
// ������
//
// ����ֵ��
/////////////////////////////////////////////////////////////////////////////////
BOOL CHttpClient::RequestPost(HINTERNET hConnect ,CString strObject ,DWORD dwFlags
                          ,CString strInfo,CString strMatch,  CString strRevMatch, 
						  CString *pszReturn)
{
    HINTERNET hRequest = NULL;
    //��POST��ʽ������
    const char * chType = "*/*";
    hRequest = HttpOpenRequest (hConnect, TEXT("POST"), (LPCTSTR)strObject, 
        TEXT("HTTP/1.0"), NULL, &chType, dwFlags, 0);

    if(!hRequest)
    {//����ʧ��
        CString strTemp = pszReturn->GetBuffer(0);
        pszReturn->Format("error=Requset Failed!$%s",strTemp);
        return FALSE;
    }
    while(!CookieList.empty())
    {//����Cookie
        char chHeader[200];
        HttpCookie cookie =CookieList.front();
        CookieList.pop();
        sprintf(chHeader,"Cookie: %s",cookie.chCookie);
        //��������ͷ����
        HttpAddRequestHeaders( hRequest, chHeader, 
            _tcslen(chHeader), HTTP_ADDREQ_FLAG_REPLACE); 
    }
	BOOL bSendRequest = FALSE;
	DWORD dwHeadLen = 0 , dwOptionLen = 0;				
	dwOptionLen = strInfo.GetLength();
    //����ʽ
	char* strType = "Content-Type: application/x-www-form-urlencoded"; 
	int nTypeLen = strlen(strType);	
    //��������
	bSendRequest = HttpSendRequest(hRequest, 
        strType,//Request Type
		nTypeLen, // Request Type Length
		(LPVOID)(strInfo.GetBuffer(strInfo.GetLength())),//Post Para
		dwOptionLen//Post Para Length
        );
	

	if(!bSendRequest)
	{//����ʧ��
        CString strTemp = pszReturn->GetBuffer(0);
        pszReturn->Format("error=Send Requset Failed!$%s",strTemp);
        return FALSE;
    }

	DWORD dwLen = 1024;
    char *chCookie = (char*)malloc(1024);
    memset(chCookie ,0 ,1024);
    //�õ���"\r\n"�ָ��Http���󷵻�ͷ
    if(!HttpQueryInfo(hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, chCookie, &dwLen, NULL))
    {//ʧ��
        DWORD dwErr=GetLastError();//Get Last Error
		CString strError=GetErrorMessage(dwErr);// Get Error Message
        CString strTemp = pszReturn->GetBuffer(0);
		pszReturn->Format("error=HttpQueryInfo:%s$%s",strError,strTemp);
		InternetCloseHandle(hRequest);
		return FALSE;
    }
    GetCookies(chCookie);  //�ӷ���ͷ�еõ�Cookie  
    int nStatus = GetStatus(chCookie);//�õ�����״̬��
    MakeReturnString(pszReturn,nStatus);
    if(nStatus >= 400)
    {
        CString strTemp = pszReturn->GetBuffer(0);
		pszReturn->Format("StepStatusCode=%d$%s",nStatus,strTemp);  
        return FALSE;
    }
    BOOL bMatch = TRUE;   
    //ReadRequestContent(hRequest, GetContentLength(chCookie));
    CString strContent = ReadRequestContent(hRequest, GetContentLength(chCookie));
    if(!strRevMatch.IsEmpty())
	{
        bMatch = ContentMatch(strContent,strRevMatch);
        if(bMatch)
        {//ƥ��ʧ��
            CString strTemp = pszReturn->GetBuffer(0);
            pszReturn->Format("error=��ƥ��ɹ�!$%s",strTemp);
            InternetCloseHandle(hRequest);//�ر�����
            delete chCookie;
            return FALSE;
        }
		bMatch = TRUE;
	}
    else if(!strMatch.IsEmpty())
    {//ƥ���ַ�����Ϊ��
        bMatch = ContentMatch(strContent,strMatch);
        if(!bMatch)
        {//ƥ��ʧ��
            CString strTemp = pszReturn->GetBuffer(0);
            pszReturn->Format("error=ƥ��ʧ��!$%s",strTemp);
            InternetCloseHandle(hRequest);//�ر�����
            delete chCookie;
            return FALSE;
        }
    }
    delete chCookie;
    InternetCloseHandle(hRequest);//�ر�����
    return bMatch;
}

/////////////////////////////////////////////////////////////////////////////////
// ������
// ˵����
// ������
//
// ����ֵ��
/////////////////////////////////////////////////////////////////////////////////
BOOL CHttpClient::HttpConnection(LPCSTR szUrl,LPCSTR szPara,int nRequestType,
                             CString strMatch, CString strRevMatch, CString *strMsg)
{
    DWORD     dwSize = 0, 
		      dwFlags = INTERNET_FLAG_RELOAD;     
	int nReturn = -1;
    
	HINTERNET hOpen = NULL;
    //��Http��ʽ��Internet����
    hOpen = InternetOpen (TEXT("CeHttp"), 0,NULL, 0, 1);
	//hOpen = InternetOpen (AGENT_NAME, INTERNET_OPEN_TYPE_PRECONFIG, 
    //                      NULL, NULL, INTERNET_FLAG_ASYNC);
	if(hOpen)//�ɹ���
	{
		CString       strServer = _T("");//����������
		CString       strObject = _T("");
		CString       strWholePass = _T("");
		INTERNET_PORT nPort = 0;
		DWORD         dwServiceType = 0;//����������
        BOOL bGetServer = AfxParseURL(szUrl,dwServiceType, strServer, 
            strObject, nPort);//�ֽ�����URL		
		HINTERNET hConnect = NULL;
        //��ָ��������������
		hConnect = InternetConnect (hOpen, (LPCTSTR)strServer, 
            nPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);		
		if(hConnect)//���ӳɹ�
		{
		    //nReturn = GetUrl(hConnect ,strObject,dwFlags,"");
            if(nRequestType == 0)
            {
                if(!RequestGet(hConnect ,strObject,dwFlags,szPara,strMatch,strRevMatch,
					strMsg))
                    return FALSE;
            }
            else if(nRequestType == 1)
            {
                if(!RequestPost(hConnect ,strObject,dwFlags,szPara,strMatch,strRevMatch,
					strMsg))
                    return FALSE;
            }
    		InternetCloseHandle (hConnect);//�ر�����
		}
        else
        {
            strMsg->Format("error=Connet Web Server Failed!");
        }
		InternetCloseHandle (hOpen);//�ر�
	}	
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
// ������ReadConfigFile
// ˵����
// ������
//
// ����ֵ��
/////////////////////////////////////////////////////////////////////////////////
void CHttpClient::ReadConfigFile(LPCSTR szFileName)
{
    //Get Step Count
    int nStepCount = GetPrivateProfileInt("information","_tranStep",0,szFileName);

	//��������Ϊ������5��
	if(nStepCount >= 5)
		nStepCount = 5;

    //Get Step Para
    for(int i=1 ; i<=nStepCount ; i++)
    {
        CString strApp;//Key Name
        strApp.Format("url%d", i);
        RequestStep request;
        //Url��ַ
        GetPrivateProfileString(strApp,"_urlname","",request.szUrl, MAX_BUF,
            szFileName);
        //Post������Get����ģʽ��Ϊ�մ���
        GetPrivateProfileString(strApp,"_post","",request.szPara, MAX_BUF,
            szFileName);
        //����ʽ 0 Get 1 Post
        request.nType=GetPrivateProfileInt(strApp,"_requestMode",0,
            szFileName);
        //ƥ���ַ���
        GetPrivateProfileString(strApp, "_verifyimg", "" , request.szMatch ,
            MAX_BUF,szFileName);
        //��ƥ���ַ���
        GetPrivateProfileString(strApp, "_verifyreverseimg", "" , request.szRevMatch ,
            MAX_BUF,szFileName);
        
		//��ӵ�����
        RequestList.push(request);
    }

	//RequestStep request;
	//
	//strcpy(request.szUrl, "http://www.sina.com.cn");
	////request.szUrl = "http://www.sina.com.cn";
	////request.szPara = "f";
	//strcpy(request.szPara, "");
	//request.nType = 0;
	////request.szMatch = "ddd";
	//strcpy(request.szMatch, "f");
	////request.szRevMatch = "ds";
	//strcpy(request.szRevMatch, "");

	//RequestList.push(request);

	//strcpy(request.szUrl, "http://www.google.com.cn");
	////request.szUrl = "http://www.sina.com.cn";
	////request.szPara = "f";
	//strcpy(request.szPara, "");
	//request.nType = 0;
	////request.szMatch = "ddd";
	//strcpy(request.szMatch, "f");
	////request.szRevMatch = "ds";
	//strcpy(request.szRevMatch, "");
	//RequestList.push(request);

	//strcpy(request.szUrl, "http://www.siteview.com");
	////request.szUrl = "http://www.sina.com.cn";
	////request.szPara = "f";
	//strcpy(request.szPara, "");
	//request.nType = 0;
	////request.szMatch = "ddd";
	//strcpy(request.szMatch, "f");
	////request.szRevMatch = "ds";
	//strcpy(request.szRevMatch, "");
	//RequestList.push(request);
	//request.szUrl = "http://www.sina.com.cn";
	//request.szPara = ;
	//request.nType = ;
	//request.szMatch = ;
	//request.szRevMatch = ;	
	//RequestList.push(request);

	//request.szUrl = "http://www.sina.com.cn";
	//request.szPara = ;
	//request.nType = ;
	//request.szMatch = ;
	//request.szRevMatch = ;	
	//RequestList.push(request);
}

//////////////////////////////////////////////////////////////////////////////////
// ������StepByStep                                                             //
// ˵�����ֲ�����                                                               //
// ������                                                                       //
//
// ����ֵ���ɹ�����ʧ��                                                         //
//////////////////////////////////////////////////////////////////////////////////
BOOL CHttpClient::StepByStep(CString *strMsg)
{
    RequestStep request;
    nStep = 1;
    while(!RequestList.empty())
    {
        request = RequestList.front();
        RequestList.pop();
        if(!HttpConnection(request.szUrl,request.szPara,request.nType,
            request.szMatch,request.szRevMatch,strMsg))
        {
            return FALSE;
            break;
        }
        nStep ++;
    }
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
// ������ParseURL                                                               //
// ˵�����ָ�url���õ��������˿ڵ�
// ������
//
// ����ֵ����
/////////////////////////////////////////////////////////////////////////////////
void CHttpClient::ParseURL(LPCSTR url, LPSTR protocol, int lprotocol, 
                           LPSTR host, int lhost, LPSTR request, 
                           int lrequest, int *port)
{
    char *work,*ptr,*ptr2;

    *protocol = *host = *request = 0;
    *port=80;

    work = strdup(url);
    strupr(work);

    ptr = strchr(work,':');	// find protocol if any
    if(ptr!=NULL)
    {
        *(ptr++) = 0;
        lstrcpyn(protocol,work,lprotocol);
    }
    else
    {
        lstrcpyn(protocol,"HTTP",lprotocol);
        ptr = work;
    }

    if( (*ptr=='/') && (*(ptr+1)=='/') )// skip past opening /'s 
        ptr+=2;

    ptr2 = ptr;										// find host
    while( ValidHostChar(*ptr2) && *ptr2 )
        ptr2++;

    *ptr2=0;
    lstrcpyn(host,ptr,lhost);

    lstrcpyn(request,url + (ptr2-work),lrequest);	// find the request

    ptr = strchr(host,':');							// find the port number, if any
    if(ptr!=NULL)
    {
        *ptr=0;
        *port = atoi(ptr+1);
    }

    free(work);
}

//////////////////////////////////////////////////////////////////////////////////
// ������MakeReturnString                                                       //
// ˵��������״̬�������ԭʼ����ֵ���췵����Ϣ                                 //
// ������                                                                       //
//      strReturn��ԭʼ����ֵ                                                   //
//      nStatus��״̬������                                                     //
// ����ֵ����                                                                   //
//////////////////////////////////////////////////////////////////////////////////
void CHttpClient::MakeReturnString(CString *strReturn, int nStatus)
{
    CString strTemp = strReturn->GetBuffer(0);
    strReturn->Format("%sStep%dStatusCode=%d$",strTemp,nStep,nStatus);
}

/////////////////////////////////////////////////////////////////////////////////
// ������
// ˵����
// ������
//
// ����ֵ��
/////////////////////////////////////////////////////////////////////////////////
BOOL CHttpClient::ValidHostChar(char ch)
{
    return( isalpha(ch) || isdigit(ch)
        || ch=='-' || ch=='.' || ch==':' );
}

//////////////////////////////////////////////////////////////////////////////////
// ������Monitor                                                                //
// ˵�������ݼ��ű��ļ����м��                                               //
// ������                                                                       //
//      strFileName�����ű��ļ�����                                           //
// ����ֵ�������                                                             //
//////////////////////////////////////////////////////////////////////////////////
CString CHttpClient::Monitor(CString strFileName)
{
    CString strMsg = _T("");
    m_bWriteLog = IsPrintLog();
    ReadConfigFile(strFileName);//Read Config File By FileName
    if(StepByStep(&strMsg))//�ֲ����
    {
        CString strTemp = strMsg;
        strMsg.Format("StepStatusCode=200$%s",strTemp);
		
		//���������Զ���ΪStepNStatusCode=0
		int nStepCount = GetPrivateProfileInt("information","_tranStep",0,strFileName);
		for(int i = nStepCount + 1; i <= 5; i++)
		{
			strTemp.Format("Step%dStatusCode=0$", i);
			strMsg += strTemp;
		}
    }
    return strMsg;//���ؼ����
}

//////////////////////////////////////////////////////////////////////////////////
// ������GetCookies                                                             //
// ˵��������HTTPͷ�õ�Cookie                                                   //
// ������                                                                       //
//      strHeaderBuffer��httpͷ                                                 //
// ����ֵ����                                                                   //
//////////////////////////////////////////////////////////////////////////////////
void CHttpClient::GetCookies(CString strHeaderBuffer)
{
    strHeaderBuffer.MakeLower();
    int nIndex = 0;
    while(nIndex != -1)
    {   
        //����ÿһ��Cookie
        nIndex = strHeaderBuffer.Find("set-cookie: ") + 12;
        if(nIndex>=12)
        {
            //�õ�Cookie����
            strHeaderBuffer = strHeaderBuffer.Right(strHeaderBuffer.GetLength()-nIndex);
            nIndex = strHeaderBuffer.Find("\r\n",0);
            HttpCookie cookie;
            sprintf(cookie.chCookie,"%s",strHeaderBuffer.Left(nIndex));
            //��ӵ�����
            CookieList.push(cookie);
            strHeaderBuffer = strHeaderBuffer.Right(strHeaderBuffer.GetLength()-nIndex-2);
        }
        else
        {
            break;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////
// ������GetContentLength                                                       //
// ˵��������Httpͷ�õ��������ݳ���                                             //
// ������                                                                       //
//      strHttpHeader��Httpͷ                                                   //
// ����ֵ���������ݳ���                                                         //
//////////////////////////////////////////////////////////////////////////////////
int CHttpClient::GetContentLength(CString strHttpHeader)
{
    if(m_bWriteLog)
        WriteRecvHeader(strHttpHeader);
    int nContentLength = 0;
    strHttpHeader.MakeLower();
    //����content-length
    int nIndex = strHttpHeader.Find("content-length: ",0)+16;
    if(nIndex>=16)
    {//����content-length
        strHttpHeader = strHttpHeader.Right(strHttpHeader.GetLength()-nIndex);
        nIndex = strHttpHeader.Find("\r\n");
        if(nIndex != -1)
            nContentLength = atoi(strHttpHeader.Left(nIndex));
    }
    return nContentLength;//���س���
}

//////////////////////////////////////////////////////////////////////////////////
// ������ContentMatch                                                           //
// ˵������ԭʼ�ַ����в����Ƿ���ڵ�ǰ�ַ���                                   //
// ������                                                                       //
//      strContent��ԭʼ�ַ���                                                  //
//      strMatch����ǰ�ַ���                                                    //
// ����ֵ���ɹ�ΪTRUE                                                           //
//////////////////////////////////////////////////////////////////////////////////
BOOL CHttpClient::ContentMatch(CString strContent, CString strMatch)
{
    int nIndex = strContent.Find(strMatch , 0);
    if(nIndex == -1)
    {//����ʧ��
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

//////////////////////////////////////////////////////////////////////////////////
// ������WriteRecvHeader                                                        //
// ˵����д���󷵻�HTTPͷ���ݵ���־�ļ���                                       //
// ������                                                                       //
//      strHttpHeader�����󷵻�Httpͷ����                                       //
// ����ֵ����                                                                   //
//////////////////////////////////////////////////////////////////////////////////
void CHttpClient::WriteRecvHeader(CString strContent)
{
    FILE *stream;
    char chTime[50] = {0};
    //���ļ�
    stream=fopen(FuncGetInstallRootPath() +"\\logs\\UrlTransRecv.log","a");
   	SYSTEMTIME time;
	//ȡϵͳ��ǰʱ��
	GetLocalTime(&time);
    //ʱ���ʽ����-��-�� ʱ-��-��
    sprintf(chTime ,"%02dY-%02dM-%02dD %02dh-%02dm-%02ds\r\n\r\n",
				time.wYear - 2000,
				time.wMonth,
				time.wDay,
				time.wHour,
				time.wMinute,
				time.wSecond);
    fputs(chTime , stream);//ʱ��
    if(!strContent.IsEmpty())
    {//���ݲ�Ϊ��
        fputs(strContent.GetBuffer(strContent.GetLength()),stream);
    }
    fputs("\r\n\r\n",stream);//�س�����
	fclose(stream); //�ر��ļ�
}

//////////////////////////////////////////////////////////////////////////////////
// ������WriteRecvContent                                                       //
// ˵����д������Ϣ���ݵ���־�ļ���                                             //
// ������                                                                       //
//      strContent����Ϣ����                                                    //
// ����ֵ����                                                                   //
//////////////////////////////////////////////////////////////////////////////////
void CHttpClient::WriteRecvContent(CString strContent)
{
    FILE *stream;
    char chTime[50] = {0};
    //���ļ�
    stream=fopen(FuncGetInstallRootPath() +"\\logs\\UrlTransContent.log","a");
   	SYSTEMTIME time;
	//ȡϵͳ��ǰʱ��
	GetLocalTime(&time);
    //ʱ���ʽ����-��-�� ʱ-��-��
    sprintf(chTime ,"%02dY-%02dM-%02dD %02dh-%02dm-%02ds\r\n\r\n",
				time.wYear - 2000,
				time.wMonth,
				time.wDay,
				time.wHour,
				time.wMinute,
				time.wSecond);
    fputs(chTime , stream);//ʱ��
    if(!strContent.IsEmpty())
    {//���ݲ�Ϊ��
        fputs(strContent.GetBuffer(strContent.GetLength()),stream);
    }
    fputs("\r\n\r\n",stream);//�س�����
	fclose(stream); //�ر��ļ�
}
//////////////////////////////////////////////////////////////////////////////////
// ������ReadRequestContent                                                     //
// ˵�����������ݳ��Ⱥ��������õ�����                                         //
// ������                                                                       //
//      hRequest��������                                                      //
//      nContentLen�����ݳ���                                                   //
// ����ֵ����������                                                             //
//////////////////////////////////////////////////////////////////////////////////
CString CHttpClient::ReadRequestContent(HINTERNET hRequest , int nContentLen)
{
	DWORD lBuffer = 1024;
	char *lpBufferA = new CHAR [lBuffer+1];//�����ڴ�
	BOOL bReadFile = FALSE;//���ļ���־��ʼ��
    CString strContent = _T("");
    if(nContentLen>0)
    {
        while(nContentLen>0)
        {	
            //��ȡ�ļ�(1024 byte)
            memset(lpBufferA, 0, lBuffer+1);//��ʼ������
	        bReadFile = InternetReadFile(hRequest, (LPVOID)lpBufferA, lBuffer, &lBuffer);
	        if (!bReadFile)//��Internet�ļ�ʧ��
	        {
                delete(lpBufferA);//ɾ���ڴ����
                return _T("");
            } 
            nContentLen -= lBuffer;
            strContent += lpBufferA;
        }
    }
    else
    {//����ͷ��δ�õ����ݳ���
        while(lBuffer >0)
        {
            //��ȡ�ļ�(1024 byte)
            memset(lpBufferA, 0, lBuffer+1);//��ʼ������
	        bReadFile = InternetReadFile(hRequest, (LPVOID)lpBufferA, lBuffer, &lBuffer);
	        if (!bReadFile)//��Internet�ļ�ʧ��
	        {
                break;
            } 
            strContent += lpBufferA;
        }
    }
    if(m_bWriteLog)
        WriteRecvContent(strContent);
    delete(lpBufferA);
	strContent = UTF2Unicode(strContent.GetBuffer(0), strContent.GetLength());
    return strContent;
}

//////////////////////////////////////////////////////////////////////////////////
// ������GetErrorMessage                                                        //
// ˵�������ݴ�����õ���������                                                 //
// ������                                                                       //
//      dwErrorCode��������                                                     //
// ����ֵ����������                                                             //
//////////////////////////////////////////////////////////////////////////////////
CString CHttpClient::GetErrorMessage(DWORD dwErrCode)
{
	CString strError=_T("");

	switch(dwErrCode)
	{
	case    ERROR_WINHTTP_AUTO_PROXY_SERVICE_ERROR:
		return "ERROR_WINHTTP_AUTO_PROXY_SERVICE_ERROR";
	case	ERROR_WINHTTP_AUTODETECTION_FAILED:
		return "ERROR_WINHTTP_AUTODETECTION_FAILED";
	case	ERROR_WINHTTP_BAD_AUTO_PROXY_SCRIPT:
		return "ERROR_WINHTTP_BAD_AUTO_PROXY_SCRIPT";
	case	ERROR_WINHTTP_CANNOT_CALL_AFTER_OPEN:				
		return "ERROR_WINHTTP_CANNOT_CALL_AFTER_OPEN";
	case	ERROR_WINHTTP_CANNOT_CALL_AFTER_SEND:				
		return "ERROR_WINHTTP_CANNOT_CALL_AFTER_SEND";
	case	ERROR_WINHTTP_CANNOT_CALL_BEFORE_OPEN:				
		return "ERROR_WINHTTP_CANNOT_CALL_BEFORE_OPEN";
	case	ERROR_WINHTTP_CANNOT_CALL_BEFORE_SEND:				
		return "ERROR_WINHTTP_CANNOT_CALL_BEFORE_SEND";
	case	ERROR_WINHTTP_CANNOT_CONNECT:					
		return "ERROR_WINHTTP_CANNOT_CONNECT";
	case	ERROR_WINHTTP_CHUNKED_ENCODING_HEADER_SIZE_OVERFLOW:
		return "ERROR_WINHTTP_CHUNKED_ENCODING_HEADER_SIZE_OVERFLOW";
	case	ERROR_WINHTTP_CLIENT_AUTH_CERT_NEEDED:	
		return "ERROR_WINHTTP_CLIENT_AUTH_CERT_NEEDED";
	case	ERROR_WINHTTP_CONNECTION_ERROR:					
		return "ERROR_WINHTTP_CONNECTION_ERROR";
	case	ERROR_WINHTTP_HEADER_ALREADY_EXISTS:
		return "ERROR_WINHTTP_HEADER_ALREADY_EXISTS";
	case	ERROR_WINHTTP_HEADER_COUNT_EXCEEDED:					
		return "ERROR_WINHTTP_HEADER_COUNT_EXCEEDED";
	case	ERROR_WINHTTP_HEADER_NOT_FOUND:					
		return "ERROR_WINHTTP_HEADER_NOT_FOUND";
	case	ERROR_WINHTTP_HEADER_SIZE_OVERFLOW:
		return "ERROR_WINHTTP_HEADER_SIZE_OVERFLOW";
	case	ERROR_WINHTTP_INCORRECT_HANDLE_STATE:
		return "ERROR_WINHTTP_INCORRECT_HANDLE_STATE";
	case	ERROR_WINHTTP_INCORRECT_HANDLE_TYPE:					
		return "ERROR_WINHTTP_INCORRECT_HANDLE_TYPE";
	case	ERROR_WINHTTP_INTERNAL_ERROR:					
		return "ERROR_WINHTTP_INTERNAL_ERROR";
	case	ERROR_WINHTTP_INVALID_OPTION:
		return "ERROR_WINHTTP_INVALID_OPTION";
	case	ERROR_WINHTTP_INVALID_QUERY_REQUEST:
		return "ERROR_WINHTTP_INVALID_QUERY_REQUEST";
	case	ERROR_WINHTTP_INVALID_SERVER_RESPONSE:
		return "ERROR_WINHTTP_INVALID_SERVER_RESPONSE";
	case	ERROR_WINHTTP_INVALID_URL:					
		return "ERROR_WINHTTP_INVALID_URL";
	case	ERROR_WINHTTP_LOGIN_FAILURE:							
		return "ERROR_WINHTTP_LOGIN_FAILURE";
	case	ERROR_WINHTTP_NAME_NOT_RESOLVED:
		return "ERROR_WINHTTP_NAME_NOT_RESOLVED";
	case	ERROR_WINHTTP_NOT_INITIALIZED:
		return "ERROR_WINHTTP_NOT_INITIALIZED";
	case	ERROR_WINHTTP_OPERATION_CANCELLED:
		return "ERROR_WINHTTP_OPERATION_CANCELLED";
	case	ERROR_WINHTTP_OPTION_NOT_SETTABLE:
		return "ERROR_WINHTTP_OPTION_NOT_SETTABLE";
	case	ERROR_WINHTTP_OUT_OF_HANDLES:						
		return "ERROR_WINHTTP_OUT_OF_HANDLES";
	case	ERROR_WINHTTP_REDIRECT_FAILED:
		return "ERROR_WINHTTP_REDIRECT_FAILED";
	case	ERROR_WINHTTP_RESEND_REQUEST:
		return "ERROR_WINHTTP_RESEND_REQUEST";
	case	ERROR_WINHTTP_RESPONSE_DRAIN_OVERFLOW:
		return "ERROR_WINHTTP_RESPONSE_DRAIN_OVERFLOW";
	case	ERROR_WINHTTP_SECURE_CERT_CN_INVALID:				
		return "ERROR_WINHTTP_SECURE_CERT_CN_INVALID";
	case	ERROR_WINHTTP_SECURE_CERT_DATE_INVALID:				
		return "ERROR_WINHTTP_SECURE_CERT_DATE_INVALID";
	case	ERROR_WINHTTP_SECURE_CERT_REV_FAILED:				
		return "ERROR_WINHTTP_SECURE_CERT_REV_FAILED";
	case	ERROR_WINHTTP_SECURE_CERT_REVOKED:					
		return "ERROR_WINHTTP_SECURE_CERT_REVOKED";
	case	ERROR_WINHTTP_SECURE_CERT_WRONG_USAGE:				
		return "ERROR_WINHTTP_SECURE_CERT_WRONG_USAGE";
	case	ERROR_WINHTTP_SECURE_CHANNEL_ERROR:					
		return "ERROR_WINHTTP_SECURE_CHANNEL_ERROR";
	case	ERROR_WINHTTP_SECURE_FAILURE:						
		return "ERROR_WINHTTP_SECURE_FAILURE";
	case	ERROR_WINHTTP_SECURE_INVALID_CA:						
		return "ERROR_WINHTTP_SECURE_INVALID_CA";
	case	ERROR_WINHTTP_SECURE_INVALID_CERT:					
		return "ERROR_WINHTTP_SECURE_INVALID_CERT";
	case	ERROR_WINHTTP_SHUTDOWN:						
		return "ERROR_WINHTTP_SHUTDOWN";
	case	ERROR_WINHTTP_TIMEOUT:								
		return "ERROR_WINHTTP_TIMEOUT";
	case	ERROR_WINHTTP_UNABLE_TO_DOWNLOAD_SCRIPT:
		return "ERROR_WINHTTP_UNABLE_TO_DOWNLOAD_SCRIPT";
	case	ERROR_WINHTTP_UNRECOGNIZED_SCHEME:				
		return "ERROR_WINHTTP_UNRECOGNIZED_SCHEME";
    case ERROR_WINHTTP_PROXY_AUTH_REQUIRED:
        return "ERROR_WINHTTP_PROXY_AUTH_REQUIRED";
	default:
		return "SYSTEM_ERROR";
	}
	return strError;
}

//////////////////////////////////////////////////////////////////////////////////
// ������GetStatus                                                              //
// ˵��������HTTP���󷵻�ͷ�õ�״̬������                                       //
// ������                                                                       //   
//      strHeaderBuffer��http���󷵻�ͷ                                         //
// ����ֵ��״̬������                                                           //
//////////////////////////////////////////////////////////////////////////////////
int CHttpClient::GetStatus(CString strHeaderBuffer)
{
    int nIndex = 0 ,nStatus = 0;
    CString strHeaderTemp = strHeaderBuffer;
    strHeaderTemp.MakeLower();//ȫ��ת��ΪСд
    nIndex = strHeaderTemp.Find("http/",0) + 5;//Э��ͷ
    nIndex = strHeaderTemp.Find(" ", nIndex) + 1;//״̬��ǰ�ո�
    strHeaderTemp=strHeaderTemp.Right(strHeaderTemp.GetLength()-nIndex);
    nStatus = atoi(strHeaderTemp.Left(3));//״̬��
    return nStatus;//����
}

BOOL CHttpClient::IsPrintLog()
{
    CString strLogConfig = FuncGetInstallRootPath()+ "\\groups\\logconfig.ini";
    int nTemp = GetPrivateProfileInt("Url Transaction","printlog",0,
            strLogConfig);
    if(nTemp == 1)
        return TRUE;
    else
        return FALSE;
}

BSTR CHttpClient::UTF2Unicode(LPCSTR lp, int nLen)
{
   BSTR str = NULL;
   int nConvertedLen = MultiByteToWideChar(CP_UTF8, 0, lp,
     nLen, NULL, NULL);
 
   // BUG FIX #1 (from Q241857): only subtract 1 from 
   // the length if the source data is nul-terminated
   if (nLen == -1)
      nConvertedLen--;
 
   str = ::SysAllocStringLen(NULL, nConvertedLen);
   if (str != NULL)
   {
     MultiByteToWideChar(CP_UTF8, 0, lp, nLen, str, nConvertedLen);
   }
   return str;
}
