#pragma once
#include <slicescasvapi.h>
#include <scasvapi.h>
#include "DllCall.h"
class ScasvapiMethodI :
	public SliceScasvapi::ScasvapiMethod
{
public:
	ScasvapiMethodI(void);
	virtual ~ScasvapiMethodI(void);

    virtual bool SliceGetUnivData(const ::SliceScasvapi::SNodeData&, ::SliceScasvapi::SForestMap&, ::std::string& , const ::Ice::Current& c);    
    virtual bool SliceSubmitUnivData(const ::SliceScasvapi::SNodeData&, const ::SliceScasvapi::SForestMap&, ::SliceScasvapi::SForestMap&, ::std::string&, const ::Ice::Current& c);
    virtual bool SliceGetForestData(const ::SliceScasvapi::SNodeData&, ::SliceScasvapi::SForestList&, ::std::string&, const ::Ice::Current& c);
    virtual void shutdown(const ::Ice::Current& c);
	void clearForestMap(ForestMap&  fmap);
	void clearForestList(ForestList&  fmap);
public:
	ForestMap n_fmap;
	ForestMap n_ifmap;
	ForestMap n_ofmap;
	ForestList n_flist;
	NodeData n_inwhat;
	string n_estr;
	CDllCall dllInvoke;
    
};
