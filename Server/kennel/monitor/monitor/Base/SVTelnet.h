//SVTelnet.h
//-------------wang peng -------------------------------

#ifndef	SVTELNET_H 

#define SVTELNET_H 


#ifdef WIN32 //Modified by wanli for windows
#define socklen_t int
#include <afxwin.h>  
#include "Winsock2.h"
#pragma comment(lib, "wsock32.lib")
#else	//for Linux
#define	BOOL	bool
#define 	SOCKET	int		
#define SOCKET_ERROR -1
#define Sleep	sleepSelf
#define BYTE	char
#define INVALID_SOCKET -1
#define TRUE true
#define FALSE false
#define closesocket	close
#define WSAETIMEDOUT 10001
#define WSAEWOULDBLOCK 10002
#define ioctlsocket ioctl
#define lstrcpy	strcpy
#define lstrlen	strlen

#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/ioctl.h>
#endif

#include <string>
using namespace std;

#define		TELNET_BUF_SIZE			500 * 1024 //Edit 2005-04-15 By Kevin.Yang
#define		PR_NUM				10
enum
{
  SB_TERM_IS = 0,
  SB_TERM_SEND = 1
};

enum _ansi_state
{
  as_normal,
  as_esc,
  as_esc1
};

enum _verb
{
  verb_sb   = 250,
  verb_will = 251,
  verb_wont = 252,
  verb_do   = 253, 
  verb_dont = 254
};

enum _state
{
  state_data,   //we expect a data byte
  state_code,   //we expect a code
  state_option  //we expect an option
};

enum _option
{
  TOPT_BIN = 0,   // Binary Transmission
  TOPT_ECHO = 1,  // Echo
  TOPT_RECN = 2,  // Reconnection
  TOPT_SUPP = 3,  // Suppress Go Ahead
  TOPT_APRX = 4,  // Approx Message Size Negotiation
  TOPT_STAT = 5,  // Status
  TOPT_TIM = 6,   // Timing Mark
  TOPT_REM = 7,   // Remote Controlled Trans and Echo
  TOPT_OLW = 8,   // Output Line Width
  TOPT_OPS = 9,   // Output Page Size
  TOPT_OCRD = 10, // Output Carriage-Return Disposition
  TOPT_OHT = 11,  // Output Horizontal Tabstops
  TOPT_OHTD = 12, // Output Horizontal Tab Disposition
  TOPT_OFD = 13,  // Output Formfeed Disposition
  TOPT_OVT = 14,  // Output Vertical Tabstops
  TOPT_OVTD = 15, // Output Vertical Tab Disposition
  TOPT_OLD = 16,  // Output Linefeed Disposition
  TOPT_EXT = 17,  // Extended ASCII
  TOPT_LOGO = 18, // Logout
  TOPT_BYTE = 19, // Byte Macro
  TOPT_DATA = 20, // Data Entry Terminal
  TOPT_SUP = 21,  // SUPDUP
  TOPT_SUPO = 22, // SUPDUP Output
  TOPT_SNDL = 23, // Send Location
  TOPT_TERM = 24, // Terminal Type
  TOPT_EOR = 25,  // End of Record
  TOPT_TACACS = 26, // TACACS User Identification
  TOPT_OM = 27,   // Output Marking
  TOPT_TLN = 28,  // Terminal Location Number
  TOPT_3270 = 29, // Telnet 3270 Regime
  TOPT_X3 = 30,  // X.3 PAD
  TOPT_NAWS = 31, // Negotiate About Window Size
  TOPT_TS = 32,   // Terminal Speed
  TOPT_RFC = 33,  // Remote Flow Control
  TOPT_LINE = 34, // Linemode
  TOPT_XDL = 35,  // X Display Location
  TOPT_ENVIR = 36,// Telnet Environment Option
  TOPT_AUTH = 37, // Telnet Authentication Option
  TOPT_NENVIR = 39,// Telnet Environment Option
  TOPT_EXTOP = 255, // Extended-Options-List
  TOPT_ERROR = 256  // Magic number
};
//////////////////////////////////////////////////////////////chen
typedef struct st_gd{
	int		term_index;
	char	databuffer[TELNET_BUF_SIZE];
	int		datalen;
	char	codebuf[256];
	unsigned char codeptr;
}GDATA,*LPGDATA;
//////////////////////////////////////////////////////////////
typedef void(*LPOPTIONPROC)(SOCKET, _verb, _option);
typedef void(*LPDATAPROC)(SOCKET, unsigned char data,LPGDATA pgd);
typedef void (*LPCODEPROC)(char*);

struct OL
{
  _option option;
  LPOPTIONPROC OptionProc;
  LPDATAPROC DataProc;
};

struct TERMINAL
{
  char* name;
  LPDATAPROC termproc;
  //pre requsites.
};

struct CODEC
{
  unsigned char cmd;
  LPCODEPROC proc;
};

void ddww_echo(SOCKET server, _verb verb, _option option);
void ddww_supp(SOCKET server, _verb verb, _option option);
void ddww_term(SOCKET server, _verb verb, _option option);
void sbproc_term(SOCKET server, unsigned char data,LPGDATA pgd);
void ddww_error(SOCKET server, _verb verb, _option option);

void nvt(SOCKET server, unsigned char data,LPGDATA pgd);
void ansi(SOCKET server, unsigned char data,LPGDATA pgd);

void ansi_set_screen_attribute(char* buffer);
void ansi_erase_line(char* buffer);
void ansi_set_position(char* buffer);
void ansi_erase_screen(char* buffer);
void ansi_move_up(char* buffer);





// 该类的主要功能是Telnet到远程主机
class SVUnixOpMonitor {

 public:
	int RESPONSE_WAIT(SOCKET s, int nsec);

#ifndef	WIN32	
	inline void sleepSelf(int msec)
	{
		usleep(msec*1000);
		
	};
#endif

 private:
	 /*
	  char * strUser;//用户名
	  char * strUserPrompt;//login提示符
	  char * strPwd;//密码
	  char * strPwdPrompt;//密码提示符
	  char * strCmd;	    //命令
	  char * strRemoteHost;//远程主机
	  char * strLoginPrompt; //登陆提示符
	  */
      int state ;
	  _verb verb;
	  LPDATAPROC DataProc;
	  char *strUser;//用户名
	  char *strUserPrompt;//login提示符
	  char *strPwd;//密码
	  char *strPwdPrompt;//密码提示符
	  char *strCmd;	    //命令
	  char *strRemoteHost;//远程主机
	  char *strLoginPrompt; //登陆提示符

	  int nPort;			  //端口
	  int nWaiting;		  //超时时间
      
	  
	  SOCKET hSocket;	
	  LPGDATA pgd;
	  
	  char	arprompt[PR_NUM][256];
	   

  public:
	int connect_timeo(int sockfd,const struct sockaddr *saptr,int salen,int nsec);

	BOOL init(  char * RemoteHost,int Port,
		char * User,  char * Pwd, 
		char * UserPrompt,  char * PwdPrompt, char * LoginPrompt);

	bool init(  string RemoteHost,int Port,
		string User,  string Pwd, 
		string UserPrompt,  string PwdPrompt, string LoginPrompt);

	int telnetOpen();//登录执行命令
	int telnetExecCmd(char * strCmd); //在连接后执行命令，必须在登录后执行。
	int SSH();
	int SSHCMD(char *strCmd);
	BOOL SVFAILED(int nResult);
	BOOL telnetclose();		
    int telnet_setlinewidth(BYTE HighByte, BYTE LowByte);
	
	SVUnixOpMonitor();
	~SVUnixOpMonitor();
#ifdef WIN32
	static  int  WSA_Init()
	{
		WORD wVersionRequested;  
		WSADATA wsaData; 
		int err; 
		wVersionRequested = MAKEWORD(1, 1); 

		err = WSAStartup(wVersionRequested, &wsaData); 

		if (err != 0) 
			return -1; 

		if ( LOBYTE( wsaData.wVersion ) != 1 || 
		   HIBYTE( wsaData.wVersion ) != 1 ) 
		{ 
			WSACleanup(); 
			return -2;
		}

		return 0;

	}
	static void WSA_Free()
	{
			WSACleanup();
	}
#else
	static  int  WSA_Init(){return 0;};
	static void WSA_Free(){};
#endif

	void SetUserPrompt(const char * UserPrompt){
		if(strUserPrompt!=NULL)
			free(strUserPrompt);
		strUserPrompt =(char *) malloc(strlen(UserPrompt)+1);
		memset(strUserPrompt,0,strlen(UserPrompt)); 
		strcpy(strUserPrompt ,UserPrompt);
		}
	char * GetUserPrompt(){
		return strUserPrompt;		}
		
	void SetUser(const char * User){
		if(strUser!=NULL)
			free( strUser);
		strUser =(char *) malloc(strlen(User)+1);
		strcpy(strUser ,User);
	}
	char * GetUser(){
		return strUser;		}
		
	void SetCmd(const char * Cmd){  //设置命令
		if(strCmd!=NULL)
			free( strCmd);
		strCmd =(char *) malloc(strlen(Cmd)+1);
		strcpy(strCmd ,Cmd);	}
	char * GetCmd(){
		return strCmd;		}
		

	void SetLoginPrompt(const char * LoginPrompt);
	char * GetLoginPrompt(){
		return strLoginPrompt;
	}

	void SetRemoteHost(const char * RemoteHost){
		if(strRemoteHost!=NULL)
			free( strRemoteHost);
		strRemoteHost =(char *) malloc(strlen(RemoteHost)+1);
		strcpy(strRemoteHost ,RemoteHost);
	}
	char * GetRemoteHost(){
		return strRemoteHost;		}

	void SetPwd(const char * Pwd){
		if(strPwd!=NULL)
			free( strPwd);
		strPwd =(char *) malloc(strlen(Pwd)+1);
		strcpy(strPwd ,Pwd);
	}
	char * GetPwd(){
		return strPwd;		}

	void SetPwdPrompt(const char * PwdPrompt){
		if(strPwdPrompt!=NULL)
			free(strPwdPrompt);
		strPwdPrompt =(char *) malloc(strlen(PwdPrompt)+1);
		strcpy(strPwdPrompt ,PwdPrompt);
	}
	char * GetPwdPrompt(){
		return strPwdPrompt;	}

	void SetPort(int Port){
		nPort =Port;
	}
	int GetPort(){
		return nPort;
	}
	void SetTimeOut(int Timeout){
		nWaiting =Timeout;
	}
	int GetTimeOut(){
		return nWaiting;
	}
	char *GetResultBuff(){
		if(pgd!=NULL)
			return pgd->databuffer;
		else return NULL;
	}
    
  private:
    BOOL init();
	
	void telnet_protocol(SOCKET server,unsigned char code,LPGDATA pgd);
	int telnetinit();
	int SVUnixOpMonitor::telnet_command();
	int telnet_connect();

	int executeExeFile(char* strCmdLine);


};
#define  TELNETSUCCESS 0
#define  HOSTERROR -1           //主机为空或无法解析 
#define  SOCKETINITERROR -2     //socket初始化异常
#define  CONNECTTIMEOUTERROR -3 //连接超时
#define  FISTCONNECTERROR -4  //等待初始信息超时 

#define  USERLOGINERROR -5    //输入用户名后等待超时

#define  PWDLOGINERROR -6     //输入密码后等待超时

#define  LOGINSOCKETERROR     -7 //socket出异常

#define  LOGININCORRECTERROR     -8 //登录用户名不正确
#define  TELNETCOMMANDERROR     -9 //socket出异常

#endif
