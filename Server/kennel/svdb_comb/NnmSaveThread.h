#ifndef SVDB_NNMSAVETHREAD_H
#define SVDB_NNMSAVETHREAD_H


#include "ThreadEx.h"
#include "SvdbMain.h"

class SvdbMain;

class NnmSaveThread :
	public ThreadEx
{
public:
	NnmSaveThread();
	~NnmSaveThread(void);

	void toExit(void);

	bool Init();
	void run(void);

    SvdbMain * m_pMain;

};

#endif
