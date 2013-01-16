
#include "stdafx.h"

BOOL MAIL_MONITOR(int nMailType, CString strSmtpServ, int nSmtpPort, CString strFrom, 
				  CString strRcptTo, int nProtoType, CString strRcptServ, int nRcptPort, 
				  CString strRcptUser, CString strRcptPass, CString strMatch, int nTimeout, 
				  int nCheckDelay, CString strAttach, int nNeedAuth, CString strVerifyUsername, 
				  CString strVerifyPassword, CString strCustomerPath, char *szReturn);
BOOL SendMail(CString SMTP_Server, int nPort, CString From, CString To, 
			  CString Attachment, int nNeedAuth, CString vUid, CString vPwd, char *szReturn);
BOOL RecvMail(CString POP3_Server, int nPort, CString strUid, CString strPwd, 
			  CString strMatch, char *szReturn);
BOOL RecvMail2(CString IMAP4_Server, int nPort, CString strUid, CString strPwd, 
			   CString strMatch, char *szReturn);
