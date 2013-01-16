#ifndef	SVDB_PAGEPOOL_
#define	SVDB_PAGEPOOL_

#include "svdbtype.h"
#include <vector>
#include "libutil/time.h"
#include <cc++/thread.h>
#ifdef WIN32
#include <windows.h>
#endif

#include "SvdbMain.h"

#include "Page.h"

using namespace std;

struct PageFileHead{
	int m_version;
	int m_pagecount;

};


class PagePool
{
public:
	struct PagePoolHead{
		PagePoolHead()
		{
			m_version=1;
			m_PageCount=0;
			m_PageSize=0;
			m_FileCount=0;
			m_PerFileSize=0;
			m_TotalCount=0;
			m_FileIndex=0;
			m_BasePageIndex=0;
			memset(m_span,0,480);
		}
		~PagePoolHead()
		{
			;
		}
		PagePoolHead(const PagePoolHead &pph)
		{
			m_version=pph.m_version;
			m_PageCount=pph.m_PageCount;
			m_PageSize=pph.m_PageSize;
			m_FileCount=pph.m_FileCount;
			m_PerFileSize=pph.m_PerFileSize;
			m_TotalCount=pph.m_TotalCount;
			m_FileIndex=pph.m_FileIndex;
			m_BasePageIndex=pph.m_BasePageIndex;
			memmove(m_span,pph.m_span,480);
		}
		PagePoolHead & operator=(const PagePoolHead &pph)
		{
			m_version=pph.m_version;
			m_PageCount=pph.m_PageCount;
			m_PageSize=pph.m_PageSize;
			m_FileCount=pph.m_FileCount;
			m_PerFileSize=pph.m_PerFileSize;
			m_TotalCount=pph.m_TotalCount;
			m_FileIndex=pph.m_FileIndex;
			m_BasePageIndex=pph.m_BasePageIndex;
			memmove(m_span,pph.m_span,480);
			return *this;
		}
		int m_version;
		int m_PageCount;
		int	m_PageSize;
		int m_FileCount;
		int	m_PerFileSize;
		int m_TotalCount;
		int m_FileIndex;
		int m_BasePageIndex;
		char m_span[480];
	};

	struct SubPool{
		SubPool()
		{
			m_hd=NULL;
		}
		~SubPool()
		{
			if(m_hd!=NULL)
				::CloseHandle(m_hd);
		}
		PagePoolHead m_phead;
		HANDLE		 m_hd;
	};

	PagePool(void);
	PagePool(int pagesize,int pagecount);
	~PagePool(void);

    //////method///

	Page *GetFree(void);
	Page *GetFreeEx(void);
	bool PutFree(Page *pt);


	bool RemoveFromFreePages(int pos);

	Page *Get(int pos);


	Page *Get(int pos,bool readonly);

	bool FlushBuffer();

	bool CreatePagePool(int PageCount,string FileTitle,int PerFileSize,int PageSize=PAGESIZE)
	{
		if(PageCount<=0)
			return false;
		if(FileTitle.empty())
			return false;

		if((PageSize<=0)||(PageSize<MINPAGESIZE)||(PageSize>MAXPAGESIZE))
			PageSize=PAGESIZE;
		if((PageSize%512)!=0)
			PageSize=PAGESIZE;
      

		m_Head.m_TotalCount=PageCount;
		m_Head.m_BasePageIndex=0;
		m_Head.m_FileCount=0;
		m_FileTitle=FileTitle;
		m_Head.m_PageSize=PageSize;
		PutPerFileSize(PerFileSize);
		if(!InitPoolEx())
			return false;
		return CreatePageEx(m_FileTitle.c_str(),0,PageCount,0,true);
	}


	bool Put(Page *pt,bool isch,bool isdel);

	bool LoadPage(const char *filepath);
	bool LoadPageEx(const char *filetitle);

	bool CreatePage(const char * filepath,bool overlay);
	bool CreatePageEx(const char * filetitle,int fileindex,int pagecount,int pageindex,bool overlay);

	bool AddNewPages(int count);
	bool AddNewPagesEx(int count);

	void PutPageSize(int PageCount,int PageSize);
	void GetPageSize(int &PageCount,int &PageSize);


	void PutPerFileSize(int PerFileLen)
	{
		if((PerFileLen<m_Head.m_PageSize+sizeof(PagePoolHead))||(PerFileLen>MAXINT-2))
			PerFileLen=PERFILESIZE;
		int p=PerFileLen/m_Head.m_PageSize;
		
		m_Head.m_PerFileSize=p*m_Head.m_PageSize;
	}

	PagePoolHead &GetPagePoolHead();

    S_UINT GetFreePageCount()
	{
		return (S_UINT)m_FreePageList.size();
	}


protected:
	ost::Mutex m_Mutex;
	ost::Mutex m_IOMutex;
	void InitPool();
	bool InitPoolEx();
	void FlushFileHead(int number);
	void FlushAllFileHead(void);
	HANDLE m_hd;
	std::vector<struct SubPool *> m_fhd;



	string m_FileTitle;
	std::vector<Page *> m_PagePool;
	std::list<Page *> m_FreePageList;

	Page *m_LastPage;
	struct PagePoolHead m_Head;

	friend class Page;

};

#endif
