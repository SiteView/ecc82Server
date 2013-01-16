//
//  MODULE:   Ado.cpp
//
//	AUTHOR: Carlos Antollini 
//
//	Date: 6/18/2001
//
//	Version 1.2
// 

#include "ado.h"

DWORD CADODatabase::GetRecordCount(_RecordsetPtr m_pRs)
{
	DWORD numRows = 0;
	
	numRows = m_pRs->GetRecordCount();

	if(numRows == -1)
	{
		if(m_pRs->EndOfFile != VARIANT_TRUE)
			m_pRs->MoveFirst();

		while(m_pRs->EndOfFile != VARIANT_TRUE)
		{
			numRows++;
			m_pRs->MoveNext();
		}
		if(numRows > 0)
			m_pRs->MoveFirst();
	}
	return numRows;
}

bool CADODatabase::Open(LPCTSTR lpstrConnection)
{
	HRESULT hr = S_OK;

	if(IsOpen())
		Close();

	if(strcmp(lpstrConnection, _T("")) != 0)
		m_strConnection = lpstrConnection;

	ASSERT(!m_strConnection.IsEmpty());

	try
	{
		hr = m_pConnection->Open(_bstr_t(m_strConnection), "", "", NULL);
		return hr == S_OK;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}
	return false;
}

void CADODatabase::dump_com_error(_com_error &e)
{
	CString ErrorStr;
	
	
	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());
	ErrorStr.Format( "CADODataBase Error\n\tCode = %08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s\n",
		e.Error(), e.ErrorMessage(), (LPCSTR)bstrSource, (LPCSTR)bstrDescription );
	m_strLastError = _T("Connection String = " + GetConnectionString() + '\n' + ErrorStr);
	#ifdef _DEBUG
		AfxMessageBox( ErrorStr, MB_OK | MB_ICONERROR );
	#endif	
}

bool CADODatabase::IsOpen()
{
	if(m_pConnection )
		return m_pConnection->GetState() != adStateClosed;
	return false;
}

void CADODatabase::Close()
{
	if(IsOpen())
		m_pConnection->Close();
}


CADORecordset::CADORecordset(CADODatabase* pAdoDatabase)
{
	m_pRecordset = NULL;
	m_pCmd = NULL;
	m_strQuery = _T("");
	m_pRecordset.CreateInstance(__uuidof(Recordset));
	m_pCmd.CreateInstance(__uuidof(Command));
	m_nEditStatus = CADORecordset::dbEditNone;
	m_nSearchDirection = CADORecordset::searchForward;

	m_pConnection = pAdoDatabase->GetActiveConnection();
}

bool CADORecordset::Open(_ConnectionPtr mpdb, LPCTSTR lpstrExec, int nOption)
{	
	Close();
	
	if(strcmp(lpstrExec, _T("")) != 0)
		m_strQuery = lpstrExec;

	ASSERT(!m_strQuery.IsEmpty());
	
	m_strQuery.TrimLeft();
	BOOL bIsSelect = m_strQuery.Mid(0, strlen("Select ")).CompareNoCase("select ") == 0;

	try
	{
		m_pRecordset->CursorLocation = adUseClient;
		if(bIsSelect || nOption == openQuery)
			m_pRecordset->Open((LPCSTR)m_strQuery, _variant_t((IDispatch*)mpdb, true), 
							adOpenStatic, adLockOptimistic, adCmdUnknown);
		else if(nOption == openTable)
			m_pRecordset->Open((LPCSTR)m_strQuery, _variant_t((IDispatch*)mpdb, true), 
							adOpenDynamic, adLockOptimistic, adCmdTable);
		else if(nOption == openStoredProc)
		{
			m_pCmd->ActiveConnection = mpdb;
			m_pCmd->CommandText = _bstr_t(m_strQuery);
			m_pCmd->CommandType = adCmdStoredProc;
			
			m_pRecordset = m_pCmd->Execute(NULL, NULL, adCmdText);
		}
		else
		{
			TRACE( "Unknown parameter. %d", nOption);
			return false;
		}
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return false;
	}

	return m_pRecordset != NULL;
}

bool CADORecordset::Open(LPCTSTR lpstrExec, int nOption)
{
	ASSERT(m_pConnection != NULL);
	return Open(m_pConnection, lpstrExec, nOption);
}

bool CADORecordset::GetFieldValue(LPCTSTR lpFieldName, double& dbValue)
{	
	double val = (double)NULL;
	_variant_t vtFld;
	
	vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
	if(vtFld.vt != VT_NULL)
		val = vtFld.dblVal;
	dbValue = val;
	return true;
}


bool CADORecordset::GetFieldValue(int nIndex, double& dbValue)
{	
	double val = (double)NULL;
	_variant_t vtFld;
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	
	vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
	if(vtFld.vt != VT_NULL)
		val = vtFld.dblVal;
	dbValue = val;
	return true;
}


bool CADORecordset::GetFieldValue(LPCTSTR lpFieldName, long& lValue)
{
	long val = (long)NULL;
	_variant_t vtFld;
	
	vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
	if(vtFld.vt != VT_NULL)
		val = vtFld.lVal;
	lValue = val;
	return true;
}

bool CADORecordset::GetFieldValue(int nIndex, long& lValue)
{
	long val = (long)NULL;
	_variant_t vtFld;
	_variant_t vtIndex;
	
	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
	if(vtFld.vt != VT_NULL)
		val = vtFld.lVal;
	lValue = val;
	return true;
}


bool CADORecordset::GetFieldValue(LPCTSTR lpFieldName, int& nValue)
{
	int val = NULL;
	_variant_t vtFld;
	
	vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
	switch(vtFld.vt)
	{
	case VT_I2:
		val = vtFld.iVal;
		break;
	case VT_BOOL:
		val = vtFld.boolVal;
	case VT_NULL:
	case VT_EMPTY:
		break;
	default:
		nValue = 0;
		return false;
	}	
	nValue = val;
	return true;
}

bool CADORecordset::GetFieldValue(int nIndex, int& nValue)
{
	int val = (int)NULL;
	_variant_t vtFld;
	_variant_t vtIndex;
	
	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
	switch(vtFld.vt)
	{
	case VT_I2:
		val = vtFld.iVal;
		break;
	case VT_NULL:
	case VT_EMPTY:
		val = 0;
		break;
	default:
		return false;
	}	
	nValue = val;
	return true;
}

bool CADORecordset::GetFieldValue(LPCTSTR lpFieldName, CString& strValue)
{
	CString str = _T("");
	_variant_t vtFld;
	
	vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
	switch(vtFld.vt) 
	{
	case VT_BSTR:
		str = vtFld.bstrVal;
		break;
	case VT_I4:
		str = IntToStr(vtFld.iVal);
		break;
	case VT_DATE:
		{
			COleDateTime dt(vtFld);

			str = dt.Format("%Y-%m-%d %H:%M:%S");
		}
		break;
	case VT_EMPTY:
	case VT_NULL:
		break;
	default:
		strValue.Empty();
		return false;
	}
	strValue = str;
	return true;
}

bool CADORecordset::GetFieldValue(int nIndex, CString& strValue)
{
	CString str = _T("");
	_variant_t vtFld;
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	
	vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
	switch(vtFld.vt) 
	{
	case VT_BSTR:
		str = vtFld.bstrVal;
		break;
	case VT_DATE:
		{
			COleDateTime dt(vtFld);
			
			str = dt.Format("%Y-%m-%d %H:%M:%S");
		}
		break;
	case VT_EMPTY:
	case VT_NULL:
		break;
	default:
		strValue.Empty();
		return false;
	}
	strValue = str;
	return true;
}

bool CADORecordset::GetFieldValue(LPCTSTR lpFieldName, COleDateTime& time)
{
	_variant_t vtFld;
	
	vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
	switch(vtFld.vt) 
	{
	case VT_DATE:
		{
			COleDateTime dt(vtFld);
			time = dt;
		}
		break;
	case VT_EMPTY:
	case VT_NULL:
		break;
	default:
		return false;
	}
	return true;
}

bool CADORecordset::GetFieldValue(int nIndex, COleDateTime& time)
{
	_variant_t vtFld;
	_variant_t vtIndex;
	
	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	
	vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
	switch(vtFld.vt) 
	{
	case VT_DATE:
		{
			COleDateTime dt(vtFld);
			time = dt;
		}
		break;
	case VT_EMPTY:
	case VT_NULL:
		break;
	default:
		return false;
	}
	return true;
}

bool CADORecordset::IsFieldNull(LPCTSTR lpFieldName)
{
	_variant_t vtFld;
	
	vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
	return vtFld.vt == VT_NULL;
}

bool CADORecordset::IsFieldNull(int nIndex)
{
	_variant_t vtFld;
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	
	vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
	return vtFld.vt == VT_NULL;
}

bool CADORecordset::IsFieldEmpty(LPCTSTR lpFieldName)
{
	_variant_t vtFld;
	
	vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
	return vtFld.vt == VT_EMPTY || vtFld.vt == VT_NULL;
}

bool CADORecordset::IsFieldEmpty(int nIndex)
{
	_variant_t vtFld;
	_variant_t vtIndex;
	
	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
		
	vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
	return vtFld.vt == VT_EMPTY || vtFld.vt == VT_NULL;
}

DWORD CADORecordset::GetRecordCount()
{
	DWORD nRows = 0;
	
	nRows = m_pRecordset->GetRecordCount();

	if(nRows == -1)
	{
		nRows = 0;
		if(m_pRecordset->EndOfFile != VARIANT_TRUE)
			m_pRecordset->MoveFirst();
		
		while(m_pRecordset->EndOfFile != VARIANT_TRUE)
		{
			nRows++;
			m_pRecordset->MoveNext();
		}
		if(nRows > 0)
			m_pRecordset->MoveFirst();
	}
	
	return nRows;
}

bool CADORecordset::IsOpen()
{
	if(m_pRecordset)
		return m_pRecordset->GetState() != adStateClosed;
	return false;
}

void CADORecordset::Close()
{
	if(IsOpen())
		m_pRecordset->Close();
		
}


bool CADODatabase::Execute(LPCTSTR lpstrExec)
{
	ASSERT(m_pConnection != NULL);
	ASSERT(strcmp(lpstrExec, _T("")) != 0);

	try
	{
		m_pConnection->Execute(_bstr_t(lpstrExec), NULL, adExecuteNoRecords);
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}
	return true;	
}

bool CADORecordset::RecordBinding(CADORecordBinding &pAdoRecordBinding)
{
	IADORecordBinding *picRs = NULL;
	HRESULT hr;

	//Open the binding interface.
	if(FAILED(hr = m_pRecordset->QueryInterface(__uuidof(IADORecordBinding), (LPVOID*)&picRs )))
	{
		_com_issue_error(hr);
		return false;
	}
	
	//Bind the recordset to class
	if(FAILED( hr = picRs->BindToRecordset(&pAdoRecordBinding)))
	{
		_com_issue_error(hr);
		return false;
	}
	return true;
}

void CADORecordset::dump_com_error(_com_error &e)
{
	CString ErrorStr;
	
	
	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());
	ErrorStr.Format( "CADORecordset Error\n\tCode = %08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s\n",
		e.Error(), e.ErrorMessage(), (LPCSTR)bstrSource, (LPCSTR)bstrDescription );
	m_strLastError = _T("Query = " + GetQuery() + '\n' + ErrorStr);

	#ifdef _DEBUG
		AfxMessageBox( ErrorStr, MB_OK | MB_ICONERROR );
	#endif	
}

bool CADORecordset::GetFieldInfo(LPCTSTR lpFieldName, CADOFieldInfo* fldInfo)
{
	_variant_t vtFld;
	
	strcpy(fldInfo->m_strName, (LPCTSTR)m_pRecordset->Fields->GetItem(lpFieldName)->GetName());
	fldInfo->m_lSize = m_pRecordset->Fields->GetItem(lpFieldName)->GetActualSize();
	fldInfo->m_lDefinedSize = m_pRecordset->Fields->GetItem(lpFieldName)->GetDefinedSize();
	fldInfo->m_nType = m_pRecordset->Fields->GetItem(lpFieldName)->GetType();
	fldInfo->m_lAttributes = m_pRecordset->Fields->GetItem(lpFieldName)->GetAttributes();
	return true;
}

bool CADORecordset::GetFieldInfo(int nIndex, CADOFieldInfo* fldInfo)
{
	_variant_t vtFld;
	_variant_t vtIndex;
	
	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
		
	strcpy(fldInfo->m_strName, (LPCTSTR)m_pRecordset->Fields->GetItem(vtIndex)->GetName());
	fldInfo->m_lSize = m_pRecordset->Fields->GetItem(vtIndex)->GetActualSize();
	fldInfo->m_lDefinedSize = m_pRecordset->Fields->GetItem(vtIndex)->GetDefinedSize();
	fldInfo->m_nType = m_pRecordset->Fields->GetItem(vtIndex)->GetType();
	fldInfo->m_lAttributes = m_pRecordset->Fields->GetItem(vtIndex)->GetAttributes();
	return true;
}


bool CADORecordset::GetChunk(LPCTSTR lpFieldName, CString& strValue)
{
	CString str = _T("");
	long lngSize, lngOffSet = 0;
	_variant_t varChunk;    
	int ChunkSize = 100;

	lngSize = m_pRecordset->Fields->GetItem(lpFieldName)->ActualSize;
	
	str.Empty();
	while(lngOffSet < lngSize)
	{
		varChunk = m_pRecordset->Fields->GetItem(lpFieldName)->GetChunk(ChunkSize);
		str += varChunk.bstrVal;
		lngOffSet += ChunkSize;
	}

	lngOffSet = 0;
	strValue = str;
	return TRUE;
}

CString CADORecordset::GetString(LPCTSTR lpCols, LPCTSTR lpRows, LPCTSTR lpNull, long numRows)
{
	_bstr_t varOutput;
	_bstr_t varNull("");
	_bstr_t varCols("\t");
	_bstr_t varRows("\r");

	if(strlen(lpCols) != 0)
		varCols = _bstr_t(lpCols);

	if(strlen(lpRows) != 0)
		varRows = _bstr_t(lpRows);
	
	if(numRows == 0)
		numRows =(long)GetRecordCount();			
			
	varOutput = m_pRecordset->GetString(adClipString, numRows, varCols, varRows, varNull);

	return (LPCTSTR)varOutput;
}

CString IntToStr(int nVal)
{
	CString strRet;
	char buff[10];
	
	itoa(nVal, buff, 10);
	strRet = buff;
	return strRet;
}

CString LongToStr(long lVal)
{
	CString strRet;
	char buff[20];
	
	ltoa(lVal, buff, 10);
	strRet = buff;
	return strRet;
}

void CADORecordset::Edit()
{
	m_nEditStatus = dbEdit;
}

bool CADORecordset::AddNew()
{
	m_nEditStatus = dbEditNone;
	if(m_pRecordset->AddNew() != S_OK)
		return false;

	m_nEditStatus = dbEditNew;
	return true;
}

bool CADORecordset::Update()
{
	bool bret = true;

	if(m_nEditStatus != dbEditNone)
	{
		if(m_pRecordset->Update() != S_OK)
			bret = false;
	}

	m_nEditStatus = dbEditNone;
	return bret;
}

void CADORecordset::CancelUpdate()
{
	m_pRecordset->CancelUpdate();
	m_nEditStatus = dbEditNone;
}

bool CADORecordset::SetFieldValue(int nIndex, CString strValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_BSTR;
	vtFld.bstrVal = _bstr_t(strValue);

	_variant_t vtIndex;	
	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	
	m_pRecordset->Fields->GetItem(vtIndex)->Value = _bstr_t(vtFld);//_bstr_t(strValue);
	return true;

}

bool CADORecordset::SetFieldValue(LPCTSTR lpFieldName, CString strValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_BSTR;
	vtFld.bstrVal = _bstr_t(strValue);
	
	m_pRecordset->Fields->GetItem(lpFieldName)->Value = _bstr_t(vtFld);//_bstr_t(strValue);
	return true;
	
}

bool CADORecordset::SetFieldValue(int nIndex, int nValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_I2;
	vtFld.iVal = nValue;
	
	_variant_t vtIndex;
	
	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	
	m_pRecordset->Fields->GetItem(vtIndex)->Value = vtFld;
	return true;
	
}

bool CADORecordset::SetFieldValue(LPCTSTR lpFieldName, int nValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_I2;
	vtFld.iVal = nValue;
	
	m_pRecordset->Fields->GetItem(lpFieldName)->Value = vtFld;
	return true;
	
}

bool CADORecordset::SetFieldValue(int nIndex, long lValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_I4;
	vtFld.lVal = lValue;
	
	_variant_t vtIndex;
	
	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	
	m_pRecordset->Fields->GetItem(vtIndex)->Value = vtFld;
	return true;
	
}

bool CADORecordset::SetFieldValue(LPCTSTR lpFieldName, long lValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_I4;
	vtFld.lVal = lValue;
	
	m_pRecordset->Fields->GetItem(lpFieldName)->Value = vtFld;
	return true;
	
}

bool CADORecordset::SetFieldValue(int nIndex, double dblValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_R8;
	vtFld.dblVal = dblValue;

	_variant_t vtIndex;
	
	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	
	m_pRecordset->Fields->GetItem(vtIndex)->Value = vtFld;
	return true;
	
}

bool CADORecordset::SetFieldValue(LPCTSTR lpFieldName, double dblValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_R8;
	vtFld.dblVal = dblValue;
		
	m_pRecordset->Fields->GetItem(lpFieldName)->Value = vtFld;
	return true;
	
}

bool CADORecordset::SetFieldValue(int nIndex, COleDateTime time)
{
	_variant_t vtFld;
	vtFld.vt = VT_DATE;
	vtFld.date = time;
	
	_variant_t vtIndex;
	
	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	
	m_pRecordset->Fields->GetItem(vtIndex)->Value = vtFld;
	return true;
	
}

bool CADORecordset::SetFieldValue(LPCTSTR lpFieldName, COleDateTime time)
{
	_variant_t vtFld;
	vtFld.vt = VT_DATE;
	vtFld.date = time;
	
	m_pRecordset->Fields->GetItem(lpFieldName)->Value = vtFld;
	return true;
	
}

bool CADORecordset::SetBookmark()
{
	if(m_varBookmark.vt != VT_EMPTY)
	{
		m_pRecordset->Bookmark = m_varBookmark;
		return true;
	}
	return false;
}

bool CADORecordset::Delete()
{
	if(m_pRecordset->Delete(adAffectCurrent) != S_OK)
		return false;

	if(m_pRecordset->Update() != S_OK)
		return false;

	return true;
}

bool CADORecordset::Find(LPCTSTR lpFind, int nSearchDirection)
{

	m_strFind = lpFind;
	m_nSearchDirection = nSearchDirection;

	ASSERT(!m_strFind.IsEmpty());

	if(m_nSearchDirection == searchForward)
	{
		m_pRecordset->Find(_bstr_t(m_strFind), 0, adSearchForward, "");
		if(!IsEof())
		{
			m_varBookFind = m_pRecordset->Bookmark;
			return true;
		}
	}
	else if(m_nSearchDirection == searchBackward)
	{
		m_pRecordset->Find(_bstr_t(m_strFind), 0, adSearchBackward, "");
		if(!IsBof())
		{
			m_varBookFind = m_pRecordset->Bookmark;
			return true;
		}
	}
	else
	{
		TRACE("Unknown parameter. %d", nSearchDirection);
		m_nSearchDirection = searchForward;
	}
	return false;
}

bool CADORecordset::FindFirst(LPCTSTR lpFind)
{
	m_pRecordset->MoveFirst();
	return Find(lpFind);
}

bool CADORecordset::FindNext()
{
	if(m_nSearchDirection == searchForward)
	{
		m_pRecordset->Find(_bstr_t(m_strFind), 1, adSearchForward, m_varBookFind);
		if(!IsEof())
		{
			m_varBookFind = m_pRecordset->Bookmark;
			return true;
		}
	}
	else
	{
		m_pRecordset->Find(_bstr_t(m_strFind), 1, adSearchBackward, m_varBookFind);
		if(!IsBof())
		{
			m_varBookFind = m_pRecordset->Bookmark;
			return true;
		}
	}
	return false;
}
