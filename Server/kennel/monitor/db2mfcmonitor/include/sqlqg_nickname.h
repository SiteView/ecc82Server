/******************************************************************************
**
** Source File Name: sqlqg_nickname.h
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
**              nickname objects
**
** Operating System: All
**
*******************************************************************************/
#ifndef SQL_H_SQLQG_NICKNAME
#define SQL_H_SQLQG_NICKNAME

#include "sqlqg_misc.h"

// Forward declarations.
class Nickname_Info;
class Server;

//////////////////////////////////////////////////////////////////////////////
// Nickname base class
//////////////////////////////////////////////////////////////////////////////
class Nickname : public Sqlqg_Cacheable_Object {
public:

  // Constructor
  Nickname (sqluint8* a_schema,
            sqluint8* a_nickname, 
            Server*   a_server,
            sqlint32* a_rc); 

  virtual sqlint32 initialize_nickname (Nickname_Info* a_nick_info);

  // Nickname registration hook.
  virtual sqlint32 register_nickname (Nickname_Info*   a_nick_info,
                                      Nickname_Info**  a_delta_info);

  virtual sqlint32 verify_register_nickname_info (Nickname_Info*   a_nick_info,
                                                  Nickname_Info**  a_delta_info);

  // Accessor methods.
  sqluint8* get_local_name (void);

  sqluint8* get_local_schema (void);

  sqluint8* get_hash_key (void);
  
  Server*   get_server (void);

  // Handy method to check if an option is recognized by DB2 and
  // should be ignored by the wrapper.
  virtual sqluint32 is_reserved_nickname_option (sqluint8* a_opt_name);
  virtual sqluint32 is_reserved_column_option (sqluint8* a_opt_name);
  
  // Friends of ours...
  friend class Server;

protected:
  
  // Give subclass a chance to initialize itself
  virtual sqlint32 initialize_my_nickname (Nickname_Info* a_nick_info);

  // verify_my_register_nickname_info() allows a nickname subclass
  // instance to verify any info specific to that nickname at
  // registration time.  It is called from
  // verify_register_nickname_info() method after verifying options
  // applicable to all nicknames
  virtual sqlint32 verify_my_register_nickname_info (Nickname_Info*  a_nick_info,
						     Nickname_Info** a_delta_info);

  ////////////////
  // Data.
  ////////////////

  // Local (DB2 II) name.
  sqluint8* name;
  
  // Local (DB2 II) schema.
  sqluint8* schema;

  // Hash key combines name and schema.  The hash table only stores a
  // pointer to the key, so we need to store the key here so that it
  // can be freed when the nickname is removed from the hash table.
  sqluint8* hash_key;
  
  Server*   server; 

  // Destructor. Protect this to keep people from deleting it
  // directly; they should call server->drop_nickname() instead.
  virtual ~Nickname (void);

};

// Inline methods
inline sqluint8*
Nickname::get_local_name (void)
{
  return name;
}

inline sqluint8*
Nickname::get_local_schema (void)
{
  return schema;
}

inline sqluint8*
Nickname::get_hash_key (void)
{
  return hash_key;
}
  
inline Server*
Nickname::get_server (void)
{
  return server;
}

#endif
