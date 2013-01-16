// NoteMail.h: interface for the LogFile class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __NoteMail_H__
#define __NoteMail_H__

#include "StdAfx.h"

#define NT 1;
#include <lapicinc.h>

#include <global.h>
#include <nsfdb.h>
#include <nsfsearc.h>
#include <names.h>
#include <nsferr.h>
#include <textlist.h>
#include <ods.h>
#include <odstypes.h>

#include <nsfdata.h>
#include <nsfnote.h>
#include <editods.h>
#include <stdnames.h>
#include <osmem.h>
#include <osmisc.h>
#include <ostime.h>
#include <oserr.h>
#include <nsfobjec.h>
#include <misc.h>
#include <osfile.h>
#include <kfm.h>                        /* SECKFMGetUserName */
     

#include <nsf.h>                        /* MAXUSERNAME, etc. */
#include <osenv.h>                      /* OSGetEnvironmentString */
#include <mail.h>                       /* MAIL_SENDTO_ITEM_NUM, etc. */
#include <mailserv.h>                   /* MailOpenMessageFile */
#include <neterr.h>                     /* ERR_NO_NETBIOS */
#include <clerr.h>                      /* ERR_SERVER_NOT_RESPONDING */

#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <io.h>
#include <direct.h>
#include <fcntl.h>
#include <log.h>
#include <lapiplat.h>
#include <addin.h>
#define SENDMAILERROR 110

#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#include <globerr.h>

int SendMail(char *szSendTo,char *szCopyTo,char *szFrom,char *szSubject,char *szBody,TIMEDATE &tdDate);
int SendMailMainProc(char *szSendTo,char *szCopyTo,char *szFrom,char *szSubject,char *szBody);
void  LNPUBLIC  ConvertNewlinesToNulls(char* szBody, int BodyLen);

STATUS near pascal GetUniqueFileName(char *Drive, char *Ext,char *FileName);
int readmail(char *szReadServerName,char *szReadMailFile);

/* constants */
#define     READMAIL_BODY_LINELEN   40



#endif // __NoteMail_H__
