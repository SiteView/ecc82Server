//SysInfo.h
//

#pragma once



#define FALSE 0 
#define TRUE 1

#define false 0 
#define true 1

int GetInstallTime(char* instime);
int GetInstallDate(char* insdate);
//int GetSysFeature(char* feature);
int GetInstallTime1(const char* fileName, char* instime);
int GetInstallDate1(const char* fileName, char* insdate);
int GetSysFeature1(const char* fileName, char* feature);
int newGUID(char* buf);
void GetCurrentDate(char* date);
double DiffDate(const char* date1, const char* date2);
int SplitFilePath(const char* fullName, char* path, char* filename);


