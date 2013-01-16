/******************************************************************************
**
** Source File Name: sqlqg_server.h
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
**              server objects
**
** Operating System: All
**
*******************************************************************************/
#ifndef SQL_H_SQLQG_SERVER
#define SQL_H_SQLQG_SERVER

#include "sqlqg_misc.h"

// Forward declarations.
class Wrapper;
class Nickname;
class Remote_User;                         
class Server_Info;
class User_Info;
class Nickname_Info;

//////////////////////////////////////////////////////////////////////////////
// Server base class
//////////////////////////////////////////////////////////////////////////////
class Server : public Sqlqg_Cacheable_Object {
public:

  // This enum helps to distinguish between relational and
  // generic remote servers.
  enum server_kind { relational_kind, generic_kind };
  
  // Constructor
  Server (sqluint8*    a_server_name,
          server_kind  a_server_kind,
          Wrapper*     a_server_wrapper,
          sqlint32*    a_rc);
  
  // Accessor methods. Note: these methods return pointers to the
  // actual data, not copies of the information.
  sqluint8* get_name (void);

  server_kind get_kind (void);

  virtual sqluint8* get_type (void);
  virtual sqluint8* get_version (void);
  Server_Info* get_info (void);

  // Initialization hook.  Invoked after creating server object to
  // initialize state from catalog information. 
  virtual sqlint32 initialize_server (Server_Info*  a_server_info,
                                      sqlint32      a_init_subclass=TRUE);

  sqlint32 find_remote_user (sqluint8*     a_user_name,
                             Remote_User** a_user);
  sqlint32 add_remote_user (Remote_User* a_user);
  virtual sqlint32 drop_remote_user (Remote_User* a_user, 
                                     Sqlqg_DeleteType a_flag = SQLQG_DELETE_ALL);
  sqlint32 drop_all_remote_users (Sqlqg_DeleteType a_flag = SQLQG_DELETE_ALL);

  sqlint32 drop_nickname (Nickname* a_nickname);
  sqlint32 drop_all_nicknames (void);

  sqlint32 drop_marked_objects (void);

  Wrapper* get_wrapper (void);

  // Remote Nickname initialization.
  Nickname* get_nickname (sqluint8*       a_schema_name,
                          sqluint8*       a_nickname_name,
			  Nickname_Info*  a_nick_info,
			  sqlint32*       a_rc);

  Remote_User* get_remote_user (sqluint8*  a_user_name,
			        User_Info* a_user_info,
			        sqlint32*  a_rc);
  
  char* get_f2pc_rm (void);

  void set_f2pc_rm (char* a_rm);

  friend class Wrapper;

  void mark_connected (void);

  void mark_disconnected (void);

  sqluint8 is_connected (void);

  // Remove the user from hash table users and free the user.
  void delete_remote_user (Remote_User* a_user);
  
protected:
  
  ////////////////
  // Methods.
  ////////////////

  // initialize_my_server(void) allows a server subclass instance to
  // initialize any extra state information from the catalog that
  // it needs.  For example, DBNAME would be extracted from the
  // server_info object for those servers that support the DBNAME.
  virtual sqlint32 initialize_my_server (Server_Info* a_server_info);

  ////////////////
  // Data.
  ////////////////

  Server_Info* info;                    // Catalog information for server.
  sqluint8*    name;                    // Server name.
  server_kind  kind;                    // Server kind
  Wrapper*     wrapper;                 // Wrapper associated with server.
  char*        f2pc_rm;

  sqluint8     connected;               // indicates whether server is
                                        // currently connected

  virtual ~Server (void);

 private:

  // create_remote_user(void) allows a server subclass instance to create an
  // instance of its own remote user subclass.  It is invoked by
  // get_remote_user(void).  The default implementation assumes the wrapper
  // does not support users.
  virtual Remote_User* create_remote_user (sqluint8*  a_user_name,
                                           sqlint32*  a_rc);
  
  // create_nickname(void) allows a server subclass instance to
  // create an instance of its own remote nickname subclass.
  // The default implementation assumes the wrapper does not support
  // nicknames.
  virtual Nickname* create_nickname (sqluint8*  a_schema_name,
				     sqluint8*  a_nickname_name,
				     sqlint32*  a_rc);

  
  // These hash tables are private so wrappers cannot access them
  // directly.  It is unsafe to use the objects in the tables without
  // validating them first.  To look up a nickname, the wrapper should
  // use the method Unfenced_Server::find_nickname instead.

  // Make the immediate subclass UnfencedServer a friend so it can use
  // the hash tables for cache management.
  friend class UnfencedServer;
  Hash_Table users;                     // Users associated with server.
  Hash_Table nicknames;                 // Nicknames associated with server

};

// Inline methods
inline sqluint8*
Server::get_name (void)
{
  return name;
}

inline Server::server_kind
Server::get_kind (void)
{
  return kind;
}

inline Server_Info*
Server::get_info (void)         
{
  return info;
}

inline Wrapper*
Server::get_wrapper (void)
{
  return wrapper;
}

inline char*
Server::get_f2pc_rm (void)
{
  return f2pc_rm;
}

inline void
Server::set_f2pc_rm (char* a_rm)
{
  f2pc_rm = a_rm;
}

inline void
Server::mark_connected (void) 
{
  connected = TRUE;
}

inline void
Server::mark_disconnected (void) 
{
  connected = FALSE;
}

inline sqluint8
Server::is_connected (void) 
{
  return connected;
}
#endif

