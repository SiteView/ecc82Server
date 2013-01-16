#ifndef	SVDB_RECORDSET_
#define	SVDB_RECORDSET_

#include "RecordType.h"
#include "libutil/hashtable.h"
#include "StringMap.h"

struct RValue
{
	DataType::valuetype vt;
	union value{
		int		iv;
		float	fv;
		char *	sv;
	} dvalue;
};

class RecordValue :public RValue
{
public:
	RecordValue();
	~RecordValue();
	RecordValue(const RecordValue &value);


	operator int() const;
	operator char *() const;
	operator string() const;
	operator float() const;


	RecordValue &operator=(int value);
	RecordValue &operator=(float value);
	RecordValue &operator=(const char *value);
	RecordValue &operator=(const std::string &value);
	RecordValue &operator=(const RecordValue &value);


};

typedef	svutil::hashtable<svutil::word,RecordValue *> VALUEMAP;

//the following info is store for each record (each data collection)
//数据格式: 每行一条记录，m_value 是 key/value pair list(dictionary), 对m_value的解析在template中定义
//m_state  m_createtime m_error m_displaystr m_value  (前后次序不一定对，内容就这些)

class Record
{
public:
	Record()
	{
		m_state=0;
		m_error=NULL;
		m_displaystr=NULL;
	}
	~Record()
	{
		if(m_error!=NULL)
			free(m_error);
		if(m_displaystr!=NULL)
			free(m_displaystr);

		VALUEMAP::iterator it;
		while(m_value.findnext(it))
		{
			RecordValue *prv=(*it).getvalue();
			if(prv!=NULL)
				delete prv;
		}
	}
	enum MonitorState{
	STATUS_OK		= 1,
	STATUS_WARNING	= 2,
	STATUS_ERROR	= 3,
	STATUS_DISABLE	= 4,
	STATUS_BAD		=5,
	STATUS_NULL=0
	};


	std::string GetErrorDescription()
	{
		if(m_error!=NULL)
			return m_error;
		else
			return "";
	}
	bool GetValue(std::string name,RecordValue &value)
	{
		if(name.empty())
			return false;
		RecordValue **rv=m_value.find(name);
		if(rv==NULL)
			return false;
		value=*(*rv);
		return true;
	}

	std::string GetDisplayStr()
	{
		if(m_displaystr!=NULL)
			return m_displaystr;
		else
			return "";
	}

	int GetState()
	{
		return m_state;
	}

	TTime GetCreateTime()
	{
		return m_createtime;
	}

	VALUEMAP & GetVALUEMAP(void)
	{
		return m_value;
	}

protected:

	int			m_state;
	TTime	m_createtime;
	char *m_error;
	char *m_displaystr;
	VALUEMAP	m_value;

	friend class RecordSet;

};

typedef std::list<Record *> RECORDLIST;

class RecordSet
{
public:
	RecordSet(void);
	~RecordSet(void);

	bool CreateObjectByRawData(const char *buf,S_UINT len);
	bool CreateObjectByRawData(StringMap *resMap,const char *buf,S_UINT len);


	RecordType m_type;
    RECORDLIST	m_records;    
};

#endif
