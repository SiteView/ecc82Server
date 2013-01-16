#include "NoteMail.h"

/****************************************************************************

    FUNCTION: SendMailMainProc()

    COMMENTS:
        Initialize "From" field with the user's name, and the "Date"
        field with the current date and time.  Call Sendmail routine to
        process and send mail message.

****************************************************************************/

int SendMailMainProc(char *szSendTo,char *szCopyTo,char *szFrom,char *szSubject,char *szBody)
{
	STATUS        status;
	TIMEDATE      tdDate;
	WORD          wDateLen;

	char          szDate[MAXALPHATIMEDATE+1];

//	/* Get user's name and initialize "From" field. */
//	if (status = SECKFMGetUserName(szFrom))
//	{  /* Unable to get user name. */
//	  printf ("\nError: unable to get user name.\n");
//	  return(status);
//	}

	/* Get current time and date. Initialize "Date" field */
	OSCurrentTIMEDATE(&tdDate);
	if (status = ConvertTIMEDATEToText (NULL, NULL,
				  &tdDate, szDate, MAXALPHATIMEDATE,
				  (WORD FAR*)&wDateLen))
	{   /* Unable to initialize date field */
	  printf ("\nError: unable to initialize Date field.\n");
	  return(status);
	}

	szDate[wDateLen] = '\0';

	status = SendMail(szSendTo,szCopyTo,szFrom,szSubject,szBody,tdDate);

	return(status);

}


/************************************************************************

    FUNCTION:   ConvertNewlinesToNulls(char* szBody, int BodyLen)

    PURPOSE:    Filter CR from body text. Replace with NUL char.

*************************************************************************/
 
void  LNPUBLIC  ConvertNewlinesToNulls(char* szBody, int BodyLen)
{
  int    wByte;

  for (wByte=0; wByte<BodyLen; wByte++)
    {
      if (szBody[wByte] == '\n' ) 
   szBody[wByte] = '\0'; 
    }
  
  return;
}


int SendMail(char *szSendTo,char *szCopyTo,char *szFrom,char *szSubject,char *szBody,TIMEDATE &tdDate)
{
	STATUS        status;

	int           BodyLen;
	char        szMailServerName[MAXUSERNAME+1];
	char        szMailFileName[MAXUSERNAME+1];
	char        szMailFilePath[MAXPATH+1];
	char        szMailBoxPath[MAXPATH+1];
	char        szLocalMailBox[10];
	STATUS      error = NOERROR;    
	DBHANDLE    hMailFile = NULLHANDLE;
	DBHANDLE    hMailBox = NULLHANDLE;
	BOOL        fLocal = FALSE;
	NOTEHANDLE  hMsg;
	HANDLE      hRecipientsList = NULLHANDLE;
	HANDLE      hSendToList = NULLHANDLE;
	HANDLE      hCopyToList = NULLHANDLE;
	WORD        wRecipientsSize = 0, wSendToSize = 0, wCopyToSize = 0;
	WORD        wRecipientsCount = 0, wSendToCount = 0, wCopyToCount = 0;
	LIST        *plistRecipients, *plistSendTo, *plistCopyTo;
	char        *szNextName;        /* used when parsing szSendTo, etc. */
	char        szPriority[] = "N"; /* "Normal" */
	char        szReport[] = "B";   /* "Basic" */
	HANDLE      hBodyItem = NULLHANDLE;
	DWORD       dwBodyItemLen;
	DBHANDLE    hOrigDB;
	NOTEID      OrigNoteID;
	OID         OrigNoteOID, NewNoteOID;
    
	BodyLen = strlen(szBody);

	/* prepare the message strings */
	ConvertNewlinesToNulls(szBody, BodyLen);

	if (!OSGetEnvironmentString(MAIL_MAILSERVER_ITEM, /*"MailServer"*/
			   szMailServerName, MAXUSERNAME))
	{   /* Unable to get mail server name */
	  printf ("\nError: unable to get mail server name.\n");
	  return(status);
	}
  
	if (!OSGetEnvironmentString(MAIL_MAILFILE_ITEM, /*"MailFile"*/
			   szMailFileName, MAXUSERNAME))
	{   /* Unable to get mail file name */
	  printf ("\nError: unable to get mail file name.\n");
	  return(status);
	}

  /*  First try to open the user's mail file on the mail server.
      If unable to open because LAN not available, then ask user if
      they would like to save locally.
  */
  OSPathNetConstruct( NULL, szMailServerName, szMailFileName,
           szMailFilePath);
  
  status = MailOpenMessageFile(szMailFilePath, &hMailFile);
  
#if 0
  if ( (ERR(status) == ERR_NO_NETBIOS) ||
       (ERR(status) == ERR_SERVER_NOT_RESPONDING) )
#endif
  if ( (ERR(status) == ERR_SERVER_NOT_RESPONDING) )
    /* laptop not connected to server */
    {
      status = NOERROR;

      /* Ask user if they would like to deliver msg to local MAIL.BOX */
      printf("Would you like to deliver the mail message to your local Mail Box (YES/NO)?: ");      
      fflush (stdout);
      gets(szLocalMailBox);
       
      /* Unable to reach Mail Server. Save in local MAIL.BOX? */
      if (stricmp(szLocalMailBox, "YES") == 0)
        {
          /* yes - create and save message in local mail file */
          fLocal = TRUE;
          status = MailOpenMessageFile(szMailFileName, &hMailFile);
        }
      else
        { /* no - user wants to connect to server. Error condition.*/
          status = ERR_SERVER_NOT_RESPONDING;
        }
    }
  
  if (status)
    { /* Unable to open user's Domino and Notes mail file */
      printf ("\nError: unable to open user's mail file.\n");
      return(status);
    }

  if (status = MailCreateMessage(hMailFile, &hMsg))
    { /* Unable to create memo in mail file */
      printf ("\nError: unable to create memo in mail file.\n");
      error=SENDMAILERROR;
      goto CloseFile;
    }

  if (status = ListAllocate(0, 0, TRUE, &hRecipientsList,
             &plistRecipients, &wRecipientsSize))
    {  /* Unable to allocate list */
      printf ("\nError: unable to allocate list.\n");
      error=SENDMAILERROR;
      goto CloseMsg;
    }
  OSUnlockObject(hRecipientsList);
  
  if (status = ListAllocate(0, 0, TRUE, &hSendToList,
             &plistSendTo, &wSendToSize))
    {
      printf ("\nError: unable to allocate list.\n");
      error=SENDMAILERROR;
      goto CloseMsg;
    }
  OSUnlockObject(hSendToList);
  
  if (status = ListAllocate(0, 0, TRUE, &hCopyToList,
             &plistCopyTo, &wCopyToSize))
    {
      printf ("\nError: unable to allocate list.\n");
      error=SENDMAILERROR;
      goto CloseMsg;
    }
  OSUnlockObject(hCopyToList);


  /* Parse SendTo string. Add names to SendTo and Recipients lists. */
  for (szNextName = strtok(szSendTo, ",");
       szNextName != (char*)NULL;
       szNextName = strtok(NULL, ","))
    {
      while (isspace(*szNextName))    /* Skip white space before name */
      szNextName++;
      
      if (status = ListAddEntry(hSendToList, TRUE, &wSendToSize,
                                wSendToCount++, szNextName,
                                (WORD)strlen(szNextName)))
        {   /* Unable to add name to SendTo list */
           printf ("\nError: unable to add name to SendTo list.\n");
           error=SENDMAILERROR;
           goto CloseMsg;
        }

      if (status = ListAddEntry(hRecipientsList, TRUE, &wRecipientsSize,
                                wRecipientsCount++, szNextName,
                                (WORD)strlen(szNextName)))
        {   /* Unable to add name to Recipients list */
           printf ("\nError: unable to add name to Recipients list.\n");
           error=SENDMAILERROR;
           goto CloseMsg;
        }
    }
  
  /* Parse CopyTo string. Add names to CopyTo and Recipients list. */
  
  for (szNextName = strtok(szCopyTo, ",");
       szNextName != (char*)NULL;
       szNextName = strtok(NULL, ","))
    {
      while (isspace(*szNextName))
      szNextName++;
      
      if (status = ListAddEntry(hCopyToList, TRUE, &wCopyToSize,
                                wCopyToCount++, szNextName,
                                (WORD)strlen(szNextName)))
        { /* Unable to add name to CopyTo list */
          printf ("\nError: unable to add name to CopyTo list.\n");
          error=SENDMAILERROR;
          goto CloseMsg;
        }

      if (status = ListAddEntry(hRecipientsList, TRUE, &wRecipientsSize,
                                wRecipientsCount++, szNextName,
                                (WORD)strlen(szNextName)))
        
        {
          printf ("\nError: unable to add name to Recipients list.\n");
          error=SENDMAILERROR;
          goto CloseMsg;
        }
    }
  
  /*  Suggested enhancements: You might want to add code here to verify
      that the name & address book on the mail server contains person or
      group documents for each of the named recipients. See Lotus C API 
	  for Domino and Notes function NAMELookup(). Possibly query user to 
	  resolve unknown recipient names. You might also want to check the 
	  recipients list to ensure it contains no duplicate names.
  */

  /* Add the Recipients item to the message. */
  if (wRecipientsCount == 0)  /* Mail memo has no recipients. */
    {
      printf ("\nError: mail message has no recipients.\n");
      error=SENDMAILERROR;
      goto CloseMsg;
    }
  
  if (status = MailAddRecipientsItem( hMsg, hRecipientsList,
                 wRecipientsSize))
    {
      /* Unable to set Recipient item in memo */
      printf ("\nError: unable to set Recipient item in memo.\n");
      error=SENDMAILERROR;
      goto CloseMsg;
    }
  /*  MailAddRecipientsItem and MailAddHeaderItemByHandle both attach
      the memory used by the list to the message. Set handle to NULL
      after these functions succeed so the code at CloseMsg: below does
      not attempt to free it.
  */
  hRecipientsList = NULLHANDLE;
  
  /* Add the SendTo and CopyTo items to the message. */
  
  if (status = MailAddHeaderItemByHandle( hMsg, MAIL_SENDTO_ITEM_NUM,
                hSendToList, wSendToSize, 0))
    {  /* Unable to set SendTo item in memo */
      printf ("\nError: unable to set SendTo item in memo.\n");
      error=SENDMAILERROR;
      goto CloseMsg;
    }
  hSendToList = NULLHANDLE;
  
  if (status = MailAddHeaderItemByHandle( hMsg, MAIL_COPYTO_ITEM_NUM,
                hCopyToList, wCopyToSize, 0))
    { /* Unable to set CopyTo item in memo */
      printf ("\nError: unable to set CopyTo item in memo.\n");
      goto CloseMsg;
    }
  hCopyToList = NULLHANDLE;
  
  /* Add the Form item to the message */
  if (status = MailAddHeaderItem( hMsg, MAIL_FORM_ITEM_NUM,
             MAIL_MEMO_FORM,
             (WORD)strlen(MAIL_MEMO_FORM)))
    { /* Unable to set Form item in memo  */
      printf ("\nError: unable to set Form item in memo.\n");
      error=SENDMAILERROR;
      goto CloseMsg;
    }

  /* Add From, Subject, Delivery Priority, & etc. items to the message */
  if (status = MailAddHeaderItem( hMsg, MAIL_FROM_ITEM_NUM,
             szFrom, (WORD)strlen(szFrom)))
    { /* Unable to set From item in memo */
      printf ("\nError: unable to set From item in memo.\n");
      error=SENDMAILERROR;
      goto CloseMsg;
    }

  if (status = MailAddHeaderItem( hMsg, MAIL_SUBJECT_ITEM_NUM,
             szSubject, (WORD)strlen(szSubject)))
    { /* Unable to set Subject item in memo */
      printf ("\nError: unable to set Subject item in memo.\n");
      error=SENDMAILERROR;
      goto CloseMsg;
    }
  
  if (status = MailAddHeaderItem( hMsg, MAIL_DELIVERYPRIORITY_ITEM_NUM,
            szPriority, (WORD)strlen(szPriority)))
    { /* Unable to set Delivery Priority item in memo */
      printf ("\nError: unable to set Delivery Priority item in memo.\n");
      error=SENDMAILERROR;
      goto CloseMsg;
    }

  if (status = MailAddHeaderItem( hMsg, MAIL_DELIVERYREPORT_ITEM_NUM,
            szReport, (WORD)strlen(szReport)))
    { /* Unable to set Delivery Report into memo */
      printf ("\nError: unable to set Delivery Report item in memo.\n");
      error=SENDMAILERROR;
      goto CloseMsg;
    }

  /*  set "ComposedDate" to tdDate = when dialog box was initialized */
  if (status = MailAddHeaderItem( hMsg, MAIL_COMPOSEDDATE_ITEM_NUM,
             (char*)(&tdDate),
             (WORD)sizeof(TIMEDATE)))
    { /* Unable to set Composed Date in memo */
      printf ("\nError: unable to set Composed Date item in memo.\n");
      error=SENDMAILERROR;
      goto CloseMsg;
    }
  
  if (status = MailCreateBodyItem (&hBodyItem, &dwBodyItemLen))
    { /* Unable to create body item in message */
      printf ("\nError: unable to create body item in memo.\n");
      hBodyItem = NULLHANDLE;
      error=SENDMAILERROR;
      goto CloseMsg;
    }
  if (status = MailAppendBodyItemLine (hBodyItem, &dwBodyItemLen,
                   szBody, (WORD)BodyLen))
    { /* Unable to append text to body */
      printf ("\nError: unable to append text to body.\n");
      error=SENDMAILERROR;
      goto CloseMsg;
    }

  if (status = MailAddBodyItemExt(hMsg, hBodyItem, dwBodyItemLen, NULL))
    { /* Unable to add Body item to memo */
      printf ("\nError: unable to add body item to memo.\n");
      error=SENDMAILERROR;
      goto CloseMsg;
    }

  /* Set "PostedDate" to the current time/date right now */
  OSCurrentTIMEDATE(&tdDate);
  if (status = MailAddHeaderItem( hMsg, MAIL_POSTEDDATE_ITEM_NUM,
             (char*)(&tdDate),
             (WORD)sizeof(TIMEDATE)))
    {
      printf ("\nError: unable to set date.\n");
      error=SENDMAILERROR;
      goto CloseMsg;
    }
 
  /*  Deliver the message. */
  /*  If local, transfer message to the local mail.box */
  if (fLocal)
    {
      if (status = MailTransferMessageLocal(hMsg))
        {
     /* Unable to transfer message to local mail box */
     printf ("\nError: unable to transfer message to local mail box.\n");
     error=SENDMAILERROR;
     goto CloseMsg;
        }
      /* else we successfully transferred msg to local mail box */
      /* Save msg to user's mail file and clean up.*/
      if (status = NSFNoteUpdate(hMsg, UPDATE_NOCOMMIT))
        {   /* Unable to update message in local mail file */
          printf ("\nError: unable to update message in local mail box.\n");
          error=SENDMAILERROR;
        }
      goto CloseMsg;
    }
  
  /*  System is connected to the LAN (fLocal == FALSE). Open the
      router's MAIL.BOX on the mail server, then copy msg there.
   */
  OSPathNetConstruct(NULL, szMailServerName, MAILBOX_NAME, szMailBoxPath);
  
  if (status = NSFDbOpen(szMailBoxPath, &hMailBox))
    {
      printf ("\nError: unable to open mail box.\n");
      error=SENDMAILERROR;
      goto CloseMsg;
    }
  /*  Copy the message, which is a note in the user's mail file,
      to the mail box. Perform this copy by changing the origin to
      the mail box then updating. Save the message's DBID, NOTEID,
      and OID. Set the DBID to the MAIL.BOX handle, the NOTEID to zero,
      and the OID to a newly generated OID associated with MAIL.BOX.
      Then update the message. This stores it in the MAIL.BOX file.
      Finally, restore the DBID, NOTEID, and OID.
   */
  NSFNoteGetInfo(hMsg, _NOTE_ID,  &OrigNoteID);
  NSFNoteGetInfo(hMsg, _NOTE_DB,  &hOrigDB);
  NSFNoteGetInfo(hMsg, _NOTE_OID, &OrigNoteOID);
  
  /* Set the message's OID database ID to match the mail box */
  if (status = NSFDbGenerateOID (hMailBox, &NewNoteOID))
    {
      /* Unable to generate originator ID for mail box */
      printf ("\nError: unable to generate id for mail box.\n");
      error=SENDMAILERROR;
      goto CloseMsg;
    }
  NSFNoteSetInfo(hMsg, _NOTE_DB,  &hMailBox);
  NSFNoteSetInfo(hMsg, _NOTE_ID,  0);
  NSFNoteSetInfo(hMsg, _NOTE_OID, &NewNoteOID);
  
  /* Update message into MAIL.BOX on mail server. */
  if (status = NSFNoteUpdate(hMsg, UPDATE_NOCOMMIT))
    {
      /* Unable to update message to router mail box */
      printf ("\nError: unable to update message to router mail box.\n");
      error=SENDMAILERROR;
    }
  else/* message successfully copied into router's mail box */
    {   /* restore msg to user's mail file and Update to save it there.*/
      NSFNoteSetInfo(hMsg, _NOTE_DB,  &hOrigDB);
      NSFNoteSetInfo(hMsg, _NOTE_ID,  &OrigNoteID);
      NSFNoteSetInfo(hMsg, _NOTE_OID, &OrigNoteOID);
      
      status = NSFNoteUpdate(hMsg, UPDATE_NOCOMMIT);
    }
  

CloseMsg:
  if (hRecipientsList != NULLHANDLE)  OSMemFree(hRecipientsList);
  if (hSendToList != NULLHANDLE)      OSMemFree(hSendToList);
  if (hCopyToList != NULLHANDLE)      OSMemFree(hCopyToList);
  

  if (error == NOERROR)
    {
      if (status = MailCloseMessage(hMsg))
      { /* Unable to close memo */
          printf ("\nError: unable to close memo.\n");
          error=SENDMAILERROR;
      }
      else
        if (hBodyItem != NULLHANDLE)  OSMemFree (hBodyItem);
    }
    else
      {
        status = MailCloseMessage(hMsg);
        if ((status == NOERROR) && (hBodyItem != NULLHANDLE))
          OSMemFree (hBodyItem);
      }
    if (hMailBox != NULLHANDLE)
        NSFDbClose (hMailBox);

CloseFile:
  if (error == NOERROR)
    {
      if (status = MailCloseMessageFile(hMailFile))
        { /* Unable to close mail file. */
          printf ("\nError: unable to close mail file.\n");
          error=SENDMAILERROR;
        }
    }
  else /* have already encountered an error condition */
    {    /* so don't check for further errors closing */
      MailCloseMessageFile(hMailFile);
    }

  if (error == NOERROR)
    /* SendMail Success */
    printf ("SendMail successful.\n");
  

  return(status);
}
/*----------------------------------------------------------------------------------------------------------------*/

STATUS near pascal GetUniqueFileName(char *Drive, char *Ext,
                                        char *FileName)

{
    int     file;
    WORD    Num;
    char    Name[17];
    char    cwd[MAXPATH];
    char   *Dir;

    /* Increment through numbered file names until a non-existent one found. */
   getcwd(cwd, MAXPATH);
   Dir = (char *)&cwd;

    for (Num = 0; Num <= 32767; Num++)
    {
        _itoa(Num, Name, 10);
        _makepath(FileName, Drive, Dir, Name, Ext);
        if ((file = open(FileName, O_BINARY | O_RDONLY)) == -1)
            return(NOERROR);
        close(file);
    }
    FileName[0] = '\0';
    return(ERR_EXISTS);
}

int readmail(char *szReadServerName,char *szReadMailFile)
{
	char szDriveLetter_blank[] = "";
	char szExtensionName_TMP[] = "TMP";
	char szNewline_Terminate[] = "\r\n";

    STATUS      error = NOERROR;
/*
    char       *szServerName = "star/admin";
    char       *szMailFile = "mail\\dragonfl";
*/

	char       *szServerName = szReadServerName;
    char       *szMailFile = szReadMailFile;

    char        szMailFilePath[MAXPATH+1];
    HANDLE      hMessageFile;
    HANDLE      hMessageList = NULLHANDLE, hMessage;
    DARRAY     *MessageList;
    WORD        MessageCount, Msg, RecipientCount, Rec;
    char        Originator[MAXRECIPIENTNAME+1];
    WORD        OriginatorLength;
    char        RecipientName[MAXRECIPIENTNAME+1];
    WORD        RecipientNameLength;
    char        UserName[MAXUSERNAME + 1];
    WORD        UserNameLength;
    char        DomainName[MAXDOMAINNAME+1];
    WORD        DomainNameLength;
    char        String[MAXSPRINTF+1];
    WORD        StringLength;
    TIMEDATE    Time;
    BOOL        NonDeliveryReport;
    DWORD       BodyFileSize;
    char        BodyFileName[MAXPATH_OLE];
    FILE       *BodyFile;
    char        OriginalFileName[MAXPATH+1];
    WORD        Att;
    BLOCKID     bhAttachment;
    char        szServer[80];           /* server names are < 80 chars */
    char        szDBName[MAXPATH];


    error = NotesInitExtended (0, NULL);

    if (error){
      printf("\nUnable to initialize Notes\n");
      return(1);
    }

    //szServerName = szServer;
    //szMailFile = szDBName;

    OSPathNetConstruct( NULL,               /* port name  */
                        szServerName,
                        szMailFile,
                        szMailFilePath);

    /* Open the message file. */

    if (error = MailOpenMessageFile(szMailFilePath, &hMessageFile))
    {
        printf ("Error: unable to open '%s'.\n", szMailFilePath);
        goto Exit0;
    }

    /* Create message list of messages in the file - just 64K */

    if (error = MailCreateMessageList(hMessageFile,
                        &hMessageList, &MessageList, &MessageCount))
    {
        printf ("Error: unable to create message list.\n");
        goto Exit1;
    }

    printf ("Mail file contains %d message(s).\n", MessageCount);

    /* Print out each of the outbound messages. */

    for (Msg = 0; Msg < MessageCount; Msg++)
    {
        printf ("\nMessage #%d: \n", Msg+1);

        if (error = MailOpenMessage (MessageList, Msg, &hMessage))
        {
            printf ("Error: unable to open message number %d.\n", Msg+1);
            goto Exit2;
        }

        /* Get the originator's name/address. */

        if (error = MailGetMessageOriginator(MessageList, Msg,
                Originator, sizeof(Originator), &OriginatorLength))
        {
            printf ("Error: unable to get message originator.\n");
            goto Exit2;
        }

        Originator[OriginatorLength] = '\0';

        printf ("\tOriginator = '%s'\n", Originator);

        if (error = MailGetMessageInfo(MessageList, Msg,
                            &RecipientCount, NULL, NULL))
        {
            printf ("Error: unable to get number of recipients in message.\n");
            MailCloseMessage (hMessage);
            goto Exit2;
        }

		//É¾³ýÓÊ¼þ
		MailPurgeMessage(MessageList, Msg);

        printf ("\tNumber of Recipients = %d.\n", RecipientCount);

        for (Rec = 0; Rec < RecipientCount; Rec++)
        {
            MailGetMessageRecipient(MessageList, Msg, Rec, RecipientName,
                    sizeof(RecipientName), &RecipientNameLength);

            MailParseMailAddress(RecipientName, RecipientNameLength,
                    UserName, sizeof(UserName), &UserNameLength,
                    DomainName, sizeof(DomainName), &DomainNameLength);

			//MailDeleteMessageRecipient(MessageList, Msg, Rec);

            UserName[UserNameLength] = '\0';
            DomainName[DomainNameLength] = '\0';

            printf ("\t\tRecipient %d = '%s'\t Domain = '%s'\n", Rec+1,
                                UserName, DomainName);
        }   /* end of loop over recipients */

        /* SendTo */
        MailGetMessageItem (hMessage, MAIL_SENDTO_ITEM_NUM, String,
                                        MAXSPRINTF, &StringLength);

        String[StringLength] = '\0';
        printf ("\tTo: %s\n", String);

        /* CopyTo */
        MailGetMessageItem (hMessage, MAIL_COPYTO_ITEM_NUM, String,
                                        MAXSPRINTF, &StringLength);
        String[StringLength] = '\0';
        printf ("\tCc: %s\n", String);

        /* From */
        MailGetMessageItem (hMessage, MAIL_FROM_ITEM_NUM, String,
                                        MAXSPRINTF, &StringLength);
        String[StringLength] = '\0';
        printf ("\tFrom: %s\n", String);

        /* PostedDate */
        MailGetMessageItemTimeDate(hMessage, MAIL_POSTEDDATE_ITEM_NUM, &Time);
        ConvertTIMEDATEToText(NULL, NULL, &Time, String,
                                    sizeof(String) - 1, &StringLength);
        String[StringLength] = '\0';
        printf("\tDate: %s\n", String);

        /* Subject. If non-delivery report, prefix with "NonDelivery of:" */

        MailGetMessageItem (hMessage, MAIL_SUBJECT_ITEM_NUM, String,
                                        MAXSPRINTF, &StringLength);
        String[StringLength] = '\0';
        if (NonDeliveryReport = MailIsNonDeliveryReport(hMessage))
        {
            printf ("\tNonDelivery of: %s\n", String);
        }
        else
        {
            printf ("\tSubject: %s\n", String);
        }

        if (NonDeliveryReport)
        {
            MailGetMessageItem(hMessage, MAIL_INTENDEDRECIPIENT_ITEM_NUM,
                            String, sizeof(String), &StringLength);
            String[StringLength] = '\0';
            printf("\tIntended Recipients: %s\n", String);

            MailGetMessageItem(hMessage, MAIL_FAILUREREASON_ITEM_NUM,
                            String, sizeof(String), &StringLength);
            String[StringLength] = '\0';
            printf("\tFailure Reason: %s\n", String);
        }

        /* Body */


        if (error = GetUniqueFileName(szDriveLetter_blank, szExtensionName_TMP, BodyFileName))
        {
            printf ("Error: unable to create temporary file name.\n");
            MailCloseMessage (hMessage);
            goto Exit2;
        }
        if (error = MailGetMessageBodyText(hMessage,
                                    NULL,     /* Use standard Body item */
                                    szNewline_Terminate,
                                    80,        /* 80 chars per line */
                                    TRUE,     /* Convert TABs */
                                    BodyFileName,
                                    &BodyFileSize))
        {
            printf ("Error: unable to get Message body into temporary file.\n");
            unlink(BodyFileName);
            MailCloseMessage (hMessage);
            goto Exit2;
        }

        /* Print each line of body text to the screen. */

        if (!(BodyFile = fopen(BodyFileName, "r")))
        {
            printf ("Error: unable to open temporary file.\n");
            unlink(BodyFileName);
            MailCloseMessage (hMessage);
            goto Exit2;
        }

        printf ("\tBody:\n");
        while (fgets(String, READMAIL_BODY_LINELEN, BodyFile))
        {
            printf("\t\t%s\n", String);
        }
        fclose(BodyFile);
        unlink(BodyFileName);

        /* Attachments */

        for (Att = 0; MailGetMessageAttachmentInfo(hMessage, Att,
                                &bhAttachment, OriginalFileName,
                                NULL, NULL, NULL, NULL, NULL); Att++)
        {
            printf("\tAttachment %d = '%s'\n", Att+1, OriginalFileName);
        }

        
		MailCloseMessage (hMessage);
    }   /* end of loop over messages */

    /* Free the message list and close the message file */

	MailPurgeMessageFile(hMessageFile);

Exit2:
    if (hMessageList != NULLHANDLE)
    {
        OSUnlockObject(hMessageList);
        OSMemFree(hMessageList);
    }

Exit1:
    if (hMessageFile != NULLHANDLE)
	{
		//MailPurgeMessageFile(hMessageFile);
        MailCloseMessageFile(hMessageFile);
	}
Exit0:
    if (ERR(error))
       printf("\nProgram completed fail.\n");
    else
       printf("\nProgram completed successfully.\n");

    
    return(ERR(error));
}
