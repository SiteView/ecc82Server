#ifndef	SVDB_SECTION_
#define	SVDB_SECTION_

#include "SerialBase.h"
#include "svdbtype.h"
#include "StringMap.h"
#include "util.h"

class CommonData :
	public SerialBase
{
public:
	CommonData(){
		m_data=NULL;
		m_DataLen=0;
		m_DataType=nulltype;
	}
	CommonData(const void *data,S_UINT len,int type){
		if((len>0)&&(data!=NULL))
		{
			m_data=new char[len];
			if(m_data)
			{
				memmove(m_data,data,len);
				m_DataLen=len;
				m_DataType=type;

			}else
			{
				m_data=NULL;
				m_DataLen=0;
	        	m_DataType=nulltype;
			}
		}else
		{
			m_data=NULL;
			m_DataLen=0;
	        m_DataType=type;
		}
	}


	enum datatype
	{
		nulltype=0,
		inttype=1,
		stringtype=2,
		binarytype=3
	};

	~CommonData()
	{
		if(m_data)
			delete [] m_data;
	}

	CommonData(const CommonData &data)
	{
		if((data.m_data!=NULL)&&(data.m_DataLen>0))
		{
			m_data=new char[data.m_DataLen];
			if(m_data)
			{
				memmove(m_data,data.m_data,data.m_DataLen);
				m_DataLen=data.m_DataLen;
				m_DataType=data.m_DataType;

			}else
			{
				m_data=NULL;
				m_DataLen=0;
			}

		}else
		{
			m_data=data.m_data;
			m_DataLen=data.m_DataLen;
			m_DataType=data.m_DataType;
		}

	}

	CommonData &operator=(const CommonData &data)
	{
		if(m_data)
			delete [] m_data;
		if((data.m_data!=NULL)&&(data.m_DataLen>0))
		{
			m_data=new char[data.m_DataLen];
			if(m_data)
			{
				memmove(m_data,data.m_data,data.m_DataLen);
				m_DataLen=data.m_DataLen;
				m_DataType=data.m_DataType;
			}else
			{
				m_data=NULL;
				m_DataLen=0;
	     		m_DataType=nulltype;
			}
		}else
		{
			m_data=data.m_data;
			m_DataLen=data.m_DataLen;
			m_DataType=data.m_DataType;

		}

		return (*this);
	}

	bool PutData(const void *data,S_UINT len,int datatype)
	{
		if((len>0)&&(data!=NULL))
		{
			if(m_data)
				delete [] m_data;
			m_data=new char[len];
			if(m_data)
			{
				memmove(m_data,data,len);
				m_DataLen=len;
				m_DataType=datatype;
			}else
			{
				m_data=NULL;
				m_DataLen=0;
				m_DataType=nulltype;
				return false;
			}
     		return true;

		}else if(len==0)
		{
			if(m_data)
				delete [] m_data;
			m_data=NULL;
			m_DataLen=0;
			m_DataType=datatype;
			return true;
		}
		return false;
	}
    void *GetData() const
	{
		return m_data;
	}
	S_UINT GetDatalen() const
	{
		return m_DataLen;
	}

	void PutDataType(int type){m_DataType=type;}
	int  GetDataType(void){return m_DataType;}


	S_UINT	GetRawDataSize(void);
	char*	GetRawData(char *lpbuf,S_UINT bufsize);
	BOOL	CreateObjectByRawData(const char *lpbuf,S_UINT bufsize);

protected:
	S_UINT	m_DataLen;
	void	*m_data;
	int		m_DataType;
    

};

typedef svutil::hashtable< svutil::word,CommonData* > SECTIONDATA;

class Section :
	public SerialBase
{
public:
	Section(void);
	~Section(void);
	enum  { hashtablesize = 27 };
	void PutSection(word section)
	{
		m_SectionName=section;
	}
	word GetSection(void)
	{
		return m_SectionName;
	}


	bool Find(word name)
	{
		CommonData **pdata=m_data.find(name);
		return pdata!=NULL;
	}

	bool Push(word name,CommonData *data)
	{
		CommonData **pdata=m_data.find(name);
		if(pdata!=NULL)
		{
			DeleteKeyInKeyOrder(name);
			delete (*pdata);
		    
		}

		m_data[name]=data;
		m_KeyOrder.push_back(name);

		return true;
	}

	bool Push(word name,const char *data,S_UINT len)
	{
		CommonData* pdata=new CommonData(data,len,CommonData::binarytype);
		if(!pdata)
			return false;
//		if(!pdata->CreateObjectByRawData(data,len))
//		    return false;
		return Push(name,pdata);
	}
	bool Push(word name,word str)
	{
		string strv=str.getword();
		S_UINT len=strv.size();

		CommonData *pdata=new CommonData(str.getword(),len,CommonData::stringtype);
		if(!pdata)
			return false;
		return Push(name,pdata);
	}
	bool Push(word name,int value)
	{
		CommonData *pdata=new CommonData(&value,sizeof(int),CommonData::inttype);
		if(!pdata)
			return false;
		return Push(name,pdata);
	}

	bool Pop(word name,int &value)
	{
		if(!Find(name))
			return false;
		if(m_data[name]->GetDataType()!=CommonData::inttype)
			return false;
		if(m_data[name]->GetDatalen()!=sizeof(int))
			return false;
		memmove(&value,m_data[name]->GetData(),sizeof(int));
		return true;
	}

	bool Pop(word name,CommonData **data)
	{
		if(!Find(name))
			return false;
		*data=m_data[name];
		return true;
	}

	bool Pop(word name,void **data,S_UINT &len)
	{
		if(!Find(name))
			return false;
		*data=m_data[name]->GetData();
		len=m_data[name]->GetDatalen();
		return true;
	}

	bool Pop(word name,word &str)
	{
		S_UINT len=0;
		char *pdata=NULL;
		if(!Pop(name,(void **)&pdata,len))
			return false;
		if(len==0)
		{
			str="";
			return true;
		}

		char *pstr=new char[len+1];
		if(!pstr)
			return false;
		memset(pstr,0,len+1);
		memmove(pstr,pdata,len);
		str=pstr;
		delete pstr;
		return true;
	}

	bool Delete(word key)
	{
		CommonData **pdata=m_data.find(key);
		if(pdata==NULL)
			return false;
        delete (*pdata);

		DeleteKeyInKeyOrder(key);
		return m_data.erase(key);
	}

	bool EditKeyName(word oldname,word newname)
	{
		if(oldname == newname)
			return true;

		CommonData **pd=m_data.find(oldname);
		if(pd==NULL)
			return false;
		if(Find(newname))
			return false;

		m_data[newname]=(*pd);
		m_data.erase(oldname);
		return ReplaceSectionInKeyOrder(oldname,newname);
	}

	bool GetKeys(std::list<string> &keylist)
	{
		WORDLIST::iterator it;
		for(it=m_KeyOrder.begin();it!=m_KeyOrder.end();it++)
			keylist.push_back((*it).getword());

/*		if(m_FirstKey.empty())
			return true;

		word key=m_FirstKey;
		while(!(key.empty()))
		{
			keylist.push_back(key.getword());
			key=m_data[key]->GetNext();
		}
*/


/*		SECTIONDATA::iterator it;
		while(m_data.findnext(it))
		{
			keylist.push_back((*it).getkey().getword());
		}
*/
		return true;
	}

	int GetValueTypeByKey(word key)
	{
		if(!Find(key))
			return -1;
		return m_data[key]->GetDataType();
	}

	S_UINT	GetRawDataSize(void);
	char*	GetRawData(char *lpbuf,S_UINT bufsize);
	BOOL	CreateObjectByRawData(const char *lpbuf,S_UINT bufsize);

protected:
	bool ReplaceSectionInKeyOrder(word oldkey,word newkey)
	{
		WORDLIST::iterator it;
		for(it=m_KeyOrder.begin();it!=m_KeyOrder.end();it++)
		{
			if((*it)==oldkey)
			{
				m_KeyOrder.insert(it,newkey);
				return DeleteKeyInKeyOrder(oldkey);
			}
		}
		return false;

	}

	bool DeleteKeyInKeyOrder(word key)
	{
		WORDLIST::iterator it;
		for(it=m_KeyOrder.begin();it!=m_KeyOrder.end();it++)
		{
			if((*it)==key)
			{
				m_KeyOrder.erase(it);
				return true;
			}
		}
		return false;

	}
	WORDLIST m_KeyOrder;
	word	m_SectionName;
	SECTIONDATA	m_data;
	S_UINT	m_hashtablesize;

};
#endif
