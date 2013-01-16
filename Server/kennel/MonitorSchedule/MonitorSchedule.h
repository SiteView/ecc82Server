#ifndef  DRAGONFLOW_MONITORSCHEDULE
#define  DRAGONFLOW_MONITORSCHEDULE

#include <cc++/string.h>
#include <list>
#include <cc++/numbers.h>
#include "MSException.h"
#include "Util.h"
#include "mydef.h"
#include <map>


#ifdef	CCXX_NAMESPACES
using namespace ost;
#endif

using namespace std;

typedef std::map< string,string > STRINGDICT;


#define		GUIDLEN		100

class Univ
{
public:
	static int seid;
	static int msappend;
	static int pluscount;
	static bool enablemass;
	static bool enabltows;

};


#endif