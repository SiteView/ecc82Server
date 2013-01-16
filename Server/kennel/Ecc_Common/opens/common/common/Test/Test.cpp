// Test.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <cc++/file.h>

#ifdef	CCXX_NAMESPACES
using namespace std;
using namespace ost;
#endif


void testmapfile()
{
	try{
		ost::MappedFile file("d:\\tst.txt",ost::File::accessReadWrite,1024*1024);
		

		char *p=(char *)file.fetch();
		if(p)
		{
			sprintf(p,"this is a test");
		}
		file.release(0,0);

	}catch(...)
	{
		puts("Exception");
	}

}

int _tmain(int argc, _TCHAR* argv[])
{
	puts("dddd");

	testmapfile();

	return 0;
}

