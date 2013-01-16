/*************************************************
*  @file svnnmapi.h
*  author :		chen xin gang
*  Copyright (C) 2005-2006 dragonflow corp.
*
*************************************************/


#ifndef	SITEVIEW_SVNNMAPI_
#define	SITEVIEW_SVNNMAPI_



//以下这几个.h 后面还连带着几十个.h ,为简化起见可以为调用工程添加“附加包含目录”： .../svdb_comb; .../svdb_comb/include; .../svdb_comb/w32
#include "svapi.h"
#include "EntityEx.h"
#include "TopologyChart.h"


//	获取 Port , EntityRelation , Edge 等请采用如下模式
//	EntityEx *pe=reinterpret_cast<EntityEx *>(nnmentityobj);
//  pe->各种方法();


////////////////////////////////////////////////////NNMEntity//////////////////////////////////////////////////////////

SVAPI_API
OBJECT GetNNMEntity(string entityid,string user="default",string addr="localhost");

SVAPI_API
OBJECT CreateNNMEntity(string nnmentityid);  // 如果 nnmentityid 为空，直接 return INVALID_VALUE;

SVAPI_API
MAPNODE GetNNMEntityMainAttribNode(OBJECT nnmentityobj);

SVAPI_API
bool DeleteNNMEntity(string entityid,string user="default",string addr="localhost");

SVAPI_API
bool CloseNNMEntity(OBJECT &entityobj);//删除对象，以免内存泄漏

SVAPI_API
bool SubmitNNMEntity(OBJECT entityobj,string user="default",string addr="localhost");

SVAPI_API 
bool GetAllNNMEntitysInfo(PAIRLIST &retlist,string infoname="sv_label",string user="default",string addr="localhost");

SVAPI_API 
bool GetAllNNMEntitysInfoInWords(string key,string value,std::list<string> &idlist,string user="default",string addr="localhost");

SVAPI_API 
bool GetAllNNMEntitysInfoInProperty(string key,string value,std::list<string> &idlist,string user="default",string addr="localhost");


/////////////////////////////////////////////////////TopologyChart///////////////////////////////////////////////////////

SVAPI_API
OBJECT GetTopologyChart(string TopologyID,string user="default",string addr="localhost");

SVAPI_API
OBJECT CreateTopologyChart(string TopologyID);  // 如果 TopologyID 为空，直接 return INVALID_VALUE;

SVAPI_API
MAPNODE GetTopologyChartMainAttribNode(OBJECT topologyobj);

SVAPI_API
bool DeleteTopologyChart(string topologyid,string user="default",string addr="localhost");

SVAPI_API
bool CloseTopologyChart(OBJECT &topologyobj);//删除对象，以免内存泄漏

SVAPI_API
bool SubmitTopologyChart(OBJECT topologyobj,string user="default",string addr="localhost");

SVAPI_API 
bool GetAllTopologyChartInfo(PAIRLIST &retlist,string infoname="sv_label",string user="default",string addr="localhost");

SVAPI_API 
bool GetAllTopologyChartInfoInWords(string key,string value,std::list<string> &idlist,string user="default",string addr="localhost");

SVAPI_API 
bool GetAllTopologyChartInfoInProperty(string key,string value,std::list<string> &idlist,string user="default",string addr="localhost");


/////////////////////////////////////////////////////VirtualGroup///////////////////////////////////////////////////////

SVAPI_API
OBJECT GetVirtualGroup(string VirtualGroupID,string user="default",string addr="localhost");

SVAPI_API
OBJECT CreateVirtualGroup(string VirtualGroupID);// 如果 VirtualGroupID 为空，直接 return INVALID_VALUE;

SVAPI_API
MAPNODE GetVirtualGroupMainAttribNode(OBJECT virtualgroupobj);

SVAPI_API
bool DeleteVirtualGroup(string virtualgroupid,string user="default",string addr="localhost");

SVAPI_API
bool CloseVirtualGroup(OBJECT &virtualgroupobj);//删除对象，以免内存泄漏

SVAPI_API
bool SubmitVirtualGroup(OBJECT virtualgroupobj,string user="default",string addr="localhost");

SVAPI_API 
bool GetAllVirtualGroupInfo(PAIRLIST &retlist,string infoname="sv_label",string user="default",string addr="localhost");

SVAPI_API 
bool GetAllVirtualGroupInfoInWords(string key,string value,std::list<string> &idlist,string user="default",string addr="localhost");

SVAPI_API 
bool GetAllVirtualGroupInfoInProperty(string key,string value,std::list<string> &idlist,string user="default",string addr="localhost");





#endif
