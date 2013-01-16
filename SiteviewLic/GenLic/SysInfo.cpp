#include "SysInfo.h"


SysInfo::SysInfo(void)
{
}

SysInfo::~SysInfo(void)
{
}

bool SysInfo::GetInstallTime(char* instime)
{
	char pBuf[MAX_PATH];
	//GetSystemDirectoryA(pBuf, MAX_PATH);
	GetCurrentDirectoryA(MAX_PATH, pBuf);
	//printf("CurrentDirectory: (%s)\n", pBuf);

	HANDLE hDir = CreateFileA(pBuf, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_DELETE, NULL, OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS, NULL);
	FILETIME lpCreationTime;
	FILETIME lpLastAccessTime;
	FILETIME lpLastWriteTime;
	if (GetFileTime(hDir, &lpCreationTime, &lpLastAccessTime, &lpLastWriteTime))
	{
//		printf("%08x %08x\n", lpCreationTime.dwLowDateTime, lpCreationTime.dwHighDateTime);
		char dtBuf[21];
		sprintf(dtBuf, "IST:%08X%08X", lpCreationTime.dwLowDateTime, lpCreationTime.dwHighDateTime);
		strncat(instime, dtBuf, 20);
	}
	else
	{
		strncat(instime, "IST:0000000000000000", 20);
	}
	CloseHandle(hDir);
	return true;
}

bool SysInfo::GetInstallDate(char* insdate)
{
	char pBuf[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, pBuf);
	HANDLE hDir = CreateFileA(pBuf, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_DELETE, NULL, OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS, NULL);
	FILETIME lpCreationTime;
	FILETIME lpLastAccessTime;
	FILETIME lpLastWriteTime;
	bool bOk = false;
	if (GetFileTime(hDir, &lpCreationTime, &lpLastAccessTime, &lpLastWriteTime))
	{
		SYSTEMTIME stUTC, stLocal;
		FileTimeToSystemTime(&lpCreationTime, &stUTC);
		SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
		sprintf(insdate, "%d%02d%02d",stLocal.wYear, stLocal.wMonth, stLocal.wDay);
		bOk = true;
	}
	CloseHandle(hDir);
	return bOk;
}


bool SysInfo::GetSysFeature(char* feature)
{
	GetInstallTime(feature);
	strcat(feature, ",");

	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;

	pAdapterInfo = (IP_ADAPTER_INFO *) GlobalAlloc(GMEM_FIXED, sizeof(IP_ADAPTER_INFO) );
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	
	// Make an initial call to GetAdaptersInfo to get the necessary size into the ulOutBufLen variable
	if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) != ERROR_SUCCESS) {
		GlobalFree (pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) GlobalAlloc(GMEM_FIXED, ulOutBufLen);
	}
	
	if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) == NO_ERROR) 
	{
		pAdapter = pAdapterInfo;
		while (pAdapter) 
		{
			char mac[18];
			sprintf(mac, "MAC:%02X%02X%02X%02X%02X%02X,",
				pAdapter->Address[0],
				pAdapter->Address[1],
				pAdapter->Address[2],
				pAdapter->Address[3],
				pAdapter->Address[4],
				pAdapter->Address[5]);
			strncat(feature, mac, 17);
			pAdapter = pAdapter->Next;
		}
	}
	GlobalFree (pAdapterInfo);
	return true;
}

bool SysInfo::newGUID(char* buf)
{
	//--COM
	CoInitialize(NULL);
	GUID guid;
	if (S_OK == ::CoCreateGuid(&guid))
	{
		_snprintf(buf, 64//sizeof(buf)
			, "{%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}"
			, guid.Data1
			, guid.Data2
			, guid.Data3
			, guid.Data4[0], guid.Data4[1]
			, guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
			, guid.Data4[6], guid.Data4[7]
			);
	}
	//--COM
	CoUninitialize();
	return true;
}

void SysInfo::GetCurrentDate(char* date)
{
	struct   tm   *time_now;  
	time_t   secs_now;  
	tzset();  
	time(&secs_now);  
	time_now   =   localtime(&secs_now);  
	strftime(date, 10, "%Y%m%d", time_now);  
}

double SysInfo::DiffDate(const string date1, const string date2)
{
	struct tm t1, t2;
	time_t tt1,tt2;
	t1.tm_year = atoi(date1.substr(0,4).c_str()) -1900;
	t1.tm_mon = atoi(date1.substr(4,2).c_str());
	t1.tm_mday = atoi(date1.substr(6,2).c_str());;
	t1.tm_hour=0;
	t1.tm_min=0;
	t1.tm_sec=1;
//	t1.tm_isdst=0;
	t2.tm_year = atoi(date2.substr(0,4).c_str())-1900;
	t2.tm_mon = atoi(date2.substr(4,2).c_str());
	t2.tm_mday = atoi(date2.substr(6,2).c_str());
	t2.tm_hour=0;
	t2.tm_min=0;
	t2.tm_sec=1;
	tt1 = mktime(&t1);
	tt2 = mktime(&t2);

	return difftime(tt1, tt2);
}