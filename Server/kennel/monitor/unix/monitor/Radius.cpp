
#include "Radius.h"
#include "Utils.h"
#include "MD5.h"

#include "base\funcGeneral.h"

BOOL RADIUS_MONITOR(char *server, int port, char *secret, char *uid, char *pwd, 
					int timeout, char *match, char *custpath, char *szReturn)
{
	int		i = 0, 
			j = 0;
	int		ulen = strlen(uid), 
			plen = strlen(pwd), 
			slen = strlen(secret);

	SOCKET	s;
	struct	hostent *hp = NULL;
	struct	sockaddr_in	serv;

	DWORD	dwBeginTime = 0, 
			dwEndTime = 0;

	u_char	request[1000] = {0};
	int		reqlen = 0;
	u_char	response[1000] = {0};

	srand((unsigned)time(NULL));

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if(s == INVALID_SOCKET)
	{
		// Create Communication Error
		sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_Radius_1%>"));//<%IDS_Radius_1%>
		return FALSE;
	}

	hp = gethostbyname(server);
	if(hp == NULL)
	{
		// DNS Resolve Error
		sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_Radius_2%>"));//<%IDS_Radius_2%>
		return FALSE;
	}

	memcpy(&serv.sin_addr,hp->h_addr_list[0],hp->h_length);
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);

	// construct access-request packet
	request[reqlen++] = 0x01;				// Code
	request[reqlen++] = rand() % 0xFF;		// Identifier
	reqlen += 2;							// Length
	request[reqlen++] = rand() % 0xFF;		// Request Authenticator
	request[reqlen++] = rand() % 0xFF;
	request[reqlen++] = rand() % 0xFF;
	request[reqlen++] = rand() % 0xFF;
	request[reqlen++] = rand() % 0xFF;
	request[reqlen++] = rand() % 0xFF;
	request[reqlen++] = rand() % 0xFF;
	request[reqlen++] = rand() % 0xFF;
	request[reqlen++] = rand() % 0xFF;
	request[reqlen++] = rand() % 0xFF;
	request[reqlen++] = rand() % 0xFF;
	request[reqlen++] = rand() % 0xFF;
	request[reqlen++] = rand() % 0xFF;
	request[reqlen++] = rand() % 0xFF;
	request[reqlen++] = rand() % 0xFF;
	request[reqlen++] = rand() % 0xFF;

	request[reqlen++] = 0x01;				// Attribute Username
	request[reqlen++] = 0x02 + ulen;
	for(i = 0;i < ulen;i ++)
		request[reqlen++] = uid[i];

	request[reqlen++] = 0x02;				// Attribute Password (crypt)
	request[reqlen++] = 0x02 + (plen % 0x10 ? (plen / 0x10 + 1) * 0x10 : plen);

	/*******************************************************************
	 *  shared secret: S
	 *	pseudo-random 128-bit request authenticator: RA
	 *	password chuncks: P1, P2, P3 ...
	 *
	 *		B1 = MD5(S + RA)		C(1) = P1 xor B1
	 *		B2 = MD5(S + C(1))		C(2) = P2 xor B2
	 *		.						.
	 *		.						.
	 *		Bi = MD5(S + C(i - 1))	C(i) = Pi xor Bi
	 *
	 *	result: C(1) + C(2) + ... + C(i)
	 *
	 *	[RFC2058]
	 *******************************************************************/
	unsigned char output[16];
	unsigned char input[1024];
	unsigned char ilen;
	for(i = 0;i < plen;i += 16)
	{
		memset(input, 0, sizeof(input));
		ilen = 0;

		strcpy((char *)input, secret);
		ilen += slen;

		if(i == 0)
			for(j = 0;j < 16;j ++) input[ilen++] = request[4 + j];
		else
			for(j = 0;j < 16;j ++) input[ilen++] = output[j];

		MD5(output, input, ilen);

        int m = 0;
		for(m = 0, j = i;j < (plen - i < 16 ? plen : 16 + i);j ++, m++)
			output[m] ^= pwd[j];

		for(j = 0;j < 16;j ++)
			request[reqlen++] = output[j];
	}

	request[2] = reqlen / 0xFF;
	request[3] = reqlen % 0xFF;

	dwBeginTime = ::GetTickCount();

	// parse radius response
	int dw, n;
	n = sendto(s, (char *)request, reqlen, 0, (struct sockaddr *)&serv, sizeof(serv));
	if(n == SOCKET_ERROR) 
	{
		// Send Data Error
		sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_Radius_3%>"));//<%IDS_Radius_3%>
		return FALSE;
	}
	
w:	if(RESPONSE_WAIT(s, timeout) > 0)
	{
		dw = sizeof(serv);
		n = recvfrom(s, (char *)response, 1000, 0, (struct sockaddr *)&serv, &dw);
		if(n == SOCKET_ERROR) 
		{
			// Receive Data Error
			sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_Radius_4%>"));//<%IDS_Radius_4%>
			return FALSE;
		}

		if(n == 0)
		{
			// No Data Responsed From Server
			sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_Radius_5%>"));//<%IDS_Radius_5%>
			return FALSE;
		}

		if(response[1] != request[1]) goto w;

		if(response[0] == 0x02)
		{
			dwEndTime = ::GetTickCount();
			sprintf(szReturn, "roundTripTime=%ld$", dwEndTime - dwBeginTime);
			return TRUE;
		}
		else if(response[0] == 0x03)
		{
			if(response[20] == 0x12)
				// Request Refused
				sprintf(szReturn, "error=%s: %s$", ::FuncGetStringFromIDS("<%IDS_Radius_6%>"), response + 22);//<%IDS_Radius_6%>
			else
				// Request Refused
				sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_Radius_7%>"));//<%IDS_Radius_7%>

			return FALSE;
		}
		else
		{
			// Unknown Error
			sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_Radius_8%>"));//<%IDS_Radius_8%>
			return FALSE;
		}
	}
	else
	{
		// Request Timeout
		sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_Radius_9%>"));//<%IDS_Radius_9%>
		return FALSE;
	}

	return TRUE;
}
