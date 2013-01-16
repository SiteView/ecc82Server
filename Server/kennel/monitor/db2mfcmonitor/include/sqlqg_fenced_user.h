/******************************************************************************
**
** Source File Name: sqlqg_fenced_user.h
**
** (C) COPYRIGHT International Business Machines Corp. 1987, 2003
** All Rights Reserved
** Licensed Materials - Property of IBM
**
** US Government Users Restricted Rights - Use, duplication or
** disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
**
** Function = Include File defining:
**              Query Gateway fenced-side interfaces for
**              user objects.
**
** Operating System: All
**
*******************************************************************************/
#ifndef SQL_H_SQLQG_FENCED_USER
#define SQL_H_SQLQG_FENCED_USER

#include "sqlqg_user.h"

// Forward declarations.
class FencedServer;

////////////////////////////////////////////////////////////////////////////////
// FencedRemote_User class
////////////////////////////////////////////////////////////////////////////////
class FencedRemote_User : public Remote_User{

public:

  // Constructor.
  FencedRemote_User (sqluint8*     a_user_name,
                     FencedServer* a_server,
                     sqlint32*     a_rc);

  // Destructor. 
  virtual ~FencedRemote_User (void);

  virtual sqlint32 initialize_user (User_Info* a_user_info);
};

#endif
