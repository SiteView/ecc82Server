
#include "stdafx.h"

BOOL VPN_MONITOR(char *vpnconn, char *uid, char *pwd, char *domain, int dialbeforevpn, 
				 char *dialconn, char *uidd, char *pwdd, char *domaind, 
				 char *cpath, char *szReturn);
BOOL DialCourse(char *dialconn, char *uid, char *pwd, char *domain, char *cpath, char *szReturn);
BOOL VpnCourse(char *vpnconn, char *uid, char *pwd, char *domain, char *cpath, char *szReturn);
