#include "PagePool.h"

PagePool::PagePool(void)
{
	m_LastPage=NULL;
	m_Head.m_PageSize=0;
	m_Head.m_PageCount=0;
	m_Head.m_TotalCount=0;
	m_hd=NULL;

}
PagePool::PagePool(int pagesize,int pagecount)
{
	m_Head.m_PageSize=pagesize;
	m_Head.m_TotalCount=pagecount;
	m_LastPage=NULL;
	m_hd=NULL;
}

PagePool::~PagePool(void)
{
	{
		MutexLock lock(m_Mutex);
		MutexLock ilock(m_IOMutex);

		for(int i=0;i<m_PagePool.size();i++)
		{
			Page *pt=m_PagePool[i];
			if(pt!=NULL)
				delete pt;
		}

		//	if(m_hd)
		//		::CloseHandle(m_hd);

		for(int n=0;n<m_fhd.size();n++)
		{
			SubPool *psp=m_fhd[n];
			if(psp!=NULL)
				delete psp;
		}
	}


}

Page *PagePool::GetFreeEx(void)
{
	ost::MutexLock lock(m_Mutex);

	Page *page=GetFree();
	if(page!=NULL)
		return page;

	if(!AddNewPagesEx(1000))
		return NULL;
	return GetFree();

}

Page *PagePool::GetFree(void){

	ost::MutexLock lock(m_Mutex);
	if(m_FreePageList.size()>0)
	{
		Page *pt=m_FreePageList.front();
		m_FreePageList.pop_front();
	//	pt->swapin();
		pt->swapinex();
		if(pt->m_isuse)
			return NULL;
		pt->m_isuse=true;
		return pt;
	}
	return NULL;
}
bool PagePool::PutFree(Page *pt)
{
	ost::MutexLock lock(m_Mutex);
	PageHead *pph=pt->GetPageHead();
	if(pph==NULL)
		return false;
	pph->m_unused=1;
	pph->m_flag=0;
	F_SET(pph->m_flag,PAGEUNUSED);
	pph->m_lastrecordpos=-1;
//	pph->m_prerecordpage=-1;
	pph->m_currentdatalen=0;
	pph->m_currentdatapos=0;
	pph->m_nextrecordpage=-1;
	pph->m_prerecordpage=-1;
	pph->m_recordcount=0;

	pt->m_ischange=true;
	pt->m_isuse=false;

	pt->swapoutex(true);
	m_FreePageList.push_back(pt);
	return true;

}

bool PagePool::RemoveFromFreePages(int pos)
{
	ost::MutexLock lock(m_Mutex);
	std::list<Page *>::iterator it;
	for(it=m_FreePageList.begin();it!=m_FreePageList.end();it++)
	{
		if((*it)->m_pos==pos)
			m_FreePageList.erase(it);
		return true;
	}
	return false;

}

Page *PagePool::Get(int pos)
{
	if((pos<0)||(pos>=m_Head.m_TotalCount))
		return NULL;

	ost::MutexLock lock(m_Mutex);
	Page *pt=m_PagePool[pos];
	if(pt->m_isuse)
		return NULL;
	pt->m_isuse=true;
	if(pt->m_rawdata==NULL)
		pt->swapinex();
	//	pt->swapin();
	PageHead *phead = pt->GetPageHead();
	if(phead==NULL)
		return false;
	if(F_ISSET(phead->m_flag,PAGEUNUSED))
		RemoveFromFreePages(pos);
	return pt;

}

Page *PagePool::Get(int pos,bool readonly)
{
	if(!readonly)
		return Get(pos);
	if((pos<0)||(pos>=m_Head.m_TotalCount))
		return NULL;
	Page *pt=m_PagePool[pos];
	return pt;
}

bool PagePool::FlushBuffer()
{
	if(m_hd==NULL)
		return false;
	return ::FlushFileBuffers(m_hd);
}

bool PagePool::Put(Page *pt,bool isch,bool isdel)
{
	ost::MutexLock lock(m_Mutex);
	pt->m_isuse=false;
	if(!isch)
	{
		if(isdel)
		{
			if(pt->m_data!=NULL)
			{
				delete [] pt->m_rawdata;
				pt->m_data=NULL;
				pt->m_rawdata=NULL;
			}
		}
		return true;	
	}

	PageHead *phead = pt->GetPageHead();
	if(phead == NULL)
	{
		puts("put phead is NULL");
		return false;
	}

	pt->m_ischange=true;
	pt->m_isuse=false;
	phead->m_unused=0;
	F_CLR(phead->m_flag,PAGEUNUSED);
	return pt->swapoutex(isdel);
}
void PagePool::InitPool()
{
	if(m_Head.m_PageCount<=0)
		return;
	m_PagePool.resize(m_Head.m_PageCount);
	for(int i=0;i<m_Head.m_PageCount;i++)
		m_PagePool[i]=NULL;
}
bool PagePool::InitPoolEx()
{
	if(m_Head.m_TotalCount<=0)
		return false;
	m_PagePool.resize(m_Head.m_TotalCount);
	for(int i=0;i<m_Head.m_TotalCount;i++)
		m_PagePool[i]=NULL;
	return true;
}

bool PagePool::LoadPage(const char *filepath)
{
	ost::MutexLock lock(m_Mutex);
	DWORD dwcd=OPEN_EXISTING;
	HANDLE hd=::CreateFile(filepath,GENERIC_WRITE|GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,dwcd,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hd == INVALID_HANDLE_VALUE)
	{
		printf("Open file failed errorid:%d\n",::GetLastError());
		return false;
	}

	DWORD rlen=0;

	if(!::ReadFile(hd,&m_Head,sizeof(struct PagePoolHead),&rlen,NULL))
	{
		printf("Read head failed errorid:%d\n",::GetLastError());
		return false;
	}

	if(m_Head.m_version!=1)
	{
		puts("文件版本错误");
		return false;
	}
	if((m_Head.m_PageCount<=0)||(m_Head.m_PageSize<MINPAGESIZE))
	{
		puts("文件格出错");
		return false;

	}


	InitPool();
	char *buf = new char[m_Head.m_PageSize];

	memset(buf,0,m_Head.m_PageSize);

	for(int i=0;i<m_Head.m_PageCount;i++)
	{
		if(!::SetFilePointer(hd,i*m_Head.m_PageSize+sizeof(struct PagePoolHead),NULL,FILE_BEGIN))
		{
			printf("Load page -- move file pointer failed errorid:%d\n",::GetLastError());
			return false;
		}
		if(!::ReadFile(hd,buf,sizeof(PageHead),&rlen,NULL))
		{
			printf("Read file failed errorid:%d\n",::GetLastError());
			delete [] buf;
			return false;
		}
		if(((PageHead*)buf)->m_pos!=i)
		{
			printf("Load page info failed\n");
			delete [] buf;
			return false;
		}

		Page * pg=new Page(this,i);
		//		memcpy(&(pg->m_Head),buf,sizeof(PageHead));
		if(pg)
		{
			pg->m_pos=i;
			if(((PageHead*)buf)->m_unused==1)
				m_FreePageList.push_back(pg);

			m_PagePool[i]=pg;
		}

	}

	delete [] buf;
	m_hd=hd;

	return true;

}
bool PagePool::LoadPageEx(const char *filetitle)
{
	ost::MutexLock lock(m_Mutex);

	m_FileTitle=filetitle;

	char filepath[1024]={0};
	sprintf(filepath,"%s_00.db",filetitle);
	std::string tempf= filepath;
	//if( tempf.find("idc_data")==std::string::npos )
	//	printf("filepath:%s\n",filepath);

	DWORD dwcd=OPEN_EXISTING;
	HANDLE hd=::CreateFile(filepath,GENERIC_WRITE|GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,dwcd,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hd == INVALID_HANDLE_VALUE)
	{
		//puts("open error");
		printf("Opening file errorid:%d\n   %s\n",::GetLastError(),filepath);
		return false;
	}

	DWORD rlen=0;

	if(!::ReadFile(hd,&m_Head,sizeof(struct PagePoolHead),&rlen,NULL))
	{
		printf("Read head failed errorid:%d\n   %s\n",::GetLastError(),filepath);
		return false;
	}

	if(m_Head.m_version!=1)
	{
		puts("文件版本错误");
		return false;
	}
	if((m_Head.m_TotalCount<=0)||(m_Head.m_PageSize<MINPAGESIZE))
	{
		puts("文件格出错");
		return false;
	}
	InitPoolEx();

	int PCount=m_Head.m_PageCount;
	int bindex=m_Head.m_BasePageIndex;

	char *buf = new char[m_Head.m_PageSize];
	memset(buf,0,m_Head.m_PageSize);


	SubPool *sp=new SubPool();
	if(sp==NULL)
		return false;
	m_fhd.resize(1);
	m_fhd[0]=sp;
	sp->m_phead=m_Head;
	sp->m_hd=hd;
	m_hd=hd;

	for(int i=0;i<=m_Head.m_FileCount;i++)
	{
		if(i>0)
		{
			sprintf(filepath,"%s_%02d.db",filetitle,i);
			hd=::CreateFile(filepath,GENERIC_WRITE|GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,dwcd,FILE_ATTRIBUTE_NORMAL,NULL);
			if(hd == INVALID_HANDLE_VALUE)
			{
				printf("Open file failed error id:%d\n",::GetLastError());
				return false;
			}

			if(!::ReadFile(hd,buf,sizeof(struct PagePoolHead),&rlen,NULL))
			{
				printf("Read head failed errorid:%d\n",::GetLastError());
				return false;
			}
			if(((PagePoolHead *)buf)->m_version!=1)
			{
				puts("Version error");
				return false;
			}

			PCount=((PagePoolHead *)buf)->m_PageCount;
			bindex=((PagePoolHead *)buf)->m_BasePageIndex;

			SubPool *sp=new SubPool();
			if(sp==NULL)
				return false;
			m_fhd.resize(i+1);
			m_fhd[i]=sp;
			sp->m_hd=hd;
			sp->m_phead=*((PagePoolHead *)buf);
			m_hd=hd;

			
		}

		for(int n=0;n<PCount;n++)
		{
			if(!::SetFilePointer(hd,n*m_Head.m_PageSize+sizeof(struct PagePoolHead),NULL,FILE_BEGIN))
			{
				printf("Load page -- move file pointer failed errorid:%d\n",::GetLastError());
				return false;
			}
			if(!::ReadFile(hd,buf,sizeof(PageHead),&rlen,NULL))
			{
				printf("Read file failed errorid:%d\n",::GetLastError());
				delete [] buf;
				return false;
			}
			if(((PageHead*)buf)->m_pos!=bindex+n)
			{
				printf("Load page info failed\n");
				delete [] buf;
				return false;
			}

			Page * pg=new Page(this,bindex+n);
			if(pg)
			{
				pg->m_pos=bindex+n;
				pg->m_mpos.m_fileindex=i;
				pg->m_mpos.m_pos=n;
				if(((PageHead*)buf)->m_unused==1)
					m_FreePageList.push_back(pg);

				m_PagePool[bindex+n]=pg;
			}

		}

	}
	
	delete [] buf;
	m_hd=hd;

	return true;
}

bool PagePool::CreatePage(const char * filepath,bool overlay)
{
	ost::MutexLock lock(m_Mutex);
	InitPool();

	DWORD dwcd;
	if(overlay)
		dwcd=CREATE_ALWAYS;
	else
		dwcd=CREATE_NEW;

	HANDLE hd=::CreateFile(filepath,GENERIC_WRITE|GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,dwcd,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hd == INVALID_HANDLE_VALUE)
	{
		printf("Create file failed:%d\n",::GetLastError());
		return false;
	}

	LONG size=m_Head.m_PageSize * m_Head.m_PageCount+sizeof(PagePoolHead);
	if((size>MAXINT-2)||(size<1))
	{
		printf("Size too big");
		return false;
	}
  


	DWORD pos=::SetFilePointer(hd,size,NULL,FILE_BEGIN);
	if(pos==INVALID_SET_FILE_POINTER)
	{
		printf("Set file size failed,error:%d\n",::GetLastError());
		return false;
	}
	::SetEndOfFile(hd);

	::SetFilePointer(hd,0,0,FILE_BEGIN);

	DWORD dwlen=0;

	if(!::WriteFile(hd,&m_Head,sizeof(PagePoolHead),&dwlen,NULL))
	{
		printf("Write head failed errorid:%d\n",::GetLastError());
		return false;
	}

	char *buf = new char[m_Head.m_PageSize];

	memset(buf,0,m_Head.m_PageSize);

	for(int i=0;i<m_Head.m_PageCount;i++)
	{
		((PageHead *)buf)->m_pos=i;
		((PageHead *)buf)->m_unused=1;
		F_SET(((PageHead *)buf)->m_flag,PAGEUNUSED);
		((PageHead *)buf)->m_datasize=m_Head.m_PageSize-sizeof(struct PageHead);
		((PageHead *)buf)->m_pagesize=m_Head.m_PageSize;
		((PageHead *)buf)->m_lastrecordpos=-1;
		((PageHead *)buf)->m_prerecordpage=-1;
		((PageHead *)buf)->m_nextrecordpage=-1;

		if(!::WriteFile(hd,buf,m_Head.m_PageSize,&dwlen,NULL))
		{
			printf("Write file failed errorid:%d\n",::GetLastError());
			delete [] buf;
			::CloseHandle(hd);
			return false;
		}

		Page * pg=new Page(this,i);
		if(pg)
		{
//			memcpy(&(pg->m_Head),buf,sizeof(PageHead));
			pg->m_pos=i;
			m_FreePageList.push_back(pg);

			m_PagePool[i]=pg;
		}

	}

	delete [] buf;
	::SetFilePointer(hd,0,0,FILE_BEGIN);

	m_hd=hd;

	return true;


}

bool PagePool::CreatePageEx(const char * filetitle,int fileindex,int pagecount,int pageindex,bool overlay)
{
	ost::MutexLock lock(m_Mutex);

	m_FileTitle=filetitle;

	char filepath[1024]={0};
	sprintf(filepath,"%s_%02d.db",filetitle,fileindex);

	DWORD dwcd;
	if(overlay)
		dwcd=CREATE_ALWAYS;
	else
		dwcd=CREATE_NEW;

	HANDLE hd=::CreateFile(filepath,GENERIC_WRITE|GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,dwcd,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hd == INVALID_HANDLE_VALUE)
	{
		printf("Create file failed:%d\n",::GetLastError());
		return false;
	}

	int PCount=pagecount;

	LONG size=m_Head.m_PageSize * pagecount;
	if((size>MAXINT-2)||(size<1))
	{
		printf("Size too big");
		return false;
	}

	bool over=true;
	if(size>m_Head.m_PerFileSize)
	{
		if(size-m_Head.m_PerFileSize>m_Head.m_PageSize)
			over=false;
		size=m_Head.m_PerFileSize;
		PCount=size/m_Head.m_PageSize;
	}


	size+=sizeof(PagePoolHead);



	DWORD pos=::SetFilePointer(hd,size,NULL,FILE_BEGIN);
	if(pos==INVALID_SET_FILE_POINTER)
	{
		printf("Set file size failed,error:%d\n",::GetLastError());
		return false;
	}
	::SetEndOfFile(hd);

	::SetFilePointer(hd,0,0,FILE_BEGIN);

   SubPool *sp = new SubPool();
   if(sp==NULL)
	   return false;

	if(fileindex==0)
	{
		m_Head.m_TotalCount=pagecount;
		m_Head.m_PageCount=PCount;
		m_Head.m_FileIndex=fileindex;
		m_Head.m_BasePageIndex=pageindex;
		sp->m_phead=m_Head;
	}else
	{
		sp->m_phead=m_Head;
		sp->m_phead.m_FileIndex=fileindex;
		sp->m_phead.m_PageCount=PCount;
		sp->m_phead.m_BasePageIndex=pageindex;
	}

	

	DWORD dwlen=0;

	if(!::WriteFile(hd,&sp->m_phead,sizeof(PagePoolHead),&dwlen,NULL))
	{
		printf("Write head failed errorid:%d\n",::GetLastError());
		return false;
	}

	char *buf = new char[m_Head.m_PageSize];

	memset(buf,0,m_Head.m_PageSize);

	int i=0;

	for(i=pageindex;i<PCount+pageindex;i++)
	{
		((PageHead *)buf)->m_pos=i;
		((PageHead *)buf)->m_unused=1;
		F_SET(((PageHead *)buf)->m_flag,PAGEUNUSED);
		((PageHead *)buf)->m_datasize=m_Head.m_PageSize-sizeof(struct PageHead);
		((PageHead *)buf)->m_pagesize=m_Head.m_PageSize;
		((PageHead *)buf)->m_lastrecordpos=-1;
		((PageHead *)buf)->m_prerecordpage=-1;
		((PageHead *)buf)->m_nextrecordpage=-1;
		((PageHead *)buf)->m_fileindex=fileindex;
		((PageHead *)buf)->m_mpos=i-pageindex;


		if(!::WriteFile(hd,buf,m_Head.m_PageSize,&dwlen,NULL))
		{
			printf("Write file failed errorid:%d\n",::GetLastError());
			delete [] buf;
			::CloseHandle(hd);
			return false;
		}

		Page * pg=new Page(this,i);
		if(pg)
		{
//			memcpy(&(pg->m_Head),buf,sizeof(PageHead));
			pg->m_pos=i;
			pg->m_mpos.m_fileindex=fileindex;
			pg->m_mpos.m_pos=i-pageindex;
			m_FreePageList.push_back(pg);

			m_PagePool[i]=pg;
		}

	}

	delete [] buf;
	::SetFilePointer(hd,0,0,FILE_BEGIN);

	m_fhd.resize(fileindex+1);
	sp->m_hd=hd;
	m_fhd[fileindex]=sp;
	m_hd=hd;
	m_Head.m_FileCount=fileindex;
	if(!over)
	{
		return CreatePageEx(filetitle,++fileindex,pagecount-PCount,i,overlay);
	}else
	{
		::SetFilePointer(m_fhd[0]->m_hd,0,0,FILE_BEGIN);
		if(!::WriteFile(m_fhd[0]->m_hd,&m_Head,sizeof(PagePoolHead),&dwlen,NULL))
		{
			printf("Write head failed errorid:%d\n",::GetLastError());
			return false;
		}
	}


	return true;

}

bool PagePool::AddNewPages(int count)
{	
	ost::MutexLock lock(m_Mutex);

	if(count<1)
		return false;
	if(m_hd==NULL)
		return false;
	LONG size=m_Head.m_PageSize * (m_Head.m_PageCount+count)+sizeof(PagePoolHead);
	if((size>MAXINT-2)||(size<1))
	{
		printf("Size too big");
		return false;
	}



	/*		m_PagePool.resize(m_Head.m_PageCount+count);
	for(int c=m_Head.m_PageCount;c<m_Head.m_PageCount+count;c++)
	m_PagePool[c]=NULL;
	*/
	DWORD oldend=::SetFilePointer(m_hd,0,NULL,FILE_END);
	if(oldend==INVALID_SET_FILE_POINTER)
	{
		printf("Seek file failed errid:%d\n",::GetLastError());
		return false;
	}

	size=m_Head.m_PageSize*count;

	DWORD newpos=::SetFilePointer(m_hd,size,NULL,FILE_CURRENT);
	if(newpos==INVALID_SET_FILE_POINTER)
	{
		printf("Seek file failed2 errid:%d\n",::GetLastError());
		return false;

	}

	if(newpos!=m_Head.m_PageSize * (m_Head.m_PageCount+count)+sizeof(PagePoolHead))
	{
		printf("Create file sapce failed newpos:%d\n",newpos);
		newpos=::SetFilePointer(m_hd,oldend,NULL,FILE_BEGIN);
		::SetEndOfFile(m_hd);
		return false;
	}

	::SetEndOfFile(m_hd);

	newpos=::SetFilePointer(m_hd,oldend,NULL,FILE_BEGIN);

	if(newpos==INVALID_SET_FILE_POINTER)
	{
		printf("Seek file failed3 errid:%d\n",::GetLastError());
		return false;

	}

	char *buf = new char[m_Head.m_PageSize];

	memset(buf,0,m_Head.m_PageSize);

	m_PagePool.resize(m_Head.m_PageCount+count);
	for(int i=m_Head.m_PageCount;i<m_Head.m_PageCount+count;i++)
		m_PagePool[i]=NULL;

	DWORD dwlen=0;

	for(int i=m_Head.m_PageCount;i<m_Head.m_PageCount+count;i++)
	{
		((PageHead *)buf)->m_pos=i;
		((PageHead *)buf)->m_unused=1;
		F_SET(((PageHead *)buf)->m_flag,PAGEUNUSED);
		((PageHead *)buf)->m_datasize=m_Head.m_PageSize-sizeof(struct PageHead);
		((PageHead *)buf)->m_pagesize=m_Head.m_PageSize;
		((PageHead *)buf)->m_lastrecordpos=-1;
		((PageHead *)buf)->m_prerecordpage=-1;
		((PageHead *)buf)->m_nextrecordpage=-1;



		if(!::WriteFile(m_hd,buf,m_Head.m_PageSize,&dwlen,NULL))
		{
			printf("Write file failed errorid:%d\n",::GetLastError());
			delete [] buf;
			::CloseHandle(m_hd);
			return false;
		}

		Page * pg=new Page(this,i);
		if(pg)
		{
//			memcpy(&(pg->m_Head),buf,sizeof(PageHead));
			pg->m_pos=i;
			m_FreePageList.push_back(pg);

			m_PagePool[i]=pg;
		}


	}

	m_Head.m_PageCount+=count;
	delete [] buf;
	::SetFilePointer(m_hd,0,0,FILE_BEGIN);
	if(!::WriteFile(m_hd,&m_Head,sizeof(PagePoolHead),&dwlen,NULL))
	{
		printf("Write head failed errorid:%d\n",::GetLastError());
		return false;
	}

	::SetFilePointer(m_hd,0,0,FILE_BEGIN);

	return true;
}
bool PagePool::AddNewPagesEx(int count)
{
	ost::MutexLock lock(m_Mutex);

	if(count<1)
		return false;
	if(m_fhd[m_Head.m_FileCount]==NULL)
		return false;


	LONG size=m_Head.m_PageSize * (m_fhd[m_Head.m_FileCount]->m_phead.m_PageCount+count)+sizeof(PagePoolHead);
	if((size>MAXINT-2)||(size<1))
	{
		printf("Size too big");
		return false;
	}
	size-=sizeof(PagePoolHead);

	if(size<=m_Head.m_PerFileSize)
	{
		ost::MutexLock ilock(m_IOMutex);

		m_hd=m_fhd[m_Head.m_FileCount]->m_hd;
		DWORD oldend=::SetFilePointer(m_hd,0,NULL,FILE_END);
		if(oldend==INVALID_SET_FILE_POINTER)
		{
			printf("Seek file failed errid:%d\n",::GetLastError());
			return false;
		}

		size=m_Head.m_PageSize*count;

		DWORD newpos=::SetFilePointer(m_hd,size,NULL,FILE_CURRENT);
		if(newpos==INVALID_SET_FILE_POINTER)
		{
			printf("Seek file failed2 errid:%d\n",::GetLastError());
			return false;

		}

		if(newpos!=m_Head.m_PageSize * (m_fhd[m_Head.m_FileCount]->m_phead.m_PageCount+count)+sizeof(PagePoolHead))
		{
			printf("Create file sapce failed newpos:%d\n",newpos);
			newpos=::SetFilePointer(m_hd,oldend,NULL,FILE_BEGIN);
			::SetEndOfFile(m_hd);
			return false;
		}

		::SetEndOfFile(m_hd);

		newpos=::SetFilePointer(m_hd,oldend,NULL,FILE_BEGIN);

		if(newpos==INVALID_SET_FILE_POINTER)
		{
			printf("Seek file failed3 errid:%d\n",::GetLastError());
			return false;

		}

		char *buf = new char[m_Head.m_PageSize];

		memset(buf,0,m_Head.m_PageSize);

		m_PagePool.resize(m_Head.m_TotalCount+count);
		for(int i=m_Head.m_TotalCount;i<m_Head.m_TotalCount+count;i++)
			m_PagePool[i]=NULL;

		DWORD dwlen=0;

		for(int i=m_fhd[m_Head.m_FileCount]->m_phead.m_PageCount;i<m_fhd[m_Head.m_FileCount]->m_phead.m_PageCount+count;i++)
		{
			((PageHead *)buf)->m_pos=i+m_fhd[m_Head.m_FileCount]->m_phead.m_BasePageIndex;
			((PageHead *)buf)->m_unused=1;
			F_SET(((PageHead *)buf)->m_flag,PAGEUNUSED);
			((PageHead *)buf)->m_datasize=m_Head.m_PageSize-sizeof(struct PageHead);
			((PageHead *)buf)->m_pagesize=m_Head.m_PageSize;
			((PageHead *)buf)->m_lastrecordpos=-1;
			((PageHead *)buf)->m_prerecordpage=-1;
			((PageHead *)buf)->m_nextrecordpage=-1;
			((PageHead *)buf)->m_mpos=i;
			((PageHead *)buf)->m_fileindex=m_fhd[m_Head.m_FileCount]->m_phead.m_FileIndex;



			if(!::WriteFile(m_hd,buf,m_Head.m_PageSize,&dwlen,NULL))
			{
				printf("Write file failed errorid:%d\n",::GetLastError());
				delete [] buf;
				::CloseHandle(m_hd);
				return false;
			}

			Page * pg=new Page(this,i+m_fhd[m_Head.m_FileCount]->m_phead.m_BasePageIndex);
			if(pg)
			{
				//			memcpy(&(pg->m_Head),buf,sizeof(PageHead));
				pg->m_pos=i+m_fhd[m_Head.m_FileCount]->m_phead.m_BasePageIndex;
				pg->m_mpos.m_fileindex=m_fhd[m_Head.m_FileCount]->m_phead.m_FileIndex;
				pg->m_mpos.m_pos=i;
				m_FreePageList.push_back(pg);

				m_PagePool[i+m_fhd[m_Head.m_FileCount]->m_phead.m_BasePageIndex]=pg;
			}


		}

		m_Head.m_TotalCount+=count;
		m_fhd[m_Head.m_FileCount]->m_phead.m_PageCount+=count;
		if(m_Head.m_FileCount==0)
			m_Head.m_PageCount+=count;
		delete [] buf;
		::SetFilePointer(m_hd,0,0,FILE_BEGIN);
		if(!::WriteFile(m_hd,&m_fhd[m_Head.m_FileCount]->m_phead,sizeof(PagePoolHead),&dwlen,NULL))
		{
			printf("Write head failed errorid-1:%d\n",::GetLastError());
			return false;
		}

		::SetFilePointer(m_hd,0,0,FILE_BEGIN);
		FlushFileHead(0);

	}else
	{
		int dlen=m_Head.m_PerFileSize-m_fhd[m_Head.m_FileCount]->m_phead.m_PageCount*m_Head.m_PageSize;
		int pcount=dlen/m_Head.m_PageSize;
        
		if(pcount>0)
		{
			if(!AddNewPagesEx(pcount))
				return false;
		}
		int freecount=count - pcount;
		char filepath[1024]={0};
		sprintf(filepath,"%s_%02d.db",m_FileTitle.c_str(),m_Head.m_FileCount+1);

		DWORD dwcd;
		dwcd=CREATE_ALWAYS;

		HANDLE hd=::CreateFile(filepath,GENERIC_WRITE|GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,dwcd,FILE_ATTRIBUTE_NORMAL,NULL);
		if(hd == INVALID_HANDLE_VALUE)
		{
			printf("Create file failed:%d\n",::GetLastError());
			return false;
		}


		SubPool *sp = new SubPool();
		if(sp==NULL)
			return false;

		sp->m_phead=m_Head;
		sp->m_phead.m_FileIndex=m_Head.m_FileCount+1;
		sp->m_phead.m_PageCount=0;
		sp->m_phead.m_BasePageIndex=m_fhd[m_Head.m_FileCount]->m_phead.m_BasePageIndex+m_fhd[m_Head.m_FileCount]->m_phead.m_PageCount;

		DWORD dwlen=0;


		if(!::WriteFile(hd,&sp->m_phead,sizeof(PagePoolHead),&dwlen,NULL))
		{
			printf("Write head failed errorid-2:%d\n",::GetLastError());
			return false;
		}

		::SetFilePointer(hd,0,0,FILE_BEGIN);
		m_Head.m_FileCount++;

		m_fhd.resize(m_Head.m_FileCount+1);
		sp->m_hd=hd;
		m_fhd[m_Head.m_FileCount]=sp;

		FlushFileHead(0);

		return 	AddNewPagesEx(freecount);
        		
	}

	return true;
}

void PagePool::PutPageSize(int PageCount,int PageSize)
{
	if((PageSize<MINPAGESIZE)||(PageCount<=0))
		return;
	m_Head.m_TotalCount=PageCount;
	m_Head.m_PageSize=PageSize;
}
void PagePool::GetPageSize(int &PageCount,int &PageSize)
{
	PageCount=m_Head.m_TotalCount;
	PageSize=m_Head.m_PageSize;
}

PagePool::PagePoolHead & PagePool::GetPagePoolHead()
{
	return m_Head;
}

void PagePool::FlushFileHead(int number)
{
	if(number<0||number>m_fhd.size())
		return ;
	ost::MutexLock lock(m_Mutex);

	ost::MutexLock ilock(m_IOMutex);


	HANDLE hd=m_fhd[number]->m_hd;
	DWORD dwlen=0;
	::SetFilePointer(hd,0,0,FILE_BEGIN);
	if(number==0)
	{
		if(!::WriteFile(hd,&m_Head,sizeof(PagePoolHead),&dwlen,NULL))
		{
			printf("Write head failed errorid-3:%d\n",::GetLastError());
			return ;
		}

	}else
	{
		if(!::WriteFile(hd,&m_fhd[number]->m_phead,sizeof(PagePoolHead),&dwlen,NULL))
		{
			printf("Write head failed errorid-4:%d\n",::GetLastError());
			return ;
		}

	}

	::SetFilePointer(hd,0,0,FILE_BEGIN);

}
void PagePool::FlushAllFileHead(void)
{
	for(int i=0;i<m_fhd.size();i++)
		FlushFileHead(i);
}
