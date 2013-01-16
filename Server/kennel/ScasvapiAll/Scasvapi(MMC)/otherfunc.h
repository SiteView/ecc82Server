
#ifndef	SITEVIEW_SCASVAPI_OTHERFUNC_H
#define	SITEVIEW_SCASVAPI_OTHERFUNC_H


#include <string>
#include "svapi.h"
#include "svdbapi.h"


void SetNodeNumNetworkNum(string nodenum, string networknum);
bool CheckLicenseIsOk();
bool GetForestDataToCheckPoint(string nodenum, string networknum);


bool QueryRecordsByTime(ForestList & fmap,  const NodeData & inwhat, string & estr);
bool QueryRecordsByCount(ForestList & fmap,  const NodeData & inwhat, string & estr);
bool QueryAlertLog(ForestList & fmap,  const NodeData & inwhat, string & estr);

bool QuerySVSE(ForestMap & fmap,  const NodeData & inwhat, string & estr);
bool QueryGroup(ForestMap & fmap,  const NodeData & inwhat, string & estr);
bool QueryEntity(ForestMap & fmap,  const NodeData & inwhat, string & estr);
bool QueryMonitor(ForestMap & fmap,  const NodeData & inwhat, string & estr);

bool GetTreeData2(ForestList & flist, const NodeData & inwhat, string & estr);

bool FilterForestData(ForestList & flist, const NodeData & inwhat, string & estr);
bool SetChildSumForestData(ForestList & flist, const NodeData & inwhat, string & estr);
bool SetBeyondLicenseForestData(ForestList & flist);
bool TryToLogin(ForestMap & fmap,  const NodeData & inwhat, string & estr, bool & isTrial);

bool encrypt(ForestMap & fmap,  const NodeData & inwhat, string & estr);
bool decrypt(ForestMap & fmap,  const NodeData & inwhat, string & estr);

bool GetDynamicData(ForestMap & fmap,  const NodeData & inwhat, string & estr);
bool RefreshMonitors(ForestMap & fmap,  const NodeData & inwhat, string & estr);
bool GetLatestRefresh(ForestMap & fmap,  const NodeData & inwhat, string & estr);
bool GetRefreshed(ForestMap & fmap,  const NodeData & inwhat, string & estr);

bool TryDecryptValue(ForestMap & fmap, string section, string type, string & estr, bool isDecrypt);
bool CanAddMorePoint(const string & entityid, string & estr, string action="", int monitorAddNum=1);
bool CheckMonitorBelongToETpl(string monitorid, string entityid, string & estr, string sv_monitortype="");

bool TestEntity(ForestMap & fmap,  const NodeData & inwhat, string & estr);
bool GetEntityDynamicData(ForestMap & fmap,  const NodeData & inwhat, string & estr);


bool CheckNetWork(string sv_devicetype);
bool SetSv_intpos(string id, OBJECT	obj);


bool GetMonitorConditon(string id, OBJECT objMonitor, string & monitorName , string & monitorType , string & monitorFreq,  string & errorCon, string & warnCon, string & okCon );
bool QueryReportData(ForestMap & fmap,  const NodeData & inwhat, string & estr);
bool QueryAllMonitorInfo(ForestMap & fmap,  const NodeData & inwhat, string & estr);
bool QueryInfo(ForestMap & fmap,  const NodeData & inwhat, string & estr);

bool AdvaceAddManyMonitor(ForestMap & fmap,  const NodeData & inwhat, string & estr);
bool AddManyMonitor(ForestMap & fmap,  const NodeData & inwhat, string & estr);
bool AppendOperateLog(ForestMap & fmap,  const NodeData & inwhat, string & estr);
bool BuildMonitor(OBJECT obj, string pid, string value, string & estr);


bool SmsTest(ForestMap & fmap,  const NodeData & inwhat, string & estr);
bool EmailTest(ForestMap & fmap,  const NodeData & inwhat, string & estr);

int  DirDllName(NodeData & ndata);
bool SmsTestByDll(ForestMap & fmap,  const NodeData & inwhat, string & estr);

bool DeleteRecords(ForestMap & fmap,  const NodeData & inwhat, string & estr);


bool WriteOrDelIniFileData(int isDel, ForestMap & fmap,  const NodeData & inwhat, string & estr);



bool GetAllView(ForestMap & fmap,  const NodeData & inwhat, string & estr);
bool AddView(ForestMap & fmap,  const NodeData & inwhat, string & estr);
bool DeleteView(ForestMap & fmap,  const NodeData & inwhat, string & estr);
bool DeleteViewItem(ForestMap & fmap,  const NodeData & inwhat, string & estr);

bool AddViewItem(ForestMap & fmap,  const NodeData & inwhat, string & estr);

bool SetValueInManyMonitor(ForestMap & fmap,  const NodeData & inwhat, string & estr);

bool GetViewData(ForestList & flist, const NodeData & inwhat, string & estr);


//----------------------------------------------------------
// 下列函数用于dcm设备
//----------------------------------------------------------
void WriteLog( const char* str );// 打印日志
bool AddToDcmServer( ForestMap& fmap, string& estr );// 在dcm服务器上同步增加设备
bool ModToDcmServer( ForestMap& fmap, const PAIRLIST dcmPairList, bool bEntity, string& estr);// 在dcm服务器上同步编辑设备
bool SetDcmServer( ForestMap& fmap, const PAIRLIST dcmPairList, string& estr );// 设置dcm管理节点属性

//-------------------------------
// 同步删除dcm设备
// 0 成功不删除
// 1 删除成功
// 2 该节点下不存在dcm设备
//-------------------------------
int DelFromDcmServer( string id );

bool DelDCMEntity( string strId, string& estr );
bool DelDCMGroup( string strId, string& estr );
bool DelDCMSVSE( string strId, string& estr );

string CopyEntity( string strSourceId, string strTargetParentId, string& estr );
string CopyEntityAndCreateTable( string strSourceId, string strTargetParentId, string& estr );

bool AddActiveAlert( ForestMap& fmap,  const NodeData& inwhat, string& estr);
bool HandleActiveAlert( ForestMap& fmap,  const NodeData& inwhat, string& estr);
bool GetActiveAlert( ForestMap& fmap,  const NodeData& inwhat, string& estr);

//----------------------------------------------------------
// 下列函数用于url监测器的步数设置
//----------------------------------------------------------
bool StepUrlSetup( ForestMap& fmap,  const NodeData& inwhat, string& estr);
bool EditUrlStep( ForestMap& fmap,  const NodeData& inwhat, string& estr);
bool AddUrlStep( ForestMap& fmap,  const NodeData& inwhat, string& estr);
bool SaveUrlStep( ForestMap& fmap,  const NodeData& inwhat, string& estr);
bool DelUrlStep( ForestMap& fmap,  const NodeData& inwhat, string& estr);
//----------------------------------------------------------
// 增加接口：查询指定序列的报警日志数据，不限制记录条数
// 增加人：邹晓
// 增加时间：2009.06.04
//----------------------------------------------------------
bool QueryMassAlertLog(ForestList & fmap,  const NodeData & inwhat, string & estr);

//-------------------------------------------------------------
//用于中组部项目中，将SQL中的数据写入SVDB中去。
bool CreateGroupInSVDB(ForestMap &fmap , const NodeData &inwhat , string &estr);
bool CreateEntityInSVDB(ForestMap &fmap , const NodeData &inwhat , string &estr);
bool CreateMonitorInSVDB(ForestMap &fmap , const NodeData &inwhat , string &estr);

bool AddGroupInSVDB(ForestMap &fmap , const NodeData &inwhat , string &estr);
bool AddEntityInSVDB(ForestMap &fmap , const NodeData &inwhat , string &estr);
bool AddMonitorInSVDB(ForestMap &fmap , const NodeData &inwhat , string &estr);

bool RemoveGroupInSVDB(ForestMap &fmap , const NodeData &inwhat , string &estr);
bool RemoveEntityInSVDB(ForestMap &fmap , const NodeData &inwhat , string &estr);
bool RemoveMonitorInSVDB(ForestMap &fmap , const NodeData &inwhat , string &estr);

bool RemoveEmptyGroup(string estr);			//删除空的组
bool RemoveEmptyEntity(string estr);		//删除空的设备
bool RemoveEmptyMonitor(string estr);		//删除空的监测器

bool GetMonitorReturnItem(ForestMap &fmap , const NodeData &inwhat , string &estr);
//用于中组部项目中，将SQL中的数据写入SVDB中去。
//-------------------------------------------------------------
bool TryToLogin_NoPassWord(ForestMap & fmap,  const NodeData & inwhat, string & estr , bool & isTrial);//中组部项目中，不输入密码来访问

#endif



