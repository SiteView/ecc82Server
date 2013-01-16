// CtripURLMonitor.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "CtripURLMonitor.h"
#include <fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//	ע�⣡
//
//		����� DLL ��̬���ӵ� MFC
//		DLL���Ӵ� DLL ������
//		���� MFC ���κκ����ں�������ǰ��
//		��������� AFX_MANAGE_STATE �ꡣ
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CCtripURLMonitorApp

BEGIN_MESSAGE_MAP(CCtripURLMonitorApp, CWinApp)
END_MESSAGE_MAP()


// CCtripURLMonitorApp ����

CCtripURLMonitorApp::CCtripURLMonitorApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CCtripURLMonitorApp ����

CCtripURLMonitorApp theApp;


// CCtripURLMonitorApp ��ʼ��

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
	int BeginTime=0,EndTime=0;		//��ȡҳ�濪ʼ�ͽ���ʱ�䡣
	int nDownloadsize=0;			//���ص���ҳ��С
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
//���Socket��İ汾�Ƿ�Ϊ1.0
		if ( LOBYTE( wsaData.wVersion ) != 1 ||
			HIBYTE( wsaData.wVersion ) != 1 ) {
				WSACleanup( );
				return FALSE;
			}
//����Socket
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
//����HTTP���ݰ�
			CString strSendBuff = _T("");
			strSendBuff.Format("GET %s HTTP/1.1\r\nAccept: */*\r\nAccept-Language: zh-cn\r\nAccept-Encoding: gzip, deflate\r\nHost: %s\r\nConnection: Close\r\n\r\n",strURL,strHostName);
//			strSendBuff.Format("GET %s HTTP/1.1\r\nAccept: */*\r\nAccept-Language: zh-cn\r\nAccept-Encoding: gzip, deflate\r\nHost: %s\r\nConnection: Close\r\n\r\n","/Domestic/SearchHotel.aspx","hotels.ctrip.com");
			send(sockClient, strSendBuff.GetBuffer(strSendBuff.GetLength()), (int)strSendBuff.GetLength()+1, 0);
//��ȡHTTP���ݰ�
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
				sprintf(tempbuf,"%s","");		//���tempbuf
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


//�Ͽ�socket����
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