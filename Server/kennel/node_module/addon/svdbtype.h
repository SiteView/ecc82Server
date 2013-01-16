#ifndef		SVDB_SVDBTYPE_H_
#define		SVDB_SVDBTYPE_H_


#ifdef	WIN32
//#include "windows.h"
//#define  strdup  _strdup
#else

typedef		unsigned long	DWORD;
typedef		true			TRUE;
typedef		false			FALSE;

#endif

typedef		unsigned int	S_UINT;

#ifndef	NULL
#define	NULL	0
#endif

typedef	int BOOL;

#include "../libutil/strkey.h"

#include <list>

using namespace svutil;

typedef std::list<svutil::word>	WORDLIST;

#define	IDSEPARATOR		'.'

/////////////////db////////////

#define UNITLEN		12
#define	NAMELEN		50
#define LABELLEN	100

#define MAXINT		2147483647
#define UMAXINT		4294967295

#define MINPAGESIZE 1024
#define MAXPAGESIZE	(1024*24)
#define PAGESIZE	4096
#define HEADPAGESIZE	2048

#define MAXTABLE	100000

#define PERFILESIZE	2097152000
// 2GB



#define CROSSPAGEUP		0x00000001
#define CROSSPAGEDOWN	0x00000002
#define PAGEUNUSED		0x00000004
#define BADPAGE			0x00000008
#define PAGEFULL		0x00000010

#define F_SET(x,f) (x) |= (f)
#define F_CLR(x,f) (x) &= ~(f)
#define F_ISSET(x,f) ((x) & (f))

#define QBUFFERLEN		1048576


#include "SVDBError.h"

#include <cc++/file.h>
#include <string>
#include <set>
#include <map>
using std::string;

class IdcUser
{
public:
	static std::set<std::string> Users;
	static bool EnableConfigDB;
	static bool EnableIdc;
	static bool AutoResolveIDS;
	static bool BoolToExit;
	static std::string RootPath;
	static std::string svdbHostAddr;
	static std::string cgiVersion;
	static int SocketTreadPlusCount;
	static int PipeTreadPlusCount;
	static void *m_pResource;
	static void *m_pLanguage;
	static S_UINT initial_time;
	static std::string ProcessID;
	
	static bool DisableEntityCombine;
	static bool nnmEntityParentOk;
	static bool PreCreateNnmEntityParent;
	static std::map<string,string> nnmEntityParentId;
	static std::map<string,string> nnmEntityParentValue;
	static std::string nnmEntityParentKey;

	static std::string StrDisable;
	static std::string StrTempDisable;
	
	std::string CreatChineseIDS(const std::string key, const std::string value, void *m_pLanguage);

	static std::set<int> set_InitLoadSEId;
	static std::string CenterAdress;
	static bool SELocked;
	static std::string CacheQueueName;
	static int msBackup;
	static bool RecordsAutoBackup;
	static bool ConfigAutoBackup;
	static bool AcceptConfigIncoming;
	static bool FullTeleBackup;
	static bool WillTeleBackup(const std::string id);
	static bool SetTeleBackupId(std::set<std::string> newid);

	static void PutLocalSEId(std::string str);
	static std::set<int> GetLocalSEId();
	static std::string GetLocalSEIdStr();
	static std::string GetFirstSEId();
	static bool IsAnLocalSEId(int index);
	static bool IsAnLocalSEId(std::string str);

	bool CreatDir(std::string fname)		
	{
		ost::MutexLock lock(m_UpdateLockCreatDir);
		bool ret;
		try{
			ret= ost::Dir::create(fname.c_str());
		}
		catch(...)
		{
			printf("Exception to creat directory:");
			printf(fname.c_str());
			printf("\n");
		}
		return ret;
	}

	static bool CreatDirNonThreadSafe(std::string fname) 		
	{
		bool ret;
		try{
			ret= ost::Dir::create(fname.c_str());
		}
		catch(...)
		{
			printf("Exception to creat directory:");
			printf(fname.c_str());
			printf("\n");
		}
		return ret;
	}

private:
	ost::Mutex	m_UpdateLockCreatDir;
	static std::set<int> set_LocalSEId;

	static std::set<std::string> set_backupId;
	static ost::Mutex	m_LockBackupId;

};


#endif