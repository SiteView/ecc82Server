
#include "stdafx.h"

typedef struct {
    word32 S0[256], S1[256], S2[256], S3[256], P[18];
    word32 iv0, iv1;		       /* for CBC mode */
} BlowfishContext;

#define GET_32BIT_LSB_FIRST(cp) \
  (((unsigned long)(unsigned char)(cp)[0]) | \
  ((unsigned long)(unsigned char)(cp)[1] << 8) | \
  ((unsigned long)(unsigned char)(cp)[2] << 16) | \
  ((unsigned long)(unsigned char)(cp)[3] << 24))

#define PUT_32BIT_LSB_FIRST(cp, value) do { \
  (cp)[0] = (value); \
  (cp)[1] = (value) >> 8; \
  (cp)[2] = (value) >> 16; \
  (cp)[3] = (value) >> 24; } while (0)

#define GET_32BIT_MSB_FIRST(cp) \
  (((unsigned long)(unsigned char)(cp)[0] << 24) | \
  ((unsigned long)(unsigned char)(cp)[1] << 16) | \
  ((unsigned long)(unsigned char)(cp)[2] << 8) | \
  ((unsigned long)(unsigned char)(cp)[3]))

#define PUT_32BIT_MSB_FIRST(cp, value) do { \
  (cp)[0] = (value) >> 24; \
  (cp)[1] = (value) >> 16; \
  (cp)[2] = (value) >> 8; \
  (cp)[3] = (value); } while (0)

#define Fprime(a,b,c,d) ( ( (S0[a] + S1[b]) ^ S2[c] ) + S3[d] )
#define F(x) Fprime( ((x>>24)&0xFF), ((x>>16)&0xFF), ((x>>8)&0xFF), (x&0xFF) )
#define ROUND(n) ( xL ^= P[n], t = xL, xL = F(xL) ^ xR, xR = t )
