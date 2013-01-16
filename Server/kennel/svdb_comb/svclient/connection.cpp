


#include "connection.h" 
#include "QueryData.h"
#include "util.h"


bool ConnectDataBaseCommit2(std::list<SingelRecord> & listrcd, string qstr, char * & retp, int & retlen, string & estr)
{
	retp=NULL;
	retlen=0;

	svutil::buffer tbuf;
	char * tdata(NULL);
	S_UINT tlen(0);

	try{
		tlen= GetMassRecordListRawDataSize(listrcd);
		if( !tbuf.checksize(tlen) )
			return false;
		tdata= GetMassRecordListRawData(listrcd,tbuf,tlen); 
		if(tdata==NULL)
			return false;
	}
	catch(...)
	{
		estr+= " Exception in ConnectDataBaseCommit2 ; ";
		return false;
	}

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype= QUERY_CONNECT_DB;
	querybuf.datatype= S_SVSE;
	strcpy(querybuf.qstr,qstr.c_str());

	INIQUERY iquery={0};
	iquery.len=sizeof(INIQUERY);
	iquery.datatype=D_STRING;
	iquery.datalen=tlen;

	S_UINT len=sizeof(INIQUERY);
	len+=tlen;

	querybuf.datalen=len;

	buffer buf;
	if(!buf.checksize(len))
		return false;
	char *pt=buf.getbuffer();
	memcpy(pt,&iquery,sizeof(INIQUERY));
	pt+=sizeof(INIQUERY);
	memmove(pt,tdata,tlen);

	QueryData qd;
	char *pdata=NULL;
	S_UINT rlen=0;
	S_UINT okserver(SVDB_FAILED);
	bool bret(false);

	if(qd.Query(&querybuf,buf,len,(void **)& pdata, (S_UINT &)rlen, "localhost"))
	{
		try{
			if(rlen>=sizeof(S_UINT))
			{
				memmove(&okserver, pdata, sizeof(S_UINT));
				if( rlen==sizeof(S_UINT) )
					estr+= "  Fatal error happen on sever(ReactConnection) ;  ";

				if(okserver==SVDB_OK)
					bret= true;

				std::list<SingelRecord> listrcd2;
				if( CreateMassRecordListByRawData(listrcd2, pdata+sizeof(S_UINT), rlen-sizeof(S_UINT)) )
				{
					std::list<SingelRecord>::iterator lit= listrcd2.begin();
					if(lit!=listrcd2.end())
					{
						estr+= lit->monitorid;
						S_UINT templen= lit->datalen;
						if( templen!=0 )
						{
							char * tempchar= new char[templen];
							if(tempchar!=NULL)
							{
								memmove(tempchar, lit->data, templen);
								retp= tempchar;
								retlen= templen;
							}
						}
					}
				}
			}
		}catch(...)
		{
			estr+= " Exception in parsing data from ConnectDataBase; ";
		}
	}
	if(pdata!=NULL)
		delete [] pdata;
	return bret;
}


bool CConnect::ConnectDataBaseCommit(MapStrMap & fmap, std::list<SingelRecord> & listrcd, string qstr, char * & retp, int & retlen, string & estr)
{
	bool bret(true);
	if(fmap.empty())
	{
		estr+= "  传入的请求 MapStrMap 为空;  ";
		bret= false;
	}

	char * tbuf(NULL);
	char * tdata(NULL);
	S_UINT tlen(0);

	try{
		if(bret)
		{
			tlen= GetForestMapRawDataSize(fmap);
			tbuf= new char [tlen];
			if(tbuf==NULL)
			{
				estr+= " new an object,that==NULL;  ";
				bret= false;
			}
			else
			{
				SingelRecord r1;
				r1.monitorid= "queryconditions";
				r1.data= tbuf;
				r1.datalen= tlen;
				listrcd.push_front(r1);

				tdata= GetForestMapRawData(fmap,tbuf,tlen); 
				if(tdata==NULL)
				{
					estr+= "   Failed to get binary data of query conditions;  "; 
					bret= false;
				}
				else
				{
					bret= ConnectDataBaseCommit2(listrcd, qstr, retp, retlen, estr);
				}
			}
		}

		for(std::list<SingelRecord>::iterator llit=listrcd.begin(); llit!=listrcd.end(); ++llit)
			if( (llit->data) != NULL )
				delete [] (llit->data);
	}
	catch(...)
	{
		estr+= " Exception in ConnectDataBaseCommit; ";
		return false;
	}

	return bret;
}




bool CConnect::ConnectDataBase(MapStrMap & fmap, string qstr, char * & retp, int & retlen, string & estr)
{
	std::list<SingelRecord> listrcd;
	return ConnectDataBaseCommit(fmap, listrcd, qstr, retp, retlen, estr);
}




