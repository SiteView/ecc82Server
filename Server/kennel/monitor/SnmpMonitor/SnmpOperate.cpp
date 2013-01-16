// SnmpOperate.cpp: implementation of the CSnmpOperate class.
//
// 
//////////////////////////////////////////////////////////////////////
// SnmpOperate.cpp: implementation of the CSnmpOperate class.
//#include <fstream>
#include <iostream>
#include <fstream>
#include <atltime.h>
#include "SnmpOperate.h"
#include "expression.h"

#include "Time.h"

#include <scsvapi\svapi.h>
#include "Base\stlini.h"

using namespace SV_ECC_SNMP_LIB;
using namespace svutil;
char str[100];

void WriteLog( const char* str );
void WriteTxt( const char* str, const char* pszFileName=NULL );
bool WriteLog(CString strFileName,const CString strLog);
void myPrintLog(const char *LogMes);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DumpLog(const char * pszFileName, const char *pBuffer, const int nLen)
{
	return;
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
	return GetSiteViewRootPath();
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
		//gRoot_path = "D:\\SiteView\\SiteView ECC";  //zjw

	char szPath[1024]={0};
	sprintf(szPath,"%s\\data\\TmpIniFile\\%s",gRoot_path.c_str(),m_FileName);
	return GetPrivateProfileInt(m_AppName,m_KeyName,k,szPath);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int GetPrivateProfileString1(char * m_AppName, char * m_KeyName, char * m_default, char * m_Ret, int size, char * m_FileName)
{
	if(gRoot_path == "")
        gRoot_path =FuncGetInstallPath();
		//gRoot_path = "D:\\SiteView\\SiteView ECC";  //zjw

	char szPath[1024]={0};
	sprintf(szPath,"%s\\data\\TmpIniFile\\%s",gRoot_path.c_str(),m_FileName);
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
	sprintf(szPath,"%s\\data\\TmpIniFile\\%s",gRoot_path.c_str(),m_FileName);
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

	cout << "chHostIP = " << chHostIP << endl;
	cout << "m_nPort = " << m_nPort << endl;
	cout << "chCommunity = " << chCommunity << endl;
	cout << "m_nSnmpVersion = " << m_nSnmpVersion << endl;
	cout << "m_nTimeOut = " << m_nTimeOut << endl;

	if(objOID.InitSNMP() != 0)//��ʼ��
	{//��ʼ��ʧ��
		printf("%s\n", "InitSNMP() failure");
		return 1;
	}
	return 0;//��ʼ���ɹ�
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
	if (pOidList)
	{
		delete []pOidList;
	}
    pOidList = new SNMPList[m_nOidCount];
    char chTempFile[MAX_BUFF_LEN] = {0};
    sprintf(chTempFile, "snmp_%s_%s_%s.ini", chHostIP, chIfIndex, chMonitorID);
	
	for(i = 0; i < m_nOidCount; i ++)
	{//��ȡÿһ��OID��¼
		sprintf(chSection, "monitor-%d", m_nTPLID);
        char key1[256] = {0};
		sprintf(key1, "oidnum_oid%d", i+1);
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
//==============================================================
//
// ���Ӻ����� RetrieveData
//            ��GetResult�У������ֻ�ȡ��������С����ʷ����ʱ��
//            ˵��mib�еļ�����������������ʱ��Ҫ���»�ȡһ��
//            ���ݣ�ԭ���������Ƕ�GetResultʵ��һ�εݹ飬����
//            �������������oid����ִ��һ��GetResult����ε�ʱ
//            ��������Ϊ1���ӣ�������ȷ��һ��ִ��GerResult��
//            ����oid���ɹ����1��Ĳ�ֵ��Ȼ���������˵�GetResult
//            �ݹ����������¶�ʣ���oid��ȡһ��ֵ����Ϊ����
//            �ĵݹ��Ѿ�Ӱ����pOidList��Ҳ����Ӱ���˶�ʣ���oid
//            �����д����������ֵݹ������ReadConfig�л�����
//            pOidList�ڴ�й¶��
//            �����޸ķ�����ֻ���Ǹ����ڷ�����oid���»�ȡһ����
//            �ݣ����ڲ��ı�GetResult������oid��pOidList��curTime��
//            ���Բ���Ӱ������oid�Ĵ���
//            û�ж�ǧ�׵Ĵ��뼴��snmpMonitorKM���޸ģ��Ƿ���Ҫ
//            �޸�Ӧ�ÿ��������
// 
// ������: ����
// ����ʱ��: 2009.06.10
//
//==============================================================
bool CSnmpOperate::RetrieveData( CSVSnmpOID objOID, int i, CSVExpression& expression )
{
	//::Sleep( 2000 );   //2��̫���ˣ�ȡ�������ݸ��ϴ���һ���ģ��ʳ���0��  duanYi 2010-06-10
	::Sleep( 5000 );

	bool bReturn = false;

	svutil::TTime curTime = svutil::TTime::GetCurrentTimeEx();

	char chOID[MAX_BUFF_LEN] = {0};


	if(InitSNMP(objOID) == 0)
	{//��ʼ��SNMP
		MonitorResult resultList;
        resultItem  resultIt;

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

		WriteTxt( "oid=", chMonitorID );
		WriteTxt( chOID, chMonitorID );

        CString strLog = _T("");
		CString strFile = _T("");
		strFile.Format("snmp_%s_%s_%s.ini", chHostIP, chMonitorID,chIfIndex);

		char szTemp[1024] = {0};
		sprintf( szTemp, "oidHasHistory=%d,hisValue=%s,hisTime=%ld,curTime=%ld", 
				    pOidList[i].nHasHistory, pOidList[i].chHisValue, pOidList[i].lHisTime, curTime.GetTime() );
		WriteTxt( szTemp, chMonitorID );
		
        resultList.clear();

		int nResult(-1);

		nResult = objOID.GetResult(resultList);

		if(nResult ==0)
		{//�õ�����ɹ�
            resultIt = resultList.begin();

			if(resultIt != resultList.end())
			{
				char strTemp[1024] = {0};
				sprintf(strTemp,"snmp return value:%s:%s,%s\n", resultIt->second.m_szOID.c_str(),
										resultIt->second.m_szValue.c_str(),
										resultIt->second.m_szIndex.c_str());

				WriteTxt( strTemp, chMonitorID );


				// ���浱������
				char chMsg[MAX_BUFF_LEN] = {0};
				char chSection[MAX_BUFF_LEN] = {0};
				char chTempFile[MAX_BUFF_LEN] = {0};
				sprintf(chTempFile, "snmp_%s_%s_%s.ini", chHostIP, chIfIndex, chMonitorID);					
				sprintf(chSection, "InterFace_%d_%s_%d", m_nTPLID, chIfIndex, i+1);
				strcpy( chMsg, resultIt->second.m_szValue.c_str() );
				WritePrivateProfileString1( chSection, "HistoryValue", chMsg, chTempFile );
				sprintf(chMsg, "%ld", curTime.GetTime());
				WritePrivateProfileString1(chSection, "ifTime", chMsg, chTempFile);

				
				WriteLog(strFile,"======= CSnmpOperate::RetrieveData  ======== ");
				strLog.Format("OID:%s	History:%s",chOID, pOidList[i].chHisValue);
				WriteLog(strFile,strLog);
				strLog.Format("OID:%s	Value:	%s",chOID, resultIt->second.m_szValue.c_str());
				WriteLog(strFile,strLog);
				WriteLog(strFile,"======= CSnmpOperate::RetrieveData End ======== ");


				unsigned long ulHisValue = 0, ulValue = 0,uSubValue = 0;
                ulHisValue = (unsigned long)atoi(pOidList[i].chHisValue);
                ulValue = (unsigned long)atoi(resultIt->second.m_szValue.c_str());

				uSubValue = ulValue - ulHisValue;

				if( ulValue < ulHisValue )
				{
					strcpy(pOidList[i].chHisValue, resultIt->second.m_szValue.c_str());
					pOidList[i].lHisTime = curTime.GetTime();
					RetrieveData( objOID, i, expression );
				}
				else
				{
					unsigned long lTime = 1;

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

					bReturn = true;

				}

			}
		
		}
	}

	return bReturn;

}


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

		string strValue6 = "", strValue7 = "";

		CString strLog = _T("");
		CString strFile = _T("");
		strFile.Format("snmp_%s_%s_%s.ini", chHostIP, chMonitorID,chIfIndex);
	    

		for(i = 0 ; (i < m_nOidCount); i++)
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

			cout <<"*******************************" <<endl;
			cout << "chOID:"<<chOID<<endl;
			cout <<"*******************************" <<endl;
            cout <<"*******************************" <<endl;
			cout << "chSelValue:"<<chSelValue<<endl;
			cout <<"*******************************" <<endl;
			char szTemp[1024] = {0};
			sprintf( szTemp, "oidHasHistory=%d,hisValue=%s,hisTime=%ld,curTime=%ld", 
				     pOidList[i].nHasHistory, pOidList[i].chHisValue, pOidList[i].lHisTime, curTime.GetTime() );
			WriteTxt( szTemp, chMonitorID );

            resultList.clear();

			int nResult(-1);

			if( ( strstr( chOID, "1.3.6.1.2.1.2.2.1.12" ) != NULL ) ||
				( strstr( chOID, "1.3.6.1.2.1.2.2.1.18" ) != NULL ) )// �������ķ���ǽ�Ľӿ�
			{

				nResult = objOID.GetResult(resultList);

				if( nResult != 0 )
				{
					nResult = 0;

					SNMP_Monitor_Result result;
									//1.3.6.1.2.1.2.2.1.12
					result.m_szOID = "1.3.6.1.2.1.2.2.1.12";
					result.m_szValue = "0";
					result.m_szIndex = pOidList[i].chOIDIndex;

					resultList[0] = result;
				}

			}
			else
			{

//duanYi{-----------------------------------------Я�̣����ӽӿ�����������һ������ֵ��2010-04-20
         //�и�OID Ϊ1.3.6.1.2.1.2.2.1.20��Ҳ�����,��˼Ӹ���
				if (strstr( chOID, "1.3.6.1.2.1.2.2.1.2." )!= NULL)
				{
					objOID.SetOIDValue("1.3.6.1.2.1.2.2.1.2");  
					objOID.SetOIDType(1);           //duanYi:ֻ�ܵ���������ȡ	    		
				}
//duanYi----------------------------------------}

		
				WriteLog(strFile,"-------------- GetResult Start ------------------");
				cout << "-------------- GetResult Start ------------------"<<endl;
				nResult = objOID.GetResult(resultList);
				cout << "-------------- GetResult End ------------------"<<endl;
				strLog.Format("OID:%s, nResult:%d", chOID, nResult);
				WriteLog(strFile,strLog);
				WriteLog(strFile,"-------------- GetResult End  -------------------");

			}

//duanYi{-----------------------------2010-04-20
			if (strstr( chOID, "1.3.6.1.2.1.2.2.1.2." )!= NULL)
			{
				for(resultIt = resultList.begin(); resultIt != resultList.end(); resultIt ++)
				{
					if (strcmp(chIfIndex ,resultIt->second.m_szIndex.c_str())==0)
					{
						strValue6 = resultIt->second.m_szValue.c_str();
						printf("strValue6 = %s\r\n", strValue6.c_str());
					}
				}
			}
//duanYi----------------------------------------}
		    if(nResult ==0)
			{//�õ�����ɹ�
                resultIt = resultList.begin();

                if(resultIt != resultList.end())
                {
					char strTemp[1024] = {0};
					sprintf(strTemp,"snmp return value:%s:%s,%s\n", resultIt->second.m_szOID.c_str(),
											resultIt->second.m_szValue.c_str(),
											resultIt->second.m_szIndex.c_str());
					///----------------------���������---------------------
					///֧�ָ�ֵ��֧��string ����һ��ֵ
				 if( strstr( chOID, "1.3.6.1.4.1.4458.1000.1.5.3" ) != NULL )
				
					{
	    				sprintf(chReturn, "ssid=%s", resultIt->second.m_szValue.c_str());
       					return true;
					}
					if( strstr( chOID, "1.3.6.1.4.1.4458.1000.1.5.9.1" ) != NULL )
				
					{
	    				sprintf(chReturn, "rss=%s", resultIt->second.m_szValue.c_str());
       					return true;
					}
                   //( strstr( chOID, "1.3.6.1.4.1.4458.1000.1.5.9.1" ) != NULL ) )
					///----------------------���������---------------------

					WriteTxt( strTemp, chMonitorID );
					
					cout << strTemp <<endl;

					// ���浱������
					char chMsg[MAX_BUFF_LEN] = {0};
					char chSection[MAX_BUFF_LEN] = {0};
					char chTempFile[MAX_BUFF_LEN] = {0};
					sprintf(chTempFile, "snmp_%s_%s_%s.ini", chHostIP, chIfIndex, chMonitorID);					
					sprintf(chSection, "InterFace_%d_%s_%d", m_nTPLID, chIfIndex, i+1);
					strcpy( chMsg, resultIt->second.m_szValue.c_str() );
					WritePrivateProfileString1( chSection, "HistoryValue", chMsg, chTempFile );
					sprintf(chMsg, "%ld", curTime.GetTime());


					WritePrivateProfileString1(chSection, "ifTime", chMsg, chTempFile);
					
					WriteLog(strFile,"======= CSnmpOperate::GetResult start======== ");
					
					strLog.Format("OID:%s History:%s",chOID,pOidList[i].chHisValue);
					WriteLog(strFile,strLog);
					strLog.Format("OID:%s Value:  %s",chOID,resultIt->second.m_szValue.c_str());
					WriteLog(strFile,strLog);
					
					WriteLog(strFile,"======= CSnmpOperate::GetResult End ======== ");
                    
//yi.duan 2010-12-22 ������������
					//unsigned long ulHisValue = 0, ulValue = 0,uSubValue = 0;
					//ulHisValue = (unsigned long)atoi(pOidList[i].chHisValue);
				    //ulValue = (unsigned long)atoi(resultIt->second.m_szValue.c_str());

					unsigned __int64 ulHisValue = 0, ulValue = 0,uSubValue = 0;
					ulHisValue = (unsigned __int64)atoi(pOidList[i].chHisValue);
					ulValue = (unsigned __int64)atoi(resultIt->second.m_szValue.c_str());
				   
//yi.duan 2010-12-22
					uSubValue = ulValue;
					// ����information����ֵ���÷���ֵ�Ǹ��ַ�����Я��ר��
					//if( strstr( chOID, "1.3.6.1.2.1.2.2.1.2" ) != NULL )
					//{	
					//	strValue6 = resultIt->second.m_szValue.c_str();  //ԭ���� duanYi 2010-05-06 
					//	continue;
					//}

//yi.duan 2010-05-06 ���˸��㣬�и�OIDΪ 1.3.6.1.2.1.2.2.1.20 ͬ���������ж�
					if( strstr( chOID, "1.3.6.1.2.1.2.2.1.2." ) != NULL )
					{	
						//dy 2010-05-06 �����Ѿ���ȡ��
						continue;
					}
					if( strstr( chOID, "1.3.6.1.2.1.31.1.1.1.18" ) != NULL )
					{
						strValue7 = resultIt->second.m_szValue.c_str();
						continue;
					}

                    if(pOidList[i].lHisTime == -99 && pOidList[i].nHasHistory == 1)
                    { 
                        bReturn = false;
                    }
                    else
                    {   
                        //unsigned long lTime = 0;//ʱ���ֵ //dy��ò����޷��ŵģ���Ϊ�������ж��Ƿ�<=0
						long lTime = 0;   //dy //��������ǰ��
                        if(pOidList[i].nHasHistory == 1)
                        {
							if(ulValue < ulHisValue)
							{
								
								////ulValue = 0;
								//char chMsg[MAX_BUFF_LEN] = {0};
								//char chSection[MAX_BUFF_LEN] = {0};
								//char chTempFile[MAX_BUFF_LEN] = {0};
								//sprintf(chTempFile, "snmp_%s_%s_%s.ini", chHostIP, chIfIndex, chMonitorID);
								//sprintf(chSection, "InterFace_%d_%s_%d", m_nTPLID, chIfIndex, i+1);
								//strcpy(pOidList[i].chHisValue, resultIt->second.m_szValue.c_str());
								//WritePrivateProfileString1(chSection, "HistoryValue", pOidList[i].chHisValue, chTempFile);
								//sprintf(chMsg, "%ld", curTime.GetTime());
								//WritePrivateProfileString1(chSection, "ifTime", chMsg, chTempFile);
								//Sleep(1000);
								//GetResult(chReturn, nSize);
								

								// ���µ���snmp��ȡһ������ old ������
								/*strcpy(pOidList[i].chHisValue, resultIt->second.m_szValue.c_str());
								pOidList[i].lHisTime = curTime.GetTime();
								bReturn = RetrieveData( objOID, i, expression );
								continue;*/
								//bin.liu when it wraps around and starts increasing again from zero
                                uSubValue = ulValue+4294967295-ulHisValue;
							}
							else
							{
								uSubValue = ulValue - ulHisValue;
							}
					
							cout << "******uSubValue******" << endl; 
							cout << "uSubValue =  "<< uSubValue << endl;
							cout << "******uSubValue******" << endl;

							strLog.Format("OID:%s,uSubValue: %d",chOID,uSubValue);
							WriteLog(strFile,strLog);

							if(uSubValue > ulValue)
							{
								ofstream fout("snmperror.log",ios::app);
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

cout << "******lTime******" << endl; 
cout << "lTime =  "<< lTime << endl;
cout << "******lTime******" << endl; 

strLog.Format("OID:%s,lTime: %ld",chOID,lTime);
WriteLog(strFile,strLog);
WriteLog(strFile,"\n\n");

                            if(lTime <= 0)
                            {
								
                                // //lTime = 1;
								//char chMsg[MAX_BUFF_LEN] = {0};
								//char chSection[MAX_BUFF_LEN] = {0};
								//char chTempFile[MAX_BUFF_LEN] = {0};
								//sprintf(chTempFile, "snmp_%s_%s_%s.ini", chHostIP, chIfIndex, chMonitorID);
								//sprintf(chSection, "InterFace_%d_%s_%d", m_nTPLID, chIfIndex, i+1);
								//strcpy(pOidList[i].chHisValue, resultIt->second.m_szValue.c_str());
								//WritePrivateProfileString1(chSection, "HistoryValue", pOidList[i].chHisValue, chTempFile);
								//sprintf(chMsg, "%ld", curTime.GetTime());
								//WritePrivateProfileString1(chSection, "ifTime", chMsg, chTempFile);
								//Sleep(1000);
								//GetResult(chReturn, nSize);
								
								// ���µ���snmp��ȡһ������
								strcpy(pOidList[i].chHisValue, resultIt->second.m_szValue.c_str());
								pOidList[i].lHisTime = curTime.GetTime();
								WriteLog(strFile,"RetrieveData");
								bReturn = RetrieveData( objOID, i, expression );
								continue;
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
                    
                }
                else
                {
                    bReturn = false;
                }
			}
			else
            {//�����ȡoidֵʧ��
                bReturn = false;
				//(TPLMIB-%d-oid-%d)  �û��޷����
                sprintf(chReturn, "error=%s", objOID.GetErrorMsg(nResult), m_nTPLID, i+1);

				nSize = static_cast<int>(strlen(chReturn));
				return false;
                break;
			}
		}

	    chResult[0] = '\0';
		memset( chReturn, 0, nSize );

		int retlen = 0;
        if(bReturn)
        {
		    try
            {

			//strLog.Format("������������%d",i);
			//WriteLog(strFile,strLog);
            for(map<string, string, less<string> >::iterator it = m_mapExpList.begin(); it != m_mapExpList.end(); it ++)
		    {//����ÿһ����ʽ������

			    string rpn;//��ʽ��ֽ��
               
				double dResult = 0.0f;
				//cout << "it->first = " << it->first << endl;
			    int err = expression.calculateDouble(it->first, dResult);
                if(err == CSVExpression::eval_ok )
				{//����ɹ� 
					
					WriteLog(strFile,"--------- ����ɹ� -----------");
					if( i==0)
					{//����ǵ�һ��������
						if( it->second.compare("information") == 0 )
						{
							sprintf(chReturn, "%s=%s %s", it->second.c_str(), strValue6.c_str(), strValue7.c_str() );
						}
						else
						{
							sprintf(chReturn, "%s=%.2f", it->second.c_str(), dResult);
							WriteLog(strFile,"---------- һ������ֵ -------------");
							strLog.Format("%s\n",chReturn);
							WriteLog(strFile, strLog);

						}
						
						retlen = static_cast<int>(strlen(chReturn)) + 1;
						nSize = retlen;							
					}
					else
					{//���ǵ�һ��������
						char chTemp[MAX_BUFF_LEN] = {0};
						char chTemp1[MAX_BUFF_LEN] = {0};
						memcpy(chTemp, chReturn, retlen);
						//��ʽ������ַ���
						if( it->second.compare("information") == 0 ) //Я��ר��
						{
						
							sprintf(chTemp1, "%s=%s %s", it->second.c_str(), strValue6.c_str(), strValue7.c_str() );
							strLog.Format("%s\n",chTemp1);
							WriteLog(strFile, strLog);

cout << "==================" << endl;
cout << "strvalue6 = " << strValue6 << endl;
cout << "==================" << endl;
						}
						else
						{
							sprintf(chTemp1, "%s=%.2f", it->second.c_str(), dResult);

							strLog.Format("%s\n",chTemp1);
							WriteLog(strFile, strLog);
						}						

						int retlen1 = static_cast<int>(strlen(chTemp1));
						memcpy(chReturn + retlen, chTemp1, retlen1);
						retlen += (retlen1 + 1);
						nSize = retlen;


					}
					
				}
				else
				{//������㹫ʽ�д��ڴ�����߼����г��ִ����磺����Ϊ0��					
                   //duanYi ֻҪ��һ������ֵ���ˣ�Ӧ�ñ��������ľ���,

					if(err == CSVExpression::eval_idiv)
                    {
						WriteLog(strFile, "----------  ������ʧ�� ------------");
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
							WriteLog(strFile, "-------- ����Ϊ�� -----------");
							WriteLog(strFile, chTemp1);
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

				WriteLog(strFile,szMsg);
            }
        }
        else//yi.duan ����ֵ̫��ͳ����ʱ(������)������ʷ���ݣ�oid�Ų�����,����SNMP_NO_HISTORY_DATA����������
        {
			//ȡֵʱ ���Ի�ȡ������Ϣ��Ӧ�÷���m_szErrorMsg =  m_pSnmp->error_msg(nResult);
			if(strlen(chReturn)<1)
			{
				std::string m_TempStr = FuncGetStringFromIDS("SV_SNMP_MONITOR", "SNMP_NO_HISTORY_DATA");
				sprintf(chReturn, "error=%s", m_TempStr.c_str());
				nSize = static_cast<int>(strlen(chReturn));
			}
			
        }	

		
      //  char chMsg[MAX_BUFF_LEN] = {0};
      //  char chSection[MAX_BUFF_LEN] = {0};
      //  char chTempFile[MAX_BUFF_LEN] = {0};
      //  sprintf(chTempFile, "snmp_%s_%s_%s.ini", chHostIP, chIfIndex, chMonitorID);
      //  for(i = 0 ; i < m_nOidCount; i++)
      //  {
      //      sprintf(chSection, "InterFace_%d_%s_%d", m_nTPLID, chIfIndex, i+1);
		    //WritePrivateProfileString1(chSection, "HistoryValue", pOidList[i].chHisValue, chTempFile);
		    //sprintf(chMsg, "%ld", curTime.GetTime());
		    //WritePrivateProfileString1(chSection, "ifTime", chMsg, chTempFile);
      //  }
		
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


//==============================================================
//
// �޸�����:
// ԭ����ȡֵ��ʽ��ͨ������ˢ��,������ȡ�õ�ֵ���,ʵ���ϻ�ȡ����
// ����ˢ�¼����ƽ��ֵ.���ڸ�Ϊ��һ��ˢ���ڼ��һ��ȡ����ֵ,Ȼ��
// �����ε�ֵ���,������õĽ����˲ʱֵ��Ϊ�ӽ�.
// 
// �޸���:     ����       ����
// �޸�ʱ��: 2009.1.24  2010.05.11
//==============================================================
/*
bool CSnmpOperate::GetResult(char* chReturn, int & nSize)
{
	bool bReturn;
	int i;
	CSVSnmpOID objOID;

	if(!ReadConfig())//��ȡ�����ļ�ʧ��
    {
        strcpy(chReturn, chResult);
		nSize = static_cast<int>(strlen(chResult));
        return FALSE;
    }

	CSVExpression expression;
	CSVSnmpOID objFirstOID, objLastOID;
	unsigned long  firstTime, lastTime; 

	MonitorResult resultList;
    resultItem  resultIt;
	char chOID[MAX_BUFF_LEN] = {0};
	unsigned long  ulValue = 0, lTime = 0;

	// ��һ�λ�ȡ����
	if(InitSNMP(objFirstOID) == 0)// ��ʼ��SNMP
	{
		for(i=0 ; i < m_nOidCount; i++)// �õ�ÿһ��oid�Ľ��
		{
            if(pOidList[i].nType == 1)
            {
				if(strlen(pOidList[i].chOIDIndex)==0)
				{
					objFirstOID.SetOIDType(1);
					sprintf(chOID, "%s", pOidList[i].chOID);
				}
				else
				{
					objFirstOID.SetOIDType(0);
                    sprintf(chOID, "%s.%s", pOidList[i].chOID , pOidList[i].chOIDIndex);
				}
            }
            else 
            {                
                sprintf(chOID, "%s", pOidList[i].chOID);
            }
		}

		objFirstOID.SetSelName(chSelValue);
		objFirstOID.SetOIDValue(chOID);

		firstTime = GetTickCount();
		
        resultList.clear();
		int nResult = objFirstOID.GetResult(resultList);

		if(nResult ==0)//�õ�����ɹ�
		{
            resultIt = resultList.begin();

			if(resultIt != resultList.end())
            {
				strcpy (pOidList[i].chHisValue , resultIt->second.m_szValue.c_str());
				pOidList[i].lHisTime = firstTime;
			}
			else
			{
				bReturn = false;
			}
		}
		else// �����ȡoidֵʧ��
		{
			bReturn = false;
			//(TPLMIB-%d-oid-%d)  �û��޷����
			sprintf(chReturn, "error=%s", objFirstOID.GetErrorMsg(nResult), m_nTPLID, i+1);

			nSize = static_cast<int>(strlen(chReturn));
		//	break;
		}
	}
	else//��ʼ��SNMPʧ��
    {
	    std::string m_IDS = FuncGetStringFromIDS("IDS_InitSNMPFailed");
        sprintf(chReturn, "error=%s", (char*)m_IDS.c_str());
		nSize = static_cast<int>(strlen(chReturn));
        return false;
    }

	::Sleep(1000);

	// �ڶ��λ�ȡ����
	if( bReturn && ( InitSNMP(objLastOID) == 0 ) )// ��ʼ��SNMP
	{
		for(i = 0 ; i < m_nOidCount; i++)// �õ�ÿһ��oid�Ľ��
		{
            if(pOidList[i].nType == 1)
            {
				if(strlen(pOidList[i].chOIDIndex)==0)
				{
					objLastOID.SetOIDType(1);
					sprintf(chOID, "%s", pOidList[i].chOID);
				}
				else
				{
					objLastOID.SetOIDType(0);
                    sprintf(chOID, "%s.%s", pOidList[i].chOID , pOidList[i].chOIDIndex);
				}
            }
            else 
            {                
                sprintf(chOID, "%s", pOidList[i].chOID);
            }
		}

		objLastOID.SetSelName(chSelValue);
		objLastOID.SetOIDValue(chOID);

		lastTime = GetTickCount();
		
        resultList.clear();
		int nResult = objLastOID.GetResult(resultList);

		if(nResult ==0)//�õ�����ɹ�
		{
            resultIt = resultList.begin();

			if(resultIt != resultList.end())
            {
				ulValue = (unsigned long)atoi(resultIt->second.m_szValue.c_str());
				lTime = lastTime;

				if(pOidList[i].nHasHistory == 1)
                {
					unsigned long ulHisValue = (unsigned long)atoi(pOidList[i].chHisValue);
					if(ulValue < ulHisValue)
					{
						ulValue += 0xffffffff;
					}
                    
					ulValue -= ulHisValue;

					lTime -= (unsigned long)(pOidList[i].lHisTime) / 1000;

				}

				char chOIDName[8] = {0};
                sprintf(chOIDName, "oid%d", i + 1);
                string szOIDName = chOIDName;

                expression.AddFields(szOIDName, "value", ulValue);
                expression.AddFields(szOIDName, "time", lTime);
                expression.AddFields(szOIDName, "avg",  objOID.AvgValue(resultList));
                expression.AddFields(szOIDName, "max", objOID.MaxValue(resultList));
                expression.AddFields(szOIDName, "min", objOID.MinValue(resultList));
                expression.AddFields(szOIDName, "total", objOID.SumValue(resultList));
                expression.AddFields(szOIDName, "indexcount", objOID.IndexCount(resultList));

			}
			else
			{
				bReturn = false;
			}
		}
		else// �����ȡoidֵʧ��
		{
			bReturn = false;
			//(TPLMIB-%d-oid-%d)  �û��޷����
			sprintf(chReturn, "error=%s", objLastOID.GetErrorMsg(nResult), m_nTPLID, i+1);

			nSize = static_cast<int>(strlen(chReturn));
			//break;
		}
	}
	else//��ʼ��SNMPʧ��
    {
	    std::string m_IDS = FuncGetStringFromIDS("IDS_InitSNMPFailed");
        sprintf(chReturn, "error=%s", (char*)m_IDS.c_str());
		nSize = static_cast<int>(strlen(chReturn));
        return false;
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

	return bReturn;

}
*/