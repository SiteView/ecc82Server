#ifndef SVDB_QUEUERECORD_
#define SVDB_QUEUERECORD_

#include "svdbtype.h"
#include "../libutil/time.h"

struct RecordPos{
	RecordPos()
	{
		m_page=-1;
		m_pos=-1;
	}
	RecordPos(const struct RecordPos &pp)
	{
		m_page=pp.m_page;
		m_pos=pp.m_pos;
	}

	RecordPos & operator=(const struct RecordPos &pp)
	{
		m_page=pp.m_page;
		m_pos=pp.m_pos;
		return *this;
	}

	bool operator==(const struct RecordPos &rp)
	{
		return ((m_page==rp.m_page)&&(m_pos==rp.m_pos));
	}

	int m_page;
	int m_pos;

};

typedef struct RecordPos	RECORDPOS, *LPRECORDPOS;

struct QueueHead{

	QueueHead()
	{
		m_prerecord.m_page=-1;
		m_prerecord.m_pos=-1;

		m_nextrecord.m_page=-1;
		m_nextrecord.m_pos=-1;
		m_level=1;
		m_datalen=0;
		m_cblen=0;
	}
	~QueueHead()
	{
		;
	}

	int	 m_cblen;

	svutil::TTime m_createtime;
	S_UINT			m_datalen;
	RECORDPOS			m_prerecord;
	RECORDPOS			m_nextrecord;
	int			m_level;
};

typedef struct QueueHead QUEUEHEAD, *LPQUEUEHEAD;


class QueueRecord
{
public:
	QueueRecord(void);
	~QueueRecord(void);

	QueueRecord(string label,svutil::TTime ctime,int level=1);
	QueueRecord(string label,svutil::TTime ctime,const char *data,S_UINT datalen,int level=1);

	inline bool PutLabel(string label)
	{
		m_label=label;
		
		return true;
	}

	inline bool PutCreateTime(svutil::TTime ctime)
	{
		m_createtime=ctime;
		return true;
	}

	inline bool PutLevel(int level)
	{
		m_level=level;
		return true;
	}
	inline bool PutData(const char *data,S_UINT datalen)
	{
		m_datalen=0;
		if(m_data!=NULL)
		{
			delete [] m_data;
			data=NULL;
		}
		m_data=new char[datalen];
        if(m_data==NULL)
			return false;
		memmove(m_data,data,datalen);
		m_datalen=datalen;

		return true;
	}

	inline string GetLabel() const
	{
		return m_label;
	}

	inline const char *GetData() const
	{
		return m_data;
	}
	inline S_UINT GetDataLen() const
	{
		return m_datalen;
	}

	inline svutil::TTime GetCreateTime() const 
	{
		return m_createtime;
	}

	inline int GetLevel()
	{
		return m_level;
	}

	S_UINT	GetRawDataSize(void);
	char*	GetRawData(char *lpbuf,S_UINT bufsize);
	BOOL	CreateObjectByRawData(const char *lpbuf,S_UINT bufsize);


protected:
	svutil::TTime m_createtime;
	int m_level;
	std::string m_label;
	char *m_data;
	S_UINT m_datalen;
};

#endif
