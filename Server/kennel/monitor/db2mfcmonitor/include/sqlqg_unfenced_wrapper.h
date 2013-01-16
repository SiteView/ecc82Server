/******************************************************************************
**
** Source File Name: sqlqg_unfenced_wrapper.h
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
#ifndef SQL_H_SQLQG_UNFENCED_WRAPPER
#define SQL_H_SQLQG_UNFENCED_WRAPPER

#include "sqlqg_wrapper.h"

////////////////////////////////////////////////////////////////////////////////
// class UnfencedWrapper
////////////////////////////////////////////////////////////////////////////////
class UnfencedWrapper : public Wrapper {
public:

  // Constructor
  UnfencedWrapper (sqlint32* a_rc,
                   sqluint8  a_wrapper_type='R',
                   sqlint32  a_wrapper_version=0);
  
  // Destructor.
  virtual ~UnfencedWrapper (void);

  // Wrapper registration hook. Invoked as a result of CREATE
  // WRAPPER DDL statement.
  virtual sqlint32 register_wrapper (Wrapper_Info*  a_wrapper_info,
                                     Wrapper_Info** a_delta_info);

  // Verify catalog information passed in at CREATE WRAPPER time.
  virtual sqlint32 verify_register_wrapper_info (Wrapper_Info*  a_wrapper_info,
                                                 Wrapper_Info** a_delta_info);

  // Verify catalog information passed in at ALTER WRAPPER time.
  virtual sqlint32 verify_alter_wrapper_info (Wrapper_Info*  a_wrapper_info,
                                              Wrapper_Info** a_delta_info);
  
  // Methods to manage DJ cache.

  // Reset validation state of cacheable objects, so they will be
  // validated against the cache on next use
  static sqlint32 reset_cached_objects (void);
  sqlint32 reset_cached_servers (void);
  // Flush objects from the cache if they could have been affected by
  // DDL performed on behalf of this transaction.
  static sqlint32 flush_cached_objects (void);
  sqlint32 flush_cached_servers (void);

  // Methods to drop objects that were marked for deletion
  static sqlint32 drop_marked_objects (void);

  sqlint32 drop_marked_servers (void);

  // Handy method to check if an option is recognized by DB2 and
  // should be ignored by the wrapper.
  virtual sqluint32 is_reserved_wrapper_option (sqluint8* a_opt_name);
  
  // Methods to maintain the global wrapper table.
  UnfencedWrapper* get_prev_wrapper (void);
  void set_prev_wrapper (UnfencedWrapper* a_wrapper);
  UnfencedWrapper* get_next_wrapper (void);
  void set_next_wrapper (UnfencedWrapper* a_wrapper);
  UnfencedWrapper* find_forward (sqluint8* a_wrapper_name);
protected:

  ////////////////
  // Methods.
  ////////////////

  // verify_my_register_wrapper_info() allows a wrapper subclass
  // to verify registration information specific to it.
  virtual sqlint32 verify_my_register_wrapper_info (Wrapper_Info*  a_wrapper_info,
                                                    Wrapper_Info** a_delta_info);

  // verify_my_alter_wrapper_info() allows a wrapper subclass
  // to verify alter information specific to it.
  virtual sqlint32 verify_my_alter_wrapper_info (Wrapper_Info*  a_wrapper_info,
                                                 Wrapper_Info** a_delta_info);

  // Validate wrapper options DB2_THREADSAFE and DB2_FENCED.
  sqlint32 validate_wrapper_options(Wrapper_Info* catalog_info,
				    sqluint8*     sql_stmt);

  ////////////////
  // Data.
  ////////////////

  // These pointers are used to implement the global
  // wrapper table.
  class UnfencedWrapper* next;
  class UnfencedWrapper* prev;

};

////////////////////////////////////////////////////////////////////////////////
// Top-level Unfenced Wrapper Hook Function
////////////////////////////////////////////////////////////////////////////////
typedef UnfencedWrapper* (*sqlqgUnfencedWrapperHookType)(Wrapper_Info*);
#endif
