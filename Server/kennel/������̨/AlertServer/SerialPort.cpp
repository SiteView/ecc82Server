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
// ������SetRecvPhone                                                          //
// ˵�������ý����ֻ�����                                                      //
// ������                                                                      //
//      strRecvPhone�������ֻ�����                                             //
/////////////////////////////////////////////////////////////////////////////////
void CSerialPort::SetRecvPhone(CString strRecvPhone)
{
    m_strRecvPhone = strRecvPhone;
}

/////////////////////////////////////////////////////////////////////////////////
// ������SetMsgContent                                                         //
// ˵�������ö�������                                                          //
// ������                                                                      //
//     strMsgContent�����ŵ�����                                               //
/////////////////////////////////////////////////////////////////////////////////
void CSerialPort::SetMsgContent(CString strMsgContent)
{
    m_strMsgContent = strMsgContent;
}

/////////////////////////////////////////////////////////////////////////////////
// ������SetSerialPortName                                                     //
// ˵����������Ҫ�򿪵Ĵ��ж˿�����                                            //
// ������                                                                      //
//      strPortName�����ж˿�����                                              //
/////////////////////////////////////////////////////////////////////////////////
void CSerialPort::SetSerialPortName(CString strPortName)
{
    m_strSerialPort = strPortName;
}

/////////////////////////////////////////////////////////////////////////////////
// ������SendMsg                                                               //
// ˵����ͨ��ָ���˿ڣ����Ͷ���                                                //
// ������                                                                      //
//      strRecvPhone�������ֻ�����                                             //
//      strMsgContent����������                                                //
//      strPortName�����ж˿ڵ�����                                            //
/////////////////////////////////////////////////////////////////////////////////
int CSerialPort::SendMsg( CString strRecvPhone , CString strMsgContent, int nSMSMaxLength )
{
    m_strRecvPhone = strRecvPhone;
    m_strMsgContent = strMsgContent;
    int i=0 , t=0;
    char ans[128];        // Ӧ��
    //char SCA[16];       // ����Ϣ�������ĺ���(SMSC��ַ)
    char TPA[16];       // Ŀ������ظ�����(TP-DA��TP-RA)
    //char TP_PID;        // �û���ϢЭ���ʶ(TP-PID)
    //char TP_DCS;        // �û���Ϣ���뷽ʽ(TP-DCS)
//    char TP_UD[161];    // ԭʼ�û���Ϣ(����ǰ�������TP-UD)
    char  cmd[20];
    MsgList *msglist = new MsgList;
	int iPage = page( strMsgContent.GetBuffer(strMsgContent.GetLength()), msglist, nSMSMaxLength );

	WriteErr( "�������ݣ�" );
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
		WriteErr( "���ţ�" );
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
        if(!gsmSendMessage(sm_param_temp))//���Ͷ���
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
// ������ClosePort                                                             //
// ˵�����ر��Ѿ��򿪵Ĵ��ж˿�                                                //
// ������                                                                      //
//      hPort���򿪵Ĵ��ڵľ��                                                //
/////////////////////////////////////////////////////////////////////////////////
BOOL CSerialPort::ClosePort(HANDLE hPort)
{
	return CloseHandle(hPort);
}

/////////////////////////////////////////////////////////////////////////////////
// ������OpenPort                                                              //
// ˵������ָ���Ĵ���                                                        //
// ������                                                                      //
//      pPort: �������ƻ��豸·��������"COM1"��"\\.\COM1"���ַ�ʽ�������ú���  //
//      nBaudRate: ������                                                      //
//      nParity: ��żУ��                                                      //
//      nByteSize: �����ֽڿ��                                                //
//      nStopBits: ֹͣλ                                                      //
// ����ֵ���򿪵Ĵ��ھ��                                                      //
/////////////////////////////////////////////////////////////////////////////////
HANDLE CSerialPort::OpenPort(const char* pPort, int nBaudRate,
							 int nParity, int nByteSize, int nStopBits)
{
    HANDLE  hPort;
    DCB dcb;        // ���ڿ��ƿ�
    COMMTIMEOUTS timeouts = {    // ���ڳ�ʱ���Ʋ���
        100,        // ���ַ������ʱʱ��: 100 ms
        1,          // ������ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms)
        500,        // ������(�����)����ʱʱ��: 500 ms
        1,          // д����ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms)
        100};       // ������(�����)д��ʱʱ��: 100 ms
    
    hPort = CreateFile(pPort,    // �������ƻ��豸·��
            GENERIC_READ | GENERIC_WRITE,    // ��д��ʽ
            0,               // ����ʽ����ռ
            NULL,            // Ĭ�ϵİ�ȫ������
            OPEN_EXISTING,   // ������ʽ
            0,               // ���������ļ�����
            NULL);           // �������ģ���ļ�
    
    if(hPort == INVALID_HANDLE_VALUE) 
    {
//        printf("Open Port Failed\n");
        return NULL;        // �򿪴���ʧ��
    }
    
    GetCommState(hPort, &dcb);        // ȡDCB
    
    dcb.BaudRate = nBaudRate;
    dcb.ByteSize = nByteSize;
    dcb.Parity = nParity;
    dcb.StopBits = nStopBits;
    
    SetCommState(hPort, &dcb);        // ����DCB    
    SetupComm(hPort, 4096, 1024);     // �������������������С    
    SetCommTimeouts(hPort, &timeouts);    // ���ó�ʱ
    
    return hPort;
}

/////////////////////////////////////////////////////////////////////////////////
// ������ReadPort                                                              //
// ˵����������                                                                //
// ������                                                                      //
//      pData: ���������ݻ�����ָ��                                            //
//      nLength: ������������ݳ���                                            //
// ����ֵ: ʵ�ʶ�������ݳ���                                                  //
/////////////////////////////////////////////////////////////////////////////////
int CSerialPort::ReadPort(void* pData, int nLength)
{
    DWORD dwNumRead;    // �����յ������ݳ���    
    ReadFile(hSerialPort, pData, (DWORD)nLength, &dwNumRead, NULL);    
    return (int)dwNumRead;
}

/////////////////////////////////////////////////////////////////////////////////
// ������WritePort                                                             //
// ˵����д����                                                                //
//      pData: ��д�����ݻ�����ָ��                                            //
//      nLength: ��д�����ݳ���                                                //
// ����ֵ���Ƿ�д�ɹ�                                                          //
/////////////////////////////////////////////////////////////////////////////////
BOOL CSerialPort::WritePort(void* pData, int nLength)
{
    DWORD dwNumWrite;    // ���ڷ��������ݳ���    
    return WriteFile(hSerialPort, pData, (DWORD)nLength, &dwNumWrite, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// ��������page                                                            //
// ���ܣ��������Ϣ������������Ϣ�ֳ���ҳ��                                //
// ������                                                                  //
//    char *SrcMsg;��ϢԴ��ַ��                                            //
//    char **DestMsg;���ڴ�ŷ�ҳ��Ķ���Ϣ��                              //
// ����ֵ��                                                                //
//    ���ض���Ϣҳ����                                                     //
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
    //��ǰҳ���ַ��ĸ���
    int num = 0;
    //��ǰ��ҳ��
    int nCur = 0;
    //��ҳ��
    int nPage = 1;
    //����Ϣ�ĳ���
    int nMsgLen = (int)strlen(SrcMsg);
	int flag=0;

    //��ҳ���ҳ��
    nPage = nMsgLen / nMax + 1;
    //��ҳ
	if(nPage==1) //ҳ��Ϊ1
    {
        strcpy(DestMsg->chMsg,SrcMsg);
		DestMsg->pNext = NULL;
    }
	else
	{
        //�ж϶��ŵĳ����Ƿ񳬹������ų���
		while(static_cast<int>(strlen(SrcMsg)) >= nMax)
		{
			flag=0;
			for(int i=0;i<nMax;i++)
			{
                //��ǰ�ַ��Ƿ������ֻ���Ӣ����ĸ
				if((unsigned char)SrcMsg[i]<(unsigned char)0x80) 
				{
					flag=flag+1;
				}
			}
			if(flag%2==1)//���Ӣ����ĸ�ǵ���
			{
                //���������ų���-1
				strncpy(msgTemp->chMsg,SrcMsg,nMax-1);
                //��ӽ�������
				msgTemp->chMsg[nMax-1]='\0';
                //ԭʼ�ַ���Ϊ��0��ʼ��ȥ�����ų���-1ʣ����ַ���
				strcpy(SrcMsg , SrcMsg+nMax-1);
			}
			else
			{
				strncpy(msgTemp->chMsg,SrcMsg,nMax-2);
				msgTemp->chMsg[nMax-2]='\0';	
				strcpy(SrcMsg , SrcMsg+nMax-2);
			}
			msgTemp->pNext = new MsgList;//�����б�
			msgTemp = msgTemp->pNext;//��ǰ����
		}
        //���ų���С�������ų��ȵĲ�����ӵ��б���
		strncpy(msgTemp->chMsg,SrcMsg,strlen(SrcMsg));
		msgTemp->chMsg[strlen(SrcMsg)]='\0';
	}
	//����ҳ��

    return nPage;
}

/////////////////////////////////////////////////////////////////////////////////
// ������gsmDeleteMessage                                                      //
// ˵����ɾ������Ϣ                                                            //
// ������                                                                      //
//      index: ����Ϣ��ţ���1��ʼ                                             //
/////////////////////////////////////////////////////////////////////////////////
bool CSerialPort::gsmDeleteMessage(const int index)
{
    int nLength;          // �����յ������ݳ���
    char cmd[16];         // ���
    char ans[128];        // Ӧ��
    
    sprintf(cmd, "AT+CMGD=%d\r", index);    // ��������
    
    // ������
    WritePort(cmd, static_cast<int>(strlen(cmd)));
    
    // ��Ӧ������
    nLength = ReadPort(ans, 128);
    
    // �����ܷ��ҵ�"+CMS ERROR"�����ɹ����
    if(nLength > 0 && strncmp(ans, "+CMS ERROR", 10) != 0)
    {
        return TRUE;
    }
    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////
// ������gsmSendMessage                                                        //
// ˵�������Ͷ���Ϣ                                                            //
// ������                                                                      //
//      pSrc: ԴPDU����ָ��                                                    //
/////////////////////////////////////////////////////////////////////////////////
bool CSerialPort::gsmSendMessage(const SM_PARAM* pSrc)
{
    int nPduLength;        // PDU������
    unsigned char nSmscLength;    // SMSC������
    int nLength;           // �����յ������ݳ���
	char cmd[16] = {0};          // ���
	char pdu[512] = {0};         // PDU��
	char ans[128] = {0};         // Ӧ��
 

	nPduLength = gsmEncodePdu(pSrc, pdu);    // ����PDU����������PDU��
    strcat(pdu, "\x01a");        // ��Ctrl-Z����
    
    gsmString2Bytes(pdu, &nSmscLength, 2);    // ȡPDU���е�SMSC��Ϣ����
    nSmscLength++;        // ���ϳ����ֽڱ���
    // �����еĳ��ȣ�������SMSC��Ϣ���ȣ��������ֽڼ�

    sprintf(cmd, "AT+CMGS=%d\r", nPduLength / 2 - nSmscLength);    // ��������


	/*
	char szTemp[1024] = {0};
	sprintf( szTemp, "������Ϣ���%s, ��Ϣ���ݣ�%s\n\0", cmd, pdu );
	WriteLog( szTemp );

	// ��������
    WritePort(cmd, strlen(cmd));    // ��������
	Sleep(ShortSleepTime);

	// �����Ϣ   	
	WritePort(pdu, strlen(pdu));
    WritePort("\x1A", 1);
    
	// ��Ӧ������
	nLength = ReadPort(ans, 128);   
	ans[nLength] = '\0';
	sprintf( szTemp, "���͵�Ӧ��%s\n\0", ans );
	WriteLog( szTemp );

	Sleep(ShortSleepTime);

	return TRUE;

	*/



    nPduLength = gsmEncodePdu(pSrc, pdu);    // ����PDU����������PDU��
        
    gsmString2Bytes(pdu, &nSmscLength, 2);    // ȡPDU���е�SMSC��Ϣ����
    nSmscLength++;        // ���ϳ����ֽڱ���

	strcat(pdu, "\x01A\0");        // ��Ctrl-Z����

    // �����еĳ��ȣ�������SMSC��Ϣ���ȣ��������ֽڼ�
    sprintf(cmd, "AT+CMGS=%d\r", nPduLength / 2 - nSmscLength);    // ��������

	//strcpy( pdu, "0891683108200105F011000D91683181076159F6000800064F60597D0021\0" );
	//strcpy( cmd, "AT+CMGS=21\r" );

	char szTemp[1024] = {0};
	sprintf( szTemp, "������Ϣ���%s, ��Ϣ���ݣ�%s\n\0", cmd, pdu );
	WriteLog( szTemp );

	//for( int i = 0; i != 6; i++ )
	{
		// ��������
        WritePort(cmd, static_cast<int>(strlen(cmd)));
		Sleep(100);

		// �����Ϣ
		WritePort(pdu, static_cast<int>(strlen(pdu)));
		//WritePort("\x1A", 1);
		Sleep(2000);

		// ������Ӧ������
		nLength = ReadPort(ans, 128);
		ans[nLength] = '\0';
		sprintf( szTemp, "���͵�Ӧ��%s\n\0", ans );
		WriteLog( szTemp );
	}

	 return true;
    

}

/////////////////////////////////////////////////////////////////////////////////
// ������gsmReadMessage                                                        //
// ˵������ȡ����Ϣ����+CMGL����+CMGR����һ���Զ���ȫ������Ϣ                  //
// ������                                                                      //
//      pMsg: ����Ϣ�������������㹻��                                         //
// ����: ����Ϣ����                                                            //
/////////////////////////////////////////////////////////////////////////////////
int CSerialPort::gsmReadMessage(SM_PARAM* pMsg)
{
	int nLength;        // �����յ������ݳ���
    int nMsg;           // ����Ϣ����ֵ
    char* ptr;          // �ڲ��õ�����ָ��
	char *ptr1;
    char cmd[16];       // ���
    char ans[1024];     // Ӧ��
   
    nMsg = 0;
    ptr = ans;

	sprintf(cmd, "AT+CMGR=1\r");    // ��������
    
    WritePort(cmd, static_cast<int>(strlen(cmd)));    // ������
	Sleep(ShortSleepTime);
    nLength = ReadPort(ans, 1024);    // ��Ӧ������
    // �����ܷ��ҵ�"+CMS ERROR"�����ɹ����
	ans[nLength]='\0';

	if(nLength > 0 && strncmp(ans, "+CMGR:", 6) != 0)
    {
        // ѭ����ȡÿһ������Ϣ, ��"+CMGL:"��ͷ
        pMsg->index=1;
        ptr = strstr(ans, "\r\n089168");
        ptr += 2;        // ����"\r\n"
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
        gsmDecodePdu(ptr, pMsg);    // PDU������
        nMsg++;        // ����Ϣ������1        
    }

    sprintf(cmd, "AT+CMGD=1,4\r");    // ��������
    
    WritePort(cmd, static_cast<int>(strlen(cmd)));    // ������
	Sleep(ShortSleepTime);
    return nMsg;
}


int CSerialPort::InitPort(CString strComName)
{
    hSerialPort = OpenPort(strComName, CBR_9600,NOPARITY,8, ONESTOPBIT);   
    if(hSerialPort==NULL)
    {
		//MessageBox(NULL, "��ָ������ʧ�ܣ�������ѡ�񴮿ڡ�","ʧ��", MB_OK|MB_ICONWARNING);
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
		ClosePort(hSerialPort);//�رն˿�
	}
}
