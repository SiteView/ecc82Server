#include "Util.h"
#include "MSException.h"

#include <cc++/file.h>
#include "stlini.h"
#include <svdbapi.h>
#include "MonitorSchedule.h"

#ifndef WIN32
#include "mqpacksoso.h"
#endif

extern CString g_strRootPath;
extern string g_ServerHost;

static ost::Mutex g_ErrorLoglock;
static ost::Mutex g_InsertMQLock;
static CString g_SVMQAddress="127.0.0.1";

CString Util::g_strSession="Refresh";

std::list<SingelRecord> Util::listrcd;


Util::Util()
{
//	strRootPath[0]='\0';
//	m_hResLibrary=NULL;
       g_SVMQAddress=GetSVMQAddress();
       
}

Util::~Util()
{
/*	if(m_hResLibrary!=NULL)
		::FreeLibrary(m_hResLibrary);
*/
}

CString Util::GetSVMQAddress()
{
         char buf[1024]={0};
	
	// strcpy(buf,"siteview.ini");
	 sprintf(buf,"%s/MonitorManager/siteview.ini",GetRootPath().getText());
        INIFile inf=LoadIni(buf);
	std::string name=GetIniSetting(inf,"InstallPath","SVMQAddress","");
	if(name.empty())
	   return "127.0.0.1";
	else
	   return name.c_str();
  
}

CString Util::GetRootPath()
{
//	return ::FuncGetInstallRootPath();
      //  return "/home/siteview/chenxing/monitor";

	string path=::GetSiteViewRootPath();
	return path.c_str();
	
         char buf[100]={0};
	
	 strcpy(buf,"siteview.ini");
        INIFile inf=LoadIni(buf);
	std::string name=GetIniSetting(inf,"InstallPath","RootPath","");
	if(name.empty())
		return "D:\\v70";
	else
	   return name.c_str();
	
	

	return "/usr/local";

	return "/usr/local";

}

BOOL Util::Init()
{

//	CString strPath=_T("");

/*	strPath=GetRootPath();
	if(strPath.IsEmpty())
		throw MSException("RootPath is empty");

	strcpy(strRootPath,(LPCSTR)strPath);*/

/*	strPath.Format("%s\\MonitorManager\\ResLibrary.dll",g_strRootPath);

	m_hResLibrary=::LoadLibrary(strPath);
	if(m_hResLibrary==NULL)
		throw MSException("Load ResLibrary.dll failed");*/
	
	

	return TRUE;

}

void Util::EventLog(CString strError)
{
	CString strPath;
	strPath=GetRootPath();
	strPath+="/temp/msevent.log";


	ThreadFile tf(strPath);

	strError+="\r\n";

	CTime stime=CTime::GetCurrentTimeEx();

	CString strlog;
	strlog.Format("%s\t%s",stime.Format().c_str(),strError.getText());

	char *pstr=strlog.getText();

	tf.append(pstr,(ccxx_size_t)strlen(pstr));
	

	

/*
	CString strTemp=_T(""),
		strFile=_T("");
	strFile.Format("%s\\MonitorManager\\meevent.log",g_strRootPath);

	CTime time=CTime::GetCurrentTime();
	strTemp=time.Format("%Y-%m-%d %H:%M:%S");

	strTemp+="\t"+strError+"\r\n";
#ifndef DEBUG_T	
	FILE *fp=::fopen(strFile,"a+");
	if(fp==NULL)
	{
		printf("���ʧ�\n");
		return ;
	}
	::fputs(strTemp,fp);
	::fclose(fp);
#else
	printf("%s",strTemp);
#endif
	*/
	return ;

}

#include <sys/types.h>
#include <sys/stat.h>
void Util::ErrorLog(CString strError)
{
	ost::MutexLock lock(g_ErrorLoglock);
	puts(strError);
//	return;

	CString strPath;

	strPath=GetRootPath();
	strPath+="/temp/mserror.log";


	ThreadFile tf(strPath);

	strError+="\r\n";

	CTime stime=CTime::GetCurrentTimeEx();

	CString strlog;
	strlog.Format("%s\t%s",stime.Format().c_str(),strError.getText());

	char *pstr=strlog.getText();

	// 判断文件大小：在不打开文件的情况下实现
	struct _stat buf;
	if( _stat( pstr, &buf ) == 0 )
	{
		if( buf.st_size > 10000*1024 )
		{
			FILE *log = fopen( pstr, "w" );
			if( log != NULL )
				fclose( log );
		}
	}

	tf.append(pstr,(ccxx_size_t)strlen(pstr));

/*
	Lock l(g_ErrorLoglock);

	CString strTemp=_T(""),
		strFile=_T("");
	strFile.Format("%s\\MonitorManager\\meerror.log",g_strRootPath);
	CTime time=CTime::GetCurrentTime();
	strTemp=time.Format("%Y-%m-%d %H:%M:%S");

	strTemp+="\t"+strError+"\r\n";

#ifndef DEBUG_T
	FILE *fp=NULL;
	fp=::fopen(strFile,"a+");
	if(fp==NULL)
	{
		printf("���ʧ�\n");
		return ;
	}
	::fputs(strTemp,fp);
	::fclose(fp);
#else
	printf("%s",strTemp);
#endif
	*/
	return ;

}

int Util::AppendThenClearAllRecords(std::list<SingelRecord> & inrcd)
{
	MutexLock lock(g_InsertMQLock);
	inrcd.clear();
	listrcd.swap(inrcd);
	if(inrcd.empty())
	{
		printf("incrd is empty!\n");
		return 0;
	}
	bool ret=::AppendMassRecord(inrcd,"default",g_ServerHost);
	std::list<SingelRecord>::iterator it;
	for(it=inrcd.begin();  it!=inrcd.end(); ++it)
		if( (it->data)!=NULL )
			delete [] it->data;
	if(ret==false) 
	{
		printf("ret==false\n");
		return -1;
	}
	return inrcd.size();
}

bool Util::CacheRecords(string tablename,const char *pdata,int datalen)
{
	char * tdata;
	try{
		tdata=new char[datalen];
		if(tdata==NULL)
			return false;
		memmove(tdata,pdata,datalen);
	}
	catch(...)
	{
		return false;
	}
	SingelRecord rcd;
	rcd.monitorid= tablename;
	rcd.data= tdata;
	rcd.datalen= datalen;

	listrcd.push_back(rcd);
	return true;
}

bool Util::InsertSvdb(string tablename,const char *pdata,int datalen,string addr)
{
	MutexLock lock(g_InsertMQLock);
	if(Univ::enablemass)
		return CacheRecords(tablename,pdata,datalen);

	bool bret=false;
	try
	{
		bret=::AppendRecord(tablename,pdata,datalen,"default",addr);
		cout<<"AppendRecord "<<tablename.c_str()<<"  to "<<addr.c_str()<<" is done!"<<endl;

	}catch(...)
	{
		return false;
	}

	return bret;


}

bool Util::InsertSvdb(string tablename,const char *pdata,int datalen)
{
	MutexLock lock(g_InsertMQLock);
	if(Univ::enablemass)
		return CacheRecords(tablename,pdata,datalen);

	bool bret=false;
	try
	{
//		bret=::AppendRecord(tablename,pdata,datalen,"default",g_ServerHost);
		bret=::AppendRecord(tablename,pdata,datalen,"default","localhost");
		cout<<"AppendRecord "<<tablename.c_str()<<"  to localhost is done!"<<endl;
	}catch(...)
	{
		return false;
	}

	return bret;

}

/*String Util::GetStringByID(UINT strid)
{
/*	if(m_hResLibrary==NULL)
	{
     	CString strTemp=_T("");
		strTemp.Format("%s\\MonitorManager\\ResLibrary.dll",g_strRootPath);
		m_hResLibrary=::LoadLibrary(strTemp);
		if(!m_hResLibrary)
		{
			ErrorLog("Load ResLibrary.dll failed!");
			return "";
		}
	}
	char buf[256]={0};
	int err=::LoadString(m_hResLibrary,strid,buf,255);

	if(err!=0)
     	return buf;

	return "";


}*/

/*BOOL Util::GetStringByID(UINT strid, char *szbuf, int len)
{
/*	if(m_hResLibrary==NULL)
	{
     	CString strTemp=_T("");
		strTemp.Format("%s\\MonitorManager\\ResLibrary.dll",g_strRootPath);
		m_hResLibrary=::LoadLibrary(strTemp);
		if(!m_hResLibrary)
		{
			ErrorLog("Load ResLibrary.dll failed!");
			return FALSE;
		}
	}

	int err=::LoadString(m_hResLibrary,strid,szbuf,len);

	return err!=0;

	return 0;

}*/

/*BOOL Util::InsertSVMQ(CString strQueueName, CString strMessage, CString strLabel)
{
      CString strError;
      if(!OpenQueue("127.0.0.1",strQueueName))
      {
         throw MSException("Open queue failed");
      }
      
        

}*/
#ifndef WIN32
BOOL Util::InsertSVMQ(CString strQueueName, CString strMessage, CString strLabel)
{
      MutexLock lock(g_InsertMQLock);
      if(strLabel.isEmpty())
         strLabel="SVMQ";

      void *pqueue=NULL;
      int err=0;
      CString strError="";
      if((err=OpenQueue(g_SVMQAddress.getText(), (char *)strQueueName, &pqueue))>0)
      {
           if((err=Push(pqueue, strLabel, strMessage))>0)
	   {
	       CloseQueue(pqueue);
	       return TRUE;
	   }else
	   {
	      CloseQueue(pqueue);
	      strError.Format("Send Message error errorid:%d",err);
	      throw MSException((char *)strError);
	      
	   }
	   
          
      }else
      {
          puts("Open queue failed");
          strError.Format("Open queue failed errorid:%d",err);
	  throw MSException(strError.getText());
      }
      
      return TRUE;

}
#endif

/*BOOL Util::InsertSVMQ(CString strQueueName, CString strMessage, CString strLabel)
{
 
	Lock l(g_InsertMQLock);

	HRESULT hr=NULL;
	IMQReal *pmq=NULL;
	if(strLabel.IsEmpty())
		strLabel="SVMQ";

	CString strTemp=_T("");

	try{
		
		hr=::CoCreateInstance(CLSID_MQReal,NULL,CLSCTX_SERVER,IID_IMQReal,(void**)&pmq);
		if(FAILED(hr))
		{
			puts("CoCreateInstance ʧ�");
			throw MSException("CoCreateInstance failed");
			return FALSE;
		}
		
		BSTR QueueName=strQueueName.AllocSysString();
		BSTR Message=strMessage.AllocSysString();
		BSTR Label=strLabel.AllocSysString();
		BSTR ip=::SysAllocString(L"127.0.0.1");
		long lr=0;
//		BSTR ErrM;
		hr=pmq->OpenQueue(ip,QueueName);
		::SysFreeString(QueueName);
		::SysFreeString(ip);

		pmq->get_ErrorCode(&lr);
		if(lr!=0)
		{
			BSTR berr;
			pmq->get_ErrorMessage(&berr);
			_bstr_t be;
			be=berr;

			printf("Open queue failed:%ld\n",lr);
			strTemp.Format("Open queue failed:%ld,Message error :%s",lr,(char*)be);
			pmq->Release();
			throw MSException((LPCSTR)strTemp);
			return FALSE;
		}
		hr=pmq->Push(Message,Label);
		::SysFreeString(Message);
		::SysFreeString(Label);

		pmq->get_ErrorCode(&lr);
		if(lr!=0)
		{
			BSTR berr;
			pmq->get_ErrorMessage(&berr);
			_bstr_t be;
			be=berr;
			printf("Send message failed:%ld",lr);
			strTemp.Format("Send message failed:%ld,error message is:%s",lr,(char*)be);
			::SysFreeString(be);
			hr=pmq->CloseQueue();
			pmq->Release();

			throw MSException((LPCSTR)strTemp);
			return FALSE;
		}
		hr=pmq->();
		
	}catch(...)
	{
		puts("InsertSVMQ failed");
		pmq->Release();
		throw MSException("InsertSVMQ Exception");
		return FALSE;
	}

	pmq->Release();

	return TRUE;


}*/
#if defined(WIN32)
BOOL Util::InitSocket()
{
	WSADATA WSAData;
	int err=0;
	err=WSAStartup(MAKEWORD(2, 2), &WSAData); 

	if(err!=0)
	{
		return FALSE;
	}
	if(WSAData.wVersion!=MAKEWORD(2,2))
	{
//	   	AddToErrorLog(::GetStringByID(IDS_INITFAILED));
//		AddToErrorLog("�ʧ�");
		return FALSE;
	}

	return TRUE;


}



void Util::ClearSocket()
{
	::WSACleanup();

}

#endif


/*!
    \fn Util::method_2()
 */
void Util::method_2()
{
    /// @todo implement me
}


/*!
    \fn Util::FreeStringList(CStringList &lst)
 */
void Util::FreeStringList(CStringList &lst)
{

 /*  CStringList::iterator it;
   for(it=lst.begin();it!=lst.end();it++)
       free(*it);*/
   lst.clear();
    /// @todo implement me
}

