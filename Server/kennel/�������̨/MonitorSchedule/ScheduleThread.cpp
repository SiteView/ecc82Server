#include "ScheduleThread.h"
#include "MakeRecord.h"


extern Util *putil;
extern CString g_strRootPath;
extern string g_ServerHost;
extern CTime g_LastSchTime;

const char *str_disable="监测器被禁止";
const char *str_gddisable="监测器被组依靠禁止所依靠监测器ID:%s";
const char *str_taskplandisable="监测器被任务计划禁止,任务计划名称:%s";
const char *str_tempdisable="监测器被临时禁止,起始时间:%s,终止时间:%s";


string s_str_disable=str_disable;
string s_str_gddisable=str_gddisable;
string s_str_taskplandisable=str_taskplandisable;
string s_str_tempdisable=str_tempdisable;

void WriteLog( const char* str );

ScheduleThread::ScheduleThread():ThreadEx()
{
	m_pThreadContrl=NULL;

}

ScheduleThread::~ScheduleThread()
{

}

void ScheduleThread::run()
{
	CMonitorList &MonitorList=m_pSchMain->GetMonitorList();
	CTime curTime,endTime;
	//	POSITION pos,temppos;
	CMonitorList::iterator it,tempit;
	Monitor *pMonitor=NULL;
	CString strTemp="",
		strError="",
		strDependID="";

	if(m_pThreadContrl==NULL)
		return;

	int n=0;
	int TaskType=0;
	//bool isABS=false;
	int isABS=0; //modify by LiMing 09.8.27

	curTime=CTime::GetCurrentTimeEx();                   //锟斤拷锟?
	//::Sleep((61-curTime.GetSecond())*1000);
	//Thread::sleep((61-curTime.GetSecond())*1000);

	//	::SetThreadPriority(this->m_hThread,THREAD_PRIORITY_HIGHEST);  //锟斤拷叱痰燃

	char ebuf[1024]={0};


	try{
		/*		if(FAILED(::CoInitialize(NULL)))
		{
		strError.Format("CoInitialize failed when create thread:%s",m_ThreadName);
		throw MSException((LPCSTR)strError);
		}*/


		while(TRUE)
		{

			curTime=CTime::GetCurrentTimeEx();

			char szTemp[1024] = {0};
			sprintf( szTemp, "\n调度时间：%s", curTime.Format().c_str() );
			WriteLog(szTemp);

		//	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>Begin List :%s<<<<<<<<<<<<<<<<<<<<<<<<<<<\n",curTime.Format().c_str());
			m_pThreadContrl->PrintTaskQueueInfo();
			printf("Current time is :%s\n",curTime.Format().c_str());
			//			pos=MonitorList.GetHeadPosition();
			{
				ost::MutexLock mlock(m_pSchMain->m_MonitorListMutex);
				g_LastSchTime=curTime;

		//	printf(">>>>Enter mutexlock :%s<<<<<<<<<<<<<\n",curTime.Format().c_str());
				it=MonitorList.begin();
				n=0;
				while(it!=MonitorList.end())
				{
					//temppos=pos;
					tempit=it;
					//pMonitor=MonitorList.GetNext(pos);
					pMonitor=*it++;
					if(pMonitor!=NULL)
					{
						if(pMonitor->isDelete)
						{
							if((!pMonitor->GetRunning())&&(!pMonitor->m_isInQueue))
							{
								//  MonitorList.RemoveAt(temppos);
								delete pMonitor;
								MonitorList.erase(tempit);
//								puts("=========删除监测器========");
								continue;
							}
							continue;
						}

						TaskType=pMonitor->GetTaskType();
						/*
						if(TaskType==Task::TASK_NULL) //修改前的
						{
							m_pSchMain->CheckTask(pMonitor,curTime,isABS);
							if(isABS)
								TaskType=Task::TASK_ABSOLUTE;
							else
								TaskType=Task::TASK_RELATIVE;

							pMonitor->SetTaskType(TaskType);
						}
						*/
						if(TaskType==Task::TASK_NULL) //modify by LiMing 09.8.27
						{
							m_pSchMain->CheckTask(pMonitor,curTime,isABS);
							if(1==isABS)
								TaskType=Task::TASK_ABSOLUTE;
							else if(2==isABS)
								TaskType=Task::TASK_PERIOD;
							else if(3==isABS)
								TaskType=Task::TASK_RELATIVE;

							pMonitor->SetTaskType(TaskType);
						}                           //end modify

						if((pMonitor->GetNextRunTime()<=curTime)||(TaskType==Task::TASK_ABSOLUTE)) 
						{
							//						if(stricmp(pMonitor->GetMonitorID(),"4ebe425d-eedf-4d84-b4f6-b1a4b844d1df")!=0)
							//							continue;

							if(pMonitor->GetDisable()) //判断是否为禁用
							{
								printf("==========monitorid:%s-----disable==================\n",pMonitor->GetMonitorID());

								MakeRecord mr(ebuf,1024,Monitor::STATUS_DISABLE);
								mr.MakeError(Monitor::STATUS_DISABLE,s_str_disable);
								char *pt=ebuf;
								int dlen=mr.GetDataSize();
								pt+=dlen;
								strcpy(pt,s_str_disable.c_str());
								dlen+=(int)strlen(s_str_disable.c_str())+1;
								ebuf[dlen-1]='\0';
								putil->InsertSvdb(pMonitor->GetMonitorID(),ebuf,dlen);
								pMonitor->CalculateNextRunTime();
								pMonitor->SetLastState(Monitor::STATUS_DISABLE);

								continue;
							}
							if(pMonitor->GetTempDisableStateByTime(curTime)) //判断是否暂时禁用
							{
								printf("==========monitorid:%s-----temp disable==================\n",pMonitor->GetMonitorID());

								MakeRecord mr(ebuf,1024,Monitor::STATUS_DISABLE);
								char mes[256]={0};
								sprintf(mes,(const char *)s_str_tempdisable.c_str(),pMonitor->GetBeginTDTime().Format().c_str(),
									pMonitor->GetEndTDTime().Format().c_str());
								mr.MakeError(Monitor::STATUS_DISABLE,mes);
								char *pt=ebuf;
								int dlen=mr.GetDataSize();
								pt+=dlen;
								strcpy(pt,mes);
								dlen+=(int)strlen(mes)+1;
								ebuf[dlen-1]='\0';
								putil->InsertSvdb(pMonitor->GetMonitorID(),ebuf,dlen);
								pMonitor->CalculateNextRunTime();
								pMonitor->SetLastState(Monitor::STATUS_DISABLE);

								continue;

							}
							if(!m_pSchMain->CheckTask(pMonitor,curTime,isABS)) //检查任务计划
							{
								puts("------------------Disable by task plan------------------------");

								if(pMonitor->GetNextRunTime()<=curTime)
								{
									MakeRecord mr(ebuf,1024,Monitor::STATUS_DISABLE);
									char mes[256]={0};
									sprintf(mes,(const char *)s_str_taskplandisable.c_str(),pMonitor->GetTaskName().c_str());
									mr.MakeError(Monitor::STATUS_DISABLE,mes);
									char *pt=ebuf;
									int dlen=mr.GetDataSize();
									pt+=dlen;
									strcpy(pt,mes);
									dlen+=(int)strlen(mes)+1;
									ebuf[dlen-1]='\0';
									putil->InsertSvdb(pMonitor->GetMonitorID(),ebuf,dlen);
									pMonitor->SetLastState(Monitor::STATUS_DISABLE);

									pMonitor->CalculateNextRunTime();

								}


								continue;
							}

							if(m_pSchMain->CheckGroupDependState(pMonitor,strDependID)) //检查组依赖状态
							{
								puts("-----------------------Disable by group depend---------------------");

								MakeRecord mr(ebuf,1024,Monitor::STATUS_DISABLE);
								char mes[256]={0};
								sprintf(mes,(const char *)s_str_gddisable.c_str(),(char *)strDependID);
								mr.MakeError(Monitor::STATUS_DISABLE,mes);
								char *pt=ebuf;
								int dlen=mr.GetDataSize();
								pt+=dlen;
								strcpy(pt,mes);
								dlen+=(int)strlen(mes)+1;
								ebuf[dlen-1]='\0';
								putil->InsertSvdb(pMonitor->GetMonitorID(),ebuf,dlen);
								printf("Disable str:%s\n",mes);
								pMonitor->SetLastState(Monitor::STATUS_DISABLE);

								pMonitor->CalculateNextRunTime();

								continue;
							}


							//		puts(">>>>>>>>>>>>>>>>befor add task<<<<<<<<<<<<<<<<<<<<<<<<<<");
							if(pMonitor->CalculateNextRunTime())
								m_pThreadContrl->AddTask(pMonitor);
							n++;

						}
					}

				}
			}
			endTime=CTime::GetCurrentTimeEx();

			sprintf( szTemp, ">>>>>>>>>>>>>>>>>>>Total count is %d,time:%s<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n",n,endTime.Format().c_str());
			WriteLog(szTemp);


			if(endTime.GetMinute()==curTime.GetMinute())
				//ThreadEx::sleep((61-endTime.GetSecond())*1000);
				ThreadEx::sleep(5000);


		}
	}catch(MSException &e)
	{
		CString strError="";
		strError.Format("************Schedule thread error,Message:%s************",e.GetDescription());
		//putil->ErrorLog(strError);
		string strTemp = strError.getText();
		WriteLog( strTemp.c_str() );

		ExitProcess(2);
		//::exit(1);

		while(true)
		{
			it=MonitorList.begin();
			while(it!=MonitorList.end())
			{

				pMonitor=*it++;
				if(pMonitor->GetRunning())
					printf("Monitor ID:%s\tMonitor Class:%s\tMonitor type:%d\tDLL name:%s\n",pMonitor->GetMonitorID(),
					(char *)pMonitor->GetMonitorClass(),pMonitor->GetMonitorType(),
					(char *)pMonitor->GetLibrary());
			}
			//::Sleep(8*1000);
			Thread::sleep(8*1000);
		}


	}catch(...)
	{
		CString strError="************Schedule thread exception*****************";
		//putil->ErrorLog(strError);
		string strTemp = strError.getText();
		WriteLog( strTemp.c_str() );

		ExitProcess(2);

	}


	//::CoUninitialize();



}

BOOL ScheduleThread::Init()
{
	SetThreadName("Main dispatch thread");
	/*	if(m_hThread==NULL)
	throw MSException("Error : Create schedule thread failed");
	*/

	OBJECT robj=::LoadResource("default",g_ServerHost);
	if(robj!=INVALID_VALUE)
	{
		MAPNODE map=GetResourceNode(robj);
		if(map!=INVALID_VALUE)
		{
			string svalue="";
			if(FindNodeValue(map,"IDS_MonitorDisabled",svalue))
				s_str_disable=svalue;
			svalue="";
			if(FindNodeValue(map,"IDS_GroupDependMonitor",svalue))
				s_str_gddisable=svalue;
			svalue="";
			if(FindNodeValue(map,"IDS_DisableMonitorSchedule",svalue))
				s_str_taskplandisable=svalue;
			svalue="";
			if(FindNodeValue(map,"IDS_TempDisableMonitorTime",svalue))
				s_str_tempdisable=svalue;
		}

		::CloseResource(robj);

	}


	m_pThreadContrl=new ThreadContrl(m_pSchMain);
	if(!m_pThreadContrl->Init())
		throw MSException("Error : initialize threads contrl failed");
	if(!m_pThreadContrl->Start())
		throw MSException("Error : Start threads contrl failed");
	InitMonitorTime();



	return TRUE;
}

BOOL ScheduleThread::InitMonitorTime()
{

	CMonitorList &MonitorList=m_pSchMain->GetMonitorList();

	CTime time=CTime::GetCurrentTimeEx();
	//	POSITION pos=MonitorList.GetHeadPosition();
	//	POSITION pos2=pos;
	CTimeSpan ts,FreSpan;
	CTime temtime;
	Monitor *pMonitor=NULL;
	int nFrequency=0;
	int n=1;
	ts=CTimeSpan(0,0,1,time.GetSecond());
	//	time-=ts;

	//	int wmicount=0;

	CMonitorList::iterator it,it2;
	it=MonitorList.begin();
	it2=it;

	char szTemp[1024] = {0};

	//	while(pos)
	while(it!=MonitorList.end())
	{
		//	pMonitor=MonitorList.GetNext(pos); 
		pMonitor=*it++;
		/*		if(pMonitor->GetMonitorClass().CollateNoCase("wmi")==0)
		wmicount++;*/

		if(pMonitor->GetNextRunTime()<time)
		{
			nFrequency=pMonitor->GetFrequency();
			if(nFrequency<=0)
				continue;
			/*			ts=CTimeSpan(0,0,0,0);
			temtime+=ts;*/
			temtime=time;
			FreSpan=CTimeSpan(0,0,nFrequency,0);
			temtime-=FreSpan;
			pMonitor->CalculateNextRunTime(temtime);

			sprintf( szTemp, "第一个监测器的ID：%s,刷新时间：%s", pMonitor->GetMonitorID(), pMonitor->GetNextRunTime().Format().c_str() );
			WriteLog( szTemp );

			//pos2=pos;
			it2=it;
			n=0;
			int sec=0;
			//while(pos2)
			while(it2!=MonitorList.end())
			{
				//pMonitor=MonitorList.GetNext(pos2);
				pMonitor=*it2++;
				if(pMonitor->GetFrequency()==nFrequency)
				{
					//if(++n>nFrequency)
					//	n=1;
					++n;
					++sec;
					temtime=time;
					//ts=CTimeSpan(0,0,n%nFrequency+1,sec%60);
					ts=CTimeSpan( 0, 0, (sec/60)%(nFrequency), sec%60 );
					temtime+=ts;
					temtime-=FreSpan;
					//					printf("Freq:%d,temtime:%s,MonitorID:%s\n",nFrequency,temtime.Format("%y-%m-%d %H:%M:%S"),pMonitor->GetMonitorID());
					pMonitor->CalculateNextRunTime(temtime);

					sprintf( szTemp, "监测器的ID：%s,刷新时间：%s", pMonitor->GetMonitorID(), pMonitor->GetNextRunTime().Format().c_str() );
					WriteLog( szTemp );
				}
			}
		}
	}


	return TRUE;

}
