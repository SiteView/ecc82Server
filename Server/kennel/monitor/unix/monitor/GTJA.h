
#include "stdafx.h"
#include "IEEvents.h"

#define WM_USER_STARTWALKING	((WM_USER) + 1)

UINT AgentThreadProc( LPVOID pParam );

struct tagParam
{
	DWORD	threadId;
	int		timeOut;
};

BOOL GTJA_MONITOR(CString strURL, CString strPostData, int nTimeout, 
				  CString strMatchContent, int nID, char *szReturn);
