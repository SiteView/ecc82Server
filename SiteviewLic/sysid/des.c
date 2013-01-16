
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/des.h>


static unsigned char cbc_key [8]={0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef};
static unsigned char cbc_iv  [8]={0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};



long des_decrypt(unsigned char *encData,long Length,unsigned char *plainText)
{
	des_cblock iv3;
	des_key_schedule ks;

	DES_set_key_checked(&cbc_key,&ks);
	memset(plainText,0,Length);
	memcpy(iv3,cbc_iv,sizeof(cbc_iv));
	des_ncbc_encrypt(encData,plainText,Length,ks,&iv3,DES_DECRYPT);
	return Length;
}
long des_encrypt(unsigned char *plainText,long Length,unsigned char *encData)
{
	des_cblock iv3;
	des_key_schedule ks;

	DES_set_key_checked(&cbc_key,&ks);
	memset(encData,0,Length);
	memcpy(iv3,cbc_iv,sizeof(cbc_iv));
	des_ncbc_encrypt(plainText,encData,Length,ks,&iv3,DES_ENCRYPT);
	return Length;
}


