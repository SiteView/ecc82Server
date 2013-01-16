#ifndef __UPDATE_SIP
#define __UPDATE_SIP
#include "stdafx.h"

typedef bool SipFun(const char * strParas , char * szReturn , int &nSize);
void WriteLog(const char * strMsg);
void GetErrorMsg(LPSTR lpBuffer);

class Update
{
public:
	int			GetUpdatingNum();									//ͳ�Ƶ�ǰ�����������豸��Ŀ
	bool		SendDownloadMsg(string strDeviceID);				//��WebService�������������ļ�����
	static void	ConvertTime(string strTime , time_t &tTime);		//����2009-10-09 14:58:14��ʱ�䴮ת��ʱ��Ϊtime_t����
	bool		GetSendMsg(string strDeviceID);						//��ECC�������ļ�Download.ini�л�ȡ��Ҫ���͵���Ϣ
	
	bool		Init();
	bool		Run();

private:
	time_t		current_time;										//��ǰ��ϵͳʱ�䣬����ƥ���Ƿ��˷������������ļ������ʱ��
	string		strDeviceID;
	string		strBeginTime;										//��ʼ����ʱ��
	string		strEntityId;
	string		strVer;
	string		strFileName;
	string		strFileType;
	string		strFileSize;
	string		strCommandKey;
	string		strIp;
	string		strURL;

	string		strUsername;										//����վ���û���
	string		strPassword;										//����վ������
	string		strUpCount;
	string		strSuccessURL;
	string		strFailureURL;

	string		CPE_UserName;										//CPE�豸�û���
	string		CPE_PassWord;										//CPE�豸����
};
#endif