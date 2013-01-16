/* check-version.h --- Check version string compatibility.
   Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005 Free
   Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation; either version 2.1, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.  */

/* Written by Simon Josefsson.  This interface is influenced by
   gcry_check_version from Werner Koch's Libgcrypt.  Paul Eggert
   suggested the use of strverscmp to simplify implementation. */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stddef.h>
#include "strverscmp.h"

/* Get specification. */
#include "check-version.h"

/* Check that the the version of the library (i.e., the CPP symbol
 * VERSION) is at minimum the requested one in REQ_VERSION (typically
 * found in a header file) and return the version string.  Return NULL
 * if the condition is not satisfied.  If a NULL is passed to this
 * function, no check is done, but the version string is simply
 * returned.
 */
const char VERSION[] = "0.3.9";

const char *
ntlm_check_version (const char *req_version)
{
  if (!req_version || strverscmp (req_version, VERSION) <= 0)
    return VERSION;

  return NULL;
}
