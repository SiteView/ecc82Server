//SNMP LIB

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <string>

using namespace std;

#include "snmp_pp.h"

#pragma warning (disable:4098)

//���峣��
#define OID_TABLE_VARIABLE 2 //������
#define OID_SIMPLE_VARIABLE 1//�򵥱���

#define SNMP_VERSION_C1 1//SNMP�汾1
#define SNMP_VERSION_C2 2//SNMP�汾2
#define SNMP_VERSION_C3 3//SNMP�汾3

#define MAX_BUFF_LEN 1024//��󳤶�
//ָ�����б�
typedef struct _OIDResult
{
	char chOID[MAX_BUFF_LEN];//OID
	char chIndex[MAX_BUFF_LEN];//����
	char chValue[MAX_BUFF_LEN];//���ֵ
	//int nLen;//����
	_OIDResult *pNext;//��һ��
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

//BasicSNMP��
class BasicSNMP
{
protected:
	int InitSNMPV3();//��ʼ��SNMPV3
	int InitUTarget();//��ʼ��UTarget
public:
	BasicSNMP();//���캯��
	~BasicSNMP();//��������
	int GetRequest();//Get����
	int GetNextRequest();//Get Next����
	int GetBulkRequest();//Get Bulk����
	int SetRequest();//��������
	int InitSNMP();//��ʼ��SNMP
	void SetAuthProtocol(char* chAuthProtocol);//
	void SetPrivProtocol(char* chPrivProtocol);//
	void SetCommunity(char* chCommunity);//���ù�ͬ������
	void SetIPAddress(char* chAddress);//����IP��ַ
	void SetOID(char *chOID);//����OID
	
	void SetOIDValue(int nValue){m_nOIDValue = nValue;}//����OID

    void SetTimeout(const int &nTimeout) {timeout = nTimeout;};
	void SetUser(char* chUserName);//���÷����û�
	void SetPwd(char* chPassword);//���÷�������
	void SetPrivatePwd(char* chPassword);//����˽�з�������
	void SetVersion(int nVersion);//���ð汾
	void SetNetworkPort(int nPort);//���������豸�Ķ˿�
	void SetSecurityLevel(int nSecurityLevel);//���÷������ȼ�
	void SetSecurityMode(int nSecurityMode);//���÷������ȼ�
	const char* GetErrorMsg(int nErrCode);//����������ʱ��ͨ���˺����õ���������
	OIDResult* GetResultList();
private:
	void DestroyResultList();
	Snmp *pSnmp;//SNMP++ SNMPClass
	UdpAddress address;//NetworkSet IP Address
	Oid oid;//OID
	int m_nOIDValue;
	//��ͬ�壬˽�з������룬�û��������룬�����û�
	OctetStr community, privPassword, authPassword, securityName,
		contextName,contextEngineID;
    int retries, timeout;// ����ʱʱ��
	long m_nSecurityLevel;//�������ȼ���,������V3
	long m_nSecurityModel;
	u_short m_nPort;//�����豸�Ķ˿�
    int m_nCount ;
	snmp_version version;//SNMP�İ汾
	OIDResult *pResult;//Result List
	char chErrMsg[MAX_BUFF_LEN], chOIDStart[MAX_BUFF_LEN];//Error Message
	long authProtocol;// = SNMPv3_usmNoAuthProtocol;
	long privProtocol;// = SNMPv3_usmNoPrivProtocol;
	CTarget ctarget;
	UTarget utarget;
};
//BasicSNMP�������

//ClsOID��ʵ����ʶ���м򵥱��������������ֵ�ʽ����������ʽ��
class clsOID
{
public:
	//����
	clsOID ();//���캯��
	clsOID (BasicSNMP objSNMP);//���캯��
	~clsOID ();//��������
	unsigned long Average();//ȡƽ��ֵ��ֻ����OID�Ǳ�����ʱ��ʵ������
	unsigned long Max();//ȡ���ֵ��ֻ����OID�Ǳ�����ʱ��ʵ������
	unsigned long Min();//ȡ��Сֵ��ֻ����OID�Ǳ�����ʱ��ʵ������
	unsigned long IndexCount();
	__int64 Total();//ȡ��ֵ
	
	int GetResult ();//�õ����

	//add by mtx
	int SetResult();//���ý��

	void SetSOID (bool bSOID);//�����Ƿ��Ǽ򵥱���
	void SetOID (const char* chOIDValue);//����OID����
	void SetOIDIndex(char* chIndex);//��������
	void SetOIDType(int nType);//��������
	void SetObjSNMP(BasicSNMP *objSNMP);//
	char* GetOID();//�õ�OID
	char* GetValue();//ֵ
    const char* GetErrorMsg(int nErrCode);//
	OIDResult*  GetResultList();//

	//�򵥱���oid��ֵ
	void SetSValue(int nValue);
	int GetSValue();
private:
    //����
	OIDResult *pResultList;//����б�ͷ
	char chOIDIndex[MAX_BUFF_LEN]; //������������ֻ��OID�Ǳ�������������
	
	char chOID[MAX_BUFF_LEN];//OID
	int	 m_nValue;
	//__int64 m_n64Total;

	int m_nType;//���������� 0 �򵥱��� 1 ������
	BasicSNMP *pSNMP;//
public:
	char selname[256];	
protected:	
};
// ��clsOID�������

// �����ռ� SV_SNMP_LIB
// ˵���������Ƽ�SNMP��
namespace SV_SNMP_LIB
{
	void InitLib();//��ʼ��
	void ReleaseLib();//�ͷ�
}
//�����ռ䶨�����

//�ļ�����
