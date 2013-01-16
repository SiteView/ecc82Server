/******************************************************************************
**
** Source File Name: sqlqg_wrapper.h
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
**              wrapper objects
**
** Operating System: All
**
*******************************************************************************/
#ifndef SQL_H_SQLQG_WRAPPER
#define SQL_H_SQLQG_WRAPPER

#include "sqlqg_misc.h"

// Forward declarations.
class Wrapper;
class Wrapper_Info;
class Server;
class Server_Info;

////////////////////////////////////////////////////////////////////////////////
// class Wrapper
//
// Function: Abstract base class representing wrapper objects
////////////////////////////////////////////////////////////////////////////////
class Wrapper : public Sqlqg_Cacheable_Object {
public:

  enum flags_bitmap 
   {
    SQLQG_WRAPPER_THREADSAFE           = 0x00000001,
    SQLQG_WRAPPER_FENCED_DRDA          = 0x00000002,
    SQLQG_WRAPPER_TRUSTED_DRDA         = 0x00000004,
    SQLQG_WRAPPER_TRUSTED_NONDRDA      = 0x00000008,
    SQLQG_WRAPPER_64BIT                = 0x00000010,
    SQLQG_WRAPPER_FENCED_NONDRDA       = 0x00000020
   };

  // Constructor
  Wrapper (sqlint32* a_rc,
           sqluint8  a_wrapper_type='R',
           sqlint32  a_wrapper_version=0);

  // Destructor.
  virtual ~Wrapper (void);

  // Accessor methods.
  sqluint8* get_name (void);

  sqluint8* get_corelib (void);

  sqluint8 get_type (void);

  sqlint32 get_version (void);

  // Initialization hook.  Invoked after creating wrapper object to
  // initialize state from catalog information. 
  virtual sqlint32 initialize_wrapper (Wrapper_Info* a_wrapper_info);
  
  // Methods to maintain use count.
  sqlint32 get_num_servers (void);

  // Method to instantiate a server
  Server* get_server (sqluint8*    a_server_name,
		      Server_Info* a_server_info,
		      sqlint32*    a_rc);
  
  sqlint32 drop_server (Server* a_server);

  sqlint32 drop_all_servers (void);

  sqlint32 drop_marked_servers (void);

  Server* get_first_server (void** a_state);
  Server* get_next_server  (void** a_state);

  Wrapper_Info* get_info (void);

  sqluint32 is_threadsafe (void);

  sqluint32 is_fenced (void);

  sqluint32 is_drda (void);

  sqluint32 is_nondrda (void);

  sqluint32 is_fenced_drda (void);

  sqluint32 is_fenced_nondrda (void);

  sqluint32 is_trusted (void);

  sqluint32 is_trusted_drda (void);

  sqluint32 is_trusted_nondrda (void);

  sqluint32 use_stp (void);

  virtual sqlint16 is_drda_kind_wrapper (void);

  virtual sqlint16 is_ready_for_fenced_mode (void);
  
  virtual bool is_safe_in_thread_mode(void) const;

  virtual bool is_creating_threads(void) const;

protected:

  // initialize_my_wrapper() allows a wrapper subclass instance to perform
  // any load time initialization.  The default implementation does
  // nothing.
  virtual sqlint32 initialize_my_wrapper (Wrapper_Info* a_wrapper_info);
                                     
  ////////////////
  // Data.
  ////////////////
  Wrapper_Info* m_info;              // Catalog information for wrapper
  sqluint8*     m_name;              // wrapper name.
  sqluint8*     m_corelib;           // library to load.

  sqluint8      m_type;              // Relational (R) or non-relational (N).
  sqlint32      m_version;           // version of wrapper code.
  
  sqlint32      m_flags;             // Threadsafe, Trusted, 64bit
  sqlint32      m_num_servers;       // Number of servers

private:

  // This method must be implemented by every instantiated subclass of
  // Wrapper.
  virtual Server* create_server (sqluint8* a_server_name, 
				 sqlint32* a_rc); 

  friend class UnfencedWrapper;
  Hash_Table    m_servers;
};

// Inline methods
inline sqluint8*
Wrapper::get_name (void)
{
  return m_name;
}

inline sqluint8*
Wrapper::get_corelib (void)
{
  return m_corelib;
}

inline sqluint8
Wrapper::get_type (void)
{
  return m_type;
}

inline sqlint32
Wrapper::get_version (void)
{
  return m_version;
}

inline sqlint32
Wrapper::get_num_servers (void)
{
  return m_num_servers;
}

inline Wrapper_Info*
Wrapper::get_info (void)
{
  return m_info;
}

inline sqluint32
Wrapper::is_threadsafe (void)
{
  return (m_flags & SQLQG_WRAPPER_THREADSAFE);
}

inline sqluint32
Wrapper::is_fenced (void)
{
  return (m_flags & (SQLQG_WRAPPER_FENCED_DRDA | 
                       SQLQG_WRAPPER_FENCED_NONDRDA));
}

inline sqluint32
Wrapper::is_drda (void)
{
  return (m_flags & (SQLQG_WRAPPER_TRUSTED_DRDA | 
                       SQLQG_WRAPPER_FENCED_DRDA));
}

inline sqluint32
Wrapper::is_nondrda (void)
{
  return (m_flags & (SQLQG_WRAPPER_TRUSTED_NONDRDA | 
                       SQLQG_WRAPPER_FENCED_NONDRDA));
}

inline sqluint32
Wrapper::is_fenced_drda (void)    
{
  return (m_flags & SQLQG_WRAPPER_FENCED_DRDA);
}

inline sqluint32
Wrapper::is_fenced_nondrda (void)    
{
  return (m_flags & SQLQG_WRAPPER_FENCED_NONDRDA);
}

inline sqluint32
Wrapper::is_trusted (void)    
{
  return (m_flags & (SQLQG_WRAPPER_TRUSTED_DRDA | SQLQG_WRAPPER_TRUSTED_NONDRDA));
}

inline sqluint32
Wrapper::is_trusted_drda (void)    
{
  return (m_flags & SQLQG_WRAPPER_TRUSTED_DRDA);
}

inline sqluint32
Wrapper::is_trusted_nondrda (void)    
{
  return (m_flags & SQLQG_WRAPPER_TRUSTED_NONDRDA);
}

inline sqluint32
Wrapper::use_stp (void)    
{
  return (m_flags & (SQLQG_WRAPPER_FENCED_DRDA |
                       SQLQG_WRAPPER_FENCED_NONDRDA | 
                       SQLQG_WRAPPER_TRUSTED_DRDA));
}

inline sqlint16
Wrapper::is_drda_kind_wrapper (void)
{
  return 0;
}

inline sqlint16
Wrapper::is_ready_for_fenced_mode (void)
{
  return 0;
}


#endif
