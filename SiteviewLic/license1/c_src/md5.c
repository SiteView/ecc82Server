
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <openssl/evp.h>
#include <openssl/md5.h>


static char *pt(unsigned char *md)
{
	int i;
	static char buf[80];

	for (i=0; i<MD5_DIGEST_LENGTH; i++)
		sprintf(&(buf[i*2]),"%02X",md[i]);
	return(buf);
}


void md5(const char* sour, char *dest)
{
	char *p;
	unsigned char md[MD5_DIGEST_LENGTH];

	EVP_Digest(sour,strlen(sour),md,NULL,EVP_md5(), NULL);
	p=pt(md);
	strcpy(dest,p);
}


