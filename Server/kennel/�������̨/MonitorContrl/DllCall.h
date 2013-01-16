
#pragma once

#include <windows.h>


#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>   
#include <functional> 

using namespace std;


#include "licFuncsDll.h"

#pragma comment(lib,"IOdll.lib")

class CDllCall
{
public:
	CDllCall(void);
public:
	~CDllCall(void);
public:
	// �Ӽ��ܴ��н��ܳ�ԭʼ����
	bool Decrypt2Data(const char* srcStr, LICENSEDATA* dstData);

	// GetMD5Msg: Converts a completed md5 digest into a char* string.
	char* GetMD5Msg(unsigned char md5Digest[16]);

	// ���ַ�������MD5ժҪ
	bool MDString(const char* srcStr, char* res);
	// ���ļ�����MD5ժҪ
	bool MDFile(const char* filename, char* res);

	bool GetLicString(const char* fileName, char* szLic);

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

	bool IncreaseTime(const char* licstr, long daltTime);

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
