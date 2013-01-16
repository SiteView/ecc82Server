
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/des.h>

#ifdef WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT extern
#endif


/*
static unsigned char cbc_key [8]={0x73,0x69,0x74,0x65,0x76,0x69,0x65,0x77};
static unsigned char cbc_iv  [8]={0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};

static unsigned char des_key [8]={0x6f,0x6c,0x64,0x68,0x61,0x6e,0x64,0x61};
static unsigned char des_iv  [8]={0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};*/




static unsigned char cbc_key [8]={0x01,0x32,0x54,0x76,0x98,0xab,0xcd,0xef};
static unsigned char cbc_iv  [8]={0xfe,0xdc,0xba,0x89,0x67,0x54,0x23,0x10};

static unsigned char des_key [8]={0x10,0x32,0x54,0x76,0x89,0xba,0xdc,0xfe};
static unsigned char des_iv  [8]={0xfe,0xdc,0xba,0x89,0x67,0x54,0x23,0x10};

static unsigned char key_key [8]={0x10,0x54,0x23,0x98,0x76,0xcd,0xab,0xfe};
static unsigned char key_iv  [8]={0xfe,0xdc,0xba,0x89,0x67,0x54,0x23,0x10};

 DLLEXPORT long des_decrypt(unsigned char *encData,long Length,unsigned char *plainText)
{
	des_cblock iv3;
	des_key_schedule ks;
   // size_t len = (Length+7)/8 * 8;
	DES_set_key_checked(&cbc_key,&ks);
	memset(plainText,0,Length);
	memcpy(iv3,cbc_iv,sizeof(cbc_iv));
	des_ncbc_encrypt(encData,plainText,Length,ks,&iv3,DES_DECRYPT);
	return Length;
}
 DLLEXPORT  long des_encrypt(unsigned char *plainText,long Length,unsigned char *encData)
{
	des_cblock iv3;
	des_key_schedule ks;
    size_t len = (Length+7)/8 * 8;
	DES_set_key_checked(&cbc_key,&ks);
	memset(encData,0,Length);
	memcpy(iv3,cbc_iv,sizeof(cbc_iv));
	des_ncbc_encrypt(plainText,encData,Length,ks,&iv3,DES_ENCRYPT);
	return len;
}

long des_lic_decrypt(unsigned char *encData,long Length,unsigned char *plainText)
{
	des_cblock iv3;
	des_key_schedule ks;
   // size_t len = (Length+7)/8 * 8;
	DES_set_key_checked(&des_key,&ks);
	memset(plainText,0,Length);
	memcpy(iv3,cbc_iv,sizeof(des_iv));
	des_ncbc_encrypt(encData,plainText,Length,ks,&iv3,DES_DECRYPT);
	return Length;
}
long des_lic_encrypt(unsigned char *plainText,long Length,unsigned char *encData)
{
	des_cblock iv3;
	des_key_schedule ks;
    size_t len = (Length+7)/8 * 8;
	DES_set_key_checked(&des_key,&ks);
	memset(encData,0,Length);
	memcpy(iv3,cbc_iv,sizeof(des_iv));
	des_ncbc_encrypt(plainText,encData,Length,ks,&iv3,DES_ENCRYPT);
	return len;
}


DLLEXPORT long des_hex_encrypt(unsigned char *plainText,long Length,unsigned char *encData)
{
	des_cblock iv3;
	des_key_schedule ks;
	unsigned char encbuff[1024]={0};
	unsigned char buff[32]={0};
    size_t len = (Length+7)/8 * 8;
	int i;

	DES_set_key_checked(&cbc_key,&ks);
	memset(encbuff,0,Length);
	memcpy(iv3,cbc_iv,sizeof(cbc_iv));
	des_ncbc_encrypt(plainText,encbuff,Length,ks,&iv3,DES_ENCRYPT);

    memset(encData,0,len*2);	

	for( i = 0; i < len; i++) 
	{  
		sprintf(buff,"%02x", (unsigned char)encbuff[i]);
		strcat(encData,buff);
	}
	return strlen(encData);
}



long key_decrypt(unsigned char *encData,long Length,unsigned char *plainText)
{
	des_cblock iv3;
	des_key_schedule ks;
   // size_t len = (Length+7)/8 * 8;
	DES_set_key_checked(&key_key,&ks);
	memset(plainText,0,Length);
	memcpy(iv3,key_iv,sizeof(key_iv));
	des_ncbc_encrypt(encData,plainText,Length,ks,&iv3,DES_DECRYPT);
	return Length;
}
long key_encrypt(unsigned char *plainText,long Length,unsigned char *encData)
{
	des_cblock iv3;
	des_key_schedule ks;
        size_t len = (Length+7)/8 * 8;
	DES_set_key_checked(&key_key,&ks);
	memset(encData,0,Length);
	memcpy(iv3,key_iv,sizeof(key_iv));
	des_ncbc_encrypt(plainText,encData,Length,ks,&iv3,DES_ENCRYPT);
	return len;
}

