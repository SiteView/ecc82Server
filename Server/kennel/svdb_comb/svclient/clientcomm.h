
#ifndef	SITEVIEW_CLIENTCOMM_H
#define	SITEVIEW_CLIENTCOMM_H


#include "svbase.h"


namespace svbase{

	class CSvTableComm : public CSvTable
	{
	public:
		CSvTableComm(void);
		~CSvTableComm(void);

		string	GetTableType();
		bool	SetTableType(string type);

		bool	InitializeDataMembers(CTableName tname, string & estr);
		void    SetSubmitted();

		S_UINT	GetRawDataSize();
		char*	GetRawData(char *lpbuf,S_UINT bufsize);	
		bool	CreateByRawData(const char *lpbuf,S_UINT bufsize); // will clear all data, then reset all
	};


} // end of namespace svbase

using namespace svbase;


#endif


