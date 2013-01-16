#ifndef  DRAGONFLOW_APPENDMASSRECORD
#define DRAGONFLOW_APPENDMASSRECORD

#include "threadex.h"

class CAppendMassRecord :
	public ThreadEx
{
public:
	virtual void run(void);
	CAppendMassRecord();
	virtual ~CAppendMassRecord();

};

#endif
