// ViewSysid.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//#include "../IODLL/license.h"
#include "../IODLL/commons.h"

#ifdef _DEBUG
#pragma comment(lib,"../debug/IODll.lib")
#else
#pragma comment(lib,"../release/IODll.lib")
#endif

#pragma comment(lib,"ssleay32.lib")

#pragma comment(lib,"libeay32.lib")



unsigned char rconv(unsigned char  a) //转换表
{
	switch(a)
	{
	case '0':return 0;
	case '1':return 1;
	case '2':return 2;
	case '3':return 3;
	case '4':return 4;
	case '5':return 5;
	case '6':return 6;
	case '7':return 7;
	case '8':return 8;
	case '9':return 9;  
	case 'a':return 10;
	case 'b':return 11;
	case 'c':return 12;
	case 'd':return 13;
	case 'e':return 14;
	case 'f':return 15;
	case 'A':return 10;
	case 'B':return 11;
	case 'C':return 12;
	case 'D':return 13;
	case 'E':return 14;
	case 'F':return 15;
	default:return ' ';
	}
}

void chartohex(unsigned char  *a,int len,unsigned char  *b)   //将字符串按十六进制存入内存中
{
	int i;
	for(i=0;i<len/2;i++) 
		b[i]=((rconv(a[2*i])<<4)|(rconv(a[2*i+1])));  
	if(len%2)
		b[i]=rconv(a[2*i])<<4;
}


void print(const char *promptStr,unsigned char *data,int len)
{
	int i;
	printf("%s[长度=%d字节]======\n",promptStr,len);
	for(i = 0; i < len; i++) printf("%02x", data[i]);
	printf("\n=======================\n");
}


int main(int argc, char *argv[])
{

    char szLic[40480]={0};
	char buffer[40960]={0};
	unsigned char decbuff[1024];
	int Length = -1;
	FILE* finput;
	long lFileLen;

	char filename[128]={0};
	if(argc == 2)
	{
		strcpy(filename, argv[1]);
		finput = fopen(filename, "rb");
		if(!finput)
		{
			return 0;
		}
		fseek(finput, 0, SEEK_END);
		lFileLen = ftell(finput); 
		if(lFileLen <= 64)
		{
			fclose(finput);
			return 0;
		}
		fseek(finput, 0, SEEK_SET);	
		memset(szLic, 0, lFileLen+1);
		fread(szLic, 1, lFileLen, finput);
		fclose(finput);

	 
	if (*(szLic+strlen(szLic)-1) == 10 || *(szLic+strlen(szLic)-1) == 13 || *(szLic+strlen(szLic)-1) == 0x20)
	{
	   *(szLic+strlen(szLic)-1) = 0;
	}
	if (*(szLic+strlen(szLic)-1) == 10 || *(szLic+strlen(szLic)-1) == 13 || *(szLic+strlen(szLic)-1) == 0x20)
	{
	   *(szLic+strlen(szLic)-1) = 0;
	}
	if (*(szLic+strlen(szLic)-1) == 10 || *(szLic+strlen(szLic)-1) == 13 || *(szLic+strlen(szLic)-1) == 0x20)
	{
	   *(szLic+strlen(szLic)-1) = 0;
	}
	if (*(szLic+strlen(szLic)-1) == 10 || *(szLic+strlen(szLic)-1) == 13 || *(szLic+strlen(szLic)-1) == 0x20)
	{
	   *(szLic+strlen(szLic)-1) = 0;
	}
	if (*(szLic+strlen(szLic)-1) == 10 || *(szLic+strlen(szLic)-1) == 13 || *(szLic+strlen(szLic)-1) == 0x20)
	{
	   *(szLic+strlen(szLic)-1) = 0;
	}

	    chartohex((unsigned char*)szLic,strlen(szLic),(unsigned char*)buffer);
	    print("机器码:",(unsigned char*)buffer,strlen(szLic)/2);

		Length = strlen(szLic)/2;
		Length = des_decrypt(buffer,Length,decbuff);
		printf("%s\n",decbuff);	
	}
	else
	{
		printf("License工具。\r\n\r\n");
	}
	
	return 0;
}

