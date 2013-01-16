///*  @file svapi.h
//*  author :		chen xin gang
//*  Copyright (C) 2005-2006 dragonflow corp.
//*


//*********************************************************************************//
//*****																    	 ******//
//*****  调用者的程序 !! 必须/MD release编译 !! （因为svapi.dll 是/MD 编译） ******//
//*****																		 ******//
//*********************************************************************************//

//                   svapi.dll 的 概述说明
//	1、  所有的配置文件的读取位置是确定的，调用者不能指定
//	2、  svdb.exe 启动时读取  \data\svdbconfig.ini\ [svdb] EnableIDC=1   // 1时启动为 idc 版， 0时启动为 非 idc 版
//	3、	 svdb.exe 启动为 idc 版后, 将产生 idc 用户 的概念，每个 idc 用户有一个独立目录，保存着
//			他自己的配置文件和 .db ，所有这些对 svapi 函数是透明的,对调用者是平滑的。
//	4、  除了 ini 函数的传入参数 user 是有用的， 之外的传入参数 user 没有意义 svapi.dll 也不作处理
//	5、  传入参数 user, 即 idc 用户的 用户id 概念：每个idc用户有且仅有一个顶层组（在se之下），这个组 id 即其“用户id”
//			例：用户 1.16 ，他拥有一个 \data\idc_data\1.16 目录，其下保存着他独有的文件，


#ifndef	SITEVIEW_SVAPI_
#define	SITEVIEW_SVAPI_

#include <iostream>
#include <String.h>
#include <list>
#include <map>

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


//typedef unsigned long	OBJECT;
//typedef unsigned long	MAPNODE;
typedef void*	OBJECT;
typedef void*	MAPNODE;


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
bool	ClearMapNode(MAPNODE node);
//清空所有的值

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
void	CloseMapNodeObject(OBJECT &obj);  //坏函数，不可用


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



////////////////////////////////// For sca / web-service ////////////////////////////


typedef std::map<string,string> StdMapStr,		NodeData;
typedef std::list< StdMapStr >	StdListMapStr,	ForestList;

SVAPI_API
bool GetForestData(ForestList & flist, string pid, bool onlySon=true, string addr="localhost");
//                     返回的数据          父id   是否只包含直接儿子
//当父id为"default" onlySon为false 时，返回整棵树；	当父id为"default" onlySon为true 时,返回最高一层节点
//当父id是一个设备时，返回的监测器数据中将包含 creat_time 、dstr 等其他 dyn 信息
//当某个节点只要有一个 监测器子孙 状态为 error、warning , 该节点的 status 就为 error、warning (另：error 覆盖 warning)
//此功能不返回 pid 节点, 只返回其所有子孙

SVAPI_API
bool DelChildren(string pid, bool autoDelTable=true, string addr="localhost");
//                 父id   是否同时自动删除与监测器对应的 Table
//递归删除服务器端的 pid 节点及其所有子孙, pid 不能是“default”,不能是 se 
//注意一个问题：删除了pid 节点及其所有子孙后，pid的父亲 最好从服务器端 重新获取一下，
//              否则客户端的 pid的父亲 仍然记忆着 pid (但pid其实已经被删除了)

SVAPI_API
bool PutValueIntoChildren(const NodeData & ndata, string pid, string addr="localhost");
//									传入的数据          父id  
//此功能对 pid 节点及其所有子孙起作用, pid 不能是“default”,不能是 se 
//比如要禁止监测器，就 sv_disable=true  (或 sv_disable=time  sv_starttime=XXX  sv_endtime=XXX )  给某一个节点及其所有子孙
//比如要启动监测器，就 sv_disable=false (或 sv_disable=      sv_starttime=     sv_endtime=    )  给某一个节点及其所有子孙



/////////////////////////////////// 通用相关    ////////////////////////////////////


SVAPI_API 
bool SetSvdbAddrByFile(string filename);
//设置与 svdb.exe 通讯的强制地址,即设置调用 svapi.dll 的进程中所有 svapi.dll 函数的 string addr 
//注意：一旦调用该函数设置后，该进程中所有其他代码里传递给 svapi.dll 函数的 string addr 都将被抛弃，而引用该函数设置的 addr
//退设置的方法: SetSvdbAddrByFile(""); 这样会返回false 并同时取消之前的设置;   任何使该函数返回false的情况,都会退设置。

SVAPI_API 
string GetSvdbAddr(void);//获取上一函数设置的与 svdb.exe 通讯的强制地址

SVAPI_API 
bool SetCgiVersionByFile(string filename);
//读取指定文件的指定字段，并保存为一个全局 string 变量
//退设置的方法: SetCgiVersionByFile(""); 这样会返回false 并同时取消之前的设置;   任何使该函数返回false的情况,都会退设置。

SVAPI_API 
string GetCgiVersion(void);//获取上一函数设置的全局 string 变量


SVAPI_API 
string	GetSiteViewRootPath();//读取 svapi.dll 所在机器的注册表,而不是 svdb.exe 所在机器的注册表

SVAPI_API
string GetMasterAddress();

SVAPI_API
bool isDemoMode();

SVAPI_API
string	FindParentID(string id);

SVAPI_API
int	FindIndexByID(string id);

SVAPI_API
bool IsSVSEID(string id);

SVAPI_API
string FindSEID(string id);

/////////////////下面这个函数非线程安全(建议用到时只在程序启动时调用一次)//////////////////

SVAPI_API
void SetServerPort(unsigned short port);

SVAPI_API
unsigned short GetServerPort();


////////////////////////////////// ini API/////////////////////////////////////////////////

//每个ini 文件可以有多个 section, 每个section 可以有多个“ key = value ”的数据对
//每一个key 有三种类型： int，string, binary ,读或写须调用相应函数
//  注意：  ------------------------------   注意
//  注意：如果 svdb.exe 启动为非idc版，则以下函数传入的 user 参数全部被抛弃，即全部解析为“default”取得总目录下的 ini 文件
//  注意：  ------------------------------   注意

SVAPI_API
bool GetIniFileSections(std::list<string> &sectionlist,string filename,string addr="localhost",string user="default");
//获取某个ini文件的所有的 section  ，//传出 section的list 引用， ini文件名，SVDB 地址， idc用户id

SVAPI_API
bool GetIniFileKeys(string section,std::list<string> &keylist,string filename,string addr="localhost",string user="default");
//获取某个section 下的所有的key

SVAPI_API
int	GetIniFileValueType(string section,string key,string filename,string addr="localhost",string user="default");
//获取某个 key 的类型
// 下面是GetIniFileValueType可能返回的值
#define INIVALUE_FAILED	-1			//获取失败
#define INIVALUE_NULL	0			//未知类型
#define INIVALUE_INT	1			//整型数据
#define	INIVALUE_STRING	2			//字符串型数据
#define INIVALUE_BINARY	3			//二进制类型数据

//批量写入(追加)string 型值到某section中：// 批量数据,   section名，      ini文件名，   SVDB 地址，             idc用户id
SVAPI_API
bool WriteIniSectionManyString(const NodeData & ndata, string section, string filename, string addr="localhost",string user="default");


// 根据key的类型写ini的3个函数 ：// section名，   key 名，value 值，  ini文件名，SVDB 地址， idc用户id
SVAPI_API
bool WriteIniFileString(string section,string key,string str,string filename,string addr="localhost",string user="default");

SVAPI_API
bool WriteIniFileInt(string section,string key,int value,string filename,string addr="localhost",string user="default");

SVAPI_API
bool WriteIniFileStruct(string section,string key, void *data,unsigned int len,string filename,string addr="localhost",string user="default");


// 根据key的类型读ini的3个函数：            // 默认返回值（随便设）
SVAPI_API
string GetIniFileString(string section,string key,string defaultret,string filename,string addr="localhost",string user="default");

SVAPI_API
int	GetIniFileInt(string section,string key,int defaultret,string filename,string addr="localhost",string user="default");

SVAPI_API
bool GetIniFileStruct(string section,string key,void *buf,unsigned int &len,string filename,string addr="localhost",string user="default");


SVAPI_API
bool DeleteIniFileSection(string section,string filename,string addr="localhost",string user="default");
//删除某个 section

SVAPI_API
bool DeleteIniFileKey(string section,string key,string filename,string addr="localhost",string user="default");
//删除某个 key


SVAPI_API
bool EditIniFileSection(string oldsection,string newsection,string filename,string addr="localhost",string user="default");
//修改某个 section

SVAPI_API
bool EditIniFileKey(string section,string oldkey,string newkey,string filename,string addr="localhost",string user="default");
//修改某个 key

SVAPI_API
bool CopyIniFile(string filecopy,string addr="localhost",string user="default");
//拷贝 .ini 文件 , 此函数提供给 idc 版使用，会将总目录下的 .ini 拷贝到某个 idc 用户的目录下,并重命名
//string filecopy 的规则,如：“yewuhumenu.ini,menu.ini;license.ini,license.ini”
//                             源文件名    ,目标文件名; 源文件名    ,目标文件名


typedef std::map< string, NodeData > ForestMap, SvIniFile;

SVAPI_API
bool GetSvIniFileBySections( SvIniFile & inifile, string filename, string sections="default", string addr="localhost", string user="default");
//                                   返回的数据    ， ini文件名，     需要的 sections 串           SVDB 地址，         idc用户id
//根据传入的以英文逗号“,”分割的 sections 串，返回相应的 svinifile ，如果传入 sections 为 “default” 则返回全部
//若 key 的类型为 int ,则转为 string 返回； 其他类型只返回说明（如 INIVALUE_BINARY 二进制类型，只返回“Binary data”） 
//由于只要一次函数调用，将比原来的旧方法快很多 



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
bool CloseSVSE(OBJECT &svseobj);//删除对象，以免内存泄漏

SVAPI_API
string AddNewSVSE(OBJECT svseobj,string user="default",string addr="localhost");

SVAPI_API
bool SubmitSVSE(OBJECT svseobj,string user="default",string addr="localhost");

SVAPI_API 
bool GetAllSVSEInfo(PAIRLIST &retlist,string user="default",string addr="localhost");
//返回所有 SE 的 label

SVAPI_API 
bool GetAllSVSEObjInfo(PAIRLIST &retlist,string infoname,string user="default",string addr="localhost");
									// 传入"ObjectVersion"，则返回 obj 版本戳



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
void CloseMonitorTemplet(OBJECT &obj);//删除对象，以免内存泄漏


SVAPI_API 
bool	SubmitMonitorTemplet(OBJECT mtobj,string user="default",string addr="localhost");

SVAPI_API 
bool GetAllMonitorTempletInfo(PAIRLIST &retlist,string infoname="sv_label",string user="default",string addr="localhost");

SVAPI_API
OBJECT CloneMonitorTemplet(OBJECT mtobj,int targetid);

/////////////////////////////Resource///////////////////////////////////////////////
SVAPI_API 
OBJECT LoadResourceByKeys(string needkeys, string language="default",string addr="localhost");
//根据传入的以英文逗号“,”分割的 key 串，返回相应的 OBJECT , 由于只返回所需要的数据，速度将快得多。 
//注意：这种不完整的 obj ,是不可以 submit 的，如果强行 submit 会返回 false 。

SVAPI_API 
OBJECT LoadResource(string language="default",string addr="localhost");

SVAPI_API
OBJECT CreateResource(string language);

SVAPI_API 
MAPNODE GetResourceNode(OBJECT rcobj);

SVAPI_API 
bool	EnumResourceAttrib(OBJECT rcobj,PAIRLIST &retlist);

SVAPI_API
bool	CloseResource(OBJECT &rcobj);//删除对象，以免内存泄漏

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
void CloseMonitor(OBJECT &monitorobj);//删除对象，以免内存泄漏

SVAPI_API
string AddNewMonitor(OBJECT monitorobj,string entityid,string user="default",string addr="localhost");

SVAPI_API 
bool	SubmitMonitor(OBJECT monitorobj,string user="default",string addr="localhost");

SVAPI_API 
bool GetAllMonitorsInfo(PAIRLIST &retlist,string infoname="sv_label",string user="default",string addr="localhost");
								// infoname传入"ObjectVersion"则返回 obj 版本戳,  user 可以指定父 id 

SVAPI_API
string MonitorCopy(string srcmonitorid,string objentityid,string user="default",string addr="localhost");

SVAPI_API
string MonitorCopyAndCreateTable(string srcmonitorid,string objentityid,string user="default",string addr="localhost");
//拷贝监测器，并创建相应的 table


SVAPI_API
bool GetMonitorsInfoBySE(string seid,PAIRLIST &retlist,string infoname="sv_label",string user="default",string addr="localhost");


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
bool CloseEntity(OBJECT &entityobj);//删除对象，以免内存泄漏

SVAPI_API
bool SubmitEntity(OBJECT entityobj,string user="default",string addr="localhost");

SVAPI_API 
bool GetAllEntitysInfo(PAIRLIST &retlist,string infoname="sv_label",string user="default",string addr="localhost");
									// infoname传入"ObjectVersion"则返回 obj 版本戳,  user 可以指定父 id 

SVAPI_API
string EntityCopy(string srcentityid,string objgroupid,string user="default",string addr="localhost");

SVAPI_API
string EntityCopyAndCreateTable(string srcentityid,string objgroupid,string user="default",string addr="localhost");
//拷贝设备及其下所有监测器，并创建相应的 table


//////////////////////////////Group////////////////////////////////////////////////
SVAPI_API
string  CreatIdcUser(string userindex, string pid="0", string addr="localhost");
// 创建一个 idc 用户   ， userindex 是 cgi程序要用到的一个 用户index，本函数只是替 cgi 程序自动保存这一属性
// pid 是idc 用户的 se 父亲的 id ， 当 pid=="0" 时 由系统自动寻找合适的 se 来添加，  当 pid!="0" 时 由调用者指定

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
bool CloseGroup(OBJECT &groupobj);//删除对象，以免内存泄漏

SVAPI_API
bool SubmitGroup(OBJECT groupobj,string user="default",string addr="localhost");

SVAPI_API 
bool GetAllGroupsInfo(PAIRLIST &retlist,string infoname="sv_label",string user="default",string addr="localhost");
								// infoname传入"ObjectVersion"则返回 obj 版本戳,  user 可以指定父 id 

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
bool GetAllTaskKeyName(std::list<string> &keynamelist,OBJECT taskobj);

SVAPI_API
bool SubmitTask(OBJECT taskobj,string user="default",string addr="localhost");

SVAPI_API
void CloseTask(OBJECT &taskobj);//删除对象，以免内存泄漏

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
bool CloseEntityGroup(OBJECT &egobj);//删除对象，以免内存泄漏

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
bool CloseEntityTemplet(OBJECT &etobj);//删除对象，以免内存泄漏

SVAPI_API 
bool GetAllEntityTemplets(PAIRLIST &retlist,string infoname="sv_Name",string user="default",string addr="localhost");

SVAPI_API 
bool GetAllEntityTempletsInProperty(string key,string value,std::list<string> &idlist,string user="default",string addr="localhost");

SVAPI_API
OBJECT CloneEntityTemplet(OBJECT etobj,string entitytempletid);




/////////////////////////////////////  cache函数  ////////////////////////////////////////

// 以下 14 个函数会将获取到的 object 缓存在本地，再次调用时将返回同一个引用。
// 务必保证不对以下 13 个函数获得的 object ,执行 CloseXXX(object); 因为万一之后在进程中其他地方引用该 object,将会导致“缓冲区溢出“。
SVAPI_API
void SetCacheUserAddr(string user="default",string addr="localhost");
//此函数会同时清空所有以下 13 个函数 (及CacheRefreshSVDYNs 2 函数) 创建的本地缓存,此函数非线程安全

//以下 5 个函数获取最新 object 的方法是重启进程
//以下 5 个函数二次获取的时间极短，几乎不可测量
SVAPI_API
OBJECT	Cache_GetMonitorTemplet(int id);

SVAPI_API
OBJECT	Cache_GetEntityGroup(string entitygroupid);

SVAPI_API
OBJECT	Cache_GetEntityTemplet(string entitytempletid);

SVAPI_API
OBJECT	Cache_LoadResourceByKeys(string needkeys, string language="default");

SVAPI_API
OBJECT	Cache_LoadResource(string language="default");

//以下 4 个函数在每次调用时都询问服务器，并保证获取最新版本的 object 。
//以下 4 个函数二次获取的速度比老函数快1-3倍
SVAPI_API
OBJECT	Cache_GetSVSE(string id);

SVAPI_API
OBJECT	Cache_GetGroup(string groupid);

SVAPI_API
OBJECT	Cache_GetEntity(string entityid);

SVAPI_API
OBJECT	Cache_GetMonitor(string monitorid);




//以下2个函数配套使用，首次获取的速度比老函数快 五 倍，二次获取快 十几 倍 
SVAPI_API
bool	CacheRefreshEntities(string parentid="default");
//为 parentid 的所有子孙 entity 从服务器集中刷新最新版本

SVAPI_API
OBJECT	CacheRefresh_GetEntity(string entityid);
//返回本地缓存的 object (不询问服务器)



//以下2个函数配套使用，首次获取的速度比老函数快 五 倍，二次获取快 十几 倍 
SVAPI_API
bool	CacheRefreshMonitors(string parentid="default");
//为 parentid 的所有子孙 monitor 从服务器集中刷新最新版本

SVAPI_API
OBJECT	CacheRefresh_GetMonitor(string monitorid);
//返回本地缓存的 object (不询问服务器)






//////////////////////////////////////db//////////////////////////////////////////////

//////////////////////////////////////dyn/////////////////////////////////////////////








//////////////////////////////////end///////////////////////////////////////////////




#endif