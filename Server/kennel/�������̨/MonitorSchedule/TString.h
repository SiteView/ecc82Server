#ifndef  DRAGONFLOW_TSTRING
#define DRAGONFLOW_TSTRING

#include <cc++\string.h>

#ifdef	CCXX_NAMESPACES
using namespace ost;
#endif


class TString :
	public String
{
public:
	TString(void);
	~TString(void);
	TString(const char *str):String(str)
	{
		;
	}
	TString(std::string str):String(str)
	{
		;
	}
	int Format(const char * format,...);
	int Format(int len,const char * format,...);

	const TString& operator=(const char *str)
	{
		this->set(str);
		return *this;
	}
	TString(ost::String str):String(str)
	{
		;
	}

	TString(const TString &original)
	{
		init();
		//copy(original);
		set(original);
	}



	int Find(const char *str,int beginpos)
	{
		return (int)find(str,beginpos);
	}
	int Find(const char c,int beginpos)
	{
		char fc[2]={0};
		fc[0]=c;
		return (int)find(fc,beginpos);
	}

	int ReverseFind(char c)
	{
		char fc[2]={0};
		fc[0]=c;
		return (int)rfind(fc);
	}

	int ReverseFind(const char* str)
	{
		return (int)rfind(str);
	}



	TString Mid(int start,int len)
	{
		return substr(start,len);
	}

	int Replace(const char *oldstr,const char *newstr)
	{
		int pos=-1,temppos=0;
		size_t len=strlen(oldstr);
		size_t nlen=strlen(newstr);
		if(len<=0)
			return 0;
		int i=0;
		while((pos=Find(oldstr,temppos))>-1)
		{
			temppos=pos;
			replace(pos,len,newstr);
			i++;
			temppos+=(int)nlen;

		}

		return i;

	}

	BOOL IsEmpty()
	{
		return this->empty();
	}

	char *GetBuffer(int nMin)
	{
		return this->getText();
	}

	int GetLength(void)
	{
		return (int)getLength();
	}

	TString &operator=(const TString &original)
	{
		//copy(original); 
		set(original);
		return *this;
	};





	inline TString &operator=(int i)
	{set(16, "%d", i); return *this;};

	inline TString &operator=(long l)
	{set(16, "%l", l); return *this;};

	inline TString &operator=(unsigned long l)
	{set(16, "%ul", l); return *this;};

	inline TString &operator=(float f)
	{set(32, "%f", f); return *this;};

	inline TString &operator=(double d)
	{set(32, "%f", d); return *this;};

	inline TString &operator=(short s)
	{set(8, "%hd", s); return *this;};

	inline TString &operator=(unsigned short s)
	{set(8, "%hu", s); return *this;};

	inline TString &operator=(std::string &s)
	{
		this->set(s.c_str());
		return *this;
	}



	TString Left(int nCount);
	TString Right(int nCount);
	int CompareNoCase(const char * str);
};

#endif
