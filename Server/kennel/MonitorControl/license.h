
/* Copyright (C) 2010  dragonflow Inc.
 * All right reserved.
 */

#ifndef _LICENSE_H_
#define _LICENSE_H_


typedef struct
{
	char ProductName[33];//��Ʒ����
	char Customer[200];//�ͻ�����
	char UserName[200];//�û�ID��������ʱΪtrial��[200]
	char Support[20];//����֧��
	char RegCode[1024];//ע���룻������ʱΪ�ض��ı��룩[50]
	int Devices;//�豸����[20]
	int Points;//��ص�����[30]
	char SysFeatures[2048];//�û�ϵͳ�����룻��ϵͳ�İ�װʱ�䡢ȫ��������MAC��ַ���ۺϣ����Զ��Ÿ���
	char StartDate[9];//��ʼ�������ڣ�
	char MakeDate[9];//�������ڣ�
	int Delaydays;//��������������
	int DelayTimes;//��ǰ���ڴ�����
	long TotalTime;//��ǰ�ۻ�����ʱ��
	int Allowmachine;//�����������
	int Isfinal; //�Ƿ���ʽ��
	int Isdefault; //�Ƿ�Ĭ��
	char Tokens[64];//tokens 
	char Modules[2048];//����ģ���б�:�Զ��Ÿ���	
}LICENSEDATA;



#ifdef __cplusplus
extern "C" {
#endif
	extern int IsValidMachineCode(const char* MachineCode);
	extern int ChangeLicense(const char* machinecode, LICENSEDATA licData);		
	extern  int GetSystemFeature(char* feature);
	extern 	long des_decrypt(unsigned char *encData,long Length,unsigned char *plainText);
	extern 	long des_encrypt(unsigned char *plainText,long Length,unsigned char *encData);
	/*		
	extern 	int  isNumeric(const char* str);
	extern  char *strtrim(char *string);
	extern 	void chartohex(unsigned char  *a,int len,unsigned char  *b);
	extern 	int verifiylicense(unsigned char *lic);
	extern 	long des_decrypt(unsigned char *encData,long Length,unsigned char *plainText);
	extern 	long des_encrypt(unsigned char *plainText,long Length,unsigned char *encData);
	extern 	long des_lic_decrypt(unsigned char *encData,long Length,unsigned char *plainText);
	extern  long des_lic_encrypt(unsigned char *plainText,long Length,unsigned char *encData);
	extern  long des_hex_encrypt(unsigned char *plainText,long Length,unsigned char *encData);*/

	extern  int IncreaseTime(const char* licstr, long daltTime);
	extern  int IncreaseTimeFile(const char* fileName, const char* licstr, long daltTime);
	extern  int IsValidLicense(const char* module, const char* licstr, long daltTime, int bVerify);
	extern  int IsValidLicenseFile(const char* fileName, const char* module, const char* licstr, long daltTime, int bVerify);
	extern  int GetLicString(const char* fileName, char* szLic);
	extern  int GetLicData(const char* licstr, int* points, int* devices);
	// �Ӽ��ܴ��н��ܳ�ԭʼ����
	extern  int Decrypt2Data(const char* srcStr, LICENSEDATA* dstData);
	// ����Ĭ�ϵ�License�ļ�
	//extern int InitLicense(const LICENSEDATA * srcData);
	// ��License���ݴ���License�ļ�
	//extern int CreateLicenseFile(const char* filename, LICENSEDATA licData);
	// ��License�ļ���ȡLicense����
	extern int GetLicense(LICENSEDATA licData, const char* licFile);
	extern int IsValidFileKey(const char* fileName);
#ifdef __cplusplus
}
#endif

#endif // !_LICENSE_H_

/* EOF */
