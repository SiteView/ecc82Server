
/* Copyright (C) 2010  dragonflow Inc.
 * All right reserved.
 */

#ifndef _LICENSE_H_
#define _LICENSE_H_

#include "../liclib/commons.h"
#include "../liclib/licFuncsDll.h"
#include "../liclib/ConstData.h"

long mac_addr_sys(unsigned char *addr);
long des_decrypt(unsigned char *encData,long Length,unsigned char *plainText);
long des_encrypt(unsigned char *plainText,long Length,unsigned char *encData);


#endif // !_LICENSE_H_

/* EOF */
