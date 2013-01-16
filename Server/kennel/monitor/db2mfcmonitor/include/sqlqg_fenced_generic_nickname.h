/******************************************************************************
**
** Source File Name: sqlqg_fenced_generic_nickname.h
**
** (C) COPYRIGHT International Business Machines Corp. 1987, 2003
** All Rights Reserved
** Licensed Materials - Property of IBM
**
** US Government Users Restricted Rights - Use, duplication or
** disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
**
** Function = Include File defining:
**              Query Gateway fenced-side interface class
**
** Operating System: All
**
*******************************************************************************/
#ifndef SQL_H_SQLQG_FENCED_GENERIC_NICKNAME
#define SQL_H_SQLQG_FENCED_GENERIC_NICKNAME

#include "sqlsystm.h"
#include "sqlqg_fenced_nickname.h"   

///////////////////////////////////////////////////////////////////////////////
// Fenced_Generic_Nickname fenced base class for non-relational data sources.
///////////////////////////////////////////////////////////////////////////////
class Fenced_Generic_Nickname : public FencedNickname
{

public:

  // Constructor.
  Fenced_Generic_Nickname (sqluint8*     a_schema,
                           sqluint8*     a_name,
                           FencedServer* a_server,
                           sqlint32*     a_rc);

  // Destructor
  virtual ~Fenced_Generic_Nickname (void);
  
  // Handy methods to check if an option is defined at the generic
  // server level.
  virtual sqluint32 is_reserved_nickname_option (sqluint8* a_opt_name);
  virtual sqluint32 is_reserved_column_option (sqluint8* a_opt_name);

protected:
  
};

// Inline methods
inline
Fenced_Generic_Nickname::Fenced_Generic_Nickname (sqluint8*     a_schema,
                                                  sqluint8*     a_name,
                                                  FencedServer* a_server,
                                                  sqlint32*     a_rc)
: FencedNickname (a_schema, a_name,  a_server, a_rc)
{}

inline
Fenced_Generic_Nickname::~Fenced_Generic_Nickname ()
{}
#endif
