

#ifndef	SITEVIEW_CONNECTION_H
#define	SITEVIEW_CONNECTION_H

#include "svdbtype.h"
#include "svclient.h"
#include "svdbapi.h"


class CConnect
{
	public:
		CConnect(void){}
		~CConnect(void){}

		bool ConnectDataBase(MapStrMap & fmap, string qstr, char * & retp, int & retlen, string & estr);
		bool ConnectDataBaseCommit(MapStrMap & fmap, std::list<SingelRecord> & listrcd, string qstr, char * & retp, int & retlen, string & estr);

	private:

};



#endif