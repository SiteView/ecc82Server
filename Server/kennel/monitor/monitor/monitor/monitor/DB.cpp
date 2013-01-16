
#include "DB.h"

#include "base\funcGeneral.h"

BOOL DB_MONITOR(char *dbconn, char *sql, int conntimeout, int qtimeout, 
				char *uid, char *pwd, char *driver, char *qfile, 
				char *match, char *custpath, char *szReturn)
{
	CDatabase	db;
	CRecordset	*rs = NULL;
	CString		szConnect;

	DWORD		dwTimeBegin = 0;
	DWORD		dwTimeEnd = 0;
	DWORD		dwRows = 0;
	BOOL		bMatchOK = FALSE;
	BOOL		bResult = TRUE;

	szConnect.Format(_T("DSN=%s;UID=%s;PWD=%s;"), dbconn, uid, pwd);

	try
	{
		dwTimeBegin = GetTickCount();
		
		if(!db.OpenEx(szConnect, CDatabase::noOdbcDialog))
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_DB_1%>"));//<%IDS_DB_1%>
			return FALSE;
		}
	

		rs = new CRecordset(&db);
		rs->Open(CRecordset::forwardOnly, sql, CRecordset::noDirtyFieldCheck);
		dwTimeEnd = GetTickCount();
		while(!rs->IsEOF())
		{
			if(dwRows == 0 && *match)
			{
				short		nFields = 0;
				int			index = 0;
				CString		strField = _T("");
				nFields = rs->GetODBCFieldCount();
				if(nFields)
				{
					rs->GetFieldValue((short)0, strField);
					index = strField.Find(match, 0);
					if(index >= 0) 
					{
						bMatchOK = TRUE;
					}
					else if(nFields > 1)
					{
						rs->GetFieldValue((short)1, strField);
						index = strField.Find(match, 0);
						if(index >= 0)
						{
							bMatchOK = TRUE;
						}
					}
				}
			}

			dwRows ++;
			rs->MoveNext();
		}
		if(*match)
		{
			if(bMatchOK)
				sprintf(szReturn, "status=%d$svrows=%ld$roundTripTime=%ld$", 
					200, dwRows, dwTimeEnd - dwTimeBegin);
			else
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_DB_2%>"));//<%IDS_DB_2%>
		}
		else
			sprintf(szReturn, "status=%d$svrows=%ld$roundTripTime=%ld$", 
				200, dwRows, dwTimeEnd - dwTimeBegin);

		rs->Close();
	}
	catch (CDBException* e)
	{
		CString strError;
		strError.Format("%s",e->m_strError);
		strError.Replace("\r\n",",");
		strError.Replace("\n",",");
		//strError.Replace("'",",");
		

		sprintf(szReturn, "error=%s", strError.GetBuffer(strError.GetLength()));
		//strcpy( szReturn , "error=Êý¾ÝÔ´´íÎó$" );
		e->Delete();
		bResult = FALSE;
	}

	if(rs != NULL) 
		delete rs;
	db.Close();

	return bResult;
}
