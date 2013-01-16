/******************************************************************************
**
** Source File Name: sqlqg_fenced_generic_wrapper.h
**
** (C) COPYRIGHT International Business Machines Corp. 1987, 2003
** All Rights Reserved
** Licensed Materials - Property of IBM
**
** US Government Users Restricted Rights - Use, duplication or
** disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
**
** Function = Include File defining:
**              Query Gateway fenced-side interface for
**              wrapper objects
**
** Operating System: All
**
*******************************************************************************/
#ifndef SQL_H_SQLQG_FENCED_GENERIC_WRAPPER
#define SQL_H_SQLQG_FENCED_GENERIC_WRAPPER

#include "sqlqg_fenced_wrapper.h"

///////////////////////////////////////////////////////////////////////////////
// Fenced_Generic_Wrapper fenced base class for non-relational data sources.
///////////////////////////////////////////////////////////////////////////////
class Fenced_Generic_Wrapper : public FencedWrapper
{
public:

  // Constructor
  Fenced_Generic_Wrapper (sqlint32* a_rc,
			  sqlint32  a_wrapper_version=0);


  // Destructor
  virtual ~Fenced_Generic_Wrapper (void);
 
  // To override the default in the wrapper class.
  virtual sqlint16 is_ready_for_fenced_mode (void);

protected:

  virtual sqlint32 initialize_my_wrapper (Wrapper_Info* a_wrapper_info);
};

// Inline methods
inline
Fenced_Generic_Wrapper::Fenced_Generic_Wrapper (sqlint32* a_rc,
                                                sqlint32  a_wrapper_version)
: FencedWrapper (a_rc,
                 'N',
                 a_wrapper_version)
{}

inline
Fenced_Generic_Wrapper::~Fenced_Generic_Wrapper ()
{}

inline sqlint32
Fenced_Generic_Wrapper::initialize_my_wrapper (Wrapper_Info* a_wrapper_info)
{
  return 0;
}

inline sqlint16
Fenced_Generic_Wrapper::is_ready_for_fenced_mode (void)
{
  return 1;
}
#endif
