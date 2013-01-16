/* base64.h
 * Copyright (C) 2000-2010 Farooq Mela. All rights reserved.
 *
 * Routines for base64 encoding and decoding.
 *
 * $Id$
 */

#ifndef _BASE64_H_
#define _BASE64_H_

void *base64_encode(const void *input, unsigned input_size, unsigned *output_size);
void *base64_decode(const void *input, unsigned input_size, unsigned *output_size);


#ifndef NDEBUG
# include <stdio.h>
# include <stdlib.h>
# undef ASSERT
# if defined(__GNUC__)
#  define ASSERT(expr)														\
	do {																	\
		if (!(expr)) {														\
			fprintf(stderr, "%s:%d (%s) assertion failed: \"%s\"\n",		\
					__FILE__, __LINE__, __PRETTY_FUNCTION__, #expr);		\
			abort();														\
		}																	\
	} while (0)
# else
#  define ASSERT(expr)														\
	do {																	\
		if (!(expr)) {														\
			fprintf(stderr, "%s:%d assertion failed: \"%s\"\n",				\
					__FILE__, __LINE__, #expr);								\
			abort();														\
		}																	\
	} while (0)
# endif
#else
# define ASSERT(expr)	(void)0
#endif


#define MALLOC(n)		weecrypt_xmalloc(n)
void *weecrypt_xmalloc(size_t size);

#endif // !_BASE64_H_
