//#pragma once
#ifndef		DRAGONFLOW_UTIL
#define		DRAGONFLOW_UTIL

#include	<cc++/string.h>
#include	<cc++/thread.h>
#include	<cc++/socket.h>
#include	"mydef.h"
#include	<svdbapi.h>

#ifdef	CCXX_NAMESPACES
using namespace ost;
#endif


class Util
{
public:

#ifdef	WIN32
	static void ClearSocket(void);
	static BOOL InitSocket(void);
#endif

#ifndef WIN32
	static BOOL InsertSVMQ(CString strQueueName,CString strMessage,CString );
#endif
	static bool InsertSvdb(string tablename,const char *pdata,int datalen);
	static bool InsertSvdb(string tablename,const char *pdata,int datalen,string addr);
	//BOOL GetStringByID(UINT strid,char *szbuf,int len);
	//CString GetStringByID(UINT strid);
	static void ErrorLog(CString strError);
	static void EventLog(CString strError);
	BOOL Init(void);
        static void FreeStringList(CStringList &lst);
	Util(void);
	~Util(void);
	static CString GetRootPath(void);
	static CString GetSVMQAddress();
	static CString g_strSession;;

	static int AppendThenClearAllRecords (std::list<SingelRecord> & inrcd);

private:
//	HMODULE m_hResLibrary;

    void method_2();

	static std::list<SingelRecord> listrcd;
	static bool CacheRecords(string tablename,const char *pdata,int datalen);


};

#endif
