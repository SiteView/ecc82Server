#ifndef	SVDB_UTIL_
#define	SVDB_UTIL_
#include "svdbtype.h"
#include "./svapi/svdbapi.h"

using namespace std;

std::string TrimSpace(const std::string & input);

std::string TruncateToUId(std::string id);

bool MoreThan2Dot(const std::string & id);

word GetParentID(word id);

int GetIndexByID(word id);

bool IsTopID(word id);

word GetTopID(word id);

word MakeIDByIndex(word pid,int index);

void outputRegeditKey(void);

std::string GetRootPath();

S_UINT GetMassRecordListRawDataSize(std::list<SingelRecord> & listrcd);
char*   GetMassRecordListRawData(std::list<SingelRecord> & listrcd,char *lpbuf,S_UINT bufsize);
bool	CreateMassRecordListByRawData(std::list<SingelRecord> & listrcd,const char *lpbuf,S_UINT bufsize);

S_UINT GetStrListRawDataSize(std::list<std::string> &strlist);
char*   GetStrListRawData(std::list<std::string> &strlist,char *lpbuf,S_UINT bufsize);
bool	CreateStrListByRawData(std::list<std::string> &strlist,const char *lpbuf,S_UINT bufsize);

int ConvertType(std::string strtype);

///////////////////////////db buffer///////////////

char *buildbuffer(int data,char *pt,int buflen);

char *buildbuffer(float data,char *pt,int buflen);

char *buildbuffer(std::string data,char *pt,int buflen);

int parsebuffer(const char *buf,int buflen,int &ret);

int parsebuffer(const char *buf,int buflen,float &ret);

int parsebuffer(const char *buf,int buflen,std::string &ret);

void AddToErrorLog(string error);

///////////////////////////  dyn  ///////////////
bool builddyn(const char *buf,int datalen,SVDYN &dyn,bool hasstr);

#endif