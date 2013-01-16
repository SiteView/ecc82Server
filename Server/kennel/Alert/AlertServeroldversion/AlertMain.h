//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

#ifndef _SV_ALERT_MAIN_H_
#define _SV_ALERT_MAIN_H_

#if _MSC_VER > 1000
#pragma once
#endif

//////////////////////////////////////////////////////////////////////////////////
// include stl lib && using namespace std
#include <iostream>
#include <string>
#include <list>
#include <map>
using namespace std;

#include <cc++/thread.h>
#include <cc++/socket.h>
#include <cstdio>
#ifdef	CCXX_NAMESPACES
using namespace ost;
#endif

#include "SendApiDefine.h"
#include "Dragonflow.h"
//#include "../../kennel/svdb/svapi/jwsmtp/jwsmtp.h"

class CAlertBaseObj;
class CAlertSendObj;
class CAlertEventObj;
class ReceiveThread;
class ProcessThread;
//class SendThread;
class SendEmailAlertThread;
class SendSmsAlertThread;
class SendScriptAlertThread;
class SendSoundAlertThread;
//class CSerialPort;
#import "..\base\SmSend.dll" no_namespace
class CAlertMain
{
	public :
		CAlertMain();
		~CAlertMain();

	public :
		bool Init();
		//void Run();
		void DoReceive();
		void DoProcess();
		//void DoSend();
		void DoSendEmailAlert();
		void DoSendSmsAlert();
		void DoSendScriptAlert();
		void DoSendSoundAlert();

		void IniChangeProc();
		void SmsPortChangeProc();		
		
		ReceiveThread * receivethread;
		ProcessThread * processthread;
		//SendThread * sendthread;

		SendEmailAlertThread * sendemailthread;
		SendSmsAlertThread * sendsmsthread;
		SendScriptAlertThread * sendscriptthread;
		SendSoundAlertThread * sendsoundthread;
		Mutex * waitlistmutex;
		//Mutex * waitlistmutexreceive;
		CAlertEventObj * event;
	private :		
		
		//报警对象列表
		list <CAlertBaseObj *> m_AlertObjList;
		list <CAlertBaseObj *>::iterator m_AlertObjListItem;
		
		//四个报警发送对象列表
		//list <CAlertSendObj *> m_WaitSendList;
		list <CAlertSendObj *>::iterator  m_WaitSendListItem;
		list <CAlertSendObj *> m_WaitSendEmailList;
		list <CAlertSendObj *> m_WaitSendSmsList;
		list <CAlertSendObj *> m_WaitSendScriptList;
		list <CAlertSendObj *> m_WaitSendSoundList;

		//报警事件对象列表
		list <CAlertEventObj *>::iterator m_AlertEventListItem;
		list <CAlertEventObj *> m_AlertEventList;

		map<string, int> m_LastEventType; //最后状态队列，苏合 2008-02-13

	private :
		int GetIntFromAlertType(string strType);
		void InitAlertObjList();
		void ReadAlertObjFromIni(string strSection);
		void DeleteAlertObjFromSection(string strSection);		
		void RefreshAlertObjList();
	
	public : 
		//static SendEmailAlert * pSendEmail;
		static HINSTANCE hDll;
		static SendEmail * pSendEmail;
		static EXECUTESCRIPT * pExcuteScript;
	
		static bool InitSendApi();
		static void UnloadSendApi();
		static bool ParserToken(list<string >&pTokenList, const char * pQueryString, char *pSVSeps);
		static bool ParserToLength(list<string >&pTokenList, string  strQueryString, int nLength);
		static string  ReplaceStdString(string strIn, string strFrom, string strTo);
		static bool IsScheduleMatch(string strSchedule);
		static string  GetMonitorPropValue(string strId, string strPropName);
		static string  GetMonitorTitle(string strId);
		static string  GetDeviceTitle(string strId);
		static string GetAllGroupTitle(string strId);

		//static CSerialPort m_smsPort;
		static bool bInitSerialPort;
		static bool InitSerialPort();
		static void CloseSerialPort();
		static int SendSmsFromComm(CString strSmsTo, CString strContent);

		static _Alert * mySoundRef;
		static bool InitSoundAlertCom();
		static void UnloadSoundAlertCom();

		static _Alert * myScriptRef;
		static bool InitScriptAlertCom();
		static void UnloadScriptAlertCom();

		static IUMSmSendPtr pSender;
		static bool InitWebSmsAlertCom();
		static void UnloadWebSmsAlertCom();

		static bool SendMail(string strServer, string strFrom, string strTo, string strSubject, string strContent, string strUser, string strPwd);

		static string strDisable;
		static string strNormal;
		static string strWarning;
		static string strError;
		static string strOther;

		static string strMontorFreq;
		static string strHour;
		static string strMinute;
		static string strMonitorFazhi;

};

class ReceiveThread : public Thread
{
public:
	ReceiveThread();
	~ReceiveThread();
	void run();

	CAlertMain* parentObj;
	ReceiveThread(CAlertMain* obj);	
};

class ProcessThread : public Thread
{
public:
	ProcessThread();
	~ProcessThread();
	void run();

	CAlertMain* parentObj;
	ProcessThread(CAlertMain* obj);	
};

//class SendThread : public Thread
//{
//public:
//	SendThread();
//	~SendThread();
//	void run();
//
//	CAlertMain* parentObj;
//	SendThread(CAlertMain* obj);
//};

class SendEmailAlertThread : public Thread
{
public:
	SendEmailAlertThread();
	~SendEmailAlertThread();
	void run();

	CAlertMain* parentObj;
	SendEmailAlertThread(CAlertMain* obj);
};

class SendSmsAlertThread : public Thread
{
public:
	SendSmsAlertThread();
	~SendSmsAlertThread();
	void run();

	CAlertMain* parentObj;
	SendSmsAlertThread(CAlertMain* obj);
};

class SendScriptAlertThread : public Thread
{
public:
	SendScriptAlertThread();
	~SendScriptAlertThread();
	void run();

	CAlertMain* parentObj;
	SendScriptAlertThread(CAlertMain* obj);
};

class SendSoundAlertThread : public Thread
{
public:
	SendSoundAlertThread();
	~SendSoundAlertThread();
	void run();

	CAlertMain* parentObj;
	SendSoundAlertThread(CAlertMain* obj);
};

#endif