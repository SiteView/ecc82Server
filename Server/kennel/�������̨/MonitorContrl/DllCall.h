
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
	// 从加密串中解密出原始数据
	bool Decrypt2Data(const char* srcStr, LICENSEDATA* dstData);

	// GetMD5Msg: Converts a completed md5 digest into a char* string.
	char* GetMD5Msg(unsigned char md5Digest[16]);

	// 从字符串生成MD5摘要
	bool MDString(const char* srcStr, char* res);
	// 从文件生成MD5摘要
	bool MDFile(const char* filename, char* res);

	bool GetLicString(const char* fileName, char* szLic);

			/* 
		* 0 - success
		* 10 - 模块是否可用
		* 20 - 日期超过有效期
		* 30 - 保留
		* 40,50 - 没有取得系统特征
		* 60 - 系统特征不对
		* 70 - 累计时间超标
		* 
		*/
	int IsValidLic(const char* module, const char* licstr, long daltTime, bool bVerify);

	bool IncreaseTime(const char* licstr, long daltTime);

			/* 
		* 0 - success
		* 10 - 模块是否可用
		* 20 - 日期超过有效期
		* 30 - 保留
		* 40,50 - 没有取得系统特征
		* 60 - 系统特征不对
		* 70 - 累计时间超标
		* 
		*/
	int IsValidLicFile(const char* filename, const char* module, const char* licstr, long daltTime, bool bVerify);

	bool IncrsTimeFile(const char* filename, const char* licstr, long daltTime);

	//从授权字符串获取授权的点数和设备数
	bool GetLicPDNumber(const char* licstr, int* points, int* devices);

};
