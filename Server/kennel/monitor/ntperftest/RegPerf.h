#include "stdafx.h"

int GetCPURate(char *pszMachineName,char* szRet,int nVersion=5);
int GetMemoryInfo1(char *pszMachineName,char* szRet);
int EnumDisks(char *pszMachineName,CStringList& lstText, CStringList& lstValue);
int GetPerfomanceInstance(HKEY hPERFKEY, int nObjIndex,CStringList& lstText, CStringList& lstValue);
int GetDiskInfo1(char *pszMachineName,char* szRet,CString strDisk);
int EnumAllNetWork1(char *pszMachineName,CStringList& lstText, CStringList& lstValue);
int GetNetWorkInfo1(char *pszMachineName,char* szRet,CString strNetWork);
int GetIISInfo1(char *pszMachineName,char* szRet);
int GetWebServerInfo1(char *pszMachineName,char* szRet);