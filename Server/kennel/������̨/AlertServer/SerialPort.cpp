// SerialPort.cpp: implementation of the CSerialPort class.
//
/////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SerialPort.h"
#include <string>
using namespace std;

/////////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////////////////
extern void WriteLogFile(CString strMsg);
void DebugePrint(string strDebugInfo);

void WriteErr(const char * str);
int WriteLog(const char * str);

/////////////////////////////////////////////////////////////////////////////////
CSerialPort::CSerialPort()
{

}

/////////////////////////////////////////////////////////////////////////////////
CSerialPort::~CSerialPort()
{
    
}

/////////////////////////////////////////////////////////////////////////////////
// 函数：SetRecvPhone                                                          //
// 说明：设置接收手机号码                                                      //
// 参数：                                                                      //
//      strRecvPhone，接收手机号码                                             //
/////////////////////////////////////////////////////////////////////////////////
void CSerialPort::SetRecvPhone(CString strRecvPhone)
{
    m_strRecvPhone = strRecvPhone;
}

/////////////////////////////////////////////////////////////////////////////////
// 函数：SetMsgContent                                                         //
// 说明：设置短信内容                                                          //
// 参数：                                                                      //
//     strMsgContent，短信的内容                                               //
/////////////////////////////////////////////////////////////////////////////////
void CSerialPort::SetMsgContent(CString strMsgContent)
{
    m_strMsgContent = strMsgContent;
}

/////////////////////////////////////////////////////////////////////////////////
// 函数：SetSerialPortName                                                     //
// 说明：设置需要打开的串行端口名称                                            //
// 参数：                                                                      //
//      strPortName，串行端口名称                                              //
/////////////////////////////////////////////////////////////////////////////////
void CSerialPort::SetSerialPortName(CString strPortName)
{
    m_strSerialPort = strPortName;
}

/////////////////////////////////////////////////////////////////////////////////
// 函数：SendMsg                                                               //
// 说明：通过指定端口，发送短信                                                //
// 参数：                                                                      //
//      strRecvPhone，接收手机号码                                             //
//      strMsgContent，短信内容                                                //
//      strPortName，串行端口的名称                                            //
/////////////////////////////////////////////////////////////////////////////////
int CSerialPort::SendMsg( CString strRecvPhone , CString strMsgContent, int nSMSMaxLength )
{
    m_strRecvPhone = strRecvPhone;
    m_strMsgContent = strMsgContent;
    int i=0 , t=0;
    char ans[128];        // 应答串
    //char SCA[16];       // 短消息服务中心号码(SMSC地址)
    char TPA[16];       // 目标号码或回复号码(TP-DA或TP-RA)
    //char TP_PID;        // 用户信息协议标识(TP-PID)
    //char TP_DCS;        // 用户信息编码方式(TP-DCS)
//    char TP_UD[161];    // 原始用户信息(编码前或解码后的TP-UD)
    char  cmd[20];
    MsgList *msglist = new MsgList;
	int iPage = page( strMsgContent.GetBuffer(strMsgContent.GetLength()), msglist, nSMSMaxLength );

	WriteErr( "报警内容：" );
	WriteErr( strMsgContent.GetBuffer() );

    
    t= m_strRecvPhone.GetLength()+2;
	for(i=2;i<t;i++)
    {
	   TPA[i]=m_strRecvPhone.GetAt(i-2);
    }
    TPA[0]='8';
    TPA[1]='6';
	TPA[t]='\0';

	for(int iIndex=0 ;iIndex < iPage;iIndex++)
	{
		//printf( "Content:%s,Msg:%s\n", strMsgContent.GetBuffer(), msglist->chMsg );
		WriteErr( "短信：" );
		WriteErr( msglist->chMsg );

        sprintf(cmd , "AT\r");
        WritePort(cmd, (int)strlen(cmd));
	    Sleep(ShortSleepTime);
        int nLength = ReadPort(ans, 128);
        ans[nLength] = '\0';

        sprintf(cmd, "ATE0\r"); 
        WritePort(cmd, (int)strlen(cmd));
	    Sleep(ShortSleepTime);
        nLength = ReadPort(ans, 128);
        ans[nLength] = '\0';

	    sprintf(cmd, "AT+CSMS=0\r");      
        WritePort(cmd, (int)strlen(cmd));  
 	    Sleep(ShortSleepTime);
        nLength = ReadPort(ans, 128);
        ans[nLength] = '\0';
 
	    sprintf(cmd, "AT+CMGF=0\r");      
        WritePort(cmd, (int)strlen(cmd));  
	    Sleep(ShortSleepTime);
        nLength = ReadPort(ans, 128);
        ans[nLength] = '\0';
          sm_param_temp= new SM_PARAM;
        strcpy(sm_param_temp->SCA,m_SmsCenterNum);

        sm_param_temp->TP_DCS=0x8;
        sm_param_temp->TP_PID=0x0;
        strcpy(sm_param_temp->TPA,TPA);
        strcpy(sm_param_temp->TP_UD,msglist->chMsg);
        if(iPage > 1)
            sprintf(sm_param_temp->TP_UD,"%d/%d %s" ,iIndex+1,iPage,msglist->chMsg);
        //printf("Msg :%s\n" ,sm_param_temp->TP_UD);
        msglist = msglist->pNext;      
        if(!gsmSendMessage(sm_param_temp))//发送短信
        {
            printf("Send SMS Failed\n");
			WriteLog("Send SMS Failed");
            return -1;
        }
        Sleep(5000);
    }
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// 函数：ClosePort                                                             //
// 说明：关闭已经打开的串行端口                                                //
// 参数：                                                                      //
//      hPort，打开的串口的句柄                                                //
/////////////////////////////////////////////////////////////////////////////////
BOOL CSerialPort::ClosePort(HANDLE hPort)
{
	return CloseHandle(hPort);
}

/////////////////////////////////////////////////////////////////////////////////
// 函数：OpenPort                                                              //
// 说明：打开指定的串口                                                        //
// 参数：                                                                      //
//      pPort: 串口名称或设备路径，可用"COM1"或"\\.\COM1"两种方式，建议用后者  //
//      nBaudRate: 波特率                                                      //
//      nParity: 奇偶校验                                                      //
//      nByteSize: 数据字节宽度                                                //
//      nStopBits: 停止位                                                      //
// 返回值：打开的串口句柄                                                      //
/////////////////////////////////////////////////////////////////////////////////
HANDLE CSerialPort::OpenPort(const char* pPort, int nBaudRate,
							 int nParity, int nByteSize, int nStopBits)
{
    HANDLE  hPort;
    DCB dcb;        // 串口控制块
    COMMTIMEOUTS timeouts = {    // 串口超时控制参数
        100,        // 读字符间隔超时时间: 100 ms
        1,          // 读操作时每字符的时间: 1 ms (n个字符总共为n ms)
        500,        // 基本的(额外的)读超时时间: 500 ms
        1,          // 写操作时每字符的时间: 1 ms (n个字符总共为n ms)
        100};       // 基本的(额外的)写超时时间: 100 ms
    
    hPort = CreateFile(pPort,    // 串口名称或设备路径
            GENERIC_READ | GENERIC_WRITE,    // 读写方式
            0,               // 共享方式：独占
            NULL,            // 默认的安全描述符
            OPEN_EXISTING,   // 创建方式
            0,               // 不需设置文件属性
            NULL);           // 不需参照模板文件
    
    if(hPort == INVALID_HANDLE_VALUE) 
    {
//        printf("Open Port Failed\n");
        return NULL;        // 打开串口失败
    }
    
    GetCommState(hPort, &dcb);        // 取DCB
    
    dcb.BaudRate = nBaudRate;
    dcb.ByteSize = nByteSize;
    dcb.Parity = nParity;
    dcb.StopBits = nStopBits;
    
    SetCommState(hPort, &dcb);        // 设置DCB    
    SetupComm(hPort, 4096, 1024);     // 设置输入输出缓冲区大小    
    SetCommTimeouts(hPort, &timeouts);    // 设置超时
    
    return hPort;
}

/////////////////////////////////////////////////////////////////////////////////
// 函数：ReadPort                                                              //
// 说明：读串口                                                                //
// 参数：                                                                      //
//      pData: 待读的数据缓冲区指针                                            //
//      nLength: 待读的最大数据长度                                            //
// 返回值: 实际读入的数据长度                                                  //
/////////////////////////////////////////////////////////////////////////////////
int CSerialPort::ReadPort(void* pData, int nLength)
{
    DWORD dwNumRead;    // 串口收到的数据长度    
    ReadFile(hSerialPort, pData, (DWORD)nLength, &dwNumRead, NULL);    
    return (int)dwNumRead;
}

/////////////////////////////////////////////////////////////////////////////////
// 函数：WritePort                                                             //
// 说明：写串口                                                                //
//      pData: 待写的数据缓冲区指针                                            //
//      nLength: 待写的数据长度                                                //
// 返回值：是否写成功                                                          //
/////////////////////////////////////////////////////////////////////////////////
BOOL CSerialPort::WritePort(void* pData, int nLength)
{
    DWORD dwNumWrite;    // 串口发出的数据长度    
    return WriteFile(hSerialPort, pData, (DWORD)nLength, &dwNumWrite, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// 函数名：page                                                            //
// 功能：如果短消息超长，将短消息分成两页。                                //
// 参数：                                                                  //
//    char *SrcMsg;消息源地址。                                            //
//    char **DestMsg;用于存放分页后的短消息。                              //
// 返回值：                                                                //
//    返回短消息页数。                                                     //
/////////////////////////////////////////////////////////////////////////////
int CSerialPort::page(char *SrcMsg, MsgList *DestMsg, int nSMSMaxLength )
{
	int nMax(0);
	if( nSMSMaxLength > 0 )
		nMax = nSMSMaxLength;
	else
		nMax = MaxSMSLen;

	//printf( "Max=%d,SMSMaxLenth=%d,MaxSMSLen=%d\n", nMax, nSMSMaxLength, MaxSMSLen );
		
	MsgList *msgTemp = DestMsg;
    //当前页中字符的个数
    int num = 0;
    //当前的页数
    int nCur = 0;
    //总页数
    int nPage = 1;
    //短消息的长度
    int nMsgLen = (int)strlen(SrcMsg);
	int flag=0;

    //分页后的页数
    nPage = nMsgLen / nMax + 1;
    //分页
	if(nPage==1) //页数为1
    {
        strcpy(DestMsg->chMsg,SrcMsg);
		DestMsg->pNext = NULL;
    }
	else
	{
        //判断短信的长度是否超过最大短信长度
		while(static_cast<int>(strlen(SrcMsg)) >= nMax)
		{
			flag=0;
			for(int i=0;i<nMax;i++)
			{
                //当前字符是否是数字或者英文字母
				if((unsigned char)SrcMsg[i]<(unsigned char)0x80) 
				{
					flag=flag+1;
				}
			}
			if(flag%2==1)//如果英文字母是单数
			{
                //拷贝最大短信长度-1
				strncpy(msgTemp->chMsg,SrcMsg,nMax-1);
                //添加结束符号
				msgTemp->chMsg[nMax-1]='\0';
                //原始字符串为从0开始减去最大短信长度-1剩余的字符串
				strcpy(SrcMsg , SrcMsg+nMax-1);
			}
			else
			{
				strncpy(msgTemp->chMsg,SrcMsg,nMax-2);
				msgTemp->chMsg[nMax-2]='\0';	
				strcpy(SrcMsg , SrcMsg+nMax-2);
			}
			msgTemp->pNext = new MsgList;//增加列表
			msgTemp = msgTemp->pNext;//当前下移
		}
        //短信长度小于最大短信长度的部分添加到列表中
		strncpy(msgTemp->chMsg,SrcMsg,strlen(SrcMsg));
		msgTemp->chMsg[strlen(SrcMsg)]='\0';
	}
	//返回页数

    return nPage;
}

/////////////////////////////////////////////////////////////////////////////////
// 函数：gsmDeleteMessage                                                      //
// 说明：删除短消息                                                            //
// 参数：                                                                      //
//      index: 短消息序号，从1开始                                             //
/////////////////////////////////////////////////////////////////////////////////
bool CSerialPort::gsmDeleteMessage(const int index)
{
    int nLength;          // 串口收到的数据长度
    char cmd[16];         // 命令串
    char ans[128];        // 应答串
    
    sprintf(cmd, "AT+CMGD=%d\r", index);    // 生成命令
    
    // 输出命令串
    WritePort(cmd, static_cast<int>(strlen(cmd)));
    
    // 读应答数据
    nLength = ReadPort(ans, 128);
    
    // 根据能否找到"+CMS ERROR"决定成功与否
    if(nLength > 0 && strncmp(ans, "+CMS ERROR", 10) != 0)
    {
        return TRUE;
    }
    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////
// 函数：gsmSendMessage                                                        //
// 说明：发送短消息                                                            //
// 参数：                                                                      //
//      pSrc: 源PDU参数指针                                                    //
/////////////////////////////////////////////////////////////////////////////////
bool CSerialPort::gsmSendMessage(const SM_PARAM* pSrc)
{
    int nPduLength;        // PDU串长度
    unsigned char nSmscLength;    // SMSC串长度
    int nLength;           // 串口收到的数据长度
	char cmd[16] = {0};          // 命令串
	char pdu[512] = {0};         // PDU串
	char ans[128] = {0};         // 应答串
 

	nPduLength = gsmEncodePdu(pSrc, pdu);    // 根据PDU参数，编码PDU串
    strcat(pdu, "\x01a");        // 以Ctrl-Z结束
    
    gsmString2Bytes(pdu, &nSmscLength, 2);    // 取PDU串中的SMSC信息长度
    nSmscLength++;        // 加上长度字节本身
    // 命令中的长度，不包括SMSC信息长度，以数据字节计

    sprintf(cmd, "AT+CMGS=%d\r", nPduLength / 2 - nSmscLength);    // 生成命令


	/*
	char szTemp[1024] = {0};
	sprintf( szTemp, "发送信息命令：%s, 信息内容：%s\n\0", cmd, pdu );
	WriteLog( szTemp );

	// 先输出命令串
    WritePort(cmd, strlen(cmd));    // 先输出命令串
	Sleep(ShortSleepTime);

	// 输出信息   	
	WritePort(pdu, strlen(pdu));
    WritePort("\x1A", 1);
    
	// 读应答数据
	nLength = ReadPort(ans, 128);   
	ans[nLength] = '\0';
	sprintf( szTemp, "发送的应答：%s\n\0", ans );
	WriteLog( szTemp );

	Sleep(ShortSleepTime);

	return TRUE;

	*/



    nPduLength = gsmEncodePdu(pSrc, pdu);    // 根据PDU参数，编码PDU串
        
    gsmString2Bytes(pdu, &nSmscLength, 2);    // 取PDU串中的SMSC信息长度
    nSmscLength++;        // 加上长度字节本身

	strcat(pdu, "\x01A\0");        // 以Ctrl-Z结束

    // 命令中的长度，不包括SMSC信息长度，以数据字节计
    sprintf(cmd, "AT+CMGS=%d\r", nPduLength / 2 - nSmscLength);    // 生成命令

	//strcpy( pdu, "0891683108200105F011000D91683181076159F6000800064F60597D0021\0" );
	//strcpy( cmd, "AT+CMGS=21\r" );

	char szTemp[1024] = {0};
	sprintf( szTemp, "发送信息命令：%s, 信息内容：%s\n\0", cmd, pdu );
	WriteLog( szTemp );

	//for( int i = 0; i != 6; i++ )
	{
		// 先输出命令串
        WritePort(cmd, static_cast<int>(strlen(cmd)));
		Sleep(100);

		// 输出信息
		WritePort(pdu, static_cast<int>(strlen(pdu)));
		//WritePort("\x1A", 1);
		Sleep(2000);

		// 读发送应答数据
		nLength = ReadPort(ans, 128);
		ans[nLength] = '\0';
		sprintf( szTemp, "发送的应答：%s\n\0", ans );
		WriteLog( szTemp );
	}

	 return true;
    

}

/////////////////////////////////////////////////////////////////////////////////
// 函数：gsmReadMessage                                                        //
// 说明：读取短消息，用+CMGL代替+CMGR，可一次性读出全部短消息                  //
// 参数：                                                                      //
//      pMsg: 短消息缓冲区，必须足够大                                         //
// 返回: 短消息条数                                                            //
/////////////////////////////////////////////////////////////////////////////////
int CSerialPort::gsmReadMessage(SM_PARAM* pMsg)
{
	int nLength;        // 串口收到的数据长度
    int nMsg;           // 短消息计数值
    char* ptr;          // 内部用的数据指针
	char *ptr1;
    char cmd[16];       // 命令串
    char ans[1024];     // 应答串
   
    nMsg = 0;
    ptr = ans;

	sprintf(cmd, "AT+CMGR=1\r");    // 生成命令
    
    WritePort(cmd, static_cast<int>(strlen(cmd)));    // 输出命令串
	Sleep(ShortSleepTime);
    nLength = ReadPort(ans, 1024);    // 读应答数据
    // 根据能否找到"+CMS ERROR"决定成功与否
	ans[nLength]='\0';

	if(nLength > 0 && strncmp(ans, "+CMGR:", 6) != 0)
    {
        // 循环读取每一条短消息, 以"+CMGL:"开头
        pMsg->index=1;
        ptr = strstr(ans, "\r\n089168");
        ptr += 2;        // 跳过"\r\n"
        ptr1=ptr;
        for(int ii=0;;ii++)
        {
            ptr1=ptr1+1;
            if(*ptr1=='\r'&&*(ptr1+1)=='\n'&&*(ptr1+2)=='\r'&&*(ptr1+3)=='\n')
            {
                *ptr1=0x0;
                break;
            }
        }     
        gsmDecodePdu(ptr, pMsg);    // PDU串解码
        nMsg++;        // 短消息计数加1        
    }

    sprintf(cmd, "AT+CMGD=1,4\r");    // 生成命令
    
    WritePort(cmd, static_cast<int>(strlen(cmd)));    // 输出命令串
	Sleep(ShortSleepTime);
    return nMsg;
}


int CSerialPort::InitPort(CString strComName)
{
    hSerialPort = OpenPort(strComName, CBR_9600,NOPARITY,8, ONESTOPBIT);   
    if(hSerialPort==NULL)
    {
		//MessageBox(NULL, "打开指定串口失败，请重新选择串口。","失败", MB_OK|MB_ICONWARNING);
        return -1;
    }
	CString cmd = "AT+CSCA?\r";
	char ans[128] ;
	WritePort(cmd.GetBuffer(0),static_cast<int>(strlen(cmd)));
	Sleep(ShortSleepTime);
	ReadPort(ans,128);
	CString ansStr = ans;
	int cou;
	if((cou=ansStr.Find("+86")) != -1)
	{
	    m_SmsCenterNum = ansStr.Mid(cou+1,13);	
	}
	else
	{
		return -2;
	}
    return 0;
}

void CSerialPort::CloseCom()
{
	if(hSerialPort != NULL)
	{
		ClosePort(hSerialPort);//关闭端口
	}
}
