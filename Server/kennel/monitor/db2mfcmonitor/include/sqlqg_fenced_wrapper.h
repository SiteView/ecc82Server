/******************************************************************************
**
** Source File Name: sqlqg_fenced_wrapper.h
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
**              for wrapper objects.
**
** Operating System: All
**
*******************************************************************************/
#ifndef SQL_H_SQLQG_FENCED_WRAPPER
#define SQL_H_SQLQG_FENCED_WRAPPER

#include "sqlqg_wrapper.h"

// Forward declarations.
class Library;
class sqlqg_FMP_Request;
class Sqlqg_Array;
class Sqlqg_Node;

////////////////////////////////////////////////////////////////////////////////
// FencedWrapper class
////////////////////////////////////////////////////////////////////////////////
class FencedWrapper : public Wrapper {
public:

  FencedWrapper (sqlint32* a_rc,
                 sqluint8  a_wrapper_type='R',
                 sqlint32  a_wrapper_version=0);

  virtual ~FencedWrapper (void);

  virtual sqlint32 initialize_wrapper (Wrapper_Info* a_wrapper_info);

  virtual bool uses_node_names(void) const;

  virtual Sqlqg_Return_Code get_node_list(
     const char *a_interfaces_file,
     Sqlqg_Array *a_node_list) const;

  virtual bool uses_db_names(void) const;

  virtual Sqlqg_Return_Code get_db_list(
     const Sqlqg_Node &a_node,
     Sqlqg_Array *a_db_list) const;

protected:

  //////////
  // Data //
  //////////
  Library* library;

  friend sqlint32 sqlqg_FMP_FindWrapper (sqluint8*,
                                         sqlqg_FMP_Request*,
                                         FencedWrapper**);
};

////////////////////////////////////////////////////////////////////////////////
// Top-level FencedWrapper hook function.
////////////////////////////////////////////////////////////////////////////////
typedef FencedWrapper* (*sqlqgFencedWrapperHookType)(Wrapper_Info*);

#endif
