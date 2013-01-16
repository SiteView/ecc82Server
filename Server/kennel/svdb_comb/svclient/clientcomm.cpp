

#include "../svdbtype.h"
#include "util.h"
#include "stlini.h"

#include "svclient.h"
#include "connection.h"
#include "somefunc.h"
#include "clientcomm.h"



CSvTableComm::CSvTableComm(void)
{

}

CSvTableComm::~CSvTableComm(void)
{

}


string	CSvTableComm::GetTableType()
{
	return GetValueInMapStrMap(m_advance,"atrribute","tabletype");
}


bool	CSvTableComm::SetTableType(string type)
{
	PutReturnMapStrMap(m_advance,"atrribute","tabletype",type);
	m_modified= true;
	return true;
}


bool	CSvTableComm::InitializeDataMembers(CTableName tname, string & estr)
{
	if(tname.isEmpty())
	{
		estr+= " CTableName is empty();  ";
		return false;
	}
	PutReturnMapStrMap(m_advance,"atrribute","dbname",tname.dbName);
	PutReturnMapStrMap(m_advance,"atrribute","tablename",tname.tableName);
	PutReturnMapStrMap(m_advance,"atrribute","tabledes",tname.tableName);
	PutReturnMapStrMap(m_advance,"atrribute","tabletype","ECC");

	StrMap ndata;
	m_advance.insert(std::make_pair("fields", ndata));
	m_advance.insert(std::make_pair("relations", ndata));
	m_advance.insert(std::make_pair("kins", ndata));	//for hierarchical database, describe table relation, not describe record relation
	m_advance.insert(std::make_pair("history", ndata));

	m_modified= true;
	return true;
}
// in hierarchy data base, any single section have data in m_values : section/ key/ value and section/ key/ type (int,float,string(default type))

void	CSvTableComm::SetSubmitted()
{
	m_modified= false;
}


S_UINT	CSvTableComm::GetRawDataSize()
{
	S_UINT len=0;

	try{
		std::list<SingelRecord> listrcd;

		SingelRecord r1;
		r1.datalen= GetForestMapRawDataSize(m_values) ;
		r1.data= NULL; 
		r1.monitorid= "m_values";
		listrcd.push_back(r1);

		SingelRecord r2;
		r2.datalen= GetForestMapRawDataSize(m_advance) ;
		r2.data= NULL; 
		r2.monitorid= "m_advance";
		listrcd.push_back(r2);

		len= GetMassRecordListRawDataSize(listrcd);
	}
	catch(...)
	{
		printf(" Exception in CConfig::GetRawDataSize. ");
		len= 0;
	}
	return len;
}

char*	CSvTableComm::GetRawData(char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return NULL;

	try{
		S_UINT totalsize= GetRawDataSize();
		if(bufsize<totalsize)
			return NULL;

		S_UINT len=0,tlen=sizeof(S_UINT);
		char *pt=lpbuf;
		char *data(NULL);

		memmove(pt,&totalsize,tlen);
		pt+=tlen;

		len=2;
		memmove(pt,&len,tlen);
		pt+=tlen;

		data= pt;
		S_UINT nlen=0;
		std::list<SingelRecord> listrcd;


		SingelRecord r1;
		nlen=  GetForestMapRawDataSize(m_values) ;
		r1.datalen= nlen;
		r1.monitorid= "m_values";

		data+= strlen(r1.monitorid.c_str()) + 1;
		data+= tlen;
		r1.data= GetForestMapRawData(m_values, data, nlen) ;

		listrcd.push_back(r1);
		data+= nlen;


		SingelRecord r2;
		nlen=  GetForestMapRawDataSize(m_advance) ;
		r2.datalen= nlen;
		r2.monitorid= "m_advance";

		data+= strlen(r2.monitorid.c_str()) + 1;
		data+= tlen;
		r2.data= GetForestMapRawData(m_advance, data, nlen) ;

		listrcd.push_back(r2);
		data+= nlen;


		for(std::list<SingelRecord>::iterator it=listrcd.begin();it!=listrcd.end();it++)
		{
			len=strlen((it->monitorid).c_str());
			strcpy(pt,(it->monitorid).c_str());
			pt+=len;
			pt[0]='\0';
			pt++;

			memmove(pt,&(it->datalen),tlen);
			pt+=tlen;

			if( (it->data)!=NULL )
				memmove(pt,it->data,it->datalen);
			pt+=it->datalen;
		}
	}catch(...)
	{
		cout<<"Exception to CConfig::GetRawData."<<endl;
		return NULL;
	}

	return lpbuf;
}


bool	CSvTableComm::CreateByRawData(const char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return false;

	if(bufsize<sizeof(S_UINT))
		return false;

	S_UINT len=0,tlen=sizeof(S_UINT);

	try{
		m_values.clear();
		m_advance.clear();

		const char *pend=lpbuf+bufsize;
		const char *pt=lpbuf;
		memmove(&len,pt,tlen);
		pt+=tlen;
		if(pt>pend)
			return false;

		if(len!=bufsize)
		{
			puts("Raw data of CConfig error");
			return false;
		}

		S_UINT count;
		memmove(&count,pt,tlen);
		pt+=tlen;


		std::string str;
		S_UINT datalen(0);
		const char * data(NULL);

		for(S_UINT i=0;i<count;i++)
		{
			str=pt;
			pt+=strlen(str.c_str())+1;
			if(pt>pend)
				return true;

			memmove(&datalen,pt,tlen);
			pt+=tlen;
			if(pt>pend)
				return true;

			data=pt;

			pt+=datalen;
			if(pt>pend)
				return true;

			if(i==0)
				CreateForestMapByRawData(m_values, data, datalen);

			if(i==1)
				CreateForestMapByRawData(m_advance, data, datalen);
		}

		MapStrMap::iterator mit= m_advance.find("history");
		if(mit!=m_advance.end())
		{
			int maxi(0);
			string maxstr;
			int ti(0);
			string strver;
			for(NodeData::iterator nit= mit->second.begin(); nit!=mit->second.end(); ++nit)
			{
				strver= nit->first;
				ti= atoi(strver.c_str());
				if(ti>maxi)
				{
					maxi= ti;
					maxstr= strver;
				}
			}
			m_version= maxstr;
		}
	}catch(...)
	{
		cout<<"Exception to CConfig::CreateByRawData ."<<endl;
		return false;
	}  
	return true;
}


