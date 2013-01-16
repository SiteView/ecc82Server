#include "SliceUtil.h"

#include <Ice/Ice.h>

std::string StringToUtf8(std::string astr)
{
	wchar_t wbuf[1024]={0};

	std::string srt;
	std::wstring wst;
	std::string stt=astr;
	int count=::MultiByteToWideChar(CP_ACP,0,astr.c_str(),(int)astr.size(),wbuf,0);
	if(count>=1024)
	{
		wchar_t *pwbuf=new wchar_t[count+1];
		if(pwbuf==NULL)
		{
			puts("Allow memory failed");
			return "";
		}
		memset(pwbuf,0,count*sizeof(wchar_t)+sizeof(wchar_t));

		MultiByteToWideChar(CP_ACP,0,astr.c_str(),(int)astr.size(),pwbuf,count+1);
		wst=pwbuf;
		delete [] pwbuf;
		srt=IceUtil::wstringToString(wst);
	}else
	{
		count=MultiByteToWideChar(CP_ACP,0,astr.c_str(),(int)astr.size(),wbuf,1024);
		srt=IceUtil::wstringToString(wbuf);
	}

	return srt;

}

std::string Utf8ToString(std::string ustr)
{
	std::wstring wst=IceUtil::stringToWstring(ustr);
	std::string srt;

	char buf[1024]={0};
	int count=::WideCharToMultiByte(CP_ACP,0,wst.c_str(),(int)wst.size(),buf,0,NULL,NULL);
//	printf("count:%d\n",count);
	if(count>=1024)
	{
		char *pbuf=new char[count+1];
		if(pbuf==NULL)
		{
			puts("Allow memory failed");
			return "";
		}
		memset(pbuf,0,count+1);
		count=WideCharToMultiByte(CP_ACP,0,wst.c_str(),(int)wst.size(),pbuf,count+1,NULL,NULL);
		srt=pbuf;
		delete [] pbuf;
		return srt;
		
	}else
	{
		count=WideCharToMultiByte(CP_ACP,0,wst.c_str(),(int)wst.size(),buf,1024,NULL,NULL);
		srt=buf;
	}

	return srt;

}
