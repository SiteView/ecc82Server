// stdafx.h : 标准系统包含文件的包含文件，
// 或是常用但不常更改的项目特定的包含文件
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
// Windows 头文件:
//#include <windows.h>

// TODO: 在此处引用程序要求的附加头文件
#include "afxcoll.h"
#include <Winsock2.h>
#include <atlstr.h>
#include <tchar.h>
#include <iostream>
#pragma comment(lib,"Ws2_32.lib")

#define		NUL					0
#define		BEL					7
#define		BS					8
#define		HT					9
#define		LF					10
#define		VT					11
#define		FF					12
#define		CR					13
#define		SE					240
#define		NOP					241
#define		DM					242
#define		BRK					243
#define		IP					244
#define		AO					245
#define		AYT					246
#define		EC					247
#define		EL					248
#define		GA					249
#define		SB					250
#define		WILL				251
#define		WONT				252
#define		DO					253
#define		DONT				254
#define		IAC					255

#define		NUM_TERMINALS		2
#define		NUM_CODEC			6
#define		READ_TIMEOUT		20*3*3
//#define		CONNECT_TIMEOUT		20*3*3
#define		CONNECT_TIMEOUT		20

#define		PR_NUM				10
#define		C_STA				"DF-CMD-STA"
#define		C_END				"DF-CMD-END"
#define		BUF_SIZE			10 * 1024


#define		__MACHINETYPE__					"_MachineType="
#define		__OSTYPE__						"_OsType="
#define		__MACHINENAME__					"_MachineName="
#define		__REMOTEMACHINENAME__			"_RemoteMachineName="
#define		__USERACCOUNT__					"_UserAccount="
#define		__PASSWORD__					"_PassWord="
#define		__PROMPT__						"_Prompt="
#define		__LOGINPROMPT__					"_LoginPrompt="
#define		__PASSWORDPROMPT__				"_PWPrompt="
#define		__PROTOCOLTYPE__				"_ProtocolType="
#define		__SERVERPORT__					"_MachineName="
#define		__DOWNLOADFILE__				"_DownLoadFile="
#define		__TIMEOUT__						"_TimeOut="
#define		__PORT__						"_Port="
#define		__CONNPORT__					"_connPort="
#define		__SENDSTR__						"_SendStr="
#define		__MATCHSTR__					"_MatchStr="
#define		__DSNNAME__						"_DSN="
#define		__DSNNAME1__					"_DsnName="

#define		__SQLSTR__						"_SqlStr="
#define		__REQUIREID__					"_RequireID="
#define		__COMMUNITY__					"_Community="
#define		__DOMAINNAME__					"_DomName="
#define		__MATCHIP__						"_MatchIP="
#define		__DIRECTORY__					"_Directory="
#define		__ISSUBDIR__					"_IsSubDir="
#define		__FILEPATH__					"_FilePath="
#define		__SERVICE__						"_Service="
#define		__DISK__						"_Disk="
#define		__SERVERMANURL__				"_ServerManUrl="
#define		__SENDBYTES__					"_SendBytes="
#define		__CPUNUM__						"_CpuNum="
#define		__DETAIL__						"_Detail="
#define		__TRACE__						"_Trace="

#define		__PROXYSERVERPORT__				"_ProxyServerPort="
#define		__PROXYUSERNAME__				"_ProxyUser="
#define		__PROXYPASSWORD__				"_ProxyPass="
#define		__RETRYDELAY__					"_RetryDelay="
#define		__CONNECTTIMEOUT__				"_ConnTimeout="
#define		__QUERYTIMEOUT__				"_QueryTimeout="
#define		__PASSIVEMODE__					"_PassiveMode="
#define		__PROVIDER__					"_Provider="
#define		__PRINCIPAL__					"_Principal="
#define		__CREDENTIALS__					"_Credentials="
#define		__QUERY__						"_Query="
#define		__FILTER__						"_Filter="
#define		__SMTPSERVERPORT__				"_SmtpServPort="
#define		__RCPTSERVERPORT__				"_RcptServPort="
#define		__MAILTYPE__					"_MailType="
#define		__MAILPROTOCOLTYPE__			"_MailProtocolType="
#define		__FROMADDRESS__					"_SendAddress="
#define		__RCPTADDRESS__					"_RcptAddress="
#define		__CHECKDELAY__					"_CheckDelay="
#define		__ATTACHMENT__					"_Attachment="
#define		__SECRET__						"_Secret="
#define		__INDEX__						"_Index="
#define		__INTERFACEINDEX__				"_InterfaceIndex="
#define		__GROUP__						"_Group="
#define		__CONNECTADDRESS__				"_ConnAddr="
#define		__URL__							"_URL="
#define		__STOPTIME__					"_StopTime="
#define		__PROXYTYPE__					"_ProxyType="
#define		__POSTDATA__					"_PostData="
#define		__ENABLEPASSWORD__				"_EnablePassword="
#define		__ENABLEPASSWORDPROMPT__		"_EnablePasswordPrompt="
#define		__ENABLEPROMPT__				"_EnalbePrompt="
#define		__VERIFYUSERNAME__				"_VerifyUsername="
#define		__VERIFYPASSWORD__				"_VerifyPassword="
#define		__REQUESTADDRESS__				"_RequestAddress="
#define		__MQQUEUEMANAGER__				"_ManageQueue="
#define		__MQQUEUENAME__					"_QueueName="
#define     __MQCHANNELNAME__               "_ChannelName="
#define		__INSTANCE__					"_Instance="
#define		__ALERTTYPE__					"_AlertType="
#define		__LOGFILE__						"_LogFilePathname="
#define		__RULEFILE__					"_RuleFile="
#define		__CUSTOMERPATH__				"_CustomerPath="
#define		__APPENDMETHOD__				"_AppendMethod="
#define		__GTJAID__						"_ID="
#define		__PARAMETERS__					"_Parameters="
#define		__SCRIPT__						"_Script="
#define		__SERVERNAME__					"_ServerName="
#define		__NEEDAUTH__					"_IsSMTPAu="
#define		__VPNCONNECTION__				"_VpnConn="
#define		__DIALBEFOREVPN__				"_DialBeforeVpn="
#define		__DIALCONNECTION__				"_DialConn="
#define		__USERACCOUNTD__				"_UserAccountD="
#define		__PASSWORDD__					"_PassWordD="
#define		__DOMAINNAMED__					"_DomNameD="
#define		__CLUSTERLOG__					"_ClusterLogPath="
#define		__RUNNING_CONFIG__				"_RunCmd="
#define		__RUNNING_CONFIG__				"_RunCmd="
#define		__NETWORK_TYPE__				"_type="

#define		__PRIVATE__						"_private="

#define		__GROUPID__						"_GroupID="  // xiao quan
#define		__MONITORID__					"_MonitorID="// xiao quan

#define     __TOTAL_MEM__                   "_total_mem="
#define     __ALWAYSMONITOR__				"_monitorcondition=" //for LOGFILE MONITOR

#define     __PRIKEYPATH__               "_PriKeyPath="


typedef struct st_gd{
	int		term_index;
	char	databuffer[BUF_SIZE];
	int		datalen;
	char	codebuf[256];
	unsigned char codeptr;
}GDATA,*LPGDATA;


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


struct Tel_Param
{
	int state;
	_verb verb ;
	LPDATAPROC DataProc;
};

