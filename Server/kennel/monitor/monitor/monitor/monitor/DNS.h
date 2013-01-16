
#define		DNS_TIMEOUT				5

#include "stdafx.h"

BOOL DNS_MONITOR(char *server, int port, char *domain, CStringList &lstAddress, 
				 char *custpath, char *szReturn);
void DNSMessageParser(u_char *buff, int len);
int VerifyDNS(u_char *result, int len, CStringList &lstAddress);

