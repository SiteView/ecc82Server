// SerialPort.h: interface for the CSerialPort class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIALPORT_H__D244580C_F08C_4E25_B3BB_C407981A6DC6__INCLUDED_)
#define AFX_SERIALPORT_H__D244580C_F08C_4E25_B3BB_C407981A6DC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GsmOperate.h"

#define MaxSMSLen 80 //最大短信长度

class CSerialPort : public CGsmOperate
{
public:
    enum
    {
        OpenPortFailed = -1,//打开端口失败
        NoSetCenter = -2,//没有设置短信中心
        ShortSleepTime = 500
    };
public:	
	void CloseCom();
	int InitPort(CString strComName);
	//读取串口数据
	int ReadPort(void* pData, int nLength); 
	//向串口写数据
	BOOL WritePort(void* pData, int nLength);
	CSerialPort();
	virtual ~CSerialPort();
	void SetRecvPhone(CString strRecvPhone );//设置接收手机号码
	void SetMsgContent(CString strMsgContent);//设置短信内容
	void SetSerialPortName(CString strPortName);//设置穿口名称
	//发送短信
	int SendMsg(CString strRecvPhone ,CString strMsgContent, int nSMSMaxLength );

private:
	struct MsgList
	{
		char chMsg[200];
		MsgList *pNext;
	};
	BOOL ClosePort(HANDLE hPort);//关闭串行端口
	//打开串行端口
	HANDLE OpenPort(const char* pPort, int nBaudRate, int nParity, int nByteSize, int nStopBits);
/*	//读取串口数据
	int ReadPort(void* pData, int nLength); 
	//向串口写数据
	BOOL WritePort(void* pData, int nLength);
*/
	//短信内容分页
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
