/******************************************************************************
**
** Source File Name: sqlqg_connection.h
**
** (C) COPYRIGHT International Business Machines Corp. 1987, 2003
** All Rights Reserved
** Licensed Materials - Property of IBM
**
** US Government Users Restricted Rights - Use, duplication or
** disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
**
** Function = Include File defining:
**              Query gateway classes for communication
**              with a remote datasource.
**
** Operating System: All
**
*******************************************************************************/
#ifndef SQL_H_SQLQG_CONNECTION
#define SQL_H_SQLQG_CONNECTION

#include "sqlqg_misc.h"
#include "sqlsystm.h"
#include "sqlxa.h"
#include "sqlqg_runtime_data_operation.h"

// Status of a connection.
#define SQLQG_CONNECTION_IDLE        0x0000 // initial state, no flags are set
#define SQLQG_CONNECTION_CONNECTED   0x0001 // connection has been established
#define SQLQG_CONNECTION_XASTARTED   0x0002 // transaction has been xaStarted 
#define SQLQG_CONNECTION_UPDATED     0x0004 // IUD has been done on this connection
#define SQLQG_CONNECTION_XASUSPENDED 0x0008 // transaction has been xaSuspended
#define SQLQG_CONNECTION_XAOPENED    0x0010 // transaction has been xaOpened
#define SQLQG_CONNECTION_IN_USE      0x0020 // connection may have an open
                                            // cursor when set, will not have
                                            // an open cursor when unset

// Constants for client_connect_mode 
#define SQLQG_CLIENT_CONNECT_1PC 0                               
#define SQLQG_CLIENT_CONNECT_2PC 1

// Forward declarations
class Server;
class FencedServer;
class Remote_User;
class FencedRemote_User;
class Remote_Operation;
class Remote_Connection;
class Remote_Query;
class Remote_Invocation;
class Remote_Passthru;
class Runtime_Operation;
class Remote_IUD;
class Remote_SVPT;
class Remote_Lock;

struct sqlri_rquery;
struct sqlri_riud;
struct sqlr_djx_savepoint_info;
struct sqlri_rpassthru;
struct sqlri_rlock;

//////////////////////////////////////////////////////////////////////////////
// Remote_Connection Base class
//////////////////////////////////////////////////////////////////////////////
class Remote_Connection : public Sqlqg_Base_Class {
public:

  enum connection_kind {no_kind, one_phase_kind, two_phase_kind};
  
  // Constructor.
  Remote_Connection (FencedServer*      remote_server,
                     FencedRemote_User* remote_user,
                     connection_kind    k=one_phase_kind,
                     sqlint32*          rc=0);

  // Connect and disconnect.
  virtual sqlint32 connect();

  virtual sqlint32 disconnect();

  // These following methods are the XA interface for supporting federated
  // two-phase commit.
  // It is wrapper's responsbility to fill in the sqlca prior to returning any
  // error to query gateway.  However, XA rc is still expected from these calls.
  virtual int xa_open (int a_rmid,
                       long a_flags);
  virtual int xa_close (int a_rmid,
                        long a_flags);
  virtual int xa_start (SQLXA_XID* a_xid,
                        int a_rmid,
                        long a_flags);
  virtual int xa_end (SQLXA_XID* a_xid,
                      int a_rmid,
                      long a_flags);
  virtual int xa_prepare (SQLXA_XID* a_xid,
                          int a_rmid,
                          long a_flags);
  virtual int xa_commit (SQLXA_XID* a_xid,
                         int a_rmid,
                         long a_flags);
  virtual int xa_rollback (SQLXA_XID* a_xid,
                           int a_rmid,
                           long a_flags);
  virtual int xa_forget (SQLXA_XID* a_xid,
                         int a_rmid,
                         long a_flags);
  virtual int xa_recover (SQLXA_XID* a_xid_array,
                          long a_count,
		          int a_rmid,
                          long a_flags);

  virtual sqlint32 map_xa_rc (int a_rmid,
                              char* function,
                              sqlint32 rc);

  // Methods to indicate/interrogate whether a connection to the
  // server exists at the moment.
  unsigned short is_connected ();

  void mark_connected (); 
  void mark_disconnected ();
  
  // Remote operation support routines.  These methods will create
  // the appropriate remote operation subclass objects for use by the
  // UDB runtime to control the execution of a remote operation such as
  // a query or an insert/update/delete operation.
  // The default implementation of these methods is to assume the
  // operation is not supported at the data source, and return
  // a NULL object.
  virtual sqlint32 create_remote_query (Runtime_Operation* runtime_query,
                                        Remote_Query**     query);
  virtual sqlint32 create_remote_invocation (Runtime_Operation*  runtime_query,
                                             Remote_Invocation** query);
  virtual sqlint32 create_remote_passthru (Runtime_Operation* runtime_passthru,
                                           Remote_Passthru**  passthru);
  virtual sqlint32 create_remote_iud (Runtime_Operation* a_runtime_iud,
                                      Remote_IUD**       a_iud);
  virtual sqlint32 create_remote_svpt (Runtime_Operation* a_runtime_svpt,
                                       Remote_SVPT**      a_svpt);
  virtual sqlint32 create_remote_lock (Runtime_Operation* runtime_lock,
                                       Remote_Lock** remote_lock);

  // Set NLS environment variables.
  virtual sqlint32 set_NLS ();

  // Accessor methods.
  FencedServer* get_server ();

  FencedRemote_User* get_user ();

  connection_kind get_kind ();

  sqluint16 get_codepage ();
  
  unsigned short get_status ();

  void set_status_bit (unsigned short bit);

  void set_status (unsigned short s);

  sqlint32 get_rmid ();

  void set_rmid (sqlint32 id);
  
  friend class FencedServer;

  friend sqlint32 sqlqg_FMP_Open (sqlri_rquery* runtime_query);
  friend sqlint32 sqlqg_FMP_Close (sqlri_rquery* runtime_query,
                                   sqlint16 status);
  friend sqlint32 sqlqg_FMP_PassthruConnect(sqlri_rpassthru* runtime_passthru);
  friend sqlint32 sqlqg_FMP_PassthruPrepare(sqlri_rpassthru* runtime_passthru,
                                            Runtime_Data_Desc_List* *runtime_data_desc_list);
  friend sqlint32 sqlqg_FMP_PassthruCleanup(sqlri_rpassthru* runtime_passthru);
  friend sqlint32 sqlqg_FMP_SetupInvoke(sqlri_rquery* runtime_query);
  friend sqlint32 sqlqgSetupInvoke(sqlri_rquery* runtime_query);
  friend sqlint32 sqlqg_FMP_OpenIUD(sqlri_riud* a_runtime_iud);
  friend sqlint32 sqlqg_FMP_Insert(sqlri_riud* a_runtime_iud, sqlint32* a_rows);
  friend sqlint32 sqlqg_FMP_Update(sqlri_riud* a_runtime_iud, sqlint32* a_rows);
  friend sqlint32 sqlqg_FMP_Delete(sqlri_riud* a_runtime_iud, sqlint32* a_rows);
  friend sqlint32 sqlqg_FMP_CloseIUD(sqlri_riud* a_runtime_iud, sqlint16 a_status);
  friend sqlint32 sqlqg_FMP_Svpt(sqlr_djx_savepoint_info* a_svptinfo, sqlint32 a_process_type);
  friend sqlint32 sqlqg_FMP_RemoteLock(sqlri_rlock* runtime_lock);


protected:

  // Destructor
  // Remote_Connection objects cannot be destroyed directly,
  // call server->disconnect() instead.
  virtual ~Remote_Connection ();

  
  ////////////////
  // Data.
  ////////////////
  connection_kind kind;                 // connection kind (1 Phase or 2 Phase).
  FencedServer* server;                 // server associated with this connection.
  FencedRemote_User* user;              // user associated with this connection.

  Remote_Operation* operations;         // Linked list of pending operations.

  unsigned short connected;             // Flag to indicate whether server
                                        // is actually connected.


  unsigned short status;                // status of connection.
                                        // Values are given by SQLQGCON_
                                        // #defines above.
                                        // It is used as a short cut to
                                        // determine whether xaStart
                                        // has to be issued to the data
                                        // sources.  It is also used to
                                        // track whether any update has
                                        // been done using this connection.
                                        // It is also used to track whether a
                                        // cursor is open on this connection.

  sqlint32 rmid;                        // transaction ID for two-phase commit.

  sqluint16 codepage;                   // code page to use for this connection.

  ////////////////
  // Methods.
  ////////////////

  // Methods to add and remote operations from the list of
  // pending operations.
  sqlint32 add_remote_operation (Remote_Operation* op);
  sqlint32 remove_remote_operation (Remote_Operation* op);
  void remove_all_remote_operations (); 

  // Transaction support routines.  1 Phase wrappers must provide
  // implementations for the commit and rollback methods.

  virtual sqlint32 commit ();

  virtual sqlint32 rollback ();

};

// Inline methods
inline int
Remote_Connection::xa_open (int     a_rmid,
                            long    a_flags)
{
  return 0;
}

inline int
Remote_Connection::xa_close (int    a_rmid,
                             long   a_flags)
{
  return 0;
}

inline int
Remote_Connection::xa_start (SQLXA_XID* a_xid,
                             int        a_rmid,
                             long       a_flags)
{
  return 0;
}

inline int
Remote_Connection::xa_end (SQLXA_XID* a_xid,
                           int        a_rmid,
                           long       a_flags)
{
  return 0;
}

inline int
Remote_Connection::xa_prepare (SQLXA_XID* a_xid,
                               int        a_rmid,
                               long       a_flags)
{
  return 0;
}

inline int
Remote_Connection::xa_commit (SQLXA_XID* a_xid,
                              int        a_rmid,
                              long       a_flags)
{
  return 0;
}

inline int
Remote_Connection::xa_rollback (SQLXA_XID* a_xid,
                                int        a_rmid,
                                long       a_flags)
{
  return 0;
}

inline int
Remote_Connection::xa_forget (SQLXA_XID* a_xid,
                              int        a_rmid,
                              long       a_flags)
{
  return 0;
}

inline int
Remote_Connection::xa_recover (SQLXA_XID* a_xid_array,
                               long       a_count,
		               int        a_rmid,
                               long       a_flags)
{
  return 0;
}

inline unsigned short
Remote_Connection::is_connected ()
{
  return connected;
}

inline FencedServer*
Remote_Connection::get_server ()
{
  return server;
}

inline FencedRemote_User*
Remote_Connection::get_user ()
{
  return user;
}

inline Remote_Connection::connection_kind
Remote_Connection::get_kind ()
{
  return kind;
}

inline sqluint16
Remote_Connection::get_codepage ()
{
  return codepage;
}
  
inline unsigned short
Remote_Connection::get_status ()
{
  return status;
}

inline void
Remote_Connection::set_status_bit (unsigned short bit)
{
  status |= bit;
}

inline void
Remote_Connection::set_status (unsigned short s)
{
  status = s;
}

inline sqlint32
Remote_Connection::get_rmid ()
{
  return rmid;
};

inline void
Remote_Connection::set_rmid (sqlint32 id)
{
  rmid = id;
}
#endif
