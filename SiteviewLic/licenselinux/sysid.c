#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "license.h"



void print(const char *promptStr,unsigned char *data,int len)
{
	int i;
	printf("%s[长度=%d字节]======\n",promptStr,len);
	for(i = 0; i < len; i++) printf("%02x", data[i]);
	printf("\n=======================\n");
}

void hextochar(unsigned char  *a,int len, char  *b);



int main(int argc, char *argv[])
{
	unsigned char addrs[256];
	unsigned char uniqueids[1024]; 

	unsigned char encbuff[1024];
	unsigned char buffer[4096];
	//unsigned char decbuff[1024];
	int Length = -1;
	FILE* foutput;

	memset(uniqueids, 0, 1024);
	if(GetSystemFeature(addrs) == 1)				
	{
		// printf("%s \n",addrs);
		strcat(uniqueids, addrs);
	}
	Length = (int)strlen(uniqueids)+1;
	Length = des_encrypt(uniqueids,Length,encbuff);

	print("机器码:",encbuff,Length);
	//print("密文",encbuff,Length);

	//Length = des_decrypt(encbuff,Length,decbuff);
	//print("明文",decbuff,Length);
	//printf("明文:%s \n",uniqueids);
	//print("明文",uniqueids,strlen(uniqueids));
	//printf("%s\n",decbuff);	

    memset(buffer, 0, 4096);
	hextochar(encbuff,Length,buffer);
	foutput = fopen("sysid.txt","wb");		
	fwrite(buffer, 1, strlen(buffer), foutput);
	fclose(foutput);
	return 0;
}


