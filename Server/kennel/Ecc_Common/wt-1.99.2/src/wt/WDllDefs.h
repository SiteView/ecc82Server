// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2006 Wim Dumon, Leuven, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#ifndef WDLLDEFS_H_
#define WDLLDEFS_H_

// Source: http://www.nedprod.com/programs/gccvisibility.html

#ifdef WIN32
  #define WT_IMPORT __declspec(dllimport)
  #define WT_EXPORT __declspec(dllexport)
  #define WT_DLLLOCAL
  #define WT_DLLPUBLIC
#else
  #define WT_IMPORT
  #ifdef GCC_HASCLASSVISIBILITY
    #define WT_IMPORT __attribute__ ((visibility("default")))
    #define WT_EXPORT __attribute__ ((visibility("default")))
    #define WT_DLLLOCAL __attribute__ ((visibility("hidden")))
    #define WT_DLLPUBLIC __attribute__ ((visibility("default")))
  #else
    #define WT_IMPORT
    #define WT_EXPORT
    #define WT_DLLLOCAL
    #define WT_DLLPUBLIC
  #endif
#endif

// Define FXAPI for DLL builds
#ifdef WITTY_DLL
  #ifdef WITTY_DLL_EXPORTS
    #define WT_API WT_EXPORT
  #else
    #define WT_API WT_IMPORT
  #endif
#else
  #define WT_API
#endif

#endif // DLLDEFS_H_
