/******************************************************************************
**
** Source File Name: sqlqg_array.h
**
** (C) COPYRIGHT International Business Machines Corp. 2002, 2003
** All Rights Reserved
** Licensed Materials - Property of IBM
**
** US Government Users Restricted Rights - Use, duplication or
** disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
**
** Function = Include File defining:
**              Query Gateway classes and routines for
**              Sqlqg_Array
**
** Operating System: All
**
*******************************************************************************/
#ifndef __SQLQG_ARRAY_H__
#define __SQLQG_ARRAY_H__


#include "sqlqg_misc.h" 

////////////////////////////////////////////////////////////////////////////////
// Class: Sqlqg_Array
//
// An instance of this class is an expandable array of objects.
// This is a general-purpose class for use by the query gateway
// and wrappers.
////////////////////////////////////////////////////////////////////////////////

class Sqlqg_Array : public Sqlqg_Base_Class
{
//===================================================================
// Public members
//===================================================================
public:

   // Constructor.
   Sqlqg_Array(Sqlqg_Return_Code *a_rc);

   // Destructor.
   ~Sqlqg_Array(void);

   //Accessor methods.
   Sqlqg_Return_Code add_element(Sqlqg_Base_Class *a_element);

   Sqlqg_Return_Code get_element(
       sqlint32 a_index, Sqlqg_Base_Class **a_element) const;

   Sqlqg_Return_Code set_element(
       sqlint32 a_index, Sqlqg_Base_Class *a_element);

   sqlint32 get_element_count(void) const;

   void remove_all_elements(void);

//===================================================================
// Private members
//===================================================================
private:

   // Functions
   Sqlqg_Return_Code check_index(
       sqluint32 a_function_id,
       sqlint32 a_index) const;
   //////////////////
   //  Data.
   //////////////////

   // m_element_ptrs[x] is a pointer to element number x in the array.
   Sqlqg_Base_Class **m_element_ptrs;

   // The number of pointers in m_element_ptrs that are in use.
   sqlint32 m_number_of_elements_in_use;

   // The number of pointers in m_element_ptrs that are allocated.
   sqlint32 m_number_of_elements_allocated;

   // Prevent users of this class from copying or assigning.
   Sqlqg_Array( Sqlqg_Array &);

   Sqlqg_Array &operator=(Sqlqg_Array &);
};

/////////////////////////////////////////////////////////////////////
// Inline functions
/////////////////////////////////////////////////////////////////////

// Return the number of elements in the array.
inline sqlint32 Sqlqg_Array::get_element_count(void) const
{
   return m_number_of_elements_in_use;
}

// Remove all elements from the array.
inline void Sqlqg_Array::remove_all_elements(void)
{
   m_number_of_elements_in_use = 0;
   // We could also clear the m_element_ptrs array, but it's really
   // not necessary because once m_number_of_elements_in_use is 0,
   // we should not access those elements again.
}

#endif
