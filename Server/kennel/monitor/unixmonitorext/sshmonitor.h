#ifndef _SV_ECC_SSH_MONITOR_H_
#define _SV_ECC_SSH_MONITOR_H_

#pragma once

#include "../base/svdefines.h"
#include "../base/SVTelnet.h"


bool SSHMONITOR(map<string, string, less<string> >InputParams, char *pszReturn, int &nSize);

bool SSH_VALUE(const char * pszServer, const int & nPort, const char * pszUid, const char * pszPwd, 
                  const char * pszOsType, const char * pszLogFile, const char * pszMatchCmd, 
                  const char * pszRuleFile, const int & nAppend, const char * pszCustomerPath, 
                  char * pszReturn, int & nSize);

bool SSHDIRFILEMONITOR(map<string, string, less<string> >InputParams, char *pszReturn, int &nSize);

bool SSH_DIREXIST_FILENUM(const char * pszServer, const int & nPort, const char * pszUid, const char * pszPwd, 
                  const char * pszOsType, const char * pszLogFile, const int &nDayNun, const char * pszMatchCmd, 
                  const char * pszMatchCmd2, const char *pszExtParam,
                  const char * pszRuleFile, const int & nAppend, const char * pszCustomerPath, 
                  char * pszReturn, int & nSize);

bool SSHDATE_MONITOR(map<string, string, less<string> >InputParams, char *pszReturn, int &nSize);

bool SSH_DATE_VALUE(const char * pszServer, const int & nPort, const char * pszUid, const char * pszPwd, 
                  const char * pszOsType, const char * pszLogFile, const char * pszMatchCmd, 
                  const char * pszRuleFile, const int & nAppend, const char * pszCustomerPath, 
                  char * pszReturn, int & nSize);

bool SSHDirectory(map<string, string, less<string> >InputParams, char *pszReturn, int nSize);

bool SSHMonitor(SVUnixOpMonitor &svUnixOpMonitor, const string &szCmd, string &szReturn);

#endif