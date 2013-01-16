#include "RecordSet.h"
#include "util.h"

RecordSet::RecordSet(void)
{
}

RecordSet::~RecordSet(void)
{
	RECORDLIST::iterator it;
	for(it=m_records.begin();it!=m_records.end();it++)
	{
		if((*it)!=NULL)
			delete (*it);
	}
	m_records.clear();
}

RecordValue::RecordValue()
{
	vt=DataType::nulltype;
	dvalue.sv=NULL;

}
RecordValue::~RecordValue()
{
	if(vt==DataType::stringtype)
		if(dvalue.sv)
			delete [] dvalue.sv;

}

RecordValue::RecordValue(const RecordValue &value)
{
	vt=value.vt;
	switch(vt)
	{
	case	DataType::inttype:
		dvalue.iv=value.dvalue.iv;
		break;
	case	DataType::floattype:
		dvalue.fv=value.dvalue.fv;
		break;
	case	DataType::stringtype:
		if(value.dvalue.sv)
			dvalue.sv=_strdup(value.dvalue.sv);
		break;
	default: 
		dvalue.sv=NULL; 
		break;
	}

}

RecordValue::operator int() const
{
	if(vt==DataType::inttype)
		return dvalue.iv;
	else
		return 0;
}

RecordValue::operator char *() const
{
	if(vt==DataType::stringtype)
		return _strdup(dvalue.sv);
	else if(vt==DataType::inttype)
	{
		char buf[100]={0};
		sprintf(buf,"%d",dvalue.iv);
		return _strdup(buf);
	}else if(vt==DataType::floattype)
	{
		char buf[100]={0};
		sprintf(buf,"%0.2f",dvalue.fv);
		return _strdup(buf);
	}
	return NULL;
}
RecordValue::operator string() const
{
	if(vt==DataType::stringtype)
		return dvalue.sv;
	else if(vt==DataType::inttype)
	{
		char buf[100]={0};
		sprintf(buf,"%d",dvalue.iv);
		return (std::string)buf;
	}else if(vt==DataType::floattype)
	{
		char buf[100]={0};
		sprintf(buf,"%0.2f",dvalue.fv);
		return (std::string)buf;
	}
	return "";

}

RecordValue::operator float() const
{
	if(vt==DataType::floattype)
		return dvalue.fv;
	else
		return 0.00;
}

RecordValue &RecordValue::operator=(int value)
{
	vt=DataType::inttype;
	dvalue.iv=value;
	return *this;
}

RecordValue &RecordValue::operator=(float value)
{
	vt=DataType::floattype;
	dvalue.fv=value;
	return *this;
}
RecordValue &RecordValue::operator=(const char *value)
{
	if(value)
	{
		vt=DataType::stringtype;
		dvalue.sv=_strdup(value);
	}

	return *this;
}
RecordValue &RecordValue::operator=(const std::string &value)
{
	vt=DataType::stringtype;
	dvalue.sv=_strdup(value.c_str());
	return *this;
}

RecordValue &RecordValue::operator=(const RecordValue &value)
{
	switch(value.vt)
	{
	case	DataType::inttype:
		dvalue.iv=value.dvalue.iv;
		break;
	case	DataType::floattype:
		dvalue.fv=value.dvalue.fv;
		break;
	case	DataType::stringtype:
		if(vt==DataType::stringtype)
		{
			if(dvalue.sv)
			{
				delete [] dvalue.sv;
				dvalue.sv=NULL;
			}
		}
		if(value.dvalue.sv)
			dvalue.sv=_strdup(value.dvalue.sv);
		break;
	default: 
		dvalue.sv=NULL; 
		break;
	}
	vt=value.vt;
	return *this;
}

string FindLabelInresMap(string label,StringMap *resMap)
{
	if(resMap==NULL)
		return label;
	svutil::word *pword=NULL;
	pword=resMap->find(label.c_str());
	if(pword==NULL)
		return label;
	else
		return (string)pword->getword();
}

bool RecordSet::CreateObjectByRawData(StringMap *resMap,const char *buf,S_UINT len)
{
	if(buf==NULL)
		return false;
	if(len<=0)
		return false;

	const char *pt=buf;
	const char *pend=buf+len;
	int tlen=sizeof(S_UINT);

	try{
		S_UINT size=0;
		S_UINT RecordCount=0;
		memmove(&size,pt,tlen);
		pt+=tlen;
		if(!m_type.CreateObjectByRawData(pt,size))
		{
			puts("Create obj failed");
			return false;
		}
		pt+=size;
		memmove(&RecordCount,pt,tlen);
		pt+=tlen;

		int count=m_type.m_data.size();
		if(count<=0)
			return false;
		string slabel="";
		Record *prd=NULL;
		for(int n=0;n<RecordCount;n++)
		{
			RecordHead *prht=(RecordHead *)pt;
			pt+=sizeof(RecordHead);
			prd=new Record();
			if(prd==NULL)
				return false;
			prd->m_createtime=prht->createtime;
			prd->m_state=prht->state;
			if((prht->state==Record::STATUS_BAD)||(prht->state==Record::STATUS_NULL)||(prht->state==Record::STATUS_DISABLE))
			{
				prd->m_error=_strdup(pt);
				pt+=strlen(prd->m_error)+1;
				m_records.push_back(prd);
				continue;
			}
			char dstr[2048]={0};

			bool badRCD=false;
			string badStr;
			for(int i=0;i<count;i++)
			{
				if(badRCD)
					break;

				DataType *prt=m_type.m_data[i];
				if(prt==NULL)
				{
					badRCD= true;
					badStr+="DataType==NULL;  ";
					continue;
				}
				int iv=0;
				float fv=0.00f;
				string sv="";

				int dlen=0;
				RecordValue *prv=new RecordValue();
				if(prv==NULL)
				{
					badRCD= true;
					badStr+="new RecordValue ==NULL;  ";
					continue;
				}

				slabel=FindLabelInresMap(prt->label,resMap);
				switch(prt->type)
				{
				case	DataType::inttype:
					if((dlen=::parsebuffer(pt,pend-pt,iv))==0)
					{
						badRCD= true;
						badStr+="bad int of ";
						badStr+=slabel;
						badStr+=";  ";
						puts(badStr.c_str());
						delete prv;
						continue;
					}
					*prv=iv;
					pt+=dlen;
					if(i==0)
						sprintf(dstr,"%s=%d",slabel.c_str(),iv);
					else
						sprintf(dstr,"%s,%s=%d",dstr,slabel.c_str(),iv);
					break;
				case	DataType::floattype:
					if((dlen=::parsebuffer(pt,pend-pt,fv))==0)
					{
						badRCD= true;
						badStr+="bad float of ";
						badStr+=slabel;
						badStr+=";  ";
						puts(badStr.c_str());
						delete prv;
						continue;
					}
					*prv=fv;
					pt+=dlen;
					if(i==0)
						sprintf(dstr,"%s=%0.2f",slabel.c_str(),fv);
					else
						sprintf(dstr,"%s,%s=%0.2f",dstr,slabel.c_str(),fv);
					break;
				case	DataType::stringtype:
					if((dlen=::parsebuffer(pt,pend-pt,sv))==0)
					{
						badRCD= true;
						badStr+="bad string of ";
						badStr+=slabel;
						badStr+=";  ";
						puts(badStr.c_str());
						delete prv;
						continue;
					}
					*prv=sv;
					pt+=dlen;
					if(i==0)
						sprintf(dstr,"%s=%s",slabel.c_str(),sv.c_str());
					else
						sprintf(dstr,"%s,%s=%s",dstr,slabel.c_str(),sv.c_str());
					break;
				default: 
					badRCD= true;
					badStr+="uknown data type of ";
					badStr+=slabel;
					badStr+=";  ";
					puts(badStr.c_str());
					delete prv;
					continue;
				}

				prd->m_value.insert(prt->name,prv);
			}
			if(badRCD)
			{
				prd->m_error= _strdup(badStr.c_str());
				prht->state= Record::STATUS_BAD;
				prd->m_state=prht->state;
			}
			else
			{
				strcat(dstr,".");
				prd->m_displaystr=_strdup(dstr);
			}
			if(prd!=NULL)
				m_records.push_back(prd);
		}

	}catch(...)
	{
		return false;
	}
	return true;
}

bool RecordSet::CreateObjectByRawData(const char *buf,S_UINT len)
{
	if(buf==NULL)
		return false;
	if(len<=0)
		return false;

	const char *pt=buf;
	const char *pend=buf+len;
	int tlen=sizeof(S_UINT);

	try{
        S_UINT size=0;
		S_UINT RecordCount=0;
		memmove(&size,pt,tlen);
		pt+=tlen;
		if(!m_type.CreateObjectByRawData(pt,size))
		{
			puts("Create obj failed");
			return false;
		}
		pt+=size;
		memmove(&RecordCount,pt,tlen);
		pt+=tlen;

		int count=m_type.m_data.size();
		if(count<=0)
			return false;
		Record *prd=NULL;
		for(int n=0;n<RecordCount;n++)
		{
			RecordHead *prht=(RecordHead *)pt;
			pt+=sizeof(RecordHead);
			prd=new Record();
			if(prd==NULL)
				return false;
			prd->m_createtime=prht->createtime;
			prd->m_state=prht->state;
			if((prht->state==Record::STATUS_BAD)||(prht->state==Record::STATUS_NULL)||(prht->state==Record::STATUS_DISABLE))
			{
				prd->m_error=_strdup(pt);
				pt+=strlen(prd->m_error)+1;
				m_records.push_back(prd);
				continue;
			}
			char dstr[2048]={0};

			bool badRCD=false;
			string badStr;
			for(int i=0;i<count;i++)
			{
				if(badRCD)
					break;

				DataType *prt=m_type.m_data[i];
				if(prt==NULL)
				{
					badRCD= true;
					badStr+="DataType==NULL;  ";
					continue;
				}
				int iv=0;
				float fv=0.00f;
				string sv="";

				int dlen=0;
				RecordValue *prv=new RecordValue();
				if(prv==NULL)
				{
					badRCD= true;
					badStr+="new RecordValue ==NULL;  ";
					continue;
				}
				switch(prt->type)
				{
				case	DataType::inttype:
					if((dlen=::parsebuffer(pt,pend-pt,iv))==0)
					{
						badRCD= true;
						badStr+="bad int of ";
						badStr+=prt->label;
						badStr+=";  ";
						puts(badStr.c_str());
						delete prv;
						continue;
					}
					*prv=iv;
					pt+=dlen;
					if(i==0)
						sprintf(dstr,"%s=%d",prt->label,iv);
					else
						sprintf(dstr,"%s,%s=%d",dstr,prt->label,iv);
					break;
				case	DataType::floattype:
					if((dlen=::parsebuffer(pt,pend-pt,fv))==0)
					{
						badRCD= true;
						badStr+="bad float of ";
						badStr+=prt->label;
						badStr+=";  ";
						puts(badStr.c_str());
						delete prv;
						continue;
					}
					*prv=fv;
					pt+=dlen;
					if(i==0)
						sprintf(dstr,"%s=%0.2f",prt->label,fv);
					else
						sprintf(dstr,"%s,%s=%0.2f",dstr,prt->label,fv);
					break;
				case	DataType::stringtype:
					if((dlen=::parsebuffer(pt,pend-pt,sv))==0)
					{
						badRCD= true;
						badStr+="bad string of ";
						badStr+=prt->label;
						badStr+=";  ";
						puts(badStr.c_str());
						delete prv;
						continue;
					}
					*prv=sv;
					pt+=dlen;
					if(i==0)
						sprintf(dstr,"%s=%s",prt->label,sv.c_str());
					else
						sprintf(dstr,"%s,%s=%s",dstr,prt->label,sv.c_str());
					break;
				default: 
					badRCD= true;
					badStr+="uknown data type of ";
					badStr+=prt->label;
					badStr+=";  ";
					puts(badStr.c_str());
					delete prv;
					continue;

				}

				prd->m_value.insert(prt->name,prv);
			}
			if(badRCD)
			{
				prd->m_error= _strdup(badStr.c_str());
				prht->state= Record::STATUS_BAD;
				prd->m_state=prht->state;
			}
			else
			{
				strcat(dstr,".");
				prd->m_displaystr=_strdup(dstr);
			}
			if(prd!=NULL)
				m_records.push_back(prd);
		}

	}catch(...)
	{
		return false;
	}
	return true;
}