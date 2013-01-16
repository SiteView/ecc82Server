#pragma once
#include "windows.h"
#include <string>
#include <map>
using namespace std;

class CShareMemFile
{
public:
	CShareMemFile(bool bAutoDelete);
	~CShareMemFile(void);

public:
	bool CreateMem (string filename, DWORD flProtected, DWORD dwSize, LPVOID lpVoid);
	LPVOID MapMem (string filename, DWORD &dwSize);
private:
	HANDLE m_hMem;
	LPVOID m_pViewOfFile;
	bool   m_bAutoDelete;
	static map<string, DWORD>	m_LanguageMap;
};
