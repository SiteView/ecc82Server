
#include "MQ.h"
#include "TelMonitor.h"
#include "Utils.h"

#include <iostream>

using namespace std;

#include "base\funcGeneral.h"

//extern char	databuffer[BUF_SIZE];
//extern int	datalen;



bool MQ_TEL_VALUE(char *server, int port, char *uid, char *pwd, char *pserver, int pport, 
				  char *puid, char *ppwd, char *lprom, char *pprom, char *prom, 
				  char *qm, char *qn, char *custpath, char *szReturn)
{
	int		ret = 0;
	int		hSocket = 0;
	bool	bResult = true;
	CString strBuffer = _T("");

	/*************************************
	 * For Reason Of Prompt
	 */
	int		c = 0;
	char	*ca = prom, *cb = NULL;
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

	LPGDATA pgd=new GDATA;
	memset(pgd,0,sizeof(GDATA));


	hSocket = telnet_init(server, port, pserver, pport, puid, ppwd);
	if(hSocket <= 0)
	{
		if(hSocket == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_1%>"));//<%IDS_MQ_1%>"连接失败：域名解析"
		else if(hSocket == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_2%>"));//<%IDS_MQ_2%>"连接失败：通信初始化"
		else if(hSocket == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_3%>"));//<%IDS_MQ_3%>"连接失败：连接超时"
		else if(hSocket == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_4%>"));//<%IDS_MQ_4%>"代理服务器连接失败"
		else if(hSocket == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_5%>"));//<%IDS_MQ_5%>"代理服务器错误"
		else 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_6%>"));//<%IDS_MQ_6%>"连接失败：未知错误"

		bResult = false;
		goto err;
	}

	if((ret = telnet_connect(hSocket, uid, pwd, lprom, pprom, arprompt,pgd)) < 0)
	{
		if(ret == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_7%>"));//<%IDS_MQ_7%>"登录失败：读数据超时"
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_8%>"));//<%IDS_MQ_8%>"登录失败：读数据错误"
		else if(ret == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_9%>"));//<%IDS_MQ_9%>"登录失败：查找提示符"
		else if(ret == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_10%>"));//<%IDS_MQ_10%>"登录失败：查找登录提示符"
		else if(ret == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_11%>"));//<%IDS_MQ_11%>"登录失败：查找密码提示符"
		else 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_12%>"));//<%IDS_MQ_12%>"登录失败：未知错误"

		bResult = false;
		goto err;
	}

    cout << "connect server succ!" << endl;

	if(MQ_GETDATA(hSocket, qm, qn, strBuffer))
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_13%>"));//<%IDS_MQ_13%>"获取数据失败"
		bResult = FALSE;
		goto err;
	}

    cout << strBuffer.GetBuffer(0) << endl;

	if(MQ_PARSE(strBuffer, szReturn))
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_14%>"));//<%IDS_MQ_14%>"数据格式错误: 可能由网速慢造成"
		bResult = false;
		goto err;
	}

err:
	shutdown(hSocket, 0x02);
	closesocket(hSocket);
	delete pgd;
	return bResult;
}

bool MQ_SSH_VALUE(char *server, int port, char *uid, char *pwd, char *qm, char *qn, 
				  char *custpath, char *szReturn)
{
	sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_15%>"));//<%IDS_MQ_15%>"未实现"
	return true;
}

bool HTTP_MQ_VALUE(char *url, char *uid, char *pwd, char *pserver, int pport, 
				   char *puid, char *ppwd, char *qm, char *qn, char *custpath, 
				   char *szReturn)
{
	sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_15%>"));//<%IDS_MQ_15%>"未实现"
	return true;
}

bool RLOGIN_MQ_VALUE(char *server, int port, char *uid, char *pwd, char *pserver, int pport, 
				     char *puid, char *ppwd, char *pprom, char *prom, 
				     char *qm, char *qn, char *custpath, char *szReturn)
{
	sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_15%>"));//<%IDS_MQ_15%>"未实现"
	return true;
}

int MQ_GETDATA(int hSocket, char *qm, char *qn, CString &strBuffer)
{
	OutputDebugString("------------MQ_GETDATA() call----------\n");
	int		n = 0;
	char	buffer[4096];
	char	*scan = NULL;

	memset(buffer, 0, sizeof(buffer));
	if(*qm) 
		sprintf(buffer, "runmqsc %s\r\n", qm);
	else
		sprintf(buffer, "runmqsc\r\n");

    cout <<  buffer << endl;

	send(hSocket, buffer, strlen(buffer), 0);

	LPGDATA pgd=new GDATA;
	memset(pgd,0,sizeof(GDATA));


	memset(pgd->databuffer, 0, BUF_SIZE);
	pgd->datalen = 0;
	while(RESPONSE_WAIT(hSocket, READ_TIMEOUT))
	{
		n = recv(hSocket, buffer, 4096, 0);
		if(n == SOCKET_ERROR) return 1;
		if(n == 0) return 1;
		buffer[n] = 0;

		scan = buffer;
		while(n--) telnet_protocol(hSocket, *scan++,pgd);

		if(strstr(pgd->databuffer, "Starting MQSeries Commands")) break;
       // if(strstr(pgd->databuffer, "Starting MQSC for queue manager SIPDS")) break;
		if(strstr(pgd->databuffer, "Starting MQSC for queue manager")) break;
        if(strstr(pgd->databuffer, "启动队列管理器")) break;
	}

	Sleep(100);

    if(strstr(pgd->databuffer, "AMQ"))
        return 1;

	memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "DISPLAY QUEUE(%s) MAXDEPTH CURDEPTH\r\n", qn);
	//sprintf(buffer, "dis ql('%s')\r\n", qn);

    cout << buffer << endl;

	send(hSocket, buffer, strlen(buffer), 0);

	memset(pgd->databuffer, 0, BUF_SIZE);
	memset(pgd,0,sizeof(GDATA));

    memset(buffer, 0, sizeof(buffer));
	pgd->datalen = 0;
	while(RESPONSE_WAIT(hSocket, MQ_TIMEOUT))
	{
		n = recv(hSocket, buffer, 4096, 0);

		if(n == SOCKET_ERROR)
            return 1;

		if(n == 0)
            return 1;

		buffer[n] = 0;

		scan = buffer;
		while(n--) telnet_protocol(hSocket, *scan++,pgd);

		if(strstr(pgd->databuffer, "CURDEPTH(")) break;
	}
	
	strBuffer =pgd->databuffer;
	delete pgd;
	return 0;
}

int MQ_PARSE(CString strBuffer, char *szReturn)
{
	OutputDebugString("--------MQ_PARSE() call---------------\n");
	int index = 0;

    printf ("%s\n", strBuffer);

    //szMaximumDepth
	index = strBuffer.Find("CURDEPTH(", 0);
	if(index < 0) 
		return 1;

	CString szCurDepth = strBuffer.Right(strBuffer.GetLength() - index - 9);

    index = strBuffer.Find("MAXDEPTH(", 0);
    if(index < 0)
        return 1;

    CString szMaxDepth = strBuffer.Right(strBuffer.GetLength() - index - 9);

	if(szCurDepth.IsEmpty() || !isdigit(szCurDepth.GetAt(0)) || 
        szMaxDepth.IsEmpty() || !isdigit(szMaxDepth.GetAt(0)))
		return 1;	
    
	sprintf(szReturn, "szCurrentDepth=%ld$szMaximumDepth=%ld", atol(szCurDepth), atol(szMaxDepth));
	
	OutputDebugString("-----------------mq depth return value------------\n");
	OutputDebugString(szReturn);
	OutputDebugString("\n");

	return 0;
}

bool MQ_TEL_STATUS_VALUE(char *server, int port, char *uid, char *pwd, char *pserver, int pport, 
				  char *puid, char *ppwd, char *lprom, char *pprom, char *prom, 
				  char *qm, char *qn, char *cn, char *custpath, char *szReturn)
{
	OutputDebugString("-----------MQ_TEL_STATUS_VALUE() call--------\n");
	int		ret = 0;
	int		hSocket = 0;
	bool	bResult = true;
	CString strBuffer = _T("");

	/*************************************
	 * For Reason Of Prompt
	 */
	int		c = 0;
	char	*ca = prom, *cb = NULL;
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

	LPGDATA pgd=new GDATA;
	memset(pgd,0,sizeof(GDATA));


	hSocket = telnet_init(server, port, pserver, pport, puid, ppwd);
	if(hSocket <= 0)
	{
		if(hSocket == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_1%>"));//<%IDS_MQ_1%>"连接失败：域名解析"
		else if(hSocket == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_2%>"));//<%IDS_MQ_2%>"连接失败：通信初始化"
		else if(hSocket == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_3%>"));//<%IDS_MQ_3%>"连接失败：连接超时"
		else if(hSocket == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_4%>"));//<%IDS_MQ_4%>"代理服务器连接失败"
		else if(hSocket == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_5%>"));//<%IDS_MQ_5%>"代理服务器错误"
		else 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_6%>"));//<%IDS_MQ_6%>"连接失败：未知错误"

		bResult = false;
		goto err;
	}

	if((ret = telnet_connect(hSocket, uid, pwd, lprom, pprom, arprompt,pgd)) < 0)
	{
		if(ret == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_7%>"));//<%IDS_MQ_7%>"登录失败：读数据超时"
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_8%>"));//<%IDS_MQ_8%>"登录失败：读数据错误"
		else if(ret == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_9%>"));//<%IDS_MQ_9%>"登录失败：查找提示符"
		else if(ret == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_10%>"));//<%IDS_MQ_10%>"登录失败：查找登录提示符"
		else if(ret == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_11%>"));//<%IDS_MQ_11%>"登录失败：查找密码提示符"
		else 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_12%>"));//<%IDS_MQ_12%>"登录失败：未知错误"

		bResult = false;
		goto err;
	}

    cout << "connect server succ!" << endl;

	if(MQ_STATUS_GETDATA(hSocket, qm, qn, cn, strBuffer))
	{
		OutputDebugString("----------MQ_STATUS_GETDATA() call failure--------\n");
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_13%>"));//<%IDS_MQ_13%>"获取数据失败"
		bResult = FALSE;
		goto err;
	}

    cout << strBuffer.GetBuffer(0) << endl;
	
	/*
	if(MQ_STATUS_PARSE(strBuffer, szReturn))
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_14%>"));//<%IDS_MQ_14%>"数据格式错误: 可能由网速慢造成"
		bResult = false;
		goto err;
	}
	*/
	OutputDebugString("----------start output MQ_STATUS_GETDATA() receive string-------\n");
	OutputDebugString(strBuffer);

	if(strstr(strBuffer, "STARTING") )
	{
		sprintf(szReturn, "szStatus=%s", "开始");
	}
	else if(strstr(strBuffer, "RUNNING") )
	{
		sprintf(szReturn, "szStatus=%s", "运行");
	}
	else if(strstr(strBuffer, "STOPPED") )
	{
		sprintf(szReturn, "szStatus=%s", "停止");
	}
	else
	{
		sprintf(szReturn, "szStatus=%s", "通道状态未找到");
	}

err:
	shutdown(hSocket, 0x02);
	closesocket(hSocket);
	delete pgd;
	return bResult;
}


int MQ_STATUS_GETDATA(int hSocket, char *qm, char *qn, char *cn, CString &strBuffer)
{
	OutputDebugString("---------MQ_STATUS_GETDATA() call start---------\n");
	int		n = 0;
	char	buffer[4096];
	char	*scan = NULL;

	memset(buffer, 0, sizeof(buffer));
	if(*qm) 
		sprintf(buffer, "runmqsc %s\r\n", qm);
	else
		sprintf(buffer, "runmqsc\r\n");

    cout <<  buffer << endl;

	send(hSocket, buffer, strlen(buffer), 0);

	LPGDATA pgd=new GDATA;
	memset(pgd,0,sizeof(GDATA));


	memset(pgd->databuffer, 0, BUF_SIZE);
	pgd->datalen = 0;
	OutputDebugString("-------mq_status_getdata() start receive--------\n");
	while(RESPONSE_WAIT(hSocket, READ_TIMEOUT))
	//if(RESPONSE_WAIT(hSocket, READ_TIMEOUT))
	{
		n = recv(hSocket, buffer, 4096, 0);		

		if(n == SOCKET_ERROR) return 1;
		if(n == 0) return 1;
		buffer[n] = 0;

		scan = buffer;
		while(n--) telnet_protocol(hSocket, *scan++,pgd);

		OutputDebugString("---------mq_status_getdata() receive---------\n");
		OutputDebugString(pgd->databuffer);
		OutputDebugString("\n");

		if(strstr(pgd->databuffer, "Starting MQSeries Commands")) break;
        if(strstr(pgd->databuffer, "Starting MQSC for queue manager")) break;
        if(strstr(pgd->databuffer, "启动队列管理器")) break;
		
	}
	
	OutputDebugString("-----------Start Display CHSTATUS---------\n");
	Sleep(100);

    if(strstr(pgd->databuffer, "AMQ"))
	{
		OutputDebugString("--------MQ_STATUS_GETDATA receive data is error--------\n");
		OutputDebugString(pgd->databuffer);
		OutputDebugString("\n");
		return 1;
	}

	memset(buffer, 0, sizeof(buffer));
   // sprintf(buffer, "DISPLAY QUEUE(%s) MAXDEPTH CURDEPTH\r\n", qn);
	sprintf(buffer, "DISPLAY CHSTATUS (%s) \r\n", cn);
	//sprintf(buffer, "dis ql('%s')\r\n", qn);

    cout << buffer << endl;

	send(hSocket, buffer, strlen(buffer), 0);

	memset(pgd->databuffer, 0, BUF_SIZE);
	memset(pgd,0,sizeof(GDATA));

    memset(buffer, 0, sizeof(buffer));
	pgd->datalen = 0;
	OutputDebugString("----------mq_status_getdata() start second receive-------\n");
	while(RESPONSE_WAIT(hSocket, MQ_TIMEOUT))
	//if(RESPONSE_WAIT(hSocket, MQ_TIMEOUT))
	{
		n = recv(hSocket, buffer, 4096, 0);

		OutputDebugString("--------mq_status_getdata() second receive-----------\n");
		OutputDebugString(buffer);
		OutputDebugString("\n");

		if(n == SOCKET_ERROR)
            return 1;

		if(n == 0)
            return 1;

		buffer[n] = 0;

		scan = buffer;
		while(n--) telnet_protocol(hSocket, *scan++,pgd);

		if(strstr(pgd->databuffer, "STATUS(")) break;
	}

	strBuffer =pgd->databuffer;
	delete pgd;
	return 0;
}

int MQ_STATUS_PARSE(CString strBuffer, char *szReturn)
{
	int index = 0;

    printf ("%s\n", strBuffer);

    //szMaximumDepth
	index = strBuffer.Find("CURDEPTH(", 0);
	if(index < 0) 
		return 1;

	CString szCurDepth = strBuffer.Right(strBuffer.GetLength() - index - 9);

    index = strBuffer.Find("MAXDEPTH(", 0);
    if(index < 0)
        return 1;

    CString szMaxDepth = strBuffer.Right(strBuffer.GetLength() - index - 9);

	if(szCurDepth.IsEmpty() || !isdigit(szCurDepth.GetAt(0)) || 
        szMaxDepth.IsEmpty() || !isdigit(szMaxDepth.GetAt(0)))
		return 1;	
    

	sprintf(szReturn, "szCurrentDepth=%ld$szMaximumDepth=%ld", atol(szCurDepth), atol(szMaxDepth));
	return 0;
}

