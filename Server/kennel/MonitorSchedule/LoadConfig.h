//#pragma once
#ifndef  DRAGONFLOW_LOADCONFIG
#define DRAGONFLOW_LOADCONFIG


#include "MonitorSchedule.h"
#include "Monitor.h"
#include "Groups.h"
#include "Subsequent.h"

//#include "CXMLConfig.h"
#include "readSVDB.h"
#include "Option.h"



//////////////////////////////////////Parameter//////////////////////////////////////////////////

#define		__MACHINETYPE__					"sv_machinetype="
#define		__OSTYPE__						"sv_ostype="
#define		__MACHINENAME__					"sv_machinename="
#define		__USERACCOUNT__					"sv_useraccount="
#define		__PASSWORD__					"sv_password="
#define		__PROMPT__						"sv_prompt="
#define		__LOGINPROMPT__					"sv_loginprompt="
#define		__PASSWORDPROMPT__				"sv_pwprompt="
#define		__PROTOCOLTYPE__				"sv_protocoltype="
#define		__SERVERPORT__					"sv_serverport="
#define		__DOWNLOADFILE__				"sv_downloadfile="
#define		__TIMEOUT__						"sv_timeout="
#define		__PORT__						"sv_port="
#define		__SENDSTR__						"sv_sendstr="
#define		__MATCHSTR__					"sv_matchstr="
#define		__DSNNAME__						"sv_dsnname="
#define		__SQLSTR__						"sv_sqlstr="
#define		__REQUIREID__					"sv_requireid="
#define		__COMMUNITY__					"sv_community="
#define		__DOMAINNAME__					"sv_domname="
#define		__MATCHIP__						"sv_matchip="
#define		__DIRECTORY__					"sv_directory="
#define		__ISSUBDIR__					"sv_issubdir="
#define		__FILEPATH__					"sv_filepath="
#define		__SERVICE__						"sv_service="
#define		__DISK__						"sv_disk="
#define		__SERVERMANURL__				"sv_servermanurl="
#define		__SENDBYTES__					"sv_sendbytes="
#define		__CPUNUM__						"sv_cpunum="
#define		__DETAIL__						"sv_detail="
#define		__TRACE__						"sv_trace="

#define		__PROXYSERVERPORT__				"sv_proxyserverport="
#define		__PROXYUSERNAME__				"sv_proxyuser="
#define		__PROXYPASSWORD__				"sv_proxypass="
#define		__RETRYDELAY__					"sv_retrydelay="
#define		__CONNECTTIMEOUT__				"sv_conntimeout="
#define		__QUERYTIMEOUT__				"sv_querytimeout="
#define		__PASSIVEMODE__					"sv_passivemode="
#define		__PROVIDER__					"sv_provider="
#define		__PRINCIPAL__					"sv_principal="
#define		__CREDENTIALS__					"sv_credentials="
#define		__QUERY__						"sv_query="
#define		__FILTER__						"sv_filter="
#define		__SMTPSERVERPORT__				"sv_smtpservport="
#define		__RCPTSERVERPORT__				"sv_rcptservport="
#define		__MAILTYPE__					"sv_mailtype="
#define		__MAILPROTOCOLTYPE__			"sv_mailprotocoltype="
#define		__FROMADDRESS__					"sv_sendaddress="
#define		__RCPTADDRESS__					"sv_rcptaddress="
#define		__CHECKDELAY__					"sv_checkdelay="
#define		__ATTACHMENT__					"sv_attachment="
#define		__SECRET__						"sv_secret="
#define		__INDEX__						"sv_index="
#define		__INTERFACEINDEX__				"sv_interfaceindex="
#define		__GROUP__						"sv_group="
#define		__CONNECTADDRESS__				"sv_connaddr="
#define		__URL__							"sv_url="
#define		__STOPTIME__					"sv_stoptime="
#define		__PROXYTYPE__					"sv_proxytype="
#define		__POSTDATA__					"sv_postdata="
#define		__ENABLEPASSWORD__				"sv_enablepassword="
#define		__ENABLEPASSWORDPROMPT__		"sv_enablepasswordprompt="
#define		__ENABLEPROMPT__				"sv_enalbeprompt="
#define		__VERIFYUSERNAME__				"sv_verifyusername="
#define		__VERIFYPASSWORD__				"sv_verifypassword="
#define		__REQUESTADDRESS__				"sv_requestaddress="
#define		__MQQUEUEMANAGER__				"sv_mqqueuemanager="
#define		__MQQUEUENAME__					"sv_mqqueuename="
#define		__INSTANCE__					"sv_instance="
#define		__ALERTTYPE__					"sv_alerttype="
#define		__LOGFILE__						"sv_logfilepathname="
#define		__RULEFILE__					"sv_rulefile="
#define		__CUSTOMERPATH__				"sv_customerpath="
#define		__APPENDMETHOD__				"sv_appendmethod="
#define		__GTJAID__						"sv_id="
#define		__PARAMETERS__					"sv_parameters="
#define		__SCRIPT__						"sv_script="
#define		__SERVERNAME__					"sv_servername="
#define		__NEEDAUTH__					"sv_issmtpau="
#define		__VPNCONNECTION__				"sv_vpnconn="
#define		__DIALBEFOREVPN__				"sv_dialbeforevpn="
#define		__DIALCONNECTION__				"sv_dialconn="
#define		__USERACCOUNTD__				"sv_useraccountd="
#define		__PASSWORDD__					"sv_passwordd="
#define		__DOMAINNAMED__					"sv_domnamed="
#define		__CLUSTERLOG__					"sv_clusterlogpath="
#define		__RUNNING_CONFIG__				"sv_runcmd="

#define		__GROUPID__						"sv_groupid="  
#define		__MONITORID__					"sv_monitorid="

#define		__TPLID__						"_TemplateID="
#define		__SERVICENAME__					"sv_servicename="

////////////////////////////////////////////////////////////////////////////////////////////////////////////

class LoadConfig
{
public:
	BOOL CreateSingleGroup(GroupsItem *pItem,string strGroupID);  //创建一个group 传回3个参数
	BOOL CreateSingleEntity(Entity *pE,string strEntityID);		//创建一个 entity 传回5个参数
	BOOL CreateSingleMonitor(Monitor *pM,string strMonitorID);		//创建一个 monitor 传回 para, return, stacon 等
	void ReleaseAll(void);
	BOOL CreateSubsequent(Subsequent*psub);
	BOOL CreateGroups(Groups *pGroups);		  			//创建所有的 group
	BOOL CreateMonitors(CMonitorList &MonitorList);		//创建所有的 monitor
	bool LoadTaskPlan(TASKMAP&tmap);
	BOOL LoadAll(void);
	LoadConfig();
	bool ClearBuffer();
	virtual ~LoadConfig();

	Option *m_pOption;

//private:
	void LoadSubsequentList(CSubsequentItemList &lstsub);
	void LoadGroups(CGroupsItemList &lstGroups);		//根据传入的 groups 的 list ,load 每一个 group
	void LoadSingleEntity(Entity*pEntity,string strEntityID);
	void LoadEntitys(CEntityList &EntityList);
	BOOL GetStateCondition(Monitor *pM,string strMonitorID);
	BOOL GetReturnList(Monitor*pM,int MonitorType);
	BOOL LoadCommonEntity(string strEntityID,list <string> & lstParam);
	BOOL LoadUnixEntity(string strEntityID, list <string> & lstParam);
	BOOL LoadWindowsEntity(string strEntityID,list <string> & lstParam);
	BOOL GetParamList(Monitor *pM,string strMonitorID, string strParentID);
	BOOL GetMonitorPro(Monitor *pM, string MonitorID);
	BOOL m_isLoad;
	//CXMLConfig* m_Monitor;
	//CXMLConfig* m_Entity;
	//CXMLConfig* m_MonitorLibrary;
	//CXMLConfig* m_Subsequent;
	//CXMLConfig* m_Group;

	readSVDB* m_Monitor;
	readSVDB* m_Entity;
	readSVDB* m_MonitorLibrary;
	readSVDB* m_Subsequent;
	readSVDB* m_Group; 


};

#endif
