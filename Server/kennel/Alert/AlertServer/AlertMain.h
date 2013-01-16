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

#define IDC_Version

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
class SendPythonAlertThread;
class SendItsmAlertThread;
class DirectProcessThread;

typedef struct _WATCH_LIST
{
	string strItemindex;
	
	int nDayOfWeek;
	int nDayOfMonth;
	
	int nYear;
	int nMonth;	
	int nDay;

	//��ӽ���ʱ��(sxf 2008.12.1)
	int nEndDayOfWeek;
	int nEndDayOfMonth;
	int nEndYear;
	int nEndMonth;
	int nEndDay;
	//
	
	int nItemStartHour;
	int nItemStartMin;	
	int nItemEndHour;	
	int nItemEndMin;

	string strPhoneNumber;
	string strEmailAddress;
	string strType;
}WATCH_LIST, *LPWATCH_LIST;

//class CSerialPort;
#import "Base/SmSend.dll" no_namespace
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
		void DoSendPythonAlert();
		void DoSendWebAlert();
		void IniChangeProc();
		void SmsPortChangeProc();

		void DoSendItsmAlert();

		void DoDirectProcess(Thread *pThread);
		void DoCheckProcess();

		int AutoCreateIssue(CAlertEventObj *pEvent);

		//����һ��������������
		void CreateItsmTask(CAlertSendObj *pSendObj,int iType);
		CAlertSendObj *CreateItsmTaskFromEvent(CAlertEventObj *pEvent);
		
		ReceiveThread * receivethread;
		ProcessThread * processthread;
		//SendThread * sendthread;

		SendEmailAlertThread * sendemailthread;
		SendSmsAlertThread * sendsmsthread;
		SendScriptAlertThread * sendscriptthread;
		SendSoundAlertThread * sendsoundthread;
		SendPythonAlertThread * sendpythonthread;
		//�Զ����������߳�
		SendItsmAlertThread *senditsmthread;
		//
		Mutex * waitlistmutex;
		//Mutex * waitlistmutexreceive;
		Mutex *waitItsmMutex;

		Mutex *queueMutex;
		Mutex *AlertObjListMutex;
		Mutex *iniMutex;
		Mutex *poolMutex;

		DWORD mainThreadId;
		DWORD busyCount;

		std::list<Thread *> workthreads;

		std::list<Thread *> threadspool;

		CAlertEventObj * event;

		DWORD	m_dwWorktime;

		int				m_nBaseThreadCount;
	private :		
		//ֻ����һ��ʱ�����������ID�����Ӧ�ļ�����ֵ
		std::map<string , string> m_AlertCount;
		std::map<string , string>::iterator p_AlertCount;
		std::string		strTempSection;					//����Alert.ini�е�Section���Ա������¼�����
		//���������б�
		list <CAlertBaseObj *> m_AlertObjList;
		list <CAlertBaseObj *>::iterator m_AlertObjListItem;
		
		//�ĸ��������Ͷ����б�
		//list <CAlertSendObj *> m_WaitSendList;
		list <CAlertSendObj *>::iterator  m_WaitSendListItem;
		list <CAlertSendObj *> m_WaitSendEmailList;
		list <CAlertSendObj *> m_WaitSendSmsList;
		list <CAlertSendObj *> m_WaitSendScriptList;
		list <CAlertSendObj *> m_WaitSendSoundList;
		list <CAlertSendObj *> m_WaitSendPythonList;
		list <CAlertSendObj *> m_WaitSendWebList;

		list <CAlertSendObj *> m_WaitSendItsmList;

		//�����¼������б�
		list <CAlertEventObj *>::iterator m_AlertEventListItem;
		list <CAlertEventObj *> m_AlertEventList;


	private :
		int GetIntFromAlertType(string strType);
		void InitAlertObjList();
		void ReadAlertObjFromIni(string strSection);
		void DeleteAlertObjFromSection(string strSection);		

		void ReadAlertObjFromIni(string strSection, string strIdcId);
		void DeleteAlertObjFromSection(string strSection, string strIdcId);		

		void RefreshAlertObjList();
	
	public : 
		//static SendEmailAlert * pSendEmail;
		static HINSTANCE hEmailDll;
		static HINSTANCE hMonitorDll;
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
		static string  GetDeviceProp(string strId, string strProp);
		static string GetAllGroupTitle(string strId);
		
		static string TruncateToUId(string id);
		static bool IsIdcGroup(const char * pQueryString);
		//ȡ�ü��������ֵ sxf 2008-12-4
		static string GetMonitorParameterValue(string strId, string strPropName);
		//��ȡ�ļ������һ��
		static string GetFileLastLine(string strFilePath,string strLineTag);
		static string GetGroupTitle(string strID);
		//��ȡ����¼��dstrֵ
		static string GetMonitorRecordDstr(string strId);
		static string GetMonitorRecordState(string strId);

		//static CSerialPort m_smsPort;
		static bool bInitSerialPort;
		static bool InitSerialPort();
		static void CloseSerialPort();
		static int SendSmsFromComm( CString strSmsTo, CString strContent, int nSMSMaxLength );
		static int TestSmsFromComm(CString strSmsTo, CString strContent);

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

		static string strENDisable;
		static string strENNormal;
		static string strENWarning;
		static string strENError;
		static string strENOther;

		static string strMontorFreq;
		static string strHour;
		static string strMinute;
		static string strMonitorFazhi;

		static string strWeiHuUserIdcId;
		static string strSysWeihuUserIdcId;
		static string strYeWuIdcId;
		static string strXiaoshouIdcId;
		static int    nSMSSendLength;//���ŷ��͵ĳ���
		static string strSMSSendURL;//���ŷ��ͷ������ĵ�ַ
		static string strSMSSendPort;//���ŷ��ͷ������Ķ˿�

		//ֵ�������б�
		static list<WATCH_LIST> m_pListWatch;
		//ֵ�������б�ı���
		static list<WATCH_LIST>::iterator m_pListWatchItem;
		//
		static string GetCfgFromWatchList(string strAlertindex, bool bEmail);
		//
		static void ReadCfgFromWatchIni();
		static void ReadCfgFromWatchIniSection(string strSection);
		static void DeleteCfgFromWatchIniSection(string strSection);
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

class SendPythonAlertThread : public Thread
{
public:
	SendPythonAlertThread();
	~SendPythonAlertThread();
	void run();

	CAlertMain* parentObj;
	SendPythonAlertThread(CAlertMain* obj);
};

class SendItsmAlertThread : public Thread
{
public:
	SendItsmAlertThread();
	~SendItsmAlertThread();
	void run();

	CAlertMain* parentObj;
	SendItsmAlertThread(CAlertMain* obj);

};

class DirectProcessThread : public Thread
{
public:
	void run();

	CAlertMain* parentObj;
	DirectProcessThread(CAlertMain* obj);
};

class ProcessCheckThread : public Thread
{
public:
	void run();

	CAlertMain* parentObj;
	ProcessCheckThread(CAlertMain* obj);
};

#endif

BOOL GetSourceHtml(char const * theUrl, char * retState) ;
string urlEncoding( string &sIn );
