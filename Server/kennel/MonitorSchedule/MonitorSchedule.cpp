/*******************************************************************
MonitorSchedule.cpp
Main file;
*******************************************************************/

//#include "stdio.h";
//#include <iostream>

#include "MonitorSchedule.h"
//#include <TTime.h>
#include "Util.h"
#include "MonitorSchMain.h"
#include "LoadConfig.h"
#include "Monitor.h"
#include "ThreadContrl.h"
#include <svdbapi.h>
#include <libutil/Time.h>

#include "ProcessMonitorData.h"

using namespace std;

#ifdef WIN32

#define		WM_COMMUNICATION	WM_USER+135

#endif

CString g_strRootPath="D:\\v70";
string	g_ServerHost="localhost";
Util *putil;

std::string g_RefreshQueueName="";
std::string g_QueueAddr="";
std::string g_ConfigTrackQueueName="SiteView70-ConfigTrack";

Option *g_pOption=NULL;

CTime g_LastSchTime;

MonitorSchMain*pMain=NULL;

int Univ::seid(1);
int Univ::msappend(1000);
int Univ::pluscount(0);
bool Univ::enablemass(false);
bool Univ::enabltows(false);


#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
void WriteLog( const char* str )
{
	char timebuf[128],datebuf[128];

	_tzset();
	_strtime( timebuf );
	_strdate( datebuf );

	string path = GetSiteViewRootPath();
	path += "\\temp\\mserror.log";

	// 判断文件大小：在不打开文件的情况下实现
	struct _stat buf;
	if( _stat( path.c_str(), &buf ) == 0 )
	{
		if( buf.st_size > 50000*1024 )
		{
			FILE *log = fopen( path.c_str(), "w" );
			if( log != NULL )
				fclose( log );
		}
	}
	

	FILE *log = fopen( path.c_str(),"a+");
	if( log != NULL )
	{
		fprintf( log, "%s %s \t%s\n", datebuf, timebuf, str );
		fclose( log );
	}

}

void WriteTxt( const char* str, const char* pszFileName=NULL )
{
	return;

	char timebuf[128],datebuf[128];

	_tzset();
	_strtime( timebuf );
	_strdate( datebuf );

	char szProgramName[] = "monitorSchedule.log";
	string	strEccPath = GetSiteViewRootPath();
	char szLogFile[128];


	if( pszFileName != NULL )
	{
		sprintf( szLogFile, "%s\\itsm\\runtime\\logs\\%s.log", 
		         strEccPath.c_str(), pszFileName );
	}
	else
	{
		//sprintf( szLogFile, "%s\\itsm\\runtime\\logs\\%s", 
		//         strEccPath.c_str(), szProgramName );

		sprintf( szLogFile, "%s", szProgramName );
	}
	

	FILE *log = fopen( szLogFile,"a+");
	if( log != NULL )
	{
		fprintf( log, "%s %s \t%s\n", datebuf, timebuf, str );
		fclose( log );
	}

}



void Run(void)
{
	puts("run()");
	//	g_strRootPath="/home/siteview/chenxing/monitor";
	g_strRootPath=Util::GetRootPath();
	if(g_strRootPath.IsEmpty())
		throw MSException("Root path is empty!");

	CString strError;
	putil=new Util;
	pMain=new MonitorSchMain;
	try{
		pMain->Init();
		pMain->Run();
	}catch (MSException &e)
	{
		strError.Format("Run failed :%s",e.Description);
		string strTemp = strError.getText();
		WriteLog( strTemp.c_str() );
		//putil->ErrorLog(strError);
		//	throw MSException((char *)strError);
		::ExitProcess(2);
		return ;
	}

}

class testtex : public ThreadEx
{
public:
	testtex()
	{
		//pe=new Event();
	}
	virtual void run(void);
	Event pe;

};

void testtex::run(void)
{
	pe.wait();
	puts("In thread");
}

void testmonitor(void)
{
	putil=new Util;
	try{	


		putil->Init();
		LoadConfig lc;
		lc.LoadAll();
		CMonitorList Monitorlst;
		lc.CreateMonitors(Monitorlst);
		int count =Monitorlst.size();
		printf("%d monitor be load\n",count);
		//Monitor *pM=NULL;
		//POSITION pos=Monitorlst.GetHeadPosition();
		//POSITION pos2=NULL,pos3=NULL;
		CMonitorList::iterator it=Monitorlst.begin();

		ReturnData* prd=NULL;
		int n=0;
		while(Monitorlst.size()>0)
		{
			//	pM=Monitorlst.RemoveTail();
			Monitor*pM=Monitorlst.front();
			Monitorlst.pop_front();
			//				pM=Monitorlst.GetNext(pos);
			printf("\n------------MonitorID:%s----------------\n",pM->GetMonitorID());
			printf("MonitorID:%s\nClass:%s\nMonitorType:%d\nSEID:%s\nParentID:%s\nFrequency:%d\n", \
				(char*)pM->GetMonitorID(),(char*)pM->GetMonitorClass(),pM->GetMonitorType(),pM->GetSEID().c_str(),pM->GetParentID().c_str(), \
				pM->GetFrequency());

			puts("\n@@Parameter list@@");

			CStringList &lst=pM->GetParameterList();
			CStringList::iterator it2;
			it2=lst.begin();
			while(it2!=lst.end())
				printf("%s\n",*it2++);

			puts("\n+++ReturnList+++\n");

			CReturnDataList&rdl=pM->GetReutrnList();

			CReturnDataList::iterator it3;
			//	pos3=rdl.GetHeadPosition();
			it3=rdl.begin();
			while(it3!=rdl.end())
			{
				//prd=rdl.GetNext(pos3);
				prd=*it3++;
				printf("Label:%s\nName:%s\nUnit:%s\nType:%s\n",(char *)prd->m_Label,(char *)prd->m_Name,(char *)prd->m_Unit,(char *)prd->m_Type);
			}

			puts("\n===StateCondition===\n");

			StateCondition ** sct=pM->GetStateCondition();
			StateCondition *psct=sct[0];
			printf("Expression:%s\nStateType:%d\n",(char *)psct->m_Expression,psct->m_Type);

			CStateConditionItemList&Item=psct->GetStateConditionList();
			//pos2=Item.GetHeadPosition();
			CStateConditionItemList::iterator it4;
			it4=Item.begin();
			while(it4!=Item.end())
			{
				//					StateConditionItem *psti=Item.GetNext(pos2);
				StateConditionItem *psti=*it4++;
				printf("ItemID:%d\nOperator:%s\nName:%s\nValue:%s\n",psti->m_ItemID,(char*)psti->m_Operator,(char*)psti->m_ParamName,(char *)psti->m_ParamValue);
			}

			psct=sct[1];
			printf("Expression:%s\nStateType:%d\n",(char *)psct->m_Expression,psct->m_Type);

			CStateConditionItemList&Item2=psct->GetStateConditionList();
			//	pos2=Item2.GetHeadPosition();
			it4=Item2.begin();
			while(it4!=Item2.end())
			{
				//					StateConditionItem *psti=Item2.GetNext(pos2);
				StateConditionItem *psti=*it4++;
				printf("ItemID:%d\nOperator:%s\nName:%s\nValue:%s\n",psti->m_ItemID,(char*)psti->m_Operator,(char*)psti->m_ParamName,(char *)psti->m_ParamValue);
			}


			psct=sct[2];
			printf("Expression:%s\nStateType:%d\n",(char *)psct->m_Expression,psct->m_Type);

			CStateConditionItemList&Item3=psct->GetStateConditionList();
			//	pos2=Item3.GetHeadPosition();
			it4=Item3.begin();
			while(it4!=Item3.end())
			{
				//			StateConditionItem *psti=Item3.GetNext(pos2);
				StateConditionItem *psti=*it4++;
				printf("ItemID:%d\nOperator:%s\nName:%s\nValue:%s\n",psti->m_ItemID,(char*)psti->m_Operator,(char*)psti->m_ParamName,(char *)psti->m_ParamValue);
			}

			n++;



			delete pM;
		}

		printf("Total %d monitors\n",n);




	}
	catch(MSException &e)
	{
		printf("putil init exception:%s\n",e.Description);
		return ;
	}
}

BOOL GetMonitorListByFile(CMonitorList &lstMonitor,const char *FileTitle)
{
	CString strFileName;

	strFileName.Format("%s/MonitorManager/%s.txt",g_strRootPath.getText(),FileTitle);
	puts(strFileName);

	FILE *pf=fopen(strFileName.getText(),"r+");
	char buf[256]={0};
	if(pf)
	{
		CString strid="";
		LoadConfig lc;
		lc.LoadAll();	
		while(!feof(pf))
		{
			char *p=fgets(buf,255,pf);
			//puts(p);


			if(p!=NULL)
			{
				strid=p;
				strid.Replace("\r","");
				strid.Replace("\n","");
				Monitor *pM=new Monitor;
				try{

					if(lc.CreateSingleMonitor(pM,strid.getText()))
					{
						puts("Create ok");
						lstMonitor.push_back(pM);
						puts("push ok");
					}
					else
					{
						puts("Create failed");
						delete pM;
					}
					memset(buf,0,256);
				}catch(...)
				{
					delete pM;
				}

			}else
				break;

		} 
		fclose(pf);  
	}else
		throw MSException("Open file failed");

	return (lstMonitor.size()>0);

}

bool MakeMonitorListByBuf(CMonitorList &lstMonitor,const char *buf)
{
	if(g_pOption==NULL)
		return false;

	const char *pt=buf;
	string str;
	LoadConfig lc;
	lc.m_pOption=g_pOption;

	lc.LoadAll();	

	do{

		str=pt;
		if(str.empty())
			break;
		Monitor *pM=new Monitor();
		if(pM==NULL)
			break;
		try{
			if(lc.CreateSingleMonitor(pM,str.c_str()))
			{
				puts("Create ok");
				lstMonitor.push_back(pM);
				puts("push ok");
			}
			else
			{
				puts("Create failed");
				delete pM;
			}


		}catch(...)
		{
			puts("Create exception");
			delete pM;
		}


		pt+=str.size()+1;

	}while(pt[0]!='\0');

	return true;
}

bool GetMonitorListByPipe(CMonitorList &lstMonitor,string pipeName,string mlabel,string addr)
{
	MQRECORD mrd=::PopMessage(pipeName,0,"default",addr);
	if(mrd==INVALID_VALUE)
		return false;
	string label;
	svutil::TTime ct;
	unsigned int len=0;
	if(!::GetMessageData(mrd,label,ct,NULL,len))
	{
		::CloseMQRecord(mrd);
		return false;
	}

	if(label.compare(mlabel)!=0)
	{
		::CloseMQRecord(mrd);
		return false;
	}
	printf("label :%s\n",label.c_str());
	if(len<=0)
	{
		::CloseMQRecord(mrd);
		return false;
	}

	char *pbuf=new char[len];
	memset(pbuf,0,len);
	if(!::GetMessageData(mrd,label,ct,pbuf,len))
	{
		::CloseMQRecord(mrd);
		return false;
	}
	::CloseMQRecord(mrd);

	printf("len :%d\n",len);

	bool bret=false;

	bret=MakeMonitorListByBuf(lstMonitor,pbuf);
	delete [] pbuf;

	return bret;

}

bool testParseOpt(string opt,string &name,string &type)
{
	int pos=0;
	pos=opt.find(':');
	if(pos<0)
		return false;
	name=opt.substr(0,pos);
	if(name.empty())
		return false;
	type=opt.substr(pos+1);

	if(type.empty())
		return false;
	return true;
}
void test_taskplan()
{
	putil=new Util;
	try{	


		putil->Init();
		LoadConfig lc;
		lc.LoadAll();

		TASKMAP tmap;
		if(!lc.LoadTaskPlan(tmap)) //加载任务计划
		{
			puts("Load task plan failed");
			return ;
		}

		TASKMAP::iterator it;
		while(tmap.findnext(it))
		{
			puts("----------------------------------------------------------------------------------");
			printf("Task name:%s,Task type:%d\n",(*it).getkey().getword(),
				(*it).getvalue()->m_type);
			for(int i=0;i<7;i++)
			{
				printf("Week %d,enable:%s\n",i,(*it).getvalue()->m_week[i].m_enable ? "true" : "false");
				int n=(*it).getvalue()->m_week[i].m_task.size();
				for(int k=0;k<n;k++)
				{
					printf("bhour:%d,bminute:%d,ehour:%d,eminute:%d\n",(*it).getvalue()->m_week[i].m_task[k].m_beginhour,
						(*it).getvalue()->m_week[i].m_task[k].m_beginminute,
						(*it).getvalue()->m_week[i].m_task[k].m_endhour,
						(*it).getvalue()->m_week[i].m_task[k].m_endminute);

				}

			}
		}
		


	}catch(...)
	{
		puts("Exception happend");
	}



}
int CheckReturnLen(const char *buf)
{
	if(buf==NULL)
		return -1;

	const char *pt =buf;
	if(pt[0]=='\0')
		return 0;

	int pos=0;
	while(true)
	{
		pt++;
		pos++;
		if((pt[0]=='\0')&&(pt[1]=='\0'))
			break;
	}

	return pos+2;

}
void Run_ProcessMonitor(string queuename)
{
   CString strError;
#ifdef WIN32

	if(queuename.empty())
		return ;
	MQRECORD mrd=::PopMessage(queuename);
	if(mrd==INVALID_VALUE)
	{
	   throw MSException("Pop Message failed failed in Run_ProcessMonitor function");
		return ;
	}
   string label="";
   unsigned int blen=0;
   svutil::TTime rt;
   if(!GetMessageData(mrd,label,rt,NULL,blen))
   {
      ::CloseMQRecord(mrd);
	   throw MSException("Get Message failed failed in Run_ProcessMonitor function");
	   return;
   }
   svutil::buffer dbuf;
   if(!dbuf.checksize(blen))
   {
       ::CloseMQRecord(mrd);
	   return ;
   }
   if(!GetMessageData(mrd,label,rt,dbuf,blen))
   {
       ::CloseMQRecord(mrd);
	   return;
   }

   ::CloseMQRecord(mrd);


   ProcessMonitorData pdata;
   if(!pdata.CreateObjectByRawData(dbuf,blen))
   {
	   ::CloseMQRecord(mrd);
	   throw MSException("Create object failed in Run_ProcessMonitor function");
	   return;
   }

 	HMODULE hm=::LoadLibrary(pdata.m_dllname.c_str());
	if(!hm)
	{
		strError.Format("RunMonitor-Load library failed,DLL name:%s",pdata.m_dllname.c_str());
		throw MSException((LPCSTR)strError);
	}

	char retbuf[RETBUFCOUNT]={0};


	LPFUNC pfunc = (LPFUNC)GetProcAddress(hm,pdata.m_funname.c_str());
	if(!pfunc)
	{
		::FreeLibrary(hm);
		throw MSException("RunMonitor-GetProcAddress failed");

	}


	int buflen=RETBUFCOUNT;

	try{

		(*pfunc)(pdata.m_data,retbuf,buflen);

	}catch(...)
	{
		::FreeLibrary(hm);
		throw MSException("RunMonitor-Execute monitor happen exception");
	}
	::FreeLibrary(hm);

	pdata.Clear();

	int ilen=CheckReturnLen(retbuf);
	if(ilen<=0)
		throw MSException("Return result error in Run_ProcessMonitor function"); 

	if(!::PushMessage(queuename,"SiteView_Return",retbuf,ilen))
		throw MSException("Push message failed in Run_ProcessMonitor function");



#else
#endif


}

void test_ProcessMonitorData()
{
	ProcessMonitorData pmdata;
	char buf[256]={0};
	pmdata.m_funname="myfunc";
	pmdata.m_dllname="C:\\siteview\\siteview ecc\\fcgi-bin\\monitor.dll";
	pmdata.m_data=buf;
	pmdata.m_datalen=256;

	unsigned int len=0;
	len=pmdata.GetRawDataSize();
	svutil::buffer dbuf(len);

	if(!pmdata.GetRawData(dbuf,len))
	{
		puts("Get raw data failed");
		return;
	}

	ProcessMonitorData clone;
//	clone.m_data=buf;
//	clone.m_datalen=256;
	if(!clone.CreateObjectByRawData(dbuf,len))
	{
		puts("Create Object failed");
		return;
	}

	printf("funname:%s,dllname:%s,datalen:%d,state:%d\n",clone.m_funname.c_str(),clone.m_dllname.c_str(),clone.m_datalen,clone.m_state);
	clone.Clear();
}


int main(int argc,char *argv[])
{
//命令行参数说明 ?
	printf("\n Process arg %d: ",argc);
	for(int i=0; i<=argc-1; ++i)
		printf("%s  ",argv[i]);
	printf("\n");
	SetSvdbAddrByFile("mc.config");
	if(argc>1)
	{
		putil=new Util();
		if(argc>4)
		{
			puts("Parameter error");
			//putil->ErrorLog("Parameter error");
			return -1;
		}
		puts("Begin refresh"); 
		BOOL isRefresh=FALSE;
		g_strRootPath=putil->GetRootPath(); 
		try
		{
			ThreadContrl *pt=new ThreadContrl();
			Option *popt=new Option;
			if(popt==NULL)
			{
				//putil->ErrorLog("Create Option object failed");
				return 1;
			}
			//load 设置文件 mc.config
			popt->LoadOption();
			g_ServerHost=popt->m_ServerAddress;
			pt->m_pOption=popt;
			g_pOption=popt;
			if(argc==2)
			{
				pt->InitRefresh(1);
				LoadConfig lc; // monitor device group file
				lc.m_pOption=popt;
				lc.LoadAll();	      
				Monitor *pM=new Monitor();
				if(!lc.CreateSingleMonitor(pM,argv[1]))
				{
					puts("Create Monitor failed");
					delete pM;
					return -1;
				}
				pt->ExecuteMonitor(pM);
				delete pM;
//				return 1;

			}
			else if(argc==3)
			{
				if(strcmp(argv[1],"-e")==0)
				{
					Run_ProcessMonitor(argv[2]);
					return 1;
				}
				pt->InitRefresh(10);
				isRefresh=TRUE;
//				putil->InsertSVMQ("SiteviewRefresh","Start",argv[1]);
//			    putil->g_strSession=argv[1];
				CMonitorList lstMonitor;
				if(GetMonitorListByFile(lstMonitor,argv[2]))
				{
					printf("Monitor counts is :%d\n",lstMonitor.size());
					pt->RefreshMonitors(lstMonitor);
				}
				else
					;
//				putil->ErrorLog("Monitor list empty");
//				putil->InsertSVMQ("SiteviewRefresh","End",argv[1]);
			}
			else if(argc==4)
			{
				char buf[256]={0};
				pt->InitRefresh(30);
				printf("Parameter:%s,%s,%s\n",argv[1],argv[2],argv[3]);
				CMonitorList lstMonitor;
				if(GetMonitorListByPipe(lstMonitor,argv[1],argv[2],argv[3]))
				{
					printf("Monitor counts is :%d\n",lstMonitor.size());
					g_RefreshQueueName=argv[2];
					g_QueueAddr=argv[3];
					pt->RefreshMonitors(lstMonitor);
				}else
				{
//					putil->ErrorLog("Refresh Monitors empty");
					puts("eeeeeeee");
				}
				sprintf(buf,"Refresh end");
				::PushMessage(argv[2],"Refresh_END",buf,strlen(buf)+1,"default",argv[3]);
//				::Sleep(200000000);
			}
		}
		catch(MSException &e)
		{
			//			if(isRefresh)
			//		           putil->InsertSVMQ("SiteviewRefresh","End",argv[1]);
			//putil->ErrorLog(e.GetDescription());
			return -1;
		}
		if( Univ::enablemass && putil!=NULL )
		{	
			std::list<SingelRecord> listrcd;
			int count(0);
			if( (count=putil->AppendThenClearAllRecords(listrcd))>0 )
				cout<<"AppendMass "<<count<<" records done,"<<" slept "<<Univ::msappend<<" ms."<<endl;
			else if(count<0)
				cout<<"AppendMassRecord failed!"<<endl;
		}
		Sleep(3000);
		return 1;

	}

#ifdef WIN32
 	HANDLE hCommEvent=::CreateEvent(NULL,TRUE,FALSE,"Global\\Siteview-Communictions");
	if(hCommEvent==NULL)
	{
		puts("Create event of communication  failed");
		return -1;
	}
	if(::GetLastError()!=ERROR_ALREADY_EXISTS)
	{
		puts("Parent process isn't exist");
		//return -2;
	}
    ::SetLastError(0);
#endif

	try
	{
		Run();
	}
	catch(MSException &e)
	{
		//putil->ErrorLog(e.GetDescription());
		WriteLog( e.GetDescription() );
		return -3;
	}

#ifdef WIN32
	MSG msg;
	BOOL bRet=TRUE;
	while((bRet=::GetMessage(&msg,NULL,0,0))!=0)
	{
//		MemLeakCheckAndFree();
		if(bRet==-1)
		{
			continue;
		}
		if(msg.message==WM_COMMUNICATION)
		{
			CTime curt=CTime::GetCurrentTimeEx();
			svutil::TTimeSpan tsp=curt-g_LastSchTime;
			char text[256]={0};
			sprintf(text,"@@@@ COMMUNICATION, min:%d ,%s -- %s @@@@\n",tsp.GetTotalMinutes(),g_LastSchTime.Format().c_str(),curt.Format().c_str());
			puts(text);
			OutputDebugString(text);
			if(tsp.GetTotalMinutes()<2)
				::ResetEvent(hCommEvent);
			else
			{
				string show= " Self-exit MonitorSchedule! ";
				show+=text;
				//putil->ErrorLog(show.c_str());
				WriteLog( show.c_str() );
				OutputDebugString(show.c_str());
				::Sleep(100);
				return 1;
			}
		}

		TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	if( Univ::enablemass && putil!=NULL )
	{	
		std::list<SingelRecord> listrcd;
		int count(0);
		if( (count=putil->AppendThenClearAllRecords(listrcd))>0 )
			cout<<"AppendMass "<<count<<" records done,"<<" slept "<<Univ::msappend<<" ms."<<endl;
		else if(count<0)
			cout<<"AppendMassRecord failed!"<<endl;
	}

	if(pMain!=NULL)
	{
		if(pMain->m_bDataChange)
			pMain->SerialMonitorData(true);
	}

#else
	while(true)
	{
		ThreadEx::sleep(2000000);
	}
#endif
	return 1;
}
