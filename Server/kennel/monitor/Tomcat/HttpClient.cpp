// HttpClient.cpp: implementation of the CHttpClient class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TomCat.h"
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

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHttpClient::CHttpClient()
{

}

CHttpClient::~CHttpClient()
{

}

/////////////////////////////////////////////////////////////////////////////////
// 函数：
// 说明：
// 参数：
//
// 返回值：
/////////////////////////////////////////////////////////////////////////////////
BOOL CHttpClient::RequestGet(HINTERNET hConnect ,CString strObject ,DWORD dwFlags
                          ,CString strInfo, CString strMatch, CString *pszReturn)
{
    HINTERNET hRequest = NULL;

    const char * chType = "*/*";
    hRequest = HttpOpenRequest (hConnect,  TEXT("GET"), (LPCTSTR)strObject, 
        TEXT("HTTP/1.0"), NULL, &chType, dwFlags, 0);

    if(!hRequest)
    {
        CString strTemp = pszReturn->GetBuffer(0);
        pszReturn->Format("error=Requset Failed(%s)!$",strTemp);
        return FALSE;
    }
	BOOL bSendRequest = FALSE;
	DWORD dwHeadLen = 0 , dwOptionLen = 0;				
	dwOptionLen = strInfo.GetLength();
    //请求方式
	char* strType = "Content-Type: application/x-www-form-urlencoded"; 
	int nTypeLen = strlen(strType);	
    //发送请求
	bSendRequest = HttpSendRequest(hRequest, 
        strType,
		nTypeLen,
		NULL,
		NULL);

	if(!bSendRequest)
	{//请求失败
        CString strTemp = pszReturn->GetBuffer(0);
        pszReturn->Format("error=Send Requset Failed(%s)!$",strTemp);
        return FALSE;
    }
	DWORD dwLen = 1024;
    char *chCookie = (char*)malloc(1024);
    memset(chCookie ,0 ,1024);
    //得到以"\r\n"分割的Http请求返回头
    if(!HttpQueryInfo(hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, chCookie, &dwLen, NULL))
    {//失败
        DWORD dwErr=GetLastError();//Get Last Error
		CString strError=GetErrorMessage(dwErr);// Get Error Message
        CString strTemp = pszReturn->GetBuffer(0);
		pszReturn->Format("error=HttpQueryInfo:%s(%s)$",strError,strTemp);
		InternetCloseHandle(hRequest);
		return FALSE;
    }
    GetCookies(chCookie);  //从返回头中得到Cookie  
    int nStatus = GetStatus(chCookie);//得到返回状态码
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
    if(!strMatch.IsEmpty())
    {//匹配字符串不为空        
        bMatch = ContentMatch(strContent,strMatch);
        if(!bMatch)
        {//匹配失败
            CString strTemp = pszReturn->GetBuffer(0);
            pszReturn->Format("error=匹配失败(%s)!$",strTemp);
            InternetCloseHandle(hRequest);//关闭请求
            delete chCookie;
            return FALSE;
        }
    }
    delete chCookie;
	InternetCloseHandle (hRequest);//关闭请求
    return bMatch;
}

/////////////////////////////////////////////////////////////////////////////////
// 函数：
// 说明：
// 参数：
//
// 返回值：
/////////////////////////////////////////////////////////////////////////////////
BOOL CHttpClient::RequestPost(HINTERNET hConnect ,CString strObject ,DWORD dwFlags
                          ,CString strInfo,CString strMatch, CString *pszReturn)
{
    HINTERNET hRequest = NULL;
    //以POST方式打开请求
    const char * chType = "*/*";
    hRequest = HttpOpenRequest (hConnect, TEXT("POST"), (LPCTSTR)strObject, 
        TEXT("HTTP/1.0"), NULL, &chType, dwFlags, 0);

    if(!hRequest)
    {//请求失败
        CString strTemp = pszReturn->GetBuffer(0);
        pszReturn->Format("error=Requset Failed(%s)!$",strTemp);
        return FALSE;
    }
	BOOL bSendRequest = FALSE;
	DWORD dwHeadLen = 0 , dwOptionLen = 0;				
	dwOptionLen = strInfo.GetLength();
    //请求方式
	char* strType = "Content-Type: application/x-www-form-urlencoded"; 
	int nTypeLen = strlen(strType);	
    //发送请求
	bSendRequest = HttpSendRequest(hRequest, 
        strType,//Request Type
		nTypeLen, // Request Type Length
		(LPVOID)(strInfo.GetBuffer(strInfo.GetLength())),//Post Para
		dwOptionLen//Post Para Length
        );
	

	if(!bSendRequest)
	{//请求失败
        CString strTemp = pszReturn->GetBuffer(0);
        pszReturn->Format("error=Send Requset Failed(%s)!$",strTemp);
        return FALSE;
    }

	DWORD dwLen = 1024;
    char *chCookie = (char*)malloc(1024);
    memset(chCookie ,0 ,1024);
    //得到以"\r\n"分割的Http请求返回头
    if(!HttpQueryInfo(hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, chCookie, &dwLen, NULL))
    {//失败
        DWORD dwErr=GetLastError();//Get Last Error
		CString strError=GetErrorMessage(dwErr);// Get Error Message
        CString strTemp = pszReturn->GetBuffer(0);
		pszReturn->Format("error=HttpQueryInfo:%s(%s)$",strError,strTemp);
		InternetCloseHandle(hRequest);
		return FALSE;
    }
    GetCookies(chCookie);  //从返回头中得到Cookie  
    int nStatus = GetStatus(chCookie);//得到返回状态码
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
    if(!strMatch.IsEmpty())
    {//匹配字符串不为空
        bMatch = ContentMatch(strContent,strMatch);
        if(!bMatch)
        {//匹配失败
            CString strTemp = pszReturn->GetBuffer(0);
            pszReturn->Format("error=%s(%s)!$",FuncGetStringFromIDS("IDS_MatchFailed"),strTemp);//匹配失败(%s)!$",strTemp);
            InternetCloseHandle(hRequest);//关闭请求
            delete chCookie;
            return FALSE;
        }
    }
    delete chCookie;
    InternetCloseHandle(hRequest);//关闭请求
    return bMatch;
}

/////////////////////////////////////////////////////////////////////////////////
// 函数：
// 说明：
// 参数：
//
// 返回值：
/////////////////////////////////////////////////////////////////////////////////
BOOL CHttpClient::HttpConnection(LPCSTR szUrl,LPCSTR szPara,int nRequestType,
                             CString strMatch, CString *strMsg)
{
    DWORD     dwSize = 0, 
		      dwFlags = INTERNET_FLAG_RELOAD;     
	int nReturn = -1;
    
	HINTERNET hOpen = NULL;
    //以Http方式打开Internet访问
    hOpen = InternetOpen (TEXT("CeHttp"), 0,NULL, 0, 1);
	if(hOpen)//成功打开
	{
		CString       strServer = _T("");//服务器名称
		CString       strObject = _T("");
		CString       strWholePass = _T("");
		INTERNET_PORT nPort = 0;
		DWORD         dwServiceType = 0;//服务器类型
        BOOL bGetServer = AfxParseURL(szUrl,dwServiceType, strServer, 
            strObject, nPort);//分解请求URL		
		HINTERNET hConnect = NULL;
        //打开指定服务器的连接
		hConnect = InternetConnect (hOpen, (LPCTSTR)strServer, 
            nPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);		
		if(hConnect)//连接成功
		{
		    //nReturn = GetUrl(hConnect ,strObject,dwFlags,"");
            if(nRequestType == 0)
            {
                if(!RequestGet(hConnect ,strObject,dwFlags,szPara,strMatch,strMsg))
                    return FALSE;
            }
            else if(nRequestType == 1)
            {
                if(!RequestPost(hConnect ,strObject,dwFlags,szPara,strMatch,strMsg))
                    return FALSE;
            }
    		InternetCloseHandle (hConnect);//关闭连接
		}
        else
        {
            strMsg->Format("error=Connet Web Server Failed!$");
        }
		InternetCloseHandle (hOpen);//关闭
	}	
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
// 函数：ParseURL                                                               //
// 说明：分割url，得到主机名端口等
// 参数：
//
// 返回值：无
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
// 函数：MakeReturnString                                                       //
// 说明：根据状态返回码和原始返回值构造返回信息                                 //
// 参数：                                                                       //
//      strReturn，原始返回值                                                   //
//      nStatus，状态返回码                                                     //
// 返回值：无                                                                   //
//////////////////////////////////////////////////////////////////////////////////
void CHttpClient::MakeReturnString(CString *strReturn, int nStatus)
{
    CString strTemp = strReturn->GetBuffer(0);
    //strReturn->Format("%sStep%dStatusCode=%d$",strTemp,nStep,nStatus);
}

/////////////////////////////////////////////////////////////////////////////////
// 函数：
// 说明：
// 参数：
//
// 返回值：
/////////////////////////////////////////////////////////////////////////////////
BOOL CHttpClient::ValidHostChar(char ch)
{
    return( isalpha(ch) || isdigit(ch)
        || ch=='-' || ch=='.' || ch==':' );
}

//////////////////////////////////////////////////////////////////////////////////
// 函数：Monitor                                                                //
// 说明：根据监测脚本文件进行监测                                               //
// 参数：                                                                       //
//      strFileName，监测脚本文件名称                                           //
// 返回值：监测结果                                                             //
//////////////////////////////////////////////////////////////////////////////////
CString CHttpClient::Monitor(CStringList &paramList)
{
    CString strMsg = _T("");
    return strMsg;//返回监测结果
}

//////////////////////////////////////////////////////////////////////////////////
// 函数：GetCookies                                                             //
// 说明：根据HTTP头得到Cookie                                                   //
// 参数：                                                                       //
//      strHeaderBuffer，http头                                                 //
// 返回值：无                                                                   //
//////////////////////////////////////////////////////////////////////////////////
void CHttpClient::GetCookies(CString strHeaderBuffer)
{
    strHeaderBuffer.MakeLower();
    int nIndex = 0;
    while(nIndex != -1)
    {   
        //查找每一条Cookie
        nIndex = strHeaderBuffer.Find("set-cookie: ") + 12;
        if(nIndex>=12)
        {
            //得到Cookie内容
            strHeaderBuffer = strHeaderBuffer.Right(strHeaderBuffer.GetLength()-nIndex);
            nIndex = strHeaderBuffer.Find("\r\n",0);
            HttpCookie cookie;
            sprintf(cookie.chCookie,"%s",strHeaderBuffer.Left(nIndex));
            //添加到对列
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
// 函数：GetContentLength                                                       //
// 说明：根据Http头得到返回内容长度                                             //
// 参数：                                                                       //
//      strHttpHeader，Http头                                                   //
// 返回值：返回内容长度                                                         //
//////////////////////////////////////////////////////////////////////////////////
int CHttpClient::GetContentLength(CString strHttpHeader)
{
    if(m_bWriteLog)
        WriteRecvHeader(strHttpHeader);
    int nContentLength = 0;
    strHttpHeader.MakeLower();
    //查找content-length
    int nIndex = strHttpHeader.Find("content-length: ",0)+16;
    if(nIndex>=16)
    {//包含content-length
        strHttpHeader = strHttpHeader.Right(strHttpHeader.GetLength()-nIndex);
        nIndex = strHttpHeader.Find("\r\n");
        if(nIndex != -1)
            nContentLength = atoi(strHttpHeader.Left(nIndex));
    }
    return nContentLength;//返回长度
}

//////////////////////////////////////////////////////////////////////////////////
// 函数：ContentMatch                                                           //
// 说明：在原始字符串中查找是否存在当前字符串                                   //
// 参数：                                                                       //
//      strContent，原始字符串                                                  //
//      strMatch，当前字符串                                                    //
// 返回值：成功为TRUE                                                           //
//////////////////////////////////////////////////////////////////////////////////
BOOL CHttpClient::ContentMatch(CString strContent, CString strMatch)
{
    int nIndex = strContent.Find(strMatch , 0);
    if(nIndex == -1)
    {//查找失败
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

//////////////////////////////////////////////////////////////////////////////////
// 函数：WriteRecvHeader                                                        //
// 说明：写请求返回HTTP头内容到日志文件中                                       //
// 参数：                                                                       //
//      strHttpHeader，请求返回Http头内容                                       //
// 返回值：无                                                                   //
//////////////////////////////////////////////////////////////////////////////////
void CHttpClient::WriteRecvHeader(CString strContent)
{
    FILE *stream;
    char chTime[50] = {0};
    //打开文件
    stream=fopen(FuncGetInstallRootPath() +"\\logs\\UrlTransRecv.log","a");
   	SYSTEMTIME time;
	//取系统当前时间
	GetLocalTime(&time);
    //时间格式：年-月-日 时-分-秒
    sprintf(chTime ,"%02dY-%02dM-%02dD %02dh-%02dm-%02ds\r\n\r\n",
				time.wYear - 2000,
				time.wMonth,
				time.wDay,
				time.wHour,
				time.wMinute,
				time.wSecond);
    fputs(chTime , stream);//时间
    if(!strContent.IsEmpty())
    {//内容不为空
        fputs(strContent.GetBuffer(strContent.GetLength()),stream);
    }
    fputs("\r\n\r\n",stream);//回车换行
	fclose(stream); //关闭文件
}

//////////////////////////////////////////////////////////////////////////////////
// 函数：WriteRecvContent                                                       //
// 说明：写接收信息内容到日志文件中                                             //
// 参数：                                                                       //
//      strContent，信息内容                                                    //
// 返回值：无                                                                   //
//////////////////////////////////////////////////////////////////////////////////
void CHttpClient::WriteRecvContent(CString strContent)
{
    FILE *stream;
    char chTime[50] = {0};
    //打开文件
    stream=fopen(FuncGetInstallRootPath() +"\\logs\\UrlTransContent.log","a");
   	SYSTEMTIME time;
	//取系统当前时间
	GetLocalTime(&time);
    //时间格式：年-月-日 时-分-秒
    sprintf(chTime ,"%02dY-%02dM-%02dD %02dh-%02dm-%02ds\r\n\r\n",
				time.wYear - 2000,
				time.wMonth,
				time.wDay,
				time.wHour,
				time.wMinute,
				time.wSecond);
    fputs(chTime , stream);//时间
    if(!strContent.IsEmpty())
    {//内容不为空
        fputs(strContent.GetBuffer(strContent.GetLength()),stream);
    }
    fputs("\r\n\r\n",stream);//回车换行
	fclose(stream); //关闭文件
}
//////////////////////////////////////////////////////////////////////////////////
// 函数：ReadRequestContent                                                     //
// 说明：根据内容长度和请求句柄得到内容                                         //
// 参数：                                                                       //
//      hRequest，请求句柄                                                      //
//      nContentLen，内容长度                                                   //
// 返回值：请求内容                                                             //
//////////////////////////////////////////////////////////////////////////////////
CString CHttpClient::ReadRequestContent(HINTERNET hRequest , int nContentLen)
{
	DWORD lBuffer = 1024;
	char *lpBufferA = new CHAR [lBuffer+1];//分配内存
	BOOL bReadFile = FALSE;//读文件标志初始化
    CString strContent = _T("");
    if(nContentLen>0)
    {
        while(nContentLen>0)
        {	
            //读取文件(1024 byte)
            memset(lpBufferA, 0, lBuffer+1);//初始化变量
	        bReadFile = InternetReadFile(hRequest, (LPVOID)lpBufferA, lBuffer, &lBuffer);
	        if (!bReadFile)//读Internet文件失败
	        {
                delete(lpBufferA);//删除内存分配
                return _T("");
            } 
            nContentLen -= lBuffer;
            strContent += lpBufferA;
        }
    }
    else
    {//返回头中未得到内容长度
        while(lBuffer >0)
        {
            //读取文件(1024 byte)
            memset(lpBufferA, 0, lBuffer+1);//初始化变量
	        bReadFile = InternetReadFile(hRequest, (LPVOID)lpBufferA, lBuffer, &lBuffer);
	        if (!bReadFile)//读Internet文件失败
	        {
                break;
            } 
            strContent += lpBufferA;
        }
    }
    if(m_bWriteLog)
        WriteRecvContent(strContent);
    delete(lpBufferA);
    return strContent;
}

//////////////////////////////////////////////////////////////////////////////////
// 函数：GetErrorMessage                                                        //
// 说明：根据错误码得到错误描述                                                 //
// 参数：                                                                       //
//      dwErrorCode，错误码                                                     //
// 返回值：错误描述                                                             //
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
// 函数：GetStatus                                                              //
// 说明：根据HTTP请求返回头得到状态返回码                                       //
// 参数：                                                                       //   
//      strHeaderBuffer，http请求返回头                                         //
// 返回值：状态返回码                                                           //
//////////////////////////////////////////////////////////////////////////////////
int CHttpClient::GetStatus(CString strHeaderBuffer)
{
    int nIndex = 0 ,nStatus = 0;
    CString strHeaderTemp = strHeaderBuffer;
    strHeaderTemp.MakeLower();//全部转换为小写
    nIndex = strHeaderTemp.Find("http/",0) + 5;//协议头
    nIndex = strHeaderTemp.Find(" ", nIndex) + 1;//状态码前空格
    strHeaderTemp=strHeaderTemp.Right(strHeaderTemp.GetLength()-nIndex);
    nStatus = atoi(strHeaderTemp.Left(3));//状态码
    return nStatus;//返回
}
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
BOOL CHttpClient::IsPrintLog()
{
    CString strLogConfig = FuncGetInstallRootPath()+ "\\groups\\logconfig.ini";
    int nTemp = GetPrivateProfileInt("TomCat","printlog",0,
            strLogConfig);
    if(nTemp == 1)
        return TRUE;
    else
        return FALSE;
}
