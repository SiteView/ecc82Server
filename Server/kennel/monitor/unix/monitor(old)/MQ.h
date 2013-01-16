
#include "stdafx.h"

#define		MQ_TIMEOUT			60

bool MQ_TEL_VALUE(char *server, int port, char *uid, char *pwd, char *pserver, int pport, 
				  char *puid, char *ppwd, char *lprom, char *pprom, char *prom, 
				  char *qm, char *qn, char *custpath, char *szReturn);
bool MQ_SSH_VALUE(char *server, int port, char *uid, char *pwd, char *qm, char *qn, 
				  char *custpath, char *szReturn);
bool HTTP_MQ_VALUE(char *url, char *uid, char *pwd, char *pserver, int pport, 
				   char *puid, char *ppwd, char *qm, char *qn, char *custpath, 
				   char *szReturn);
bool RLOGIN_MQ_VALUE(char *server, int port, char *uid, char *pwd, char *pserver, int pport, 
				     char *puid, char *ppwd, char *pprom, char *prom, 
				     char *qm, char *qn, char *custpath, char *szReturn);
int MQ_GETDATA(int hSocket, char *qm, char *qn, CString &strBuffer);
int MQ_PARSE(CString strBuffer, char *szReturn);

bool MQ_TEL_STATUS_VALUE(char *server, int port, char *uid, char *pwd, char *pserver, int pport, 
				  char *puid, char *ppwd, char *lprom, char *pprom, char *prom, 
				  char *qm, char *qn, char *cn, char *custpath, char *szReturn);
int MQ_STATUS_GETDATA(int hSocket, char *qm, char *qn, char *cn, CString &strBuffer);
int MQ_STATUS_PARSE(CString strBuffer, char *szReturn);