#ifndef SVDB_OPTION_
#define SVDB_OPTION_
#include "svdbtype.h"


class Option
{
public:
	Option(void)
	{
		m_inifilecount=5;
		m_defaultlanguage="chinese";
		m_rootpath="d:\\v70\\data\\main";
		m_inimaxidletime=1;
		m_logkeepdays=100;
		m_enablealert=true;
		m_AlertQueueName="SiteView70-Alert";
		m_ConfigTrackQueueName="SiteView70-ConfigTrack";
		m_enableconfigtrack=true;
		m_trackinifiles=",alert.ini,smsconfig.ini,";
		m_enableinifiletrack=false;
		m_IniFileTrackQueueName="SiteView70-IniFileTrack";
		m_ResidentIniFile=",oscmd.ini,";
		m_dbPerFileSize=0;
		m_dbPageSize=0;
		m_waitMode=WAITMODE_FIX;
		m_listenport=8600;
		m_enabledbrecordtrack=false;
		m_DBRecordTrackQueueName="SiteView70-DBRecordTrack";
		m_enableIDC=false;
	}
	~Option(void)
	{
	}

	enum{
		WAITMODE_FIX=1,
		WAITMODE_VAR=2
	};

	bool LoadOption();

	std::string	m_defaultlanguage;
	std::string	m_rootpath;
	std::string m_AlertQueueName;
	std::string m_ConfigTrackQueueName;
	std::string m_trackinifiles;
	std::string m_IniFileTrackQueueName;
	std::string m_ResidentIniFile;
	std::string m_DBRecordTrackQueueName;

	int			m_inifilecount;
	int			m_inimaxidletime;
	int			m_logkeepdays;
	bool		m_enablealert;
	bool		m_enableconfigtrack;
	bool		m_enableinifiletrack;
	bool		m_enabledbrecordtrack;
	bool		m_enableIDC;

	int			m_dbPerFileSize;
	int			m_dbPageSize;

	int 		m_waitMode;
	unsigned	m_listenport;

};

#endif
