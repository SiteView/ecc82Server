//////////////////////////////////////////////////////////////////////////////////
// SVDefines.h                                                                  //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////

#ifndef _SV_BASE_DEFINES_H_
#define _SV_BASE_DEFINES_H_

#ifdef WIN32
#define DLL_EXPORT extern "C" __declspec(dllexport)
#else
#define DLL_EXPORT extern "C"
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include <iostream>
#include <string>
#include <list>

using namespace std;

#ifdef WIN32
#pragma warning (disable : 4786)
#endif

//////////////////////////////////////////////////////////////////////////////////
// 

const	int	CPU_TYPE_MONITOR	 =	0x01;
const	int	MEMORY_TYPE_MONITOR	 =	0x02;
const	int	DISK_TYPE_MONITOR	 =	0x03;
const	int	SERVICE_TYPE_MONITOR =	0x04;
const	int	FILE_TYPE_MONITOR	 =	0x15;
const	int	DISKS_TYPE_MONITOR	 =	0x05;
const	int	PMEMORY_TYPE_MONITOR =	0x12;

//////////////////////////////////////////////////////////////////////////////////
// About Siteview
// Version
const char SV_Version [] = "7.0.1.1" ;
// Copyright
const char SV_CopyRight [] = "Copyright 2000-2006 DragonFlow Inc." ;

//////////////////////////////////////////////////////////////////////////////////
// Base

// TPL ID
const char SV_TPLID [] = "_TemplateID=" ;
// SE ID
const char SV_SEID [] = "_SEID=" ;
// Entity ID
const char SV_EntityID [] = "_EntityID=" ;
// Group ID
const char SV_GroupID [] = "_GroupID=" ;
// Monitor ID
const char SV_MonitorID [] = "_MonitorID=" ;
// Entity Type
const char SV_EntityType [] = "_EntityType=" ;

//Domain Name
const char SV_DomainName [] ="_DomName=";
const char SV_MatchIP [] ="_MatchIP=";
// Host Name
const char SV_Host [] = "_Machinename=" ;
// User Name
const char SV_User [] = "_UserAccount=" ;
// Password
const char SV_Pwd [] = "_Password=" ;
// OS Type
const char SV_OSType [] = "_OsType=" ;
// Port
const char SV_Port [] = "_Port=" ;
// Login prompt
const char SV_LoginPrompt [] = "_LoginPrompt=" ;
// Password prompt
const char SV_PwdPrompt [] = "_PWPrompt=" ;
// Success prompt
const char SV_Prompt [] = "_Prompt=" ;
// Login Protocol Type (Telnet/SSH)
const char SV_ProtocolType [] = "_ProtocolType=" ;
// Use Private key file login UNIX
const char SV_UerKeyFile [] = "_UseKeyFile=" ;
// Private Key File
const char SV_KeyFile [] = "_PrivateKey=" ;
// Customer Path
const char SV_CustomerPath [] = "_CustomerPath=" ;

//////////////////////////////////////////////////////////////////////////////////
// SNMP
// Community
const char SV_Community [] = "_Community=" ;
// Version
const char SV_SNMPVer [] = "_SNMPVer=" ;
// Interface Index
const char SV_InterfaceIndex [] = "_InterfaceIndex=" ;
// SNMP Disk Interface Index
const char SV_SNMPDisk [] = "_DiskIndex=" ;
// Select Value
const char SV_SNMPSelvalue [] = "_SelValue=" ;
// Set Value
const char SV_SNMPSetValue [] = "_SetValue=" ;
// Net Set Type
const char SV_SNMPNetSetType [] = "_netsettype=" ;
// 
const char SV_SNMPMatchCont [] = "_matchcont=" ;

const char SV_SNMPIndex [] = "_index=" ;

const char SV_SNMPRequireID [] = "_RequireID=" ;

const char SV_SNMPDowntime [] = "_downtime" ;

//////////////////////////////////////////////////////////////////////////////////
//
// Disk 
const char SV_Disk [] = "_Disk=" ;
// File path
const char SV_FilePath [] = "_FilePath=" ;
// Folder
const char SV_Folder [] = "_Folder=" ;
// Service
const char SV_Service [] = "_Service=" ;
// Connect Port
const char SV_ConnPort [] = "_ConnPort=" ;
// Log File
const char SV_LogFile [] = "_LogFilePathname=" ;
// Process
const char SV_Process [] = "_ProcessName=" ;
// Window Event Log Name
const char SV_WinEventName [] = "_logName=" ;
// Window Event Log Type
const char SV_WinEventType [] = "_eventType=" ;
// Window Event Log Key
const char SV_WinEventKey [] = "_eventKey=" ;
// Window Event Log Match
const char SV_WinEventMatch [] = "_eventMachine=" ;
// Window Event Log EventCode Filter List
const char SV_WinEventCode [] = "_CodeList=" ;
// Window Event Log Source Filter List
const char SV_WinEventSource [] = "_SourceList=" ;
// 
const char SV_WinEventPosMatch [] = "_positiveMatch=" ;
//
const char SV_WinEventNegMatch [] = "_negativeMatch=" ;
//
const char SV_WinEventMachine [] = "_eventMachine=" ;
// Domain Host Name
const char SV_DomainHost [] = "_DomainMachine=" ;
// Domain User Name
const char SV_DomainUser [] = "_DomainUserAccount=" ;
// Domain User's Password
const char SV_DomainPwd [] = "_DomainPassword=" ;
// Unix Script
const char SV_UnixScript [] = "_Script=" ;
// Unix Total Memory
const char SV_UnixTotalMemory [] = "_totalMemory=" ;
// Unix Command
const char SV_UnixCommand [] = "_UnixCommand=" ;
//////////////////////////////////////////////////////////////////////////////////
// Proxy Server 

// Proxy Server & Port
const char SV_ProxyServer [] = "_ProxyServerPort=" ;
// Proxy Server User
const char SV_ProxyUser [] = "_ProxyUser=" ;
// Proxy Server Password
const char SV_ProxyPwd [] = "_ProxyPass=" ;
// Proxy Type
const char SV_ProxyType [] = "_ProxyType=";
// Passive Mode
const char SV_PassiveMode [] = "_PassiveMode=";
/////////////////////////////////////////////////////////////////////////////////
//
// Match String 
const char SV_MatchStr [] = "_MatchStr=" ;
// Send String
const char SV_SendStr [] = "_SendStr=" ;

//////////////////////////////////////////////////////////////////////////////////
// URL

// URL
const char SV_Url [] = "_URL=" ;
// Post Data
const char SV_UrlPost [] = "_PostData=" ;

//////////////////////////////////////////////////////////////////////////////////
// Time Out

// Time out
const char SV_TimeOut [] = "_Timeout=" ;
// Connect Timeout
const char SV_ConnTimeout [] = "_ConnTimeout=" ;
// Execute Query Timeout
const char SV_QueryTimeout [] = "_QueryTimeout=" ;

//////////////////////////////////////////////////////////////////////////////////
// FTP
// User Name
const char SV_FtpUserName [] = "_FTPUserAccount=" ;
// User's password
const char SV_FtpPassword [] = "_FTPPassWord=" ;
// Server Port
const char SV_FtpServerPort [] = "_ServerPort=" ;
// Test Download
const char SV_FtpDownlad [] = "_DownLoadFile=" ;

// Function Define
//////////////////////////////////////////////////////////////////////////////////
// Net
int SV_NetInit();
void SV_NetFree();
char *  strrstr ( const char * str1, const char * str2 );
char *  strtriml(char * str1);
char *  strtrimr(char * str1);
//////////////////////////////////////////////////////////////////////////////////
#ifndef _DWORD_DEFINED
#define _DWORD_DEFINED
typedef unsigned long DWORD;
#endif

#include <time.h>

inline void SetRandSeed(){ srand((unsigned)time(NULL)); };



#ifdef WIN32
#include <WINSOCK2.H>
#pragma comment (lib, "wsock32.lib")
#pragma warning (disable : 4089)
#else
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <unistd.h>


typedef u_int           SOCKET;
#define INVALID_SOCKET (SOCKET)(~0)
#define SOCKET_ERROR (-1)

inline int closesocket(SOCKET s){ close(s); };
inline long GetTickCount() { time_t tm;    return time(&tm);};
// 1 milisecond = 1000 microsecond
// Windows  Sleep 使用毫秒 miliseconds 
// Linux  usleep 使用微秒 microsecond 
// 由于原来的代码是在 Windows 中使用的，所以参数要有一个微秒到毫秒的转换
inline void Sleep(unsigned int useconds) { usleep(useconds * 1000); };

char * _itoa ( int val, char *buf, int radix );

static void xtoa ( unsigned long val, char *buf, unsigned radix, int is_neg );

int stricmp (
        const char * dst,
        const char * src
        );

int _strcmpi(const char * dst, const char * src);

#endif

#endif

