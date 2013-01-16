
#include "stdafx.h"

typedef struct {
    uint32 h[4];
} MD5_Core_State;

struct MD5Context {
    MD5_Core_State core;
    unsigned char block[64];
    int blkused;
    uint32 lenhi, lenlo;
};

void MD5Init(struct MD5Context *context);
void MD5Update(struct MD5Context *context, unsigned char const *buf, unsigned len);
void MD5Final(unsigned char digest[16], struct MD5Context *context);
void hmac_md5(unsigned char *text, int text_len, unsigned char *key, int key_len, unsigned char *digest);
void MD5(unsigned char output[16], const unsigned char *input, int ilen);
