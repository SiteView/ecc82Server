// 下列 ifdef 块是创建使从 DLL 导出更简单的
//宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 IDCSVAPI_EXPORTS
// 符号编译的。在使用此 DLL 的
//任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将 
// IDCSVAPI_API 函数视为是从此 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
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
// 此类是从 idcsvapi.dll 导出的
class IDCSVAPI_API Cidcsvapi {
public:
	Cidcsvapi(void);
	// TODO: 在此添加您的方法。
};

extern IDCSVAPI_API int nidcsvapi;

IDCSVAPI_API int fnidcsvapi(void);

////////////////////////////////// ini API/////////////////////////////////////////////////

//每个ini 文件可以有多个 section, 每个section 可以有多个“ key = value ”的数据对
//每一个key 有三种类型： int，string, binary ,读或写须调用相应函数

IDCSVAPI_API
bool GetIniFileSections_2(std::list<string> &sectionlist,string filename,string addr="localhost",string user="default");
//获取某个ini文件的所有的 section  ，//传出 section的list 引用， ini文件名，SVDB 地址， idc用户id

IDCSVAPI_API
bool GetIniFileKeys_2(string section,std::list<string> &keylist,string filename,string addr="localhost",string user="default");
//获取某个section 下的所有的key

IDCSVAPI_API
int	GetIniFileValueType_2(string section,string key,string filename,string addr="localhost",string user="default");
//获取某个 key 的类型

//// 下面是GetIniFileValueType可能返回的值
//#define INIVALUE_FAILED	-1			//获取失败
//#define INIVALUE_NULL	0			//未知类型
//#define INIVALUE_INT	1			//整型数据
//#define	INIVALUE_STRING	2			//字符串型数据
//#define INIVALUE_BINARY	3			//二进类型数据



// 根据key的类型写ini的3个函数 ：// section名，   key 名，value 值，  ini文件名，SVDB 地址， idc用户id
IDCSVAPI_API
bool WriteIniFileString_2(string section,string key,string str,string filename,string addr="localhost",string user="default");

IDCSVAPI_API
bool WriteIniFileInt_2(string section,string key,int value,string filename,string addr="localhost",string user="default");

IDCSVAPI_API
bool WriteIniFileStruct_2(string section,string key, void *data,unsigned int len,string filename,string addr="localhost",string user="default");


// 根据key的类型读ini的3个函数：            // 默认返回值（随便设）
IDCSVAPI_API
string GetIniFileString_2(string section,string key,string defaultret,string filename,string addr="localhost",string user="default");

IDCSVAPI_API
int	GetIniFileInt_2(string section,string key,int defaultret,string filename,string addr="localhost",string user="default");

IDCSVAPI_API
bool GetIniFileStruct_2(string section,string key,void *buf,unsigned int &len,string filename,string addr="localhost",string user="default");


IDCSVAPI_API
bool DeleteIniFileSection_2(string section,string filename,string addr="localhost",string user="default");
//删除某个 section

IDCSVAPI_API
bool DeleteIniFileKey_2(string section,string key,string filename,string addr="localhost",string user="default");
//删除某个 key


IDCSVAPI_API
bool EditIniFileSection_2(string oldsection,string newsection,string filename,string addr="localhost",string user="default");
//修改某个 section

IDCSVAPI_API
bool EditIniFileKey_2(string section,string oldkey,string newkey,string filename,string addr="localhost",string user="default");
//修改某个 key
