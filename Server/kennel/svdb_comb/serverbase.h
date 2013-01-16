

#ifndef	SITEVIEW_SERVERBASE_H
#define	SITEVIEW_SERVERBASE_H


#include "svbase.h"
#include "svapi.h"
#include "svdbtype.h"
#include <cc++/thread.h>
using ost::Mutex;



namespace svfound{

	//non-threadsafe
	class CConfig
	{
	public:
		CConfig(string tname);
		CConfig(string tname, string ver);
		~CConfig(void);
		
		S_UINT	GetRawDataSize();
		char*	GetRawData(char *lpbuf,S_UINT bufsize);	
		bool	CreateByRawData(const char *lpbuf,S_UINT bufsize); // will clear all data, then reset all

		bool	CheckClientVerIsLatest(string clientver, string & server_version, string & estr);
		bool	AnalyseDataToSetVersion(string & estr);

		// for server
		bool	GetHistory(StrMap & ndata, string & estr);
		bool	PushVersion(string ver, string & estr);
		bool	ClearInvalidVersion();
		void	DisplayDataMembers();
		CTableName GetTableName();

	private:
		string			mf_tablename;
		string			mf_currentver;

		MapStrMap		mf_values;
		MapStrMap		mf_advance;	
	}; // end of class CConfig


	//thread safe
	typedef map<string,CConfig *> MapTableP; 
	class CConfigFile
	{
	public:
		CConfigFile(string dbname, string path, string historypath, string version);
		~CConfigFile(void);

		// for client
		bool UpdateCConfig(CTableName tableName, CConfig * ctable, string & estr);
		bool UpdateBatchCConfig( map<CTableName,CConfig *> & tables, string & estr);

		CTableName GetRecommendNextTableName();
		CTableName GetChildTableName(CTableName parentTableName);
		
		bool CreateNewTable(CTableName newTableName,  CConfig * ctable, string & estr);
		bool CheckNewTableNameAvailable(CTableName newTableName, string & estr);

		bool DeleteTable(CTableName tableName, string & estr);
		bool GetAllTableName(set<CTableName> & tnames);

		string GetCurrentVersion();

		bool SetRevertNum(int historyNum, string & estr);
		int	 GetNumOfRevertSetting();

		bool RevertDataBaseToVersion(string oldver, string & estr);

		// for server
		string SetNewVersionAndBackupOld(string & estr);
		CConfig * GetTable(CTableName tableName, string & estr);

		S_UINT GetRawDataSize();
		char*  GetRawData(char *lpbuf,S_UINT bufsize);
		bool   CreateByRawData(const char *lpbuf,S_UINT bufsize);//non-threadsafe;	will clear all data, then reset all

		bool SaveData(string & estr);	//save database; should call this func  manually after any changing
		bool LoadData(string & estr);	//non-threadsafe;	load database; 		
		
		void DisplayDataMembers();

	private:

		int			mf_version;
		bool		mf_modified;
		string		mf_dbname;
		string		mf_configdbFile;
		string		mf_configdbPath;
		string		mf_configdbHistoryPath;
		
		StrMap		mf_advance;
		MapTableP	mf_tables;

		Mutex		mf_LockDataBase;

	}; // end of class CConfigFile

	

	//thread safe
	typedef map<string,CConfigFile *> MapCFileP; 
	class CWholeConfig
	{
	public:
		CWholeConfig(string fname, string path, string historypath);
		~CWholeConfig(void);

		// for client
		bool CreateNewDataBase(string dbname, string message, string & estr);
		bool CheckNewDataBaseNameAvailable(string dbname, string & estr);

		bool DeleteDataBase(string dbname, string & estr);
		bool GetAllDataBaseName(set<string> & dnames);

		bool GetVersionHistory(string dbname, StrMap & ndata,const StrMap & needs, string & estr);
		
		// for server
		CConfigFile * GetDataBase(string dbname, string & estr);
		bool PushVersion(string dbname, string ver, string message, string & estr);

		bool SaveData(string & estr);	//should call this func  manually after any changing
		bool LoadData(string & estr);	//non-threadsafe;

		void DisplayDataMembers();

	private:
		string		mf_configdbIndexFile;
		string		mf_configdbPath;
		string		mf_configdbHistoryPath;

		bool		mf_modified;
		MapCFileP	mf_files;

		StrMap		mf_nameversion;
		MapStrMap   mf_historymap;

		Mutex		mf_LockFileNames;	

	}; // end of class CWholeConfig


    

} // end of namespace svfound


using namespace svfound;


#endif




