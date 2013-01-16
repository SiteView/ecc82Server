#include "StringMap.h"
#include <string.h>

StringMap::StringMap(void):svutil::hashtable<svutil::word,svutil::word>(17)
{
}

StringMap::~StringMap(void)
{
	//puts("In ~StringMap");
}

S_UINT StringMap::GetRawDataSize(void)
{
	S_UINT len=0;
	len+=sizeof(S_UINT)*2;
	iterator it;
	while(findnext(it))
	{
		len+=(S_UINT)strlen((*it).getkey())+1;
		len+=(S_UINT)strlen((*it).getvalue())+1;
	}

	return len;

	return 1;
}
char* StringMap::GetRawData(char *lpbuf,S_UINT bufsize)
{
	try{
		S_UINT count=size();
		int tlen=sizeof(S_UINT);
		char *pt=lpbuf;

		memcpy(pt,&count,tlen);
		pt+=tlen;
		count=spacesize();
		memcpy(pt,&count,tlen);
		pt+=tlen;
		iterator it;
		S_UINT len=tlen*2;
		while(findnext(it))
		{
			char *p=(*it).getkey();
			len+=(S_UINT)strlen(p)+1;
			if(len<=bufsize)
			{
				strcpy(pt,p);
				pt+=(S_UINT)strlen(p);
				pt[0]='\0';
				pt++;
			}else
				return NULL;
			p=(*it).getvalue();

			len+=(S_UINT)strlen(p)+1;
			if(len<=bufsize)
			{
				strcpy(pt,p);
				pt+=(S_UINT)strlen(p);
				pt[0]='\0';
				pt++;
			}else
				return NULL;

		}

		return lpbuf;
	}
	catch(...)
	{
		printf("\n Exception to StringMap::GetRawData \n");
		return NULL;
	}
}
BOOL StringMap::CreateObjectByRawData(const char *lpbuf,S_UINT bufsize)
{//converting data to objects

	if(!lpbuf)
		return false;
	int tlen=sizeof(S_UINT);
	S_UINT count=0,spcount=0;

	try{
		const char *pt=lpbuf;
		memcpy(&count,pt,tlen);
		pt+=tlen;
		memcpy(&spcount,pt,tlen);
		pt+=tlen;
		resetsize(spcount);
		S_UINT len=0;
		len+=tlen*2;
		//TDB:  this loop is very slow, see how to optimize, making resourcepool loading very slow
		for(S_UINT i=0;i<count;i++)
		{
			if(len+strlen(pt)+1>bufsize)
			{
				clearnodes();
				return false;
			}
			const char *p=pt;
			int n=strlen(p)+1;
			pt+=n;
			len+=n;
			if(len+strlen(pt)+1>bufsize)
			{
				clearnodes();
				return false;
			}
			insert(p,pt);
			n=strlen(pt)+1;
			pt+=n;
			len+=n;		

		}
	}catch(...)
	{
		printf("Exception to StringMap::CreateObjectByRawData \n");
		return false;
	}

	return true;

}

