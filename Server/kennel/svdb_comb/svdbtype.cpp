

#include "svdbtype.h"

std::set<std::string> IdcUser::Users;
std::string IdcUser::RootPath("");
std::string IdcUser::svdbHostAddr("");
std::string IdcUser::cgiVersion("");
std::string IdcUser::StrDisable("Monitor is disabled");
std::string IdcUser::StrTempDisable("disable temporarily, start time:%s,end time:%s");

bool IdcUser::PreCreateNnmEntityParent(true);
std::string IdcUser::nnmEntityParentKey("CombinedEntityTypeMarker");
std::map<string,string> IdcUser::nnmEntityParentValue;

std::map<string,string> IdcUser::nnmEntityParentId;
bool IdcUser::nnmEntityParentOk(false);
bool IdcUser::DisableEntityCombine(false);

std::string IdcUser::CenterAdress;
bool IdcUser::SELocked(false);
std::string IdcUser::CacheQueueName("SiteView70_CentralBackupCache");
int IdcUser::msBackup(5000);
bool IdcUser::RecordsAutoBackup(false);
bool IdcUser::ConfigAutoBackup(false);
bool IdcUser::AcceptConfigIncoming(false);
bool IdcUser::FullTeleBackup(false);
std::set<int> IdcUser::set_LocalSEId;
std::set<int> IdcUser::set_InitLoadSEId;
std::set<std::string> IdcUser::set_backupId;
ost::Mutex IdcUser::m_LockBackupId;

bool IdcUser::EnableConfigDB(false);
bool IdcUser::EnableIdc(false);
bool IdcUser::AutoResolveIDS(false);
bool IdcUser::BoolToExit(false);
int IdcUser::SocketTreadPlusCount(0);
int IdcUser::PipeTreadPlusCount(0);
void* IdcUser::m_pResource(NULL);
void* IdcUser::m_pLanguage(NULL);

#include "libutil/Time.h"
#include "StringMap.h" 
#include "Resource.h"
#include "ResourcePool.h"
#include "Util.h"

std::string getProcessId()
{
	DWORD pid= GetCurrentProcessId();
	char buf[32]={0};
	sprintf(buf, "%d", pid);
	return buf;
}

std::string IdcUser::ProcessID(getProcessId());

void IdcUser::PutLocalSEId(std::string str)
{
	try{
		string dvalue=str;
		for(string::size_type index=0; index !=dvalue.size(); ++index)
			if(dvalue[index]==',') dvalue[index]='\n';

		std::istringstream input_temp(dvalue);
		string tempkey;
		while(  std::getline(input_temp,tempkey)  )
		{
			tempkey=TrimSpace(tempkey);
			set_LocalSEId.insert(atoi(tempkey.c_str()));
		}
	}
	catch(...)
	{
		printf("Exception to PutLocalSEId.\n");
	}
}

bool IdcUser::IsAnLocalSEId(int index)
{
	std::set<int>::iterator it=set_LocalSEId.find(index);
	if(it==set_LocalSEId.end())
		return false;
	else
		return true;
}

bool IdcUser::IsAnLocalSEId(std::string str)
{
	return IsAnLocalSEId(atoi(GetTopID(str.c_str()).getword()));
}

std::string IdcUser::GetLocalSEIdStr()
{
	string str;
	for(std::set<int>::iterator it=set_LocalSEId.begin(); it!=set_LocalSEId.end(); ++it)
	{
		char cstr[32]= {0};
		sprintf(cstr,"%d,",*it);
		str+= cstr;
	}
	return str;
}

std::set<int> IdcUser::GetLocalSEId()
{
	return set_LocalSEId;
}

std::string IdcUser::GetFirstSEId()
{
	std::set<int>::iterator it=set_InitLoadSEId.begin();
	if(it==set_InitLoadSEId.end())
		return "1";

	char cstr[32]= {0};
	sprintf(cstr,"%d",*it);
	return cstr;
}

S_UINT getInitialTime(void)
{
	S_UINT ret(11);
	try{
		time_t now_time=time(NULL);
		ret= (S_UINT)now_time;
	}
	catch(...)
	{
		printf("Exception to getInitialTime.");
	}
	return ret;
}

S_UINT IdcUser::initial_time(getInitialTime());

std::string IdcUser::CreatChineseIDS(const std::string key, const std::string value, void *m_pLanguage)
{
	if( key==("sv_label")   || key==("sv_description")   || key==("sv_helptext") 
		||key==("sv_tip") ||  key.find("sv_itemlabel")==0 )
	{
		if(m_pLanguage==NULL)
			return value;

		StringMap & lang= ((Resource *)m_pLanguage)->GetResourceDict();
		if( lang.size()==0 )
			return value;

		//得到新的ids
		StringMap::iterator langit;
		while(lang.findnext(langit))
		{
			string langvalue= (*langit).getvalue().getword();
			if(langvalue.compare(value)==0)
				return (*langit).getkey().getword();
		}

		ResourcePool * temp_pResource=NULL;
		if(IdcUser::m_pResource!=NULL && IdcUser::m_pResource!=NULL)
		{
			temp_pResource= (ResourcePool *)IdcUser::m_pResource;
			std::map<string,string>::iterator mit;
			mit= (temp_pResource->newids).find(value);
			if(mit!=(temp_pResource->newids).end())
				return mit->second;
		}

		int newnum=2000;
		StringMap::iterator it;
		while(lang.findnext(it))
		{
			string strids= (*it).getkey().getword();
			if( strids.find("IDS_")== 0 )
			{
				string rname= strids;
				rname.erase(0,4);
				int tempnum= atoi(rname.c_str()) ; 
				if( tempnum>newnum )
					newnum= tempnum;
			}
		}
		if(IdcUser::m_pResource!=NULL && IdcUser::m_pResource!=NULL)
		{
			temp_pResource= (ResourcePool *)IdcUser::m_pResource;
			std::map<string,string>::iterator mit;
			for(mit=(temp_pResource->newids).begin(); mit!=(temp_pResource->newids).end(); ++mit)
			{
				string strids= mit->first;
				if( strids.find("IDS_")== 0 )
				{
					string rname= strids;
					rname.erase(0,4);
					int tempnum= atoi(rname.c_str()) ; 
					if( tempnum>newnum )
						newnum= tempnum;
				}
			}
		}
		newnum++;

		static unsigned int seed2=0;
		seed2 +=1111;
		for(string::size_type index=0; index<=value.size()-1; ++index)
			seed2 +=(unsigned char)value[index];

		TTime temptime= TTime::GetCurrentTimeEx();
		srand((unsigned int)(temptime.GetTime()) + seed2);
		int tempc= rand()%1000;

		unsigned int seed=GetTickCount();
		srand( seed + seed2 );
		int tempk= rand()%1000;

		char snewnum[128];
		sprintf(snewnum,"IDS_%d_%d%d",newnum,tempc,tempk);
		string retkey= snewnum;

		cout<<"Creat IDS: "<<retkey.c_str()<<" = "<<value.c_str()<<endl;

		if(temp_pResource!=NULL)
			temp_pResource->InsertIntoNewIds(retkey, value);
		return retkey;
	}
	else
		return value;
}


bool IdcUser::WillTeleBackup(const std::string id)
{
	if(IdcUser::FullTeleBackup)
		return true;
	if(id.empty())
		return false;

	ost::MutexLock lock(m_LockBackupId);
	std::set<std::string>::iterator it=set_backupId.find(id);
	if(it== set_backupId.end())
		return false;
	else
		return true;
}

bool IdcUser::SetTeleBackupId(std::set<std::string> newid)
{
	ost::MutexLock lock(m_LockBackupId);
	newid.swap(set_backupId);
	return true;
}

