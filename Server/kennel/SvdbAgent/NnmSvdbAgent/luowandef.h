
#ifndef LUOWAN_DEF_H
#define LUOWAN_DEF_H
#pragma once   
//typedef int (*F_TLOADINIFILE)(char *filename);
typedef int (*F_TSTARTGETDEVICEINFO)(TreadParameter readParameter, int &rowNo, int &colNo, TmibData *myMibTable);
typedef int (*F_TWRITEMIB)(TwriteParameter writeParatmeter);
//typedef int  (*F_TREADDEVICEINFO)(Tmibtable *tmpMibTable);
//typedef int  (*F_TREADDEVICEINFOEX)(int index, char *oid, char *value);
//const std::string STR_TLOADINIFILE = "TloadIniFile";
const std::string STR_TWRITEMIB = "TwriteMib";
const std::string STR_TSTARTGETDEVICEINFO = "TstartGetDeviceInfo";
//const std::string STR_TREADDEVICEINFO = "TreadDeviceInfo";
//const std::string STR_TREADDEVICEINFOEX = "TreadDeviceInfoEX";
const std::string DLL_READDEVICEINFO = "readDeviceInfo.dll";
#endif