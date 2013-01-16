#ifndef	SVDB_STRINGMAP_
#define	SVDB_STRINGMAP_

#include "svdbtype.h"
#include "SerialBase.h"
#include "libutil/hashtable.h"
#include <list>


class StringMap : public svutil::hashtable<svutil::word,svutil::word>, public SerialBase
{
public:
	StringMap(void);
	StringMap(S_UINT size):svutil::hashtable<svutil::word,svutil::word>(size)
	{;};

	S_UINT	GetRawDataSize(void);
	char*	GetRawData(char *lpbuf,S_UINT bufsize);
	BOOL	CreateObjectByRawData(const char *lpbuf,S_UINT bufsize);

	~StringMap(void);
};

typedef std::list<StringMap *>	STRMAPLIST;


#endif
