//#pragma once
#ifndef	SVDB_BUFFER_
#define SVDB_BUFFER_
#include "svdbtype.h"
#include <string.h>

namespace svutil{
	class buffer
	{
	public:
		buffer(void);
		~buffer(void);
		enum { maxsize=2147483646 };

		buffer(const buffer &buf);
		buffer(S_UINT size);
		bool checksize(S_UINT length);
		S_UINT size()
		{
			return m_length;
		}

		void clearbuffer();

		bool resetsize(S_UINT length);

		buffer &operator=(const buffer &buf);

		operator char *()
		{
			return m_pbuf;
		}
		operator const char *() const
		{
			return m_pbuf;
		}
		operator void *()
		{
			return (void *)m_pbuf;
		}

        char & operator [](S_UINT index);

		char * getbuffer()
		{
			return m_pbuf;
		}
		void zerobuf()
		{
			if(m_pbuf!=NULL)
				memset(m_pbuf,0,m_length);

		}
	private:
		S_UINT m_length;
		char *m_pbuf;
	};
}

#endif
