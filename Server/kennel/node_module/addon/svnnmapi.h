/*************************************************
*  @file svnnmapi.h
*  author :		chen xin gang
*  Copyright (C) 2005-2006 dragonflow corp.
*
*************************************************/


#ifndef	SITEVIEW_SVNNMAPI_
#define	SITEVIEW_SVNNMAPI_



//�����⼸��.h ���滹�����ż�ʮ��.h ,Ϊ���������Ϊ���ù�����ӡ����Ӱ���Ŀ¼���� .../svdb_comb; .../svdb_comb/include; .../svdb_comb/w32
#include "addon.h"
#include "EntityEx.h"
#include "TopologyChart.h"


//	��ȡ Port , EntityRelation , Edge �����������ģʽ
//	EntityEx *pe=reinterpret_cast<EntityEx *>(nnmentityobj);
//  pe->���ַ���();


////////////////////////////////////////////////////NNMEntity//////////////////////////////////////////////////////////

SVAPI_API
OBJECT GetNNMEntity(string entityid,string user="default",string addr="localhost");

SVAPI_API
OBJECT CreateNNMEntity(string nnmentityid);  // ��� nnmentityid Ϊ�գ�ֱ�� return INVALID_VALUE;

SVAPI_API
MAPNODE GetNNMEntityMainAttribNode(OBJECT nnmentityobj);

SVAPI_API
bool DeleteNNMEntity(string entityid,string user="default",string addr="localhost");

SVAPI_API
bool CloseNNMEntity(OBJECT &entityobj);//ɾ�����������ڴ�й©

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
OBJECT CreateTopologyChart(string TopologyID);  // ��� TopologyID Ϊ�գ�ֱ�� return INVALID_VALUE;

SVAPI_API
MAPNODE GetTopologyChartMainAttribNode(OBJECT topologyobj);

SVAPI_API
bool DeleteTopologyChart(string topologyid,string user="default",string addr="localhost");

SVAPI_API
bool CloseTopologyChart(OBJECT &topologyobj);//ɾ�����������ڴ�й©

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
OBJECT CreateVirtualGroup(string VirtualGroupID);// ��� VirtualGroupID Ϊ�գ�ֱ�� return INVALID_VALUE;

SVAPI_API
MAPNODE GetVirtualGroupMainAttribNode(OBJECT virtualgroupobj);

SVAPI_API
bool DeleteVirtualGroup(string virtualgroupid,string user="default",string addr="localhost");

SVAPI_API
bool CloseVirtualGroup(OBJECT &virtualgroupobj);//ɾ�����������ڴ�й©

SVAPI_API
bool SubmitVirtualGroup(OBJECT virtualgroupobj,string user="default",string addr="localhost");

SVAPI_API 
bool GetAllVirtualGroupInfo(PAIRLIST &retlist,string infoname="sv_label",string user="default",string addr="localhost");

SVAPI_API 
bool GetAllVirtualGroupInfoInWords(string key,string value,std::list<string> &idlist,string user="default",string addr="localhost");

SVAPI_API 
bool GetAllVirtualGroupInfoInProperty(string key,string value,std::list<string> &idlist,string user="default",string addr="localhost");





#endif
