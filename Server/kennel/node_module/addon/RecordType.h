#ifndef SVDB_RECORDTYPE_
#define SVDB_RECORDTYPE_

#include "svdbtype.h"
#include <vector>
#include "../libutil/time.h"

using namespace svutil;

class DataType
{
public:
	DataType(void){
		type=nulltype;
		memset(unit,0,UNITLEN);

	};
	~DataType(void){

	};
	DataType(const DataType &dt)
	{
		type=dt.type;
		memcpy(unit,dt.unit,UNITLEN);
		memcpy(name,dt.name,NAMELEN);
		memcpy(label,dt.label,LABELLEN);
	}

	DataType &operator=(const DataType &dt)
	{
		type=dt.type;
		memcpy(unit,dt.unit,UNITLEN);
		memcpy(name,dt.name,NAMELEN);
		memcpy(label,dt.label,LABELLEN);
		return *this;
	}
	enum valuetype{
		nulltype=0,
		inttype=1,
		floattype=2,
		stringtype=3
	};


	valuetype type;
	char  unit[UNITLEN];
	char  name[NAMELEN];
	char  label[LABELLEN];

};

class RecordType
{
public:
	RecordType(void){
		m_monitortype=0;
		m_monitorid="";
	};
	~RecordType(void)
	{
		for(std::vector<DataType *>::iterator it=m_data.begin(); it!=m_data.end(); ++it)
			if((*it)!=NULL)
				delete (*it);
		m_data.clear();
	};

	int	m_monitortype;
	std::string m_monitorid;

	std::vector<DataType *> m_data;

	S_UINT GetRawDataSize();
	bool GetRawData(char *buf,S_UINT buflen);
	bool CreateObjectByRawData(const char *buf,S_UINT buflen);

};

struct RecordHead
{
	int prercord;
	int state;
	TTime createtime;
	int datalen;

};


#endif