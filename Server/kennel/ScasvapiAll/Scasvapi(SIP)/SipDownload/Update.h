#ifndef __UPDATE_SIP
#define __UPDATE_SIP
#include "stdafx.h"

typedef bool SipFun(const char * strParas , char * szReturn , int &nSize);
void WriteLog(const char * strMsg);
void GetErrorMsg(LPSTR lpBuffer);

class Update
{
public:
	int			GetUpdatingNum();									//统计当前正在升级的设备数目
	bool		SendDownloadMsg(string strDeviceID);				//向WebService发送下载升级文件请求
	static void	ConvertTime(string strTime , time_t &tTime);		//将“2009-10-09 14:58:14”时间串转化时间为time_t类型
	bool		GetSendMsg(string strDeviceID);						//从ECC的配置文件Download.ini中获取到要发送的信息
	
	bool		Init();
	bool		Run();

private:
	time_t		current_time;										//当前的系统时间，用以匹配是否到了发送下载升级文件请求的时间
	string		strDeviceID;
	string		strBeginTime;										//开始升级时间
	string		strEntityId;
	string		strVer;
	string		strFileName;
	string		strFileType;
	string		strFileSize;
	string		strCommandKey;
	string		strIp;
	string		strURL;

	string		strUsername;										//升级站点用户名
	string		strPassword;										//升级站点密码
	string		strUpCount;
	string		strSuccessURL;
	string		strFailureURL;

	string		CPE_UserName;										//CPE设备用户名
	string		CPE_PassWord;										//CPE设备密码
};
#endif