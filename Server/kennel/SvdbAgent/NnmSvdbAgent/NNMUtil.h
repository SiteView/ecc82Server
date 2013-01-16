#ifndef SVDB_NNMUTIL_
#define SVDB_NNMUTIL_

#include <iostream>
using namespace std;

#ifdef WIN32
#include "windows.h"
#endif

#include "writeLog.h"

class NNMUtil
{
public:
	NNMUtil(void);
	~NNMUtil(void);

	static bool RunScanNetProcess(std::string sdir,std::string cmdline,PROCESS_INFORMATION &pi,bool asy=false);

private:
	cWriteLog *myWriteLog;

};
#endif
