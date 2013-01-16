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
        char SCA[16];       // ����Ϣ�������ĺ���(SMSC��ַ)
        char TPA[16];       // Ŀ������ظ�����(TP-DA��TP-RA)
        char TP_PID;        // �û���ϢЭ���ʶ(TP-PID)
        char TP_DCS;        // �û���Ϣ���뷽ʽ(TP-DCS)
        char TP_SCTS[16];   // ����ʱ����ַ���(TP_SCTS), ����ʱ�õ�
        char TP_UD[161];    // ԭʼ�û���Ϣ(����ǰ�������TP-UD)
        char index;         // ����Ϣ��ţ��ڶ�ȡʱ�õ�
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
