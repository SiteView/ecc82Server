


#include <windows.h>

#include "../IODll/license.h"


#pragma once

class CDllCall
{
public:
	CDllCall(void);
public:
	~CDllCall(void);
public:
	bool Encrypt2Str(const LICENSEDATA& srcData, char* dest);
	// �Ӽ��ܴ��н��ܳ�ԭʼ����
	bool Decrypt2Data(const char* srcStr, LICENSEDATA* dstData);

	// GetMD5Msg: Converts a completed md5 digest into a char* string.
	char* GetMD5Msg(unsigned char md5Digest[16]);

	// ���ַ�������MD5ժҪ
	bool MDString(const char* srcStr, char* res);
	// ���ļ�����MD5ժҪ
	bool MDFile(const char* filename, char* res);
	//��ʼ��License�ļ�
	bool InitLic(const LICENSEDATA& srcData);
	// ����������License
	bool GenLic(LICENSEDATA srcData);

	bool GetLicString(const char* fileName, char* szLic);
	bool GetOldLic(const char* fileName, LICENSEDATA* licData);

	/* 
	 * 0 - success
	 * 10 - ģ���Ƿ����
	 * 20 - ���ڳ�����Ч��
	 * 30 - ����
	 * 40,50 - û��ȡ��ϵͳ����
	 * 60 - ϵͳ��������
	 * 70 - �ۼ�ʱ�䳬��
	 * 
	 */
	int IsValidLic(const char* module, const char* licstr, long daltTime, bool bVerify);

	bool IncreaseTime(const char* , long);

			/* 
		* 0 - success
		* 10 - ģ���Ƿ����
		* 20 - ���ڳ�����Ч��
		* 30 - ����
		* 40,50 - û��ȡ��ϵͳ����
		* 60 - ϵͳ��������
		* 70 - �ۼ�ʱ�䳬��
		* 
		*/
	int IsValidLicFile(const char* filename, const char* module, const char* licstr, long daltTime, bool bVerify);

	bool IncrsTimeFile(const char* filename, const char* licstr, long daltTime);


	//����Ȩ�ַ�����ȡ��Ȩ�ĵ������豸��
	bool GetLicPDNumber(const char* licstr, int* points, int* devices);

};
