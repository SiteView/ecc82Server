#include "Table.h"
#include "RecordSet.h"

Table::Table(void)
{
	m_firstpage=-1;
	m_currentpage=-1;
	m_hasindex=0;
	m_page=NULL;
	m_pagepool=NULL;
	m_laststatekeeptimes=0;
	m_state=0;
	m_laststatechangetime=svutil::TTime::GetCurrentTimeEx();
	m_time=svutil::TTime::GetCurrentTimeEx();
	m_displaystr="";
}

Table::~Table(void)
{
	int count=m_indextype.size();
	for(int i=0;i<count;i++)
	{
		DataType *pd=m_indextype[i];
		if(pd)
			delete pd;
	}

	if(m_page!=NULL)
		m_page->m_ppt->PutFree(m_page);


}



bool Table::CreateObjByPage(Page *pgage)
{
	if(pgage==NULL)
		return false;
	if(pgage->m_data==NULL)
		return false;
	PageHead *ppagehead = pgage->GetPageHead();
	if(ppagehead==NULL)
		return false;

	if(ppagehead->m_tablesize>ppagehead->m_datasize)
		return false;

	char *pt=pgage->m_data;
	memmove(&m_createtime,pt,sizeof(svutil::TTime));
	pt+=sizeof(svutil::TTime);

	memmove(&m_firstpage,pt,sizeof(int));
	pt+=sizeof(int);

	memmove(&m_currentpage,pt,sizeof(int));
	pt+=sizeof(int);

	memmove(&m_hasindex,pt,sizeof(int));
	pt+=sizeof(int);

	memmove(&(m_type.m_monitortype),pt,sizeof(int));
	pt+=sizeof(int);

	m_type.m_monitorid=pt;
	pt+=m_type.m_monitorid.size()+1;

	int count=0;

	memmove(&count,pt,sizeof(int));
	pt+=sizeof(int);
	if(pt-pgage->m_data>ppagehead->m_tablesize)
		return false;


    DataType *pdt=NULL;
	for(int i=0;i<count;i++)
	{
		pdt=new DataType();

		if(pdt==NULL)
			return false;
		memmove(pdt,pt,sizeof(DataType));
		m_type.m_data.push_back(pdt);

		pt+=sizeof(DataType);
		if(pt-pgage->m_data>ppagehead->m_tablesize)
			return false;

	}

	if(m_hasindex)
	{
		memmove(&count,pt,sizeof(int));
		pt+=sizeof(int);
		for(int i=0;i<count;i++)
		{
			pdt=new DataType();
			if(pdt==NULL)
				return false;
			memmove(pdt,pt,sizeof(DataType));

			m_indextype.push_back(pdt);
			pt+=sizeof(DataType);
			if(pt-pgage->m_data>ppagehead->m_tablesize)
				return false;
		}
	}

	//////////////////////////////dyn///////////////////////////

	memmove(&m_time,pt,sizeof(svutil::TTime));
	pt+=sizeof(svutil::TTime);

	memmove(&m_state,pt,sizeof(int));
	pt+=sizeof(int);

	memmove(&m_laststatechangetime,pt,sizeof(svutil::TTime));
	pt+=sizeof(svutil::TTime);

	memmove(&m_laststatekeeptimes,pt,sizeof(unsigned int));
	pt+=sizeof(unsigned int);
    
	if(ppagehead->m_tablesize<ppagehead->m_datasize)
		m_displaystr=pt;
	else
		m_displaystr="";

	////////////////////////////////////////////////////////////

	m_page=pgage;



	return true;
}

bool Table::FlushData()
{
	Page *pt=m_page->m_ppt->Get(m_page->m_pos);
	if(pt==NULL)
	{
		printf("pt is NULL pos is :%d\n",m_page->m_pos);
		return false;
	}
	if(pt->m_data==NULL)
	{
		m_page->m_ppt->Put(pt,false,true);
		puts("m_data is NULL");
		return false;
	}

	PageHead *ppagehead = pt->GetPageHead();
	if(ppagehead==NULL)
	{
		m_page->m_ppt->Put(pt,false,true);
		puts("ppagehead is NULL");
		return false;
	}

	int tlen=sizeof(int);
	int count=0;
	int tablesize=GetRawDataSize();

	if(ppagehead->m_datasize<tablesize)
	{
		ppagehead->m_badpage=1;
		F_SET(ppagehead->m_flag,BADPAGE);
		m_page->m_ppt->Put(pt,true,true);
		puts("data size is small");
		return false;
	}

	char *pdata=pt->m_data;

	memmove(pdata,&m_createtime,sizeof(svutil::TTime));
	pdata+=sizeof(svutil::TTime);

	memmove(pdata,&m_firstpage,tlen);
	pdata+=tlen;

	memmove(pdata,&m_currentpage,tlen);
	pdata+=tlen;

	memmove(pdata,&m_hasindex,tlen);
	pdata+=tlen;

	memmove(pdata,&m_type.m_monitortype,tlen);
	pdata+=tlen;

	strcpy(pdata,m_type.m_monitorid.c_str());
	pdata+=m_type.m_monitorid.size();
	pdata[0]='\0';
	pdata++;

	count=m_type.m_data.size();
	memmove(pdata,&count,tlen);
	pdata+=tlen;

	for(int i=0;i<count;i++)
	{
		memmove(pdata,m_type.m_data[i],sizeof(DataType));
		pdata+=sizeof(DataType);
	}

	if(m_hasindex)
	{
		count=m_indextype.size();
		memmove(pdata,&count,tlen);
		pdata+=tlen;
		for(int i=0;i<count;i++)
		{
			memmove(pdata,m_indextype[i],sizeof(DataType));
			pdata+=sizeof(DataType);
		}
	}

	/////////////////////dyn////////////////////////////////

	memmove(pdata,&m_time,sizeof(svutil::TTime));
	pdata+=sizeof(svutil::TTime);

	memmove(pdata,&m_state,tlen);
	pdata+=tlen;

	memmove(pdata,&m_laststatechangetime,sizeof(svutil::TTime));
	pdata+=sizeof(svutil::TTime);

	memmove(pdata,&m_laststatekeeptimes,sizeof(unsigned int));
	pdata+=sizeof(unsigned int);


	if(ppagehead->m_datasize-tablesize<m_displaystr.size()+1)
	{
		if(ppagehead->m_datasize-tablesize>0)
			pdata[0]='\0';
	}else
	{
		strcpy(pdata,m_displaystr.c_str());
		pdata+=m_displaystr.size();
		pdata[0]='\0';
		pdata++;
	}

	////////////////////////////////////////////////////////

	ppagehead->m_tablesize=tablesize;

	return m_page->m_ppt->Put(pt,true,false);
}
int Table::GetRawDataSize()
{
	int len=0,tlen=sizeof(int);

	len+=sizeof(svutil::TTime);
	len+=tlen;
	len+=tlen;
	len+=tlen;
	len+=tlen;

	len+=m_type.m_monitorid.size()+1;
    
	len+=tlen;
	int count=m_type.m_data.size();
	len+=sizeof(DataType)*count;

	if(m_hasindex)
	{
		len+=tlen;
		count=m_indextype.size();
		len+=sizeof(DataType)*count;
	}

	//////////////////////dyn//////////////////////////

	len+=sizeof(svutil::TTime);   //m_time
	len+=tlen;					//m_state
	len+=sizeof(svutil::TTime);   //m_laststatechangetime
	len+=sizeof(unsigned int);	//m_laststatekeeptimes

	/*
	  m_displaystr 由于空间的原因这个值暂不计算在内.由flushdata酌情处理.
    */
    
	/////////////////////////////////////////////////

	return len;

}
bool Table::InsertRecord(int state,char *data,int len)
{
	Page *ppage=m_pagepool->Get(this->m_currentpage);


	if(ppage==NULL)
	{
		puts("Get page failed");
		return false;
	}

	PageHead *ppagehead = ppage->GetPageHead();
	if(ppagehead==NULL)
		return false;

	if(F_ISSET(ppagehead->m_flag,BADPAGE))
		return false;

	RecordHead rht={0};
	rht.state=state;
	rht.createtime=svutil::TTime::GetCurrentTimeEx();
	rht.datalen=len;
	if(ppagehead->m_currentdatalen==0)
	{
		if(F_ISSET(ppagehead->m_flag,CROSSPAGEUP))
	    	rht.prercord=-1;
		else
			rht.prercord=-2;
	}
	else
    	rht.prercord=ppagehead->m_currentdatapos-ppagehead->m_currentdatalen;
	int dlen=sizeof(RecordHead)+len;

	char *pt=ppage->m_data+ppagehead->m_currentdatapos;
	int freesize=ppagehead->m_datasize-ppagehead->m_currentdatapos;

	ppagehead->m_lastrecordpos=ppagehead->m_currentdatapos;

	if(dlen<=freesize)
	{
		memcpy(pt,&rht,sizeof(RecordHead));
		pt+=sizeof(RecordHead);	
		memcpy(pt,data,len);

		ppagehead->m_currentdatalen=dlen;
		ppagehead->m_currentdatapos+=dlen;
		ppagehead->m_recordcount++;

		m_pagepool->Put(ppage,true,false);

		m_time=svutil::TTime::GetCurrentTimeEx();
		if(m_state!=state)
		{
			m_laststatechangetime=m_time;
			m_laststatekeeptimes=1;
		}else
			m_laststatekeeptimes++;

    	m_state=state;


		return true;


	}else if(freesize>=sizeof(RecordHead))
	{
		memcpy(pt,&rht,sizeof(RecordHead));
		pt+=sizeof(RecordHead);
		F_SET(ppagehead->m_flag,CROSSPAGEDOWN);

		freesize=freesize-sizeof(RecordHead);
		char *pdd=data;
		int mlen=len;
		if(freesize>0)
		{
			memcpy(pt,pdd,freesize);
			mlen=len-freesize;
			pt+=freesize;
			pdd+=freesize;
		}
		int pos=0;
		Page *ptem=NULL;
		while(true)
		{
			Page *pnext=m_pagepool->GetFree();
		
			if(pnext==NULL)
			{
				puts("ddddd");
				if(!m_pagepool->AddNewPagesEx(1000))
				{
					puts("Add new page failed-1");
					F_SET(ppagehead->m_flag,PAGEFULL);
					F_SET(ppagehead->m_flag,BADPAGE);
					m_pagepool->Put(ppage,true,true);
					return false;
				}
				pnext=m_pagepool->GetFree();
				if(pnext==NULL)
				{
					puts("Get new page failed");
					F_SET(ppagehead->m_flag,PAGEFULL);
					F_SET(ppagehead->m_flag,BADPAGE);
					m_pagepool->Put(ppage,true,true);
					return false;

				}
			}
			
			PageHead *pnexthead = pnext->GetPageHead();
			if(pnexthead==NULL)
			{
				puts("Get new page failed");
				F_SET(ppagehead->m_flag,PAGEFULL);
				F_SET(ppagehead->m_flag,BADPAGE);
				m_pagepool->Put(ppage,true,true);
				m_pagepool->Put(pnext,false,true);
				return false;
			}

			if(pos==0)
			{
				ppagehead->m_nextrecordpage=pnext->m_pos;
				pnexthead->m_prerecordpage=ppage->m_pos;
//				ptem=pnext;
			}else
			{
				pnexthead->m_prerecordpage=ptem->m_pos;
				ptem->GetPageHead()->m_nextrecordpage=pnext->m_pos;
//				ppage->m_Head.m_recordcount++;
				m_pagepool->Put(ptem,true,true);
			}

			ptem=pnext;
			pos++;

			F_SET(pnexthead->m_flag,CROSSPAGEUP);

			if(mlen<=pnexthead->m_datasize)
			{
				memmove(pnext->m_data,pdd,mlen);
				pdd+=mlen;

			   pnexthead->m_currentdatalen=0;
		       pnexthead->m_currentdatapos=mlen;

			   ppagehead->m_recordcount++;
			   m_pagepool->Put(ppage,true,true);
			   m_pagepool->Put(pnext,true,false);
			   m_currentpage=pnext->m_pos;

			//   FlushData();

			   m_time=svutil::TTime::GetCurrentTimeEx();
			   m_state=state;
			   if(m_state!=state)
			   {
				   m_laststatechangetime=m_time;
				   m_laststatekeeptimes=1;
			   }else
				   m_laststatekeeptimes++;

			   return true;
			}else{
				F_SET(pnexthead->m_flag,CROSSPAGEDOWN);
				F_SET(pnexthead->m_flag,PAGEFULL);
				memmove(pnext->m_data,pdd,pnexthead->m_datasize);
				pdd+=pnexthead->m_datasize;

			   pnexthead->m_currentdatalen=0;
		       pnexthead->m_currentdatapos=pnexthead->m_datasize;

			   mlen=mlen-pnexthead->m_datasize;
			}


		}

	}else
	{
		F_SET(ppagehead->m_flag,PAGEFULL);
		Page *pnext=m_pagepool->GetFree();
		
		if(pnext==NULL)
		{
			if(!m_pagepool->AddNewPagesEx(1000))
			{
				puts("Add new page failed");
				m_pagepool->Put(ppage,true,true);
				return false;
			}
			pnext=m_pagepool->GetFree();
			if(pnext==NULL)
			{
				puts("Get new page failed");
				m_pagepool->Put(ppage,true,true);
				return false;

			}
		}
		PageHead *pnexthead = pnext->GetPageHead();
		if(pnexthead==NULL)
		{
			puts("Get new page failed");
			m_pagepool->Put(ppage,true,true);
			m_pagepool->Put(pnext,true,true);
			return false;

		}


		ppagehead->m_lastrecordpos=ppagehead->m_currentdatapos-ppagehead->m_currentdatalen;

		ppagehead->m_nextrecordpage=pnexthead->m_pos;
		pnexthead->m_prerecordpage=ppagehead->m_pos;
		m_currentpage=pnext->m_pos;

		m_pagepool->Put(ppage,true,true);
		m_pagepool->Put(pnext,true,false);
		//FlushData();
		return InsertRecord(state,data,len);

	}

	return true;

}
int Table::AppendRecord(const char *rawdata,S_UINT datalen)
{
	if((rawdata==NULL)||(datalen<sizeof(RecordHead)))
		return -2;


	Page *ppage=m_pagepool->Get(this->m_currentpage);


	if(ppage==NULL)
	{
		printf("Append table record Get page failed ,page id:%d\n",m_currentpage);
		puts("Get page failed");
		return false;
	}

	PageHead *ppagehead = ppage->GetPageHead();
	if(ppagehead==NULL)
	{
		m_pagepool->Put(ppage,false,false);
		return false;
	}

	if(F_ISSET(ppagehead->m_flag,BADPAGE))
	{
		m_pagepool->Put(ppage,false,false);
		return false;
	}

	RecordHead *prht=(RecordHead *)rawdata;
	const char *data=rawdata+sizeof(RecordHead);

	int len=prht->datalen;

	if(ppagehead->m_currentdatalen==0)
	{
		if(F_ISSET(ppagehead->m_flag,CROSSPAGEUP))
	    	prht->prercord=-1;
		else
			prht->prercord=-2;
	}
	else
    	prht->prercord=ppagehead->m_currentdatapos-ppagehead->m_currentdatalen;

/*	RecordHead rht={0};
	rht.state=state;
	rht.createtime=svutil::TTime::GetCurrentTimeEx();
	rht.datalen=len;
	if(ppagehead->m_currentdatalen==0)
	{
		if(F_ISSET(ppagehead->m_flag,CROSSPAGEUP))
	    	rht.prercord=-1;
		else
			rht.prercord=-2;
	}
	else
    	rht.prercord=ppagehead->m_currentdatapos-ppagehead->m_currentdatalen;
		*/
	try{
		const char * tbuf2(NULL);
		S_UINT tbuflen2(0);
		RecordHead *prht=(RecordHead *)(rawdata);
		if((prht->state==Record::STATUS_BAD)||(prht->state==Record::STATUS_NULL)||(prht->state==Record::STATUS_DISABLE))
		{
			tbuf2= NULL;
			tbuflen2= 0;
		}
		else
		{
			tbuf2= rawdata+sizeof(RecordHead);
			tbuflen2= len;
		}
		SetLatestRCD(tbuf2, tbuflen2);
	}
	catch(...)
	{
		printf("Exception to SetLatestRCD in AppendRecord\n");
		SetLatestRCD(NULL, 0);
	}


	int dlen=sizeof(RecordHead)+len;
	m_displaystr=rawdata+dlen;

	char *pt=ppage->m_data+ppagehead->m_currentdatapos;
	int freesize=ppagehead->m_datasize-ppagehead->m_currentdatapos;

	ppagehead->m_lastrecordpos=ppagehead->m_currentdatapos;

	if(dlen<=freesize)
	{
		memcpy(pt,prht,sizeof(RecordHead));
		pt+=sizeof(RecordHead);	
		memcpy(pt,data,len);

		ppagehead->m_currentdatalen=dlen;
		ppagehead->m_currentdatapos+=dlen;
		ppagehead->m_recordcount++;

		m_pagepool->Put(ppage,true,false);

		m_time=svutil::TTime::GetCurrentTimeEx();
		if(m_state!=prht->state)
		{
			m_laststatechangetime=m_time;
			m_laststatekeeptimes=1;
		}else
			m_laststatekeeptimes++;

		pt+=len;
		m_state=prht->state;

	//	FlushData();

		return true;


	}else if(freesize>=sizeof(RecordHead))
	{
		memcpy(pt,prht,sizeof(RecordHead));
		pt+=sizeof(RecordHead);
		F_SET(ppagehead->m_flag,CROSSPAGEDOWN);

		freesize=freesize-sizeof(RecordHead);
		const char *pdd=data;
		int mlen=len;
		if(freesize>0)
		{
			memcpy(pt,pdd,freesize);
			mlen=len-freesize;
			pt+=freesize;
			pdd+=freesize;
		}
		int pos=0;
		Page *ptem=NULL;
		while(true)
		{
			Page *pnext=m_pagepool->GetFreeEx();
		
			if(pnext==NULL)
			{
/*				puts("ddddd");
				if(!m_pagepool->AddNewPagesEx(1000))
				{
					puts("Add new page failed-1");
					F_SET(ppagehead->m_flag,PAGEFULL);
					F_SET(ppagehead->m_flag,BADPAGE);
					m_pagepool->Put(ppage,true,true);
					if(ptem!=NULL)
						m_pagepool->Put(ptem,true,true);
					return false;
				}
				pnext=m_pagepool->GetFree();*/
				if(pnext==NULL)
				{
					puts("Get new page failed");
					F_SET(ppagehead->m_flag,PAGEFULL);
					F_SET(ppagehead->m_flag,BADPAGE);
					m_pagepool->Put(ppage,true,true);
					if(ptem!=NULL)
						m_pagepool->Put(ptem,true,true);
					return false;

				}
			}
			
			PageHead *pnexthead = pnext->GetPageHead();
			if(pnexthead==NULL)
			{
				puts("Get new page failed");
				F_SET(ppagehead->m_flag,PAGEFULL);
				F_SET(ppagehead->m_flag,BADPAGE);
				m_pagepool->Put(ppage,true,true);
				m_pagepool->Put(pnext,false,true);
				if(ptem!=NULL)
					m_pagepool->Put(ptem,true,true);
				return false;
			}

			if(pos==0)
			{
				ppagehead->m_nextrecordpage=pnext->m_pos;
				pnexthead->m_prerecordpage=ppage->m_pos;
//				ptem=pnext;
			}else
			{
				pnexthead->m_prerecordpage=ptem->m_pos;
				ptem->GetPageHead()->m_nextrecordpage=pnext->m_pos;
//				ppage->m_Head.m_recordcount++;
				m_pagepool->Put(ptem,true,true);
			}

			ptem=pnext;
			pos++;

			F_SET(pnexthead->m_flag,CROSSPAGEUP);

			if(mlen<=pnexthead->m_datasize)
			{
				memmove(pnext->m_data,pdd,mlen);
				pdd+=mlen;

			   pnexthead->m_currentdatalen=0;
		       pnexthead->m_currentdatapos=mlen;

			   ppagehead->m_recordcount++;
			   m_pagepool->Put(ppage,true,true);
			   m_pagepool->Put(pnext,true,false);
			   m_currentpage=pnext->m_pos;


			   m_time=svutil::TTime::GetCurrentTimeEx();
			   if(m_state!=prht->state)
			   {
				   m_laststatechangetime=m_time;
				   m_laststatekeeptimes=1;
			   }else
				   m_laststatekeeptimes++;

			   m_state=prht->state;

			   FlushData();

			   return true;
			}else{
				F_SET(pnexthead->m_flag,CROSSPAGEDOWN);
				F_SET(pnexthead->m_flag,PAGEFULL);
				memmove(pnext->m_data,pdd,pnexthead->m_datasize);
				pdd+=pnexthead->m_datasize;

			   pnexthead->m_currentdatalen=0;
		       pnexthead->m_currentdatapos=pnexthead->m_datasize;

			   mlen=mlen-pnexthead->m_datasize;
			}


		}

	}else
	{
		F_SET(ppagehead->m_flag,PAGEFULL);
		Page *pnext=m_pagepool->GetFreeEx();
		
		if(pnext==NULL)
		{
	/*		if(!m_pagepool->AddNewPagesEx(1000))
			{
				puts("Add new page failed");
				m_pagepool->Put(ppage,true,true);
				return false;
			}
			pnext=m_pagepool->GetFree();*/
			if(pnext==NULL)
			{
				puts("Get new page failed");
				m_pagepool->Put(ppage,true,true);
				return false;

			}
		}
		PageHead *pnexthead = pnext->GetPageHead();
		if(pnexthead==NULL)
		{
			puts("Get new page failed");
			m_pagepool->Put(ppage,true,true);
			m_pagepool->Put(pnext,true,true);
			return false;

		}


		ppagehead->m_lastrecordpos=ppagehead->m_currentdatapos-ppagehead->m_currentdatalen;

		ppagehead->m_nextrecordpage=pnexthead->m_pos;
		pnexthead->m_prerecordpage=ppagehead->m_pos;
		m_currentpage=pnext->m_pos;

		m_pagepool->Put(ppage,true,true);
		m_pagepool->Put(pnext,true,false);
		FlushData();
		return AppendRecord(rawdata,datalen);

	}

	return 1;


}
int Table::QueryRecordByCount(S_UINT count,svutil::buffer &buf,int headlen,int &datalen)
{
	if(count<=0)
		return -3;
	Page *pt=this->m_pagepool->Get(this->m_currentpage);
	if(pt==NULL)
		return -2;

	if(pt->m_data==NULL)
	{
		m_pagepool->Put(pt,false,true);
		return -3;
	}
	PageHead *ppagehead=pt->GetPageHead();
	if(ppagehead == NULL)
	{
		m_pagepool->Put(pt,false,true);
		return -3;
	}
	if((m_currentpage==m_firstpage)&&(ppagehead->m_lastrecordpos<0))
	{
		m_pagepool->Put(pt,false,false);
		datalen=0;
		return 0;

	}


	if(ppagehead->m_lastrecordpos<0)
	{
		Page *ptemp=GetPreRecordPage(pt);
		if(ptemp==NULL)
		{
	    	m_pagepool->Put(pt,false,false);
			return -2;
		}
		m_pagepool->Put(pt,false,false);
		pt=ptemp;
		ppagehead=pt->GetPageHead();
		if(ppagehead==NULL)
		{
			m_pagepool->Put(pt,false,false);
			return -2;
		}
	}

	int i=0;

	char *ptbuf=buf;
	ptbuf+=headlen;
	int buflen=buf.size()-headlen;

	int pos=ppagehead->m_lastrecordpos;
	char *pc=pt->m_data+pos;
	int rlen=0,blen=buflen;
	int prep=0;
	while(true)
	{
		svutil::TTime rtime=((RecordHead*)pc)->createtime;

		//if(rtime<begin)
		//{
		//	m_pagepool->Put(pt,false,true);
		//	break;
		//}
		prep=((RecordHead*)pc)->prercord;

	//	string stime=rtime.Format();

		if(i<count)
		{

			//stime=rtime.Format();
		//	printf("stime:%s\n",stime.c_str());

			while(true)
			{
				rlen=GetSingleRecord(pt,pos,ptbuf,blen);
				if(rlen<0)
				{
					if(rlen==BUFFERTOOSMALL)
					{
						S_UINT rsize=buf.size()+QBUFFERLEN;
						int tpos=ptbuf-buf.getbuffer();
						if(!buf.resetsize(rsize))
						{
							m_pagepool->Put(pt,false,true);
							return -7;
						}

						blen+=QBUFFERLEN;
						ptbuf=buf.getbuffer();
						ptbuf+=tpos;
						continue;

					}

					m_pagepool->Put(pt,false,true);
					return rlen;
				}else
					break;
			}
			ptbuf+=rlen;
			blen-=rlen;
			i++;
		}

		if(prep>=0)
		{
			pc=pt->m_data+prep;
			pos=prep;
			continue;
		}else
		{
			Page *ptem=GetPreRecordPage(pt);
			m_pagepool->Put(pt,false,true);
			if(ptem==NULL)
				break;
			pt=ptem;
			ppagehead=pt->GetPageHead();
			if(ppagehead==NULL)
			{
				m_pagepool->Put(pt,false,true);
				break;
			}
			pos=ppagehead->m_lastrecordpos;
			pc=pt->m_data+pos;
		}

	}

	datalen=ptbuf-buf.getbuffer()-headlen;

	return i;

}


bool Table::QueryRecordCount(string monitorid, int & count)
{	
	cout<<"Query record count of \""<<monitorid.c_str()<<"\":  ";
	Page *pt=this->m_pagepool->Get(this->m_currentpage);
	if(pt==NULL)
		return false;

	if(pt->m_data==NULL)
	{
		m_pagepool->Put(pt,false,true);
		return false;
	}

	PageHead *ppagehead=pt->GetPageHead();
	if(ppagehead == NULL)
	{
		m_pagepool->Put(pt,false,true);
		return false;
	}
	
	if((m_currentpage==m_firstpage)&&(ppagehead->m_lastrecordpos<0))
	{
		m_pagepool->Put(pt,false,false);
		count=0;
		cout<<"0"<<endl;
		return true;
	}

	int pos=ppagehead->m_lastrecordpos;
	char *pc=pt->m_data+pos;

	count= ppagehead->m_recordcount;
	if(count==0)
	{
		cout<<"0"<<endl;
		return true;
	}

	while(true)
	{
		int rlen=0,prep=0;
		
		prep=((RecordHead*)pc)->prercord;
		if(prep>=0)
		{
			pc=pt->m_data+prep;
			pos=prep;
			continue;
		}
		else
		{
			Page *ptem=GetPreRecordPage(pt);
			m_pagepool->Put(pt,false,true);
			if(ptem==NULL)
				break;
			pt=ptem;
			ppagehead=pt->GetPageHead();
			if(ppagehead==NULL)
				break;
			count+= ppagehead->m_recordcount;

			pos=ppagehead->m_lastrecordpos;
			pc=pt->m_data+pos;
		}

	}

	cout<<count<<endl;
	return true;
}

int Table::QueryRecordByTime(svutil::TTime begin,svutil::TTime end,svutil::buffer &buf,int headlen,int &datalen)
{
	if(begin>end)
		return -3;

	Page *pt=this->m_pagepool->Get(this->m_currentpage);
	if(pt==NULL)
		return -2;

	if(pt->m_data==NULL)
	{
		m_pagepool->Put(pt,false,true);
		return -3;
	}
	PageHead *ppagehead=pt->GetPageHead();
	if(ppagehead == NULL)
	{
		m_pagepool->Put(pt,false,true);
		return -3;
	}
	if((m_currentpage==m_firstpage)&&(ppagehead->m_lastrecordpos<0))
	{
		m_pagepool->Put(pt,false,false);
		datalen=0;
		return 0;

	}


	if(ppagehead->m_lastrecordpos<0)
	{
		Page *ptemp=GetPreRecordPage(pt);
		if(ptemp==NULL)
		{
	    	m_pagepool->Put(pt,false,false);
			return -2;
		}
		m_pagepool->Put(pt,false,false);
		pt=ptemp;
		ppagehead=pt->GetPageHead();
		if(ppagehead==NULL)
		{
			m_pagepool->Put(pt,false,false);
			return -2;
		}
	}

	int i=0;

	char *ptbuf=buf;
	ptbuf+=headlen;
	int buflen=buf.size()-headlen;

	int pos=ppagehead->m_lastrecordpos;
	char *pc=pt->m_data+pos;
	int rlen=0,blen=buflen;
	int prep=0;
	while(true)
	{
		svutil::TTime rtime=((RecordHead*)pc)->createtime;

		if(rtime<begin)
		{
			m_pagepool->Put(pt,false,true);
			break;
		}
		prep=((RecordHead*)pc)->prercord;

//		string stime=rtime.Format();

		if((rtime>=begin)&&(rtime<=end))
		{

			//stime=rtime.Format();
		//	printf("stime:%s\n",stime.c_str());

			while(true)
			{
				rlen=GetSingleRecord(pt,pos,ptbuf,blen);
				if(rlen<0)
				{
					if(rlen==BUFFERTOOSMALL)
					{
						S_UINT rsize=buf.size()+QBUFFERLEN;
						int tpos=ptbuf-buf.getbuffer();
						if(!buf.resetsize(rsize))
						{
							m_pagepool->Put(pt,false,true);
							return -7;
						}

						blen+=QBUFFERLEN;
						ptbuf=buf.getbuffer();
						ptbuf+=tpos;
						continue;

					}

					m_pagepool->Put(pt,false,true);
					return rlen;
				}else
					break;
			}
			ptbuf+=rlen;
			blen-=rlen;
			i++;
		}

		if(prep>=0)
		{
			pc=pt->m_data+prep;
			pos=prep;
			continue;
		}else
		{
			Page *ptem=GetPreRecordPage(pt);
			m_pagepool->Put(pt,false,true);
			if(ptem==NULL)
				break;
			pt=ptem;
			ppagehead=pt->GetPageHead();
			if(ppagehead==NULL)
			{
				m_pagepool->Put(pt,false,true);
				break;
			}
			pos=ppagehead->m_lastrecordpos;
			pc=pt->m_data+pos;
		}

	}

	datalen=ptbuf-buf.getbuffer()-headlen;

	return i;

}
int Table::QueryRecordByTime(svutil::TTime begin,svutil::TTime end,char *buf,int &buflen)
{
	if(begin>end)
		return -3;

	Page *pt=this->m_pagepool->Get(this->m_currentpage);
	if(pt==NULL)
		return -2;

	if(pt->m_data==NULL)
	{
		m_pagepool->Put(pt,false,true);
		return -3;
	}
	PageHead *ppagehead=pt->GetPageHead();
	if(ppagehead == NULL)
	{
		m_pagepool->Put(pt,false,true);
		return -3;
	}
	if((m_currentpage==m_firstpage)&&(ppagehead->m_lastrecordpos<0))
	{
		m_pagepool->Put(pt,false,false);
		buflen=0;
		return 0;

	}

	if(ppagehead->m_lastrecordpos<0)
	{
		Page *ptemp=GetPreRecordPage(pt);
		if(ptemp==NULL)
		{
	    	m_pagepool->Put(pt,false,false);
			return -2;
		}
		m_pagepool->Put(pt,false,false);
		pt=ptemp;
		ppagehead=pt->GetPageHead();
		if(ppagehead==NULL)
		{
			m_pagepool->Put(pt,false,false);
			return -2;
		}
	}

	int i=0;

	char *ptbuf=buf;
	int pos=ppagehead->m_lastrecordpos;
	char *pc=pt->m_data+pos;
	int rlen=0,blen=buflen;
	int prep=0;
	while(true)
	{
		svutil::TTime rtime=((RecordHead*)pc)->createtime;
		if(rtime<begin)
		{
			m_pagepool->Put(pt,false,true);
			break;
		}
		prep=((RecordHead*)pc)->prercord;

		if((rtime>=begin)&&(rtime<=end))
		{
			rlen=GetSingleRecord(pt,pos,ptbuf,blen);
			if(rlen<0)
			{
				m_pagepool->Put(pt,false,true);
				return rlen;
			}
			ptbuf+=rlen;
			blen-=rlen;
			i++;
		}

		if(prep>=0)
		{
			pc=pt->m_data+prep;
			pos=prep;
			continue;
		}else
		{
			Page *ptem=GetPreRecordPage(pt);
			m_pagepool->Put(pt,false,true);
			if(ptem==NULL)
				break;
			pt=ptem;
			ppagehead=pt->GetPageHead();
			if(ppagehead==NULL)
			{
				m_pagepool->Put(pt,false,true);
				break;
			}
			pos=ppagehead->m_lastrecordpos;
			pc=pt->m_data+pos;
		}

	}

	buflen=ptbuf-buf;

	return i;
}
Page *Table::GetPreRecordPage(Page *ppage)
{
	if(ppage==NULL)
		return NULL;

	Page *pt=ppage;
	int i=0;

	PageHead *ppagehead=pt->GetPageHead();
	if(ppagehead==NULL)
		return NULL;

	
	do{
		int pos=ppagehead->m_prerecordpage;
		if(pos<0)
			return NULL;

		if(i>0)
	    	m_pagepool->Put(pt,false,false);

		i++;
		pt=m_pagepool->Get(pos);
		
		if(pt==NULL)
			return NULL;
		ppagehead=pt->GetPageHead();
		if(ppagehead==NULL)
		{
			m_pagepool->Put(pt,false,true);
			return NULL;
		}
		if(pt->m_data==NULL)
		{
			m_pagepool->Put(pt,false,true);
			return NULL;
		}
	}while(ppagehead->m_lastrecordpos<0);

	return pt;

}
Page *Table::GetNextPage(Page *ppage)
{
	if(ppage==NULL)
		return NULL;
	PageHead *ppagehead=ppage->GetPageHead();
	if(ppagehead==NULL)
		return NULL;

	if(ppagehead->m_nextrecordpage<0)
		return NULL;

	return m_pagepool->Get(ppagehead->m_nextrecordpage);
}

int Table::GetSingleRecord(Page *ppage,int pos,char *buf,int buflen)
{
	try{

		if(ppage==NULL)
			return -2;
		if(ppage->m_data==NULL)
			return -2;
		PageHead *ppagehead=ppage->GetPageHead();
		if(ppagehead==NULL)
			return -2;

		if(pos<0||pos>ppagehead->m_datasize)
			return -3;
		char *pt=ppage->m_data+pos;

		int len=((RecordHead*)pt)->datalen;
		len+=sizeof(RecordHead);

		if(len>buflen)
			return BUFFERTOOSMALL;

		if(len<=ppagehead->m_datasize-pos)
		{
			memmove(buf,pt,len);
			return len;
		}else if(F_ISSET(ppagehead->m_flag,CROSSPAGEDOWN))
		{
			int csize=ppagehead->m_datasize-pos;
			char *ptbuf=buf;
			memmove(ptbuf,pt,csize);
			ptbuf+=csize;
			int mlen=len-csize;
			Page *ptpt=ppage;
			while(true)
			{
				Page *ptt=GetNextPage(ptpt);
				if(ptpt!=ppage)
				{
					m_pagepool->Put(ptpt,false,true);
					ptpt=NULL;

				}
				if(ptt==NULL)
				{
					return -5;
				}
				PageHead *ptthead=ptt->GetPageHead();
				if(ptthead==NULL)
				{
					m_pagepool->Put(ptt,false,false);
					return -6;
				}
				if(F_ISSET(ptthead->m_flag,CROSSPAGEUP))
				{
					if(mlen<=ptthead->m_datasize)
					{
						memmove(ptbuf,ptt->m_data,mlen);
						m_pagepool->Put(ptt,false,true);
						return len;
					}else
					{
						memmove(ptbuf,ptt->m_data,ptthead->m_datasize);
						ptbuf+=ptthead->m_datasize;
						ptpt=ptt;
						mlen=mlen-ptthead->m_datasize;
					}

				}

			}
		}else
			return -6;
	}
	catch(...)
	{
		puts("GetSingleRecord failed.");
		return -2;
	}

}

int Table::QueryDyn(char *buf,S_UINT &buflen)
{
	S_UINT dlen=GetDynSize();
	if(buf==NULL)
	{
		buflen=dlen;
		return 0;
	}
	if(buflen<dlen)
		return BUFFERTOOSMALL;

	char *pt=buf;
	memmove(pt,&m_time,sizeof(svutil::TTime));
	pt+=sizeof(svutil::TTime);
	memmove(pt,&m_state,sizeof(int));
	pt+=sizeof(int);
	svutil::TTimeSpan ts=svutil::TTime::GetCurrentTimeEx()-m_laststatechangetime;
	memmove(pt,&ts,sizeof(svutil::TTimeSpan));
	pt+=sizeof(svutil::TTimeSpan);
	memmove(pt,&m_laststatekeeptimes,sizeof(unsigned int));
	pt+=sizeof(unsigned int);
	strcpy(pt,m_displaystr.c_str());
	pt+=m_displaystr.size();
	pt[0]='\0';
	pt++;

	buflen=dlen;
	return 0;
}
bool Table::GetAlertInfoData(char *buf,S_UINT &buflen)
{
	if(m_state==Record::STATUS_OK)
	{
		if(m_laststatekeeptimes>1)
			return false;
	}

	return (QueryDyn(buf,buflen)==0);

}
int Table::QueryDynNOStr(char *buf,S_UINT &buflen)
{
	S_UINT dlen=GetDynSize();
	dlen-=(m_displaystr.size()+1);
	if(buf==NULL)
	{
		buflen=dlen;
		return 0;
	}
	if(buflen<dlen)
		return BUFFERTOOSMALL;

	char *pt=buf;
	memmove(pt,&m_time,sizeof(svutil::TTime));
	pt+=sizeof(svutil::TTime);
	memmove(pt,&m_state,sizeof(int));
	pt+=sizeof(int);
	svutil::TTimeSpan ts=svutil::TTime::GetCurrentTimeEx()-m_laststatechangetime;
	memmove(pt,&ts,sizeof(svutil::TTimeSpan));
	pt+=sizeof(svutil::TTimeSpan);
	memmove(pt,&m_laststatekeeptimes,sizeof(unsigned int));
	pt+=sizeof(unsigned int);

	buflen=dlen;

	return 0;

}

S_UINT Table::GetDynSize(void)
{
	S_UINT len=0,tlen=sizeof(int);


	len+=sizeof(svutil::TTime);  //createtime;
	len+=tlen;    //state;
	len+=sizeof(svutil::TTimeSpan);  //keep last state time
	len+=sizeof(unsigned int);
	len+=m_displaystr.size()+1;

	return len;
}

bool Table::DeleteData()
{
	Page *pt=m_pagepool->Get(m_firstpage);
	if(pt==NULL)
		return false;

	bool bret=FreePage(pt);
	m_firstpage=-1;
	m_currentpage=-1;

	m_displaystr.empty();

	this->FlushData();

	return bret;
}
bool Table::DeleteData(svutil::TTime begin,svutil::TTime end)
{
	return true;
}
bool Table::DeleteData(svutil::TTime before)
{
	Page *pt=m_pagepool->Get(m_firstpage);
	if(pt==NULL)
		return false;

	return FreePage(pt,before);
}

bool Table::FreePage(Page *ppage)
{
	if(ppage==NULL)
		return false;

	Page *pg=this->GetNextPage(ppage);
	m_pagepool->PutFree(ppage);
	
	if(pg!=NULL)
		FreePage(pg);

	return true;
}

bool Table::FreePage(Page *ppage,svutil::TTime begin,svutil::TTime end)
{
	return true;

}
bool Table::FreePage(Page *ppage,svutil::TTime before)
{
	PageHead *pph=ppage->GetPageHead();
	Page *pt=GetNextPage(ppage);
	int pos=pph->m_lastrecordpos;
	if(pph==NULL)
	{
		if(pt!=NULL)
			m_pagepool->Put(pt,true,true);
		if(ppage!=NULL)
			m_pagepool->Put(ppage,true,true);
		return false;
	}

	
	if(pph->m_lastrecordpos<0)
	{
		if(pt!=NULL)
			m_pagepool->Put(pt,true,true);
		if(ppage!=NULL)
			m_pagepool->Put(ppage,true,true);
		return false;
	}

	char *pc=ppage->m_data+pos;

	if(((RecordHead*)pc)->createtime<before)
	{
		if(ppage->m_pos==this->m_currentpage)
		{
			m_firstpage=ppage->m_pos;
			F_CLR(pph->m_flag,CROSSPAGEUP);
			pph->m_prerecordpage=-2;
			m_pagepool->Put(ppage,true,true);
			if(pt!=NULL)
				m_pagepool->Put(pt,false,true);
	    	this->FlushData();
			return true;
		}
		m_pagepool->PutFree(ppage);
	}
	else
	{
		m_firstpage=ppage->m_pos;
		F_CLR(pph->m_flag,CROSSPAGEUP);
		pph->m_prerecordpage=-2;
		m_pagepool->Put(ppage,true,true);
		if(pt!=NULL)
			m_pagepool->Put(pt,true,true);
		this->FlushData();
		return true;
	}

	if(pt!=NULL)
		FreePage(pt,before);
	else
	{
		Page *pfree=GetFreePage();
		if(pfree==NULL)
			return false;
		pfree->GetPageHead()->m_prerecordpage=-2;
		m_firstpage=pfree->m_pos;
		this->m_currentpage=m_firstpage;
		m_pagepool->Put(pfree,false,true);
		this->FlushData();
	}

    return true;
}

Page *Table::GetFreePage()
{
	Page *pnext=m_pagepool->GetFreeEx();

	if(pnext==NULL)
	{
		puts("ddddd");
	/*	if(!m_pagepool->AddNewPagesEx(1000))
		{
			puts("Add new page failed-1");
			return NULL;
		}
		pnext=m_pagepool->GetFree();*/
	}

	return pnext;

}

bool Table::BuildRecordType(char *buf,S_UINT &buflen)
{
	if(buf==NULL)
		return false;
	if(buflen<m_type.GetRawDataSize()+sizeof(S_UINT))
		return false;

	char *pt=buf;

	S_UINT len=m_type.GetRawDataSize();
	memmove(pt,&len,sizeof(S_UINT));
	pt+=sizeof(S_UINT);
	buflen=len+sizeof(S_UINT);
	return m_type.GetRawData(pt,buflen-sizeof(S_UINT));
	
}

bool Table::SetLatestRCD(const char *buf,int buflen)
{
	if(m_latestRCD.data!=NULL)
		delete [] m_latestRCD.data;

	m_latestRCD.data= NULL;
	m_latestRCD.datalen= 0;
	m_latestRCD.monitorid="done";

	if(buflen<=0 || buf==NULL)
		return true;

	char * tbuf= new char[buflen];
	if(tbuf==NULL)
		return false;
	memmove(tbuf,buf,buflen);

	m_latestRCD.data= tbuf;
	m_latestRCD.datalen= buflen;
	return true;
}


bool Table::GetLatestRCD(char * & buf,S_UINT &buflen)
{
	buf= (char *) m_latestRCD.data;
	buflen= m_latestRCD.datalen;
	if(m_latestRCD.monitorid.compare("done")==0)
		return true;
	else
		return false;
}



