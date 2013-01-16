#include "NNMUtil.h"


NNMUtil::NNMUtil(void)
{
	myWriteLog = new cWriteLog();
}

NNMUtil::~NNMUtil(void)
{
}

bool NNMUtil::RunScanNetProcess(std::string sdir,std::string cmdline,PROCESS_INFORMATION &pi,bool asy)
{
#ifdef WIN32
    STARTUPINFO si;
 //   PROCESS_INFORMATION pi;

	DWORD dcid=::GetCurrentProcessId();

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(PROCESS_INFORMATION) );

	char buf[2048]={0};
	char currentDir[256] = {0};

	strcpy(currentDir, "D:\\netscanC");
	//sprintf(buf,"%s\\NetScan.exe %s",sdir.c_str(), cmdline.c_str());

	sprintf(buf,"E:\\Python24\\python.exe %s\\netscanC.py %s",currentDir, cmdline.c_str() );
	puts(buf);

	int n=0;
	BOOL bRet=TRUE;

	if(!::CreateProcess(NULL,
		buf,
		NULL,
		NULL,
		FALSE,
		/*CREATE_NO_WINDOW*/CREATE_NEW_CONSOLE,
		NULL,
		currentDir,	//sdir.c_str(),
		&si,&pi))
	{
		printf(buf, "∆Ù∂Ø≥Ã–Ú£∫%s ß∞‹£¨ErrorCode£∫%d\n",buf,::GetLastError());
		puts(buf);

		return false;
	}

	/*
	  Error code 14001:This application has failed to start because the application configuration is incorrect. Reinstalling the application may fix this problem.
	*/

	if(asy)
		return true;
	else
	   ::WaitForSingleObject(pi.hProcess,INFINITE);

	return true;
#else
	 return false;
#endif

}
