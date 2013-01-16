//#pragma once
#ifndef  DRAGONFLOW_MONITORTHREAD
#define DRAGONFLOW_MONITORTHREAD

#include "threadex.h"
#include "Monitor.h"
#include "ReturnValue.h"
#include <libutil/buffer.h>

#define		RETBUFCOUNT		(10*1024)
#define		INBUFCOUNT		(1024*2)


///////////////////////////////////////////////////////////////////////////////////////
#define		SEPARATOR		'#'
///////////////////////////////////////////////////////////////////////////////////////

class ThreadContrl;

using namespace svutil;

typedef BOOL(GatherData)(CStringList&, char*);
 
typedef bool (*LPFUNC)(const char *,char *,int &);


class MonitorThread :
	public ThreadEx
{
public:
	void ExecuteMonitor(Monitor *pMonitor);
	void ExecuteMonitor(void);
	virtual void run(void);
	MonitorThread();
	MonitorThread(ThreadContrl *pTC)
	{
		SetThreadName("Work thread");
//		m_hEvent=::CreateEvent(NULL,TRUE,FALSE,NULL);
		InitData();
		m_pThreadContrl=pTC;
	}
	void SetThreadContrl(ThreadContrl *pTC)
	{
		m_pThreadContrl=pTC;
	}
	virtual ~MonitorThread();
	enum{ retcount=5 };

	enum{
		Normal=0x1,
		Warning,
		Error,
	};

	void SetMonitor(Monitor *pMonitor)
	{
		m_Monitor=pMonitor;
	}

protected:
	void CloseSocket();
	BOOL InitSocket(void);
	ThreadContrl* m_pThreadContrl;
	void RunMonitor(void);
	Monitor* m_Monitor;
//	HANDLE	m_hEvent;
	Event m_Event;

private:
	CTime m_StartTime;
	void ProcessResult(void);
	void ProcessResultV70(void);
	int ParserExpression(CString strExpression, CStringList &lstOperator, CStringList &lstID);
	BOOL Judge(const char *szSource, const char *szDestination, const char *szRelation);
	void ClearResult(void);
	int m_nRunCount;
	BOOL CheckSingleItemState(/*int Type*/StateCondition *pSt ,int ItemID);
	BOOL CheckStateByType(int Type);
	void ParserMonitorState(void);
	CString m_strDisplay;
	bool BulidStringMap(STRINGMAP &map,const char *buf);
	BOOL GetProperty(const char *szProperty, const CString strSource, int &nRet);
	BOOL GetProperty(const char *szProperty, const CString strSource, float &fRet);
	BOOL GetProperty(const char *szProperty, const CString strSource, char *psret);
	BOOL GetProperty(const char *szProperty, const CString strSource, CString &strRet);
	int MakeInBuf();
	int m_MonitorState;
	BOOL PaserResult(void);
	BOOL PaserResultV70(void);
	void InitData(void);
	CReturnValueList m_RetValueList;
	char m_RetBuf[RETBUFCOUNT];
	svutil::buffer m_InBuf;
	ReturnValue m_RetValues[retcount];
	int	m_RetValueCount;
	STRINGMAP m_RVmap;

protected:
	bool RunInProcess(int iLen,string dllname,string funname);
	bool Run_Dll(LPFUNC func,int buflen);

};

#endif
