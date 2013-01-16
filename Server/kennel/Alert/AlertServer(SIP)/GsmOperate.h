// GsmOperate.h: interface for the CGsmOperate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GSMOPERATE_H__782220F8_0498_424C_81E9_33D96928DEFF__INCLUDED_)
#define AFX_GSMOPERATE_H__782220F8_0498_424C_81E9_33D96928DEFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define GSM_7BIT                        BYTE(0x0)
#define GSM_8BIT                        BYTE(0x4)
#define GSM_UCS2                        BYTE(0x8)

//#include "Alert.h"

class CGsmOperate //: public CAlert
{
public:
	struct SM_PARAM
	{
        char SCA[16];       // 短消息服务中心号码(SMSC地址)
        char TPA[16];       // 目标号码或回复号码(TP-DA或TP-RA)
        char TP_PID;        // 用户信息协议标识(TP-PID)
        char TP_DCS;        // 用户信息编码方式(TP-DCS)
        char TP_SCTS[16];   // 服务时间戳字符串(TP_SCTS), 接收时用到
        char TP_UD[161];    // 原始用户信息(编码前或解码后的TP-UD)
        char index;         // 短消息序号，在读取时用到
	} ;
	CGsmOperate();
	virtual ~CGsmOperate();	
	int gsmDecode8bit(const unsigned char *pSrc, char *pDst, int nSrcLength);
	int gsmEncode8bit(const char *pSrc, unsigned char *pDst, int nSrcLength);
	int gsmDecodeUcs2(const unsigned char* pSrc, char* pDst, int nSrcLength);
	int gsmEncodeUcs2(const char* pSrc, unsigned char* pDst, int nSrcLength);
	int gsmDecode7bit(const unsigned char* pSrc, char* pDst, int nSrcLength);
	int gsmEncode7bit(const char* pSrc, unsigned char* pDst, int nSrcLength);
	int gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength);
	int gsmBytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength);
	int gsmDecodePdu(const char* pSrc, SM_PARAM* pDst);
	int gsmEncodePdu(const SM_PARAM* pSrc, char* pDst);
	int gsmSerializeNumbers(const char* pSrc, char* pDst, int nSrcLength);
    int gsmInvertNumbers(const char* pSrc, char* pDst, int nSrcLength);
	struct SM_PARAM    *sm_param_temp;
	struct SM_PARAM    sm_param;

    CString m_strSerialPort;
	CString m_strMsgContent;
	CString m_strRecvPhone;
};

#endif // !defined(AFX_GSMOPERATE_H__782220F8_0498_424C_81E9_33D96928DEFF__INCLUDED_)
