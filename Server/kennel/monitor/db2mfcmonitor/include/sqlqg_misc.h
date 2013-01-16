/******************************************************************************
**
** Source File Name: sqlqg_misc.h
**
** (C) COPYRIGHT International Business Machines Corp. 1987, 2003
** All Rights Reserved
** Licensed Materials - Property of IBM
**
** US Government Users Restricted Rights - Use, duplication or
** disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
**
** Function = Include File defining:
**              Query Gateway memory support classes
**              QG hash utility class
**              QG system values
**
** Operating System: All
**
*******************************************************************************/
#ifndef SQL_H_SQLQG_MISC
#define SQL_H_SQLQG_MISC

#include <stdlib.h>
#include "sql.h"

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

// Forward declarations
struct Hash_Table_Entry_Allocation;
class Hash_Entry;
class Hash_Table;
class Sqlqg_Base_Class;
struct SMemPool;

enum Sqlqg_DeleteType
{
  SQLQG_DELETE_ALL,       // Delete both eng and fmp sides
  SQLQG_DELETE_FMP_ONLY,  // Delete only fmp side
  SQLQG_DELETE_ENG_ONLY   // Delete only eng side
};

////////////////////////////////////////////////////////////////////////////////
// enum Sqlqg_LOBflags: LOB flags
// Function: Enumerated type for different flags used to support LOB data type.
//           The Meanings of the flags are as follows:
//
//          SQLQG_NON_LOB  :   No LOB column in SELECT LIST
//          SQLQG_LOB_INIT :   Flag to initialize to query LOB 
//          SQLQG_LOB_NEW  :   Nnew lob column in a SELECT LIST
//          SQLQG_LOB_MORE :   Have another piece of lob data to fetch
//          SQLQG_LOB_LAST :   Last  piece of data for this lob column
//          SQLQG_ROW_DONE :   The whole row including lob is fetched
////////////////////////////////////////////////////////////////////////////////
enum Sqlqg_LOBflags{
    SQLQG_NON_LOB  =  0x0001, 
    SQLQG_LOB_INIT =  0x0002,
    SQLQG_LOB_NEW  =  0x0004,
    SQLQG_LOB_MORE =  0x0008,
    SQLQG_LOB_LAST =  0x0010,
    SQLQG_ROW_DONE =  0x0020 
};

#define SQLQG_LOB_FLAG   (SQLQG_LOB_INIT| \
                          SQLQG_LOB_NEW | \
                          SQLQG_LOB_MORE| \
                          SQLQG_LOB_LAST)

////////////////////////////////////////////////////////////////////////////////
// enum Sqlqg_OpType: Operation type
// Function: Enumerated type for differentiating operations on catalog objects.
//           Used when calling the sqlqgFindXXX functions to convey the caller's
//           intended use of the object.  Meanings are as follows:
//             sqlqg_Read:   object will be used to access remote data.  A
//                           connection to a remote source may be established.
//                           The object should not be altered or dropped until
//                           after this UOW has completed, and will be marked
//                           as (validated and) busy.
//             sqlqg_Update: object will be modified by DDL.  Operation should
//                           be rejected if the object has been used to access
//                           remote data in this UOW (i.e. if the object has
//                           been marked busy).  Object will not be marked
//                           busy, and validated mark will be cleared (to
//                           force reload on next use).
//             sqlqg_Simple: object will be examined but not modified by DDL.
//                           If a connection to a remote source is established,
//                           it will be closed before this operation
//                           terminates.  Object will be marked validated (but
//                           not busy).
//             sqlqg_Alter:
//             sqlqg_Drop:
////////////////////////////////////////////////////////////////////////////////
enum Sqlqg_OpType {
  sqlqg_Read,
  sqlqg_Update,
  sqlqg_Simple,
  sqlqg_Alter,
  sqlqg_Drop};

/////////////////////////////////////////////////////////////////////
// A return code.
//
// New functions should use this as the return code type. Older
// functions do not currently use this, but might be changed in
// the future.
/////////////////////////////////////////////////////////////////////

typedef sqlint32 Sqlqg_Return_Code;

// Maximum length of a timestamp in characters
#define SQLQG_MAX_TIMESTAMP_LEN      51

////////////////////////////////////////////////////////////////////////////////
// class Sqlqg_Base_Class
// Function: Abstract class that helps manage storage.  All gateway and
//           heap-allocated wrapper classes should be subclases of this.
////////////////////////////////////////////////////////////////////////////////
class Sqlqg_Base_Class {
public:

  // Overload new/delete to do memory management using sqlogblk and
  // sqlofmblk.  Storage will be allocated from SQLQG_BASE_POOL, defined
  // above.  If a derived class wants storage from somewhere else, it
  // must overload new/delete again.
  static void *operator new (size_t    a_size,
                             sqlint32* r_rc);
  static void *operator new (size_t);  // This is a no-op; some compilers
                                       // require that this be overridden
                                       // when a placement new is specified.
  static void operator delete (void* a_pointer);

};

////////////////////////////////////////////////////////////////////////////////
// class Sqlqg_Cacheable_Object
// Function: Base class for objects that can be constructed from cached
//           catalog information.  This class implements a timestamp that
//           records when the object was created from catalog data, and a
//           flag that indicates whether the object has been validated
//           against the cache during the current unit-of-work.
////////////////////////////////////////////////////////////////////////////////
class Sqlqg_Cacheable_Object: public Sqlqg_Base_Class {
public:

  // Enumeration type for the states of a DJ object.
  enum Sqlqg_ObjState {sqlqg_Check, sqlqg_Valid, sqlqg_Busy};

  // Constructor.
  Sqlqg_Cacheable_Object (void);

  Sqlqg_ObjState get_validation_state (void);

  // Updates the state as appropriate given a specific operation.
  // Returns SQLQG_EBUSY if the requested operation cannot be applied
  // given the current state.  valid_now will be set to TRUE if the
  // object has already been validated against the cache (i.e., if
  // current state is sqlqg_Valid or sqlqg_Busy).

  // The following matrix gives the new state for each combination of
  // old state and operation request.
  // Operation types: S = sqlqg_Simple (stateless read)
  //                  R = sqlqg_Read (may leave open connection)
  //                  U = sqlqg_Update (modified by DDL)
  // States: C = sqlqg_Check (updated or not yet validated; check cache)
  //         V = sqlqg_Valid (validated, no open connection possible)
  //         B = sqlqg_Busy (validated, may be open connection)
  //         X = operation REJECTED; state remains sqlqg_Busy
  //
  //                     Op Requested
  //                    || S | R | U
  //                  ===============
  //                  C || V | B | C
  //                  ---------------
  // Current state    V || V | B | C
  //                  ---------------
  //                  B || B | B | X
  sqlint32 set_validation_state (Sqlqg_OpType   a_op,
                                 unsigned int*  a_valid_now);

  sqlint32 reset_validation_state (void);

  sqluint8 *get_tstamp (void);

  // Copies the timestamp into the object.  Caller is free to delete
  // her copy of the timestamp.
  void set_tstamp (sqluint8*  a_new_tstamp);

  // Sets drop-pending flag.
  sqlint32 mark_drop_pending (void);

  // Tests drop_pending flag
  sqlint16 is_drop_pending (void);

protected:

  // Trackes whether this object has been validated against the DJ
  // cache in this unit of work, and whether thee may be an open
  // connection to a data source that requires the continued existence
  // of this object.
  Sqlqg_ObjState validation_state;

  // Timestamp obtained when this object was created from info in the
  // DJ cache.  If the timestamp in the cache is bigger, this object
  // is stale and must be recreated.  See sqlqgValidateXXX.
  sqluint8 object_tstamp[SQLQG_MAX_TIMESTAMP_LEN];

  // Drop-pending flag.  If set, object will be deleted at commit or
  // rollback.
  sqlint16 drop_pending;
};

///////////////////////////////////////////////////////////////////////////////
// Some useful functions
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// deconstruct_version()
//
// Given a string with a valid version of the form xxxx.yyyy.zzzz, this function
// will extracts the first (SQLQG_VERSION_MAX - 1) chars from the major version,
// first (SQLQG_VERSION_MAX -1 ) chars from minor version, and first
// (SQLQG_VERSION_MAX - 1) from release. This function expects the caller to
// provide sqluint arrays for the major, minor, and the release. These arrays
// should be of length SQLQG_VERSION_MAX. These arrays will be cleared, values
// will be copied into them, and will be NULL terminated.
//
///////////////////////////////////////////////////////////////////////////////
#define SQLQG_VERSION_MAX 5
sqlint32 deconstruct_version (sqluint8*  a_version,
                              sqluint8*  a_major,
                              sqluint8*  a_minor,
                              sqluint8*  a_release);

///////////////////////////////////////////////////////////////////////////////
// is_yes_no()
//
// Returns 1 if the string or char is either a Y, y, N, or N, and 0 otherwise.
// String or char will be converted to upper case if it is a valid Yes or No.
///////////////////////////////////////////////////////////////////////////////
sqlint32 is_yes_no (sqluint8*       a_string,
                    const sqlint32  a_is_null_terminated = TRUE);

///////////////////////////////////////////////////////////////////////////////
// Functions for dealing with multi-part names, where each part can
// contain any character.  To generate a unique name from parts, each
// part is enclosed between occurrences of a delimiter character, and
// parts are separated by a separator character WHICH MUST NOT BE THE
// SAME AS THE DELIMITER!!!.  Within each part, occurrences of the
// delimiter are escaped by doubling them.  For example:
// Delimiter: "
// Separator: .
// Part 1:	Walter
// Part 2:	"Fats"
// Part 3:	Smith
// Combined name: "Walter"."""Fats"""."Smith"
///////////////////////////////////////////////////////////////////////////////

// Delimiter character.  Each part will begin and end with this
// character, and internal occurrences will be doubled.
#define SQLQG_NAME_DELIMITER '"'

// Separator character.  This character will be placed between
// adjacent parts.
#define SQLQG_NAME_SEPARATOR '.'

///////////////////////////////////////////////////////////////////////////////
// Generic hash table.
//
// This class can be used to store and retrieve (key, value) pairs.
// Hash_Table is an array of size SQLQG_HASH_SIZE, and its entries are
// of type Hash_Entry*, and each element of the array is a linked list of
// Hash_Entry instances.
//
// The key can either be a null-terminated string, or a non-null terminated
// string that is the same length for all entries.  By default, the key is
// assumed to be null terminated.  If the key is non-null terminated and of a
// fixed size, pass the length of the string in to the constructor.
//
// A hash function is used to determine the position of an entry in the table.
// The value of an entry is assumed to be a pointer to an instance of a
// subclass of SQLQG_Base_Class (e.g. Wrapper, Server, etc.). The caller is
// responsible for managing the storage associated with the key and the
// SQLQG_Base_Class instance.
//
// The Hash_Table class maintains a list of pre-allocated entries that are
// filled in when an entry is added to the table.  When an entry is deleted
// from the table, the entry is returned to the list of pre-allocated entries
// and may be used again the next time an entry is added to the table.  The hash
// table will automatically allocate new entries when its list of pre-allocated
// entries have been used up.
//
// The keys that identifies entries need not be unique, and therefore it is up
// to the caller to determine whether or not a duplicate key is an error.
// If the caller is sure that keys are unique, the routine get_value() may be
// called to obtain the value associated with a key.
// If keys are not unique, the caller may call get_first_value() and
// get_next_value() to retrieve the set of values associated with a particular
// key.
//
///////////////////////////////////////////////////////////////////////////////
// The hash size is  chosen  because  it  will have good cover for SQQG objects
// with many entities (i.e., Remote_Function) and  objects with only relatively
// small number of entities(i.e., Connection)cached in the span of a connection
// to the federated server. Of course, this number is a prime.
// For hash  entry allocation,  we allocate a block of them (100) at a time and
// then place them in the free entry list to be ready for use.
///////////////////////////////////////////////////////////////////////////////
#define SQLQG_HASH_SIZE 277
#define SQLQG_NUM_HASH_ENTRIES_PER_ALLOCATION 100

class Hash_Table {
public:
  // Storage management.  Hash_Table will be allocated from
  // SQLQG_BASE_POOL by default, but a different pool can be
  // specified.  If a different pool is requeted, before to pass the
  // same pool to the constructor so that Hash_Entry objects will be
  // allocated from there as well.
  static void* operator new (size_t     a_size,
                             sqlint32*  a_rc,
                             SMemPool*  a_pool);
  static void* operator new (size_t     a_size,
                             sqlint32*  a_rc);
  static void* operator new (size_t); // No-op for platform compatibility
  static void operator delete (void*  a_pointer);

  // Constructors.
  Hash_Table (sqlint32*  a_rc,
              sqlint32   a_num_table_entries=20,
              sqlint16   a_key_size=-1);

  // If you specify a pool, be sure to also specify this pool to
  // operator new.
  Hash_Table (sqlint32*  a_rc,
              SMemPool*  a_entry_pool,
              sqlint32   a_num_table_entries=20,
              sqlint16   a_key_size=-1);

  // Destructor
  ~Hash_Table (void);

  sqlint32 add_entry (sqluint8*          a_key,
                      Sqlqg_Base_Class*  a_value);
                                       // Adds an entry to table.

  sqlint32 replace_entry (sqluint8*           a_key,
                          Sqlqg_Base_Class*   a_value,
                          Sqlqg_Base_Class**  a_replaced_value);
                                       // Adds an entry to table, replacing
                                       // any previous value with same key.
                                       // If a value is replaced, the replaced
                                       // value is returned in replaced_value.
                                       // The caller is reponsible for deleting
                                       // it.

  sqlint32 remove_entries_by_key (sqluint8*  a_key);
                                       // Removes ALL entries associated
                                       // with a key from the table.

  void remove_specific_entry (sqluint8*          a_key,
                              Sqlqg_Base_Class*  a_value);
                                       // Removes entry with a specific key and
                                       // value from table.

  sqlint32 get_value (sqluint8*           a_key,
                      Sqlqg_Base_Class**  a_value);
                                       // Gets value associated with key and
                                       // makes sure the value is unique.

  sqlint32 get_first_value (sqluint8*           a_key,
                            Sqlqg_Base_Class**  a_value,
                            void**              a_state=NULL);
                                       // Gets first entry with a
                                       // specific key value.
                                       // state contains state information
                                       // required by get_next_table_entry.

  sqlint32 get_next_value (sqluint8*           a_key,
                           Sqlqg_Base_Class**  a_value,
                           void**              a_state);
                                       // Gets the next entry with a
                                       // specific key value, based on
                                       // state information.

  sqlint32 get_first_entry (sqluint8**          a_key,
                            Sqlqg_Base_Class**  a_value,
                            void**              a_state);
                                       // Gets the first entry in hash table.

  sqlint32 get_next_entry (sqluint8**          a_key,
                           Sqlqg_Base_Class**  a_value,
                           void**              a_state);
                                       // Gets the next entry in hash table,
                                       // based on the current entry.

  sqlint32 clear (void);               // Clears all entries from hash table.
                                       // Return the entries to the free list.
                                       // Note: does NOT delete keys and values!
                                       // Caller must do this herself first.

  sqlint32 hash (sqluint8*  a_key);
                                       // Generates a hash value from the
                                       // given key.

  // Method to make unique keys from an arbitrary number of parts.
  // Each part can contain any character except a null, and
  // zero-length parts are allowed.  Null parts are NOT allowed.  The
  // form of the combined key wll be:
  //   <d><part1><d><s><d><part2><d><s> ... <s><d><partN><d>
  // where:
  //    <d> is the delimiter character
  //    <s> is the separator character
  // Occurrences of <d> within a part are escaped by doubling them.
  // For example, suppose the following are specified:
  //   delimiter: ' (single quote)
  //   separator: . (period)
  //   part1: John
  //   part2: F.
  //   part3: O'Malley
  // Then the delimited key would be:
  //           'John'.'F.'.'O''Malley'
  // The parts are specified using an array of null-terminated strings
  // (part_array) and a part count (n_parts).  The caller is
  // responsible for freeing the storage allocated to hold the
  // combined key.
  static sqlint32 make_delimited_key (sqluint8**  a_part_array,
                                      sqlint32    a_n_parts,
                                      sqluint8**  a_combined_key,
                                      sqluint8    a_delim = SQLQG_NAME_DELIMITER,
                                      sqluint8    a_sep = SQLQG_NAME_SEPARATOR);

  // Breaks a delimited key into its component parts, removing and
  // un-escaping delimiters.  The parts are returned via part_array, a
  // pointer array of size max_parts, which must be allocated (and
  // freed) by the caller.  Each part will be returned in part_array
  // as a null-terminated string, whose storage becomes the property
  // of the caller.  The actual number of parts filled in will be
  // returned as n_parts.  If there are more than max_parts parts, an
  // error will be returned.
  static sqlint32 decompose_delimited_key (sqluint8*   a_combined_key,
                                           sqluint8**  a_part_array,
                                           sqlint32    a_max_parts,
                                           sqlint32*   a_n_parts,
                                           sqluint8    a_delim = SQLQG_NAME_DELIMITER,
                                           sqluint8    a_sep = SQLQG_NAME_SEPARATOR);

private:
  //////////
  // Data //
  //////////
  sqlint32                      num_entries;       // max num table entries.
  sqlint16                      fixed_key_size;    // key length in bytes
  SMemPool*                     entry_pool;        // new entry storage
  Hash_Table_Entry_Allocation*  hash_entry_block;
  Hash_Entry*                   empty_chain_head;  // head of empty entry chain.
  Hash_Entry*                   table[SQLQG_HASH_SIZE];  // Hash table

  ////////////
  // Methods.
  ////////////
  sqlint32 get_empty_entry (Hash_Entry**  a_empty_entry);
                                       // Finds an unused entry on the empty
                                       // entry chain and removes it.

  sqlint32 return_entry (Hash_Entry*  a_entry);
                                       // Adds previously used entry to chain.

  sqlint32 add_hash_entry_after (sqlint32     a_hash_index,
                                 Hash_Entry*  a_prev_entry,
                                 Hash_Entry*  a_new_entry);
                                       // Adds entry to hash chain.

  sqlint32 remove_hash_entry (Hash_Entry*  a_hash_entry);
                                       // Removes entry from hash chain.

  sqlint32 find_hash_entry_position (sqlint32      a_hash_index,
                                     sqluint8*     a_key,
                                     Hash_Entry**  a_hash_entry);
                                       // Finds position for first entry
                                       // for key in hash chain.  If no
                                       // entry exists, it returns null.

  sqlint32 get_first_table_entry (Hash_Entry**  a_hash_entry);
                                       // Gets the first entry in hash table.
  sqlint32 get_next_table_entry (Hash_Entry**  a_hash_entry);
                                       // Gets the next entry in hash table,
                                       // based on the current entry.

  // Utility methods used by make_delimited_key

  // Returns the length of the string after it has been bracketed by the
  // delimiting character and internal occurrences of this character
  // have been doubled
  static sqlint32 delimited_length (sqluint8*  a_string,
                                    sqluint8   a_delim = SQLQG_NAME_DELIMITER);

  // Copies the input string to the output buffer, bracketed by the
  // delimiting character, and with internal occurrences doubled.
  // MODIFIES THE BUFFER POINTER to point to the next free byte.
  static sqlint32 copy_delimit_and_escape (sqluint8*   a_string,
                                           sqluint8**  a_bufp,
                                           sqluint8    a_delim = SQLQG_NAME_DELIMITER);

  // Copies the input string to the output buffer, removing the
  // bracketing delimiters and un-doubling any escaped occurrences of
  // the delimiter within the string.  The input string must be
  // well-formed, and terminated either by a null byte or an
  // occurrence of the separator character.  MODIFIES THE INPUT BUFFER
  // POINTER.  The input pointer will point to the character following
  // the closing delimiter of the input string (either the separator
  // or a null)
  static sqlint32 copy_and_strip (sqluint8**  a_string,
                                  sqluint8*   a_bufp,
                                  sqluint8    a_delim = SQLQG_NAME_DELIMITER,
                                  sqluint8    a_sep   = SQLQG_NAME_SEPARATOR);

};

///////////////////////////////////////////////////////////////////////////////
// Entry in hash table.
// This class does NOT manage the storage for the key or the entry.
///////////////////////////////////////////////////////////////////////////////
class Hash_Entry {
public:
  static void* operator new (size_t     a_size,
                             sqlint32*  a_rc);
  static void* operator new (size_t     a_size,
                             sqlint32*  a_rc,
                             SMemPool*  a_pool);
  static void* operator new (size_t); // No-op for compiler compatibility
  static void  operator delete (void* a_pointer);

  // Constructor
  Hash_Entry (sqluint8*         a_key=NULL,
              Sqlqg_Base_Class* a_value=NULL);

  // Destructor
  ~Hash_Entry (void);

  // Sets up all relevant data members.
  sqlint32 set (sqlint32          a_hash,
                sqlint16          a_is_on_hash,
                sqluint8*         a_key,
                Sqlqg_Base_Class* a_value);

  // Clears out all relevant data members;
  sqlint32 clear (void);

  // Accessor methods.
  sqlint16 is_on_hash (void);
  sqlint32 set_is_on_hash (sqlint16  a_is_on_hash);

  sqlint32 get_hash_value (void);
  sqlint32 set_hash_value (sqlint32  a_hash_value);

  sqluint8* get_key (void);
  sqlint32 set_key (sqluint8*  a_key);

  Sqlqg_Base_Class* get_value (void);
  sqlint32 set_value (Sqlqg_Base_Class*  a_value);


  Hash_Entry* get_prev_on_hash_chain (void);
  sqlint32 set_prev_on_hash_chain (Hash_Entry*  a_hash_entry);

  Hash_Entry* get_next_on_hash_chain (void);
  void set_next_on_hash_chain (Hash_Entry*  a_hash_entry);

  Hash_Entry* get_prev_on_empty_chain (void);
  sqlint32 set_prev_on_empty_chain (Hash_Entry*  a_hash_entry);

  Hash_Entry* get_next_on_empty_chain (void);
  void set_next_on_empty_chain (Hash_Entry*  a_hash_entry);

  friend class Hash_Table;

protected:

  //////////
  // Data //
  //////////
  sqlint32           hash;         // hash value for this entry.
  sqlint16           on_hash;      // whether or not entry is in hash table.
  sqluint8*          key;          // entry key (i.e. hash_value = hash(key)).
  Sqlqg_Base_Class*  value;        // value stored for entry.  must be
                                   // subclass of SQLQG_Class.

  // Pointers to maintain the hash chain for a given
  // hash value.
  Hash_Entry*        prev_on_hash_chain;
  Hash_Entry*        next_on_hash_chain;
  Hash_Entry*        prev_on_empty_chain;
  Hash_Entry*        next_on_empty_chain;
};

///////////////////////////////////////////////////////////////////////////////
// Entry block in hash table.
///////////////////////////////////////////////////////////////////////////////
struct Hash_Table_Entry_Allocation
{
   Hash_Table_Entry_Allocation*   mNextOnAllocationChain;
   Hash_Entry                     mEntries[SQLQG_NUM_HASH_ENTRIES_PER_ALLOCATION];
};

//////////////////////////////////////////////////////////////////////
//
// Sqlqg_Data_Buffer
// -----------------
//
// Data buffer for interaction with unsafe code.
//
// This data buffer class supports storing multiple rows of data,
// each with the same column format.  Values are enclosed in
// "wall" values to make sure if a buffer overflow happens that
// we know about it, and can then react appropriately.  Also, by
// co-locating a bunch of unsafe data, buffer overflows aren't
// likely to wipe out data we assume is correct.
//
// This is mainly intended to allow wrappers to bind in/out values
// to data source clients that may write too much data under some
// circumstances.
//
// e.g., When a nickname column is altered locally from CHAR(100) to CHAR(1) --
// some data sources (e.g. MED sources) aren't aware of the local length
// limit, and may try to write out the whole 100 bytes.
//
// By providing an interface to copy the data, this class also
// performs data length checking to make sure that controlled writes
// aren't going to cause a buffer overflow.
//
//////////////////////////////////////////////////////////////////////
class Sqlqg_Data_Buffer : public Sqlqg_Base_Class
{
 public:

  // Constructor
  Sqlqg_Data_Buffer(sqlint32  a_num_rows,    /* num rows to alloc for       */
		    sqlint32  a_num_columns, /* num cols per row to store   */
		    sqlint32* a_col_lengths, /* array of num_columns length */
		    sqlint32* a_rc);

  // Destructor
  ~Sqlqg_Data_Buffer (void);

  // Get a pointer to the data buffer for a specific column in a row
  sqlint32 get_pointer(sqlint32   a_row_num,
		       sqlint32   a_col_num,
		       void**     a_ptr);    /* ptr to the ptr to return */

  // Copy a column's value to another address
  sqlint32 get_data(sqlint32  a_row_num,
		    sqlint32  a_col_num,
		    void*     a_buffer,    /* buffer to copy data to */
		    sqlint32  a_buf_len,   /* size of a_buffer avail */
		    sqlint32* a_data_len); /* amount of data copied  */

  // Copy data from an outside address into a column
  sqlint32 set_data(sqlint32  a_row_num,
		    sqlint32  a_col_num,
		    void*     a_buffer,    /* buffer containing data */
		    sqlint32  a_buf_len,   /* size of data to copy   */
		    sqlint32* a_data_len); /* amount of data copied  */

  sqlint32 get_num_rows (void);
  sqlint32 get_num_cols (void);

 private:

  // Pointer to the block of memory managed by this class that
  // contains all of the values.
  sqluint8*  m_data_buffer;

  // Copies of info passed to the constructor
  sqlint32  m_num_rows;
  sqlint32  m_num_cols;
  sqlint32* m_col_lens; /* array of column lengths */

  sqlint32  m_row_size;
  sqlint32  m_total_size;

  // The data to put between fields is a member variable
  sqluint8 m_eyecatcher;
};


//-------------------------------------------------------------------
// Return codes from sqlqgConvert and
// UnfencedRelational_Server::convert_my_value
//-------------------------------------------------------------------

// The buffer is too small to hold the converted value.
#define SQLQG_BUFFER_OVERFLOW 1

// The value cannot be converted.
#define SQLQG_CANNOT_CONVERT (-1)
//-------------------------------------------------------------------

// Returned by Catalog_Info accessor methods when requested
// attribute's value is undefined
#define SQLQG_NOVALUE 2

// Returned by sqlqgFindXXX when the target object is requested in
// update mode, but has already been used in read mode by the current
// transaction.
#define SQLQG_EBUSY 3

// Returned when connection to a server is lost
#define SQLQG_LOST_CONNECTION 4

/***************************************************************************/
/* EOS = end of statement (as opposed to end of query - 1 statement to     */
/*                         DJ can result in multiple DJ to DS queries)     */
/* EOQ = end of query, still in statement (i.e. inner loop of remote       */
/*                                         nested loop)                    */
/* EOA = end-of-all.  We are done with statement, free everything          */
/***************************************************************************/
#define SQLQG_CLOSE_EOS  0
#define SQLQG_CLOSE_EOQ  1
#define SQLQG_CLOSE_EOA  2

#define SQLQG_UNREADY    0
#define SQLQG_READY      1
#define SQLQG_OPEN       2
#define SQLQG_EOF        3

#define SQLQG_SERVER_OPTION   "SERVER"
#define SQLQG_COLUMN_OPTION   "COLUMN"
#define SQLQG_WRAPPER_OPTION  "WRAPPER"
#define SQLQG_NICKNAME_OPTION "NICKNAME"
#define SQLQG_USER_OPTION     "USER MAPPING"
#define SQLQG_FUNCTION_OPTION "FUNCTION MAPPING"
#define SQLQG_TABLE_OPTION    "TABLE"

///////////////////////////////////////////////////////////////////////////////
// Valid options for generic nicknames.
// The following is a list of currently supported nickname options for
// generic sources.
//
// For each option, we describe the following information:
//       Description: meaning of the option.
// Optional/Required: Whether it is optional or required, and for what class
//                    of servers.
//     Default value: Default value for option, if there is one.
//      Valid values: valid values for the option.
//       Supplied by: who supplies the value
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//            Option: ADVANCE_COST
//       Description: Advance cost associated with nickname default cost
//                    model.
// Optional/Required: Optional
//     Default value: NONE
//      Valid values: Unsigned (positive) integeger
//       Supplied by: DDL statement
///////////////////////////////////////////////////////////////////////////////
#define SQLQG_GENNICKNAME_ADVANCE_COST_OPTION "ADVANCE_COST"

///////////////////////////////////////////////////////////////////////////////
//            Option: SETUP_COST
//       Description: Setup cost associated with nickname default cost
//                    model.
// Optional/Required: Optional
//     Default value: NONE
//      Valid values: Unsigned (positive) integeger
//       Supplied by: DDL statement
///////////////////////////////////////////////////////////////////////////////
#define SQLQG_GENNICKNAME_SETUP_COST_OPTION "SETUP_COST"

///////////////////////////////////////////////////////////////////////////////
//            Option: SUBMISSION_COST
//       Description: Submission cost associated with nickname default cost
//                    model.
// Optional/Required: Optional
//     Default value: NONE
//      Valid values: Unsigned (positive) integeger
//       Supplied by: DDL statement
///////////////////////////////////////////////////////////////////////////////
#define SQLQG_GENNICKNAME_SUBMISSION_COST_OPTION "SUBMISSION_COST"

/***************************************************************************/
/* Note: Any changes to these constants may need to be reflected in the    */
/*       corresponding column declarations in sqlrlcon.h                   */
/*       If the corresponding cloumns in catalog change, these values need */
/*       to be in sync.                                                    */
/***************************************************************************/
// server type
#define SQLQG_MAX_SERVER_TYPE_LEN              (30+1)

// server version
#define SQLQG_MAX_SERVER_VERSION_LEN           (SQL_LG_IDENT+1)

// wrapper (protocol) name
#define SQLQG_MAX_WRAPPER_LEN                  (SQL_MAX_IDENT+1)

// server name
#define SQLQG_MAX_SERVER_NAME_LEN              (128+1)
// Server/table/index/column option setting
// (stored as option setting in many catalog tables)
#define SQLQG_MAX_OPTION_VALUE_LEN             (2048+1)

// Server/table/index/column option name
// (stored as option name in many catalog tables)
#define SQLQG_MAX_OPTION_NAME_LEN              (SQL_MAX_IDENT+1)

//-----------------------------------------------------------------
// Use this macro to get the length of a constant string.
//
// NOTE: The argument must be a constant string enclosed in quotes
// or an array of chars.  A char pointer will return incorrect
// results (namely, the size of a pointer minus 1).
//-----------------------------------------------------------------
#define SQLQG_CONST_STRLEN(a_const_string) (sizeof(a_const_string) - 1)
//-----------------------------------------------------------------

// Inline methods

// Sqlqg_Base_Class
inline void*
Sqlqg_Base_Class::operator new (size_t)
{
  return (void*)NULL;
}

// Sqlqg_Cacheable_Object
inline Sqlqg_Cacheable_Object::Sqlqg_ObjState
Sqlqg_Cacheable_Object::get_validation_state (void)
{
  return validation_state;
}

inline sqlint32
Sqlqg_Cacheable_Object::reset_validation_state (void)
{
  validation_state = sqlqg_Check;
  return 0;
}

inline sqluint8*
Sqlqg_Cacheable_Object::get_tstamp (void)
{
  return object_tstamp;
}

inline sqlint32
Sqlqg_Cacheable_Object::mark_drop_pending (void)
{
  drop_pending = TRUE;
  return 0;
}

inline sqlint16
Sqlqg_Cacheable_Object::is_drop_pending (void)
{
  return drop_pending;
}

// Hash_Table
inline void*
Hash_Table::operator new (size_t)
{
  return (void*)NULL;
}

// Hash_Entry
inline void*
Hash_Entry::operator new (size_t)
{
  return (void*)NULL;
};

inline sqlint16
Hash_Entry::is_on_hash (void)
{
  return on_hash;
}

inline sqlint32
Hash_Entry::set_is_on_hash (sqlint16 a_is_on_hash)
{
  on_hash = a_is_on_hash;
  return 0;
}

inline sqlint32
Hash_Entry::get_hash_value (void)
{
  return hash;
}

inline sqlint32
Hash_Entry::set_hash_value (sqlint32 a_hash_value)
{
  hash = a_hash_value;
  return 0;
}

inline sqluint8*
Hash_Entry::get_key (void)
{
  return key;
}

inline sqlint32
Hash_Entry::set_key (sqluint8* a_key)
{
  key = a_key;
  return 0;
}

inline Sqlqg_Base_Class*
Hash_Entry::get_value (void)
{
  return value;
}

inline sqlint32
Hash_Entry::set_value (Sqlqg_Base_Class* a_value)
{
  value = a_value;
  return(0);
}

inline Hash_Entry*
Hash_Entry::get_prev_on_hash_chain (void)
{
  return prev_on_hash_chain;
}

inline sqlint32
Hash_Entry::set_prev_on_hash_chain (Hash_Entry*  a_hash_entry)
{
  prev_on_hash_chain =  a_hash_entry;
  return 0;
}

inline Hash_Entry*
Hash_Entry::get_next_on_hash_chain (void)
{
  return next_on_hash_chain;
}

inline void
Hash_Entry::set_next_on_hash_chain (Hash_Entry*  a_hash_entry)
{
  next_on_hash_chain =  a_hash_entry;
}

inline Hash_Entry*
Hash_Entry::get_prev_on_empty_chain (void)
{
  return prev_on_empty_chain;
}

inline sqlint32
Hash_Entry::set_prev_on_empty_chain (Hash_Entry*  a_hash_entry)
{
  prev_on_empty_chain =  a_hash_entry;
  return 0;
}

inline Hash_Entry*
Hash_Entry::get_next_on_empty_chain (void)
{
  return next_on_empty_chain;
}

inline void
Hash_Entry::set_next_on_empty_chain (Hash_Entry*  a_hash_entry)
{
  next_on_empty_chain = a_hash_entry;
}

// Sqlqg_Data_Buffer
inline sqlint32
Sqlqg_Data_Buffer::get_num_rows (void)
{
  return m_num_rows;
}

inline sqlint32
Sqlqg_Data_Buffer::get_num_cols (void)
{
  return m_num_cols;
}

#endif
