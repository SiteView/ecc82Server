
#include "stdafx.h"

BOOL CISCOCPU_MONITOR(char *server, int port, CString strIndex, char *community, 
					  int timeout, int nrdelay, char *custpath, char *szReturn);
BOOL HUAWEICPU_MONITOR(char *server, int port, CString strIndex, char *community, 
					  int timeout, int nrdelay, char *custpath, char *szReturn);
BOOL HUAWEISWITCH_CPU(char *server, int port, CString strIndex, char *community, 
					  int timeout, int nrdelay, char *custpath,  CString  strType,
					  char *szReturn);