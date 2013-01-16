/******************************************************************************
**
** Source File Name: sqlqg_unfenced_nickname.h
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
**              catalog information objects
**
** Operating System: All
**
*******************************************************************************/
#ifndef SQL_H_SQLQG_UNFENCED_NICKNAME
#define SQL_H_SQLQG_UNFENCED_NICKNAME

#include "sqlqg_nickname.h"

// Forward declaratoins
class Nickname_Info;
class UnfencedServer;

class  UnfencedNickname : public Nickname {
public:
 
  // Constructor.
  UnfencedNickname (sqluint8*       a_schema_name,
                    sqluint8*       a_nickname_name,
                    UnfencedServer* a_server,
                    sqlint32*       a_rc);  

  
  // Verify options specified on ALTER nickname object.
  virtual sqlint32 verify_alter_nickname_info (Nickname_Info*  a_nickname_info,
                                               Nickname_Info** a_delta_info);

  friend class UnfencedServer;
protected:

  // Destructor. Protect this to keep people from deleting it
  // directly; they should call server->drop_nickname() instead.
  virtual ~UnfencedNickname (void);

  virtual sqlint32 verify_my_alter_nickname_info (Nickname_Info*  a_nick_info,
						  Nickname_Info** a_delta_info);
};

#endif
