 

//该工程由 SiteViewTOOL 在 创建新监测器 时自动生成


//****************************************************************************
//	Monitor Export Function
//  本工程必须设置为 MD 编译 （项目属性\ C/C++ \代码生成\运行时库：）
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

bool MakeStringListByChar(list<string> & pList, const char * pInput );	//将输入参数拆入一个 list<string> 中
bool MakeCharByString(char *pOut,int &nOutSize,string strInput );		//按要求输出返回值，将 $ 替换为 \0,  以实现 \0 分割 和  \0\0 为结束标志 的要求

//下面两个参数通过配置文件“smspara.ini”进行配置
string strSMSSendURL;//发送短信的地址
int nSMSSendPort=0;//发送短信的端口号
int nMaxSMSLen=0;//发送短信的最大长度


//===========================================================
//打印日志
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

	wchar_t strLogFile[] = L"EIC-NS20平台.log";

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

	char strLogFile[] = "EIC-NS20平台.log";

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
	//读取短信发送的配置参数
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
				//读入每条短信的最大长度
				string beFindStr="smslength=";
				string::size_type nPos=line.find(beFindStr,0);
				string::size_type nLength=beFindStr.size();
				if(nPos!= string::npos)//如果找到了smslength=，则读出这个值
				{
					line.erase(0,nPos+nLength);
					sscanf(line.c_str(),"%d",&nMaxSMSLen);
				}

				//读入短信服务器的URL地址
				beFindStr="smsurl=";
				nPos=line.find(beFindStr,0);
				nLength=beFindStr.size();
				if(nPos!= string::npos)//如果找到了smsurl=，则读出这个值
				{
					line.erase(0,nPos+nLength);
					strSMSSendURL=line;
				}

				//读入短信服务器的端口号
				beFindStr="smsport=";
				nPos=line.find(beFindStr,0);
				nLength=beFindStr.size();
				if(nPos!= string::npos)//如果找到了smsport=，则读出这个值
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

	/* 初始化Socket */
	status=WSAStartup(MAKEWORD(1, 1), &Data);
	if (status != 0)
	{
		printDebug("ERROR: WSAStartup unsuccessful");
		cerr << "ERROR: WSAStartup unsuccessful"
		<< endl;
	}
	/* 转换服务器地址*/
	destAddr=inet_addr(strSMSSendURL.c_str());
	memcpy(&destSockAddr.sin_addr,
		&destAddr, sizeof(destAddr));
	/* 指定Socket端口 */
	destSockAddr.sin_port=htons(nSMSSendPort);
	destSockAddr.sin_family=AF_INET;

	char strTemp[1024];
	printDebug("-------------------------------------");
	sprintf( strTemp, "%s:%d", strSMSSendURL.c_str(), nSMSSendPort );
    printDebug( strTemp );
	printDebug("-------------------------------------");
	
	

	/* 创建有连接的Socket */
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

	cout << "连接到IP地址: "
		<< strSMSSendURL << "端口：  "<<nSMSSendPort<<endl;



	/* 连接服务器 */
	status=connect(destSocket,
		(LPSOCKADDR) &destSockAddr,
		sizeof(destSockAddr));
	if (status == SOCKET_ERROR)
	{
		cerr << "错误: 连接不成功" << endl;
		printDebug("错误: 连接不成功");
		status=closesocket(destSocket);
		if (status == SOCKET_ERROR)
			cerr << "错误: 关闭SOCKET不成功"
			<< endl;
		printDebug("错误: 关闭SOCKET不成功");
		status=WSACleanup();
		if (status == SOCKET_ERROR)
			cerr << "错误: WSACleanup 不成功"
			<< endl;
		printDebug("错误: WSACleanup 不成功");
		return(0);
	}

	cout << "连接..." << endl;
	int nCount=0;
	bool bSend=false;
	while(1)
	{
		cout << "发送..." << nCount+1<<endl;
		cout << "发送..." << szSendText.c_str()<<endl;

		
		wchar_t wstrSendText[200] = {0};
		//单字节变成Unicode宽字符，如数字1的编码"31"变成"3100"
		MultiByteToWideChar( CP_ACP, 0, szSendText.c_str(), -1, wstrSendText, szSendText.size() );

		numsnt = send( destSocket, (LPSTR)wstrSendText, 2*wcslen( wstrSendText ), 0 );

		if (numsnt >= 2*szSendText.size())//如果发送成功，则退出循环
		{
			bSend=true;
			break;
		}
		//最多循环发送3次
		nCount++;
		if (nCount>=3)
			break;
		/* Wait before sending the message again */
		Sleep(1000);
	} /* while */
	if (bSend)
		cout<<endl<<"发送成功"<<endl;
	printDebug("发送成功");
	
	cout<<"开始接收。。。"<<endl;
	printDebug("开始接收。。。");
	int   nSelectRet;   
	int   nRet; 
	int   nLen;
	char recvbuf[1024];
	memset(recvbuf,0,1024);
	//接收超时设置   
	timeval   outtime   ;     //   超时结构   

	FD_SET   fdr   =   {1,   destSocket};     
	outtime.tv_sec   =   5;       
	outtime.tv_usec   =0;   

	nSelectRet=::select(0,   &fdr,   NULL,   NULL,   &outtime);   //检查可读状态     
	if(nSelectRet==SOCKET_ERROR)     
	{   
		closesocket(destSocket);   
		destSocket   =   NULL;   
	}   
	if(nSelectRet==0)   //超时发生，无可读数据     
	{   
		closesocket(destSocket);   
		destSocket   =   NULL;   
		cout<<"接收数据超时"<<endl;
		printDebug("接收数据超时");
	}   
	else     
	{   
		nLen = 0;
		//接收数据   
		nRet   =   recv(destSocket,   recvbuf,   nLen,   0);   
		if(nRet   ==   SOCKET_ERROR)   
		{   
			//cout<<"无法接收到数据"<<endl;
			printDebug( "无法接收到数据" );
		}   
		else
			printDebug( recvbuf );
			//cout<<"收到：..."<<recvbuf<<endl;
		cout<< "发送完毕,接收到的数据为: "<<recvbuf<<endl;
		printDebug( "发送完毕,接收到的数据为: ");
		printDebug( recvbuf );
	}   
	
	//发送结束，关闭Socket
	cout << "连接终止" << endl;
	printDebug("连接终止");
	status=closesocket(destSocket);
	if (status == SOCKET_ERROR)
	{
		cerr << "错误: 关闭Socket不成功"
			<< endl;
		return 0;
	}
	status=WSACleanup();
	if (status == SOCKET_ERROR)
	{
		cerr << "错误: WSACleanup 不成功"
			<< endl;
		return 0;
	}
	return 1;
}


//通过Socket发送短信
int SendBySocket(char * szSMSTo, char *szContent)
{
	//判断是否配置了strSMSSendURL和nSMSSendPort
	if (strSMSSendURL.empty() || (nSMSSendPort==0))
	    return 0;

	// 信息头
	char strInfoHead[100] = {0};
	int nNumSize = strlen( szSMSTo );
	if( nNumSize > 9 )
        sprintf( strInfoHead, "0%d%s", nNumSize, szSMSTo );
	else
		sprintf( strInfoHead, "00%d%s", nNumSize, szSMSTo );

	
	// 将短信截断
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

		strTemp = strInfoHead + str_content.substr(begin,num_copy);// 信息头＋信息内容
		listSmsContent.push_back(strTemp);

		cout<<"msg:"<<strTemp<<endl
			<<"msg_length="<<strTemp.size()<<endl<<endl;

		begin +=num_copy;
	}
	
	strTemp = strInfoHead + str_content.substr(begin,msg_len - begin);// 信息头＋信息内容
	listSmsContent.push_back(strTemp);

	cout<<"msg:"<<strTemp<<endl
			<<"msg_length="<<strTemp.size()<<endl<<endl;
	
	string sendMess;
	std::list<string>::iterator listSmsContentItem;

	for(listSmsContentItem = listSmsContent.begin(); listSmsContentItem!=listSmsContent.end(); listSmsContentItem++)
	{		
		//采用Socket发送短信
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
	retstr = "利用socket方式发送短信";
	return 1;
}


//动态链接库的入口
//funname,调用的发送短信的函数
//szSMTo，短信发送给谁
//szContent,短信内容
//返回为1表示成功
extern "C" __declspec(dllexport) int run(char * szFunName, char * szSMTo, char *szContent)   
{
	ReadConfig();
	//调用发送函数发送短信
	SendBySocket( szSMTo, szContent);
	//sprintf(strTemp,"========== run,content=%s ============",szContent);

	return 1;
}
