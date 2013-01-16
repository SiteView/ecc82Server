#include "stdafx.h"
#include <string>
#include <iostream>
#include <list>
#include <atltime.h>
using namespace std;

#include <winsock.h>
#pragma comment(lib , "ws2_32.lib")

#include <svapi.h>
#include <svdbapi.h>

#include "md5.h"
#include "CChineseCode.h"

extern void WriteLog(const char *strMsg);
void ConvertToHexStr(string strSour , string &strDes);

class SmsSend
{
public:
	bool	GetMsgContent(string strMsg);			//获取短信内容
	bool	GetDestID(list<string> DestIdList);			//获取要发送的手机号码
	bool	InitServer();							//初始化服务器信息
	bool	InitSocket();							//初始化Socket信息、
	bool	SendMsg();
	void	ParseURL(string strURL);
	char * GetMD5Msg(unsigned char md5Digest[16]);

private:
	string	ServerAddr;				//USMP平台服务器服务器详细地址
	string	HostAddr;				//USMP平台主机名
	string	HostIP;					//USMP平台服务器IP
	string	HostPort;				//USMP平台服务器端口，最长支持端口号为6位十进制的数
	string	SmsContent;				//短信内容
	string	SourcePhone;			//发送短信的手机号码
	string	SmsHead;
	string	strContent;
	string	URLRoot;
	list<string> ObjPhoneLis;		//接收短信的手机号列表

	SOCKET socketClient;
	SOCKADDR_IN addrSrv;
};
inline BYTE toHex(const BYTE &x);
string urlEncoding( string &sIn );