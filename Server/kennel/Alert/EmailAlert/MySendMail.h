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
		//smtp��������ַ
		char* smptAddress; 
		//�ŵ�����
		char* content;
		//����������
		char* sendName;
		//����������
		char* senderAddress;
		//����������
		char* receiverName;
		//��������
		char* receiverAddress;
		//�ʼ�����
		char* mailSubject;

		//�û���
		char* user;
		//����
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
