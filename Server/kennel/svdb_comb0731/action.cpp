


#include "action.h"
#include "svdbapi.h"
#include "util.h"
#include "QueryProtocol.h"
#include "somefunc.h"



bool ReactConnection2(SvdbMain *m_pMain, string qstr, std::list<SingelRecord> & listrcd, SingelRecord & r1, string & estr)
{
	std::list<SingelRecord>::iterator lsit= listrcd.begin();
	if(lsit==listrcd.end())
	{
		estr+= " Can't get query conditions;  ";
		return false;
	}

	MapStrMap fmap;
	CreateForestMapByRawData(fmap, lsit->data, lsit->datalen);

	S_UINT dsize(0);
	char * tempp= NULL; 
	char * buf(NULL);

	r1.datalen= 0;
	r1.data= NULL;

	string what=GetValueInMapStrMap(fmap, "inwhat", "dowhat");
	if(what.empty())
	{
		estr+= " dowhat is empty;  ";
		return false;
	}
	cout<<"   dowhat==\""<<what.c_str()<<"\""<<endl;

	CWholeConfig *m_pWholeConfig(NULL);
	if(m_pMain==NULL)
	{
		estr+= "   Error: m_pMain==NULL;   ";
		return false;
	}
	m_pWholeConfig= m_pMain->m_pWholeConfig;
	if(m_pWholeConfig==NULL)
	{
		estr+= "   Error: m_pWholeConfig==NULL;   ";
		return false;
	}

	if(what.compare("IsLatestVersion")==0)
	{
		CTableName tnamein(GetValueInMapStrMap(fmap, "inwhat", "dbName"),GetValueInMapStrMap(fmap, "inwhat", "tableName"));
		if(tnamein.isEmpty())
		{
			estr+= " CTableName from client is empty;   ";
			return false;
		}
		string clientver= GetValueInMapStrMap(fmap, "inwhat", "clientver");
		CConfigFile * ccdb= m_pWholeConfig->GetDataBase(tnamein.dbName, estr);
		if(ccdb==NULL)
		{
			estr+= "   Failed to GetDataBase: "+ tnamein.dbName +" ;   ";
			return false;
		}
		CConfig * cctable= ccdb->GetTable(tnamein, estr);
		if(cctable==NULL)
		{
			estr+= "   Failed to GetTable: "+ tnamein.tableName +" ;   ";
			return false;
		}

		string server_ver;
		MapStrMap fmaptname;
		if( cctable->CheckClientVerIsLatest(clientver, server_ver, estr) )
			PutReturnMapStrMap(fmaptname,"return",	"ClientVersionIsLatest", "true");
		else
			PutReturnMapStrMap(fmaptname,"return",	"ClientVersionIsLatest", "false");

		dsize= GetForestMapRawDataSize(fmaptname);
		if(dsize==0)
		{
			estr+= "   Failed to get data size of return object;   ";
			return false;
		}
		buf= new char[dsize];
		if(buf==NULL)
		{
			estr+= " new an object,that==NULL;  ";
			return false;
		}
		tempp= GetForestMapRawData(fmaptname,buf,dsize);
		if(tempp==NULL)
		{
			estr+= "   Failed to get binary data of return object;   ";
			delete [] buf;
			return false;
		}
	}
	else if(what.compare("GetDataFromServer")==0)
	{
		CTableName tname(GetValueInMapStrMap(fmap, "inwhat", "dbName"),GetValueInMapStrMap(fmap, "inwhat", "tableName"));
		if(tname.isEmpty())
		{
			estr+= " CTableName from client is empty;   ";
			return false;
		}

		CConfigFile * ccdb= m_pWholeConfig->GetDataBase(tname.dbName, estr);
		if(ccdb==NULL)
		{
			estr+= "   Failed to GetDataBase: "+ tname.dbName +" ;   ";
			return false;
		}
		CConfig * cctable= ccdb->GetTable(tname, estr);
		if(cctable==NULL)
		{
			estr+= "   Failed to GetTable: "+ tname.tableName +" ;   ";
			return false;
		}

		dsize= cctable->GetRawDataSize();
		if(dsize==0)
		{
			estr+= "   Failed to get data size of table: "+ tname.tableName +" ;   ";
			return false;
		}
		buf= new char[dsize];
		if(buf==NULL)
		{
			estr+= " new an object,that==NULL;  ";
			return false;
		}
		tempp= cctable->GetRawData(buf,dsize);
		if(tempp==NULL)
		{
			estr+= "   Failed to get binary data of table: "+ tname.tableName +" ;   ";
			return false;
		}
	}
	else if(what.compare("SubmitTable")==0)
	{
		string message= "SubmitTable "+GetValueInMapStrMap(fmap, "inwhat", "message");
		CTableName tname(GetValueInMapStrMap(fmap, "inwhat", "dbName"),GetValueInMapStrMap(fmap, "inwhat", "tableName"));
		if(tname.isEmpty())
		{
			estr+= " CTableName from client is empty;   ";
			return false;
		}

		if(++lsit==listrcd.end())
		{
			estr+= " Can't get data of CSvTable ;  ";
			return false;
		}
		CConfig * ctable= new CConfig(tname.tableName);
		if(ctable==NULL)
		{
			estr+= " new an object , that==NULL;  ";
			return false;
		}
		if( !ctable->CreateByRawData(lsit->data, lsit->datalen) )
		{
			estr+= " Can't recover object of CSvTable ;  ";
			delete ctable;
			return false;
		}
		ctable->AnalyseDataToSetVersion(estr);

		CConfigFile * ccdb= m_pWholeConfig->GetDataBase(tname.dbName, estr);
		if(ccdb==NULL)
		{
			estr+= "   Failed to GetDataBase: "+ tname.dbName +" ;   ";
			delete ctable;
			return false;
		}
	
		string newver= ccdb->SetNewVersionAndBackupOld(estr);
		ctable->PushVersion(newver,estr);
		if( ccdb->UpdateCConfig(tname, ctable, estr) )
		{
			ccdb->SaveData(estr);
			m_pWholeConfig->PushVersion(tname.dbName,newver,message,estr);
			m_pWholeConfig->SaveData(estr);
		}
	}
	else if(what.compare("CreateNewTable")==0)
	{
		string message= "CreateNewTable "+GetValueInMapStrMap(fmap, "inwhat", "message");
		CTableName tname(GetValueInMapStrMap(fmap, "inwhat", "dbName"),GetValueInMapStrMap(fmap, "inwhat", "tableName"));
		if(tname.isEmpty())
		{
			estr+= " CTableName from client is empty;   ";
			return false;
		}

		if(++lsit==listrcd.end())
		{
			estr+= " Can't get data of CSvTable ;  ";
			return false;
		}
		CConfig * ctable= new CConfig(tname.tableName);
		if(ctable==NULL)
		{
			estr+= " new an object , that==NULL;  ";
			return false;
		}
		if( !ctable->CreateByRawData(lsit->data, lsit->datalen) )
		{
			estr+= " Can't recover object of CSvTable ;  ";
			delete ctable;
			return false;
		}

		CConfigFile * ccdb= m_pWholeConfig->GetDataBase(tname.dbName, estr);
		if(ccdb==NULL)
		{
			estr+= "   Failed to GetDataBase: "+ tname.dbName +" ;   ";
			delete ctable;
			return false;
		}

		string estr2;
		CConfig * cctable= ccdb->GetTable(tname, estr2);
		if(cctable!=NULL)
		{
			estr+= "  " + tname.tableName + " already exists;  ";
			delete ctable;
			return false;
		}
	
		string newver= ccdb->SetNewVersionAndBackupOld(estr);
		ctable->PushVersion(newver,estr2);
		if( !ccdb->CreateNewTable(tname, ctable, estr) )
		{
			estr+= "   Failed to CreateNewTable: "+ tname.tableName +" ;   ";
			delete ctable;
			return false;
		}

		ccdb->SaveData(estr);
		m_pWholeConfig->PushVersion(tname.dbName,newver,message,estr);
		m_pWholeConfig->SaveData(estr);
	}
	else if(what.compare("GetRecommendNextTableName")==0)
	{
		string dbName(GetValueInMapStrMap(fmap, "inwhat", "dbName"));
		if(dbName.empty())
		{
			estr+= " DB name from client is empty;   ";
			return false;
		}

		CConfigFile * ccdb= m_pWholeConfig->GetDataBase(dbName, estr);
		if(ccdb==NULL)
		{
			estr+= "   Failed to GetDataBase: "+ dbName +" ;   ";
			return false;
		}
		CTableName tname= ccdb->GetRecommendNextTableName();
		if(tname.isEmpty())
		{
			estr+= "   Failed to GetRecommendNextTableName of database: "+ dbName +" ;   ";
			return false;
		}

		MapStrMap fmaptname;
		PutReturnMapStrMap(fmaptname,"return",	"dbName",		tname.dbName);
		PutReturnMapStrMap(fmaptname,"return",	"tableName",	tname.tableName);

		dsize= GetForestMapRawDataSize(fmaptname);
		if(dsize==0)
		{
			estr+= "   Failed to get data size of return object;   ";
			return false;
		}
		buf= new char[dsize];
		if(buf==NULL)
		{
			estr+= " new an object,that==NULL;  ";
			return false;
		}
		tempp= GetForestMapRawData(fmaptname,buf,dsize);
		if(tempp==NULL)
		{
			estr+= "   Failed to get binary data of return object;   ";
			delete [] buf;
			return false;
		}
	}
	else if(what.compare("GetAllTableName")==0)
	{
		string dbName(GetValueInMapStrMap(fmap, "inwhat", "dbName"));
		if(dbName.empty())
		{
			estr+= " DB name from client is empty;   ";
			return false;
		}

		CConfigFile * ccdb= m_pWholeConfig->GetDataBase(dbName, estr);
		if(ccdb==NULL)
		{
			estr+= "   Failed to GetDataBase: "+ dbName +" ;   ";
			return false;
		}

		set<CTableName> tnames;
		if(!ccdb->GetAllTableName(tnames))
		{
			estr+= "   Failed to GetAllTableName of data base: "+ dbName +" ;   ";
			return false;
		}

		MapStrMap fmaptname;
		for(set<CTableName>::iterator  sit= tnames.begin(); sit!=tnames.end(); ++sit)
			PutReturnMapStrMap(fmaptname,"return",	(*sit).tableName, "tablename");
		
		dsize= GetForestMapRawDataSize(fmaptname);
		if(dsize==0)
		{
			estr+= "   Failed to get data size of return object;   ";
			return false;
		}
		buf= new char[dsize];
		if(buf==NULL)
		{
			estr+= " new an object,that==NULL;  ";
			return false;
		}
		tempp= GetForestMapRawData(fmaptname,buf,dsize);
		if(tempp==NULL)
		{
			estr+= "   Failed to get binary data of return object;   ";
			delete [] buf;
			return false;
		}
	}
	else if(what.compare("GetChildTableName")==0)
	{
		CTableName tnamein(GetValueInMapStrMap(fmap, "inwhat", "dbName"),GetValueInMapStrMap(fmap, "inwhat", "tableName"));
		if(tnamein.isEmpty())
		{
			estr+= " CTableName from client is empty;   ";
			return false;
		}

		CConfigFile * ccdb= m_pWholeConfig->GetDataBase(tnamein.dbName, estr);
		if(ccdb==NULL)
		{
			estr+= "   Failed to GetDataBase: "+ tnamein.dbName +" ;   ";
			return false;
		}
		CTableName tname= ccdb->GetChildTableName(tnamein);
		if(tname.isEmpty())
		{
			estr+= "   Failed to GetChildTableName of database: "+ tnamein.dbName +" ;   ";
			return false;
		}

		MapStrMap fmaptname;
		PutReturnMapStrMap(fmaptname,"return",	"dbName",		tname.dbName);
		PutReturnMapStrMap(fmaptname,"return",	"tableName",	tname.tableName);

		dsize= GetForestMapRawDataSize(fmaptname);
		if(dsize==0)
		{
			estr+= "   Failed to get data size of return object;   ";
			return false;
		}
		buf= new char[dsize];
		if(buf==NULL)
		{
			estr+= " new an object,that==NULL;  ";
			return false;
		}
		tempp= GetForestMapRawData(fmaptname,buf,dsize);
		if(tempp==NULL)
		{
			estr+= "   Failed to get binary data of return object;   ";
			delete [] buf;
			return false;
		}
	}
	else if(what.compare("CheckNewTableNameAvailable")==0)
	{
		CTableName tnamein(GetValueInMapStrMap(fmap, "inwhat", "dbName"),GetValueInMapStrMap(fmap, "inwhat", "tableName"));
		if(tnamein.isEmpty())
		{
			estr+= " CTableName from client is empty;   ";
			return false;
		}

		CConfigFile * ccdb= m_pWholeConfig->GetDataBase(tnamein.dbName, estr);
		if(ccdb==NULL)
		{
			estr+= "   Failed to GetDataBase: "+ tnamein.dbName +" ;   ";
			return false;
		}

		MapStrMap fmaptname;
		if( !ccdb->CheckNewTableNameAvailable(tnamein, estr) )
		{
			estr+= "  Unavailable TableName: "+ tnamein.tableName +" ;   ";
			PutReturnMapStrMap(fmaptname,"return",	"NewTableNameAvailable", "false");
		}
		else
			PutReturnMapStrMap(fmaptname,"return",	"NewTableNameAvailable", "true");

		dsize= GetForestMapRawDataSize(fmaptname);
		if(dsize==0)
		{
			estr+= "   Failed to get data size of return object;   ";
			return false;
		}
		buf= new char[dsize];
		if(buf==NULL)
		{
			estr+= " new an object,that==NULL;  ";
			return false;
		}
		tempp= GetForestMapRawData(fmaptname,buf,dsize);
		if(tempp==NULL)
		{
			estr+= "   Failed to get binary data of return object;   ";
			delete [] buf;
			return false;
		}
	}
	else if(what.compare("DeleteTable")==0)
	{
		CTableName tnamein(GetValueInMapStrMap(fmap, "inwhat", "dbName"),GetValueInMapStrMap(fmap, "inwhat", "tableName"));
		if(tnamein.isEmpty())
		{
			estr+= " CTableName from client is empty;   ";
			return false;
		}
		string message= "DeleteTable "+GetValueInMapStrMap(fmap, "inwhat", "message");

		CConfigFile * ccdb= m_pWholeConfig->GetDataBase(tnamein.dbName, estr);
		if(ccdb==NULL)
		{
			estr+= "   Failed to GetDataBase: "+ tnamein.dbName +" ;   ";
			return false;
		}

		MapStrMap fmaptname;
		if( !ccdb->DeleteTable(tnamein, estr) )
		{
			estr+= "  Failed to DeleteTable: "+ tnamein.tableName +" ;   ";
			PutReturnMapStrMap(fmaptname,"return",	"DeleteTableDone", "false");
		}
		else
		{
			cout<<"         Succeeded to delete table: "<<tnamein.tableName<<" of "<<tnamein.dbName<<endl;
			string newver= ccdb->SetNewVersionAndBackupOld(estr);
			ccdb->SaveData(estr);
			m_pWholeConfig->PushVersion(tnamein.dbName,newver,message,estr);
			m_pWholeConfig->SaveData(estr);
			PutReturnMapStrMap(fmaptname,"return",	"DeleteTableDone", "true");
		}

		dsize= GetForestMapRawDataSize(fmaptname);
		if(dsize==0)
		{
			estr+= "   Failed to get data size of return object;   ";
			return false;
		}
		buf= new char[dsize];
		if(buf==NULL)
		{
			estr+= " new an object,that==NULL;  ";
			return false;
		}
		tempp= GetForestMapRawData(fmaptname,buf,dsize);
		if(tempp==NULL)
		{
			estr+= "   Failed to get binary data of return object;   ";
			delete [] buf;
			return false;
		}
	}
	else if(what.compare("CreateNewDataBase")==0)
	{
		string  dbname(GetValueInMapStrMap(fmap, "inwhat", "dbName"));
		if(dbname.empty())
		{
			estr+= " dbName from client is empty;   ";
			return false;
		}
		string message= "CreateNewDataBase "+GetValueInMapStrMap(fmap, "inwhat", "message");

		MapStrMap fmaptname;
		if( !m_pWholeConfig->CreateNewDataBase(dbname, message ,estr) )
		{
			estr+= "  Failed to CreateNewDataBase: "+ dbname +" ;   ";
			PutReturnMapStrMap(fmaptname,"return",	"CreateNewDataBaseDone", "false");
		}
		else
		{
			m_pWholeConfig->SaveData(estr);
			PutReturnMapStrMap(fmaptname,"return",	"CreateNewDataBaseDone", "true");
		}

		dsize= GetForestMapRawDataSize(fmaptname);
		if(dsize==0)
		{
			estr+= "   Failed to get data size of return object;   ";
			return false;
		}
		buf= new char[dsize];
		if(buf==NULL)
		{
			estr+= " new an object,that==NULL;  ";
			return false;
		}
		tempp= GetForestMapRawData(fmaptname,buf,dsize);
		if(tempp==NULL)
		{
			estr+= "   Failed to get binary data of return object;   ";
			delete [] buf;
			return false;
		}
	}
	else if(what.compare("CheckNewDataBaseNameAvailable")==0)
	{
		string  dbname(GetValueInMapStrMap(fmap, "inwhat", "dbName"));
		if(dbname.empty())
		{
			estr+= " dbName from client is empty;   ";
			return false;
		}

		MapStrMap fmaptname;
		if( !m_pWholeConfig->CheckNewDataBaseNameAvailable(dbname, estr) )
		{
			estr+= "  Unavailable new database name: "+ dbname +" ;   ";
			PutReturnMapStrMap(fmaptname,"return",	"NewDataBaseNameAvailable", "false");
		}
		else
			PutReturnMapStrMap(fmaptname,"return",	"NewDataBaseNameAvailable", "true");

		dsize= GetForestMapRawDataSize(fmaptname);
		if(dsize==0)
		{
			estr+= "   Failed to get data size of return object;   ";
			return false;
		}
		buf= new char[dsize];
		if(buf==NULL)
		{
			estr+= " new an object,that==NULL;  ";
			return false;
		}
		tempp= GetForestMapRawData(fmaptname,buf,dsize);
		if(tempp==NULL)
		{
			estr+= "   Failed to get binary data of return object;   ";
			delete [] buf;
			return false;
		}
	}
	else if(what.compare("DeleteDataBase")==0)
	{
		string  dbname(GetValueInMapStrMap(fmap, "inwhat", "dbName"));
		if(dbname.empty())
		{
			estr+= " dbName from client is empty;   ";
			return false;
		}

		MapStrMap fmaptname;
		if( !m_pWholeConfig->DeleteDataBase(dbname, estr) )
		{
			estr+= "  Failed to DeleteDataBase: "+ dbname +" ;   ";
			PutReturnMapStrMap(fmaptname,"return",	"DeleteDataBaseDone", "false");
		}
		else
		{
			m_pWholeConfig->SaveData(estr);
			PutReturnMapStrMap(fmaptname,"return",	"DeleteDataBaseDone", "true");
		}

		dsize= GetForestMapRawDataSize(fmaptname);
		if(dsize==0)
		{
			estr+= "   Failed to get data size of return object;   ";
			return false;
		}
		buf= new char[dsize];
		if(buf==NULL)
		{
			estr+= " new an object,that==NULL;  ";
			return false;
		}
		tempp= GetForestMapRawData(fmaptname,buf,dsize);
		if(tempp==NULL)
		{
			estr+= "   Failed to get binary data of return object;   ";
			delete [] buf;
			return false;
		}
	}
	else if(what.compare("GetAllDataBaseName")==0)
	{
		set<string> dnames;
		if(!m_pWholeConfig->GetAllDataBaseName(dnames))
		{
			estr+= "   Failed to GetAllDataBaseName;   ";
			return false;
		}

		MapStrMap fmaptname;
		for(set<string>::iterator  sit= dnames.begin(); sit!=dnames.end(); ++sit)
			PutReturnMapStrMap(fmaptname,"return",	(*sit), "dbname");
		
		dsize= GetForestMapRawDataSize(fmaptname);
		if(dsize==0)
		{
			estr+= "   Failed to get data size of return object;   ";
			return false;
		}
		buf= new char[dsize];
		if(buf==NULL)
		{
			estr+= " new an object,that==NULL;  ";
			return false;
		}
		tempp= GetForestMapRawData(fmaptname,buf,dsize);
		if(tempp==NULL)
		{
			estr+= "   Failed to get binary data of return object;   ";
			delete [] buf;
			return false;
		}
	}
	else if(what.compare("RefreshBatchData")==0)
	{
		std::list<SingelRecord> temp_listrcd;
		for(ForestMap::iterator fit=fmap.begin(); fit!=fmap.end();  ++fit)
		{
			if( fit->first.compare("inwhat")==0 )
				continue;

			string clientver;
			CTableName tname;
			for(NodeData::iterator nit=fit->second.begin(); nit!=fit->second.end(); ++nit)
			{
				if(nit->second.compare(fit->first+"version")==0)
					clientver= nit->first;
				else
				{
					tname.dbName= nit->second;
					tname.tableName= nit->first;
				}
			}
			if(tname.isEmpty())
				continue;

			CConfigFile * ccdb= m_pWholeConfig->GetDataBase(tname.dbName, estr);
			if(ccdb==NULL)
			{
				estr+= "   Failed to GetDataBase: "+ tname.dbName +" ;   ";

				SingelRecord temprcd;
				temprcd.datalen= 0;
				temprcd.monitorid= tname.dbName +"."+ tname.tableName;
				temprcd.data= NULL;
				temp_listrcd.push_back(temprcd);
				continue;
			}

			CConfig * cctable= ccdb->GetTable(tname, estr);
			if(cctable==NULL)
			{
				estr+= "   Failed to GetTable: "+ tname.tableName +" ;   ";

				SingelRecord temprcd;
				temprcd.datalen= 0;
				temprcd.monitorid= tname.dbName +"."+ tname.tableName;
				temprcd.data= NULL;
				temp_listrcd.push_back(temprcd);
				continue;
			}

			string estr2;
			string server_ver;
			if( !clientver.empty() && cctable->CheckClientVerIsLatest(clientver, server_ver, estr2))
				continue;

			cout<<"     ";
			tname.Display();

			S_UINT tempsize= cctable->GetRawDataSize();
			if(tempsize==0)
			{
				estr+= "   Failed to get data size of table: "+ tname.tableName +" ;   ";
				continue;
			}

			SingelRecord temprcd;
			temprcd.datalen= tempsize;

			char * tempbuf= new char[temprcd.datalen];
			if(tempbuf==NULL)
			{
				estr+= " new an object,that==NULL;  ";
				continue;
			}

			char * tempchar= cctable->GetRawData(tempbuf, temprcd.datalen);
			if(tempchar==NULL)
			{
				estr+= "   Failed to get binary data of table: "+ tname.tableName +" ;   ";
				delete [] tempbuf;
				continue;
			}

			temprcd.monitorid= tname.tableName;
			temprcd.data= tempchar;
			temp_listrcd.push_back(temprcd);

		}

		dsize= GetMassRecordListRawDataSize(temp_listrcd);
		if(dsize)
		{
			buf= new char[dsize];
			if(buf!=NULL)
			{
				tempp= GetMassRecordListRawData(temp_listrcd, buf, dsize);
				if(tempp==NULL)
				{
					delete [] buf;
					buf= NULL;
					dsize= 0;
					estr+= "   Failed to get binary data of batch return object ;   ";
				}
			}
			else
			{
				estr+= " new an object,that==NULL;  ";
				dsize= 0;
			}
		}
		else
			estr+= "   Failed to get data size of batch return object ;   ";

		for(std::list<SingelRecord>::iterator  llit=temp_listrcd.begin(); llit!=temp_listrcd.end(); ++llit)
			if( (llit->data) != NULL )
				delete [] (llit->data);
	}
	else if(what.compare("SubmitAllCacheData")==0)
	{
		string message= "SubmitAllCacheData "+ GetValueInMapStrMap(fmap, "inwhat", "message");
		map<CTableName,CConfig *> temptables;
		set<string> dbs;
		bool bret(true);
		string estr2;
		for(std::list<SingelRecord>::iterator  lsit=listrcd.begin(); lsit!=listrcd.end(); ++lsit)
		{
			if(lsit==listrcd.begin())
				continue;
			if(lsit->monitorid.empty())
				continue;

			CConfig * ctable= new CConfig(lsit->monitorid);
			if(ctable==NULL)
			{
				estr+= " new an object , that==NULL;  ";
				bret= false;
				continue;
			}
			if( !ctable->CreateByRawData(lsit->data, lsit->datalen) )
			{
				estr+= " Can't recover object of CSvTable:"+lsit->monitorid+" ;  ";
				delete ctable;
				bret= false;
				continue;
			}
			ctable->AnalyseDataToSetVersion(estr2);

			CTableName tname(ctable->GetTableName());
			CConfigFile * ccdb= m_pWholeConfig->GetDataBase(tname.dbName, estr);
			if(ccdb==NULL)
			{
				estr+= "   Failed to GetDataBase: "+ tname.dbName +" ;   ";
				delete ctable;
				bret= false;
				continue;
			}

			temptables.insert(std::make_pair(tname,ctable));
			dbs.insert(tname.dbName);
		}

		int count1= temptables.size();
		for(set<string>::iterator sit= dbs.begin(); sit!= dbs.end(); ++sit)
		{
			CConfigFile * ccdb= m_pWholeConfig->GetDataBase(*sit, estr);
			if(ccdb==NULL)
			{
				bret= false;
				continue;
			}
			string newver= ccdb->SetNewVersionAndBackupOld(estr);
			for(map<CTableName,CConfig *>::iterator tit= temptables.begin(); tit!= temptables.end(); ++tit )
				if(tit->first.dbName.compare(*sit)==0 && tit->second)
						tit->second->PushVersion(newver,estr);
			
			try{
				if( ccdb->UpdateBatchCConfig(temptables, estr) )
				{
					ccdb->SaveData(estr);
					m_pWholeConfig->PushVersion(*sit,newver,message,estr);
					m_pWholeConfig->SaveData(estr);
				}
				else
					bret= false;
			}
			catch(...)
			{
				bret=false;
				estr+= " Exception in updating batch tables of database: "+ *sit +"; ";		
				cout<<"\n\n-----estr: "<<estr.c_str()<<endl;
			}		
		}
		int count2= temptables.size();
		for(map<CTableName,CConfig *>::iterator tit= temptables.begin(); tit!= temptables.end(); ++tit )
		{
			if(tit->second)
				delete tit->second;
			bret= false;
			estr+= "  Some error of table: "+ tit->first.tableName+"/"+tit->first.dbName +" ; ";	
		}

		char tempchar[128]={0};
		sprintf(tempchar,"%d",count1-count2);
		string text= tempchar;
		estr+= "  Update "+text+" tables in server;  ";		

		return bret;
	}
	else if(what.compare("GetDBVersionHistory")==0)
	{
		string dbName= GetValueInMapStrMap(fmap, "inwhat", "dbName");
		if(dbName.empty())
		{
			estr+= " dbName from client is empty;   ";
			return false;
		}
		
		StrMap ndata, needs;
		if(!m_pWholeConfig->GetVersionHistory(dbName, ndata, needs, estr))
		{
			estr+= "   Failed to get history of database: "+dbName+"  ;   ";
			return false;
		}

		MapStrMap fmaptname;
		fmaptname.insert(std::make_pair("return",ndata));
		
		dsize= GetForestMapRawDataSize(fmaptname);
		if(dsize==0)
		{
			estr+= "   Failed to get data size of return object;   ";
			return false;
		}
		buf= new char[dsize];
		if(buf==NULL)
		{
			estr+= " new an object,that==NULL;  ";
			return false;
		}
		tempp= GetForestMapRawData(fmaptname,buf,dsize);
		if(tempp==NULL)
		{
			estr+= "   Failed to get binary data of return object;   ";
			delete [] buf;
			return false;
		}
	}
	else if(what.compare("GetTableVersionHistory")==0)
	{
		CTableName tname(GetValueInMapStrMap(fmap, "inwhat", "dbName"),GetValueInMapStrMap(fmap, "inwhat", "tableName"));
		if(tname.isEmpty())
		{
			estr+= " CTableName from client is empty;   ";
			return false;
		}
		
		CConfigFile * ccdb= m_pWholeConfig->GetDataBase(tname.dbName, estr);
		if(ccdb==NULL)
		{
			estr+= "   Failed to GetDataBase: "+ tname.dbName +" ;   ";
			return false;
		}
		CConfig * cctable= ccdb->GetTable(tname, estr);
		if(cctable==NULL)
		{
			estr+= "   Failed to GetTable: "+ tname.tableName +" ;   ";
			return false;
		}

		StrMap ndata, needs;
		if(!cctable->GetHistory(needs,estr))
		{
			estr+= "   Failed to get history of table: "+tname.tableName+"  ;   ";
			return false;
		}

		if(!m_pWholeConfig->GetVersionHistory(tname.dbName, ndata, needs, estr))
		{
			estr+= "   Failed to get history of database: "+tname.dbName+"  ;   ";
			return false;
		}

		MapStrMap fmaptname;
		fmaptname.insert(std::make_pair("return",ndata));
		
		dsize= GetForestMapRawDataSize(fmaptname);
		if(dsize==0)
		{
			estr+= "   Failed to get data size of return object;   ";
			return false;
		}
		buf= new char[dsize];
		if(buf==NULL)
		{
			estr+= " new an object,that==NULL;  ";
			return false;
		}
		tempp= GetForestMapRawData(fmaptname,buf,dsize);
		if(tempp==NULL)
		{
			estr+= "   Failed to get binary data of return object;   ";
			delete [] buf;
			return false;
		}
	}
	else if(what.compare("SetDataBaseRevert")==0)
	{
		string dbName= GetValueInMapStrMap(fmap, "inwhat", "dbName");
		string message= "SetDataBaseRevert "+GetValueInMapStrMap(fmap, "inwhat", "message");
		if(dbName.empty())
		{
			estr+= " dbName from client is empty;   ";
			return false;
		}
		int historyNum= atoi( GetValueInMapStrMap(fmap, "inwhat", "historyNum").c_str() );
		if(historyNum>1000||historyNum<0)
		{
			estr+= " Invalid historyNum input;  ";
			return false;
		}
		
		CConfigFile * ccdb= m_pWholeConfig->GetDataBase(dbName, estr);
		if(ccdb==NULL)
		{
			estr+= "   Failed to GetDataBase: "+ dbName +" ;   ";
			return false;
		}
		cout<<"         Database: "<<dbName.c_str()<<" set revert num to "<<historyNum<<endl;
		bool bret= ccdb->SetRevertNum(historyNum,estr);
		if(bret)
		{
			string newver= ccdb->SetNewVersionAndBackupOld(estr);
			ccdb->SaveData(estr);
			m_pWholeConfig->PushVersion(dbName,newver,message,estr);
			m_pWholeConfig->SaveData(estr);
		}
		return bret;
	}
	else if(what.compare("GetNumOfRevertSetting")==0)
	{
		string dbName= GetValueInMapStrMap(fmap, "inwhat", "dbName");
		if(dbName.empty())
		{
			estr+= " dbName from client is empty;   ";
			return false;
		}
		CConfigFile * ccdb= m_pWholeConfig->GetDataBase(dbName, estr);
		if(ccdb==NULL)
		{
			estr+= "   Failed to GetDataBase: "+ dbName +" ;   ";
			return false;
		}

		MapStrMap fmaptname;
		char tempchar[128]={0};
		sprintf(tempchar,"%d",ccdb->GetNumOfRevertSetting());
		PutReturnMapStrMap(fmaptname,"return","NumOfRevertSetting",tempchar);

		dsize= GetForestMapRawDataSize(fmaptname);
		if(dsize==0)
		{
			estr+= "   Failed to get data size of return object;   ";
			return false;
		}
		buf= new char[dsize];
		if(buf==NULL)
		{
			estr+= " new an object,that==NULL;  ";
			return false;
		}
		tempp= GetForestMapRawData(fmaptname,buf,dsize);
		if(tempp==NULL)
		{
			estr+= "   Failed to get binary data of return object;   ";
			delete [] buf;
			return false;
		}
	}
	else if(what.compare("RevertDataBaseToVersion")==0)
	{
		string dbName(GetValueInMapStrMap(fmap, "inwhat", "dbName"));
		if(dbName.empty())
		{
			estr+= " DB name from client is empty;   ";
			return false;
		}
		string message= "RevertDataBaseToVersion "+ GetValueInMapStrMap(fmap, "inwhat", "message");
		string oldver(GetValueInMapStrMap(fmap, "inwhat", "oldver"));
		if(oldver.empty())
		{
			estr+= " oldver from client is empty;   ";
			return false;
		}

		CConfigFile * ccdb= m_pWholeConfig->GetDataBase(dbName, estr);
		if(ccdb==NULL)
		{
			estr+= "   Failed to GetDataBase: "+ dbName +" ;   ";
			return false;
		}

		string newver= ccdb->SetNewVersionAndBackupOld(estr);
		if(!ccdb->RevertDataBaseToVersion(oldver,estr))
		{
			estr+= "  Failed to RevertDataBaseToVersion of database: "+dbName+";   ";	
			return false;
		}

		m_pWholeConfig->PushVersion(dbName,newver,message,estr);
		m_pWholeConfig->SaveData(estr);
	}
	else
	{
		estr+= " Undefine dowhat:"+what+";  ";
		return false;
	}


	r1.datalen= dsize;
	r1.data= tempp;
	return true;
}


S_UINT ReactConnection(SvdbMain *m_pMain, string qstr, const char *data,S_UINT datalen, std::list<SingelRecord> & listrcd)
{
	string estr;
	bool bret(false);
	SingelRecord r1;

	try{
		std::list<SingelRecord> listrcd;
		if( CreateMassRecordListByRawData(listrcd, data, datalen) )
		{
			for(std::list<SingelRecord>::iterator lit=listrcd.begin(); lit!=listrcd.end(); ++lit)
			{
				if(lit->datalen)
				{
					char * tempchar= new char[lit->datalen];
					if(tempchar!=NULL)
					{
						memmove(tempchar,lit->data,lit->datalen);
						lit->data= tempchar;
					}
					else
						lit->data= NULL;
				}
				else
					lit->data= NULL;
			}

			try{
				bret= ReactConnection2(m_pMain, qstr, listrcd, r1, estr);
			}
			catch(...)
			{
				bret=false;
				estr+= " Exception in ReactConnection2; ";		
				cout<<"\n\n-----estr: "<<estr.c_str()<<endl;
			}
			for(std::list<SingelRecord>::iterator llit=listrcd.begin(); llit!=listrcd.end(); ++llit)
				if( (llit->data) != NULL )
					delete [] (llit->data);
		}
	}
	catch(...)
	{
		bret=false;
		estr+= " Exception in ReactConnection; ";
		cout<<"\n\n-----estr: "<<estr.c_str()<<endl;
	}
	r1.monitorid= estr;
	listrcd.push_back(r1);

	if(bret)
		return SVDB_OK;
	else
		return SVDB_FAILED;
}


