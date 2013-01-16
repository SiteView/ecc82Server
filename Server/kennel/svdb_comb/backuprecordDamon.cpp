


#include "backuprecordDamon.h"
#include "util.h"


CBackupRecordDamon::CBackupRecordDamon()
{
	m_exitAnyway= false;
	m_latest_time= svutil::TTime::GetCurrentTimeEx().GetTime();
}

CBackupRecordDamon::~CBackupRecordDamon()
{
	m_pBkpRcdThread= NULL;
}

void CBackupRecordDamon::setBkpRcdThread(CBackupRecord * pBkpRcdThread)
{
	m_pBkpRcdThread= pBkpRcdThread;
}

void CBackupRecordDamon::toExit()
{
	m_exitAnyway= true;
	this->exit();
	MoveOneStep(true);
}

void CBackupRecordDamon::run()
{
	m_exitAnyway= false;
	ThreadEx::sleep(5000);	
	while(true)
	{
		if(m_exitAnyway)
			break;

		try{
			MoveOneStep();
		}
		catch(...)
		{
		}
		if(m_exitAnyway)
			break;
		ThreadEx::sleep(20 * 1000);	
	}
}


bool CBackupRecordDamon::MoveOneStep(bool willExit)
{
	if(m_pBkpRcdThread == NULL)
		return true;
	
	std::map<string, time_t> data;
	time_t latest_time;
	{
	ost::MutexLock lock(m_data_Lock);
	std::map<string, time_t> tempT(m_data);
	data.swap(tempT);
	latest_time= m_latest_time;
	}

	svutil::TTime nowtime= svutil::TTime::GetCurrentTimeEx();
	if(!willExit)
	{
		bool ret= true;
		for(std::map<string, time_t>::iterator it= data.begin(); it!= data.end(); ++it)
		{
			string key= it->first;
			if(key.compare("t00")!=0)
				continue;
			time_t oldtime= it->second;
			try{
				svutil::TTime oldTTime(oldtime);
				TTimeSpan ts= nowtime- oldTTime;
				//cout<<"   "<<it->first<<" now/old: "<<nowtime.Format()<<"/"<<oldTTime.Format()<<"  TotalMinutes:"<<ts.GetTotalMinutes()<<endl;
				if( ts.GetTotalMinutes()>=5 )
				{
					ret= false;
					break;
				}
			}
			catch(...)
			{
			}
		}
		if(ret)
			return true;
	}

	string text(" ** Backup Record Damon restartting BackupRecordThread ** \n");
	if(willExit)
		text= " ** Backup Record Damon will exit ** \n";

	for(std::map<string, time_t>::iterator it= data.begin(); it!= data.end(); ++it)
	{
		time_t oldtime= it->second;
		try{
			svutil::TTime oldTTime(oldtime);
			TTimeSpan ts= nowtime- oldTTime;
			char textTs[32]={0};
			sprintf(textTs,"%d,",ts.GetTotalMinutes());
			text += "  " + it->first + ":" + textTs + oldTTime.Format();
		}
		catch(...)
		{
		}
	}
	cout<<text<<endl;
	OutputDebugString(text.c_str());
	AddToErrorLog(text);
	
	if(!willExit)
	{
		m_pBkpRcdThread->toExit();
		ThreadEx::sleep(2000);
		int startcode= m_pBkpRcdThread->start();

		char starttext[256]={0};
		sprintf(starttext," ** Damon restartting BackupRecordThread,  %d    ** \n",startcode);
		AddToErrorLog(starttext);
		OutputDebugString(starttext);
		puts(starttext);

		ThreadEx::sleep(60 * 1000);	
	}
	return false;
}

void CBackupRecordDamon::setVisit(std::string key, time_t time)
{
	ost::MutexLock lock(m_data_Lock);
	
	m_latest_time= time;
	std::map<string, time_t>::iterator it= m_data.find(key);
	if(it== m_data.end() )
		m_data.insert(std::make_pair(key, time));
	else
		it->second= time;
}


