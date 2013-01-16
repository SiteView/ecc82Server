// UrlMonitor.h : main header file for the URLMONITOR DLL
//

#if !defined(AFX_URLMONITOR_H__FD98B18C_6202_4757_BA9A_844B253C9D37__INCLUDED_)
#define AFX_URLMONITOR_H__FD98B18C_6202_4757_BA9A_844B253C9D37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CUrlMonitorApp
// See UrlMonitor.cpp for the implementation of this class
//

class CUrlMonitorApp : public CWinApp
{
public:
	CUrlMonitorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUrlMonitorApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CUrlMonitorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//--------------------------------------------------written by chenxingang
#define ERROR_WINHTTP_AUTO_PROXY_SERVICE_ERROR					12178
#define ERROR_WINHTTP_AUTODETECTION_FAILED						12180
#define ERROR_WINHTTP_BAD_AUTO_PROXY_SCRIPT						12166
#define ERROR_WINHTTP_CANNOT_CALL_AFTER_OPEN					12103
#define ERROR_WINHTTP_CANNOT_CALL_AFTER_SEND					12102
#define ERROR_WINHTTP_CANNOT_CALL_BEFORE_OPEN					12100
#define ERROR_WINHTTP_CANNOT_CALL_BEFORE_SEND					12101
#define ERROR_WINHTTP_CANNOT_CONNECT							12029
#define ERROR_WINHTTP_CHUNKED_ENCODING_HEADER_SIZE_OVERFLOW		12183
#define ERROR_WINHTTP_CLIENT_AUTH_CERT_NEEDED					12044
#define ERROR_WINHTTP_CONNECTION_ERROR							12030
#define ERROR_WINHTTP_HEADER_ALREADY_EXISTS						12155
#define ERROR_WINHTTP_HEADER_COUNT_EXCEEDED						12181
#define ERROR_WINHTTP_HEADER_NOT_FOUND							12150
#define ERROR_WINHTTP_HEADER_SIZE_OVERFLOW						12182
#define ERROR_WINHTTP_INCORRECT_HANDLE_STATE					12019
#define ERROR_WINHTTP_INCORRECT_HANDLE_TYPE						12018
#define ERROR_WINHTTP_INTERNAL_ERROR							12004
#define ERROR_WINHTTP_INVALID_OPTION							12009
#define ERROR_WINHTTP_INVALID_QUERY_REQUEST						12154
#define ERROR_WINHTTP_INVALID_SERVER_RESPONSE					12152
#define ERROR_WINHTTP_INVALID_URL								12005
#define ERROR_WINHTTP_LOGIN_FAILURE								12015
#define ERROR_WINHTTP_NAME_NOT_RESOLVED							12007
#define ERROR_WINHTTP_NOT_INITIALIZED							12172
#define ERROR_WINHTTP_OPERATION_CANCELLED						12017
#define ERROR_WINHTTP_OPTION_NOT_SETTABLE						12011
#define ERROR_WINHTTP_OUT_OF_HANDLES							12001
#define ERROR_WINHTTP_REDIRECT_FAILED							12156
#define ERROR_WINHTTP_RESEND_REQUEST							12032
#define ERROR_WINHTTP_RESPONSE_DRAIN_OVERFLOW					12184
#define ERROR_WINHTTP_SECURE_CERT_CN_INVALID					12038
#define ERROR_WINHTTP_SECURE_CERT_DATE_INVALID					12037
#define ERROR_WINHTTP_SECURE_CERT_REV_FAILED					12057
#define ERROR_WINHTTP_SECURE_CERT_REVOKED						12170
#define ERROR_WINHTTP_SECURE_CERT_WRONG_USAGE					12179
#define ERROR_WINHTTP_SECURE_CHANNEL_ERROR						12157
#define ERROR_WINHTTP_SECURE_FAILURE							12175
#define ERROR_WINHTTP_SECURE_INVALID_CA							12045
#define ERROR_WINHTTP_SECURE_INVALID_CERT						12169
#define ERROR_WINHTTP_SHUTDOWN									12012
#define ERROR_WINHTTP_TIMEOUT									12002
#define ERROR_WINHTTP_UNABLE_TO_DOWNLOAD_SCRIPT					12167
#define ERROR_WINHTTP_UNRECOGNIZED_SCHEME						12006
//--------------------------------------------------written by chenxingang


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#define		__MACHINETYPE__					"_MachineType="
#define		__OSTYPE__						"_OsType="
#define		__MACHINENAME__					"_MachineName="
#define		__USERACCOUNT__					"_URLUserAccount="
#define		__PASSWORD__					"_URLPassWord="
#define		__PROMPT__						"_Prompt="
#define		__LOGINPROMPT__					"_LoginPrompt="
#define		__PASSWORDPROMPT__				"_PWPrompt="
#define		__PROTOCOLTYPE__				"_ProtocolType="
#define		__SERVERPORT__					"_ServerPort="
#define		__DOWNLOADFILE__				"_DownLoadFile="
#define		__TIMEOUT__						"_TimeOut="
#define		__PORT__						"_Port="
#define		__SENDSTR__						"_SendStr="
#define		__MATCHSTR__					"_MatchStr="
#define		__DSNNAME__						"_DsnName="
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

#define		__GROUPID__						"_GroupID="  // xiao quan
#define		__MONITORID__					"_MonitorID="// xiao quan
#define		__CUSTOMER__					"_CustomerPath="// xiao quan

#define		__CUSTOMERPATH__				"_CustomerPath="

#define     __IP__							"_IP="		//lish
#define		__AND__							"&&"		//lish
#define     __OR__							"||"		//lish
#define     __CHARSET__						"_Charset=" //lish

#define     CHAR_GBK						0x01		//lish
#define		CHAR_BIG5						0x02		//lish
#define		CHAR_UTF8						0x03		//lish
#endif // !defined(AFX_URLMONITOR_H__FD98B18C_6202_4757_BA9A_844B253C9D37__INCLUDED_)
