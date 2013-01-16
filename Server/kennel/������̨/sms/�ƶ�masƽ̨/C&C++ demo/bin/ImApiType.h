/**
*   File Name           ��	IMApiType.h
*   Version             ��	2.0
*   Project Information :	IMAPI
*   Author              ��	wangxinguo
*   Brief Description   ��	IM�ƶ���Ϣ��API��̬�� ��������������ļ�
*======================================================
* Revision History      ��
* 1.0  2004-06-08	wangxinguo created
*/


#ifndef IM_API_TYPE_H_
#define IM_API_TYPE_H_


/***********************************
 *   ��������
 ***********************************/
#define SM_ID_LEN			    8		// ����ID����󳤶�(0-99999999)
#define SM_MOBILE_LEN		    16		// �ֻ�������󳤶� 
#define SM_CONTEXT_LEN		    2000	// ����������󳤶�   
#define SM_RPT_LEN				100		// ���Ż�ִ��������󳤶�   
#define TIME_LEN                40      // ʱ�䳤��
#define SM_TYPE_PDU				2       // �������ͣ�2��PDU����
#define SM_TYPE_NORMAL			0		// �������� 0:��ͨ����

typedef enum
{
	IMAPI_SUCC				 = 0,   	// �����ɹ�
	IMAPI_CONN_ERR			 = -1,	    // �������ݿ����
	IMAPI_CONN_CLOSE_ERR	 = -2,	    // ���ݿ�ر�ʧ��
	IMAPI_INS_ERR			 = -3,	    // ���ݿ�������
	IMAPI_DEL_ERR			 = -4,	    // ���ݿ�ɾ������
	IMAPI_QUERY_ERR			 = -5,	    // ���ݿ��ѯ����  
	IMAPI_DATA_ERR			 = -6,	    // ��������
	IMAPI_API_ERR			 = -7,		// API��ʶ�Ƿ�
	IMAPI_DATA_TOOLONG		 = -8, 		// ��Ϣ����̫��
	IMAPI_INIT_ERR			 = -9,	    // û�г�ʼ�����ʼ��ʧ��
	IMAPI_IFSTATUS_INVALID	 = -10,		// API�ӿڴ�����ͣ��ʧЧ��״̬
	IMAPI_GATEWAY_CONN_ERR	 = -11		// ��������δ����
};



/***********************************
 *   �ṹ�嶨��
 ***********************************/

// MO������Ϣ�ṹ��
typedef struct MOItem
{
    char	mobile[SM_MOBILE_LEN];			// ����MO���ֻ�����	    
    char	smContent[SM_CONTEXT_LEN];		// MO�Ķ�������	
    long	smId;		                    // MO����ΨһID�Ż�������
	char    moTime[TIME_LEN];               // MO����ʱ��
	int     msgFmt;							// ���ŵı����ʽ
}MO_Item;

// MT���ŵĻ�ִ��Ϣ�ṹ��
typedef struct RptItem
{
	char	mobile[SM_MOBILE_LEN];			// MT���͵��ֻ�����	    
	long	smId;		                    // MT����ΨһID�Ż�������
	int		rptId;							// MT�Ļ�ִ����
	char	rptDesc[SM_RPT_LEN];			// MT�Ļ�ִ��������Ϣ
	char    rptTime[TIME_LEN];              // MT�Ļ�ִʱ�䣺2006-12-28 12:00:00
}Rpt_Item;	
 

#endif /*IM_API_TYPE_H_*/
