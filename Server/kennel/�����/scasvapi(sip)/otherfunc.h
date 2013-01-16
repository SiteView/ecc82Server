
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
// 下列函数用于华为设备
//----------------------------------------------------------
bool ValueChange(ForestMap& fmap,  const NodeData& inwhat, string& estr);
bool Boot(ForestMap& fmap,  const NodeData& inwhat, string& estr);
bool GetParameter(ForestMap& fmap,  const NodeData& inwhat, string& estr);
bool SetParameter(ForestMap& fmap,  const NodeData& inwhat, string& estr);
bool GetAlertID(ForestMap& fmap,  const NodeData& inwhat, string& estr);

bool MassMoveEntity(ForestMap& fmap,  const NodeData& inwhat, string& estr);

bool SetSingleParameter(ForestMap& fmap,  const NodeData& inwhat, string& estr);

bool SaveConditionToMTP(ForestMap& fmap,  const NodeData& inwhat, string& estr);

bool GetSipIsdn(ForestMap& fmap,  const NodeData& inwhat, string& estr);

bool CanAddSipMonitor( string strEntityId, string strIsdn, string strMonitorType, string& estr );

bool MassAddMonitor(ForestMap& fmap,  const NodeData& inwhat, string& estr);

bool MassGetSipIsdn(ForestMap& fmap,  const NodeData& inwhat, string& estr);

bool CpeTransferComplete(ForestMap &fmap , const NodeData &inwhat , string &estr);

#endif



