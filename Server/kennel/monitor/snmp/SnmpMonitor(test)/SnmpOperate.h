

//#include "../../base/SNMP_lib.h"
#include <string>
#include <map>

using namespace std;

#pragma warning (disable:4098)

#include "snmplib.h"
#include "expression.h"

class CSnmpOperate  
{
    typedef struct _SNMPList
    {//OID
        _SNMPList()
        {
            memset(chOID, 0, MAX_BUFF_LEN);
            memset(chOIDIndex, 0, MAX_BUFF_LEN);
            memset(chHisValue, 0, MAX_BUFF_LEN);
            memset(chUnit, 0, MAX_BUFF_LEN);
            nType = 0;
            lHisTime = 0;
            nHasHistory = 0;
			bString = false;
			
        };

	    char chOID[MAX_BUFF_LEN];//OID
	    char chOIDIndex[MAX_BUFF_LEN];//OID索引
	    char chHisValue[MAX_BUFF_LEN];//历史数据
        char chUnit[MAX_BUFF_LEN];//时间单位
        int nHasHistory;//是否使用历史数据计算
		bool bString;// 是否为字符遍量
	    int nType;//OID变量类型
        long lHisTime;//上次采集时间
        unsigned long ulHisValueList[5];
        unsigned long ulHisTimeList[5];
    }SNMPList;

public:
    //构造函数（SMI文件，监测器ID，网络设备IP,
    //共同体
	CSnmpOperate(const char *pSMIFile, const char *pMonitorID, const char *pHostIP,	const char *pCommunity,
        const char * strIndex, const char* strSelValue, const int &nTPLID,
		const int &nPort, const int & nSnmpVer = 2, const int &nTimeout = 300);
    //构造函数
	CSnmpOperate();
    //析构函数
	virtual ~CSnmpOperate();
    //得到计算结果
	bool RetrieveData( CSVSnmpOID objOID, int i, CSVExpression& expression );
	bool GetResult(char *chReturn, int & nSize);
    //设置SMI文件路径
	void SetSMIFilePath(const char* chPath);
    //设置监测器ID
	void SetMonitorID(const char* pMomitorID);
    //设置网络设备IP地址
	void SetHostIP(const char* pHostIP);
    //设置网络设备访问共同体
	void SetCommunity(const char* pCommunity);
    //设置TPL ID
	void SetTplID(const int nTplID);
    //设置索引
	void SetIndex(const char *pIfIndex);
    //设置端口
	void SetPort(int nPort);
private:    
	char chResult[MAX_BUFF_LEN];//结果    
	char chSMIFile[MAX_BUFF_LEN];//SMI文件路径
	char chIfIndex[MAX_BUFF_LEN];//OID索引
	int  m_nTPLID;//TPL ID
	char chMonitorID[MAX_BUFF_LEN];//监测器ID
	char chHostIP[MAX_BUFF_LEN];//IP地址
	char chCommunity[MAX_BUFF_LEN];//共同体
	char chSelValue[MAX_BUFF_LEN];//wangpeng 选择匹配的指标
	int m_nPort;//端口
	int m_nExpCount;//计算式总数
	int m_nOidCount;//oid总数
	int m_nTimeOut;
	int m_nRetrys;
    int m_nSnmpVersion;
	//BasicSNMP objSNMP;//SNMP基础类
	//clsOID clsoid;//oid类
	//Function List
	int InitSNMP(CSVSnmpOID &objOID);	//初始化SNMP
	SNMPList *pOidList;// = new SNMPList;	//OID列表
    map<string, string, less<string> > m_mapExpList;
	//SNMPExpList *pExpList;// = new SNMPExpList;//计算式列表
	bool ReadConfig();//读取配置文件
};

std::string GetResourceValue(const string rkey);
std::string FuncGetStringFromIDS(const char * szIDS);
std::string FuncGetStringFromIDS(const char* szSection, const char * szIDS);
int FuncGetProfileIntBy(char * m_AppName, char * m_KeyName, char * m_FileName);
int GetPrivateProfileInt1(char * m_AppName, char * m_KeyName, int k, char * m_FileName );
int GetPrivateProfileString1(char * m_AppName, char * m_KeyName, char * m_default, char * m_Ret, int size, char * m_FileName);
std::string FuncGetProfileStringBy( char * m_AppName, char * m_KeyName, char * m_FileName);
//std::string FuncGetProfileString( char * m_AppName, char * m_KeyName, char * m_FileName);
std::string FuncGetInstallPath();
std::string FuncGetInstallRootPath();
int WritePrivateProfileString1( char * m_AppName, char * m_KeyName, char * m_Value, char * m_FileName);
static std::string  gRoot_path="";
//static char szSmi[1024]={0};