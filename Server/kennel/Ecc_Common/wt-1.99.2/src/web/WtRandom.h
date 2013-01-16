// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 * $Id: WtRandom.h,v 1.2 2006/12/05 11:26:28 jozef Exp $
 */

#ifndef _WTRANDOM_H_
#define _WTRANDOM_H_
#include "WDllDefs.h"
#include <stdlib.h>

#ifdef WIN32
// Windows port of unix function (falls back to rand())
WT_API long lrand48();

// Windows port of unix function (falls back to rand())
WT_API double drand48();

// Windows port of unix function (falls back to srand())
WT_API void srand48(long seed);
#endif

#endif
