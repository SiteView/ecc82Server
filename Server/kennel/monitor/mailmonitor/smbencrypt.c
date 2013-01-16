/*
 * Copyright (C) 2005, 2006 Simon Josefsson
 * Copyright (C) 1998-1999  Brian Bruns
 * Copyright (C) 2004 Frediano Ziglio
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License
 * as published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include "global.h"

#include "md5.h"
#include "md4.h"
#include "des.h"


/* C89 compliant way to cast 'char' to 'unsigned char'. */
static __inline unsigned char
to_uchar (char ch)
{
  return ch;
}

/*
 * The following code is based on some psuedo-C code from ronald@innovation.ch
 */

static void ntlm_encrypt_answer (char *hash,
				 const char *challenge, char *answer);
static void ntlm_convert_key (char *key_56, des_ctx * ks);

void ntlm_create_romkey(uint8 * romKey)
{
  int i = 0;
  MD5_CTX ctx;
  srand( (unsigned)0xff);
  for(i = 0; i < 8; i++)
    sprintf(&romKey[i], "%02x", rand() % 0xff);

  MD5Init(&ctx);
  MD5Update(&ctx, romKey, 8);
  MD5Final(&ctx);
  memcpy(romKey, ctx.digest, 8);
  memset(&ctx, 0, sizeof(MD5_CTX));

}

void
ntlm_create_endkey(const uint8 * romKey,
                const uint8 * challenge,
                uint8 * answer)
{
	MD5_CTX ctx;
    MD5Init(&ctx);
	MD5Update(&ctx, challenge, 8);
	MD5Update(&ctx, romKey, 8);
    MD5Final(&ctx);
    memcpy(answer, ctx.digest, 8);
    memset(&ctx, 0, sizeof(MD5_CTX));
}


void
ntlm_smb_encrypt (const char *passwd, const uint8 * challenge, uint8 * answer)
{
#define MAX_PW_SZ 14
  int len;
  int i;
  static const char magic[8] =
    { 0x4B, 0x47, 0x53, 0x21, 0x40, 0x23, 0x24, 0x25 };
  des_ctx ks;
  char hash[21] = {0};
  char passwd_up[MAX_PW_SZ];

  /* convert password to upper and pad to 14 chars */
  memset (passwd_up, 0, MAX_PW_SZ);
  len = strlen (passwd);
  if (len > MAX_PW_SZ)
    len = MAX_PW_SZ;

  for (i = 0; i < len; i++)
    passwd_up[i] = toupper (passwd[i]);
  for (; i < MAX_PW_SZ; i++)
    passwd_up[i] = 0;

  /* hash the first 7 characters */
  ntlm_convert_key (passwd_up, &ks);
  des_ecb_encrypt (&ks, magic, hash + 0);

  /* hash the second 7 characters */
  ntlm_convert_key (passwd_up + 7, &ks);
  des_ecb_encrypt (&ks, magic, hash + 8);

  memset (hash + 16, 0, 5);

  ntlm_encrypt_answer (hash, challenge, answer);

  /* with security is best be pedantic */
  memset (&ks, 0, sizeof (ks));
  memset (hash, 0, sizeof (hash));
  memset (passwd_up, 0, sizeof (passwd_up));
}

void
ntlm_smb_nt_encrypt (const char *passwd,
		     const uint8 * challenge,
		     uint8 * answer)
{
  size_t len, i;
  unsigned char hash[21] = {0};
  unsigned char nt_pw[256] = {0};

  /* NT resp */
  len = strlen (passwd);
  if (len > 128)
    len = 128;
  for (i = 0; i < len; ++i)
    {
      nt_pw[2 * i] = passwd[i];
      nt_pw[2 * i + 1] = 0;
    }

  md4_buffer (nt_pw, len * 2, hash);

  memset (hash + 16, 0, 5);
  ntlm_encrypt_answer (hash, challenge, answer);

  /* with security is best be pedantic */
  memset (hash, 0, sizeof (hash));
  memset (nt_pw, 0, sizeof (nt_pw));
}

/*
 * takes a 21 byte array and treats it as 3 56-bit DES keys. The
 * 8 byte plaintext is encrypted with each key and the resulting 24
 * bytes are stored in the results array.
 */
static void
ntlm_encrypt_answer (char *hash, const char *challenge, char *answer)
{
  des_ctx ks;

  ntlm_convert_key (hash, &ks);
  des_ecb_encrypt (&ks, challenge, answer);

  ntlm_convert_key (&hash[7], &ks);
  des_ecb_encrypt (&ks, challenge, &answer[8]);

  ntlm_convert_key (&hash[14], &ks);
  des_ecb_encrypt (&ks, challenge, &answer[16]);

  memset (&ks, 0, sizeof (ks));
}

/*
 * turns a 56 bit key into the 64 bit, and sets the key schedule ks.
 */
static void
ntlm_convert_key (char *key_56, des_ctx * ks)
{
  char key[8];

  key[0] = to_uchar (key_56[0]);
  key[1] = ((to_uchar (key_56[0]) << 7) & 0xFF) | (to_uchar (key_56[1]) >> 1);
  key[2] = ((to_uchar (key_56[1]) << 6) & 0xFF) | (to_uchar (key_56[2]) >> 2);
  key[3] = ((to_uchar (key_56[2]) << 5) & 0xFF) | (to_uchar (key_56[3]) >> 3);
  key[4] = ((to_uchar (key_56[3]) << 4) & 0xFF) | (to_uchar (key_56[4]) >> 4);
  key[5] = ((to_uchar (key_56[4]) << 3) & 0xFF) | (to_uchar (key_56[5]) >> 5);
  key[6] = ((to_uchar (key_56[5]) << 2) & 0xFF) | (to_uchar (key_56[6]) >> 6);
  key[7] = (to_uchar (key_56[6]) << 1) & 0xFF;

  des_setkey (ks, key);

  memset (&key, 0, sizeof (key));
}


/** \@} */
