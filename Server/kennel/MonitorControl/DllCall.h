#include <stdio.h>
#include <string.h>

#include "license.h"


#pragma once

class CDllCall
{
public:
	CDllCall(void);
public:
	~CDllCall(void);
public:
	// 从加密串中解密出原始数据
	bool Decrypt2Data(const char* srcStr, LICENSEDATA* dstData);

	// 从数据生成License
	//bool GenLic(LICENSEDATA srcData);

	bool GetLicString(const char* fileName, char* szLic);
	bool GetOldLic(const char* fileName, LICENSEDATA* licData);


	int IsValidLic(const char* module, const char* licstr, long daltTime, bool bVerify);

	bool IncreaseTime(const char* , long);
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
	bool IsValidFileKey(const char* filename);

};
