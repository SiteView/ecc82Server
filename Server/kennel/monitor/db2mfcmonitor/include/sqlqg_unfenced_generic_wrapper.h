/******************************************************************************
**
** Source File Name: sqlqg_unfenced_generic_wrapper.h
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
**              unfenced-side wrapper objects
**
** Operating System: All
**
*******************************************************************************/
#ifndef SQL_H_SQLQG_UNFENCED_GENERIC_WRAPPER
#define SQL_H_SQLQG_UNFENCED_GENERIC_WRAPPER

#include "sqlqg_unfenced_wrapper.h"

///////////////////////////////////////////////////////////////////////////////
// Unfenced_Generic_Wrapper Base class
///////////////////////////////////////////////////////////////////////////////

class Unfenced_Generic_Wrapper : public UnfencedWrapper {
public:

  // Default constructor. 
  Unfenced_Generic_Wrapper (sqlint32* a_rc,
                            sqlint32  a_wrapper_version=0);
  
  // Destructor.
  virtual ~Unfenced_Generic_Wrapper (void);

  // To override the default in the wrapper class.
  virtual sqlint16 is_ready_for_fenced_mode (void);
};

#endif
