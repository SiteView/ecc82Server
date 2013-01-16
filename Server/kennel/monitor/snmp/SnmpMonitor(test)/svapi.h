///*  @file svapi.h
//*  author :		chen xin gang
//*  Copyright (C) 2005-2006 dragonflow corp.
//*


//*********************************************************************************//
//*****																    	 ******//
//*****  �����ߵĳ��� !! ����/MD release���� !! ����Ϊsvapi.dll ��/MD ���룩 ******//
//*****																		 ******//
//*********************************************************************************//

//                   svapi.dll �� ����˵��
//	1��  ���е������ļ��Ķ�ȡλ����ȷ���ģ������߲���ָ��
//	2��  svdb.exe ����ʱ��ȡ  \data\svdbconfig.ini\ [svdb] EnableIDC=1   // 1ʱ����Ϊ idc �棬 0ʱ����Ϊ �� idc ��
//	3��	 svdb.exe ����Ϊ idc ���, ������ idc �û� �ĸ��ÿ�� idc �û���һ������Ŀ¼��������
//			���Լ��������ļ��� .db ��������Щ�� svapi ������͸����,�Ե�������ƽ���ġ�
//	4��  ���� ini �����Ĵ������ user �����õģ� ֮��Ĵ������ user û������ svapi.dll Ҳ��������
//	5��  ������� user, �� idc �û��� �û�id ���ÿ��idc�û����ҽ���һ�������飨��se֮�£�������� id ���䡰�û�id��
//			�����û� 1.16 ����ӵ��һ�� \data\idc_data\1.16 Ŀ¼�����±����������е��ļ���


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
//������е�ֵ

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
void	CloseMapNodeObject(OBJECT &obj);  //��������������


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
//                     ���ص�����          ��id   �Ƿ�ֻ����ֱ�Ӷ���
//����idΪ"default" onlySonΪfalse ʱ��������������	����idΪ"default" onlySonΪtrue ʱ,�������һ��ڵ�
//����id��һ���豸ʱ�����صļ���������н����� creat_time ��dstr ������ dyn ��Ϣ
//��ĳ���ڵ�ֻҪ��һ�� ��������� ״̬Ϊ error��warning , �ýڵ�� status ��Ϊ error��warning (��error ���� warning)
//�˹��ܲ����� pid �ڵ�, ֻ��������������

SVAPI_API
bool DelChildren(string pid, bool autoDelTable=true, string addr="localhost");
//                 ��id   �Ƿ�ͬʱ�Զ�ɾ����������Ӧ�� Table
//�ݹ�ɾ���������˵� pid �ڵ㼰����������, pid �����ǡ�default��,������ se 
//ע��һ�����⣺ɾ����pid �ڵ㼰�����������pid�ĸ��� ��ôӷ������� ���»�ȡһ�£�
//              ����ͻ��˵� pid�ĸ��� ��Ȼ������ pid (��pid��ʵ�Ѿ���ɾ����)

SVAPI_API
bool PutValueIntoChildren(const NodeData & ndata, string pid, string addr="localhost");
//									���������          ��id  
//�˹��ܶ� pid �ڵ㼰����������������, pid �����ǡ�default��,������ se 
//����Ҫ��ֹ��������� sv_disable=true  (�� sv_disable=time  sv_starttime=XXX  sv_endtime=XXX )  ��ĳһ���ڵ㼰����������
//����Ҫ������������� sv_disable=false (�� sv_disable=      sv_starttime=     sv_endtime=    )  ��ĳһ���ڵ㼰����������



/////////////////////////////////// ͨ�����    ////////////////////////////////////


SVAPI_API 
bool SetSvdbAddrByFile(string filename);
//������ svdb.exe ͨѶ��ǿ�Ƶ�ַ,�����õ��� svapi.dll �Ľ��������� svapi.dll ������ string addr 
//ע�⣺һ�����øú������ú󣬸ý������������������ﴫ�ݸ� svapi.dll ������ string addr �����������������øú������õ� addr
//�����õķ���: SetSvdbAddrByFile(""); �����᷵��false ��ͬʱȡ��֮ǰ������;   �κ�ʹ�ú�������false�����,���������á�

SVAPI_API 
string GetSvdbAddr(void);//��ȡ��һ�������õ��� svdb.exe ͨѶ��ǿ�Ƶ�ַ

SVAPI_API 
bool SetCgiVersionByFile(string filename);
//��ȡָ���ļ���ָ���ֶΣ�������Ϊһ��ȫ�� string ����
//�����õķ���: SetCgiVersionByFile(""); �����᷵��false ��ͬʱȡ��֮ǰ������;   �κ�ʹ�ú�������false�����,���������á�

SVAPI_API 
string GetCgiVersion(void);//��ȡ��һ�������õ�ȫ�� string ����


SVAPI_API 
string	GetSiteViewRootPath();//��ȡ svapi.dll ���ڻ�����ע���,������ svdb.exe ���ڻ�����ע���

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

/////////////////��������������̰߳�ȫ(�����õ�ʱֻ�ڳ�������ʱ����һ��)//////////////////

SVAPI_API
void SetServerPort(unsigned short port);

SVAPI_API
unsigned short GetServerPort();


////////////////////////////////// ini API/////////////////////////////////////////////////

//ÿ��ini �ļ������ж�� section, ÿ��section �����ж���� key = value �������ݶ�
//ÿһ��key ���������ͣ� int��string, binary ,����д�������Ӧ����
//  ע�⣺  ------------------------------   ע��
//  ע�⣺��� svdb.exe ����Ϊ��idc�棬�����º�������� user ����ȫ������������ȫ������Ϊ��default��ȡ����Ŀ¼�µ� ini �ļ�
//  ע�⣺  ------------------------------   ע��

SVAPI_API
bool GetIniFileSections(std::list<string> &sectionlist,string filename,string addr="localhost",string user="default");
//��ȡĳ��ini�ļ������е� section  ��//���� section��list ���ã� ini�ļ�����SVDB ��ַ�� idc�û�id

SVAPI_API
bool GetIniFileKeys(string section,std::list<string> &keylist,string filename,string addr="localhost",string user="default");
//��ȡĳ��section �µ����е�key

SVAPI_API
int	GetIniFileValueType(string section,string key,string filename,string addr="localhost",string user="default");
//��ȡĳ�� key ������
// ������GetIniFileValueType���ܷ��ص�ֵ
#define INIVALUE_FAILED	-1			//��ȡʧ��
#define INIVALUE_NULL	0			//δ֪����
#define INIVALUE_INT	1			//��������
#define	INIVALUE_STRING	2			//�ַ���������
#define INIVALUE_BINARY	3			//��������������

//����д��(׷��)string ��ֵ��ĳsection�У�// ��������,   section����      ini�ļ�����   SVDB ��ַ��             idc�û�id
SVAPI_API
bool WriteIniSectionManyString(const NodeData & ndata, string section, string filename, string addr="localhost",string user="default");


// ����key������дini��3������ ��// section����   key ����value ֵ��  ini�ļ�����SVDB ��ַ�� idc�û�id
SVAPI_API
bool WriteIniFileString(string section,string key,string str,string filename,string addr="localhost",string user="default");

SVAPI_API
bool WriteIniFileInt(string section,string key,int value,string filename,string addr="localhost",string user="default");

SVAPI_API
bool WriteIniFileStruct(string section,string key, void *data,unsigned int len,string filename,string addr="localhost",string user="default");


// ����key�����Ͷ�ini��3��������            // Ĭ�Ϸ���ֵ������裩
SVAPI_API
string GetIniFileString(string section,string key,string defaultret,string filename,string addr="localhost",string user="default");

SVAPI_API
int	GetIniFileInt(string section,string key,int defaultret,string filename,string addr="localhost",string user="default");

SVAPI_API
bool GetIniFileStruct(string section,string key,void *buf,unsigned int &len,string filename,string addr="localhost",string user="default");


SVAPI_API
bool DeleteIniFileSection(string section,string filename,string addr="localhost",string user="default");
//ɾ��ĳ�� section

SVAPI_API
bool DeleteIniFileKey(string section,string key,string filename,string addr="localhost",string user="default");
//ɾ��ĳ�� key


SVAPI_API
bool EditIniFileSection(string oldsection,string newsection,string filename,string addr="localhost",string user="default");
//�޸�ĳ�� section

SVAPI_API
bool EditIniFileKey(string section,string oldkey,string newkey,string filename,string addr="localhost",string user="default");
//�޸�ĳ�� key

SVAPI_API
bool CopyIniFile(string filecopy,string addr="localhost",string user="default");
//���� .ini �ļ� , �˺����ṩ�� idc ��ʹ�ã��Ὣ��Ŀ¼�µ� .ini ������ĳ�� idc �û���Ŀ¼��,��������
//string filecopy �Ĺ���,�磺��yewuhumenu.ini,menu.ini;license.ini,license.ini��
//                             Դ�ļ���    ,Ŀ���ļ���; Դ�ļ���    ,Ŀ���ļ���


typedef std::map< string, NodeData > ForestMap, SvIniFile;

SVAPI_API
bool GetSvIniFileBySections( SvIniFile & inifile, string filename, string sections="default", string addr="localhost", string user="default");
//                                   ���ص�����    �� ini�ļ�����     ��Ҫ�� sections ��           SVDB ��ַ��         idc�û�id
//���ݴ������Ӣ�Ķ��š�,���ָ�� sections ����������Ӧ�� svinifile ��������� sections Ϊ ��default�� �򷵻�ȫ��
//�� key ������Ϊ int ,��תΪ string ���أ� ��������ֻ����˵������ INIVALUE_BINARY ���������ͣ�ֻ���ء�Binary data���� 
//����ֻҪһ�κ������ã�����ԭ���ľɷ�����ܶ� 



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
bool CloseSVSE(OBJECT &svseobj);//ɾ�����������ڴ�й©

SVAPI_API
string AddNewSVSE(OBJECT svseobj,string user="default",string addr="localhost");

SVAPI_API
bool SubmitSVSE(OBJECT svseobj,string user="default",string addr="localhost");

SVAPI_API 
bool GetAllSVSEInfo(PAIRLIST &retlist,string user="default",string addr="localhost");
//�������� SE �� label

SVAPI_API 
bool GetAllSVSEObjInfo(PAIRLIST &retlist,string infoname,string user="default",string addr="localhost");
									// ����"ObjectVersion"���򷵻� obj �汾��



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

/* //������������û��
SVAPI_API 
bool	AddMTParameterItem(LISTITEM &item);

SVAPI_API 
bool	AddAdvanceParameterItem(LISTITEM &item);

SVAPI_API 
bool	AddAlertConditionItem(LISTITEM &item);
*/

SVAPI_API
void CloseMonitorTemplet(OBJECT &obj);//ɾ�����������ڴ�й©


SVAPI_API 
bool	SubmitMonitorTemplet(OBJECT mtobj,string user="default",string addr="localhost");

SVAPI_API 
bool GetAllMonitorTempletInfo(PAIRLIST &retlist,string infoname="sv_label",string user="default",string addr="localhost");

SVAPI_API
OBJECT CloneMonitorTemplet(OBJECT mtobj,int targetid);

/////////////////////////////Resource///////////////////////////////////////////////
SVAPI_API 
OBJECT LoadResourceByKeys(string needkeys, string language="default",string addr="localhost");
//���ݴ������Ӣ�Ķ��š�,���ָ�� key ����������Ӧ�� OBJECT , ����ֻ��������Ҫ�����ݣ��ٶȽ���öࡣ 
//ע�⣺���ֲ������� obj ,�ǲ����� submit �ģ����ǿ�� submit �᷵�� false ��

SVAPI_API 
OBJECT LoadResource(string language="default",string addr="localhost");

SVAPI_API
OBJECT CreateResource(string language);

SVAPI_API 
MAPNODE GetResourceNode(OBJECT rcobj);

SVAPI_API 
bool	EnumResourceAttrib(OBJECT rcobj,PAIRLIST &retlist);

SVAPI_API
bool	CloseResource(OBJECT &rcobj);//ɾ�����������ڴ�й©

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
void CloseMonitor(OBJECT &monitorobj);//ɾ�����������ڴ�й©

SVAPI_API
string AddNewMonitor(OBJECT monitorobj,string entityid,string user="default",string addr="localhost");

SVAPI_API 
bool	SubmitMonitor(OBJECT monitorobj,string user="default",string addr="localhost");

SVAPI_API 
bool GetAllMonitorsInfo(PAIRLIST &retlist,string infoname="sv_label",string user="default",string addr="localhost");
								// infoname����"ObjectVersion"�򷵻� obj �汾��,  user ����ָ���� id 

SVAPI_API
string MonitorCopy(string srcmonitorid,string objentityid,string user="default",string addr="localhost");

SVAPI_API
string MonitorCopyAndCreateTable(string srcmonitorid,string objentityid,string user="default",string addr="localhost");
//�������������������Ӧ�� table


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
bool CloseEntity(OBJECT &entityobj);//ɾ�����������ڴ�й©

SVAPI_API
bool SubmitEntity(OBJECT entityobj,string user="default",string addr="localhost");

SVAPI_API 
bool GetAllEntitysInfo(PAIRLIST &retlist,string infoname="sv_label",string user="default",string addr="localhost");
									// infoname����"ObjectVersion"�򷵻� obj �汾��,  user ����ָ���� id 

SVAPI_API
string EntityCopy(string srcentityid,string objgroupid,string user="default",string addr="localhost");

SVAPI_API
string EntityCopyAndCreateTable(string srcentityid,string objgroupid,string user="default",string addr="localhost");
//�����豸���������м��������������Ӧ�� table


//////////////////////////////Group////////////////////////////////////////////////
SVAPI_API
string  CreatIdcUser(string userindex, string pid="0", string addr="localhost");
// ����һ�� idc �û�   �� userindex �� cgi����Ҫ�õ���һ�� �û�index��������ֻ���� cgi �����Զ�������һ����
// pid ��idc �û��� se ���׵� id �� �� pid=="0" ʱ ��ϵͳ�Զ�Ѱ�Һ��ʵ� se ����ӣ�  �� pid!="0" ʱ �ɵ�����ָ��

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

pid Ϊ1����Ϊ��id��Ϊ1��se

*/
SVAPI_API
string AddNewGroup(OBJECT groupobj,string pid="1",string user="default",string addr="localhost");

SVAPI_API
bool CloseGroup(OBJECT &groupobj);//ɾ�����������ڴ�й©

SVAPI_API
bool SubmitGroup(OBJECT groupobj,string user="default",string addr="localhost");

SVAPI_API 
bool GetAllGroupsInfo(PAIRLIST &retlist,string infoname="sv_label",string user="default",string addr="localhost");
								// infoname����"ObjectVersion"�򷵻� obj �汾��,  user ����ָ���� id 

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
void CloseTask(OBJECT &taskobj);//ɾ�����������ڴ�й©

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
bool CloseEntityGroup(OBJECT &egobj);//ɾ�����������ڴ�й©

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
bool CloseEntityTemplet(OBJECT &etobj);//ɾ�����������ڴ�й©

SVAPI_API 
bool GetAllEntityTemplets(PAIRLIST &retlist,string infoname="sv_Name",string user="default",string addr="localhost");

SVAPI_API 
bool GetAllEntityTempletsInProperty(string key,string value,std::list<string> &idlist,string user="default",string addr="localhost");

SVAPI_API
OBJECT CloneEntityTemplet(OBJECT etobj,string entitytempletid);




/////////////////////////////////////  cache����  ////////////////////////////////////////

// ���� 14 �������Ὣ��ȡ���� object �����ڱ��أ��ٴε���ʱ������ͬһ�����á�
// ��ر�֤�������� 13 ��������õ� object ,ִ�� CloseXXX(object); ��Ϊ��һ֮���ڽ����������ط����ø� object,���ᵼ�¡��������������
SVAPI_API
void SetCacheUserAddr(string user="default",string addr="localhost");
//�˺�����ͬʱ����������� 13 ������ (��CacheRefreshSVDYNs 2 ����) �����ı��ػ���,�˺������̰߳�ȫ

//���� 5 ��������ȡ���� object �ķ�������������
//���� 5 ���������λ�ȡ��ʱ�伫�̣��������ɲ���
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

//���� 4 ��������ÿ�ε���ʱ��ѯ�ʷ�����������֤��ȡ���°汾�� object ��
//���� 4 ���������λ�ȡ���ٶȱ��Ϻ�����1-3��
SVAPI_API
OBJECT	Cache_GetSVSE(string id);

SVAPI_API
OBJECT	Cache_GetGroup(string groupid);

SVAPI_API
OBJECT	Cache_GetEntity(string entityid);

SVAPI_API
OBJECT	Cache_GetMonitor(string monitorid);




//����2����������ʹ�ã��״λ�ȡ���ٶȱ��Ϻ����� �� �������λ�ȡ�� ʮ�� �� 
SVAPI_API
bool	CacheRefreshEntities(string parentid="default");
//Ϊ parentid ���������� entity �ӷ���������ˢ�����°汾

SVAPI_API
OBJECT	CacheRefresh_GetEntity(string entityid);
//���ر��ػ���� object (��ѯ�ʷ�����)



//����2����������ʹ�ã��״λ�ȡ���ٶȱ��Ϻ����� �� �������λ�ȡ�� ʮ�� �� 
SVAPI_API
bool	CacheRefreshMonitors(string parentid="default");
//Ϊ parentid ���������� monitor �ӷ���������ˢ�����°汾

SVAPI_API
OBJECT	CacheRefresh_GetMonitor(string monitorid);
//���ر��ػ���� object (��ѯ�ʷ�����)






//////////////////////////////////////db//////////////////////////////////////////////

//////////////////////////////////////dyn/////////////////////////////////////////////








//////////////////////////////////end///////////////////////////////////////////////




#endif