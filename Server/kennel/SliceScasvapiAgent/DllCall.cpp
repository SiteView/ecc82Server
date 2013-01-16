
#include "DllCall.h"
using namespace sca_svapi;

CDllCall::CDllCall(void)
{
}

CDllCall::~CDllCall(void)
{
	if(!d_fmap.empty())
		d_fmap.clear();
	d_flist.clear();
	d_inwhat.clear();
	
}

bool CDllCall::GetUnivData(ForestMap & fmap,const NodeData & inwhat, string & estr)
{
	return ::GetUnivData(fmap, inwhat, estr);
}

bool CDllCall::GetUnivData()
{
	bool bret =  ::GetUnivData(this->d_fmap, this->d_inwhat, this->d_estr);
	this->d_fmap.clear();
	this->d_inwhat.clear();

	return bret;
}

ForestMap CDllCall::Slice_SubmitUnivData(const NodeData & inwhat, ForestMap fmap, string & estr, bool & bret)
{
	ForestMap fmaptemp;
	ForestMap::const_iterator mit = fmap.begin();

	char szTemp[1024*5] = {0}; 
	while( mit != fmap.end() )
	{
		NodeData nodedata;
		NodeData::const_iterator nit = mit->second.begin();

		sprintf( szTemp, "%s:\n", mit->first.c_str() );
		while( nit != mit->second.end() )
		{
			//if( (strlen(szTemp)+nit->first.size()+nit->second.size()) < sizeof(szTemp) )
			//{
				//sprintf( szTemp, "%s%s=%s\n", szTemp, nit->first.c_str(), nit->second.c_str() );
			//}
			nodedata.insert(std::make_pair((nit->first.c_str()),(nit->second.c_str())));

			
			nit++;							
		}
		//WriteLog( szTemp );
		fmaptemp.insert(std::make_pair(mit->first.c_str(), nodedata));

		mit++;
	}	
	bret = SubmitUnivData(fmaptemp, inwhat, estr);

	ForestMap fmaptemp1;
	mit = fmaptemp.begin();

	//char szTemp[1024*5] = {0}; 
	while( mit != fmaptemp.end() )
	{
		NodeData nodedata;
		NodeData::const_iterator nit = mit->second.begin();

		sprintf( szTemp, "%s:\n", mit->first.c_str() );
		while( nit != mit->second.end() )
		{
			//if( (strlen(szTemp)+nit->first.size()+nit->second.size()) < sizeof(szTemp) )
			//{
				//sprintf( szTemp, "%s%s=%s\n", szTemp, nit->first.c_str(), nit->second.c_str() );
			//}
			nodedata.insert(std::make_pair((nit->first.c_str()),(nit->second.c_str())));

			
			nit++;							
		}
		//WriteLog( szTemp );
		fmaptemp1.insert(std::make_pair(mit->first.c_str(), nodedata));

		mit++;
	}	
//	fmap.clear();
//	fmaptemp.clear();
	return fmaptemp1;
}
