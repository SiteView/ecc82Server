#if !defined(_ANYOU_COOL_ADO_H)
#define _ANYOU_COOL_ADO_H

#if _MSC_VER > 1000
#pragma once
#endif 
 
// 导入 ado 库 -----------------------------------------------------------
#pragma warning(disable:4146)
#import "C:\Program Files\Common Files\System\ADO\msado15.dll" named_guids rename("EOF","adoEOF"), rename("BOF","adoBOF")
#pragma warning(default:4146)
using namespace ADODB;  

#include <icrsint.h>
class CAdoConnection;
#include "AdoRecordSet.h"
#include "AdoCommand.h"

// 数值类型转换 -----------------------------------
COleDateTime vartodate(const _variant_t& var);
COleCurrency vartocy(const _variant_t& var);
bool vartobool(const _variant_t& var);
BYTE vartoby(const _variant_t& var);
short vartoi(const _variant_t& var);
long vartol(const _variant_t& var);
double vartof(const _variant_t& var);
CString vartostr(const _variant_t& var);

/*########################################################################
			  ------------------------------------------------
						    CAdoConnection class
			  ------------------------------------------------
  ########################################################################*/
class CAdoConnection
{
// 构建/析构 ------------------------------------------
public:
	CAdoConnection();
	virtual ~CAdoConnection();

protected:
	void Release();

// 属性 -----------------------------------------------
public:
	// 连接对象 ----------------------------------
	_ConnectionPtr& GetConnection() {return m_pConnection;};

	// 异常信息 ----------------------------------
	CString GetLastErrorText();
	ErrorsPtr GetErrors();
	ErrorPtr  GetError(long index);

	// 连接字串 ----------------------------------
	CString GetConnectionText() {return m_strConnect;}

	// 连接信息 ----------------------------------
	CString GetProviderName();
	CString GetVersion();
	CString GetDefaultDatabase();

	// 连接状态 ----------------------------------
	BOOL IsOpen();
	long GetState();
	
	// 连接模式 ----------------------------------
	ConnectModeEnum GetMode();
	BOOL SetMode(ConnectModeEnum mode);

	// 连接时间 ----------------------------------
	long GetConnectTimeOut();
	BOOL SetConnectTimeOut(long lTime = 5);
	
	// 数据源信息 -------------------------------
	_RecordsetPtr OpenSchema(SchemaEnum QueryType);

// 操作 -----------------------------------------------
public:
	// 数据库连接 --------------------------------
	BOOL Open(LPCTSTR lpszConnect =_T(""), long lOptions = adConnectUnspecified);
	BOOL ConnectSQLServer(CString dbsrc, CString dbname, CString user, CString pass, long lOptions = adConnectUnspecified);
	BOOL ConnectAccess(CString dbpath, CString pass = _T(""), long lOptions = adConnectUnspecified);
	BOOL OpenUDLFile(LPCTSTR strFileName, long lOptions = adConnectUnspecified);
	void Close();

// 处理 -----------------------------------------------
public:
	// 事务处理 ----------------------------------
	long BeginTrans();
	BOOL RollbackTrans();
	BOOL CommitTrans();

	// 执行 SQL 语句 ------------------------------
	_RecordsetPtr Execute(LPCTSTR strSQL, long lOptions = adCmdText);
	BOOL Cancel();

// 数据 -----------------------------------------------
protected:
	CString			m_strConnect;
	_ConnectionPtr	m_pConnection;
};

#endif // !defined(_ANYOU_COOL_ADO_H)










































