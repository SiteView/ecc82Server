#ifndef SVDB_NNM2ECC_
#define SVDB_NNM2ECC_

#include "SvdbMain.h"
#include "Entity.h"
#include "EntityEx.h"
#include "Monitor.h"
#include "svse.h"
#include "group.h"
#include "writeLog.h"

#define NNMGROUP "自动扫描设备组"
class SvdbMain;
class CNNM2ECC
{
public:
	CNNM2ECC(void);
	~CNNM2ECC(void);

	// EntityEx提交至ECC
	bool SubmitEntityExToEcc(EntityEx* pEx);
	bool DeleteEccEntity(word id);
	bool AddMonitor(Entity* pEn);
	bool InitNNMGroup();
	bool InitNNMSubGroup();

private:
	bool addNewGroup(string svname,svutil::word & putid,string parentid);
	
	svutil::word m_nnmGroupid;
	svutil::word m_nnmSubGroupid[7];
	cWriteLog *myWriteLog;


};


#endif