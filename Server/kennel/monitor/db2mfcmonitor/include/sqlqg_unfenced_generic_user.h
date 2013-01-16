/******************************************************************************
**
** Source File Name: sqlqg_unfenced_generic_user.h
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
#ifndef SQL_H_SQLQG_UNFENCED_GENERIC_USER
#define SQL_H_SQLQG_UNFENCED_GENERIC_USER

#include "sqlsystm.h"
#include "sqlqg_unfenced_user.h"


///////////////////////////////////////////////////////////////////////////////
// Unfenced_Generic_User base class for non-relational data sources.
///////////////////////////////////////////////////////////////////////////////

class Unfenced_Generic_User : public UnfencedRemote_User {
public:
  Unfenced_Generic_User (sqluint8*       a_user_name,
                         UnfencedServer* a_server,
                         sqlint32*       a_rc);

  virtual ~Unfenced_Generic_User (void);

};

// Inline methods
inline
Unfenced_Generic_User::Unfenced_Generic_User (sqluint8*       a_user_name,
                                              UnfencedServer* a_server,
                                              sqlint32*       a_rc)
: UnfencedRemote_User (a_user_name,
                       a_server,
                       a_rc)
{}

inline
Unfenced_Generic_User::~Unfenced_Generic_User (void)
{}
#endif
