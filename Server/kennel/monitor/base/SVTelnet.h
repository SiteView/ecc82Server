
#ifndef	SV_ECC_TELNET_H_ 
#define SV_ECC_TELNET_H_

#pragma once

#define		TELNET_BUF_SIZE			500 * 1024 //Edit 2005-04-15 By Kevin.Yang
#define		PR_NUM				10

#include "SVDefines.h"

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
  TOPT_BIN = 0,         // Binary Transmission
  TOPT_ECHO = 1,        // Echo
  TOPT_RECN = 2,        // Reconnection
  TOPT_SUPP = 3,        // Suppress Go Ahead
  TOPT_APRX = 4,        // Approx Message Size Negotiation
  TOPT_STAT = 5,        // Status
  TOPT_TIM = 6,         // Timing Mark
  TOPT_REM = 7,         // Remote Controlled Trans and Echo
  TOPT_OLW = 8,         // Output Line Width
  TOPT_OPS = 9,         // Output Page Size
  TOPT_OCRD = 10,       // Output Carriage-Return Disposition
  TOPT_OHT = 11,        // Output Horizontal Tabstops
  TOPT_OHTD = 12,       // Output Horizontal Tab Disposition
  TOPT_OFD = 13,        // Output Formfeed Disposition
  TOPT_OVT = 14,        // Output Vertical Tabstops
  TOPT_OVTD = 15,       // Output Vertical Tab Disposition
  TOPT_OLD = 16,        // Output Linefeed Disposition
  TOPT_EXT = 17,        // Extended ASCII
  TOPT_LOGO = 18,       // Logout
  TOPT_BYTE = 19,       // Byte Macro
  TOPT_DATA = 20,       // Data Entry Terminal
  TOPT_SUP = 21,        // SUPDUP
  TOPT_SUPO = 22,       // SUPDUP Output
  TOPT_SNDL = 23,       // Send Location
  TOPT_TERM = 24,       // Terminal Type
  TOPT_EOR = 25,        // End of Record
  TOPT_TACACS = 26,     // TACACS User Identification
  TOPT_OM = 27,         // Output Marking
  TOPT_TLN = 28,        // Terminal Location Number
  TOPT_3270 = 29,       // Telnet 3270 Regime
  TOPT_X3 = 30,         // X.3 PAD
  TOPT_NAWS = 31,       // Negotiate About Window Size
  TOPT_TS = 32,         // Terminal Speed
  TOPT_RFC = 33,        // Remote Flow Control
  TOPT_LINE = 34,       // Linemode
  TOPT_XDL = 35,        // X Display Location
  TOPT_ENVIR = 36,      // Telnet Environment Option
  TOPT_AUTH = 37,       // Telnet Authentication Option
  TOPT_NENVIR = 39,     // Telnet Environment Option
  TOPT_EXTOP = 255,     // Extended-Options-List
  TOPT_ERROR = 256      // Magic number
};
//////////////////////////////////////////////////////////////chen
typedef struct st_gd
{
    int		term_index;
    char	databuffer[TELNET_BUF_SIZE];
    int		datalen;
    char	codebuf[256];
    unsigned char codeptr;
}GDATA, *LPGDATA;
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
};

struct CODEC
{
  unsigned char cmd;
  LPCODEPROC proc;
};

void ddww_echo  (SOCKET server, _verb verb, _option option);
void ddww_supp  (SOCKET server, _verb verb, _option option);
void ddww_error (SOCKET server, _verb verb, _option option);
void ddww_term  (SOCKET server, _verb verb, _option option);
void sbproc_term(SOCKET server, unsigned char data,LPGDATA pgd);


void nvt    (SOCKET server, unsigned char data, LPGDATA pgd);
void ansi   (SOCKET server, unsigned char data, LPGDATA pgd);

void ansi_set_screen_attribute  (char* buffer);
void ansi_erase_line            (char* buffer);
void ansi_set_position          (char* buffer);
void ansi_erase_screen          (char* buffer);
void ansi_move_up               (char* buffer);


// 该类的主要功能是Telnet到远程主机
class SVUnixOpMonitor 
{
private:
    _verb verb;
    LPDATAPROC DataProc;
    char      * m_pszUser;         //用户名
    char      * m_pszUserPrompt;   //login提示符
    char      * m_pszPwd;          //密码
    char      * m_pszPwdPrompt;    //密码提示符
    char      * m_pszCmd;	        //命令
    char      * m_pszRemoteHost;   //远程主机
    char      * m_pszLoginPrompt;  //登陆提示符

    int       m_nPort;		    //端口
    int       m_nWaiting;        //超时时间
    int       m_nState ;      

    SOCKET hSocket;	
    LPGDATA pgd;

    char	arprompt[PR_NUM][256]; 
public:
    int     telnetOpen();                           //登录执行命令
    int     telnetExecCmd(const char * pszCmd);     //在连接后执行命令，必须在登录后执行。
    int     SSH();
    int     SSHCMD(const char *strCmd);		
    int     telnet_setlinewidth(BYTE HighByte, BYTE LowByte);
    bool    SVFAILED(int nResult);
    bool    telnetclose();
    bool    init(const char * pszRemoteHost, const int &nPort, const char * pszUser, 
                const char * pszPwd, const char * pszUserPrompt, const char * pszPwdPrompt,
                const char * pszLoginPrompt);

    SVUnixOpMonitor();
    ~SVUnixOpMonitor();
////////////////////////////////////////////////////////////////////////////////////////////
	void SetUserPrompt(const char * pszUserPrompt)
    {
        if(m_pszUserPrompt != NULL)
            delete []m_pszUserPrompt;
        m_pszUserPrompt = new char[strlen(pszUserPrompt) + 1];
        if(m_pszUserPrompt)
        {
            memset(m_pszUserPrompt, 0, strlen(pszUserPrompt)); 
            strcpy(m_pszUserPrompt, pszUserPrompt);
        }
    }
////////////////////////////////////////////////////////////////////////////////////////////
	char * GetUserPrompt() const
    {
		return m_pszUserPrompt;		
    }
////////////////////////////////////////////////////////////////////////////////////////////		
	void SetUser(const char * pszUser)
    {
		if(m_pszUser != NULL)
			delete []m_pszUser;
		m_pszUser = new char[strlen(pszUser) + 1];
        if(m_pszUser)
        {
            memset(m_pszUser, 0, strlen(pszUser));
		    strcpy(m_pszUser, pszUser);
        }
	}
////////////////////////////////////////////////////////////////////////////////////////////
	char * GetUser() const
    {
		return m_pszUser;		
    }
////////////////////////////////////////////////////////////////////////////////////////////		
	void SetCmd(const char * pszCmd)
    {  //设置命令
		if(m_pszCmd != NULL)
			delete []m_pszCmd;
		m_pszCmd = new char[strlen(pszCmd) + 1];
        if(m_pszCmd)
		    strcpy(m_pszCmd , pszCmd);	
    }
////////////////////////////////////////////////////////////////////////////////////////////
	char * GetCmd() const
    {
		return m_pszCmd;
    }		
////////////////////////////////////////////////////////////////////////////////////////////
	void SetLoginPrompt(const char * LoginPrompt);
	char * GetLoginPrompt() const
    {
		return m_pszLoginPrompt;
	}
////////////////////////////////////////////////////////////////////////////////////////////
	void SetRemoteHost(const char * RemoteHost)
    {
		if(m_pszRemoteHost != NULL)
			delete []m_pszRemoteHost;
		m_pszRemoteHost = new char[strlen(RemoteHost) + 1];
        if(m_pszRemoteHost)
		    strcpy(m_pszRemoteHost ,RemoteHost);
	}
////////////////////////////////////////////////////////////////////////////////////////////
	char * GetRemoteHost() const
    {
		return m_pszRemoteHost;	
    }
////////////////////////////////////////////////////////////////////////////////////////////
	void SetPwd(const char * pszPwd)
    {
		if(m_pszPwd != NULL)
			delete m_pszPwd;
		m_pszPwd = new char[strlen(pszPwd) + 1];
        if(m_pszPwd)
		    strcpy(m_pszPwd, pszPwd);
	}
////////////////////////////////////////////////////////////////////////////////////////////
	char * GetPwd() const
    {
		return m_pszPwd;		
    }
////////////////////////////////////////////////////////////////////////////////////////////
	void SetPwdPrompt(const char * pszPwdPrompt)
    {
		if(m_pszPwdPrompt != NULL)
			delete []m_pszPwdPrompt;
		m_pszPwdPrompt = new char[strlen(pszPwdPrompt) + 1];
        if(m_pszPwdPrompt)
		strcpy(m_pszPwdPrompt, pszPwdPrompt);
	}
////////////////////////////////////////////////////////////////////////////////////////////
	char * GetPwdPrompt() const
    {
		return m_pszPwdPrompt;	
    }
////////////////////////////////////////////////////////////////////////////////////////////
	void SetPort(const int &nPort)
    {
		m_nPort = nPort;
	}
////////////////////////////////////////////////////////////////////////////////////////////
	int GetPort()
    {
		return m_nPort;
	}
////////////////////////////////////////////////////////////////////////////////////////////
	void SetTimeOut(const int &nTimeout) 
    {
		m_nWaiting = nTimeout;
	}
////////////////////////////////////////////////////////////////////////////////////////////
	int GetTimeOut() const
    {
		return m_nWaiting;
	}
////////////////////////////////////////////////////////////////////////////////////////////
	char *GetResultBuff() const
    {
		if(pgd != NULL)
			return pgd->databuffer;
		else 
            return NULL;
	}
////////////////////////////////////////////////////////////////////////////////////////////    
private:
	void    telnet_protocol(SOCKET server,unsigned char code,LPGDATA pgd);
	int     telnetinit();
	int     SVUnixOpMonitor::telnet_command();
	int     telnet_connect();
	int     executeExeFile(const string &szCmdLine);

};

#define  TELNETSUCCESS          0
#define  HOSTERROR              -1          //主机为空或无法解析 
#define  SOCKETINITERROR        -2          //socket初始化异常
#define  CONNECTTIMEOUTERROR    -3          //连接超时
#define  FISTCONNECTERROR       -4          //等待初始信息超时 
#define  USERLOGINERROR         -5          //输入用户名后等待超时
#define  PWDLOGINERROR          -6          //输入密码后等待超时
#define  LOGINSOCKETERROR       -7          //socket出异常
#define  LOGININCORRECTERROR    -8          //登录用户名不正确
#define  TELNETCOMMANDERROR     -9          //socket出异常



#endif

