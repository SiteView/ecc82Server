/******************************************************************************
**
** Source File Name: sqlqg_unfenced_user.h
**
** (C) COPYRIGHT International Business Machines Corp. 1987, 2003
** All Rights Reserved
** Licensed Materials - Property of IBM
**
** US Government Users Restricted Rights - Use, duplication or
** disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
**
** Function = Include File defining:
**              Query Gateway classes and routines for
**              unfenced-side user objects
**
** Operating System: All
**
*******************************************************************************/
#ifndef SQL_H_SQLQG_UNFENCED_USER
#define SQL_H_SQLQG_UNFENCED_USER

#include "sqlqg_user.h"

// Forward declarations.
class User_Info;
class UnfencedServer;

////////////////////////////////////////////////////////////////////////////////
// class UnfencedRemote_User
////////////////////////////////////////////////////////////////////////////////
class UnfencedRemote_User : public Remote_User {
public:
  // Constructor.
  UnfencedRemote_User (sqluint8* a_local_name,
                       UnfencedServer* a_server,
                       sqlint32* a_rc);

  // Destructor. 
  virtual ~UnfencedRemote_User (void);

  // User registration hook.
  virtual sqlint32 register_user (User_Info*  a_user_info,
                                  User_Info** a_delta_info);

  // Verifies options specified on CREATE USER object.    
  virtual sqlint32 verify_register_user_info (User_Info*  a_user_info,
                                              User_Info** a_delta_info);

  // Verifies options specified on ALTER USER object.    
  virtual sqlint32 verify_alter_user_info (User_Info*  a_user_info,
                                           User_Info** a_delta_info);

  // Handy method to check if an option is recognized by DB2 and
  // should be ignored by the wrapper.
  virtual sqluint32 is_reserved_user_option (sqluint8* a_opt_name);
  
protected:  
  // verify_my_register_info() allows a user subclass instance to
  // verify any user info specific to that user.  It is called from
  // verify_register_user_info() method after verifying options applicable
  // to all users.
  virtual sqlint32 verify_my_register_user_info (User_Info*  a_user_info,
                                                 User_Info** a_delta_info);

  // verify_my_alter_info() allows a user subclass instance to
  // verify any user info specific to that user.  It is called from
  // verify_alter_user_info() method after verifying options applicable
  // to all users.
  virtual sqlint32 verify_my_alter_user_info (User_Info*  a_user_info,
                                              User_Info** a_delta_info);   
};

#endif
