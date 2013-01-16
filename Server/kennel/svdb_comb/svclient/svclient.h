
#ifndef	SITEVIEW_SVCLIENT_DLL_H
#define	SITEVIEW_SVCLIENT_DLL_H


#ifdef SVCLIENT_EXPORTS
#define SVCLIENT_API  __declspec(dllexport)
#else
#define SVCLIENT_API  __declspec(dllimport)
#endif



#include "svbase.h"


namespace svbase{




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//以下6个函数都线程安全（都直接/间接跟 CSvTable 类有关联）
	//注意: 从以下函数得到的 CSvTable * 指针,  务必不可执行"delete 指针"操作,  该指针指向的内容由 svclient.dll 自动管理
	class CSvTable;

	//从服务器取得最新数据，并缓存在本进程中; 如果一个本地 table 已经改动了，将会被从服务器新获取的数据覆盖
	//注意 ----------------------------  注意  ----------------------  注意 
	//注意: 从本函数得到的 CSvTable * 指针,  务必不可执行"delete 指针"操作,  该指针指向的内容由 svclient.dll 自动管理
	SVCLIENT_API
		CSvTable * GetTableFromServer(CTableName tname, string & estr);			


	//从本进程缓存中取得数据，不询问服务器, 不保证取得的数据是最新的; 会自动进行首次获取(即如果对某个表首次调用该函数,会自动从服务器进行首次获取)
	//注意 ----------------------------  注意  ----------------------  注意 
	//注意: 从本函数得到的 CSvTable * 指针,  务必不可执行"delete 指针"操作,  该指针指向的内容由 svclient.dll 自动管理
	SVCLIENT_API
		CSvTable * GetTableFromCache(CTableName tname, string & estr);			




	//批量从服务器取得最新数据，并缓存在本进程中; 如果一个本地 table 已经改动了，将会被从服务器新获取的数据覆盖
	SVCLIENT_API
		bool RefreshTables(const std::set<CTableName> & list_tname, string & estr);	

	//批量从服务器取得整个数据库所有表的最新数据，并缓存在本进程中; 如果一个本地 table 已经改动了，将会被从服务器新获取的数据覆盖
	SVCLIENT_API
		bool RefreshDataBase(string dbname, string & estr);	




	//提交一个表的数据给服务器;  执行完本函数后,本进程中缓存的数据将老于服务器端
	// ptable			传入的表
	// message			关于这次数据提交的备忘消息，在数据库回滚时将用上， 该参数不能为空
	// estr				服务器返回的错误信息，便于调试, 多条信息以 ; 分隔
	SVCLIENT_API
		bool SubmitTable(CSvTable * ptable, string message, string & estr);	


	//提交本进程缓存中,所有修改过的表给服务器;  执行完本函数后,本进程中缓存的数据将老于服务器端
	SVCLIENT_API
		bool SubmitAllCache(string message, string & estr);




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//此类的所有方法(除了 IsLatestVersion )都是本地操作，跟服务器无关
	//此类的所有写操作都非线程安全，  所有读操作都线程安全
	class SVCLIENT_API CSvTable
	{
	public:
		CSvTable(void);
		~CSvTable(void);

		enum fieldtype { STR_T,INT_T,FLOAT_T,  PK_T,FK_T,LANGUAGE_T  ,ERROR_T }; 

		//查询本地缓存数据是否是服务器上最新的数据, 必要时需要与服务器通讯
		//如果本地缓存数据修改过，本方法不询问服务器，直接返回 false
		bool IsLatestVersion(string & estr);

		CTableName GetTableName();
		string GetTableDescription();
		bool SetTableDescription(string des);

		//获取建议的下一个默认主键
		//（每一行记录都必须有一个默认主键，另外，每一行记录还可以由用户再创建其他的主键）
		CDefultPK GetRecommendNextDPK();
		
		//检查用户自己取的默认主键，是否可用
		bool CheckNewDPKAvailable(CDefultPK dpk);

		const StrMap & GetFields();
		const StrMap & GetRelations();

		int  GetFieldType(CDefultPK dpk, string field);

		bool GetValue(CDefultPK dpk, string field, string    & value);  //适用字段类型： STR_T, PK_T
		bool GetValue(CDefultPK dpk, string field, int	     & value);  //适用字段类型： INT_T
		bool GetValue(CDefultPK dpk, string field, float	 & value);  //适用字段类型： FLOAT_T
		bool GetValue(CDefultPK dpk, string field, CDefultPK & value);  //适用字段类型： FK_T，LANGUAGE_T

		bool SetValue(CDefultPK dpk, string field, string    value);    //适用字段类型： STR_T, PK_T
		bool SetValue(CDefultPK dpk, string field, int	     value);    //适用字段类型： INT_T
		bool SetValue(CDefultPK dpk, string field, float     value);    //适用字段类型： FLOAT_T
		bool SetValue(CDefultPK dpk, string field, CDefultPK value);    //适用字段类型： FK_T，LANGUAGE_T

		bool IsModified();
		string GetVersion();

		//显示数据成员，用于 console 程序的调试
		void	DisplayDataMembers();

	private:
		bool GetGeneralValue(CDefultPK dpk, string field, string & value);
		bool SetGeneralValue(CDefultPK dpk, string field, string   value);

		bool		m_modified;
		string		m_version;

		//储存所有的值
		MapStrMap	m_values;

		//内有 atrribute、fields、relations、dpks 等 StrMap
		//	attribute	内有 version (版本戳)，tablename( 表名,即表的 id ,数据库内唯一), dbname(所属的数据库名)，des(表的描述,用于显示) 等等
		//	fields		所有字段 (字段类型：int/string/float,pk/fk/ne/index/language, ... and more)
		//	relations	所有关系 (关系类型: one-one/one-many, ... and more; describe record relation)
		//  history		所有历史版本号及其备注消息, 数据库回滚预留
		MapStrMap	m_advance;	

		friend class CSvTableComm;

	}; // end of class CSvTable


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//此类的所有方法（除了 GetDataBaseName）都需要跟服务器通讯
	class SVCLIENT_API CSvDataBase
	{
	public:
		CSvDataBase(string dbname);
		~CSvDataBase(void);

		CTableName GetRecommendNextTableName(string & estr);
		CTableName GetChildTableName(CTableName parentTableName,string & estr);

		//newTableName.tableName 不能包含空格, 不能重复使用
		bool CheckNewTableNameAvailable(CTableName newTableName, string & estr);
		bool CreateNewTable(CTableName newTableName, string message, string & estr);

		bool DeleteTable(CTableName tableName, string message, string & estr);
		bool GetAllTableName(set<CTableName> & tnames,string & estr);

		bool GetTableVersionHistory(CTableName tableName, map<int,string> & verhis, string & estr);
		bool GetDBVersionHistory(map<int,string> & verhis, string & estr);

		//historyNum 回滚备份数量（超过此数量的老版本将删除）,该数字不能大于 1000; 所有新建的数据库，默认回滚数量是20
		bool EnableRevert(int historyNum, string message, string & estr);
		bool DisableRevert(string message, string & estr);
		bool GetNumOfRevertSetting(int & historyNum, string & estr);
		bool RevertDataBaseToVersion(int oldver, string message, string & estr);

		string GetDataBaseName()
		{
			return m_dbname;
		}

	private:
		string	m_dbname;

	}; // end of class CSvDataBase


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	//此类的所有方法都需要跟服务器通讯
	class SVCLIENT_API CAllDataBase
	{
	public:
		CAllDataBase(void){}
		~CAllDataBase(void){}

		bool CreateNewDataBase(string dbname, string message, string & estr);

		//dbname 不能包含点"."  ,不能包含空格, 不能重复使用
		bool CheckNewDataBaseNameAvailable(string dbname, string & estr);

		//数据库一旦删除后若要恢复，需停止服务器及繁琐的手动操作
		bool DeleteDataBase(string dbname, string & estr);
		bool GetAllDataBaseName(set<string> & dnames, string & estr);

	}; // end of class CAllDataBase




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	////non-threadsafe
	////the class provide advance sql function, and will be complex
	//typedef set<svbase::CDefultPK> DPKIndex;			//dpk1,dpk2 ...
	//typedef map<string,svbase::DPKIndex> VALUEIndex;	// value1- DPKIndex1, value2- DPKIndex2 ...
	//typedef map<string,svbase::VALUEIndex> TABLEIndex;	//   field1- VALUEIndex1, field2- VALUEIndex2
	//class SVCLIENT_API CSvView
	//{
	//public:
	//	CSvView(void){}
	//	~CSvView(void){}
	//private:
	//	TABLEIndex m_tableindex;
	//}; // end of class CSvView



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//设置客户端进程与 svdb.exe 通讯的地址
	//退设置的方法: SetSvdbAddrByFile(""); 这样会返回false 并同时取消之前的设置;   任何使该函数返回false的情况,都会退设置。
	//不设置或退设置的通讯地址都为 "localhost"
	SVCLIENT_API 
		bool SetSvdbAddrByFile(string filename);
	
	//获取上一函数设置的通讯地址
	SVCLIENT_API 
		string GetSvdbAddr(void);


	//读取客户端所在机器的注册表,而不是 svdb.exe 所在机器的注册表
	SVCLIENT_API 
		string	GetSiteViewRootPath();



	/////////////////下面这个函数非线程安全(建议用到时只在程序启动时调用一次)//////////////////

	SVCLIENT_API
		void SetServerPort(unsigned short port);

	SVCLIENT_API
		unsigned short GetServerPort();




} // end of namespace svbase

using namespace svbase;


#endif


