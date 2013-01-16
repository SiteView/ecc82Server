// SnmpOperate.cpp: implementation of the CSnmpOperate class.
//
// 
//////////////////////////////////////////////////////////////////////
// SnmpOperate.cpp: implementation of the CSnmpOperate class.
#include <fstream>
#include <iostream>
#include "SnmpOperate.h"
#include "expression.h"

#include "Time.h"

#include "Svdb\svapi\svapi.h"
#include "Base\stlini.h"

using namespace SV_ECC_SNMP_LIB;
using namespace svutil;
char str[100];

void WriteLog( const char* str );
void WriteTxt( const char* str, const char* pszFileName=NULL );

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
// 函数：CSnmpOperate                                                      //
// 说明：构造函数                                                          //
/////////////////////////////////////////////////////////////////////////////
CSnmpOperate::CSnmpOperate()
{
    pOidList = NULL;
	m_nTimeOut = 300;
	m_nRetrys = 3;
}

/////////////////////////////////////////////////////////////////////////////
// 函数：~CSnmpOperate                                                     //
// 说明：析构函数                                                          //       
/////////////////////////////////////////////////////////////////////////////
CSnmpOperate::~CSnmpOperate()
{
    if(pOidList)
        delete []pOidList;
}

/////////////////////////////////////////////////////////////////////////////
// 函数：CSnmpOperate                                                      //
// 说明：构造函数                                                          //
// 参数：                                                                  //
//      pSMIFile, SMI文件路径                                              //
//      pMonitorID, 监测器ID                                               //
//      nTPLID, TPL ID                                                     //
//      pHostIP, 网络设备的IP地址                                          //
//      pCommunity, 共同体名称                                             //
//      nPort, 网络设备的端口                                              //
//      nIndex, OID索引                                                    //
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
	strcpy(chSMIFile, pSMIFile);//SMI文件路径
	strcpy(chMonitorID, pMonitorID);//监测器ID
	strcpy(chHostIP, pHostIP);//网络设备的IP地址
	strcpy(chCommunity, pCommunity);//共同体名称
	strcpy(chIfIndex, strIndex);
	strcpy(chSelValue, strSelValue);
    pOidList = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 函数：InitSNMP                                                          //
// 说明：初始化SNMP                                                        //
// 参数：无                                                                //
// 返回值：                                                                //
//      成功返回0，失败返回1                                               //
/////////////////////////////////////////////////////////////////////////////
int CSnmpOperate::InitSNMP(CSVSnmpOID &objOID)
{
	objOID.SetIPAddress(chHostIP);//设置网络设备IP地址
	objOID.SetNetworkPort(m_nPort);//设置网络设备端口
	objOID.SetCommunity(chCommunity);//设置共同体
	objOID.SetVersion(m_nSnmpVersion);
    objOID.SetTimeout(m_nTimeOut);
	if(objOID.InitSNMP() != 0)//初始化
	{//初始化失败
		printf("%s\n", "InitSNMP() failure");
		return 1;
	}
	return 0;//初始化成功
}

/////////////////////////////////////////////////////////////////////////////
// 函数：ReadConfig                                                        //
// 说明：根据SMI文件读取配置TPL对应的OID和计算式                           //
// 参数：																   //
//		pOidList, OID列表                                                  //
//		pExpList, 计算式列表											   //
// 返回值：																   //
//      读取成功true，否则为false                                          //	
/////////////////////////////////////////////////////////////////////////////
bool CSnmpOperate::ReadConfig()
{
	int i;
	char chSection[MAX_BUFF_LEN] = {0};//项
	string m_szRet;
	sprintf(chSection, "monitor-%d", m_nTPLID);//***new add***//

    //读取OID总数和计算式总数
	strcpy(chSMIFile , "smi.ini");//***new add***//
	m_nOidCount = FuncGetProfileIntBy(chSection, "oidnum", chSMIFile);
	m_nExpCount = FuncGetProfileIntBy(chSection, "returnnum", chSMIFile);
	if(m_nOidCount == 0 || m_nExpCount == 0)
	{//如果OID总数或者计算式总数任一为0
		sprintf(chResult, "error=%s(TPLMIB_%d)", 
            FuncGetStringFromIDS("SV_SNMP_MONITOR", "SNMP_SMI_LOST_PARAM").c_str(), m_nTPLID);
		return false;//返回失败
	}

    pOidList = new SNMPList[m_nOidCount];
    char chTempFile[MAX_BUFF_LEN] = {0};
    sprintf(chTempFile, "snmp_%s_%s_%s.ini", chHostIP, chIfIndex, chMonitorID);
	
	for(i = 0; i < m_nOidCount; i ++)
	{//读取每一条OID纪录
		sprintf(chSection, "monitor-%d", m_nTPLID);
        char key1[256] = {0};
		sprintf(key1, "oidnum_oid%d", i + 1);
        //oid
		m_szRet = FuncGetProfileStringBy(chSection, key1, chSMIFile);
		strcpy(pOidList[i].chOID, m_szRet.c_str());

        //类型
		memset(key1, 0, sizeof(key1));
		sprintf(key1, "oidnum_type%d", i + 1);
		pOidList[i].nType = FuncGetProfileIntBy(chSection, key1, chSMIFile) - 1;

        //是否使用记录历史数据
		memset(key1, 0, sizeof(key1));
		sprintf(key1, "oidnum_hashis%d", i + 1);
		pOidList[i].nHasHistory = FuncGetProfileIntBy(chSection, key1, chSMIFile);
        //OID索引
		if(pOidList[i].nType == 1)
        {
			strcpy(pOidList[i].chOIDIndex, chIfIndex);
        }
        //时间单位
		memset(key1, 0, sizeof(key1));
		sprintf(key1, "returnunit_%d", i + 1);
		m_szRet = FuncGetProfileStringBy(chSection,key1,chSMIFile);
        strcpy(pOidList[i].chUnit, m_szRet.c_str());

        memset(key1, 0, sizeof(key1));
		sprintf(key1, "oidnum_max%d", i + 1);
        if(pOidList[i].nHasHistory ==1)
        {
            //历史数据
            sprintf(chSection, "InterFace_%d_%s_%d", m_nTPLID, chIfIndex, i+1);
		    GetPrivateProfileString1(chSection, "HistoryValue", "0", pOidList[i].chHisValue, MAX_BUFF_LEN, chTempFile);
            //上次采集时间
            pOidList[i].lHisTime = GetPrivateProfileInt1(chSection, "ifTime",  -99, chTempFile); 
        }
	}
	for(i = 0; i < m_nExpCount; i ++)
	{//读每一条计算式
		sprintf(chSection, "monitor-%d", m_nTPLID);
		char key1[256];
		memset(key1, 0, 256);
		sprintf(key1, "returnnum_value%d", i + 1);
        //计算公式
		m_szRet = FuncGetProfileStringBy(chSection, key1, chSMIFile);

        //计算公式名称
		memset(key1, 0, 256);
		sprintf(key1, "returnnum_name%d", i + 1);
		m_mapExpList[m_szRet] = FuncGetProfileStringBy(chSection, key1, chSMIFile);
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// 函数：SetSMIFilePath                                                    //
// 说明：设置SMI文件路径                                                   //
// 参数：                                                                  //
//      chPath，SMI文件路径                                                //
// 返回值：无                                                              //
/////////////////////////////////////////////////////////////////////////////
void CSnmpOperate::SetSMIFilePath(const char* chPath)
{
	strcpy(chSMIFile, chPath);
}

/////////////////////////////////////////////////////////////////////////////
// 函数：SetMonitorID                                                      //
// 说明：设置监测器ID                                                      //
// 参数：                                                                  //
//      pMonitorID，监测器ID                                               //
// 返回值：无                                                              //
/////////////////////////////////////////////////////////////////////////////
void CSnmpOperate::SetMonitorID(const char* pMonitorID)
{
	strcpy(chMonitorID, pMonitorID);
}

/////////////////////////////////////////////////////////////////////////////
// 函数：SetHostIP                                                         //
// 说明：设置网络设备IP地址                                                //
// 参数：                                                                  //
//      pHostIP，网络设备IP地址                                            //
// 返回值：无                                                              //
/////////////////////////////////////////////////////////////////////////////
void CSnmpOperate::SetHostIP(const char* pHostIP)
{
	strcpy(chHostIP, pHostIP);
}

/////////////////////////////////////////////////////////////////////////////
// 函数：SetCommunity                                                      //
// 说明：设置访问网络设备的共同体名称                                      //
// 参数：                                                                  //
//      pCommunity，共同体名称                                             //
// 返回值：无                                                              //
/////////////////////////////////////////////////////////////////////////////
void CSnmpOperate::SetCommunity(const char* pCommunity)
{
	strcpy(chCommunity, pCommunity);
}

/////////////////////////////////////////////////////////////////////////////
// 函数：SetPort                                                           //
// 说明：设置网络设备的访问端口                                            //
// 参数：                                                                  //
//      nPort，网络设备端口                                                //
// 返回值：无                                                              //
/////////////////////////////////////////////////////////////////////////////
void CSnmpOperate::SetPort(int nPort)
{
	m_nPort = nPort;
}

/////////////////////////////////////////////////////////////////////////////
// 函数：SetTplID                                                          //
// 说明：设置TPL ID                                                        //
// 参数：                                                                  //
//      nTplID，TPL ID                                                     //
// 返回值：无                                                              //
/////////////////////////////////////////////////////////////////////////////
void CSnmpOperate::SetTplID(const int nTplID)
{
	m_nTPLID = nTplID;
}

/////////////////////////////////////////////////////////////////////////////
// 函数：GetResult                                                         //
// 说明：根据已经配置好的参数来进行计算                                    //
// 参数：                                                                  //
//      chReturn, [in]类型，计算结果                                       //
// 返回值：                                                                //
//      成果返回true,否则返回false                                         //
/////////////////////////////////////////////////////////////////////////////
//==============================================================
//
// 增加函数： RetrieveData
//            在GetResult中，当发现获取到的数据小于历史数据时，
//            说明mib中的计数器发生翻滚，此时需要重新获取一次
//            数据，原来的做法是对GetResult实行一次递归，估计
//            本意是想对所有oid重新执行一次GetResult，这次的时
//            间间隔设置为1秒钟，所有能确保一次执行GerResult对
//            所有oid都成功获得1秒的差值，然而他忽略了当GetResult
//            递归回来后会重新对剩余的oid又取一次值，因为里面
//            的递归已经影响了pOidList，也就是影响了对剩余的oid
//            的所有处理。并且这种递归调用在ReadConfig中会引起
//            pOidList内存泄露。
//            现在修改方案是只对那个存在翻滚的oid重新获取一次数
//            据，由于不改变GetResult中其他oid的pOidList和curTime，
//            所以不会影响其他oid的处理。
//            没有对千兆的代码即，snmpMonitorKM作修改，是否需要
//            修改应该看具体情况
// 
// 增加人: 邹晓
// 增加时间: 2009.06.10
//
//==============================================================
bool CSnmpOperate::RetrieveData( CSVSnmpOID objOID, int i, CSVExpression& expression )
{
	::Sleep( 1000 );

	bool bReturn = false;

	svutil::TTime curTime = svutil::TTime::GetCurrentTimeEx();

	char chOID[MAX_BUFF_LEN] = {0};

	if(InitSNMP(objOID) == 0)
	{//初始化SNMP
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

		char szTemp[1024] = {0};
		sprintf( szTemp, "oidHasHistory=%d,hisValue=%s,hisTime=%ld,curTime=%ld", 
				    pOidList[i].nHasHistory, pOidList[i].chHisValue, pOidList[i].lHisTime, curTime.GetTime() );
		WriteTxt( szTemp, chMonitorID );
		
        resultList.clear();

		int nResult(-1);

		nResult = objOID.GetResult(resultList);

		if(nResult ==0)
		{//得到结果成功
            resultIt = resultList.begin();

			if(resultIt != resultList.end())
			{
				char strTemp[1024] = {0};
				sprintf(strTemp,"snmp return value:%s:%s,%s\n", resultIt->second.m_szOID.c_str(),
										resultIt->second.m_szValue.c_str(),
										resultIt->second.m_szIndex.c_str());

				WriteTxt( strTemp, chMonitorID );


				// 保存当次数据
				char chMsg[MAX_BUFF_LEN] = {0};
				char chSection[MAX_BUFF_LEN] = {0};
				char chTempFile[MAX_BUFF_LEN] = {0};
				sprintf(chTempFile, "snmp_%s_%s_%s.ini", chHostIP, chIfIndex, chMonitorID);					
				sprintf(chSection, "InterFace_%d_%s_%d", m_nTPLID, chIfIndex, i+1);
				strcpy( chMsg, resultIt->second.m_szValue.c_str() );
				WritePrivateProfileString1( chSection, "HistoryValue", chMsg, chTempFile );
				sprintf(chMsg, "%ld", curTime.GetTime());
				WritePrivateProfileString1(chSection, "ifTime", chMsg, chTempFile);


				unsigned long ulHisValue = 0, ulValue = 0,uSubValue = 0;
                ulHisValue = (unsigned long)atoi(pOidList[i].chHisValue);
                ulValue = (unsigned long)atoi(resultIt->second.m_szValue.c_str());

				uSubValue = ulValue - ulHisValue;

				if( uSubValue < 0 )
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
	{//初始化SNMP 
		if(!ReadConfig())
        {//读取配置文件失败
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

		string strValue6, strValue7;

		for(i = 0 ; (i < m_nOidCount); i++)
		{//得到每一个oid的结果
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

			OutputDebugString("*******************************");
			OutputDebugString(chOID);
			OutputDebugString("\n");

			WriteTxt( "oid=", chMonitorID );
			WriteTxt( chOID, chMonitorID );

			char szTemp[1024] = {0};
			sprintf( szTemp, "oidHasHistory=%d,hisValue=%s,hisTime=%ld,curTime=%ld", 
				     pOidList[i].nHasHistory, pOidList[i].chHisValue, pOidList[i].lHisTime, curTime.GetTime() );
			WriteTxt( szTemp, chMonitorID );
			
            resultList.clear();

			int nResult(-1);

			if( ( strstr( chOID, "1.3.6.1.2.1.2.2.1.12" ) != NULL ) ||
				( strstr( chOID, "1.3.6.1.2.1.2.2.1.18" ) != NULL ) )// 针对特殊的防火墙的接口
			{
				nResult = objOID.GetResult(resultList);

				if( nResult != 0 )
				{
					nResult = 0;

					SNMP_Monitor_Result result;
					result.m_szOID = "1.3.6.1.2.1.2.2.1.12";
					result.m_szValue = "0";
					result.m_szIndex = pOidList[i].chOIDIndex;

					resultList[0] = result;
				}

			}
			else
			{
				nResult = objOID.GetResult(resultList);
			}
			
		    if(nResult ==0)
			{//得到结果成功
                resultIt = resultList.begin();

//				FILE *stream;
//				
//				sprintf(str,"%s\n%s:%s,%s\n",str,resultIt->second.m_szOID.c_str(),
//											resultIt->second.m_szValue.c_str(),
//											resultIt->second.m_szIndex.c_str());
//
//				stream = fopen( "fread.txt", "a+" );
//				fwrite( str, sizeof( char ), strlen(str), stream );
//				fclose(stream);
//

                if(resultIt != resultList.end())
                {
					char strTemp[1024] = {0};
					sprintf(strTemp,"snmp return value:%s:%s,%s\n", resultIt->second.m_szOID.c_str(),
											resultIt->second.m_szValue.c_str(),
											resultIt->second.m_szIndex.c_str());

					WriteTxt( strTemp, chMonitorID );


					// 保存当次数据
					char chMsg[MAX_BUFF_LEN] = {0};
					char chSection[MAX_BUFF_LEN] = {0};
					char chTempFile[MAX_BUFF_LEN] = {0};
					sprintf(chTempFile, "snmp_%s_%s_%s.ini", chHostIP, chIfIndex, chMonitorID);					
					sprintf(chSection, "InterFace_%d_%s_%d", m_nTPLID, chIfIndex, i+1);
					strcpy( chMsg, resultIt->second.m_szValue.c_str() );
					WritePrivateProfileString1( chSection, "HistoryValue", chMsg, chTempFile );
					sprintf(chMsg, "%ld", curTime.GetTime());
					WritePrivateProfileString1(chSection, "ifTime", chMsg, chTempFile);
					

					unsigned long ulHisValue = 0, ulValue = 0,uSubValue = 0;
                    ulHisValue = (unsigned long)atoi(pOidList[i].chHisValue);
                    ulValue = (unsigned long)atoi(resultIt->second.m_szValue.c_str());
				    uSubValue = ulValue;

					// 用于information返回值，该返回值是个字符串，携程专用
					if( strstr( chOID, "1.3.6.1.2.1.2.2.1.2." ) != NULL )
					{
						strValue6 = resultIt->second.m_szValue.c_str();
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
                        unsigned long lTime = 0;//时间差值
                        if(pOidList[i].nHasHistory == 1)
                        {
							if(ulValue < ulHisValue)
							{
								/*
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
								*/

								// 重新调用snmp获取一次数据
								strcpy(pOidList[i].chHisValue, resultIt->second.m_szValue.c_str());
								pOidList[i].lHisTime = curTime.GetTime();
								bReturn = RetrieveData( objOID, i, expression );
								continue;

							}
							else
								uSubValue = ulValue - ulHisValue;

							if(uSubValue > ulValue)
							{
								ofstream fout("snmperror.log",ios::app);
								fout << "oid" << i + 1<<":hisvalue="<< pOidList[i].chHisValue<<":"<<ulHisValue << 
									", curvalue="<<resultIt->second.m_szValue<<":"<<ulValue<<
									", subvalue="<<uSubValue<<", time="<<lTime<<", curtime="<<curTime.Format()<<"\r\n"; 
								fout << flush; 
								fout.close(); 
							}

				            TTime bTime(pOidList[i].lHisTime);//上次采集数据时间
		                    TTimeSpan spanTime = curTime - bTime; //时间差                     
                            strlwr(pOidList[i].chUnit);//单位所有字符变换为小写
                            if(strcmp(pOidList[i].chUnit, "second") == 0)
                            {//单位是秒
                                lTime = spanTime.GetTotalSeconds();
                            }
                            else if(strcmp(pOidList[i].chUnit, "minute") == 0)
                            {//单位是分钟
                                lTime = spanTime.GetTotalMinutes();
                            }
                            else if(strcmp(pOidList[i].chUnit, "hours") == 0)
                            {//单位是小时
						        lTime = spanTime.GetHours();
                            }
                            else
                            {//缺省条件以秒计算
                                lTime = spanTime.GetTotalSeconds();
                            }

                            if(lTime <= 0)
                            {
								/*
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
								*/
								// 重新调用snmp获取一次数据
								strcpy(pOidList[i].chHisValue, resultIt->second.m_szValue.c_str());
								pOidList[i].lHisTime = curTime.GetTime();
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
            {//如果获取oid值失败
                bReturn = false;
				//(TPLMIB-%d-oid-%d)  用户无法理解
                sprintf(chReturn, "error=%s", objOID.GetErrorMsg(nResult), m_nTPLID, i+1);

				nSize = static_cast<int>(strlen(chReturn));
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
            for(map<string, string, less<string> >::iterator it = m_mapExpList.begin(); it != m_mapExpList.end(); it ++)
		    {//根据每一个公式计算结果
				
			    string rpn;//公式拆分结果
                double dResult = 0.0f;
			    int err = expression.calculateDouble(it->first, dResult);
                if(err == CSVExpression::eval_ok )
				{//计算成功 

					if( i == 0)
					{//如果是第一条计算结果
						if( it->second.compare("information") == 0 )
						{
							sprintf(chReturn, "%s=%s %s", it->second.c_str(), strValue6.c_str(), strValue7.c_str() );
						}
						else
						{
							sprintf(chReturn, "%s=%.2f", it->second.c_str(), dResult);
						}
						
						retlen = static_cast<int>(strlen(chReturn)) + 1;
						nSize = retlen;							
					}
					else
					{//不是第一条计算结果
						char chTemp[MAX_BUFF_LEN] = {0};
						char chTemp1[MAX_BUFF_LEN] = {0};
						memcpy(chTemp, chReturn, retlen);
						//格式化结果字符串
						if( it->second.compare("information") == 0 )
						{
							sprintf(chTemp1, "%s=%s %s", it->second.c_str(), strValue6.c_str(), strValue7.c_str() );
						}
						else
						{
							sprintf(chTemp1, "%s=%.2f", it->second.c_str(), dResult);
						}
						
						int retlen1 = static_cast<int>(strlen(chTemp1));
						memcpy(chReturn + retlen, chTemp1, retlen1);
						retlen += (retlen1 + 1);
						nSize = retlen;

					}
					
				}
				else
				{//如果计算公式中存在错误或者计算中出现错误（如：除数为0）					
                    if(err == CSVExpression::eval_idiv)
                    {
                        if( i == 0)
                        {//如果是第一条计算结果
						    sprintf(chReturn, "%s=0.0", it->second.c_str());
							retlen = static_cast<int>(strlen(chReturn)) + 1;
                        }
					    else
                        {//不是第一条计算结果
                            char chTemp[MAX_BUFF_LEN] = {0};
							char chTemp1[MAX_BUFF_LEN] = {0};
							memcpy(chTemp, chReturn, retlen);
                            //格式化结果字符串
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

		/*
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
		*/
	}
    else
    {//初始化SNMP失败
	    std::string m_IDS = FuncGetStringFromIDS("IDS_InitSNMPFailed");
        sprintf(chReturn, "error=%s", (char*)m_IDS.c_str());
		nSize = static_cast<int>(strlen(chReturn));
        return false;
    }

	return true;
}

/*
//==============================================================
//
// 修改内容:
// 原来的取值方式是通过两次刷新,将两次取得的值相减,实质上获取的是
// 两次刷新间隔的平均值.现在改为在一次刷新内间隔一秒取两次值,然后
// 将两次的值相减,这样获得的结果和瞬时值较为接近.
// 
// 修改人: 邹晓
// 修改时间: 2009.1.24
//
//==============================================================
bool CSnmpOperate::GetResult(char* chReturn, int & nSize)
{
	bool bReturn;

	if(!ReadConfig())//读取配置文件失败
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

	unsigned long  ulValue = 0, lTime = 0;

	// 第一次获取数据
	if(InitSNMP(objFirstOID) == 0)// 初始化SNMP
	{
		for(i = 0 ; i < m_nOidCount; i++)// 得到每一个oid的结果
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

		if(nResult ==0)//得到结果成功
		{
            resultIt = resultList.begin();

			if(resultIt != resultList.end())
            {
				pOidList[i].chHisValue = resultIt->second.m_szValue.c_str();
				pOidList[i].lHisTime = firstTime;
			}
			else
			{
				bReturn = false;
			}
		}
		else// 如果获取oid值失败
		{
			bReturn = false;
			//(TPLMIB-%d-oid-%d)  用户无法理解
			sprintf(chReturn, "error=%s", objFirstOID.GetErrorMsg(nResult), m_nTPLID, i+1);

			nSize = static_cast<int>(strlen(chReturn));
			break;
		}
	}
	else//初始化SNMP失败
    {
	    std::string m_IDS = FuncGetStringFromIDS("IDS_InitSNMPFailed");
        sprintf(chReturn, "error=%s", (char*)m_IDS.c_str());
		nSize = static_cast<int>(strlen(chReturn));
        return false;
    }

	::Sleep(1000);

	// 第二次获取数据
	if( bReturn && ( InitSNMP(objLastOID) == 0 ) )// 初始化SNMP
	{
		for(i = 0 ; i < m_nOidCount; i++)// 得到每一个oid的结果
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

		if(nResult ==0)//得到结果成功
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
		else// 如果获取oid值失败
		{
			bReturn = false;
			//(TPLMIB-%d-oid-%d)  用户无法理解
			sprintf(chReturn, "error=%s", objLastOID.GetErrorMsg(nResult), m_nTPLID, i+1);

			nSize = static_cast<int>(strlen(chReturn));
			break;
		}
	}
	else//初始化SNMP失败
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
		{//根据每一个公式计算结果

			
			string rpn;//公式拆分结果
            double dResult = 0.0f;
			int err = expression.calculateDouble(it->first, dResult);
            if(err == CSVExpression::eval_ok )
			{//计算成功     
				if( i == 0)
                {//如果是第一条计算结果					
					sprintf(chReturn, "%s=%.2f", it->second.c_str(), dResult);
					retlen = static_cast<int>(strlen(chReturn)) + 1;
					nSize = retlen;							
                }
				else
                {//不是第一条计算结果
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
			{//如果计算公式中存在错误或者计算中出现错误（如：除数为0）					
                if(err == CSVExpression::eval_idiv)
                {
                    if( i == 0)
                    {//如果是第一条计算结果
						sprintf(chReturn, "%s=0.0", it->second.c_str());
						retlen = static_cast<int>(strlen(chReturn)) + 1;
                    }
					else
                    {//不是第一条计算结果
                        char chTemp[MAX_BUFF_LEN] = {0};
						char chTemp1[MAX_BUFF_LEN] = {0};
						memcpy(chTemp, chReturn, retlen);
                        //格式化结果字符串
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