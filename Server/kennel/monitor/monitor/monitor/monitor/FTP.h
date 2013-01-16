
#include "stdafx.h"

BOOL FTP_MONITOR(char *server, int port, char *file, char *uid, char *pwd, 
				 int timeout, int ptype, char *pserver, int pport, 
				 char *puid, char *ppwd, char *match, char *custpath, char *szReturn);
BOOL TFTP_MONITOR(char *server, int port, char *file, int timeout, char *custpath, 
				  char *szReturn);
int OpenControlChannel(char *hostname, int port, char *username, char *password, 
					   int timeout, char *retrfile, int ptype, char *pserver, int pport, 
					   char *puid, char *ppwd, char *szReturn);
int OpenDataChannel(char *retrfile, int timeout, int ptype, 
					char *pserver, int pport, char *puid, char *ppwd, char *szReturn);
void QuitFromServer();
int WaitVersionInfo(SOCKET s, int nSec);
int FTP_COMMAND(SOCKET s, CString strCmd, CString &strMessage);
int EnterPassiveMode();
int WriteString(SOCKET s, CString sendStr);
int ReadString2(SOCKET s, char *buffer);
