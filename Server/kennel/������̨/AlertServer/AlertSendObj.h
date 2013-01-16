//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

#ifndef _SV_ALERT_SENDOBJ_H_
#define _SV_ALERT_SENDOBJ_H_

#if _MSC_VER > 1000
#pragma once
#endif

//////////////////////////////////////////////////////////////////////////////////
// include stl lib && using namespace std
#include <iostream>
#include <string>
#include <list>
#include <vector>

using namespace std;

#include "SendApiDefine.h"

#include "svdb/libutil/time.h"

//
class CAlertSendObj
{
	public :
		CAlertSendObj();
		virtual ~CAlertSendObj();
	public :
		
		int nType;
		int nSendId;
		bool bUpgrade;
		int nEventCount;					//监测器出现报警的次数
		int nEventType;
		string strAlertMonitorId;			//有报警的监测器ID
		string strAlertName;				//报警名称
		string strAlertIndex;
		string strEventDes;
		string strTime;

		//添加消息过期时间 2008-12-2 sxf
		svutil::TTime m_dtExpireTime;

		string strIdcId;

	public :
		void InsertRecord(string strAlertReceive, int nType, int nStatu);
		void InsertRecord(string strAlertReceive, string strIdcId, int nType, int nStatu);

		virtual bool SendAlert();
		virtual string GetDebugInfo();
		virtual void SetUpgradeTrue();
};


//
struct SendInfo
{
	string str_mail_to;
	string str_alert_title;
	string str_alert_content;

};

class CAlertEmailSendObj : public CAlertSendObj
{
	public :
		CAlertEmailSendObj();
		~CAlertEmailSendObj();
		string strEmailAdressValue, strOtherAdressValue, strEmailTemplateValue;
		string strAlertUpgradeToValue;

	public :
		virtual bool SendAlert();
		virtual string GetDebugInfo();
		virtual void SetUpgradeTrue();

	private :		
		bool GetInfoFromEmailAddress(string strAddressName);
		void MakeAlertTitle();
		bool ToChinese(string&,const string);
		void MakeAlertContent();
		
		string szSchedule, szMailTo;
public:
		string strAlertTitle;
		string strAlertContent;

		list<SendInfo> send_list;
};

struct SMSInfo
{
	string str_sms_to;
	string str_alert_content;
};

#import "./Base/SmSend.dll" no_namespace

typedef int (*RUNFUNC)(char *, char *, char *);

class CAlertSmsSendObj : public CAlertSendObj
{
	public :
		CAlertSmsSendObj();
		~CAlertSmsSendObj();
		string strSmsNumberValue, strOtherNumberValue, strSmsSendMode, strSmsTemplateValue;
		string strAlertUpgradeToValue;

	public :
		virtual bool SendAlert();		
		virtual string GetDebugInfo();
		virtual void SetUpgradeTrue();
		
		bool SendSmsFromCom();
		
		bool SendSmsFromWeb();		

		bool SendSmsFromSelfDefine();

		bool SendSmsFromDatabase(string& err=string());

		//IUMSmSendPtr pSender;
		int GetResponseCode(_bstr_t bstrResponse);
		BSTR UTF2GB(LPCSTR lp, int nLen);
		void String2Array(std::vector<std::string> &vOut,string strIn,string strDiv);
		int process_msg(std::vector<std::string> &vOut,string strIn,string strFmt,int msglen);
		
		int Run_DLL(RUNFUNC p,char* param,char *to,char* sms);

	private :		
		bool GetInfoFromSmsAddress(string strAddressName);
		void MakeAlertTitle();
		void MakeWebAlertContent();
		void MakeComAlertContent();
		
		
public:
		string strAlertTitle;
		string strAlertContent;

		list<SMSInfo> send_list;
		string szSchedule, szSmsTo;

};

//
class CAlertScriptSendObj : public CAlertSendObj
{
	public :
		CAlertScriptSendObj();
		~CAlertScriptSendObj();
		string strServerTextValue, strScriptServerId, strScriptFileValue, strScriptParamValue;
		string strUserName, strUserPwd;

	public :
		
		virtual bool SendAlert();
		virtual string GetDebugInfo();

		void GetUserInfoFromServer();
		CString DoWinScript();
		bool DoUnixScript();
};


//
class CAlertSoundSendObj : public CAlertSendObj
{
	public :
		CAlertSoundSendObj();
		~CAlertSoundSendObj();	
		string strServerValue, strLoginNameValue, strLoginPwdValue;

	public :		
		virtual bool SendAlert();
		virtual string GetDebugInfo();

		CString DoMsgBeep();
		void DoPythonTaskTest();
};

//
class CAlertPythonSendObj : public CAlertSendObj
{
	public :
		CAlertPythonSendObj();
		~CAlertPythonSendObj();	
		string strReceive, strLevel, strContent;

	public :		
		virtual bool SendAlert();
		virtual string GetDebugInfo();

		void DoPythonTask();
		void InvokeWithParm();
};

class CInternetSession;

class CAlertItsmSendObj : public CAlertSendObj
{
public :
	class Conditions
	{
	public:
		string id;
		string desc;
		bool isuse;
		
	};
	CAlertItsmSendObj();
	~CAlertItsmSendObj();
	string strEmailAdressValue, strOtherAdressValue, strEmailTemplateValue;
	string strAlertUpgradeToValue;
	string strURL,strUserName,strPWD;

public :
	virtual bool SendAlert();
	virtual string GetDebugInfo();
	virtual void SetUpgradeTrue();

	void String2Array(std::vector<std::string> &vOut,string strIn,string strDiv);

private :		
	bool GetItsmConfig();
	void MakeAlertTitle();
	bool ToChinese(string&,const string);
	void MakeAlertContent();

	string szSchedule, szMailTo;
public:
	string strAlertTitle;
	string strAlertContent;
	bool   m_bAutoCreateIssue;
	bool requestJSONAction(CInternetSession *sess,string& url,string& req,string& reply);
	bool MatchCondition(string strMonitorId, string strCondition);

	typedef std::map<std::string,DWORD> MonitorCacheType;

	static MonitorCacheType m_mapMonitorCache;
	static MonitorCacheType m_mapFilterCache;
};



// 利用webService发送报警信息
class CAlertWebSendObj : public CAlertSendObj
{
	public :
		CAlertWebSendObj();
		~CAlertWebSendObj();

	public :
		virtual bool SendAlert();

	private :
		void MakeAlertContent();
		
public:
		string strAlertContent;
};

#endif