
#include "Sybase.h"

#include "..\..\base\funcGeneral.h"

BOOL SYBASE_MONITOR(char *dbconn, char *uid, char *pwd, char *custpath, char *szReturn)
{
	BOOL		bResult = TRUE;
	CDatabase	db;
	CRecordset	*rs = NULL;
	CString		szConnect;
	CString		strSQL = _T("select * from master..spt_monitor");

	//szConnect.Format(_T("DSN=%s;SRVR=dragonxu;UID=%s;PWD=%s;"), dbconn, uid, pwd);
	szConnect.Format(_T("DSN=%s;UID=%s;PWD=%s;"), dbconn, uid, pwd);
	puts(szConnect);

	try
	{
		if(!db.OpenEx(szConnect, CDatabase::noOdbcDialog))
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_DB_1%>"));//<%IDS_DB_1%>
			return FALSE;
		}
		puts("连接成功");

		rs = new CRecordset(&db);
		rs->Open(CRecordset::forwardOnly, strSQL, CRecordset::noDirtyFieldCheck);
		if(rs->IsEOF())
		{
			bResult = FALSE;
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SYBASE_01%>"));
			goto w;
		}

		{
		CString	/*strlastRun = _T(""), */strcpuBusy = _T(""), strioBusy = _T(""), 
				strIdle = _T(""), strpackReceived = _T(""), strpackSent = _T(""), 
				strConnections = _T(""), strpackErrors = _T(""), strtotalRead = _T(""), 
				strtotalWrite = _T(""), strtotalErrors = _T("");

		//rs->GetFieldValue(_LASTRUN, strlastRun);
		puts("select ok");
		rs->GetFieldValue(_CPU_BUSY, strcpuBusy);
		rs->GetFieldValue(_IO_BUSY, strioBusy);
		rs->GetFieldValue(_IDLE, strIdle);
		rs->GetFieldValue(_PACK_RECEIVED, strpackReceived);
		rs->GetFieldValue(_PACK_SENT, strpackSent);
		rs->GetFieldValue(_CONNECTIONS, strConnections);
		rs->GetFieldValue(_PACK_ERRORS, strpackErrors);
		rs->GetFieldValue(_TOTAL_READ, strtotalRead);
		rs->GetFieldValue(_TOTAL_WRITE, strtotalWrite);
		rs->GetFieldValue(_TOTAL_ERRORS, strtotalErrors);

		/*
		sprintf(szReturn, "cpuBusy=%s$ioBusy=%s$Idle=%s$packReceived=%s$packSent=%s$Connections=%s$packErrors=%s$totalRead=%s$totalWrite=%s$totalErrors=%s$", 
						strcpuBusy, strioBusy, strIdle, strpackReceived, strpackSent, 
						strConnections, strpackErrors, strtotalRead, strtotalWrite, 
						strtotalErrors);
						*/
		sprintf(szReturn, "cpu_busy=%s$io_busy=%s$packets_received=%s$packets_sent=%s$connections=%s$packet_errors=%s$total_read=%s$total_write=%s$total_errors=%s$", 
						strcpuBusy, strioBusy, strpackReceived, strpackSent, 
						strConnections, strpackErrors, strtotalRead, strtotalWrite, 
						strtotalErrors);
		}
w:		rs->Close();
	}
	catch (CDBException* e)
	{
		sprintf(szReturn, "error=%s", e->m_strError.GetBuffer(e->m_strError.GetLength()));
		e->Delete();
	}

	if(rs != NULL) 
		delete rs;
	db.Close();

	return bResult;
}
