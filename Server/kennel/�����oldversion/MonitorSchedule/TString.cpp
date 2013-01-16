#include ".\tstring.h"

TString::TString(void)
{
}

TString::~TString(void)
{
}

int TString::Format(const char * format,...)
{
	clear();
	int chars=1;
	va_list args;
	va_start(args, format);
	while(true)
	{
    	init();
		resize(chars);

		char *ptr = new char[chars];
		if(ptr==NULL)
			return 0;
		int n=vsnprintf(ptr, chars, format, args);
		if(n>-1)
		{
			set(ptr,strlen(ptr));
			setLength(strlen(getText()));
			va_end(args);
			delete [] ptr;
			return 0;
		}
		else
		{
			delete [] ptr;
			clear();
	    	chars+=256;
		}
	}
}

int TString::Format(int len,const char * format,...)
{
	clear();
	int chars=len;
	init();
	resize(chars);

        va_list args;
        va_start(args, format);

        char *ptr = getText();
	vsnprintf(ptr, chars, format, args);
        setLength(strlen(getText()));
	va_end(args);

	return 0;

}

TString TString::Left(int nCount)
{
	if(nCount<=0)
		return "";

	if(nCount>=this->getLength())
		return *this;

	return this->substr(0,nCount);	

}

TString TString::Right(int nCount)
{
	if(nCount<=0)
		return "";

	if(nCount>=this->getLength())
		return *this;

	return substr(getLength()-nCount,nCount);

}

int TString::CompareNoCase(const char * str)
{
	return stricmp(this->getText(),str);
}
