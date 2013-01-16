
/* Copyright (C) 2010  dragonflow Inc.
 * All right reserved.
 */

#ifndef _LICENSE_H_
#define _LICENSE_H_


typedef struct
{
	char ProductName[33];//产品名称
	char Customer[200];//客户名称
	char UserName[200];//用户ID；（试用时为trial）[200]
	char Support[20];//技术支持
	char RegCode[1024];//注册码；（试用时为特定的编码）[50]
	int Devices;//设备数；[20]
	int Points;//监控点数；[30]
	char SysFeatures[2048];//用户系统特征码；（系统的安装时间、全部网卡的MAC地址的综合），以逗号隔开
	char StartDate[9];//开始试用日期；
	char MakeDate[9];//生成日期；
	int Delaydays;//允许试用天数；
	int DelayTimes;//当前延期次数；
	long TotalTime;//当前累积运行时间
	int Allowmachine;//允许更换机器
	int Isfinal; //是否正式版
	int Isdefault; //是否默认
	char Tokens[64];//tokens 
	char Modules[2048];//可用模块列表:以逗号隔开	
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
	// 从加密串中解密出原始数据
	extern  int Decrypt2Data(const char* srcStr, LICENSEDATA* dstData);
	// 生成默认的License文件
	//extern int InitLicense(const LICENSEDATA * srcData);
	// 从License数据创建License文件
	//extern int CreateLicenseFile(const char* filename, LICENSEDATA licData);
	// 从License文件读取License数据
	extern int GetLicense(LICENSEDATA licData, const char* licFile);
	extern int IsValidFileKey(const char* fileName);
#ifdef __cplusplus
}
#endif

#endif // !_LICENSE_H_

/* EOF */
