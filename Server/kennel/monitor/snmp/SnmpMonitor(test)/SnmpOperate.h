

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
	    char chOIDIndex[MAX_BUFF_LEN];//OID����
	    char chHisValue[MAX_BUFF_LEN];//��ʷ����
        char chUnit[MAX_BUFF_LEN];//ʱ�䵥λ
        int nHasHistory;//�Ƿ�ʹ����ʷ���ݼ���
		bool bString;// �Ƿ�Ϊ�ַ�����
	    int nType;//OID��������
        long lHisTime;//�ϴβɼ�ʱ��
        unsigned long ulHisValueList[5];
        unsigned long ulHisTimeList[5];
    }SNMPList;

public:
    //���캯����SMI�ļ��������ID�������豸IP,
    //��ͬ��
	CSnmpOperate(const char *pSMIFile, const char *pMonitorID, const char *pHostIP,	const char *pCommunity,
        const char * strIndex, const char* strSelValue, const int &nTPLID,
		const int &nPort, const int & nSnmpVer = 2, const int &nTimeout = 300);
    //���캯��
	CSnmpOperate();
    //��������
	virtual ~CSnmpOperate();
    //�õ�������
	bool RetrieveData( CSVSnmpOID objOID, int i, CSVExpression& expression );
	bool GetResult(char *chReturn, int & nSize);
    //����SMI�ļ�·��
	void SetSMIFilePath(const char* chPath);
    //���ü����ID
	void SetMonitorID(const char* pMomitorID);
    //���������豸IP��ַ
	void SetHostIP(const char* pHostIP);
    //���������豸���ʹ�ͬ��
	void SetCommunity(const char* pCommunity);
    //����TPL ID
	void SetTplID(const int nTplID);
    //��������
	void SetIndex(const char *pIfIndex);
    //���ö˿�
	void SetPort(int nPort);
private:    
	char chResult[MAX_BUFF_LEN];//���    
	char chSMIFile[MAX_BUFF_LEN];//SMI�ļ�·��
	char chIfIndex[MAX_BUFF_LEN];//OID����
	int  m_nTPLID;//TPL ID
	char chMonitorID[MAX_BUFF_LEN];//�����ID
	char chHostIP[MAX_BUFF_LEN];//IP��ַ
	char chCommunity[MAX_BUFF_LEN];//��ͬ��
	char chSelValue[MAX_BUFF_LEN];//wangpeng ѡ��ƥ���ָ��
	int m_nPort;//�˿�
	int m_nExpCount;//����ʽ����
	int m_nOidCount;//oid����
	int m_nTimeOut;
	int m_nRetrys;
    int m_nSnmpVersion;
	//BasicSNMP objSNMP;//SNMP������
	//clsOID clsoid;//oid��
	//Function List
	int InitSNMP(CSVSnmpOID &objOID);	//��ʼ��SNMP
	SNMPList *pOidList;// = new SNMPList;	//OID�б�
    map<string, string, less<string> > m_mapExpList;
	//SNMPExpList *pExpList;// = new SNMPExpList;//����ʽ�б�
	bool ReadConfig();//��ȡ�����ļ�
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