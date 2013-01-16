
#ifndef	SITEVIEW_STDMAP_WITH_SELFSORT_H
#define	SITEVIEW_STDMAP_WITH_SELFSORT_H

#include <string>
using std::string;

struct StlMapWithSelfSort 
{
    bool operator()(const std::string& x, const std::string& y) const;
};

#define NPOS std::string::npos 

inline bool StlMapWithSelfSort::operator()(const std::string& x, const std::string& y) const
{
	try{
		string k1,k2,s1(x),s2(y);
		std::string::size_type pos1,pos2;
		int i1,i2;

		while(1)
		{
			pos1=s1.find(".");
			pos2=s2.find(".");
			if(pos1==NPOS && pos2==NPOS)
			{
				i1= atoi(s1.c_str());
				i2= atoi(s2.c_str());
				return i1<=i2;
			}
			if(pos1==NPOS)
				return true;
			if(pos2==NPOS)
				return false;

			k1= s1.substr(0,pos1+1);
			k2= s2.substr(0,pos2+1);
			i1= atoi(k1.c_str());
			i2= atoi(k2.c_str());

			if( i1 != i2 )
				return i1<i2;

			s1.erase(0,pos1+1);
			s2.erase(0,pos2+1);		
		}
	}
	catch(...)
	{
		printf("Exception in StlMapWithSelfSort");
		return true;
	}
}


typedef std::map<std::string, std::string, StlMapWithSelfSort > SelfDefStrMap;


#endif

