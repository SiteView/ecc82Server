
#include "DllCall.h"


CDllCall::CDllCall(void)
{
}

CDllCall::~CDllCall(void)
{
}

//初始化License文件
/*
bool CDllCall::InitLic(const LICENSEDATA& srcData)
{
	return InitLicense(&srcData) == 1;
}
*/

bool CDllCall::GetLicString(const char* fileName, char* szLic)
{
	return ::GetLicString(fileName,szLic) == true;	
}


// 从数据生成License
/*bool CDllCall::GenLic(LICENSEDATA srcData)
{
	return CreateLicenseFile(srcData.ProductName,srcData) == 1;
}*/

bool CDllCall::GetOldLic(const char* fileName, LICENSEDATA* licData)
{
	FILE* finput;
	finput = fopen(fileName, "rb");
	if(!finput)
	{
		return false;
	}
	fseek(finput, 0, SEEK_END);
	long lFileLen = ftell(finput); 
	if(lFileLen <= 64)
	{
		fclose(finput);
		return false;
	}
	fseek(finput, 0, SEEK_SET);
	unsigned char* inBuff = new unsigned char[lFileLen+1];
	memset(inBuff, 0, lFileLen+1);
	fread(inBuff, 1, lFileLen, finput);
	fclose(finput);
	return Decrypt2Data((char*)inBuff, licData);
}

// 从加密串中解密出原始数据
bool CDllCall::Decrypt2Data(const char* srcStr, LICENSEDATA* dstData)
{	
	return ::Decrypt2Data(srcStr, dstData)  == true;
}
int CDllCall::IsValidLic(const char* module, const char* licstr, long daltTime, bool bVerify)
{
	return ::IsValidLicense(module, licstr, daltTime, bVerify);
}

bool CDllCall::IncreaseTime(const char* licstr, long daltTime)
{
	return ::IncreaseTime(licstr, daltTime) == true;
}

bool CDllCall::GetLicPDNumber(const char* licstr, int* points, int* devices)
{	
	return ::GetLicData(licstr, points, devices) == true;
}

int CDllCall::IsValidLicFile(const char* fileName, const char* module, const char* licstr, long daltTime, bool bVerify)
{
	return ::IsValidLicenseFile(fileName, module, licstr, daltTime, bVerify);
}

bool CDllCall::IncrsTimeFile(const char* fileName, const char* licstr, long daltTime)
{
	return ::IncreaseTimeFile(fileName,licstr, daltTime) == true ;
}

bool CDllCall::IsValidFileKey(const char* filename)
{
	return ::IsValidFileKey(filename) == true;
}
