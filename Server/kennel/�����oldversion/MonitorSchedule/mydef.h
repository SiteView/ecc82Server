#ifndef	DRAGONFLOW_MYDEF
#define	DRAGONFLOW_MYDEF
#include <string.h>
#include <iostream>
#include "TString.h"
#include <libutil/Time.h>
#include <list>

#include <libutil/hashtable.h>


using namespace std;
using namespace ost;
using namespace svutil;

typedef svutil::hashtable<svutil::word,svutil::word>	STRINGMAP;

#ifdef	SVVER70
#undef	SVVER70
#endif
#define	SVVER70

typedef	std::list<string> CStringList;
typedef TString CString;
typedef TTime   CTime;
typedef TTimeSpan CTimeSpan;




#ifndef  WIN32

#define	_strdup	strdup
#define	_stricmp	stricmp

//typedef strdup	_strdup;
typedef unsigned int  UINT;
typedef int BOOL;
typedef unsigned int  DWORD;

typedef const char* LPCSTR;
typedef const char* LPCTSTR;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#endif


#endif
