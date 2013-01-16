#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>


#define FALSE 0 
#define TRUE 1

#define false 0 
#define true 1


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
	char buf[200];
	char *date; 
	int res;
	struct stat fstat; 
	struct   tm   *time_now;  	 
	
	if(getcwd(buf, sizeof(buf)-1) != NULL)
	{
	    if ((res = stat (buf, &fstat)) != 0)
	    {   
		strncat(instime, "IST:0000000000000000", 20);
	    }
	    else
	    {	   
		time_now = localtime (& fstat. st_ctime); 
		strftime(buf, 20, "IST:%Y%m%d%H%M%S", time_now);  
		strncpy(instime, buf, 20);
		return true;		    
	    }
	}
	else
	{
		strncat(instime, "IST:0000000000000000", 20);
	}
	return false;
#endif
}

void GetCurrentDateTime(char* date)
{
	struct   tm   *time_now;  
	time_t   secs_now;  
	tzset();  
	time(&secs_now);  
	time_now   =   localtime(&secs_now);  
	strftime(date, 16, "%Y%m%d", time_now);  
}

int main(int argc, char *argv[])
{
	char instime[200];
	char time[200];
	GetInstallTime(instime);
	printf ("Date:%s\r\n", instime); 
	
	GetCurrentDateTime(time);
	
	printf ("time:%s\r\n", time); 

	return 0;
}


