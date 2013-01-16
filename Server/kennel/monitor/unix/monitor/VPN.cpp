
#include "VPN.h"
#include <ras.h>
#include <raserror.h>

#include "base\funcGeneral.h"

DWORD		g_ThreadID = 0;
HRASCONN	g_hRasDialConn = NULL;
HRASCONN	g_hRasVpnConn = NULL;

VOID WINAPI RasDialFunc( UINT unMsg, RASCONNSTATE rasconnstate, DWORD dwError )
{
	PostThreadMessage(	g_ThreadID, 
						WM_RASDIALEVENT, 
						(WPARAM) rasconnstate, 
						(LPARAM) dwError );
}

/*
BOOL VPN_MONITOR(char *vpnconn, char *uid, char *pwd, char *domain, int dialbeforevpn, 
				 char *dialconn, char *uidd, char *pwdd, char *domaind, 
				 char *cpath, char *szReturn)
{
	DWORD			dwRet;
	char			szBuffer[256];
	RASDIALPARAMS	rdParams;
	HRASCONN		hRasConn;

	DWORD			dwbTime = 0, dweTime = 0;

	g_ThreadID = GetCurrentThreadId();

    rdParams.dwSize = sizeof(RASDIALPARAMS);
    lstrcpy(rdParams.szEntryName, vpnconn );
    rdParams.szPhoneNumber[0] = '\0';
    rdParams.szCallbackNumber[0] = '*';
    rdParams.szCallbackNumber[0] = '\0';

    lstrcpy(rdParams.szUserName, uid);
    lstrcpy(rdParams.szPassword, pwd);
    lstrcpy(rdParams.szDomain, domain);

	dwbTime = ::GetTickCount();

	hRasConn = NULL;
	dwRet = RasDial( NULL, NULL, &rdParams, 0L, (RASDIALFUNC) RasDialFunc, &hRasConn );
    if (dwRet)
    {
		memset(szBuffer, 0, sizeof(szBuffer));
        if(RasGetErrorString((UINT)dwRet, (LPSTR)szBuffer, 256) != 0)
		{
            wsprintf((LPSTR)szBuffer, "%s (%ld).", FuncGetStringFromIDS("<%IDS_VPN_01%>"), dwRet);	// δ�����RAS���Ŵ���
		}

		if(dwRet != ERROR_CANNOT_FIND_PHONEBOOK_ENTRY)
		{
			if(hRasConn) 
			{
				RasHangUp(hRasConn);
				hRasConn = NULL;
				Sleep(2000);
			}

			sprintf(szReturn, "error=%s (%ld)$", szBuffer, dwRet);
			return FALSE;
		}
    }

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (WM_RASDIALEVENT == msg.message && NULL == msg.hwnd)
		{
			memset(szBuffer, 0, sizeof(szBuffer));
			if ( msg.lParam )  // error occurred
			{
				if (RasGetErrorString( (UINT)msg.lParam, szBuffer, sizeof(szBuffer)) != 0 )
				{
					wsprintf((LPSTR)szBuffer, "%s", FuncGetStringFromIDS("<%IDS_VPN_01%>"));
				}

				if(hRasConn)
				{
					RasHangUp(hRasConn);
					hRasConn = NULL;
					Sleep(2000);
				}

				sprintf(szReturn, "error=%s", szBuffer);
				return FALSE;
			}                                                                                                               
			else if ( RASCS_DONE & msg.wParam )
			{
				dweTime = ::GetTickCount();

				sprintf(szReturn, "roundTripTime=%ld$", dweTime - dwbTime);

				Sleep(500);

				RasHangUp(hRasConn);
				hRasConn = NULL;
				break;
			}
		}
		else
		{
			DispatchMessage(&msg);
		}
	}

	if(hRasConn)
	{
		RasHangUp(hRasConn);
		hRasConn = NULL;
		Sleep(2000);
	}

	return TRUE;
}
*/

BOOL VPN_MONITOR(char *vpnconn, char *uid, char *pwd, char *domain, int dialbeforevpn, 
				 char *dialconn, char *uidd, char *pwdd, char *domaind, 
				 char *cpath, char *szReturn)
{
	DWORD	dwbTime = 0, dweTime = 0;

	g_ThreadID = GetCurrentThreadId();

	dwbTime = ::GetTickCount();

	if(dialbeforevpn)
	{
		if(!DialCourse(dialconn, uidd, pwdd, domaind, cpath, szReturn))
		{
			if(g_hRasDialConn)
			{
				RasHangUp(g_hRasDialConn);
				g_hRasDialConn = NULL;
				Sleep(2000);
			}

			return FALSE;
		}
	}

	if(!VpnCourse(vpnconn, uid, pwd, domain, cpath, szReturn))
	{
		if(g_hRasVpnConn)
		{
			RasHangUp(g_hRasVpnConn);
			g_hRasVpnConn = NULL;
			Sleep(1000);
		}

		if(g_hRasDialConn)
		{
			RasHangUp(g_hRasDialConn);
			g_hRasDialConn = NULL;
			Sleep(1000);
		}

		return FALSE;
	}

	dweTime = ::GetTickCount();
	sprintf(szReturn, "roundTripTime=%ld$", dweTime - dwbTime);

	Sleep(500);

	if(g_hRasVpnConn)
	{
		RasHangUp(g_hRasVpnConn);
		g_hRasVpnConn = NULL;
		Sleep(1000);
	}

	if(g_hRasDialConn)
	{
		RasHangUp(g_hRasDialConn);
		g_hRasDialConn = NULL;
		Sleep(1000);
	}

	return TRUE;
}

BOOL DialCourse(char *dialconn, char *uid, char *pwd, char *domain, char *cpath, char *szReturn)
{
	DWORD			dwRet;
	char			szBuffer[256];
	RASDIALPARAMS	rdParams;

    rdParams.dwSize = sizeof(RASDIALPARAMS);
    lstrcpy(rdParams.szEntryName, dialconn );
    rdParams.szPhoneNumber[0] = '\0';
    rdParams.szCallbackNumber[0] = '*';
    rdParams.szCallbackNumber[0] = '\0';

    lstrcpy(rdParams.szUserName, uid);
    lstrcpy(rdParams.szPassword, pwd);
    lstrcpy(rdParams.szDomain, domain);

	g_hRasDialConn = NULL;
	dwRet = RasDial( NULL, NULL, &rdParams, 0L, (RASDIALFUNC) RasDialFunc, &g_hRasDialConn );
    if (dwRet)
    {
		memset(szBuffer, 0, sizeof(szBuffer));
        if(RasGetErrorString((UINT)dwRet, (LPSTR)szBuffer, 256) != 0)
		{
            wsprintf((LPSTR)szBuffer, "%s (%ld).", FuncGetStringFromIDS("<%IDS_VPN_01%>"), dwRet);	// δ�����RAS���Ŵ���
		}

		if(dwRet != ERROR_CANNOT_FIND_PHONEBOOK_ENTRY)
		{
			if(g_hRasDialConn) 
			{
				RasHangUp(g_hRasDialConn);
				g_hRasDialConn = NULL;
				Sleep(2000);
			}

			sprintf(szReturn, "error=%s (%ld)$", szBuffer, dwRet);
			return FALSE;
		}
    }

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (WM_RASDIALEVENT == msg.message && NULL == msg.hwnd)
		{
			memset(szBuffer, 0, sizeof(szBuffer));
			if ( msg.lParam )
			{
				if (RasGetErrorString( (UINT)msg.lParam, szBuffer, sizeof(szBuffer)) != 0 )
				{
					wsprintf((LPSTR)szBuffer, "%s", FuncGetStringFromIDS("<%IDS_VPN_01%>"));
				}

				if(g_hRasDialConn)
				{
					RasHangUp(g_hRasDialConn);
					g_hRasDialConn = NULL;
					Sleep(2000);
				}

				sprintf(szReturn, "error=%s", szBuffer);
				return FALSE;
			}                                                                                                               
			else if ( RASCS_DONE & msg.wParam )
			{
				break;
			}
		}
		else
		{
			DispatchMessage(&msg);
		}
	}

	return TRUE;
}

BOOL VpnCourse(char *vpnconn, char *uid, char *pwd, char *domain, char *cpath, char *szReturn)
{
	DWORD			dwRet;
	char			szBuffer[256];
	RASDIALPARAMS	rdParams;

    rdParams.dwSize = sizeof(RASDIALPARAMS);
    lstrcpy(rdParams.szEntryName, vpnconn );
    rdParams.szPhoneNumber[0] = '\0';
    rdParams.szCallbackNumber[0] = '*';
    rdParams.szCallbackNumber[0] = '\0';

    lstrcpy(rdParams.szUserName, uid);
    lstrcpy(rdParams.szPassword, pwd);
    lstrcpy(rdParams.szDomain, domain);

	g_hRasVpnConn = NULL;
	dwRet = RasDial( NULL, NULL, &rdParams, 0L, (RASDIALFUNC) RasDialFunc, &g_hRasVpnConn );
    if (dwRet)
    {
		memset(szBuffer, 0, sizeof(szBuffer));
        if(RasGetErrorString((UINT)dwRet, (LPSTR)szBuffer, 256) != 0)
		{
            wsprintf((LPSTR)szBuffer, "%s (%ld).", FuncGetStringFromIDS("<%IDS_VPN_01%>"), dwRet);	// δ�����RAS���Ŵ���
		}

		if(dwRet != ERROR_CANNOT_FIND_PHONEBOOK_ENTRY)
		{
			if(g_hRasVpnConn) 
			{
				RasHangUp(g_hRasVpnConn);
				g_hRasVpnConn = NULL;
				Sleep(2000);
			}

			sprintf(szReturn, "error=%s (%ld)$", szBuffer, dwRet);
			return FALSE;
		}
    }

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (WM_RASDIALEVENT == msg.message && NULL == msg.hwnd)
		{
			memset(szBuffer, 0, sizeof(szBuffer));
			if ( msg.lParam )
			{
				if (RasGetErrorString( (UINT)msg.lParam, szBuffer, sizeof(szBuffer)) != 0 )
				{
					wsprintf((LPSTR)szBuffer, "%s", FuncGetStringFromIDS("<%IDS_VPN_01%>"));
				}

				if(g_hRasVpnConn)
				{
					RasHangUp(g_hRasVpnConn);
					g_hRasVpnConn = NULL;
					Sleep(2000);
				}

				sprintf(szReturn, "error=%s", szBuffer);
				return FALSE;
			}                                                                                                               
			else if ( RASCS_DONE & msg.wParam )
			{
				break;
			}
		}
		else
		{
			DispatchMessage(&msg);
		}
	}

	return TRUE;
}