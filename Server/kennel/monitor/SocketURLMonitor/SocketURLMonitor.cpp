// SocketURLMonitor.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "SocketURLMonitor.h"
#include <fstream>
#include <iostream>
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

// CSocketURLMonitorApp

BEGIN_MESSAGE_MAP(CSocketURLMonitorApp, CWinApp)
END_MESSAGE_MAP()


// CSocketURLMonitorApp 构造

CSocketURLMonitorApp::CSocketURLMonitorApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CSocketURLMonitorApp 对象

CSocketURLMonitorApp theApp;


// CSocketURLMonitorApp 初始化

BOOL CSocketURLMonitorApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	return TRUE;
}
bool PrintLog(const char * strParas)
{
#ifndef __DEBUG
	return false;
#endif
	
		char datebuf[128]={0},timebuf[128]={0},tempbuf[10240]={0};
		_strtime(timebuf);
		_strdate(datebuf);
		sprintf(tempbuf,"%s-%s",datebuf,timebuf);
		ofstream FileStream;
		FileStream.open("SocketURLMonitor.log",ios::app);
		FileStream<<tempbuf<<"\t"<<strParas<<endl;
		FileStream.close();
	
	return true;
}
bool MakeStringListByChar(CStringList &strList , const char * strParas)
{
	const char * p;
	p = strParas;
	int nSize;
	while(*p!='\0')
	{
		nSize = static_cast<int>(strlen(p));
		if(nSize>0)
		{
			strList.AddTail(p);
		}
		p+=strlen(p)+1;
	}
	return true;
}
//将输入字符串strInput中的内容保存到strOut中，并且将其中的$用\0替换，如果strInput的长度+2达到nSize，则转化失败
bool MakeCharByString(char * strOut , CString strInput, int &nSize)
{
	char * p;
	if(strInput.GetLength()+2<nSize)
	{
		strcpy(strOut,strInput.GetBuffer(strInput.GetLength()));
	}
	else
	{
		return false;
	}
	p = strOut;
	while(*p)
	{
		if(*p=='$')
			*p='\0';
		p++;
	}
	nSize = strInput.GetLength()+1;
	return true;

}
extern "C" __declspec(dllexport)
BOOL URLMonitor(const char * strParas, char * szReturn, int &nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CStringList strList;
	CString strURL = _T("");
	CString strHostName = _T("");
	CString strIP  = _T("");
	int BeginTime=0,EndTime=0;		//获取页面开始和结束时间。
	int nDownloadsize=0;			//下载的网页大小
	int nStatusCode = 0;
	int nPort = 80;
	MakeStringListByChar(strList , strParas);

	int nStatus = 0;

	POSITION pos;
	pos = strList.GetHeadPosition();
	while(pos)
	{
		CString strTemp = strList.GetNext(pos);
		if(strTemp.Find(__URL__)==0)
		{
			strURL = strTemp.Right(strTemp.GetLength() - static_cast<int>(strlen(__URL__)));
		}
		else if(strTemp.Find(__IP__)==0)
		{
			strIP = strTemp.Right(strTemp.GetLength() - static_cast<int>(strlen(__IP__)));
		}
		else if(strTemp.Find(__HOSTNAME__)==0)
		{
			strHostName = strTemp.Right(strTemp.GetLength() - static_cast<int>(strlen(__HOSTNAME__)));
		}
		else if(strTemp.Find(__PORT__)==0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - static_cast<int>(strlen(__PORT__))).GetBuffer(1));
		}
	}
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

		if ( LOBYTE( wsaData.wVersion ) != 1 ||
			HIBYTE( wsaData.wVersion ) != 1 ) {
				WSACleanup( );
				return FALSE;
			}
			SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0);

			SOCKADDR_IN addrSrv;
			addrSrv.sin_addr.S_un.S_addr = inet_addr(strIP.GetBuffer(strIP.GetLength()));
			addrSrv.sin_family=AF_INET;
//			addrSrv.sin_port = htons(80);
			addrSrv.sin_port = htons(nPort);
			nStatus = connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

			if(nStatus==0)
			{
				cout<<"Connect establis!"<<endl;
			}
			else
			{
				cout<<"Connect Failer!"<<endl;
				sprintf(szReturn,"error=%s","Socket error! Cann't connect to specified IP Server!");
				return false;
			}
			CString strSendBuff = _T("");

//			strSendBuff = "GET /publish_index/index?ver=2 HTTP/1.1\r\nAccept: */*\r\nAccept-Language: zh-cn\r\nUA-CPU: x86\r\nAccept-Encoding: gzip, deflate\r\nHost: fs.funshion.com\r\nconnection: Keep-Alive\r\n\r\n";
//			strSendBuff.Format("GET %s HTTP/1.1\r\nAccept: */*\r\nAccept-Language: zh-cn\r\nUA-CPU: x86\r\nAccept-Encoding: gzip, deflate\r\nHost: %s\r\nconnection: Keep-Alive\r\n\r\n",strURL,strHostName);
			strSendBuff.Format("GET %s HTTP/1.1\r\nAccept: */*\r\nAccept-Language: zh-cn\r\nAccept-Encoding: gzip, deflate\r\nHost: %s\r\n\r\n",strURL,strHostName);

			send(sockClient, strSendBuff.GetBuffer(strSendBuff.GetLength()), (int)strSendBuff.GetLength()+1, 0);
			char recvBuf[1024*200+1]={0};
			int temp=0;
			char strTMP[1024]={0};
			CString strTemp = _T("");
			temp=recv(sockClient,recvBuf,1024*200,0);
			strTemp.Format("%s",recvBuf);

			if(strTemp.Find("HTTP/1.0")==0)
			{
				strTemp.TrimLeft("HTTP/1.0 ");
				nStatusCode = atoi(strTemp.Left(3).GetBuffer(3));
			}
			else if(strTemp.Find("HTTP/1.1")==0)
			{
				strTemp.Trim("HTTP/1.1 ");
				nStatusCode = atoi(strTemp.Left(3).GetBuffer(3));
			}
			else
			{
				sprintf(szReturn,"error=%s", "Cann't find return code!");
				return false;
			}

			nDownloadsize+=temp;
			while(temp != SOCKET_ERROR&& temp>0)
			{
				cout<<recvBuf<<endl;
				PrintLog(recvBuf);
				temp = recv(sockClient,recvBuf,1024*200,0);
				nDownloadsize+=temp;
			}
			printf("%s\n",recvBuf);
			closesocket(sockClient);
			cout<<"closesocket"<<endl;
			WSACleanup();
			cout<<"WSACleanup"<<endl;
			EndTime = ::GetTickCount();
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
		return TRUE;

}