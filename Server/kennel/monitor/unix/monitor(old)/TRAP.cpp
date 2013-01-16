
#include "TRAP.h"

#include "..\..\base\funcGeneral.h"

BOOL TRAP_MONITOR(char *mstr, int altype, char *custpath, char *szReturn)
{
	FILE *fp = NULL;
	int	 lines = 0, matchCount = 0;
	char *ca = NULL, *cb = NULL;
	char szBuffer[8192] = {0};
	char std_log[256] = {0};

	sprintf(std_log, "%s\\logs\\traps.log", FuncGetInstallRootPath());
	if((fp = fopen(std_log, "r")) == NULL)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TRAP_1%>"));//<%IDS_TRAP_1%>
		return FALSE;
	}

	while(!feof(fp))
	{
		memset(szBuffer, 0, sizeof(szBuffer));

		fgets(szBuffer, sizeof(szBuffer), fp);

		if(!strncmp(szBuffer, "+++SNMP-TRAP-PDU: ", 18))
		{
			lines ++;

			if(*mstr)
			{
				ca = szBuffer;
				while(cb = strstr(ca, "[Message]:="))
				{
					char szMess[4096] = {0};
					sscanf(cb + 11, "%[^$$$]$$$", szMess);
					if(strstr(szMess, mstr)) 
					{
						matchCount ++;
						break;
					}

					ca = cb + 11;
				}
			}
		}
	}

	fclose(fp);

	sprintf(szReturn, "lines=%d$matchCount=%d$", lines, matchCount);

	return TRUE;
}
