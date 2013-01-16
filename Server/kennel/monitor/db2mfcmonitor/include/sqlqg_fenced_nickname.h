/******************************************************************************
**
** Source File Name: sqlqg_fenced_nickname.h
**
** (C) COPYRIGHT International Business Machines Corp. 1987, 2003
** All Rights Reserved
** Licensed Materials - Property of IBM
**
** US Government Users Restricted Rights - Use, duplication or
** disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
**
** Function = Include File defining:
**              Query Gateway fenced-side interfaces
**              for nicknames.
**
** Operating System: All
**
*******************************************************************************/
#ifndef SQL_H_SQLQG_FENCED_NICKNAME
#define SQL_H_SQLQG_FENCED_NICKNAME

#include "sqlqg_nickname.h"

/* Forward declarations. */
class FencedServer;
class Sqlqg_Array;

////////////////////////////////////////////////////////////////////////////////
// FencedNickname class
////////////////////////////////////////////////////////////////////////////////
class FencedNickname : public Nickname {
public:

  // Constructor.
  FencedNickname (sqluint8*     a_schema,
                  sqluint8*     a_nickname_name,
                  FencedServer* a_nickname_server,
                  sqlint32*     a_rc);  


  // Methods in support of Transparent DDL
  virtual sqlint32 create_remote_table (Nickname_Info*  a_nickname_info,
                                        Nickname_Info** a_delta_info);
  virtual sqlint32 verify_create_remote_table_info (Nickname_Info*  a_nickname_info,
                                                    Nickname_Info** a_delta_info);

  virtual sqlint32 drop_remote_table (Nickname_Info*  a_nickname_info,
                                      Nickname_Info** a_delta_info);
  virtual sqlint32 verify_drop_remote_table_info (Nickname_Info*  a_catalog_info,
                                                  Nickname_Info** a_delta_info);

  virtual sqlint32 alter_remote_table (Nickname_Info*  a_nickname_info,
                                       Nickname_Info** a_delta_info);
  virtual sqlint32 verify_alter_remote_table_info (Nickname_Info*  a_catalog_info,
                                                   Nickname_Info** a_delta_info);

  virtual Sqlqg_Return_Code get_column_list(
                            const char *a_remote_auth_id,
                            const char *a_remote_password,
                            Sqlqg_Array *a_column_list);

protected:
  virtual ~FencedNickname (void);
};

#endif
