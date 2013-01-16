/******************************************************************************
**
** Source File Name: sqlqg_unfenced_generic_nickname.h
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
**              unfenced-side nickname objects
**
** Operating System: All
**
*******************************************************************************/
#ifndef SQL_H_SQLQG_UNFENCED_GENERIC_NICKNAME
#define SQL_H_SQLQG_UNFENCED_GENERIC_NICKNAME

#include "sqlqg_unfenced_nickname.h"

/* Forward declarations. */
class Nickname_Info;
class Unfenced_Generic_Server;

// Default values for the default cost model; the three "costs"
// are times expressed in milliseconds.  They are:
//  SQLQG_DEFAULT_CARDINALITY     The default number of rows
//                                for a nickname.
//  SQLQG_DEFAULT_ADVANCE_COST    The cost to fetch a row after
//                                the first row of a simple query
//  SQLQG_DEFAULT_SETUP_COST      The cost to set up a query the
//                                very first time
//  SQLQG_DEFAULT_SUBMISSION_COST The cost to submit a query, outside
//                                of the first-time setup cost.
enum {
   SQLQG_DEFAULT_CARDINALITY = 1000,
   SQLQG_DEFAULT_ADVANCE_COST = 50,
   SQLQG_DEFAULT_SETUP_COST = 250,
   SQLQG_DEFAULT_SUBMISSION_COST = 2000
};

class  Unfenced_Generic_Nickname : public UnfencedNickname {
public:

    // Constructor.
  Unfenced_Generic_Nickname (sqluint8*                a_schema,
                             sqluint8*                a_nickname_name,
                             Unfenced_Generic_Server* a_nickname_server,
                             sqlint32*                a_rc);

  //Nickname object initialization
  virtual sqlint32 initialize_nickname (Nickname_Info* a_cat_info);

  void     get_card (sqlint64 *a_card) const;
  sqlint32 get_advance_cost (void) const;
  sqlint32 get_setup_cost (void) const;
  sqlint32 get_submission_cost (void) const;
  
  // Handy methods to check if an option is defined at the generic
  // nickname level.
  virtual sqluint32 is_reserved_nickname_option (sqluint8* a_opt_name);
  virtual sqluint32 is_reserved_column_option (sqluint8* a_opt_name);

  // Verify options specified on ALTER nickname object.
  virtual sqlint32 verify_alter_nickname_info (Nickname_Info*  a_nick_info,
                                               Nickname_Info** a_delta_info);

  virtual sqlint32 verify_register_nickname_info (Nickname_Info*  a_nick_info,
                                                  Nickname_Info** a_delta_info);

protected:

  /////////////
  // Methods //
  /////////////

  // Destructor. Protect this to keep people from deleting it
  // directly; they should call server->drop_nickname() instead.
  virtual ~Unfenced_Generic_Nickname (void);

  //////////
  // Data //
  //////////
  sqlint64 m_cardinality;
  sqlint32 m_advance_cost;
  sqlint32 m_setup_cost;
  sqlint32 m_submission_cost;
};

// Inline methods
inline void
Unfenced_Generic_Nickname::get_card (sqlint64 *a_card) const
{
  *a_card = m_cardinality;
}

inline sqlint32
Unfenced_Generic_Nickname::get_advance_cost (void) const
{
  return m_advance_cost;
}

inline sqlint32
Unfenced_Generic_Nickname::get_setup_cost (void) const
{
  return m_setup_cost;
}

inline sqlint32
Unfenced_Generic_Nickname::get_submission_cost (void) const
{
  return m_submission_cost;
}
#endif
