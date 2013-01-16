
#include <ctype.h>
#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commons.h"


unsigned char rconv(unsigned char  a);
char *strtrim(char *string);


#ifdef WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT extern
#endif

int isNumeric(const char* str)
{
	for(;!(*str==0);str++)
	{
		if(!isdigit(*str))	return false;
	}
	return true;
}

void hextochar(unsigned char  *a,int len, char  *b)
{
	int i;
	char buff[16]={0};
	strcpy(b,"");
	for(i = 0; i < len; i++) 
	{
		sprintf(buff,"%02x", a[i]);
		strcat(b,buff);
	}
}

void chartohex(unsigned char  *a,int len,unsigned char  *b)   //���ַ�����ʮ�����ƴ����ڴ���
{
	int i;
	for(i=0;i<len/2;i++) 
		b[i]=((rconv(a[2*i])<<4)|(rconv(a[2*i+1])));  
	if(len%2)
		b[i]=rconv(a[2*i])<<4;
}



int verifiylicense(unsigned char *lic)
{
        int i;
	unsigned char s;
        for ( i = 0;i < (int)strlen(lic); i++)
	{
		s = *(lic+i);
	        if ((s == 0x2c) || (s == 0x3a)  || (s == 0x3b) || (s == 45) || (s == 46) || ((s >= 0x30) && (s <= 0x39)) || ((s >= 0x61) && (s <= 0x7a)) || ((s >= 0x41) && (s <= 0x5a))) continue;
		printf("______________%d %c",s,s);       
		return false;
	}
	return true;
}

unsigned char rconv(unsigned char  a) //ת����
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


char *strtrim(char *string)
{ 
	char *s;
	if (strlen(string) > 0)
	{
		s=string+strlen(string)-1; //�����һ���ַ��ĵ�ַ��ŵ�s��//
		while(*s==',')//���Ŷ�ȡ�ַ�//
		s--;
		*(s+1)='\0'; //ֱ�����ַ����ǿո񣬲��ڸ��ַ��������һ��������//
		return string; //����ֵ���ַ������һ���ǿո��ַ��ĵ�ַ//
	}
	return string;
} 

const MODULEITEM modulesdata[]={
	{"整体视图", "WHOLEVIEW"},
	{"树状视图", "TREEVIEW"},
	{"拓扑视图", "TUOPUVIEW"},
	{"拓扑扫描", "TUOPUSCAN"},
	{"工单", "TASKPLAN"},
	{"报表", "REPORT"},
	{"告警", "ALARM"},
	{"设置", "CONFIG"},
	{"流量分析", "FA"} 
};

void modulesreplace(char *src, char *dst)
{

	char modules[256]={0};
	int n;
	char * pch;

	if  (verifiylicense(src) )
	{
		strcpy(dst,src);
		return;
	}
	strcpy(dst,"");
	if(NULL != src || strcmp(src, "") != 0)
	{		
		pch = strtok(src,":,.-");
		while (pch != NULL)
		{
			for(n = 0; n < sizeof(modulesdata)/ sizeof(MODULEITEM); n++)
			{
				if(strcmp(modulesdata[n].name,pch) == 0)
				{
					strcat(dst,modulesdata[n].shortName);
					strcat(dst,",");
					break;
				}
			}
			pch = strtok (NULL, ":,.-");
		}
	}
	if (*(dst+strlen(dst)-1) == ',')
	{
	   *(dst+strlen(dst)-1) = 0;
	}
}

void replace(const char *src, const char *from, const char *to,char *dst)
{
	size_t fromlen = strlen(from);
	const char *match;
	size_t temp;
	
	char buff[4096] = {0};	
	char newbuff[4096] = {0};	
	strcpy(buff,src);	
	for ( ;; )
	{
		match = strstr(buff, from);
		if ( match != NULL )
		{
			temp = match-buff;
			memset(newbuff,0,4096);
			strncpy(newbuff,buff,temp);
			strcat(newbuff,to);
			strcat(newbuff,match+fromlen);
			memset(buff,0,4096);
			strcpy(buff,newbuff);
		}
		else 
		{
			strcpy(dst,newbuff);
			break;
		}
	}
}
