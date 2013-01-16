//
//  MODULE:   Ado.h
//
//	AUTHOR: Carlos Antollini 
//
//	Date: 6/28/2001 
//  
//	Version 1.2
//

#ifndef _ADO_H_
#define _ADO_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "stdafx.h"
//#include "resource.h"
// CG : In order to use this code against a different version of ADO, the appropriate
// ADO library needs to be used in the #import statement
#import "C:\Program Files\Common Files\System\ADO\msado15.dll" rename_namespace("ADOCG") rename("EOF", "EndOfFile")
using namespace ADOCG;
#include "icrsint.h"

struct CADOFieldInfo
{
	char m_strName[30]; 
	short m_nType;
	long m_lSize; 
	long m_lDefinedSize;
	long m_lAttributes;
	short m_nOrdinalPosition;
	BOOL m_bRequired;   
	BOOL m_bAllowZeroLength; 
	long m_lCollatingOrder;  
};

CString IntToStr(int nVal);

CString LongToStr(long lVal);

class CADODatabase
{
public:
	bool Execute(LPCTSTR lpstrExec);
	CADODatabase()
	{
		::CoInitialize(NULL);
			
		m_pConnection = NULL;
		m_strConnection = _T("");
		m_pConnection.CreateInstance(__uuidof(Connection));
	}
	
	~CADODatabase()
	{
		Close();
		m_pConnection.Release();
		m_pConnection = NULL;
		::CoUninitialize();
	}
	
	bool Open(LPCTSTR lpstrConnection = _T(""));
	_ConnectionPtr GetActiveConnection() {return m_pConnection;};
	DWORD GetRecordCount(_RecordsetPtr m_pRs);
	long BeginTransaction() 
		{return m_pConnection->BeginTrans();};
	long CommitTransaction() 
		{return m_pConnection->CommitTrans();};
	long RollbackTransaction() 
		{return m_pConnection->RollbackTrans();};
	bool IsOpen();
	void Close();
	void SetConnectionString(LPCTSTR lpstrConnection)
		{m_strConnection = lpstrConnection;};
	CString GetConnectionString()
		{return m_strConnection;};
	CString GetLastError() 
		{return m_strLastError;};
protected:
	void dump_com_error(_com_error &e);

protected:
	_ConnectionPtr m_pConnection;
	CString m_strConnection;
	CString m_strLastError;
};

class CADORecordset
{
public:
	enum cadoOpenEnum
	{
		openUnknown = 0,
		openQuery = 1,
		openTable = 2,
		openStoredProc = 3
	};

	enum cadoEditEnum
	{
		dbEditNone = 0,
		dbEditNew = 1,
		dbEdit = 2
	};
	
	enum cadoPositionEnum
	{
	
		positionUnknown = -1,
		positionBOF = -2,
		positionEOF = -3
	};
	
	enum cadoSearchEnum
	{	
		searchForward = 1,
		searchBackward = -1
	};

	
	bool SetFieldValue(int nIndex, CString strValue);
	bool SetFieldValue(LPCTSTR lpFieldName, CString strValue);
	bool SetFieldValue(int nIndex, int nValue);
	bool SetFieldValue(LPCTSTR lpFieldName, int nValue);
	bool SetFieldValue(int nIndex, long lValue);
	bool SetFieldValue(LPCTSTR lpFieldName, long lValue);
	bool SetFieldValue(int nIndex, double dblValue);
	bool SetFieldValue(LPCTSTR lpFieldName, double dblValue);
	bool SetFieldValue(int nIndex, COleDateTime time);
	bool SetFieldValue(LPCTSTR lpFieldName, COleDateTime time);

	void CancelUpdate();
	bool Update();
	void Edit();
	bool AddNew();

	bool Find(LPCTSTR lpFind, int nSearchDirection = CADORecordset::searchForward);
	bool FindFirst(LPCTSTR lpFind);
	bool FindNext();

	CADORecordset()
	{
		m_pRecordset = NULL;
		m_pCmd = NULL;
		m_strQuery = _T("");
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		m_pCmd.CreateInstance(__uuidof(Command));
		m_nEditStatus = CADORecordset::dbEditNone;
		m_nSearchDirection = CADORecordset::searchForward;
	}

	CADORecordset(CADODatabase* pAdoDatabase);

	~CADORecordset()
	{
		Close();
		m_pRecordset.Release();
		m_pCmd.Release();
		m_pRecordset = NULL;
		m_pCmd = NULL;
		m_strQuery = _T("");
		m_nEditStatus = dbEditNone;
	}

	CString GetQuery() 
		{return m_strQuery;};
	void SetQuery(LPCSTR strQuery) 
		{m_strQuery = strQuery;};
	bool RecordBinding(CADORecordBinding &pAdoRecordBinding);
	DWORD GetRecordCount();
	bool IsOpen();
	void Close();
	bool Open(_ConnectionPtr mpdb, LPCTSTR lpstrExec = _T(""), int nOption = CADORecordset::openUnknown);
	bool Open(LPCTSTR lpstrExec = _T(""), int nOption = CADORecordset::openUnknown);
	bool GetFieldValue(LPCTSTR lpFieldName, double& dbValue);
	bool GetFieldValue(int nIndex, double& dbValue);
	bool GetFieldValue(LPCTSTR lpFieldName, long& lValue);
	bool GetFieldValue(int nIndex, long& lValue);
	bool GetFieldValue(LPCTSTR lpFieldName, int& nValue);
	bool GetFieldValue(int nIndex, int& nValue);
	bool GetFieldValue(LPCTSTR lpFieldName, CString& strValue);
	bool GetFieldValue(int nIndex, CString& strValue);
	bool GetFieldValue(LPCTSTR lpFieldName, COleDateTime& time);
	bool GetFieldValue(int nIndex, COleDateTime& time);
	bool IsFieldNull(LPCTSTR lpFieldName);
	bool IsFieldNull(int nIndex);
	bool IsFieldEmpty(LPCTSTR lpFieldName);
	bool IsFieldEmpty(int nIndex);	
	bool IsEof()
		{return m_pRecordset->EndOfFile == VARIANT_TRUE;};
	bool IsEOF()
		{return m_pRecordset->EndOfFile == VARIANT_TRUE;};
	bool IsBof()
		{return m_pRecordset->BOF == VARIANT_TRUE;};
	bool IsBOF()
		{return m_pRecordset->BOF == VARIANT_TRUE;};
	void MoveFirst() 
		{m_pRecordset->MoveFirst();};
	void MoveNext() 
		{m_pRecordset->MoveNext();};
	void MovePrevious() 
		{m_pRecordset->MovePrevious();};
	void MoveLast() 
		{m_pRecordset->MoveLast();};
	long GetAbsolutePage()
		{return m_pRecordset->GetAbsolutePage();};
	void SetAbsolutePage(int nPage)
		{m_pRecordset->PutAbsolutePage((enum PositionEnum)nPage);};
	long GetPageCount()
		{return m_pRecordset->GetPageCount();};
	long GetPageSize()
		{return m_pRecordset->GetPageSize();};
	void SetPageSize(int nSize)
		{m_pRecordset->PutPageSize(nSize);};
	long GetAbsolutePosition()
		{return m_pRecordset->GetAbsolutePosition();};
	void SetAbsolutePosition(int nPosition)
		{m_pRecordset->PutAbsolutePosition((enum PositionEnum)nPosition);};
	bool GetFieldInfo(LPCTSTR lpFieldName, CADOFieldInfo* fldInfo);
	bool GetFieldInfo(int nIndex, CADOFieldInfo* fldInfo);
	bool GetChunk(LPCTSTR lpFieldName, CString& strValue);
	CString GetString(LPCTSTR lpCols, LPCTSTR lpRows, LPCTSTR lpNull, long numRows = 0);
	CString GetLastError() {return m_strLastError;};
	void GetBookmark()
		{m_varBookmark = m_pRecordset->Bookmark;};
	bool SetBookmark();
	bool Delete();

protected:
	_ConnectionPtr m_pConnection;
	int m_nSearchDirection;
	CString m_strFind;
	_variant_t m_varBookFind;
	_variant_t m_varBookmark;
	int m_nEditStatus;
	CString m_strLastError;
	void dump_com_error(_com_error &e);
	_RecordsetPtr m_pRecordset;
	_CommandPtr m_pCmd;
	CString m_strQuery;
};

#endif

