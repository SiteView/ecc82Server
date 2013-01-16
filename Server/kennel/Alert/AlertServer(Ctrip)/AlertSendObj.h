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

using namespace std;

#include "SendApiDefine.h"
#include "svdb/libutil/time.h"
//
class CAlertSendObj
{
	public :
		CAlertSendObj();
		~CAlertSendObj();
	public :
		
		int nType;
		int nSendId;
		bool bUpgrade;
		int nEventCount;
		int nEventType;
		string strAlertMonitorId;
		string strAlertName;
		string strAlertIndex;
		string strEventDes;
		string strTime;

		string strIdcId;

	public :
		void InsertRecord(string strAlertReceive, int nType, int nStatu);
		void InsertRecord(string strAlertReceive, string strIdcId, int nType, int nStatu);

		virtual bool SendAlert();
		virtual string GetDebugInfo();
		virtual void SetUpgradeTrue();
};


//
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
		string strAlertTitle;
		string strAlertContent;
};

//#import "../base/SmSend.dll" no_namespace
#import "Base/SmSend.dll" no_namespace
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
		//IUMSmSendPtr pSender;
		int GetResponseCode(_bstr_t bstrResponse);
		BSTR UTF2GB(LPCSTR lp, int nLen);

	private :		
		bool GetInfoFromSmsAddress(string strAddressName);
		void MakeAlertTitle();
		void MakeAlertContent();
		
		string szSchedule, szSmsTo;
		string strAlertTitle;
		string strAlertContent;
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


#endif