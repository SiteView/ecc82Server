// CtripURLMonitor.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "CtripURLMonitor.h"
#include <fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//	注意！
//
//		如果此 DLL 动态链接到 MFC
//		DLL，从此 DLL 导出并
//		调入 MFC 的任何函数在函数的最前面
//		都必须添加 AFX_MANAGE_STATE 宏。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CCtripURLMonitorApp

BEGIN_MESSAGE_MAP(CCtripURLMonitorApp, CWinApp)
END_MESSAGE_MAP()


// CCtripURLMonitorApp 构造

CCtripURLMonitorApp::CCtripURLMonitorApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CCtripURLMonitorApp 对象

CCtripURLMonitorApp theApp;


// CCtripURLMonitorApp 初始化

BOOL CCtripURLMonitorApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	return TRUE;
}

int PrintLog(const char * strPrint , const char * FileName)
{
#ifndef __DEBUG
	return 0;
#endif
	char timebuf[128]={0},datebuf[128]={0}, tempbuf[1024]={0};
	_strdate(datebuf);
	_strtime(timebuf);
	sprintf(tempbuf, "%s-%s", datebuf, timebuf);
	ofstream oFile;
	oFile.open(FileName,ios::app);
	oFile<<tempbuf<<"\t"<<strPrint<<endl;
	oFile.close();
	return 1;
}

bool MakeStringListByChar(CStringList &strList , const char * strParas)
{
	try
	{
		const char * strBuffer = strParas;
		while(*strBuffer)
		{
			strList.AddTail(strBuffer);
			strBuffer+=strlen(strBuffer)+1;
		}
	}
	catch(...)
	{
		char tempbuf[1024]={0};
		sprintf(tempbuf,"error number is %d!",::GetLastError());
		PrintLog(tempbuf,"CtripSwitchChar.log");
		return false;
	}
	return true;

}

bool MakeCharByString(char * szReturn , CString strInput , int &nSize)
{
	char * tempbuf;
	if(strInput.GetLength()+2<nSize)
	{
		strcpy(szReturn, strInput.GetBuffer(strInput.GetLength()));
	}
	else
	{
		return false;
	}	
	tempbuf = szReturn;
    while(*tempbuf)
	{
		if(*tempbuf=='$')
		{
			*tempbuf='\0';
		}
		tempbuf++;
	}
	nSize = strInput.GetLength() + 1;
	return true;
}

char * FindStrValue(const char * strSource, const char * strObj)
{
	CStringList strList;
	MakeStringListByChar(strList, strSource);
	char tempbuf[1024]={0};
	sprintf(tempbuf, "%s",strObj);
	CString strValue = _T("");

	POSITION pos = strList.GetHeadPosition();
	while(pos)
	{
		CString strTemp = strList.GetNext(pos);
		if(strTemp.Find(tempbuf)==0)
		{
			strValue = strTemp.Right(strTemp.GetLength() - static_cast<int>(strlen(tempbuf))-1);
		}
	}
	return strValue.GetBuffer(strValue.GetLength());
}

extern "C" __declspec(dllexport)
BOOL URLMonitor(const char * strParas, char * szReturn , int &nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;
	CString strURL = _T("");
	CString strHostName = _T("");
	CString strIP  = _T("");
	int BeginTime=0,EndTime=0;		//获取页面开始和结束时间。
	int nDownloadsize=0;			//下载的网页大小
	int nStatusCode = 0;
	int nPort = 80;
	int nStatus = 0;

	CStringList strWebContent;

	CString strMonitorID = _T("");

	strURL = FindStrValue(strParas , "_url");
	strHostName = FindStrValue(strParas, "_hostname");
	strIP  = FindStrValue(strParas, "_IP");
	nPort = atoi(FindStrValue(strParas, "_Port"));
	strMonitorID = FindStrValue(strParas, "_MonitorID");

	char strTemp[1024]={0};
	sprintf(strTemp,"CtripURLMonitor%s.log",strMonitorID);


	try
	{
		WORD wVersionRequested;
		WSADATA wsaData;
		int err;
		BeginTime = ::GetTickCount();
		wVersionRequested = MAKEWORD( 1, 1 );
		err = WSAStartup( wVersionRequested, &wsaData );
		if ( err != 0 ) 
		{
			sprintf(szReturn, "error=%s", "Socket initialize error!");
			return FALSE;
		}
//检查Socket库的版本是否为1.0
		if ( LOBYTE( wsaData.wVersion ) != 1 ||
			HIBYTE( wsaData.wVersion ) != 1 ) {
				WSACleanup( );
				return FALSE;
			}
//创建Socket
			SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0);

			SOCKADDR_IN addrSrv;
			addrSrv.sin_addr.S_un.S_addr = inet_addr(strIP.GetBuffer(strIP.GetLength()));
			addrSrv.sin_family=AF_INET;
			addrSrv.sin_port = htons(nPort);
			nStatus = connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

			if(nStatus==0)
			{
				PrintLog("Socket establis","CtripMonitor.log");
			}
			else
			{
				sprintf(szReturn,"error=%s","Socket error! Cann't connect to specified IP Server!");
				return false;
			}
//发送HTTP数据包
			CString strSendBuff = _T("");
			strSendBuff.Format("GET %s HTTP/1.1\r\nAccept: */*\r\nAccept-Language: zh-cn\r\nAccept-Encoding: gzip, deflate\r\nHost: %s\r\nConnection: Close\r\n\r\n",strURL,strHostName);
//			strSendBuff.Format("GET %s HTTP/1.1\r\nAccept: */*\r\nAccept-Language: zh-cn\r\nAccept-Encoding: gzip, deflate\r\nHost: %s\r\nConnection: Close\r\n\r\n","/Domestic/SearchHotel.aspx","hotels.ctrip.com");
			send(sockClient, strSendBuff.GetBuffer(strSendBuff.GetLength()), (int)strSendBuff.GetLength()+1, 0);
//收取HTTP数据包
			char recvBuf[1024*200+1]={0};
			int temp=0;
			CString strTemp = _T("");
			char tempbuf[1024]={0};
			temp=recv(sockClient,recvBuf,1024*200,0);
			strTemp.Format("%s",recvBuf);
			if((strTemp.Find("HTTP/1.1")!=-1)||strTemp.Find("HTTP/1.0")!=-1)
			{
				nStatusCode = 201;
				for(int i=9;i<12;i++)
				{
					tempbuf[i-9]=recvBuf[i];
				}
				nStatusCode = atoi(tempbuf);
				sprintf(tempbuf,"%s","");		//清空tempbuf
/*
				int SizePosition = strTemp.Find(__FILESIZE__)+strlen(__FILESIZE__);
				for(int i =SizePosition;i<SizePosition+6;i++)
				{
					if(recvBuf[i]=='\r')
						break;
					tempbuf[i-SizePosition] = recvBuf[i];
				}
				PrintLog(tempbuf,"CtripMonitor.log");
*/

			}
			else
			{
				sprintf(szReturn,"error=%s","Cann't find return Code!");
				return false;
			}

			nDownloadsize+=temp;
			while(temp != SOCKET_ERROR&& temp>0)
			{				
				temp = recv(sockClient,recvBuf,1024*200,0);
				nDownloadsize+=temp;
			}	


//断开socket链接
//			printf("%s\n",recvBuf);
			closesocket(sockClient);
			WSACleanup();
			EndTime = ::GetTickCount();
			bRet = true;
	}
	catch(...)
	{
		int nErrNum = ::GetLastError();
		sprintf(szReturn,"error=error occured! error number is %d",nErrNum);
		return false;
	}
	sprintf(szReturn,"status=%d$totaltime=%0.2f$downloadsize=%d$",nStatusCode,(EndTime-BeginTime)/1000.0,nDownloadsize);
	CString strInput;
	strInput = szReturn;
	MakeCharByString(szReturn,strInput,nSize);

	return bRet;

}