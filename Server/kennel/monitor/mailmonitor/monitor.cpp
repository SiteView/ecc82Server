#include "monitor.h"
#include "Mime.h"
#include "SMTP.h"
#include "Pop3.h"
#include "IMAP4Message.h"
#include "IMAP4Connection.h"
#include "../base/SVDefines.h"
#include <fstream>

int PrintLog(const char * strParas)
{
	char timebuf[128]={0},datebuf[128]={0},tempbuf[10240]={0};
	try
	{
		_strtime(timebuf);
		_strdate(datebuf);
		sprintf(tempbuf,"%s-%s",datebuf,timebuf);
		ofstream filename;
		LPTSTR strRootPath = NULL;
		strRootPath = new TCHAR[1024];
		GetCurrentDirectory(1024,strRootPath);
		string strFileName=strRootPath;
		strFileName+="\\";
		strFileName+="MailMonitor.log";
		filename.open(strFileName.c_str(),ios::app);
		filename<<tempbuf<<"\t"<<strParas<<endl;
		filename.close();
		strRootPath=NULL;
		delete [] strRootPath;
	}
	catch(...)
	{
		return 0;
	}
	return 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 监测入口函数
// 参数表
// 输入参数 返回值 返回值缓冲区大小
// 返回值类型 bool(监测成功true|监测失败false)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DLL_EXPORT bool MAIL(const char *pszParams,  char *pszReturn, int &nSize)
{
	int			nSmtpPort       = 25,           // SMTP Server Default Port: 25
				nRcptPort       = 110, 		    // POP3 Server Default Port: 110
				nMailType       = 0,			// 0:send && receive mail || 1: Receive mail only || 2:Send mail only
				nProtoType      = 0, 		    // 0:POP3 || 1:IMAP4
                nSMTPNeedSPA    = 0,            // 0:Don't Use SPA Verify || 1:Use SPA Verify
                nRCPTNeedSPA    = 0,            // 0:Don't Use SPA Verify || 1:Use SPA Verify
				nNeedAuth       = 1,			// 0:Don't Need Auth Login || 1: Need Auth Login
                nHoldCopy       = 0,            // 0:不保留 || 1: 保留副本
                nTimeOut        = 10,           // Timeout: unit--second
                nCheckDelay     = 10;            // Check Mail delay time(second)

    string      szSMTPServer    = "",                                           // SMTP Sever
                szMailFrom      = "",                                           // Mail From
                szMailTo        = "",                                           // Mail To
                szSubject       = "Standard Email Monitor",                     // Mail's Default Subject
//                szContent       = "This is test mail from SiteView ECC 7.0.",   // Mail's Default Content
				szContent		= "This is test mail from SiteView ECC",		// Mail's Default Content
                szAttch         = "",                                           // Attch file's path
                szSMTPAuth      = "",                                           // User Name(Send Mail)
                szSMTPPwd       = "",                                           // User's Password(Send Mail)
                szRCPTServer    = "",                                           // Receive Mail Server
                szRCPTAuth      = "",                                           // User Name(Receive Mail)
                szRCPTPwd       = "",                                           // User's Password(Receive Mail)
                szMatch         = "";                                           // Match string
/**/
	const char * strReceive = pszParams;
	while(*strReceive)
	{
		PrintLog(strReceive);
		strReceive+=strlen(strReceive)+1;
	}

	HKEY hKey = NULL;
	LONG lRet = NULL;
	lRet = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\Siteview\\SiteviewECC",0,KEY_QUERY_VALUE,&hKey);
	char strTMP[1024]={0};
	sprintf(strTMP,"lRet=%d",lRet);
//	PrintLog(strTMP);
	if(lRet == ERROR_SUCCESS)
	{
		DWORD dwIndex = 0;
		while(1)
		{
			char szValueName[512]={0};
			DWORD dwSizeValue = sizeof(szValueName) -1;
			char szValue[1024] = {0};
			DWORD len = 1024;

			lRet = ::RegEnumValue(hKey,dwIndex, szValueName , &dwSizeValue, NULL , NULL , (LPBYTE)szValue , &len);
			if(lRet != ERROR_SUCCESS)
				break;
			if(strcmp(szValueName , "version")==0)
			{
				szContent="This is test mail from ";
				szContent += szValue;
			}
			dwIndex++;
		}
	}
	RegCloseKey(hKey);
	
    map<string, string, less<string> > InputParams;
    map<string, string, less<string> >::iterator paramItem;
    if(!splitparam(pszParams, InputParams))
    {
        nSize = sprintf(pszReturn , "error=%s", GetResourceValue("IDS_MONITOR_PARAM_ERROR").c_str());
        return false;
    }    

    //SV_MailType
    paramItem = InputParams.find(SV_MailType);
    if(paramItem != InputParams.end())
        nMailType = atoi(paramItem->second.c_str());
    
    //SV_Attachment
    paramItem = InputParams.find(SV_Attachment);
    if(paramItem != InputParams.end())  
        szAttch = paramItem->second;

    //SV_MatchStr
    paramItem = InputParams.find(SV_MatchStr);
    if(paramItem != InputParams.end())  
        szMatch = paramItem->second;
    
    //SV_HoldCopy
    paramItem = InputParams.find(SV_HoldCopy);
    if(paramItem != InputParams.end())  
    {
        if(paramItem->second == "true")
            nHoldCopy = 1;
        else if(!paramItem->second.empty() && paramItem->second != "false")
            nHoldCopy = atoi(paramItem->second.c_str());
    }

    //SV_TimeOut
    paramItem = InputParams.find(SV_TimeOut);
    if(paramItem != InputParams.end())  
        nTimeOut = atoi(paramItem->second.c_str());

    //SV_SMTPServerPort
    paramItem = InputParams.find(SV_SMTPServerPort);
    if(paramItem != InputParams.end())
    {
        string szServer = paramItem->second;
        int nPos = static_cast<int>(szServer.find(":"));
        if(nPos > 0)
        {
            szSMTPServer = szServer.substr(0, nPos);
            nSmtpPort = atoi(szServer.substr(nPos + 1).c_str());
            if(nSmtpPort <= 0)
            {
                sprintf(pszReturn, "error=%s", GetResourceValue("IDS_MAIL_SMTP_SERVER_PORT_ERROR").c_str());
                return false;
            }
            //    nSmtpPort = 25;
        }
        else
        {
            szSMTPServer = szServer;
        }
    }
    else if(nMailType != 1)
    {
        //缺少发件服务器
        sprintf(pszReturn, "error=%s", GetResourceValue("IDS_MAIL_SMTP_SERVER_IS_NULL").c_str());
        return false;
    }

    //SV_MailProtocol
    paramItem = InputParams.find(SV_MailProtocol);
    if(paramItem != InputParams.end())
        nProtoType = atoi(paramItem->second.c_str());

    //SV_RcptServerPort
    paramItem = InputParams.find(SV_RcptServerPort);
    if(paramItem != InputParams.end())
    {
        string szServer = paramItem->second;
        int nPos = static_cast<int>(szServer.find(":"));
        if(nPos > 0)
        {
            szRCPTServer = szServer.substr(0, nPos);
            nRcptPort = atoi(szServer.substr(nPos + 1).c_str());
            if(nRcptPort <= 0)
            {
                sprintf(pszReturn, "error=%s", GetResourceValue("IDS_MAIL_RCPT_SERVER_PORT_ERROR").c_str());
                return false;                
            }
            //    nRcptPort = 110;
        }
        else
        {
            szRCPTServer = szServer;
            if(nProtoType == 1)
                nRcptPort = 143;
        }
    }
    else if(nMailType != 2)
    {
        //缺少收件服务器
        sprintf(pszReturn, "error=%s", GetResourceValue("IDS_MAIL_RCPT_SERVER_IS_NULL").c_str());
        return false;
    }
    //SV_SendAddress
    paramItem = InputParams.find(SV_SendAddress);
    if(paramItem != InputParams.end())
    {
        szMailFrom = paramItem->second;
    }
    else if(nMailType != 1)
    {
        //缺少发件地址
        sprintf(pszReturn, "error=%s", GetResourceValue("IDS_MAIL_FROM_ADDRESS_IS_NULL").c_str());
        return false;
    }
    //SV_User
    paramItem = InputParams.find(SV_User);
    if(paramItem != InputParams.end())
    {
        szRCPTAuth = paramItem->second;
    }
    else if(nMailType != 2)
    {
		sprintf(pszReturn, "error=%s", GetResourceValue("IDS_MAIL_RCTP_SERVER_USERNAME_IS_NULL").c_str());
		return false;
    }

    //SV_Pwd
    paramItem = InputParams.find(SV_Pwd);
    if(paramItem != InputParams.end())
        szRCPTPwd = paramItem->second;
    //SV_RcptAddress
    paramItem = InputParams.find(SV_RcptAddress);
    if(paramItem != InputParams.end())
    {
        szMailTo = paramItem->second;
    }
    else
    {
        //缺少收件地址
        sprintf(pszReturn, "error=%s", GetResourceValue("IDS_MAIL_TO_ADDRESS_IS_NULL").c_str());
        return false;
    }    
    //SV_CheckDelay
    paramItem = InputParams.find(SV_CheckDelay);
    if(paramItem != InputParams.end())
    {
        nCheckDelay = atoi(paramItem->second.c_str());
        if(nCheckDelay < 0)
        {
            sprintf(pszReturn, "error=%s", GetResourceValue("IDS_MAIL_CHECK_DELAY_ERROR").c_str());
            return false;
        }
    }// ?? 需要和王老大确认是否需要输入此值，现在已使用了 5 作为缺省值
    //else
    //{
    //    sprintf(pszReturn, "error=输入检测邮件时间间隔错误");//, GetResourceValue("IDS_MAIL_CHECK_DELAY_ERROR"));
    //    return false;
    //}

    //SV_SMTPAuth
    paramItem = InputParams.find(SV_SMTPAuth);
    if(paramItem != InputParams.end())
    {
        if(paramItem->second == "true")
            nNeedAuth = 1;
        else if(!paramItem->second.empty() && paramItem->second != "false")
            nNeedAuth = atoi(paramItem->second.c_str());
        else if(paramItem->second == "false")
        {
            nNeedAuth = 0;
        }
    }

    //SV_VerifyUsername
    paramItem = InputParams.find(SV_VerifyUsername);
    if(paramItem != InputParams.end())
    {
        szSMTPAuth = paramItem->second;
    }
    else if(nNeedAuth == 1)
    {
        sprintf(pszReturn, "error=%s", GetResourceValue("IDS_MAIL_SMTP_SERVER_USERNAME_IS_NULL").c_str());
        return false;
    }
    //SV_VerifyPassword    
    paramItem = InputParams.find(SV_VerifyPassword);
    if(paramItem != InputParams.end())
        szSMTPPwd = paramItem->second;

    //SV_SMTPSPA
    paramItem = InputParams.find(SV_SMTPSPA);
    if(paramItem != InputParams.end())
    {
        if(paramItem->second == "true")
            nSMTPNeedSPA = 1;
    }

    //SV_RcptSPA
    paramItem = InputParams.find(SV_RcptSPA);
    if(paramItem != InputParams.end())
    {
        if(paramItem->second == "true")    
            nRCPTNeedSPA = 1;
    }
	if(SV_NetInit())
	{
		sprintf(pszReturn, "error=%s", GetResourceValue("IDS_NET_INIT_FAILED").c_str());//<%IDS_Monitor_61%>"通信初始化错误"
		return FALSE;
	}
    bool bRet =  mailmonitor(nMailType, szSMTPServer.c_str(), nSmtpPort, szMailFrom.c_str(), szMailTo.c_str(),
        szSubject.c_str(), szContent.c_str(), szAttch.c_str(), szSMTPAuth.c_str(), szSMTPPwd.c_str(), nNeedAuth, nSMTPNeedSPA,
        nTimeOut, nCheckDelay, szRCPTServer.c_str(), nProtoType, nRcptPort, nRCPTNeedSPA, nHoldCopy,
        szRCPTAuth.c_str(), szRCPTPwd.c_str(), szMatch.c_str(), pszReturn, nSize);
    SV_NetFree();
    return bRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// 拆分参数表
//// 参数表
//// 输入参数 参数表
//// 返回值类型 bool(拆分成功true|拆分失败false) 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//bool splitparam(const char *pszParams, map<string, string, less<string> > &Params)
//{
//    const char * pPos = pszParams;
//    while(*pPos != '\0')
//    {
//        int nSize = strlen(pPos);
//        cout << pPos << endl;
//        char *chTmp  = strdup(pPos);
//        if(chTmp)
//        {
//            char *pEqualSign = strchr(chTmp, '=');
//            if(pEqualSign)
//            {
//                *pEqualSign = '\0';
//                pEqualSign ++;
//                Params[chTmp] = pEqualSign;
//            }
//            free(chTmp);
//        }
//        else
//        {
//            return false;
//        }
//        pPos = pPos + nSize + 1;
//    }
//    return true;
//}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 进行监测
// 参数表
// 邮件监测类型（0发送接收1只发送2只接收） 发送邮件服务器 发送邮件服务器端口 
// 邮件来源地址 邮件送达地址 邮件标题 
// 邮件内容 邮件附件 发送邮件服务器身份验证用户名 
// 发送邮件服务器身份验证密码 我的发件服务器是否需要身份验证 发件服务器是否使用安全密码验证
// 登陆超时时间 校验等待时间 接收邮件服务器
// 接收邮件服务器类型(0POP3||1IMAP4) 接收邮件服务器端口 接收邮件服务器是否使用安全密码验证 
// 是否在服务器上保留邮件副本 接收邮件服务器身份验证用户名 接收邮件服务器身份验证密码
// 比较字符串 返回值 返回值回写长度
// 返回值类型 bool(监测成功true|监测失败false)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool mailmonitor(const int &nMonitorType, const char *pszSMTPServer, const int &nSMTPPort, 
                 const char *pszMailFrom, const char *pszMailTo, const char *pszSubject, 
                 const char *pszMailContent, const char *pszAttch, const char *pszAuthUser, 
                 const char *pszAuthPwd, const int &nAuthType, const int &nSMTPNeedSPA, 
                 const int &nTimeout, const int &nCheckDelay, const char *pszRcptServer, 
                 const int &nRcptType, const int &nRcptPort, const int & nRCPTNeedSPA, 
                 const int &nHoldCopy, const char *pszRcptUser, const char * pszRcptPwd, 
                 const char* pszMatch, char *pszReturn, int &nSize)
{
    bool bRet = false;
    unsigned long uStartTime = GetTickCount();
    unsigned long uEndTime = 0;
    if(nMonitorType != 1)
    {
        bRet = sendmail(pszSMTPServer, nSMTPPort, pszMailFrom, pszMailTo, pszSubject, pszMailContent, 
            pszAttch, pszAuthUser, pszAuthPwd, nAuthType, nSMTPNeedSPA, nTimeout, pszReturn, nSize);
        if(bRet && nMonitorType == 0)
        {
            Sleep(nCheckDelay * 1000);
            if(nRcptType == 0)
                bRet = recvpopmail(pszRcptServer, nRcptPort, nRCPTNeedSPA, pszRcptUser, pszRcptPwd, 
                                    pszMatch, nTimeout, nHoldCopy, pszReturn, nSize);
            else
                bRet = recvimapmail(pszRcptServer, nRcptPort, nRCPTNeedSPA, pszRcptUser, pszRcptPwd, pszMatch, nTimeout, pszReturn, nSize);
        }
    }
    else if(nMonitorType == 1)
    {
        if(nRcptType == 0)
            bRet = recvpopmail(pszRcptServer, nRcptPort, nRCPTNeedSPA, pszRcptUser, pszRcptPwd, 
                                    pszMatch, nTimeout, nHoldCopy, pszReturn, nSize);
        else
            bRet = recvimapmail(pszRcptServer, nRcptPort, nRCPTNeedSPA, pszRcptUser, pszRcptPwd, pszMatch, nTimeout, pszReturn, nSize);
    }

    if(bRet)
    {
        uEndTime = GetTickCount();
		sprintf(pszReturn, "roundTripTime=%ld", uEndTime - uStartTime);
    }

    return bRet;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 发送邮件函数
// 参数表
// 发送邮件服务器 发送邮件服务器端口 邮件来源地址
// 邮件送达地址 邮件标题 邮件内容
// 邮件附件 身份验证用户名 身份验证密码
// 我的发件服务器是否需要身份验证 是否使用安全密码验证 登陆超时时间 返回值
// 返回值回写长度
// 返回值类型 bool(发送成功true|发送失败false)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sendmail(const char *pszSMTPServer, const int &nSMTPPort, const char *pszMailFrom, 
              const char *pszMailTo, const char *pszSubject, const char *pszMailContent, 
              const char *pszAttch, const char *pszAuthUser, const char *pszAuthPwd, 
              const int &nAuthType, const int &nNeedSPA, const int &nTimeout, char *pszReturn,
              int &nSize)
{
	PrintLog("========sendmail========\n");
	char strTemp[1024]={0};
    bool bRet = false;
    CSMTP smtp(pszSMTPServer, nSMTPPort);
    if(smtp.Connect(nAuthType, nNeedSPA, pszAuthUser, pszAuthPwd))
    {
		sprintf(strTemp,"pszAuthUser=%s",pszAuthUser);
		PrintLog(strTemp);
		sprintf(strTemp,"pszAuthPwd=%s",pszAuthPwd);
		PrintLog(strTemp);

        CMimeMessage mail;
        mail.SetFrom(pszMailFrom);
        mail.SetTo(pszMailTo);
        mail.SetSubject(pszSubject);
        mail.SetText(pszMailContent);
        if(pszAttch && strlen(pszAttch))
        {
            CMimeBody *pMimeBody = mail.CreatePart();
            if(pMimeBody)
            {
                pMimeBody->SetTransferEncoding("base64");
                pMimeBody->ReadFromFile(pszAttch);
            }
        }   
        bRet = smtp.SendMail(mail);
        if(!bRet)
		{
            sprintf(pszReturn, "error=%s", smtp.GetLastError());
		}
        smtp.Disconnect();
    }
    else
    {
        sprintf(pszReturn, "error=%s", smtp.GetLastError());
    }
	PrintLog("========sendmail End========!\n");
    return bRet;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 接收POP3邮件函数
// 参数表
// 接收邮件服务器  接收邮件服务器端口 是否使用安全密码验证登陆 接收邮件服务器身份验证用户名
// 身份验证密码 比较字符串 超时时长 是否在服务器上保留邮件副本
// 返回值 回写返回值长度
// 返回值类型 bool(接收成功true|接收失败false)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool recvpopmail(const char *pszRcptServer, const int &nRcptPort, const int &nNeedSPA, const char *pszRcptUser, 
              const char * pszRcptPwd, const char* pszMatch, const int &nTimeout, const int &nHoldCopy, 
              char *pszReturn, int &nBuffSize)
{
	int	 count = 0;
	bool bResult = false;

	CPop3Connection p3;
	if (!p3.Connect(pszRcptServer, pszRcptUser, pszRcptPwd, nNeedSPA, nRcptPort))
	{
		sprintf(pszReturn, "error=%s", p3.GetLastErrorInfor().c_str());
		return false;
	}

	if (!p3.Noop())
	{
		sprintf(pszReturn, "error=%s", p3.GetLastCommandResponse().c_str());
		//string sError = p3.GetLastCommandResponse();
        return false;
	}

	int nMails = 0;
	int nSize = 0;
	if (!p3.Statistics(nMails, nSize))
	{
		sprintf(pszReturn, "error=%s", p3.GetLastErrorInfor().c_str());
		//string sError = p3.GetLastCommandResponse();
		return false;
	}

    if (nMails)
	{
		int i = 0;
		string	strMessage;
		CPop3Message message;

		for(i = nMails;i > 0;i --)
		{
			strMessage = "";
			if(!p3.Top(i, strMessage))
			{
				sprintf(pszReturn, "error=%s", p3.GetLastErrorInfor().c_str());
				return false;
			}

			if (!p3.Retrieve(i, message))
			{
				sprintf(pszReturn, "error=%s", p3.GetLastErrorInfor().c_str());
				//string sError = p3.GetLastCommandResponse();
			}
			else
			{
                cout << "Email Message Text: " << message.GetMessageText() << endl;
                if(pszMatch && strlen(pszMatch))
                {
				    if(strstr(message.m_pszMessage, pszMatch))
				    {
					    bResult = true;
				    }
				    else
				    {
					    sprintf(pszReturn, "error=%s",GetResourceValue("IDS_MatchStringFailed").c_str()); //匹配字符串失败");//, GetResourceValue("<%IDS_mail_6%>"));//<%IDS_mail_6%>
					    bResult = false;
				    }
                }
                else
                    bResult = true;
			}
			break;
		}

		if (i != 0)
		{
            if(nHoldCopy != 1)
            {
            cout << "delete mail message id is: " << i << endl;
			    if (!p3.Delete(i))
			    {
				    sprintf(pszReturn, "error=%s", p3.GetLastErrorInfor().c_str());
		            //string sError = p3.GetLastCommandResponse();
		            return false;
			    }
            }
		}
	}
	else
	{
		// No Mail
		bResult = true;
	}

	if (!p3.Disconnect())
	{
		unsigned long dwError = GetLastError();
		string sError = p3.GetLastCommandResponse();
	}

	return bResult;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 接收IMAP邮件函数
// 参数表
// 接收邮件服务器  接收邮件服务器端口 是否使用安全密码验证登陆 接收邮件服务器身份验证用户名
// 身份验证密码 比较字符串 超时时长 返回值 回写返回值长度
// 返回值类型 bool(接收成功true|接收失败false)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool recvimapmail(const char *pszRcptServer, const int &nRcptPort, const int &nNeedSPA, const char *pszRcptUser, 
              const char * pszRcptPwd, const char* pszMatch, const int &nTimeout, char *pszReturn, int &nSize)
{
	bool bResult = false;

	CIMAP4Connection imap4;
	if (!imap4.Connect(pszRcptServer, pszRcptUser, pszRcptPwd, nRcptPort))
	{
		sprintf(pszReturn, "error=%s", imap4.m_sLastCommandResponse);
		return false;
	}

	if (!imap4.Noop())
	{
	}

	if (!imap4.Select("INBOX"))
	{
		sprintf(pszReturn, "error=%s", imap4.GetLastError());
		imap4.Disconnect();
		return false;
	}

	CIMAP4Message message;
	if (!imap4.Retrieve(1, message))
	{
		// No Mail
		bResult = true;
	}
	else
	{
        if(pszMatch && strlen(pszMatch))
        {
		    if(strstr(message.m_pszMessage, pszMatch))
		    {
			    bResult = true;
		    }
		    else
		    {
			    sprintf(pszReturn, "error=%s",GetResourceValue("IDS_MatchStringFailed").c_str());//匹配字符串失败");//, GetResourceValue("<%IDS_mail_6%>"));//<%IDS_mail_6%>
			    bResult = false;
		    }
        }
        else
            bResult = true;
	}

	imap4.Disconnect();
	return bResult;
}


