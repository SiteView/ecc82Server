/******************************************************************************
**
** Source File Name: sqlqg_user.h
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
**              user objects
**
** Operating System: All
**
*******************************************************************************/
#ifndef SQL_H_SQLQG_USER
#define SQL_H_SQLQG_USER

#include "sqlqg_misc.h"

// Forward declarations.
class User_Info;
class Server;

//////////////////////////////////////////////////////////////////////////////
// Remote_user base class
//////////////////////////////////////////////////////////////////////////////

////////////////////////////////
// Base class.
////////////////////////////////

/* This user class is abstract class it is just used as 
an uniform interface for FencedRemote_User and UnfencedRemote_User */

class Remote_User : public Sqlqg_Cacheable_Object {
public:

  Remote_User (sqluint8* a_local_name,
               Server*   a_server,
               sqlint32* a_rc);
  virtual ~Remote_User (void);

  // Accessor methods.
  sqluint8* get_local_name (void);

  sqlint32 set_local_name (sqluint8* a_local_name);

  sqluint8* get_remote_name (void);

  User_Info* get_info (void);

  Server* get_server (void);

  // Initialization hook.  Invoked after creating user object to
  // initialize state from catalog information. 
  virtual sqlint32 initialize_user (User_Info* a_user_info);

  friend class Server;
  
protected:

  ////////////////
  // Methods.
  ////////////////

  // initialize_my_user() allows a user subclass instance to
  // initialize any extra state information from the catalog that
  // it needs.  This will probably not happen very often, so
  // the default implementation is to do nothing.
  virtual sqlint32 initialize_my_user (User_Info* a_user_info);

  ////////////////
  // Data.
  ////////////////

  // Local (DB2 II) authid.
  sqluint8*  local_name;
  Server*    server;  
  User_Info* info;
};

// Inline methods
inline sqluint8*
Remote_User::get_local_name (void)
{
  return local_name;
}

inline sqlint32
Remote_User::set_local_name (sqluint8* a_local_name)
{
  local_name = a_local_name;
  return(0);
}

inline User_Info*
Remote_User::get_info (void)
{
  return info;
}

inline Server*
Remote_User::get_server (void)
{
  return server;
}
#endif
