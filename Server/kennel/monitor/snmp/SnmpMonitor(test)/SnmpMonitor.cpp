// SnmpMonitor.cpp : Defines the initialization routines for the DLL.
//


#include "SnmpMonitor.h"
//#include "stlini.h"
#include "Time.h"
#include <stdlib.h>
#include <fstream>

#include "Base/SVDefines.h"

//#include "..\..\base\funcGeneral.h"
//#include "SNMP_lib.h"

#include "SnmpOperate.h"

using namespace SV_ECC_SNMP_LIB;
using namespace svutil;

#define     COUNTER_MAX    4294976295

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef WIN32
#include <windows.h>
#endif


//===============================================================
//打印日志函数(zou_xiao)
//===============================================================
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
void WriteLog( const char* str )
{
	return;

	char timebuf[128],datebuf[128];

	_tzset();
	_strtime( timebuf );
	_strdate( datebuf );

	char szLogFile[] = "snmpmonitor.log";

	// 判断文件大小：在不打开文件的情况下实现
	struct _stat buf;
	if( _stat( szLogFile, &buf ) == 0 )
	{
		if( buf.st_size > 1000*1024 )
		{
			FILE *log = fopen( szLogFile, "w" );
			if( log != NULL )
				fclose( log );
		}
	}
	


	FILE *log = fopen( szLogFile,"a+");
	if( log != NULL )
	{
		fprintf( log, "%s %s \t%s\n", datebuf, timebuf, str );
		fclose( log );
	}

}

void WriteTxt( const char* str, const char* pszFileName=NULL )
{

	return;

	char timebuf[128],datebuf[128];

	_tzset();
	_strtime( timebuf );
	_strdate( datebuf );

	char szProgramName[] = "scasvapi.log";
	char szLogFile[128];

	if( pszFileName != NULL )
	{
		sprintf( szLogFile, "%s.log", pszFileName );
	}
	else
	{
		sprintf( szLogFile, "%s", szProgramName );
	}
	

	
	// 判断文件大小：在不打开文件的情况下实现
	/*
	struct _stat buf;
	if( _stat( szLogFile, &buf ) == 0 )
	{
		if( buf.st_size > 1000*1024 )
		{
			FILE *log = fopen( szLogFile, "w" );
			if( log != NULL )
				fclose( log );
		}
	}
	*/
	


	FILE *log = fopen( szLogFile,"a+");
	if( log != NULL )
	{
		fprintf( log, "%s %s \t%s\n", datebuf, timebuf, str );
		fclose( log );
	}

}


//#define TOFILE

void PrintDebug(const char * str)
{
#ifdef TOFILE
    WriteLog(str);
#endif

#ifdef TOCONSOLE
	OutputDebugString(str);
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PrintDebugString(const char * szMsg)
{
#ifdef WIN32
    OutputDebugString("SNMPMonitor : ");
    OutputDebugString(szMsg);
    OutputDebugString("\n");
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PrintDebugString(const string & szMsg)
{
    PrintDebugString(szMsg.c_str());
}


bool MakeStringListByChar(StringList& pList, const char * pInput )//将字符转化为字符串链表形式
{
	const char * p;//*p是常量，不能作为左值，p是变量，可以作为左值。这样确保输入串的内容不被显示的修改
	int nSize;
	p=pInput;//将输入串保存到p中。确保输入串不被改动。
	while(*p!='\0')
	{
		nSize = static_cast<int>(strlen(p));//nSize保存串的长度
		if( nSize>0 )
		{	
			//pList.AddHead(p);
			pList.push_back((char*)p);

		}
		p= p + nSize + 1;
	}

	return true;
}




inline int Find( char * m_Str1, char * m_Str2)
{
	int num = 0;
	char * m_Temp = m_Str2;
	char * m_Temp1 = m_Str1;

	for( ; *m_Str1 != '\0'; m_Str1++)
	{
		if( *m_Str1 == *m_Str2)
		{
			num = 0;
			for( ; (*m_Str1 != '\0')&&(*m_Temp != '\0')&&(*m_Str1 == *m_Temp); m_Str1++, m_Temp++)
			{
				num++;
			
			}
			if( num == strlen(m_Str2))
			{
				
				//break;

				return static_cast<int>(m_Str1 - m_Temp1 - num);
			}
			else
			{
				m_Temp = m_Str2;
				m_Str1 -= num;
			}
		}
		
	}
	return -1;
}

inline char * Right( char * m_Str1, int pos)
{
	char * m_Temp = NULL;
	int len = static_cast<int>(strlen( m_Str1));
	m_Temp = (char*)malloc( len - pos);
	memset( m_Temp , 0, len-pos);
	memcpy( m_Temp, m_Str1 + pos, len - pos);
	return m_Temp;
}

inline char * Left( char * m_Str1, int pos)
{
	char * m_Temp = NULL;
	int len = static_cast<int>(strlen( m_Str1));
	m_Temp = (char*)malloc(pos);
	memset( m_Temp , 0, pos);
	memcpy( m_Temp , m_Str1, pos);
	return m_Temp;
}
//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		 int PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CSnmpMonitorApp

/////////////////////////////////////////////////////////////////////////////
// CSnmpMonitorApp construction

CSnmpMonitorApp::CSnmpMonitorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	gRoot_path =FuncGetInstallPath();

}
 bool CSnmpMonitorApp::InitInstance()
{
	gRoot_path =FuncGetInstallPath();
	return true;

}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSnmpMonitorApp objectstring



int CheckOIDIndex(StringList &lsIndex, char* strIniFilePath, CSVSnmpOID &objOID, int nTplID);

enum
{
    forReading,//
    forWriting,
    forAppending
};


/////////////////////////////////////////////////////////////////////////////

extern "C" __declspec(dllexport)  int SNMPDone(const char * strParas, char * szReturn, int& nSize)
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    
	int bReturn = TRUE;
	std::string strIP ;
	std::string	strCommunity ;
	std::string	strMonitorID ;
	std::string strIniFilePath ;
	std::string	strIndex;
	std::string	strSelValue;
	std::string strValue ;	
	int nTplID = 0, nPort = 161, nSnmpVer = 2, nTimeout = 300;//, nIfIndex = 0;

    map<string, string, less<string> > InputParams;
    map<string, string, less<string> >::iterator paramItem;
    if(!splitparam(strParas, InputParams))
    {
        string szErrMsg = FuncGetStringFromIDS("IDS_MONITOR_PARAM_ERROR");
        nSize = sprintf(szReturn , "error=some parameter is wrong");//, FuncGetStringFromIDS("IDS_MONITOR_PARAM_ERROR"));
        return FALSE;
    }

/*	_sleep(1000);

	FILE *stream;
	stream = fopen( "finputdata.txt", "a+" );
	fprintf(stream,"\n\nFunction:SNMPDone:\n");
	map<string, string, less<string> >::iterator Item;
	for(Item=InputParams.begin();Item!=InputParams.end();Item++)
	{
		fprintf(stream,"%s=%s\n",Item->first.c_str(),Item->second.c_str());
	}
	fclose(stream);
*/

    paramItem = InputParams.find(SV_MonitorID);
	
    if(paramItem != InputParams.end())
        strMonitorID = paramItem->second;


    paramItem = InputParams.find(SV_Community);
    if(paramItem != InputParams.end())
        strCommunity = paramItem->second;

    paramItem = InputParams.find(SV_Host);
    if(paramItem != InputParams.end())
        strIP = paramItem->second; 
//////////解决接口信息的显示问题。	张驹武 2007.12.28
    paramItem = InputParams.find(SV_InterfaceIndex);
    if(paramItem != InputParams.end())
        strIndex = paramItem->second;

	//
	int nPos = strIndex.find('_');
	if(nPos != -1)
		strIndex.erase(nPos, strIndex.length()-nPos);
//////////解决接口信息的显示问题。	张驹武 2007.12.28

    paramItem = InputParams.find(SV_SNMPDisk);
    if(paramItem != InputParams.end())
        strIndex = paramItem->second; 
/////////解决进程显示问题
    paramItem = InputParams.find(SV_SNMPSelvalue);
    if(paramItem != InputParams.end())
	{		
		/*
		string strIn=paramItem->second;
		int n=strIn.find("_");

		string strSub=strIn.substr(n+1);

        strSelValue = strSub;
		*/ 
		strSelValue=paramItem->second;
		//nPos=strSelValue.find('_');
		//if(nPos !=-1)
		//	strSelValue.erase(nPos,strSelValue.length()-nPos);

	}
/////////解决进程显示问题
    paramItem = InputParams.find(SV_Port);
    if(paramItem != InputParams.end())
    {
        nPort = atoi(paramItem->second.c_str());
        if(nPort <= 0)
            nPort = 161;
    }

    paramItem = InputParams.find(SV_TimeOut);
    if(paramItem != InputParams.end())
    {
        nTimeout = atoi(paramItem->second.c_str());
        if(nTimeout <= 0)
            nTimeout = 300;
		else 
            nTimeout = nTimeout * 100;
    } 

    paramItem = InputParams.find(SV_SNMPVersion);
    if(paramItem != InputParams.end())
    {
        nSnmpVer = atoi(paramItem->second.c_str());
        if(nSnmpVer <= 0)
            nSnmpVer = 2;
    }

    paramItem = InputParams.find(SV_TPLID);
    if(paramItem != InputParams.end())
        nTplID = atoi(paramItem->second.c_str());

	StringList::iterator pos;
	if(strIP.empty())
	{
		std::string m_Ret = FuncGetStringFromIDS("SV_NETWORK_SET", "NETWORK_SET_IP_ADDRESS_NULL");
		sprintf(szReturn, "error=%s", m_Ret.c_str());
		nSize = static_cast<int>(strlen(szReturn));
		return FALSE;
	}
	strIniFilePath = "smi.ini";

	CSnmpOperate snmpOperate(strIniFilePath.c_str(), strMonitorID.c_str(), 
        strIP.c_str(), strCommunity.c_str(), strIndex.c_str(), strSelValue.c_str(),
		nTplID, nPort, nSnmpVer, nTimeout);
	
    SV_ECC_SNMP_LIB::InitLib();

    snmpOperate.GetResult(szReturn, nSize);
	
    SV_ECC_SNMP_LIB::ReleaseLib();

	/*
	stream = fopen( "finputdata.txt", "a+" );
	fprintf(stream,"\n100%c\n",'_');
	map<string, string, less<string> > OutputParams;
	splitparam(szReturn, OutputParams);
	map<string, string, less<string> > ::iterator I;
	for(I=OutputParams.begin();I!=OutputParams.end();I++)
	{
		fprintf(stream,"%s=%s\n",I->first.c_str(),I->second.c_str());
	}

	fclose(stream);
	*/

	WriteTxt( "结果：", strMonitorID.c_str() );
	char* pszTemp = szReturn;
	while(*pszTemp)
	{
		WriteTxt( pszTemp, strMonitorID.c_str() );
		pszTemp += strlen(pszTemp) + 1;
	}

    return bReturn;
}

/////////////////////////////////////////////////////////////////////////////

extern "C" __declspec(dllexport)  int SNMPList(const char * strParas, char * szReturn, int& nSize)
{
    map<string, string, less<string> > InputParams;
    if(!splitparam(strParas, InputParams))
    {
        nSize = sprintf(szReturn , "error=some parameter is wrong");
        return FALSE;
    }

	/*
	FILE *stream;
	stream = fopen( "finputdata.txt", "a+" );
	fprintf(stream,"\n\nFunction:SNMPList:\n");
	map<string, string, less<string> >::iterator Item;
	for(Item=InputParams.begin();Item!=InputParams.end();Item++)
	{
		fprintf(stream,"%s=%s\n",Item->first.c_str(),Item->second.c_str());
	}
	fclose(stream);
	*/

    string strIP(""), strCommunity("");	
    int nTplID = 0, nPort = 161, nIfIndex = 0, nSnmpVer = 2;

    map<string, string, less<string> >::iterator paramItem;
    paramItem = InputParams.find(SV_Community);
    if(paramItem != InputParams.end())
        strCommunity = paramItem->second;

    paramItem = InputParams.find(SV_Host);
    if(paramItem != InputParams.end())
        strIP = paramItem->second; 

    paramItem = InputParams.find(SV_Port);
    if(paramItem != InputParams.end())
    {
        nPort = atoi(paramItem->second.c_str());
        if(nPort <= 0)
            nPort = 161;
    }

    paramItem = InputParams.find(SV_SNMPVersion);
    if(paramItem != InputParams.end())
    {
        nSnmpVer = atoi(paramItem->second.c_str());
        if(nSnmpVer <= 0)
            nSnmpVer = 2;
    }

    paramItem = InputParams.find(SV_TPLID);
    if(paramItem != InputParams.end())
        nTplID = atoi(paramItem->second.c_str());

    //paramItem = InputParams.find(SV_InterfaceIndex);
    //if(paramItem != InputParams.end())
    //    nIfIndex = atoi(paramItem->second.c_str());

    if(strIP.empty())
    {     
        std::string m_Ret = FuncGetStringFromIDS("SV_NETWORK_SET", "NETWORK_SET_IP_ADDRESS_NULL");
        sprintf(szReturn, "error=%s", (char*)m_Ret.c_str());
        return FALSE;
    }		

    char chSection[MAX_BUFF_LEN] = {0};//项
    string strIniFilePath("smi.ini");
    sprintf(chSection, "monitor-%d", nTplID);//Section
    //读取总数
    int nCount = FuncGetProfileIntBy(chSection, "showoid", (char*)strIniFilePath.c_str());
    if( nCount == 0)
    {//如果OID总数为零
        char szReturn[MAX_BUFF_LEN] = {0}; 
        std::string m_Ret =  FuncGetStringFromIDS("SV_SNMP_MONITOR", "SNMP_LIST_PARAM_ERROR");      
        sprintf(szReturn, "error=%s(tplmib-%d)$", m_Ret.c_str(), nTplID);
        return FALSE;
    }
    ///////////////////////////////////////////////////////////////////////// 
    SV_ECC_SNMP_LIB::InitLib(); 
    //BasicSNMP objSnmp;
    CSVSnmpOID objOID;
    objOID.SetIPAddress((char*)strIP.c_str());
    objOID.SetCommunity((char*)strCommunity.c_str());
    objOID.SetNetworkPort(nPort);
    objOID.SetVersion(nSnmpVer);

    if(objOID.InitSNMP() != 0)
    {
        std::string m_TempBuf = FuncGetStringFromIDS("IDS_InitSNMPFailed");
        sprintf(szReturn, "error=%s", (char*)m_TempBuf.c_str());
        return FALSE;
    }

    StringList lstValues;
    ////得到索引
    int bReturn = CheckOIDIndex(lstValues, (char*)strIniFilePath.c_str(), objOID, nTplID);	
	
	for(StringList::iterator pos=lstValues.begin();pos!=lstValues.end();pos++)
	{
	}

    if(bReturn != FALSE)
    {
        int nRealValue = FuncGetProfileIntBy(chSection, "index_sure", (char*)strIniFilePath.c_str());

        StringList lstTexts, lsRealValue, lsTemp;
        char chOID[MAX_BUFF_LEN] = {0}, key[256] = {0}, chFixLable[MAX_BUFF_LEN] = {0};
        int nResult = 0, nSubSize = 0, nSubStr = 0;
        string szTemp("");
        MonitorResult resultList;
        resultItem  resultIt;
        for(StringList::iterator pos = lstValues.begin(); pos != lstValues.end(); pos ++)
        {
            for(int i = 1; i <= nCount; i ++)
            {
	            sprintf(key, "showoid_substring%d", i);
                nSubStr = GetPrivateProfileInt1(chSection, key, 0, (char*)strIniFilePath.c_str());

	            memset(key, 0, 256);
	            sprintf(key, "showoid_subsize%d", i);
                nSubSize = GetPrivateProfileInt1(chSection, key, 0, (char*)strIniFilePath.c_str());

	            memset(key, 0, 256);
	            sprintf(key, "showoid_fixlable%d", i);
	            szTemp = FuncGetProfileStringBy(chSection, key, (char*)strIniFilePath.c_str());
	            if(szTemp.c_str() != "error")
		            strcpy(chFixLable, szTemp.c_str());

	            memset(key, 0, 256);
	            sprintf(key, "showoid_oid%d", i);
	            szTemp = FuncGetProfileStringBy(chSection, key, (char*)strIniFilePath.c_str());
                sprintf(chOID, "%s.%s",  szTemp.c_str(), (*pos).c_str());
                objOID.SetOIDValue(chOID);
                objOID.SetOIDType(0);

                resultList.clear();
                nResult = objOID.GetResult(resultList);//得到结果
                if(nResult == 0)
                {
                    resultIt = resultList.begin();

					/*
					char str[10000];

					FILE *stream;				
					sprintf(str,"%s\n%s:%s,%s\n",str,resultIt->second.m_szOID.c_str(),
											resultIt->second.m_szValue.c_str(),
											resultIt->second.m_szIndex.c_str());
					stream = fopen( "fread.txt", "a+" );
					fwrite( str, sizeof( char ), strlen(str), stream );
					fclose(stream);
					*/



                    if(resultIt != resultList.end())
                    {
                        string szValue(resultIt->second.m_szValue);
                        if(strlen(chFixLable) == 0)
                        {
                            if(nSubStr == 1)
                            {    
				                szValue = szValue.substr( 0, nSubSize);
                            }
                            else if(nSubStr == 2)
                            {
				                szValue = szValue.substr( nSubSize, szValue.size() - nSubSize);
                            }
                        }
                        else
                        {
                            szValue = chFixLable;
                        }
                        
		                lsTemp.push_back(szValue);
                        if(nRealValue == i)
                            lsRealValue.push_back(szValue);
                    }
	            }
            }  
            MakeLabel(lsTemp, lstTexts, chFixLable, nSubStr, nSubSize);
            lsTemp.clear();
        }

        if(nRealValue > 0)
        {
            lstValues.clear();
            for(StringList::iterator pos = lsRealValue.begin(); pos != lsRealValue.end() ; pos++)
	            lstValues.push_back(*pos);
        }

        if(lstTexts.size() == lstValues.size())
        {
            int nLen = 0;
			int nOneLen;
            StringList::iterator pos1 = lstValues.begin();
            for(StringList::iterator pos = lstTexts.begin(), pos1 = lstValues.begin() ; pos != lstTexts.end() || pos1 != lstValues.end(); pos++, pos1++)
            {
				nOneLen = static_cast<int>((*pos1).length())+(static_cast<int>((*pos).length()) + 2);
				if (nOneLen + nLen >nSize)
					break;

				memcpy(szReturn + nLen, (*pos1).c_str(), (*pos1).length());
                nLen += static_cast<int>((*pos1).length());
				memcpy(szReturn + nLen, "_", 1);
                nLen ++;
                memcpy(szReturn + nLen, (*pos).c_str(), (*pos).length());
                nLen += static_cast<int>((*pos).length());
                memcpy(szReturn + nLen, "=", 1);
                nLen ++;
				memcpy(szReturn + nLen, (*pos1).c_str(), (*pos1).length());
                nLen += static_cast<int>((*pos1).length());
				memcpy(szReturn + nLen, "_", 1);
                nLen ++;
                memcpy(szReturn + nLen, (*pos).c_str(), (*pos).length());
                nLen += (static_cast<int>((*pos).length()) + 1);

            }
            nSize = nLen;
        }
        else
        {
            bReturn = FALSE;
        }
    }
    SV_ECC_SNMP_LIB::ReleaseLib();

    ///////////////////////////////////////////////////////////////////////// 
	/*
	stream = fopen( "finputdata.txt", "a+" );
	fprintf(stream,"\n100%c\n",'_');
	map<string, string, less<string> > OutputParams;
	splitparam(szReturn, OutputParams);
	map<string, string, less<string> > ::iterator I;
	for(I=OutputParams.begin();I!=OutputParams.end();I++)
	{
		fprintf(stream,"%s=%s\n",I->first.c_str(),I->second.c_str());
	}
	*/


    return bReturn;
}

extern "C" __declspec(dllexport)  int SNMPProList(const char * strParas, char * szReturn, int& nSize)
{
    map<string, string, less<string> > InputParams;
    if(!splitparam(strParas, InputParams))
    {
        nSize = sprintf(szReturn , "error=some parameter is wrong");
        return FALSE;
    }

    string strIP(""), strCommunity("");	
    int nTplID = 0, nPort = 161, nIfIndex = 0, nSnmpVer = 2;

    map<string, string, less<string> >::iterator paramItem;
    paramItem = InputParams.find(SV_Community);
    if(paramItem != InputParams.end())
        strCommunity = paramItem->second;

    paramItem = InputParams.find(SV_Host);
    if(paramItem != InputParams.end())
        strIP = paramItem->second; 

    paramItem = InputParams.find(SV_Port);
    if(paramItem != InputParams.end())
    {
        nPort = atoi(paramItem->second.c_str());
        if(nPort <= 0)
            nPort = 161;
    }

    paramItem = InputParams.find(SV_SNMPVersion);
    if(paramItem != InputParams.end())
    {
        nSnmpVer = atoi(paramItem->second.c_str());
        if(nSnmpVer <= 0)
            nSnmpVer = 2;
    }

    paramItem = InputParams.find(SV_TPLID);
    if(paramItem != InputParams.end())
        nTplID = atoi(paramItem->second.c_str());

    if(strIP.empty())
    {     
        std::string m_Ret = FuncGetStringFromIDS("SV_NETWORK_SET", "NETWORK_SET_IP_ADDRESS_NULL");
        sprintf(szReturn, "error=%s", (char*)m_Ret.c_str());
        return FALSE;
    }		

    char chSection[MAX_BUFF_LEN] = {0};//项
    string strIniFilePath("smi.ini");
    sprintf(chSection, "monitor-%d", nTplID);//Section
    //读取总数
    int nCount = FuncGetProfileIntBy(chSection, "showoid", (char*)strIniFilePath.c_str());
    if( nCount == 0)
    {//如果OID总数为零
        char szReturn[MAX_BUFF_LEN] = {0}; 
        std::string m_Ret =  FuncGetStringFromIDS("SV_SNMP_MONITOR", "SNMP_LIST_PARAM_ERROR");      
        sprintf(szReturn, "error=%s(tplmib-%d)$", m_Ret.c_str(), nTplID);
        return FALSE;
    }
    ///////////////////////////////////////////////////////////////////////// 
    SV_ECC_SNMP_LIB::InitLib(); 
    //BasicSNMP objSnmp;
    CSVSnmpOID objOID;
    objOID.SetIPAddress((char*)strIP.c_str());
    objOID.SetCommunity((char*)strCommunity.c_str());
    objOID.SetNetworkPort(nPort);
    objOID.SetVersion(nSnmpVer);

    if(objOID.InitSNMP() != 0)
    {
        std::string m_TempBuf = FuncGetStringFromIDS("IDS_InitSNMPFailed");
        sprintf(szReturn, "error=%s", (char*)m_TempBuf.c_str());
        return FALSE;
    }

    StringList lstValues;
    ////得到索引
    int bReturn = CheckOIDIndex(lstValues, (char*)strIniFilePath.c_str(), objOID, nTplID);	
	
	for(StringList::iterator pos=lstValues.begin();pos!=lstValues.end();pos++)
	{
	}

    if(bReturn != FALSE)
    {
        int nRealValue = FuncGetProfileIntBy(chSection, "index_sure", (char*)strIniFilePath.c_str());

        StringList lstTexts, lsRealValue, lsTemp;
        char chOID[MAX_BUFF_LEN] = {0}, key[256] = {0}, chFixLable[MAX_BUFF_LEN] = {0};
        int nResult = 0, nSubSize = 0, nSubStr = 0;
        string szTemp("");
        MonitorResult resultList;
        resultItem  resultIt;
        for(StringList::iterator pos = lstValues.begin(); pos != lstValues.end(); pos ++)
        {
            for(int i = 1; i <= nCount; i ++)
            {
	            sprintf(key, "showoid_substring%d", i);
                nSubStr = GetPrivateProfileInt1(chSection, key, 0, (char*)strIniFilePath.c_str());

	            memset(key, 0, 256);
	            sprintf(key, "showoid_subsize%d", i);
                nSubSize = GetPrivateProfileInt1(chSection, key, 0, (char*)strIniFilePath.c_str());

	            memset(key, 0, 256);
	            sprintf(key, "showoid_fixlable%d", i);
	            szTemp = FuncGetProfileStringBy(chSection, key, (char*)strIniFilePath.c_str());
	            if(szTemp.c_str() != "error")
		            strcpy(chFixLable, szTemp.c_str());

	            memset(key, 0, 256);
	            sprintf(key, "showoid_oid%d", i);
	            szTemp = FuncGetProfileStringBy(chSection, key, (char*)strIniFilePath.c_str());
                sprintf(chOID, "%s.%s",  szTemp.c_str(), (*pos).c_str());
                objOID.SetOIDValue(chOID);
                objOID.SetOIDType(0);

                resultList.clear();
                nResult = objOID.GetResult(resultList);//得到结果
                if(nResult == 0)
                {
                    resultIt = resultList.begin();

                    if(resultIt != resultList.end())
                    {
                        string szValue(resultIt->second.m_szValue);
                        if(strlen(chFixLable) == 0)
                        {
                            if(nSubStr == 1)
                            {    
				                szValue = szValue.substr( 0, nSubSize);
                            }
                            else if(nSubStr == 2)
                            {
				                szValue = szValue.substr( nSubSize, szValue.size() - nSubSize);
                            }
                        }
                        else
                        {
                            szValue = chFixLable;
                        }
                        
		                lsTemp.push_back(szValue);
                        if(nRealValue == i)
                            lsRealValue.push_back(szValue);
                    }
	            }
            }  
            MakeLabel(lsTemp, lstTexts, chFixLable, nSubStr, nSubSize);
            lsTemp.clear();
        }

        if(nRealValue > 0)
        {
            lstValues.clear();
            for(StringList::iterator pos = lsRealValue.begin(); pos != lsRealValue.end() ; pos++)
	            lstValues.push_back(*pos);
        }

        if(lstTexts.size() == lstValues.size())
        {
            int nLen = 0;
			int nOneLen;
            StringList::iterator pos1 = lstValues.begin();
            for(StringList::iterator pos = lstTexts.begin(), pos1 = lstValues.begin() ; pos != lstTexts.end() || pos1 != lstValues.end(); pos++, pos1++)
            {
				nOneLen = static_cast<int>((*pos1).length())+(static_cast<int>((*pos).length()) + 2);
				if (nOneLen + nLen >nSize)
					break;

				memcpy(szReturn + nLen, (*pos).c_str(), (*pos).length());
				nLen += static_cast<int>((*pos).length());
				memcpy(szReturn + nLen, "=", 1);
				nLen ++;
				memcpy(szReturn + nLen, (*pos).c_str(), (*pos).length());
				nLen += (static_cast<int>((*pos).length()) + 1);

            }
            nSize = nLen;
        }
        else
        {
            bReturn = FALSE;
        }
    }
    SV_ECC_SNMP_LIB::ReleaseLib();

    return bReturn;
}

void MakeLabel(StringList &lsTemp, StringList &lsLable, char* strFixLable ,int nSelSub, int nSubSize)
{
    //POSITION posTmp, posLable, posLables;
	StringList::iterator posTmp;
	int nLableCount = static_cast<int>(lsTemp.size());    
	//int nTmpCount = lsTemp.size();    
	std::string strTmp("");
    if(nLableCount == 1)
    {
        for(posTmp = lsTemp.begin(); posTmp != lsTemp.end(); posTmp ++)
        {
			lsLable.push_back((*posTmp));
        }
    }
    else
    {
		for(posTmp = lsTemp.begin(); posTmp != lsTemp.end(); posTmp ++)
        {
			strTmp += *posTmp;  
			strTmp += "_";
            posTmp ++;
            strTmp += *posTmp;
        }
        lsLable.push_back(strTmp);
    }	
}

int CheckOIDIndex(StringList &lsIndex, char* strIniFilePath, CSVSnmpOID &objOID, int nTplID)
{
    int nResult = 0;
	char chTmp[MAX_BUFF_LEN] = {0}, chOID[MAX_BUFF_LEN] = {0}, chSection[MAX_BUFF_LEN] = {0},
        chValue[MAX_BUFF_LEN] = {0};

    StringList lsTemp;
	sprintf(chSection, "monitor-%d", nTplID);
    //索引值 OID
    std::string szTempBuf = FuncGetProfileStringBy(chSection, "index_oid", strIniFilePath);
    if(szTempBuf == "error")
        return FALSE;

    objOID.SetOIDValue(szTempBuf.c_str());
    //OID类型(0 简单变量;1 表格变量)
    objOID.SetOIDType(FuncGetProfileIntBy(chSection, "index_type", strIniFilePath) - 1);

    MonitorResult resultList;
    nResult = objOID.GetResult(resultList);//得到结果
    if(nResult == 0)
    {//	
        for(resultItem it = resultList.begin(); it != resultList.end(); it ++)
       	    lsTemp.push_back(it->second.m_szIndex);

		//sprintf(chSection, "monitor-%d", nTplID);
		szTempBuf = FuncGetProfileStringBy(chSection, "match_oid", strIniFilePath);
		memset(chOID, 0, MAX_BUFF_LEN);
		if(szTempBuf != "error")
		{
			strcpy(chOID, szTempBuf.c_str());
		}

	    if(strcmp(chOID , "") == 0 || strlen(chOID) <= 0)
	    {
			for(StringList::iterator pos = lsTemp.begin(); pos != lsTemp.end(); pos++)
            {//根据每一条索引值查询类型
				lsIndex.push_back((*pos));
            }
	    }
        else  if(strlen(chOID) > 0)
        {  		
		    szTempBuf = FuncGetProfileStringBy(chSection, "match_value", strIniFilePath);
            if(szTempBuf == "error")
                return FALSE;

            strcpy(chValue, szTempBuf.c_str());    
			for(StringList::iterator pos = lsTemp.begin(); pos != lsTemp.end(); pos++)
            {//根据每一条索引值查询类型
                szTempBuf = (*pos);
				memset(chTmp, 0, MAX_BUFF_LEN);
                sprintf(chTmp, "%s.%s", chOID, szTempBuf.c_str());
                objOID.SetOIDValue(chTmp);
                objOID.SetOIDType(0);
                resultList.clear();
                nResult = objOID.GetResult(resultList); 
                if(nResult == 0)
                {
                    resultItem resultIt = resultList.begin();
                    if(resultIt != resultList.end())
                    {
                        if(strcmp(chValue, resultIt->second.m_szValue.c_str()) == 0)
						    lsIndex.push_back(szTempBuf);
                    }
                }
                else
                {
                    return FALSE;
                }
            }
        }
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

extern "C" __declspec(dllexport)  bool INTERFACES(const char * strParas, char *pszReturn, int & nSize)
{
    map<string, string, less<string> > InputParams;
    if(!splitparam(strParas, InputParams))
    {
        nSize = sprintf(pszReturn , "error=some parameter is wrong");
        return false;
    }

	/*
	FILE *stream;
	stream = fopen( "finputdata.txt", "a+" );
	fprintf(stream,"\n\nFunction:INTERFACES:\n");
	map<string, string, less<string> >::iterator Item;
	for(Item=InputParams.begin();Item!=InputParams.end();Item++)
	{
		fprintf(stream,"%s=%s\n",Item->first.c_str(),Item->second.c_str());
	}
	fclose(stream);
	*/


    string strIP(""), strCommunity("");
	int nPort = 161, nSnmpVer = 1;

    map<string, string, less<string> >::iterator paramItem;
    paramItem = InputParams.find(SV_Community);
    if(paramItem != InputParams.end())
        strCommunity = paramItem->second;

    paramItem = InputParams.find(SV_Host);
    if(paramItem != InputParams.end())
        strIP = paramItem->second; 

    paramItem = InputParams.find(SV_Port);
    if(paramItem != InputParams.end())
    {
        nPort = atoi(paramItem->second.c_str());
        if(nPort <= 0)
            nPort = 161;
    }

    paramItem = InputParams.find(SV_SNMPVersion);
    if(paramItem != InputParams.end())
    {
        nSnmpVer = atoi(paramItem->second.c_str());
        if(nSnmpVer <= 0)
            nSnmpVer = 2;
    }

    if(strIP.empty())
    {
        std::string m_Ret = FuncGetStringFromIDS("SV_NETWORK_SET", "NETWORK_SET_IP_ADDRESS_NULL");
        sprintf(pszReturn, "error=%s", (char*)m_Ret.c_str());
        return FALSE;
    }

    ///////////////////////////////////////////////////////////////////////// 
    SV_ECC_SNMP_LIB::InitLib(); 
    //BasicSNMP objSnmp;
    CSVSnmpOID objOID;
    objOID.SetIPAddress((char*)strIP.c_str());
    objOID.SetCommunity((char*)strCommunity.c_str());
    objOID.SetNetworkPort(nPort);
    objOID.SetVersion(nSnmpVer);

    if(objOID.InitSNMP() != 0)
    {
        std::string m_TempBuf = FuncGetStringFromIDS("IDS_InitSNMPFailed");
        sprintf(pszReturn, "error=%s", (char*)m_TempBuf.c_str());
		ofstream fout("snmperror.log",ios::app);
		fout << pszReturn <<"\r\n"; 
		fout << flush; 
		fout.close(); 
        return false;
    }

    objOID.SetOIDValue("1.3.6.1.2.1.2.2.1.1");
    objOID.SetOIDType(1);

    MonitorResult resultList;
    map<int, string, less<int> > lsInterfaces;
    map<int, string, less<int> >::iterator itInterfaces;
    resultList.clear();
    int nResult = objOID.GetResult(resultList);//得到结果
    if(nResult == 0)
    {
        resultItem resultIt;
        for(resultIt = resultList.begin(); resultIt != resultList.end(); resultIt ++)
        {
            lsInterfaces[atoi(resultIt->second.m_szIndex.c_str())] = resultIt->second.m_szValue;
        }

        resultList.clear();

        objOID.SetOIDValue("1.3.6.1.2.1.2.2.1.2");
        objOID.SetOIDType(1);

        nResult = objOID.GetResult(resultList);//得到结果
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

            objOID.SetOIDValue("1.3.6.1.2.1.2.2.1.3");
            objOID.SetOIDType(1);

            nResult = objOID.GetResult(resultList);//得到结果
            if(nResult == 0)
            {
                for(resultIt = resultList.begin(); resultIt != resultList.end(); resultIt ++)
                {
                    itInterfaces = lsInterfaces.find(atoi(resultIt->second.m_szIndex.c_str()));
                    if(itInterfaces != lsInterfaces.end())
                    {
                        unsigned long nifType = (unsigned long)atoi(resultIt->second.m_szValue.c_str());
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

                objOID.SetOIDValue("1.3.6.1.2.1.2.2.1.8");
                objOID.SetOIDType(1);

                nResult = objOID.GetResult(resultList);//得到结果
                if(nResult == 0)
                {
                    for(resultIt = resultList.begin(); resultIt != resultList.end(); resultIt ++)
                    {
                        itInterfaces = lsInterfaces.find(atoi(resultIt->second.m_szIndex.c_str()));
                        if(itInterfaces != lsInterfaces.end())
                        {
                            unsigned long nifType = (unsigned long)atoi(resultIt->second.m_szValue.c_str());

                            itInterfaces->second += "_";
				            itInterfaces->second += ( nifType == 1 ? "up":
								                        nifType == 2 ? "down":
								                        nifType == 3 ? "testing" : "unknown");
                        }
                    }

                    resultList.clear();

                    objOID.SetOIDValue("1.3.6.1.2.1.31.1.1.1.1");
                    objOID.SetOIDType(1);

                    nResult = objOID.GetResult(resultList);//得到结果
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
					else
					{
						ofstream fout("snmperror.log",ios::app);
						fout << "Get 1.3.6.1.2.1.31.1.1.1.1 failed" <<"\r\n"; 
						fout << flush; 
						fout.close(); 
					}
                }
				else
				{
					ofstream fout("snmperror.log",ios::app);
					fout << "Get 1.3.6.1.2.1.2.2.1.8 failed" <<"\r\n"; 
					fout << flush; 
					fout.close(); 
				}
            }
			else
			{
				ofstream fout("snmperror.log",ios::app);
				fout << "Get 1.3.6.1.2.1.2.2.1.3 failed" <<"\r\n"; 
				fout << flush; 
				fout.close(); 
			}
        }
		else
		{
			ofstream fout("snmperror.log",ios::app);
			fout << "Get 1.3.6.1.2.1.2.2.1.2 failed" <<"\r\n"; 
			fout << flush; 
			fout.close(); 
		}
    }
	else
	{
		ofstream fout("snmperror.log",ios::app);
		fout << "Get 1.3.6.1.2.1.2.2.1.1 failed" <<"\r\n"; 
		fout << flush; 
		fout.close(); 
	}

    char *pPosition = pszReturn;
    int nWritten = 0;
    int nLen = 0;
	int nOneLen=0;
	char tmpchar[50]={0};
    for(itInterfaces = lsInterfaces.begin(); itInterfaces != lsInterfaces.end(); itInterfaces ++)
    {
		nLen = pPosition- pszReturn;
		//itoa(itInterfaces->first,tmpchar,49);
		sprintf(tmpchar ,"%d", itInterfaces->first);
		//nOneLen = strlen(tmpchar) + itInterfaces->second.size()+2;
		nOneLen = strlen(tmpchar) + strlen(tmpchar) + 2;
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
//备注:
//在SNMPInterfaceStatus(StringList &paramList, char *szReturn)函数中屏蔽了INTERFACES函数

extern "C" __declspec(dllexport)  int SNMPListForPro(const char * strParas, char * szReturn, int& nSize)
{
  map<string, string, less<string> > InputParams;
  if(!splitparam(strParas, InputParams))
  {
    nSize = sprintf(szReturn , "error=some parameter is wrong");
    return FALSE;
  }

  
  /*
  FILE *stream;
	stream = fopen( "finputdata.txt", "a+" );
	fprintf(stream,"\n\nFunction:SNMPListForPro:\n");
	map<string, string, less<string> >::iterator Item;
	for(Item=InputParams.begin();Item!=InputParams.end();Item++)
	{
		fprintf(stream,"%s=%s\n",Item->first.c_str(),Item->second.c_str());
	}
	fclose(stream);
	*/



  string strIP(""), strCommunity("");	
  int nTplID = 0, nPort = 161, nIfIndex = 0, nSnmpVer = 2;

  map<string, string, less<string> >::iterator paramItem;
  paramItem = InputParams.find(SV_Community);
  if(paramItem != InputParams.end())
    strCommunity = paramItem->second;

  paramItem = InputParams.find(SV_Host);
  if(paramItem != InputParams.end())
    strIP = paramItem->second; 

  paramItem = InputParams.find(SV_Port);
  if(paramItem != InputParams.end())
  {
    nPort = atoi(paramItem->second.c_str());
    if(nPort <= 0)
      nPort = 161;
  }

  paramItem = InputParams.find(SV_SNMPVersion);
  if(paramItem != InputParams.end())
  {
    nSnmpVer = atoi(paramItem->second.c_str());
    if(nSnmpVer <= 0)
      nSnmpVer = 2;
  }

  paramItem = InputParams.find(SV_TPLID);
  if(paramItem != InputParams.end())
    nTplID = atoi(paramItem->second.c_str());

  //paramItem = InputParams.find(SV_InterfaceIndex);
  //if(paramItem != InputParams.end())
  //    nIfIndex = atoi(paramItem->second.c_str());

  if(strIP.empty())
  {     
    std::string m_Ret = FuncGetStringFromIDS("SV_NETWORK_SET", "NETWORK_SET_IP_ADDRESS_NULL");
    sprintf(szReturn, "error=%s", (char*)m_Ret.c_str());
    return FALSE;
  }		

  char chSection[MAX_BUFF_LEN] = {0};//项
  string strIniFilePath("smi.ini");
  sprintf(chSection, "monitor-%d", nTplID);//Section
  //读取总数
  int nCount = FuncGetProfileIntBy(chSection, "showoid", (char*)strIniFilePath.c_str());
  if( nCount == 0)
  {//如果OID总数为零
    char szReturn[MAX_BUFF_LEN] = {0}; 
    std::string m_Ret =  FuncGetStringFromIDS("SV_SNMP_MONITOR", "SNMP_LIST_PARAM_ERROR");      
    sprintf(szReturn, "error=%s(tplmib-%d)$", m_Ret.c_str(), nTplID);
    return FALSE;
  }
  ///////////////////////////////////////////////////////////////////////// 
  SV_ECC_SNMP_LIB::InitLib(); 
  //BasicSNMP objSnmp;
  CSVSnmpOID objOID;
  objOID.SetIPAddress((char*)strIP.c_str());
  objOID.SetCommunity((char*)strCommunity.c_str());
  objOID.SetNetworkPort(nPort);
  objOID.SetVersion(nSnmpVer);

  if(objOID.InitSNMP() != 0)
  {
    std::string m_TempBuf = FuncGetStringFromIDS("IDS_InitSNMPFailed");
    sprintf(szReturn, "error=%s", (char*)m_TempBuf.c_str());
    return FALSE;
  }

  StringList lstValues;
  ////得到索引
  int bReturn = CheckOIDIndex(lstValues, (char*)strIniFilePath.c_str(), objOID, nTplID);	
  if(bReturn != FALSE)
  {
    int nRealValue = FuncGetProfileIntBy(chSection, "index_sure", (char*)strIniFilePath.c_str());

    StringList lstTexts, lsRealValue, lsTemp;
    char chOID[MAX_BUFF_LEN] = {0}, key[256] = {0}, chFixLable[MAX_BUFF_LEN] = {0};
    int nResult = 0, nSubSize = 0, nSubStr = 0;
    string szTemp("");
    MonitorResult resultList;
    resultItem  resultIt;
    for(StringList::iterator pos = lstValues.begin(); pos != lstValues.end(); pos ++)
    {
      for(int i = 1; i <= nCount; i ++)
      {
        sprintf(key, "showoid_substring%d", i);
        nSubStr = GetPrivateProfileInt1(chSection, key, 0, (char*)strIniFilePath.c_str());

        memset(key, 0, 256);
        sprintf(key, "showoid_subsize%d", i);
        nSubSize = GetPrivateProfileInt1(chSection, key, 0, (char*)strIniFilePath.c_str());

        memset(key, 0, 256);
        sprintf(key, "showoid_fixlable%d", i);
        szTemp = FuncGetProfileStringBy(chSection, key, (char*)strIniFilePath.c_str());
        if(szTemp.c_str() != "error")
          strcpy(chFixLable, szTemp.c_str());

        memset(key, 0, 256);
        sprintf(key, "showoid_oid%d", i);
        szTemp = FuncGetProfileStringBy(chSection, key, (char*)strIniFilePath.c_str());
        sprintf(chOID, "%s.%s",  szTemp.c_str(), (*pos).c_str());
        objOID.SetOIDValue(chOID);
        objOID.SetOIDType(0);

        resultList.clear();
        nResult = objOID.GetResult(resultList);//得到结果
        if(nResult == 0)
        {
          resultIt = resultList.begin();
          if(resultIt != resultList.end())
          {
            string szValue(resultIt->second.m_szValue);
            if(strlen(chFixLable) == 0)
            {
              if(nSubStr == 1)
              {    
                szValue = szValue.substr( 0, nSubSize);
              }
              else if(nSubStr == 2)
              {
                szValue = szValue.substr( nSubSize, szValue.size() - nSubSize);
              }
            }
            else
            {
              szValue = chFixLable;
            }

            lsTemp.push_back(szValue);
            if(nRealValue == i)
              lsRealValue.push_back(szValue);
          }
        }
      }  
      MakeLabel(lsTemp, lstTexts, chFixLable, nSubStr, nSubSize);
      lsTemp.clear();
    }

    if(nRealValue > 0)
    {
      lstValues.clear();
      for(StringList::iterator pos = lsRealValue.begin(); pos != lsRealValue.end() ; pos++)
        lstValues.push_back(*pos);
    }

    if(lstTexts.size() == lstValues.size())
    {
      int nLen = 0;
      int nOneLen;
      StringList::iterator pos1 = lstValues.begin();
      for(StringList::iterator pos = lstTexts.begin(), pos1 = lstValues.begin() ; pos != lstTexts.end() || pos1 != lstValues.end(); pos++, pos1++)
      {
        nOneLen = static_cast<int>((*pos1).length())+(static_cast<int>((*pos).length()) + 2);
        if (nOneLen + nLen >nSize)
          break;

        
        memcpy(szReturn + nLen, (*pos).c_str(), (*pos).length());
        nLen += static_cast<int>((*pos).length());
        memcpy(szReturn + nLen, "=", 1);
        nLen ++;
        memcpy(szReturn + nLen, (*pos).c_str(), (*pos).length());
        nLen += (static_cast<int>((*pos).length()) + 1);

      }
      nSize = nLen;
    }
    else
    {
      bReturn = FALSE;
    }
  }
  SV_ECC_SNMP_LIB::ReleaseLib();

  ///////////////////////////////////////////////////////////////////////// 
  return bReturn;
}
