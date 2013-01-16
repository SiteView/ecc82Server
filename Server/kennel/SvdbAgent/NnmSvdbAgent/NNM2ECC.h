#ifndef SVDB_NNM2ECC_
#define SVDB_NNM2ECC_

#include "SvdbMain.h"
#include "Entity.h"
#include "EntityEx.h"
#include "Monitor.h"
#include "svse.h"
#include "group.h"
#include "writeLog.h"

#define NNMGROUP "�Զ�ɨ���豸��"
class SvdbMain;
class CNNM2ECC
{
public:
	CNNM2ECC(void);
	~CNNM2ECC(void);

	// EntityEx�ύ��ECC
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