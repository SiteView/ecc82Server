/*
 Copyright (C) 1998, 1999, 2001, 2002, 2003, 2004 J�r�me Lecomte

 This file is part of XLW, a free-software/open-source C++ wrapper of the
 Excel C API - http://xlw.sourceforge.net/

 XLW is free software: you can redistribute it and/or modify it under the
 terms of the XLW license.  You should have received a copy of the
 license along with this program; if not, please email xlw-users@lists.sf.net

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#ifndef INC_macros_H
#define INC_macros_H

/*!
\file macros.h
\brief I am not a big fan of macros, but these make life easier.
*/

// $Id: macros.h,v 1.12 2006/07/13 13:55:07 nando Exp $

#include <xlw/XlfException.h>
#include <xlw/XlfExcel.h>

#if defined(_MSC_VER)
#pragma once
#endif

//! Export flag.
/*!
Export macro that tells the compiler that the function is to be exported.
*/
#define EXCEL_EXPORT __declspec(dllexport)


//! Initialization macro
/*!
Macros to be inserted at the begining and end of *each* C function
to be exported to excel.

Frees the internal buffer if needed of XlfExcel used to return
heap allocated data to Excel.
\sa XlfExcel
*/
#define EXCEL_BEGIN XlfExcel::Instance().FreeMemory(); \
try \
{

//! Clean up macro
/*!
Macro to be inserted at the end of *each* C function to be exported
to excel.

Catch exception in C function calls
Catching rules are as follow :
- try first to catch excel exception that need the framework to return to excel
asap (namely coerce, uncalculated cell, and abort). If catched, returns 0 that
Excel will translate to #NULL!.
- catch runtime errors and send string to excel
- catch strings and echo them
- catch const char * and echo them
- catch cell matrix and echo it, useful for complicated errors
- catch all other kind of exception. Return #VALUE! error.

You could easily add your own exception here. Note that it causes all your
interface to recompile.
*/
#define EXCEL_END \
} catch (XlfException&) { \
	return 0; \
} catch (std::runtime_error& error){\
	return XlfOper(error.what());\
} catch (std::string& error){\
	return XlfOper(error);\
} catch (const char* error){\
	return XlfOper(error);\
} catch (const CellMatrix& error){\
	return XlfOper(error);\
} catch (...) { \
	return XlfOper::Error(xlerrValue); \
}

/* as example:
#define EXCEL_END \
} catch (XlfException&) { \
	return 0; \
} catch (yourException&) { \
	exceptionHandling; \
} catch (...) { \
	return XlfOper::Error(xlerrValue); \
}

*/

#define __HERE__ __FILE__ "(" _MAKESTRING(__LINE__) "): "
#define _MAKESTRING(a) __MAKESTRING(a)
#define __MAKESTRING(a) #a

#endif
