/******************************************************************************
**
** Source File Name: sqlqg_unfenced_generic_server.h
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
**              unfenced-side server objects
**
** Operating System: All
**
*******************************************************************************/
#ifndef SQL_H_SQLQG_UNFENCED_GENERIC_SERVER
#define SQL_H_SQLQG_UNFENCED_GENERIC_SERVER

#include "sqlsystm.h"
#include "sqlqg_unfenced_server.h"

class Remote_User;
class Request;
class Reply;
class Predicate_List;

///////////////////////////////////////////////////////////////////////////////
// Unfenced_Generic_Server base class for non-relational data sources.
///////////////////////////////////////////////////////////////////////////////
class Unfenced_Generic_Server : public UnfencedServer {
public:

  // Constructor.
  Unfenced_Generic_Server (sqluint8*        a_server_name,
                           UnfencedWrapper* a_wrapper,
                           sqlint32*        a_rc);
   
  // Destructor.
  virtual ~Unfenced_Generic_Server (void);

  virtual Remote_User* create_remote_user (sqluint8* a_user_name,
					   sqlint32* a_rc);

  // Compile. plan_request must be implemented in the wrapper, for the rest
  // there are default implementations
  virtual sqlint32  plan_request (Request* a_req,
                                  Reply**  a_rpl) = 0; 

  virtual sqlint32  get_selectivity (Predicate_List* a_pl,
                                     float*          a_selectivity);

  virtual sqlint32  create_reply (Request* a_req,
                                  Reply**  a_rpl);

  //Not to be overriden by the wrapper writer
  sqlint32 initialize_server (Server_Info* a_server_info,
                              sqlint32     a_init_subclass=TRUE);

  sqlint32 verify_register_server_info (Server_Info*  a_server_info,
                                        Server_Info** a_delta_info);
  sqlint32 verify_alter_server_info (Server_Info*  a_server_info,
                                     Server_Info** a_delta_info);

protected:
};

#endif
