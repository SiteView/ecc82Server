 

//�ù����� SiteViewTOOL �� �����¼���� ʱ�Զ�����


//****************************************************************************
//	Monitor Export Function
//  �����̱�������Ϊ MD ���� ����Ŀ����\ C/C++ \��������\����ʱ�⣺��
//****************************************************************************

#include <string>
#include <list>
#include <windows.h>
#include "atlstr.h"
#include <stdio.h>
#include <stdlib.h>
#include <iosfwd>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include "C:\SiteView\Ecc_Common\Base\StringHelper.h"

using namespace SH;
using namespace std;
using std::string;
using std::list;

bool MakeStringListByChar(list<string> & pList, const char * pInput );	//�������������һ�� list<string> ��
bool MakeCharByString(char *pOut,int &nOutSize,string strInput );		//��Ҫ���������ֵ���� $ �滻Ϊ \0,  ��ʵ�� \0 �ָ� ��  \0\0 Ϊ������־ ��Ҫ��

//������������ͨ�������ļ���smspara.ini����������
string strSMSSendURL;//���Ͷ��ŵĵ�ַ
int nSMSSendPort=0;//���Ͷ��ŵĶ˿ں�
int nMaxSMSLen=0;//���Ͷ��ŵ���󳤶�


//===========================================================
//��ӡ��־
//===========================================================
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

void writeWLog(const wchar_t * str)
{
	return ;

	wchar_t timeBuf[128],dateBuf[128];

	_tzset();
	_wstrtime( timeBuf );
	_wstrdate( dateBuf );

	wchar_t strLogFile[] = L"EIC-NS20ƽ̨.log";

	FILE *log ;
	
	log = _wfopen( strLogFile, L"a+" );
	if( log != NULL )
	{
		fwprintf( log, L"%s %s\t%s\n", dateBuf, timeBuf, str );
		fclose( log );
	}

}

void writeLog(const char * str)
{
	char timeBuf[128],dateBuf[128];

	_tzset();
	_strtime(timeBuf);
	_strdate(dateBuf);

	char strLogFile[] = "EIC-NS20ƽ̨.log";

	FILE *log ;

	struct stat fStat;
    if( stat( strLogFile, &fStat ) != -1 )
	{

		if((long)fStat.st_size > 10*1024)
		{
			log=fopen(strLogFile,"w");
			if(log != NULL)
			{
				fclose(log);
			}
		}
    }
	
	log=fopen(strLogFile,"a+");
	if(log != NULL)
	{
		fprintf(log,"%s %s\t%s\n",dateBuf,timeBuf,str);
		fclose(log);
	}

}

//#define TOFILE
//#define TOCONSOLE

void printDebug( const char* str )
{
#ifdef TOFILE
    writeLog( str );
#endif

#ifdef TOCONSOLE
	//OutputDebugString( str );
	cout << str << endl;
#endif
}

bool ReadConfig()
{
	//��ȡ���ŷ��͵����ò���
	printDebug("=================== ReadConfig =====================");

	ifstream input("smspara.ini", ios::in);
	string line; 
	if (input.is_open())
	{
		while(1) 
		{ 
			getline(input,line);
			if(!line.empty())
			{
				//����ÿ�����ŵ���󳤶�
				string beFindStr="smslength=";
				string::size_type nPos=line.find(beFindStr,0);
				string::size_type nLength=beFindStr.size();
				if(nPos!= string::npos)//����ҵ���smslength=����������ֵ
				{
					line.erase(0,nPos+nLength);
					sscanf(line.c_str(),"%d",&nMaxSMSLen);
				}

				//������ŷ�������URL��ַ
				beFindStr="smsurl=";
				nPos=line.find(beFindStr,0);
				nLength=beFindStr.size();
				if(nPos!= string::npos)//����ҵ���smsurl=����������ֵ
				{
					line.erase(0,nPos+nLength);
					strSMSSendURL=line;
				}

				//������ŷ������Ķ˿ں�
				beFindStr="smsport=";
				nPos=line.find(beFindStr,0);
				nLength=beFindStr.size();
				if(nPos!= string::npos)//����ҵ���smsport=����������ֵ
				{
					line.erase(0,nPos+nLength);
					sscanf(line.c_str(),"%d",&nSMSSendPort);
				}
			}
			if (input.eof())
				break;

		}
	}
	input.close();
	return true;
}

bool ParserToLength(list<string >&pTokenList, string  strQueryString, int nLength)
{
	int nCount = strQueryString.length() / nLength;
	for(int i = 0; i <= nCount; i++)
	{
		string strTmp = strQueryString.substr(0, nLength);
		pTokenList.push_back(strTmp);
		strQueryString.erase(0, nLength);
	}

	return true;
}

int SocketSend(string szSendText)
{
	WSADATA Data;
	SOCKADDR_IN destSockAddr;
	SOCKET destSocket;
	unsigned long destAddr;
	int status;
	int numsnt;

	/* ��ʼ��Socket */
	status=WSAStartup(MAKEWORD(1, 1), &Data);
	if (status != 0)
	{
		printDebug("ERROR: WSAStartup unsuccessful");
		cerr << "ERROR: WSAStartup unsuccessful"
		<< endl;
	}
	/* ת����������ַ*/
	destAddr=inet_addr(strSMSSendURL.c_str());
	memcpy(&destSockAddr.sin_addr,
		&destAddr, sizeof(destAddr));
	/* ָ��Socket�˿� */
	destSockAddr.sin_port=htons(nSMSSendPort);
	destSockAddr.sin_family=AF_INET;

	char strTemp[1024];
	printDebug("-------------------------------------");
	sprintf( strTemp, "%s:%d", strSMSSendURL.c_str(), nSMSSendPort );
    printDebug( strTemp );
	printDebug("-------------------------------------");
	
	

	/* ���������ӵ�Socket */
	destSocket=socket(AF_INET, SOCK_STREAM, 0);
	if (destSocket == INVALID_SOCKET)
	{
		printDebug("ERROR: socket unsuccessful");
		cerr << "ERROR: socket unsuccessful" << endl;
		status=WSACleanup();
		if (status == SOCKET_ERROR)
			cerr << "ERROR: WSACleanup unsuccessful" 
			<< endl;
		printDebug("ERROR: WSACleanup unsuccessful");
		return(1);
	}

	cout << "���ӵ�IP��ַ: "
		<< strSMSSendURL << "�˿ڣ�  "<<nSMSSendPort<<endl;



	/* ���ӷ����� */
	status=connect(destSocket,
		(LPSOCKADDR) &destSockAddr,
		sizeof(destSockAddr));
	if (status == SOCKET_ERROR)
	{
		cerr << "����: ���Ӳ��ɹ�" << endl;
		printDebug("����: ���Ӳ��ɹ�");
		status=closesocket(destSocket);
		if (status == SOCKET_ERROR)
			cerr << "����: �ر�SOCKET���ɹ�"
			<< endl;
		printDebug("����: �ر�SOCKET���ɹ�");
		status=WSACleanup();
		if (status == SOCKET_ERROR)
			cerr << "����: WSACleanup ���ɹ�"
			<< endl;
		printDebug("����: WSACleanup ���ɹ�");
		return(0);
	}

	cout << "����..." << endl;
	int nCount=0;
	bool bSend=false;
	while(1)
	{
		cout << "����..." << nCount+1<<endl;
		cout << "����..." << szSendText.c_str()<<endl;

		
		wchar_t wstrSendText[200] = {0};
		//���ֽڱ��Unicode���ַ���������1�ı���"31"���"3100"
		MultiByteToWideChar( CP_ACP, 0, szSendText.c_str(), -1, wstrSendText, szSendText.size() );

		numsnt = send( destSocket, (LPSTR)wstrSendText, 2*wcslen( wstrSendText ), 0 );

		if (numsnt >= 2*szSendText.size())//������ͳɹ������˳�ѭ��
		{
			bSend=true;
			break;
		}
		//���ѭ������3��
		nCount++;
		if (nCount>=3)
			break;
		/* Wait before sending the message again */
		Sleep(1000);
	} /* while */
	if (bSend)
		cout<<endl<<"���ͳɹ�"<<endl;
	printDebug("���ͳɹ�");
	
	cout<<"��ʼ���ա�����"<<endl;
	printDebug("��ʼ���ա�����");
	int   nSelectRet;   
	int   nRet; 
	int   nLen;
	char recvbuf[1024];
	memset(recvbuf,0,1024);
	//���ճ�ʱ����   
	timeval   outtime   ;     //   ��ʱ�ṹ   

	FD_SET   fdr   =   {1,   destSocket};     
	outtime.tv_sec   =   5;       
	outtime.tv_usec   =0;   

	nSelectRet=::select(0,   &fdr,   NULL,   NULL,   &outtime);   //���ɶ�״̬     
	if(nSelectRet==SOCKET_ERROR)     
	{   
		closesocket(destSocket);   
		destSocket   =   NULL;   
	}   
	if(nSelectRet==0)   //��ʱ�������޿ɶ�����     
	{   
		closesocket(destSocket);   
		destSocket   =   NULL;   
		cout<<"�������ݳ�ʱ"<<endl;
		printDebug("�������ݳ�ʱ");
	}   
	else     
	{   
		nLen = 0;
		//��������   
		nRet   =   recv(destSocket,   recvbuf,   nLen,   0);   
		if(nRet   ==   SOCKET_ERROR)   
		{   
			//cout<<"�޷����յ�����"<<endl;
			printDebug( "�޷����յ�����" );
		}   
		else
			printDebug( recvbuf );
			//cout<<"�յ���..."<<recvbuf<<endl;
		cout<< "�������,���յ�������Ϊ: "<<recvbuf<<endl;
		printDebug( "�������,���յ�������Ϊ: ");
		printDebug( recvbuf );
	}   
	
	//���ͽ������ر�Socket
	cout << "������ֹ" << endl;
	printDebug("������ֹ");
	status=closesocket(destSocket);
	if (status == SOCKET_ERROR)
	{
		cerr << "����: �ر�Socket���ɹ�"
			<< endl;
		return 0;
	}
	status=WSACleanup();
	if (status == SOCKET_ERROR)
	{
		cerr << "����: WSACleanup ���ɹ�"
			<< endl;
		return 0;
	}
	return 1;
}


//ͨ��Socket���Ͷ���
int SendBySocket(char * szSMSTo, char *szContent)
{
	//�ж��Ƿ�������strSMSSendURL��nSMSSendPort
	if (strSMSSendURL.empty() || (nSMSSendPort==0))
	    return 0;

	// ��Ϣͷ
	char strInfoHead[100] = {0};
	int nNumSize = strlen( szSMSTo );
	if( nNumSize > 9 )
        sprintf( strInfoHead, "0%d%s", nNumSize, szSMSTo );
	else
		sprintf( strInfoHead, "00%d%s", nNumSize, szSMSTo );

	
	// �����Žض�
	CAnalyzer4_bstr_t Analyzer;
	_bstr_t strMsgs = szContent;
	OutputDebugString(szContent);

	string str_content = szContent;

	int msg_len = str_content.length();
	//cout<<"msg_len:"<<msg_len<<endl;
	int max_len = nMaxSMSLen;

	int begin(0);
	int num_copy(0);

	string strTemp;
	std::list<string> listSmsContent;

	while((msg_len-begin)>max_len)
	{
		int num_unicode(0);
		
		for(basic_string<char>::iterator it = str_content.begin()+begin;
			it != str_content.begin()+begin+max_len;it++)
		{
			if(unsigned char(*it) >= unsigned char(0x80))
				num_unicode++;
		}

		cout<<"num_unicode:"<<num_unicode<<endl;

		if(num_unicode%2)
			num_copy = max_len - 1;
		else
			num_copy = max_len;

		strTemp = strInfoHead + str_content.substr(begin,num_copy);// ��Ϣͷ����Ϣ����
		listSmsContent.push_back(strTemp);

		cout<<"msg:"<<strTemp<<endl
			<<"msg_length="<<strTemp.size()<<endl<<endl;

		begin +=num_copy;
	}
	
	strTemp = strInfoHead + str_content.substr(begin,msg_len - begin);// ��Ϣͷ����Ϣ����
	listSmsContent.push_back(strTemp);

	cout<<"msg:"<<strTemp<<endl
			<<"msg_length="<<strTemp.size()<<endl<<endl;
	
	string sendMess;
	std::list<string>::iterator listSmsContentItem;

	for(listSmsContentItem = listSmsContent.begin(); listSmsContentItem!=listSmsContent.end(); listSmsContentItem++)
	{		
		//����Socket���Ͷ���
		cout<<"send_msg:"<<(*listSmsContentItem)<<endl;

		sendMess=(*listSmsContentItem);
		SocketSend(sendMess);
		Sleep(3000);
		printDebug(sendMess.c_str());
	}
	printDebug(sendMess.c_str());
}

extern "C" __declspec(dllexport) int getinfo(string&retstr)
{
	retstr = "����socket��ʽ���Ͷ���";
	return 1;
}


//��̬���ӿ�����
//funname,���õķ��Ͷ��ŵĺ���
//szSMTo�����ŷ��͸�˭
//szContent,��������
//����Ϊ1��ʾ�ɹ�
extern "C" __declspec(dllexport) int run(char * szFunName, char * szSMTo, char *szContent)   
{
	ReadConfig();
	//���÷��ͺ������Ͷ���
	SendBySocket( szSMTo, szContent);
	//sprintf(strTemp,"========== run,content=%s ============",szContent);

	return 1;
}
