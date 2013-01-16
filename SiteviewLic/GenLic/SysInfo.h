//SysInfo.h
//

#pragma once

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>


#include"iphlpapi.h"

#pragma comment(lib,"Iphlpapi.lib")

#include <string>

using namespace std;


class SysInfo
{
public:
	SysInfo(void);
public:
	~SysInfo(void);

public:
	static bool GetInstallTime(char* instime);
	static bool GetInstallDate(char* insdate);
	static bool GetSysFeature(char* feature);
	static bool newGUID(char* buf);
	static void GetCurrentDate(char* date);
	static double DiffDate(const string date1, const string date2);

};
