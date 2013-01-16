
#include "stdafx.h"


BOOL LOGFILE_TEL_VALUE(char *server, int port, char *uid, char *pwd, char *pserver, 
					   int pport, char *puid, char *ppwd, char *lprom, char *pprom, 
					   char *prom, char *inifile, char *logfile, char *match, 
					   char *rulefile, int append, int ngid, char* strmid, char *custpath, 
					   int nOnce, char *szReturn);
BOOL LOGFILE_SSH_VALUE(char *server, int port, char *uid, char *pwd, char *inifile, 
					   char *logfile, char *match, char *rulefile, int append, int ngid, 
					    char* strmid, char *custpath,int nOnce, char *privatekeyfile, char *szReturn);

BOOL HTTP_LOGFILE_VALUE(char *url, char *uid, char *pwd, char *pserver, 
		    		    int pport, char *puid, char *ppwd, char *inifile, 
						char *logfile, char *match, char *rulefile, int append, 
						int ngid, int nmid, char *custpath, char *szReturn);

BOOL RLOGIN_LOGFILE_VALUE(char *server, int port, char *uid, char *pwd, char *pserver, 
					      int pport, char *puid, char *ppwd, char *pprom, 
					      char *prom, char *inifile, char *logfile, char *match, 
					      char *rulefile, int append, int ngid, int nmid, char *custpath, 
					      char *szReturn);

int TelLogFileSize(int hSocket, char *inifile, char *logfile, long &size);
int RloginLogFileSize(int hSocket, char *inifile, char *logfile, long &size);
int GetColumnFromLine(CString strIn, int nColumn, long &size);
int SshLogFileSize(char *server, int port, char *uid, char *pwd, char *inifile, char *logfile,char *privatekeyfile, long &size);
int HttpLogFileSize(char *url, char *uid, char *pwd, char *pserver, int pport, 
					char *puid, char *ppwd, char *inifile, char *logfile, long &size);

int TelLogFileExist(int hSocket, char *inifile, char *logfile);

int SshLogFileExist(char *server, int port, char *uid, char *pwd, char *inifile, char *logfile, char *privatekeyfile);

void WriteLogData(char *msg, const char* strmid);

void WriteLogTime(const char* strmid);

void AddToLogFileTemLog(char *msg, int ngid, int nmid);

//yi.duan improve SshLogFile
int SshLogFileLine(char *server, int port, char *uid, char *pwd, char *inifile, char *logfile,char *privatekeyfile, long &size);
BOOL LOGFILE_SSH_VALUE_IMPROVE(char *server, int port, char *uid, char *pwd, char *inifile, 
							   char *logfile, char *match, char *rulefile, int append, int ngid, 
							   char* strmid, char *custpath,int nOnce,char *privatekeyfile, char *szReturn);