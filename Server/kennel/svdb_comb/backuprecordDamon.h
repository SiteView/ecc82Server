#ifndef  DRAGONFLOW_BACKUPRECORDDAMON
#define DRAGONFLOW_BACKUPRECORDDAMON

#include "threadex.h"
#include "backuprecord.h"

class CBackupRecord;

class CBackupRecordDamon :
	public ThreadEx
{
public:
	CBackupRecordDamon();
	virtual ~CBackupRecordDamon();
	virtual void run(void);
	void toExit(void);

	void setBkpRcdThread(CBackupRecord * pBkpRcdThread);
	void setVisit(std::string key, time_t time);

private:
	bool MoveOneStep(bool willExit= false);
	CBackupRecord * m_pBkpRcdThread;

	std::map<string, time_t> m_data;
	time_t m_latest_time;
	bool m_exitAnyway;
	ost::Mutex m_data_Lock;


};

#endif
