//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

#ifndef _SV_ALERT_BASEOBJ_H_
#define _SV_ALERT_BASEOBJ_H_

#if _MSC_VER > 1000
#pragma once
#endif

//////////////test ////////////////////////////////////////////////////////////////////
// include stl lib && using namespace std
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
using namespace std;  
class CAlertEventObj;
class CAlertSendObj;

class CAlertBaseObj
{
	public :
		CAlertBaseObj();
		int nSendId;
	public :
		
		//��������
		string strIndex, strAlertName, strAlertType, strAlertCategory, strAlertState, strAlertTargerList, strNewAlertTargerList;

		string strIdcId;
		
		//��������		
		std::list<string> pAlertTargetList;	
		map<string, int, less<string> > pAlertTargetMap;

		//
		int nCond, nAlertType;
		
		//bin.liu
		int nGoodAlert; // ���ϻָ��澯 0 δ�澯 1 �Ѹ澯
		//��������
		string strAlwaysTimesValue, strOnlyTimesValue,strSelTimes1Value,strSelTimes2Value;
		int nAlwaysTimes,nOnlyTimes ,nSelTimes1 ,nSelTimes2;
		
		//
		//CAlertEventObj * curEventObj;

	public :
		void WriteAlertTargerById(string strId);
		void AnalysisAlertTarget();		
		
		//
		virtual bool IsMatching(CAlertEventObj * eventObj);
		virtual bool IsUpgradeMatching(CAlertEventObj * eventObj);
		virtual CAlertSendObj * MakeSendObj(CAlertEventObj * eventObj);
		virtual string GetDebugInfo();
		virtual void RefreshData();
};

//
class CAlertEmailObj: public CAlertBaseObj
{
	public :
		CAlertEmailObj();
	
	public :		
		string strEmailAdressValue, strOtherAdressValue, strEmailTemplateValue;
		string strAlertUpgradeValue, strAlertUpgradeToValue, strAlertStopValue;
		int nStopValue, nUpgradeValue;
	
	public :		
		virtual bool IsMatching(CAlertEventObj * eventObj);
		virtual bool IsUpgradeMatching(CAlertEventObj * eventObj);
		virtual CAlertSendObj * MakeSendObj(CAlertEventObj * eventObj);
		virtual string GetDebugInfo();	
		virtual void RefreshData();

	private	: 
		int nUpgradeStatCount;
		int nStopStatCount;

};

//
class CAlertSmsObj: public CAlertBaseObj
{
	public :
		CAlertSmsObj();
	
	public :		
		string strSmsNumberValue, strOtherNumberValue, strSmsSendMode, strSmsTemplateValue;	
		string strAlertUpgradeValue, strAlertUpgradeToValue, strAlertStopValue;
		int nStopValue, nUpgradeValue;

	public :
		virtual bool IsMatching(CAlertEventObj * eventObj);
		virtual bool IsUpgradeMatching(CAlertEventObj * eventObj);
		virtual CAlertSendObj * MakeSendObj(CAlertEventObj * eventObj);
		virtual string GetDebugInfo();	
		virtual void RefreshData();
	private	: 
		int nUpgradeStatCount;
		int nStopStatCount;
};

//
class CAlertScriptObj: public CAlertBaseObj
{
	public :
		CAlertScriptObj();
	
	public :		
		string strServerTextValue, strScriptServerId, strScriptFileValue, strScriptParamValue;
	public :
		virtual bool IsMatching(CAlertEventObj * eventObj);
		virtual CAlertSendObj * MakeSendObj(CAlertEventObj * eventObj);
		virtual string GetDebugInfo();	
};

//
class CAlertSoundObj: public CAlertBaseObj
{
	public :
		CAlertSoundObj();
	
	public :		
		string strServerValue, strLoginNameValue, strLoginPwdValue;

	public :
		virtual bool IsMatching(CAlertEventObj * eventObj);
		virtual CAlertSendObj * MakeSendObj(CAlertEventObj * eventObj);
		virtual string GetDebugInfo();	
};

//
class CAlertPythonObj: public CAlertBaseObj
{
	public :
		CAlertPythonObj();
	
	public :		
		string strReceive, strLevel, strContent;

	public :
		virtual bool IsMatching(CAlertEventObj * eventObj);
		virtual CAlertSendObj * MakeSendObj(CAlertEventObj * eventObj);
		virtual string GetDebugInfo();	
};

//�����ҳ������ʽ
class CAlertWebObj: public CAlertBaseObj
{
public :
	CAlertWebObj();

public :		
	string strEmailAdressValue, strOtherAdressValue, strEmailTemplateValue;
	string strAlertUpgradeValue, strAlertUpgradeToValue, strAlertStopValue;
	int nStopValue, nUpgradeValue;

public :		
	virtual bool IsMatching(CAlertEventObj * eventObj);
	virtual bool IsUpgradeMatching(CAlertEventObj * eventObj);
	virtual CAlertSendObj * MakeSendObj(CAlertEventObj * eventObj);
	virtual string GetDebugInfo();	
	virtual void RefreshData();

private	: 
	int nUpgradeStatCount;
	int nStopStatCount;

};
/*
//���Itsm������������
class CAlertItsmObj: public CAlertBaseObj
{
public:
	CAlertItsmObj();
public :
	virtual bool IsMatching(CAlertEventObj * eventObj);
	virtual CAlertSendObj * MakeSendObj();
	virtual string GetDebugInfo();	
};
*/

#endif