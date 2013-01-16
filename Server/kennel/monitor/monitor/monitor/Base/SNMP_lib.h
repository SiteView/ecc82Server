//SNMP LIB

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <string>

using namespace std;

#include "snmp_pp.h"

#pragma warning (disable:4098)

//定义常量
#define OID_TABLE_VARIABLE 2 //表格变量
#define OID_SIMPLE_VARIABLE 1//简单变量

#define SNMP_VERSION_C1 1//SNMP版本1
#define SNMP_VERSION_C2 2//SNMP版本2
#define SNMP_VERSION_C3 3//SNMP版本3

#define MAX_BUFF_LEN 1024//最大长度
//指向结果列表
typedef struct _OIDResult
{
	char chOID[MAX_BUFF_LEN];//OID
	char chIndex[MAX_BUFF_LEN];//索引
	char chValue[MAX_BUFF_LEN];//结果值
	//int nLen;//长度
	_OIDResult *pNext;//下一条
    public:
        _OIDResult()
        {
            pNext = NULL;
            //nLen = 0;
            memset(chOID, 0, MAX_BUFF_LEN);
            memset(chIndex, 0, MAX_BUFF_LEN);
            memset(chValue, 0, MAX_BUFF_LEN);
        };
}OIDResult;

//BasicSNMP类
class BasicSNMP
{
protected:
	int InitSNMPV3();//初始化SNMPV3
	int InitUTarget();//初始化UTarget
public:
	BasicSNMP();//构造函数
	~BasicSNMP();//析构函数
	int GetRequest();//Get请求
	int GetNextRequest();//Get Next请求
	int GetBulkRequest();//Get Bulk请求
	int SetRequest();//设置请求
	int InitSNMP();//初始化SNMP
	void SetAuthProtocol(char* chAuthProtocol);//
	void SetPrivProtocol(char* chPrivProtocol);//
	void SetCommunity(char* chCommunity);//设置共同体名称
	void SetIPAddress(char* chAddress);//设置IP地址
	void SetOID(char *chOID);//设置OID
	
	void SetOIDValue(int nValue){m_nOIDValue = nValue;}//设置OID

    void SetTimeout(const int &nTimeout) {timeout = nTimeout;};
	void SetUser(char* chUserName);//设置访问用户
	void SetPwd(char* chPassword);//设置访问密码
	void SetPrivatePwd(char* chPassword);//设置私有访问密码
	void SetVersion(int nVersion);//设置版本
	void SetNetworkPort(int nPort);//设置网络设备的端口
	void SetSecurityLevel(int nSecurityLevel);//设置访问优先级
	void SetSecurityMode(int nSecurityMode);//设置访问优先级
	const char* GetErrorMsg(int nErrCode);//当发生错误时候，通过此函数得到错误描述
	OIDResult* GetResultList();
private:
	void DestroyResultList();
	Snmp *pSnmp;//SNMP++ SNMPClass
	UdpAddress address;//NetworkSet IP Address
	Oid oid;//OID
	int m_nOIDValue;
	//共同体，私有访问密码，用户访问密码，访问用户
	OctetStr community, privPassword, authPassword, securityName,
		contextName,contextEngineID;
    int retries, timeout;// ，超时时间
	long m_nSecurityLevel;//访问优先级别,仅限于V3
	long m_nSecurityModel;
	u_short m_nPort;//网络设备的端口
    int m_nCount ;
	snmp_version version;//SNMP的版本
	OIDResult *pResult;//Result List
	char chErrMsg[MAX_BUFF_LEN], chOIDStart[MAX_BUFF_LEN];//Error Message
	long authProtocol;// = SNMPv3_usmNoAuthProtocol;
	long privProtocol;// = SNMPv3_usmNoPrivProtocol;
	CTarget ctarget;
	UTarget utarget;
};
//BasicSNMP定义结束

//ClsOID（实例标识）有简单变量、表格变量（字典式排序）两种形式。
class clsOID
{
public:
	//方法
	clsOID ();//构造函数
	clsOID (BasicSNMP objSNMP);//构造函数
	~clsOID ();//析构函数
	unsigned long Average();//取平均值，只有在OID是表格变量时有实际意义
	unsigned long Max();//取最大值，只有在OID是表格变量时有实际意义
	unsigned long Min();//取最小值，只有在OID是表格变量时有实际意义
	unsigned long IndexCount();
	__int64 Total();//取和值
	
	int GetResult ();//得到结果

	//add by mtx
	int SetResult();//设置结果

	void SetSOID (bool bSOID);//设置是否是简单变量
	void SetOID (const char* chOIDValue);//设置OID名称
	void SetOIDIndex(char* chIndex);//设置索引
	void SetOIDType(int nType);//设置类型
	void SetObjSNMP(BasicSNMP *objSNMP);//
	char* GetOID();//得到OID
	char* GetValue();//值
    const char* GetErrorMsg(int nErrCode);//
	OIDResult*  GetResultList();//

	//简单变量oid的值
	void SetSValue(int nValue);
	int GetSValue();
private:
    //变量
	OIDResult *pResultList;//结果列表头
	char chOIDIndex[MAX_BUFF_LEN]; //变量的索引，只有OID是表格变量才有作用
	
	char chOID[MAX_BUFF_LEN];//OID
	int	 m_nValue;
	//__int64 m_n64Total;

	int m_nType;//变量的类型 0 简单变量 1 表格变量
	BasicSNMP *pSNMP;//
public:
	char selname[256];	
protected:	
};
// 类clsOID定义结束

// 命名空间 SV_SNMP_LIB
// 说明：游龙科技SNMP库
namespace SV_SNMP_LIB
{
	void InitLib();//初始化
	void ReleaseLib();//释放
}
//命名空间定义结束

//文件结束
