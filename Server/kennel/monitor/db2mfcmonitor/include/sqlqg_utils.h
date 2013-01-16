/******************************************************************************
**
** Source File Name: sqlqg_utils.h
**
** (C) COPYRIGHT International Business Machines Corp. 1987, 2003
** All Rights Reserved
** Licensed Materials - Property of IBM
**
** US Government Users Restricted Rights - Use, duplication or
** disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
**
** Function = Include File defining:
**              Query Gateway utility routines
**
** Operating System: All
**
*******************************************************************************/
#ifndef SQL_H_SQLQG_UTILS
#define SQL_H_SQLQG_UTILS

/****************************************************************************
* Wrapper Utilities Base class
*
* This class represents the set of wrapper level utilities for use by
* all wrappers.  The class has no internal state, and all methods should
* be static.
****************************************************************************/

class Wrapper_Utilities {
public:

  // Splits a string into tokens.
  static char* string_to_tokens (char*       a_string,
                                 const char* a_sep,
                                 char**      a_last);

  // convert a string to upper case.
  static int convert_to_upper (char*        a_string,
                               size_t       a_length,
                               unsigned int a_codepage);

  // convert a string to lower case.
  static int convert_to_lower (char*        a_string,
                               size_t       a_length,
                               unsigned int a_codepage);

  // report an error.
  static sqlint32 report_error (const char* a_function_name,
                                int         a_sql_code,
                                int         a_number_of_tokens,
                                ...);

  // report a warning
  static sqlint32 report_warning (const char* a_function_name,
                                  int         a_warning_index,
                                  char        a_warning_flag,
                                  int         a_number_of_tokens,
                                  ...);

  // allocate memory.
  static int allocate (size_t a_size,
                       void** a_block);

  // deallocate a chunk of memory.
  static void deallocate (void* a_block);

  // get the single byte codepage.
  static int get_sb_DB_codepage (void);

  // get the double byte codepage.
  static int get_db_DB_codepage (void);

  // get the isolation level
  static sqluint8 get_isolation (void);

  // get the codepage.
  static sqlint16 get_codepage (void);

  // get the rqrioblk.
  static sqlint32 get_rqrioblk (void);

  // get the application control block status
  static sqlint32 get_app_cb_status (void);

  // get the datastream
  static sqluint16 get_datastream (void);

  // get the authid
  static char* get_authid (void);

  // get the authid len.
  static int get_authid_len (void);

  //traces a block of data on behalf of wrappers
  static void trace_data (int   a_probe,
                          void* a_data,
                          int   a_data_size);

  // get the db2 installation path
  static sqlint32 get_db2_install_path (char*  a_path,
                                        int    a_path_size);

  // get the db2 instance path
  static sqlint32 get_db2_instance_path (char*  a_path,
                                         int    a_path_size);

  // Get the DB2 release (including fix pack) that this wrapper
  // is running under.  The value returned is defined in header
  // file sql.h (for example, SQL_REL8103).  This value will be
  // updated at each DB2 fix pack.
  static int get_db2_release(void);

  //Wrapper tracing functions.

  //Trace entry point of a function
  static void fnc_entry (sqlint16     a_function_id,
                         const char*  a_function_name);

  //Trace exit point of a function
  static void fnc_exit (sqlint16     a_function_id,
                        const char*  a_function_name,
                        sqlint32    a_rc);

  //Trace single data element
  static void fnc_data (sqlint16     a_function_id,
                        const char*  a_function_name,
                        sqluint32    a_probe,
                        sqluint32    a_data1_size,
                        const void*  a_data1);

  //Trace two data elements
  static void fnc_data2 (sqlint16     a_function_id,
                         const char*  a_function_name,
                         sqluint32    a_probe,
                         sqluint32    a_data1_size,
                         const void*  a_data1,
                         sqluint32    a_data2_size,
                         const void*  a_data2);

  //Trace three data elements
  static void fnc_data3 (sqlint16     a_function_id,
                         const char*  a_function_name,
                         sqluint32    a_probe,
                         sqluint32    a_data1_size,
                         const void*  a_data1,
                         sqluint32    a_data2_size,
                         const void*  a_data2,
                         sqluint32    a_data3_size,
                         const void*  a_data3);

  //Trace error
  static void trace_error (sqlint16     a_function_id,
                           const char*  a_function_name,
                           sqluint32    a_probe,
                           sqluint32    a_data_size,
                           const void*  a_data);

  //Converts input data from the source code page to the target code page
  static sqlint32 convert_codepage(
                         sqluint8     **a_input_data,
                         sqluint32      a_input_len,
                         sqluint32      a_source_CP,
                         sqluint32      a_target_CP,
                         sqluint8      *a_output_data,
                         sqluint32     *a_output_len,
                         bool          *a_EBCDIC_is_DBCS,
                         sqluint32     *a_substitute );

  //Get an expected byte length of a converted string
  static sqluint32 get_expected_conv_len(
                         sqluint32 a_input_len, 
                         sqluint32 a_source_CP, 
                         sqluint32 a_target_CP);
                               
  //Obtain the language settings from the environment/system
  static sqluint32 get_env_lang(sqlint8 *a_buffer);


  //Change the endian-ness of a double-byte character string
  static void change_endian2(sqlint8 *a_source, sqluint32 a_source_len)
  {
    unsigned int i;
    for( i = 0; i < a_source_len; i+=2 )
    {
          char cSaved = a_source[i]; 
          a_source[i] = a_source[i+1]; 
          a_source[i+1] = cSaved;
    }
  }

  //----------------------------------------------------------------
  // Constants
  //----------------------------------------------------------------
  enum
  {
     CP_CONV_OK              = 0,
     CP_CONV_BUFFER_SMALL    = 1,
     CP_CONV_CP_SAME         = 2,
     CP_CONV_DBCS_TRUNCATE   = 3,
     CP_CONV_ERROR           = -1,
     CP_CONV_NOT_SUPPORTED   = -2
  };

};

#endif
