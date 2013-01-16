/*_############################################################################
  _## 
  _##  reentrant.cpp  
  _##
  _##  SNMP++v3.2.15
  _##  -----------------------------------------------
  _##  Copyright (c) 2001-2004 Jochen Katz, Frank Fock
  _##
  _##  This software is based on SNMP++2.6 from Hewlett Packard:
  _##  
  _##    Copyright (c) 1996
  _##    Hewlett-Packard Company
  _##  
  _##  ATTENTION: USE OF THIS SOFTWARE IS SUBJECT TO THE FOLLOWING TERMS.
  _##  Permission to use, copy, modify, distribute and/or sell this software 
  _##  and/or its documentation is hereby granted without fee. User agrees 
  _##  to display the above copyright notice and this license notice in all 
  _##  copies of the software and any documentation of the software. User 
  _##  agrees to assume all liability for the use of the software; 
  _##  Hewlett-Packard and Jochen Katz make no representations about the 
  _##  suitability of this software for any purpose. It is provided 
  _##  "AS-IS" without warranty of any kind, either express or implied. User 
  _##  hereby grants a royalty-free license to any and all derivatives based
  _##  upon this software code base. 
  _##  
  _##  Stuttgart, Germany, Tue Jan  4 21:42:42 CET 2005 
  _##  
  _##########################################################################*/
char reentrant_cpp_version[]="#(@) SNMP++ $Id: reentrant.cpp,v 1.4 2004/03/03 23:11:21 katz Exp $";

#include "snmp_pp/reentrant.h"

#ifdef SNMP_PP_NAMESPACE
namespace Snmp_pp {
#endif

SnmpSynchronized::SnmpSynchronized()
{
#ifdef _THREADS
#ifdef WIN32
	InitializeCriticalSection(&_mutex);
#else
	pthread_mutex_init(&_mutex, 0);
#endif
#endif
}

SnmpSynchronized::~SnmpSynchronized()
{
#ifdef _THREADS
#ifdef WIN32
	DeleteCriticalSection(&_mutex);
#endif
#endif
}

void SnmpSynchronized::lock()
{
#ifdef _THREADS
#ifdef WIN32
	EnterCriticalSection(&_mutex);
#else
	pthread_mutex_lock(&_mutex);
#endif
#endif
}	

void SnmpSynchronized::unlock()
{
#ifdef _THREADS
#ifdef WIN32
	LeaveCriticalSection(&_mutex);
#else
	pthread_mutex_unlock(&_mutex);
#endif
#endif
}	

#ifdef SNMP_PP_NAMESPACE
}; // end of namespace Snmp_pp
#endif 

