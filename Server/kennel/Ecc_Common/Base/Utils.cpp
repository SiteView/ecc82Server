#include "Utils.h"
#include ".\Base64.h"
#include "math.h"


HINSTANCE g_hInstance = NULL;
HINSTANCE g_hInstance2 = NULL;

/*
int WSA_Init()
{
	WORD wVersionRequested;  
	WSADATA wsaData; 
	int err; 
	wVersionRequested = MAKEWORD(1, 1); 

	err = WSAStartup(wVersionRequested, &wsaData); 

	if (err != 0) 
		return -1; 

	if ( LOBYTE( wsaData.wVersion ) != 1 || 
	   HIBYTE( wsaData.wVersion ) != 1 ) 
	{ 
		WSACleanup(); 
		return -2;
	}

	return 0;
}

void WSA_Free()
{
	WSACleanup();
}
*/

int connect_timeo(int sockfd,const struct sockaddr *saptr,int salen,int nsec)
{
	int n;
	int error;
	int len;
	fd_set rset,wset;
	struct timeval tv;
	
	u_long val = 1;
	ioctlsocket(sockfd,FIONBIO,&val);

	error = 0;
	if((n = connect(sockfd,saptr,salen)) < 0)
	{
		if(WSAGetLastError() != WSAEWOULDBLOCK) 
		{
			return -1;
		}
	}	

	if(n == 0)
		goto done;

	FD_ZERO(&rset);
	FD_SET(sockfd,&rset);
	wset = rset;
	tv.tv_sec = nsec;
	tv.tv_usec = 0;

	if((n = select(sockfd + 1,&rset,&wset,NULL,nsec?&tv:NULL)) == 0)
	{
		shutdown(sockfd, 0x02);
		closesocket(sockfd);
		WSASetLastError(WSAETIMEDOUT);
		return -1;
	}

	if(FD_ISSET(sockfd,&rset) || FD_ISSET(sockfd,&wset))
	{
		len = sizeof(error);
		if(getsockopt(sockfd,SOL_SOCKET,SO_ERROR,(char *)&error,&len) < 0)
		{
			shutdown(sockfd, 0x02);
			closesocket(sockfd);
			return -1;
		}
	}			
	else
	{
		//printf("select error: sockfd not set");
		return -1;
	}		

done:
	val = 0;
	ioctlsocket(sockfd,FIONBIO,&val);
	if(error)
	{
		shutdown(sockfd, 0x02);
		closesocket(sockfd);
		WSASetLastError(error);
		return -1;
	}	
	return 0;
}

int RESPONSE_WAIT(SOCKET s, int nsec)
{
	struct timeval Timeout;
	fd_set readfds;

	readfds.fd_count = 1;
	readfds.fd_array[0] = s;
	Timeout.tv_sec = nsec;
    Timeout.tv_usec = 0;
	return(select(1, &readfds, NULL, NULL, &Timeout));
}

int socks5_protocol(SOCKET s, char *server, int port, char *pserver, int pport, 
					char *puid, char *ppwd, int timeout)
{
	struct hostent *hent;
	unsigned long	ip;
	sockaddr_in		sa;

	hent = gethostbyname(pserver);
	if(hent == NULL)
		return 1;

	memcpy(&sa.sin_addr, hent->h_addr_list[0], hent->h_length);
	sa.sin_family = AF_INET;
	sa.sin_port = htons(pport);

	if(connect_timeo(s, (sockaddr*)&sa, sizeof(sockaddr), timeout) < 0)
		return 1;

	unsigned char request[22];
	int request_len = 0;
	unsigned char response[22];
	int response_len = 0;

	request[0] = 0x05; // SOCKS version 5
	request[1] = 0x03; // number of auth procotols we recognize
	request[2] = 0x00; // no authentication required
	request[3] = 0x01; // GSSAPI
	request[4] = 0x02; // username/password

	request_len = 2 + request[1];
	int write_len = send(s, (const char *)request, request_len, 0);
	if (write_len != request_len) 
		return 1;

	response_len = 22;
	response_len = recv(s, (char *)response, response_len, 0);

	if (response_len <= 0) 
		return 1;

	if (response[0] != 0x05) 
		return 1;

	switch (response[1]) 
	{
		case 0x00:
			// no auth
			break;
		case 0x01:
			// GSSAPI
			return 1;
		case 0x02:
			{
			// username/password
			int i = 0;
			request[0] = 0x01;
			if(*puid)
			{
				request[1] = strlen(puid);
				for(i = 0;i < (int)strlen(puid);i ++)
					request[2+i] = puid[i];
			}
			else
			{
				request[1] = 0x01;
				request[2] = 0x00;
			}
			request_len = 2 + i;

			if(*ppwd)
			{
				request[request_len++] = strlen(ppwd);
				for(i = 0;i < (int)strlen(ppwd);i ++)
					request[request_len+i] = ppwd[i];
				request_len += strlen(ppwd);
			}
			else
			{
				request[request_len++] = 0x01;
				request[request_len++] = 0x00;
			}

			write_len = send(s, (const char *)request, request_len, 0);
			response_len = 22;
			response_len = recv(s, (char *)response, response_len, 0);
			if(response[0] != 1)
				return 1;
			if(response[1] != 0)
				return 1;
			}
			break;
		default:
			return 1;
	}

	// we are now authenticated, so lets tell
	// the server where to connect to
	request_len = 6;

	request[0] = 0x05; // SOCKS version 5
	request[1] = 0x01; // CONNECT command
	request[2] = 0x00; // obligatory reserved field (perfect for MS tampering!)
    
	request[3] = 0x01; // encoding of destination address (1 == IPv4)
	request_len += 4;

	char	ipaddr[20] = {0};
	ip = inet_addr(server);
	if (ip == INADDR_NONE)
	{
		hent = gethostbyname(server);
		if (hent == NULL)
			return 1;

		strcpy(ipaddr, inet_ntoa(*((struct in_addr *)hent->h_addr)));
	}
	else
	{
		strcpy(ipaddr, server);
	}
	
	if(sscanf(ipaddr, "%d.%d.%d.%d", &request[4], &request[5], &request[6], &request[7]) != 4)
		return 1;

	// destination port
	unsigned int destPort = htons(port);

	request[request_len-1] = (unsigned char)(destPort >> 8);
	request[request_len-2] = (unsigned char)destPort;

	if (send(s, (const char *)request, request_len, 0) != request_len)
		return 1;

	response_len = 22;
	response_len = recv(s, (char *)response, response_len, 0);
	if (response_len <= 0)
		return 1;

	if (response[0] != 0x05)
		return 1;

	if (response[1] != 0x00)
		return 1;

	return 0;
}

