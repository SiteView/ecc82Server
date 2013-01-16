
#include "Mail.h"
#include "Base64.h"
#include "MIMEMessage.h"
#include "SMTP.h"
#include "Pop3.h"
#include "IMAP4Connection.h"
#include "Utils.h"

#include "C:\SiteView\Ecc_Common\Base\funcGeneral.h"


BOOL MAIL_MONITOR(int nMailType, CString strSmtpServ, int nSmtpPort, CString strFrom, 
				  CString strRcptTo, int nProtoType, CString strRcptServ, int nRcptPort, 
				  CString strRcptUser, CString strRcptPass, CString strMatch, int nTimeout, 
				  int nCheckDelay, CString strAttach, int nNeedAuth, CString strVerifyUsername, 
				  CString strVerifyPassword, CString strCustomerPath, char *szReturn)
{
	
	WriteLogFile1(strRcptTo);

	if(!nSmtpPort) nSmtpPort = 25;
	if(!nProtoType) nRcptPort = 110; else nRcptPort = 143;

	DWORD	dwBeginTime = ::GetTickCount();
	DWORD	dwEndTime = 0;

	BOOL	bResult = FALSE;
	//0 -- send & receive	1 -- receive only 2--Send only
	if(nMailType != 1)
	{
		bResult = SendMail(strSmtpServ, nSmtpPort, strFrom, strRcptTo, strAttach, nNeedAuth, strVerifyUsername, strVerifyPassword, szReturn);
		if(bResult)
		{

			printf("---nCheckDelay=---%d-------------",nCheckDelay);
			Sleep(nCheckDelay * 1000);
			if(!nProtoType)
				bResult = RecvMail(strRcptServ, nRcptPort, strRcptUser, strRcptPass, strMatch, szReturn);
			else
				bResult = RecvMail2(strRcptServ, nRcptPort, strRcptUser, strRcptPass, strMatch, szReturn);
		}
	}
	
	if(nMailType != 2)
	{
		if(!nProtoType)
			bResult = RecvMail(strRcptServ, nRcptPort, strRcptUser, strRcptPass, strMatch, szReturn);
		else
			bResult = RecvMail2(strRcptServ, nRcptPort, strRcptUser, strRcptPass, strMatch, szReturn);
	}

	if(bResult)
	{
		dwEndTime = ::GetTickCount();
		sprintf(szReturn, "roundTripTime=%ld$", dwEndTime - dwBeginTime);
	}

	return bResult;
}

BOOL SendMail(CString SMTP_Server, int nPort, CString From, CString To, 
			  CString Attachment, int nNeedAuth, CString vUid, CString vPwd, char *szReturn)
{
	CMIMEMessage msg;
	CSMTP smtp( SMTP_Server );

	char hostname[256] = {0};
	char ipaddress[20] = {0};
	gethostname(hostname, sizeof(hostname));
	if(*hostname)
	{
		struct hostent *hp;
		hp = gethostbyname(hostname);
		if(hp) strcpy(ipaddress, inet_ntoa(*((struct in_addr *)hp->h_addr)));
	}

	msg.m_sFrom    = From;
	msg.m_sSubject.Format("%s, %s", FuncGetStringFromIDS("<%IDS_mail_1%>"), ipaddress);//<%IDS_mail_1%>
	CTime curTime = CTime::GetCurrentTime();
	msg.m_sBody.Format("%s, %d-%2.2d-%2.2d %2.2d:%2.2d\r\n\r\n", FuncGetStringFromIDS("<%IDS_mail_2%>"),
						curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), 
						curTime.GetHour(), curTime.GetMinute());//<%IDS_mail_2%>
	msg.m_sBody += _T("-----------------------------------------------------\r\n");
//	msg.m_sBody += _T("北京游龙网网络科技有限公司\r\n");
//	msg.m_sBody += _T("地址：　　北京西长安街88号首都时代广场西翼第二座13层\r\n");
//	msg.m_sBody += _T("网址： 　 www.dragonflow.net\r\n");
//	msg.m_sBody += _T("邮编：　　100031\r\n");
//	msg.m_sBody += _T("电话：　　8610—83915361、83915362\r\n");
//	msg.m_sBody += _T("传真：　　8610—83915360\r\n");
//	msg.m_sBody += _T("电子邮件：contact@dragonflow.net\r\n");

	msg.AddMultipleRecipients(To);
	if(!Attachment.IsEmpty()) 
	{
		if((_access(Attachment, 0x04)) == -1)
		{
			if(errno == EACCES)
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_mail_3%>"));//<%IDS_mail_3%>
			else if(errno == ENOENT)
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_mail_4%>"));//<%IDS_mail_4%>
			else
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_mail_5%>"));//<%IDS_mail_5%>

			return FALSE;
		}

		msg.AddMIMEPart(Attachment);
	}

	//printf("-------%d----", nNeedAuth);
	if(!smtp.Connect(nNeedAuth, vUid, vPwd))
	{
		sprintf(szReturn, "error=%s", smtp.GetLastError());
		return FALSE;
	}
	puts("connect ok");
	if (!smtp.SendMessage( &msg ))
	{
		sprintf(szReturn, "error=%s", smtp.GetLastError());
		return FALSE;
	}
	puts("send ok");

	smtp.Disconnect();

	return TRUE;
}

BOOL RecvMail(CString POP3_Server, int nPort, CString strUid, CString strPwd, 
			  CString strMatch, char *szReturn)
{
	int	 count = 0;
	BOOL bResult = FALSE;

	CPop3Connection p3;
	if (!p3.Connect(POP3_Server, strUid, strPwd, nPort))
	{
		sprintf(szReturn, "error=%s", p3.GetLastError());
		return FALSE;
	}

	if (!p3.Noop())
	{
		sprintf(szReturn, "error=%s", p3.GetLastError());
		CString sError = p3.GetLastCommandResponse();
	}

	int nMails = 0;
	int nSize = 0;
	if (!p3.Statistics(nMails, nSize))
	{
		sprintf(szReturn, "error=%s", p3.GetLastError());
		CString sError = p3.GetLastCommandResponse();
		return FALSE;
	}

	if (nMails)
	{
		int i = 0;
		CString	strMessage;
		CPop3Message message;

		for(i = nMails;i > 0;i --)
		{
			strMessage = _T("");
			if(!p3.Top(i, strMessage))
			{
				sprintf(szReturn, "error=%s", p3.GetLastError());
				return FALSE;
			}

			//if(strMessage.Find("X-Mailer: DragonFlow\r\n", 0) >= 0)
			//{
				if (!p3.Retrieve(i, message))
				{
					sprintf(szReturn, "error=%s", p3.GetLastError());
					CString sError = p3.GetLastCommandResponse();
				}
				else
				{
					if(strstr(message.m_pszMessage, strMatch.GetBuffer(strMatch.GetLength())))
					{
						bResult = TRUE;
					}
					else
					{
						sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_mail_6%>"));//<%IDS_mail_6%>
						bResult = FALSE;
					}
				}

				break;
			//}
		}

		if (i != 0)
		{
			if (!p3.Delete(i))
			{
				DWORD dwError = GetLastError();
				CString sError = p3.GetLastCommandResponse();
			}
		}

//		if (!p3.Reset())
//		{
//			DWORD dwError = GetLastError();
//			CString sError = p3.GetLastCommandResponse();
//		}
	}
	else
	{
		// No Mail
		bResult = TRUE;
	}

	if (!p3.Disconnect())
	{
		DWORD dwError = GetLastError();
		CString sError = p3.GetLastCommandResponse();
	}

	return bResult;
}

BOOL RecvMail2(CString IMAP4_Server, int nPort, CString strUid, CString strPwd, 
			   CString strMatch, char *szReturn)
{
	BOOL bResult = FALSE;

	CIMAP4Connection imap4;
	if (!imap4.Connect(IMAP4_Server, strUid, strPwd, nPort))
	{
		sprintf(szReturn, "error=%s", imap4.m_sLastCommandResponse);
		return FALSE;
	}

	if (!imap4.Noop())
	{
	}

	if (!imap4.Select(_T("INBOX")))
	{
		sprintf(szReturn, "error=%s", imap4.GetLastError());
		imap4.Disconnect();
		return FALSE;
	}

	CIMAP4Message message;
	if (!imap4.Retrieve(1, message))
	{
		// No Mail
		bResult = TRUE;
	}
	else
	{
		if(strstr(message.m_pszMessage, strMatch.GetBuffer(strMatch.GetLength())))
		{
			bResult = TRUE;
		}
		else
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_mail_7%>"));//<%IDS_mail_7%>
			bResult = FALSE;
		}
	}

	if (!imap4.Disconnect())
	{
	}

	return bResult;
}