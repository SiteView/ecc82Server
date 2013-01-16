


#include "appendmassrecord.h"
#include "MonitorSchedule.h"
#include <svdbapi.h>
#include <svdbtype.h>



extern Util *putil;

CAppendMassRecord::CAppendMassRecord()
{
}

CAppendMassRecord::~CAppendMassRecord()
{
	std::list<SingelRecord> listrcd;
	putil->AppendThenClearAllRecords(listrcd);
}

void CAppendMassRecord::run()
{
	while(true)
	{
		char szTemp[1024] = {0};
		std::list<SingelRecord> listrcd;
		int count(0);
		if( (count=putil->AppendThenClearAllRecords(listrcd))>0 )
		{
			cout<<"AppendMass "<<count<<" records done,"<<" slept "<<Univ::msappend<<" ms."<<endl;
			sprintf( szTemp, "AppendMass %d records done, slept %d ms.\n", count, Univ::msappend );
			OutputDebugString( szTemp );
		}
			
		else if(count<0)
		{
			cout<<"AppendMassRecord failed!"<<endl;
			OutputDebugString( "AppendMassRecord failed!\n" );
		}
		ThreadEx::sleep(Univ::msappend);	
	}
}