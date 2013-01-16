// testmonitor.cpp : 定义控制台应用程序的入口点。
//
//#include "D:\monitor\monitor\monitor\Monitor.h"
//#include "D:\monitor\monitor\monitor\TelMonitor.h"
#include <afxsock.h>
#include "stdafx.h"
#include "windows.h"
#include <iostream>
#include <string>

#define MEMORY_TYPE_MONITOR 0x02
#define PR_NUM 10
#define BUF_SIZE 1000*1024

using namespace std;

typedef struct st_gd{
	int		term_index;
	char	databuffer[BUF_SIZE];
	int		datalen;
	char	codebuf[256];
	unsigned char codeptr;
}GDATA,*LPGDATA;

typedef void (*LPDATAPROC)(SOCKET,unsigned char ,LPGDATA);

enum _verb
{
  verb_sb   = 250,
  verb_will = 251,
  verb_wont = 252,
  verb_do   = 253, 
  verb_dont = 254
};

struct Tel_Param
{
    int state;
	_verb verb ;
	LPDATAPROC DataProc;
};

//#pragma comment(lib,"monitor.lib")

//extern "C" __declspec(dllexport) 
//BOOL MEMORY(const char *strParas,  char *szReturn,int & nSize);

//extern  bool  MonitorValue(char *server, int port, char *uid, char *pwd, 
//				  char *proxy_server, int proxy_port, char *proxy_uid, char *proxy_pwd, 
//				  char *logprompt, char *pwdprompt, char *prompts, 
//				  char *inifile, int mtype, char *extraparam, char *custpath, char *szReturn);

bool (*pMonitorValue)(char *, int , char *, char *, 
				      char *, int , char *, char *, 
				      char *, char *, char *, char *, 
					  int, char *, char *, char *);

BOOL (*pMEMORY)(const char *,  char *,int & );

int (*ptelnet_connect)(SOCKET hSocket, char *uid, char *pwd, 
				   char *logprompt, char *pwdprompt, char arprompt[PR_NUM][256],
                   LPGDATA pgd, struct Tel_Param *pParam);

HINSTANCE hd;


int _tmain(int argc, _TCHAR* argv[])
{
	hd = LoadLibrary("monitor.dll");
	if(hd != NULL)
	{
		//pMonitorValue = (bool (*)(char *,int ,char *,char *,char *,int ,char *,
			            // char *,char *,char *,char *,char *, int , char *, char *, char *)) 
						// GetProcAddress(hd,"MonitorValue");
		ptelnet_connect = (int (*)(SOCKET hSocket, char *uid, char *pwd, 
				   char *logprompt, char *pwdprompt, char arprompt[PR_NUM][256],
                   LPGDATA pgd, struct Tel_Param *pParam)) GetProcAddress(hd,"telnet_connect");
		pMEMORY = (BOOL (*)(const char *,  char *,int & )) GetProcAddress(hd,"MEMORY");
	}
	char *ps = "192.168.0.139";//机器名IP
	int iport = 23;//远程服务器端口号(telnet代理)
	char *puid = "root";//远程服务器的登录名
	char *ppwd = "root";//远程服务器的登录密码
	char *pproxy_server = "192.168.0.139";//代理服务器
	int  iproxy_port = 0;//代理服务器端口(telnet代理)
	char *pproxy_uid = NULL;//代理服务器登录名
	char *pproxy_pwd = NULL;//代理服务器登录密码
	char *plogprompt = "ogin:";//登录提示符
	char *ppwdprompt = "assword:";//密码提示符
	char *pprompts = "# $ >";//操作命令提示符
	char *pinifile = "Unix";//远程服务器的操作系统
	int imtype = 2;//监测类型
	char *pcustpath = "D:\\monitortest\\temp";
	char *pszReturn = NULL;//
	char cstr[255] = {0};
	SOCKET  hSocket;
	LPGDATA pgd;
	char arprompt[PR_NUM][256];

	int nSize = 0;
	const char *paras = "_MachineName=192.168.0.139 _Port=23 _UserAccount=root _PassWord=root _OsType=unix _prompt=#$> _LoginPrompt=ogin: _PWPrompt=assword: _ProxyServerPort=23 _ProxyUser=NULL _ProxyPass=NULL _CustomerPath=D:\\SiteView\\SiteView ECC\\temp _ProtocolType=0 ";
	if(pMEMORY)
	{
		pMEMORY(paras,pszReturn,nSize);
	}
	else
	{
		cout<<"pMEMORY is NULL(Pointer)!"<<endl;
	}

//	cout<<"nSize = "<<nSize<<endl;
//	if(pMonitorValue)
//	{
//		pMonitorValue(ps,iport,puid,ppwd,pproxy_server,iproxy_port,
//						pproxy_uid,pproxy_pwd,plogprompt,ppwdprompt,
//						pprompts,pinifile,imtype,NULL,pcustpath,pszReturn);
//	}
//	else
//	{
//		cout<<"pMonitorValue is NULL(Pointer)!"<<endl;
//	}
	struct Tel_Param *pParam;
	if(ptelnet_connect)
	{
		ptelnet_connect(hSocket, puid, ppwd, 
				   NULL, NULL, arprompt[0][0],pgd,pParam);
	}
	else
	{
		cout<<"ptelnet_connect is NULL(Pointer)!"<<endl;
	}

	FreeLibrary(hd);
	return 0;
}





