/**
*   File Name           ��	ImApi.h
*   Version             ��	2.0
*   Project Information :	IMAPI
*   Author              ��	wangxinguo
*   Brief Description   ��	IM�ƶ���Ϣ��API��̬�� ���ļ���ͷ�ļ�
*======================================================
* Revision History      ��
* 1.  2005-5-18	wangxinguo Created
*/


#ifndef IM_API_H_
#define IM_API_H_

#include <windows.h>
#include "ImApiType.h"

#ifdef __cplusplus
extern "C"{
#endif /*__cplusplus*/


// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the IMAPI_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// IMAPI_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#ifdef IMAPI_EXPORTS
#define __stdcall(dllexport)
#else
#define __stdcall(dllimport)
#endif /*IMAPI_EXPORTS*/


/***********************************
 *   API ��������
 ***********************************/

// ��ʼ���ӿ�
int __stdcall init(	const char* ip,                // ���ݿ����ip
			        const char* username,          // ���ݿ��û���
			        const char* password,          // ���ݿ�����
			        const char* apiCode            // api�ӿڱ���
			      );

// ��ʼ���ӿڲ�ָ�����ݿ�
int __stdcall initWithDB( const char* ip,           // ���ݿ����ip
			              const char* username,     // ���ݿ��û���
			              const char* password,     // ���ݿ�����
					      const char* db,			// ���ݿ�����
			              const char* apiCode       // api�ӿڱ���
			              );

// �ͷŽӿ�
int __stdcall release();
 
// ������Ϣ
int __stdcall sendSM( const char* mobile,			// Ŀ���ֻ�����
				      const char* content,		    // ��������
				      const long  smId			    // ����ID
				      );

// ������Ϣ�������ֻ�����ʾ�ض�����ˮ�ţ�
int __stdcall sendSMAdv( const char* mobile,		// Ŀ���ֻ�����
				         const char* content,		// ��������
				         const long  smId,			// ����ID
					     const long  srcId			// �ն�Դ��ַ
				         );

// ������Ϣ�������ֻ�����ʾ�ض�����ˮ�ţ�
int __stdcall sendSMAdvTime( const char* mobile,		// Ŀ���ֻ�����
				         const char* content,		// ��������
						 const char* sendTime,		// ����ʱ��
				         const long  smId,			// ����ID
					     const long  srcId			// �ն�Դ��ַ						 
				         );

//����Wap Push��Ϣ
int __stdcall sendWapPushSM( const char* mobile,	// Ŀ���ֻ�����
	                         const char* content,	// ��������
	                         const long  smId,		// ����ID
	                         const char* url		// Wap Push ���ŵ����ӵ�ַ
					         );

//����Wap Push��Ϣ
int __stdcall sendWapPushSMAdv( const char* mobile,	 // Ŀ���ֻ�����
	                            const char* content, // ��������
	                            const long  smId,	 // ����ID
					            const long  srcId,	 // �ն�Դ��ַ
	                            const char* url		 // Wap Push ���ŵ����ӵ�ַ
					            );

//����Wap Push��Ϣ
int __stdcall sendWapPushSMAdvTime( const char* mobile,	 // Ŀ���ֻ�����
									const char* content, // ��������
									const long  smId,	 // ����ID
									const long  srcId,	 // �ն�Դ��ַ
									const char* url,	 // Wap Push ���ŵ����ӵ�ַ
									const char* sendTime // ����ʱ��
					            );

//����PDU���ţ���֧��srcID�ķ���
int __stdcall sendPDU( const char* mobile, // Ŀ���ֻ�����
					   const char* content, // ��������
					   const long smId,   // ����ID
					   const int msgFmt,  // ������ű�ʶ
					   const long tpPID,   // ����ָ����Ĭ��Ϊ0 
					   const long tpUdhi,  // ����ָ����Ĭ��Ϊ0 
					   const char* feeTerminalID, // �Ʒ��û������ֶα��Ʒ��û��ĺ��� 
					   const char* feeType,  // �Ʒ��û������ֶ�
					   const char* feeCode,  // �ʷѴ��루�Է�Ϊ��λ��
					   const int feeUserType // �ʷ����
					   );

//����PDU���ţ�֧��srcID�ķ���
int __stdcall sendAdvPDU(const char* mobile, 
						 const char* content, 
						 const long smId, 
						 const long srcId, 
						 const int msgFmt, // ������ű�ʶ
						 const long tpPID, 
						 const long tpUdhi, 
						 const char* feeTerminalID, 
						 const char* feeType, 
						 const char* feeCode, 
						 const int feeUserType
					   );

// ���ն��ţ����ز�ѯ���Ķ���������ɾ����Щ����
int __stdcall receiveSM( MO_Item   retMO[],         // ��ѯ�������
				         const int retSize          // ������Ŀ
				         );  


// ���ն��ţ����ز�ѯ���Ķ���������ɾ����Щ����
int __stdcall receiveSMAdv( MO_Item   retMO[],			// ��ѯ�������
						 const long srcID,				// �ֻ���ʾβ��
				         const int retSize				// ������Ŀ
				         );        
			        

// ���ջ�ִ�����ز�ѯ���Ļ�ִ������ɾ����Щ��ִ
int __stdcall receiveRPT( Rpt_Item  retRpt[],       // ��ѯ�������
				          const int retSize         // ������Ŀ
				          );  

// ���ջ�ִ�����ز�ѯ���Ļ�ִ������ɾ����Щ��ִ
int __stdcall receiveRPTAdv( Rpt_Item  retRpt[],       // ��ѯ�������
							 const long smID,		   // ����smID
							 const int retSize         // ������Ŀ
				          );  

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*IM_API_H_*/

