/******************************************************************************
**
** Source File Name: sqlqg_fenced_server.h
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
**              server objects.
**
** Operating System: All
**
*******************************************************************************/
#ifndef SQL_H_SQLQG_FENCED_SERVER
#define SQL_H_SQLQG_FENCED_SERVER

#include "sqlqg_server.h"
#include "sqlqg_misc.h"

// Forward declarations.
class FencedWrapper;
class FencedRemote_User;
class Remote_Connection;
class sqlqg_FMP_Request;
class sqlqg_FMP_Reply;
struct sqlo_xlatch;
class FencedNickname;
class Nickname_Info;
class User_Info;
class Sqlqg_Array;
////////////////////////////////////////////////////////////////////////////////
// FencedServer class
////////////////////////////////////////////////////////////////////////////////
class FencedServer : public Server {
public:

  // Constructors
  FencedServer (sqluint8*       a_server_name,
                server_kind     a_server_kind,
                FencedWrapper*  a_server_wrapper,
                sqlint32*       a_rc);

  virtual sqlint32 initialize_server (Server_Info*  a_server_info,
				      sqlint32      a_init_subclass=TRUE);

  virtual sqlint32 commit (void);
  virtual sqlint32 rollback (void);

  virtual Sqlqg_Return_Code get_nickname_list(
                            const char *a_remote_auth_id,
                            const char *a_remote_password,
                            Sqlqg_Array *a_nickname_list);

  sqlint32 drop_remote_user (Remote_User* a_user,
                             Sqlqg_DeleteType a_flag = SQLQG_DELETE_ALL);

  sqlint32 connect_current_user (Remote_Connection** a_connection);

  sqlint32 find_connection (Remote_Connection** a_connection);

  sqlint32 connect (FencedRemote_User*   a_user,
                    Remote_Connection**  a_connection);

  sqlint32 disconnect (Remote_Connection* a_connection);

  sqlint32 update_unref_counts (void);

  sqlint32 isExternalDataSource (void);

  sqlqg_FMP_Reply* getReply (void);
  void setReply (sqlqg_FMP_Reply* a_reply);

  friend sqlint32 sqlqg_FMP_FindServer (sqluint8*          a_server_name,
                                        sqlqg_FMP_Request* a_req,
                                        FencedServer**     a_server);

  sqlint32 reset_unreferenced_count (const sqlint32 a_remote_operation_rc);

  void has_connected (void);
  void has_disconnected (void);

  sqluint8 get_mon_connect (void); 
  sqluint8 get_mon_disconnect (void); 

  sqlint32 block_one_req_iud (void);

  void Latch (void);

  void UnLatch (void);

  virtual const char *get_licensing_data_source_name(void) const;

  virtual const char *get_licensing_server_name(void) const;
  
protected:

  //////////////
  // Data     //
  //////////////
  struct sqlo_xlatch* xlatchP;

  sqlqg_FMP_Reply*    m_reply;

  FencedRemote_User*  connected_user;

  Hash_Table          connections;

  Remote_Connection*  mru_connection;

  sqluint8 mon_connect;    // flag to indicate whether server has connected
  sqluint8 mon_disconnect; // flag to indicate whether server has disconnected

  //////////////
  // Methods  //
  //////////////
  // Destructor.
  // Do not destroy directly; use wrapper->drop_server ()
  virtual ~FencedServer (void);

  virtual sqlint32 create_remote_connection (FencedRemote_User*   a_user,
                                             Remote_Connection**  a_connection);

};

// Inline methods
inline sqlqg_FMP_Reply*
FencedServer::getReply (void)
{
  return m_reply;
}

inline void
FencedServer::setReply (sqlqg_FMP_Reply* a_reply)
{
  m_reply = a_reply;
}

inline void
FencedServer::has_connected (void) 
{
  mon_connect = TRUE;
}

inline void
FencedServer::has_disconnected (void) 
{
  mon_disconnect = TRUE;
}

#endif
