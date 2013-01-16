#include "stdafx.h"
#include <list>
#include <fstream>

using namespace std;
typedef struct std::list<char*> StringList;

#define		CPU_MONITOR					1001	//CPU
#define		MEMORY_MONITOR				1002	//内存
#define		DISK_MONITOR				1003	//磁盘
#define		DISKS_MONITOR				1004	//磁盘列表
#define		PROCESS_MONITOR				1005	//进程
#define		PROCESSES_MONITOR			1006	//进程列表
#define		DEVICE_MONITOR				1007	//设备
#define		DEVICES_MONITOR				1008	//设备列表
#define		FILE_MONITOR				1009	//文件
#define		FILES_MONITOR				1010	//文件列表
#define		SCRIPTS_MONITOR				1011	//脚本
#define		ERRLOG_MONITOR				1012	//错误日志
#define		CICS_MONITOR				1013	//CICS服务器
#define		CICSES_MONITOR				1014	//CICS服务器列表

bool MakeStringListByChar(StringList& pList, const char * pInput )
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

char *FindStrValue(const char *strParas, CString str)
{
	char *pValue = NULL;
	string m_TempStr;

	std::list<char*> strList;
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
			m_TempStr = strTemp1.substr( m_Fpos + strlen(str)+1, strTemp1.size() - strlen(str) - 1); 
			pValue=(char*)malloc(m_TempStr.size()+1);
			strcpy(pValue, m_TempStr.c_str());
			
		}
		pos++;
	}

	return pValue;
	
}

BOOL MakeChar(char * szReturn, int& nSize) 
{
	CString strReturn;
	
	std::list<char*> strList;
	MakeStringListByChar(strList, szReturn);
	std::list<char *>::iterator pos = strList.begin();

	int nCount = 1; 
	while(pos != strList.end())
	{
		CString strTemp1 = *pos;
		CString strTemp;
		strTemp.Format("Return%d=%s$", nCount, strTemp1);
		strReturn += strTemp;
		nCount++;
		pos++;
	}

	strcpy(szReturn,strReturn);
	CString strOutRet;
	strOutRet =szReturn;
	nSize = 2048;
	return MakeCharByString(szReturn,nSize,strOutRet);	 
}

BOOL MakeCharByParam(char * szReturn, int& nSize, char * param, int nCol) 
{
	CString strReturn;
	
	std::list<char*> strList;
	MakeStringListByChar(strList, szReturn);
	std::list<char *>::iterator pos = strList.begin();

	while(pos != strList.end())
	{
		CString strTemp = *pos;
		if(strcmp(strTemp, param) == 0)
		{
			CString strTemp1, strTemp2;
			for(int i=1; i<nCol; i++)
			{
				pos++;
				strTemp1 = *pos;
				strTemp2.Format("Return%d=%s$", i, strTemp1); 
				strReturn += strTemp2;			
			}
			break;
		}
		for(int i=0; i<nCol; i++)
			pos++;
	}

	strcpy(szReturn,strReturn);
	CString strOutRet;
	strOutRet =szReturn;
	nSize = 2048;
	return MakeCharByString(szReturn,nSize,strOutRet);	 
}

BOOL MakeCharList(char * szReturn, int& nSize) 
{
	CString strReturn;
	
	std::list<char*> strList;
	MakeStringListByChar(strList, szReturn);
	std::list<char *>::iterator pos = strList.begin();

	while(pos != strList.end())
	{
		CString strTemp1 = *pos;
		CString strTemp;
		strTemp.Format("%s=%s$", strTemp1, strTemp1);
		strReturn += strTemp;
		pos++;
	}

	strcpy(szReturn,strReturn);
	CString strOutRet;
	strOutRet =szReturn;
	nSize = 1048576;
	return MakeCharByString(szReturn,nSize,strOutRet);	 
}

extern "C" _declspec(dllexport) 
BOOL UnivMon(const char * strParas, char * szReturn, int& nSize)
{
	typedef int(*pReadByTelnet)(char * TargetIp,char * Username,char *Password, char * CmdParam, int indicatorIndex,int &nCount,
		char* pTelnetData,int &nRow,int &nCol);

	char *machinename=NULL, *user=NULL, *pswd=NULL, *param=NULL;
	int nAct;

	
			
	machinename = FindStrValue(strParas, "_MachineName");
	user = FindStrValue(strParas, "_UserAccount");
	pswd = FindStrValue(strParas, "_PassWord");
	param = FindStrValue(strParas, "_CmdParam");
	nAct = atoi(FindStrValue(strParas, "_Action"));
	if(!param)
		param = "";

	//ofstream fout("univmon.log",ios::app);
	//fout << param <<"\r\n"; 
	//fout << flush; 
	//fout.close(); 

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
			ReadByTelnet(machinename, user, pswd, "", nAct, nSize, szReturn, nRow, nCol);
			MakeCharByParam(szReturn, nSize, param, nCol);	
		break;
		default:
			ReadByTelnet(machinename, user, pswd, param, nAct, nSize, szReturn, nRow, nCol);
			MakeChar(szReturn, nSize);
		break;
	}
	return TRUE;
}

BOOL UnivMonList(const char * strParas, char * szReturn, int& nSize, int nAction)
{
	typedef int(*pReadByTelnet)(char * TargetIp,char * Username,char *Password, char * CmdParam, int indicatorIndex,int &nCount,
		char* pTelnetData,int &nRow,int &nCol);

	char *machinename=NULL, *user=NULL, *pswd=NULL, *param=NULL;
	int nAct;

	machinename = FindStrValue(strParas, "_MachineName");
	user = FindStrValue(strParas, "_UserAccount");
	pswd = FindStrValue(strParas, "_PassWord");
	param = FindStrValue(strParas, "_CmdParam");
	if(!param)
		param = "";
	
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
	ReadByTelnet(machinename, user, pswd, param, nAction, nSize, szReturn, nRow, nCol);

	MakeCharList(szReturn, nSize);

	return TRUE;
}

extern "C" _declspec(dllexport) 
BOOL EnumDisks(const char * strParas, char * szReturn, int& nSize)
{
	return UnivMonList(strParas, szReturn, nSize, DISKS_MONITOR);
}

extern "C" _declspec(dllexport) 
BOOL EnumProcesses(const char * strParas, char * szReturn, int& nSize)
{
	return UnivMonList(strParas, szReturn, nSize, PROCESSES_MONITOR);
}

extern "C" _declspec(dllexport) 
BOOL EnumDevices(const char * strParas, char * szReturn, int& nSize)
{
	return UnivMonList(strParas, szReturn, nSize, DEVICES_MONITOR);
}

extern "C" _declspec(dllexport) 
BOOL EnumFiles(const char * strParas, char * szReturn, int& nSize)
{
	return UnivMonList(strParas, szReturn, nSize, FILES_MONITOR);
}

extern "C" _declspec(dllexport) 
BOOL EnumCicses(const char * strParas, char * szReturn, int& nSize)
{
	return UnivMonList(strParas, szReturn, nSize, CICSES_MONITOR);
}