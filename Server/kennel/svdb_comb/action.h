

#ifndef	SITEVIEW_ACTION_H
#define	SITEVIEW_ACTION_H

#include "svbase.h"
#include "svdbtype.h"
#include "svdbapi.h"
#include "SvdbMain.h"

//  listrcd.begin()->monitorid ÓÃÓÚ±£´æ estr
S_UINT ReactConnection(SvdbMain *m_pMain, string qstr, const char *data,S_UINT datalen, std::list<SingelRecord> & listrcd);


#endif




