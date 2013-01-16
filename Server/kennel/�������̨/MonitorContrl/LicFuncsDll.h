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
	char ProductName[33];//��Ʒ����
	char CompenentName[33]; //���ĵ���Ȩ������ƣ���λ����Ȩ�ļ���ǰ32���ֽ��У���[33]
	char UserName[200];//�û�ID��������ʱΪtrial��[200]
	char Support[20];//����֧��
	char RegCode[51];//ע���룻������ʱΪ�ض��ı��룩[50]
	int Devices;//�豸����[20]
	int Points;//��ص�����[30]
	char SysFeatures[2048];//�û�ϵͳ�����룻��ϵͳ�İ�װʱ�䡢ȫ��������MAC��ַ���ۺϣ����Զ��Ÿ���
	char StartDate[9];//��ʼ�������ڣ�
	int Delaydays;//��������������
	int DelayTimes;//��ǰ���ڴ�����
	long TotalTime;//��ǰ�ۻ�����ʱ�䣻
	char Modules[2048];//����ģ���б�:�Զ��Ÿ���
	char Component_MD5[33];// ��Ȩ����ļ����ݵ�MD5ժҪ��[32]
	char License_MD5[33];//��Ȩ�ļ���MD5ժҪ��[32]
	char dkey[1024];
	char ekey[1024];
	char nkey[1024];
}LICENSEDATA;




namespace SvLic
{
    class LicFuncs
    {
    public:
		// �Ӽ��ܴ��н��ܳ�ԭʼ����
		static  bool Decrypt2Data(const char* srcStr, LICENSEDATA* dstData);

		static  char* PrintMD5(unsigned char  md5Digest[16]);
		static  bool MD5String(char* szString,char* res);
		static bool MD5File(char* szFilename, char* res);

		// �ж���Ȩ����Ч��
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
		static  int IsValidLicense(const char* module, const char* licstr, long daltTime, bool bVerify);
		// �ۼ�ʱ��
		static bool IncreaseTime(const char* licstr, long daltTime);
		// �ж���Ȩ����Ч��
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
		static  int IsValidLicenseFile(const char* fileName, const char* module, const char* licstr, long daltTime, bool bVerify);
		// �ۼ�ʱ��
		static bool IncreaseTimeFile(const char* fileName, const char* licstr, long daltTime);
		//��ȡ��Ȩ�������豸��
		static bool GetLicData(const char* licstr, int* points, int* devices);


	};

}

#undef AFX_DATA
#define AFX_DATA