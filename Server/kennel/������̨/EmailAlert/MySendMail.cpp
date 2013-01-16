#include "stdafx.h"
#include "mysendmail.h"

using namespace SiteView_MySenderMail;
using namespace MUtils;

MySendMail* MySendMail::m_sendmail = NULL;

void WriteLog( const char* str );

MySendMail::MySendMail(void)
{
	smptAddress = NULL;
	content = NULL;
	sendName = NULL;
	senderAddress = NULL;
	receiverAddress = NULL;
	mailSubject = NULL;
	receiverName = NULL;
	user = NULL;
	password = NULL;
}

MySendMail::~MySendMail(void)
{
	
}
bool MySendMail::SendMail()
{
	WriteLog("=========== MySendMail::SendMail ==========");

	if(!smptAddress || !content || !sendName || !senderAddress || !receiverAddress || !mailSubject
		|| !user || !password || !receiverName)
	{
		return false;
	}
	WinSockHelper wshelper;
	SMailer::TextPlainContent  mailContent(content);
	SMailer::MailInfo info;
	info.setSenderName(sendName);
    info.setSenderAddress(senderAddress);
    info.addReceiver(receiverName, receiverAddress);
    
    info.setPriority(SMailer::Priority::normal);
    info.setSubject(mailSubject);
    info.addMimeContent(&mailContent);

	try
    {
		WriteLog("try send mail!");
		sender.GetMailBaseInfo(smptAddress, user, password);
		WriteLog("GetMailBaseInfo successful!");
        sender.setMail(&SMailer::MailWrapper(&info));
		WriteLog("setMail successful!");
        sender.sendMail();
		WriteLog("sendMail successful!");
    }
    catch (SMailer::MailException& e)
    {
		WriteLog("发生异常：");
		WriteLog(e.what());

		try
		{
			sender.quit();
		}
		catch(...)
		{
			int nErrId = ::GetLastError();
			TCHAR strErrInfo[512] = {0};
			::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				nErrId,
				0,
				strErrInfo,
				0,
				NULL
				);
			WriteLog(strErrInfo);

		}
        return false;
    }
    catch (...)
    {
		WriteLog("发生未知异常！");
		int nErr = ::GetLastError();
		TCHAR lpBuffer[512] = {0};
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS ,
			NULL,
			nErr,
			0,
			lpBuffer,
			0,
			NULL
			);
		WriteLog(lpBuffer);

		try
		{
			sender.quit();
		}catch(...)
		{
		}
        return false;
    }
	return true;
}

MySendMail* MySendMail::GetInstance()
{
	if(m_sendmail == NULL)
	{
		m_sendmail = new MySendMail();
	}
	return m_sendmail;
}
void MySendMail::DisposeInstance()
{
	if(m_sendmail != NULL)
	{
		delete m_sendmail;
	}
	m_sendmail = NULL;
}