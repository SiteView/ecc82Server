

#include "backupconfig.h"
#include <svdbtype.h>
#include <util.h>
#include "QueryData.h"

CBackupConfig::CBackupConfig(SvdbMain * pMain)
{
	m_pMain= pMain;
	m_exitAnyway= false;

	m_monitor_need= true;
	m_entity_need= true;
	m_group_need= true;
	m_svse_need= true;

}

CBackupConfig::~CBackupConfig()
{

}


void CBackupConfig::toExit()
{
	m_exitAnyway= true;
	this->exit();
}

void CBackupConfig::run()
{
	while(true)
	{
		if(m_exitAnyway)
			break;
		SendOutConfig();
		ThreadEx::sleep(IdcUser::msBackup);	
	}
}



bool CBackupConfig::CacheConfig(string type, string id)
{
	try{
		if(m_exitAnyway || !IdcUser::ConfigAutoBackup || !IdcUser::IsAnLocalSEId(id))
			return true;

		if(type.compare("MONITOR")==0)
		{
			if(m_monitor_need)
				return true;
			else
			{
				ost::MutexLock lock(m_config_Lock);
				m_monitor_need= true;
				m_entity_need= true;
				m_group_need= true;
				m_svse_need= true;
			}
		}

		if(type.compare("ENTITY")==0)
		{
			if(m_entity_need)
				return true;
			else
			{
				ost::MutexLock lock(m_config_Lock);
				m_entity_need= true;
				m_group_need= true;
				m_svse_need= true;
			}
		}

		if(type.compare("GROUP")==0)
		{
			if(m_group_need)
				return true;
			else
			{
				ost::MutexLock lock(m_config_Lock);
				m_group_need= true;
				m_svse_need= true;
			}
		}

		if(type.compare("SVSE")==0)
		{
			if(m_svse_need)
				return true;
			else
			{
				ost::MutexLock lock(m_config_Lock);
				m_svse_need= true;
			}
		}
	}
	catch(...)
	{
		printf(" Exception in CBackupConfig::CacheConfig ! ");
		return false;
	}

	return true;
}


bool CBackupConfig::UpdataConfig(const char *data,S_UINT datalen)
{
	if(m_exitAnyway || !IdcUser::AcceptConfigIncoming)
		return false;

	try{
		std::list<SingelRecord> listrcd;
		if( !CreateMassRecordListByRawData(listrcd,data,datalen) )
			return false;
		for(std::list<SingelRecord>::iterator it=listrcd.begin(); it!=listrcd.end(); ++it)
		{
			bool upret(false);
			if(it->monitorid.find("monitor_")==0)
			{
				string str= it->monitorid.substr(8);
				upret= m_pMain->m_pMonitor->UpdateConfig(str, it->data, it->datalen);
			}
			if(it->monitorid.find("entity_")==0)
			{
				string str= it->monitorid.substr(7);
				upret= m_pMain->m_pEntity->UpdateConfig(str, it->data, it->datalen);
			}
			if(it->monitorid.find("group_")==0)
			{
				string str= it->monitorid.substr(6);
				upret= m_pMain->m_pGroup->UpdateConfig(str, it->data, it->datalen);
			}
			if(it->monitorid.find("svse_")==0)
			{
				upret= m_pMain->m_pSVSE->UpdateConfig(it->data, it->datalen);
			}

			string ok;
			if(upret)
				ok="ok. ";
			else
				ok="failed. ";

			char text[1024]={0};
			sprintf(text,"  == Backup config in local : %s len %d, %s\n",it->monitorid.c_str(), it->datalen, ok.c_str());
			cout<<text;
			OutputDebugString(text);

			//if(!upret)
			//	return false;
		}


		return true;
	}
	catch(...)
	{
		printf(" Exception in CBackupConfig::UpdataConfig ! ");
		return false;
	}
}


bool SendSingleConfig(std::list<SingelRecord> & listrcd,string user,string addr)
{
	if(listrcd.empty()||user.empty()||addr.empty())
		return false;

	unsigned int len= GetMassRecordListRawDataSize(listrcd);
	svutil::buffer buf;
	if(!buf.checksize(len))
		return false;

	const char *data= GetMassRecordListRawData(listrcd,buf,len); 
	if(data==NULL)
		return false;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_SVSE;
	strcpy(query.idcuser,user.c_str());
	query.querytype= QUERY_BACKUPCONFIG;
	query.datalen=len;

	QueryData qd;
	char *pdata=NULL;
	SVDBRESULT ret={0};

	return qd.UpdateWithTimeOut(&query,data,len,&ret,addr);
}

bool SendConfig(std::list<SingelRecord> & listrcd, string type)
{
	if(listrcd.empty())
		return true;

	bool ret= SendSingleConfig(listrcd,"default",IdcUser::CenterAdress);
	for(std::list<SingelRecord>::iterator it=listrcd.begin();  it!=listrcd.end(); ++it)
		if( (it->data)!=NULL )
			delete [] it->data;

	string show;
	if(ret)
		show= "  == Succeeded to send out backup "+ type + "_" + IdcUser::GetLocalSEIdStr() + "\n";
	else
	{
		show= "  ** Tried to send out backup "+ type + "_" + IdcUser::GetLocalSEIdStr() + "  f .\n";
		return ret;
	}

	cout<<show.c_str();
	OutputDebugString(show.c_str());
	ThreadEx::sleep(50);	
	return ret;
}

bool CBackupConfig::SendOutConfig()
{
	try{
		if(m_exitAnyway)
			return true;

		if(IdcUser::RecordsAutoBackup || IdcUser::ConfigAutoBackup)
		{
			if(!m_monitor_need && !m_entity_need && !m_group_need && !m_svse_need)
				return true;
			m_pMain->m_pMonitor->ReSetBackupId();
		}

		if(!IdcUser::ConfigAutoBackup)
			return true;
		if(!m_monitor_need && !m_entity_need && !m_group_need && !m_svse_need)
			return true;

		if(m_monitor_need)
		{
			std::list<SingelRecord> listrcd;
			if( m_pMain->m_pMonitor->GetBackupData(listrcd) )
			{
				if( SendConfig(listrcd, "monitor") )
				{
					ost::MutexLock lock(m_config_Lock);
					m_monitor_need= false;
				}
				else
					return false;
			}
			else
			{
				string show("  ** Failed to get backup data of monitor. \n");
				cout<<show.c_str();
				OutputDebugString(show.c_str());
			}
		}
		if(m_entity_need)
		{
			std::list<SingelRecord> listrcd;
			if( m_pMain->m_pEntity->GetBackupData(listrcd) )
			{
				if( SendConfig(listrcd, "entity") )
				{
					ost::MutexLock lock(m_config_Lock);
					m_entity_need= false;
				}
				else
					return false;
			}
			else
			{
				string show("  ** Failed to get backup data of entity. \n");
				cout<<show.c_str();
				OutputDebugString(show.c_str());
			}
		}
		if(m_group_need)
		{
			std::list<SingelRecord> listrcd;
			if( m_pMain->m_pGroup->GetBackupData(listrcd) )
			{
				if( SendConfig(listrcd, "group") )
				{
					ost::MutexLock lock(m_config_Lock);
					m_group_need= false;
				}
				else
					return false;
			}
			else
			{
				string show("  ** Failed to get backup data of group. \n");
				cout<<show.c_str();
				OutputDebugString(show.c_str());
			}
		}
		if(m_svse_need)
		{
			std::list<SingelRecord> listrcd;
			if( m_pMain->m_pSVSE->GetBackupData(listrcd) )
			{
				if( SendConfig(listrcd, "svse") )
				{
					ost::MutexLock lock(m_config_Lock);
					m_svse_need= false;
				}
				else
					return false;
			}
			else
			{
				string show("  ** Failed to get backup data of svse. \n");
				cout<<show.c_str();
				OutputDebugString(show.c_str());
			}
		}		
		return true;
	}
	catch(...)
	{
		printf(" Exception in CBackupConfig::SendOutConfig ! ");
		return false;
	}
}











