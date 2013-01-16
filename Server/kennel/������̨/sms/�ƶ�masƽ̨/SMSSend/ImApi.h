/**
*   File Name           ：	ImApi.h
*   Version             ：	2.0
*   Project Information :	IMAPI
*   Author              ：	wangxinguo
*   Brief Description   ：	IM移动信息机API动态库 主文件的头文件
*======================================================
* Revision History      ：
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
 *   API 函数定义
 ***********************************/

// 初始化接口
int __stdcall init(	const char* ip,                // 数据库机器ip
			        const char* username,          // 数据库用户名
			        const char* password,          // 数据库密码
			        const char* apiCode            // api接口编码
			      );

// 初始化接口并指定数据库
int __stdcall initWithDB( const char* ip,           // 数据库机器ip
			              const char* username,     // 数据库用户名
			              const char* password,     // 数据库密码
					      const char* db,			// 数据库名称
			              const char* apiCode       // api接口编码
			              );

// 释放接口
int __stdcall release();
 
// 发送消息
int __stdcall sendSM( const char* mobile,			// 目的手机号码
				      const char* content,		    // 短信内容
				      const long  smId			    // 短信ID
				      );

// 发送消息（可在手机上显示特定的流水号）
int __stdcall sendSMAdv( const char* mobile,		// 目的手机号码
				         const char* content,		// 短信内容
				         const long  smId,			// 短信ID
					     const long  srcId			// 终端源地址
				         );

// 发送消息（可在手机上显示特定的流水号）
int __stdcall sendSMAdvTime( const char* mobile,		// 目的手机号码
				         const char* content,		// 短信内容
						 const char* sendTime,		// 发送时间
				         const long  smId,			// 短信ID
					     const long  srcId			// 终端源地址						 
				         );

//发送Wap Push消息
int __stdcall sendWapPushSM( const char* mobile,	// 目的手机号码
	                         const char* content,	// 短信内容
	                         const long  smId,		// 短信ID
	                         const char* url		// Wap Push 短信的链接地址
					         );

//发送Wap Push消息
int __stdcall sendWapPushSMAdv( const char* mobile,	 // 目的手机号码
	                            const char* content, // 短信内容
	                            const long  smId,	 // 短信ID
					            const long  srcId,	 // 终端源地址
	                            const char* url		 // Wap Push 短信的链接地址
					            );

//发送Wap Push消息
int __stdcall sendWapPushSMAdvTime( const char* mobile,	 // 目的手机号码
									const char* content, // 短信内容
									const long  smId,	 // 短信ID
									const long  srcId,	 // 终端源地址
									const char* url,	 // Wap Push 短信的链接地址
									const char* sendTime // 发送时间
					            );

//发送PDU短信，不支持srcID的方法
int __stdcall sendPDU( const char* mobile, // 目的手机号码
					   const char* content, // 短信内容
					   const long smId,   // 短信ID
					   const int msgFmt,  // 特殊短信标识
					   const long tpPID,   // 网关指定，默认为0 
					   const long tpUdhi,  // 网关指定，默认为0 
					   const char* feeTerminalID, // 计费用户类型字段被计费用户的号码 
					   const char* feeType,  // 计费用户类型字段
					   const char* feeCode,  // 资费代码（以分为单位）
					   const int feeUserType // 资费类别
					   );

//发送PDU短信，支持srcID的方法
int __stdcall sendAdvPDU(const char* mobile, 
						 const char* content, 
						 const long smId, 
						 const long srcId, 
						 const int msgFmt, // 特殊短信标识
						 const long tpPID, 
						 const long tpUdhi, 
						 const char* feeTerminalID, 
						 const char* feeType, 
						 const char* feeCode, 
						 const int feeUserType
					   );

// 接收短信，返回查询到的短信数，并删除这些短信
int __stdcall receiveSM( MO_Item   retMO[],         // 查询结果数组
				         const int retSize          // 接收数目
				         );  


// 接收短信，返回查询到的短信数，并删除这些短信
int __stdcall receiveSMAdv( MO_Item   retMO[],			// 查询结果数组
						 const long srcID,				// 手机显示尾码
				         const int retSize				// 接收数目
				         );        
			        

// 接收回执，返回查询到的回执数，并删除这些回执
int __stdcall receiveRPT( Rpt_Item  retRpt[],       // 查询结果数组
				          const int retSize         // 接收数目
				          );  

// 接收回执，返回查询到的回执数，并删除这些回执
int __stdcall receiveRPTAdv( Rpt_Item  retRpt[],       // 查询结果数组
							 const long smID,		   // 短信smID
							 const int retSize         // 接收数目
				          );  

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*IM_API_H_*/

