
#include "TRAP.h"

#include "base\funcGeneral.h"

BOOL TRAP_MONITOR(char *mstr, int altype, char *custpath, char *szReturn, int& nSize)
{
	FILE *fp = NULL;
	int	 lines = 0, matchCount = 0;
	char *ca = NULL, *cb = NULL;
	char szBuffer[8192] = {0};
	char std_log[256] = {0};

	sprintf(std_log, "%s\\logs\\snmptraps.log", FuncGetInstallRootPath());
	printf("std_log = %s\n",std_log);
	if((fp = fopen(std_log, "r")) == NULL)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TRAP_1%>"));//<%IDS_TRAP_1%>
		return FALSE;
	}

	while(!feof(fp))
	{
		memset(szBuffer, 0, sizeof(szBuffer));

		fgets(szBuffer, sizeof(szBuffer), fp);

		if(!strncmp(szBuffer, "hahahatrap:", 11))
		{
			lines ++;

			if(*mstr)
			{
				ca = szBuffer;
				cb = strstr(ca, "Message=");
				char szMess[4096] = {0};
				sscanf(cb ,"%[^ ]", szMess);
				if(strstr(szMess, mstr)) 
				{
					matchCount ++;
					//break;
				}
				
			}
		}
	}

	fclose(fp);
 
	sprintf(szReturn, "lines=%d$matchCount=%d$", lines, matchCount);
	CString strInput ;
	strInput =szReturn;
	MakeCharByString(szReturn,nSize,strInput);

	return TRUE;
}
