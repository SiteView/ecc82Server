
/************************************************************************************
 * CURL Return Codes:																*
 * ---------------------															*																					*
 * 1    Unsupported protocol. This build of curl has no support for this protocol.	*
 * 2    Failed to initialize.														*
 * 3    URL malformat. The syntax was not correct.									*
 * 4    URL user malformatted. The user-part of the URL  syntax was not correct.	*
 * 5    Couldn't  resolve proxy. The given proxy host could not be resolved.		*
 * 6    Couldn't resolve host. The given remote  host  was  not resolved.			*
 * 7    Failed to connect to host.													*
 * 8    FTP  weird  server  reply.  The  server  sent data curl couldn't parse.		*
 * 9    FTP access denied. The server denied login.									*
 * 10   FTP user/password incorrect. Either one  or  both  were not accepted by the	* 
 *		server.																		*
 * 11   FTP  weird  PASS  reply.  Curl couldn't parse the reply sent to the PASS	*
 *		request.																	*
 * 12   FTP weird USER reply. Curl  couldn't  parse  the  reply sent to the USER	*
 *		request.																	*
 * 13   FTP  weird  PASV  reply,  Curl couldn't parse the reply sent to the PASV	*
 *		request.																	*
 * 14   FTP weird 227 format. Curl couldn't parse the  227-line the server sent.	*
 * 15   FTP can't get host. Couldn't resolve the host IP we got in the 227-line.	*
 * 16   FTP can't reconnect. Couldn't connect to  the  host  we got in the 227-line.*
 * 17   FTP  couldn't  set  binary.  Couldn't  change  transfer method to binary.	*
 * 18   Partial file. Only a part of the file was transfered.						*
 * 19   FTP couldn't RETR file. The RETR command failed.							*
 * 20   FTP write error. The transfer was reported bad  by  the server.				*
 * 21   FTP  quote  error.  A quote command returned error from the server.			*
 * 22   HTTP not found. The requested page was not found.  This return code only	*
 *		appears if --fail is used.													*
 * 23   Write  error.  Curl  couldn't  write  data  to  a local filesystem or		*
 *		similar.																	*
 * 24   Malformat user. User name badly specified.									*
 * 25   FTP couldn't STOR file.  The  server  denied  the  STOR operation.			*
 * 26   Read error. Various reading problems.										*
 * 27   Out of memory. A memory allocation request failed.							*
 * 28   Operation  timeout.  The  specified time-out period was reached according to*
 *		the conditions.																*
 * 29   FTP couldn't set ASCII. The server returned an  unknown reply.				*
 * 30   FTP PORT failed. The PORT command failed.									*
 * 31   FTP couldn't use REST. The REST command failed.								*
 * 32   FTP  couldn't  use  SIZE.  The SIZE command failed. The command is an		*
 *		extension to the original  FTP  spec  RFC 959.								*
 * 33   HTTP range error. The range \"command\" didn't work.						*
 * 34   HTTP   post  error.  Internal  post-request  generation error.				*
 * 35   SSL connect error. The SSL handshaking failed.								*
 * 36   FTP bad download resume. Couldn't continue  an  earlier aborted download.	*
 * 37   FILE  couldn't read file. Failed to open the file. Permissions?				*
 * 38   LDAP cannot bind. LDAP bind operation failed.								*
 * 39   LDAP search failed.															*
 * 40   Library not found. The LDAP library was not found.							*
 * 41   Function not found. A required LDAP  function  was  not found.				*
 * 42   Aborted  by callback. An application told curl to abort the operation.		*
 * 43   Internal error. A function was called with a bad parameter.					*
 * 44   Internal error. A function was called in a bad order.						*
 * 45   Interface  error.  A specified outgoing interface could not be used.		*
 * 46   Bad password entered. An error was  signaled  when  the password was		*
 *		entered.																	*
 * 47   Too  many redirects. When following redirects, curl hit the maximum amount.	*
 * 48   Unknown TELNET option specified.											*
 * 49   Malformed telnet option.													*
 * 51   The remote peer's SSL certificate wasn't ok									*
 * 52   The server didn't reply anything, which here is considered an error.		*
 ************************************************************************************/

#include "stdafx.h"

//#define		CURL_PATH			"curl.bat"
#define		CURL_PATH			"curl.exe"
#define		CURL_TMPFILE		".\\curl_tmp_file"
#define		CURL_TIMEOUT		120 * 1000
#define		CURL_FORMAT			"\"-w\" \"\\r\\n>>>Status Code: %{http_code} \\r\\n>>>Total Time: %{time_total} \\r\\n>>>DNS Time: %{time_namelookup} \\r\\n>>>Connect Time: %{time_connect} \\r\\n>>>Pretransfer Time: %{time_pretransfer} \\r\\n>>>Start Transfer Time: %{time_starttransfer} \\r\\n>>>Download Size: %{size_download} \\r\\n>>>Upload Size: %{size_upload} \\r\\n>>>Header Size: %{size_header} \\r\\n>>>Request Size: %{size_request} \\r\\n>>>Download Speed: %{speed_download} \\r\\n>>>Upload Speed: %{speed_upload}\""
#define		WM_USER_CURLTIMEOUT	((WM_USER) + 10)
#define		WM_USER_CURLOK		((WM_USER) + 9)

#define		STATUS_CODE_MATCH			"\r\n>>>Status Code: " 
#define		DOWNLOAD_SIZE_MATCH			"\r\n>>>Download Size: " 
#define		UPLOAD_SIZE_MATCH			"\r\n>>>Upload Size: " 
#define		HEADER_SIZE_MATCH			"\r\n>>>Header Size: " 
#define		REQUEST_SIZE_MATCH			"\r\n>>>Request Size: "
#define		TOTAL_TIME_MATCH			"\r\n>>>Total Time: " 
#define		DNS_TIME_MATCH				"\r\n>>>DNS Time: " 
#define		CONNECT_TIME_MATCH			"\r\n>>>Connect Time: " 
#define		PRETRANSFER_TIME_MATCH		"\r\n>>>Pretransfer Time: " 
#define		START_TRANSFER_TIME_MATCH	"\r\n>>>Start Transfer Time: " 
#define		DOWNLOAD_SPEED_MATCH		"\r\n>>>Download Speed: "
#define		UPLOAD_SPEED_MATCH			"\r\n>>>Upload Speed: "

struct tagCurlParam {
	DWORD	dwThreadID;
	CString strCmdLine;
};

BOOL URL_MONITOR(char *url, char *match, char *post, int timeout, char *uid, char *pwd, 
				 char *pserver, int pport, char *puid, char *ppwd,  
				 int ngid, int nmid, char *custpath, char *szReturn);
//void ExecuteFile(CString strCmdLine);
void ExecuteFile(CString strCmdLine, CString &strContent, int ngid, int nmid);
UINT CurlThreadProc( LPVOID pParam );
UINT CurlThreadProc2( LPVOID pParam );
BOOL CODE_VALUE(CString strResult, CString &strCode);
