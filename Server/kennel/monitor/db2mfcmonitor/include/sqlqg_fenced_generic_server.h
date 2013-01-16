/******************************************************************************
**
** Source File Name: sqlqg_fenced_generic_server.h
**
** (C) COPYRIGHT International Business Machines Corp. 1987, 2003
** All Rights Reserved
** Licensed Materials - Property of IBM
**
** US Government Users Restricted Rights - Use, duplication or
** disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
**
** Function = Include File defining:
**              Query Gateway fenced-side interface
**              class for servers.
**
** Operating System: All
**
*******************************************************************************/
#ifndef SQL_H_SQLQG_FENCED_GENERIC_SERVER
#define SQL_H_SQLQG_FENCED_GENERIC_SERVER

#include "sqlsystm.h"
#include "sqlqg_fenced_server.h" 

///////////////////////////////////////////////////////////////////////////////
// Fenced_Generic_Server fenced base class for non-relational data sources.
// Defaults provided for all methods
///////////////////////////////////////////////////////////////////////////////
class Fenced_Generic_Server : public FencedServer
{
public:
  
  
  Fenced_Generic_Server (sqluint8*      a_server_name,
                         FencedWrapper* a_server_wrapper,
                         sqlint32*      a_rc);

  virtual ~Fenced_Generic_Server (void);

  virtual Remote_User* create_remote_user (sqluint8* a_user_name,
                                           sqlint32* a_rc); 

  virtual Nickname* create_nickname (sqluint8* a_schema_name,
				     sqluint8* a_nickname_name,
				     sqlint32* a_rc);
  
protected:
  
};

// Inline methods
inline
Fenced_Generic_Server::Fenced_Generic_Server (sqluint8*      a_server_name,
                                              FencedWrapper* a_server_wrapper,
                                              sqlint32*      a_rc)
: FencedServer (a_server_name,
                Server::generic_kind,
                a_server_wrapper,
                a_rc)
{}

inline
Fenced_Generic_Server::~Fenced_Generic_Server (void)
{}
#endif
