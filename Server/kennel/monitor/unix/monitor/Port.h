
#include "stdafx.h"
#include "Utils.h"

BOOL PORT_MONITOR(char *server, int port, char *sendstring, char *matchstring, 
				  int timeout, char *pserver, int pport, char *puid, char *ppwd, 
				  char *custpath, char *szReturn);
int TcpConnectPort(char *Host, int Port, char *SendStr, char *MatchStr, int Timeout, 
				   char *szReturn);

