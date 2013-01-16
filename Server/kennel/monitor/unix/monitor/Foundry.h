
#include "stdafx.h"

BOOL FOUNDRYCPUS_MONITOR(char *server, int port, char *community, char *cpath, char *szReturn);
BOOL FOUNDRYCPU_MONITOR(char *server, int port, char *community, CString strIndex, 
			int rdelay, int timeout, char *cpath, char *szReturn);
BOOL FOUNDRYCPU2_MONITOR(char *server, int port, char *community, CString strIndex, 
			int rdelay, int timeout, char *cpath, char *szReturn);
BOOL FOUNDRYMEM_MONITOR(char *server, int port, char *community, CString strIndex, 
			int rdelay, int timeout, char *cpath, char *szReturn);
