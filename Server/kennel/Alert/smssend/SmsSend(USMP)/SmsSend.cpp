#include "stdafx.h"
#include "SmsSend.h"
char * SmsSend::GetMD5Msg(unsigned char md5Digest[16])
{
	char chBuffer[256];
	char chEach[10];
	int nCount;
	memset(chBuffer,0,256);
	memset(chEach, 0, 10);
	for (nCount = 0; nCount < 16; nCount++)
	{
		sprintf(chEach, "%02x", md5Digest[nCount]);
		strncat(chBuffer, chEach, sizeof(chEach));
	}
	return strdup(chBuffer);
}

void SmsSend::ParseURL(string strURL)
{
	string strTemp1 ,strTemp2;
	if(strURL.find("http://") != -1)
	{
		strTemp1 = strURL.substr(7 , strURL.length() - 7);
	}
	else
	{
		strTemp1 = strURL;
	}
	strTemp2 = strTemp1.substr(0 , strTemp1.find("/"));
	if(strTemp2.find(":") != -1)
	{
		HostAddr = strTemp2.substr(0 , strTemp2.find(":"));
		HostPort = strTemp2.substr(strTemp2.find(":")+1 , strTemp2.length()-strTemp2.find(":")-1);
	}
	else
	{
		HostAddr = strTemp2;
		HostPort = "80";
	}
	URLRoot = strTemp1.substr(strTemp2.length() , strTemp1.length() - strTemp2.length());
}

bool SmsSend::GetMsgContent(string strMsg)
{
	try
	{
		if(strMsg.empty())
		{
			WriteLog("传入的短信内容为空！");
			return false;
		}
//		strContent = urlEncoding(strMsg);
		ConvertToHexStr(strMsg, strContent);		
		return true;
	}
	catch(...)
	{
		DWORD dwErr = GetLastError();
		LPSTR lpBuffer = NULL;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwErr,
			0,
			(LPTSTR)&lpBuffer,
			0,
			NULL
			);
		char strMsg[128] = {0};
		sprintf(strMsg , "Exception occured in GetMsgContent! error number:%d\terror msg:%s" , dwErr , lpBuffer);
		LocalFree(lpBuffer);
		WriteLog(strMsg);
		return false;
	}
}

bool SmsSend::GetDestID(list<string> DestIdList)
{
	try
	{
		if(DestIdList.empty())
		{
			WriteLog("传入的目标手机号为空！");
			return false;
		}
		list<string>::iterator pDestList = DestIdList.begin();
		while(pDestList != DestIdList.end())
		{
			ObjPhoneLis.push_back(*pDestList);
			pDestList++;
		}
		
		return true;
	}
	catch(...)
	{
		DWORD dwErr = GetLastError();
		LPSTR lpBuffer = NULL;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwErr,
			0,
			(LPTSTR)&lpBuffer,
			0,
			NULL
			);
		char strMsg[128] = {0};
		sprintf(strMsg , "Exception occured in GetMsgContent! error number:%d\terror msg:%s" , dwErr , lpBuffer);
		LocalFree(lpBuffer);
		WriteLog(strMsg);
		return false;
	}
}

bool SmsSend::InitServer()
{
	try
	{
		char strURL[128] = {0};
		string strPath = GetSiteViewRootPath();
		char strFile[128] = {0};
		sprintf(strFile , "%s\\fcgi-bin\\USMPSmsSend.ini" , strPath.c_str());
		GetPrivateProfileString("Parameter" , "ServerAddr" , "" , strURL , 128 , strFile);
		ServerAddr = strURL;
		if(ServerAddr.empty())
		{
			WriteLog("获取服务器详细地址信息为空");
			return false;
		}
		md5 alg;
		string strTemp="";
/*
		string strTemp  = ServerAddr.substr(0 , ServerAddr.find("cn:") + 10);
		WriteLog(ServerAddr.c_str());
		WriteLog(strTemp.c_str());
		HostAddr = strTemp.substr(7 , strTemp.find("cn" , 0) - 5);
		if(HostAddr.empty())
		{
			WriteLog("USMP主机地址为空");
			return false;
		}
		HostPort = strTemp.substr(strTemp.find("cn" , 0) + 3 , strTemp.find_last_of("/" , strTemp.length()) - strTemp.find_last_of(":" , strTemp.length())-1);
*/
		ParseURL(ServerAddr);

		char TempBuf[128] = {0};
		SmsContent = "USER_ID=";
		GetPrivateProfileString("Parameter" , "USER_ID" , "" , TempBuf , 128 , strFile);
		SmsContent += TempBuf;

		SmsContent += "&USER=";
		GetPrivateProfileString("Parameter" , "USER_NAME" , "" , TempBuf , 128 , strFile);
		SmsContent += TempBuf;
		strTemp += TempBuf;

		GetPrivateProfileString("Parameter" , "PASSWORD" , "" , TempBuf , 128 , strFile);
		strTemp += TempBuf;

		SmsContent += "&SRC_ID=";
		GetPrivateProfileString("Parameter" , "SRC_ID" , "" , TempBuf , 128 , strFile);
		SmsContent += TempBuf;

		SmsContent += "&SERVICE_ID=";
		GetPrivateProfileString("Parameter" , "SERVICE_ID" , "" , TempBuf , 128 , strFile);
		SmsContent += TempBuf;

		SmsContent += "&ENCODE_TYPE=GB";

		SmsContent += "&MSG_CONTENT=";
		SmsContent += strContent;

		SmsContent += "&DEST_TERMINAL_ID=13977778888";

		SmsContent += "&TS=";
		CTime CurTime = CTime::GetCurrentTime();
		sprintf(TempBuf , "%.4d%.2d%.2d%.2d%.2d%.2d" , CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay(), CurTime.GetHour(), CurTime.GetMinute(), CurTime.GetSecond());
		SmsContent += TempBuf;
		strTemp += TempBuf;

		SmsContent += "&PASSWD=";
		alg.Update((unsigned char *)strTemp.c_str(), (unsigned int)strTemp.length());
		alg.Finalize();
		SmsContent +=GetMD5Msg(alg.Digest());

		SmsContent += TempBuf;

		SmsContent += "&GW_ID=";
		GetPrivateProfileString("Parameter" , "GW_ID" , "" , TempBuf , 128 , strFile);
		SmsContent += TempBuf;


		return true;
	}
	catch(...)
	{
		DWORD dwErr = GetLastError();
		LPSTR lpBuffer = NULL;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwErr,
			0,
			(LPTSTR)&lpBuffer,
			0,
			NULL
			);
		char strMsg[128] = {0};
		sprintf(strMsg , "Exception occured in InitServer! error number:%d\terror msg:%s" , dwErr , lpBuffer);
		LocalFree(lpBuffer);
		WriteLog(strMsg);
		return false;
	}
}
bool SmsSend::InitSocket()
{
	try
	{
		WORD wVersionRequested;
		WSADATA wsaData;
		int err;
		DWORD BeginTime = ::GetTickCount();
		wVersionRequested = MAKEWORD( 1, 1 );
		err = WSAStartup( wVersionRequested, &wsaData );
		if ( err != S_OK ) 
		{
			char szReturn[128] = {0};
			sprintf(szReturn, "error=%s", "Socket initialize error!");
			WriteLog(szReturn);
			return false;
		}
		else
		{
			WriteLog("WSAStartup successful!");
		}
		if ( LOBYTE( wsaData.wVersion ) != 1 ||
			HIBYTE( wsaData.wVersion ) != 1 )
		{
				WSACleanup( );
				return false;
		}
		else
		{
			WriteLog("wsaData version successful!");
		}
//将主机名转化为IP地址。
		PHOSTENT	pHost;
		char		name[128] = {0};
		sprintf(name , "%s" , HostAddr.c_str());
		pHost = ::gethostbyname(name);
		char pAddrList[128] = {0};			
		int i=0;
		char *p;
		while(pHost->h_addr_list[i] != NULL)
		{
			p = inet_ntoa(*(in_addr *)pHost->h_addr_list[i]);
			sprintf(pAddrList, "%s" ,p);
			cout<<pAddrList<<endl;
			i++;
		}
		HostIP = pAddrList;
		char TempBuf[128] = {0};
		SmsHead = "POST ";
		SmsHead += ServerAddr;//URLRoot;
//		cout<<URLRoot.c_str()<<endl;
		SmsHead += " HTTP/1.1\r\nAccept: */*\r\nContent-Length: ";
		sprintf(TempBuf , "%d" , SmsContent.length());
		SmsHead += TempBuf;
		SmsHead += "\r\nContent-Type: application/x-www-form-urlencoded\r\nHost: ";
		SmsHead += HostIP;
		SmsHead += "\r\nPragma: no-cache\r\nCache-Control: no-cache\r\n\r\n";

		socketClient = socket(AF_INET,SOCK_STREAM,0);
		addrSrv.sin_addr.S_un.S_addr = inet_addr(HostIP.c_str());
		addrSrv.sin_family = AF_INET;
		addrSrv.sin_port = htons(atoi(HostPort.c_str()));

		int nStatus = connect(socketClient, (SOCKADDR *)&addrSrv, sizeof(SOCKADDR));
		cout<<nStatus<<endl;
		if(nStatus != 0)
		{
			char strLog[128] = {0};
			sprintf(strLog , "Socket error! Cann't connect to specified IP or Server:%s!" , HostAddr.c_str());
			WriteLog(strLog);
			return false;
		}

		return true;
	}
	catch(...)
	{
		DWORD dwErr = GetLastError();
		LPSTR lpBuffer = NULL;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwErr,
			0,
			(LPTSTR)&lpBuffer,
			0,
			NULL
			);
		char strMsg[128] = {0};
		sprintf(strMsg , "Exception occured in InitSocket! error number:%d\terror msg:%s" , dwErr , lpBuffer);
		LocalFree(lpBuffer);
		WriteLog(strMsg);
		return false;
	}
}

bool SmsSend::SendMsg()
{
	bool bRet(false);
	try
	{
		int buflen(4096);
		char buf[4096] = {0};
//		send(socketClient, SmsHead.c_str(), SmsHead.length(), 0);
//		recv(socketClient, buf, buflen, 0);//接收结果 

		if(socketClient == INVALID_SOCKET)
		{
			WriteLog("发送短信内容失败！");
			return false;
		}
		list<string>::iterator pDestList = ObjPhoneLis.begin();
		while(pDestList != ObjPhoneLis.end())
		{
			if(SmsContent.find("DEST_TERMINAL_ID") == -1)
			{
				SmsContent += "&DEST_TERMINAL_ID=";
				SmsContent += *pDestList;
			}
			else
			{
				char strTemp[20] = {0};
				sprintf(strTemp, "%s" , pDestList->c_str());
				SmsContent.replace(SmsContent.find("DEST_TERMINAL_ID")+17, 11, strTemp, 11);
			}
			string sendmsg = SmsHead + SmsContent;
//			sendmsg += "\r\n\r\n";
/*
			CChineseCode cCode;
			char strInputData[1024] = {0};
			sprintf(strInputData, "%s", sendmsg.c_str());
			string SendBuf = "";
			cCode.GB2312ToUTF_8(SendBuf, strInputData, sendmsg.length());
*/
			int nStatus = send(socketClient, sendmsg.c_str(), sendmsg.length(), 0);
			recv(socketClient, buf, buflen, 0);//接收结果 
			WriteLog(buf);
			pDestList++;

		}
		closesocket(socketClient);
		WSACleanup();
		return true;
	}
	catch(...)
	{
		DWORD dwErr = GetLastError();
		LPSTR lpBuffer = NULL;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwErr,
			0,
			(LPTSTR)&lpBuffer,
			0,
			NULL
			);
		char strMsg[128] = {0};
		sprintf(strMsg , "Exception occured in SendMsg(class)! error number:%d\terror msg:%s" , dwErr , lpBuffer);
		LocalFree(lpBuffer);
		WriteLog(strMsg);
		return false;
	}
}