
#ifndef	SITEVIEW_SVBASE_H
#define	SITEVIEW_SVBASE_H

#pragma warning(disable: 4251 4267 4018; once: )


#include <string>
#include <iostream>
#include <map>
#include <set>
#include <list>

using namespace std;


namespace svbase{


	typedef std::map<string,string>	StrMap;
	typedef std::map<string,StrMap>	MapStrMap;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//此类包含了： 数据库名、表名
	class CTableName
	{
	public:
		CTableName(){}

		CTableName(const CTableName & tname)
		{
			dbName= tname.dbName;
			tableName= tname.tableName;
		}
		CTableName(string dbname, string tbname)
		{
			dbName= dbname;
			tableName= tbname;
		}
		const CTableName& operator=(const CTableName& tname)
		{
			dbName= tname.dbName;
			tableName= tname.tableName;
			return *this;
		}
		bool operator<(CTableName tname) const
		{
			return ((dbName+tableName).compare(tname.dbName + tname.tableName)<0);
		}
		bool operator>(CTableName tname) const
		{
			return ((dbName+tableName).compare(tname.dbName + tname.tableName)>0);
		}
		bool operator==(CTableName tname) const
		{
			return ((dbName+tableName).compare(tname.dbName + tname.tableName)==0);
		}
		bool isEmpty(void)
		{
			if(dbName.empty()||tableName.empty())
				return true;
			return false;
		}

		void Display()
		{
			cout<<"      tableName:"<<tableName.c_str()<<"  dbName:"<<dbName.c_str()<<endl;
		}

		string dbName;
		string tableName;
	};// end of class CTableName


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//此类包含了： 数据库名、表名、默认主键名
	class CDefultPK
	{
	public:
		CDefultPK(){}

		CDefultPK(const CDefultPK & dname)
		{
			dbName= dname.dbName;
			tableName= dname.tableName;
			dpk= dname.dpk;
		}
		CDefultPK(string dbname, string tbname, string defaultpk)
		{
			dbName= dbname;
			tableName= tbname;
			dpk= defaultpk;
		}
		const CDefultPK& operator=(const CDefultPK & dname)
		{
			dbName= dname.dbName;
			tableName= dname.tableName;
			dpk= dname.dpk;
			return *this;
		}
		bool operator<(CDefultPK dname) const
		{
			return ((dbName+tableName+dpk).compare(dname.dbName+dname.tableName+dname.dpk)<0);
		}
		bool operator>(CDefultPK dname) const
		{
			return ((dbName+tableName+dpk).compare(dname.dbName+dname.tableName+dname.dpk)>0);
		}
		bool operator==(CDefultPK dname) const
		{
			return ((dbName+tableName+dpk).compare(dname.dbName+dname.tableName+dname.dpk)==0);
		}
		bool isEmpty(void)
		{
			if(dbName.empty()||tableName.empty()||dpk.empty())
				return true;
			return false;
		}

		void Display()
		{
			cout<<"      dpk:"<<dpk.c_str()<<"  tableName:"<<tableName.c_str()<<"  dbName:"<<dbName.c_str()<<endl;
		}

		string dbName;
		string tableName;
		string dpk;
	};//end of class CDefultPK


} // end of namespace svbase


using namespace svbase;


#endif



