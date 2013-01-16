// NTService.h: interface for the CNTService class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NTSERVICE_H__5F538EB7_1686_49A8_838E_DB585A839DB2__INCLUDED_)
#define AFX_NTSERVICE_H__5F538EB7_1686_49A8_838E_DB585A839DB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNTService  
{
public:
	CNTService();
	virtual ~CNTService();

public:
	static BOOL IsServiceStarted(LPCSTR sComputerName, LPCSTR sServiceName);
	static BOOL IsWindowsNT();
	static BOOL StopService(LPCSTR sComputerName, LPCSTR sServiceName);
};

#endif // !defined(AFX_NTSERVICE_H__5F538EB7_1686_49A8_838E_DB585A839DB2__INCLUDED_)
