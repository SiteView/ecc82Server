// SnmpMonitor.cpp : Defines the initialization routines for the DLL.
//

//#include "afxcoll.h"
#include "TelnetCisco.h"
#include "SnmpMonitor.h"
//#include "stlini.h"
#include "Time.h"
#include <stdlib.h>
#include <fstream>
#include <atltime.h>
#include <vector>
#include "Base/SVDefines.h"
//#include "windows.h"

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


HANDLE hMutex;	//
HANDLE hEvent;

typedef struct 
{
	CString strServer;
	CString strDevID;
	CString strPwd;
	CString strPwdPrompt;
	CString strCMD;
	CString strCmdPrompt;
	int nPort;
	bool bAlive;// 线程存在标志

} PARM ;

//===============================================================
//打印日志函数(zou_xiao)
//===============================================================
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

int GetConfirms(CString strMonitorID, int nDataSize);
CTime StrToDate(CString sDateTime);
bool WriteLog(CString strFileName,const CString strLog);
BOOL TelnetSaveFile(PARM* lpParm);
bool MonitorDataParse(char* databuf, const char* szCMD , StringList& pList);

void myPrintLog(const char *LogMes)
{
	//return;
	char timebuf[128],datebuf[128];

	_tzset();
	_strtime( timebuf );
	_strdate( datebuf );

	FILE *fp = NULL;
	if ((fp=fopen("dyMonitorSnmpError.log", "a"))==NULL)
	{
		printf("file not find ,then create it ..\r\n");
		return ;
	}

	fprintf(fp, "%s %s \t%s\n", datebuf, timebuf, LogMes );

	fclose(fp);
}

void WriteLog( const char* str )
{
	return;
	char timebuf[128],datebuf[128];

	_tzset();
	_strtime( timebuf );
	_strdate( datebuf );

	char szLogFile[] = "snmperror.log";

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
	return;//-_-! 

	char timebuf[128],datebuf[128];

	_tzset();
	_strtime( timebuf );
	_strdate( datebuf );

	char szProgramName[] = "Snmp.log";
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
	else
	{
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

bool MakeCharByString(char *pOut,int &nOutSize,CString strInput )
{
	char *p;

	int nSize=strInput.GetLength();
	if(nSize+2 <nOutSize)
	{
		strcpy(pOut,strInput.GetBuffer(strInput.GetLength()));
	}else return false;
	p=pOut;
	//printf("%d\n",nSize);23028830 13602067678 王波
	for(int i=0;i<nSize;i++)
	{
		if(*p=='$') 	
			*p='\0';
		p++;
	}
	nOutSize=nSize+1;
	return true;

}

extern "C" __declspec(dllexport)
int SNMPDone(const char * strParas, char * szReturn, int& nSize)
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

	int nTplID = 0, nPort = 161, nSnmpVer = 2, 
	//int	nTimeout = 300;//, nIfIndex = 0;   //nTimeout = 300;原来的
	int	nTimeout = 2000; //yi.duan 上海检察院2010-06-07

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
		{    
			nTimeout = 300; 
		}
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



	//bin.liu 0 的话补值
	if(nTplID==433)
	{
		
		char chTempFile[MAX_BUFF_LEN] = {0};
		sprintf(chTempFile, "snmpresult_%s.ini", strMonitorID.c_str());
        char oldvalue1[MAX_BUFF_LEN];
		char oldvalue2[MAX_BUFF_LEN];
		//GetPrivateProfileString1("", "HistoryValue", "13", pOidList[i].chHisValue, MAX_BUFF_LEN, chTempFile);
		GetPrivateProfileString1("result", "ifInOctetsRate", "13", oldvalue1, MAX_BUFF_LEN, chTempFile);
		GetPrivateProfileString1("result", "ifOutOctetsRate", "12.00", oldvalue2, MAX_BUFF_LEN, chTempFile);
		const char * p;
		//char * pszReturn;
	   // memset(pszReturn,0,100000);
		//char * p;
		int nSize1 = 0;
		p=szReturn;
		//pszReturn=szReturn;
		while(*p!='\0')
		{
			nSize1 += strlen(p) + 1;
			p += strlen(p)+1;
		}

		//将'\0' 替换为'$'
		CString strOut= _T("");
		CString allstrOut= _T("");
		CString temptrOut= _T("");
		char * value1;
		char * value2;
		int poss=0;
		for (int j = 0;j<nSize1; j++)
		{
			if(szReturn[j]=='\0')
			{
				
				poss=strOut.Find("ifInOctetsRate");// ifInOctetsRate
				
				if(poss==0)
				{
				  poss=strOut.Find("=");
				  CString tempp=strOut.Mid(poss+1,strOut.GetLength()-poss-1);
				  printf("dddddddddddddddddddd   %s \n",tempp.GetBuffer(tempp.GetLength()));
				  if(tempp!="0.00")
				  {
				   value1=tempp.GetBuffer(tempp.GetLength());
				   WritePrivateProfileString1("result", "ifInOctetsRate",  value1, chTempFile );
				  }else
				  {
					 
					  temptrOut=oldvalue1;
					   printf("dddddddddddddddddddd   %s \n",temptrOut.GetBuffer(temptrOut.GetLength()));
					  strOut="ifInOctetsRate="+temptrOut; 
					   printf("dddddddddddddddddddd   %s \n",strOut.GetBuffer(strOut.GetLength()));
				  }

				}
				 poss=strOut.Find("ifOutOctetsRate");
				if(poss==0)
				{
				  poss=strOut.Find("=");
				  CString tempp=strOut.Mid(poss+1,strOut.GetLength()-poss-1);
				  if(tempp!="0.00")
				  {
				  value2=tempp.GetBuffer(tempp.GetLength());
				  WritePrivateProfileString1("result", "ifOutOctetsRate", value2, chTempFile );
				  }else
				  {
                      temptrOut=oldvalue2;
					  strOut="ifOutOctetsRate="+temptrOut; 
				  }

				}
				/*if (strOut.Mid("ifInOctetsRate")!=-1)
				{
                  value1="1";
				}*/
				allstrOut+=strOut;
				allstrOut+="$";
				
				strOut="";
			}else
			{
				strOut+=szReturn[j];
			}
		}
		//GetBuffer(strTest.GetLength());
		
		printf("dddddddddddddddddddd   %s",allstrOut);
		sprintf(szReturn,allstrOut);
	   char * strEnd = szReturn;
		while(*strEnd)
	   {
		if(*strEnd == '$')
			*strEnd = '\0';
		strEnd++;
	   }
		//bool dd= MakeCharByString(szReturn,nSize1,strInput);	
		//printf("dddddddddddddddddddd   %d",dd);

      
	}

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

//字符串分割函数
 std::vector<std::string> split(std::string str,std::string pattern)
 {
     std::string::size_type pos;
     std::vector<std::string> result;
     str+=pattern;//扩展字符串以方便操作
     int size=str.size();
 
     for(int i=0; i<size; i++)
     {
         pos=str.find(pattern,i);
         if(pos<size)
         {
             std::string s=str.substr(i,pos-i);
             result.push_back(s);
             i=pos+pattern.size()-1;
         }
     }
     return result;
 }

extern "C" __declspec(dllexport)
int SNMPProcListDone(const char * strParas, char * szReturn, int& nSize)
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

	int nTplID = 0, nPort = 161, nSnmpVer = 2, 
	//int	nTimeout = 300;//, nIfIndex = 0;   //nTimeout = 300;原来的
	int	nTimeout = 2000; //yi.duan 上海检察院2010-06-07

    map<string, string, less<string> > InputParams;
    map<string, string, less<string> >::iterator paramItem;

    if(!splitparam(strParas, InputParams))
    {
        string szErrMsg = FuncGetStringFromIDS("IDS_MONITOR_PARAM_ERROR");
        nSize = sprintf(szReturn , "error=some parameter is wrong");//, FuncGetStringFromIDS("IDS_MONITOR_PARAM_ERROR"));
        return FALSE;
    }

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
		{    
			nTimeout = 300; 
		}
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

	std::vector<std::string> strProcList = split(strSelValue.c_str(), ",");
	CString strProc = "", strTmp = "";
	int nProcCount = 0;
	
	SV_ECC_SNMP_LIB::InitLib();

	for(int i = 0; i< strProcList.size(); i ++ )
	{
		CSnmpOperate snmpOperate(strIniFilePath.c_str(), strMonitorID.c_str(), 
			strIP.c_str(), strCommunity.c_str(), strIndex.c_str(), strProcList[i].c_str(),
			nTplID, nPort, nSnmpVer, nTimeout);

		snmpOperate.GetResult(szReturn, nSize);
		strTmp="";
		strTmp.Format("%s", szReturn);
		strTmp.Replace("ProcessCount=", "");
		strTmp.Replace(".00", "");
		nProcCount += atoi(strTmp);
		strProc += strProcList[i].c_str();
		strProc += ":" ;
		strProc += strTmp;
		strProc += " " ;
		
	}

    SV_ECC_SNMP_LIB::ReleaseLib();

	CString strInput ;

	strInput.Format("ProcessCount=%d$Dstr=%s", nProcCount, strProc);
	nSize = strInput.GetLength() + 3;
	//strInput =szReturn;
	MakeCharByString(szReturn,nSize,strInput);

/*	WriteTxt( "结果：", strMonitorID.c_str() );
	char* pszTemp = szReturn;
	while(*pszTemp)
	{
		WriteTxt( pszTemp, strMonitorID.c_str() );
		pszTemp += strlen(pszTemp) + 1;
	}
*/
    return bReturn;
}


/////////////////////////////////////////////////////////////////////////////

extern "C" __declspec(dllexport) 
int SNMPList(const char * strParas, char * szReturn, int& nSize)
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
            for(int i=1; i <= nCount; i ++)
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
				cout << "-------------- GetResult Start ------------------"<<endl;
                nResult = objOID.GetResult(resultList);//得到结果
				cout << "-------------- GetResult End ------------------"<<endl;
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

						cout << "szValue = " << szValue << endl;

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
                        cout << "szValue2 = " << szValue << endl;
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

extern "C" __declspec(dllexport) 
int SNMPProList(const char * strParas, char * szReturn, int& nSize)
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

extern "C" __declspec(dllexport)  
bool INTERFACES(const char * strParas, char *pszReturn, int & nSize)
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

        objOID.SetOIDValue("1.3.6.1.2.1.2.2.1.2"); //description
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



bool MonitorParse(char* databuf, const char* szCMD , StringList& pList, char* szReturn  )
{
	CString strData = databuf;
	CString strFind="";
	CString strVal = "";
	int nPos =-1, nPrePos =-1;
	char* szVal;
	

	if (strstr(databuf, szCMD))
	{
		strFind.Format("%s is",szCMD);
		nPos = strData.Find(strFind,0);
		if (nPos == -1) //接口状态不存在
		{
			return false;
		}
		nPrePos = nPos + strFind.GetLength();
		nPos = strData.Find(",",nPrePos);
		strVal = strData.Mid(nPrePos, nPos - nPrePos);
		strVal.TrimLeft();
		strVal.TrimRight();
		szVal = strVal.GetBuffer();
		pList.push_back(szVal);


		strFind.Format("line protocol is");//连接状态
		nPos = strData.Find(strFind, nPos);
		if(nPos == -1)
		{
			return false;
		}
		nPrePos = nPos + strFind.GetLength();
		nPos = strData.Find("\r\n",nPrePos);
		strVal = strData.Mid(nPrePos, nPos - nPrePos);
		strVal.TrimLeft();
		strVal.TrimRight();
		szVal = strVal.GetBuffer();
		pList.push_back(szVal);

		strFind.Format("packets input,");//接收字节
		nPos = strData.Find(strFind,nPos);
		if(nPos == -1)
		{
			return false;
		}
		nPrePos = nPos + strFind.GetLength();
		nPos = strData.Find("bytes",nPrePos);
		strVal = strData.Mid(nPrePos, nPos - nPrePos);
		strVal.TrimLeft();
		strVal.TrimRight();
		szVal = strVal.GetBuffer();
		pList.push_back(szVal);

		strFind.Format("packets output,");//发送字节
		nPos = strData.Find(strFind,nPos);
		if(nPos == -1)
		{
			return false;
		}
		nPrePos = nPos + strFind.GetLength();
		nPos = strData.Find("bytes",nPrePos);
		strVal = strData.Mid(nPrePos, nPos - nPrePos);
		strVal.TrimLeft();
		strVal.TrimRight();
		szVal = strVal.GetBuffer();
		pList.push_back(szVal);

	}
	else
	{
		// sprintf(szReturn,"error");
		return false;
	}
	return true;
}

extern "C" __declspec(dllexport)  bool INTERFACESTest(const char * strParas, char *strReturn, int & nSize)
{
	CString strFileName = "INTERFACESTest";
	CString strLog = "";
	//const char szInPut[1024]=_T("_Community=public1\0_MachineName=192.168.0.251\0_Port=161\0Version=2\0_MonitorID=1.22.7.67\0\0");
	map<string, string, less<string> > InputParams;
	if(!splitparam(strParas, InputParams))
	{
		nSize = sprintf(strReturn , "error=some parameter is wrong");
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
	cout << "===================" << endl; 
	cout << "===================" << endl;
	cout << "===================" << endl;
	cout << "===================" << endl;
	cout << "===================" << endl;
	cout << "===================" << endl;
	cout << "===================" << endl;
	cout << "===================" << endl;
	cout << "===================" << endl;

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
		sprintf(strReturn, "error=%s", (char*)m_Ret.c_str());
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
		sprintf(strReturn, "error=%s", (char*)m_TempBuf.c_str());
		ofstream fout("snmperror.log",ios::app);
		fout << strReturn <<"\r\n"; 
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
					itInterfaces->second = resultIt->second.m_szValue;

				}
			}

			resultList.clear();


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

	char *pPosition = strReturn;
	int nWritten = 0;
	int nLen = 0;
	int nOneLen=0;
	char tmpchar[50]={0};
	for(itInterfaces = lsInterfaces.begin(); itInterfaces != lsInterfaces.end(); itInterfaces ++)
	{
		nLen = pPosition- strReturn;
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
	//sprintf(strReturn,"Size=%d\0\0",strlen(strReturn));

	SV_ECC_SNMP_LIB::ReleaseLib();
	return true;
}


bool WriteLog(CString strFileName,const CString strLog)
{
	return true;
	//char szDataBuf[128]		= _T("");
	//char szTimeBuf[128]		= _T("");
	//char szTempBuf[1024*10] = _T("");
	//_strdate(szDataBuf);
	//_strtime(szTimeBuf);
	//sprintf(szTempBuf,"%s-%s",szDataBuf,szTimeBuf);

	//CString szFileName="";
	//char szFileDirectory[MAX_PATH]={};
	//GetCurrentDirectory(MAX_PATH, szFileDirectory);
	//szFileName.Format("%s\\TestLog",szFileDirectory);
	//CreateDirectory(szFileName,NULL);
	//szFileName.Format("%s\\%s.log",szFileName,strFileName);
	////szFileName.Format("%s.log",strFileName);

	//struct _stat buf;
	//if( _stat(szFileName.GetBuffer(szFileName.GetLength()), &buf ) == 0 )
	//{
	//	if( buf.st_size > 1000*1024 )  //文件过大
	//	{
	//		FILE *log = fopen( szFileName.GetBuffer(szFileName.GetLength()), "w" );
	//		if( log != NULL )
	//		{
	//			fclose( log );
	//		}
	//	}
	//}

	//ofstream stFileStream;
	//stFileStream.open(szFileName, ios::app);
	//stFileStream<<szTempBuf<<"\t"<<strLog<<endl;
	//stFileStream.close();
	//return true;
}

extern "C" __declspec(dllexport)  BOOL TelnetInteface(const char * strParas, char *strReturn, int & nSize)
{
	int bReturn = TRUE;
	std::string strIP ;
	std::string	strCommunity ;
	std::string	strMonitorID ;
	std::string strIniFilePath ;
	std::string	strIndex;
	std::string	strSelValue;
	std::string strValue ;
	std::string strPWD;
	std::string pwdPrompt;
	std::string cmdPrompt;

	cout << "============== TelnetInteface Start ===================" <<endl;
	
	int nTplID = 0, nPort = 23, nSnmpVer = 2, nTimeout = 300, nTelPort=23;//, nIfIndex = 0;

	map<string, string, less<string> > InputParams;
	map<string, string, less<string> >::iterator paramItem;
	if(!splitparam(strParas, InputParams))
	{
		string szErrMsg = FuncGetStringFromIDS("IDS_MONITOR_PARAM_ERROR");
		nSize = sprintf(strReturn , "error=some parameter is wrong");//, FuncGetStringFromIDS("IDS_MONITOR_PARAM_ERROR"));
		return FALSE;
	}
	cout << "================"<<endl;
	paramItem = InputParams.find(SV_MonitorID);

	if(paramItem != InputParams.end())
		strMonitorID = paramItem->second;


	//paramItem = InputParams.find(SV_Community);
	//if(paramItem != InputParams.end())
	//	strCommunity = paramItem->second;

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

	//paramItem = InputParams.find(SV_SNMPDisk);
	//if(paramItem != InputParams.end())
	//	strIndex = paramItem->second; 
	/////////解决进程显示问题
	//paramItem = InputParams.find(SV_SNMPSelvalue);
	//if(paramItem != InputParams.end())
	//{		
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

	//}
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

	//paramItem = InputParams.find(SV_SNMPVersion);
	//if(paramItem != InputParams.end())
	//{
	//	nSnmpVer = atoi(paramItem->second.c_str());
	//	if(nSnmpVer <= 0)
	//		nSnmpVer = 2;
	//}

	paramItem = InputParams.find(SV_TPLID);
	if(paramItem != InputParams.end())
		nTplID = atoi(paramItem->second.c_str());

	/*lll paramItem = InputParams.find(SV_TelPort);
	if (paramItem != InputParams.end())
	{
		nTelPort = atoi(paramItem->second.c_str());
	}*/

	/*paramItem = InputParams.find(SV_TelPassword);
	if(paramItem != InputParams.end())
		strPWD = paramItem->second;*/

	/*paramItem = InputParams.find(SV_TelPrompt);
	if(paramItem != InputParams.end())
		pwdPrompt = paramItem->second;*/


	//paramItem = InputParams.find(SV_PromptCMD);
	//if(paramItem != InputParams.end())
	//	cmdPrompt = paramItem->second;

	//StringList::iterator pos;
	if(strIP.empty())
	{
		std::string m_Ret = FuncGetStringFromIDS("SV_NETWORK_SET", "NETWORK_SET_IP_ADDRESS_NULL");
		sprintf(strReturn, "error=%s", m_Ret.c_str());
		nSize = static_cast<int>(strlen(strReturn));
		return FALSE;
	}

	if (strPWD.empty())
	{
		sprintf(strReturn,"error=密码不能为空！\0");
		nSize = static_cast<int>(strlen(strReturn)); 
		return FALSE;
	}

	//设备ID
	CString strDevID= strMonitorID.c_str();
	nPos=0;
	cout << "strDevID :"<<strDevID <<endl;
	for (int i=0; i<3; i++)
	{

		nPos =strDevID.Find(".",nPos+1);
		cout <<"nPos =" <<nPos << endl;
	}
	if (nPos != -1)
	{
		strDevID =  strDevID.Mid(0,nPos);
	}

	cout << "strDevID :"<<strDevID <<endl;

	CString strFile = "";
	strFile.Format("TelnetInteface%s.log",strMonitorID.c_str());

	//cout << "------------- Telnet Start ------------"<< endl;
	LPGDATA pgd=new GDATA;
	if (pgd == NULL)
	{
		sprintf(strReturn, "error=内存分配失败！\0");
		return FALSE ;
	}
	//cout << "pgd->term_index = " << pgd->term_index <<endl;
	//cout << "pgd->databuffer = " << pgd->databuffer <<endl;
	
	int count = 0;

LOOP:
	memset(pgd,0,sizeof(GDATA));
	//cout << "pgd->term_index = " << pgd->term_index <<endl;
	//cout << "pgd->databuffer = " << pgd->databuffer <<endl;
	int nRet = WSA_Init();
	if(nRet != 0)
	{
		sprintf(strReturn,"error=通信初始化错误！\0");
		delete pgd;
		return FALSE;
	}
	//cout << "----------- 通讯初始化成功 ---------------" <<endl;

	srand(GetTickCount());

	int nSleepTime = rand()%10;

	cout <<"nSleepTime = " <<nSleepTime <<endl;

	Sleep(nSleepTime*500);

	SOCKET hSocket;
	nRet = TelnetInit(strIP.c_str() ,nTelPort);
	if(nRet <= 0)
	{
		if(nRet == -1)
		{
			sprintf(strReturn,"error=连接失败：域名解析错误！\0");
		}
		else if(nRet == -2 )
		{
			sprintf(strReturn,"error=连接失败：通信初始化\0");

		}
		else if(nRet == -3)
		{
			if (count <3)
			{
				count ++;
				WSA_Free();
				Sleep(1000);
				goto LOOP;
			}
			else
			{
				sprintf(strReturn,"error=连接失败：连接超时\0");
			}
			
		}
		WSA_Free();
		delete pgd;
		return FALSE;
	}
	hSocket = nRet;
	/*nRet = TelnetConnect(hSocket,strPWD.c_str(),pwdPrompt.c_str(),cmdPrompt.c_str(), pgd );
	if (nRet < 0)
	{
		if (nRet == -1)
		{
			if (count < 3)
			{
				count++;
				shutdown(hSocket, 0x02);
				closesocket(hSocket);
				WSA_Free();
				goto LOOP;

			}
			else
			{
				sprintf(strReturn,"error=登录失败：读数据超时\0");
			}
		}
		else if (nRet == -2)
		{
			sprintf(strReturn,"error=登录失败：读数据错误\0");
		}
		else if (nRet == -3)
		{
			sprintf(strReturn,"error=登陆失败：登陆提示符为空");
		}
		else if (nRet == -4)
		{
			sprintf(strReturn,"error=登陆失败：命令提示符为空\0");
		}
		else if (nRet == -5)
		{
			sprintf(strReturn,"error=登陆失败：查找登陆提示符\0");
		}
		else if (nRet == -6)
		{
			sprintf(strReturn,"error=登陆失败：查找命令提示符\0");
		}
		else if (nRet == -7)
		{
			sprintf(strReturn,"error=登录失败：密码为空\0");
		}
		else
		{
			sprintf(strReturn,"error=登录失败：未知错误\0");
		}

		if (hSocket != INVALID_SOCKET)
		{
			shutdown(hSocket, 0x02);
			closesocket(hSocket);
		}

		WSA_Free();
		delete pgd;
		return FALSE;

	}*/

	CString strCMD= "";
	strCMD.Format("show int %s",strIndex.c_str());

	//nRet = TelnetCommand(hSocket,strCMD,pgd);
	//if (nRet < 0)
	//{
	//	sprintf(strReturn,"error=登录失败：读数据错误\0");

	//	if (hSocket != INVALID_SOCKET)
	//	{
	//		shutdown(hSocket, 0x02);
	//		closesocket(hSocket);
	//	}

	//	WSA_Free();
	//	delete pgd;
	//	return FALSE;

	//}
	
	//cout <<"CMD:	"<<strCMD<<endl;
	nRet = TelnetProcess(hSocket,strPWD.c_str(), pwdPrompt.c_str(),strCMD, cmdPrompt.c_str(), pgd );	
	if (nRet <0)
	{
		if (nRet == -1)
		{
			if (count < 3)
			{
				count++;
				shutdown(hSocket, 0x02);
				closesocket(hSocket);
				WSA_Free();
				Sleep(1000);
				goto LOOP;

			}
			else
			{
				sprintf(strReturn,"error=登录失败：读数据超时\0");
			}
			
		}
		else if (nRet == -2)
		{
			sprintf(strReturn,"error=登录失败：读数据错误\0");
		}
		else if (nRet == -3)
		{
			sprintf(strReturn,"error=登录失败：查找登录提示符不成功\0");
		}
		else if (nRet == -4)
		{
			sprintf(strReturn,"error=登录失败：登陆提示符为空\0");
		}
		else if (nRet == -5)
		{
			sprintf(strReturn,"error=登录失败：查询提示符确认不成功\0");
		}
		else if (nRet == -6)
		{
			sprintf(strReturn,"error=登录失败：查询提示符为空\0");
		}
		else if (nRet == -7)
		{
			sprintf(strReturn,"error=登录失败：密码为空\0");
		}
		else if (nRet == -8)
		{
			if (count < 3)
			{
				count++;
				shutdown(hSocket, 0x02);
				closesocket(hSocket);
				WSA_Free();
				Sleep(2000);
				goto LOOP;
			}
			else
			{
				sprintf(strReturn,"error=登陆失败：服务器关闭连接\0");
			}		
			
		}
		else
		{
			sprintf(strReturn,"error=登录失败：未知错误\0");
		}
		if (hSocket != INVALID_SOCKET)
		{
			shutdown(hSocket, 0x02);
			closesocket(hSocket);
		}
		
		WSA_Free();
		delete pgd;
		return FALSE;
	}
	CTime curTime  = CTime::GetCurrentTime();//得到当前时间
	if (hSocket != INVALID_SOCKET)
	{
		shutdown(hSocket, 0x02);
		closesocket(hSocket);
	}
	WSA_Free();	
	StringList pList;
	bool bRet(false);
	bRet = MonitorParse(pgd->databuffer, strIndex.c_str(), pList, strReturn );	
	if (!bRet)
	{
		sprintf(strReturn,"error=无法解析的字符串\0");
		//WriteLog(pgd->databuffer);
		WriteLog("MonitorParse",pgd->databuffer);
		delete pgd;
		return FALSE;
	}
	
    StringList::iterator pos = pList.begin();

	ULONG64 nInputTotal =0;
	ULONG64 nOutputTotal=0;
	CString strLineStatus="";
	CString strFaceStatus="";
	int nCount = 0;
	while (pos != pList.end())
	{
		nCount++;
		string strVal = *pos;
		if (nCount == 1)
		{
			strFaceStatus = strVal.c_str();
			cout << "strFaceStatus:"<<strFaceStatus << endl;
		}
		else if(nCount == 2)
		{
			strLineStatus = strVal.c_str();
			cout <<"strLineStatus:"<< strLineStatus<< endl;
		}
		else if (nCount == 3)
		{
			nInputTotal = _atoi64(strVal.c_str());
			cout <<"nInputTatol:"<< nInputTotal << endl;
		}
		else if (nCount == 4)
		{
			nOutputTotal = _atoi64(strVal.c_str());
			cout<<"nOutputTatol:" << nOutputTotal <<endl;
		}
		
		pos++;
	}
	
//	if ( (nInputTotal<0) || (nOutputTotal<0))
//	{
//		Sleep(1000);
//		goto LOOP;
//	}
//	cout <<"***********************************"<<endl;
	CString strFileName = "";
	CString strTime="";
	CString strLog="";
	strFileName.Format("..\\data\\TmpIniFile\\TelCisco_%s.ini", strMonitorID.c_str());
	strTime = curTime.Format("%Y%m%d%H%M%S");
	//cout <<"strTime:" <<strTime << endl;
	
	ULONG64 nPreInput=0;
	char szPreInput[64]={0};
	// nPreInput = GetPrivateProfileInt(strMonitorID.c_str(),"InputTatol",-1,strFileName);
	
	GetPrivateProfileString(strMonitorID.c_str(),"InputTatol","",szPreInput,63,strFileName);
	nPreInput = _atoi64(szPreInput);

	//strLog.Format("%I64d",nPreInput);
	//WriteLog(strFile,strLog);
	ULONG64 nPreOutput=0;
	char szPreOutput[64]={0};
	// nPreOutput =  GetPrivateProfileInt(strMonitorID.c_str(),"OutputTatol",-1,strFileName);	
	GetPrivateProfileString(strMonitorID.c_str(),"OutputTatol","",szPreOutput,63,strFileName);
	nPreOutput =  _atoi64(szPreOutput);
	//strLog.Format("%I64d",nPreOutput);
	//WriteLog(strFile,strLog);

	cout << "nPreInput:	"<<nPreInput<<endl;
	cout << "nPreOutput:	"<<nPreOutput<<endl;
	cout << "nInputTotal:	"<<nInputTotal<<endl;
	cout << "nOutputTotal:	"<<nOutputTotal<< endl;
	char szLastTime[64] ={0};
	
	GetPrivateProfileString(strMonitorID.c_str(),"LastTime","",szLastTime,63,strFileName);
	
	cout << "szLastTime:	"<<szLastTime << endl;

	CString strDataSize ="";
	strDataSize.Format("%I64d",nInputTotal);
	WritePrivateProfileString(strMonitorID.c_str(),"InputTatol",strDataSize,strFileName);
	strDataSize.Format("%I64d",nOutputTotal);
	WritePrivateProfileString(strMonitorID.c_str(),"OutputTatol",strDataSize,strFileName);
	WritePrivateProfileString(strMonitorID.c_str(),"LastTime",strTime,strFileName);
	CString strTemp = szLastTime;
	if( strTemp.IsEmpty() )
	{
		//sprintf(strReturn,"error=无历史数据\0");
		//delete pgd;
		//return FALSE;
		Sleep(2000);
		goto LOOP;
	}
	
	if ( (nInputTotal < nPreInput) || (nOutputTotal < nPreOutput) )
	{
		Sleep(2000);
		goto LOOP;
	}
	
	CTimeSpan preTime;
	CTime latsTime =  StrToDate(szLastTime);
	preTime = curTime - latsTime;
	LONGLONG nTime = preTime.GetTotalSeconds();
	//cout <<"nTime:	"<< nTime << endl;
	if (nTime <= 1)
	{
		Sleep(2000);
		goto LOOP;
	}
	
	delete pgd;
	
	double ifInOctetsRate =0.00, ifOutOctetsRate=0.00;
	ULONG64 nInSubVal = nInputTotal - nPreInput;
	ULONG64 nOutSubVal = nOutputTotal - nPreOutput;
	if (nInputTotal != nPreInput)
	{
		ifInOctetsRate = (nInputTotal - nPreInput)/(1024.00*nTime);
		ifInOctetsRate *= 8;
		
	}
	if (nOutputTotal != nPreOutput)
	{
		ifOutOctetsRate = (nOutputTotal - nPreOutput)/(1024.00*nTime);
		ifOutOctetsRate *= 8;
	}
	
	
	sprintf(strReturn,"ifInOctetsRate=%.2f$ifOutOctetsRate=%.2f$InterfaceStatus=%s$LineStatus=%s$$",ifInOctetsRate,ifOutOctetsRate,strFaceStatus,strLineStatus);
	//strLog.Format("\nnInputTotal	=%I64d\nnPreInput	=%I64d\nnOutputTotal	=%I64d\nnPreOutput	=%I64d\nnTime=	%I64d\n",nInputTotal,nPreInput,nOutputTotal,nPreOutput,nTime);
	//WriteLog(strFile,strLog);
	//strLog.Format("\nifInOctetsRate	=%.2f\nifOutOctetsRate	=%.2f\n",ifInOctetsRate,ifOutOctetsRate);
	
	//WriteLog(strFile, strLog);
	CString strOut = strReturn;
	MakeCharByString(strReturn,nSize, strOut);
	cout << "============== TelnetInteface End ===================" <<endl;
	return TRUE;

}

CTime StrToDate(CString sDateTime)
{
	if (sDateTime.IsEmpty()||(sDateTime.GetLength()<14))
	{
		CTime curTime = CTime::GetCurrentTime();
		return NULL;
	}
	int iYear = 0, iMonth = 0, iDay =0;
	int iHour = 0, iMinute = 0, iSecond =0;

	CString sYear,sMonth,sDay;
	CString sHour,sMinute,sSecond;

	//int nPos = 0;
	sYear	= sDateTime.Mid(0,4);
	sMonth  = sDateTime.Mid(4,2);
	sDay	= sDateTime.Mid(6,2);
	sHour	= sDateTime.Mid(8,2);
	sMinute = sDateTime.Mid(10,2);
	sSecond = sDateTime.Mid(12,2);

	iYear	= atoi(sYear);
	iMonth	= atoi(sMonth);
	iDay	= atoi(sDay);
	iHour	= atoi(sHour);
	iMinute	= atoi(sMinute);
	iSecond	= atoi(sSecond);

	CTime tTime(iYear, iMonth, iDay, iHour, iMinute, iSecond);
	return tTime;

}

int GetConfirms( const char* strMonitorID, __int64& nInputTatol, __int64& nOutputTatol, CTime& preTime )
{
	CString strFileName, strDate;
	//strFileName.Format("..\\data\\TmpIniFile\\TelCisco_%s.ini", strMonitorID);
	strFileName.Format(".\\TelCisco.ini",strMonitorID);
	CTime curTime  = CTime::GetCurrentTime();
	strDate = curTime.Format("%Y%m%d%H%M%S");	
	__int64 nPreInput = GetPrivateProfileInt(strMonitorID,"InputTatol",-1,strFileName);
	__int64 nPreOutput =  GetPrivateProfileInt(strMonitorID,"OutputTatol",-1,strFileName);
	char strLastDate[64] ={0};
	GetPrivateProfileString(strMonitorID,"LastTime","",strLastDate,64,strFileName);

	preTime = StrToDate(strLastDate);

	CString strDataSize ="";
	strDataSize.Format("%I64d",nInputTatol);
	WritePrivateProfileString(strMonitorID,"InputTatol",strDataSize,strFileName);
	strDataSize.Format("%I64d",nOutputTatol);
	WritePrivateProfileString(strMonitorID,"OutputTatol",strDataSize,strFileName);
	WritePrivateProfileString(strMonitorID,"LastTime",strDate,strFileName);
	nInputTatol = nPreInput;
	nPreOutput  = nPreOutput;

	return 1;
}


bool MonitorDataParse(char* databuf, const char* szCMD , StringList& pList)
{
	CString strData = databuf;
	CString strFind="";
	CString strVal = "";
	int nPos =-1, nPrePos =-1;
	char* szVal;

	if (strstr(databuf, szCMD))
	{
		strFind.Format("%s is",szCMD);
		nPos = strData.Find(strFind,0);
		if (nPos == -1) //接口状态不存在
		{
			return false;
		}
		nPrePos = nPos + strFind.GetLength();
		nPos = strData.Find(",",nPrePos);
		strVal = strData.Mid(nPrePos, nPos - nPrePos);
		strVal.TrimLeft();
		strVal.TrimRight();
		szVal = strVal.GetBuffer();
		pList.push_back(szVal);


		strFind.Format("line protocol is");//连接状态
		nPos = strData.Find(strFind, nPos);
		if(nPos == -1)
		{
			return false;
		}
		nPrePos = nPos + strFind.GetLength();
		nPos = strData.Find("\r\n",nPrePos);
		strVal = strData.Mid(nPrePos, nPos - nPrePos);
		strVal.TrimLeft();
		strVal.TrimRight();
		szVal = strVal.GetBuffer();
		pList.push_back(szVal);

		strFind.Format("input rate");//接收速率
		nPos = strData.Find(strFind,nPos);
		if (nPos == -1)
		{
			return false;
		}
		nPrePos = nPos + strFind.GetLength();
		nPos = strData.Find("bits/sec", nPrePos);
		strVal = strData.Mid(nPrePos, nPos-nPrePos);
		strVal.TrimLeft();
		strVal.TrimRight();
		szVal = strVal.GetBuffer();
		pList.push_back(szVal);

		strFind.Format("output rate");//接收速率
		nPos = strData.Find(strFind,nPos);
		if (nPos == -1)
		{
			return false;
		}
		nPrePos = nPos + strFind.GetLength();
		nPos = strData.Find("bits/sec", nPrePos);
		strVal = strData.Mid(nPrePos, nPos-nPrePos);
		strVal.TrimLeft();
		strVal.TrimRight();
		szVal = strVal.GetBuffer();
		pList.push_back(szVal);

		strFind.Format("packets input,");//接收字节
		nPos = strData.Find(strFind,nPos);
		if(nPos == -1)
		{
			return false;
		}
		nPrePos = nPos + strFind.GetLength();
		nPos = strData.Find("bytes",nPrePos);
		strVal = strData.Mid(nPrePos, nPos - nPrePos);
		strVal.TrimLeft();
		strVal.TrimRight();
		szVal = strVal.GetBuffer();
		pList.push_back(szVal);

		strFind.Format("packets output,");//发送字节
		nPos = strData.Find(strFind,nPos);
		if(nPos == -1)
		{
			return false;
		}
		nPrePos = nPos + strFind.GetLength();
		nPos = strData.Find("bytes",nPrePos);
		strVal = strData.Mid(nPrePos, nPos - nPrePos);
		strVal.TrimLeft();
		strVal.TrimRight();
		szVal = strVal.GetBuffer();
		pList.push_back(szVal);

	}
	else
	{
		// sprintf(szReturn,"error");
		return false;
	}

	return true;
}

DWORD WINAPI WorkerTelnetFunction(LPVOID lpParam)
{
	PARM *pThreadParm;
	pThreadParm = (PARM*) lpParam;
	CString strFile = _T("");
	CString strLog = "";
	strFile.Format("..\\data\\TmpIniFile\\Cisco_%s.ini",pThreadParm->strDevID);
	char strTime[64]={0};
	char strPreTime[64]={0};
	CString strCurTime="";
	CTime curTime;
	CTime telTime;
	CTimeSpan spanTime;
	int nSecond=0;
	cout << "WorkerTelnetFunction Start" << endl;
	WaitForSingleObject(hMutex, INFINITE);
	
	cout << "WorkerTelnetFunction Get the hMutex" <<endl;
	GetPrivateProfileString(pThreadParm->strDevID.GetBuffer(),"TelnetTime","",strTime,63,strFile);
	telTime = StrToDate(strTime);
	curTime = CTime::GetCurrentTime();
	spanTime = curTime - telTime;
	nSecond = spanTime.GetTotalSeconds();
	cout << "nSecond"<< nSecond <<endl;
	if( (nSecond>TIME_SPAN) || (telTime == NULL) )
	{
		if(TelnetSaveFile(pThreadParm))
		{
			cout << "TelnetSaveFile Success"<<endl;
			curTime = CTime::GetCurrentTime();
			strCurTime = curTime.Format("%Y%m%d%H%M%S");
			GetPrivateProfileString(pThreadParm->strDevID.GetBuffer(),"TelnetTime","",strPreTime,63,strFile);
			
			WritePrivateProfileString(pThreadParm->strDevID.GetBuffer(),"PreTelTime",strPreTime,strFile);
			
			WritePrivateProfileString(pThreadParm->strDevID.GetBuffer(),"TelnetTime",strCurTime,strFile);
		}
	}
	pThreadParm->bAlive = false;

	ReleaseMutex(hMutex);
	//CloseHandle(hMutex);
	cout << "ReleaseMutex " <<endl; 

	return 0;
}

BOOL ReadInteFaceFile(CString strFileName, CString& strFileContent)
{
	HANDLE hFile; 
	cout << "文件路径："<<strFileName <<endl;
	hFile = CreateFile(strFileName, GENERIC_READ ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		cout <<"Code:" <<GetLastError()<<endl;
		cout << "CreatFile Error!"<<endl;
		CloseHandle(hFile);
		return FALSE;
	}	
	int nFileLen = GetFileSize(hFile,NULL);
	char* szTemp = new char[nFileLen +1];
	if (szTemp == NULL)
	{
		cout <<"Memory malloc Error"<<endl;
		return FALSE;
	}
	memset(szTemp,0,nFileLen+1);
	DWORD dw;
	if( ReadFile(hFile,szTemp, nFileLen, &dw,0) == FALSE )
	{
		cout<<"ReadFile Error!"<<endl;
		CloseHandle(hFile);
		cout <<"Read Content:" <<szTemp << endl;
		delete[]szTemp;
		return FALSE;
	}
	CloseHandle(hFile);
	strFileContent = szTemp;
	delete[]szTemp;
	return TRUE;
}

extern "C" __declspec(dllexport)  bool TelnetAllInteface(const char * strParas, char *strReturn, int & nSize)
{
	int bReturn = TRUE;
	std::string strIP ;
	std::string	strCommunity ;
	std::string	strMonitorID ;
	std::string strIniFilePath ;
	std::string	strIndex;
	std::string	strSelValue;
	std::string strValue ;
	std::string strPWD;
	std::string pwdPrompt;
	std::string cmdPrompt;


	cout << "============== TelnetInteface Start ===================" <<endl;

	int nTplID = 0, nPort = 23, nSnmpVer = 2, nTimeout = 300, nTelPort=23;//, nIfIndex = 0;

	map<string, string, less<string> > InputParams;
	map<string, string, less<string> >::iterator paramItem;
	if(!splitparam(strParas, InputParams))
	{
		string szErrMsg = FuncGetStringFromIDS("IDS_MONITOR_PARAM_ERROR");
		nSize = sprintf(strReturn , "error=some parameter is wrong");//, FuncGetStringFromIDS("IDS_MONITOR_PARAM_ERROR"));
		return FALSE;
	}
	cout << "================"<<endl;
	paramItem = InputParams.find(SV_MonitorID);

	if(paramItem != InputParams.end())
		strMonitorID = paramItem->second;


	//paramItem = InputParams.find(SV_Community);
	//if(paramItem != InputParams.end())
	//	strCommunity = paramItem->second;

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

	//paramItem = InputParams.find(SV_SNMPDisk);
	//if(paramItem != InputParams.end())
	//	strIndex = paramItem->second; 
	/////////解决进程显示问题
	//paramItem = InputParams.find(SV_SNMPSelvalue);
	//if(paramItem != InputParams.end())
	//{		
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

	//}
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

	paramItem = InputParams.find(SV_TPLID);
	if(paramItem != InputParams.end())
		nTplID = atoi(paramItem->second.c_str());

	/*paramItem = InputParams.find(SV_TelPort);
	if (paramItem != InputParams.end())
	{
		nTelPort = atoi(paramItem->second.c_str());
	}*/

	//paramItem = InputParams.find(SV_TelPassword);
	//if(paramItem != InputParams.end())
	//	strPWD = paramItem->second;

	/*paramItem = InputParams.find(SV_TelPrompt);
	if(paramItem != InputParams.end())
		pwdPrompt = paramItem->second;

	paramItem = InputParams.find(SV_PromptCMD);
	if(paramItem != InputParams.end())
		cmdPrompt = paramItem->second;*/

	//StringList::iterator pos;
	if(strIP.empty())
	{
		std::string m_Ret = FuncGetStringFromIDS("SV_NETWORK_SET", "NETWORK_SET_IP_ADDRESS_NULL");
		sprintf(strReturn, "error=%s", m_Ret.c_str());
		nSize = static_cast<int>(strlen(strReturn));
		return FALSE;
	}

	if (strPWD.empty())
	{
		sprintf(strReturn,"error=密码不能为空！\0");
		nSize = static_cast<int>(strlen(strReturn)); 
		return FALSE;
	}

	//设备ID
	CString strDevID= strMonitorID.c_str();
	nPos=0;
	for (int i=0; i<3; i++)
	{
		nPos =strDevID.Find(".",nPos+1);
	}
	if (nPos != -1)
	{
		strDevID =  strDevID.Mid(0,nPos);
	}
	

	CString strMutexName= "";
	strMutexName.Format("Mutex_%s_%s",strIP.c_str(),strDevID);
	cout << "MutexName:" <<strMutexName << endl;
//	hMutex = CreateMutex(NULL,FALSE,strMutexName.GetBuffer());
//	if (hMutex == NULL)
//	{
//		cout<<"CreateMutex error:"<<GetLastError()<<endl;
////		CloseHandle(hMutex);
//		sprintf(strReturn,"error=创建互斥对象失败");
//		return FALSE;
//	}
//	else
//	{
//		if ( GetLastError() == ERROR_ALREADY_EXISTS)
//		{
//			cout << "CreateMutex opened existing mutex"<<endl;
//		}
//		else
//		{
//			cout << "Create Mutex successfully!"<< endl;
//		}
//		
//	}
//	hEvent =  CreateEvent(NULL, TRUE, FALSE, NULL);

	DWORD dw;

	PARM threadParm;
	threadParm.bAlive		= true;
	threadParm.nPort		= nTelPort;
	threadParm.strServer	= strIP.c_str();
	threadParm.strCmdPrompt	= cmdPrompt.c_str();
	threadParm.strPwd		= strPWD.c_str();
	threadParm.strPwdPrompt = pwdPrompt.c_str();
	threadParm.strCMD		="show int";
	threadParm.strDevID		= strDevID;

	CTime curTime = CTime::GetCurrentTime();

	char strTelTime[64] = {0};
	char strPreTime[64] ={0};
	CString strCurTime="";
	CString strFile="";
	strFile.Format("..\\data\\TmpIniFile\\Cisco_%s.ini",strDevID);
	GetPrivateProfileString(strDevID,"TelnetTime","",strTelTime,63,strFile);
	CTimeSpan preTime;
	CTime telTime = StrToDate(strTelTime);
	preTime = curTime - telTime;

	int nSecond = preTime.GetTotalSeconds();
	cout << "strTelTime:" << strTelTime <<endl;
	cout << "nSecond:" << nSecond <<endl;
	HANDLE handle;
	HANDLE hLock;
	CString strFileLock ="";
	CString strLog = "";
	strFileLock.Format("..\\data\\TmpIniFile\\Lock_%s.txt",strDevID);
	if ( (nSecond > TIME_SPAN) || (telTime == NULL)  )//时间差 5分钟
	{
		
		/*handle = CreateThread(NULL, 0, WorkerTelnetFunction ,&threadParm, 0 , &dw);
		if ( handle == NULL )
		{
			cout << "CreateThread Error"<<endl;
			threadParm.bAlive = false;	
		}	*/
LOOPW:		
		hLock = CreateFile(strFileLock,GENERIC_READ|GENERIC_WRITE,0,NULL,CREATE_ALWAYS, 0,NULL);
		if (hLock == INVALID_HANDLE_VALUE)
		{
			cout << "File Lock Create Faile!"<<endl;
			Sleep(200);
			goto LOOPW;
		}


		memset(strTelTime,0,64);
		//WaitForSingleObject(hMutex,INFINITE);

		cout << "------ Telnet Get hMutex -------" <<endl;
		GetPrivateProfileString(strDevID,"TelnetTime","",strTelTime,63,strFile);
		telTime = StrToDate(strTelTime);
		curTime = CTime::GetCurrentTime();
		preTime = curTime - telTime;
		nSecond = preTime.GetTotalSeconds();
		cout << "strTelTime:"<<strTelTime<<endl;
		cout << "nSecond:" << nSecond <<endl;
		if ( (nSecond > TIME_SPAN) || (telTime == NULL) )
		{
			try
			{

				
				if (TelnetSaveFile(&threadParm))
				{
					cout << "TelnetSaveFile Success"<<endl;
					curTime = CTime::GetCurrentTime();
					strCurTime = curTime.Format("%Y%m%d%H%M%S");
					cout << "strCurTime:" << strCurTime <<endl;
					GetPrivateProfileString(threadParm.strDevID.GetBuffer(),"TelnetTime","",strPreTime,63,strFile);
					cout << "strPreTime:" <<strPreTime<<endl;

					WritePrivateProfileString(threadParm.strDevID.GetBuffer(),"PreTelTime",strPreTime,strFile);

					WritePrivateProfileString(threadParm.strDevID.GetBuffer(),"TelnetTime",strCurTime,strFile);
				}
				else
				{
					cout << "TelnetSaveFile failed!" <<endl;
				}
			}
			catch(...)
			{
				if (hLock != INVALID_HANDLE_VALUE)
				{
                    strLog.Format("程序异常 Monitor ID：%s",strMonitorID.c_str());				
					WriteLog("TelnetAllInteface",strLog);
					CloseHandle(hLock);
				}
			}
		}
		else
		{
			cout << "----- No TelnetSaveFile -----" << endl;
		}
		//ReleaseMutex(hMutex);
		cout << "------- Telnet Release hMutex ------" <<endl;

		if (hLock != INVALID_HANDLE_VALUE )
		{
			CloseHandle(hLock);
		}
		

	}
	else
	{
		cout <<"---- No Telnet -----" <<endl;
		//threadParm.bAlive = false;
	}

	/*while(threadParm.bAlive)
	{
		cout << "Wait thread end!"<<endl;
		Sleep(2000);

	}*/


    
	CString strFileName="";
	strFileName.Format("..\\data\\TmpIniFile\\Cisco_%s.txt",strDevID);
	CString strFielContent="";

LOOPR:
	hLock = CreateFile(strFileLock,GENERIC_READ|GENERIC_WRITE,0,NULL,CREATE_ALWAYS, 0,NULL);
	if (hLock == INVALID_HANDLE_VALUE)
	{
		cout << "File Lock Create Faile!"<<endl;
		Sleep(200);
		goto LOOPR;
	}

	//WaitForSingleObject(hMutex,INFINITE);

	cout << "ReadInteFaceFile Get the hMutex" <<endl;
	bool bRet = ReadInteFaceFile(strFileName, strFielContent);
	
	memset(strTelTime,0,64);
	GetPrivateProfileString(strDevID,"PreTelTime","",strPreTime,63,strFile);
	GetPrivateProfileString(strDevID,"TelnetTime","",strTelTime,63,strFile);

	//ReleaseMutex(hMutex);
	if ( hLock != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hLock);
	}
	
	//
	//cout << "hMutex Close" << endl;
	//CloseHandle(hMutex);
	//CloseHandle(handle);
	if ( !bRet)
	{
		cout<< "Read File is failed"<<endl;
		sprintf(strReturn,"error=读取文件失败");
		return false;
	}
	
	StringList pList;

	bRet = MonitorDataParse(strFielContent.GetBuffer(),strIndex.c_str() ,pList );
	if (!bRet)
	{
		sprintf(strReturn,"error=无法解析的字符串");
		return false;
	}
	
	StringList::iterator pos = pList.begin();

	ULONG64 nInputTotal		= 0;	// 总接收字节 （bytes）
	ULONG64 nOutputTotal	= 0;	// 总发送字节 （bytes）
	ULONG64 nInputRate		= 0;	// 平均接收率 （bits/sec）
	ULONG64 nOutputRate		= 0;	// 平均发送率 （bits/sec）
	CString strLineStatus	= "";	// 连接状态
	CString strFaceStatus	= "";	// 接口状态
	double ifInOctetsRate =0.00;	// 接收速率
	double ifOutOctetsRate=0.00;	// 发送速率

	int nCount = 0;
	while (pos != pList.end())
	{
		nCount++;
		string strVal = *pos;
		if (nCount == 1)
		{
			strFaceStatus = strVal.c_str();
			cout << "strFaceStatus:" << strFaceStatus << endl;
		}
		else if(nCount == 2)
		{
			strLineStatus = strVal.c_str();
			cout << "strLineStatus:" << strLineStatus << endl;
		}
		else if (nCount == 3 )
		{
			nInputRate = _atoi64(strVal.c_str());
			cout << "nInputRate:"<< nInputRate << endl;
		}
		else if (nCount == 4)
		{
			nOutputRate = _atoi64(strVal.c_str());
			cout << "nOutputRate:" << nOutputRate <<endl;
		}
		else if (nCount == 5)
		{
			nInputTotal = _atoi64(strVal.c_str());
			cout <<"nInputTatol:"<< nInputTotal << endl;
		}
		else if (nCount == 6)
		{
			nOutputTotal = _atoi64(strVal.c_str());
			cout<<"nOutputTatol:" << nOutputTotal <<endl;
		}

		pos++;
	}

	

	LONGLONG nTime = 0;
	CTime lastTime;

	if ( strlen(strTelTime) == 0)
	{
		sprintf(strReturn,"error=无历史数据");
		return false;
	}

	strFileName.Format("..\\data\\TmpIniFile\\TelCisco_%s.ini", strMonitorID.c_str());
	ULONG64 nPreInput=0;
	ULONG64 nPreOutput=0;
	char szPreInput[64]={0};	
	char szPreOutput[64]={0};

	GetPrivateProfileString(strMonitorID.c_str(),"InputTatol","",szPreInput,63,strFileName);
	nPreInput = _atoi64(szPreInput);
	GetPrivateProfileString(strMonitorID.c_str(),"OutputTatol","",szPreOutput,63,strFileName);
	nPreOutput =  _atoi64(szPreOutput);

	CString strDataSize ="";
	strDataSize.Format("%I64d",nInputTotal);
	WritePrivateProfileString(strMonitorID.c_str(),"InputTatol",strDataSize,strFileName);
	strDataSize.Format("%I64d",nOutputTotal);
	WritePrivateProfileString(strMonitorID.c_str(),"OutputTatol",strDataSize,strFileName);
	strDataSize.Format("%s",strTelTime);
	WritePrivateProfileString(strMonitorID.c_str(),"TelnetTime",strDataSize,strFileName);
	strDataSize.Format("%s",strPreTime);
	WritePrivateProfileString(strMonitorID.c_str(),"PreTelTime",strDataSize,strFileName);


	ifInOctetsRate = nInputRate/1024.00;
	ifOutOctetsRate = nOutputRate/1024.00;

	sprintf(strReturn,"ifInOctetsRate=%.2f$ifOutOctetsRate=%.2f$InterfaceStatus=%s$LineStatus=%s$",ifInOctetsRate,ifOutOctetsRate,strFaceStatus,strLineStatus);
	//strLog.Format("\nnInputTotal	=%I64d\nnPreInput	=%I64d\nnOutputTotal	=%I64d\nnPreOutput	=%I64d\nnTime=	%I64d\n",nInputTotal,nPreInput,nOutputTotal,nPreOutput,nTime);
	//WriteLog(strFile,strLog);
	//strLog.Format("\nifInOctetsRate	=%.2f\nifOutOctetsRate	=%.2f\n",ifInOctetsRate,ifOutOctetsRate);

	//WriteLog(strFile, strLog);
	CString strOut = strReturn;
	MakeCharByString(strReturn,nSize, strOut);


	/*if ( strlen(strPreTime) == 0)
	{
		ifInOctetsRate = nInputRate/1024.00;
		ifOutOctetsRate = nOutputRate/1024.00;

	}
	else
	{
		telTime = StrToDate(strTelTime);
		lastTime = StrToDate(strPreTime);
		preTime =  telTime - lastTime;
		nTime = preTime.GetTotalSeconds();
	}*/
	


	
	return true;
}

BOOL TelnetSaveFile(PARM* lpParm)
{
	CString strFileName = "";
	LPGDATA pgd = new GDATA;
	if (pgd == NULL)
	{
		return FALSE;
	}
	
LOOP:
	memset(pgd , 0, sizeof(GDATA));
	int nRet = WSA_Init();
	if(nRet != 0)
	{
		delete pgd;
		return FALSE;
	}
	srand(GetTickCount());
	int nSleepTime = rand()%10;
	Sleep(nSleepTime*10);

	SOCKET hSocket;
	cout << "Server:" <<lpParm->strServer.GetBuffer()<<endl;
	cout << "Port:" << lpParm->nPort<<endl;
	nRet = TelnetInit(lpParm->strServer.GetBuffer(),lpParm->nPort);
	if (nRet<0)
	{
		cout <<"TelnetInit Error!" <<endl;
		WSA_Free();
		delete pgd;
		return FALSE;
	}
	cout << "nRet:" <<nRet <<endl;
	strFileName.Format("..\\data\\TmpIniFile\\Cisco_%s.txt",lpParm->strDevID);
	hSocket = nRet;
	nRet = TelnetProcessNew(hSocket, lpParm->strPwd.GetBuffer(),
		                    lpParm->strPwdPrompt.GetBuffer(),
							lpParm->strCMD.GetBuffer(),
							lpParm->strCmdPrompt.GetBuffer(),
							strFileName.GetBuffer(),
							pgd);

	if (nRet < 0)
	{
		if (hSocket != INVALID_SOCKET)
		{
			shutdown(hSocket, 0x02);
			closesocket(hSocket);
		}

		WSA_Free();
		delete pgd;
		return FALSE;
	}
	if (hSocket != INVALID_SOCKET)
	{
		shutdown(hSocket, 0x02);
		closesocket(hSocket);
	}
	WSA_Free();	
	delete pgd;

	return TRUE;
}


//yi.duan 2011-09-14 HDS监测器
//ams2500
//dfRegressionStatus	1.3.6.1.4.1.116.5.11.1.2.2.1
//
//dfRegressionStatus OBJECT-TYPE
//SYNTAX    INTEGER
//ACCESS    read-only
//STATUS    mandatory
//DESCRIPTION
//"A value of regression status.
//
//Bit          Comment
//0           drive status.
//1           spare drive status.
//2           data drive status.
//3           ENC status.
//4-5         not used,always 0.
//6           warning status.
//7           Other controller status.
//8           UPS status.
//9           loop status.
//10          path status.
//11          NAS Server status.
//12          NAS Path status.
//13          NAS UPS status.
//14-15       not used,always 0.
//16          battery status.
//17          power supply status.
//18          AC status.
//19          BK status.
//20          fan status.
//21          additional battery status.
//22-23       not used,always 0.
//24          cache memory status.
//25          SATA spare drive status.
//26          SATA data drive status.
//27          SENC status.
//28          HostConnector status.
//29          NNC status.
//30          I/F board status.
//31          not used,always 0.
//
//When the status is normal,each bit value
//is 0. When the status is abnormal,each bit 
//value is 1.
//"

//int 装换为2进制字符串返回
string int_to_bit(int a) 
{ 
	int i; 
	int n = sizeof(int) * CHAR_BIT; 
	int mask = 1 << (n - 1); 
	string strBinary("");

	for(i = 1; i <= n; ++i)
	{ 
		if (((a&mask)==0))
		{
			strBinary += "0";
		}
		else
		{
			strBinary += "1";
		}
		a <<= 1; 
		if(i % CHAR_BIT == 0 && i < n)
		{
			strBinary += " ";
		}
	}
	return strBinary;
}


//获取变量num中的index位 0或1
int get_bit(int num, int index) 
{
	return (num>>(index-1)) & 0x00000001;
} 

extern "C" __declspec(dllexport) 
bool SnmpHDS(const char * strParas, char *strReturn, int & nSize)
{

	//const char szInPut[1024]=_T("_Community=public1\0_MachineName=192.168.0.251\0_Port=161\0Version=2\0_MonitorID=1.22.7.67\0\0");
	map<string, string, less<string> > InputParams;
	if(!splitparam(strParas, InputParams))
	{
		nSize = sprintf(strReturn , "error=some parameter is wrong");
		return false;
	}

	string strIP(""), strCommunity(""),strHDSDevice("");
	//string strOid = "1.3.6.1.4.1.116.5.11.1.2.2.1";
	string strOid = "1.3.6.1.2.1.1.7"; //for test
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

	paramItem = InputParams.find("_HDSRegressionStatus");
	if(paramItem != InputParams.end())
	{
		strHDSDevice = paramItem->second;
		printf("strHDSDevice=%s\n",strHDSDevice.c_str());
	}
	
	if(strIP.empty())
	{
		printf("error=IP empty");
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
		ofstream fout("snmperror.log",ios::app);
		fout << strReturn <<"\r\n"; 
		fout << flush; 
		fout.close(); 
		return false;
	}

	objOID.SetOIDValue(strOid.c_str());
	//objOID.SetOIDValue("1.3.6.1.2.1.1");
	//objOID.SetOIDValue("1.3.6.1.2.1.25.3.2");

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
	//	percentFull根据strHDSDevice的前2位获取其32位中的位置, 再换算成0或者1
			int iHDS = atoi((strHDSDevice.substr(0,2)).c_str());
			printf("iHDS:%d\n",iHDS);

			sprintf(strReturn,"percentFull=%d$totalStatus=%s$",
				get_bit(atoi((resultIt->second.m_szValue).c_str()), iHDS+1),
				(int_to_bit(atoi((resultIt->second.m_szValue).c_str()))).c_str());

			printf("result:%s\n",(resultIt->second.m_szValue).c_str());

		}
		resultList.clear();
	}
	else
	{
		printf("error");
		sprintf(strReturn,"error");
	}

	CString strOut = strReturn;
	MakeCharByString(strReturn,nSize,strOut);

	printf("strReturn=%s\n",strReturn);
	SV_ECC_SNMP_LIB::ReleaseLib();
	return true;
}

extern "C" __declspec(dllexport) 
int SnmpHDSList(const char * strParas, char * szReturn, int& nSize)
{
	map<string, string, less<string> > InputParams;
	if(!splitparam(strParas, InputParams))
	{
		nSize = sprintf(szReturn , "error=some parameter is wrong");
		return FALSE;
	}

	string strIP("");	
	map<string, string, less<string> >::iterator paramItem;
	paramItem = InputParams.find(SV_Host);
	if(paramItem != InputParams.end())
		strIP = paramItem->second; 


	if(strIP.empty())
	{     
		std::string m_Ret = FuncGetStringFromIDS("SV_NETWORK_SET", "NETWORK_SET_IP_ADDRESS_NULL");
		sprintf(szReturn, "error=%s", (char*)m_Ret.c_str());
		return FALSE;
	}	

	char *k[] = {"00 drive status.",
	"01 spare drive status.",
	"02 data drive status.",
	"03 ENC status.",
	"04 not used,always 0.",
	"05 not used,always 0.",
	"06 warning status.",
	"07 Other controller status.",
	"08 UPS status.",
	"09 loop status.",
	"10 path status.",
	"11 NAS Server status.",
	"12 NAS Path status.",
	"13 NAS UPS status.",
	"14 not used,always 0.",
	"15 not used,always 0.",
	"16 battery status.",
	"17 power supply status.",
	"18 AC status.",
	"19 BK status.",
	"20 fan status.",
	"21 additional battery status.",
	"22 not used,always 0.",
	"23 not used,always 0.",
	"24 cache memory status.",
	"25 SATA spare drive status.",
	"26 SATA data drive status.",
	"27 SENC status.",
	"28 HostConnector status.",
	"29 NNC status.",
	"30 I/F board status.",
	"31 not used,always 0."};

	char  *p = szReturn;
	for (int i=0; i<32; i++)
	{
		//printf("line=%s\n", k[i]);
		sprintf(p,"%s=%s",k[i],k[i]);
		p+= 2* strlen(k[i])+2;
	}
/*
	if(gRoot_path == "")
		gRoot_path =FuncGetInstallPath();
	//gRoot_path = "D:\\SiteView\\SiteView ECC";  //zjw

	char szPath[1024]={0};
	sprintf(szPath,"%s\\data\\TmpIniFile\\FDSList.ini",gRoot_path.c_str());

	FILE *fp = NULL;
	if ((fp = fopen(szPath,"r"))==NULL)
	{
		sprintf(szReturn, "open FDSlist error");
		return false;
	}

	char  *p = szReturn;

	char line[256] = {0}; 	
	while( fgets(line,sizeof(line),fp) !=NULL ) 
	{
		printf("line=%s\n", line);
		sprintf(p,"%s=%s",line,line);
		p+= 2* strlen(line)+2;
	}
	fclose(fp);
*/
	return TRUE;
}

