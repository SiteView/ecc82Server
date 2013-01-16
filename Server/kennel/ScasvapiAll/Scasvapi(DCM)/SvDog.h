#pragma once


#include <list>
#include <vector>
#include <string> 
using std::string;
using std::vector;
using std::list;


typedef struct
{
	int Num;
	bool flag;
	char name[40];
	char shortName[20];
}MDLITEM;

typedef struct
{
	long Devices;//设备数
	long Points;//监控点数
	char StopDate[9];//到期日期,格式为yyyymmdd
	MDLITEM Modules[32];//可用模块列表
}DATA_DOG;


const MDLITEM modules_ecc[]={
	{0,true,"整体视图", "WHOLEVIEW"},
	{1,true,"树状视图", "TREEVIEW"},
	{2,true,"监测器浏览", "MONITORBROWSE"},
	{3,true,"监测器设置", "MONITORSET"},
	{4,false,"拓扑视图", "TUOPUVIEW"},
	{5,false,"拓扑扫描", "TUOPUSCAN"},
	{6,false,"工单", "TASKPLAN"},
	{7,true,"报表", "REPORT"},
	{8,true,"告警", "ALARM"},
	{9,true,"设置", "CONFIG"}
};

const MDLITEM modules_nnm[]={
	{0,true,"整体视图", "WHOLEVIEW"},
	{1,true,"树状视图", "TREEVIEW"},
	{2,true,"拓扑视图", "TUOPUVIEW"},
	{3,true,"拓扑扫描", "TUOPUSCAN"},
	{4,true,"报表", "REPORT"},
	{5,true,"告警", "ALARM"},
	{6,true,"设置", "CONFIG"},
	{7,false,"流量分析", "FA"}
};

const MDLITEM modules_dm[]={
	{0,true,"远程桌面管理", "REMOTEDM"},
	{1,true,"软件分发", "RELEASE"},
	{2,true,"资产报告", "ASSETREPORT"},
	{3,true,"软件控制", "SOFTCONTROL"},
	{4,true,"未管理设备搜寻", "DEVICEFIND"},
	{5,true,"查询", "SEATCH"},
	{6,true,"申请维护", "MAINTAPPLY"},
	{7,true,"系统设置", "SYSCONFIG"},
	{8,true,"网络访问控制", "NETACCESSCNTRL"},
	{9,true,"合同管理", "CONTRACTMANAGE"},
	{10,true,"补丁管理", "SPMANAGE"},
	{11,true,"计算机资产报表", "COMPUTERRPT"},
	{12,true,"设备维修记录", "DEVICEREPAIR"},
	{13,true,"设备远程控制", "REMOTECNTRL"},
	{14,true,"应用程序监视", "APPMONITOR"}
};



class SvDog
{
public:
	SvDog(void);
	~SvDog(void);

	// 读狗
	// 0- 成功，否则失败
	// product - "ECC", "NNM", "DM"
	// dataDog -成功时，是读出的值
	int SvReadDog(const char* product , DATA_DOG* dataDog);

};
