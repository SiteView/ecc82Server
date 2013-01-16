
#include "DHCP.h"
#include "Utils.h"

#include "..\..\base\funcGeneral.h"

BOOL DHCP_MONITOR(int timeout, char *re_addr, char *custpath, char *szReturn)
{
	int		i;
	SOCKET	s;
	struct	sockaddr_in lsa, rsa;
	DWORD	dwbTime, dweTime;

	int		reqlen = 0;
	u_char	reqmessage[1000] = {0};
	u_char	resmessage[1000] = {0};

	dwbTime = ::GetTickCount();

	DHCPMessage reqdm, resdm;

	srand((unsigned)time(NULL));

	memset(&reqdm, 0, sizeof(struct DHCP_MESSAGE));
	reqdm.op = 0x01;
	reqdm.htype = 0x01;
	reqdm.hlen = 0x06;

	// Physical Address
	GetMacAddress(reqdm.ch_addr);

	for(i = 0;i < 4;i ++)
		reqdm.xid[i] = rand() % 0xff;

	reqdm.opts.oplen = 0;

	// magic cookie
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x63;
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x82;
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x53;
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x63;

	// DHCP Message Type
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x35;
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x01;
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x01;

	reqdm.opts.opstr[reqdm.opts.oplen++] = 0xFB;
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x01;
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x01;

	// Client-identifier
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x3D;
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x07;
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x01;
	GetMacAddress(reqdm.opts.opstr + reqdm.opts.oplen);
	reqdm.opts.oplen += 6;

	// Requested IP Address
	if(*re_addr)
	{
		int		f_1 = 0, f_2 = 0, f_3 = 0, f_4 = 0;

		if(sscanf(re_addr, "%d.%d.%d.%d", &f_1, &f_2, &f_3, &f_4) != 4)
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_DHCP_1%>"));//<%IDS_DHCP_1%>
			return FALSE;
		}

		reqdm.opts.opstr[reqdm.opts.oplen++] = 0x32;
		reqdm.opts.opstr[reqdm.opts.oplen++] = 0x04;
		reqdm.opts.opstr[reqdm.opts.oplen++] = f_1;
		reqdm.opts.opstr[reqdm.opts.oplen++] = f_2;
		reqdm.opts.opstr[reqdm.opts.oplen++] = f_3;
		reqdm.opts.opstr[reqdm.opts.oplen++] = f_4;
	}

	// Host Name Option
	TCHAR lhost[80];
	gethostname(lhost, sizeof(lhost));
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x0C;
	reqdm.opts.opstr[reqdm.opts.oplen++] = strlen(lhost);
	for(i = 0;i < (int)strlen(lhost);i ++)
		reqdm.opts.opstr[reqdm.opts.oplen++] = lhost[i];

	// Class-identifier
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x3C;
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x08;
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x44;
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x46;
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x2E;
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x46;
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x72;
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x61;
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x6E;
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x6B;

	// Parameter Request List
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x37;
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x0A;
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x01;
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x0F;
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x03;
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x06;
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x2C;
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x2E;
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x2F;
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x1F;
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x21;
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0x2B;

	// end
	reqdm.opts.opstr[reqdm.opts.oplen++] = 0xFF;

	// padding
	if(reqdm.opts.oplen < 64)
	{
		int left = 64 - reqdm.opts.oplen;
		for(i = 0;i < left;i ++)
			reqdm.opts.opstr[reqdm.opts.oplen++] = 0x00;
	}

	reqlen = topacket(&reqdm, reqmessage);

	if((s = socket(PF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_DHCP_2%>"));//<%IDS_DHCP_2%>
		return FALSE;
	}

	// set socket broadcast flag
	{
	const int on = 1;
	setsockopt(s, SOL_SOCKET, SO_BROADCAST, (const char *)&on, sizeof(on));
	}

	memset(&lsa, 0, sizeof(lsa));
	lsa.sin_family = AF_INET;
	lsa.sin_addr.s_addr = htonl(inet_addr(DHCP_LOCAL_ADDRESS));
	lsa.sin_port=htons(DHCP_CLIENT_PORT);
	if(bind(s, (struct sockaddr *)&lsa, sizeof(lsa)) == SOCKET_ERROR)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_DHCP_3%>"));//<%IDS_DHCP_3%>
		return FALSE;
	}

    rsa.sin_family = AF_INET;
	rsa.sin_addr.s_addr = htonl(inet_addr(DHCP_MULTICAST_ADDRESS));
    rsa.sin_port = htons(DHCP_SERVER_PORT);

	int dw, n;
	if((n = sendto(s, (char *)reqmessage, reqlen, 0, (struct sockaddr *)&rsa, sizeof(rsa))) == SOCKET_ERROR)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_DHCP_4%>"));//<%IDS_DHCP_4%>
		return FALSE;
	}
	
	if(RESPONSE_WAIT(s, timeout) > 0)
	{
		dw = sizeof(rsa);
		if((n = recvfrom(s, (char *)resmessage, 1000, 0, (struct sockaddr *)&rsa, &dw)) == SOCKET_ERROR)
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_DHCP_5%>"));//<%IDS_DHCP_5%>
			return FALSE;
		}

		if(n == 0)
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_DHCP_6%>"));//<%IDS_DHCP_6%>
			return FALSE;
		}

		if(tostruct(&resdm, resmessage, n))
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_DHCP_7%>"));//<%IDS_DHCP_7%>
			return FALSE;
		}

		if(resdm.op != 2 || resdm.htype != 1 || resdm.hlen != 6)
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_DHCP_8%>"));//<%IDS_DHCP_8%>
			return FALSE;
		}

		if(!resdm.siaddr)
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_DHCP_9%>"));//<%IDS_DHCP_9%>
			return FALSE;
		}

		dweTime = ::GetTickCount();
		sprintf(szReturn, "roundTripTime=%ld", dweTime - dwbTime);
		return TRUE;

	}

	sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_DHCP_10%>"));//<%IDS_DHCP_10%>
	return FALSE;
}

int topacket(LPDHCPMessage pdm, u_char *packet)
{
	int i = 0;
	int packlen = 0;

	packet[packlen++] = pdm->op;
	packet[packlen++] = pdm->htype;
	packet[packlen++] = pdm->hlen;
	packet[packlen++] = pdm->hops;

	packet[packlen++] = pdm->xid[0];
	packet[packlen++] = pdm->xid[1];
	packet[packlen++] = pdm->xid[2];
	packet[packlen++] = pdm->xid[3];

	packet[packlen++] = pdm->secs[0];
	packet[packlen++] = pdm->secs[1];

	packet[packlen++] = pdm->flags[0];
	packet[packlen++] = pdm->flags[1];

	packet[packlen++] = pdm->ciaddr[0];
	packet[packlen++] = pdm->ciaddr[1];
	packet[packlen++] = pdm->ciaddr[2];
	packet[packlen++] = pdm->ciaddr[3];

	packet[packlen++] = pdm->yiaddr[0];
	packet[packlen++] = pdm->yiaddr[1];
	packet[packlen++] = pdm->yiaddr[2];
	packet[packlen++] = pdm->yiaddr[3];

	packet[packlen++] = pdm->siaddr[0];
	packet[packlen++] = pdm->siaddr[1];
	packet[packlen++] = pdm->siaddr[2];
	packet[packlen++] = pdm->siaddr[3];

	packet[packlen++] = pdm->giaddr[0];
	packet[packlen++] = pdm->giaddr[1];
	packet[packlen++] = pdm->giaddr[2];
	packet[packlen++] = pdm->giaddr[3];

	for(i = 0;i < 16;i ++)
		packet[packlen++] = pdm->ch_addr[i];

	for(i = 0;i < 64;i ++)
		packet[packlen++] = pdm->sname[i];

	for(i = 0;i < 128;i ++)
		packet[packlen++] = pdm->file[i];

	for(i = 0;i < pdm->opts.oplen;i ++)
		packet[packlen++] = pdm->opts.opstr[i];

	return packlen;
}

int tostruct(LPDHCPMessage pdm, u_char *packet, int packlen)
{
	int i = 0, index = 0;

	if(packlen < 236)
		return 1;

	pdm->op = packet[index++];
	pdm->htype = packet[index++];
	pdm->hlen = packet[index++];
	pdm->hops = packet[index++];

	pdm->xid[0] = packet[index++];
	pdm->xid[1] = packet[index++];
	pdm->xid[2] = packet[index++];
	pdm->xid[3] = packet[index++];

	pdm->secs[0] = packet[index++];
	pdm->secs[1] = packet[index++];

	pdm->flags[0] = packet[index++];
	pdm->flags[1] = packet[index++];

	pdm->ciaddr[0] = packet[index++];
	pdm->ciaddr[1] = packet[index++];
	pdm->ciaddr[2] = packet[index++];
	pdm->ciaddr[3] = packet[index++];

	pdm->yiaddr[0] = packet[index++];
	pdm->yiaddr[1] = packet[index++];
	pdm->yiaddr[2] = packet[index++];
	pdm->yiaddr[3] = packet[index++];

	pdm->siaddr[0] = packet[index++];
	pdm->siaddr[1] = packet[index++];
	pdm->siaddr[2] = packet[index++];
	pdm->siaddr[3] = packet[index++];

	pdm->giaddr[0] = packet[index++];
	pdm->giaddr[1] = packet[index++];
	pdm->giaddr[2] = packet[index++];
	pdm->giaddr[3] = packet[index++];

	for(i = 0;i < 16;i ++)
		pdm->ch_addr[i] = packet[index++];

	for(i = 0;i < 64;i ++)
		pdm->sname[i] = packet[index++];

	for(i = 0;i < 128;i ++)
		pdm->file[i] = packet[index++];

	if(index < packlen)
	{
		pdm->opts.oplen = packlen - index;
		for(i = 0;i < pdm->opts.oplen;i ++)
			pdm->opts.opstr[i] = packet[index++];
	}

	return 0;
}

BOOL GetMacAddress(u_char *mac)
{
	int	i = 0;
	lpUuidCreateSequential pUuidCreate;

	HINSTANCE hInstance = ::LoadLibrary("rpcrt4.dll");
	if (!hInstance) 
		goto err;

	pUuidCreate = (lpUuidCreateSequential)::GetProcAddress(hInstance, "UuidCreateSequential");
	if(!pUuidCreate)
		goto err;

    UUID uuid;
	pUuidCreate(&uuid);

    for (i = 2; i < 8; ++i) 
        mac[i-2] = int (uuid.Data4[i]);

	::FreeLibrary(hInstance);

	return TRUE;

err:
	for(i = 0;i < 6;i ++)
		mac[i] = 0x33;

	if(hInstance) ::FreeLibrary(hInstance);

	return FALSE;
}