
#include "SilverStream.h"
#include "Utils.h"

#include "base\funcGeneral.h"

BOOL SILVERSTREAM_MONITOR(char *url, int timeout, char *uid, char *pwd, 
						  char *pserver, int pport, char *puid, char *ppwd, 
						  char *custpath, char *szReturn)
{
	LONG hits = 0, bytes = 0, rpt_min = 0, rpt_avg = 0, rpt_max = 0;
	LONG tc_free = 0, tc_idle = 0, tc_total = 0;
	LONG ms_free = 0, ms_total = 0, ms_gc = 0;
	LONG cur_load = 0;
	LONG idle_sessions = 0, total_sessions = 0, used_licenses = 0, total_licenses = 0;

	char msg[1024] = {0};
	if(!GetUrlData(url, timeout, uid, pwd, pserver, pport, puid, ppwd, 0, msg))
	{
		sprintf(szReturn, "error=%s", msg);
		return FALSE;
	}

	char *ca = msg, *cb = NULL;
	
	// hits
	if(cb = strstr(ca, "hits:"))
	{
		hits = atol(cb + 5);
		ca = cb;
	}
	else
	{
		// Data Malformed
		sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_SilverStream_1%>"));//<%IDS_SilverStream_1%>
		return FALSE;
	}

	// bytes
	if(cb = strstr(ca, "bytes:"))
	{
		bytes = atol(cb + 6);
		ca = cb;
	}
	else
	{
		// Data Malformed
		sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_SilverStream_2%>"));//<%IDS_SilverStream_2%>
		return FALSE;
	}

	// Request processing times(min, avg, max)
	if(cb = strstr(ca, "Request processing times:"))
		ca = cb;
	else
	{
		// Data Malformed
		sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_SilverStream_3%>"));//<%IDS_SilverStream_3%>
		return FALSE;
	}

	int count = 0;
	while(count < 3)
	{
		if(cb = strstr(ca, ">"))
		{
			if(isdigit(*(cb+1)))
			{
				if(count == 0)
					rpt_min = atol(cb+1);
				else if(count == 1)
					rpt_avg = atol(cb+1);
				else if(count == 2)
					rpt_max = atol(cb+1);

				count ++;
			}

			ca = cb + 1;
		}
		else
		{
			// Data Malformed
			sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_SilverStream_4%>"));//<%IDS_SilverStream_4%>
			return FALSE;
		}
	}

	// Thread counts(Free, Idle, Total)
	if(cb = strstr(ca, "Thread counts:"))
		ca = cb;
	else
	{
		// Data Malformed
		sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_SilverStream_5%>"));//<%IDS_SilverStream_5%>
		return FALSE;
	}

	count = 0;
	while(count < 3)
	{
		if(cb = strstr(ca, ">"))
		{
			if(isdigit(*(cb+1)))
			{
				if(count == 0)
					tc_free = atol(cb+1);
				else if(count == 1)
					tc_idle = atol(cb+1);
				else if(count == 2)
					tc_total = atol(cb+1);

				count ++;
			}

			ca = cb + 1;
		}
		else
		{
			// Data Malformed
			sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_SilverStream_6%>"));//<%IDS_SilverStream_6%>
			return FALSE;
		}
	}

	// Memory status(Free memory, Total memory, GC count)
	if(cb = strstr(ca, "Memory status:"))
		ca = cb;
	else
	{
		// Data Malformed
		sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_Pop3_25%>"));//<%IDS_SilverStream_7%>
		return FALSE;
	}

	count = 0;
	while(count < 3)
	{
		if(cb = strstr(ca, ">"))
		{
			if(isdigit(*(cb+1)))
			{
				if(count == 0)
					ms_free = atol(cb+1);
				else if(count == 1)
					ms_total = atol(cb+1);
				else if(count == 2)
					ms_gc = atol(cb+1);

				count ++;
			}

			ca = cb + 1;
		}
		else
		{
			// Data Malformed
			sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_SilverStream_8%>"));//<%IDS_SilverStream_8%>
			return FALSE;
		}
	}

	// Current load
	if(cb = strstr(ca, "Current load (1=LIGHT to 4=HIGH):"))
	{
		cur_load = atol(cb + 33);
		ca = cb;
	}
	else
	{
		// Data Malformed
		sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_SilverStream_9%>"));//<%IDS_SilverStream_9%>
		return FALSE;
	}

	// Session/License status
	if(cb = strstr(ca, "Session/License status:"))
		ca = cb;
	else
	{
		// Data Malformed
		sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_SilverStream_10%>"));//<%IDS_SilverStream_10%>
		return FALSE;
	}

	count = 0;
	while(count < 4)
	{
		if(cb = strstr(ca, ">"))
		{
			if(isdigit(*(cb+1)))
			{
				if(count == 0)
					idle_sessions = atol(cb+1);
				else if(count == 1)
					total_sessions = atol(cb+1);
				else if(count == 2)
					used_licenses = atol(cb+1);
				else if(count == 3)
					total_licenses = atol(cb+1);

				count ++;
			}

			ca = cb + 1;
		}
		else
		{
			// Data Malformed
			sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_SilverStream_11%>"));//<%IDS_SilverStream_11%>
			return FALSE;
		}
	}

	sprintf(szReturn, "hits=%ld$bytes=%ld$rpt_min=%ld$rpt_avg=%ld$rpt_max=%ld$tc_free=%ld$tc_idle=%ld$tc_total=%ld$ms_free=%ld$ms_total=%ld$ms_gc=%ld$cur_load=%ld$idle_sessions=%ld$total_sessions=%ld$used_licenses=%ld$total_licenses=%ld$", 
				hits, bytes, rpt_min, rpt_avg, rpt_max, 
				tc_free, tc_idle, tc_total, ms_free, ms_total, ms_gc, 
				cur_load, idle_sessions, total_sessions, used_licenses, total_licenses);

	return TRUE;
}
