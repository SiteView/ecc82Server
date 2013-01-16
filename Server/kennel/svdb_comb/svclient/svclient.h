
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


	//����6���������̰߳�ȫ����ֱ��/��Ӹ� CSvTable ���й�����
	//ע��: �����º����õ��� CSvTable * ָ��,  ��ز���ִ��"delete ָ��"����,  ��ָ��ָ��������� svclient.dll �Զ�����
	class CSvTable;

	//�ӷ�����ȡ���������ݣ��������ڱ�������; ���һ������ table �Ѿ��Ķ��ˣ����ᱻ�ӷ������»�ȡ�����ݸ���
	//ע�� ----------------------------  ע��  ----------------------  ע�� 
	//ע��: �ӱ������õ��� CSvTable * ָ��,  ��ز���ִ��"delete ָ��"����,  ��ָ��ָ��������� svclient.dll �Զ�����
	SVCLIENT_API
		CSvTable * GetTableFromServer(CTableName tname, string & estr);			


	//�ӱ����̻�����ȡ�����ݣ���ѯ�ʷ�����, ����֤ȡ�õ����������µ�; ���Զ������״λ�ȡ(�������ĳ�����״ε��øú���,���Զ��ӷ����������״λ�ȡ)
	//ע�� ----------------------------  ע��  ----------------------  ע�� 
	//ע��: �ӱ������õ��� CSvTable * ָ��,  ��ز���ִ��"delete ָ��"����,  ��ָ��ָ��������� svclient.dll �Զ�����
	SVCLIENT_API
		CSvTable * GetTableFromCache(CTableName tname, string & estr);			




	//�����ӷ�����ȡ���������ݣ��������ڱ�������; ���һ������ table �Ѿ��Ķ��ˣ����ᱻ�ӷ������»�ȡ�����ݸ���
	SVCLIENT_API
		bool RefreshTables(const std::set<CTableName> & list_tname, string & estr);	

	//�����ӷ�����ȡ���������ݿ����б���������ݣ��������ڱ�������; ���һ������ table �Ѿ��Ķ��ˣ����ᱻ�ӷ������»�ȡ�����ݸ���
	SVCLIENT_API
		bool RefreshDataBase(string dbname, string & estr);	




	//�ύһ��������ݸ�������;  ִ���걾������,�������л�������ݽ����ڷ�������
	// ptable			����ı�
	// message			������������ύ�ı�����Ϣ�������ݿ�ع�ʱ�����ϣ� �ò�������Ϊ��
	// estr				���������صĴ�����Ϣ�����ڵ���, ������Ϣ�� ; �ָ�
	SVCLIENT_API
		bool SubmitTable(CSvTable * ptable, string message, string & estr);	


	//�ύ�����̻�����,�����޸Ĺ��ı��������;  ִ���걾������,�������л�������ݽ����ڷ�������
	SVCLIENT_API
		bool SubmitAllCache(string message, string & estr);




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//��������з���(���� IsLatestVersion )���Ǳ��ز��������������޹�
	//���������д���������̰߳�ȫ��  ���ж��������̰߳�ȫ
	class SVCLIENT_API CSvTable
	{
	public:
		CSvTable(void);
		~CSvTable(void);

		enum fieldtype { STR_T,INT_T,FLOAT_T,  PK_T,FK_T,LANGUAGE_T  ,ERROR_T }; 

		//��ѯ���ػ��������Ƿ��Ƿ����������µ�����, ��Ҫʱ��Ҫ�������ͨѶ
		//������ػ��������޸Ĺ�����������ѯ�ʷ�������ֱ�ӷ��� false
		bool IsLatestVersion(string & estr);

		CTableName GetTableName();
		string GetTableDescription();
		bool SetTableDescription(string des);

		//��ȡ�������һ��Ĭ������
		//��ÿһ�м�¼��������һ��Ĭ�����������⣬ÿһ�м�¼���������û��ٴ���������������
		CDefultPK GetRecommendNextDPK();
		
		//����û��Լ�ȡ��Ĭ���������Ƿ����
		bool CheckNewDPKAvailable(CDefultPK dpk);

		const StrMap & GetFields();
		const StrMap & GetRelations();

		int  GetFieldType(CDefultPK dpk, string field);

		bool GetValue(CDefultPK dpk, string field, string    & value);  //�����ֶ����ͣ� STR_T, PK_T
		bool GetValue(CDefultPK dpk, string field, int	     & value);  //�����ֶ����ͣ� INT_T
		bool GetValue(CDefultPK dpk, string field, float	 & value);  //�����ֶ����ͣ� FLOAT_T
		bool GetValue(CDefultPK dpk, string field, CDefultPK & value);  //�����ֶ����ͣ� FK_T��LANGUAGE_T

		bool SetValue(CDefultPK dpk, string field, string    value);    //�����ֶ����ͣ� STR_T, PK_T
		bool SetValue(CDefultPK dpk, string field, int	     value);    //�����ֶ����ͣ� INT_T
		bool SetValue(CDefultPK dpk, string field, float     value);    //�����ֶ����ͣ� FLOAT_T
		bool SetValue(CDefultPK dpk, string field, CDefultPK value);    //�����ֶ����ͣ� FK_T��LANGUAGE_T

		bool IsModified();
		string GetVersion();

		//��ʾ���ݳ�Ա������ console ����ĵ���
		void	DisplayDataMembers();

	private:
		bool GetGeneralValue(CDefultPK dpk, string field, string & value);
		bool SetGeneralValue(CDefultPK dpk, string field, string   value);

		bool		m_modified;
		string		m_version;

		//�������е�ֵ
		MapStrMap	m_values;

		//���� atrribute��fields��relations��dpks �� StrMap
		//	attribute	���� version (�汾��)��tablename( ����,����� id ,���ݿ���Ψһ), dbname(���������ݿ���)��des(�������,������ʾ) �ȵ�
		//	fields		�����ֶ� (�ֶ����ͣ�int/string/float,pk/fk/ne/index/language, ... and more)
		//	relations	���й�ϵ (��ϵ����: one-one/one-many, ... and more; describe record relation)
		//  history		������ʷ�汾�ż��䱸ע��Ϣ, ���ݿ�ع�Ԥ��
		MapStrMap	m_advance;	

		friend class CSvTableComm;

	}; // end of class CSvTable


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//��������з��������� GetDataBaseName������Ҫ��������ͨѶ
	class SVCLIENT_API CSvDataBase
	{
	public:
		CSvDataBase(string dbname);
		~CSvDataBase(void);

		CTableName GetRecommendNextTableName(string & estr);
		CTableName GetChildTableName(CTableName parentTableName,string & estr);

		//newTableName.tableName ���ܰ����ո�, �����ظ�ʹ��
		bool CheckNewTableNameAvailable(CTableName newTableName, string & estr);
		bool CreateNewTable(CTableName newTableName, string message, string & estr);

		bool DeleteTable(CTableName tableName, string message, string & estr);
		bool GetAllTableName(set<CTableName> & tnames,string & estr);

		bool GetTableVersionHistory(CTableName tableName, map<int,string> & verhis, string & estr);
		bool GetDBVersionHistory(map<int,string> & verhis, string & estr);

		//historyNum �ع������������������������ϰ汾��ɾ����,�����ֲ��ܴ��� 1000; �����½������ݿ⣬Ĭ�ϻع�������20
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



	//��������з�������Ҫ��������ͨѶ
	class SVCLIENT_API CAllDataBase
	{
	public:
		CAllDataBase(void){}
		~CAllDataBase(void){}

		bool CreateNewDataBase(string dbname, string message, string & estr);

		//dbname ���ܰ�����"."  ,���ܰ����ո�, �����ظ�ʹ��
		bool CheckNewDataBaseNameAvailable(string dbname, string & estr);

		//���ݿ�һ��ɾ������Ҫ�ָ�����ֹͣ���������������ֶ�����
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


	//���ÿͻ��˽����� svdb.exe ͨѶ�ĵ�ַ
	//�����õķ���: SetSvdbAddrByFile(""); �����᷵��false ��ͬʱȡ��֮ǰ������;   �κ�ʹ�ú�������false�����,���������á�
	//�����û������õ�ͨѶ��ַ��Ϊ "localhost"
	SVCLIENT_API 
		bool SetSvdbAddrByFile(string filename);
	
	//��ȡ��һ�������õ�ͨѶ��ַ
	SVCLIENT_API 
		string GetSvdbAddr(void);


	//��ȡ�ͻ������ڻ�����ע���,������ svdb.exe ���ڻ�����ע���
	SVCLIENT_API 
		string	GetSiteViewRootPath();



	/////////////////��������������̰߳�ȫ(�����õ�ʱֻ�ڳ�������ʱ����һ��)//////////////////

	SVCLIENT_API
		void SetServerPort(unsigned short port);

	SVCLIENT_API
		unsigned short GetServerPort();




} // end of namespace svbase

using namespace svbase;


#endif


