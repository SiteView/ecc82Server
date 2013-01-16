
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>


#include "sysinfo.h"


#ifdef WIN32
#include <windows.h>
#include"iphlpapi.h"
#pragma comment(lib,"Iphlpapi.lib")
#else	

#endif

#ifdef WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT extern
#endif



long mac_addr_sys(unsigned char *addr);
int GetCPUID(char* cpuinfo);

int winwmi(char* buffer);

void FindInAll(const char *Path, char* CurDate);
void md5(const char* sour, char *dest);




void GetCurrentDateTime(char* date)
{
	struct   tm   *time_now;  
	time_t   secs_now;  
	tzset();  
	time(&secs_now);  
	time_now   =   localtime(&secs_now);  
	strftime(date, 16, "%Y%m%d%H%M%S", time_now);  
}


void getguid(char* guid)
{
	unsigned char addrs[256]={0};
    unsigned char uniqueids[1024]={0}; 
    unsigned char randbuff[256]={0};
	int Length = -1;

	memset(uniqueids, 0, 1024);
	GetCurrentDateTime(uniqueids);

	srand(999);
	sprintf(randbuff,"%X",rand() % 999);
	strcat(uniqueids, ",");
	strcat(uniqueids, randbuff);

	if(mac_addr_sys(addrs) == 1)				
    {
		strcat(uniqueids, addrs);
    }	

   // strcpy(guid,uniqueids);
	md5(uniqueids,guid);
	return ;

}


void GetMaxDate(char* CurDate)
{
   char MaxDate[16]={"20000101"};
   #ifdef WIN32
   char pBuf[MAX_PATH]={0};   
  

   GetSystemDirectory(pBuf, MAX_PATH);
  // printf ("目录： %s\n\r",pBuf);
   FindInAll(pBuf,MaxDate);
 //  printf ("MaxDate :  %s\n\r",MaxDate);

   GetWindowsDirectory(pBuf, MAX_PATH);
  // printf ("目录： %s\n\r",pBuf);
   FindInAll(pBuf,MaxDate);
  // printf ("MaxDate :  %s\n\r",MaxDate);
   #endif
   strcpy(CurDate,MaxDate);

}


void FindInAll(const char *Path, char* CurDate)
{
#ifdef WIN32
	char pBuf[MAX_PATH]={0};
        char MaxDate[16]={"0"};

	FILETIME lpCreationTime;
	FILETIME lpLastAccessTime;
	FILETIME lpLastWriteTime;
    
	SYSTEMTIME stUTC, stLocal;
	
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	HANDLE hDir;

	strcpy(pBuf,Path);
	strcat(pBuf,"//*");

	strcpy(MaxDate,CurDate);

	hFind = FindFirstFile(pBuf, &FindFileData);
	if (hFind != INVALID_HANDLE_VALUE) 	
	{	
		do
		{
			if(FindFileData.cFileName[0]=='.') //过滤本级目录和父目录
				continue;		  		
					
			strcpy(pBuf,Path);
			strcat(pBuf,"\\");
			strcat(pBuf,FindFileData.cFileName);

			//printf ("目录： %s\n\r",pBuf);

			hDir = CreateFileA(pBuf, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_DELETE, NULL, OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS, NULL);

			if (GetFileTime(hDir, &lpCreationTime, &lpLastAccessTime, &lpLastWriteTime))
			{						
				FileTimeToSystemTime(&lpLastWriteTime, &stUTC);
				SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
				sprintf(pBuf,"%d%02d%02d",stLocal.wYear, stLocal.wMonth, stLocal.wDay);

				//printf("  %s   %s  %f \r\n",pBuf,MaxDate,DiffDate(pBuf,MaxDate));

				if (DiffDate(pBuf,MaxDate) > 0)
				{
					strcpy(MaxDate,pBuf);
				}
			}
			CloseHandle(hDir);
			  
		}
		while(FindNextFile(hFind,&FindFileData));
		FindClose(hFind);
	}
	strcpy(CurDate,MaxDate);
#endif	
}

int SplitFilePath(const char* fullName, char* path, char* filename)
{
	char *p;
	p = strrchr(fullName,'\\');
	if (p == NULL)
	{
		strcpy(path,fullName);
		strcpy(filename,fullName);
	}
	else
	{
		strcpy(filename,p+1);
		strncpy(path,fullName,strlen(fullName) - strlen(p));
	}	
	return true;
}

int GetInstallTime(char* instime)
{
#ifdef WIN32
	char pBuf[MAX_PATH];
	HANDLE hDir;
	FILETIME lpCreationTime;
	FILETIME lpLastAccessTime;
	FILETIME lpLastWriteTime;
	char dtBuf[21];

	GetCurrentDirectoryA(MAX_PATH, pBuf);
	hDir = CreateFileA(pBuf, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_DELETE, NULL, OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS, NULL);

	if (GetFileTime(hDir, &lpCreationTime, &lpLastAccessTime, &lpLastWriteTime))
	{
//		printf("%08x %08x\n", lpCreationTime.dwLowDateTime, lpCreationTime.dwHighDateTime);	
		sprintf(dtBuf, "IST:%08X%08X", lpCreationTime.dwLowDateTime, lpCreationTime.dwHighDateTime);
		strncat(instime, dtBuf, 20);
	}
	else
	{
		strncat(instime, "IST:0000000000000000", 20);
	}
	CloseHandle(hDir);
	return true;
#else	
	return true;
#endif
}



int GetInstallTime1(const char* fileName, char* instime)
{
#ifdef WIN32
	char pBuf[MAX_PATH]={0};
	char name[MAX_PATH]={0};
	FILETIME lpCreationTime;
	FILETIME lpLastAccessTime;
	FILETIME lpLastWriteTime;
	char dtBuf[21];
	HANDLE hDir;

	SplitFilePath(fileName, pBuf, name);

	hDir = CreateFileA(pBuf, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_DELETE, NULL, OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS, NULL);

	if (GetFileTime(hDir, &lpCreationTime, &lpLastAccessTime, &lpLastWriteTime))
	{
//		printf("%08x %08x\n", lpCreationTime.dwLowDateTime, lpCreationTime.dwHighDateTime);
		sprintf(dtBuf, "IST:%08X%08X", lpCreationTime.dwLowDateTime, lpCreationTime.dwHighDateTime);
		strncat(instime, dtBuf, 20);
	}
	else
	{
		strncat(instime, "IST:0000000000000000", 20);
	}
	CloseHandle(hDir);
	return true;
#else	
	return true;
#endif
}

int GetInstallDate(char* insdate)
{
#ifdef WIN32
	char pBuf[MAX_PATH];
	FILETIME lpCreationTime;
	FILETIME lpLastAccessTime;
	FILETIME lpLastWriteTime;
	SYSTEMTIME stUTC, stLocal;
	HANDLE hDir;
	int bOk = false;
	GetCurrentDirectoryA(MAX_PATH, pBuf);

	hDir = CreateFileA(pBuf, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_DELETE, NULL, OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS, NULL);

	if (GetFileTime(hDir, &lpCreationTime, &lpLastAccessTime, &lpLastWriteTime))
	{
		FileTimeToSystemTime(&lpCreationTime, &stUTC);
		SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
		sprintf(insdate, "%d%02d%02d",stLocal.wYear, stLocal.wMonth, stLocal.wDay);
		bOk = true;
	}
	CloseHandle(hDir);
	return bOk;

#else	
	return true;
#endif
}

int GetInstallDate1(const char* fileName, char* insdate)
{
#ifdef WIN32
	char pBuf[MAX_PATH]={0};
	char name[MAX_PATH]={0};
	FILETIME lpCreationTime;
	FILETIME lpLastAccessTime;
	FILETIME lpLastWriteTime;
	HANDLE hDir;
	int bOk = false;
	//strcpy(fileName,"D:\\softcontrol\\SiteviewLic\\IODll\\SysInfo.cpp");
	SplitFilePath(fileName, pBuf, name);
	hDir = CreateFileA(pBuf, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_DELETE, NULL, OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS, NULL);

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
#else	
	return true;
#endif
}
DLLEXPORT int GetSystemFeature(char* feature)
{
	unsigned char addrs[256]={0};
	unsigned char uniqueids[1024]={0}; 
    unsigned char cpuinfo[256]={0};
	unsigned char wmiinfo[512]={0};
  
	int Length = -1;

	memset(uniqueids, 0, 1024);	

    if(mac_addr_sys(addrs) == 1)				
    {
		 strcat(uniqueids, addrs);
    }
	if(GetCPUID(cpuinfo) == 1)				
    {
		if (strcmp(uniqueids,"") == 0)
		{
			strcat(uniqueids, "CPU:");
		}
		else
		{
			strcat(uniqueids, ",CPU:");
		}
		strcat(uniqueids, cpuinfo);
    }
#ifdef WIN32	
	if(winwmi(wmiinfo) == 1)				
	{
		if (strcmp(wmiinfo,"") != 0)
		{
			if (strcmp(uniqueids,"") != 0)		
			{
				strcat(uniqueids, ",");
			}
			strcat(uniqueids, wmiinfo);
		}
	}
#endif
	strcpy(feature,uniqueids);
	return true;

}

DLLEXPORT int GetSysFeature(char* feature)
{
	unsigned char addrs[256]={0};
	unsigned char uniqueids[1024]={0}; 
    unsigned char cpuinfo[256]={0};
	unsigned char wmiinfo[512]={0};
	int Length = -1;

	memset(uniqueids, 0, 1024);

	GetInstallTime(feature);
	strcat(feature, ",");

    if(mac_addr_sys(addrs) == 1)				
    {
		strcat(uniqueids, addrs);
    }
	if(GetCPUID(cpuinfo) == 1)				
    {
		strcat(uniqueids, ",CPU:");
		strcat(uniqueids, cpuinfo);
    }
#ifdef WIN32	
	if(winwmi(wmiinfo) == 1)				
	{
		if (strcmp(wmiinfo,"") != 0)
		{
			if (strcmp(uniqueids,"") != 0)		
			{
				strcat(uniqueids, ",");
			}
			strcat(uniqueids, wmiinfo);
		}
	}
#endif
	strcat(feature,uniqueids);
	return true;
	
}


int GetSysFeature1(const char* fileName, char* feature)
{
	unsigned char addrs[256]={0};
	unsigned char uniqueids[1024]={0}; 
    unsigned char cpuinfo[256]={0};
 	unsigned char wmiinfo[512]={0}; 
	int Length = -1;

	memset(uniqueids, 0, 1024);

	GetInstallTime1(fileName, feature);
	strcat(feature, ",");

    if(mac_addr_sys(addrs) == 1)				
    {
		strcat(uniqueids, addrs);
    }
	if(GetCPUID(cpuinfo) == 1)				
    {
		strcat(uniqueids, ",CPU:");
		strcat(uniqueids, cpuinfo);
    }
#ifdef WIN32	
	/*if(winwmi(wmiinfo) == 1)				
	{
		if (strcmp(wmiinfo,"") != 0)
		{
			if (strcmp(uniqueids,"") != 0)		
			{
				strcat(uniqueids, ",");
			}
			strcat(uniqueids, wmiinfo);
		}
	}*/
#endif
	strcat(feature,uniqueids);
	return true;	
}

int newGUID(char* buf)
{
	//--COM
	/*CoInitialize(NULL);
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
	CoUninitialize();*/
	return true;
}

void GetCurrentDate(char* date)
{
	struct   tm   *time_now;  
	time_t   secs_now;  
	tzset();  
	time(&secs_now);  
	time_now   =   localtime(&secs_now);  
	strftime(date, 10, "%Y%m%d", time_now);  
}

double DiffDate(const char* date1, const char* date2)
{
	struct tm t1, t2;
	time_t tt1,tt2;
	char buff[16] = {0};
	memset(buff,0,16);
	strncpy(buff,date1,4);	
	t1.tm_year = atoi(buff) -1900;
	memset(buff,0,16);
	strncpy(buff,date1+4,2);
	t1.tm_mon = atoi(buff);
	memset(buff,0,16);
	strncpy(buff,date1+6,2);
	t1.tm_mday = atoi(buff);
	t1.tm_hour=0;
	t1.tm_min=0;
	t1.tm_sec=1;
//	t1.tm_isdst=0;
	memset(buff,0,16);
	strncpy(buff,date2,4);
	t2.tm_year = atoi(buff) -1900;
	memset(buff,0,16);
	strncpy(buff,date2+4,2);
	t2.tm_mon = atoi(buff);
	memset(buff,0,16);
	strncpy(buff,date2+6,2);
	t2.tm_mday = atoi(buff);
	t2.tm_hour=0;
	t2.tm_min=0;
	t2.tm_sec=1;
	tt1 = mktime(&t1);
	tt2 = mktime(&t2);

	return difftime(tt1, tt2);
}



