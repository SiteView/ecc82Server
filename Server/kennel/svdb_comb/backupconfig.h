#ifndef  DRAGONFLOW_BACKUPCONFIG
#define DRAGONFLOW_BACKUPCONFIG


#include <svdbapi.h>
#include <SvdbMain.h>


class CBackupConfig :
	public ThreadEx
{
public:
	CBackupConfig(SvdbMain * pMain);
	virtual ~CBackupConfig();
	virtual void run(void);
	void toExit(void);

	bool CacheConfig(string type, string id);
	bool UpdataConfig(const char *data,S_UINT datalen);
	bool SendOutConfig();

private:
	bool m_monitor_need;
	bool m_entity_need;
	bool m_group_need;
	bool m_svse_need;
	ost::Mutex m_config_Lock;
	
	SvdbMain * m_pMain;
	bool m_exitAnyway;
};

#endif
