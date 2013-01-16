
#include "stdafx.h"

BOOL CTRONSSRCPU_MONITOR(char *server, int port, char *community, int rdelay, int timeout, char *cpath, char *szReturn);
BOOL CTRONETHERSTATUS_MONITOR(char *server, int port, char *community, CString strIndex, 
			int rdelay, int timeout, int ngid, int nmid, char *cpath, char *szReturn);
BOOL CtronEtherIndexs(char *server, int port, char *community, int rdelay, int timeout, char *cpath, CString &strBuffer);
