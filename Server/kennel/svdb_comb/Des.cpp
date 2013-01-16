// Des.cpp: implementation of the Des class.
//
//////////////////////////////////////////////////////////////////////
/*
#include "time.h"
#include "stdafx.h"
*/
#include "Des.h"
/*
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
*/
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

char map[16][16]={"e", "R", "O", "z", "P", "c", "G", "1", "Q", "K", "7", "I", "J", "L", "N", "4"};
unsigned char key[24] = {0};
unsigned char key8[8] = {0x02, 0x35, 0x41, 0xF3, 0x4F, 0x20, 0x0F, 0x88};

#include <stdlib.h>
#include<string.h>
#include<stdio.h>
#include <time.h>

Des::Des()
{

}

Des::~Des()
{

}

bool Des::Encrypt(const char *szInput, char *szOutput)
{
	int i = 0, j = 0;
	int inlen = strlen(szInput);
	int outlen = 0;

	srand((unsigned)time(NULL));

    des_key_setup(GET_32BIT_MSB_FIRST(key8),
		  GET_32BIT_MSB_FIRST(key8 + 4), &crkey);

	unsigned char dest[MAX_LEN] = {0};
	des_cbc_encrypt(dest, (unsigned char *)szInput, inlen, &crkey);
	outlen = inlen % 8 ? inlen + 8 - inlen % 8 : inlen;

	for(i = 0;i < outlen;i ++)
	{
		sprintf(szOutput, "%s%c", szOutput, map[dest[i] / 0x10][0]);
		for(j = 0;j < (int)strlen(map[dest[i] / 0x10]) - 1;j ++)
			sprintf(szOutput, "%s%c", szOutput, rand() % 26 + 0x41);

		sprintf(szOutput, "%s%c", szOutput, map[dest[i] % 0x10][0]);
		for(j = 0;j < (int)strlen(map[dest[i] % 0x10]) - 1;j ++)
			sprintf(szOutput, "%s%c", szOutput, rand() % 26 + 0x41);
	}

	//memset(key8, 0, 8);
	//memset(map, 0, 256);

	return true;

}

void Des::des_key_setup(word32 key_msw, word32 key_lsw, DESContext *sched)
{
   static const int PC1_Cbits[] = {
	7, 15, 23, 31, 39, 47, 55, 63, 6, 14, 22, 30, 38, 46,
	54, 62, 5, 13, 21, 29, 37, 45, 53, 61, 4, 12, 20, 28
    };
    static const int PC1_Dbits[] = {
	1, 9, 17, 25, 33, 41, 49, 57, 2, 10, 18, 26, 34, 42,
	50, 58, 3, 11, 19, 27, 35, 43, 51, 59, 36, 44, 52, 60
    };
    /*
     * The bit numbers in the two lists below don"t correspond to
     * the ones in the above description of PC2, because in the
     * above description C and D are concatenated so `bit 28" means
     * bit 0 of C. In this implementation we"re using the standard
     * `bitsel" function above and C is in the second word, so bit
     * 0 of C is addressed by writing `32" here.
     */
    static const int PC2_0246[] = {
	49, 36, 59, 55, -1, -1, 37, 41, 48, 56, 34, 52, -1, -1, 15, 4,
	25, 19, 9, 1, -1, -1, 12, 7, 17, 0, 22, 3, -1, -1, 46, 43
    };
    static const int PC2_1357[] = {
	-1, -1, 57, 32, 45, 54, 39, 50, -1, -1, 44, 53, 33, 40, 47, 58,
	-1, -1, 26, 16, 5, 11, 23, 8, -1, -1, 10, 14, 6, 20, 27, 24
    };
    static const int leftshifts[] =
	{ 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };

    word32 C, D;
    word32 buf[2];
    int i;

    buf[0] = key_lsw;
    buf[1] = key_msw;

    C = bitsel(buf, PC1_Cbits, 28);
    D = bitsel(buf, PC1_Dbits, 28);

    for (i = 0; i < 16; i++) {
	C = rotl28(C, leftshifts[i]);
	D = rotl28(D, leftshifts[i]);
	buf[0] = D;
	buf[1] = C;
	sched->k0246[i] = bitsel(buf, PC2_0246, 32);
	sched->k1357[i] = bitsel(buf, PC2_1357, 32);
    }

    sched->eiv0 = sched->eiv1 = 0;
    sched->div0 = sched->div1 = 0;     /* for good measure */

}

void Des::des_cbc_encrypt(unsigned char *dest, const unsigned char *src, unsigned int len, DESContext *sched)
{
   word32 out[2], iv0, iv1;
    unsigned int i;

    iv0 = sched->eiv0;
    iv1 = sched->eiv1;
    for (i = 0; i < len; i += 8) {
	iv0 ^= GET_32BIT_MSB_FIRST(src);
	src += 4;
	iv1 ^= GET_32BIT_MSB_FIRST(src);
	src += 4;
	des_encipher(out, iv0, iv1, sched);
	iv0 = out[0];
	iv1 = out[1];
	PUT_32BIT_MSB_FIRST(dest, iv0);
	dest += 4;
	PUT_32BIT_MSB_FIRST(dest, iv1);
	dest += 4;
    }
    sched->eiv0 = iv0;
    sched->eiv1 = iv1;

}

word32 Des::bitsel(word32 *input, const int *bitnums, int size)
{
    word32 ret = 0;
    while (size--) {
	int bitpos = *bitnums++;
	ret <<= 1;
	if (bitpos >= 0)
	    ret |= 1 & (input[bitpos / 32] >> (bitpos % 32));
    }
    return ret;


}

void Des::des_encipher(word32 *output, word32 L, word32 R, DESContext *sched)
{
   word32 swap, s0246, s1357;

    IP(L, R);

    L = rotl(L, 1);
    R = rotl(R, 1);

    L ^= f(R, sched->k0246[0], sched->k1357[0]);
    R ^= f(L, sched->k0246[1], sched->k1357[1]);
    L ^= f(R, sched->k0246[2], sched->k1357[2]);
    R ^= f(L, sched->k0246[3], sched->k1357[3]);
    L ^= f(R, sched->k0246[4], sched->k1357[4]);
    R ^= f(L, sched->k0246[5], sched->k1357[5]);
    L ^= f(R, sched->k0246[6], sched->k1357[6]);
    R ^= f(L, sched->k0246[7], sched->k1357[7]);
    L ^= f(R, sched->k0246[8], sched->k1357[8]);
    R ^= f(L, sched->k0246[9], sched->k1357[9]);
    L ^= f(R, sched->k0246[10], sched->k1357[10]);
    R ^= f(L, sched->k0246[11], sched->k1357[11]);
    L ^= f(R, sched->k0246[12], sched->k1357[12]);
    R ^= f(L, sched->k0246[13], sched->k1357[13]);
    L ^= f(R, sched->k0246[14], sched->k1357[14]);
    R ^= f(L, sched->k0246[15], sched->k1357[15]);

    L = rotl(L, 31);
    R = rotl(R, 31);

    swap = L;
    L = R;
    R = swap;

    FP(L, R);

    output[0] = L;
    output[1] = R;

}

bool Des::Decrypt(const char *szInput, char *szOutput)
{
	int i = 0;
	int inlen = strlen(szInput);
	int datalen = 0;

	if(inlen <= 8 || inlen > 512)
		return false;

	i = 0;
	char src[MAX_LEN] = {0};
	while(i < inlen)
	{
		int len = 0;
		int hi = map_index(szInput[i], len);
		i += len;
		int lo = map_index(szInput[i], len);
		i += len;

		if(hi == 99 || lo == 99)
			return false;

		src[datalen++] = hi * 0x10 + lo;
	}
	
    des_key_setup(GET_32BIT_MSB_FIRST(key8),
		  GET_32BIT_MSB_FIRST(key8 + 4), &crkey);

	unsigned char dest[256] = {0};
	des_cbc_decrypt(dest, (unsigned char *)src, datalen, &crkey);

	sprintf(szOutput, "%s", dest);

	//memset(key, 0, 8);
	//memset(map, 0, 256);

	return true;

}

unsigned char Des::map_index(char ch, int &len)
{
	for(int i = 0;i < 16;i ++)
		if(ch == map[i][0])
		{
			len = strlen(map[i]);
			return i;
		}

	return 99;


}

void Des::des_cbc_decrypt(unsigned char *dest, const unsigned char *src, unsigned int len, DESContext *sched)
{
   word32 out[2], iv0, iv1, xL, xR;
    unsigned int i;

    iv0 = sched->div0;
    iv1 = sched->div1;
    for (i = 0; i < len; i += 8) {
	xL = GET_32BIT_MSB_FIRST(src);
	src += 4;
	xR = GET_32BIT_MSB_FIRST(src);
	src += 4;
	des_decipher(out, xL, xR, sched);
	iv0 ^= out[0];
	iv1 ^= out[1];
	PUT_32BIT_MSB_FIRST(dest, iv0);
	dest += 4;
	PUT_32BIT_MSB_FIRST(dest, iv1);
	dest += 4;
	iv0 = xL;
	iv1 = xR;
    }
    sched->div0 = iv0;
    sched->div1 = iv1;

}

void Des::des_decipher(word32 *output, word32 L, word32 R, DESContext *sched)
{
   word32 swap, s0246, s1357;

    IP(L, R);

    L = rotl(L, 1);
    R = rotl(R, 1);

    L ^= f(R, sched->k0246[15], sched->k1357[15]);
    R ^= f(L, sched->k0246[14], sched->k1357[14]);
    L ^= f(R, sched->k0246[13], sched->k1357[13]);
    R ^= f(L, sched->k0246[12], sched->k1357[12]);
    L ^= f(R, sched->k0246[11], sched->k1357[11]);
    R ^= f(L, sched->k0246[10], sched->k1357[10]);
    L ^= f(R, sched->k0246[9], sched->k1357[9]);
    R ^= f(L, sched->k0246[8], sched->k1357[8]);
    L ^= f(R, sched->k0246[7], sched->k1357[7]);
    R ^= f(L, sched->k0246[6], sched->k1357[6]);
    L ^= f(R, sched->k0246[5], sched->k1357[5]);
    R ^= f(L, sched->k0246[4], sched->k1357[4]);
    L ^= f(R, sched->k0246[3], sched->k1357[3]);
    R ^= f(L, sched->k0246[2], sched->k1357[2]);
    L ^= f(R, sched->k0246[1], sched->k1357[1]);
    R ^= f(L, sched->k0246[0], sched->k1357[0]);

    L = rotl(L, 31);
    R = rotl(R, 31);

    swap = L;
    L = R;
    R = swap;

    FP(L, R);

    output[0] = L;
    output[1] = R;

}
/*
#include "TString.h"
int main(int argc, char *argv[])
{
	Des des;
	char out[50]={0};

	printf("input abcdefg11\n");

	des.Encrypt("abcdefg11",out);
	
	printf("after Encrypt=%s\n",out);

	char out2[50]={0};
	des.Decrypt(out, out2);

	printf("after Decrypt=%s\n",out2);

	return 0;
}
*/