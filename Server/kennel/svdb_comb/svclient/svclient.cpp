// svclient.cpp : 定义 DLL 应用程序的入口点。
//

//#include "stdafx.h"

#include "../svdbtype.h"
#include "util.h"
#include "stlini.h"

#include "svclient.h"
#include "connection.h"
#include "somefunc.h"
#include "clientcomm.h"
#include "tableCache.h"

unsigned short g_port=8600;



BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



CSvTable::CSvTable(void)
{
	m_modified= false;
}

CSvTable::~CSvTable(void)
{

}


bool CSvTable::IsLatestVersion(string & estr)
{
	if(m_version.empty()||atoi(m_version.c_str())==0)
	{
		estr+= " Error to get client version;  ";
		return false;
	}
	if(m_modified)
	{
		estr+= " Your local data has been modified, so returning false;  ";
		return false;
	}

	CTableName tname(GetTableName());

	MapStrMap fmap,fmap2;
	PutReturnMapStrMap(fmap,"inwhat",	"dowhat",		"IsLatestVersion");
	PutReturnMapStrMap(fmap,"inwhat",	"dbName",		tname.dbName);
	PutReturnMapStrMap(fmap,"inwhat",	"tableName",	tname.tableName);
	PutReturnMapStrMap(fmap,"inwhat",	"clientver",	m_version);

	char * retp(NULL);
	int rlen(0); 
	CConnect con;
	if( !con.ConnectDataBase(fmap,"", retp, rlen, estr) )
		estr+= "   ConnectDataBase return false;   ";

	bool bcreate= CreateForestMapByRawData(fmap2, retp, rlen);
	if(retp!=NULL)
		delete [] retp;
	if(!bcreate)
		estr+= "   Failed to create return object;   ";

	string tnameok= GetValueInMapStrMap(fmap2,"return","ClientVersionIsLatest");
	if(tnameok.compare("true")==0)
		return true;
	else
		return false;
}


CTableName CSvTable::GetTableName()
{
	CTableName tanme(GetValueInMapStrMap(m_advance,"atrribute","dbname"),
					 GetValueInMapStrMap(m_advance,"atrribute","tablename"));
	return tanme;
}

string CSvTable::GetTableDescription()
{
	return GetValueInMapStrMap(m_advance,"atrribute","tabledes");
}

bool CSvTable::SetTableDescription(string des)
{
	PutReturnMapStrMap(m_advance,"atrribute","tabledes",des);
	m_modified= true;
	return true;
}


CDefultPK CSvTable::GetRecommendNextDPK()
{
	CDefultPK cdpk("tpl","resource","sv_table");
	return cdpk;
}


bool CSvTable::CheckNewDPKAvailable(CDefultPK dpk)
{

	return true;
}

const StrMap & CSvTable::GetFields()
{
	MapStrMap::iterator mit= m_advance.find("fields");
	if(mit==m_advance.end())
	{
		m_modified= true;
		StrMap ndata;
		m_advance.insert(std::make_pair("fields", ndata));
		mit= m_advance.find("fields");
	}
	return mit->second;
}

const StrMap & CSvTable::GetRelations()
{
	MapStrMap::iterator mit= m_advance.find("relations");
	if(mit==m_advance.end())
	{
		m_modified= true;
		StrMap ndata;
		m_advance.insert(std::make_pair("relations", ndata));
		mit= m_advance.find("relations");
	}
	return mit->second;
}

int  CSvTable::GetFieldType(CDefultPK dpk, string field)
{
	return  CSvTable::STR_T;
}

bool CSvTable::GetValue(CDefultPK dpk, string field, string & value)
{
	return false;
}

bool CSvTable::GetValue(CDefultPK dpk, string field, int & value)
{
	return false;
}

bool CSvTable::GetValue(CDefultPK dpk, string field, float & value)
{
	return false;
}


bool CSvTable::GetValue(CDefultPK dpk, string field, CDefultPK & value)
{
	return false;
}


bool CSvTable::SetValue(CDefultPK dpk, string field, string value)
{
	return false;
}

bool CSvTable::SetValue(CDefultPK dpk, string field, int	 value)
{
	return false;
}

bool CSvTable::SetValue(CDefultPK dpk, string field, float  value)
{
	return false;
}


bool CSvTable::SetValue(CDefultPK dpk, string field, CDefultPK value) 
{
	
	return false;
}


bool CSvTable::IsModified()
{
	return m_modified;
}


string CSvTable::GetVersion()
{
	return m_version;
}


void CSvTable::DisplayDataMembers()
{
	cout<<"\n\n-----------  DisplayDataMembers   ---------------"<<endl;
	GetTableName().Display();
	cout<<"        ---  m_modified:"<<m_modified<<"     ---"<<endl;
	cout<<"        ---  m_version :"<<m_version.c_str()<<"  ---"<<endl;
	cout<<"-----------------  m_values    ----------------"<<endl;
	OutputForestMap(m_values);
	cout<<"-----------------  m_advance   ----------------"<<endl;
	OutputForestMap(m_advance);
	cout<<"----------  End of DisplayDataMembers  -----------\n\n\n"<<endl;
}

bool CSvTable::GetGeneralValue(CDefultPK dpk, string field, string & value)
{

	return false;
}


bool CSvTable::SetGeneralValue(CDefultPK dpk, string field, string   value)
{

	return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


CSvDataBase::CSvDataBase(string dbname)
{
	m_dbname= dbname;
}

CSvDataBase::~CSvDataBase(void)
{

}

CTableName CSvDataBase::GetRecommendNextTableName(string & estr)
{
	MapStrMap fmap,fmap2;
	PutReturnMapStrMap(fmap,"inwhat",	"dowhat",		"GetRecommendNextTableName");
	PutReturnMapStrMap(fmap,"inwhat",	"dbName",		m_dbname);

	char * retp(NULL);
	int rlen(0); 
	CConnect con;
	if( !con.ConnectDataBase(fmap,"", retp, rlen, estr) )
		estr+= "   ConnectDataBase return false;   ";

	bool bcreate= CreateForestMapByRawData(fmap2, retp, rlen);
	if(retp!=NULL)
		delete [] retp;
	if(!bcreate)
		estr+= "   Failed to create return object;   ";

	CTableName tname(GetValueInMapStrMap(fmap2,"return","dbName"),GetValueInMapStrMap(fmap2,"return","tableName"));
	return tname;
}



CTableName CSvDataBase::GetChildTableName(CTableName parentTableName,string & estr)
{
	if(parentTableName.isEmpty())
	{
		estr+= " CTableName from client is empty;   ";
		return CTableName();
	}

	MapStrMap fmap,fmap2;
	PutReturnMapStrMap(fmap,"inwhat",	"dowhat",		"GetChildTableName");
	PutReturnMapStrMap(fmap,"inwhat",	"dbName",		m_dbname);
	PutReturnMapStrMap(fmap,"inwhat",	"tableName",	parentTableName.tableName);

	char * retp(NULL);
	int rlen(0); 
	CConnect con;
	if( !con.ConnectDataBase(fmap,"", retp, rlen, estr) )
		estr+= "   ConnectDataBase return false;   ";

	bool bcreate= CreateForestMapByRawData(fmap2, retp, rlen);
	if(retp!=NULL)
		delete [] retp;
	if(!bcreate)
		estr+= "   Failed to create return object;   ";

	CTableName tname(GetValueInMapStrMap(fmap2,"return","dbName"),GetValueInMapStrMap(fmap2,"return","tableName"));
	return tname;
}



bool CSvDataBase::CreateNewTable(CTableName newTableName, string message, string & estr)
{
	if(newTableName.isEmpty())
	{
		estr+= " CTableName from client is empty;   ";
		return false;
	}
	if(message.empty())
	{
		estr+= "  Your parameter message is empty;  ";
		return false;
	}

	MapStrMap fmap;
	PutReturnMapStrMap(fmap,"inwhat",	"dowhat",		"CreateNewTable");
	PutReturnMapStrMap(fmap,"inwhat",	"dbName",		newTableName.dbName);
	PutReturnMapStrMap(fmap,"inwhat",	"tableName",	newTableName.tableName);
	PutReturnMapStrMap(fmap,"inwhat",	"message",		message);

	CSvTable * ptable= new CSvTable();
	if(ptable==NULL)
	{
		estr+= " new an object,that==NULL;  ";
		return false;
	}

	CSvTableComm * pcomm=  reinterpret_cast<CSvTableComm*>(ptable); 
	pcomm->InitializeDataMembers(newTableName,estr);

	S_UINT dlen= pcomm->GetRawDataSize();
	char * data= new char[dlen];
	if(data==NULL)
	{
		estr+= " new an object (to SubmitTable), that==NULL;  ";
		delete ptable;
		return false;
	}
	char * tdata= pcomm->GetRawData(data,dlen);
	if(tdata==NULL)
	{
		estr+= " Failed to GetRawData to SubmitTable;  ";
		delete ptable;
		delete [] data;
		return false;
	}

	std::list<SingelRecord> listrcd;
	SingelRecord r1;
	r1.data= tdata;
	r1.datalen= dlen;
	r1.monitorid= newTableName.tableName;
	listrcd.push_back(r1);

	char * retp(NULL);
	int rlen(0); 
	CConnect con;
	bool bret= con.ConnectDataBaseCommit(fmap,listrcd,"", retp, rlen, estr);
	if(retp!=NULL)
		delete [] retp;
	
	delete ptable;
	return bret;
}



bool CSvDataBase::CheckNewTableNameAvailable(CTableName newTableName, string & estr)
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

	MapStrMap fmap,fmap2;
	PutReturnMapStrMap(fmap,"inwhat",	"dowhat",		"CheckNewTableNameAvailable");
	PutReturnMapStrMap(fmap,"inwhat",	"dbName",		m_dbname);
	PutReturnMapStrMap(fmap,"inwhat",	"tableName",	newTableName.tableName);

	char * retp(NULL);
	int rlen(0); 
	CConnect con;
	if( !con.ConnectDataBase(fmap,"", retp, rlen, estr) )
		estr+= "   ConnectDataBase return false;   ";

	bool bcreate= CreateForestMapByRawData(fmap2, retp, rlen);
	if(retp!=NULL)
		delete [] retp;
	if(!bcreate)
		estr+= "   Failed to create return object;   ";

	string tnameok= GetValueInMapStrMap(fmap2,"return","NewTableNameAvailable");
	if(tnameok.compare("true")==0)
		return true;
	else
		return false;
}


bool CSvDataBase::DeleteTable(CTableName tableName, string message, string & estr)
{
	if(tableName.isEmpty())
	{
		estr+= " Input CTableName is emtpty;  ";
		return false;
	}
	if(message.empty())
	{
		estr+= "  Your parameter message is empty;  ";
		return false;
	}

	MapStrMap fmap,fmap2;
	PutReturnMapStrMap(fmap,"inwhat",	"dowhat",		"DeleteTable");
	PutReturnMapStrMap(fmap,"inwhat",	"dbName",		m_dbname);
	PutReturnMapStrMap(fmap,"inwhat",	"tableName",	tableName.tableName);
	PutReturnMapStrMap(fmap,"inwhat",	"message",		message);

	char * retp(NULL);
	int rlen(0); 
	CConnect con;
	if( !con.ConnectDataBase(fmap,"", retp, rlen, estr) )
		estr+= "   ConnectDataBase return false;   ";

	bool bcreate= CreateForestMapByRawData(fmap2, retp, rlen);
	if(retp!=NULL)
		delete [] retp;
	if(!bcreate)
		estr+= "   Failed to create return object;   ";

	string tnameok= GetValueInMapStrMap(fmap2,"return","DeleteTableDone");
	if(tnameok.compare("true")==0)
		return true;
	else
		return false;
}


bool CSvDataBase::GetAllTableName(set<CTableName> & tnames,string & estr)
{
	MapStrMap fmap,fmap2;
	PutReturnMapStrMap(fmap,"inwhat",	"dowhat",		"GetAllTableName");
	PutReturnMapStrMap(fmap,"inwhat",	"dbName",		m_dbname);

	char * retp(NULL);
	int rlen(0); 
	CConnect con;
	if( !con.ConnectDataBase(fmap,"", retp, rlen, estr) )
		estr+= "   ConnectDataBase return false;   ";

	bool bcreate= CreateForestMapByRawData(fmap2, retp, rlen);
	if(retp!=NULL)
		delete [] retp;
	if(!bcreate)
	{
		estr+= "   Failed to create return object;   ";
		return false;
	}

	MapStrMap::iterator mit= fmap2.begin();
	if(mit==fmap2.end())
	{
		estr+= "   Not any table in data base: "+m_dbname+" ;   ";
		return true;
	}
	tnames.clear();

	for(StrMap::iterator it= mit->second.begin(); it!=mit->second.end(); ++it)
	{
		CTableName tname(m_dbname,it->first);
		tnames.insert(tname);
	}
	return true;
}


bool CSvDataBase::GetTableVersionHistory(CTableName tableName, map<int,string> & verhis, string & estr)
{
	MapStrMap fmap,fmap2;
	PutReturnMapStrMap(fmap,"inwhat",	"dowhat",		"GetTableVersionHistory");
	PutReturnMapStrMap(fmap,"inwhat",	"tableName",	tableName.tableName);
	PutReturnMapStrMap(fmap,"inwhat",	"dbName",		m_dbname);

	char * retp(NULL);
	int rlen(0); 
	CConnect con;
	if( !con.ConnectDataBase(fmap,"", retp, rlen, estr) )
		estr+= "   ConnectDataBase return false;   ";

	bool bcreate= CreateForestMapByRawData(fmap2, retp, rlen);
	if(retp!=NULL)
		delete [] retp;
	if(!bcreate)
	{
		estr+= "   Failed to create return object;   ";
		return false;
	}

	MapStrMap::iterator mit= fmap2.begin();
	if(mit==fmap2.end())
	{
		estr+= "   Not any history in table: "+tableName.tableName+" / "+m_dbname+" ;   ";
		return true;
	}
	verhis.clear();

	for(StrMap::iterator it= mit->second.begin(); it!=mit->second.end(); ++it)
		verhis.insert(std::make_pair(atoi(it->first.c_str()),it->second));

	return true;
}

bool CSvDataBase::GetDBVersionHistory(map<int,string> & verhis,string & estr)
{
	MapStrMap fmap,fmap2;
	PutReturnMapStrMap(fmap,"inwhat",	"dowhat",		"GetDBVersionHistory");
	PutReturnMapStrMap(fmap,"inwhat",	"dbName",		m_dbname);

	char * retp(NULL);
	int rlen(0); 
	CConnect con;
	if( !con.ConnectDataBase(fmap,"", retp, rlen, estr) )
		estr+= "   ConnectDataBase return false;   ";

	bool bcreate= CreateForestMapByRawData(fmap2, retp, rlen);
	if(retp!=NULL)
		delete [] retp;
	if(!bcreate)
	{
		estr+= "   Failed to create return object;   ";
		return false;
	}

	MapStrMap::iterator mit= fmap2.begin();
	if(mit==fmap2.end())
	{
		estr+= "   Not any history in database: "+m_dbname+" ;   ";
		return true;
	}
	verhis.clear();

	for(StrMap::iterator it= mit->second.begin(); it!=mit->second.end(); ++it)
		verhis.insert(std::make_pair(atoi(it->first.c_str()),it->second));

	return true;
}

bool CSvDataBase::EnableRevert(int historyNum, string message, string & estr)
{
	if(historyNum>1000||historyNum<0)
	{
		estr+= " Invalid historyNum input;  ";
		return false;
	}
	if(message.empty())
	{
		estr+= "  Your parameter message is empty;  ";
		return false;
	}

	char tempchar[128]={0};
	sprintf(tempchar,"%d",historyNum);

	MapStrMap fmap,fmap2;
	PutReturnMapStrMap(fmap,"inwhat",	"dowhat",		"SetDataBaseRevert");
	PutReturnMapStrMap(fmap,"inwhat",	"dbName",		m_dbname);
	PutReturnMapStrMap(fmap,"inwhat",	"historyNum",	tempchar);
	PutReturnMapStrMap(fmap,"inwhat",	"message",		message);

	char * retp(NULL);
	int rlen(0); 
	CConnect con;
	bool bret= con.ConnectDataBase(fmap,"", retp, rlen, estr);
	if( !bret )
		estr+= "   ConnectDataBase return false;   ";
	if(retp!=NULL)
		delete [] retp;	
	return bret;
}

bool CSvDataBase::DisableRevert(string message, string & estr)
{
	return EnableRevert(0, message, estr);
}

bool CSvDataBase::GetNumOfRevertSetting(int & historyNum, string & estr)
{
	MapStrMap fmap,fmap2;
	PutReturnMapStrMap(fmap,"inwhat",	"dowhat",		"GetNumOfRevertSetting");
	PutReturnMapStrMap(fmap,"inwhat",	"dbName",		m_dbname);

	char * retp(NULL);
	int rlen(0); 
	CConnect con;
	bool bret= con.ConnectDataBase(fmap,"", retp, rlen, estr);
	if( !bret )
		estr+= "   ConnectDataBase return false;   ";

	bool bcreate= CreateForestMapByRawData(fmap2, retp, rlen);
	if(retp!=NULL)
		delete [] retp;
	if(!bcreate)
		estr+= "   Failed to create return object;   ";

	historyNum= atoi(GetValueInMapStrMap(fmap2,"return","NumOfRevertSetting").c_str());
	return bret;
}

bool CSvDataBase::RevertDataBaseToVersion(int oldver, string message, string & estr)
{
	if(oldver<=0)
	{
		estr+= "  Your parameter oldver is invalid;  ";
		return false;
	}
	if(message.empty())
	{
		estr+= "  Your parameter message is empty;  ";
		return false;
	}

	char tempchar[128]={0};
	sprintf(tempchar,"%d",oldver);

	MapStrMap fmap,fmap2;
	PutReturnMapStrMap(fmap,"inwhat",	"dowhat",		"RevertDataBaseToVersion");
	PutReturnMapStrMap(fmap,"inwhat",	"dbName",		m_dbname);
	PutReturnMapStrMap(fmap,"inwhat",	"message",		message);
	PutReturnMapStrMap(fmap,"inwhat",	"oldver",		tempchar);

	char * retp(NULL);
	int rlen(0); 
	CConnect con;
	bool bret= con.ConnectDataBase(fmap,"", retp, rlen, estr);
	if( !bret )
		estr+= "   ConnectDataBase return false;   ";

	if(retp!=NULL)
		delete [] retp;

	return bret;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////




bool CAllDataBase::CreateNewDataBase(string dbname, string message, string & estr)
{
	if(dbname.empty())
	{
		estr+= " Input dbname is emtpty;  ";
		return false;
	}
	if(message.empty())
	{
		estr+= "  Your parameter message is empty;  ";
		return false;
	}

	MapStrMap fmap,fmap2;
	PutReturnMapStrMap(fmap,"inwhat",	"dowhat",		"CreateNewDataBase");
	PutReturnMapStrMap(fmap,"inwhat",	"dbName",		dbname);
	PutReturnMapStrMap(fmap,"inwhat",	"message",		message);

	char * retp(NULL);
	int rlen(0); 
	CConnect con;
	if( !con.ConnectDataBase(fmap,"", retp, rlen, estr) )
		estr+= "   ConnectDataBase return false;   ";

	bool bcreate= CreateForestMapByRawData(fmap2, retp, rlen);
	if(retp!=NULL)
		delete [] retp;
	if(!bcreate)
		estr+= "   Failed to create return object;   ";

	string tnameok= GetValueInMapStrMap(fmap2,"return","CreateNewDataBaseDone");
	if(tnameok.compare("true")!=0)
		return false;

	CSvDataBase cdb(dbname);
	cdb.EnableRevert(20, message, estr);
	return true;
}



bool CAllDataBase::CheckNewDataBaseNameAvailable(string dbname, string & estr)
{
	if(dbname.empty()||dbname.find(".")!=std::string::npos||dbname.find(" ")!=std::string::npos)
	{
		estr+= " Invalid database name; ";
		return false;
	}

	MapStrMap fmap,fmap2;
	PutReturnMapStrMap(fmap,"inwhat",	"dowhat",		"CheckNewDataBaseNameAvailable");
	PutReturnMapStrMap(fmap,"inwhat",	"dbName",		dbname);

	char * retp(NULL);
	int rlen(0); 
	CConnect con;
	if( !con.ConnectDataBase(fmap,"", retp, rlen, estr) )
		estr+= "   ConnectDataBase return false;   ";

	bool bcreate= CreateForestMapByRawData(fmap2, retp, rlen);
	if(retp!=NULL)
		delete [] retp;
	if(!bcreate)
		estr+= "   Failed to create return object;   ";

	string tnameok= GetValueInMapStrMap(fmap2,"return","NewDataBaseNameAvailable");
	if(tnameok.compare("true")==0)
		return true;
	else
		return false;
}



bool CAllDataBase::DeleteDataBase(string dbname, string & estr)
{
	if(dbname.empty())
	{
		estr+= " Input dbname is emtpty;  ";
		return false;
	}

	MapStrMap fmap,fmap2;
	PutReturnMapStrMap(fmap,"inwhat",	"dowhat",		"DeleteDataBase");
	PutReturnMapStrMap(fmap,"inwhat",	"dbName",		dbname);

	char * retp(NULL);
	int rlen(0); 
	CConnect con;
	if( !con.ConnectDataBase(fmap,"", retp, rlen, estr) )
		estr+= "   ConnectDataBase return false;   ";

	bool bcreate= CreateForestMapByRawData(fmap2, retp, rlen);
	if(retp!=NULL)
		delete [] retp;
	if(!bcreate)
		estr+= "   Failed to create return object;   ";

	string tnameok= GetValueInMapStrMap(fmap2,"return","DeleteDataBaseDone");
	if(tnameok.compare("true")==0)
		return true;
	else
		return false;
}



bool CAllDataBase::GetAllDataBaseName(set<string> & dnames, string & estr)
{
	MapStrMap fmap,fmap2;
	PutReturnMapStrMap(fmap,"inwhat",	"dowhat",		"GetAllDataBaseName");

	char * retp(NULL);
	int rlen(0); 
	CConnect con;
	if( !con.ConnectDataBase(fmap,"", retp, rlen, estr) )
		estr+= "   ConnectDataBase return false;   ";

	bool bcreate= CreateForestMapByRawData(fmap2, retp, rlen);
	if(retp!=NULL)
		delete [] retp;
	if(!bcreate)
	{
		estr+= "   Failed to create return object;   ";
		return false;
	}

	MapStrMap::iterator mit= fmap2.begin();
	if(mit==fmap2.end())
	{
		estr+= "   There isn't any database;   ";
		return true;
	}
	dnames.clear();

	for(StrMap::iterator it= mit->second.begin(); it!=mit->second.end(); ++it)
		dnames.insert(it->first);
	return true;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



namespace svbase{

	SVCLIENT_API
		CSvTable * GetTableFromServer(CTableName tname, string & estr)
	{
		return SvTableCache::GetDataFromServer(tname, estr);
	}

	SVCLIENT_API
		CSvTable * GetTableFromCache(CTableName tname, string & estr)
	{
		return SvTableCache::GetDataFromCache(tname, estr);
	}

	SVCLIENT_API
		bool RefreshTables(const std::set<CTableName> & list_tname, string & estr)
	{
		return SvTableCache::RefreshBatchData(list_tname, estr);
	}

	SVCLIENT_API
		bool RefreshDataBase(string dbname, string & estr)
	{
		return SvTableCache::RefreshDB(dbname, estr);
	}

	SVCLIENT_API
		bool SubmitTable(CSvTable * ptable, string message, string & estr)
	{
		return SvTableCache::SubmitData(ptable, message, estr);
	}

	SVCLIENT_API
		bool SubmitAllCache(string message, string & estr)
	{
		return SvTableCache::SubmitAllCacheData(message, estr);
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	SVCLIENT_API 
		string	GetSiteViewRootPath()
	{
		return GetRootPath();
	}


	SVCLIENT_API 
		bool SetSvdbAddrByFile(string addr)
	{
		if(addr.empty())
		{
			IdcUser::svdbHostAddr.clear();
			return false;
		}
		INIFile inif=LoadIni(addr.c_str());

		string svalue=GetIniSetting(inif,"svapi","SvdbHostAddr","");
		if(!svalue.empty())
		{
			IdcUser::svdbHostAddr= svalue;
			return true;
		}
		IdcUser::svdbHostAddr.clear();
		return false;
	}


	SVCLIENT_API 
		string GetSvdbAddr(void)
	{
		return IdcUser::svdbHostAddr;
	}


	SVCLIENT_API
		void SetServerPort(unsigned short port)
	{
		g_port=port;
	}

	SVCLIENT_API
		unsigned short GetServerPort()
	{
		return g_port;
	}


} // end of namespace svbase


