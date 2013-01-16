/******************************************************************************
**
** Source File Name: sqlqg_operation.h
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
**              managing query operations
**
** Operating System: All
**
*******************************************************************************/
#ifndef SQL_H_SQLQG_OPERATION
#define SQL_H_SQLQG_OPERATION

#include "sqlsystm.h"
#include "sqlqg_misc.h"
#include "sqlqg_runtime_data_operation.h"

#define SQL_STATEMENT_DEFAULT_INCREMENT 512
#define MAX_STATEMENT_LENGTH 32766

// Forward declarations.
class Runtime_Operation;
class FencedServer;
class Remote_Connection;
class Runtime_Data_Desc_list;
class Request_Exp;


//////////////////////////////////////////////////////////////////////////////
// Remote_Operation base class (used to represent operations performed at
// a remote data source).
//////////////////////////////////////////////////////////////////////////////
class Remote_Operation : public Sqlqg_Base_Class {
public:

  // This enum helps to distinguish between the different
  // kinds of remote operations.

  enum operation_kind {
    no_kind,
    query_kind,
    iud_kind,
    lock_kind,
    passthru_kind,
    stored_procedure_kind,
    invocation_kind,
    remote_lock_kind,
    svpt_kind};

  // Constructor.
  Remote_Operation (Remote_Connection* a_active_connection,
                    Runtime_Operation* a_runtime_op_info,
                    operation_kind     a_op_kind,
                    sqlint32*          a_rc);

  // Unpack constructor.  This constructor builds a Remote_Query from
  // the packed buffer stored on the runtime operator.
  Remote_Operation (void*      a_buf,
                    sqlint32*  a_rc);

  // Destructor.
  virtual ~Remote_Operation (void);

  // Makes a copy of a SQL statement, storing it inline if possible,
  // otherwise storing it out-of-line.
  sqlint32 copy_SQL_statement (char*  a_sql_statement);

  // Allocates out-of-line storage for SQL statement.
  sqlint32 enlarge_SQL_statement (sqlint32 a_new_length);

  // Appends to end of SQL statement, possible enlarging it.
  sqlint32 append_SQL_statement (char* a_new_text);

  // Inserts into the sqlqgdle of the SQL_statement buffer, possibly
  // enlarging it.  This code was taken from oracle_services.C,
  // in the BindInvariants section.
  // SQL_statement_start points to where we should begin searching
  // in SQL_statement.  It will be set to the beginning of the place
  // holder in the string.
  // place_holder is the place_holder to search for in SQL_statement,
  // and text is the text to be inserted.
  // text_length is the length of the text.
  sqlint32 insert_into_SQL_statement (char*   a_place_holder,
                                      char*   a_text,
                                      int     a_text_length,
                                      char**  a_sql_statement_start);

  // Methods to maintain the linked list of operations.
  Remote_Operation* get_prev (void);

  void set_prev (Remote_Operation*  a_previous_op);

  Remote_Operation* get_next (void);

  void set_next (Remote_Operation*  a_next_op);

  // Accessor methods.
  operation_kind get_kind (void);

  Remote_Connection* get_connection (void);

  FencedServer* get_server (void);

  Runtime_Data_List* get_input_data (void);

  sqlint32  set_input_data (Runtime_Data_List*  a_new_input_data);

  Runtime_Data_List* get_output_data (void);

  sqlint32  set_output_data (Runtime_Data_List*  a_new_output_data);

  char* get_SQL_statement (void);

  //for the generic servers instead of an SQL statement
  void get_exec_desc (void**  a_exec_desc,
                      int*    a_exec_desc_len);

  // Replace host variables (":Hnnn") in the SQL statement with
  // parameter markers ("?").
  void replace_host_vars (void);

  // Return the number (the "nnn" part) of the given host
  // variable (":Hnnn").
  static sqlint32 get_host_var_number (char* a_host_var_ptr);

  // Move the given pointer past the host variable.  Optionally,
  // replace the host variable with a parameter marker ("?").
  static void move_host_var_ptr (char**      a_host_var_ptr,
                                 const bool  a_replace_flag);

protected:

  ////////////////
  // Data.
  ////////////////
  operation_kind     kind;
  Remote_Connection* connection;
  FencedServer*      server;

  Remote_Operation*  next;
  Remote_Operation*  prev;
  Runtime_Data_List* input_data;                 // Input data
  Runtime_Data_List* output_data;                // Output data

  // These fields are used to store a COPY of the SQL statement
  // (constructed by statement gen) that lives on the runtime operator.
  // If the statement is small enough, it will be stored inline the the
  // small_statement field.  If it is too large, it will be stored out-of-line
  // on the statement field.
  int   SQL_statement_length;                     // actual buffer length for
                                                  // SQL statment.
  int   max_statement_length;                     // allocated buffer length for
                                                  // SQL statment.
  char* SQL_statement;                            // SQL statement buffer.
  char  small_statement[MAX_STATEMENT_LENGTH];    // inline storage for SQL
                                                  // statement.  If the
                                                  // statement is small
                                                  // enough, it will be stored
                                                  // here.  Otherwise, it will
                                                  // be stored out-of-line.
  Runtime_Operation* runtime_info;

  ////////////////
  // Methods.
  ////////////////

  //-----------------------------------------------------------------
  // Find the end of an SQL string (enclosed in single quotes).
  //
  // Input:
  //   a_string = Pointer to the char past the opening quote.
  //
  // Return value:
  //   A pointer to the char past the closing quote.  If there is
  //   no closing quote, then a pointer to the terminating null.
  //-----------------------------------------------------------------
  static char* find_end_of_string (char*  a_string);

  //-----------------------------------------------------------------
  // Determine if two strings are the same.
  //
  // Input:
  //   a_string    = Pointer to string 1.
  //   a_substring = Pointer to string 2.
  //
  // Return value:
  //   0 = The two strings are different.
  //   1 = The two strings are the same (up to the terminating null).
  //-----------------------------------------------------------------
  static sqlint32 tell_if_string (const char*  a_string,
                                  const char*  a_substring);

public:
  //-----------------------------------------------------------------
  // Search the given string for the given host variable name,
  // ignoring occurrences of the host variable name that are inside
  // SQL strings (enclosed in single quotes).
  //
  // Input:
  //   a_string    = Pointer to the string to search.
  //   a_substring = Pointer to the host variable name to search for.
  //               It must begin with a colon.
  // Return value:
  //   If the host variable name is found, then a pointer to it
  //   within the string.  Otherwise, NULL.
  //-----------------------------------------------------------------
  static inline char* parse_string (char*        a_string,
                                    const char*  a_substring);

  Runtime_Operation* get_runtime_info (void);                         

protected:
  // Destroys SQL_statement buffer if it had been allocated.
  void destroy_SQL_statement (void);

  // Encapsulated definition of EOF return code
  virtual sqlint32 report_eof (void);

  friend class DJ_Wrapper_Utilities;
private:
};

////////////////////////////////////////////////////////////////////////////////
// Remote Query class.
// This class is used to represent a query to be executed at a remote data
// source.
////////////////////////////////////////////////////////////////////////////////
class Remote_Query : public Remote_Operation {
public:
  // Constructor.
  Remote_Query (Remote_Connection*  a_active_connection,
                Runtime_Operation*  a_runtime_info,
                sqlint32*           a_rc);

  // Destructor.
  virtual ~Remote_Query (void);

  // Runtime routines.
  virtual sqlint32 open (void);                    // open a cursor
  virtual sqlint32 reopen (sqlint16  a_status);   // reopen cursor
  virtual sqlint32 fetch (void);                  // fetches a row
  virtual sqlint32 close (sqlint16  a_status);    // closes a cursor

  // LOB interface
  // Open a query with LOB column(s)
  virtual  sqlint32 open_input_lob (sqlint32*       a_column_number,
                                    sqluint32       a_mat_size,
                                    sqluint32       a_xfer_bytes,
                                    unsigned char*  a_buffer);

  virtual  sqlint32 reopen_input_lob (sqlint16        a_status,
                                      sqlint32*       a_column_number,
                                      sqluint32       a_mat_size,
                                      sqluint32       a_xfer_bytes,
                                      unsigned char*  a_buffer);

  virtual sqlint32 fetch_lob (unsigned char* a_buffer,
			      sqluint32      a_buffer_size,
			      sqluint32      a_bytes_written);


  // Set status and return EOF return code
  sqlint32 report_eof (void);

  // Accessor methods.
  sqluint8 get_status (void);

  void set_status (sqluint8  a_new_status);

  virtual sqluint32 get_row_status (void);
  virtual void reset_row_status (void);
  // set the row_status that might have been changed in runtime operation.
  virtual void set_row_status (sqluint32 a_row_status);
  sqlint32 is_for_update (void);

  // Internal LOB interface
  virtual sqlint32 fetch_lob (sqlint32*       a_column_number,
                              sqluint32       a_buffer_size,
                              sqluint32*      a_fetch_bytes,
                              sqluint32*      a_total_bytes,
                              unsigned char*  a_buffer_pointer);

protected:
  
  enum sqlqg_lob_status {
    LOB_MORE,               // More data available for this LOB
    LOB_LAST};              // Last data for this LOB

  enum sqlqg_lob_intent {
    LOB_NEXT,               // Fetch another LOB next
    NON_LOB_NEXT,           // Fetch non-LOB column next
    ROW_DONE};              // Row is complete, no more data

  ////////////////
  // Methods.
  ////////////////

  // To be called from fetch_lob after xferring data to buffer
  sqlint32 lob_data_ready (sqlint32         a_col_number,
			   sqluint32        a_bytes_ready,
			   sqlqg_lob_status a_status,
			   sqlqg_lob_intent a_next);

  // To be called from fetch when lob data is to be fetched next
  // Do *not* call from fetch_lob!!
  void set_lob_next (void);

  ////////////////
  // Data.
  ////////////////
  sqluint8 status;                                  // Status of request.

 private:

  ////////////////
  // Data.
  ////////////////

  // row_status flag for wrappers that use the public lob interface
  sqluint32      m_row_status;

  // Pointers to fields in DB2 rquery_lob structure
  sqlint32*      m_p_col_num;
  sqluint32*     m_p_bytes_this_write;
  sqluint32*     m_p_tot_bytes;

  // LOB buffer info
  unsigned char* m_buf_ptr;
  sqluint32      m_buf_size;
};

////////////////////////////////////////////////////////////////////////////////
// Remote IUD class.
// This class is used to represent an insert/update/delete to be executed at a
// remote data source.
////////////////////////////////////////////////////////////////////////////////
class Remote_IUD : public Remote_Operation {
public:
  // Constructor.
  Remote_IUD (Remote_Connection* a_active_connection,
              Runtime_Operation* a_runtime_info,
              sqlint32*          a_rc);

  // Destructor.
  virtual   ~Remote_IUD (void);

  // Runtime routines.
  virtual  sqlint32 insert_row (sqlint32* a_number_of_rows);

  // Insert a row with LOB column(s)
  virtual  sqlint32 insert_row_lob (sqlint32*       a_number_of_rows,
                                    sqlint32*       a_column_number,
                                    sqluint32       a_max_size,
                                    sqluint32       a_xfer_bytes,
                                    unsigned char*  a_buffer);

  virtual  sqlint32 update_row(sqlint32 *a_number_of_rows);

  // Update a row with LOB column(s)
  virtual  sqlint32 update_row_lob (sqlint32*       a_number_of_rows,
                                    sqlint32*       a_column_number,
                                    sqluint32       a_max_size,
                                    sqluint32       a_xfer_bytes,
                                    unsigned char*  a_buffer);

  virtual  sqlint32 delete_row (sqlint32* a_number_of_rows);

  // Accessor methods.
  sqluint8 get_status (void);

  void set_status (sqluint8  a_new_status);

  Remote_Operation* get_remote_query (void);

  sqluint8 get_reprepare (void);

  //get the interim status of INSERT/UPDATE a row
  virtual sqluint32 get_row_status (void);

  // set the row_status that might have been changed in runtime operation.
  virtual void set_row_status (sqluint32 a_row_status);

protected:

  ////////////////
  // Data.
  ////////////////
  sqluint8          m_status;                     // Status of request.
  Remote_Operation* m_remote_query;
  sqluint8          re_prepare;

};

////////////////////////////////////////////////////////////////////////////////
// Remote SVPT class.
// This class is used to represent a savepoint at a remote data source.
////////////////////////////////////////////////////////////////////////////////
class Remote_SVPT : public Remote_Operation {
public:
  // Constructor.
  Remote_SVPT (Remote_Connection* a_active_connection,
               Runtime_Operation* a_runtime_info,
               sqlint32*          a_rc);

  // Destructor.
  virtual   ~Remote_SVPT (void);

  // Runtime routines.
  virtual  sqlint32 create_svpt (void);         // create remote svpt
  virtual  sqlint32 drop_svpt (void);           // drop remote svpt
  virtual  sqlint32 rollback_svpt (void);       // rollback remote svpt

  // Accessor methods.
  sqluint8 get_status (void);

  void set_status (sqluint8  a_new_status);

  unsigned char* get_svpt_name (void);

protected:

  ////////////////
  // Data.
  ////////////////
  sqluint8 m_status;                             // Status of request.

};

////////////////////////////////////////////////////////////////////////////////
// Remote Passthru class.
// This class is used to represent a passthru session at a remote data source.
////////////////////////////////////////////////////////////////////////////////
class Remote_Passthru : public Remote_Operation {
public:
  // Constructor.
  Remote_Passthru (Remote_Connection*  a_active_connection,
                   Runtime_Operation*  a_runtime_passthru,
                   sqlint32*           a_rc);

  // Destructor.
  virtual ~Remote_Passthru (void);

  // Runtime routines.
  virtual sqlint32 prepare (
                 Runtime_Data_Desc_List*  a_data_description_list);
                                                 // prepares a passthru
                                                 // session at a remote source.

  virtual sqlint32 describe (
                  Runtime_Data_Desc_List*  a_data_description_list);
                                                 // describes result set of a
                                                 // statement executed via a
                                                 // passthru session.
  virtual sqlint32 execute (void);               // executes a statement
                                                 // via a passthru session.
  virtual sqlint32 open (void);                  // opens a cursor for
                                                 // a passthru session.
  virtual sqlint32 fetch (void);                 // fetches a row from a
                                                 // passthru cursor.
  virtual sqlint32 close (void);                 // closes a passthru cursor.


};

////////////////////////////////////////////////////////////////////////////////
// Remote Invocation class.
// This class is used to represent a function to be executed at a remote data
// source.
////////////////////////////////////////////////////////////////////////////////
class Remote_Invocation : public Remote_Operation {
public:
  // Constructor.
  Remote_Invocation (Remote_Connection*  a_active_connection,
                     Runtime_Operation*  a_runtime_info,
                     sqlint32*           a_rc);

  // Destructor.
  virtual ~Remote_Invocation (void);

  // Runtime routines.
  virtual sqlint32 invoke (void);              //invoke the function

};

////////////////////////////////////////////////////////////////////////////////
//
// Remote Lock class.
// This class is used to represent a remote lock session at a remote data
// source.
////////////////////////////////////////////////////////////////////////////////
class Remote_Lock : public Remote_Operation {
public:
  // Constructor.
  Remote_Lock (Remote_Connection*  a_active_connection,
               Runtime_Operation*  a_runtime_lock,
               sqlint32*           a_rc);

  // Destructor.
  virtual ~Remote_Lock (void);

  virtual sqlint32 prepare (void);               // prepares the lock
                                                 // session at a remote source.
  virtual sqlint32 execute (void);
};

// Inline methods

// Remote_Operation
inline Remote_Operation*
Remote_Operation::get_prev (void)
{
  return prev;
}

inline void
Remote_Operation::set_prev (Remote_Operation*  a_previous_op)
{
  prev = a_previous_op;
}

inline Remote_Operation*
Remote_Operation::get_next (void)
{
  return next;
}

inline void
Remote_Operation::set_next (Remote_Operation*  a_next_op)
{
  next = a_next_op;
}

inline Remote_Operation::operation_kind
Remote_Operation::get_kind (void)
{
  return kind;
}

inline Remote_Connection*
Remote_Operation::get_connection (void)
{
  return connection;
}

inline Runtime_Data_List*
Remote_Operation::get_input_data (void)
{
  return input_data;
}

inline Runtime_Data_List*
Remote_Operation::get_output_data (void)
{
  return output_data;
}

inline char*
Remote_Operation::get_SQL_statement (void)
{
  return SQL_statement;
}

inline char*
Remote_Operation::find_end_of_string (char*  a_string)
{
  char* ptr = a_string;

  while('\0' != *ptr)
  {
    if ('\'' == *ptr)
    {
      ptr += 1;
      if ('\'' != *ptr)
      {
        break;
      }
    }
    ptr += 1;
  }
  return (ptr);
}

inline sqlint32
Remote_Operation::tell_if_string (const char*  a_string,
                                  const char*  a_substring)
{
  const char* ptr1 = a_string;
  const char* ptr2 = a_substring;
  sqlint32 rc = 0;

  do {
    ptr1 += 1;
    ptr2 += 1;
  }
  while ('\0' != *ptr1 && *ptr1 == *ptr2);

  if ('\0' == *ptr2)
  {
    rc = 1;
  }
  return (rc);
}

inline char*
Remote_Operation::parse_string (char*        a_string,
                                const char*  a_substring)
{
  char* ptr1 = a_string;
  const char* ptr2 = a_substring;

  while ('\0' != *ptr1)
  {
    if ('\'' == *ptr1)
    {
      ptr1 += 1;
      ptr1 = find_end_of_string (ptr1);
    }
    else
    {
      if (':' == *ptr1)
      {
        if (0 != tell_if_string (ptr1, ptr2))
          goto exit;
      }
      ptr1 += 1;
    }
  }
  ptr1 = NULL;

exit:
  return (ptr1);
}

inline Runtime_Operation*
Remote_Operation::get_runtime_info (void)
{
  return (runtime_info);
}

// Remote_Query
inline sqlint32
Remote_Query::report_eof (void)
{
  status = SQLQG_EOF;
  return Remote_Operation::report_eof ();
}

inline sqluint8
Remote_Query::get_status (void)
{
  return status;
}

inline void
Remote_Query::set_status (sqluint8  a_new_status)
{
  status = a_new_status;
}

// Remote_IUD
inline sqluint8
Remote_IUD::get_status (void)
{
  return m_status;
}

inline void
Remote_IUD::set_status (sqluint8  a_new_status)
{
  m_status = a_new_status;
}

inline Remote_Operation*
Remote_IUD::get_remote_query (void)
{
  return m_remote_query;
}

inline sqluint8
Remote_IUD::get_reprepare (void)
{
  return re_prepare;
}

// Remote_SVPT
inline sqluint8
Remote_SVPT::get_status (void)
{
  return m_status;
}

inline void
Remote_SVPT::set_status (sqluint8  a_new_status)
{
  m_status =  a_new_status;
}
#endif
