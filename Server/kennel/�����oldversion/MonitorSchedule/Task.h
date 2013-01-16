#ifndef		DRAGONFLOW_TASK_
#define		DRAGONFLOW_TASK_
#include "MonitorSchedule.h"
#include <vector>

struct PairTime{
	PairTime()
	{
		m_beginhour=0;
		m_beginminute=0;
		m_endhour=0;
		m_endminute=0;

	}
	int	m_beginhour;
	int m_beginminute;
	int  m_endhour;
	int m_endminute;
};
typedef struct PairTime PAIRTIME,*LPPAIRTIME;

struct TaskWeekDay{

	std::vector< PAIRTIME > m_task;
	bool m_enable;
};

typedef struct TaskWeekDay	TASKWEEKDAY,*LPTASKWEEKDAY;

class Task
{
public:
	Task(void);
	~Task(void);
	enum ttype{ TASK_NULL=0,TASK_ABSOLUTE=1,TASK_RELATIVE=2 };

	int m_type;
	TASKWEEKDAY m_week[7];
	string m_taskname;

	bool ToTextMap(STRINGMAP &strmap);
	bool FromTextMap(STRINGMAP &strmap);
};

typedef svutil::hashtable<svutil::word,Task* > TASKMAP;

#endif
