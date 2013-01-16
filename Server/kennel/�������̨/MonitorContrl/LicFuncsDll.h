// LicFuncsDll.h

#undef AFX_DATA
#define AFX_DATA AFX_EXT_DATA

//#include "aes.h"
//#include "md5.h"
//#include "rsa.h"

//#include "sysinfo.h"

#include <string>
#include <vector>

#define EOF (-1)

using namespace std;


typedef basic_string<char>::size_type S_T;
static const S_T npos = -1;

typedef struct
{
	int Num;
	bool bChecked;
	char name[40];
	char shortName[20];
}MODULEITEM;

typedef struct
{
	char ProductName[33];//产品名称
	char CompenentName[33]; //明文的授权组件名称；（位于授权文件的前32个字节中）；[33]
	char UserName[200];//用户ID；（试用时为trial）[200]
	char Support[20];//技术支持
	char RegCode[51];//注册码；（试用时为特定的编码）[50]
	int Devices;//设备数；[20]
	int Points;//监控点数；[30]
	char SysFeatures[2048];//用户系统特征码；（系统的安装时间、全部网卡的MAC地址的综合），以逗号隔开
	char StartDate[9];//开始试用日期；
	int Delaydays;//允许试用天数；
	int DelayTimes;//当前延期次数；
	long TotalTime;//当前累积运行时间；
	char Modules[2048];//可用模块列表:以逗号隔开
	char Component_MD5[33];// 授权组件文件内容的MD5摘要；[32]
	char License_MD5[33];//授权文件的MD5摘要；[32]
	char dkey[1024];
	char ekey[1024];
	char nkey[1024];
}LICENSEDATA;




namespace SvLic
{
    class LicFuncs
    {
    public:
		// 从加密串中解密出原始数据
		static  bool Decrypt2Data(const char* srcStr, LICENSEDATA* dstData);

		static  char* PrintMD5(unsigned char  md5Digest[16]);
		static  bool MD5String(char* szString,char* res);
		static bool MD5File(char* szFilename, char* res);

		// 判定授权的有效性
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
		static  int IsValidLicense(const char* module, const char* licstr, long daltTime, bool bVerify);
		// 累加时间
		static bool IncreaseTime(const char* licstr, long daltTime);
		// 判定授权的有效性
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
		static  int IsValidLicenseFile(const char* fileName, const char* module, const char* licstr, long daltTime, bool bVerify);
		// 累加时间
		static bool IncreaseTimeFile(const char* fileName, const char* licstr, long daltTime);
		//获取授权点数和设备数
		static bool GetLicData(const char* licstr, int* points, int* devices);


	};

}

#undef AFX_DATA
#define AFX_DATA