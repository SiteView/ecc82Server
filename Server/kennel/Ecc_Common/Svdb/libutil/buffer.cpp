#include "buffer.h"
#include "exception.h"


using namespace svutil;

buffer::buffer(void)
{
	m_length=0;
	m_pbuf=NULL;
}

buffer::~buffer(void)
{
	if(m_pbuf)
		delete [] m_pbuf;
}

buffer::buffer(const buffer &buf)
{

	char *p=new char[buf.m_length];
	if(p)
	{
		m_length=buf.m_length;
		m_pbuf=p;
		memset(m_pbuf,0,buf.m_length);
		memmove(m_pbuf,buf.m_pbuf,m_length);
	}else
		throw exception("Allocate memory failed");


}
buffer::buffer(S_UINT size)
{
	if(size>maxsize)
		throw exception("Size too long");;
	char *p=new char[size];
	if(p)
	{
		m_length=size;
		m_pbuf=p;
		memset(m_pbuf,0,m_length);
	}else
		throw exception("Allocate memory failed");
}


bool buffer::checksize(S_UINT length){
	if(m_length>=length)
	{
		memset(m_pbuf,0,length);
		return true;
	}
	if(length>maxsize)
		return false;

	char *p=new char[length];
	if(p==NULL)
		return false;

	if(m_pbuf!=NULL)
		delete [] m_pbuf;

	m_pbuf=p;
	m_length=length;
	memset(m_pbuf,0,m_length);
	return true;
}
void buffer::clearbuffer()
{
	if(m_pbuf!=NULL)
		delete [] m_pbuf;
	m_pbuf=NULL;
	m_length=0;
}

bool buffer::resetsize(S_UINT length)
{
	if(length>maxsize)
		return false;
	if(length<=m_length)
		return true;
	char *p=new char[length];
	if(p==NULL)
		return false;
	memset(p,0,length);
	if(m_pbuf!=NULL)
	{
		memmove(p,m_pbuf,m_length);
		delete [] m_pbuf;
	}
	m_pbuf=p;
	m_length=length;
	return true;
}

buffer & buffer::operator=(const buffer &buf)
{
	char *p=new char[buf.m_length];
	if(p!=NULL)
	{
		clearbuffer();
		m_pbuf=p;
		memmove(m_pbuf,buf.m_pbuf,buf.m_length);
		m_length=buf.m_length;
	}
	return *this;
}

char &buffer::operator[](S_UINT index)
{
	if(index>maxsize)
		throw this;
	if(index>=m_length)
	{
		if(!resetsize(index+1))
			throw exception("Resetsize memory failed");;
		return m_pbuf[index];
	}

	return m_pbuf[index];
}




