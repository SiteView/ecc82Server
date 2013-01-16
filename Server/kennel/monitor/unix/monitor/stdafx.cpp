// stdafx.cpp : source file that includes just the standard includes
//	Monitor.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"



void WriteDebugString(LPCTSTR pszString, bool bIsMS, int nNum, LPCTSTR pszFile, int nLine)
{
	try
	{
		if (NULL == pszString)
			pszString = _T("");

		std::ofstream ofDbgFile("monitor.dbg.log", std::ios_base::app | std::ios_base::out);
		ofDbgFile << (int)::GetCurrentProcessId() << "." << (int)::GetCurrentThreadId() << " - Write debug string." << std::endl;
		if (bIsMS)
		{
			ofDbgFile << pszFile << "(" << nLine << ") : ";
			LPCTSTR p = pszString;
			while(*p)
			{
				ofDbgFile << p << " ";
				p += lstrlen(p) + 1;
			}
			ofDbgFile << "[" << nNum << "]" << std::endl;
		}
		else
		{
			ofDbgFile << pszFile << "(" << nLine << ") : " << pszString << "[" << nNum << "]" << std::endl;
		}
	}
	catch(...)
	{
	}
}