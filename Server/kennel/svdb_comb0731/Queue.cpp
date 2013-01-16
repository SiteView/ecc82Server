#include "Queue.h"
#include "QueueRecord.h"

Queue::Queue(void)
{
	m_recordcount=0;

}

Queue::~Queue(void)
{
}
/*
由于时间原因暂不处理优先队列
*/
bool Queue::Append(const char * data,S_UINT len,int level)
{
//	MutexLock lock(m_mutex);

	Page *ppage=m_pagepool->Get(this->m_currentpage);


	if(ppage==NULL)
	{
		printf("Append queue record get page failed queue name :%s page id:%d",m_queuename.c_str(),m_currentpage);
		puts("Get page failed");
		return false;
	}

	PageHead *ppagehead = ppage->GetPageHead();
	if(ppagehead==NULL)
		return false;

	if(F_ISSET(ppagehead->m_flag,BADPAGE))
		return false;

	QUEUEHEAD rht;
	rht.m_cblen=sizeof(QUEUEHEAD);
	rht.m_createtime=svutil::TTime::GetCurrentTimeEx();
	rht.m_level=level;
	rht.m_datalen=len;


	if(ppagehead->m_currentdatalen==0)
	{
/*		if(F_ISSET(ppagehead->m_flag,CROSSPAGEUP))
		{
			rht.m_prerecord.m_page=m_currentpage;
			rht.m_prerecord.m_pos=-1;
		}
		else
		{
			rht.m_prerecord.m_page=m_currentpage;
			rht.m_prerecord.m_pos=-2;
//			m_top=rht.m_prerecord;
		}
		*/
		rht.m_prerecord=m_last;
	}
	else
	{
/*		rht.m_prerecord.m_page=m_currentpage;
    	rht.m_prerecord.m_pos=ppagehead->m_currentdatapos-ppagehead->m_currentdatalen;
*/
		rht.m_prerecord=m_last;

	}

	int dlen=sizeof(QUEUEHEAD)+len;


	char *pt=ppage->m_data+ppagehead->m_currentdatapos;
	int freesize=ppagehead->m_datasize-ppagehead->m_currentdatapos;

    int oldlastrecordpos=ppagehead->m_lastrecordpos;
	ppagehead->m_lastrecordpos=ppagehead->m_currentdatapos;

	if(dlen<=freesize)
	{
		//if(freesize-dlen>=sizeof(QUEUEHEAD))
		//{
		//	rht.m_nextrecord.m_page=m_currentpage;
		//	rht.m_nextrecord.m_pos=rht.m_prerecord.m_pos+dlen;
		//}

		memcpy(pt,&rht,sizeof(QUEUEHEAD));
		pt+=sizeof(QUEUEHEAD);	
		memcpy(pt,data,len);



		if(m_last.m_page==m_currentpage)
		{
			LPQUEUEHEAD pprh=(LPQUEUEHEAD)(ppage->m_data+m_last.m_pos);
			pprh->m_nextrecord.m_page=m_currentpage;
			pprh->m_nextrecord.m_pos=ppagehead->m_currentdatapos;
		}else
			SetNext(m_last.m_page,m_last.m_pos,m_currentpage,ppagehead->m_currentdatapos);

		m_last.m_page=m_currentpage;
		m_last.m_pos=ppagehead->m_currentdatapos;

		if(m_recordcount==0)
			m_top=m_last;

    	ppagehead->m_currentdatalen=dlen;
		ppagehead->m_currentdatapos+=dlen;
		ppagehead->m_recordcount++;

		m_pagepool->Put(ppage,true,false);

//		m_time=svutil::TTime::GetCurrentTimeEx();
		m_recordcount++;
		FlushData();

		if(m_recordcount>0)
			m_event.signal();

		return true;


	}else if(freesize>=sizeof(QUEUEHEAD))
	{


		memcpy(pt,&rht,sizeof(QUEUEHEAD));
		pt+=sizeof(QUEUEHEAD);
		F_SET(ppagehead->m_flag,CROSSPAGEDOWN);

		freesize=freesize-sizeof(QUEUEHEAD);
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
				puts("Add new page failed-1");
				F_SET(ppagehead->m_flag,PAGEFULL);
				F_SET(ppagehead->m_flag,BADPAGE);
				m_pagepool->Put(ppage,true,true);
				return false;

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

			pnexthead->m_pagetype=PageHead::queuerecordpagetype;
			if(pos==0)
			{
				ppagehead->m_nextrecordpage=pnext->m_pos;
				pnexthead->m_prerecordpage=ppage->m_pos;
			}else
			{
				pnexthead->m_prerecordpage=ptem->m_pos;
				ptem->GetPageHead()->m_nextrecordpage=pnext->m_pos;
				m_pagepool->Put(ptem,true,true);
			}

			ptem=pnext;
			pos++;

			F_SET(pnexthead->m_flag,CROSSPAGEUP);

			if(mlen<=pnexthead->m_datasize)
			{
				memmove(pnext->m_data,pdd,mlen);
				pdd+=mlen;

				RECORDPOS oldlast=m_last;
				m_last.m_page=m_currentpage;
				m_last.m_pos=ppagehead->m_currentdatapos;

				if(m_recordcount==0)
					m_top=m_last;



			   pnexthead->m_currentdatalen=0;
		       pnexthead->m_currentdatapos=mlen;

			   ppagehead->m_recordcount++;
			   m_pagepool->Put(ppage,true,true);
			   m_pagepool->Put(pnext,true,false);
			   m_currentpage=pnext->m_pos;
			   m_recordcount++;

			   SetNext(oldlast.m_page,oldlast.m_pos,m_last.m_page,m_last.m_pos);

			   FlushData();

			   if(m_recordcount>0)
				   m_event.signal();

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
			puts("Add new page failed");
			m_pagepool->Put(ppage,true,true);
			return false;

		}
		PageHead *pnexthead = pnext->GetPageHead();
		if(pnexthead==NULL)
		{
			puts("Get new page failed");
			m_pagepool->Put(ppage,true,true);
			m_pagepool->Put(pnext,true,true);
			return false;

		}

		pnexthead->m_pagetype=PageHead::queuerecordpagetype;

		ppagehead->m_pagetype=PageHead::queuerecordpagetype;
//		ppagehead->m_lastrecordpos=ppagehead->m_currentdatapos-ppagehead->m_currentdatalen;
		ppagehead->m_lastrecordpos=oldlastrecordpos;

		ppagehead->m_nextrecordpage=pnexthead->m_pos;
		pnexthead->m_prerecordpage=ppagehead->m_pos;
		m_currentpage=pnext->m_pos;

		m_pagepool->Put(ppage,true,true);
		m_pagepool->Put(pnext,true,false);

		FlushData();

		return Append(data,len,level);

	}

	return true;

}
int Queue::Peek(svutil::buffer &buf,S_UINT &datalen,bool block,S_UINT timer)
{
	bool isrepeat=false;

LOOP:

	if(m_recordcount==0)
	{
		if(isrepeat)
			return QUEUE_EMPTY;
		m_event.reset();

		if(block)
		{
			if(!m_event.wait())
			{
				puts("wait failed");
				return QUEUE_SYNERROR;

			}
		}
		else
		{
	    	timeout_t tim=timer;
			if(timer==0)
				return QUEUE_EMPTY;
			if(!m_event.wait(tim))
			{
//				puts("wait time out");
				return QUEUE_EMPTY;
			}
			isrepeat=true;
		}

		goto LOOP;

	}


	MutexLock lock(m_mutex);

	int rlen=0;
	S_UINT blen=buf.size();

	while(true)
	{
		rlen=this->PeekTopRecord(buf,blen);
		puts("Pop record over");

		if(rlen!=QUEUE_OK)
		{
			if(rlen==QUEUE_BUFFERSMALL)
			{
				
				blen+=QBUFFERLEN;
				if(!buf.resetsize(blen))
				{
					return QUEUE_MEMORYERROR;
				}

				continue;
			}

			return rlen;
		}else
			break;
	}

	datalen=blen;

	return rlen;

}
int Queue::PopTop(svutil::buffer &buf,S_UINT &datalen,bool block,S_UINT timer)
{
	bool isrepeat=false;

LOOP:

	if(m_recordcount==0)
	{
		if(isrepeat)
			return QUEUE_EMPTY;
		m_event.reset();

		if(block)
		{
			if(!m_event.wait())
			{
				puts("wait failed");
				return QUEUE_SYNERROR;

			}
		}
		else
		{
	    	timeout_t tim=timer;
			if(timer==0)
				return QUEUE_EMPTY;
			if(!m_event.wait(tim))
			{
//				puts("wait time out");
				return QUEUE_EMPTY;
			}
			isrepeat=true;
		}

		goto LOOP;

	}


	MutexLock lock(m_mutex);

	int rlen=0;
	S_UINT blen=buf.size();

	while(true)
	{
		rlen=PopTopRecord(buf,blen);
		puts("Pop record over");

		if(rlen!=QUEUE_OK)
		{
			if(rlen==QUEUE_BUFFERSMALL)
			{
				
				blen+=QBUFFERLEN;
				if(!buf.resetsize(blen))
				{
					return QUEUE_MEMORYERROR;
				}

				continue;
			}

			return rlen;
		}else
			break;
	}

	datalen=blen;

	return rlen;

}
bool Queue::DeleteData()
{
	MutexLock lock(m_mutex);

	if((m_top.m_page<0)||(m_top.m_pos<0))
		return true;

	Page* ppage=m_pagepool->Get(m_top.m_page);
	if(ppage==NULL)
		return false;

	FreePage(ppage);

	m_currentpage=-1;

	FlushData();
	m_event.reset();

	return true;

}
bool Queue::Clear()
{
	MutexLock lock(m_mutex);

	if((m_top.m_page<0)||(m_top.m_pos<0))
		return true;

	Page* ppage=m_pagepool->Get(m_top.m_page);
	if(ppage==NULL)
		return false;

	FreePage(ppage);

	m_recordcount=0;
	m_top.m_page=-1;
	m_top.m_pos=-1;
	m_last=m_top;

	Page *pcpage=m_pagepool->GetFreeEx();
	if(pcpage==NULL)
	{
		m_firstpage=-1;
		m_currentpage=-1;
	}
	else
	{
    	m_firstpage=pcpage->m_pos;
		m_currentpage=m_firstpage;
		pcpage->GetPageHead()->m_pagetype=PageHead::queuerecordpagetype;
    	m_pagepool->Put(pcpage,true,false);
	}

	FlushData();
	m_event.reset();

	return true;

}
int  Queue::PeekTopRecord(char *buf,S_UINT &buflen)
{
	MutexLock lock(m_mutex);

	if(m_recordcount==0)
		return QUEUE_EMPTY;

	if((m_top.m_page<0)||(m_top.m_pos<0))
		return QUEUE_UNKNOWERROR;

	Page *ppage=m_pagepool->Get(m_top.m_page);
	if(ppage==NULL)
		return QUEUE_GETPAGEERROR;

	if(ppage->m_data==NULL)
	{
		m_pagepool->Put(ppage,false,true);
		return QUEUE_GETPAGEERROR;
	}
	PageHead *ppagehead=ppage->GetPageHead();
	if(ppagehead==NULL)
	{
		m_pagepool->Put(ppage,false,true);
		return QUEUE_GETPAGEERROR;
	}


	char *pt=ppage->m_data+m_top.m_pos;
	if(((LPQUEUEHEAD)pt)->m_cblen!=sizeof(QUEUEHEAD))
	{
		m_pagepool->Put(ppage,false,true);
		return QUEUE_UNKNOWERROR;
	}

	S_UINT len=((LPQUEUEHEAD)pt)->m_datalen;
	len+=sizeof(QUEUEHEAD);


	if(len>buflen)
	{
		m_pagepool->Put(ppage,false,false);
		return QUEUE_BUFFERSMALL;
	}


	int flen=ppagehead->m_datasize-m_top.m_pos;

	if(len<=flen)
	{
		memmove(buf,pt,len);
		m_pagepool->Put(ppage,false,false);
		buflen=len;

		return QUEUE_OK;

	}else if(F_ISSET(ppagehead->m_flag,CROSSPAGEDOWN))
	{
		int csize=ppagehead->m_datasize-m_top.m_pos;
		char *ptbuf=buf;
		memmove(ptbuf,pt,csize);
		ptbuf+=csize;
		int mlen=len-csize;
		Page *ptpt=ppage;
		while(true)
		{
			Page *ptt=GetNextPage(ptpt);
			if(ptt==NULL)
			{
				m_pagepool->Put(ptpt,false,true);
				return QUEUE_GETPAGEERROR;
			}
			PageHead *ptthead=ptt->GetPageHead();
			if(ptthead==NULL)
			{
				m_pagepool->Put(ptt,false,true);
				m_pagepool->Put(ptpt,false,true);
				return QUEUE_GETPAGEERROR;
			}
		
			m_pagepool->Put(ptpt,false,true);

			if(F_ISSET(ptthead->m_flag,CROSSPAGEUP))
			{
				flen=ptthead->m_datasize;
				if(mlen<=flen)
				{
					memmove(ptbuf,ptt->m_data,mlen);
					m_pagepool->Put(ptt,false,true);
					buflen=len;

					return QUEUE_OK;
				}else
				{
					memmove(ptbuf,ptt->m_data,ptthead->m_datasize);
					ptbuf+=ptthead->m_datasize;
					ptpt=ptt;
					mlen=mlen-ptthead->m_datasize;
				}

			}else
			{
				m_pagepool->Put(ptt,false,true);
				return QUEUE_UNKNOWERROR;
			}

		}
	}else
	{
		m_pagepool->Put(ppage,false,true);
		return QUEUE_UNKNOWERROR;
	}


	return QUEUE_OK;


}
int Queue::PopTopRecord(char *buf,S_UINT &buflen)
{
	MutexLock lock(m_mutex);
	if(m_recordcount==0)
		return QUEUE_EMPTY;

	if((m_top.m_page<0)||(m_top.m_pos<0))
		return QUEUE_UNKNOWERROR;

	Page *ppage=m_pagepool->Get(m_top.m_page);
	if(ppage==NULL)
		return QUEUE_GETPAGEERROR;

	if(ppage->m_data==NULL)
	{
		m_pagepool->Put(ppage,false,true);
		return QUEUE_GETPAGEERROR;
	}
	PageHead *ppagehead=ppage->GetPageHead();
	if(ppagehead==NULL)
	{
		m_pagepool->Put(ppage,false,true);
		return QUEUE_GETPAGEERROR;
	}


	char *pt=ppage->m_data+m_top.m_pos;

	if(((LPQUEUEHEAD)pt)->m_cblen!=sizeof(QUEUEHEAD))
	{
		m_pagepool->Put(ppage,false,true);
		return QUEUE_UNKNOWERROR;
	}

	S_UINT len=((LPQUEUEHEAD)pt)->m_datalen;
	len+=sizeof(QUEUEHEAD);

	if(len>buflen)
	{
		m_pagepool->Put(ppage,false,false);
		return QUEUE_BUFFERSMALL;
	}

	bool isend=false;
	RECORDPOS next;
	if(m_top==m_last)
	{
		next.m_page=-1;
		next.m_pos=-1;
		m_last=next;
		isend=true;

	}else
		next=((LPQUEUEHEAD)pt)->m_nextrecord;

	RECORDPOS ttp=m_top;




	int flen=ppagehead->m_datasize-m_top.m_pos;

	if(len<=flen)
	{
		memmove(buf,pt,len);
//		buflen=len;
		ppagehead->m_recordcount--;
		m_recordcount--;
		m_top=next;
		if(isend)
		{
			ppagehead->m_currentdatalen=0;
			ppagehead->m_currentdatapos=0;
			m_recordcount=0;
			m_pagepool->Put(ppage,true,false);

		}else
		{
			if((flen==len)||(ttp.m_pos==ppagehead->m_lastrecordpos))
				m_pagepool->PutFree(ppage);
			else
				m_pagepool->Put(ppage,false,false);
		}

		FlushData();
		buflen=len;

		return QUEUE_OK;

	}else if(F_ISSET(ppagehead->m_flag,CROSSPAGEDOWN))
	{
		int csize=ppagehead->m_datasize-m_top.m_pos;
		char *ptbuf=buf;
		memmove(ptbuf,pt,csize);
		ptbuf+=csize;
		int mlen=len-csize;
		Page *ptpt=ppage;
		ppagehead->m_recordcount--;
		while(true)
		{
			Page *ptt=GetNextPage(ptpt);
			if(ptt==NULL)
			{
				m_pagepool->Put(ptpt,false,true);
				return QUEUE_GETPAGEERROR;
			}
			PageHead *ptthead=ptt->GetPageHead();
			if(ptthead==NULL)
			{
				m_pagepool->Put(ptt,false,false);
				m_pagepool->Put(ptpt,false,true);
				return QUEUE_GETPAGEERROR;
			}
		
			//m_pagepool->Put(ptpt,false,true);
			m_pagepool->PutFree(ptpt);

			if(F_ISSET(ptthead->m_flag,CROSSPAGEUP))
			{
				flen=ptthead->m_datasize;
				if(mlen<=flen)
				{
					memmove(ptbuf,ptt->m_data,mlen);
				//	buflen=len;
					m_recordcount--;
					m_top=next;
					if(isend)
					{
						m_recordcount=0;
						ptthead->m_currentdatalen=0;
						ptthead->m_currentdatapos=0;
						m_pagepool->Put(ptt,true,false);

					}else
					{
						if((flen==mlen)||(ptthead->m_lastrecordpos<0))
							m_pagepool->PutFree(ptt);
						else
						{
							F_CLR(ptthead->m_flag,CROSSPAGEUP);
							m_pagepool->Put(ptt,true,false);
						}
					}
					FlushData();

					buflen=len;

					return QUEUE_OK;
				}else
				{
					memmove(ptbuf,ptt->m_data,ptthead->m_datasize);
					ptbuf+=ptthead->m_datasize;
					ptpt=ptt;
					mlen=mlen-ptthead->m_datasize;
				}

			}else
			{
				m_pagepool->Put(ptt,false,true);
				return QUEUE_UNKNOWERROR;
			}

		}
	}else
	{
		m_pagepool->Put(ppage,false,true);
		return QUEUE_UNKNOWERROR;
	}

	return QUEUE_OK;

}
bool Queue::QueryAllRecord(svutil::buffer &buf,int &datalen)
{
	return true;

}

bool Queue::GetQueueAllMessageLabels(std::list<string> & retlist)
{
	MutexLock lock(m_mutex);
	cout<<"Get all labels of queue:"<<endl;

	if((m_last.m_page<0)||(m_last.m_pos<0))
		return false;

	Page *ppage=m_pagepool->Get(m_last.m_page,false); 
	if(ppage==NULL)
		return false;
	m_pagepool->Put(ppage,false,false);

	if(ppage->m_data==NULL)
		return false;

	PageHead *ppagehead=ppage->GetPageHead();
	if(ppagehead==NULL)
		return false;

	if((m_currentpage==m_firstpage)&&(ppagehead->m_lastrecordpos<0))
		return true;

	if(ppagehead->m_lastrecordpos<0)
	{
		Page *ptemp=GetPreRecordPage(ppage);
		if(ptemp==NULL)
			return false;
		m_pagepool->Put(ppage,false,true);

		ppage=ptemp;
		ppagehead=ppage->GetPageHead();
		if(ppagehead==NULL)
			return false;
	}

	int pos= m_last.m_pos;

	char *pc=ppage->m_data + pos;
	if(((LPQUEUEHEAD)pc)->m_cblen!=sizeof(QUEUEHEAD))
		return false;

	//svutil::buffer tempbuf;
	//if(!tempbuf.checksize(4096))
	//	return false;

	cout<<"    "<<m_queuename.c_str()<<endl;
	const int AtLeaseSize( sizeof(QUEUEHEAD)+sizeof(S_UINT)+sizeof(svutil::TTime)+sizeof(int)+sizeof(S_UINT) );

	std::set<Page *> pagepoints;
	std::set<LPQUEUEHEAD> pcpoints;

	int times(0),pagecount(0);
	bool retlabel=true, anerror=true;
	while(true)
	{
		int rlen=0,blen=0,prep=0,pmpage=0;
		int flen=ppagehead->m_datasize - m_top.m_pos;
		LPQUEUEHEAD temppc;

		try{
			temppc= reinterpret_cast<LPQUEUEHEAD>(pc);
			if(temppc!=NULL)
			{
				std::set<LPQUEUEHEAD>::iterator pit=pcpoints.find(temppc);
				if(pit!=pcpoints.end())
					anerror=false;
			}
			else
			{
				anerror=false;
				cout<<"anerror==false"<<endl;
			}

			if( anerror )
			{
				blen = temppc->m_datalen;
				prep = temppc->m_prerecord.m_pos;
				pmpage=  temppc->m_prerecord.m_page;
			}
		}
		catch(...)
		{
			anerror=false;
			cout<<"anerror==false"<<endl;
		}

		if( anerror )
		{
			pcpoints.insert(temppc);
			blen+= sizeof(QUEUEHEAD);

			if(blen<=flen && blen>AtLeaseSize )
			{
				//cout<<" blen < flen : "<<blen<<"  "<<flen<<"   prep: "<<prep<<"   pmpage: "<<pmpage<<endl;
				retlabel=GetLabelData(retlist, pc, blen);
			}
			else
			{
				//cout<<" blen > flen : "<<blen<<"  "<<flen<<"   prep: "<<prep<<"   pmpage: "<<pmpage<<endl;

				//char * buf= tempbuf;
				//int i=0;
				//while(true)
				//{
				//	rlen=GetSingleRecord(ppage,pos,buf,blen);
				//	if(rlen>=0)
				//	{
				//		buf+=rlen;
				//		blen-=rlen;
				//		GetLabelData(retlist, buf, rlen+sizeof(QUEUEHEAD));
				//		break;
				//	}
				//	if(++i>500)
				//		break;
				//}
			}
		}

		if(retlist.size()>=500)
			return true;

		if(prep==-1 || pmpage==-1)
			return true;

		if(anerror && prep>=0 && blen>AtLeaseSize && blen<=flen && (++times)<500) 
		{
			if( (++times)>500 )
			{
				cout<<" times protection"<<endl;
				break;
			}
			pc=ppage->m_data+prep;
			pos=prep;
			continue;
		}
		else
		{
			times=0;
			retlabel=true;  anerror=true;

			std::set<Page *>::iterator it;
			Page *ptem;
			do{
				ptem=GetPreRecordPage(ppage);   
				if(ptem==NULL)
				{
					m_pagepool->Put(ptem,false,true);
					return true;
				}
				if(ptem->m_data==NULL)
				{
					m_pagepool->Put(ptem,false,true);
					return true;
				}
				it=pagepoints.find(ptem);
			}while(it!=pagepoints.end());

			cout<<"---Get labels from another page: "<<++pagecount<<" ---"<<endl;

			ppage=ptem;
			pagepoints.insert(ppage);

			m_pagepool->Put(ppage,false,false);

			ppagehead=ppage->GetPageHead();
			if(ppagehead==NULL)
			{
				m_pagepool->Put(ppage,false,true);
				return true;
			}
			if(ppagehead->m_lastrecordpos<0)
			{
				m_pagepool->Put(ppage,false,true);
				return true;
			}

			pos=ppagehead->m_lastrecordpos; 
			pc=ppage->m_data+pos;
		}

	}
	return true;
}

#include <sstream>
bool Queue::GetLabelData(std::list<string> & retlist,const char * pt,S_UINT len)
{
	try{
		QueueRecord prd;
		if(!prd.CreateObjectByRawData(pt+sizeof(QUEUEHEAD),len-sizeof(QUEUEHEAD)))
		{
			puts("Create QueueRecord LabelData object failed");
			return false;
		}
		std::ostringstream textstm;
		textstm<<prd.GetCreateTime().Format();
		textstm<<"  label: \""<<prd.GetLabel()<<"\"";
		textstm<<"  level: \""<<prd.GetLevel()<<"\"";
		textstm<<"  datalen: \""<<prd.GetDataLen()<<"\"";
		retlist.push_back( textstm.str() );
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool Queue::FlushData()
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
	int queuesize=GetRawDataSize();

	if(ppagehead->m_datasize<queuesize)
	{
		ppagehead->m_badpage=1;
		F_SET(ppagehead->m_flag,BADPAGE);
		m_page->m_ppt->Put(pt,true,true);
		puts("data size is small");
		return false;
	}

	char *pdata=pt->m_data;

	memmove(pdata,&m_currentpage,tlen);
	pdata+=tlen;

	memmove(pdata,&m_createtime,sizeof(svutil::TTime));
	pdata+=sizeof(svutil::TTime);

	memmove(pdata,&m_queuetype,tlen);
	pdata+=tlen;

	memmove(pdata,&m_recordcount,sizeof(S_UINT));
	pdata+=sizeof(S_UINT);

	memmove(pdata,&m_top,sizeof(RECORDPOS));
	pdata+=sizeof(RECORDPOS);

	memmove(pdata,&m_last,sizeof(RECORDPOS));
	pdata+=sizeof(RECORDPOS);

	strcpy(pdata,m_queuename.c_str());
	pdata+=m_queuename.size();
	pdata[0]='\0';
	pdata++;

	ppagehead->m_tablesize=queuesize;   //借用

	return m_page->m_ppt->Put(pt,true,false);

}

int Queue::GetRawDataSize()
{
	int len=0,tlen=sizeof(int);

	len+=sizeof(int);  //m_currentpage

	len+=sizeof(svutil::TTime);
	len+=tlen;       //m_queuetype
	len+=sizeof(S_UINT);  //m_recordcount
	len+=sizeof(RECORDPOS);  //m_top
	len+=sizeof(RECORDPOS);  //m_last
	len+=m_queuename.size()+1;

	return len;
    
}
bool Queue::CreateObjByPage(Page *pgage)
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

	memmove(&m_currentpage,pt,sizeof(int));
	pt+=sizeof(int);

	memmove(&m_createtime,pt,sizeof(svutil::TTime));
	pt+=sizeof(svutil::TTime);

	memmove(&m_queuetype,pt,sizeof(int));
	pt+=sizeof(int);

	memmove(&m_recordcount,pt,sizeof(S_UINT));
	pt+=sizeof(S_UINT);

	memmove(&m_top,pt,sizeof(RECORDPOS));
	pt+=sizeof(RECORDPOS);

	memmove(&m_last,pt,sizeof(RECORDPOS));
	pt+=sizeof(RECORDPOS);

	m_queuename=pt;

	m_page=pgage;

	return true;
}
bool Queue::SetNext(RECORDPOS src,RECORDPOS pos)
{
	return SetNext(src.m_page,src.m_pos,pos.m_page,pos.m_pos);

}
bool Queue::SetNext(int srcpage,int srcpos,int page,int pos)
{
	if((srcpage<0)||(srcpos<0))
		return false;

	Page *pt = m_pagepool->Get(srcpage);
	if(pt==NULL)
		return false;

	if(pos>pt->GetPageHead()->m_datasize-sizeof(QUEUEHEAD))
	{
		m_pagepool->Put(pt,false,true);
		return false;
	}

	if(pt->m_data==NULL)
	{
		m_pagepool->Put(pt,false,true);
		return false;
	}

	char *pd=pt->m_data+srcpos;

	((LPQUEUEHEAD)pd)->m_nextrecord.m_page=page;
	((LPQUEUEHEAD)pd)->m_nextrecord.m_pos=pos;

	m_pagepool->Put(pt,true,true);
	return true;

}

