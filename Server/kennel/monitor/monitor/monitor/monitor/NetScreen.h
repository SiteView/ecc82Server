
#include "stdafx.h"

BOOL NETSCREENRES_MONITOR(char *server, int port, char *community, CString strIndex, int retrydealy, 
					      int timeout, char *custpath, char *szReturn);

BOOL NETSCREENSERVICE_MONITOR(char *server, int port, char *community, CString strIndex, int retrydealy, 
							  int timeout, char *custpath, char *szReturn);

BOOL NETSCREENSERVICEGROUP_MONITOR(char *server, int port, char *community, CString strIndex, int retrydealy, 
								   int timeout, char *custpath, char *szReturn);

BOOL NETSCREENSERVICEGROUPMEMBER_MONITOR(char *server, int port, char *community, CString strIndex, int retrydealy, 
								   int timeout, char *custpath, char *szReturn);

BOOL NETSCREENVPN_MONITOR(char *server, int port, char *community, CString strIndex, int retrydealy, 
					      int timeout, char *custpath, char *szReturn);

BOOL NETSCREENATTACK_MONITOR(char *server, int port, char *community, CString strIndex, int retrydealy, 
							 int timeout, char *custpath, char *szReturn);

BOOL NETSCREENPACKDROP_MONITOR(char *server, int port, char *community, CString strIndex, int retrydealy, 
							   int timeout, char *custpath, char *szReturn);
