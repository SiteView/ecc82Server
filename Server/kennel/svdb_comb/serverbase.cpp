


#include "serverbase.h"
#include <cc++/file.h>
#include "util.h"
#include "somefunc.h"



CConfig::CConfig(string tname)
{
	mf_tablename= tname;
}

CConfig::CConfig(string tname, string ver)
{
	mf_tablename= tname;
	mf_currentver= ver;
}

CConfig::~CConfig(void)
{

}


S_UINT CConfig::GetRawDataSize()
{
	S_UINT len=0;

	try{
		std::list<SingelRecord> listrcd;

		SingelRecord r1;
		r1.datalen= GetForestMapRawDataSize(mf_values) ;
		r1.data= NULL; 
		r1.monitorid= "mf_values";
		listrcd.push_back(r1);

		SingelRecord r2;
		r2.datalen= GetForestMapRawDataSize(mf_advance) ;
		r2.data= NULL; 
		r2.monitorid= "mf_advance";
		listrcd.push_back(r2);

		len= GetMassRecordListRawDataSize(listrcd);
	}
	catch(...)
	{
		printf(" Exception in CConfig::GetRawDataSize. ");
		len= 0;
	}
	return len;
}


char*  CConfig::GetRawData(char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return NULL;

	try{
		S_UINT totalsize= GetRawDataSize();
		if(bufsize<totalsize)
			return NULL;

		S_UINT len=0,tlen=sizeof(S_UINT);
		char *pt=lpbuf;
		char *data(NULL);

		memmove(pt,&totalsize,tlen);
		pt+=tlen;

		len=2;
		memmove(pt,&len,tlen);
		pt+=tlen;

		data= pt;
		S_UINT nlen=0;
		std::list<SingelRecord> listrcd;


		SingelRecord r1;
		nlen=  GetForestMapRawDataSize(mf_values) ;
		r1.datalen= nlen;
		r1.monitorid= "mf_values";

		data+= strlen(r1.monitorid.c_str()) + 1;
		data+= tlen;
		r1.data= GetForestMapRawData(mf_values, data, nlen) ;

		listrcd.push_back(r1);
		data+= nlen;


		SingelRecord r2;
		nlen=  GetForestMapRawDataSize(mf_advance) ;
		r2.datalen= nlen;
		r2.monitorid= "mf_advance";

		data+= strlen(r2.monitorid.c_str()) + 1;
		data+= tlen;
		r2.data= GetForestMapRawData(mf_advance, data, nlen) ;

		listrcd.push_back(r2);
		data+= nlen;


		for(std::list<SingelRecord>::iterator it=listrcd.begin();it!=listrcd.end();it++)
		{
			len=strlen((it->monitorid).c_str());
			strcpy(pt,(it->monitorid).c_str());
			pt+=len;
			pt[0]='\0';
			pt++;

			memmove(pt,&(it->datalen),tlen);
			pt+=tlen;

			if( (it->data)!=NULL )
				memmove(pt,it->data,it->datalen);
			pt+=it->datalen;
		}
	}catch(...)
	{
		cout<<"Exception to CConfig::GetRawData."<<endl;
		return NULL;
	}

	return lpbuf;
}


bool   CConfig::CreateByRawData(const char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return false;

	if(bufsize<sizeof(S_UINT))
		return false;
		
	S_UINT len=0,tlen=sizeof(S_UINT);

	try{
		mf_values.clear();
		mf_advance.clear();

		const char *pend=lpbuf+bufsize;
		const char *pt=lpbuf;
		memmove(&len,pt,tlen);
		pt+=tlen;
		if(pt>pend)
			return false;

		if(len!=bufsize)
		{
			puts("Raw data of CConfig error");
			return false;
		}

		S_UINT count;
		memmove(&count,pt,tlen);
		pt+=tlen;


		std::string str;
		S_UINT datalen(0);
		const char * data(NULL);

		for(S_UINT i=0;i<count;i++)
		{
			str=pt;
			pt+=strlen(str.c_str())+1;
			if(pt>pend)
				return true;

			memmove(&datalen,pt,tlen);
			pt+=tlen;
			if(pt>pend)
				return true;

			data=pt;

			pt+=datalen;
			if(pt>pend)
				return true;

			if(i==0)
				CreateForestMapByRawData(mf_values, data, datalen);
			
			if(i==1)
				CreateForestMapByRawData(mf_advance, data, datalen);
		}
	}catch(...)
	{
		cout<<"Exception to CConfig::CreateByRawData ."<<endl;
		return false;
	}  
	return true;
}


bool	CConfig::CheckClientVerIsLatest(string clientver, string & server_version, string & estr)
{
	int inputver= atoi(clientver.c_str());
	if( !mf_currentver.empty() )
	{
		server_version= mf_currentver;
		int sint= atoi(mf_currentver.c_str());
		if(inputver==sint)
			return true;
		else
		{
			cout<<"          Table: "<<mf_tablename.c_str()<<"'s version in client/server: "<<inputver<<"/"<<sint<<endl;
			estr+= "  Table's version are different between client and server: "+clientver+"/"+mf_currentver+"  ;  ";
			return false;
		}
	}
	AnalyseDataToSetVersion(estr);
	server_version= mf_currentver;
	int sint= atoi(mf_currentver.c_str());

	if( sint>inputver )
	{
		cout<<"          Table: "<<mf_tablename.c_str()<<"'s version in client/server: "<<inputver<<"/"<<sint<<endl;
		estr+= "  Table's version are different between client and server: "+clientver+"/"+mf_currentver+"  ;  ";
		return false;
	}
	return true;	
}

bool	CConfig::AnalyseDataToSetVersion(string & estr)
{
	MapStrMap::iterator mit= mf_advance.find("history");
	if(mit==mf_advance.end())
	{
		estr+= "  Error to get version flag in server;  ";
		return false;
	}

	int maxi(0);
	string maxstr;
	int ti(0);
	string strver;
	for(NodeData::iterator nit= mit->second.begin(); nit!=mit->second.end(); ++nit)
	{
		strver= nit->first;
		ti= atoi(strver.c_str());
		if(ti>maxi)
		{
			maxi= ti;
			maxstr= strver;
		}
	}
	if(maxi==0)
	{
		estr+= "  Error to get version flag in server;  ";
		return false;
	}
	
	mf_currentver= maxstr;
	return true;
}

bool	CConfig::GetHistory(StrMap & ndata, string & estr)
{
	MapStrMap::iterator mit= mf_advance.find("history");
	if(mit==mf_advance.end())
	{
		estr+= "  " + mf_tablename + "'s history doesn't exist;  ";
		return false;
	}

	ndata= mit->second;
	return true;
}

bool	CConfig::PushVersion(string ver, string & estr)
{
	MapStrMap::iterator mit= mf_advance.find("history");
	if(mit==mf_advance.end())
	{
		NodeData ndata;
		mf_advance.insert(std::make_pair("history",ndata));
		mit= mf_advance.find("history");
	}

	int inputver= atoi(ver.c_str());
	if( mit->second.size()>50 )
	{
		map<int,string> mintver;
		int ti(0);
		std::pair< map<int,string>::iterator, bool > pr;
		list<string> todel;
		string strver;

		for(NodeData::iterator nit= mit->second.begin(); nit!=mit->second.end(); ++nit)
		{
			strver= nit->first;
			ti= atoi(strver.c_str());
			if( ti>inputver )
				todel.push_back(strver);
			else
			{
				pr= mintver.insert(std::make_pair(ti,strver));
				if(pr.second==false)
					todel.push_back(strver);
			}
		}

		ti=0;
		for(map<int,string>::reverse_iterator iit= mintver.rbegin(); iit!=mintver.rend(); ++iit)
		{
			if(++ti<=30)
				continue;
			mit->second.erase(iit->second);
		}

		for(list<string>::iterator lit=todel.begin(); lit!=todel.end(); ++lit)
			mit->second.erase(*lit);
	}

	if( !mf_currentver.empty() && !ver.empty() && atoi(ver.c_str())!=(atoi(mf_currentver.c_str())+1) )
		estr+="  Overwrote other versions of \""+mf_tablename+"\" (your version/server version: "+mf_currentver+"/"+ver+"); ";

	mf_currentver= ver;
	cout<<"         Table: "<<mf_tablename.c_str()<<"'s latest version in server: "<<mf_currentver.c_str()<<endl;
	PutReturnMapStrMap(mf_advance,"history",ver,"");
	return true;
}

bool	CConfig::ClearInvalidVersion()
{
	if(mf_currentver.empty())
		return false;

	MapStrMap::iterator mit= mf_advance.find("history");
	if(mit==mf_advance.end())
	{
		NodeData ndata;
		mf_advance.insert(std::make_pair("history",ndata));
		mit= mf_advance.find("history");
	}

	int inputver= atoi(mf_currentver.c_str());

	map<int,string> mintver;
	int ti(0);
	std::pair< map<int,string>::iterator, bool > pr;
	list<string> todel;
	string strver;

	for(NodeData::iterator nit= mit->second.begin(); nit!=mit->second.end(); ++nit)
	{
		strver= nit->first;
		ti= atoi(strver.c_str());
		if( ti>inputver )
			todel.push_back(strver);
		else
		{
			pr= mintver.insert(std::make_pair(ti,strver));
			if(pr.second==false)
				todel.push_back(strver);
		}
	}

	ti=0;
	for(map<int,string>::reverse_iterator iit= mintver.rbegin(); iit!=mintver.rend(); ++iit)
	{
		if(++ti<=30)
			continue;
		mit->second.erase(iit->second);
	}

	for(list<string>::iterator lit=todel.begin(); lit!=todel.end(); ++lit)
		mit->second.erase(*lit);

	return true;
}

void	CConfig::DisplayDataMembers()
{
	cout<<"-------  mf_values of table:"<<mf_tablename.c_str()<<"  ------"<<endl;
	OutputForestMap(mf_values);
	cout<<"-------  mf_advance of table:"<<mf_tablename.c_str()<<"  ------"<<endl;
	OutputForestMap(mf_advance);
	cout<<"-----------------  end    ---------------------\n\n\n"<<endl;
}

CTableName CConfig::GetTableName()
{
	CTableName tanme(GetValueInMapStrMap(mf_advance,"atrribute","dbname"),
					 GetValueInMapStrMap(mf_advance,"atrribute","tablename"));
	return tanme;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////




CConfigFile::CConfigFile(string dbname, string path, string historypath, string version)
{
	mf_version= atoi(version.c_str());
	if(mf_version<1000)
		mf_version=1000;

	mf_dbname= dbname;
	mf_configdbPath= path;
	mf_configdbHistoryPath= historypath;
	mf_configdbFile= mf_configdbPath + mf_dbname + ".data";
	printf("      configdb: %s\n",mf_configdbFile.c_str());
	
	mf_modified= true;
}


CConfigFile::~CConfigFile(void)
{
	CConfig * ctable(NULL);
	for( MapTableP::iterator it= mf_tables.begin(); it!=mf_tables.end(); ++it  )
	{
		ctable= it->second;
		if(ctable!=NULL)
			delete ctable;
	}
}



bool CConfigFile::UpdateCConfig(CTableName tableName, CConfig * ctable, string & estr)
{
	if(tableName.isEmpty())
	{
		estr+= " Input CTableName is emtpty;  ";
		return false;
	}

	MapTableP::iterator it= mf_tables.find(tableName.tableName);
	if (it== mf_tables.end())
	{
		estr+= "  " + tableName.tableName  +" of "+ tableName.dbName + " doesn't exist;  ";
		return false;
	}

	ost::MutexLock lock(mf_LockDataBase);
	CConfig * oldtable= it->second;
	if(oldtable!=NULL)
		delete oldtable;

	it->second= ctable;
	mf_modified= true;
	return true;
}


bool CConfigFile::UpdateBatchCConfig(map<CTableName,CConfig *> & tables, string & estr)
{
	if(tables.empty())
	{
		estr+= " Input tables to update is emtpty;  ";
		return true;
	}

	ost::MutexLock lock(mf_LockDataBase);
	set<CTableName> todel;
	map<CTableName,CConfig *>::iterator mit;
	for(mit= tables.begin(); mit!= tables.end(); ++mit)
	{
		CTableName tableName= mit->first;
		if(tableName.dbName.compare(mf_dbname)!=0)
			continue;
		if(mit->second==NULL)
			continue;

		MapTableP::iterator it= mf_tables.find(tableName.tableName);
		if (it== mf_tables.end())
		{
			estr+= "  " + tableName.tableName  +" of "+ tableName.dbName + " doesn't exist;  ";
			continue;
		}

		CConfig * oldtable= it->second;
		if(oldtable!=NULL)
			delete oldtable;

		it->second= mit->second;
		todel.insert(tableName);
	}

	for(set<CTableName>::iterator it= todel.begin(); it!= todel.end(); ++it)
		tables.erase(*it);

	mf_modified= true;
	return true;
}


CTableName CConfigFile::GetRecommendNextTableName()
{
	int maxint(0);
	string tempstr;
	for(MapTableP::iterator it= mf_tables.begin(); it!=mf_tables.end(); ++it  )
	{
		tempstr= it->first;
		int tempint= atoi(tempstr.c_str());
		if(tempint>maxint)
			maxint= tempint;
	}
	++maxint;
	char tempchar[128];
	sprintf(tempchar,"%d",maxint);
	
	CTableName tname(mf_dbname,tempchar);
	return tname;
}


CTableName CConfigFile::GetChildTableName(CTableName parentTableName)
{
	CTableName tname;
	if(parentTableName.isEmpty())
		return tname;
	string ptname= parentTableName.tableName;
	MapTableP::iterator it= mf_tables.find(ptname);
	if (it== mf_tables.end())
		return tname;

	std::string::size_type pos;	
	int maxint(0);
	string tempstr;
	string target(ptname+".");
	std::string::size_type targetsize= target.size();
	for( it= mf_tables.begin(); it!=mf_tables.end(); ++it  )
	{
		pos= (it->first).find(target);
		if( pos==0 )
		{
			tempstr= it->first;
			tempstr.erase(0,targetsize);
			int tempint= atoi(tempstr.c_str());
			if(tempint>maxint)
				maxint= tempint;
		}
	}
	++maxint;
	char tempchar[128];
	sprintf(tempchar,"%d",maxint);
	
	tname.dbName= parentTableName.dbName;
	tname.tableName= parentTableName.tableName + "." + tempchar;
	return tname;
}



bool CConfigFile::CreateNewTable(CTableName newTableName, CConfig * ctable, string & estr)
{
	if( !CheckNewTableNameAvailable(newTableName,estr) )
		return false;
	
	if(ctable==NULL)
	{
		estr+= " input table==NULL;  ";
		return false;
	}

	string tname= newTableName.tableName;
	MapTableP::iterator pit= mf_tables.find( tname );
	if( pit!= mf_tables.end() )
	{
		estr+= "  " + tname + " already exists;  ";
		return false;
	}

	ost::MutexLock lock(mf_LockDataBase);
	mf_tables.insert(std::make_pair(tname,ctable));
	mf_modified= true;
	return true;
}


bool CConfigFile::CheckNewTableNameAvailable(CTableName newTableName, string & estr)
{
	if(newTableName.isEmpty())
	{
		estr+= " Input CTableName is emtpty;  ";
		return false;
	}

	string tname= newTableName.tableName;
	if(tname.empty()||tname.find(" ")!=std::string::npos)
	{
		estr+= " Invalid table name:"+tname+" ;  ";	
		return false;
	}

	MapTableP::iterator it= mf_tables.find(tname);
	if(it!= mf_tables.end())
	{
		estr+= "  " + tname + " already exists;  ";
		return false;
	}
	return true;
}



bool CConfigFile::DeleteTable(CTableName tableName, string & estr)
{
	if(tableName.isEmpty())
	{
		estr+= " Input CTableName is emtpty;  ";
		return false;
	}

	string tname= tableName.tableName;
	MapTableP::iterator it= mf_tables.find(tname);
	if (it== mf_tables.end())
	{
		estr+= "  " + tname + " doesn't exist;  ";
		return false;
	}

	ost::MutexLock lock(mf_LockDataBase);
	CConfig * oldtable= it->second;
	if(oldtable!=NULL)
		delete oldtable;
	
	mf_tables.erase(it);
	mf_modified= true;
	return true;
}


bool CConfigFile::GetAllTableName(set<CTableName> & tnames)
{
	tnames.clear();
	for( MapTableP::iterator it= mf_tables.begin(); it!=mf_tables.end(); ++it  )
		tnames.insert(CTableName(mf_dbname,it->first));
	return true;
}


string CConfigFile::GetCurrentVersion()
{
	char tempchar[128]={0};
	sprintf(tempchar,"%d",mf_version);
	return tempchar;
}

bool CConfigFile::SetRevertNum(int historyNum, string & estr)
{
	if(historyNum>1000||historyNum<0)
	{
		estr+= " Invalid historyNum input;  ";
		return false;
	}
	char tempchar[128]={0};
	sprintf(tempchar,"%d",historyNum);

	ost::MutexLock lock(mf_LockDataBase);
	PutValueInStrMap(mf_advance, "NumOfRevertSetting", tempchar);
	mf_modified= true;
	return true;
}


int CConfigFile::GetNumOfRevertSetting()
{
	return atoi( GetValueInStrMap(mf_advance, "NumOfRevertSetting").c_str() );
}


bool CConfigFile::RevertDataBaseToVersion(string oldver, string & estr)
{
	ost::MutexLock lock(mf_LockDataBase);

	CConfigFile * ccfile1 = new CConfigFile(mf_dbname+"."+oldver, mf_configdbHistoryPath, mf_configdbHistoryPath, "1000");
	if( ccfile1 ==NULL )
	{
		estr+= " new an object,that==NULL;  ";
		return false;
	}

	string estr2;
	if( !ccfile1->LoadData(estr2) )
	{
		estr+= "  Failed to read database: "+mf_dbname+" of old version "+oldver+";  ";
		string textdel= mf_configdbHistoryPath+mf_dbname+"."+oldver+".data";
		DeleteFile(textdel.c_str());
		delete ccfile1;
		return false;
	}
	delete ccfile1;

	if( CopyFile((mf_configdbHistoryPath+mf_dbname+"."+oldver+".data").c_str(), 
		(mf_configdbPath+mf_dbname+".data").c_str(),false )==0)
	{
		estr+= "  Failed to move "+mf_dbname+ " from history path to work path .But it's current version deleted, so need you manually fix this problem; ";
		return false;
	}

	LoadData(estr);
	return true;
}


// for server
string CConfigFile::SetNewVersionAndBackupOld(string & estr)
{
	ost::MutexLock lock(mf_LockDataBase);

	string oldver= GetCurrentVersion();

	int num= GetNumOfRevertSetting();
	if(num>0)
	{
		int delnum= atoi(oldver.c_str()) - num;
		if(delnum>0)
		{
			char tmpchar[128]={0};
			sprintf(tmpchar,"%d",delnum);
			string textdel= mf_configdbHistoryPath+mf_dbname+"."+tmpchar+".data";
			DeleteFile(textdel.c_str());
		}

		if( CopyFile((mf_configdbPath+mf_dbname+".data").c_str(), 
			(mf_configdbHistoryPath+mf_dbname+"."+oldver+".data").c_str(),false )==0)
			estr+= "  Failed to backup "+mf_dbname+ "'s "+ oldver +"version ; ";
	}

	if(++mf_version<1000)
	{
		estr+= "  Fatal error, version cycle to negative number.  ";
		return "";
	}

	char tempchar[128]={0};
	sprintf(tempchar,"%d",mf_version);
	return tempchar;
}


CConfig * CConfigFile::GetTable(CTableName tableName, string & estr)
{
	if(tableName.isEmpty())
	{
		estr+= " Input CTableName is emtpty;  ";
		return false;
	}

	MapTableP::iterator it= mf_tables.find(tableName.tableName);
	if (it== mf_tables.end())
	{
		estr+= "  " + tableName.tableName  +" of "+ tableName.dbName + " doesn't exist;  ";
		return false;
	}
	
	return it->second;
}


bool CConfigFile::SaveData(string & estr)
{
	ost::MutexLock lock(mf_LockDataBase);

	if( !mf_modified )
	{
		estr+= "  "+mf_configdbFile+"  hasn't any modification and needn't SaveData;  ";
		return true;
	}


	S_UINT len= GetRawDataSize();
	try{
		ost::MappedFile file(mf_configdbFile.c_str(),ost::File::accessReadWrite,len);
		char *p=(char *)file.fetch(0,0);
		if(p)
		{
			if(GetRawData(p,len)==NULL)
			{
				estr+= " Failed to GetRawData in CConfigFile::SaveData;  ";
				return false;
			}
		}

	}catch(...)
	{
		puts("Exception in CConfigFile::SaveData");
		estr+= "  Exception in CConfigFile::SaveData;  ";
		return false;
	}

	mf_modified=false;
	return true;
}

//non-threadsafe
bool CConfigFile::LoadData(string & estr)
{
	S_UINT len=0;
	bool ret(false);
	try{

		{
			ost::ThreadFile fl(mf_configdbFile.c_str());
			len =fl.getCapacity();
		}

		if(len<=0)
		{
			estr+= "  File capacity of "+mf_configdbFile+" is zero;    ";
			return false;
		}

		ost::MappedFile file(mf_configdbFile.c_str(),0,0,ost::File::accessReadWrite);
		char*p=(char *)file.fetch(0,0);
		if(p)
		{
			ret= CreateByRawData(p,len);
			if( !ret )
			{
				estr+= "  Failed to create object of "+mf_configdbFile+" ;    ";
				return false;
			}
		}

	}catch(...)
	{
		puts("Exception in CConfigFile::LoadData");
		estr+= "  Exception in CConfigFile::LoadData;  ";
		return false;
	}
	return ret;
}


S_UINT CConfigFile::GetRawDataSize()
{
	S_UINT len=0;

	try{
		std::list<SingelRecord> listrcd;
		SingelRecord r1;
		r1.datalen= GetNodeDataRawDataSize(mf_advance) ;
		r1.data= NULL; 
		r1.monitorid= "mf_advance";
		listrcd.push_back(r1);

		for(MapTableP::iterator fit=mf_tables.begin(); fit!=mf_tables.end(); ++fit)
		{
			if(fit->second==NULL)
				continue;

			SingelRecord r1;
			r1.datalen= fit->second->GetRawDataSize() ;
			r1.data= NULL; 
			r1.monitorid= fit->first;

			listrcd.push_back(r1);
		}
		len= GetMassRecordListRawDataSize(listrcd);
	}
	catch(...)
	{
		printf(" Exception in CConfigFile::GetRawDataSize. ");
		len= 0;
	}
	return len;
}


char*  CConfigFile::GetRawData(char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return NULL;

	try{
		S_UINT totalsize= GetRawDataSize();
		if(bufsize<totalsize)
			return NULL;

		S_UINT len=0,tlen=sizeof(S_UINT);
		char *pt=lpbuf;
		char *data(NULL);

		memmove(pt,&totalsize,tlen);
		pt+=tlen;

		len=mf_tables.size();
		memmove(pt,&len,tlen);
		pt+=tlen;

		data= pt;
		S_UINT nlen=0;
		std::list<SingelRecord> listrcd;

		SingelRecord r1;
		nlen= GetNodeDataRawDataSize(mf_advance) ;

		data+= strlen("mf_advance") + 1;
		data+= tlen;
		r1.data= GetNodeDataRawData(mf_advance, data, nlen); 
		data+= nlen;

		r1.monitorid= "mf_advance";
		r1.datalen= nlen;

		listrcd.push_back(r1);

		for(MapTableP::iterator fit=mf_tables.begin(); fit!=mf_tables.end(); ++fit)
		{			
			if(fit->second==NULL)
				continue;

			SingelRecord r1;
			nlen= fit->second->GetRawDataSize();

			data+= strlen(fit->first.c_str()) + 1;
			data+= tlen;
			r1.data= fit->second->GetRawData(data, nlen); 
			data+= nlen;

			r1.monitorid= fit->first;
			r1.datalen= nlen;

			listrcd.push_back(r1);
		}

		for(std::list<SingelRecord>::iterator it=listrcd.begin();it!=listrcd.end();it++)
		{
			len=strlen((it->monitorid).c_str());
			strcpy(pt,(it->monitorid).c_str());
			pt+=len;
			pt[0]='\0';
			pt++;

			memmove(pt,&(it->datalen),tlen);
			pt+=tlen;

			if( (it->data)!=NULL )
				memmove(pt,it->data,it->datalen);
			pt+=it->datalen;
		}
	}catch(...)
	{
		cout<<"Exception to CConfigFile::GetRawData."<<endl;
		return NULL;
	}

	return lpbuf;

}


bool   CConfigFile::CreateByRawData(const char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return false;

	if(bufsize<sizeof(S_UINT))
		return false;
		
	S_UINT len=0,tlen=sizeof(S_UINT);

	try{
		mf_advance.clear();
		for( MapTableP::iterator it= mf_tables.begin(); it!=mf_tables.end(); ++it  )
			if(it->second!=NULL)
				delete it->second;
		mf_tables.clear();

		const char *pend=lpbuf+bufsize;
		const char *pt=lpbuf;
		memmove(&len,pt,tlen);
		pt+=tlen;
		if(pt>pend)
			return false;

		if(len!=bufsize)
		{
			puts("Raw data of CConfigFile error");
			return false;
		}

		S_UINT count;
		memmove(&count,pt,tlen);
		pt+=tlen;

		std::string str;
		S_UINT datalen(0);
		const char * data(NULL);


        str=pt;
		pt+=strlen(str.c_str())+1;
		if(pt>pend)
			return false;

		memmove(&datalen,pt,tlen);
		pt+=tlen;
		if(pt>pend)
			return false;

		data=pt;

		pt+=datalen;
		if(pt>pend)
			return false;

        if( !CreateNodeDataByRawData(mf_advance, data, datalen) )
			return false;

		MapTableP::iterator pit;
		for(S_UINT i=0;i<count;i++)
		{
			str=pt;
			pt+=strlen(str.c_str())+1;
			if(pt>pend)
				return true;

			memmove(&datalen,pt,tlen);
			pt+=tlen;
			if(pt>pend)
				return true;

			data=pt;

			pt+=datalen;
			if(pt>pend)
				return true;

			CConfig * ctable= new CConfig(str,GetCurrentVersion());
			if(ctable==NULL)
				continue;

			if( !ctable->CreateByRawData( data, datalen) )
			{
				delete ctable;
				cout<<" Failed to CreateByRawData of table: "<<str.c_str()<<endl;
				continue;
			}
			ctable->ClearInvalidVersion();

			pit= mf_tables.find( str );
			if( pit== mf_tables.end() )
				mf_tables.insert(std::make_pair( str,ctable ));	
			else
			{
				delete pit->second;
				pit->second= ctable;
			}
		}
	}catch(...)
	{
		cout<<"Exception to CreateForestMapByRawData."<<endl;
		return false;
	}  
	return true;
}


void   CConfigFile::DisplayDataMembers()
{
	cout<<"------ mf_advance of database: "<<mf_dbname.c_str()<<"     ------"<<endl;
	OutputNodeData(mf_advance);
	cout<<"-----------------  end    ---------------------\n\n\n"<<endl;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



CWholeConfig::CWholeConfig(string fname, string path, string historypath)
{
	mf_configdbPath= path;
	mf_configdbIndexFile= fname; 
	mf_configdbHistoryPath= historypath;
	printf("configdb index: %s\n",mf_configdbIndexFile.c_str());
	
	mf_modified= true;
}


CWholeConfig::~CWholeConfig(void)
{
	string estr;
	SaveData(estr);

	CConfigFile * ccfile(NULL);
	for( MapCFileP::iterator it= mf_files.begin(); it!=mf_files.end(); ++it  )
	{
		ccfile= it->second;
		if(ccfile!=NULL)
			delete ccfile;
	}
}


bool CWholeConfig::CreateNewDataBase(string dbname, string message, string & estr)
{
	if( dbname.empty() || !CheckNewDataBaseNameAvailable(dbname,estr) )
		return false;

	ost::MutexLock lock(mf_LockFileNames);
	CConfigFile * ccfile= new CConfigFile(dbname, mf_configdbPath, mf_configdbHistoryPath, "1000");
	if(ccfile==NULL)
	{
		estr+= " new an object,that==NULL;  ";
		return false;
	}

	bool bload= ccfile->LoadData(estr);
	if(bload)
	{
		estr+= "  Something error, " +dbname+ " doesn't include in configdb index, but exists in " +mf_configdbPath+ " ;  ";
		delete ccfile;
		return false;
	}

	MapCFileP::iterator pit= mf_files.find( dbname );
	if( pit== mf_files.end() )
		mf_files.insert(std::make_pair(dbname, ccfile));
	else
	{
		delete pit->second;
		pit->second= ccfile;
		estr += "  Something error, why " +dbname+ "exists? ;  ";
	}

	PutValueInStrMap(mf_nameversion,dbname,"1000");
	PutReturnMapStrMap(mf_historymap,dbname,"1000",svutil::TTime::GetCurrentTimeEx().Format()+"  "+message);

	mf_modified= true;
	return true;
}


bool CWholeConfig::CheckNewDataBaseNameAvailable(string dbname, string & estr)
{
	if(dbname.empty()||dbname.find(".")!=std::string::npos||dbname.find(" ")!=std::string::npos)
	{
		estr+= " Invalid database name; ";
		return false;
	}

	MapCFileP::iterator it= mf_files.find(dbname);
	if(it!= mf_files.end())
	{
		estr+= "  " + dbname + " already exists;  ";
		return false;
	}
	return true;
}


bool CWholeConfig::DeleteDataBase(string dbname, string & estr)
{
	if( dbname.empty() )
	{
		estr+= " Invalid database name; ";
		return false;
	}

	MapCFileP::iterator it= mf_files.find(dbname);
	if (it== mf_files.end())
	{
		estr+= "  " + dbname + " doesn't exist;  ";
		return false;
	}

	ost::MutexLock lock(mf_LockFileNames);
	delete it->second;
	mf_files.erase(it);

	StrMap::iterator nit= mf_nameversion.find(dbname);	
	if( nit!=mf_nameversion.end() )
		mf_nameversion.erase(nit);

	MapStrMap::iterator mit= mf_historymap.find(dbname);
	if(mit!=mf_historymap.end())
		mf_historymap.erase(mit);

	string timetext(".DeletedAt");
	try{
		time_t now_time=time(NULL);
		char tmpchar[256];
		strftime( tmpchar, sizeof(tmpchar), ".DeletedAt%Yy%mm%dd%Hh%Mm%Ss", localtime(&now_time) ); 
		timetext= tmpchar;
	}
	catch(...)
	{
	}
	if( MoveFileEx((mf_configdbPath+dbname+".data").c_str(), 
				(mf_configdbHistoryPath+dbname+timetext+".data").c_str(),MOVEFILE_REPLACE_EXISTING )==0)
		estr+= "  Failed to move "+dbname+ " to backup path; ";

	mf_modified= true;
	return true;
}


bool CWholeConfig::GetAllDataBaseName(set<string> & dnames)
{
	dnames.clear();
	for( MapCFileP::iterator it= mf_files.begin(); it!=mf_files.end(); ++it  )
		dnames.insert(it->first);
	return true;
}

//deeply copy
bool CWholeConfig::GetVersionHistory(string dbname, StrMap & ndata,const StrMap & needs, string & estr)
{
	MapStrMap::iterator mit= mf_historymap.find(dbname);
	if(mit==mf_historymap.end())
	{
		estr+= "  " + dbname + "'s history doesn't exist;  ";
		return false;
	}

	if(needs.empty())
	{
		ndata= mit->second;
		return true;
	}

	StrMap::const_iterator sit;
	for(StrMap::iterator it= mit->second.begin(); it!= mit->second.end(); ++it)
	{
		sit= needs.find(it->first);
		if(sit==needs.end())
			continue;

		PutValueInStrMap(ndata,it->first,it->second);
	}

	return true;
}


bool CWholeConfig::PushVersion(string dbname, string ver, string message, string & estr)
{
	ost::MutexLock lock(mf_LockFileNames);

	StrMap::iterator nit= mf_nameversion.find(dbname);	
	if( nit==mf_nameversion.end() )
	{
		estr+= "  " + dbname + " doesn't exist in mf_nameversion;  ";
		return false;
	}
	else
		nit->second= ver;	

	MapStrMap::iterator mit= mf_historymap.find(dbname);
	if(mit==mf_historymap.end())
	{
		StrMap ndata;
		mf_historymap.insert(std::make_pair(dbname, ndata));
		mit= mf_historymap.find(dbname);
	}

	int inputver= atoi(ver.c_str());
	if( mit->second.size()>1300 )
	{
		map<int,string> mintver;
		int ti(0);
		std::pair< map<int,string>::iterator, bool > pr;
		list<string> todel;
		string strver;

		for(NodeData::iterator nit= mit->second.begin(); nit!=mit->second.end(); ++nit)
		{
			strver= nit->first;
			ti= atoi(strver.c_str());
			if( ti>inputver )
				todel.push_back(strver);
			else
			{
				pr= mintver.insert(std::make_pair(ti,strver));
				if(pr.second==false)
					todel.push_back(strver);
			}
		}

		ti=0;
		for(map<int,string>::reverse_iterator iit= mintver.rbegin(); iit!=mintver.rend(); ++iit)
		{
			if(++ti<=1000)
				continue;
			mit->second.erase(iit->second);
		}

		for(list<string>::iterator lit=todel.begin(); lit!=todel.end(); ++lit)
			mit->second.erase(*lit);
	}

	nit= mit->second.find(ver);	
	string text= svutil::TTime::GetCurrentTimeEx().Format()+"  "+message;
	if( nit==mit->second.end() )
		mit->second.insert(std::make_pair(ver,text));
	else
		nit->second= text;
	mf_modified= true;

	cout<<"         Database: "<<dbname.c_str()<<"'s latest version in server: "<<ver.c_str()<<" ,\n               message: "<<text.c_str()<<" .  "<<endl;
	return true;

}



CConfigFile * CWholeConfig::GetDataBase(string dbname, string & estr)
{
	if( dbname.empty() )
		return false;

	MapCFileP::iterator it= mf_files.find(dbname);
	if (it== mf_files.end())
	{
		estr+= "  " + dbname + " doesn't exist;  ";
		return false;
	}
	
	return  it->second;
}


bool CWholeConfig::SaveData(string & estr)
{
	ost::MutexLock lock(mf_LockFileNames);

	if( !mf_modified )
	{
		estr+= "  configdb index hasn't any modification and needn't SaveData;  ";
		return true;
	}

	std::list<SingelRecord> listrcd;
	SingelRecord r1;
	r1.datalen= GetNodeDataRawDataSize(mf_nameversion);
	r1.monitorid= "mf_nameversion";
	r1.data= NULL;
	listrcd.push_back(r1);

	SingelRecord r2;
	r2.datalen= GetForestMapRawDataSize(mf_historymap);
	r2.monitorid= "mf_historymap";
	r2.data= NULL;
	listrcd.push_back(r2);

	S_UINT len= GetMassRecordListRawDataSize(listrcd);
	try{
		ost::MappedFile file(mf_configdbIndexFile.c_str(),ost::File::accessReadWrite,len);
		char *p=(char *)file.fetch(0,0);
		if(p)
		{
			char * tdata= p;
			tdata+= sizeof(S_UINT)*3;
			tdata+= strlen(r1.monitorid.c_str())+1;
			r1.data= GetNodeDataRawData(mf_nameversion,tdata,r1.datalen);
			tdata+= r1.datalen;

			tdata+= sizeof(S_UINT);
			tdata+= strlen(r2.monitorid.c_str())+1;
			r2.data= GetForestMapRawData(mf_historymap,tdata,r2.datalen);

			if(GetMassRecordListRawData(listrcd,p,len)==NULL)
			{
				estr+= " Failed to GetRawData in CWholeConfig::SaveData;  ";
				return false;
			}
		}

	}catch(...)
	{
		puts("Exception in CWholeConfig::SaveData");
		estr += "   Exception in CWholeConfig::SaveData;   ";
		return false;
	}

	mf_modified=false;
	return true;
}



//non-threadsafe
bool CWholeConfig::LoadData(string & estr)
{
	S_UINT len=0;
	bool ret(false);
	try{

		{
			ost::ThreadFile fl(mf_configdbIndexFile.c_str());
			len =fl.getCapacity();
		}

		if(len<=0)
		{
			estr+= "  File capacity of configdb index is zero;    ";
			return false;
		}

		ost::MappedFile file(mf_configdbIndexFile.c_str(),0,0,ost::File::accessReadWrite);
		char*p=(char *)file.fetch(0,0);
		if(p)
		{
			mf_nameversion.clear();
			mf_historymap.clear();

			std::list<SingelRecord> listrcd;
			ret= CreateMassRecordListByRawData(listrcd,p,len);
			int i(0);
			for(std::list<SingelRecord>::iterator lit=listrcd.begin(); lit!=listrcd.end(); ++lit)
			{
				++i;
				if(i==1)
					CreateNodeDataByRawData(mf_nameversion, lit->data, lit->datalen);
				else if(i==2)
					CreateForestMapByRawData(mf_historymap, lit->data, lit->datalen);
				else 
				{
					ret= false;
					break;
				}
			}

			MapCFileP::iterator pit;
			string dbname;
			for(NodeData::iterator it=mf_nameversion.begin(); it!=mf_nameversion.end(); ++it)
			{
				dbname= it->first;
				CConfigFile * ccfile= new CConfigFile(dbname, mf_configdbPath, mf_configdbHistoryPath, it->second);
				if( ccfile==NULL )
					continue;

				if( !ccfile->LoadData(estr) )
					estr += "   "+dbname+" doesn't exist in " +mf_configdbPath+ ", so creating new one automatically;  ";

				pit= mf_files.find( dbname );
				if( pit== mf_files.end() )
					mf_files.insert(std::make_pair(dbname, ccfile));
				else
				{
					delete pit->second;
					pit->second= ccfile;
				}
			}
		}


	}catch(...)
	{
		puts("Exception in CWholeConfig::LoadData");
		estr += "   Exception in CWholeConfig::LoadData;   ";
		return false;
	}

	if(!ret)
		estr+= " Failed to create object in CWholeConfig::LoadData;  ";

	return ret;
}


void	CWholeConfig::DisplayDataMembers()
{
	cout<<"-----  configdbIndex: mf_nameversion  ---------"<<endl;
	OutputNodeData(mf_nameversion);
	cout<<"-----  configdbIndex: mf_historymap   ---------"<<endl;
	OutputForestMap(mf_historymap);
	cout<<"-----------------  end    ---------------------\n\n\n"<<endl;
}



