// SnmpOperate.cpp: implementation of the CSnmpOperate class.
//
// 
//////////////////////////////////////////////////////////////////////
// SnmpOperate.cpp: implementation of the CSnmpOperate class.
#include <fstream>
#include "SnmpOperate.h"
#include "expression.h"

#include "Time.h"

//#include "C:\siteview\Ecc_Common\Svdb\svapi\svapi.h"
//#include "C:\siteview\Ecc_Common\base\svapi.h"
#include <scsvapi\svapi.h>
//#include "../../kennel/svdb/svapi/svapi.h"
//#include "../../base/stlini.h"

#include <string>
using namespace std;

using namespace SV_ECC_SNMP_LIB;
using namespace svutil;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	16����ת����10����
unsigned __int64 OXTOD(string szValue)
{
	int iLen = 0;
	iLen = szValue.length() - 1;
	unsigned __int64 iReturn=0, iTemp=0;
	int i=0;
	for(iLen; iLen>=2; iLen--)
	{
		iTemp=1;
		string szSub = "";
		szSub = szValue.substr(iLen, 1);
		const char *ch = szSub.c_str();
		int iSub = 0;
		if((*ch>='A') && (*ch<='Z'))
		{
			switch(*ch)
			{
			case 'A':
				iSub = 10;
				break;
			case 'B':
				iSub = 11;
				break;
			case 'C':
				iSub = 12;
				break;
			case 'D':
				iSub = 13;
				break;
			case 'E':
				iSub = 14;
				break;
			case 'F':
				iSub = 15;
				break;
			default:
				break;
			}
		}
		else
		{
			iSub = atoi(szSub.c_str());
		}
		if(i==0)
		{
			iTemp = iSub;
		}
		else
		{
			for(int j=0; j<i; j++)
			{
				iTemp = iTemp * 16;
			}
			iTemp = iTemp * iSub;
		}
		iReturn += iTemp;
		i++;
	}
	return iReturn;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DumpLog(const char * pszFileName, const char *pBuffer, const int nLen)
{
    FILE *fp = fopen (pszFileName, "a+");
    if(fp)
    {
        fprintf(fp, "%s\n", svutil::TTime::GetCurrentTimeEx().Format().c_str());
        fprintf(fp, "%s", pBuffer);
        fprintf(fp, "\n---------------------------\n");
    }
    fclose(fp);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline char * strlwr(char * m_Str)
{
	for( ;  *m_Str!= '\0'; m_Str++)
	{
		*m_Str = tolower(*m_Str);
	}
	return m_Str;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
string FuncGetInstallPath()
{
	string stemp=GetSiteViewRootPath();
	return stemp;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
string FuncGetProfileStringBy( char * m_AppName, char * m_KeyName, char * m_FileName)
{

	char szReturn[2048]={0};
	int nSize=2047;
	string strInt ;
    GetPrivateProfileString1(m_AppName, m_KeyName, "", szReturn, nSize, m_FileName);
	strInt = szReturn;
	return strInt;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
string FuncGetStringFromIDS(const char * szIDS)
{
	return GetResourceValue(szIDS);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
string FuncGetStringFromIDS(const char* szSection, const char * szIDS)
{
	return GetResourceValue(szIDS);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int FuncGetProfileIntBy(char * m_AppName, char * m_KeyName, char * m_FileName)
{
	return  GetPrivateProfileInt1(m_AppName,m_KeyName,0,m_FileName);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int GetPrivateProfileInt1(char * m_AppName, char * m_KeyName, int k, char * m_FileName )
{
	if(gRoot_path == "")
        gRoot_path =FuncGetInstallPath();

	char szPath[1024]={0};
	sprintf(szPath,"%s\\data\\tmpIniFile\\%s",gRoot_path.c_str(),m_FileName);
	return GetPrivateProfileInt(m_AppName,m_KeyName,k,szPath);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int GetPrivateProfileString1(char * m_AppName, char * m_KeyName, char * m_default, char * m_Ret, int size, char * m_FileName)
{
	if(gRoot_path == "")
        gRoot_path =FuncGetInstallPath();

	char szPath[1024]={0};
	sprintf(szPath,"%s\\data\\tmpIniFile\\%s",gRoot_path.c_str(),m_FileName);
	return GetPrivateProfileString(m_AppName,m_KeyName,m_default,m_Ret,size,szPath);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int WritePrivateProfileString1( char * m_AppName, char * m_KeyName, char * m_Value, char * m_FileName)
{
	string strInt;
	if(gRoot_path=="")
		gRoot_path =FuncGetInstallPath();

	char szPath[1024]={0};
	sprintf(szPath,"%s\\data\\tmpIniFile\\%s",gRoot_path.c_str(),m_FileName);
	return WritePrivateProfileString(m_AppName,m_KeyName,m_Value,szPath);
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ������CSnmpOperate                                                      //
// ˵�������캯��                                                          //
/////////////////////////////////////////////////////////////////////////////
CSnmpOperate::CSnmpOperate()
{
    pOidList = NULL;
	m_nTimeOut = 300;
	m_nRetrys = 3;
}

/////////////////////////////////////////////////////////////////////////////
// ������~CSnmpOperate                                                     //
// ˵������������                                                          //       
/////////////////////////////////////////////////////////////////////////////
CSnmpOperate::~CSnmpOperate()
{
	if(pOidList)
		delete []pOidList;
	if(pIfOidList)
		delete []pIfOidList;
}

/////////////////////////////////////////////////////////////////////////////
// ������CSnmpOperate                                                      //
// ˵�������캯��                                                          //
// ������                                                                  //
//      pSMIFile, SMI�ļ�·��                                              //
//      pMonitorID, �����ID                                               //
//      nTPLID, TPL ID                                                     //
//      pHostIP, �����豸��IP��ַ                                          //
//      pCommunity, ��ͬ������                                             //
//      nPort, �����豸�Ķ˿�                                              //
//      nIndex, OID����                                                    //
/////////////////////////////////////////////////////////////////////////////
//int nTimeOut,int nRetrys ,
CSnmpOperate::CSnmpOperate(const char *pSMIFile, const char *pMonitorID, const char *pHostIP,	const char *pCommunity,
        const char * strIndex, const char* strSelValue, const int &nTPLID,
        const int &nPort, const int & nSnmpVer, const int &nTimeout):
m_nSnmpVersion(nSnmpVer),
m_nTimeOut(nTimeout),
m_nTPLID(nTPLID),
m_nPort(nPort)
{
	strcpy(chSMIFile, pSMIFile);//SMI�ļ�·��
	strcpy(chMonitorID, pMonitorID);//�����ID
	strcpy(chHostIP, pHostIP);//�����豸��IP��ַ
	strcpy(chCommunity, pCommunity);//��ͬ������
	strcpy(chIfIndex, strIndex);
	strcpy(chSelValue, strSelValue);
	pOidList = NULL;
	pIfOidList = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// ������InitSNMP                                                          //
// ˵������ʼ��SNMP                                                        //
// ��������                                                                //
// ����ֵ��                                                                //
//      �ɹ�����0��ʧ�ܷ���1                                               //
/////////////////////////////////////////////////////////////////////////////
int CSnmpOperate::InitSNMP(CSVSnmpOID &objOID)
{
	objOID.SetIPAddress(chHostIP);//���������豸IP��ַ
	objOID.SetNetworkPort(m_nPort);//���������豸�˿�
	objOID.SetCommunity(chCommunity);//���ù�ͬ��
	objOID.SetVersion(m_nSnmpVersion);
    objOID.SetTimeout(m_nTimeOut);
	if(objOID.InitSNMP() != 0)//��ʼ��
	{//��ʼ��ʧ��
		printf("%s\n", "InitSNMP() failure");
		return 1;
	}
	return 0;//��ʼ���ɹ�
}

/////////////////////////////////////////////////////////////////////////////
// ������ReadInterfaceOID                                                        //
// ˵��������SMI�ļ���ȡ���ö�ȡ���ڻ�������豸�˿���Ϣ��OID                           //
// ������																   //
//		pOidList, OID�б�                                                  //
//		pExpList, ����ʽ�б�											   //
// ����ֵ��																   //
//      ��ȡ�ɹ�true������Ϊfalse                                          //	
/////////////////////////////////////////////////////////////////////////////
bool CSnmpOperate::ReadInterfaceOID()
{
	int i;
	char chSection[MAX_BUFF_LEN] = {0};//��
	string m_szRet;
	sprintf(chSection, "interfaceoid");//***���������öΣ��������ö�ȡ�����豸�˿���Ϣ��OID***//

	//��ȡOID�����ͼ���ʽ����
	strcpy(chSMIFile , "smi.ini");//***new add***//
	m_nIfOidCount = FuncGetProfileIntBy(chSection, "oidnum", chSMIFile);
	if(m_nIfOidCount == 0)//���û�����ö�ȡ�˿���Ϣ��OID����ʹ��ȱʡ��OID
	{
		pIfOidList = new SNMPIfOIDList[1];
		strcpy(pIfOidList[0].oid_ifindex,"1.3.6.1.2.1.2.2.1.1");//��ȡ�˿�������OID
		strcpy(pIfOidList[0].oid_ifdescr,"1.3.6.1.2.1.2.2.1.2");//��ȡ�˿�������OID
		strcpy(pIfOidList[0].oid_iftype,"1.3.6.1.2.1.2.2.1.3");//��ȡ�˿����͵�OID
		strcpy(pIfOidList[0].oid_ifStatus,"1.3.6.1.2.1.2.2.1.8");//��ȡ�˿�״̬��OID
		m_nIfOidCount=1;
		return true;
	}

	pIfOidList = new SNMPIfOIDList[m_nIfOidCount];
	char chTempFile[MAX_BUFF_LEN] = {0};
	sprintf(chTempFile, "snmp_If_%s_%s.ini", chHostIP, chMonitorID);

	for(i = 0; i < m_nIfOidCount; i ++)
	{//��ȡÿһ��OID��¼
		sprintf(chSection, "interfaceoid");;
		char key1[256] = {0};
		//����
		sprintf(key1, "oid_ifindex%d", i + 1);
		//oid
		m_szRet = FuncGetProfileStringBy(chSection, key1, chSMIFile);
		strcpy(pIfOidList[i].oid_ifindex, m_szRet.c_str());
		
		//����
		sprintf(key1, "oid_ifdescr%d", i + 1);
		//oid
		m_szRet = FuncGetProfileStringBy(chSection, key1, chSMIFile);
		strcpy(pIfOidList[i].oid_ifdescr, m_szRet.c_str());

		//����
		sprintf(key1, "oid_iftype%d", i + 1);
		//oid
		m_szRet = FuncGetProfileStringBy(chSection, key1, chSMIFile);
		strcpy(pIfOidList[i].oid_iftype, m_szRet.c_str());

		//״̬
		sprintf(key1, "oid_ifStatus%d", i + 1);
		//oid
		m_szRet = FuncGetProfileStringBy(chSection, key1, chSMIFile);
		strcpy(pIfOidList[i].oid_ifStatus, m_szRet.c_str());
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// ������ReadConfig                                                        //
// ˵��������SMI�ļ���ȡ����TPL��Ӧ��OID�ͼ���ʽ                           //
// ������																   //
//		pOidList, OID�б�                                                  //
//		pExpList, ����ʽ�б�											   //
// ����ֵ��																   //
//      ��ȡ�ɹ�true������Ϊfalse                                          //	
/////////////////////////////////////////////////////////////////////////////
bool CSnmpOperate::ReadConfig()
{
	int i;
	char chSection[MAX_BUFF_LEN] = {0};//��
	string m_szRet;
	sprintf(chSection, "monitor-%d", m_nTPLID);//***new add***//

    //��ȡOID�����ͼ���ʽ����
	strcpy(chSMIFile , "smi.ini");//***new add***//
	m_nOidCount = FuncGetProfileIntBy(chSection, "oidnum", chSMIFile);
	m_nExpCount = FuncGetProfileIntBy(chSection, "returnnum", chSMIFile);
	if(m_nOidCount == 0 || m_nExpCount == 0)
	{//���OID�������߼���ʽ������һΪ0
		sprintf(chResult, "error=%s(TPLMIB_%d)", 
            FuncGetStringFromIDS("SV_SNMP_MONITOR", "SNMP_SMI_LOST_PARAM").c_str(), m_nTPLID);
		return false;//����ʧ��
	}

    pOidList = new SNMPList[m_nOidCount];
    char chTempFile[MAX_BUFF_LEN] = {0};
    sprintf(chTempFile, "snmp_%s_%s_%s.ini", chHostIP, chIfIndex, chMonitorID);
	
	for(i = 0; i < m_nOidCount; i ++)
	{//��ȡÿһ��OID��¼
		sprintf(chSection, "monitor-%d", m_nTPLID);
        char key1[256] = {0};
		sprintf(key1, "oidnum_oid%d", i + 1);
        //oid
		m_szRet = FuncGetProfileStringBy(chSection, key1, chSMIFile);
		strcpy(pOidList[i].chOID, m_szRet.c_str());

        //����
		memset(key1, 0, sizeof(key1));
		sprintf(key1, "oidnum_type%d", i + 1);
		pOidList[i].nType = FuncGetProfileIntBy(chSection, key1, chSMIFile) - 1;

        //�Ƿ�ʹ�ü�¼��ʷ����
		memset(key1, 0, sizeof(key1));
		sprintf(key1, "oidnum_hashis%d", i + 1);
		pOidList[i].nHasHistory = FuncGetProfileIntBy(chSection, key1, chSMIFile);
        //OID����
		if(pOidList[i].nType == 1)
        {
			strcpy(pOidList[i].chOIDIndex, chIfIndex);
        }
        //ʱ�䵥λ
		memset(key1, 0, sizeof(key1));
		sprintf(key1, "returnunit_%d", i + 1);
		m_szRet = FuncGetProfileStringBy(chSection,key1,chSMIFile);
        strcpy(pOidList[i].chUnit, m_szRet.c_str());

        memset(key1, 0, sizeof(key1));
		sprintf(key1, "oidnum_max%d", i + 1);
        if(pOidList[i].nHasHistory ==1)
        {
            //��ʷ����
            sprintf(chSection, "InterFace_%d_%s_%d", m_nTPLID, chIfIndex, i+1);
		    GetPrivateProfileString1(chSection, "HistoryValue", "0", pOidList[i].chHisValue, MAX_BUFF_LEN, chTempFile);
            //�ϴβɼ�ʱ��
            pOidList[i].lHisTime = GetPrivateProfileInt1(chSection, "ifTime",  -99, chTempFile); 
        }
	}
	for(i = 0; i < m_nExpCount; i ++)
	{//��ÿһ������ʽ
		sprintf(chSection, "monitor-%d", m_nTPLID);
		char key1[256];
		memset(key1, 0, 256);
		sprintf(key1, "returnnum_value%d", i + 1);
        //���㹫ʽ
		m_szRet = FuncGetProfileStringBy(chSection, key1, chSMIFile);

        //���㹫ʽ����
		memset(key1, 0, 256);
		sprintf(key1, "returnnum_name%d", i + 1);
		m_mapExpList[m_szRet] = FuncGetProfileStringBy(chSection, key1, chSMIFile);
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// ������SetSMIFilePath                                                    //
// ˵��������SMI�ļ�·��                                                   //
// ������                                                                  //
//      chPath��SMI�ļ�·��                                                //
// ����ֵ����                                                              //
/////////////////////////////////////////////////////////////////////////////
void CSnmpOperate::SetSMIFilePath(const char* chPath)
{
	strcpy(chSMIFile, chPath);
}

/////////////////////////////////////////////////////////////////////////////
// ������SetMonitorID                                                      //
// ˵�������ü����ID                                                      //
// ������                                                                  //
//      pMonitorID�������ID                                               //
// ����ֵ����                                                              //
/////////////////////////////////////////////////////////////////////////////
void CSnmpOperate::SetMonitorID(const char* pMonitorID)
{
	strcpy(chMonitorID, pMonitorID);
}

/////////////////////////////////////////////////////////////////////////////
// ������SetHostIP                                                         //
// ˵�������������豸IP��ַ                                                //
// ������                                                                  //
//      pHostIP�������豸IP��ַ                                            //
// ����ֵ����                                                              //
/////////////////////////////////////////////////////////////////////////////
void CSnmpOperate::SetHostIP(const char* pHostIP)
{
	strcpy(chHostIP, pHostIP);
}

/////////////////////////////////////////////////////////////////////////////
// ������SetCommunity                                                      //
// ˵�������÷��������豸�Ĺ�ͬ������                                      //
// ������                                                                  //
//      pCommunity����ͬ������                                             //
// ����ֵ����                                                              //
/////////////////////////////////////////////////////////////////////////////
void CSnmpOperate::SetCommunity(const char* pCommunity)
{
	strcpy(chCommunity, pCommunity);
}

/////////////////////////////////////////////////////////////////////////////
// ������SetPort                                                           //
// ˵�������������豸�ķ��ʶ˿�                                            //
// ������                                                                  //
//      nPort�������豸�˿�                                                //
// ����ֵ����                                                              //
/////////////////////////////////////////////////////////////////////////////
void CSnmpOperate::SetPort(int nPort)
{
	m_nPort = nPort;
}

/////////////////////////////////////////////////////////////////////////////
// ������SetTplID                                                          //
// ˵��������TPL ID                                                        //
// ������                                                                  //
//      nTplID��TPL ID                                                     //
// ����ֵ����                                                              //
/////////////////////////////////////////////////////////////////////////////
void CSnmpOperate::SetTplID(const int nTplID)
{
	m_nTPLID = nTplID;
}

/////////////////////////////////////////////////////////////////////////////
// ������GetResult                                                         //
// ˵���������Ѿ����úõĲ��������м���                                    //
// ������                                                                  //
//      chReturn, [in]���ͣ�������                                       //
// ����ֵ��                                                                //
//      �ɹ�����true,���򷵻�false                                         //
/////////////////////////////////////////////////////////////////////////////
bool CSnmpOperate::GetResult(char* chReturn, int & nSize)
{
	int i;
	CSVSnmpOID objOID;

	if(InitSNMP(objOID) == 0)
	{//��ʼ��SNMP 
		if(!ReadConfig())
        {//��ȡ�����ļ�ʧ��
            strcpy(chReturn, chResult);
			nSize = static_cast<int>(strlen(chResult));
            return FALSE;
        }		
        bool bReturn = true;
		svutil::TTime curTime = svutil::TTime::GetCurrentTimeEx();
		char chOID[MAX_BUFF_LEN] = {0};

        CSVExpression expression;
        MonitorResult resultList;
        resultItem  resultIt;
		for(i = 0 ; i < m_nOidCount; i++)
		{//�õ�ÿһ��oid�Ľ��
            if(pOidList[i].nType == 1)
            {
				if(strlen(pOidList[i].chOIDIndex)==0)
				{
					objOID.SetOIDType(1);
					sprintf(chOID, "%s", pOidList[i].chOID);
				}
				else
				{
					objOID.SetOIDType(0);
                    sprintf(chOID, "%s.%s", pOidList[i].chOID , pOidList[i].chOIDIndex);
				}
            }
            else 
            {                
                sprintf(chOID, "%s", pOidList[i].chOID);
            }

			objOID.SetSelName(chSelValue);
			objOID.SetOIDValue(chOID);
            resultList.clear();

			int nResult = objOID.GetResult(resultList);
		    if(nResult ==0)
			{//�õ�����ɹ�
                resultIt = resultList.begin();

				OutputDebugString("firstValue=");
				OutputDebugString(resultIt->second.m_szIndex.c_str());
				OutputDebugString(",");
				OutputDebugString(resultIt->second.m_szOID.c_str());
				OutputDebugString(",");
				OutputDebugString(resultIt->second.m_szValue.c_str());
				OutputDebugString("\n");

                if(resultIt != resultList.end())
                {
                    unsigned __int64 ulHisValue = 0, ulValue = 0,uSubValue = 0;
					string szMibID = chOID;
					string szDot = ".";
					size_t nPos = szMibID.rfind(szDot);
					string szSub = szMibID.substr(0, nPos);
					if((szSub=="1.3.6.1.2.1.31.1.1.1.6") || (szSub=="1.3.6.1.2.1.31.1.1.1.10"))
					{
						ulValue = OXTOD(resultIt->second.m_szValue);
						ulHisValue = OXTOD(pOidList[i].chHisValue);
					}
					else
					{
						ulHisValue = (unsigned __int64)_atoi64(pOidList[i].chHisValue);
						ulValue = (unsigned __int64)_atoi64(resultIt->second.m_szValue.c_str());
					}
				    uSubValue = ulValue;
                    if(pOidList[i].lHisTime == -99 && pOidList[i].nHasHistory == 1)
                    { 
                        bReturn = false;
						OutputDebugString("��ʷ��������\n");
                    }
                    else
                    {   
                        unsigned long lTime = 0;//ʱ���ֵ
                        if(pOidList[i].nHasHistory == 1)
                        {
							if(ulValue < ulHisValue)
							{
								//ulValue = 0;
								char chMsg[MAX_BUFF_LEN] = {0};
								char chSection[MAX_BUFF_LEN] = {0};
								char chTempFile[MAX_BUFF_LEN] = {0};
								sprintf(chTempFile, "snmp_%s_%s_%s.ini", chHostIP, chIfIndex, chMonitorID);
								sprintf(chSection, "InterFace_%d_%s_%d", m_nTPLID, chIfIndex, i+1);
								strcpy(pOidList[i].chHisValue, resultIt->second.m_szValue.c_str());
								WritePrivateProfileString1(chSection, "HistoryValue", pOidList[i].chHisValue, chTempFile);
								sprintf(chMsg, "%ld", curTime.GetTime());
								WritePrivateProfileString1(chSection, "ifTime", chMsg, chTempFile);
								Sleep(1000);
								GetResult(chReturn, nSize);
							}
							else
								uSubValue = ulValue - ulHisValue;

							if(uSubValue > ulValue)
							{
								ofstream fout("snmptest.log",ios::app);
								fout << "oid" << i + 1<<":hisvalue="<< pOidList[i].chHisValue<<":"<<ulHisValue << 
									", curvalue="<<resultIt->second.m_szValue<<":"<<ulValue<<
									", subvalue="<<uSubValue<<", time="<<lTime<<", curtime="<<curTime.Format()<<"\r\n"; 
								fout << flush; 
								fout.close(); 
							}

				            TTime bTime(pOidList[i].lHisTime);//�ϴβɼ�����ʱ��
		                    TTimeSpan spanTime = curTime - bTime; //ʱ���                     
                            strlwr(pOidList[i].chUnit);//��λ�����ַ��任ΪСд
                            if(strcmp(pOidList[i].chUnit, "second") == 0)
                            {//��λ����
                                lTime = spanTime.GetTotalSeconds();
                            }
                            else if(strcmp(pOidList[i].chUnit, "minute") == 0)
                            {//��λ�Ƿ���
                                lTime = spanTime.GetTotalMinutes();
                            }
                            else if(strcmp(pOidList[i].chUnit, "hours") == 0)
                            {//��λ��Сʱ
						        lTime = spanTime.GetHours();
                            }
                            else
                            {//ȱʡ�����������
                                lTime = spanTime.GetTotalSeconds();
                            }

                            if(lTime <= 0)
                            {
                                //lTime = 1;
								char chMsg[MAX_BUFF_LEN] = {0};
								char chSection[MAX_BUFF_LEN] = {0};
								char chTempFile[MAX_BUFF_LEN] = {0};
								sprintf(chTempFile, "snmp_%s_%s_%s.ini", chHostIP, chIfIndex, chMonitorID);
								sprintf(chSection, "InterFace_%d_%s_%d", m_nTPLID, chIfIndex, i+1);
								strcpy(pOidList[i].chHisValue, resultIt->second.m_szValue.c_str());
								WritePrivateProfileString1(chSection, "HistoryValue", pOidList[i].chHisValue, chTempFile);
								sprintf(chMsg, "%ld", curTime.GetTime());
								WritePrivateProfileString1(chSection, "ifTime", chMsg, chTempFile);
								Sleep(1000);
								GetResult(chReturn, nSize);
                            }
                        }

                        char chOIDName[8] = {0};
                        sprintf(chOIDName, "oid%d", i + 1);
                        string szOIDName = chOIDName;

                        expression.AddFields(szOIDName, "value", uSubValue);
                        expression.AddFields(szOIDName, "time", lTime);
                        expression.AddFields(szOIDName, "avg",  objOID.AvgValue(resultList));
                        expression.AddFields(szOIDName, "max", objOID.MaxValue(resultList));
                        expression.AddFields(szOIDName, "min", objOID.MinValue(resultList));
                        expression.AddFields(szOIDName, "total", objOID.SumValue(resultList));
                        expression.AddFields(szOIDName, "indexcount", objOID.IndexCount(resultList));
                    }
					strcpy(pOidList[i].chHisValue, resultIt->second.m_szValue.c_str());
                }
                else
                {
					OutputDebugString("��ȡ����ʧ�ܣ�\n");
                    bReturn = false;
                }
			}
			else
            {//�����ȡoidֵʧ��
                bReturn = false;
				//(TPLMIB-%d-oid-%d)  �û��޷����
                sprintf(chReturn, "error=%s", objOID.GetErrorMsg(nResult), m_nTPLID, i+1);

				nSize = static_cast<int>(strlen(chReturn));
                break;
			}
		}

	    chResult[0] = '\0';
		int retlen = 0;
        if(bReturn)
        {
		    try
            {
            for(map<string, string, less<string> >::iterator it = m_mapExpList.begin(); it != m_mapExpList.end(); it ++)
		    {//����ÿһ����ʽ������
			    string rpn;//��ʽ��ֽ��
                double dResult = 0.0f;
			    int err = expression.calculateDouble(it->first, dResult);
                if(err == CSVExpression::eval_ok )
				{//����ɹ�  
					//if(dResult > 1000000000)
					//	WriteLog();
					if( i == 0)
                    {//����ǵ�һ��������					
						sprintf(chReturn, "%s=%.2f", it->second.c_str(), dResult);
						retlen = static_cast<int>(strlen(chReturn)) + 1;
						nSize = retlen;							
                    }
					else
                    {//���ǵ�һ��������
                        char chTemp[MAX_BUFF_LEN] = {0};
						char chTemp1[MAX_BUFF_LEN] = {0};
						memcpy(chTemp, chReturn, retlen);
                        //��ʽ������ַ���
						sprintf(chTemp1, "%s=%.2f", it->second.c_str(), dResult);
						int retlen1 = static_cast<int>(strlen(chTemp1));
						memcpy(chReturn + retlen, chTemp1, retlen1);
						retlen += (retlen1 + 1);
						nSize = retlen;

                    }
				}
				else
				{//������㹫ʽ�д��ڴ�����߼����г��ִ����磺����Ϊ0��					
                    if(err == CSVExpression::eval_idiv)
                    {
                        if( i == 0)
                        {//����ǵ�һ��������
						    sprintf(chReturn, "%s=0.0", it->second.c_str());
							retlen = static_cast<int>(strlen(chReturn)) + 1;
                        }
					    else
                        {//���ǵ�һ��������
                            char chTemp[MAX_BUFF_LEN] = {0};
							char chTemp1[MAX_BUFF_LEN] = {0};
							memcpy(chTemp, chReturn, retlen);
                            //��ʽ������ַ���
							sprintf(chTemp1, "%s=0.0f", it->second.c_str());
							int retlen1 = static_cast<int>(strlen(chTemp1));
							memcpy(chReturn + retlen, chTemp1, retlen1);
							retlen += (retlen1 + 1);
                        }
						nSize = retlen;
                    }
			    }
		    }
            }
            catch(...)
            {
                DWORD dwError = GetLastError();
                char szMsg[512] = {0};
                int nlen = sprintf(szMsg, "Error Number is %08X --*Get Result*---", dwError);
                DumpLog("snmpmonitor-exp.log", szMsg, nlen);
            }
        }
        else
        {
			if(strlen(chReturn)<1)
			{
				std::string m_TempStr = FuncGetStringFromIDS("SV_SNMP_MONITOR", "SNMP_NO_HISTORY_DATA");
				sprintf(chReturn, "error=%s", m_TempStr.c_str());
				nSize = static_cast<int>(strlen(chReturn));
			}
			
        }	
        char chMsg[MAX_BUFF_LEN] = {0};
        char chSection[MAX_BUFF_LEN] = {0};
        char chTempFile[MAX_BUFF_LEN] = {0};
        sprintf(chTempFile, "snmp_%s_%s_%s.ini", chHostIP, chIfIndex, chMonitorID);
        for(i = 0 ; i < m_nOidCount; i++)
        {
            sprintf(chSection, "InterFace_%d_%s_%d", m_nTPLID, chIfIndex, i+1);
		    WritePrivateProfileString1(chSection, "HistoryValue", pOidList[i].chHisValue, chTempFile);
		    sprintf(chMsg, "%ld", curTime.GetTime());
		    WritePrivateProfileString1(chSection, "ifTime", chMsg, chTempFile);
        }
	}
    else
    {//��ʼ��SNMPʧ��
	    std::string m_IDS = FuncGetStringFromIDS("IDS_InitSNMPFailed");
        sprintf(chReturn, "error=%s", (char*)m_IDS.c_str());
		nSize = static_cast<int>(strlen(chReturn));
        return false;
    }
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// ������GetResultByOID                                                       //
// ˵��������ָ����OID����ò���                                    //
// ������                                                                  //
//      chReturn, [in]���ͣ�������                                       //
// ����ֵ��                                                                //
//      �ɹ�����true,���򷵻�false                                         //
/////////////////////////////////////////////////////////////////////////////

bool CSnmpOperate::GetResultByOID(char* chReturn, string strOID,int & nSize)
{
	int i;
	CSVSnmpOID objOID;

	if(InitSNMP(objOID) == 0)//��ʼ��SNMP 
	{
		bool bReturn = true;
		svutil::TTime curTime = svutil::TTime::GetCurrentTimeEx();
		char chOID[MAX_BUFF_LEN] = {0};

		CSVExpression expression;
		MonitorResult resultList;
		resultItem  resultIt;
		int nResult;

		objOID.SetOIDValue(strOID.c_str());
		objOID.SetOID(strOID.c_str());
		resultList.clear();
		nResult = objOID.GetRequest(resultList);


		if(nResult ==0)
		{//�õ�����ɹ�
			chResult[0] = '\0';
			int retlen = 0;
			nSize=0;
			resultIt = resultList.begin();

			OutputDebugString("firstValue=");
			OutputDebugString(resultIt->second.m_szIndex.c_str());
			OutputDebugString(",");
			OutputDebugString(resultIt->second.m_szOID.c_str());
			OutputDebugString(",");
			OutputDebugString(resultIt->second.m_szValue.c_str());
			OutputDebugString("\n");
			for(resultIt=resultList.begin();resultIt!=resultList.end();resultIt++)
			{
				//��ֵд�뷵���ַ���
				//				sprintf(chReturn, "%s=%.2f", it->second.c_str(), dResult);
				//				retlen = static_cast<int>(strlen(chReturn)) + 1;
				sprintf(chReturn+nSize, "value=%s\0", resultIt->second.m_szValue.c_str());
				retlen = resultIt->second.m_szValue.size()*2+6;//��6��ʾ����value=��\0
				nSize += retlen;							

			}
			chReturn[nSize]='\0';
		}
		else
		{//�����ȡoidֵʧ��
			bReturn = false;
			//(TPLMIB-%d-oid-%d)  �û��޷����
			sprintf(chReturn, "error=%s", objOID.GetErrorMsg(nResult), m_nTPLID, i+1);

			nSize = static_cast<int>(strlen(chReturn));
		}

		if(!bReturn)
		{
			if(strlen(chReturn)<1)
			{
				std::string m_TempStr = FuncGetStringFromIDS("SV_SNMP_MONITOR", "SNMP_NO_HISTORY_DATA");
				sprintf(chReturn, "error=%s", m_TempStr.c_str());
				nSize = static_cast<int>(strlen(chReturn));
			}

		}	
	}
	else
	{//��ʼ��SNMPʧ��
		std::string m_IDS = FuncGetStringFromIDS("IDS_InitSNMPFailed");
		sprintf(chReturn, "error=%s", (char*)m_IDS.c_str());
		nSize = static_cast<int>(strlen(chReturn));
		return false;
	}
	return true;
}
bool CSnmpOperate::GetInterfaceInfo(char* chReturn, string strOID,int & nSize)
{
	//����ӿ�OID����
	int i;
	CSVSnmpOID objOID;

	if(InitSNMP(objOID) != 0)
	{
		std::string m_TempBuf = FuncGetStringFromIDS("IDS_InitSNMPFailed");
		sprintf(chReturn, "error=%s", (char*)m_TempBuf.c_str());
		return false;	
	}
	//��ʼ��SNMP 
	if(!ReadInterfaceOID())
	{//��ȡ�����ļ�ʧ��
		strcpy(chReturn, chResult);
		nSize = static_cast<int>(strlen(chResult));
		return FALSE;
	}
	MonitorResult resultList;
	map<int, string, less<int> > lsInterfaces;
	map<int, string, less<int> >::iterator itInterfaces;
	for (int nIndex=0;nIndex<m_nIfOidCount;nIndex++)
	{
		objOID.SetOIDValue(pIfOidList[nIndex].oid_ifindex);
		objOID.SetOIDType(1);

		resultList.clear();
		int nResult = objOID.GetResult(resultList);//�õ����
		if(nResult == 0)
		{
			resultItem resultIt;
			for(resultIt = resultList.begin(); resultIt != resultList.end(); resultIt ++)
			{
				lsInterfaces[atoi(resultIt->second.m_szIndex.c_str())] = resultIt->second.m_szValue;
			}

			resultList.clear();

			objOID.SetOIDValue(pIfOidList[nIndex].oid_ifdescr);
			objOID.SetOIDType(1);

			nResult = objOID.GetResult(resultList);//�õ����
			if(nResult == 0)
			{
				for(resultIt = resultList.begin(); resultIt != resultList.end(); resultIt ++)
				{
					itInterfaces = lsInterfaces.find(atoi(resultIt->second.m_szIndex.c_str()));
					if(itInterfaces != lsInterfaces.end())
					{
						itInterfaces->second += "_";
						itInterfaces->second += resultIt->second.m_szValue;
					}
				}

				resultList.clear();

				objOID.SetOIDValue(pIfOidList[nIndex].oid_iftype);
				objOID.SetOIDType(1);

				nResult = objOID.GetResult(resultList);//�õ����
				if(nResult == 0)
				{
					for(resultIt = resultList.begin(); resultIt != resultList.end(); resultIt ++)
					{
						itInterfaces = lsInterfaces.find(atoi(resultIt->second.m_szIndex.c_str()));
						if(itInterfaces != lsInterfaces.end())
						{
							unsigned __int64 nifType = (unsigned __int64)_atoi64(resultIt->second.m_szValue.c_str());
							itInterfaces->second += "_";
							itInterfaces->second += ( nifType == 1 ? "other" : 
							nifType == 2 ? "regular1822" :
							nifType == 3 ? "hdh1822":
							nifType == 4 ? "ddn-x25":
							nifType == 5 ? "rfc877-x25":
							nifType == 6 ? "ethernet-csmacd":
							nifType == 7 ? "iso88023-csmacd":
							nifType == 8 ? "iso88024-tokenBus":
							nifType == 9 ? "iso88025-tokenRing":
							nifType == 10 ? "iso88026-man":
							nifType == 11? "starLan":
							nifType == 12 ? "proteon-10Mbit":
							nifType == 13 ? "proteon-80Mbit":
							nifType == 14 ? "hyperchannel":
							nifType == 15 ? "fddi":
							nifType == 16 ? "lapb":
							nifType == 17 ? "sdlc":
							nifType == 18 ? "ds1":
							nifType == 19 ? "e1":
							nifType == 20 ? "basicISDN":
							nifType == 21 ? "primaryISDN":
							nifType == 22 ? "propPointToPointSerial":
							nifType == 23 ? "ppp":
							nifType == 24 ? "softwareLoopback":
							nifType == 25 ? "eon":
							nifType == 26 ? "ethernet-3Mbit":
							nifType == 27 ? "nsip":
							nifType == 28 ? "slip":
							nifType == 29 ? "ultra":
							nifType == 30 ? "ds3":
							nifType == 31 ? "sip":
							nifType == 32 ? "frame-relay": "unknown");
						}

					}
					resultList.clear();

					objOID.SetOIDValue(pIfOidList[nIndex].oid_ifStatus);
					objOID.SetOIDType(1);

					nResult = objOID.GetResult(resultList);//�õ����
					if(nResult == 0)
					{
						for(resultIt = resultList.begin(); resultIt != resultList.end(); resultIt ++)
						{
							itInterfaces = lsInterfaces.find(atoi(resultIt->second.m_szIndex.c_str()));
							if(itInterfaces != lsInterfaces.end())
							{
								unsigned __int64 nifType = (unsigned __int64)_atoi64(resultIt->second.m_szValue.c_str());

								itInterfaces->second += "_";
								itInterfaces->second += ( nifType == 1 ? "up":
								nifType == 2 ? "down":
								nifType == 3 ? "testing" : "unknown");
							}
						}

						resultList.clear();

						objOID.SetOIDValue("1.3.6.1.2.1.31.1.1.1.1");
						objOID.SetOIDType(1);

						nResult = objOID.GetResult(resultList);//�õ����
						if(nResult == 0)
						{
							for(resultIt = resultList.begin(); resultIt != resultList.end(); resultIt ++)
							{
								itInterfaces = lsInterfaces.find(atoi(resultIt->second.m_szIndex.c_str()));
								if(itInterfaces != lsInterfaces.end())
								{
									itInterfaces->second += "_";
									itInterfaces->second += resultIt->second.m_szValue;
								}
							}
						}
					}
				}
			}
			break;
		}
	}
	char *pPosition = chReturn;
	int nWritten = 0;
	int nLen = 0;
	int nOneLen=0;
	char tmpchar[50]={0};
	for(itInterfaces = lsInterfaces.begin(); itInterfaces != lsInterfaces.end(); itInterfaces ++)
	{
		nLen = pPosition- chReturn;
		//itoa(itInterfaces->first,tmpchar,49);
		sprintf(tmpchar ,"%d", itInterfaces->first);
		nOneLen = strlen(tmpchar) + itInterfaces->second.size()+2;
		printf("\nnOneLen=%d  nLen=%d   nSize=%d\n",nOneLen,nLen,nSize);
		if(nOneLen +nLen>nSize )
		{
			break;
		}

		//nWritten = sprintf(pPosition, "%d=%s", itInterfaces->first, itInterfaces->second.c_str());
		//nWritten = sprintf(pPosition, "%d=%d", itInterfaces->first, itInterfaces->first);
		nWritten = sprintf(pPosition, "%s=%s", itInterfaces->second.c_str(), itInterfaces->second.c_str());
		if(nWritten != -1)
		{
			pPosition += nWritten;
			pPosition[0] = '\0';
			pPosition ++;


		}
		else
		{
			break;
		}
	}

	SV_ECC_SNMP_LIB::ReleaseLib();
	return true;
}
bool CSnmpOperate::WriteLog()
{
	int i;
	CSVSnmpOID objOID;
	bool bReturn = true;

	if(InitSNMP(objOID) == 0)
	{//��ʼ��SNMP 
		if(!ReadConfig())
		//��ȡ�����ļ�ʧ��
            return FALSE;		
        
		svutil::TTime curTime = svutil::TTime::GetCurrentTimeEx();
		char chOID[MAX_BUFF_LEN] = {0};

        CSVExpression expression;
        MonitorResult resultList;
        resultItem  resultIt;
		for(i = 0 ; i < m_nOidCount; i++)
		{//�õ�ÿһ��oid�Ľ��
            if(pOidList[i].nType == 1)
            {
				if(strlen(pOidList[i].chOIDIndex)==0)
				{
					objOID.SetOIDType(1);
					sprintf(chOID, "%s", pOidList[i].chOID);
				}
				else
				{
					objOID.SetOIDType(0);
                    sprintf(chOID, "%s.%s", pOidList[i].chOID , pOidList[i].chOIDIndex);
				}
            }
            else 
            {                
                sprintf(chOID, "%s", pOidList[i].chOID);
            }

			objOID.SetSelName(chSelValue);
			objOID.SetOIDValue(chOID);
            resultList.clear();

			int nResult = objOID.GetResult(resultList);
		    if(nResult ==0)
			{//�õ�����ɹ�
                resultIt = resultList.begin();
                if(resultIt != resultList.end())
                {
                    unsigned __int64 ulHisValue = 0, ulValue = 0,uSubValue = 0;
					string szMibID = chOID;
					string szDot = ".";
					size_t nPos = szMibID.rfind(szDot);
					string szSub = szMibID.substr(0, nPos);
					if((szSub=="1.3.6.1.2.1.31.1.1.1.6") || (szSub=="1.3.6.1.2.1.31.1.1.1.10"))
					{
						ulValue = OXTOD(resultIt->second.m_szValue);
						ulHisValue = OXTOD(pOidList[i].chHisValue);
					}
					else
					{
						ulHisValue = (unsigned __int64)_atoi64(pOidList[i].chHisValue);
						ulValue = (unsigned __int64)_atoi64(resultIt->second.m_szValue.c_str());
					}
				    unsigned __int64 uTmp = ulValue;
                    if(pOidList[i].lHisTime == -99 && pOidList[i].nHasHistory == 1)
                    { 
                        bReturn = false;
                    }
                    else
                    {   
                        unsigned long lTime = 0;//ʱ���ֵ
                        if(pOidList[i].nHasHistory == 1)
                        {
				            TTime bTime(pOidList[i].lHisTime);//�ϴβɼ�����ʱ��
		                    TTimeSpan spanTime = curTime - bTime; //ʱ���                     
                            strlwr(pOidList[i].chUnit);//��λ�����ַ��任ΪСд
                            if(strcmp(pOidList[i].chUnit, "second") == 0)
                            {//��λ����
                                lTime = spanTime.GetTotalSeconds();
                            }
                            else if(strcmp(pOidList[i].chUnit, "minute") == 0)
                            {//��λ�Ƿ���
                                lTime = spanTime.GetTotalMinutes();
                            }
                            else if(strcmp(pOidList[i].chUnit, "hours") == 0)
                            {//��λ��Сʱ
						        lTime = spanTime.GetHours();
                            }
                            else
                            {//ȱʡ�����������
                                lTime = spanTime.GetTotalSeconds();
                            }
                        }
                        if(pOidList[i].nHasHistory == 1)
                        {	
                            if(ulValue < ulHisValue)
                                uSubValue = ulValue - ulHisValue - 1;
                            else
                                uSubValue = ulValue - ulHisValue;
                        }

						ofstream fout("snmptest.log",ios::app);
						fout << "oid" << i + 1<<":hisvalue="<< pOidList[i].chHisValue << ",curvalue="<<resultIt->second.m_szValue<<
							",subvalue="<<uSubValue<<",time="<<lTime<<",curtime="<<curTime.Format()<<"\r\n"; 
						fout << flush; 
						fout.close(); 
                    }
                }
                else
                    bReturn = false;
			}
			else
            {//�����ȡoidֵʧ��
                bReturn = false;
				break;
			}
		}
	}
	else
	//��ʼ��SNMPʧ��
		return false;

	return bReturn;
}