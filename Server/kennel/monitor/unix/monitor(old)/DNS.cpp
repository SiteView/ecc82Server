
#include "DNS.h"
#include "Utils.h"

#include "..\..\base\funcGeneral.h"

BOOL DNS_MONITOR(char *server, int port, char *domain, CStringList &lstAddress, 
				 char *custpath, char *szReturn)
{
	u_char reply[1000];
	SOCKET udpSocket;
    struct sockaddr_in serv;
	unsigned int n;
	int dw, ret, Count = 0, iCount = 0;
	LONG	dwBeginTime, dwElapseTime;

	u_char request[1000] =
				{ 
				0x01, 0x01, 0x01, 0x00,
				0x00, 0x01, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00
				};
	Count = 12;

	for(n = 0;n < strlen(domain);n ++)
	{
		if(domain[n] != '.')
		{
			iCount ++;
			request[Count + iCount] = domain[n];
		}
		else
		{
			request[Count] = iCount;
			Count = Count + iCount + 1;
			iCount = 0;
		}
	}
	request[Count] = iCount;
	Count = Count + iCount + 1;
	request[Count] = 0x00;
	request[Count + 1] = 0x00;
	request[Count + 2] = 0x01;
	request[Count + 3] = 0x00;
	request[Count + 4] = 0x01;
	Count += 5;

	udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if(udpSocket == INVALID_SOCKET)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_DNS_1%>"));//<%IDS_DNS_1%>
		return FALSE;
	}

	struct hostent *hp;
	hp = gethostbyname(server);
	if(!hp)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_DNS_2%>"));//<%IDS_DNS_2%>
		return FALSE;
	}

	memcpy(&serv.sin_addr,hp->h_addr_list[0],hp->h_length);
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
	
	dwBeginTime = GetTickCount();
    n = sendto(udpSocket, (char *)request, Count, 0, (struct sockaddr *)&serv, sizeof(serv));
	if(n == SOCKET_ERROR)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_DNS_3%>"));//<%IDS_DNS_3%>
		return FALSE;
	}
	
	ret = RESPONSE_WAIT(udpSocket, DNS_TIMEOUT);
	if(ret > 0)
	{
		dw = sizeof(serv);
		if((n = recvfrom(udpSocket, (char *)reply, 1000, 0, (struct sockaddr *)&serv, &dw)) < 0)
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_DNS_4%>"));//<%IDS_DNS_4%>
			return FALSE;
		}

		switch(reply[3] & 0x0F)
		{
		case 0:
			break;
		case 1:
			// Malformed DNS Request Packet
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_DNS_5%>"));//<%IDS_DNS_5%>
			return FALSE;
		case 2:
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_DNS_6%>"));//<%IDS_DNS_6%>
			return FALSE;
		case 3:
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_DNS_7%>"));//<%IDS_DNS_7%>
			return FALSE;
		}

		if(reply[0] == request[0] && reply[1] == request[1])
		{
			if(reply[6] * 256 + reply[7] == 0)
			{
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_DNS_8%>"));//<%IDS_DNS_8%>
				return FALSE;
			}

			//DNSMessageParser(reply, n);

			if(lstAddress.GetCount() != 0)
			{
				ret = VerifyDNS(reply, n, lstAddress);
				if(ret == 0)
				{
					dwElapseTime = GetTickCount() - dwBeginTime;
					sprintf(szReturn, "roundTripTime=%ld$", dwElapseTime);
					return TRUE;
				}
				else if(ret == -1)
				{
					sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_DNS_9%>"));//<%IDS_DNS_9%>
					return FALSE;
				}
				else if(ret == -2)
				{
					sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_DNS_11%>"));	//Verify Failed,<%IDS_DNS_11%> 
					return FALSE;
				}
				else
				{
					sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_DNS_12%>"));//<%IDS_DNS_12%>
					return FALSE;
				}
			}
			else
			{
				dwElapseTime = GetTickCount() - dwBeginTime;
				sprintf(szReturn, "roundTripTime=%ld$", dwElapseTime);
			}
		}
		else
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_DNS_13%>"));//<%IDS_DNS_13%>
			return FALSE;
		}
	}
	else
	{
		if(ret == 0)
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_DNS_14%>"));	//<%IDS_DNS_14%>
			return FALSE;
		}
		else
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_DNS_15%>"));//<%IDS_DNS_15%>
			return FALSE;
		}
	}

	return TRUE;
}

void DNSMessageParser(u_char *buff, int len)
{
	int i = 0;
	int flen = 0;
	u_char *ptr = buff;

	for(i = 0;i < len;i ++)
	{
		if( i % 16 == 0) printf("\r\n");
		printf("%2.2x ", buff[i]);
	}
	printf("\r\n\r\n");

	// Identification
	printf("Identification: %2.2x%2.2x\r\n", *ptr, *(ptr + 1));
	ptr += 2, flen += 2;

	// Parameter
	printf("Parameter: %2.2x%2.2x\r\n", *ptr, *(ptr + 1));
	ptr += 2, flen += 2;

	// Number Of Questions
	printf("Question Number: %d\r\n", *ptr * 256 + *(ptr + 1));
	ptr += 2, flen += 2;

	// Number Of Answers
	printf("Answer Number: %d\r\n", *ptr * 256 + *(ptr + 1));
	ptr += 2, flen += 2;

	// Number Of Authority
	printf("Authority Number: %d\r\n", *ptr * 256 + *(ptr + 1));
	ptr += 2, flen += 2;

	// Number Of Additional
	printf("Additional Number: %d\r\n\r\n", *ptr * 256 + *(ptr + 1));
	ptr += 2, flen += 2;

	// Question Section
	printf("Question Section: \r\n");
	printf("	Query Domain Name: ");
	while(*ptr && ((*ptr & 0xC0) != 0xC0))
	{
		for(i = 0;i < *ptr;i ++)
			printf("%c", *(ptr + 1 + i));

		flen += 1 + *ptr, ptr += 1 + *ptr;
		if(*ptr && ((*ptr & 0xC0) != 0xC0))
			printf("%c", '.');
	}
	printf("\r\n");
	ptr ++, flen ++;

	printf("	Query Type: %d\r\n", *ptr * 256 + *(ptr + 1));
	ptr += 2, flen += 2;

	printf("	Query Class: %d\r\n\r\n", *ptr * 256 + *(ptr + 1));
	ptr += 2, flen += 2;

	// Answer Section
	printf("Answer Section: \r\n");
	printf("	Resource Domain name: ");
	if((*ptr & 0xC0) == 0xC0)
	{
		flen += (*ptr & 0x3F) * 0xFF + *(ptr + 1);
		ptr += (*ptr & 0x3F) * 0xFF + *(ptr + 1);
		while(*ptr && ((*ptr & 0xC0) != 0xC0))
		{
			for(i = 0;i < *ptr;i ++)
				printf("%c", *(ptr + 1 + i));

			flen += 1 + *ptr, ptr += 1 + *ptr;
			if(*ptr && ((*ptr & 0xC0) != 0xC0))
				printf("%c", '.');
		}
	}
	else
	{
		while(*ptr && ((*ptr & 0xC0) != 0xC0))
		{
			for(i = 0;i < *ptr;i ++)
				printf("%c", *(ptr + 1 + i));

			flen += 1 + *ptr, ptr += 1 + *ptr;
			if(*ptr && ((*ptr & 0xC0) != 0xC0))
				printf("%c", '.');
		}
	}
	printf("\r\n\r\n");

	if(*ptr == 0xC0) ptr += 2, flen += 2;
	if(!*ptr) ptr ++, flen ++;

	u_char st_1 = *ptr;
	u_char st_2 = *(ptr + 1);
	while(*ptr == st_1 && *(ptr + 1) == st_2)
	{
		ptr += 2, flen += 2;

		printf("	Resource Type: %d\r\n", *ptr * 256 + *(ptr + 1));
		ptr += 2, flen += 2;

		printf("	Resource Class: %d\r\n", *ptr * 256 + *(ptr + 1));
		ptr += 2, flen += 2;

		printf("	Resource Time To Live: %ld\r\n", *ptr * 256 * 256 * 256 + *(ptr + 1) * 256 * 256 + *(ptr + 2) * 256 + *(ptr + 3));
		ptr += 4, flen += 4;

		printf("	Resource Data Length: %d\r\n", *ptr * 256 + *(ptr + 1));
		ptr += 2, flen += 2;

		printf("	Resource Data: %d.%d.%d.%d\r\n\r\n", *ptr, *(ptr + 1), *(ptr + 2), *(ptr + 3));
		ptr += 4, flen += 4;
	}

	// Authority Section
	printf("Authority Section: \r\n");

	st_1 = *ptr;
	st_2 = *(ptr + 1);
	while(*ptr == st_1 && *(ptr + 1) == st_2)
	{
		ptr += 2, flen += 2;

		int type = *ptr * 256 + *(ptr + 1);
		printf("	Resource Type: %d\r\n", type);
		ptr += 2, flen += 2;

		printf("	Resource Class: %d\r\n", *ptr * 256 + *(ptr + 1));
		ptr += 2, flen += 2;

		printf("	Resource Time To Live: %ld\r\n", *ptr * 256 * 256 * 256 + *(ptr + 1) * 256 * 256 + *(ptr + 2) * 256 + *(ptr + 3));
		ptr += 4, flen += 4;

		int rlen = *ptr * 256 + *(ptr + 1);
		printf("	Resource Data Length: %d\r\n", rlen);
		ptr += 2, flen += 2;

		printf("	Resource Data: ");
		switch(type)
		{
		case 1:
			printf("%d.%d.%d.%d", *ptr, *(ptr + 1), *(ptr + 2), *(ptr + 3));
			ptr += rlen, flen += rlen;
			break;
		case 2:
			while(*ptr && ((*ptr & 0xC0) != 0xC0))
			{
				for(i = 0;i < *ptr;i ++)
					printf("%c", *(ptr + 1 + i));

				flen += 1 + *ptr, ptr += 1 + *ptr;
				if(*ptr && ((*ptr & 0xC0) != 0xC0))
					printf("%c", '.');
			}

			if(*ptr == 0xC0) ptr += 2, flen += 2;
			if(!*ptr) ptr ++, flen ++;
			break;
		default:
			for(i = 0;i < rlen;i ++)
				printf("%c", *(ptr + i));
			ptr += rlen, flen += rlen;
			break;
		}

		printf("\r\n\r\n");
		
	}

	// Additional Informatoin Section
	printf("Additional Information Section: \r\n");

	while(flen < len)
	{
		ptr += 2, flen += 2;

		int type = *ptr * 256 + *(ptr + 1);
		printf("	Resource Type: %d\r\n", type);
		ptr += 2, flen += 2;

		printf("	Resource Class: %d\r\n", *ptr * 256 + *(ptr + 1));
		ptr += 2, flen += 2;

		printf("	Resource Time To Live: %ld\r\n", *ptr * 256 * 256 * 256 + *(ptr + 1) * 256 * 256 + *(ptr + 2) * 256 + *(ptr + 3));
		ptr += 4, flen += 4;

		int rlen = *ptr * 256 + *(ptr + 1);
		printf("	Resource Data Length: %d\r\n", rlen);
		ptr += 2, flen += 2;

		printf("	Resource Data: ");
		switch(type)
		{
		case 1:
			printf("%d.%d.%d.%d", *ptr, *(ptr + 1), *(ptr + 2), *(ptr + 3));
			ptr += rlen, flen += rlen;
			break;
		case 2:
			while(*ptr && ((*ptr & 0xC0) != 0xC0))
			{
				for(i = 0;i < *ptr;i ++)
					printf("%c", *(ptr + 1 + i));

				flen += 1 + *ptr, ptr += 1 + *ptr;
				if(*ptr && ((*ptr & 0xC0) != 0xC0))
					printf("%c", '.');
			}

			if(*ptr == 0xC0) ptr += 2, flen += 2;
			if(!*ptr) ptr ++, flen ++;
			break;
		default:
			for(i = 0;i < rlen;i ++)
				printf("%c", *(ptr + i));
			ptr += rlen, flen += rlen;
			break;
		}

		printf("\r\n\r\n");
	}
}

int VerifyDNS(u_char *result, int len, CStringList &lstAddress)
{
	int	addr_1 = 0,
		addr_2 = 0,
		addr_3 = 0,
		addr_4 = 0;
	int ret = 0;

	POSITION pos = lstAddress.FindIndex(0);
	while(pos)
	{
		CString strAddress = lstAddress.GetNext(pos);
		for(int i = 0;i < strAddress.GetLength();i ++)
		{
			TCHAR ch = strAddress.GetAt(i);
			if(ch != '.' && !isdigit(ch))
				return -1;
		}

		ret = sscanf(strAddress, "%d.%d.%d.%d", &addr_1, &addr_2, &addr_3, &addr_4);
		if(ret != 4)
		{
			return -1;
		}

		for(i = 0;i <= len - 4;i ++)
		{
			if(	result[i] == addr_1 && result[i + 1] == addr_2 &&
				result[i + 2] == addr_3 && result[i + 3] == addr_4)
				break;
		}

		if(i == len - 3) return -2;
	}

	return 0;
}

