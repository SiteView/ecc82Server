/******************************************************************************
**
** Source File Name: sqlqg_request.h
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
**              representing requests and replies
**
** Operating System: All
**
*******************************************************************************/
#ifndef SQL_H_SQLQG_REQUEST
#define SQL_H_SQLQG_REQUEST


#define _GENERIC_SERVER_DEBUG_

#include "sql.h" 
#include "sqlqg_misc.h"

// Forward declarations
//sqlqg
class Nickname;
class Unfenced_Generic_Nickname;   
class Unfenced_Generic_Server;

//request
class Parsed_Query_Fragment;
class Request_Exp;
class Request_Constant;
class Request;
class Request_Exp_Type;

//other
struct sqlno_globals;
struct sqlno_qtb;
struct sqlno_iset;
struct sqlno_qgmP_array;
struct sqlno_apcb;
union  sqlno_set;
class  sqlnq_pid;

////////////////////////////////////////////////////////////////////////////////
//
//
//                                Request_Exp
//
//
////////////////////////////////////////////////////////////////////////////////
//
// Note: This class is an interface to underlying DB2 data structures.  It
// should not be instantiated or subclassed.
//
////////////////////////////////////////////////////////////////////////////////
class Request_Exp   {
public:
  enum kind { badkind, column, unbound, constant, oper};

  // No constructor
  
  //**** Start of the interface used by the wrapper writer ****
  
  sqlint32        get_kind (Parsed_Query_Fragment*  a_query_fragment,
                            kind*                   a_kind);
  
  sqlint32        get_kind (Request*   a_request,
                            kind*      a_kind);

  sqlint32        get_kind (kind*      a_kind);   // Does not distinguish
                                                  // between bound and unbound
                                                  // columns
  sqlint32        get_data_type (Request_Exp_Type** a_new_type);
  Request_Exp*    get_parent (void);
  Request_Exp*    get_next_child (void);
  sqlint32        get_handle (int* a_handle);    


  // For columns only
  sqlint32        get_column_name (char** a_col_name,
                                   int*   a_name_length);
  sqlint32        get_column_quantifier_handle (int*  a_quant_handle);

  
  //For constants only
  sqlint32        get_value (Request_Constant**  a_constant_value);


  //For operators only
  Request_Exp*    get_first_child (void);
  int             get_number_of_children (void);
  sqlint32        get_token (char**  a_operator_token,
                             int*    a_token_length);
  sqlint32        get_signature (char** a_signature);
  

  //**** End of the interface used by the wrapper writer ****


  //For use by the DB2 optimizer only
  sqlint32        get_kind (sqlno_iset* a_qun_iset,
                            kind*       a_kine);
  static sqlint32 get_pred_given_global_index (int           a_global_index,
                                               Request_Exp** a_predicate);
  static sqlint32 get_head_exp_given_global_index (Request*      a_rq,
                                                   int           a_global_index,
                                                   Request_Exp** a_head_exp);
protected:    
  // Constructor and operator new are protected and unimplemented
  // instantiation.
  static void* operator new (size_t);  // Unimplemented

  Request_Exp (void);                  // Unimplemented
  ~Request_Exp (void);                 // Unimplemented

};

////////////////////////////////////////////////////////////////////////////////
//
//
//                           Parsed_Query_Fragment
//
//
////////////////////////////////////////////////////////////////////////////////
class Parsed_Query_Fragment : public Sqlqg_Base_Class  {

public:


  //constructor
  Parsed_Query_Fragment (void);
  
  //functions to examine the size of the reply clauses
  int       get_number_of_quantifiers (void);  
  int       get_number_of_predicates (void);  
  int       get_number_of_head_exp (void);
  int       get_number_of_gb_exp (void);

  //Functions to access the handles at a position in the request
  sqlint32       get_quantifier_handle (int   a_quant_pos, 
                                        int*  a_quant_handle);
  sqlint32       get_predicate_handle (int   a_pred_pos,
                                       int*  a_pred_handle);
  sqlint32       get_head_exp_handle (int   a_head_exp_pos,
                                      int*  a_head_exp_handle);
  sqlint32       get_gb_exp_handle (int   a_gb_exp_pos,
                                    int*  a_gb_exp_handle);

  
  static sqlint32     get_nickname (int                          a_quant_handle,
                                    Unfenced_Generic_Nickname**  a_nickname);
  sqlint32     get_head_exp (int            a_head_exp_handle,
                             Request_Exp**  a_head_exp);
  sqlint32     get_predicate (int            a_pred_handle,
                              Request_Exp**  a_pred_exp);
  sqlint32     get_table_function_exp (int            a_quant_handle,
                                       Request_Exp**  a_table_exp);

  void set_distinct (int  a_distinct);
  int get_distinct (void);

  int is_generated_pid (int a_hexp_pos);
   
protected:
  //the sets for the query clauses
  sqlno_iset*       preds;
  sqlno_iset*       gb_exps;
  sqlno_iset*       hexps;
  sqlno_iset*       quns;
  class sqlnq_pid** m_pid_array;
  int               m_pid_array_size;
  int               distinct;

  //set management
  static sqlint32 add_to_set (sqlno_iset*, int);
  static sqlint32 get_set_element (sqlno_iset* a_set,
                                   int         a_index,
                                   int*        a_elmement);
  static void     copy_iset (sqlno_iset* a_source,
                             sqlno_iset* a_target);


  friend class Predicate_List;
  friend sqlint32    Request_Exp::get_kind (Parsed_Query_Fragment* a_query_fragment,
                                            Request_Exp::kind*     a_kind);
};


////////////////////////////////////////////////////////////////////////////////
//
//
//                                Request 
//
//
////////////////////////////////////////////////////////////////////////////////
class Request : public Parsed_Query_Fragment  {

public:

  // Constructor  - request constructed only by the optimizer
  Request (sqlno_qtb* a_optqtbP,
           sqlno_set* a_quns  = NULL,
	   sqlno_set* a_cols  = NULL,
	   sqlno_set* a_preds = NULL);


  // Destructor - requests destroyed by the optimizer
  virtual ~Request (void);
  
protected:

  //////////
  // Data //
  //////////
  sqlno_qtb* optqtbP;
  sqlno_set* quns_bit_set;

  /////////////
  // Methods //
  /////////////
  Request(void);                            // Unimplemented

  sqlint32 make_pid (int          a_global_index,
                     sqlnq_pid**  q_pid);

  friend sqlint32 Request_Exp::get_kind (Request*           a_request,
                                        Request_Exp::kind*  a_kind);
  friend sqlint32 Request_Exp::get_head_exp_given_global_index 
                                       (Request*      a_request,
                                        int           a_global_index,
                                        Request_Exp** a_head_exp);
  friend class Reply;
  friend class Predicate_List;
};

////////////////////////////////////////////////////////////////////////////////
//
//
//                                Reply 
//
//
////////////////////////////////////////////////////////////////////////////////

class Reply : public Parsed_Query_Fragment {
public:

  //**** Start of the interface used by the wrapper writer ****

  enum order_direction {ASC, DSC};


  //Reply allocation - called from create_empty_reply
  static void* operator new (size_t       a_size,
                             sqlint32*    a_rc,
                             sqlno_apcb*  a_poolP = NULL);

  // create a Reply of the same size as the Request
  Reply (Request*                  a_rq,
         Unfenced_Generic_Server*  a_server,
         sqlint32*                 a_rc,
         sqlno_apcb*               a_poolP= NULL);

  virtual ~Reply(void);

  //functions to populate the reply
  sqlint32 add_head_exp (int a_head_exp_handle);
  sqlint32 add_predicate (int a_pred_handle);
  sqlint32 add_quantifier (int a_quant_handle);
  sqlint32 add_gb_exp (int a_gb_exp_handle);
  sqlint32 add_order_entry (int                     a_gindex,
                            Reply::order_direction  a_direction);
  
  //Order info manipulation
  int       get_number_of_order_entries (void);
  sqlint32  get_order_entry (int                     a_pos,
                             int*                    a_gindexP,
                             Reply::order_direction* a_direction);

  //Get the execution descriptor and its size
  void get_exec_desc (void**  a_exec_desc,
                      int*    a_exec_desc_size);
  void set_exec_desc (void*   a_exec_desc,
                      int     a_exec_desc_size);

  inline short get_no_block();
  inline void  set_no_block();

  //parameter ordering
  sqlint32 get_parameter_order (int*   a_number_of_params,
                                int**  a_param_handle_array);
  
  //costing - these can be overriden by the wrapper writer
  virtual sqlint32 cardinality (float*  a_cardinality);
  virtual sqlint32 total_cost (float*   a_total_cost);
  virtual sqlint32 re_exec_cost (float* a_re_exec_cost);
  virtual sqlint32 first_tuple_cost (float* a_first_tuple_cost);
  virtual sqlint32 all_costs (float* a_total_cost,
                              float* a_first_tuple_cost,
                              float* a_re_exec_cost);
  //next reply in the list
  Reply* next (void);
  void  set_next_reply (Reply* a_next_reply);

  //Return the server object
  Unfenced_Generic_Server* get_server (void);

  //**** End of the interface used by the wrapper writer ****
  
  //To save the reply of the final plan 
  sqlint32 copy_reply (Reply**        a_new_reply,
                       sqlno_globals* a_optP,
                       sqlno_apcb*    a_poolP);


protected:

  //////////
  // Data //
  //////////
  sqlno_iset* order_info;       // order description
  void*       exec_desc;        // execution descriptor - the 'black box' with 
  int         exec_desc_size;   // the wrapper's execution info
  Reply*      next_reply;       // Chaining of replies when the wrapper returns
                                // more than one  

  sqlno_qtb* optqtbP;
  Unfenced_Generic_Server* srv;   //Server for which this reply was generated
  short       no_block; // Indicates if we want to disable block fetch, 0 by default  @d276032mjb

  //////////////
  // Methods //
  /////////////

  Reply (void);
  
  sqlint32 allocate_isets (struct sqlno_apcb* a_poolP,
                           int                a_num_quantifiers,
                           int                a_num_head_exp,
                           int                a_num_predicates,
                           int                a_num_gb_exp,
                           int                a_num_ob_exp);

  
  friend sqlint32 sqlnt_build_rpd_incol_list (sqlno_globals*     a_optP,
                                              Reply*             a_reply,
                                              sqlno_qgmP_array** a_qgm_arrayP);
  friend class Predicate_List;
};


////////////////////////////////////////////////////////////////////////////////
//
//
//                                Predicate_List 
//
//
////////////////////////////////////////////////////////////////////////////////
class Predicate_List : public Sqlqg_Base_Class  {

public:

  //Constructor, input the request from which the predicates come
  static sqlint32 create_empty_predicate_list (Reply*           a_reply,
                                               Predicate_List** a_pred_list);
  static sqlint32 create_and_copy_predicate_list (Reply*           a_reply,
                                                  Predicate_List** a_pred_list);
  
  // new predicates
  int          get_number_of_predicates (void);
  sqlint32     get_predicate (int            a_pred_handle,
                              Request_Exp**  a_pred_exp);
  sqlint32     get_predicate_handle (int  a_pred_pos,
                                     int* a_pred_handle);

  // already applied predicates
  int          get_number_of_applied_predicates (void);
  sqlint32     get_applied_predicate (int            a_applied_pred_handle,
                                      Request_Exp**  a_applied_pred_exp);
  sqlint32     get_applied_predicate_handle (int   a_applied_pred_pos,
                                             int*  a_applied_pred_handle);

  sqlint32     add_predicate (int a_pred_handle);
  sqlint32     add_applied_predicate (int a_applied_pred_handle);

  ~Predicate_List (void); 

  
  //DB2 optimizer use only
  Predicate_List (sqlno_iset* a_predsIn,
                  void*       a_context,
                  sqlint32*   a_rc);
  
  friend  class Unfenced_Generic_Server;
  
protected:
  Predicate_List (void); // Unimplemented

  //Used by create_empty_predicate_list
  Predicate_List (Reply*    a_reply,
                  sqlint32* a_rc);

  //initialization - called from the constructors
  sqlint32 init_predicate_list (sqlno_iset* a_predsIn);
    
  void*       context;
  sqlno_iset* preds;
  sqlno_iset* applied_preds;
  sqlno_iset* hexps;
  sqlno_qtb*  optqtbP;
};

// Inline methods

// Parsed_Query_Fragment
inline int
Parsed_Query_Fragment::is_generated_pid (int a_hexp_pos) 
{
  return (m_pid_array && m_pid_array[a_hexp_pos]) ? 1 :0;
}

// Reply
inline
Reply::Reply (void)
{}

inline Unfenced_Generic_Server*
Reply::get_server (void)
{
  return srv;
}

inline short Reply::get_no_block()
{
  return no_block;
}

inline void Reply::set_no_block()
{
  no_block = 1;
}

#endif //  __SQLQG_REQUEST_H__ included
