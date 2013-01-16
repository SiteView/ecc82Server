// SerialPort.h: interface for the CSerialPort class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIALPORT_H__D244580C_F08C_4E25_B3BB_C407981A6DC6__INCLUDED_)
#define AFX_SERIALPORT_H__D244580C_F08C_4E25_B3BB_C407981A6DC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GsmOperate.h"

#define MaxSMSLen 80 //�����ų���

class CSerialPort : public CGsmOperate
{
public:
    enum
    {
        OpenPortFailed = -1,//�򿪶˿�ʧ��
        NoSetCenter = -2,//û�����ö�������
        ShortSleepTime = 500
    };
public:	
	void CloseCom();
	int InitPort(CString strComName);
	//��ȡ��������
	int ReadPort(void* pData, int nLength); 
	//�򴮿�д����
	BOOL WritePort(void* pData, int nLength);
	CSerialPort();
	virtual ~CSerialPort();
	void SetRecvPhone(CString strRecvPhone );//���ý����ֻ�����
	void SetMsgContent(CString strMsgContent);//���ö�������
	void SetSerialPortName(CString strPortName);//���ô�������
	//���Ͷ���
	int SendMsg(CString strRecvPhone ,CString strMsgContent, int nSMSMaxLength );

private:
	struct MsgList
	{
		char chMsg[200];
		MsgList *pNext;
	};
	BOOL ClosePort(HANDLE hPort);//�رմ��ж˿�
	//�򿪴��ж˿�
	HANDLE OpenPort(const char* pPort, int nBaudRate, int nParity, int nByteSize, int nStopBits);
/*	//��ȡ��������
	int ReadPort(void* pData, int nLength); 
	//�򴮿�д����
	BOOL WritePort(void* pData, int nLength);
*/
	//�������ݷ�ҳ
	int page( char *SrcMsg, MsgList *DestMsg, int nSMSMaxLength );
    bool gsmDeleteMessage(const int index);	
    bool gsmSendMessage(const SM_PARAM* pSrc);
	int gsmReadMessage(SM_PARAM* pMsg);
private:
	HANDLE hSerialPort;
	CString m_SmsCenterNum;
protected:
};

#endif // !defined(AFX_SERIALPORT_H__D244580C_F08C_4E25_B3BB_C407981A6DC6__INCLUDED_)
