/******************************************************************************
**
** Source File Name: sqlqg_catalog.h
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
#ifndef SQL_H_SQLQG_CATALOG
#define SQL_H_SQLQG_CATALOG

////////////////////////////////////////////////////////////////////////////////
// Assumptions:
//
// CatalogInfo is an abstract class that encapsulates catalog
// information.  It is subclassed for each persistent object such as a
// Wrapper, Server, Nickname, etc. Each subclass contains the
// information obtained from a DDL statement or a catalog that
// describes the object.  For example, the instance passed into the
// Wrapper::register(void) method would contain the wrapper name,
// location, and options that were specified on the CREATE WRAPPER
// statement. Similarly, the instance passed into the
// Server::register(void) method would contain the server name, wrapper
// name, and options that were specified on the CREATE SERVER
// statement.  The CatalogInfo information is entered into the
// catalogs after a successful call to the appropriate register
// method.
//
////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include "sqlsystm.h"

#include "sqlqg_misc.h"

// Forward declarations
class Local_Type;
class Nickname_Info;
class sqlqg_FMP_Runtime_Object;

//////////////////////////////////////////////////////////////////////////////
// Option_Value class:
// Encapsulates one option value of a multi-valued option, with its
// associated timestamp and unique id.
//////////////////////////////////////////////////////////////////////////////
class Option_Value : public Sqlqg_Base_Class {
public:

  // Constructor.  Copies value, timestamp and uid into new
  // storage which will be deallocated with this object.
  Option_Value (sqlint32*  a_rc,
                sqluint8*  a_value,
                sqlint32   a_value_len,
                void*      a_tstamp,
                sqluint8*  a_uid,
                sqlint32   a_uid_len);

  // "Unpack" constructor.  Modifies buffer pointer!
  Option_Value (sqlint32*  a_rc,
                void**     a_ppbuf);

  // Destructor.  Frees storage for value, tstamp and uid.
  ~Option_Value (void);

  // Accessors
  sqluint8* get_value (void);

  void* get_tstamp (void);
  void set_tstamp (void* new_tstamp);

  sqluint8* get_uid (void);
  sqlint32 set_uid (sqluint8*  a_new_uid,
                    sqlint32   a_new_len);

  sqlint32 get_packed_length (void);
  sqlint32 pack (void**     a_ppbuf,
                 sqlint32*  a_pbuf_len);

private:

  // Header for packed value
  struct Value_Header {
    sqlint16 value_len;
    sqlint16 uid_len;
    char     tstamp[SQLQG_MAX_TIMESTAMP_LEN];
  };

  sqlint32       value_length;

  sqluint8*      value;
  sqluint8*      uid;
  char           tstamp[SQLQG_MAX_TIMESTAMP_LEN];
  Option_Value*  next_val;
  Option_Value*  prev_val;

  friend class Multi_Value_Option;
};

//////////////////////////////////////////////////////////////////////////////
// Catalog_Option base class:
// Encapsulates one or more (name, value) pairs plus links to next and previous.
// Subclasses are Single_Value_Option and Multi_Value_Option.
//////////////////////////////////////////////////////////////////////////////
class Catalog_Option : public Sqlqg_Base_Class {
public:

  // Enumeration type describing what should be done with this option.
  // Actions are only relevant for Catalog_Option objects that
  // describe info obtained from a DDL statement.
  enum Action {sqlqg_None, sqlqg_Add, sqlqg_Set, sqlqg_Drop, sqlqg_SetSO};

  // Default constructor, for use by subtype "unpack" constructors
  Catalog_Option (void);

  // Constructor for subtype constructors to initialize the base
  // class.  opt_name need not be null-terminated.  It will be copied
  // into new storage from the ? heap, and null-terminated.  A default
  // action ("None") is provided for situations where the action is not
  // relevant.
  Catalog_Option (sqlint32*  a_rc,
                  sqluint8*  a_opt_nm,
                  sqlint32   a_nm_len,
                  Action     a_act = sqlqg_None);

  // Destructor.  The storage associated with the name(s) and value(s)
  // will be deleted.
  virtual ~Catalog_Option (void);

  // Method to get option name as null-terminated string
  sqluint8* get_name (void);

  Action get_action (void);

  // Pure virtual functions implemented by subclasses
  // Get value of option.  Returns current value for multi-valued options
  virtual sqluint8* get_value (void);

  virtual sqluint8* get_value (sqlint32*  a_length);

  // These are defined on the base class, but return SQLQG_ERROR for
  // single-valued options.

  virtual sqlint32 drop_value (sqluint8*  a_uid,
                               sqlint32   a_uid_len);

  virtual sqlint32 add_value (sqluint8*  a_opt_val,
                              sqlint32   a_val_len,
                              void*      a_tstamp,
                              sqluint8*  a_uid,
                              sqlint32   a_uid_len,
                              const char*a_option_type="",
                              const char*a_object_name="");

  virtual sqlint32 get_value (sqluint8*       a_uid,
                              sqlint32        a_uid_len,
                              Option_Value**  a_valp);

  virtual Option_Value* get_first_value (void);

  virtual Option_Value* get_next_value (Option_Value*  a_current_value);

  virtual sqlint16 n_values (void);   // returns "1" for single-valued opts

  virtual sqlint32 get_packed_length (void);

  virtual sqlint32 pack (void**     a_ppbuf,
                         sqlint32*  a_pbuf_len);

protected:
  sqluint8*        opt_name;
  Action           action;

  // Make the Catalog_Info class a friend so that it can manipulate
  // the links.  Note that this friendship is not inherited by the
  // Catalog_Info subclasses.
  friend class Catalog_Info;
  Catalog_Option*  next_opt;
  Catalog_Option*  prev_opt;
};

//////////////////////////////////////////////////////////////////////////////
// Single-valued option class.
// Methods that make sense only for multi-valued options
// push an error and return SQLQG_ERROR
//////////////////////////////////////////////////////////////////////////////
class Single_Value_Option : public Catalog_Option {
public:

  // Constructor
  Single_Value_Option (sqlint32*  a_rc,
                       sqluint8*  a_opt_nm,
                       sqlint32   a_nm_len,
                       sqluint8*  a_opt_val,
                       sqlint32   a_val_len,
                       Action     a_act =  sqlqg_None);

  // "Unpack" constructor.  Modifies buffer pointer!
  Single_Value_Option (sqlint32*  a_rc,
                       void**     a_ppbuf);

  ~Single_Value_Option (void);

  sqluint8* get_value (void);

  sqluint8* get_value (sqlint32*  a_length);

  sqlint16 n_values (void);

  // These make sense for multi-value options only.  The
  // implementations here push an error and return SQLQG_ERROR
  sqlint32 drop_value (sqluint8*  a_uid,
                       sqlint32   a_uid_len);

  sqlint32 add_value (sqluint8*  a_opt_val,
                      sqlint32   a_val_len,
                      void*      a_tstamp,
                      sqluint8*  a_uid,
                      sqlint32   a_uid_len,
                      const char*a_option_type="",
                      const char*a_oject_name="");

  sqlint32 get_value (sqluint8*       a_uid,
                      sqlint32        a_uid_len,
                      Option_Value**  a_valp);
  Option_Value* get_first_value (void);
  Option_Value* get_next_value (Option_Value*  a_current_value);

  sqlint32 get_packed_length (void);

  // Modifies buffer pointer and length!
  sqlint32 pack (void**     a_ppbuf,
                 sqlint32*  a_pbuf_len);

private:

  sqlint32 value_length;

  // Header for packed Single_Value_Option
  struct SV_Opt_Header {
    sqlint16 name_len;
    sqlint16 value_len;
    sqlint16 action;
  };

  sqluint8* opt_value;
};

//////////////////////////////////////////////////////////////////////////////
// Multi-valued option class.
// Each value has a timestamp and unique id.  The
// value with the maximum timestamp is considered the "current" value,
// and is returned by the get_value(void) method.  To drop a value, its
// unique id must be supplied.  Methods to iterate through all values
// and look up a value by uid are also provided.
//////////////////////////////////////////////////////////////////////////////
class Multi_Value_Option : public Catalog_Option {
public:

  // Constructor
  Multi_Value_Option (sqlint32*  a_rc,
                      sqluint8*  a_opt_nm,
                      sqlint32   a_nm_len,
                      Action     a_act = sqlqg_None);

  // "Unpack" constructor.  Modifies buffer pointer!
  Multi_Value_Option (sqlint32*  a_rc,
                      void**     a_ppbuf);

  ~Multi_Value_Option (void);

  sqluint8* get_value (void);    // returns current value (max tstamp)
  sqluint8* get_value (sqlint32*  a_length);

  sqlint16 n_values (void);

  sqlint32 drop_value (sqluint8*  a_uid,
                       sqlint32   a_uid_len);

  sqlint32 add_value (sqluint8*  a_opt_val,
                      sqlint32   a_val_len,
                      void*      a_tstamp,
                      sqluint8*  a_uid,
                      sqlint32   a_uid_len,
                      const char*a_option_type="",
                      const char*a_object_name="");
  sqlint32 get_value (sqluint8*       a_uid,
                      sqlint32        a_uid_len,
                      Option_Value**  a_valp);

  Option_Value* get_first_value (void);
  Option_Value* get_next_value (Option_Value*  a_current_value);

  sqlint32 get_packed_length (void);
  sqlint32 pack (void**     a_ppbuf,
                 sqlint32*  a_pbuf_len);

private:
  void initialize (void);

  // Insert a value into the linked list.  This routine is used both
  // by the add_value method and by the "unpack" constructor.
  sqlint32 insert_value (Option_Value*  a_new_val);

  // Header for packed Multi_Value_Option
  struct MV_Opt_Header {
    sqlint16 name_len;
    sqlint16 n_values;
    sqlint16 action;
  };

  int            num_values;
  Option_Value*  val_chain;
};

//////////////////////////////////////////////////////////////////////////////
// Catalog_Info base class
//////////////////////////////////////////////////////////////////////////////
class Catalog_Info : public Sqlqg_Base_Class {
public:
  // Make Catalog_Option and Option_Value friends so they can use the
  // copy_and_terminate method.
  friend class Catalog_Option;
  friend class Single_Value_Option;
  friend class Multi_Value_Option;
  friend class Option_Value;

  // Default constructor.
  Catalog_Info (void);

  // Destructor.  Deletes all associated Catalog_Option objects as well
  virtual ~Catalog_Info (void);

  // Add a single-valued option with the specified name and value.
  // Subtypes of Catalog_Info that use multi-valued options must
  // override this method's implementation.  add_option(void) will return
  // SQLQG_ERROR if the option is already on the list, and its value
  // will not be altered.  Once an option has been added, its storage
  // becomes the responsibility of the Catalog_Info object to which it
  // was added.
  virtual sqlint32 add_option (sqluint8*               a_opt_name,
                               sqlint32                a_name_len,
                               sqluint8*               a_opt_value,
                               sqlint32                a_value_len,
                               Catalog_Option::Action  a_action = Catalog_Option::sqlqg_None,
                               const char*             a_option_type="",
                               const char*             a_object_name="");

  // Drops an option and all of its values.  Subtypes that do not
  // wish to allow "bulk delete" for multi-valued options should
  // override this method.  drop_option(void) deletes the dropped
  // Catalog_Option object.
  virtual sqlint32 drop_option (Catalog_Option*  a_option);

  // get_option(void) returns the option specified by opt_name.  The method
  // returns an error if it encounters one.  Note that it is not an
  // error if the option does not exist.  In this case, the routine
  // will return SQLQG_NOVALUE and set option to NULL.
  sqlint32 get_option (sqluint8*         a_opt_name,
                       sqlint32          a_name_len,
                       Catalog_Option**  a_option);

  // Methods to iterate through the option list
  Catalog_Option* get_first_option (void);
  Catalog_Option* get_next_option (Catalog_Option*  a_current_option);

  virtual sqluint8* get_timestamp_option (void);
  virtual sqlint16  get_drop_pending_option (void);

  // Virtual methods for packing info into a memory block, which
  // must be implemented by each Catalog_Info subclass.  The
  // corresponding unpacking operation is done by an "unpack"
  // constructor for the appropriate class.  The packed format can be
  // used to send DDL over a wire, or as a "poor man's packed
  // descriptor" for data sources not wishing to implement their own
  // packed descriptor format.  Note that the "pack" method modifies
  // its arguments!  This allows invocations of "pack" to be chained
  // or nested, because upon return the pointer points to the next
  // free byte in the buffer and the length reflects the number of
  // unused bytes remaining.  The "unpack" constructor works the same
  // way.

  virtual sqlint32 get_packed_length (void);

  virtual sqlint32 pack (void**     a_ppbuf,
                         sqlint32*  a_pbuf_len);

  // Merge the options from a "delta" Catalog_Info into a master
  // Catalog_Info.  The Catalog_Info implementation of this method
  // will only work for subtypes of Catalog_Info that use the default
  // Catalog_Option subtype, Single_Value_Option.  Other subtypes
  // (e.g. Server_Info) should reimplement this method for the
  // Catalog_Option subtype they use (e.g. Multi_Value_Option).
  virtual sqlint32 merge_options (Catalog_Info*  a_delta_info);

protected:

  // Insert an option into the linked list.  This routine is
  // implemented here so it can be used by the add_option and
  // unpack_options methods as well as by subtypes that wish to
  // override or extend these methods.
  sqlint32 insert_option (Catalog_Option*  a_new_opt);

  // Generally useful string management routines.  Also used by
  // Catalog_Option class, via "friend" mechanism.

  // Copies an unterminated string of specified length and terminates
  // the copy.
  static sqlint32 copy_and_terminate (sqluint8*   a_inArray,
                                      sqlint32    a_len,
                                      sqluint8**  a_outStr);

  // Copies an unterminated string of specified length, terminates the
  // copy, and if successful sets flag to indicate attribute is valid.
  // The flag will only be set valid if the input array (and hence the
  // output string) is non-null.  This effectively precludes having an
  // attribute that is valid but NULL.  Should this be desired, this
  // routine is all that one should need to change.
  static sqlint32 copy_set_valid_and_terminate (sqluint8*   a_inArray,
                                                sqlint32    a_len,
                                                sqluint8**  a_outStr,
                                                int*        a_pFlag);

  static sqlint32 get_string_if_valid (int         a_flag,
                                       sqluint8*   a_src,
                                       sqluint8**  a_target);

  // A suite of overloaded functions. If source attribute is
  // valid, copy it to target.  Update validity flag at target.  Used
  // for implementing the XXX_Info::merge.

  // For strings.  Takes current rc, and does nothing if an error has
  // already occurred.
  static void copy_if_valid (sqlint32*   a_rc,
                             sqluint8*   a_src,
                             int         a_sflag,
                             sqluint8**  a_target,
                             int*        a_tflag);

  // For fixed-length char arrays
  static void copy_if_valid (sqluint8*  a_src,
                             int        a_sflag,
                             int        a_slength,
                             sqluint8*  a_target,
                             int*       a_tflag);

  // For sqlint32s
  static void copy_if_valid (sqlint32   a_src,
                             int        a_sflag,
                             sqlint32*  a_target,
                             int*       a_tflag);

  // For sqlint16s
  static void copy_if_valid (sqlint16   a_src,
                             int        a_sflag,
                             sqlint16*  a_target,
                             int*       a_tflag);

  // For sqluint8s
  static void copy_if_valid (sqluint8   a_src,
                             int        a_sflag,
                             sqluint8*  a_target,
                             int*       a_tflag);

  // For sqlint64s
  static void copy_if_valid (sqlint64   a_src,
                             int        a_sflag,
                             sqlint64*  a_target,
                             int*       a_tflag);


  // Handy routine for packing.  If the attr is not valid, set
  // hdr_length to HDR_LENGTH_INVALID and don't copy anything.  If
  // attr is valid but NULL, set hdr_length to HDR_LENGTH_NULL and
  // don't copy anything.  If attr is valid and non-NULL, update count
  // of bytes remaining and see if there is room for it in the buffer.
  // If there is room, set hdr_length, copy data to buffer and update
  // buffer pointer.  Note that the space remaining is updated even if
  // nothing is copied because the buffer is too small.  This means
  // you can safely issue a whole string of calls to this routine and
  // just check for buffer overflow once at the end.
  static void copy_and_adjust_if_valid (sqluint8*   a_attr,
                                        int         a_flag,
                                        sqlint16*   a_pHdr_length,
                                        sqluint8**  a_bufpp,
                                        sqlint32*   a_pRemaining);

  // Handy routine for unpacking.  If hdr_length is
  // HDR_LENGTH_INVALID, attribute is not valid.  Clear valid flag and
  // set pointer to NULL.  If hdr_length is HDR_LENGTH_NULL, attribute
  // is valid but NULL.  Set valid flag and set pointer to NULL.
  // Otherwise, set valid flag and copy value to new null-terminated
  // string.  Return SQLQG_ERROR if there is trouble
  // (e.g. allocation failure).
  static sqlint32 copy_set_valid_and_adjust (sqluint8**  a_attr,
                                             int*        a_pFlag,
                                             sqlint32    a_hdr_length,
                                             sqluint8**  a_bufpp);

  // Obtain length of options when packed into a block.  Generic
  // implementation; works for any Catalog_Option subtype.
  sqlint32 get_packed_option_length (void);

  // Pack options into a block.  Update buffer pointer and length
  // remaining.  Generic implementation; works for any Catalog_Option
  // subtype.
  sqlint32 pack_options (void**     a_ppbuf,
                         sqlint32*  a_pbuf_len);

  // Unpack options into the target Catalog_Info object.  Modifies the
  // buffer pointer!  Unlike the preceding two methods, the
  // Catalog_Info implementation of this method will only work for
  // subtypes of Catalog_Info that use the default Catalog_Option
  // subtype, Single_Value_Option.  Other subtypes (e.g. Server_Info)
  // should reimplement this method for the Catalog_Option subtype
  // they use (e.g. Multi_Value_Option).
  virtual sqlint32 unpack_options (void**  a_ppbuf);

private:
  Catalog_Option* opt_chain;
};

////////////////////////////////
// Wrapper_Info subclass.
////////////////////////////////
class Wrapper_Info: public Catalog_Info {
public:

  // Default constructor
  Wrapper_Info (void);

  // "Unpack" constructor.  Modifies buffer pointer!
  Wrapper_Info (sqlint32*  a_rc,
                void**     a_ppbuf);

  ~Wrapper_Info (void);

  sqlint32 add_option (sqluint8*  a_opt_name,  sqlint32  a_opt_name_len,
                       sqluint8*  a_opt_value, sqlint32  a_opt_value_len,
                       Catalog_Option::Action  a_act = Catalog_Option::sqlqg_None,
                       const char*      a_opt_type=SQLQG_WRAPPER_OPTION);

  // Accessor methods.  The "get" methods return 0 if the field
  // contains valid data, or SQLQG_NOVALUE if it doesn't.  The "set"
  // methods can return a "real" error, e.g., if storage allocation
  // fails.
  sqlint32 set_wrapper_name (sqluint8*  a_name,
                             sqlint32   a_name_len);
  sqlint32 get_wrapper_name (sqluint8**  a_name);

  sqlint32 set_corelib (sqluint8* a_lib_name,
                        sqlint32  a_lib_name_len);
  sqlint32 get_corelib (sqluint8** a_lib_name);

  void set_type (sqluint8 a_wrapper_type);
  sqlint32 get_type (sqluint8* a_wrapper_type);

  void set_version (sqlint32 a_wrapper_version);
  sqlint32 get_version (sqlint32* a_wrapper_version);

  sqlint32 merge (Wrapper_Info* a_delta_info);

  sqlint32 get_packed_length (void);

  sqlint32 pack (void**     a_ppbuf,
                 sqlint32*  a_pbuf_len);

  sqlint32 copy (Wrapper_Info** a_new_wrapper_info);

private:

  void initialize (void);

  // Header for packed Wrapper_Info
  struct Wrapper_Info_Hdr {
    sqlint16 name_len;
    sqlint16 corelib_len;
    sqlint16 type_valid;
    sqluint8 type;
    sqlint16 version_valid;
    sqlint32 version;
  };

  sqluint8* name;
  int       name_valid;

  sqluint8* corelib;
  int       corelib_valid;

  sqluint8  type;
  int       type_valid;

  sqlint32  version;
  int       version_valid;

};

//////////////////////////////////////
// RFunc_Parm_Info subclass.
////////////////////////////////////////
class RFunc_Parm_Info : public Catalog_Info {
public:
  // Default constructor.  Clear all "valid" fields
  RFunc_Parm_Info (void);

  // Constructor for case where ordinal is known at creation time
  RFunc_Parm_Info (sqlint16  a_ordinal);

  // "Unpack" constructor.  Modifies buffer pointer!
  RFunc_Parm_Info (sqlint32*  a_rc,
                   void**     a_ppbuf);

  // Destructor.
  ~RFunc_Parm_Info (void);

  void set_ordinal (sqlint16  a_ordinal);
  sqlint32 get_ordinal (sqlint16*  a_ordinal);

  RFunc_Parm_Info* get_next_parm (void);

  sqlint32 merge (RFunc_Parm_Info* a_delta_info);

  sqlint32 get_packed_length (void);

  // Modifies buffer pointer and length!
  sqlint32 pack (void**     a_ppbuf,
                 sqlint32*  a_pbuf_len);

private:
  void initialize (void);

  sqlint16         ordinal;
  int              ordinal_valid;
  RFunc_Parm_Info* next_parm;

  friend class Remote_Function_Info;

  struct RFunc_Parm_Info_Hdr {
    sqlint16 ord;
  };
};

//////////////////////////////////////
// Remote_Function_Info subclass.
////////////////////////////////////////
class Remote_Function_Info : public Catalog_Info {
public:

  // Default constructor.  Clear all "valid" fields
  Remote_Function_Info (void);

  // "Unpack" constructor.  Modifies buffer pointer!
  Remote_Function_Info (sqlint32*  a_rc,
                        void**  a_ppbuf);

  // Constructor for complete function mapping (excluding parameters)
  Remote_Function_Info (sqlint32*  a_rc,
                        Catalog_Option::Action  a_act,
                        sqluint8*  a_mapping_name,    sqlint32  a_m_n_len,
                        sqluint8*  a_local_signature, sqlint32  a_l_sig_len,
                        sqluint8*  a_local_schema,    sqlint32  a_l_sch_len,
                        sqlint32  a_funcid,
                        sqluint8*  a_specific_name,   sqlint32  a_s_n_len,
                        sqluint8*  a_definer,         sqlint32  a_d_len,
                        void*  a_tstamp);

  Remote_Function_Info (sqlint32*  a_rc,
                        sqluint8*  a_mapping_name,    sqlint32  a_m_n_len,
                        sqluint8*  a_local_signature, sqlint32  a_l_sig_len,
                        sqluint8*  a_local_schema,    sqlint32  a_l_sch_len,
                        sqlint32  a_funcid,
                        sqluint8*  a_specific_name,   sqlint32  a_s_n_len,
                        sqluint8*  a_definer,         sqlint32  a_d_len,
                        void*  a_tstamp);

  // Destructor
  ~Remote_Function_Info (void);

  void set_action (Catalog_Option::Action  a_act);

  sqlint32 add_option (sqluint8*  a_opt_name,  sqlint32  a_opt_name_len,
                       sqluint8*  a_opt_value, sqlint32  a_opt_value_len,
                       Catalog_Option::Action  a_act = Catalog_Option::sqlqg_None,
                       const char*      a_opt_type=SQLQG_FUNCTION_OPTION);


  Catalog_Option::Action get_action (void);

  sqlint32 set_mapping_name (sqluint8*  a_mapping_name,
                             sqlint32   a_mapping_name_len);
  sqlint32 get_mapping_name (sqluint8**  a_mapping_name);

  sqlint32 set_local_signature (sqluint8*  a_local_signature,
                                sqlint32   a_local_signature_len);
  sqlint32 get_local_signature (sqluint8**  a_local_signature);

  sqlint32 set_local_schema (sqluint8*  a_local_schema,
                             sqlint32   a_local_schema_len);
  sqlint32 get_local_schema (sqluint8**  a_local_schema);

  void set_funcid (sqlint32  a_funcid);
  sqlint32 get_funcid (sqlint32*  a_funcid);

  sqlint32 set_specific_name (sqluint8*  a_specific_name,
                              sqlint32   a_specific_name_len);
  sqlint32 get_specific_name (sqluint8**  a_specific_name);

  sqlint32 set_definer (sqluint8*  a_definer,
                        sqlint32   a_definer_len);
  sqlint32 get_definer (sqluint8**  a_definer);

  void set_tstamp (void*  a_timestamp);

  sqlint32 get_tstamp (void**  a_timestamp);

  // Methods to insert into the local and remtote parameter lists.
  // These methods do NOT copy the RFunc_Parm_Info object.  After
  // calling them, storage for the RFunc_Parm_Info object becomes the
  // responsibility of the Remote_Function_Info object to which it was
  // added.
  sqlint32 insert_local_parm (RFunc_Parm_Info*  a_new_parm);

  sqlint32 insert_remote_parm (RFunc_Parm_Info*  a_new_parm);

  // Methods to start iterations through the parameter lists
  RFunc_Parm_Info* get_first_local_parm (void);

  RFunc_Parm_Info* get_first_remote_parm (void);

  sqlint32 merge_parms (RFunc_Parm_Info*  a_master_parms,
                        RFunc_Parm_Info*  a_delta_parms);

  sqlint32 merge (Remote_Function_Info*  a_delta_info);

  sqlint32 get_packed_length (void);

  // Modifies buffer pointer and length!
  sqlint32 pack (void**  a_ppbuf,
                 sqlint32*  a_pbuf_len);

  sqlint32 copy (Remote_Function_Info**  a_new_rfunc_info);

private:
  void initialize (void);

  // Private method to add a parameter to a list.  Used by
  // public insert_local_parm and insert_remote_parm methods.
  sqlint32 insert_parm (RFunc_Parm_Info*   a_new_parm,
                        RFunc_Parm_Info**  a_headP,
                        RFunc_Parm_Info**  a_tailP);

  Catalog_Option::Action  action;

  sqluint8*               mapping_name;
  int                     mapping_name_valid;

  sqluint8*               local_signature;
  int                     local_signature_valid;

  sqluint8*               local_schema;
  int                     local_schema_valid;

  sqlint32                funcid;
  int                     funcid_valid;

  sqluint8*               specific_name;
  int                     specific_name_valid;

  sqluint8*               definer;
  int                     definer_valid;

  char                    tstamp[SQLQG_MAX_TIMESTAMP_LEN];
  int                     tstamp_valid;

  RFunc_Parm_Info*        lcl_parm_list_head;
  RFunc_Parm_Info*        lcl_parm_list_tail;

  RFunc_Parm_Info*        rem_parm_list_head;
  RFunc_Parm_Info*        rem_parm_list_tail;

  // Links for the list of Remote_Function_Info objects belonging to a
  // Server_Info object.
  friend class Server_Info;
  friend class V6_7Server_Info;
  Remote_Function_Info*   prev_func;
  Remote_Function_Info*   next_func;

  struct Remote_Function_Info_Hdr {
    sqlint16 action;
    sqlint16 mapping_name_len;
    sqlint16 local_signature_len;
    sqlint16 local_schema_len;
    sqlint32 funcid;
    sqlint16 funcid_valid;
    sqlint16 specific_name_len;
    sqlint16 definer_len;
    char     tstamp[SQLQG_MAX_TIMESTAMP_LEN];
    sqlint16 tstamp_valid;
    sqlint16 n_lcl_parms;
    sqlint16 n_rem_parms;
  };
};

////////////////////////////////
// Local_Type_Info subclass.
////////////////////////////////
class Local_Type_Info : public Catalog_Info {
public:

  // Default constructor.  Clear all "valid" fields
  Local_Type_Info (void);

  // "Unpack" constructor.  Modifies buffer pointer!
  Local_Type_Info (sqlint32*  a_rc,
                   void**     a_ppbuf);

  // "Unpack" constructor.  Modifies buffer pointer!
  Local_Type_Info (sqlint32*  a_rc,
                   void**     a_ppbuf,
                   bool       a_extended);

  // "Reverse" constructor.
  Local_Type_Info (Local_Type* a_local_type);

  // Destructor
  ~Local_Type_Info (void);

  sqlint32 set_action (Catalog_Option::Action  a_action);

  Catalog_Option::Action get_action (void);

  sqlint32 set_mapping_name (sqluint8*  a_mapping_name,
                             sqlint32 a_mapping_name_len);
  sqlint32 get_mapping_name (sqluint8**  a_mapping_name);

  sqlint32 set_local_schema (sqluint8*  a_local_schema,
                             sqlint32   a_local_scema_len);
  sqlint32 get_local_schema (sqluint8**  a_local_schema);

  sqlint32 set_local_name (sqluint8*  a_local_name,
                           sqlint32   a_local_name_len);
  sqlint32 get_local_name (sqluint8**  a_local_name);

  sqlint32 set_typid (sqlint16  a_typid);
  sqlint32 get_typid (sqlint16*  a_typid);

  sqlint32 set_source_typid (sqlint16  a_source_typid);
  sqlint32 get_source_typid (sqlint16*  a_source_typid);

  sqlint32 set_definer (sqluint8*  a_definer,
                        sqlint32   a_definer_len);
  sqlint32 get_definer (sqluint8**  a_definer);

  sqlint32 set_local_length (sqlint32  a_local_length);
  sqlint32 get_local_length (sqlint32*  a_local_length);

  sqlint32 set_local_scale (sqlint16  a_local_scale);
  sqlint32 get_local_scale (sqlint16*  a_local_scale);

  sqlint32 set_local_bit_data (sqluint8  a_local_bit_data);
  sqlint32 get_local_bit_data (sqluint8*  a_local_bit_data);

  sqlint32 set_remote_schema (sqluint8*  a_remote_schema,
                              sqlint32   a_remote_schma_len);
  sqlint32 get_remote_schema (sqluint8**  a_remote_schema);

  sqlint32 set_remote_name (sqluint8*  a_remote_name,
                            sqlint32   a_remote_name_len);
  sqlint32 get_remote_name (sqluint8**  a_remote_name);

  sqlint32 set_remote_meta_type (sqluint8 a_rem_meta_type);
  sqlint32 get_remote_meta_type (sqluint8* a_rem_meta_type);

  sqlint32 set_remote_lower_length (sqlint32 a_rem_lower_length);
  sqlint32 get_remote_lower_length (sqlint32* a_rem_lower_length);

  sqlint32 set_remote_upper_length (sqlint32 a_rem_upper_length);
  sqlint32 get_remote_upper_length (sqlint32* a_rem_upper_length);

  sqlint32 set_remote_lower_scale (sqlint16 a_rem_lower_scale);
  sqlint32 get_remote_lower_scale (sqlint16* a_rem_lower_scale);

  sqlint32 set_remote_upper_scale (sqlint16  a_rem_upper_scale);
  sqlint32 get_remote_upper_scale (sqlint16*  a_rem_upper_scale);

  void reset_remote_lower_length ();
  void reset_remote_upper_length ();
  void reset_remote_lower_scale ();
  void reset_remote_upper_scale ();

  sqlint32 set_remote_s_opr_p (sqluint8*   a_rem_s_opr_p);
  sqlint32 get_remote_s_opr_p (sqluint8**  a_rem_s_opr_p);

  sqlint32 set_remote_bit_data (sqluint8  a_rem_bit_data);
  sqlint32 get_remote_bit_data (sqluint8*  a_rem_bit_data);

  sqlint32 set_tstamp (void*  a_timestamp);
  sqlint32 get_tstamp (void**  a_timestamp);

  sqlint32 set_directionality (sqluint8  a_directionality);
  sqlint32 get_directionality (sqluint8*  a_directionality);

  sqlint32 merge (Local_Type_Info*  a_delta_info);

  sqlint32 get_packed_length ();

  sqlint32 get_packed_length (bool a_extended);

  // Modifies buffer pointer and length!
  sqlint32 pack (void**     a_ppbuf,
                 sqlint32*  a_pbuf_len);

  // Modifies buffer pointer and length!
  sqlint32 pack (void**     a_ppbuf,
                 sqlint32*  a_pbuf_len,
                 bool       a_extended);

  sqlint32 copy (Local_Type_Info**  a_new_ltype_info);

private:
  void initialize (void);

  sqlint32 initialize (void**  a_ppbuf,
                       bool    a_extended);

  Catalog_Option::Action action;

  sqluint8* mapping_name;
  int       mapping_name_valid;

  sqluint8* local_schema;
  int       local_schema_valid;

  sqluint8* local_name;
  int       local_name_valid;

  sqlint16  typid;
  int       typid_valid;

  sqlint16  source_typid;
  int       source_typid_valid;

  sqluint8* definer;
  int       definer_valid;

  sqlint32  local_length;
  int       local_length_valid;

  sqlint16  local_scale;
  int       local_scale_valid;

  sqluint8  local_bit_data;
  int       local_bit_data_valid;

  sqluint8* remote_schema;
  int       remote_schema_valid;

  sqluint8* remote_name;
  int       remote_name_valid;

  sqluint8  remote_meta_type;
  int       remote_meta_type_valid;

  sqlint32  remote_lower_length;
  int       remote_lower_length_valid;

  sqlint32  remote_upper_length;
  int       remote_upper_length_valid;

  sqlint16  remote_lower_scale;
  int       remote_lower_scale_valid;

  sqlint16  remote_upper_scale;
  int       remote_upper_scale_valid;

  sqluint8  remote_s_opr_p[2];
  int       remote_s_opr_p_valid;

  sqluint8  remote_bit_data;
  int       remote_bit_data_valid;

  char      tstamp[SQLQG_MAX_TIMESTAMP_LEN];
  int       tstamp_valid;

  sqluint8  directionality;
  int       directionality_valid;

  // Links for the list of Local_Type_Info objects belonging to a
  // Server_Info object.
  friend class Server_Info;
  Local_Type_Info* prev_ltype;
  Local_Type_Info* next_ltype;

  struct Local_Type_Info_Hdr {
    sqlint16  action;
    sqlint16  mapping_name_len;
    sqlint16  local_schema_len;
    sqlint16  local_name_len;
    sqlint16  definer_len;
    sqlint16  typid;
    sqlint16  typid_valid;
    sqlint16  source_typid;
    sqlint16  source_typid_valid;
    sqlint16  local_length;
    sqlint16  local_length_valid;
    sqlint16  local_scale;
    sqlint16  local_scale_valid;
    sqluint8  local_bit_data;
    sqlint16  local_bit_data_valid;
    sqlint16  remote_schema_len;
    sqlint16  remote_name_len;
    sqluint8  remote_meta_type;
    sqlint16  remote_meta_type_valid;
    sqlint16  remote_lower_length;       
    sqlint16  remote_lower_length_valid;
    sqlint16  remote_upper_length;      
    sqlint16  remote_upper_length_valid;
    sqlint16  remote_lower_scale;
    sqlint16  remote_lower_scale_valid;
    sqlint16  remote_upper_scale;
    sqlint16  remote_upper_scale_valid;
    sqluint8  remote_s_opr_p[2];
    sqlint16  remote_s_opr_p_valid;
    sqluint8  remote_bit_data;
    sqlint16  remote_bit_data_valid;
    char      tstamp[SQLQG_MAX_TIMESTAMP_LEN];
    sqlint16  tstamp_valid;
    sqluint8  directionality;
    sqlint16  directionality_valid;
  };

  struct Local_Type_Info_Hdr_Ex {
    sqlint32  local_length;
    sqlint32  remote_upper_length;
    sqlint32  remote_lower_length;
  };

};

////////////////////////////////
// Server_Info subclass.
////////////////////////////////
class Server_Info : public Catalog_Info {
public:

  // Default constructor.  Clear all "valid" fields
  Server_Info (void);

  // "Unpack" constructor.  Modifies buffer pointer!
  Server_Info (sqlint32*  a_rc, void**  a_ppbuf);

  // "Unpack" constructor.  Modifies buffer pointer!
  Server_Info (sqlint32*  a_rc,
               void**     a_ppbuf,
               bool       a_extended);

  // Constructor given basic info
  Server_Info (sqlint32*  a_rc,
               sqluint8*  a_server_name,    sqlint32  a_name_len,
               sqluint8*  a_server_type,    sqlint32  a_type_len,
               sqluint8*  a_server_version, sqlint32  a_ver_len,
               sqluint8*  a_server_wrapper, sqlint32  a_wrap_len);

  // Destructor
  ~Server_Info (void);

  sqlint32 add_option (sqluint8*  a_opt_name,  sqlint32  a_opt_name_len,
                       sqluint8*  a_opt_value, sqlint32  a_opt_value_len,
                       Catalog_Option::Action  a_act = Catalog_Option::sqlqg_None,
                       const char*      a_opt_type=SQLQG_SERVER_OPTION);

  sqlint32 drop_option (Catalog_Option*  a_option);

  // Add an option value with a specified name, value, timestamp and
  // unique id.
  sqlint32 add_option (sqluint8*  a_opt_name,  sqlint32  a_opt_name_len,
                       sqluint8*  a_opt_value, sqlint32  a_opt_value_len,
                       void*      a_tstamp,
                       sqluint8*  a_uid,       sqlint32  a_uid_len,
                       Catalog_Option::Action  a_act = Catalog_Option::sqlqg_None,
                       const char*      a_opt_type=SQLQG_SERVER_OPTION);

  // Drop the option value with the specified unique id.
  sqlint32 drop_option (Catalog_Option*  a_option,
                        sqluint8*        a_uid,     sqlint32  a_uid_len);

  // Will return SQLQG_ERROR if any of the basic attributes are not
  // valid
  sqlint32 get_basic_info (sqluint8**  a_server_name,
                           sqluint8**  a_server_type,
                           sqluint8**  a_server_version,
                           sqluint8**  a_server_wrapper);

  // Accessor methods.  The "get" methods return 0 if the field
  // contains valid data, or SQLQG_NOVALUE if it doesn't.  The "set"
  // methods can return a "real" error, e.g., if storage allocation
  // fails.
  sqlint32 set_server_name (sqluint8*  a_server_name,
                            sqlint32   a_server_name_len);
  sqlint32 get_server_name (sqluint8**  a_server_name);

  sqlint32 set_server_type (sqluint8*  a_server_type,
                            sqlint32   a_server_type_len);
  sqlint32 get_server_type (sqluint8** a_server_type);

  sqlint32 set_server_version (sqluint8*  a_server_version,
                               sqlint32   a_server_version_len);
  sqlint32 get_server_version (sqluint8** a_server_version);

  sqlint32 set_wrapper_name (sqluint8*  a_wrapper_name,
                             sqlint32   a_wrapper_name_len);
  sqlint32 get_wrapper_name (sqluint8** a_wrapper_name);

  sqlint32 set_authid (sqluint8*  a_authid,
                       sqlint32   a_authid_len);
  sqlint32 get_authid (sqluint8** a_authid);

  sqlint32 set_password (sqluint8*  a_password,
                         sqlint32   a_password_len);
  sqlint32 get_password (sqluint8** a_password);

  // Does NOT copy the Remote_Function_Info object.  After calling
  // this, storage for the Remote_Function_Info object becomes the
  // responsibility of the Server_Info object to which it was added.
  sqlint32 insert_remote_function (Remote_Function_Info* a_new_func_info);

  sqlint32 drop_remote_function (Remote_Function_Info* a_func_info);

  // get_remote_function(void) finds a function mapping specified by name.
  // If the function mapping does not exist, SQLQG_NOVALUE will be
  // returned and fcn will be set to NULL.
  sqlint32 get_remote_function (sqluint8*               a_func_name,
                                sqlint32                a_func_name_len,
                                Remote_Function_Info**  a_func_info);

  Remote_Function_Info* get_first_remote_function (void);
  Remote_Function_Info* get_next_remote_function (
                                Remote_Function_Info* a_cur_func_info);

  // Does NOT copy the Local_Type_Info object.  After calling this,
  // storage for the Local_Type_Info object becomes the responsibility
  // of the Server_Info object to which it was added.
  sqlint32 insert_local_type (Local_Type_Info* a_new_ltyp_info);

  sqlint32 drop_local_type (Local_Type_Info* a_ltype_info);

  // get_local_type(void) returns a type mapping specified by name.  If
  // the type mapping does not exist, SQLQG_NOVALUE will be returned
  // and ltype will be set to NULL.
  sqlint32 get_local_type (sqluint8*          a_ltype_name,
                           sqlint32           a_ltype_name_len,
                           Local_Type_Info**  a_ltype_info);

  Local_Type_Info* get_first_local_type (void);
  Local_Type_Info* get_next_local_type (Local_Type_Info* a_cur_ltype_info);

  // Version to handle Multi_Value Options.  REPLACES current value
  // for matching options.  Returns an error if either a master or a
  // delta option has more than one value.
  sqlint32 merge_mv_options (Server_Info* a_delta_info);

  sqlint32 merge (Server_Info* a_delta_info);

  sqlint32 get_packed_length (void);

  sqlint32 get_packed_length (bool a_extended);

  // Modifies buffer pointer and length!
  sqlint32 pack (void**     a_ppbuf,
                 sqlint32*  a_pbuf_len);

  // Modifies buffer pointer and length!
  sqlint32 pack (void**     a_ppbuf,
                 sqlint32*  a_pbuf_len,
                 bool       a_extended);

  sqlint32 copy (Server_Info** a_new_server_info);

private:
  void initialize (void);

  sqlint32 initialize (void**  a_ppbuf,
                       bool    a_extended);

  // Overrides default implementation from Catalog_Info, because
  // Server_Info uses the Multi_Value_Option subtype of
  // Catalog_Option.  The default implementation assumes single-value
  // options.
  sqlint32 unpack_options (void** a_ppbuf);

  // basic fields.
  sqluint8* name;
  int       name_valid;

  sqluint8* wrapper_name;
  int       wrapper_name_valid;

  sqluint8* type;
  int       type_valid;

  sqluint8* version;
  int       version_valid;

  sqluint8* authid;
  int       authid_valid;

  sqluint8* password;
  int       password_valid;

  // Remote Functions (Funtion Mappings)
  Remote_Function_Info* function_list_head;

  // Local Types (Type Mappings)
  Local_Type_Info* ltype_list_head;

  // Header for packed Server_Info
  struct Server_Info_Hdr {
    sqlint16 name_len;
    sqlint16 wrapper_name_len;
    sqlint16 type_len;
    sqlint16 version_len;
    sqlint16 authid_len;
    sqlint16 password_len;
    sqlint16 n_funcs;
    sqlint16 n_ltypes;
  };

};

////////////////////////////////
// User_Info subclass.
////////////////////////////////
class User_Info : public Catalog_Info {
public:

  // Default constructor.  Clear all "valid" fields
  User_Info (void);

  // "Unpack" constructor.  Modifies buffer pointer!
  User_Info (sqlint32*  a_rc,
             void**     a_ppbuf);

  // Destructor
  ~User_Info (void);

  sqlint32 add_option (sqluint8*  a_opt_name,  sqlint32  a_opt_name_len,
                       sqluint8*  a_opt_value, sqlint32  a_opt_value_len,
                       Catalog_Option::Action  a_act = Catalog_Option::sqlqg_None,
                 const char*      a_opt_type=SQLQG_USER_OPTION);

  // Accessor methods.  The "get" methods return 0 if the field
  // contains valid data, or SQLQG_NOVALUE if it doesn't.  The "set" methods can
  // return a "real" error, e.g., if storage allocation fails.
  sqlint32 set_server_name (sqluint8*  a_server_name,
                            sqlint32   a_server_name_len);
  sqlint32 get_server_name (sqluint8**  a_server_name);

  sqlint32 set_authid (sqluint8*  a_authid,
                       sqlint32   a_authid_len);
  sqlint32 get_authid (sqluint8**  a_authid);

  sqlint32 merge (User_Info* a_delta_info);

  sqlint32 get_packed_length (void);

  // Modifies buffer pointer and length!
  sqlint32 pack (void**     a_ppbuf,
                 sqlint32*  a_pbuf_len);

  sqlint32 copy (User_Info** a_new_user_info);

private:
  void initialize (void);

  sqluint8* authid;
  int       authid_valid;

  sqluint8* server_name;
  int       server_name_valid;

  // Header for packed User_Info
  struct User_Info_Hdr {
    sqlint16 server_name_len;
    sqlint16 authid_len;
  };

};


////////////////////////////////
// Column_Info subclass.
////////////////////////////////
class Column_Info : public Catalog_Info {
public:
  // Constructor.  Clear all "valid" fields
  Column_Info (void);

  // "Unpack" constructor.  Modifies buffer pointer!
  Column_Info (sqlint32*  a_rc,
               void**     a_ppbuf);

  // Destructor
  ~Column_Info (void);

  sqlint32 add_option (sqluint8*  a_opt_name,  sqlint32  a_opt_name_len,
                       sqluint8*  a_opt_value, sqlint32  a_opt_value_len,
                       Catalog_Option::Action  a_act = Catalog_Option::sqlqg_None,
                 const char*      a_opt_type=SQLQG_COLUMN_OPTION);

  // Accessor methods.  The "get" methods return 0 if the field
  // contains valid data, or SQLQG_NOVALUE if it doesn't.  The "set"
  // methods can return a "real" error, e.g., if storage allocation
  // fails.
  sqlint32 set_column_name (sqluint8*  a_column_name,
                            sqlint32   a_column_name_len);
  sqlint32 get_column_name (sqluint8** a_column_name);

  // These should only be used for a Column_Info object built as a
  // result of an ALTER NICKNAME DDL statement that includes an ALTER
  // (or SET) COLUMN clause to rename a column.
  sqlint32 set_new_column_name (sqluint8*  a_new_col_name,
                                sqlint32   a_new_col_name_len);
  sqlint32 get_new_column_name (sqluint8**  a_new_col_name);

  sqlint32 set_column_type (sqluint8*  a_column_type,
                            sqlint32   a_column_type_len);
  sqlint32 get_column_type (sqluint8**  a_column_type);

  void set_for_bit_data (sqluint8  a_for_bit_data);
  sqlint32 get_for_bit_data (sqluint8*  a_for_bit_data);

  void set_column_ID (sqlint16  a_column_ID);
  sqlint32 get_column_ID (sqlint16*  a_column_ID);

  sqlint32 set_type_schema (sqluint8*  a_type_schema,
                            sqlint32   a_type_schema_len);
  sqlint32 get_type_schema (sqluint8**  a_type_schema);

  sqlint32 set_type_name (sqluint8*  a_type_name,
                          sqlint32   a_type_name_len);
  sqlint32 get_type_name (sqluint8**  a_type_name);

  void set_org_length (sqlint32  a_org_length);
  sqlint32 get_org_length (sqlint32*  a_org_length);

  void set_org_scale (sqlint16  a_org_scale);
  sqlint32 get_org_scale (sqlint16*  a_org_scale);

  void set_avg_len (sqlint32  a_avg_len);
  sqlint32 get_avg_len (sqlint32*  a_avg_len);

  sqlint32 set_high2key (sqluint8*  a_high2key,
                         sqlint32   a_high2key_len);
  sqlint32 get_high2key (sqluint8**  a_high2key);

  sqlint32 set_low2key (sqluint8*  a_low2key,
                        sqlint32   a_low2key_len);
  sqlint32 get_low2key (sqluint8**  a_low2key);

  void set_nulls (sqluint8  a_nulls);
  sqlint32 get_nulls (sqluint8*  a_nulls);

  sqlint32 set_default (sqluint8*  a_default,
                        sqlint32   a_default_len);
  sqlint32 get_default (sqluint8**  a_default);

  void set_colcard (sqlint64  a_colcard);
  sqlint32 get_colcard (sqlint64*  a_colcard);

  // Set the SBCS codepage.
  void set_codepage1 (sqlint16  a_codepage1);
  // Get the SBCS codepage.
  sqlint32 get_codepage1 (sqlint16*  a_codepage1);

  // Set the DBCS codepage.
  void set_codepage2 (sqlint16  a_codepage2);
  // Get the DBCS codepage.
  sqlint32 get_codepage2 (sqlint16*  a_codepage2);

  // Set the SBCS and DBCS codepages from a composite codepage.
  sqlint32 set_composite_codepage(
     const sqlint16 a_composite_codepage);

  sqlint32 merge (Column_Info* a_delta_info);

  sqlint32 get_packed_length (void);

  // Modifies buffer pointer and length!
  sqlint32 pack (void**     a_ppbuf,
                 sqlint32*  a_pbuf_len);

  sqlint32 copy (Column_Info** a_new_column_info);

  friend sqlint32 sqlqgGetNicknameInfo (struct sqlrg_pd* a_table_pd,
                                        Nickname_Info**  a_nick_info);

private:
  void initialize (void);

  friend class Nickname_Info; // for access to link field (next_col)
  Column_Info* next_col;

  sqluint8* column_name;            // This is the LOCAL name
  int       column_name_valid;

  sqluint8* new_column_name;        // Meaningful only for ALTER COLUMN clause
  int       new_column_name_valid;

  sqluint8* column_type;            // This is the LOCAL type
  int       column_type_valid;

  sqluint8  for_bit_data;
  int       for_bit_data_valid;

  sqlint16  column_ID;
  int       column_ID_valid;

  sqluint8* type_schema;
  int       type_schema_valid;

  sqluint8* type_name;
  int       type_name_valid;

  sqlint32  org_length;
  int       org_length_valid;

  sqlint16  org_scale;
  int       org_scale_valid;

  sqlint32  avg_len;
  int       avg_len_valid;

  sqluint8* high2key;
  int       high2key_valid;

  sqluint8* low2key;
  int       low2key_valid;

  sqluint8  nulls;
  int       nulls_valid;

  sqluint8* deflt;
  int       default_valid;

  sqlint64  colcard;
  int       colcard_valid;

  sqlint16  codepage1;
  int       codepage1_valid;

  sqlint16  codepage2;
  int       codepage2_valid;

  // Header for packed Column_Info
  struct Column_Info_Hdr {
    sqlint16 column_name_len;
    sqlint16 new_column_name_len;
    sqlint16 column_type_len;
    sqluint8 for_bit_data;
    sqlint16 for_bit_data_valid;
    sqlint16 column_ID;
    sqlint16 column_ID_valid;
    sqlint16 type_schema_len;
    sqlint16 type_name_len;
    sqlint32 org_length;
    sqlint16 org_length_valid;
    sqlint16 org_scale;
    sqlint16 org_scale_valid;
    sqlint32 avg_len;
    sqlint16 avg_len_valid;
    sqlint16 high2key_len;
    sqlint16 low2key_len;
    sqluint8 nulls;
    sqlint16 nulls_valid;
    sqlint16 default_len;
    sqlint64 colcard;
    sqlint16 colcard_valid;
    sqlint16 codepage1;
    sqlint16 codepage1_valid;
    sqlint16 codepage2;
    sqlint16 codepage2_valid;
  };

};

////////////////////////////////
// Index_Info subclass.
////////////////////////////////
class Index_Info : public Catalog_Info {
public:
  // Constructor.  Clear all "valid" fields
  Index_Info (void);

  // "Unpack" constructor.  Modifies buffer pointer!
  Index_Info (sqlint32*  a_rc,
              void**     a_ppbuf);

  // Destructor
  ~Index_Info (void);

  // Accessor methods.  The "get" methods return 0 if the field
  // contains valid data, or SQLQG_NOVALUE if it doesn't.  The "set"
  // methods can return a "real" error, e.g., if storage allocation
  // fails.
  sqlint32 set_index_name (sqluint8*  a_index_name,
                           sqlint32   a_index_name_len);
  sqlint32 get_index_name (sqluint8** a_index_name);

  sqlint32 set_colnames (sqluint8*  a_colnames,
                         sqlint32   a_colnames_len);
  sqlint32 get_colnames (sqluint8**  a_colnames);

  void set_unique_rule (sqluint8  a_unique_rule);
  sqlint32 get_unique_rule (sqluint8*  a_unique_rule);

  void set_colcount (sqlint16  a_colcount);
  sqlint32 get_colcount (sqlint16*  a_colcount);

  void set_unique_colcount (sqlint16  a_unique_colcount);
  sqlint32 get_unique_colcount (sqlint16*  a_unique_colcount);

  void set_nleaf (sqlint32 a_nleaf);
  sqlint32 get_nleaf (sqlint32* a_nleaf);

  void set_nlevels (sqlint16 a_nlevels);
  sqlint32 get_nlevels (sqlint16* a_nlevels);

  void set_first_keycard (sqlint64 a_first_keycard);
  sqlint32 get_first_keycard (sqlint64* a_first_keycard);

  void set_full_keycard (sqlint64 a_full_keycard);
  sqlint32 get_full_keycard (sqlint64* a_full_keycard);

  void set_cluster_ratio (sqlint16 a_cluster_ratio);
  sqlint32 get_cluster_ratio (sqlint16* a_cluster_ratio);

  sqlint32 merge (Index_Info* a_delta_info);

  sqlint32 get_packed_length (void);

  // Modifies buffer pointer and length!
  sqlint32 pack (void**     a_ppbuf,
                 sqlint32*  a_pbuf_len);

  sqlint32 copy (Index_Info** a_new_index_info);

  friend sqlint32 sqlqgGetNicknameInfo (struct sqlrg_pd* a_table_pd,
                                        Nickname_Info**  a_nick_info);

private:
  void initialize (void);

  friend class Nickname_Info; // for access to link field (next_idx)
  Index_Info* next_idx;
  Index_Info* prev_idx;

  sqluint8*   index_name;
  int         index_name_valid;

  sqluint8*   colnames;              // String with +/- prefixes
  int         colnames_valid;

  sqluint8    unique_rule;
  int         unique_rule_valid;

  sqlint16    colcount;
  int         colcount_valid;

  sqlint16    unique_colcount;
  int         unique_colcount_valid;

  sqlint32    nleaf;
  int         nleaf_valid;

  sqlint16    nlevels;
  int         nlevels_valid;

  sqlint64    first_keycard;
  int         first_keycard_valid;

  sqlint64    full_keycard;
  int         full_keycard_valid;

  sqlint16    cluster_ratio;
  int         cluster_ratio_valid;

  // Header for packed Index_Info
  struct Index_Info_Hdr {
    sqlint16 index_name_len;
    sqlint32 colnames_len;
    sqluint8 unique_rule;
    sqlint16 unique_rule_valid;
    sqlint16 colcount;
    sqlint16 colcount_valid;
    sqlint16 unique_colcount;
    sqlint16 unique_colcount_valid;
    sqlint32 nleaf;
    sqlint16 nleaf_valid;
    sqlint16 nlevels;
    sqlint16 nlevels_valid;
    sqlint64 first_keycard;
    sqlint16 first_keycard_valid;
    sqlint64 full_keycard;
    sqlint16 full_keycard_valid;
    sqlint16 cluster_ratio;
    sqlint16 cluster_ratio_valid;
  };

};

////////////////////////////////
// Nickname_Info subclass.
////////////////////////////////
class Nickname_Info : public Catalog_Info {
public:

  // Constructor.  Clear all "valid" fields and list pointers
  Nickname_Info (void);

  // "Unpack" constructor.  Modifies buffer pointer!
  Nickname_Info (sqlint32*  a_rc,
                 void**     a_ppbuf);

  // Destructor.  Will also delete any Column_Info or Index_Info objects
  ~Nickname_Info (void);

  sqlint32 add_option (sqluint8*  a_opt_name,  sqlint32  a_opt_name_len,
                       sqluint8*  a_opt_value, sqlint32  a_opt_value_len,
                       Catalog_Option::Action  a_act = Catalog_Option::sqlqg_None,
                 const char*      a_opt_type=SQLQG_NICKNAME_OPTION);

  // Accessor methods.  The "get" methods return 0 if the field
  // contains valid data, or SQLQG_NOVALUE if it doesn't.  The "set"
  // methods can return a "real" error, e.g., if storage allocation
  // fails.
  sqlint32 set_local_schema (sqluint8*  a_local_schema,
                             sqlint32   a_local_schema_len);
  sqlint32 get_local_schema (sqluint8**  a_local_schema);

  sqlint32 set_nickname (sqluint8*  a_nickname,
                         sqlint32   a_nickname_len);
  sqlint32 get_nickname (sqluint8**  a_nickname);

  sqlint32 set_server_name (sqluint8*  a_server_name,
                            sqlint32   a_server_name_len);
  sqlint32 get_server_name (sqluint8**  a_server_name);

  void set_card (sqlint64  a_card);
  sqlint32 get_card (sqlint64*  a_card);

  void set_npages (sqlint32  a_npages);
  sqlint32 get_npages (sqlint32* a_npages);

  void set_fpages (sqlint32 a_fpages);
  sqlint32 get_fpages (sqlint32* a_fpages);

  void set_overflow (sqlint32 a_overflow);
  sqlint32 get_overflow (sqlint32* a_overflow);

  // Does NOT copy the Column_Info object.  After calling this,
  // storage for the Column_Info object becomes the responsibility of
  // the Nickname_Info object to which it was added.
  sqlint32 insert_column (Column_Info*  a_new_col_info);
  sqlint32 replace_column (Column_Info*  a_new_col_info);
  sqlint16 get_number_columns (void);

  // Methods to iterate through the column list
  Column_Info* get_first_column (void);
  Column_Info* get_next_column (Column_Info* a_cur_col_info);

  // Find the info for a named column.  Returns SQLQG_NOVALUE if
  // column with specified name doesn't exist.
  sqlint32 get_column (sqluint8*      a_col_name,
                       sqlint32       a_col_name_len,
                       Column_Info**  a_col_info);

  // Find the info for the column with the given remote column
  // name.  If none is found, then return SQLQG_NOVALUE.
  sqlint32 get_column_with_remote_column_name (
                       sqluint8*      a_remote_column_name,
                       Column_Info**  a_col_info);

  // Does NOT copy the Index_Info object.  After calling this,
  // storage for the Index_Info object becomes the responsibility of
  // the Nickname_Info object to which it was added.
  sqlint32 insert_index (Index_Info*  a_new_idx_info);
  sqlint32 replace_index (Index_Info*  a_new_idx_info);
  sqlint16 get_number_indexes (void);

  // Methods to iterate through the index list
  Index_Info* get_first_index (void);
  Index_Info* get_next_index (Index_Info* a_cur_idx_info);

  // Find the info for a named index.  Returns SQLQG_NOVALUE if
  // index with specified name doesn't exist.
  sqlint32 get_index (sqluint8*     a_idx_name,
                      sqlint32      a_idx_name_len,
                      Index_Info**  a_idx_info);

  void* get_nickname_pd (void);

  void set_nickname_pd (void* a_new_pd);

  sqlint32 merge (Nickname_Info* a_delta_info);

  sqlint32 get_packed_length (void);

  // Modifies buffer pointer and length!
  sqlint32 pack (void**     a_ppbuf,
                 sqlint32*  a_pbuf_len);

  sqlint32 copy (Nickname_Info** a_new_idx_info);

  friend sqlint32 sqlqgGetNicknameInfo (struct sqlrg_pd* a_table_pd,
                                        Nickname_Info** a_nick_info);

  sqlqg_FMP_Runtime_Object* get_m_agt_runtime_obj (void);

  void set_m_agt_runtime_obj (sqlqg_FMP_Runtime_Object* a_agt_runtime_obj);

  sqlqg_FMP_Runtime_Object* get_m_stp_runtime_obj (void);

  void set_m_stp_runtime_obj (sqlqg_FMP_Runtime_Object* a_stp_runtime_obj);

private:

  void initialize (void);

  void* pd;

  sqlint16      n_columns;
  Column_Info*  column_list_head;
  Column_Info*  column_list_tail;

  sqlint16      n_indexes;
  Index_Info*   index_list_head;

  sqluint8*     schema;
  int           schema_valid;

  sqluint8*     nickname;
  int           nickname_valid;

  sqluint8*     server_name;
  int           server_name_valid;

  sqlint64      card;
  int           card_valid;

  sqlint32      npages;
  int           npages_valid;

  sqlint32      fpages;
  int           fpages_valid;

  sqlint32      overflow;
  int           overflow_valid;

  // Header for packed Nickname_Info
  struct Nickname_Info_Hdr {
    sqlint16 schema_len;
    sqlint16 nickname_len;
    sqlint16 server_name_len;
    sqlint64 card;
    sqlint16 card_valid;
    sqlint32 npages;
    sqlint16 npages_valid;
    sqlint32 fpages;
    sqlint16 fpages_valid;
    sqlint32 overflow;
    sqlint16 overflow_valid;
    sqlint16 n_columns;
    sqlint16 n_indexes;
  };

  sqlqg_FMP_Runtime_Object* m_agt_runtime_obj;
  sqlqg_FMP_Runtime_Object* m_stp_runtime_obj;
};

////////////////////////////////
// Procparm_Info subclass.
////////////////////////////////
class Procparm_Info : public Catalog_Info {
public:
  // Constructor.  Clear all "valid" fields
  Procparm_Info (void);

  // "Unpack" constructor.  Modifies buffer pointer!
  Procparm_Info (sqlint32*  a_rc,
                 void**     a_ppbuf);

  // Destructor
  ~Procparm_Info (void);

  // Accessor methods.  The "get" methods return 0 if the field
  // contains valid data, or SQLQG_NOVALUE if it doesn't.  The "set"
  // methods can return a "real" error, e.g., if storage allocation
  // fails.
  sqlint32 set_parm_name (sqluint8*  a_parm_name,
                          sqlint32   a_parm_name_len);
  sqlint32 get_parm_name (sqluint8** a_parm_name);

  // Set IN, OUT or INOUT
  sqlint32 set_parm_mode (sqluint8*  a_parm_mode,
                          sqlint32   a_parm_mode_len);
  sqlint32 get_parm_type (sqluint8**  a_parm_mode);

  void set_for_bit_data (sqluint8 a_for_bit_data);
  sqlint32 get_for_bit_data (sqluint8* a_for_bit_data);

  void set_remote_for_bit_data (sqluint8 a_rem_for_bit_data);
  sqlint32 get_remote_for_bit_data (sqluint8* a_rem_for_bit_data);

  sqlint32 set_type_schema (sqluint8*  a_type_schema,
                            sqlint32   a_type_schema_len);
  sqlint32 get_type_schema (sqluint8**  a_type_schema);

  sqlint32 set_type_name (sqluint8*  a_type_name,
                          sqlint32   a_type_name_len);
  sqlint32 get_type_name (sqluint8**  a_type_name);

  void set_org_length (sqlint32 a_org_length);
  sqlint32 get_org_length (sqlint32* a_org_length);

  void set_org_scale (sqlint16 a_org_scale);
  sqlint32 get_org_scale (sqlint16* a_org_scale);

  void set_nulls (sqluint8 a_nulls);
  sqlint32 get_nulls (sqluint8* a_nulls);

  void set_remote_nulls (sqluint8 a_rem_nulls);
  sqlint32 get_remote_nulls (sqluint8* a_rem_nulls);

  void set_remote_length (sqlint16 a_rem_length);
  sqlint32 get_remote_length (sqlint16* a_rem_length);

  void set_remote_scale (sqlint16 a_rem_scale);
  sqlint32 get_remote_scale (sqlint16* a_rem_scale);

  sqlint32 set_remote_type_schema (sqluint8*  a_rem_type_schema,
                                   sqlint32   a_rem_type_schema_len);
  sqlint32 get_remote_type_schema (sqluint8**  a_rem_type_schema);

  sqlint32 set_remote_type_name (sqluint8*  a_rem_type_name,
                                 sqlint32   a_rem_type_name_len);
  sqlint32 get_remote_type_name (sqluint8**  a_rem_type_name);

  void set_codepage1 (sqlint16 a_codepage1);
  sqlint32 get_codepage1 (sqlint16* a_codepage1);

  void set_codepage2 (sqlint16 a_codepage2);
  sqlint32 get_codepage2 (sqlint16* a_codepage2);

  sqlint32 get_packed_length (void);

  // Modifies buffer pointer and length!
  sqlint32 pack (void**     a_ppbuf,
                 sqlint32*  a_pbuf_len);

  sqlint32 copy (Procparm_Info** a_new_procparm_info);

private:
  void initialize (void);

  friend class Procedure_Info;    // for access to link field (next_parm)
  Procparm_Info*  next_parm;

  sqluint8*       parm_name;      // This is the LOCAL name
  int             parm_name_valid;

  sqluint8*       parm_mode;
  int             parm_mode_valid;

  sqluint8        for_bit_data;
  int             for_bit_data_valid;

  sqluint8        remote_for_bit_data;
  int             remote_for_bit_data_valid;

  sqluint8*       type_schema;
  int             type_schema_valid;

  sqluint8*       type_name;
  int             type_name_valid;

  sqluint8*       remote_type_schema;
  int             remote_type_schema_valid;

  sqluint8*       remote_type_name;
  int             remote_type_name_valid;

  sqlint32        org_length;
  int             org_length_valid;

  sqlint16        org_scale;
  int             org_scale_valid;

  sqlint16        remote_length;
  int             remote_length_valid;

  sqlint16        remote_scale;
  int             remote_scale_valid;

  sqluint8        remote_nulls;
  int             remote_nulls_valid;

  sqluint8        nulls;
  int             nulls_valid;

  sqlint16        codepage1;
  int             codepage1_valid;

  sqlint16        codepage2;
  int             codepage2_valid;

  // Header for packed Procparm_Info
  struct Procparm_Info_Hdr {
    sqlint16 parm_name_len;
    sqlint16 parm_mode_len;
    sqluint8 for_bit_data;
    sqlint16 for_bit_data_valid;
    sqluint8 remote_for_bit_data;
    sqlint16 remote_for_bit_data_valid;
    sqlint16 type_schema_len;
    sqlint16 type_name_len;
    sqlint16 remote_type_schema_len;
    sqlint16 remote_type_name_len;
    sqlint32 org_length;
    sqlint16 org_length_valid;
    sqlint16 org_scale;
    sqlint16 org_scale_valid;
    sqluint8 nulls;
    sqlint16 nulls_valid;
    sqlint16 remote_length;
    sqlint16 remote_length_valid;
    sqlint16 remote_scale;
    sqlint16 remote_scale_valid;
    sqluint8 remote_nulls;
    sqlint16 remote_nulls_valid;
    sqlint16 codepage1;
    sqlint16 codepage1_valid;
    sqlint16 codepage2;
    sqlint16 codepage2_valid;
  };

};

////////////////////////////////
// Procedure_Info subclass.
////////////////////////////////
class Procedure_Info : public Catalog_Info {
public:

  // Constructor.  Clear all "valid" fields and list pointers
  Procedure_Info (void);

  // "Unpack" constructor.  Modifies buffer pointer!
  Procedure_Info (sqlint32*  a_rc,
                  void**     a_ppbuf);

  // Destructor.  Will also delete any Procparm_Info
  ~Procedure_Info (void);

  // Accessor methods.  The "get" methods return 0 if the field
  // contains valid data, or SQLQG_NOVALUE if it doesn't.  The "set"
  // methods can return a "real" error, e.g., if storage allocation
  // fails.
  sqlint32 set_local_schema (sqluint8*  a_local_schema,
                             sqlint32   a_local_schema_len);
  sqlint32 get_local_schema (sqluint8**  a_local_schema);

  sqlint32 set_nickname (sqluint8*  a_nickname,
                         sqlint32  a_nickname_len);
  sqlint32 get_nickname (sqluint8** a_nickname);

  // Does NOT copy the Procparm_Info object.  After calling this,
  // storage for the Procparm_Info object becomes the responsibility of
  // the Procedure_Info object to which it was added.
  sqlint32 insert_parm (Procparm_Info* a_new_procparm_info);
  sqlint16 get_number_parms (void);

  // Methods to iterate through the parm list
  Procparm_Info* get_first_parm (void);
  Procparm_Info* get_next_parm (Procparm_Info* a_cur_procparm_info);

  sqlint32 get_packed_length (void);

  // Modifies buffer pointer and length!
  sqlint32 pack (void**     a_ppbuf,
                 sqlint32*  a_pbuf_len);

  sqlint32 copy (Procedure_Info**  a_new_procparm_info);

private:

  void initialize (void);

  sqlint16        n_parms;
  Procparm_Info*  parm_list_head;
  Procparm_Info*  parm_list_tail;

  sqluint8*       schema;
  int             schema_valid;

  sqluint8*       nickname;
  int             nickname_valid;

  struct Procedure_Info_Hdr {
    sqlint16 schema_len;
    sqlint16 nickname_len;
    sqlint16 n_parms;
  };

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Inline methods
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Option_Value
////////////////////////////////////////////////////////////////////////////////
inline sqluint8* Option_Value::get_value (void) { return value; }
inline void* Option_Value::get_tstamp (void) { return tstamp; }
inline sqluint8* Option_Value::get_uid (void) { return uid; }

////////////////////////////////////////////////////////////////////////////////
// Catalog_Option
////////////////////////////////////////////////////////////////////////////////
inline Catalog_Option::Catalog_Option (void) { next_opt = prev_opt = NULL; }
inline sqluint8* Catalog_Option::get_name (void) { return opt_name; }
inline Catalog_Option::Action Catalog_Option::get_action (void)
    { return action; }

////////////////////////////////////////////////////////////////////////////////
// Single_Value_Option
////////////////////////////////////////////////////////////////////////////////
inline sqluint8* Single_Value_Option::get_value (void) { return opt_value; }
inline sqluint8* Single_Value_Option::get_value (sqlint32*  a_length)
     {*a_length = value_length; return opt_value; }
inline sqlint16 Single_Value_Option::n_values (void)  { return (sqlint32)1; }

////////////////////////////////////////////////////////////////////////////////
// Multi_Value_Option
////////////////////////////////////////////////////////////////////////////////
inline sqluint8* Multi_Value_Option::get_value (sqlint32*  a_length)
     {*a_length=0;return NULL;} ;
inline sqlint16 Multi_Value_Option::n_values (void)
     { return num_values; };
inline Option_Value* Multi_Value_Option::get_first_value (void)
     { return val_chain; };
inline Option_Value* Multi_Value_Option::get_next_value (Option_Value*  a_current_value)
     { return a_current_value->next_val; };
inline void Multi_Value_Option::initialize (void)
     { num_values = 0; val_chain = NULL; };

////////////////////////////////////////////////////////////////////////////////
// Catalog_Info
////////////////////////////////////////////////////////////////////////////////
inline Catalog_Info::Catalog_Info (void): opt_chain (NULL) { }
inline Catalog_Option* Catalog_Info::get_first_option (void)
     { return opt_chain; };
inline Catalog_Option* Catalog_Info:: get_next_option (Catalog_Option*  a_current_option)
     { return a_current_option->next_opt; }
inline sqlint32
Catalog_Info::get_string_if_valid (int         a_flag,
                                   sqluint8*   a_src,
                                   sqluint8**  a_target)
{
  return a_flag ? (*a_target = a_src, 0) : (*a_target = NULL, SQLQG_NOVALUE);
}
inline void
Catalog_Info::copy_if_valid (sqlint32*   a_rc,
                             sqluint8*   a_src,
                             int         a_sflag,
                             sqluint8**  a_target,
                             int*        a_tflag)
{
  if ((*a_rc == 0) && a_sflag)
  {
     *a_rc = copy_set_valid_and_terminate (a_src,
                                           strlen((char* ) a_src),
                                           a_target,
                                           a_tflag);
  }
}
inline void
Catalog_Info::copy_if_valid (sqlint32   a_src,
                             int        a_sflag,
                             sqlint32*  a_target,
                             int*       a_tflag)
{
  if (a_sflag)
  {
    *a_tflag = a_sflag;
    *a_target = a_src;
  }
}
inline void
Catalog_Info::copy_if_valid (sqlint16   a_src,
                             int        a_sflag,
                             sqlint16*  a_target,
                             int*       a_tflag)
{
  if (a_sflag)
  {
    *a_tflag = a_sflag;
    *a_target = a_src;
  }
}
inline void
Catalog_Info::copy_if_valid (sqluint8   a_src,
                             int        a_sflag,
                             sqluint8*  a_target,
                             int*       a_tflag)
{
  if (a_sflag)
  {
    *a_tflag = a_sflag;
    *a_target = a_src;
  }
}
inline void
Catalog_Info::copy_if_valid (sqlint64   a_src,
                             int        a_sflag,
                             sqlint64*  a_target,
                             int*       a_tflag)
{
  if (a_sflag)
  {
    *a_tflag = a_sflag;
    *a_target = a_src;
  }
}

////////////////////////////////////////////////////////////////////////////////
// Wrapper_Info
////////////////////////////////////////////////////////////////////////////////
inline Wrapper_Info::Wrapper_Info (void) { initialize (); }
inline sqlint32
Wrapper_Info::set_wrapper_name (sqluint8*  a_name,
                                sqlint32   a_name_len)
{
  return copy_set_valid_and_terminate (a_name,
                                       a_name_len,
                                       &name,
                                       &name_valid);
}
inline sqlint32
Wrapper_Info::get_wrapper_name (sqluint8**  a_name)
{
  return get_string_if_valid (name_valid,
                              name,
                              a_name);
}
inline sqlint32
Wrapper_Info::set_corelib (sqluint8* a_lib_name,
                           sqlint32  a_lib_name_len)
{
  return copy_set_valid_and_terminate (a_lib_name,
                                       a_lib_name_len,
                                       &corelib,
                                       &corelib_valid);
}
inline sqlint32
Wrapper_Info::get_corelib (sqluint8** a_lib_name)
{
  return get_string_if_valid (corelib_valid,
                              corelib,
                              a_lib_name);
}
inline void
Wrapper_Info::set_type (sqluint8 a_wrapper_type)
{
  type = a_wrapper_type;
  type_valid = TRUE;
}
inline sqlint32
Wrapper_Info::get_type (sqluint8* a_wrapper_type)
{
  return type_valid ? *a_wrapper_type = type, 0 : SQLQG_NOVALUE;
}
inline void
Wrapper_Info::set_version (sqlint32 a_wrapper_version)
{
  version = a_wrapper_version;
  version_valid = TRUE;
}
inline sqlint32
Wrapper_Info::get_version (sqlint32* a_wrapper_version)
{
  return version_valid ? *a_wrapper_version = version, 0 : SQLQG_NOVALUE;
}
inline void
Wrapper_Info::initialize (void)
{
  name_valid = corelib_valid = type_valid = version_valid = FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// RFunc_Parm_Info
////////////////////////////////////////////////////////////////////////////////
inline RFunc_Parm_Info::RFunc_Parm_Info (void) { initialize(); }
inline RFunc_Parm_Info::RFunc_Parm_Info (sqlint16  a_ordinal)
  { initialize(); ordinal_valid = TRUE; ordinal = a_ordinal; }
inline RFunc_Parm_Info::~RFunc_Parm_Info (void) {}
inline void RFunc_Parm_Info::set_ordinal (sqlint16  a_ordinal)
  { ordinal = a_ordinal; ordinal_valid = TRUE; return; }
inline sqlint32
RFunc_Parm_Info::get_ordinal (sqlint16*  a_ordinal)
  { return ordinal_valid ? *a_ordinal = ordinal, 0 : SQLQG_NOVALUE; }
inline RFunc_Parm_Info*
RFunc_Parm_Info::get_next_parm (void)
  { return next_parm; }
inline sqlint32
RFunc_Parm_Info::get_packed_length (void)
  { return sizeof (struct RFunc_Parm_Info_Hdr) + get_packed_option_length (); }
inline void RFunc_Parm_Info::initialize (void)
  { ordinal_valid = FALSE; next_parm = NULL; }

////////////////////////////////////////////////////////////////////////////////
// Remote_Function_Info
////////////////////////////////////////////////////////////////////////////////
inline Remote_Function_Info::Remote_Function_Info (void) { initialize(); }
inline void
Remote_Function_Info::set_action (Catalog_Option::Action  a_act)
  { action = a_act; }
inline Catalog_Option::Action
Remote_Function_Info::get_action (void)
  { return action; }
inline sqlint32
Remote_Function_Info::set_mapping_name (sqluint8*  a_mapping_name,
                                        sqlint32   a_mapping_name_len)
  { return copy_set_valid_and_terminate (a_mapping_name,
                                         a_mapping_name_len,
                                         &mapping_name,
                                         &mapping_name_valid); }
inline sqlint32
Remote_Function_Info::get_mapping_name (sqluint8**  a_mapping_name)
  { return get_string_if_valid (mapping_name_valid,
                                mapping_name,
                                a_mapping_name); }
inline sqlint32
Remote_Function_Info::set_local_signature (sqluint8*  a_local_signature,
                                           sqlint32   a_local_signature_len)
  { return copy_set_valid_and_terminate (a_local_signature,
                                         a_local_signature_len,
                                         &local_signature,
                                         &local_signature_valid); }
inline sqlint32
Remote_Function_Info::get_local_signature (sqluint8**  a_local_signature)
  { return get_string_if_valid (local_signature_valid,
                                local_signature,
                                a_local_signature); }
inline sqlint32
Remote_Function_Info::set_local_schema (sqluint8*  a_local_schema,
                                        sqlint32   a_local_schema_len)
  { return copy_set_valid_and_terminate (a_local_schema,
                                         a_local_schema_len,
                                         &local_schema,
                                         &local_schema_valid); }
inline sqlint32
Remote_Function_Info::get_local_schema (sqluint8**  a_local_schema)
  { return get_string_if_valid (local_schema_valid,
                                local_schema,
                                a_local_schema); }
inline void
Remote_Function_Info::set_funcid (sqlint32  a_funcid)
  { funcid_valid = TRUE; funcid = a_funcid; return; }
inline sqlint32
Remote_Function_Info::get_funcid (sqlint32*  a_funcid)
  { return funcid_valid ?*a_funcid = funcid, 0 : SQLQG_NOVALUE; }
inline sqlint32
Remote_Function_Info::set_specific_name (sqluint8*  a_specific_name,
                                         sqlint32   a_specific_name_len)
  { return copy_set_valid_and_terminate (a_specific_name,
                                         a_specific_name_len,
                                         &specific_name,
                                         &specific_name_valid); }
inline sqlint32
Remote_Function_Info::get_specific_name (sqluint8**  a_specific_name)
  { return get_string_if_valid (specific_name_valid,
                                specific_name,
                                a_specific_name); }
inline sqlint32
Remote_Function_Info::set_definer (sqluint8*  a_definer,
                                   sqlint32   a_definer_len)
  { return copy_set_valid_and_terminate (a_definer,
                                         a_definer_len,
                                         &definer,
                                         &definer_valid); }
inline sqlint32
Remote_Function_Info::get_definer (sqluint8**  a_definer)
  { return get_string_if_valid (definer_valid,
                                definer,
                                a_definer); }
inline sqlint32
Remote_Function_Info::get_tstamp (void**  a_timestamp)
  { return tstamp_valid ? *a_timestamp = tstamp, 0 : SQLQG_NOVALUE; }

inline sqlint32
Remote_Function_Info::insert_local_parm (RFunc_Parm_Info*  a_new_parm)
  { return insert_parm (a_new_parm,
                        &lcl_parm_list_head,
                        &lcl_parm_list_tail); }
inline sqlint32
Remote_Function_Info::insert_remote_parm (RFunc_Parm_Info*  a_new_parm)
  { return insert_parm (a_new_parm,
                        &rem_parm_list_head,
                        &rem_parm_list_tail); }
inline RFunc_Parm_Info*
Remote_Function_Info::get_first_local_parm (void)
  { return lcl_parm_list_head; }
inline RFunc_Parm_Info*
Remote_Function_Info::get_first_remote_parm (void)
  { return rem_parm_list_head; }
inline void
Remote_Function_Info::initialize (void)
  {
    action = Catalog_Option::sqlqg_None;
    mapping_name_valid = local_signature_valid = FALSE;
    local_schema_valid = specific_name_valid = FALSE;
    definer_valid = funcid_valid = tstamp_valid = FALSE;
    lcl_parm_list_head = lcl_parm_list_tail = NULL;
    rem_parm_list_head = rem_parm_list_tail = NULL;
    next_func = prev_func = NULL;
  }

////////////////////////////////////////////////////////////////////////////////
// Remote_Function_Info
////////////////////////////////////////////////////////////////////////////////
inline Local_Type_Info::Local_Type_Info (void) { initialize(); }
inline sqlint32
Local_Type_Info::set_action (Catalog_Option::Action  a_action)
  { action = a_action; return(0); }
inline Catalog_Option::Action
Local_Type_Info::get_action (void) { return action; }
inline sqlint32
Local_Type_Info::set_mapping_name (sqluint8*  a_mapping_name,
                                   sqlint32 a_mapping_name_len)
  { return copy_set_valid_and_terminate (a_mapping_name,
                                         a_mapping_name_len,
                                         &mapping_name,
                                         &mapping_name_valid); }
inline sqlint32
Local_Type_Info::get_mapping_name (sqluint8**  a_mapping_name)
  { return get_string_if_valid (mapping_name_valid,
                                mapping_name,
                                a_mapping_name); }
inline sqlint32
Local_Type_Info::set_local_schema (sqluint8*  a_local_schema,
                                   sqlint32   a_local_scema_len)
  { return copy_set_valid_and_terminate (a_local_schema,
                                         a_local_scema_len,
                                         &local_schema,
                                         &local_schema_valid); }
inline sqlint32
Local_Type_Info::get_local_schema (sqluint8**  a_local_schema)
  { return get_string_if_valid (local_schema_valid,
                                local_schema,
                                a_local_schema); }
inline sqlint32
Local_Type_Info::set_local_name (sqluint8*  a_local_name,
                                 sqlint32   a_local_name_len)
  { return copy_set_valid_and_terminate (a_local_name,
                                         a_local_name_len,
                                         &local_name,
                                         &local_name_valid); }
inline sqlint32
Local_Type_Info::get_local_name (sqluint8**  a_local_name)
  { return get_string_if_valid (local_name_valid,
                                local_name,
                                a_local_name); }
inline sqlint32
Local_Type_Info::set_typid (sqlint16  a_typid)
  { typid_valid = TRUE; typid = a_typid; return(0); }
inline sqlint32
Local_Type_Info::get_typid (sqlint16*  a_typid)
  { return typid_valid ?*a_typid = typid, 0 : SQLQG_NOVALUE; }
inline sqlint32
Local_Type_Info::set_source_typid (sqlint16  a_source_typid)
  { source_typid_valid = TRUE; source_typid = a_source_typid; return(0); }
inline sqlint32
Local_Type_Info::get_source_typid (sqlint16*  a_source_typid)
  { return source_typid_valid ?*a_source_typid = source_typid, 0 : SQLQG_NOVALUE; }
inline sqlint32
Local_Type_Info::set_definer (sqluint8*  a_definer,
                              sqlint32   a_definer_len)
  { return copy_set_valid_and_terminate (a_definer,
                                         a_definer_len,
                                         &definer,
                                         &definer_valid); }
inline sqlint32
Local_Type_Info::get_definer (sqluint8**  a_definer)
  { return get_string_if_valid (definer_valid,
                                definer,
                                a_definer); }
inline sqlint32
Local_Type_Info::set_local_length (sqlint32  a_local_length)
  { local_length_valid = TRUE; local_length = a_local_length; return(0); }
inline sqlint32
Local_Type_Info::get_local_length (sqlint32*  a_local_length)
  { return local_length_valid ? *a_local_length = local_length, 0 : SQLQG_NOVALUE; }
inline sqlint32
Local_Type_Info::set_local_scale (sqlint16  a_local_scale)
  { local_scale_valid = TRUE; local_scale = a_local_scale; return(0); }
inline sqlint32
Local_Type_Info::get_local_scale (sqlint16*  a_local_scale)
  { return local_scale_valid ? *a_local_scale = local_scale, 0 : SQLQG_NOVALUE; }
inline sqlint32
Local_Type_Info::set_local_bit_data (sqluint8  a_local_bit_data)
  { local_bit_data_valid = TRUE; local_bit_data = a_local_bit_data; return(0); }
inline sqlint32
Local_Type_Info::get_local_bit_data (sqluint8*  a_local_bit_data)
  { return local_bit_data_valid ? *a_local_bit_data = local_bit_data, 0 : SQLQG_NOVALUE; }
inline sqlint32
Local_Type_Info::set_remote_schema (sqluint8*  a_remote_schema,
                                    sqlint32   a_remote_schma_len)
  { return copy_set_valid_and_terminate (a_remote_schema,
                                         a_remote_schma_len,
                                         &remote_schema,
                                         &remote_schema_valid); }
inline sqlint32
Local_Type_Info::get_remote_schema (sqluint8**  a_remote_schema)
  { return get_string_if_valid (remote_schema_valid,
                                remote_schema,
                                a_remote_schema); }
inline sqlint32
Local_Type_Info::set_remote_name (sqluint8*  a_remote_name,
                                  sqlint32   a_remote_name_len)
  { return copy_set_valid_and_terminate (a_remote_name,
                                         a_remote_name_len,
                                         &remote_name,
                                         &remote_name_valid); }
inline sqlint32
Local_Type_Info::get_remote_name (sqluint8**  a_remote_name)
  { return get_string_if_valid (remote_name_valid,
                                remote_name,
                                a_remote_name); }
inline sqlint32
Local_Type_Info::set_remote_meta_type (sqluint8 a_rem_meta_type)
  { remote_meta_type_valid = TRUE; remote_meta_type = a_rem_meta_type; return(0); }
inline sqlint32
Local_Type_Info::get_remote_meta_type (sqluint8* a_rem_meta_type)
  { return remote_meta_type_valid ? *a_rem_meta_type = remote_meta_type, 0 : SQLQG_NOVALUE; }
inline sqlint32
Local_Type_Info::set_remote_lower_length (sqlint32 a_rem_lower_length)
  { remote_lower_length_valid = TRUE; remote_lower_length = a_rem_lower_length; return(0); }
inline sqlint32
Local_Type_Info::get_remote_lower_length (sqlint32* a_rem_lower_length)
  { return remote_lower_length_valid ? *a_rem_lower_length = remote_lower_length, 0 : SQLQG_NOVALUE; }
inline sqlint32
Local_Type_Info::set_remote_upper_length (sqlint32 a_rem_upper_length)
  { remote_upper_length_valid = TRUE; remote_upper_length = a_rem_upper_length; return(0); }
inline sqlint32
Local_Type_Info::get_remote_upper_length (sqlint32* a_rem_upper_length)
  { return remote_upper_length_valid ? *a_rem_upper_length = remote_upper_length, 0 : SQLQG_NOVALUE; }
inline sqlint32
Local_Type_Info::set_remote_lower_scale (sqlint16 a_rem_lower_scale)
  { remote_lower_scale_valid = TRUE; remote_lower_scale = a_rem_lower_scale; return(0); }
inline sqlint32
Local_Type_Info::get_remote_lower_scale (sqlint16* a_rem_lower_scale)
  { return remote_lower_scale_valid ? *a_rem_lower_scale = remote_lower_scale, 0 : SQLQG_NOVALUE; }
inline sqlint32
Local_Type_Info::set_remote_upper_scale (sqlint16  a_rem_upper_scale)
  { remote_upper_scale_valid = TRUE; remote_upper_scale = a_rem_upper_scale; return(0); }
inline sqlint32
Local_Type_Info::get_remote_upper_scale (sqlint16*  a_rem_upper_scale)
  { return remote_upper_scale_valid ? *a_rem_upper_scale = remote_upper_scale, 0 : SQLQG_NOVALUE; }
inline void
Local_Type_Info::reset_remote_lower_length ()
  { remote_lower_length_valid = FALSE; remote_lower_length = 0; }
inline void
Local_Type_Info::reset_remote_upper_length ()
  { remote_upper_length_valid = FALSE; remote_upper_length = 0; }
inline void
Local_Type_Info::reset_remote_lower_scale ()
  { remote_lower_scale_valid = FALSE; remote_lower_scale = 0; }
inline void
Local_Type_Info::reset_remote_upper_scale ()
  { remote_upper_scale_valid = FALSE; remote_upper_scale = 0; }
inline sqlint32
Local_Type_Info::get_remote_s_opr_p (sqluint8**  a_rem_s_opr_p)
  { return remote_s_opr_p_valid ? *a_rem_s_opr_p = remote_s_opr_p, 0 : SQLQG_NOVALUE; }
inline sqlint32
Local_Type_Info::set_remote_bit_data (sqluint8  a_rem_bit_data)
  { remote_bit_data_valid = TRUE; remote_bit_data =  a_rem_bit_data; return(0); }
inline sqlint32
Local_Type_Info::get_remote_bit_data (sqluint8*  a_rem_bit_data)
  { return remote_bit_data_valid ? * a_rem_bit_data = remote_bit_data, 0 : SQLQG_NOVALUE; }
inline sqlint32
Local_Type_Info::get_tstamp (void**  a_timestamp)
  { return tstamp_valid ? *a_timestamp = tstamp, 0 : SQLQG_NOVALUE; }

inline sqlint32
Local_Type_Info::set_directionality (sqluint8  a_directionality)
  { directionality_valid = TRUE; directionality = a_directionality; return(0); }
inline sqlint32
Local_Type_Info::get_directionality (sqluint8*  a_directionality)
  { return directionality_valid ? *a_directionality = directionality, 0: SQLQG_NOVALUE; }
inline void
Local_Type_Info::initialize (void)
  {
    action = Catalog_Option::sqlqg_None;
    mapping_name_valid = definer_valid = tstamp_valid = FALSE;
    local_schema_valid = local_name_valid = FALSE;
    typid_valid = source_typid_valid = FALSE;
    local_length_valid = local_scale_valid = local_bit_data_valid = FALSE;
    remote_schema_valid = remote_name_valid = remote_meta_type_valid = FALSE;
    remote_lower_length_valid = remote_upper_length_valid = FALSE;
    remote_lower_scale_valid = remote_upper_scale_valid = FALSE;
    remote_s_opr_p_valid = remote_bit_data_valid = FALSE;
    directionality_valid = FALSE;
  }

////////////////////////////////////////////////////////////////////////////////
// Server_Info
////////////////////////////////////////////////////////////////////////////////
inline Server_Info::Server_Info (void) { initialize(); }
inline sqlint32
Server_Info::set_server_name (sqluint8*  a_server_name,
                              sqlint32   a_server_name_len)
  { return copy_set_valid_and_terminate (a_server_name,
                                         a_server_name_len,
                                         &name,
                                         &name_valid); }
inline sqlint32
Server_Info::get_server_name (sqluint8**  a_server_name)
  { return get_string_if_valid (name_valid,
                                name,
                                a_server_name); }

inline sqlint32
Server_Info::set_server_type (sqluint8*  a_server_type,
                              sqlint32   a_server_type_len)
  { return copy_set_valid_and_terminate (a_server_type,
                                         a_server_type_len,
                                         &type,
                                         &type_valid); }
inline sqlint32
Server_Info::get_server_type (sqluint8** a_server_type)
  { return get_string_if_valid (type_valid,
                                type,
                                a_server_type); }

inline sqlint32
Server_Info::set_server_version (sqluint8*  a_server_version,
                                 sqlint32   a_server_version_len)
  { return copy_set_valid_and_terminate (a_server_version,
                                         a_server_version_len,
                                         &version,
                                         &version_valid); }
inline sqlint32
Server_Info::get_server_version (sqluint8** a_server_version)
  { return get_string_if_valid (version_valid,
                                version,
                                a_server_version); }

inline sqlint32
Server_Info::set_wrapper_name (sqluint8*  a_wrapper_name,
                               sqlint32   a_wrapper_name_len)
  { return copy_set_valid_and_terminate (a_wrapper_name,
                                         a_wrapper_name_len,
                                         &wrapper_name,
                                         &wrapper_name_valid); }
inline sqlint32
Server_Info::get_wrapper_name (sqluint8** a_wrapper_name)
  { return get_string_if_valid (wrapper_name_valid,
                                wrapper_name,
                                a_wrapper_name); }

inline sqlint32
Server_Info::set_authid (sqluint8*  a_authid,
                         sqlint32   a_authid_len)
  { return copy_set_valid_and_terminate (a_authid,
                                         a_authid_len,
                                         &authid,
                                         &authid_valid); }
inline sqlint32
Server_Info::get_authid (sqluint8** a_authid)
  { return get_string_if_valid (authid_valid,
                                authid,
                                a_authid); }

inline sqlint32
Server_Info::set_password (sqluint8*  a_password,
                           sqlint32   a_password_len)
  { return copy_set_valid_and_terminate (a_password,
                                         a_password_len,
                                         &password,
                                         &password_valid); }
inline sqlint32
Server_Info::get_password (sqluint8** a_password)
  { return get_string_if_valid (password_valid,
                                password,
                                a_password); }
inline Remote_Function_Info*
Server_Info::get_first_remote_function (void)
  { return function_list_head; }
inline Remote_Function_Info*
Server_Info::get_next_remote_function (Remote_Function_Info* a_cur_func_info)
  { return a_cur_func_info->next_func; }
inline Local_Type_Info*
Server_Info::get_first_local_type (void)
  { return ltype_list_head; }
inline Local_Type_Info*
Server_Info::get_next_local_type (Local_Type_Info* a_cur_ltype_info)
  { return a_cur_ltype_info->next_ltype; }
inline void
Server_Info::initialize (void)
  {
    name_valid = wrapper_name_valid = type_valid = version_valid =
      authid_valid = password_valid = FALSE;
    function_list_head = NULL;
    ltype_list_head = NULL;
  }

////////////////////////////////////////////////////////////////////////////////
// User_Info
////////////////////////////////////////////////////////////////////////////////
inline User_Info::User_Info (void) { initialize (); }
inline sqlint32
User_Info::set_server_name (sqluint8*  a_server_name,
                            sqlint32   a_server_name_len)
  { return copy_set_valid_and_terminate (a_server_name,
                                         a_server_name_len,
                                         &server_name,
                                         &server_name_valid); }
inline sqlint32
User_Info::get_server_name (sqluint8**  a_server_name)
  { return get_string_if_valid (server_name_valid,
                                server_name,
                                a_server_name); }
inline sqlint32
User_Info::set_authid (sqluint8*  a_authid,
                       sqlint32   a_authid_len)
  { return copy_set_valid_and_terminate (a_authid,
                                         a_authid_len,
                                         &authid,
                                         &authid_valid); }
inline sqlint32
User_Info::get_authid (sqluint8**  a_authid)
  { return get_string_if_valid (authid_valid,
                                authid,
                                a_authid); }
inline void
User_Info::initialize (void)
  { server_name_valid = authid_valid = FALSE; }

////////////////////////////////////////////////////////////////////////////////
// Column_Info
////////////////////////////////////////////////////////////////////////////////
inline Column_Info::Column_Info (void) { initialize (); }
inline sqlint32
Column_Info::set_column_name (sqluint8*  a_column_name,
                              sqlint32   a_column_name_len)
  { return copy_set_valid_and_terminate (a_column_name,
                                         a_column_name_len,
                                         &column_name,
                                         &column_name_valid); }
inline sqlint32
Column_Info::get_column_name (sqluint8** a_column_name)
  { return get_string_if_valid (column_name_valid,
                                column_name,
                                a_column_name); }
inline sqlint32
Column_Info::set_new_column_name (sqluint8*  a_new_col_name,
                                  sqlint32   a_new_col_name_len)
  { return copy_set_valid_and_terminate (a_new_col_name,
                                         a_new_col_name_len,
                                         &new_column_name,
                                         &new_column_name_valid); }
inline sqlint32
Column_Info::get_new_column_name (sqluint8**  a_new_col_name)
  { return get_string_if_valid (new_column_name_valid,
                                new_column_name,
                                a_new_col_name); }
inline sqlint32
Column_Info::set_column_type (sqluint8*  a_column_type,
                              sqlint32   a_column_type_len)
  { return copy_set_valid_and_terminate (a_column_type,
                                         a_column_type_len,
                                         &column_type,
                                         &column_type_valid); }
inline sqlint32
Column_Info::get_column_type (sqluint8**  a_column_type)
  { return get_string_if_valid (column_type_valid,
                                column_type,
                                a_column_type); }
inline void
Column_Info::set_for_bit_data (sqluint8  a_for_bit_data)
  { for_bit_data_valid = TRUE; for_bit_data = a_for_bit_data; return; }
inline sqlint32
Column_Info::get_for_bit_data (sqluint8*  a_for_bit_data)
  { return for_bit_data_valid ? *a_for_bit_data = for_bit_data, 0 : SQLQG_NOVALUE; }
inline void
Column_Info::set_column_ID (sqlint16  a_column_ID)
  { column_ID_valid = TRUE; column_ID = a_column_ID; return; }
inline sqlint32
Column_Info::get_column_ID (sqlint16*  a_column_ID)
  { return column_ID_valid ? *a_column_ID = column_ID, 0 : SQLQG_NOVALUE; }
inline sqlint32
Column_Info::set_type_schema (sqluint8*  a_type_schema,
                              sqlint32   a_type_schema_len)
  { return copy_set_valid_and_terminate (a_type_schema,
                                         a_type_schema_len,
                                         &type_schema,
                                         &type_schema_valid); }
inline sqlint32
Column_Info::get_type_schema (sqluint8**  a_type_schema)
  { return get_string_if_valid (type_schema_valid,
                                type_schema,
                                a_type_schema); }
inline sqlint32
Column_Info::set_type_name (sqluint8*  a_type_name,
                            sqlint32   a_type_name_len)
  { return copy_set_valid_and_terminate (a_type_name,
                                         a_type_name_len,
                                         &type_name,
                                         &type_name_valid); }
inline sqlint32
Column_Info::get_type_name (sqluint8**  a_type_name)
  { return get_string_if_valid (type_name_valid,
                                type_name,
                                a_type_name); }
inline void
Column_Info::set_org_length (sqlint32  a_org_length)
  { org_length_valid = TRUE; org_length = a_org_length; return; }
inline sqlint32
Column_Info::get_org_length (sqlint32*  a_org_length)
  { return org_length_valid ? *a_org_length = org_length, 0 : SQLQG_NOVALUE; }
inline void
Column_Info::set_org_scale (sqlint16  a_org_scale)
  { org_scale_valid = TRUE; org_scale = a_org_scale; return; }
inline sqlint32
Column_Info::get_org_scale (sqlint16*  a_org_scale)
  { return org_scale_valid ? *a_org_scale = org_scale, 0 : SQLQG_NOVALUE; }
inline void
Column_Info::set_avg_len (sqlint32  a_avg_len)
  { avg_len_valid = TRUE; avg_len = a_avg_len; return; }
inline sqlint32
Column_Info::get_avg_len (sqlint32*  a_avg_len)
  { return avg_len_valid ? *a_avg_len = avg_len, 0 : SQLQG_NOVALUE; }
inline sqlint32
Column_Info::set_high2key (sqluint8*  a_high2key,
                           sqlint32   a_high2key_len)
  { return copy_set_valid_and_terminate (a_high2key,
                                         a_high2key_len,
                                         &high2key,
                                         &high2key_valid); }
inline sqlint32
Column_Info::get_high2key (sqluint8**  a_high2key)
  { return get_string_if_valid (high2key_valid,
                                high2key,
                                a_high2key); }
inline sqlint32
Column_Info::set_low2key (sqluint8*  a_low2key,
                          sqlint32   a_low2key_len)
  { return copy_set_valid_and_terminate (a_low2key,
                                         a_low2key_len,
                                         &low2key,
                                         &low2key_valid); }
inline sqlint32
Column_Info::get_low2key (sqluint8**  a_low2key)
  { return get_string_if_valid (low2key_valid,
                                low2key,
                                a_low2key); }
inline void
Column_Info::set_nulls (sqluint8  a_nulls)
  { nulls_valid = TRUE; nulls = a_nulls; return; }
inline sqlint32
Column_Info::get_nulls (sqluint8*  a_nulls)
  { return nulls_valid ? *a_nulls = nulls, 0 : SQLQG_NOVALUE; }
inline sqlint32
Column_Info::set_default (sqluint8*  a_default,
                          sqlint32   a_default_len)
  { return copy_set_valid_and_terminate (a_default,
                                         a_default_len,
                                         &deflt,
                                         &default_valid); }
inline sqlint32
Column_Info::get_default (sqluint8**  a_default)
  { return get_string_if_valid (default_valid,
                                deflt,
                                a_default); }
inline void
Column_Info::set_colcard (sqlint64  a_colcard)
  { colcard_valid = TRUE; colcard = a_colcard; return; }
inline sqlint32
Column_Info::get_colcard (sqlint64*  a_colcard)
  { return colcard_valid ? *a_colcard = colcard, 0 : SQLQG_NOVALUE; }
inline void
Column_Info::set_codepage1 (sqlint16  a_codepage1)
  { codepage1_valid = TRUE; codepage1 = a_codepage1; return; }
inline sqlint32
Column_Info::get_codepage1 (sqlint16*  a_codepage1)
  { return codepage1_valid ? *a_codepage1 = codepage1, 0 : SQLQG_NOVALUE; }
inline void
Column_Info::set_codepage2 (sqlint16  a_codepage2)
  { codepage2_valid = TRUE; codepage2 = a_codepage2; return; }
inline sqlint32
Column_Info::get_codepage2 (sqlint16*  a_codepage2)
  { return codepage2_valid ? *a_codepage2 = codepage2, 0 : SQLQG_NOVALUE; }
inline void
Column_Info::initialize (void)
  {
    column_name_valid = column_type_valid = new_column_name_valid = FALSE;
    for_bit_data_valid = column_ID_valid = type_schema_valid = FALSE;
    type_name_valid = FALSE;
    org_length_valid = org_scale_valid = avg_len_valid = FALSE;
    high2key_valid = low2key_valid = nulls_valid = default_valid = FALSE;
    colcard_valid = codepage1_valid = codepage2_valid = FALSE;
    next_col = NULL;
  }

////////////////////////////////////////////////////////////////////////////////
// Index_Info
////////////////////////////////////////////////////////////////////////////////
inline Index_Info::Index_Info (void) { initialize (); }
inline sqlint32
Index_Info::set_index_name (sqluint8*  a_index_name,
                           sqlint32   a_index_name_len)
  { return copy_set_valid_and_terminate (a_index_name,
                                         a_index_name_len,
                                         &index_name,
                                         &index_name_valid); }
inline sqlint32
Index_Info::get_index_name (sqluint8** a_index_name)
  { return get_string_if_valid (index_name_valid,
                                index_name,
                                a_index_name); }
inline sqlint32
Index_Info::set_colnames (sqluint8*  a_colnames,
                         sqlint32   a_colnames_len)
  { return copy_set_valid_and_terminate (a_colnames,
                                         a_colnames_len,
                                         &colnames,
                                         &colnames_valid); }
inline sqlint32
Index_Info::get_colnames (sqluint8**  a_colnames)
  { return get_string_if_valid (colnames_valid,
                                colnames,
                                a_colnames); }
inline void
Index_Info::set_unique_rule (sqluint8  a_unique_rule)
  { unique_rule_valid = TRUE; unique_rule = a_unique_rule; return; }
inline sqlint32
Index_Info::get_unique_rule (sqluint8*  a_unique_rule)
  { return unique_rule_valid ? *a_unique_rule = unique_rule, 0 : SQLQG_NOVALUE; }
inline void
Index_Info::set_colcount (sqlint16  a_colcount)
  { colcount_valid = TRUE; colcount = a_colcount; return; }
inline sqlint32
Index_Info::get_colcount (sqlint16*  a_colcount)
  { return colcount_valid ? *a_colcount = colcount, 0 : SQLQG_NOVALUE; }
inline void
Index_Info::set_unique_colcount (sqlint16  a_unique_colcount)
  { unique_colcount_valid = TRUE; unique_colcount = a_unique_colcount; return; }
inline sqlint32
Index_Info::get_unique_colcount (sqlint16*  a_unique_colcount)
  { return unique_colcount_valid ? *a_unique_colcount = unique_colcount, 0 : SQLQG_NOVALUE; }
inline void
Index_Info::set_nleaf (sqlint32 a_nleaf)
  { nleaf_valid = TRUE; nleaf = a_nleaf; return; }
inline sqlint32
Index_Info::get_nleaf (sqlint32* a_nleaf)
  { return nleaf_valid ? *a_nleaf = nleaf, 0 : SQLQG_NOVALUE; }
inline void
Index_Info::set_nlevels (sqlint16 a_nlevels)
  { nlevels_valid = TRUE; nlevels = a_nlevels; return; }
inline sqlint32
Index_Info::get_nlevels (sqlint16* a_nlevels)
  { return nlevels_valid ? *a_nlevels = nlevels, 0 : SQLQG_NOVALUE; }
inline void
Index_Info::set_first_keycard (sqlint64 a_first_keycard)
  { first_keycard_valid = TRUE; first_keycard = a_first_keycard; return; }
inline sqlint32
Index_Info::get_first_keycard (sqlint64* a_first_keycard)
  { return first_keycard_valid ? *a_first_keycard = first_keycard, 0 : SQLQG_NOVALUE; }
inline void
Index_Info::set_full_keycard (sqlint64 a_full_keycard)
  { full_keycard_valid = TRUE; full_keycard = a_full_keycard; return; }
inline sqlint32
Index_Info::get_full_keycard (sqlint64* a_full_keycard)
  { return full_keycard_valid ? *a_full_keycard = full_keycard, 0 : SQLQG_NOVALUE; }
inline void
Index_Info::set_cluster_ratio (sqlint16 a_cluster_ratio)
  { cluster_ratio_valid = TRUE; cluster_ratio = a_cluster_ratio; return; }
inline sqlint32
Index_Info::get_cluster_ratio (sqlint16* a_cluster_ratio)
  { return cluster_ratio_valid ? *a_cluster_ratio = cluster_ratio, 0 : SQLQG_NOVALUE; }
inline void
Index_Info::initialize (void)
  {
    next_idx = prev_idx = NULL;
    index_name_valid = colnames_valid = unique_rule_valid = FALSE;
    colcount_valid = nleaf_valid = nlevels_valid = FALSE;
    first_keycard_valid = full_keycard_valid = cluster_ratio_valid = FALSE;
    unique_colcount_valid = FALSE;
  }

////////////////////////////////////////////////////////////////////////////////
// Nickname_Info
////////////////////////////////////////////////////////////////////////////////
inline Nickname_Info::Nickname_Info (void) { initialize (); }
inline sqlint32
Nickname_Info::set_local_schema (sqluint8*  a_local_schema,
                             sqlint32   a_local_schema_len)
  { return copy_set_valid_and_terminate (a_local_schema,
                                         a_local_schema_len,
                                         &schema,
                                         &schema_valid); }
inline sqlint32
Nickname_Info::get_local_schema (sqluint8**  a_local_schema)
  { return get_string_if_valid (schema_valid,
                                schema,
                                a_local_schema); }
inline sqlint32
Nickname_Info::set_nickname (sqluint8*  a_nickname,
                         sqlint32   a_nickname_len)
  { return copy_set_valid_and_terminate (a_nickname,
                                         a_nickname_len,
                                         &nickname,
                                         &nickname_valid); }
inline sqlint32
Nickname_Info::get_nickname (sqluint8**  a_nickname)
  { return get_string_if_valid (nickname_valid,
                                nickname,
                                a_nickname); }
inline sqlint32
Nickname_Info::set_server_name (sqluint8*  a_server_name,
                            sqlint32   a_server_name_len)
  { return copy_set_valid_and_terminate (a_server_name,
                                         a_server_name_len,
                                         &server_name,
                                         &server_name_valid); }
inline sqlint32
Nickname_Info::get_server_name (sqluint8**  a_server_name)
  { return get_string_if_valid (server_name_valid,
                                server_name,
                                a_server_name); }
inline void
Nickname_Info::set_card (sqlint64  a_card)
  { card_valid = TRUE; card = a_card; return; }
inline sqlint32
Nickname_Info::get_card (sqlint64*  a_card)
  { return card_valid ? *a_card = card, 0 : SQLQG_NOVALUE; }
inline void
Nickname_Info::set_npages (sqlint32  a_npages)
  { npages_valid = TRUE; npages = a_npages; return; }
inline sqlint32
Nickname_Info::get_npages (sqlint32* a_npages)
  { return npages_valid ? *a_npages = npages, 0 : SQLQG_NOVALUE; }
inline void
Nickname_Info::set_fpages (sqlint32 a_fpages)
  { fpages_valid = TRUE; fpages = a_fpages; return; }
inline sqlint32
Nickname_Info::get_fpages (sqlint32* a_fpages)
  { return fpages_valid ? *a_fpages = fpages, 0 : SQLQG_NOVALUE; }
inline void
Nickname_Info::set_overflow (sqlint32 a_overflow)
  { overflow_valid = TRUE; overflow = a_overflow; return; }
inline sqlint32
Nickname_Info::get_overflow (sqlint32* a_overflow)
  { return overflow_valid ? *a_overflow = overflow, 0 : SQLQG_NOVALUE; }
inline sqlint16
Nickname_Info::get_number_columns (void)
  { return n_columns; }
inline Column_Info*
Nickname_Info::get_first_column (void) { return column_list_head; }
inline Column_Info*
Nickname_Info::get_next_column (Column_Info* a_cur_col_info)
  { return a_cur_col_info->next_col; }
inline sqlint16
Nickname_Info::get_number_indexes (void)
  { return n_indexes; }
inline Index_Info*
Nickname_Info::get_first_index (void) { return index_list_head; }
inline Index_Info*
Nickname_Info::get_next_index (Index_Info* a_cur_idx_info)
  { return a_cur_idx_info->next_idx; }
inline void*
Nickname_Info::get_nickname_pd (void)
  { return pd; }
inline void
Nickname_Info::set_nickname_pd (void* a_new_pd)
  { pd = a_new_pd; }
inline sqlqg_FMP_Runtime_Object*
Nickname_Info::get_m_agt_runtime_obj (void)
  { return m_agt_runtime_obj; }
inline void
Nickname_Info::set_m_agt_runtime_obj (sqlqg_FMP_Runtime_Object* a_agt_runtime_obj)
  { m_agt_runtime_obj = a_agt_runtime_obj; }
inline sqlqg_FMP_Runtime_Object*
Nickname_Info::get_m_stp_runtime_obj (void)
  { return m_stp_runtime_obj; }
inline void
Nickname_Info::set_m_stp_runtime_obj (sqlqg_FMP_Runtime_Object* a_stp_runtime_obj)
  { m_stp_runtime_obj = a_stp_runtime_obj; }
inline void
Nickname_Info::initialize (void)
  {
    server_name_valid = nickname_valid = schema_valid = FALSE;
    card_valid = npages_valid = fpages_valid = overflow_valid = FALSE;
    column_list_head = column_list_tail = NULL;
    index_list_head = NULL;
    n_columns = n_indexes = 0;
    m_agt_runtime_obj = NULL;
    m_stp_runtime_obj = NULL;
  }

////////////////////////////////////////////////////////////////////////////////
// Procparm_Info
////////////////////////////////////////////////////////////////////////////////
inline Procparm_Info::Procparm_Info (void) { initialize (); };
inline sqlint32
Procparm_Info::set_parm_name (sqluint8*  a_parm_name,
                          sqlint32   a_parm_name_len)
  { return copy_set_valid_and_terminate (a_parm_name,
                                         a_parm_name_len,
                                         &parm_name,
                                         &parm_name_valid); };
inline sqlint32
Procparm_Info::get_parm_name (sqluint8** a_parm_name)
  { return get_string_if_valid (parm_name_valid,
                                parm_name,
                                a_parm_name); };
inline sqlint32
Procparm_Info::set_parm_mode (sqluint8*  a_parm_mode,
                          sqlint32   a_parm_mode_len)
  { return copy_set_valid_and_terminate (a_parm_mode,
                                          a_parm_mode_len,
                                          &parm_mode,
                                          &parm_mode_valid); };
inline sqlint32
Procparm_Info::get_parm_type (sqluint8**  a_parm_mode)
  { return get_string_if_valid (parm_mode_valid,
                                parm_mode,
                                a_parm_mode); };
inline void
Procparm_Info::set_for_bit_data (sqluint8 a_for_bit_data)
  { for_bit_data_valid = TRUE; for_bit_data = a_for_bit_data; return; };
inline sqlint32
Procparm_Info::get_for_bit_data (sqluint8* a_for_bit_data)
  { return for_bit_data_valid ? *a_for_bit_data = for_bit_data, 0 : SQLQG_NOVALUE; };
inline void
Procparm_Info::set_remote_for_bit_data (sqluint8 a_rem_for_bit_data)
  { remote_for_bit_data_valid = TRUE; remote_for_bit_data = a_rem_for_bit_data; return; };
inline sqlint32
Procparm_Info::get_remote_for_bit_data (sqluint8* a_rem_for_bit_data)
  { return remote_for_bit_data_valid ? *a_rem_for_bit_data = remote_for_bit_data, 0 : SQLQG_NOVALUE; };
inline sqlint32
Procparm_Info::set_type_schema (sqluint8*  a_type_schema,
                            sqlint32   a_type_schema_len)
  { return copy_set_valid_and_terminate (a_type_schema,
                                         a_type_schema_len,
                                         &type_schema,
                                         &type_schema_valid); };
inline sqlint32
Procparm_Info::get_type_schema (sqluint8**  a_type_schema)
  { return get_string_if_valid (type_schema_valid,
                                type_schema,
                                a_type_schema); };
inline sqlint32
Procparm_Info::set_type_name (sqluint8*  a_type_name,
                          sqlint32   a_type_name_len)
  { return copy_set_valid_and_terminate (a_type_name,
                                         a_type_name_len,
                                         &type_name,
                                         &type_name_valid); };
inline sqlint32
Procparm_Info::get_type_name (sqluint8**  a_type_name)
  { return get_string_if_valid (type_name_valid,
                                type_name,
                                a_type_name); };
inline void
Procparm_Info::set_org_length (sqlint32 a_org_length)
  { org_length_valid = TRUE; org_length = a_org_length; return; };
inline sqlint32
Procparm_Info::get_org_length (sqlint32* a_org_length)
  { return org_length_valid ? *a_org_length = org_length, 0 : SQLQG_NOVALUE; };
inline void
Procparm_Info::set_org_scale (sqlint16 a_org_scale)
  { org_scale_valid = TRUE; org_scale = a_org_scale; return; };
inline sqlint32
Procparm_Info::get_org_scale (sqlint16* a_org_scale)
  { return org_scale_valid ? *a_org_scale = org_scale, 0 : SQLQG_NOVALUE; };
inline void
Procparm_Info::set_nulls (sqluint8 a_nulls)
  { nulls_valid = TRUE; nulls = a_nulls; return; };
inline sqlint32
Procparm_Info::get_nulls (sqluint8* a_nulls)
  { return nulls_valid ? *a_nulls = nulls, 0 : SQLQG_NOVALUE; };
inline void
Procparm_Info::set_remote_nulls (sqluint8 a_rem_nulls)
  { remote_nulls_valid = TRUE; remote_nulls = a_rem_nulls; return; };
inline sqlint32
Procparm_Info::get_remote_nulls (sqluint8* a_rem_nulls)
  { return remote_nulls_valid ? *a_rem_nulls = remote_nulls, 0 : SQLQG_NOVALUE; };
inline void
Procparm_Info::set_remote_length (sqlint16 a_rem_length)
  { remote_length_valid = TRUE; remote_length = a_rem_length; return; };
inline sqlint32
Procparm_Info::get_remote_length (sqlint16* a_rem_length)
  { return remote_length_valid ? *a_rem_length = remote_length, 0 : SQLQG_NOVALUE; };
inline void
Procparm_Info::set_remote_scale (sqlint16 a_rem_scale)
  { remote_scale_valid = TRUE; remote_scale = a_rem_scale; return; };
inline sqlint32
Procparm_Info::get_remote_scale (sqlint16* a_rem_scale)
  { return remote_scale_valid ? *a_rem_scale = remote_scale, 0 : SQLQG_NOVALUE; };
inline sqlint32
Procparm_Info::set_remote_type_schema (sqluint8*  a_rem_type_schema,
                                   sqlint32   a_rem_type_schema_len)
  { return copy_set_valid_and_terminate (a_rem_type_schema,
                                         a_rem_type_schema_len,
                                         &remote_type_schema,
                                         &remote_type_schema_valid); };
inline sqlint32
Procparm_Info::get_remote_type_schema (sqluint8**  a_rem_type_schema)
  { return get_string_if_valid (remote_type_schema_valid,
                                remote_type_schema,
                                a_rem_type_schema); };
inline sqlint32
Procparm_Info::set_remote_type_name (sqluint8*  a_rem_type_name,
                                 sqlint32   a_rem_type_name_len)
  { return copy_set_valid_and_terminate (a_rem_type_name,
                                         a_rem_type_name_len,
                                         &remote_type_name,
                                         &remote_type_name_valid); };
inline sqlint32
Procparm_Info::get_remote_type_name (sqluint8**  a_rem_type_name)
  { return get_string_if_valid (remote_type_name_valid,
                                remote_type_name,
                                a_rem_type_name); };
inline void
Procparm_Info::set_codepage1 (sqlint16 a_codepage1)
  { codepage1_valid = TRUE; codepage1 = a_codepage1; return; };
inline sqlint32
Procparm_Info::get_codepage1 (sqlint16* a_codepage1)
  { return codepage1_valid ? *a_codepage1 = codepage1, 0 : SQLQG_NOVALUE; };
inline void
Procparm_Info::set_codepage2 (sqlint16 a_codepage2)
  { codepage2_valid = TRUE; codepage2 = a_codepage2; return; };
inline sqlint32
Procparm_Info::get_codepage2 (sqlint16* a_codepage2)
  { return codepage2_valid ? *a_codepage2 = codepage2, 0 : SQLQG_NOVALUE; };
inline void
Procparm_Info::initialize (void)
  {
    parm_name_valid = parm_mode_valid = FALSE;
    for_bit_data_valid =  type_schema_valid = FALSE;
    type_name_valid = codepage1_valid = codepage2_valid = FALSE;
    org_length_valid = org_scale_valid = FALSE;
    remote_type_name_valid = remote_type_schema_valid = FALSE;
    remote_nulls_valid = remote_scale_valid = remote_length_valid = FALSE;
    nulls_valid = FALSE;
    next_parm = NULL;
  }

////////////////////////////////////////////////////////////////////////////////
// Procedure_Info
////////////////////////////////////////////////////////////////////////////////
inline Procedure_Info::Procedure_Info (void) { initialize (); };
inline sqlint32
Procedure_Info::set_local_schema (sqluint8*  a_local_schema,
                             sqlint32   a_local_schema_len)
  { return copy_set_valid_and_terminate (a_local_schema,
                                         a_local_schema_len,
                                         &schema,
                                         &schema_valid); };
inline sqlint32
Procedure_Info::get_local_schema (sqluint8**  a_local_schema)
  { return get_string_if_valid (schema_valid,
                                schema,
                                a_local_schema); };
inline sqlint32
Procedure_Info::set_nickname (sqluint8*  a_nickname,
                         sqlint32  a_nickname_len)
  { return copy_set_valid_and_terminate (a_nickname,
                                         a_nickname_len,
                                         &nickname,
                                         &nickname_valid); };
inline sqlint32
Procedure_Info::get_nickname (sqluint8** a_nickname)
  { return get_string_if_valid (nickname_valid,
                                nickname,
                                a_nickname); };
inline sqlint16
Procedure_Info::get_number_parms (void) { return n_parms; };
inline Procparm_Info*
Procedure_Info::get_first_parm (void) { return parm_list_head; };
inline Procparm_Info*
Procedure_Info::get_next_parm (Procparm_Info* a_cur_procparm_info)
  { return a_cur_procparm_info->next_parm; };
inline void
Procedure_Info::initialize (void)
  {
    nickname_valid = schema_valid = FALSE;
    parm_list_head = parm_list_tail = NULL;
    n_parms = 0;
  };
#endif
