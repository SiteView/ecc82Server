 // SnmpMonitor.cpp : Defines the initialization routines for the DLL.
//


#include "commonMonitor.h"
//#include "stlini.h"
#include "Time.h"
#include <stdlib.h>
//#include <fstream>
//#include <afxwin.h>         // MFC 核心组件和标准组件

#include <Base\SVDefines.h>
#include <scsvapi\svapi.h>
#include "SnmpOperate.h"

//#include "..\..\base\funcGeneral.h"
//#include "SNMP_lib.h"


using namespace SV_ECC_SNMP_LIB;
using namespace svutil;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef WIN32
#include <windows.h>
#endif


enum
{
	forReading,//
	forWriting,
	forAppending
};

int WriteLog(const char * str)
{
#ifndef _DEBUG
	return 0;
#endif
	char timebuf[128],datebuf[128],wyt[1024];
	_tzset();

	_strtime(timebuf);
	_strdate(datebuf);

	sprintf(wyt,"%s %s\t",datebuf,timebuf);

	FILE *log ;
	log=fopen("CommonNetworkDeviceMonitor.Log","a+");

	if(log!=NULL)
	{
		strcat(wyt,str);
		fprintf(log,"%s\n",wyt);
		fclose(log);
	}
	return 0;
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

#define DebugToFile//输入到文件
//下面的函数用来打印一个字符串
void PrintDebug(const char * szMsg)
{
#ifndef DebugToFile
	OutputDebugString("CommonSNMPMonitor : ");
	OutputDebugString(szMsg);
	OutputDebugString("\n");
#else
	printf(szMsg);
	FILE *fp;
	fp=fopen("CommonNetworkDeviceMonitordebug.txt","a+");
	fprintf(fp,"%s\n",szMsg);
	fclose(fp);		
#endif
	return;
}

//下面的函数用来打印输入的参数
void PrintDebug(map<string, string, less<string> > InputParams)
{
	map<string, string, less<string> >::iterator paramItem;
	string strTemp;
	for (paramItem=InputParams.begin();paramItem != InputParams.end();paramItem++)
	{
		strTemp=paramItem->first+"  "+paramItem->second+"  ";
		PrintDebug(strTemp.c_str());
	}
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


bool MakeStringListByChar(pStringList& pList, const char * pInput )
{
	const char * p;
	int nSize;
	p=pInput;
	while(*p!='\0')
	{
		nSize =strlen(p);
		if( nSize>0 )
		{	
			//pList.AddHead(p);
			pList.push_back((char*)p);

		}
		p=p+nSize+1;
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

int GetCharLength(const char * pInput)
{
	const char * p;
	int nSize = 0;
	p=pInput;
	while(*p!='\0')
	{
		nSize += strlen(p) + 1;
		p += strlen(p)+1;
	}

	return nSize;
}

bool MakeCharByString(char *pOut,int &nOutSize,string strInput )
{
	char *p;

	int nSize=strInput.size();
	if(nSize+2 <nOutSize)
	{
		strcpy(pOut,strInput.c_str());
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

char *FindStrValue(const char *strParas, string str)
{
	char *pValue = NULL;
	string m_TempStr;

	std::list<char *> strList;
	MakeStringListByChar(strList, strParas);
	std::list<char *>::iterator pos = strList.begin();

	while(pos != strList.end())
	{
		//CString strTemp = strList.GetNext(pos);
		char * strTemp = *pos;
		std::string strTemp1 = *pos;
		int m_Fpos = 0;

		if((m_Fpos = strTemp1.find(str, 0)) >= 0)
		{
			m_TempStr = strTemp1.substr( m_Fpos + str.size()+1, strTemp1.size() - str.size() - 1); 
			pValue=(char*)malloc(m_TempStr.size()+1);
			strcpy(pValue, m_TempStr.c_str());

		}
		pos++;
	}

	return pValue;

}

BOOL MakeChar(char * szReturn, int& nSize) 
{
	string strReturn;

	std::list<char*> strList;
	MakeStringListByChar(strList, szReturn);
	std::list<char *>::iterator pos = strList.begin();

	int nCount = 1; 
	while(pos != strList.end())
	{
		string strTemp1 = *pos;
		string strTemp;
		char  chTemp[256];
		sprintf(chTemp,"Return%d=%s$", nCount, strTemp1.c_str());
		strTemp=chTemp;
		strReturn += strTemp;
		nCount++;
		pos++;
	}

	nSize = 2048;
	return MakeCharByString(szReturn,nSize,strReturn);	 
}

BOOL MakeCharByParam(char * szReturn, int& nSize, char * param, int nCol) 
{
	string  strReturn;

	std::list<char*> strList;
	MakeStringListByChar(strList, szReturn);
	std::list<char *>::iterator pos = strList.begin();

	while(pos != strList.end())
	{
		string strTemp = *pos;
		if(strTemp==param)
		{
			string strTemp1, strTemp2;
			for(int i=1; i<nCol; i++)
			{
				pos++;
				strTemp1 = *pos;
				char  chTemp[256];
				sprintf(chTemp,"Return%d=%s$", i, strTemp1.c_str());
				strTemp2=chTemp; 
				strReturn += strTemp2;		
			}
			break;
		}
		for(int i=0; i<nCol; i++)
			pos++;
	}

	nSize = 2048;
	return MakeCharByString(szReturn,nSize,strReturn);	 
}

BOOL MakeCharList(char * szReturn, int& nSize) 
{
	string strReturn;

	std::list<char*> strList;
	MakeStringListByChar(strList, szReturn);
	std::list<char *>::iterator pos = strList.begin();

	while(pos != strList.end())
	{
		string strTemp1 = *pos;
		string strTemp;
		strTemp=strTemp1+"="+strTemp1+"$";
		pos++;
	}

	nSize = 1048576;
	return MakeCharByString(szReturn,nSize,strReturn);	 
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


//通过SNMP方式获取网络设备的数据
bool GetDataBySNMP(map<string, string, less<string> > InputParams,char *szReturn,int& nSize)
{
	//从传入参数中获得所需要的参数
	std::string strIP ;
	std::string	strCommunity ;
	std::string	strMonitorID ;
	std::string strIniFilePath ;
	std::string	strIndex;
	std::string	strSelValue;
	std::string strValue ;	
	std::string strOID ;	
	int nTplID = 0, nPort = 161, nSnmpVer = 2, nTimeout = 300;//, nIfIndex = 0;
	//获得监测器的索引值
	map<string, string, less<string> >::iterator paramItem;
	paramItem = InputParams.find(SV_MonitorID);
	if(paramItem != InputParams.end())
		strMonitorID = paramItem->second;

	//监测器模板的ID号
	paramItem = InputParams.find(SV_TPLID);
	if(paramItem != InputParams.end())
		nTplID = atoi(paramItem->second.c_str());

	paramItem = InputParams.find(SV_Community);
	if(paramItem != InputParams.end())
		strCommunity = paramItem->second;

	paramItem = InputParams.find(SV_Host);
	if(paramItem != InputParams.end())
		strIP = paramItem->second; 
	//////////解决接口信息的显示问题
	paramItem = InputParams.find(SV_InterfaceIndex);
	if(paramItem != InputParams.end())
		strIndex = paramItem->second;

	//设置SNMP的OID值
	paramItem = InputParams.find(SV_OID);
	if(paramItem != InputParams.end())
		strOID = paramItem->second;
	//设置OID值
	int nPos = strIndex.find('_');
	if(nPos != -1)
		strIndex.erase(nPos, strIndex.length()-nPos);
	//////////解决接口信息的显示问题

	paramItem = InputParams.find(SV_SNMPDisk);
	if(paramItem != InputParams.end())
		strIndex = paramItem->second; 
	/////////解决进程显示问题
	paramItem = InputParams.find(SV_SNMPSelvalue);
	if(paramItem != InputParams.end())
		strSelValue=paramItem->second;
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

	//如果没有传入OID参数，则采用配置文件中的OID读取数据
	if (strOID.empty())
		return snmpOperate.GetResult(szReturn, nSize);
	else//否则，如果传入了OID，则使用此OID读取数据
		return snmpOperate.GetResultByOID(szReturn,strOID,nSize);
	return true;
}

//通过SNMP方式获得网络设备的端口信息
/////////////////////////////////////////////////////////////////////////////
bool GetIfBySNMP(map<string, string, less<string> > InputParams,char *szReturn,int& nSize)
{
	//从传入参数中获得所需要的参数
	std::string strIP ;
	std::string	strCommunity ;
	std::string	strMonitorID ;
	std::string strIniFilePath ;
	std::string	strIndex;
	std::string	strSelValue;
	std::string strValue ;	
	std::string strOID ;	
	int nTplID = 0, nPort = 161, nSnmpVer = 2, nTimeout = 300;//, nIfIndex = 0;
	//获得监测器的索引值
	map<string, string, less<string> >::iterator paramItem;
	paramItem = InputParams.find(SV_MonitorID);
	if(paramItem != InputParams.end())
		strMonitorID = paramItem->second;

	//监测器模板的ID号
	paramItem = InputParams.find(SV_TPLID);
	if(paramItem != InputParams.end())
		nTplID = atoi(paramItem->second.c_str());

	paramItem = InputParams.find(SV_Community);
	if(paramItem != InputParams.end())
		strCommunity = paramItem->second;

	paramItem = InputParams.find(SV_Host);
	if(paramItem != InputParams.end())
		strIP = paramItem->second; 
	//////////解决接口信息的显示问题
	paramItem = InputParams.find(SV_InterfaceIndex);
	if(paramItem != InputParams.end())
		strIndex = paramItem->second;

	//设置SNMP的OID值
	paramItem = InputParams.find(SV_OID);
	if(paramItem != InputParams.end())
		strOID = paramItem->second;
	//设置OID值
	int nPos = strIndex.find('_');
	if(nPos != -1)
		strIndex.erase(nPos, strIndex.length()-nPos);
	//////////解决接口信息的显示问题

	paramItem = InputParams.find(SV_SNMPDisk);
	if(paramItem != InputParams.end())
		strIndex = paramItem->second; 
	/////////解决进程显示问题
	paramItem = InputParams.find(SV_SNMPSelvalue);
	if(paramItem != InputParams.end())
		strSelValue=paramItem->second;
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

	return snmpOperate.GetInterfaceInfo(szReturn,strOID,nSize);
	return true;
}

//通过CLI方式获得网络设备的端口信息
/////////////////////////////////////////////////////////////////////////////
bool GetIFByCLI(map<string, string, less<string> > InputParams,char *szReturn,int& nSize)
{
	return true;
}

//通过CLI（telnet）方式获取网络设备的数据
bool GetDataByCLI(map<string, string, less<string> > InputParams,char *szReturn,int& nSize)
{
	typedef int(*pReadByTelnet)(char * TargetIp,char * Username,char *Password, char * CmdParam, int indicatorIndex,int &nCount,
		char* pTelnetData,int &nRow,int &nCol);

	std::string machinename;
	std::string user;
	std::string pswd;
	std::string param;
	int nAct;
	map<string, string, less<string> >::iterator paramItem;
	//获得设备名或IP地址
	paramItem = InputParams.find( "_MachineName");
	if(paramItem != InputParams.end())
		machinename = paramItem->second; 
	//获得用户账号
	paramItem = InputParams.find( "_UserAccount");
	if(paramItem != InputParams.end())
		user = paramItem->second; 
	//获得用户口令
	paramItem = InputParams.find( "_PassWord");
	if(paramItem != InputParams.end())
		pswd = paramItem->second; 
	//获得采取的行为
	paramItem = InputParams.find( "_Action");
	if(paramItem != InputParams.end())
		nAct = atoi(paramItem->second.c_str()); 


	HINSTANCE hDLL = NULL;
	pReadByTelnet ReadByTelnet;

	hDLL=LoadLibrary("ReadTelnet.dll");//加载动态链接库MyDll.dll文件；
	if(!hDLL)
	{
		sprintf(szReturn, "error=Error ocured:Load DLL Failed!");
		return FALSE;
	}
	ReadByTelnet = (pReadByTelnet)GetProcAddress(hDLL,"ReadByTelnet");

	int nRow,nCol;
	switch (nAct)
	{
	case CICS_MONITOR:
		//case DISK_MONITOR:
		ReadByTelnet((char *)machinename.c_str(), (char *)user.c_str(), (char *)pswd.c_str(), "", nAct, nSize, szReturn, nRow, nCol);
		MakeCharByParam(szReturn, nSize, (char *)param.c_str(), nCol);	
		break;
	default:
		ReadByTelnet((char *)machinename.c_str(), (char *)user.c_str(), (char *)pswd.c_str(), (char *)param.c_str(), nAct, nSize, szReturn, nRow, nCol);
		MakeChar(szReturn, nSize);
		break;
	}
	return TRUE;
}

//通过SMI-S方式获取网络设备的数据
//暂缺
bool GetDataBySMI(map<string, string, less<string> > InputParams,char *szReturn,int& nSize)
{
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
extern "C" __declspec(dllexport)  int GetData(const char * strParas, char * szReturn, int& nSize)
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	WriteLog("\n\n=================== GetData ======================");
    
	string stemp=GetSiteViewRootPath();
	int bReturn = TRUE;
	int nGetValueMode=0;//取值方式，先设置为零
    map<string, string, less<string> > InputParams;
    map<string, string, less<string> >::iterator paramItem;
    if(!splitparam(strParas, InputParams))
    {
        string szErrMsg = FuncGetStringFromIDS("IDS_MONITOR_PARAM_ERROR");
        nSize = sprintf(szReturn , "error=some parameter is wrong");//, FuncGetStringFromIDS("IDS_MONITOR_PARAM_ERROR"));
        return FALSE;
    }
	
	//输出监测的输入数据
	PrintDebug(InputParams);

	//获得监测器的取值方式
	paramItem = InputParams.find(SV_GETMODE);
	if(paramItem != InputParams.end())
		nGetValueMode = atoi(paramItem->second.c_str());

	//根据取值方式，调用相应的函数或动态链接库
	switch(nGetValueMode)
	{
		case 0:
			GetDataBySNMP(InputParams,szReturn,nSize);
			break;
		case 1:
			GetDataByCLI(InputParams,szReturn,nSize);
			break;
		case 2:
			GetDataBySMI(InputParams,szReturn,nSize);
			break;
	}
	

	
    SV_ECC_SNMP_LIB::ReleaseLib();

	WriteLog("\n\n-----------------------------------");

	char* pstrOutput=szReturn;

	while(*pstrOutput)
	{
		WriteLog(pstrOutput);
		pstrOutput+=strlen(pstrOutput)+1;
	}
	
	//输出监测的输入数据
	InputParams.clear();
	if(splitparam(szReturn, InputParams))
	{
		PrintDebug(InputParams);
	}
	
    return bReturn;
}

//读取网络设备的端口信息
extern "C" __declspec(dllexport)  bool INTERFACES(const char * strParas, char *pszReturn, int & nSize)
{
	//	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	WriteLog("\n\n=================== GetData ======================");

	int bReturn = TRUE;
	int nGetValueMode=0;//取值方式，先设置为零
	map<string, string, less<string> > InputParams;
	map<string, string, less<string> >::iterator paramItem;
	if(!splitparam(strParas, InputParams))
	{
		string szErrMsg = FuncGetStringFromIDS("IDS_MONITOR_PARAM_ERROR");
		nSize = sprintf(pszReturn , "error=some parameter is wrong");//, FuncGetStringFromIDS("IDS_MONITOR_PARAM_ERROR"));
		return FALSE;
	}

	//获得监测器的取值方式
	paramItem = InputParams.find(SV_GETMODE);
	if(paramItem != InputParams.end())
		nGetValueMode = atoi(paramItem->second.c_str());

	//根据取值方式，调用相应的函数或动态链接库
	switch(nGetValueMode)
	{
	case 0:
		GetIfBySNMP(InputParams,pszReturn,nSize);
		break;
	case 1:
		GetIFByCLI(InputParams,pszReturn,nSize);
		break;
	case 2:
		break;
	}



	SV_ECC_SNMP_LIB::ReleaseLib();

	WriteLog("\n\n-----------------------------------");

	char* pstrOutput=pszReturn;

	while(*pstrOutput)
	{
		WriteLog(pstrOutput);
		pstrOutput+=strlen(pstrOutput)+1;
	}
	//输出监测的输入数据
	InputParams.clear();
	if(splitparam(pszReturn, InputParams))
	{
		PrintDebug(InputParams);
	}

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
