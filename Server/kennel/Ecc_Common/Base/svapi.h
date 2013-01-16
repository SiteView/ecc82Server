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

typedef struct sv_list{
	sv_list()
	{
		list=INVALID_VALUE;
		iterator=INVALID_VALUE;
	}

	OBJECT list;
	OBJECT iterator;

}LISTITEM;

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
void	ReleaseItemList(const LISTITEM &listi);

SVAPI_API
bool	CreateNewItemList(LISTITEM&listi);

/*
OBJECT
*/

SVAPI_API 
void	CloseMapNodeObject(OBJECT obj);

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


//////////////////////////////////end///////////////////////////////////////////////






















#endif