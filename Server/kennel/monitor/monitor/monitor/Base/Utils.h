//#include "stdafx.h"
#include <afxsock.h>

int WSA_Init();
void WSA_Free();

int connect_timeo(int sockfd, const struct sockaddr *saptr, int salen, int nsec);
int RESPONSE_WAIT(SOCKET s, int nsec);
int socks5_protocol(SOCKET s, char *server, int port, char *pserver, int pport, 
					char *puid, char *ppwd, int timeout);