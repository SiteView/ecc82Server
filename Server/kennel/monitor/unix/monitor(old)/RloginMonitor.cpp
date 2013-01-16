
#include "RloginMonitor.h"
#include "Utils.h"

#include "..\..\base\funcGeneral.h"

char	rlogin_databuffer[BUF_SIZE] = {0};
int		rlogin_datalen = 0;

#ifdef WIN32
#pragma warning (disable : 4267)
#endif

SOCKET rlogin_init(char *server, int port, char *pserver, int pport, char *puid, char *ppwd)
{
	unsigned long	ip;
	hostent			*ent;
	sockaddr_in		rsa;
	sockaddr_in		lsa;
	SOCKET			hSocket;

	if(*pserver)
	{
		ent = gethostbyname(pserver);
		if(ent == NULL)
			return -4;

		memcpy(&rsa.sin_addr,ent->h_addr_list[0],ent->h_length);
		rsa.sin_family = AF_INET;
		rsa.sin_port = htons(pport);
	
		hSocket = socket(AF_INET,SOCK_STREAM,0);
		if (hSocket == INVALID_SOCKET) 
			return -2;

		if(connect_timeo(hSocket, (sockaddr*)&rsa, sizeof(sockaddr), RLOGIN_CONNECT_TIMEOUT) < 0)
		//if(connect(hSocket, (struct sockaddr*)&sa, sizeof(sa)) < 0)
			return -4;

		unsigned char request[22];
		int request_len = 0;
		unsigned char response[22];
		int response_len = 0;
    
		request[0] = 0x05; // SOCKS version 5
		request[1] = 0x03; // number of auth procotols we recognize
		// auth protocols
		request[2] = 0x00; // no authentication required
		// compliant implementations MUST implement GSSAPI
		// and SHOULD implement username/password
		// TODO: we don't
		request[3] = 0x01; // GSSAPI
		request[4] = 0x02; // username/password
    
		request_len = 2 + request[1];
		int write_len = send(hSocket, (const char *)request, request_len, 0);
		if (write_len != request_len) 
			return -5;
    
		// get the server's response
		response_len = 22;
		response_len = recv(hSocket, (char *)response, response_len, 0);
    
		if (response_len <= 0) 
			return -5;
    
		if (response[0] != 0x05) 
			// it's a either not SOCKS or not our version
			return -5;

		switch (response[1]) 
		{
			case 0x00:
				// no auth
				break;
			case 0x01:
				// GSSAPI
				// TODO: implement
				return -5;
			case 0x02:
				{
				// username/password
				// TODO: implement
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

				write_len = send(hSocket, (const char *)request, request_len, 0);
				response_len = 22;
				response_len = recv(hSocket, (char *)response, response_len, 0);
				if(response[0] != 1)
					return -5;
				if(response[1] != 0)
					return -5;
				}
				break;
			default:
				// unrecognized auth method
				return -5;
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
			ent = gethostbyname(server);
			if (ent == NULL)
				return -1;

			strcpy(ipaddr, inet_ntoa(*((struct in_addr *)ent->h_addr)));
		}
		else
		{
			strcpy(ipaddr, server);
		}
		
		if(sscanf(ipaddr, "%d.%d.%d.%d", &request[4], &request[5], &request[6], &request[7]) != 4)
			return -3;

		// destination port
		unsigned int destPort = htons(port);
    
		request[request_len-1] = (unsigned char)(destPort >> 8);
		request[request_len-2] = (unsigned char)destPort;
    
		if (send(hSocket, (const char *)request, request_len, 0) != request_len) {
			// bad write
			return -5;
		}
    
		response_len = 22;
		response_len = recv(hSocket, (char *)response, response_len, 0);
		if (response_len <= 0) {
			// bad read
			return -5;
		}
    
		if (response[0] != 0x05) {
			// bad response
			return -5;
		}
    
		if (response[1] != 0x00) {
			// SOCKS server failed to connect
			return -5;
		}
	}
	else
	{
		ent = gethostbyname(server);
		if(ent == NULL) 
			return -1;

		hSocket = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
		if (hSocket == INVALID_SOCKET) 
			return -2;

		memset(&lsa, 0, sizeof(lsa));
		lsa.sin_family = AF_INET;
		lsa.sin_addr.s_addr = htonl(INADDR_ANY);
		lsa.sin_port=htons(RLOGIN_CLIENT_PORT);
		if(bind(hSocket, (struct sockaddr *)&lsa, sizeof(lsa)) == SOCKET_ERROR)
			return -6;

		ip = *(unsigned long*)(ent->h_addr);

		rsa.sin_family = AF_INET;
		rsa.sin_port = htons(port);
		rsa.sin_addr = *(in_addr*)&ip;

		if(connect_timeo(hSocket, (sockaddr*)&rsa, sizeof(sockaddr), RLOGIN_CONNECT_TIMEOUT) < 0)
			return -3;
	}

	return hSocket;
}

int rlogin_connect(SOCKET hSocket, char *uid, char *pwd, char *pwdprompt, char arprompt[PR_NUM][256])
{
	int		n = 0;
	char	buffer[4096] = {0};
	char	*scan = NULL;
	int		rloginprogress = 1;
	int		count = 0;

	u_char	req[1024] = {0};
	u_char	res[1024] = {0};
	int		reqlen = 0;

	req[reqlen ++] = 0x00;

	memcpy(req + reqlen, uid, strlen(uid));
	reqlen += strlen(uid);
	req[reqlen ++] = 0x00;

	memcpy(req + reqlen, uid, strlen(uid));
	reqlen += strlen(uid);
	req[reqlen ++] = 0x00;

	memcpy(req + reqlen, TERM_TYPE, strlen(TERM_TYPE));
	reqlen += strlen(TERM_TYPE);
	memcpy(req + reqlen, "/", 1);
	reqlen ++;
	memcpy(req + reqlen, TERM_SPEED, strlen(TERM_SPEED));
	reqlen += strlen(TERM_SPEED);
	req[reqlen ++] = 0x00;

	n = send(hSocket, (char *)req, reqlen, 0);
	if(RESPONSE_WAIT(hSocket, RLOGIN_READ_TIMEOUT) <= 0)
		return -1;

	n = recv(hSocket, (char *)res, sizeof(res), 0);
	if(n == SOCKET_ERROR || n == 0)
		return -2;

	if(res[0] != 0)
		return -7;

	memset(rlogin_databuffer, 0, BUF_SIZE);
	rlogin_datalen = 0;
	while(1)
	{
		memset(buffer, 0, 4096);
		if(RESPONSE_WAIT(hSocket, RLOGIN_READ_TIMEOUT) <= 0)
			return -1;

		Sleep(500);

		n = recv(hSocket, buffer, 4096, 0);
		if(n == SOCKET_ERROR)
			return -2;
		buffer[n] = 0;

		scan = buffer;
		while(n--)
		{
			rlogin_protocol(hSocket, *scan++);
			TRACE("%c", *scan);
		}
		switch(rloginprogress)
		{
//		case 0:
//			if(strstr(databuffer, logprompt))
//			{
//				char	buf[255] = {0};
//				strcpy(buf, uid);
//				strcat(buf, "\r\n");
//				Sleep(100);			// delay
//				send(hSocket, buf, strlen(buf),0);
//				telnetprogress ++;
//				count = 0;
//				memset(databuffer, 0, BUF_SIZE);
//				datalen = 0;
//			}
//			break;
		case 1:
			if(strstr(rlogin_databuffer, pwdprompt))
			{
				char	buf[255] = {0};
				strcpy(buf, pwd);
				strcat(buf, "\r\n");
				Sleep(100);			// delay
				send(hSocket, buf, strlen(buf), 0);
				rloginprogress ++;
				count = 0;
				memset(rlogin_databuffer, 0, BUF_SIZE);
				rlogin_datalen = 0;
			}
			break;
		case 2:
			if(strstr(rlogin_databuffer, "ogin incorrect"))
			{
				memset(rlogin_databuffer, 0, BUF_SIZE);
				rlogin_datalen = 0;
				return -6;
			}

			int		i = 0;
			char	prompt[256];
			while(i < PR_NUM && *arprompt[i])
			{
				memset(prompt, 0, sizeof(prompt));
				strcpy(prompt, arprompt[i]);

				if(strstr(rlogin_databuffer, prompt))
				{
					memset(rlogin_databuffer, 0, BUF_SIZE);
					rlogin_datalen = 0;
					return 0;
				}

				i ++;
			}
			break;
		}

		if(++count > 80)
		{
			if(rloginprogress == 0)
				return -3;
			if(rloginprogress == 1) 
				return -4;
			if(rloginprogress == 2) 
				return -5;
		}
	}
}

void rlogin_protocol(SOCKET server,unsigned char code)
{
	rlogin_databuffer[rlogin_datalen ++] = code;
/*
	//These vars are the finite state
	static int state = state_data;
	static _verb verb = verb_sb;
	static LPDATAPROC DataProc = terminal[(term_index==NUM_TERMINALS)?(NUM_TERMINALS-1):term_index].termproc;

	//Decide what to do (state based)
	switch(state)
	{
	case state_data:
		switch(code)
		{
		case IAC: state = state_code; break;
		default: DataProc(server,code);
		}
		break;
	case state_code:
		state = state_data;
		switch(code)
		{
		// State transition back to data
		case IAC: 
			DataProc(server,code);
			break;
		// Code state transitions back to data
		case SE:
			DataProc = terminal[(term_index==NUM_TERMINALS)?(NUM_TERMINALS-1):term_index].termproc;
			break;
		case NOP:
			break;
		case DM:
			break;
		case BRK:
			break;
		case IP:
			break;
		case AO:
			break;
		case AYT:
			break;
		case EC:
			break;
		case EL:
			break;
		case GA:
			break;
		// Transitions to option state
		case SB:
			verb = verb_sb;
			state = state_option;
			break;
		case WILL:
			verb = verb_will;
			state = state_option;
			break;
		case WONT:
			verb = verb_wont;
			state = state_option;
			break;
		case DO:
			verb = verb_do;
			state = state_option;
			break;
		case DONT:
			verb = verb_dont;
			state = state_option;
		break;
		}
		break;
	case state_option:
		state = state_data;

		//Find the option entry
		for(
			int i = 0;
			ol[i].option != TOPT_ERROR && ol[i].option != code;
			i++);

		//Do some verb specific stuff
		if(verb == verb_sb)
			DataProc = ol[i].DataProc;
		else
			ol[i].OptionProc(server,verb,(_option)code);
		break;
	}
*/
}

int rlogin_command(SOCKET hSocket, char *cmd)
{
	int		n = 0;
	char	buffer[4096] = {0};
	char	*scan = NULL;
	char	*ca = NULL, *cb = NULL;

	sprintf(buffer, "echo %s;%s;echo %s\r\n", C_STA, cmd, C_END);

	send(hSocket, buffer, strlen(buffer), 0);

	memset(rlogin_databuffer, 0, BUF_SIZE);
	rlogin_datalen = 0;
	while(RESPONSE_WAIT(hSocket, RLOGIN_READ_TIMEOUT))
	{
		n = recv(hSocket, buffer, 4096, 0);
		if(n == SOCKET_ERROR) return -2;
		if(n == 0) return -2;
		buffer[n] = 0;

		scan = buffer;
		while(n--) rlogin_protocol(hSocket, *scan++);

		char match_1[20] = {0}, match_2[20] = {0};
		sprintf(match_1, "\r\n%s", C_STA);
		sprintf(match_2, "\r\n%s", C_END);
		if(ca = strstr(rlogin_databuffer, match_1)) 
		{
			if(cb = strstr(rlogin_databuffer, match_2))
			{
				ca += strlen(C_STA) + 4;
				cb += 2;
				strncpy(rlogin_databuffer, ca, cb - ca);
				rlogin_databuffer[cb - ca] = 0;
				break;
			}
		}
	}

	return strlen(rlogin_databuffer);
}

BOOL RloginMonitorValue(char *server, int port, char *uid, char *pwd, 
						char *pserver, int pport, char *puid, char *ppwd, 
						char *pwdprompt, char *prompts, 
						char *inifile, int mtype, char *extraparam, char *custpath, 
						char *szReturn)
{
	int		ret = 0;
	int		hSocket = 0;
	BOOL	bResult = TRUE;
	char	iniFile[255] = {0};
	char	cmd[255] = {0};

	/*************************************
	 * For Reason Of Prompt
	 */
	int		c = 0;
	char	*ca = prompts, *cb = NULL;
	char	arprompt[PR_NUM][256];
	memset(arprompt, 0, sizeof(arprompt));

	while(cb = strchr(ca, 0x20))
	{
		if(ca == cb) 
		{
			ca ++;
			continue;
		}

		strncpy(arprompt[c++], ca, cb - ca);
		ca = cb + 1;
	}

	strcpy(arprompt[c++], ca);
	/***********************************/

	hSocket = rlogin_init(server, port, pserver, pport, puid, ppwd);
	if(hSocket <= 0)
	{
		if(hSocket == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_1%>"));    //  <%IDS_TelMonitor_1%>
		else if(hSocket == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_2%>"));   //  <%IDS_TelMonitor_2%>
		else if(hSocket == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_3%>"));     //  <%IDS_TelMonitor_3%>
		else if(hSocket == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_4%>"));     //  <%IDS_TelMonitor_4%>
		else if(hSocket == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_5%>"));        //  <%IDS_TelMonitor_5%>
		else 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_6%>"));     //  <%IDS_TelMonitor_6%>

		bResult = FALSE;
		goto err;
	}

	if((ret = rlogin_connect(hSocket, uid, pwd, pwdprompt, arprompt)) < 0)
	{
		if(ret == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_7%>"));    //  <%IDS_TelMonitor_7%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_8%>"));     //  <%IDS_TelMonitor_8%>
		else if(ret == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_9%>"));     //  <%IDS_TelMonitor_9%>
		else if(ret == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_10%>"));   //  <%IDS_TelMonitor_10%>
		else if(ret == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_11%>"));   //  <%IDS_TelMonitor_11%>
		else if(ret == -6)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_12%>"));   //  <%IDS_TelMonitor_12%>
		else 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_13%>"));     //  <%IDS_TelMonitor_13%>

		bResult = FALSE;
		goto err;

	}

	GetIniFileName(inifile, iniFile);
	
	if(!RETURN_COMMAND(mtype, iniFile, cmd))
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_14%>"));    //  <%IDS_TelMonitor_14%>
		bResult = FALSE;
		goto err;
	}

	if(ca = strchr(cmd, '<')) 
	{
		strcpy(ca, extraparam);
		*(ca + strlen(extraparam)) = 0;
	}

	if((ret = rlogin_command(hSocket, cmd)) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_15%>"));    //  <%IDS_TelMonitor_15%>
		bResult = FALSE;
		goto err;
	}

	if(!MONITOR_PARSE(mtype, rlogin_databuffer, inifile, extraparam, szReturn))
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_16%>"));   //  <%IDS_TelMonitor_16%>
		bResult = FALSE;
		goto err;
	}

err:
	shutdown(hSocket, 0x02);
	closesocket(hSocket);
	return bResult;
}

BOOL RloginUnixPerfMonitorValue(char *server, int port, char *uid, char *pwd, 
						char *pserver, int pport, char *puid, char *ppwd, 
						char *pwdprompt, char *prompts, 
						char *inifile, char *custpath, char *szReturn)
{
	int		ret = 0;
	int		hSocket = 0;
	BOOL	bResult = TRUE;
	char	iniFile[255] = {0};
	char	cmd[1024] = {0};

	/*************************************
	 * For Reason Of Prompt
	 */
	int		c = 0;
	char	*ca = prompts, *cb = NULL;
	char	arprompt[PR_NUM][256];
	memset(arprompt, 0, sizeof(arprompt));

	while(cb = strchr(ca, 0x20))
	{
		if(ca == cb) 
		{
			ca ++;
			continue;
		}

		strncpy(arprompt[c++], ca, cb - ca);
		ca = cb + 1;
	}

	strcpy(arprompt[c++], ca);
	/***********************************/

	hSocket = rlogin_init(server, port, pserver, pport, puid, ppwd);
	if(hSocket <= 0)
	{
		if(hSocket == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_1%>"));    //  <%IDS_TelMonitor_1%>
		else if(hSocket == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_2%>"));   //  <%IDS_TelMonitor_2%>
		else if(hSocket == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_3%>"));     //  <%IDS_TelMonitor_3%>
		else if(hSocket == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_4%>"));     //  <%IDS_TelMonitor_4%>
		else if(hSocket == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_5%>"));        //  <%IDS_TelMonitor_5%>
		else 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_6%>"));     //  <%IDS_TelMonitor_6%>

		bResult = FALSE;
		goto err;
	}

	if((ret = rlogin_connect(hSocket, uid, pwd, pwdprompt, arprompt)) < 0)
	{
		if(ret == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_7%>"));    //  <%IDS_TelMonitor_7%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_8%>"));     //  <%IDS_TelMonitor_8%>
		else if(ret == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_9%>"));     //  <%IDS_TelMonitor_9%>
		else if(ret == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_10%>"));   //  <%IDS_TelMonitor_10%>
		else if(ret == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_11%>"));   //  <%IDS_TelMonitor_11%>
		else if(ret == -6)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_12%>"));   //  <%IDS_TelMonitor_12%>
		else 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_13%>"));     //  <%IDS_TelMonitor_13%>

		bResult = FALSE;
		goto err;

	}

	GetIniFileName(inifile, iniFile);
	
	if(!RETURN_COMMAND(0, iniFile, cmd))
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_14%>"));    //  <%IDS_TelMonitor_14%>
		bResult = FALSE;
		goto err;
	}

	if((ret = rlogin_command(hSocket, cmd)) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_15%>"));    //  <%IDS_TelMonitor_15%>
		bResult = FALSE;
		goto err;
	}

	if(!MONITOR_PARSE(0, rlogin_databuffer, inifile, NULL, szReturn))
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_16%>"));   //  <%IDS_TelMonitor_16%>
		bResult = FALSE;
		goto err;
	}

err:
	shutdown(hSocket, 0x02);
	closesocket(hSocket);
	return bResult;
}

