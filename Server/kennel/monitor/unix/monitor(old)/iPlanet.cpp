
#include "iPlanet.h"
#include "Utils.h"

#include "..\..\base\funcGeneral.h"

BOOL IPLANET_MONITOR(char *url, int timeout, char *uid, char *pwd, 
					 char *pserver, int pport, char *puid, char *ppwd, 
					 char *custpath, char *szReturn)
{
	int		totalRequests = 0,
			badRequests = 0,
			status_2xx = 0,
			status_3xx = 0,
			status_4xx = 0,
			status_5xx = 0,
			status_xxx = 0,
			status_200 = 0,
			status_302 = 0,
			status_304 = 0,
			status_401 = 0,
			status_403 = 0;

	char msg[8192] = {0};
	if(!GetUrlData(url, timeout, uid, pwd, pserver, pport, puid, ppwd, 8192, msg))
	{
		sprintf(szReturn, "error=%s", msg);
		return FALSE;
	}

	int index = 0;
	int		percent = 0;
	char tmp[256] = {0};
	CString strBuff(msg);
	while(1)
	{
		index = strBuff.Find('<', 0);
		if(index < 0) break;
		int index2 = strBuff.Find('>', index);
		if(index2 < 0) break;
		strBuff.Insert(index, ':');
		strBuff.Delete(index + 1, index2 - index + 1);
	}
	strBuff.Replace("\r\n", " ");
	strBuff.Replace(':', ' ');
	strBuff.MakeLower();

	index = strBuff.Find("total requests", 0);
	if(index < 0) 
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_iPlanet_1%>"));//<%IDS_iPlanet_1%>
		return FALSE;
	}
	strBuff.Delete(0, index + 6);
	if(sscanf(strBuff, "%s %d", tmp, &totalRequests) != 2)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_iPlanet_2%>"));//<%IDS_iPlanet_2%>
		return FALSE;
	}

	index = strBuff.Find("bad requests", 0);
	if(index < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_iPlanet_3%>"));//<%IDS_iPlanet_3%>
		return FALSE;
	}
	strBuff.Delete(0, index + 4);
	if(sscanf(strBuff, "%s %d%% %d", tmp, &percent, &badRequests) != 3)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_iPlanet_4%>"));//<%IDS_iPlanet_4%>
		return FALSE;
	}

	index = strBuff.Find("2xx", 0);
	if(index < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_iPlanet_5%>"));//<%IDS_iPlanet_5%>
		return FALSE;
	}
	strBuff.Delete(0, index);
	if(sscanf(strBuff, "%s %d%% %d", tmp, &percent, &status_2xx) != 3)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_iPlanet_6%>"));//<%IDS_iPlanet_6%>
		return FALSE;
	}

	index = strBuff.Find("3xx", 0);
	if(index < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_iPlanet_7%>"));//<%IDS_iPlanet_7%>
		return FALSE;
	}
	strBuff.Delete(0, index);
	if(sscanf(strBuff, "%s %d%% %d", tmp, &percent, &status_3xx) != 3)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_iPlanet_8%>"));//<%IDS_iPlanet_8%>
		return FALSE;
	}

	index = strBuff.Find("4xx", 0);
	if(index < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_iPlanet_9%>"));//<%IDS_iPlanet_9%>
		return FALSE;
	}
	strBuff.Delete(0, index);
	if(sscanf(strBuff, "%s %d%% %d", tmp, &percent, &status_4xx) != 3)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_iPlanet_10%>"));//<%IDS_iPlanet_10%>
		return FALSE;
	}

	index = strBuff.Find("5xx", 0);
	if(index < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_iPlanet_11%>"));//<%IDS_iPlanet_11%>
		return FALSE;
	}
	strBuff.Delete(0, index);
	if(sscanf(strBuff, "%s %d%% %d", tmp, &percent, &status_5xx) != 3)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_iPlanet_12%>"));//<%IDS_iPlanet_12%>
		return FALSE;
	}

	index = strBuff.Find("xxx", 0);
	if(index < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_iPlanet_13%>"));//<%IDS_iPlanet_13%>
		return FALSE;
	}
	strBuff.Delete(0, index);
	if(sscanf(strBuff, "%s %d%% %d", tmp, &percent, &status_xxx) != 3)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_iPlanet_14%>"));//<%IDS_iPlanet_14%>
		return FALSE;
	}
	
	if(status_xxx == 200)
	{
		index = strBuff.Find("200", 0);
		if(index >= 0)
			strBuff.Delete(0, index + 3);
	}
	index = strBuff.Find("200", 0);
	if(index < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_iPlanet_15%>"));//<%IDS_iPlanet_15%>
		return FALSE;
	}
	strBuff.Delete(0, index);
	if(sscanf(strBuff, "%s %d%% %d", tmp, &percent, &status_200) != 3)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_iPlanet_16%>"));//<%IDS_iPlanet_16%>
		return FALSE;
	}

	if(status_200 == 302)
	{
		index = strBuff.Find("302", 0);
		if(index >= 0)
			strBuff.Delete(0, index + 3);
	}
	index = strBuff.Find("302", 0);
	if(index < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_iPlanet_17%>"));//<%IDS_iPlanet_17%>
		return FALSE;
	}
	strBuff.Delete(0, index);
	if(sscanf(strBuff, "%s %d%% %d", tmp, &percent, &status_302) != 3)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_iPlanet_18%>"));//<%IDS_iPlanet_18%>
		return FALSE;
	}

	if(status_302 == 304)
	{
		index = strBuff.Find("304", 0);
		if(index >= 0)
			strBuff.Delete(0, index + 3);
	}

	index = strBuff.Find("304", 0);
	if(index < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_iPlanet_19%>"));//<%IDS_iPlanet_19%>
		return FALSE;
	}
	strBuff.Delete(0, index);
	if(sscanf(strBuff, "%s %d%% %d", tmp, &percent, &status_304) != 3)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_iPlanet_20%>"));//<%IDS_iPlanet_20%>
		return FALSE;
	}

	if(status_304 == 401)
	{
		index = strBuff.Find("401", 0);
		if(index >= 0)
			strBuff.Delete(0, index + 3);
	}
	index = strBuff.Find("401", 0);
	if(index < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_iPlanet_21%>"));//<%IDS_iPlanet_21%>
		return FALSE;
	}
	strBuff.Delete(0, index);
	if(sscanf(strBuff, "%s %d%% %d", tmp, &percent, &status_401) != 3)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_iPlanet_22%>"));//<%IDS_iPlanet_22%>
		return FALSE;
	}

	if(status_401 == 403)
	{
		index = strBuff.Find("403", 0);
		if(index >= 0)
			strBuff.Delete(0, index + 3);
	}
	index = strBuff.Find("403", 0);
	if(index < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_iPlanet_23%>"));//<%IDS_iPlanet_23%>
		return FALSE;
	}
	strBuff.Delete(0, index);
	if(sscanf(strBuff, "%s %d%% %d", tmp, &percent, &status_403) != 3)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_iPlanet_24%>"));//<%IDS_iPlanet_24%>
		return FALSE;
	}

	sprintf(szReturn, "totalRequests=%d$badRequests=%d$status_2xx=%d$status_3xx=%d$status_4xx=%d$status_5xx=%d$status_xxx=%d$status_200=%d$status_302=%d$status_304=%d$status_401=%d$status_403=%d$", 
			totalRequests, badRequests, status_2xx, status_3xx, status_4xx, 
			status_5xx, status_xxx, status_200, status_302, status_304, 
			status_401, status_403);

	return TRUE;
}
