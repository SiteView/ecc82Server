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

// APIҪ������ߣ���������Ӧ�ó��򣩶�������ȫ�ֱ�����
extern short int DogBytes,DogAddr;				//�����ֽ�����1~200��,�׵�ַ��0~199��
extern unsigned long DogPassword;				//��������ڶ�д����
extern unsigned long NewPassword;				//�¿���
extern unsigned long DogResult;					//�任���
extern void * DogData;							//�����������ָ��
extern unsigned char DogCascade;				//�����룬�˰汾ģ���ṩ
												// ΢����ͬ�Ź��������ܣ�
												//��USB����DogCascade��ȡֵ����Ϊ0��

// API�������º�����
extern unsigned long DogCheck(void);			//�鹷
extern unsigned long ReadDog(void);				// ����
extern unsigned long DogConvert(void);			//�任
extern unsigned long WriteDog(void);			//д��
extern unsigned long DisableShare(void);		//��ֹ����
extern unsigned long GetCurrentNo(void);		// ȡ��ˮ��
extern unsigned long SetPassword(void);			//��������
extern unsigned long SetDogCascade(void);		// ���ü�����

#ifdef  __cplusplus
}
#endif
#endif //_GSMH_H_
  
