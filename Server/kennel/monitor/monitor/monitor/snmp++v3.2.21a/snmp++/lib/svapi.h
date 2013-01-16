/*************************************************
*  @file svapi.h
*  author :		chen xin gang
*  Copyright (C) 2005-2006 dragonflow corp.
*
*************************************************/


#ifndef	SITEVIEW_SVAPI_
#define	SITEVIEW_SVAPI_

#include <iostream>
#include <String.h>
#include <list>

using namespace std;


#ifdef	WIN32
#ifdef SVAPI_EXPORTS
#define SVAPI_API __declspec(dllexport)
#else
#define SVAPI_API __declspec(dllimport)
#endif
#else
#define	SVAPI_API	extern "C"
#endif


typedef unsigned long	OBJECT;
typedef unsigned long	MAPNODE;


#define	INVALID_VALUE	0

struct sv_list{
	sv_list()
	{
		list=INVALID_VALUE;
		iterator=INVALID_VALUE;
	}
	sv_list(const struct sv_list &item)
	{
		list=item.list;
		iterator=item.iterator;
	}

	const struct sv_list &operator=(const struct sv_list &item)
	{
		list=item.list;
		iterator=item.iterator;

		return *this;
	}

	OBJECT list;
	OBJECT iterator;

};

typedef struct sv_list LISTITEM;

struct sv_pair{
	sv_pair(){;}
	sv_pair(const struct sv_pair &pi)
	{
		name=pi.name;
		value=pi.value;
	}
	~sv_pair(){;}
	struct sv_pair& operator=(const struct sv_pair &pi)
	{
		name=pi.name;
		value=pi.value;
		return *this;
	}
	string name;
	string value;
};

typedef struct sv_pair SVPAIR;

typedef std::list<struct sv_pair> PAIRLIST;



/////////////////////////////General API///////////////////////////////////////////

/*
 MAPNODE 
 */
SVAPI_API 
bool	EnumNodeAttrib(MAPNODE node,PAIRLIST &retlist);

SVAPI_API 
unsigned int GetNodeSize(MAPNODE node);


SVAPI_API 
bool	FindNodeValue(MAPNODE node,string name,string &value);

SVAPI_API 
bool	DeleteNodeAttrib(MAPNODE node,string name);

SVAPI_API 
bool	AddNodeAttrib(MAPNODE node,const SVPAIR &data);

SVAPI_API 
bool	AddNodeAttrib(MAPNODE node,string name,string value);

SVAPI_API 
MAPNODE	CreateNewMapNode();

SVAPI_API
MAPNODE CloneMapNode(MAPNODE node);

SVAPI_API 
void	CloseMapNodeObject(OBJECT obj);


/*
LISTITEM
*/

SVAPI_API 
MAPNODE FindNext(const LISTITEM &item);

SVAPI_API 
unsigned int GetListItemSize(const LISTITEM &item);

SVAPI_API 
bool	DeleteItem(const LISTITEM &item);

SVAPI_API 
MAPNODE	GetItemNode(const LISTITEM &item);

SVAPI_API 
bool	ResetListItem(const LISTITEM &item);

SVAPI_API 
bool	AddItemToList(const LISTITEM &listi,const MAPNODE item);

SVAPI_API
bool	InsertItemToList(const LISTITEM &listi,const MAPNODE item);

SVAPI_API
void	ReleaseItemList(const LISTITEM &listi);

SVAPI_API
bool	CreateNewItemList(LISTITEM&listi);

/*
OBJECT 以下两个函数只作测试用
*/



/*
通用相关
*/

SVAPI_API 
string	GetSiteViewRootPath();

SVAPI_API
string	FindParentID(string id);

SVAPI_API
int	FindIndexByID(string id);

SVAPI_API
bool IsSVSEID(string id);

SVAPI_API
string FindSEID(string id);


/*

Ini file 相关

*/

SVAPI_API
bool WriteIniFileString(string section,string key,string str,string filename,string addr="localhost");

SVAPI_API
bool WriteIniFileInt(string section,string key,int value,string filename,string addr="localhost");

SVAPI_API
bool WriteIniFileStruct(string section,string key, void *data,unsigned int len,string filename,string addr="localhost");

SVAPI_API
string GetIniFileString(string section,string key,string defaultret,string filename,string addr="localhost");

SVAPI_API
int	GetIniFileInt(string section,string key,int defaultret,string filename,string addr="localhost");

SVAPI_API
bool GetIniFileStruct(string section,string key,void *buf,unsigned int &len,string filename,string addr="localhost");

SVAPI_API
bool DeleteIniFileSection(string section,string filename,string addr="localhost");

SVAPI_API
bool DeleteIniFileKey(string section,string key,string filename,string addr="localhost");

SVAPI_API
bool GetIniFileSections(std::list<string> &sectionlist,string filename,string addr="localhost");

SVAPI_API
bool GetIniFileKeys(string section,std::list<string> &keylist,string filename,string addr="localhost");

SVAPI_API
bool EditIniFileSection(string oldsection,string newsection,string filename,string addr="localhost");

SVAPI_API
bool EditIniFileKey(string section,string oldkey,string newkey,string filename,string addr="localhost");

SVAPI_API
int	GetIniFileValueType(string section,string key,string filename,string addr="localhost");

/*
  下面是GetIniFileValueType可能返回的值
*/

#define INIVALUE_FAILED	-1			//获取失败
#define INIVALUE_NULL	0			//未知类型
#define INIVALUE_INT	1			//整型数据
#define	INIVALUE_STRING	2			//字符串型数据
#define INIVALUE_BINARY	3			//二进类型数据




//////////////////////////////////SVSE API///////////////////////////////////////////

SVAPI_API 
OBJECT	GetSVSE(string id,string user="default",string addr="localhost");

SVAPI_API
OBJECT	CreateSVSE(string label);

SVAPI_API	
string GetSVSELabel(OBJECT svseobj);

SVAPI_API
bool PutSVSELabel(OBJECT svseobj,string label);

SVAPI_API
bool GetSubGroupsIDBySE(OBJECT svseobj,std::list<string> &idlist);

SVAPI_API
bool GetSubGroupsIDBySEEx(OBJECT svseobj,std::list<char *> &idlist);

SVAPI_API
bool GetSubEntitysIDBySE(OBJECT svseobj,std::list<string> &idlist);

SVAPI_API
bool DeleteSVSE(string id,string user="default",string addr="localhost");

SVAPI_API
bool CloseSVSE(OBJECT svseobj);

SVAPI_API
string AddNewSVSE(OBJECT svseobj,string user="default",string addr="localhost");

SVAPI_API
bool SubmitSVSE(OBJECT svseobj,string user="default",string addr="localhost");

SVAPI_API 
bool GetAllSVSEInfo(PAIRLIST &retlist,string user="default",string addr="localhost");



///////////////////////////Monitor templet API///////////////////////////////////////

SVAPI_API 
OBJECT	GetMonitorTemplet(int id,string user="default",string addr="localhost");

SVAPI_API 
MAPNODE	GetMTMainAttribNode(OBJECT mtobj);

SVAPI_API 
bool	FindMTParameterFirst(OBJECT mtobj,LISTITEM &item);

SVAPI_API 
bool	FindMTAdvanceParameterFirst(OBJECT mtobj,LISTITEM &item);

SVAPI_API 
bool	FindMTReturnFirst(OBJECT mtobj,LISTITEM &item);

SVAPI_API 
MAPNODE	GetMTErrorAlertCondition(OBJECT mtobj);

SVAPI_API 
MAPNODE GetMTWarningAlertCondition(OBJECT mtobj);

SVAPI_API 
MAPNODE	GetMTGoodAlertCondition(OBJECT mtobj);

SVAPI_API
OBJECT CreateMonitorTemplet(int id);

SVAPI_API
bool DeleteMonitorTemplet(int id,string user="default",string addr="localhost");

/* //以下三个函数没用
SVAPI_API 
bool	AddMTParameterItem(LISTITEM &item);

SVAPI_API 
bool	AddAdvanceParameterItem(LISTITEM &item);

SVAPI_API 
bool	AddAlertConditionItem(LISTITEM &item);
*/

SVAPI_API
void CloseMonitorTemplet(OBJECT obj);


SVAPI_API 
bool	SubmitMonitorTemplet(OBJECT mtobj,string user="default",string addr="localhost");

SVAPI_API 
bool GetAllMonitorTempletInfo(PAIRLIST &retlist,string infoname="sv_label",string user="default",string addr="localhost");

SVAPI_API
OBJECT CloneMonitorTemplet(OBJECT mtobj,int targetid);

/////////////////////////////Resource///////////////////////////////////////////////

SVAPI_API 
OBJECT LoadResource(string language="default",string addr="localhost");

SVAPI_API
OBJECT CreateResource(string language);

SVAPI_API 
MAPNODE GetResourceNode(OBJECT rcobj);

SVAPI_API 
bool	EnumResourceAttrib(OBJECT rcobj,PAIRLIST &retlist);

SVAPI_API
bool	CloseResource(OBJECT rcobj);

SVAPI_API
bool	DeleteResource(string language,string addr="localhost");

SVAPI_API 
bool	SubmitResource(OBJECT rcobj,string addr="localhost");

SVAPI_API 
bool GetAllResourceInfo(PAIRLIST &retlist,string addr="localhost");


/////////////////////////////Monitor///////////////////////////////////////////////

SVAPI_API
OBJECT	GetMonitor(string monitorid,string user="default",string addr="localhost");

SVAPI_API
OBJECT CreateMonitor(void);

SVAPI_API
MAPNODE GetMonitorMainAttribNode(OBJECT monitorobj);

SVAPI_API
MAPNODE GetMonitorParameter(OBJECT monitorobj);

SVAPI_API
MAPNODE GetMonitorAdvanceParameterNode(OBJECT monitorobj);

SVAPI_API 
MAPNODE	GetMonitorErrorAlertCondition(OBJECT monitorobj);

SVAPI_API 
MAPNODE GetMonitorWarningAlertCondition(OBJECT monitorobj);

SVAPI_API 
MAPNODE	GetMonitorGoodAlertCondition(OBJECT monitorobj);

SVAPI_API
bool DeleteSVMonitor(string monitorid,string user="default",string addr="localhost");

SVAPI_API
void CloseMonitor(OBJECT monitorobj);

SVAPI_API
string AddNewMonitor(OBJECT monitorobj,string entityid,string user="default",string addr="localhost");

SVAPI_API 
bool	SubmitMonitor(OBJECT monitorobj,string user="default",string addr="localhost");

SVAPI_API 
bool GetAllMonitorsInfo(PAIRLIST &retlist,string infoname="sv_label",string user="default",string addr="localhost");


///////////////////////////////Entity///////////////////////////////////////////////

SVAPI_API
OBJECT	GetEntity(string entityid,string user="default",string addr="localhost");

SVAPI_API
OBJECT CreateEntity();

SVAPI_API
MAPNODE GetEntityMainAttribNode(OBJECT entityobj);

SVAPI_API
bool GetSubMonitorsIDByEntity(OBJECT entityobj,std::list<string> &monitoridlist);

/*
SVAPI_API
string GetEntityNextMonitorID(OBJECT entityobj);
*/

SVAPI_API
bool DeleteEntity(string entityid,string user="default",string addr="localhost");

SVAPI_API
string AddNewEntity(OBJECT entityobj,string groupid="1",string user="default",string addr="localhost");

SVAPI_API
bool CloseEntity(OBJECT entityobj);

SVAPI_API
bool SubmitEntity(OBJECT entityobj,string user="default",string addr="localhost");

SVAPI_API 
bool GetAllEntitysInfo(PAIRLIST &retlist,string infoname="sv_label",string user="default",string addr="localhost");


//////////////////////////////Group////////////////////////////////////////////////

SVAPI_API
OBJECT	GetGroup(string groupid,string user="default",string addr="localhost");

SVAPI_API
OBJECT CreateGroup(void);

SVAPI_API
MAPNODE GetGroupMainAttribNode(OBJECT groupobj);

SVAPI_API
bool GetSubGroupsIDByGroup(OBJECT groupobj,std::list<string> &idlist);

SVAPI_API
bool GetSubGroupsIDByGroupEx(OBJECT groupobj,std::list<char *> &idlist);


SVAPI_API
bool GetSubEntitysIDByGroup(OBJECT groupobj,std::list<string> &idlist);



SVAPI_API
bool DeleteGroup(string groupid,string user="default",string addr="localhost");

/*

pid 为1代表为父id是为1的se

*/
SVAPI_API
string AddNewGroup(OBJECT groupobj,string pid="1",string user="default",string addr="localhost");

SVAPI_API
bool CloseGroup(OBJECT groupobj);

SVAPI_API
bool SubmitGroup(OBJECT groupobj,string user="default",string addr="localhost");

SVAPI_API 
bool GetAllGroupsInfo(PAIRLIST &retlist,string infoname="sv_label",string user="default",string addr="localhost");

/////////////////////////////////Task///////////////////////////////////////////////

SVAPI_API
OBJECT GetTask(string taskname,string user="default",string addr="localhost");

SVAPI_API
OBJECT CreateTask(string taskname);

SVAPI_API
string GetTaskValue(string key,OBJECT taskobj);

SVAPI_API
bool  SetTaskValue(string key,string value,OBJECT taskobj);

SVAPI_API
bool  DeleteTaskKey(string key,OBJECT taskobj);

SVAPI_API
bool  DeleteTask(string taskname,string user="default",string addr="localhost");

SVAPI_API
bool GetAllTaskName(std::list<string> &tasknamelist,string user="default",string addr="localhost");

SVAPI_API
bool SubmitTask(OBJECT taskobj,string user="default",string addr="localhost");

SVAPI_API
void CloseTask(OBJECT taskobj);

SVAPI_API
bool EditTask(OBJECT taskobj,string taskname="",string user="default",string addr="localhost");


//////////////////////////////////EntityGroup/////////////////////////////////////////

SVAPI_API
OBJECT GetEntityGroup(string entitygroupid,string user="default",string addr="localhost");

SVAPI_API
OBJECT CreateEntityGroup(string entitygroupid);

SVAPI_API
MAPNODE GetEntityGroupMainAttribNode(OBJECT egobj);

SVAPI_API
bool GetSubEntityTempletIDByEG(std::list<string> &etlist,OBJECT egobj);

SVAPI_API
bool DeleteEntityGroup(string entitygroupid,string user="default",string addr="localhost");

SVAPI_API
bool AddSubEntityTempletIDToEG(OBJECT egobj,string entitytempletid);

SVAPI_API
bool DeleteSubEntityTempletIDInEG(OBJECT egobj,string entitytempletid);

SVAPI_API
bool SubmitEntityGroup(OBJECT egobj,string user="default",string addr="localhost");

SVAPI_API
bool CloseEntityGroup(OBJECT egobj);

SVAPI_API 
bool GetAllEntityGroups(PAIRLIST &retlist,string infoname="sv_Name",string user="default",string addr="localhost");

SVAPI_API
OBJECT CloneEntityGroup(OBJECT egobj,string entitygroupid);

/////////////////////////////////////EntityTemplet////////////////////////////////////////

SVAPI_API
OBJECT GetEntityTemplet(string entitytempletid,string user="default",string addr="localhost");

SVAPI_API
OBJECT CreateEntityTemplet(string entitytempletid);

SVAPI_API
bool DeleteEntityTemplet(string entitytempletid,string user="default",string addr="localhost");

SVAPI_API
MAPNODE GetEntityTempletMainAttribNode(OBJECT etobj);

SVAPI_API 
bool FindETContrlFirst(OBJECT etobj,LISTITEM &item);

SVAPI_API
bool GetSubMonitorTypeByET(OBJECT etobj,std::list<int> &mtlist);

SVAPI_API
std::list<int> & GetSubMonitorTypeList(OBJECT etobj);

SVAPI_API
bool AddSubMonitorTypeToET(OBJECT etobj,int MonitorType);

SVAPI_API
bool DeleteSubMonitorTypeInET(OBJECT etobj,int MonitorType);

SVAPI_API
bool SubmitEntityTemplet(OBJECT etobj,string user="default",string addr="localhost");

SVAPI_API
bool CloseEntityTemplet(OBJECT etobj);

SVAPI_API 
bool GetAllEntityTemplets(PAIRLIST &retlist,string infoname="sv_Name",string user="default",string addr="localhost");

SVAPI_API
OBJECT CloneEntityTemplet(OBJECT etobj,string entitytempletid);


//////////////////////////////////////db//////////////////////////////////////////////

//////////////////////////////////////dyn/////////////////////////////////////////////








//////////////////////////////////end///////////////////////////////////////////////




#endif