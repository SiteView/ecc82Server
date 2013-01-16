#include <stdio.h>
#include <string.h>

#include <scasvapi.h>


#pragma once

class CDllCall
{
public:
	CDllCall(void);
public:
	~CDllCall(void);
public:
	bool GetUnivData(ForestMap & fmap,const NodeData & inwhat, string & estr);
	bool GetUnivData();
	ForestMap Slice_SubmitUnivData(const NodeData & inwhat, ForestMap fmap, string & estr, bool & bret);
public:
	ForestMap d_fmap;
	ForestList d_flist;
	NodeData d_inwhat;
	string d_estr;
};
