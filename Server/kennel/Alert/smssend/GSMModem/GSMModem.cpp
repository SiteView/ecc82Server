// GSMModem.cpp : ���� DLL Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <list>
using namespace std;

#include "SHShenjiGSMModemSoap.nsmap"
#include "soapSHShenjiGSMModemSoapProxy.h"

#include <sys/stat.h>
#include <sys/types.h>

#include <svapi.h>
#include <svdbapi.h>
#include "CChineseCode.h"

_ns1__SendNoteInfo SendNoteInfo;
_ns1__SendNoteInfoResponse SendNoteInfoRes;
string	ServerAddr;

list<string> phoList;
string strSmsContent;
string strSmsVFlag;
string strSmsDestineDateTime;


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

//��ӡ��־����
void WriteLog(const char* str )
{
	//	return;
	char timebuf[128],datebuf[128];

	_tzset();
	_strtime( timebuf );
	_strdate( datebuf );
	char * p = datebuf;
	while(*p)
	{
		if(*p == '/')
			*p = '-';
		p++;
	}

	char szProgramName[128] = {0};
	sprintf(szProgramName , "%s.log" , "GSMModem");
	string	strEccPath = GetSiteViewRootPath();
	char szLogFile[128];
	sprintf( szLogFile, "%s\\temp\\%s", 
		strEccPath.c_str(), szProgramName );

	// �ж��ļ���С���ڲ����ļ��������ʵ��
	struct _stat buf;
	if( _stat( szLogFile, &buf ) == 0 )
	{
		if( buf.st_size > 10000*1024 )
		{
			FILE *log = fopen( szLogFile, "w" );
			if( log != NULL )
				fclose( log );
		}
	}

	FILE *log = fopen( szLogFile,"a+");
	if( log != NULL )
	{
		fprintf( log, "%s-%s \t%s\n",datebuf, timebuf, str );
		fclose( log );
	}
}

//����ECC��������Ŀ���ֻ����룬�����ں���������
void PaserPhoneList(list<string> & phoList, char * strPhones)
{
    if(strPhones == NULL)
		return;
	
	char buf[1024] = {0};
	sprintf(buf, "%s", strPhones);

	char * strTok =",";
	char * p = strtok(buf, strTok);
	while(p != NULL)
	{
		phoList.push_back(p);
		p = strtok(NULL, strTok);
	}
}

//��ʼ��������Ϣ
bool InitInfo(string strContent, char * strPhone)
{
	WriteLog(">>>>>>>>InitInfo Start!<<<<<<<<");
	try
	{
		if(strContent.empty())
		{
			WriteLog("��������Ϊ��");
			return false;
		}

		if(strlen(strPhone) == 0)
		{
			WriteLog("�ֻ�����Ϊ�գ�");
			return false;
		}

		PaserPhoneList(phoList, strPhone);
		if(phoList.empty())
		{
			WriteLog("�ֻ���������Ϊ�գ�");
			return false;
		}

		char strUrl[128] = {0};
		char strPort[10] = {0};
		char strFlag[128] = {0};
		char strDataTime[128] = {0};

		char strFile[128] = {0};
		sprintf(strFile,"%s\\fcgi-bin\\GSMModem.ini", GetSiteViewRootPath().c_str());
		GetPrivateProfileString("GSMModem", "UrlAddr", "", strUrl, 128, strFile);
//		GetPrivateProfileString("GSMModem", "UrlPort", "", strPort, 128, strFile);
		GetPrivateProfileString("GSMModem", "vFlag", "", strFlag, 128, strFile);
		GetPrivateProfileString("GSMModem", "destineDateTime", "", strDataTime, 128, strFile);
		ServerAddr = strUrl;

//		ServerPort = atoi(strPort);
		strSmsContent.clear();
		strSmsContent += strContent;

		strSmsVFlag.clear();
		strSmsVFlag += strFlag;

		strSmsDestineDateTime.clear();
		strSmsDestineDateTime = strDataTime;

		SendNoteInfo.notinfo = &strSmsContent;
		SendNoteInfo.vFlag = &strSmsVFlag;
		SendNoteInfo.destineDateTime = &strSmsDestineDateTime;
		WriteLog("�������ļ������ȡ���Ĳ�����");
		char buf[1024] = {0};
		sprintf(buf, "\nUrlAddr=%s\nvFlag=%s\ndestinDateTime=%s",ServerAddr.c_str(), SendNoteInfo.vFlag->c_str(), SendNoteInfo.destineDateTime->c_str());
		WriteLog(buf);
//		SendNoteInfo.phoneNo = 
		return true;
	}
	catch(...)
	{
		WriteLog("--------InitInfo Exception--------");
		DWORD dwError = ::GetLastError();
		LPSTR lpBuffer = NULL;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwError,
			0,
			(LPTSTR)&lpBuffer,
			0,
			NULL
			);
		char buf[128] = {0};
		sprintf(buf, "Exception occured, error code:%d\terror msg:%s" , dwError, lpBuffer);		
			LocalFree(lpBuffer);
		WriteLog(buf);
		WriteLog("--------InitInfo End--------");
		return false;
	}
}

bool SendMsg()
{
	bool bRet(false);
	try
	{
//		SHShenjiGSMModemSoap oGsm(ServerAddr.c_str() , ServerPort);
		SHShenjiGSMModemSoap oGsm(ServerAddr.c_str());
		for(list<string>::iterator pPhoList = phoList.begin(); pPhoList != phoList.end(); pPhoList++)
		{
			string strTemp = *pPhoList;
			SendNoteInfo.phoneNo = &strTemp;
			int nRet = oGsm.__ns2__SendNoteInfo(&SendNoteInfo, &SendNoteInfoRes);

			if(nRet == SOAP_OK)
			{
				bRet = SendNoteInfoRes.SendNoteInfoResult;
				if(bRet)
				{
					bRet= true;
				}
				else
				{
					WriteLog("Send msg failed!:��������");
					string strTemp = "URLAddress=";
					strTemp += ServerAddr;
					strTemp += "\nnotinfo=";
					strTemp += (SendNoteInfo.notinfo)->c_str();
					strTemp += "\nphoneNo=";
					strTemp += (SendNoteInfo.phoneNo)->c_str();
					WriteLog(strTemp.c_str());
					bRet = false;
				}
			}
			else
			{
				
				WriteLog("Send msg failed!:soap ����");
				char buf[128] = {0};
				sprintf(buf, "Return code:%d!" , nRet);
				WriteLog(buf);
				WriteLog("���������");
				WriteLog(SendNoteInfo.destineDateTime->c_str());
				WriteLog(SendNoteInfo.notinfo->c_str());
				WriteLog(SendNoteInfo.phoneNo->c_str());
				WriteLog(SendNoteInfo.vFlag->c_str());
				bRet = false;
			}		

		}
	}
	catch(...)
	{
		WriteLog("--------SendMsg Exception--------");
		bRet = false;
		DWORD dwError = ::GetLastError();
		LPSTR lpBuffer = NULL;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwError,
			0,
			(LPTSTR)&lpBuffer,
			0,
			NULL
			);
		char buf[128] = {0};
		sprintf(buf, "Exception occured, error code:%d\terror msg:%s" , dwError, lpBuffer);	
			LocalFree(lpBuffer);
		WriteLog(buf);
		WriteLog("--------SendMsg End--------");
	}
	return bRet;
}

extern "C" __declspec(dllexport)
int getinfo(string &retstr)
{
	retstr = "�Ϻ���ƾֶ���ƽ̨";
	return 1;
}

extern "C" __declspec(dllexport)
int run(char *strParas, char *strPhone, char *strContent)
{
	string strMsgCont;
	CChineseCode::GB2312ToUTF_8(strMsgCont, strContent, strlen(strContent));
	try
	{
		if(InitInfo(strMsgCont, strPhone))
		{
            if(SendMsg())
			{
				return 1;

			}
			else
			{
				return 0;

			}
		}
		else
		{
			WriteLog("��ʼ����Ϣʧ�ܣ�");
			return 0;
		}

	}
	catch(...)
	{
		DWORD dwError = ::GetLastError();
		LPSTR lpBuffer = NULL;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwError,
			0,
			(LPTSTR)&lpBuffer,
			0,
			NULL
			);
		char buf[128] = {0};
		sprintf(buf, "Exception occured, error code:%d\terror msg:%s" , dwError, lpBuffer);		
		LocalFree(lpBuffer);
		WriteLog(buf);
		return 0;
	}
}