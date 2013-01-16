//#pragma once

#ifndef SVDB_STRKEY_
#define SVDB_STRKEY_

#include <string>
#include "utiltype.h"

#undef	S_STRDUP

#ifdef WIN32
#define S_STRDUP	_strdup
#else
#define S_STRDUP	strdup
#endif

namespace svutil{
	class word{
	public:
		word(){m_word=S_STRDUP("");}
		word(const char* wd);
		word(const word& wd);
		word(const std::string &str);
		virtual ~word()
		{
			if(m_word)
				free(m_word);
		}

		word& operator=(const word& wd);

		word& operator=(const char *p);

		bool operator==(const word &wd) const;
		bool operator<(const word &wd) const;

		bool operator>(const word &wd) const;


		bool operator>=(const word &wd) const;
		bool operator<=(const word &wd) const;

		const char *getword(void) const 
		{
			return m_word;
		}

		operator char*() const
		{
			return m_word;
		}
		operator const char *() const
		{
			return m_word;
		}
		bool empty()
		{
			if((m_word==NULL)||(strlen(m_word)==0))
				return true;
			return false;
		}

	private:
		char *m_word;
	};
}

#endif
