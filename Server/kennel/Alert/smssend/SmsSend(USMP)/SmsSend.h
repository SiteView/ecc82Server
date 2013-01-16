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
	bool	GetMsgContent(string strMsg);			//��ȡ��������
	bool	GetDestID(list<string> DestIdList);			//��ȡҪ���͵��ֻ�����
	bool	InitServer();							//��ʼ����������Ϣ
	bool	InitSocket();							//��ʼ��Socket��Ϣ��
	bool	SendMsg();
	void	ParseURL(string strURL);
	char * GetMD5Msg(unsigned char md5Digest[16]);

private:
	string	ServerAddr;				//USMPƽ̨��������������ϸ��ַ
	string	HostAddr;				//USMPƽ̨������
	string	HostIP;					//USMPƽ̨������IP
	string	HostPort;				//USMPƽ̨�������˿ڣ��֧�ֶ˿ں�Ϊ6λʮ���Ƶ���
	string	SmsContent;				//��������
	string	SourcePhone;			//���Ͷ��ŵ��ֻ�����
	string	SmsHead;
	string	strContent;
	string	URLRoot;
	list<string> ObjPhoneLis;		//���ն��ŵ��ֻ����б�

	SOCKET socketClient;
	SOCKADDR_IN addrSrv;
};
inline BYTE toHex(const BYTE &x);
string urlEncoding( string &sIn );