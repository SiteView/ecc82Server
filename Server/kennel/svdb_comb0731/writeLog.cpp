/*
 Luowan 2007.5.31

 ¼ÇÂ¼ Log
*/

#include "writeLog.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

cWriteLog::cWriteLog(void)
{
	strcpy(cFilename, "SVDBtrack.log");

	bOnlyDebug	= true;
	bWriteToFile = false;

	isDebug = false;
#ifndef _DEBUG
	isDebug = true;
#endif
}

bool cWriteLog::set_Filename(char *filename)
{
	strcpy(cFilename, filename);

	return true;
}

bool cWriteLog::set_WriteToFile(bool bToFile)
{
	bWriteToFile = bToFile;

	return true;
}

bool cWriteLog::set_OnlyInDebug(bool bInDebug)
{
	bOnlyDebug = bInDebug;

	return true;
}

bool cWriteLog::clear(void)
{
	FILE* log;
		
	log = fopen(cFilename, "w");
		
	if(log != NULL)
		fclose(log);
	else
		return false;

	return true;
}

size_t cWriteLog::WriteLog(char *logInfo)
{
	if(bOnlyDebug == true)
	{
		if(isDebug == true)
			return 0;
	}

	char timeBuf[128], dateBuf[128];
	char *pBuf;
	size_t size;

	
	_tzset();
	
	_strtime( timeBuf );
	_strdate( dateBuf );
	
	size = strlen(timeBuf)+ strlen(dateBuf)+strlen(logInfo);

	pBuf = new char[size+20];

    sprintf(pBuf, "%s %s\t", dateBuf, timeBuf );
	
	strcat(pBuf, " >>> ");
	strcat(pBuf, logInfo);
	strcat(pBuf, "\n");

	size = strlen(pBuf);
	if(bWriteToFile == false)
	{
		printf(pBuf);
	}
	else
	{
		FILE* log;
		
		log = fopen(cFilename, "a+");
		
		if(log != NULL)
		{
			fprintf(log, "%s", pBuf);
			fclose(log);
		}
	}

	delete pBuf;
	return size;
}
