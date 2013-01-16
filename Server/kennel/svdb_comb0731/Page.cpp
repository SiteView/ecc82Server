#include "Page.h"
#include "PagePool.h"

Page::Page(void)
{
//	m_Head.m_pos=0;
//	m_Head.m_unused=1;
	m_data=NULL;
	m_ischange=false;
	m_isuse=false;
	m_rawdata=NULL;
	m_pos=NULL;

}

Page::Page(PagePool *ppt,int index)
{
	m_ppt=ppt;
//	m_Head.m_pos=0;
//	m_Head.m_unused=1;
	m_data=NULL;
	m_isuse=false;
	m_ischange=false;
	m_rawdata=NULL;
	m_pos=index;
}

Page::~Page(void)
{
	if(m_ischange)
		swapout(false);
	if(m_rawdata)
		delete [] m_rawdata;

}


/*bool Page::swapin()
{
	ost::MutexLock lock(m_ppt->m_IOMutex);

	if(m_data==NULL)
		m_data=new char[m_ppt->GetPagePoolHead().m_PageSize-sizeof(PageHead)];
	memset(m_data,0,m_ppt->GetPagePoolHead().m_PageSize-sizeof(PageHead));

	int pos=m_Head.m_pos*m_ppt->GetPagePoolHead().m_PageSize+sizeof(struct PagePool::PagePoolHead)+sizeof(PageHead);

	if(!::SetFilePointer(m_ppt->m_hd,pos,NULL,FILE_BEGIN))
	{
		printf("swapin failed move file pointor error errorid :%d\n",::GetLastError());
		return false;
	}

	DWORD size=0;

	if(!::ReadFile(m_ppt->m_hd,m_data,m_ppt->GetPagePoolHead().m_PageSize-sizeof(PageHead),&size,NULL))
	{
		printf("swapin failed read file failed:%d\n",::GetLastError());
		return false;
	}

	return true;

}
bool Page::swapout(bool isdel)
{
	ost::MutexLock lock(m_ppt->m_IOMutex);
	if(m_ischange)
	{
		int pos=m_Head.m_pos*m_ppt->GetPagePoolHead().m_PageSize+sizeof(struct PagePool::PagePoolHead);
		m_ischange=false;
		if(!::SetFilePointer(m_ppt->m_hd,pos,NULL,FILE_BEGIN))
			return false;
		DWORD size=0;
		if(!::WriteFile(m_ppt->m_hd,&m_Head,sizeof(PageHead),&size,NULL))
			return false;
		if(m_data!=NULL)
		{
			if(!::WriteFile(m_ppt->m_hd,m_data,m_ppt->GetPagePoolHead().m_PageSize-sizeof(PageHead),&size,NULL))
			{
				printf("swap(write) failed errorid:%d\n",::GetLastError());
				return false;
			}
		}
	}

	if(isdel)
		if(m_data!=NULL)
		{
			delete [] m_data;
			m_data=NULL;
		}

	return true;

}
*/

bool Page::swapin()
{
	ost::MutexLock lock(m_ppt->m_IOMutex);

	if(m_rawdata==NULL)
		m_rawdata=new char[m_ppt->GetPagePoolHead().m_PageSize];
	memset(m_rawdata,0,m_ppt->GetPagePoolHead().m_PageSize);

	int pos=m_pos*m_ppt->GetPagePoolHead().m_PageSize+sizeof(struct PagePool::PagePoolHead);

	if(!::SetFilePointer(m_ppt->m_hd,pos,NULL,FILE_BEGIN))
	{
		printf("swapin failed move file pointor error errorid :%d\n",::GetLastError());
		return false;
	}

	DWORD size=0;

	if(!::ReadFile(m_ppt->m_hd,m_rawdata,m_ppt->GetPagePoolHead().m_PageSize,&size,NULL))
	{
		printf("swapin failed read file failed:%d\n",::GetLastError());
		return false;
	}

	m_data=m_rawdata+sizeof(PageHead);

	return true;

}
bool Page::swapout(bool isdel)
{
	ost::MutexLock lock(m_ppt->m_IOMutex);
	if(m_ischange)
	{
		int pos=m_pos*m_ppt->GetPagePoolHead().m_PageSize+sizeof(struct PagePool::PagePoolHead);
		m_ischange=false;
		if(!::SetFilePointer(m_ppt->m_hd,pos,NULL,FILE_BEGIN))
			return false;
		DWORD size=0;
		if(m_rawdata!=NULL)
		{
			if(!::WriteFile(m_ppt->m_hd,m_rawdata,m_ppt->GetPagePoolHead().m_PageSize,&size,NULL))
			{
				printf("swap(write) failed errorid:%d\n",::GetLastError());
				return false;
			}
		}
	}

	if(isdel)
		if(m_rawdata!=NULL)
		{
			delete [] m_rawdata;
			m_data=NULL;
			m_rawdata=NULL;
		}

	return true;

}

bool Page::swapinex()
{
	ost::MutexLock lock(m_ppt->m_IOMutex);

	if(m_rawdata==NULL)
		m_rawdata=new char[m_ppt->GetPagePoolHead().m_PageSize];
	memset(m_rawdata,0,m_ppt->GetPagePoolHead().m_PageSize);
    int pos=m_mpos.m_pos*m_ppt->GetPagePoolHead().m_PageSize+sizeof(struct PagePool::PagePoolHead);

	if(!::SetFilePointer(m_ppt->m_fhd[m_mpos.m_fileindex]->m_hd,pos,NULL,FILE_BEGIN))
	{
		printf("swapin failed move file pointor error errorid :%d\n",::GetLastError());
		return false;
	}

	DWORD size=0;

	if(!::ReadFile(m_ppt->m_fhd[m_mpos.m_fileindex]->m_hd,m_rawdata,m_ppt->GetPagePoolHead().m_PageSize,&size,NULL))
	{
		printf("swapin failed read file failed:%d\n",::GetLastError());
		return false;
	}

	m_data=m_rawdata+sizeof(PageHead);

	return true;

}
bool Page::swapoutex(bool isdel)
{
	ost::MutexLock lock(m_ppt->m_IOMutex);
	if(m_ischange)
	{
		int pos=m_mpos.m_pos*m_ppt->GetPagePoolHead().m_PageSize+sizeof(struct PagePool::PagePoolHead);

		m_ischange=false;
		if(!::SetFilePointer(m_ppt->m_fhd[m_mpos.m_fileindex]->m_hd,pos,NULL,FILE_BEGIN))
		{
			puts("swap out -- Set file pointer failed");
			return false;
		}
		DWORD size=0;
		if(m_rawdata!=NULL)
		{
			if(!::WriteFile(m_ppt->m_fhd[m_mpos.m_fileindex]->m_hd,m_rawdata,m_ppt->GetPagePoolHead().m_PageSize,&size,NULL))
			{
				printf("swap(write) failed errorid:%d\n",::GetLastError());
				return false;
			}
		}
	}

	if(isdel)
		if(m_rawdata!=NULL)
		{
			delete [] m_rawdata;
			m_data=NULL;
			m_rawdata=NULL;
		}

	return true;
}
