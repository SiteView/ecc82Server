
//CString EnumServices(char *pszMachineName, DWORD dwType, DWORD dwState,DWORD *pdwCount);
void  EnumServices(char *pszMachineName, DWORD dwType, DWORD dwState,DWORD *pdwCount ,CStringList& lstText, CStringList& lstValue);
int GetServiceInfo1(char* pszMachineName,char* pszServiceName, char* szRet);


