#pragma once
#include "SMailer.h"
#include "WinSockHelper.h"

namespace SiteView_MySenderMail
{
	class MySendMail
	{
	public:
		~MySendMail(void);
	private:
		MySendMail(void);
	public:
		//smtp服务器地址
		char* smptAddress; 
		//信的内容
		char* content;
		//发送者姓名
		char* sendName;
		//发送者邮箱
		char* senderAddress;
		//接收者姓名
		char* receiverName;
		//接收邮箱
		char* receiverAddress;
		//邮件主题
		char* mailSubject;

		//用户名
		char* user;
		//密码
		char* password;
	public:
		bool SendMail();
		static MySendMail* GetInstance();
		static void DisposeInstance();
	private:
		static MySendMail* m_sendmail;
		SMailer::MailSender sender;
	};
	
};
