#ifndef	SVDB_PAGE_
#define SVDB_PAGE_

#include "svdbtype.h"



struct PageHead{
	enum pagetype{
		unknowpagetype=0,
		tableheadpagetype=1,
		recordpagetype=2,
		temptablepagetype=3,
		temprecordpagetype=4,
		queueheadpagetype=5,
		queuerecordpagetype=6
	};
	PageHead()
	{
		m_fileindex=0;
		m_mpos=0;
		m_pos=-1;
		m_unused=1;
		m_pagetype=unknowpagetype;
		m_pagesize=0;
		m_datasize=0;
		m_nextrecordpage=-1;
		m_prerecordpage=-1;
		m_recordcount=0;
		m_recordtype=0;
		m_currentdatapos=0;
		m_currentdatalen=0;
		m_tablesize=0;
		m_badpage=0;
		m_flag=0;
		F_SET(m_flag,PAGEUNUSED);
		m_lastrecordpos=-1;
		memset(m_span,0,444);
	}
	~PageHead(){;};
	int m_fileindex;
	int m_mpos;
	int m_pos;
	int m_unused;
	int m_pagetype;
	int m_pagesize;
	int m_datasize;
	int m_nextrecordpage;
	int m_prerecordpage;
	int m_recordcount;
	int m_recordtype;
	int m_currentdatapos;
	int m_currentdatalen;
	int m_tablesize;        //only use dbhead
	int m_badpage;
	unsigned int m_flag;
	int m_lastrecordpos;
	char m_span[444];
};

struct PagePos{

	PagePos()
	{
		m_fileindex=0;
		m_pos=0;
	}
	PagePos(const struct PagePos &pp)
	{
		m_fileindex=pp.m_fileindex;
		m_pos=pp.m_pos;
	}

	PagePos & operator=(const struct PagePos &pp)
	{
		m_fileindex=pp.m_fileindex;
		m_pos=pp.m_pos;
		return *this;
	}

	int m_fileindex;
	int m_pos;

};


class PagePool;

class Page
{
public:
	Page(void);
	Page(PagePool *ppt,int index);
	~Page(void);

	void PutPagePos(PagePos pos)
	{
		m_mpos=pos;
	}

	bool swapin();
	bool swapout(bool isdel);

	bool swapinex();
	bool swapoutex(bool isdel);

	char *m_data;
	char *m_rawdata;
	bool m_ischange;
	bool m_isuse;
	int m_pos;

	PagePos m_mpos;

	PageHead * GetPageHead()
	{
		if(m_rawdata==NULL)
			return NULL;
		return (PageHead *)m_rawdata;
	}
//	struct PageHead m_Head;

	PagePool *m_ppt;

protected:

};

#endif
