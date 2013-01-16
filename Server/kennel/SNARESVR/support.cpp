//#include "stdafx.h"   //Only required for the SNARE GUI
// Let us access the win2k+ apis if required
#define _WIN32_WINNT 0x500
#include <windows.h>
#include <winsvc.h>
#include <aclapi.h>
#include <stdio.h>
#include "support.h"


// Write a DWORD to the registry
BOOL MyWriteProfileDWORD(LPCTSTR lpszSection, LPCTSTR lpszEntry, DWORD nValue)
{
	HKEY hSecKey = MyGetSectionKey(lpszSection);
	if (hSecKey == NULL)
		return TRUE;
	LONG lResult = RegSetValueEx(hSecKey, lpszEntry, NULL, REG_DWORD,
		(LPBYTE)&nValue, sizeof(nValue));
	RegCloseKey(hSecKey);
	return lResult == ERROR_SUCCESS;
}

// Get a DWORD from the registry
DWORD MyGetProfileDWORD(LPCTSTR lpszSection, LPCTSTR lpszEntry, DWORD nDefault)
{
	HKEY hSecKey = MyGetSectionKey(lpszSection);
	if (hSecKey == NULL)
		return nDefault;
	DWORD dwValue;
	DWORD dwType;
	DWORD dwCount = sizeof(DWORD);
	LONG lResult = RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
		(LPBYTE)&dwValue, &dwCount);
	RegCloseKey(hSecKey);
	if (lResult == ERROR_SUCCESS)
	{
		return dwValue;
	}
	return nDefault;
}

// Write a string to the registry
BOOL MyWriteProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszString)
{
	HKEY hSecKey = MyGetSectionKey(lpszSection);
	if (hSecKey == NULL)
		return TRUE;
	LONG lResult = RegSetValueEx(hSecKey, lpszEntry, NULL, REG_SZ,
		(unsigned char *)lpszString, strlen(lpszString)+1);
	RegCloseKey(hSecKey);
	return lResult == ERROR_SUCCESS;
}

// Get a string from the registry
BOOL MyGetProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszString, DWORD dwStringBuffer)
{
	HKEY hSecKey = MyGetSectionKey(lpszSection);
	if (hSecKey == NULL)
		return TRUE;
	DWORD dwSize = dwStringBuffer;
	DWORD dwType;
	LONG lResult = RegQueryValueEx(hSecKey, (LPCTSTR)lpszEntry, NULL, &dwType,
		(LPBYTE)lpszString, &dwSize);
	RegCloseKey(hSecKey);
	return lResult == ERROR_SUCCESS;
}

// Get the setion registry key
HKEY MyGetSectionKey(LPCTSTR lpszSection)
{
	HKEY hSectionKey = NULL;
	HKEY hAppKey = MyGetServiceRegistryKey();
	if (hAppKey == NULL)
		return NULL;
	
	DWORD dw;
	RegCreateKeyEx(hAppKey, lpszSection, 0, REG_NONE,
		REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
		&hSectionKey, &dw);
	RegCloseKey(hAppKey);
	return hSectionKey;
}

HKEY MyGetServiceRegistryKey()
{
	// Store the information within the InterSect Alliance high level key.
	char m_sServiceName[256]="InterSect Alliance";
	
	HKEY hServicesKey = NULL;
	static HKEY hParametersKey = NULL;
	HKEY hAppKey = NULL;
	
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE", 0, KEY_WRITE|KEY_READ,
		&hServicesKey) == ERROR_SUCCESS)
	{
		DWORD dw;
		if (RegCreateKeyEx(hServicesKey, m_sServiceName, 0, REG_NONE,
			REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
			&hAppKey, &dw) == ERROR_SUCCESS)
		{
			RegCreateKeyEx(hAppKey, "SnareSvr", 0, REG_NONE,
				REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
				&hParametersKey, &dw);
		}
	}
	if (hServicesKey != NULL)
		RegCloseKey(hServicesKey);
	if (hAppKey != NULL)
		RegCloseKey(hAppKey);
	
	return hParametersKey;
}



BOOL IsNT5plus(void) {
	DWORD winVer;
	BOOL returncode=FALSE;
	winVer=GetVersion();
	if(winVer<0x80000000) {
		OSVERSIONINFO *osvi;
		osvi=(OSVERSIONINFO *)malloc(sizeof(OSVERSIONINFO));
		if(osvi!=NULL) {
			memset(osvi,0,sizeof(OSVERSIONINFO));
			osvi->dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
			GetVersionEx(osvi);
			if(osvi->dwMajorVersion>=5) {
				returncode=TRUE;
			}
			free(osvi);
		}
	}
	return(returncode);
}
