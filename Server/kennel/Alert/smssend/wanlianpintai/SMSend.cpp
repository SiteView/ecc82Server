 

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
#include "Base\StringHelper.h"

using namespace SH;
using namespace std;
using std::string;
using std::list;

#include <svapi.h>
#include <svdbapi.h>

bool MakeStringListByChar(list<string> & pList, const char * pInput );	//�������������һ�� list<string> ��
bool MakeCharByString(char *pOut,int &nOutSize,string strInput );		//��Ҫ���������ֵ���� $ �滻Ϊ \0,  ��ʵ�� \0 �ָ� ��  \0\0 Ϊ������־ ��Ҫ��

//������������ͨ�������ļ���smspara.ini����������
string strSMSSendURL;//���Ͷ��ŵĵ�ַ
int nSMSSendPort=0;//���Ͷ��ŵĶ˿ں�
int nMaxSMSLen=0;//���Ͷ��ŵ���󳤶�


char strTemp[1024];


//===============================================================
// ��ӡ��־����
//===============================================================
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

void PrintDebug( const char* str )
{
	char timebuf[128],datebuf[128];

	_tzset();
	_strtime( timebuf );
	_strdate( datebuf );

	string strRootPath  = GetSiteViewRootPath();
	char szLogFile[128] = {0};
	sprintf(szLogFile , "%s\\temp\\%s" , strRootPath.c_str() , "smsSend.log");

	// �ж��ļ���С���ڲ����ļ��������ʵ��
	struct _stat buf;
	if( _stat( szLogFile, &buf ) == 0 )
	{
		if( buf.st_size > 10000*1024 )
		{
			FILE *log = fopen( szLogFile, "w" );
			if( log != NULL )
				fclose( log );
		}
	}


	FILE *log = fopen( szLogFile,"a+");
	if( log != NULL )
	{
		fprintf( log, "%s %s \t%s\n", datebuf, timebuf, str );
		fclose( log );
	}

}

bool ReadConfig()
{
	//��ȡ���ŷ��͵����ò���
	PrintDebug("=================== ReadConfig =====================");

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
		PrintDebug("ERROR: WSAStartup unsuccessful");
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

	PrintDebug("-------------------------------------");
	sprintf( strTemp, "%s:%d", strSMSSendURL.c_str(), nSMSSendPort );
    PrintDebug( strTemp );
	PrintDebug("-------------------------------------");
	

	/* ���������ӵ�Socket */
	destSocket=socket(AF_INET, SOCK_STREAM, 0);
	if (destSocket == INVALID_SOCKET)
	{
		PrintDebug("ERROR: socket unsuccessful");
		cerr << "ERROR: socket unsuccessful" << endl;
		status=WSACleanup();
		if (status == SOCKET_ERROR)
			cerr << "ERROR: WSACleanup unsuccessful" 
			<< endl;
		PrintDebug("ERROR: WSACleanup unsuccessful");
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
		PrintDebug("����: ���Ӳ��ɹ�");
		status=closesocket(destSocket);
		if (status == SOCKET_ERROR)
			cerr << "����: �ر�SOCKET���ɹ�"
			<< endl;
		PrintDebug("����: �ر�SOCKET���ɹ�");
		status=WSACleanup();
		if (status == SOCKET_ERROR)
			cerr << "����: WSACleanup ���ɹ�"
			<< endl;
		PrintDebug("����: WSACleanup ���ɹ�");
		return(0);
	}

	cout << "����..." << endl;
	int nCount=0;
	bool bSend=false;
	while(1)
	{
		cout << "����..." << nCount+1<<endl;
		cout << "����..." << szSendText.c_str()<<endl;
		numsnt=send(destSocket, szSendText.c_str(), 
			szSendText.size() + 1, 0);
		if (numsnt >=szSendText.size())//������ͳɹ������˳�ѭ��
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
	PrintDebug("���ͳɹ�");
	
	cout<<"��ʼ���ա�����"<<endl;
	PrintDebug("��ʼ���ա�����");
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

		PrintDebug( "�������ݳ�ʱ\n" );
	}   
	else     
	{   
		nLen = 0;
		//��������   
		nRet   =   recv(destSocket,   recvbuf,   nLen,   0);   
		if(nRet   ==   SOCKET_ERROR)   
		{   
			cout<<"�޷����յ�����"<<endl;
			PrintDebug( "�޷����յ�����\n" );
		}   
		else
			cout<<"�յ���..."<<recvbuf<<endl;
		cout<< "�������,���յ�������Ϊ: "<<recvbuf<<endl;
		PrintDebug( "�������,���յ�������Ϊ:" );
		PrintDebug(recvbuf);

	}   
	
	//���ͽ������ر�Socket
	cout << "������ֹ" << endl;
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
	//�ӹ������͵ı���
	
	string SMSHead;//����ͷ
	string szSmsTo=szSMSTo;
	
	//�����Žض�
	/*
	CAnalyzer4_bstr_t Analyzer;
	_bstr_t strMsgs = szContent;
	OutputDebugString(szContent);
	//	int nMaxSMSLen = MAX_SMS_LENGTH;
	//�޸�Ϊ�û��趨��ֵ

	// ��ԭʼ�������Ľ��п��ַ��ֶ�
	
	int nPage = 0;
	
	string content = szContent;
	int totalLength = 0;
	int chalen = content.length();
	for(int i = 0;i<chalen;i++)
	{
		totalLength++;
		if((szContent[i]>=0x81)&&(szContent[i]<=0xfe))
		{
			totalLength++;
		}
	}

	nPage = (totalLength/nMaxSMSLen)+(((nMaxSMSLen%totalLength)>0)?1:0);

	std::list<string> listSmsContent;
	std::list<string>::iterator listSmsContentItem;
	
	int left = totalLength;
	for (int i = 0; i < nPage; i++)
	{
		char buf[300];
		memset(buf, 0, 300);
		string strMsg;
		if(left<nMaxSMSLen)
		{
			strMsg = content.substr(totalLength-left,left);
			left = 0;
		}
		else
		{
			strMsg = content.substr(totalLength-left,nMaxSMSLen);
			left = left-nMaxSMSLen;
		}
		strcpy(buf, strMsg.c_str());
		listSmsContent.push_back(buf);
	}
	*/

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

		strTemp = str_content.substr(begin,num_copy);
		listSmsContent.push_back(strTemp);

		cout<<"msg:"<<strTemp<<endl
			<<"msg_length="<<strTemp.size()<<endl<<endl;

		begin +=num_copy;
	}
	
	strTemp = str_content.substr(begin,msg_len - begin);
	listSmsContent.push_back(strTemp);

	cout<<"msg:"<<strTemp<<endl
			<<"msg_length="<<strTemp.size()<<endl<<endl;
	
	string sendMess;
	std::list<string>::iterator listSmsContentItem;

	for(listSmsContentItem = listSmsContent.begin(); listSmsContentItem!=listSmsContent.end(); listSmsContentItem++)
	{
		string strSMS = (*listSmsContentItem);
		SMSHead="<Command>SMS</Command><Value1>";
		SMSHead=SMSHead+szSmsTo+"</Value1><Value2>";
		
		//����Socket���Ͷ���
		cout<<"send_msg:"<<(*listSmsContentItem)<<endl;

		sendMess=SMSHead+(*listSmsContentItem)+"</Value2>";
		SocketSend(sendMess);
		Sleep(3000);
		PrintDebug(sendMess.c_str());
	}
	PrintDebug(sendMess.c_str());
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
	SendBySocket(szSMTo,szContent);
	sprintf(strTemp,"========== run,content=%s ============",szContent);

	return 1;
}
