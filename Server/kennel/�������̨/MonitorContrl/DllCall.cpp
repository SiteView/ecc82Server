
#include "DllCall.h"

#include "md5.h"

CDllCall::CDllCall(void)
{
}

CDllCall::~CDllCall(void)
{
}

bool CDllCall::GetLicString(const char* fileName, char* szLic)
{
	FILE* finput;
	finput = fopen(fileName, "rb");
	if(!finput)
	{
		//MessageBox("不能打开原来的License文件!");
		return false;
	}
	fseek(finput, 0, SEEK_END);
	long lFileLen = ftell(finput); 
	if(lFileLen <= 64)
	{
		//MessageBox("License文件内容已经被破坏!");
		fclose(finput);
		return false;
	}
	fseek(finput, 0, SEEK_SET);
	unsigned char* inBuff = new unsigned char[lFileLen+1];
	memset(inBuff, 0, lFileLen+1);
	fread(inBuff, 1, lFileLen, finput);
	fclose(finput);

	strcpy(szLic, (char*)inBuff);

	//vector<string> items = tokenize(string((char*)inBuff), ";", true);
	////item0 = endata; item1 = compentname, item2=md1, item3= rsacipher, item4=dkey, item5=nkey
	//if(items.size() != 6)
	//{
	//	return false;
	//}

	////检验MD数据的完整性(RSA Decrypt
	//char md[5]={0}, mdLic[33] = {0};
	//strcpy((char*)&mdLic, items[2].c_str());
	//SvLic::LicFuncs::RsaDecrypt(items[4].c_str(), items[5].c_str(), items[3].c_str(), (char*)&md);
	//for(int i = 0; i < 4; i++)
	//{
	//	mdLic[i+28] = md[i];
	//}

	////校验MD数据
	//char myMD[33] = {0};
	//MDFile(items[1].c_str(), (char*)&myMD);
	//if(strcmp(mdLic, myMD) != 0)
	//{
	//	//MessageBox("控制组件内容不正确!");
	//	return false;
	//}

	//if(NULL == items[0].c_str())
	//{
	//	return false;
	//}
	//else
	//{
	//	strcpy(szLic, items[0].c_str());
	//}

	return true;
}



// 从加密串中解密出原始数据
bool CDllCall::Decrypt2Data(const char* srcStr, LICENSEDATA* dstData)
{
	return SvLic::LicFuncs::Decrypt2Data(srcStr, dstData);
}

// GetMD5Msg: Converts a completed md5 digest into a char* string.
char* CDllCall::GetMD5Msg(unsigned char md5Digest[16])
{
	char chBuffer[256];
	char chEach[10];
	int nCount;
	memset(chBuffer,0,256);
	memset(chEach, 0, 10);
	for (nCount = 0; nCount < 16; nCount++)
	{
		sprintf(chEach, "%02x", md5Digest[nCount]);
		strncat(chBuffer, chEach, sizeof(chEach));
	}
	return strdup(chBuffer);
}


// 从字符串生成MD5摘要
bool CDllCall::MDString(const char* srcStr, char* res)
{
	//call dll 
	//ProcMD5Str(srcStr, res);
	//

	//call md5 class
	int nLen = strlen(srcStr);
	md5 alg;
	alg.Update((unsigned char*)srcStr, (unsigned int)nLen);
	alg.Finalize();
	strcpy(res, GetMD5Msg(alg.Digest()));
	return true;
}

// 从文件生成MD5摘要
bool CDllCall::MDFile(const char* filename, char* res)
{
	//ProcMD5File(filename, res);

	FILE* file;
	md5 alg;
	int nLen;
	unsigned char chBuffer[20480];
	try
	{
		memset(chBuffer, 0, 20480);
		if ((file = fopen (filename, "rb")) != NULL)
		{
			while (nLen = fread (chBuffer, 1, 20480, file))
				alg.Update(chBuffer, nLen);
			alg.Finalize();
			fclose (file);
			strcpy(res, GetMD5Msg(alg.Digest()));
			return true;
		}
	}
	catch(...)
	{
	}
	return false;
}

int CDllCall::IsValidLic(const char* module, const char* licstr, long daltTime, bool bVerify)
{
	return SvLic::LicFuncs::IsValidLicense(module, licstr, daltTime, bVerify);
}

bool CDllCall::IncreaseTime(const char* licstr, long daltTime)
{
	return SvLic::LicFuncs::IncreaseTime(licstr, daltTime);
}

int CDllCall::IsValidLicFile(const char* fileName, const char* module, const char* licstr, long daltTime, bool bVerify)
{
	return SvLic::LicFuncs::IsValidLicenseFile(fileName, module, licstr, daltTime, bVerify);
}

bool CDllCall::IncrsTimeFile(const char* fileName, const char* licstr, long daltTime)
{
	return SvLic::LicFuncs::IncreaseTimeFile(fileName,licstr, daltTime);
}

bool CDllCall::GetLicPDNumber(const char* licstr, int* points, int* devices)
{
	return SvLic::LicFuncs::GetLicData(licstr, points, devices);
}
