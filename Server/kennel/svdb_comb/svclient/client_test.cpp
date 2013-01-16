// client_test.cpp : 定义控制台应用程序的入口点。
//

#include "svclient.h"


void test1(string temp="")
{
	string estr;
	CTableName tname("testdb2","table1");
	if(!temp.empty())
		tname.tableName= temp;

	CSvTable * ptable= GetTableFromCache(tname,estr);
	cout<<" estr: "<<estr.c_str()<<endl;
	if(ptable==NULL)
		return ;

	ptable->GetTableName().Display();
	cout<<"version: "<<ptable->GetVersion().c_str()<<endl;
	ptable->DisplayDataMembers();
	ptable->SetTableDescription("table1test");
	cout<<" IsModified: "<<ptable->IsModified()<<endl;

	estr.clear();
	cout<<"SubmitTable: "<<SubmitTable(ptable,"clientmodify",estr)<<endl;
	cout<<" estr: "<<estr.c_str()<<endl;
}


void test2(string temp="")
{
	string estr;

	CTableName tname("testdb2","table1");
	//if(!temp.empty())
	//	tname.tableName= temp;

	CSvTable * ptable= GetTableFromServer(tname,estr);
	cout<<" estr: "<<estr.c_str()<<endl;
	if(ptable==NULL)
	{
		cout<<"ptable==NULL"<<endl;
		return ;
	}
	ptable->GetTableName().Display();
	//ptable->DisplayDataMembers();
	ptable->SetTableDescription("table1test");
	cout<<" IsModified: "<<ptable->IsModified()<<endl;

	estr.clear();
	cout<<"SubmitTable: "<<SubmitTable(ptable,"clientmodify",estr)<<endl;
	cout<<" estr: "<<estr.c_str()<<endl;

}


void test3(string temp="")
{
	string estr;

	CTableName tname("testdb2","table1");
	if(!temp.empty())
		tname.tableName= temp;

	CSvDataBase cdb(tname.dbName);
	cout<<"CreateNewTable: "<<cdb.CreateNewTable(tname, "nov 16th test", estr)<<endl;
	cout<<" estr: "<<estr.c_str()<<endl;
	
}

void test4(string temp="")
{
	string estr;

	CTableName tname("testdb","table4");
	CSvDataBase cdb(tname.dbName);
	tname= cdb.GetRecommendNextTableName(estr);
	cout<<" estr: "<<estr.c_str()<<endl;
	tname.Display();
	
}

void test5(string temp="")
{
	string estr;

	CTableName tname("testdb","table1");
	if(!temp.empty())
		tname.dbName= temp;
	CSvDataBase cdb(tname.dbName);

	set<CTableName> tnames;
	cout<<"GetAllTableName: "<<cdb.GetAllTableName(tnames,estr)<<endl;
	cout<<" estr: "<<estr.c_str()<<endl;
	
	for(set<CTableName>::iterator  sit= tnames.begin(); sit!=tnames.end(); ++sit)
		sit->Display();
	
}
void test6(string temp="")
{
	string estr;

	CTableName tname("testdb","table4");
	CSvDataBase cdb(tname.dbName);
	tname= cdb.GetChildTableName(tname,estr);
	cout<<" estr: "<<estr.c_str()<<endl;
	tname.Display();
	
}
void test7(string temp="")
{
	string estr;

	CTableName tname("testdb","table4");
	if(!temp.empty())
		tname.tableName= temp;
	CSvDataBase cdb(tname.dbName);
	cout<<"CheckNewTableNameAvailable: "<<cdb.CheckNewTableNameAvailable(tname,estr)<<endl;
	cout<<" estr: "<<estr.c_str()<<endl;
	
}
void test8(string temp="")
{
	string estr;

	CTableName tname("testdb2","table7");
	if(!temp.empty())
		tname.tableName= temp;
	CSvDataBase cdb(tname.dbName);
	cout<<"DeleteTable: "<<cdb.DeleteTable(tname,"client_test",estr)<<endl;
	cout<<" estr: "<<estr.c_str()<<endl;
	
}
void test9(string temp="")
{
	string estr;
	string dbname("testdb2");
	if(!temp.empty())
		dbname= temp;
	CAllDataBase alldb;
	cout<<"CreateNewDataBase: "<<alldb.CreateNewDataBase(dbname,"client_test",estr)<<endl;
	cout<<" estr: "<<estr.c_str()<<endl;
	
}
void test10(string temp="")
{
	string estr;
	string dbname("testdb5");
	if(!temp.empty())
		dbname= temp;
	CAllDataBase alldb;
	cout<<"CheckNewDataBaseNameAvailable: "<<alldb.CheckNewDataBaseNameAvailable(dbname, estr)<<endl;
	cout<<" estr: "<<estr.c_str()<<endl;
	
}
void test11(string temp="")
{
	string estr;
	string dbname("testdb2");
	if(!temp.empty())
		dbname= temp;
	CAllDataBase alldb;
	cout<<"DeleteDataBase: "<<alldb.DeleteDataBase(dbname,estr)<<endl;
	cout<<" estr: "<<estr.c_str()<<endl;
	
}
void test12(string temp="")
{
	string estr;
	string dbname("testdb6");
	if(!temp.empty())
		dbname= temp;
	set<string> dnames;
	CAllDataBase alldb;
	cout<<"GetAllDataBaseName: "<<alldb.GetAllDataBaseName(dnames,estr)<<endl;
	cout<<" estr: "<<estr.c_str()<<endl;

	for(set<string>::iterator  sit= dnames.begin(); sit!=dnames.end(); ++sit)
		cout<<sit->c_str()<<endl;
	
}
void test13(string temp="")
{
	string estr;

	CTableName tname("testdb","table2");
	if(!temp.empty())
		tname.tableName= temp;
	CSvTable * ptable= GetTableFromServer(tname,estr);
	cout<<" estr: "<<estr.c_str()<<endl;
	if(ptable==NULL)
	{
		cout<<"ptable==NULL"<<endl;
		return ;
	}

	estr.clear();
	cout<<"IsLatestVersion: "<<ptable->IsLatestVersion(estr)<<endl;
	cout<<" estr: "<<estr.c_str()<<endl;
}

void test14(string temp="")
{
	string estr;
	string dbname("testdb2");
	//if(!temp.empty())
	//	dbname= temp;

	cout<<"RefreshDataBase: "<<RefreshDataBase(dbname,estr)<<endl;
	cout<<" estr: "<<estr.c_str()<<endl;

	//test1(temp);
	//return;

	CSvDataBase cdb(dbname);
	set<CTableName> tnames;
	estr.clear();
	cout<<"GetAllTableName: "<<cdb.GetAllTableName(tnames,estr)<<endl;
	cout<<" estr: "<<estr.c_str()<<endl;

	if(temp.empty())
		temp= "1000";
	int count= atoi(temp.c_str());
	int i(0);
	for(set<CTableName>::iterator  sit= tnames.begin(); sit!=tnames.end(); ++sit)
	{
		if(++i>count)
			continue;

		CSvTable * ptable= GetTableFromCache(*sit,estr);
		//CSvTable * ptable= GetTableFromCache(CTableName("testdb","table4"),estr);
		if(ptable==NULL)
		{
			cout<<" ptable==NULL ";
			sit->Display();
			continue ;
		}
		ptable->GetTableName().Display();
		//ptable->DisplayDataMembers();
	}
}

void test15(string temp="")
{
	string estr;

	for(int time=0; time<=1; ++time)
	{
		string dbname("testdb");
		if(time==1)
			dbname= "testdb2";

		estr.clear();
		cout<<"RefreshDataBase: "<<RefreshDataBase(dbname,estr)<<endl;
		cout<<" estr: "<<estr.c_str()<<endl;

		CSvDataBase cdb(dbname);
		set<CTableName> tnames;
		estr.clear();
		cout<<"GetAllTableName: "<<cdb.GetAllTableName(tnames,estr)<<endl;
		cout<<" estr: "<<estr.c_str()<<endl;

		if(temp.empty())
			temp= "1000";
		int count= atoi(temp.c_str());
		int i(0);
		for(set<CTableName>::iterator  sit= tnames.begin(); sit!=tnames.end(); ++sit)
		{
			if(++i>count)
				continue;

			CSvTable * ptable= GetTableFromCache(*sit,estr);
			if(ptable==NULL)
			{
				cout<<" ptable==NULL ";
				sit->Display();
				continue ;
			}

			//ptable->DisplayDataMembers();
			ptable->SetTableDescription("clienttest");
		}
	}
	
	estr.clear();
	cout<<"SubmitAllCache: "<<SubmitAllCache("testsubmitallcache", estr)<<endl;
	cout<<" estr: "<<estr.c_str()<<endl;
}
void test16(string temp="")
{
	string estr;

	CTableName tname("testdb2","table1");
	if(!temp.empty())
		tname.dbName= temp;
	CSvDataBase cdb(tname.dbName);

	map<int,string> verhis;
	cout<<"GetDBVersionHistory: "<<cdb.GetDBVersionHistory(verhis,estr)<<endl;
	cout<<" estr: "<<estr.c_str()<<endl;

	cout<<"  size:"<<verhis.size()<<endl;
	for(map<int,string>::iterator  sit= verhis.begin(); sit!=verhis.end(); ++sit)
		cout<<sit->first<<" : "<<sit->second.c_str()<<endl;
	
}
void test17(string temp="")
{
	string estr;

	CTableName tname("testdb2","table1");
	if(!temp.empty())
		tname.tableName= temp;
	CSvDataBase cdb(tname.dbName);

	map<int,string> verhis;
	cout<<"GetTableVersionHistory: "<<cdb.GetTableVersionHistory(tname, verhis, estr)<<endl;
	cout<<" estr: "<<estr.c_str()<<endl;

	cout<<"  size:"<<verhis.size()<<endl;
	for(map<int,string>::iterator  sit= verhis.begin(); sit!=verhis.end(); ++sit)
		cout<<sit->first<<" : "<<sit->second.c_str()<<endl;
	
}
void test18(string temp="")
{
	string estr;

	CTableName tname("testdb2","table7");
	if(!temp.empty())
		tname.dbName= temp;

	CSvDataBase cdb(tname.dbName);
	int num(0);
	cout<<"GetNumOfRevertSetting: "<<cdb.GetNumOfRevertSetting(num, estr)<<endl;
	cout<<"NumOfRevertSetting: "<<num<<endl;
	cout<<" estr: "<<estr.c_str()<<endl;
	
}
void test19(string temp="")
{
	string estr;

	CTableName tname("testdb2","table7");
	if(!temp.empty())
		tname.dbName= temp;

	CSvDataBase cdb(tname.dbName);
	int num(30);
	cout<<"EnableRevert: "<<cdb.EnableRevert(num, "clienttest", estr)<<endl;
	cout<<" estr: "<<estr.c_str()<<endl;
	
}
void test20(string temp="")
{
	string estr;
	string dbname("testdb2");
	if(temp.empty())
		temp= "1024";

	CSvDataBase cdb(dbname);
	cout<<"RevertDataBaseToVersion: "<<cdb.RevertDataBaseToVersion(atoi(temp.c_str()), "revertest", estr)<<endl;
	cout<<" estr: "<<estr.c_str()<<endl;
	
}




int main(int argc, char* argv[])
{

	string temp;
	while( temp!="q" && temp!="Q")
	{	
		test1(temp);
		//test15(temp);
		
		cout<<"press enter key to continue, q + enter to quit."<<endl;
		getline(cin, temp);
	}
	return 0;
}
