#include "MonitorSchMain.h"
#include "MonitorSchedule.h"
#include "ScheduleThread.h"
#include "stlini.h"
#include "SerialData.h"

extern Util *putil;
extern CString g_strRootPath;
extern string	g_ServerHost;


MonitorSchMain::MonitorSchMain()
{
	m_pGroups=NULL;
	m_pSub=NULL;
	m_ScheduleThread=NULL;
	m_pOption=NULL;
	m_bDataChange=false;
	m_pAppendThread=NULL;

}

MonitorSchMain::~MonitorSchMain()
{
	delete m_pGroups;
	delete m_pSub;

	CMonitorList::iterator it;
	while(it!=m_MonitorList.end())
	{
		Monitor *pMonitor=*it++;
		delete pMonitor;
	}
/*	while(m_MonitorList.GetCount()>0)
	{
		Monitor *pMonitor=m_MonitorList.RemoveTail();
		delete pMonitor;
	}
	*/

	TASKMAP::iterator itt;
	while(m_TaskMap.findnext(itt))
	{
		Task *p=(*itt).getvalue();
		if(p)
			delete p;
	}

}

BOOL MonitorSchMain::Init()
{
	////锟绞憋拷锟缴?

//	m_pSub=new Subsequent;
//	m_pGroups=new Groups;

	m_pOption=new Option;
	if(m_pOption==NULL)
		throw MSException("Create option object failed");
	m_pOption->LoadOption();

	g_ServerHost=m_pOption->m_ServerAddress;
	m_lc.m_pOption=m_pOption;

	LoadPreLibrary();

	bool flag=false;

	m_lc.LoadAll();

	if(!m_pOption->m_UseLocalBuffer)
	{// for master
		m_pSub=new Subsequent;
		m_lc.CreateSubsequent(m_pSub);

		m_pGroups=new Groups;
		m_lc.CreateGroups(m_pGroups);

		m_lc.LoadTaskPlan(m_TaskMap);

		m_lc.CreateMonitors(m_MonitorList);

		m_lc.ReleaseAll();

	}else
	{//for slave
		char buffile[1024]={0};
		sprintf(buffile,"%s/temp/MonitorScheduleSerialData.txt",g_strRootPath.getText());
		FILE *pf=fopen(buffile,"r");
		if(pf==NULL)
		{
			m_lc.LoadAll();
			m_pSub=new Subsequent;
			m_lc.CreateSubsequent(m_pSub);

			m_pGroups=new Groups;
			m_lc.CreateGroups(m_pGroups);

			m_lc.LoadTaskPlan(m_TaskMap);

			m_lc.CreateMonitors(m_MonitorList);

			m_lc.ReleaseAll();

		}else
		{
			fclose(pf);
			m_pSub=new Subsequent;
			m_pGroups=new Groups;
			if(!SerialMonitorData(false))
			{
				m_lc.LoadAll();
				m_lc.CreateSubsequent(m_pSub);
				m_lc.CreateGroups(m_pGroups);
				m_lc.LoadTaskPlan(m_TaskMap);
				m_lc.CreateMonitors(m_MonitorList);
				m_lc.ReleaseAll();

			}else
				flag=true;
		}
	}

	if((m_pOption->m_UseLocalBuffer)&&!flag)
		SerialMonitorData(true);

	if(m_MonitorList.size()==0)
		throw MSException("Monitor list is empty");

	m_ScheduleThread=new ScheduleThread(this);
	if(m_ScheduleThread==NULL)
		throw MSException("Create schdule thread failed");
	m_ScheduleThread->Init();

	m_pCommThread=new CommunicationThread(this);
	if(m_pCommThread==NULL)
		throw MSException("Create communication thread failed");
	m_pCommThread->Start();

	if( Univ::enablemass )
	{
		m_pAppendThread=new CAppendMassRecord();
		if(m_pCommThread==NULL)
			throw MSException("Create append mass records thread failed");
		m_pAppendThread->Start();
	}
	return TRUE;
}

void MonitorSchMain::Run()
{
/*	POSITION pos=m_MonitorList.GetHeadPosition();
	while(pos)
	{

		Monitor *pm=m_MonitorList.GetNext(pos);
		puts("\n\n************************************************************************");
		printf("Monitor id:%s, DLL name:%s, Function:%s\n",pm->GetMonitorID(),pm->GetLibrary(),pm->GetProcess());
		CStringList&lstParam=pm->GetParameterList();
		POSITION pos2=lstParam.GetHeadPosition();
		while(pos2)
		    printf("%s\n",lstParam.GetNext(pos2));
	}
*/
	if(m_ScheduleThread!=NULL)
		m_ScheduleThread->Start();

}

CMonitorList & MonitorSchMain::GetMonitorList()
{
	return m_MonitorList;

}

BOOL MonitorSchMain::CheckGroupDependState(Monitor *pMonitor,CString &strDependID)
{
	int nstate=0;

	char mes[500]={0};

	Monitor *pGM=NULL;
	switch(pMonitor->GetGroupDependState())
	{
	case Monitor::groupdependno:
		return FALSE;
	case Monitor::groupdependyes:
		pGM=pMonitor->GetGroupDependMonitor();
		if(pGM==NULL)
			break;
		nstate=pGM->GetLastState();
		nstate = (nstate==Monitor::STATUS_BAD) ? Monitor::STATUS_ERROR : nstate;

		//sprintf(mes,"In depend yes Monitor id:%s,depend monitorid:%s,last state:%d,DependCondition:%d",pMonitor->GetMonitorID(),pGM->GetMonitorID(),nstate,pMonitor->GetGroupDependCondition());
		//putil->ErrorLog(mes);

		if(nstate!=pMonitor->GetGroupDependCondition())
		{
			strDependID=pGM->GetMonitorID();
	    	return TRUE;
		}
		else
			return FALSE;
	default:break;

	}


	string seid=pMonitor->GetSEID();
	string pParentID=pMonitor->GetParentID();

	if(stricmp(seid.c_str(),pParentID.c_str())==0)
	{
		pMonitor->SetGroupDependState( Monitor::groupdependno);
		return FALSE;
	}

	Entity *pEntity=m_pGroups->GetEntityByID(pParentID.c_str());
	if(pEntity==NULL)
	{
		pMonitor->SetGroupDependState( Monitor::groupdependno);
		return FALSE;
	}
	int nCondition=0;
//	LPCSTR DependMonitorID=NULL;
	const char* DependMonitorID=NULL;


	bool isy=false;


	DependMonitorID=pEntity->GetDepend();
	if(strlen(DependMonitorID)>0)
	{
//		sprintf(mes,"In entity Monitor id:%s,DependCondition:%d",pMonitor->GetMonitorID(),pMonitor->GetGroupDependCondition());
//		putil->ErrorLog(mes);

	
		nCondition=pEntity->GetDependsCondition();
		if(nCondition<1||nCondition>4)
		{
			pMonitor->SetGroupDependState( Monitor::groupdependno);
			return FALSE;
		}

		pGM=GetMonitorByID(DependMonitorID);
		if(pGM==NULL)
		{
			pMonitor->SetGroupDependState( Monitor::groupdependno);
			return FALSE;
		}

		pMonitor->SetGroupDependState(Monitor::groupdependyes);
//		pMonitor->SetGroupDependID((LPCSTR)DependMonitorID);
		pMonitor->SetGroupDependMonitor(pGM);
		pMonitor->SetGroupDependCondition(nCondition);

		
//		if(nCondition!=GetMonitorLastState(DependMonitorID))
		nstate=pGM->GetLastState();
		nstate = (nstate==Monitor::STATUS_BAD) ? Monitor::STATUS_ERROR : nstate;

		//sprintf(mes,"In depend yes Monitor id:%s,depend monitorid:%s,last state:%d,DependCondition:%d",pMonitor->GetMonitorID(),pGM->GetMonitorID(),nstate,nCondition);
		//putil->ErrorLog(mes);

		if(nCondition!=nstate)
		{
			strDependID=DependMonitorID;
			return TRUE;
		}
		isy=true;

	}


    pParentID=pEntity->GetParentID();
	while(stricmp(pParentID.c_str(),seid.c_str())!=0)
	{
		GroupsItem *pGroup=m_pGroups->GetSingleGroupByID(pParentID.c_str());
		if(pGroup==NULL)
		{
			pMonitor->SetGroupDependState( Monitor::groupdependno);
			return FALSE;
		}

		DependMonitorID=pGroup->GetDependSon();
		if(strlen(DependMonitorID)>0)
		{
		//sprintf(mes,"In group Monitor id:%s,DependCondition:%d",pMonitor->GetMonitorID(),nstate,pMonitor->GetGroupDependCondition());
		//putil->ErrorLog(mes);

			nCondition=pGroup->GetDependsCondition();
			if(nCondition<1||nCondition>4)
			{
			   pMonitor->SetGroupDependState( Monitor::groupdependno);
    			return FALSE;
			}
			
			pGM=GetMonitorByID(DependMonitorID);
			if(pGM==NULL)
			{
				pMonitor->SetGroupDependState( Monitor::groupdependno);
				return FALSE;	
			}
			
			pMonitor->SetGroupDependState(Monitor::groupdependyes);
//			pMonitor->SetGroupDependID((LPCSTR)DependMonitorID);
			pMonitor->SetGroupDependMonitor(pGM);
			pMonitor->SetGroupDependCondition(nCondition);

//			if(nCondition!=GetMonitorLastState(DependMonitorID))
			nstate=pGM->GetLastState();
			nstate = (nstate==Monitor::STATUS_BAD) ? Monitor::STATUS_ERROR : nstate;
		//	sprintf(mes,"In group yes Monitor id:%s,depend monitorid:%s,last state:%d,DependCondition :%d",pMonitor->GetMonitorID(),pGM->GetMonitorID(),nstate,nCondition);
		//putil->ErrorLog(mes);


			if(nCondition!=nstate)
			{
				strDependID=DependMonitorID;
				return TRUE;
			}
			isy=true;

		}
		pParentID=pGroup->GetParentID();
	}



	if(!isy)
		pMonitor->SetGroupDependState( Monitor::groupdependno);

	return FALSE;

}

bool MonitorSchMain::CheckTask(Monitor *pMonitor,CTime ct,bool &isabs)
{
	ost::MutexLock lock(m_TaskMutex);
	isabs=false;

	string task=pMonitor->GetTaskName();
	if(task.empty())
		return true;
	Task **pt=m_TaskMap.find(task);
	if(pt==NULL)
		return true;

	int wd=ct.GetWeekDay();
	if((wd<0)||(wd>6))
		return true;

	int hour=ct.GetHour();
	int minute=ct.GetMinute();

	int n=(*pt)->m_week[wd].m_task.size();

	if((*pt)->m_type==Task::TASK_ABSOLUTE)
	{
		isabs=true;
		for(int i=0;i<n;i++)
		{
			if(hour==(*pt)->m_week[wd].m_task[i].m_beginhour)
				if(minute==(*pt)->m_week[wd].m_task[i].m_beginminute)
				{
					if((*pt)->m_week[wd].m_enable)
				    	return true;
					else
						return false;
				}
		}

		if((*pt)->m_week[wd].m_enable)
	    	return false;
		else
			return true;

	}else if((*pt)->m_type==Task::TASK_RELATIVE)
	{
		isabs=false;

		CTime btm;
		CTime etm;
		for(int i=0;i<n;i++)
		{
			btm=CTime(ct.GetYear(),ct.GetMonth(),ct.GetDay(),(*pt)->m_week[wd].m_task[i].m_beginhour,(*pt)->m_week[wd].m_task[i].m_beginminute,0);
			etm=CTime(ct.GetYear(),ct.GetMonth(),ct.GetDay(),(*pt)->m_week[wd].m_task[i].m_endhour,(*pt)->m_week[wd].m_task[i].m_endminute,59);

			if((ct>=btm)&&(ct<=etm))
			{
				if((*pt)->m_week[wd].m_enable)
					return true;
				else
					return false;
			}
			
		}

		if((*pt)->m_week[wd].m_enable)
	    	return false;
		else
			return true;

	}

	return true;
}

int MonitorSchMain::GetMonitorLastState(const char *MonitorID)
{
	if(m_MonitorList.empty())
		return 0;

	CMonitorList::iterator it;
	for(it=m_MonitorList.begin();it!=m_MonitorList.end();it++)
	{
		Monitor *pMonitor=*it;
		if(strcmp(MonitorID,pMonitor->GetMonitorID())==0)
			return pMonitor->GetLastState();
	}
/*	POSITION pos = m_MonitorList.GetHeadPosition();
	while(pos)
	{
		Monitor *pMonitor = m_MonitorList.GetNext(pos);
		if(_stricmp(MonitorID,pMonitor->GetMonitorID())==0)
			return pMonitor->GetLastState();
	}*/

	return 0;

}

Monitor* MonitorSchMain::GetMonitorByID(const char *MonitorID)
{
	if(m_MonitorList.empty())
		return NULL;

	CMonitorList::iterator it;
	for(it=m_MonitorList.begin();it!=m_MonitorList.end();it++)
	{
		Monitor *pMonitor=*it;
		if(strcmp(MonitorID,pMonitor->GetMonitorID())==0)
			return pMonitor;
	}


/*	POSITION pos = m_MonitorList.GetHeadPosition();
	while(pos)
	{
		Monitor *pMonitor = m_MonitorList.GetNext(pos);
		if(_stricmp(MonitorID,pMonitor->GetMonitorID())==0)
			return pMonitor;
	}*/

	return NULL;

}

void MonitorSchMain::UpdateDataByFile(LPCSTR FileTitle)
{
	if(strlen(FileTitle)<=0)

		return;



	CString strFileName="";

	strFileName.Format("%s/MonitorManager/%s.ini",(char *)g_strRootPath,(char *)FileTitle);

 

//	char buf[256]={0};

//	char typebuf[256]={0};



	INIFile inf=LoadIni(strFileName.getText());

	string strv=GetIniSetting(inf,"action","actiontype","");



//	DWORD dw=::GetPrivateProfileString("action","actiontype","",buf,255,strFileName);

	if(strv.empty())

//	if(strlen(buf)==0)

		throw MSException("Get action actiontype failed while update config");

	

//	::GetPrivateProfileString("action","type","",typebuf,255,strFileName);

	string strtype=GetIniSetting(inf,"action","type","");



	if(strtype.empty())

//	if(strlen(typebuf)==0)

		throw MSException("Get action type failed while update config");



	m_lc.LoadAll();

	try{

		

	//	if(_stricmp(buf,"add")==0)

		if(stricmp(strv.c_str(),"add")==0)

		{

			puts("in add");



			if(_stricmp(strtype.c_str(),"group")==0)

				AddGroup(strFileName,FALSE);

			else if(_stricmp(strtype.c_str(),"entity")==0)

				AddEntity(strFileName,FALSE);

			else if(_stricmp(strtype.c_str(),"monitor")==0)

				AddMonitor(strFileName,FALSE);

			else

				throw MSException("Invalid action type");

			

		}else if(_stricmp(strv.c_str(),"edit")==0)

		{

			puts("in edit");



			if(_stricmp(strtype.c_str(),"group")==0)

				AddGroup(strFileName,TRUE);

			else if(_stricmp(strtype.c_str(),"entity")==0)

				AddEntity(strFileName,TRUE);

			else if(_stricmp(strtype.c_str(),"monitor")==0)

				AddMonitor(strFileName,TRUE);

			else

				throw MSException("Invalid action type");

		}else if(_stricmp(strv.c_str(),"delete")==0)

		{

			puts("in delete");



			DeleteMonitor(strFileName);

			

		}else if(_stricmp(strv.c_str(),"disabled")==0)

		{

			puts("in disabled");



			AddMonitor(strFileName,TRUE);

		}else

		{

			throw MSException("Invalid actiontype");

		}

	}catch(MSException &e)

	{
	
	try {

#ifdef	WIN32
  
      ::DeleteFile(strFileName);
      
#else
	remove(strFileName);
#endif
       	
	
	
	}catch(...)
	{ ;}

		m_lc.ReleaseAll();

		throw MSException(e.Description);

	}

	

	m_lc.ReleaseAll();

	try{

		puts(strFileName);

		

//		CFile::Remove(strFileName);

#ifdef WIN32

		::DeleteFile(strFileName);

#else

	    remove(strFileName);	

#endif

	

	}catch(...)

	{

		;

	}



}



void MonitorSchMain::AddGroup(LPCSTR FileName,BOOL isEdit)

{

	puts("in addgroup");



	INIFile inf=LoadIni(FileName);



	



//	int count=::GetPrivateProfileInt("group","count",1,FileName);

	int count=atoi(GetIniSetting(inf,"group","count","1").c_str());

//	char buf[256]={0};

	char section[100]={0};

	CString strError="";

	std::string strt;

	for(int i=0;i<count;i++)

	{

//		memset(buf,0,256);

		sprintf(section,"list%d",i);

	//	GetPrivateProfileString("group",section,"",buf,255,FileName);

		strt=GetIniSetting(inf,"group",section,"");

	//	if(strlen(buf)==0)

		if(strt.empty())

		{

			strError.Format("Get group section:%s failed while update group config",section);

			throw MSException((LPCSTR)strError);

		}



		GroupsItem *pItem=new GroupsItem;

		if(!m_lc.CreateSingleGroup(pItem,strt.c_str()))

		{

			delete pItem;

			strError.Format("Create group failed while update config",strt.c_str());

			throw MSException(strError);

		}



		CGroupsItemList &GroupsList=m_pGroups->GetGroupsList();



		if(!isEdit)

			GroupsList.push_back(pItem);

//			GroupsList.AddTail(pItem);

		else

		{

//			POSITION pos=GroupsList.GetHeadPosition();

//			POSITION tempos=pos;

			CGroupsItemList::iterator it,tempit;

			it=GroupsList.begin();

			tempit=it;		

			while(it!=GroupsList.end())

			{

		//		tempos=pos;

				tempit=it;

		//		GroupsItem *pGI=GroupsList.GetNext(pos);

				GroupsItem *pGI=*it++;

				if(stricmp(pGI->GetGroupID(),strt.c_str())==0)

				{

					//GroupsList.RemoveAt(tempos);

					GroupsList.erase(tempit);

					delete pGI;

					break;

				}

				

			}

	//		GroupsList.AddTail(pItem);

			GroupsList.push_back(pItem);

			AddMonitor(FileName,TRUE);

		}

		



	}



}



void MonitorSchMain::AddEntity(LPCSTR FileName, BOOL isEdit)

{

	puts("in addentity");



	INIFile inf=LoadIni(FileName);



//	int count=::GetPrivateProfileInt("entity","count",1,FileName);

	int count=atoi(GetIniSetting(inf,"entity","count","1").c_str());



//	char buf[256]={0};

	char section[100]={0};

	CString strError="";

	std::string strt;

	for(int i=0;i<count;i++)

	{

//		memset(buf,0,256);

		sprintf(section,"list%d",i);

		//GetPrivateProfileString("entity",section,"",buf,255,FileName);

		strt=GetIniSetting(inf,"entity",section,"");



//		if(strlen(buf)==0)

        if(strt.empty())

		{

			strError.Format("Get entity section:%s failed while update entity config",section);

			throw MSException((LPCSTR)strError);

		}



		Entity *pItem=new Entity;

		if(!m_lc.CreateSingleEntity(pItem,strt.c_str()))

		{

			delete pItem;

			strError.Format("Create entity failed while update config",strt.c_str());

			throw MSException(strError);

		}



		CEntityList &EntityList=m_pGroups->GetEntityList();



		if(!isEdit)

			EntityList.push_back(pItem);

//			EntityList.AddTail(pItem);

		else

		{

//			POSITION pos=EntityList.GetHeadPosition();

//			POSITION tempos=pos;

			CEntityList::iterator it,tempit;

			it=EntityList.begin();

			tempit=it;

			while(it!=EntityList.end())

			{

		//		tempos=pos;

				tempit=it;

		//		Entity *pE=EntityList.GetNext(pos);

				Entity *pE=*it++;

				if(stricmp(pE->GetEntityID(),strt.c_str())==0)

				{

			//		EntityList.RemoveAt(tempos);

					EntityList.erase(tempit);

					delete pE;

					break;

				}

				

			}

	//		EntityList.AddTail(pItem);

			EntityList.push_back(pItem);



			AddMonitor(FileName,TRUE);

		}



		pItem=NULL;

		



	}







}



BOOL MonitorSchMain::AddMonitor(LPCSTR FileName, BOOL isEdit)

{

	puts("in addmonitor");



	INIFile inf=LoadIni(FileName);



//	int count=::GetPrivateProfileInt("monitor","count",1,FileName);

	int count=atoi(GetIniSetting(inf,"monitor","count","1").c_str());



//	char buf[256]={0};

	char section[100]={0};

	CString strError="";

	std::string strt;

	for(int i=0;i<count;i++)

	{

//		memset(buf,0,256);

		sprintf(section,"list%d",i);

	//	GetPrivateProfileString("monitor",section,"",buf,255,FileName);

		strt=GetIniSetting(inf,"monitor",section,"");



	//	if(strlen(buf)==0)

		if(strt.empty())

		{

		//	strError.Format("Get monitor section:%s failed while update monitor config",section);

		//	throw MSException((LPCSTR)strError);

			continue;

		}



		Monitor *pItem=new Monitor;

		if(!m_lc.CreateSingleMonitor(pItem,strt.c_str()))

		{

			delete pItem;

			strError.Format("Create monitor failed while update config",strt.c_str());

			throw MSException(strError);

		}





		if(!isEdit)

		{

			CTime ct=CTime::GetCurrentTimeEx();

			CTimeSpan ts=CTimeSpan(0,0,0,60-ct.GetSecond());

			ct+=ts;

			pItem->SetNextRunTime(ct);

		//	m_MonitorList.AddTail(pItem);

			m_MonitorList.push_back(pItem);

		}

		else

		{

		//	POSITION pos=m_MonitorList.GetHeadPosition();

			CMonitorList::iterator it;

			it=m_MonitorList.begin();

			while(it!=m_MonitorList.end())

			{

			//	Monitor *pM=m_MonitorList.GetNext(pos);

				Monitor *pM=*it++;

				if(stricmp(pM->GetMonitorID(),strt.c_str())==0)

				{

					puts("Get edit monitor");
					
					if(pItem->GetFrequency()==pM->GetFrequency())
						pItem->SetNextRunTime(pM->GetNextRunTime());
					else{
						CTime ct=CTime::GetCurrentTimeEx();
	
						CTimeSpan ts=CTimeSpan(0,0,0,60-ct.GetSecond());
			
						ct+=ts;
			
						pItem->SetNextRunTime(ct);
			
					//	m_MonitorList.AddTail(pItem);
			
						m_MonitorList.push_back(pItem);
					
					}

					pM->isDelete=TRUE;

		        //	m_MonitorList.AddTail(pItem);

					m_MonitorList.push_back(pItem);

					break;

				}

				

			}

		}



		pItem=NULL;

		



	}



	return TRUE;



}



void MonitorSchMain::DeleteMonitor(LPCSTR FileName)

{

	puts("in delete Monitor");

	INIFile inf=LoadIni(FileName);

//	int count=::GetPrivateProfileInt("monitor","count",1,FileName);

	int count=atoi(GetIniSetting(inf,"monitor","count","1").c_str());


//	char buf[256]={0};

	char section[100]={0};

	CString strError="";

	std::string strt;

	for(int i=0;i<count;i++)

	{

//		memset(buf,0,256);

		sprintf(section,"list%d",i);

	//	GetPrivateProfileString("monitor",section,"",buf,255,FileName);

		strt=GetIniSetting(inf,"monitor",section,"");

	//	if(strlen(buf)==0)

		if(strt.empty())

		{

			//			strError.Format("Get monitor section:%s failed while update monitor config",section);

			//			throw MSException((LPCSTR)strError);

			continue;

		}


//		POSITION pos=m_MonitorList.GetHeadPosition();

		CMonitorList::iterator it;

		it=m_MonitorList.begin();

		while(it!=m_MonitorList.end())
		{

//			Monitor *pM=m_MonitorList.GetNext(pos);

			Monitor *pM=*it++;

			if(stricmp(pM->GetMonitorID(),strt.c_str())==0)

			{

				pM->isDelete=TRUE;

				break;

			}
			

		}

	}

	

	return ;


}
bool MonitorSchMain::ParseOpt(string opt,string &name,string &type)
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
bool MonitorSchMain::AddMonitorV70(string monitorid,bool isEdit)
{
	ost::MutexLock lock(m_MonitorListMutex);

	if(monitorid.empty())
		return false;
	CString strError="";

	puts("In Add Monitor");

	Monitor *pItem=new Monitor;
    m_lc.LoadAll();


	if(!m_lc.CreateSingleMonitor(pItem,monitorid.c_str()))
	{
		puts("Create monitor failed");

		delete pItem;
		strError.Format("Create monitor failed while update config",monitorid.c_str());
		throw MSException(strError);
	}


	if(!isEdit)
	{

		CTime ct=CTime::GetCurrentTimeEx();
		CTimeSpan ts=CTimeSpan(0,0,0,60-ct.GetSecond());
		ct+=ts;
		pItem->SetNextRunTime(ct);
		DeleteMonitorV70(monitorid);

		m_MonitorList.push_back(pItem);

	}
	else
	{

		CMonitorList::iterator it;
		it=m_MonitorList.begin();
		while(it!=m_MonitorList.end())
		{

			Monitor *pM=*it++;
			if(stricmp(pM->GetMonitorID(),monitorid.c_str())==0)
			{
				puts("Get edit monitor");

				if(pItem->GetFrequency()==pM->GetFrequency())
					pItem->SetNextRunTime(pM->GetNextRunTime());
				else{
					CTime ct=CTime::GetCurrentTimeEx();
					CTimeSpan ts=CTimeSpan(0,0,0,60-ct.GetSecond());
					ct+=ts;
					pItem->SetNextRunTime(ct);

				}
				pItem->SetLastState(pM->GetLastState());
				InitGroupDependByMonitorChangeV70(pM,pItem,false);
				DeleteMonitorV70(monitorid);
				pM->isDelete=TRUE;
				m_MonitorList.push_back(pItem);
				break;

			}

		}
	}

	puts("Add Monitor Ok");

	return true;
}
bool MonitorSchMain::DeleteMonitorV70(string monitorid)
{
	ost::MutexLock lock(m_MonitorListMutex);

	if(monitorid.empty())
		return false;

	CMonitorList::iterator it;
	it=m_MonitorList.begin();
	while(it!=m_MonitorList.end())
	{
		Monitor *pM=*it++;

		if(stricmp(pM->GetMonitorID(),monitorid.c_str())==0)
		{
			InitGroupDependByMonitorChangeV70(pM,NULL,true);
			pM->isDelete=TRUE;
		}
	}

	return true;

}
bool MonitorSchMain::InitGroupDependByMonitorChangeV70(Monitor *oldpm,Monitor* newpm,bool isdel)
{
	ost::MutexLock lock(m_MonitorListMutex);

	CMonitorList::iterator it;
	it=m_MonitorList.begin();
	while(it!=m_MonitorList.end())
	{
		Monitor *pM=*it++;
		if(oldpm==pM->GetGroupDependMonitor())
		{
			if(!isdel)
				pM->SetGroupDependMonitor(newpm);
			else
				pM->SetGroupDependState(Monitor::groupdependnull);

		}

	}

	return true;
}
bool MonitorSchMain::AddEntityV70(string entityid,bool isEdit)
{
	if(entityid.empty())
		return false;

	puts("In Add Entity");

	CString strError="";

	Entity *pItem=new Entity;
	if(!m_lc.CreateSingleEntity(pItem,entityid.c_str()))
	{
		delete pItem;
		strError.Format("Create entity failed while update config",entityid.c_str());
		throw MSException(strError);

	}

	CEntityList &EntityList=m_pGroups->GetEntityList();

	if(!isEdit)
		EntityList.push_back(pItem);
	else
	{
		CEntityList::iterator it,tempit;
		it=EntityList.begin();
		tempit=it;
		while(it!=EntityList.end())
		{
			tempit=it;
			Entity *pE=*it++;
			if(stricmp(pE->GetEntityID(),entityid.c_str())==0)
			{
				pItem->CopyTempData(pE);
                EntityChangeV70(pE,pItem,false);
				EntityList.erase(tempit);
		//		delete pE;                          //不删除此对像,防止有线程正在用它.(会丢掉部分内存)
				break;
			}

		}

		EntityList.push_back(pItem);
		ReLoadMonitorsV70(entityid);

	}
	puts("Add Entity ok");

	return true;
}
bool MonitorSchMain::EntityChangeV70(Entity *oldE,Entity *newE,bool isdel)
{
	ost::MutexLock lock(m_MonitorListMutex);

	CMonitorList::iterator it;
	it=m_MonitorList.begin();
	while(it!=m_MonitorList.end())
	{
		Monitor *pM=*it++;
		if(pM->GetEntity()==oldE)
			pM->SetEntity(newE);
	}

	return true;



}
bool MonitorSchMain::ReLoadMonitorsV70(string parentid)
{
	ost::MutexLock lock(m_MonitorListMutex);

	std::list<string> listid;

	string monitorid="";

	CMonitorList::iterator it;
	it=m_MonitorList.begin();
	while(it!=m_MonitorList.end())
	{

		Monitor *pM=*it++;
		int pos=0;
		monitorid=pM->GetMonitorID();
		if((pos=monitorid.find(parentid))==0)
			listid.push_back(monitorid);
	}

	if(!listid.empty())
	{
		std::list<string>::iterator its;
		for(its=listid.begin();its!=listid.end();its++)
			AddMonitorV70((*its),true);
	}

	return true;
}
bool MonitorSchMain::InitMonitorsGroupDependV70(string parentid)
{
	ost::MutexLock lock(m_MonitorListMutex);

	std::list<string> listid;

	string monitorid="";

	CMonitorList::iterator it;
	it=m_MonitorList.begin();
	while(it!=m_MonitorList.end())
	{

		Monitor *pM=*it++;
		pM->SetGroupDependState(Monitor::groupdependnull);
	}

	return true;

}
bool MonitorSchMain::AddGroupV70(string groupid,bool isEdit)
{
	if(groupid.empty())
		return false;

	puts("In AddGroup");

	CString strError="";

	GroupsItem *pItem=new GroupsItem;
	if(!m_lc.CreateSingleGroup(pItem,groupid.c_str()))
	{
		delete pItem;
		strError.Format("Create group failed while update config",groupid.c_str());
		throw MSException(strError);
	}
	CGroupsItemList &GroupsList=m_pGroups->GetGroupsList();

	if(!isEdit)
		GroupsList.push_back(pItem);
	else
	{
		CGroupsItemList::iterator it,tempit;
		it=GroupsList.begin();
		tempit=it;		
		while(it!=GroupsList.end())
		{
			tempit=it;
			GroupsItem *pGI=*it++;
			if(stricmp(pGI->GetGroupID(),groupid.c_str())==0)
			{
				GroupsList.erase(tempit);
//				delete pGI;				//不删除此对像,防止有线程正在用它.(会丢掉部分内存)
				break;
			}

		}
		GroupsList.push_back(pItem);
		InitMonitorsGroupDependV70(groupid);
	}

	puts("Add group ok");

	return true;

}

bool MonitorSchMain::ReLoadTaskV70()
{
	ost::MutexLock lock(m_TaskMutex);

	TASKMAP::iterator it;
	while(m_TaskMap.findnext(it))
	{
		Task*pt=(*it).getvalue();
		if(pt)
			delete pt;
	}
	m_TaskMap.clear();

	return m_lc.LoadTaskPlan(m_TaskMap);
    
}

bool MonitorSchMain::ProcessConfigChange(string opt,string id)
{
	puts("InProcessConfigChange");

	if(opt.empty()||id.empty())
		return false;
	string name;
	string type;
	if(!ParseOpt(opt,name,type))
		return false;

	m_lc.ClearBuffer();


	if(name.compare("MONITOR")==0)
	{
		if(type.compare("ADDNEW")==0)
			AddMonitorV70(id,false);
		else if(type.compare("UPDATE")==0)
			AddMonitorV70(id,true);
		else if(type.compare("DELETE")==0)
			DeleteMonitorV70(id);
		else
			return false;
	}else if(name.compare("ENTITY")==0)
	{
		if(type.compare("ADDNEW")==0)
			AddEntityV70(id,false);
		else if(type.compare("UPDATE")==0)
			AddEntityV70(id,true);
		else
			return false;

	}else if(name.compare("GROUP")==0)
	{
		if(type.compare("ADDNEW")==0)
			AddGroupV70(id,false);
		else if(type.compare("UPDATE")==0)
			AddGroupV70(id,true);
		else
			return false;

	}else if(name.compare("TASK")==0)
	{
		ReLoadTaskV70();
	}else
		return false;

	return true;

}

bool MonitorSchMain::LoadPreLibrary(void)
{
	if(m_pOption->m_PreLoadLibrary.empty())
		return true;


	char filepath[1024]={0};
    
	int pos=m_pOption->m_PreLoadLibrary.find(',');
	string str;
	if(pos<0)
		return true;
	int tpos=0;
	while(pos>=0)
	{
		str=m_pOption->m_PreLoadLibrary.substr(tpos,pos-tpos);
		tpos=pos+1;
		pos=m_pOption->m_PreLoadLibrary.find(',',tpos);
		if(str.empty())
			continue;
		sprintf(filepath,"%s/fcgi-bin/%s",g_strRootPath.getText(),str.c_str());
		printf("PreLoad:%s\n",filepath);
		try{
#ifdef WIN32
			::LoadLibrary(filepath);
#else
#endif
		}catch(...)
		{
			continue;
		}
	}

	return false;
}
bool MonitorSchMain::SerialMonitorData(bool out)
{
	try{
		char filepath[1024]={0};
		sprintf(filepath,"%s/temp/MonitorScheduleSerialData.txt",g_strRootPath.getText());
		SerialData sd(filepath);
		bool bret =sd.Serial(m_MonitorList,m_pGroups,m_pSub,m_TaskMap,out)==0;
		if(m_MonitorList.empty())
			return false;

		return bret;
		
	}catch(...)
	{
		putil->ErrorLog("SerialData exception");
	}
	return true;
}
