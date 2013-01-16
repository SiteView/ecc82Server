#ifndef	SVDB_POOLBASE_
#define	SVDB_POOLBASE_

#include "svdbtype.h"

#include <cc++/thread.h>
#include <string>
#include <map>
using std::map;
using std::string;

using namespace ost;


class PoolBase
{
public:
	PoolBase(void);
	 ~PoolBase(void);

	PoolBase(word filepath){ m_FilePath=filepath;}

	virtual bool Load()=0;
	virtual bool Submit()=0;
	void PutFilePath(word filepath){
		if(filepath==m_FilePath)
			return;
		m_FilePath=filepath;
		m_changed=true;
	}
	word GetFilePath(void){return m_FilePath;}

protected:
	word	m_FilePath;
	Mutex	m_UpdateLock;
	bool	m_loaded;
	bool	m_changed;

};


class PoolBase2
{
public:
	PoolBase2(void);
	 ~PoolBase2(void);

	PoolBase2(word filepath){ m_FilePath=filepath;}

	virtual bool Load()=0;
	virtual bool Submit(std::string modifyid="")=0;
	void PutFilePath(word filepath){
		if(filepath==m_FilePath)
			return;
		m_FilePath=filepath;
		m_changed=true;
	}
	word GetFilePath(void){return m_FilePath;}


	bool NewVersion(word id)
	{
		std::map<string,S_UINT>::iterator it=m_mversion.find( id.getword() );
		if( it==m_mversion.end() )
		{
			m_mversion.insert(std::make_pair(id.getword(),IdcUser::initial_time));
			return true;
		}

		S_UINT ver=it->second;
		++ver;
		if(ver<=10)
			ver=11;

		it->second= ver;
		return true;
	}

	S_UINT GetVersion(word id)
	{
		std::map<string,S_UINT>::iterator it=m_mversion.find( id.getword() );
		if( it==m_mversion.end() )
            return 0;

		S_UINT ver=it->second;
		if( ver<=10 )
			return 0;
		return ver;
	}

	bool NewVersion(S_UINT id)
	{
		std::map<S_UINT,S_UINT>::iterator it=m_mversion_int.find( id );
		if( it==m_mversion_int.end() )
		{
			m_mversion_int.insert(std::make_pair(id,IdcUser::initial_time));
			return true;
		}

		S_UINT ver=it->second;
		++ver;
		if(ver<=10)
			ver=11;

		it->second= ver;
		return true;
	}

	S_UINT GetVersion(S_UINT id)
	{
		std::map<S_UINT,S_UINT>::iterator it=m_mversion_int.find( id );
		if( it==m_mversion_int.end() )
            return 0;

		S_UINT ver=it->second;
		if( ver<=10 )
			return 0;
		return ver;
	}

protected:
	word	m_FilePath;
	Mutex	m_UpdateLock;
	bool	m_loaded;
	bool	m_changed;
	
	std::map<string,S_UINT> m_mversion;
	std::map<S_UINT,S_UINT> m_mversion_int;

};

#endif
