/******************************************************************************
**
** Source File Name: sqlqg_fenced_generic_user.h
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
**              class for user objects.
**
** Operating System: All
**
*******************************************************************************/
#ifndef SQL_H_SQLQG_FENCED_GENERIC_USER
#define SQL_H_SQLQG_FENCED_GENERIC_USER

#include "sqlsystm.h"
#include "sqlqg_fenced_user.h"

///////////////////////////////////////////////////////////////////////////////
// FencedGeneric_User fenced base class for non-relational data sources.
///////////////////////////////////////////////////////////////////////////////
class Fenced_Generic_User : public FencedRemote_User{
  
public:
  
  // Constructor.
  Fenced_Generic_User (sqluint8*     a_local_user_name,
                       FencedServer* a_server,
                       sqlint32*     a_rc);

  // Destructor
  virtual ~Fenced_Generic_User (void);

protected:
  

};

// Inline methods
inline
Fenced_Generic_User::Fenced_Generic_User (sqluint8*     a_local_user_name,
                                          FencedServer* a_server,
                                          sqlint32*     a_rc)
: FencedRemote_User (a_local_user_name,
                     a_server,
                     a_rc)
{}

inline
Fenced_Generic_User::~Fenced_Generic_User (void)
{}
#endif
