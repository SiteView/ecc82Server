//###########################################################################
//
//       Module:  GSMH.H
//
//  Decriptions:  This is header file of RC-UMH's Win32 API module.
//
//
//  Copyright (C) 2004 SafeNet China Ltd. All Rights Reserved.
//
//###########################################################################

#ifndef _GSMH_H_
#define _GSMH_H_


#ifdef  __cplusplus
extern "C" {
#endif

// API要求调用者（即开发商应用程序）定义以下全局变量。
extern short int DogBytes,DogAddr;				//操作字节数（1~200）,首地址（0~199）
extern unsigned long DogPassword;				//口令，仅用于读写操作
extern unsigned long NewPassword;				//新口令
extern unsigned long DogResult;					//变换结果
extern void * DogData;							//输入输出数据指针
extern unsigned char DogCascade;				//级联码，此版本模块提供
												// 微狗的同号狗级联功能！
												//对USB狗，DogCascade的取值必须为0。

// API定义以下函数：
extern unsigned long DogCheck(void);			//查狗
extern unsigned long ReadDog(void);				// 读狗
extern unsigned long DogConvert(void);			//变换
extern unsigned long WriteDog(void);			//写狗
extern unsigned long DisableShare(void);		//禁止共享
extern unsigned long GetCurrentNo(void);		// 取流水号
extern unsigned long SetPassword(void);			//设置密码
extern unsigned long SetDogCascade(void);		// 设置级联码

#ifdef  __cplusplus
}
#endif
#endif //_GSMH_H_
  
