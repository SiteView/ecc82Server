// NTService.cpp: implementation of the CNTService class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NTService.h"

#include <Winsvc.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNTService::CNTService()
{

}

CNTService::~CNTService()
{

}

BOOL CNTService::StopService(LPCSTR sComputerName, LPCSTR sServiceName)
{
	SC_HANDLE hSCManager = OpenSCManager(sComputerName, SERVICES_ACTIVE_DATABASE, SC_MANAGER_ALL_ACCESS);
	if ( hSCManager == NULL )
		return FALSE;
	
	SC_HANDLE messengerService = OpenService( hSCManager, sServiceName, SERVICE_STOP );
	CloseServiceHandle(hSCManager);
	if ( messengerService == NULL )
		return FALSE;
	
	SERVICE_STATUS serviceStatus  ;
	if ( !ControlService(messengerService, SERVICE_CONTROL_STOP, &serviceStatus) )
		return FALSE;
	CloseServiceHandle(messengerService);
	return TRUE;
}

BOOL CNTService::IsWindowsNT()
{
	return (GetVersion() < 0x80000000) ;        // Windows NT/2000, Whistler
}

BOOL CNTService::IsServiceStarted(LPCSTR sComputerName, LPCSTR sServiceName)
{
	SC_HANDLE hSCManager = OpenSCManager(sComputerName, SERVICES_ACTIVE_DATABASE, SC_MANAGER_ALL_ACCESS);
	if ( hSCManager == NULL )
		return FALSE;

	SC_HANDLE messengerService = OpenService( hSCManager, sServiceName, SERVICE_QUERY_STATUS );
	CloseServiceHandle(hSCManager);
	if ( messengerService == NULL )
		return FALSE;
	
	SERVICE_STATUS serviceStatus  ;
	if ( !QueryServiceStatus(messengerService, &serviceStatus) )
		return FALSE;
	
	CloseServiceHandle(messengerService);
	return (serviceStatus.dwCurrentState == SERVICE_RUNNING);

}
