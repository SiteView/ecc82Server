

#include "svdbtype.h"

std::set<std::string> IdcUser::Users;
std::string IdcUser::RootPath("");
std::string IdcUser::svdbHostAddr("");
std::string IdcUser::cgiVersion("");
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