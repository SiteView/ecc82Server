//Commons.h
//

#pragma once

#define FALSE 0 
#define TRUE 1

#define false 0 
#define true 1

#define MAX_PATH 260

typedef struct
{
	int Num;
	int bChecked;
	char name[40];
	char shortName[20];
}MODULEITEM;

typedef struct
{
	char ProductName[33];//产品名称
	char Customer[200];//客户名称
	char UserName[200];//用户ID；（试用时为trial）[200]
	char Support[20];//技术支持
	char RegCode[1024];//注册码；（试用时为特定的编码）[50]
	int Devices;//设备数；[20]
	int Points;//监控点数；[30]
	char SysFeatures[2048];//用户系统特征码；（系统的安装时间、全部网卡的MAC地址的综合），以逗号隔开
	char StartDate[9];//开始试用日期；
	char MakeDate[9];//生成日期；
	int Delaydays;//允许试用天数；
	int DelayTimes;//当前延期次数；
	long TotalTime;//当前累积运行时间
	int Allowmachine;//允许更换机器
	int Isfinal; //是否正式版
	int Isdefault; //是否默认
	char Tokens[64];//tokens 
	char Modules[2048];//可用模块列表:以逗号隔开	
}LICENSEDATA;