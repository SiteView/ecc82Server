/******************************************************************************
**
** Source File Name: sqlqg_runtime_data_operation.h
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
**              managing runtime data
**
** Operating System: All
**
*******************************************************************************/
#ifndef SQL_H_SQLQG_RUNTIME_DATA_OPERATION
#define SQL_H_SQLQG_RUNTIME_DATA_OPERATION

#include "sqlqg_misc.h"
#include "sql.h"

#define SQL_STATEMENT_DEFAULT_INCREMENT 512
#define MAX_STATEMENT_LENGTH 32766

// Forward declarations.
class Request_Exp;
struct sqlri_rquery;
struct sqlri_riud;
struct sqlr_djx_savepoint_info;
struct sqlri_rpassthru;
class sqlnn_dat_TYPTYPE;
class sqlnq_fcs;
struct sqlri_rcol;
struct sqlri_rcollist;

/******************************************************************************
* Externalized data representation classes:
*
* It is important to keep their interfaces the same - one set for data
* description classes and few additional methods for the classes that also
* contain a buffer attribute. These classes are used by the wrapper writer, so
* we want them to use single interface for all data/data_description
* externalized by DB2
*
* Data description:
*    Request_Exp_Type        - each request exp. node
*    Runtime_Data_Desc       - data exchange during passthru
*
* Data:
*    Runtime_Data            - data exchange during runtime
*    Request_Constant        - request exp. constant node.
*
******************************************************************************/

//////////////////////////////////////////////////////////////////////////////
// Runtime_Data_Desc base class (used to represent a description of a
// data value to be exchanged between the Query Gateway and a wrapper).
// Used in passthru for the wrapper to return the type description back to DB2
//////////////////////////////////////////////////////////////////////////////
class Runtime_Data_Desc : public Sqlqg_Base_Class {
public:

  // Constructors
  Runtime_Data_Desc (void);

  Runtime_Data_Desc (sqlint32*          a_rc,
                     short              a_type,
                     int                a_max_length,
                     short              a_code_page,
                     short              a_null_ind,
                     unsigned char      a_data_precision=0,
                     unsigned char      a_data_scale=0,
                     sqluint8*          a_data_name=NULL,
                     short              a_name_length=0,
                     short              a_remote_type=0);

  Runtime_Data_Desc (sqlint32*          a_rc,
                     sqlnn_dat_TYPTYPE* a_dt);

  Runtime_Data_Desc (sqlint32*          a_rc,
                     sqlri_rcol*        a_runtime_desc);

  // Destructor
  ~Runtime_Data_Desc (void);


  unsigned char get_for_bit_data (void);

  short get_null_indicator (void);

  short get_data_type (void);

  int get_maximum_length (void);

  unsigned char get_precision (void);

  unsigned char get_scale (void);

  unsigned short get_codepage (void);

  unsigned char* get_name (void);

  short  get_name_length (void);

  short get_remote_type (void);

  friend class DJ_Wrapper_Utilities;

private:
  ////////////////
  // Data.
  ////////////////
  short           data_type;               // SQL type of data.
  short           length;                  // length of data.
  unsigned char   for_bit_data;            // data value is for bit data.
  unsigned char   precision;               // precision of data (if decimal).
  unsigned char   scale;                   // scale of data (if decimal).
  unsigned short  codepage;                // code page to interpret data.
  short           null_indicator;          // whether or not nulls are allowed.
  unsigned char   name[SQL_MAX_IDENT+1];   // column name.
  short           name_length;             // length of column name.
  short           remote_type;             // remote type (if any).

};

//////////////////////////////////////////////////////////////////////////////
// Runtime_Data_Desc_List (used to represent a set of data value
// descriptions to be exchanged between the query gateway and wrappers).
//////////////////////////////////////////////////////////////////////////////
class Runtime_Data_Desc_List : public Sqlqg_Base_Class {
public:
  // Constructor.
  // Initialize with room for a_num_values entries
  Runtime_Data_Desc_List (sqlint32* a_rc,
                          int       a_num_values=0);

  // Destructor
  // Destroys contained values as well
  ~Runtime_Data_Desc_List (void);

  int get_number_of_values (void);
  sqlint32 set_number_of_values (int a_count);

  Runtime_Data_Desc* get_ith_value (int a_index);
  sqlint32 set_ith_value (Runtime_Data_Desc*  a_desc,
                          int                 a_index);

  Runtime_Data_Desc* operator[] (unsigned int  a_index);

  Runtime_Data_Desc** get_values (void);

  void* get_values_addr (void);

  void set_values (Runtime_Data_Desc** a_new_values);

  friend class DJ_Wrapper_Utilities;

private:

  ////////////////
  // Data.
  ////////////////
  int                  number_of_values;   // Number of descriptions in list.
  Runtime_Data_Desc**  values;             // Array of description objects.
};


//////////////////////////////////////////////////////////////////////////////
// Request_Constant class (used to represent a data value to be
// exchanged between the Query Gateway and a wrapper during compile time (consts)
// This class MUST have exactly the same interface as Runtime_Data
//////////////////////////////////////////////////////////////////////////////
class Request_Constant  {

public:
  static Request_Constant* mk_request_constant (sqlnq_fcs* a_fcs);

  int get_actual_length (void);
  unsigned char* get_data (void);
  sqlint32   is_data_null (void);

  // from Runtime_Data_Desc
  unsigned char get_for_bit_data (void);
  short get_null_indicator (void);
  short get_data_type (void);
  int get_maximum_length (void);
  unsigned char get_precision (void);
  unsigned char get_scale (void);
  unsigned short get_codepage (void);
  unsigned char* get_name (void);
  short  get_name_length (void);

protected:
  ~Request_Constant (void);    // Unimplemented

private:
  Request_Constant (void);     // Unimplemented
};

//////////////////////////////////////////////////////////////////////////////
// Request_Exp_Type class - a type of a request expression
// The interface MUST be exactly the same as Runtime_Data_Desc
//////////////////////////////////////////////////////////////////////////////
class Request_Exp_Type  {

public:
  static Request_Exp_Type* mk_request_exp_type (Request_Exp* a_expression);

  unsigned char get_for_bit_data (void);
  short get_null_indicator (void);
  short get_data_type (void);
  int get_maximum_length (void);
  unsigned char get_precision (void);
  unsigned char get_scale (void);
  unsigned short get_codepage (void);
  unsigned char* get_name (void);
  short  get_name_length (void);

private:
  Request_Exp_Type (void);         // Unimplemented
  ~Request_Exp_Type (void);        // Unimplemented
};



//////////////////////////////////////////////////////////////////////////////
// Runtime_Data base class. Represent a data value to be
// exchanged between the Query Gateway and a wrapper during runtime
// The interface MUST match with Request_Constant interface
//////////////////////////////////////////////////////////////////////////////
class Runtime_Data  : public Sqlqg_Base_Class{
public:
  Runtime_Data (sqlint32*  a_rc,
               sqlri_rcol* a_runtime_data);

  ~Runtime_Data (void);

  int get_data_index (void);

  unsigned char get_invariant (void);

  void set_data_description (Runtime_Data_Desc* a_description);

  int get_actual_length (void);
  void set_actual_length (int a_length);

  unsigned char* get_data (void);
  sqlint32 set_data (unsigned char*  a_data_ptr,
                     int             a_copy_len,
                     const bool      a_pad_to_max_len = false);

  // Use for both semantic and "friendly arithmetic" nulls.  Will
  // return SQLQG_RC_E407 if column is not nullable
  sqlint32 set_data_null (void);
  // Clear the null indicator, including "friendly arithmetic" bits
  sqlint32 clear_null_indicator (void);
  // For "friendly arithmetic" nulls use one of these as well.  Will
  // return SQLQG_ERROR (for now) if column not set to null first
  sqlint32 set_friendly_div_by_0 (void);
  sqlint32 set_friendly_exception (void);

  // Will return TRUE for both semantic and "friendly arithmetic" nulls
  sqlint32 is_data_null (void);
  sqlint32 is_data_nullable (void);

  // Will return TRUE for semantic null only
  sqlint32 is_semantic_null (void);

  // These need not be checked unless is_data_null() returns TRUE
  sqlint32 check_friendly_div_by_0 (void);
  sqlint32 check_friendly_exception (void);

   unsigned char get_for_bit_data (void);
   short get_null_indicator (void);
   void  set_null_indicator (short a_indicator);
   short get_data_type (void);
   int get_maximum_length (void);
   unsigned char get_precision (void);
   void          set_precision (unsigned char a_precision);
   unsigned char get_scale (void);
   void          set_scale (unsigned char a_scale);
   unsigned short get_codepage (void);
   unsigned char* get_name (void);
   short  get_name_length (void);
   short  get_remote_type (void);
   int get_remote_length (void);
   unsigned char get_remote_precision (void);
   unsigned char get_remote_scale (void);

  friend class DJ_Wrapper_Utilities;
  friend class Runtime_Data_List;

private:
  ////////////////
  // Data.
  ////////////////

  sqlri_rcol* runtime_info;         // pointer to internal runtime information.

  ////////////////
  // Methods.
  ////////////////
  // For internal use only.
  sqlri_rcol* get_runtime_info (void);

protected:
  void set_runtime_info (sqlri_rcol *a_rcol);
};

//////////////////////////////////////////////////////////////////////////////
// Runtime_Data list (used to represent a set of data values to be exchanged
// between the query gateway and wrappers).
//////////////////////////////////////////////////////////////////////////////
class Runtime_Data_List : public Sqlqg_Base_Class {
public:
  // Constructor
  Runtime_Data_List (sqlint32*        a_rc,
                     sqlri_rcollist*  a_runtime_data_list);

  // Destructor
  ~Runtime_Data_List (void);

  int get_number_of_values (void);

  void refresh (sqlint32*       a_rc,
                sqlri_rcollist* a_runtime_values);

  Runtime_Data* get_ith_value (int a_index);

  Runtime_Data* operator[] (unsigned int a_index);

  friend class DJ_Wrapper_Utilities;

private:

  ////////////////
  // Data.
  ////////////////
  int number_of_values;       // Number of columns in list.
  Runtime_Data  **values;     // Array of column objects.
};

// Inline methods

// Runtime_Data_Desc
inline
Runtime_Data_Desc::Runtime_Data_Desc (void)
{}

inline unsigned char
Runtime_Data_Desc::get_for_bit_data (void)
{
  return for_bit_data;
}

inline short
Runtime_Data_Desc::get_null_indicator (void)
{
  return null_indicator;
}

inline short
Runtime_Data_Desc::get_data_type (void)
{
  return data_type;
}

inline int
Runtime_Data_Desc::get_maximum_length (void)
{
  return length;
}

inline unsigned short
Runtime_Data_Desc::get_codepage (void)
{
  return codepage;
}

inline unsigned char*
Runtime_Data_Desc::get_name (void)
{
  return name;
}

inline short
Runtime_Data_Desc::get_name_length (void)
{
  return name_length;
}

inline short
Runtime_Data_Desc::get_remote_type (void)
{
  return remote_type;
}

// Runtime_Data_Desc_List
inline int
Runtime_Data_Desc_List::get_number_of_values (void)
{
  return  number_of_values;
}

inline Runtime_Data_Desc**
Runtime_Data_Desc_List::get_values (void)
{
  return values;
}

inline void*
Runtime_Data_Desc_List::get_values_addr (void)
{
  return &values;
}

inline void
Runtime_Data_Desc_List::set_values (Runtime_Data_Desc** a_new_values)
{
  values = a_new_values;
}

// Runtime_Data
inline sqlri_rcol*
Runtime_Data::get_runtime_info (void)
{
  return runtime_info;
}

// Runtime_Data_List
inline int
Runtime_Data_List::get_number_of_values (void)
{
  return  number_of_values;
}
#endif
