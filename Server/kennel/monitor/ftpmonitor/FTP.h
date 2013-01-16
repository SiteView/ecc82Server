#include "stdafx.h"

class CFtpParam : public CObject
{
public:
	CFtpParam();
	virtual ~CFtpParam();

public:
	SOCKET		ctrlSocket;
	SOCKET		dataSocket;

	DWORD		beginTickCount;
	DWORD		endTickCount;
	DWORD		recvTickCount;
	DWORD		recvBytes;

	char		remote_host[64];
	int			remote_port;

	char		local_host[64];
	int			local_port;

	int			passive_mode;
};

BOOL FTP_MONITOR(char *server, int port, char *file, char *uid, char *pwd, 
				 int timeout, int ptype, char *pserver, int pport, 
				 char *puid, char *ppwd, char *match, char *custpath, int passive_mode, char *szReturn);
BOOL TFTP_MONITOR(char *server, int port, char *file, int timeout, char *custpath, 
				  char *szReturn);
int OpenControlChannel(char *hostname, int port, char *username, char *password, 
					   int timeout, char *retrfile, int ptype, char *pserver, int pport, 
					   char *puid, char *ppwd, char *szReturn, CFtpParam& ftpParam);
int OpenDataChannel(char *retrfile, int timeout, int ptype, 
					char *pserver, int pport, char *puid, char *ppwd, char *szReturn, CFtpParam& ftpParam);
void QuitFromServer(CFtpParam& ftpParam);
int WaitVersionInfo(SOCKET s, int nSec);
int FTP_COMMAND(SOCKET s, CString strCmd, CString &strMessage);
int EnterPassiveMode(CFtpParam& ftpParam);
int WriteString(SOCKET s, CString sendStr);
int ReadString2(SOCKET s, char *buffer);