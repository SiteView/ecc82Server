// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
//��ı�׼�������� DLL �е������ļ��������������϶���� IDCSVAPI_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
//�κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ 
// IDCSVAPI_API ������Ϊ�ǴӴ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef IDCSVAPI_EXPORTS
#define IDCSVAPI_API __declspec(dllexport)
#else
#define IDCSVAPI_API __declspec(dllimport)
#endif

#include <iostream>
#include <String.h>
#include <list>

#define IDC_Version

using namespace std;
// �����Ǵ� idcsvapi.dll ������
class IDCSVAPI_API Cidcsvapi {
public:
	Cidcsvapi(void);
	// TODO: �ڴ�������ķ�����
};

extern IDCSVAPI_API int nidcsvapi;

IDCSVAPI_API int fnidcsvapi(void);

////////////////////////////////// ini API/////////////////////////////////////////////////

//ÿ��ini �ļ������ж�� section, ÿ��section �����ж���� key = value �������ݶ�
//ÿһ��key ���������ͣ� int��string, binary ,����д�������Ӧ����

IDCSVAPI_API
bool GetIniFileSections_2(std::list<string> &sectionlist,string filename,string addr="localhost",string user="default");
//��ȡĳ��ini�ļ������е� section  ��//���� section��list ���ã� ini�ļ�����SVDB ��ַ�� idc�û�id

IDCSVAPI_API
bool GetIniFileKeys_2(string section,std::list<string> &keylist,string filename,string addr="localhost",string user="default");
//��ȡĳ��section �µ����е�key

IDCSVAPI_API
int	GetIniFileValueType_2(string section,string key,string filename,string addr="localhost",string user="default");
//��ȡĳ�� key ������

//// ������GetIniFileValueType���ܷ��ص�ֵ
//#define INIVALUE_FAILED	-1			//��ȡʧ��
//#define INIVALUE_NULL	0			//δ֪����
//#define INIVALUE_INT	1			//��������
//#define	INIVALUE_STRING	2			//�ַ���������
//#define INIVALUE_BINARY	3			//������������



// ����key������дini��3������ ��// section����   key ����value ֵ��  ini�ļ�����SVDB ��ַ�� idc�û�id
IDCSVAPI_API
bool WriteIniFileString_2(string section,string key,string str,string filename,string addr="localhost",string user="default");

IDCSVAPI_API
bool WriteIniFileInt_2(string section,string key,int value,string filename,string addr="localhost",string user="default");

IDCSVAPI_API
bool WriteIniFileStruct_2(string section,string key, void *data,unsigned int len,string filename,string addr="localhost",string user="default");


// ����key�����Ͷ�ini��3��������            // Ĭ�Ϸ���ֵ������裩
IDCSVAPI_API
string GetIniFileString_2(string section,string key,string defaultret,string filename,string addr="localhost",string user="default");

IDCSVAPI_API
int	GetIniFileInt_2(string section,string key,int defaultret,string filename,string addr="localhost",string user="default");

IDCSVAPI_API
bool GetIniFileStruct_2(string section,string key,void *buf,unsigned int &len,string filename,string addr="localhost",string user="default");


IDCSVAPI_API
bool DeleteIniFileSection_2(string section,string filename,string addr="localhost",string user="default");
//ɾ��ĳ�� section

IDCSVAPI_API
bool DeleteIniFileKey_2(string section,string key,string filename,string addr="localhost",string user="default");
//ɾ��ĳ�� key


IDCSVAPI_API
bool EditIniFileSection_2(string oldsection,string newsection,string filename,string addr="localhost",string user="default");
//�޸�ĳ�� section

IDCSVAPI_API
bool EditIniFileKey_2(string section,string oldkey,string newkey,string filename,string addr="localhost",string user="default");
//�޸�ĳ�� key
