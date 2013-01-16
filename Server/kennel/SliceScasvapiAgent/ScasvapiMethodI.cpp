#include <scasvapimethodi.h>
#include <scasvapi.h>
#include <Ice/Ice.h>
#include "DllCall.h"
using namespace sca_svapi;
using namespace std;
ForestMap n_ifmap;

std::string StringToUtf8(std::string astr)
{
	wchar_t wbuf[1024]={0};

	std::string srt;
	std::wstring wst;
	std::string stt=astr;
	int count=::MultiByteToWideChar(CP_ACP,0,astr.c_str(),(int)astr.size(),wbuf,0);
	if(count>=1024)
	{
		wchar_t *pwbuf=new wchar_t[count+1];
		if(pwbuf==NULL)
		{
			puts("Allow memory failed");
			return "";
		}
		memset(pwbuf,0,count*sizeof(wchar_t)+sizeof(wchar_t));

		MultiByteToWideChar(CP_ACP,0,astr.c_str(),(int)astr.size(),pwbuf,count+1);
		wst=pwbuf;
		delete [] pwbuf;
		srt=IceUtil::wstringToString(wst);
	}else
	{
		count=MultiByteToWideChar(CP_ACP,0,astr.c_str(),(int)astr.size(),wbuf,1024);
		srt=IceUtil::wstringToString(wbuf);
	}

	return srt;

}

std::string Utf8ToString(std::string ustr)
{
	std::wstring wst=IceUtil::stringToWstring(ustr);
	std::string srt;

	char buf[1024]={0};
	int count=::WideCharToMultiByte(CP_ACP,0,wst.c_str(),(int)wst.size(),buf,0,NULL,NULL);
//	printf("count:%d\n",count);
	if(count>=1024)
	{
		char *pbuf=new char[count+1];
		if(pbuf==NULL)
		{
			puts("Allow memory failed");
			return "";
		}
		memset(pbuf,0,count+1);
		count=WideCharToMultiByte(CP_ACP,0,wst.c_str(),(int)wst.size(),pbuf,count+1,NULL,NULL);
		srt=pbuf;
		delete [] pbuf;
		return srt;
		
	}else
	{
		count=WideCharToMultiByte(CP_ACP,0,wst.c_str(),(int)wst.size(),buf,1024,NULL,NULL);
		srt=buf;
	}

	return srt;

}

void outputForestMap(ForestMap & fmap,int max=99999999)
{
	return;
	ForestMap::iterator fit;
	NodeData::iterator nit;
	int count(0);
	for(fit=fmap.begin(); fit!=fmap.end();  ++fit)
	{
		if(count>max)
			break;

		cout<<"\nnode:  "<<fit->first.c_str()<<", NO: "<<++count<<endl;
		for(nit=fit->second.begin(); nit!=fit->second.end(); ++nit)
			cout<<nit->first.c_str()<<" = "<<nit->second.c_str()<<endl;
	}
	cout<<"\nnode total: "<<fmap.size()<<"\n\n\n\n"<<endl;
}


void outputForestMap1(ForestMap & fmap,int max=99999999)
{
	ForestMap::iterator fit;
	NodeData::iterator nit;
	int count(0);
	for(fit=fmap.begin(); fit!=fmap.end();  ++fit)
	{
		if(count>max)
			break;

		cout<<"\ outputForestMap1 nnode:  "<<fit->first.c_str()<<", NO: "<<++count<<endl;
		for(nit=fit->second.begin(); nit!=fit->second.end(); ++nit)
			cout<<nit->first.c_str()<<" = "<<nit->second.c_str()<<endl;
	}
	cout<<"\outputForestMap1 nnode total: "<<fmap.size()<<"\n\n\n\n"<<endl;
}

void outputForestList(ForestList & flist, bool showdepends=false)
{
	//return;
	ForestList::iterator fit;
	NodeData::iterator nit;
	int count(0);
	string estr2;
	for(fit=flist.begin(); fit!=flist.end();  ++fit)
	{
		++count;
		
		if(	showdepends && GetValueInNodeData(*fit,"sv_dependson_svname",estr2).empty() )
			continue;
		cout<<"\nnode NO: "<<count<<endl;

		for(nit=fit->begin(); nit!=fit->end(); ++nit)
			cout<<nit->first.c_str()<<" = "<<nit->second.c_str()<<endl;
	}
	cout<<"\nnode total: "<<flist.size()<<"\n\n\n\n"<<endl;
}

void ScasvapiMethodI::clearForestMap(ForestMap& fmap)
{
	//return;
	ForestMap::iterator fit;
	NodeData::iterator nit;
	int count(0);
	for(fit=fmap.begin(); fit!=fmap.end();  ++fit)
	{
		if(!(*fit).second.empty())
			(*fit).second.clear();
		//fmap[(*fit).first.c_str()] = NULL

		//fmap.erase(fit)
		//if(!fit->second.empty())
		//	fit->second.clear();
		//nit->clear();
		//for(nit=fit->second.begin(); nit!=fit->second.end(); ++nit)
		//	nit->clear();
	}
	
//	fmap.empty();
	fmap.clear();
}

void ScasvapiMethodI::clearForestList(ForestList& fmap)
{
	//return;
	ForestList::iterator fit;
	NodeData::iterator nit;
	int count(0);
	for(fit=fmap.begin(); fit!=fmap.end();  ++fit)
	{
		if(!(*fit).empty())
		{
			//(*fit).erase((*fit).begin(), (*fit).end());
			
			//for(nit=fit->begin(); nit!=fit->end(); ++nit)				
				//(*fit).erase(nit);

			fmap.pop_front();
				
			//	cout<<nit->first.c_str()<<" = "<<nit->second.c_str()<<endl;
		}
		//fmap[(*fit).first.c_str()] = NULL;

		//fmap.erase(fit)
		//if(!fit->second.empty())
		//	fit->second.clear();
		//nit->clear();
	}
	
//	fmap.empty();
	fmap.clear();
}

ScasvapiMethodI::ScasvapiMethodI(void)
{
}

ScasvapiMethodI::~ScasvapiMethodI(void)
{
	n_ifmap.clear();	
	n_inwhat.clear();
//	::SliceClearForestMap(n_fmap);
//	::SliceClearForestMap(n_ofmap);	
//	::SliceClearForestList(n_flist);
}

bool ScasvapiMethodI::SliceGetUnivData(const ::SliceScasvapi::SNodeData& s_inwhat, ::SliceScasvapi::SForestMap& s_fmap, ::std::string& s_estr, const ::Ice::Current& c)
{	
//	s_fmap.clear();

//	 dllInvoke = new CDllCall();
	::SliceScasvapi::SNodeData::const_iterator it;
	for( it = s_inwhat.begin(); it != s_inwhat.end(); it++ )
	{
		//inwhat[(it->first)] = (it->second);
		n_inwhat.insert(std::make_pair(Utf8ToString(it->first.c_str()), Utf8ToString(it->second.c_str())));
		//cout<<"SliceGetUnivData IN:"<<Utf8ToString(it->first.c_str())<<Utf8ToString(it->second.c_str())<<endl;
		//dllInvoke->d_inwhat.insert(std::make_pair(it->first.c_str(), it->second.c_str()));
	}


	//inwhat["dowhat"] = "GetMonitor";
	//inwhat["id"] = "1.26.19.1";
	bool bret = false;
	
//	bret = dllInvoke->GetUnivData(fmap, inwhat, estr);
//	dllInvoke->d_inwhat = n_inwhat;
//	bret = dllInvoke->GetUnivData();
//	delete dllInvoke;

	bret = ::GetUnivData(n_fmap, n_inwhat, n_estr);
	
//	ForestMap t_fmap;
//	t_fmap = ::Slice_GetUnivData(n_inwhat, n_estr, bret);
	//t_map.clear();
	
	/*
	::SliceScasvapi::SNodeData nodedata;
	nodedata["dd"] = "dd";
	s_fmap["ddd"] = nodedata;
	s_estr = "ds";
	*/
	
	ForestMap::const_iterator mit = n_fmap.begin();
	//ForestMap::const_iterator mit = dllInvoke->d_fmap.begin();
	
	//char szTemp[1024*5] = {0}; 
	while( mit != n_fmap.end() )
	//while( mit != dllInvoke->d_fmap.end() )		
	{
		NodeData::const_iterator nit = mit->second.begin();

		//sprintf( szTemp, "%s:\n", mit->first.c_str() );
		::SliceScasvapi::SNodeData nodedata;
		while( nit != mit->second.end() )
		{
			//if( (strlen(szTemp)+nit->first.size()+nit->second.size()) < sizeof(szTemp) )
			{
				//sprintf( szTemp, "%s%s=%s\n", szTemp, nit->first.c_str(), nit->second.c_str() );
				//nodedata[(nit->first.c_str())] = (nit->second.c_str());
				nodedata.insert(std::make_pair(StringToUtf8(nit->first.c_str()), StringToUtf8(nit->second.c_str())));
			}
			
			nit++;
		}
		//WriteLog( szTemp );

		s_fmap[StringToUtf8(mit->first.c_str())] = nodedata;
		
		mit++;
	}

	s_estr = StringToUtf8(n_estr);
	n_estr = "";
	//s_estr = dllInvoke->d_estr;

	n_inwhat.clear();	

	outputForestMap(n_fmap);
	::SliceClearForestMap(n_fmap);
		
	//clearForestMap(n_fmap);	
	/*if(!n_fmap.empty())
	{
		n_fmap.erase(n_fmap.begin(), n_fmap.end());
		n_fmap.clear();
	}*/
	
	//delete dllInvoke;

	/*ForestMap n_fmap1;
	NodeData nodedata;
	nodedata["dddccc"] = "sdfas";
	nodedata["ddcc"] = "ddddsdfas";
	n_fmap1["ddd"] = nodedata;
	n_fmap1.clear();*/

	return bret;
}

bool ScasvapiMethodI::SliceSubmitUnivData(const ::SliceScasvapi::SNodeData& s_inwhat, const ::SliceScasvapi::SForestMap& s_ifmap,::SliceScasvapi::SForestMap& s_ofmap, ::std::string& s_estr, const ::Ice::Current& c)
{
	//n_ifmap.clear();
	//ForestMap n_ifmap;
	::SliceScasvapi::SNodeData::const_iterator it;
	for( it = s_inwhat.begin(); it != s_inwhat.end(); it++ )
	{
		//inwhat[(it->first)] = (it->second);
		n_inwhat.insert(std::make_pair(Utf8ToString(it->first.c_str()), Utf8ToString(it->second.c_str())));
	}

//	ForestMap n_ifmap;
	::SliceScasvapi::SForestMap::const_iterator s_mit = s_ifmap.begin();
	//char szTemp[1024*5] = {0}; 
	while( s_mit != s_ifmap.end() )
	{
		::SliceScasvapi::SNodeData::const_iterator s_nit = s_mit->second.begin();

		//sprintf( szTemp, "%s:\n", mit->first.c_str() );
		NodeData nodedata;
		while( s_nit != s_mit->second.end() )
		{
			//if( (strlen(szTemp)+nit->first.size()+nit->second.size()) < sizeof(szTemp) )
			{
				//sprintf( szTemp, "%s%s=%s\n", szTemp, nit->first.c_str(), nit->second.c_str() );
				//nodedata[(nit->first.c_str())] = (nit->second.c_str());
				nodedata.insert(std::make_pair(Utf8ToString(s_nit->first.c_str()),Utf8ToString(s_nit->second.c_str())));
			}
			
			s_nit++;
		}
		//WriteLog( szTemp );

		//n_ifmap[Utf8ToString(s_mit->first.c_str())] = nodedata;
		n_ifmap.insert(std::make_pair(Utf8ToString(s_mit->first.c_str()),nodedata));
		
		s_mit++;
	}

	//outputForestMap1(n_ifmap);
	//inwhat["dowhat"] = "GetMonitor";
	//inwhat["id"] = "1.26.19.1";
	bool bret = false;
	//CDllCall dllInvoke;
	
	
	//bret = ::SubmitUnivData(n_ifmap, n_inwhat, n_estr);
	//ForestMap n_ofmap = ::Slice_SubmitUnivData(n_inwhat, n_ifmap, n_estr, bret);
	//ForestMap n_ofmap = dllInvoke.Slice_SubmitUnivData(n_inwhat, n_ifmap, n_estr, bret);
	
	//outputForestMap1(n_ifmap);

	::Slice_SubmitUnivData(n_inwhat, n_ifmap, n_ofmap, n_estr, bret);

	ForestMap::const_iterator mit = n_ofmap.begin();
	//char szTemp[1024*5] = {0}; 
	while( mit != n_ofmap.end() )
	{
		NodeData::const_iterator nit = mit->second.begin();

		//sprintf( szTemp, "%s:\n", mit->first.c_str() );
		::SliceScasvapi::SNodeData nodedata;
		while( nit != mit->second.end() )
		{
			//if( (strlen(szTemp)+nit->first.size()+nit->second.size()) < sizeof(szTemp) )
			{
				//sprintf( szTemp, "%s%s=%s\n", szTemp, nit->first.c_str(), nit->second.c_str() );
				//nodedata[(nit->first.c_str())] = (nit->second.c_str());
				nodedata.insert(std::make_pair(StringToUtf8(nit->first.c_str()),StringToUtf8(nit->second.c_str())));
			}
			
			nit++;
		}
		//WriteLog( szTemp );

		//s_ofmap[StringToUtf8(mit->first.c_str())] = nodedata;
		s_ofmap.insert(std::make_pair(StringToUtf8(mit->first.c_str()), nodedata));
		
		mit++;
	}

	s_estr = StringToUtf8(n_estr);
	n_estr = "";
	 
	n_inwhat.clear();
	::SliceClearForestMap(n_ofmap);
	n_ifmap.clear();	
	//n_ifmap.clear();
	//::SliceClearForestMap(n_ofmap);
	
	//outputForestMap(n_fmap);
	//clearForestMap(n_fmap);
	//n_fmap.empty();
	//n_fmap.clear();

	return bret;
}

//bool ScasvapiMethodI::SliceGetForestData(const ::SliceScasvapi::SNodeData&, ::SliceScasvapi::SForestList&, ::std::string&, const ::Ice::Current& c)
bool ScasvapiMethodI::SliceGetForestData(const ::SliceScasvapi::SNodeData& s_inwhat, ::SliceScasvapi::SForestList& s_flist, ::std::string& s_estr, const ::Ice::Current& c)
{
//	s_flist.clear();

//	if(!n_flist.empty())
//		n_flist.clear();
//	ForestList n_flist;
//	ForestList* n_flist = new ForestList();

	::SliceScasvapi::SNodeData::const_iterator it;
	for( it = s_inwhat.begin(); it != s_inwhat.end(); it++ )
	{
		//inwhat[(it->first)] = (it->second);
		n_inwhat.insert(std::make_pair(Utf8ToString(it->first.c_str()), Utf8ToString(it->second.c_str())));
		//cout<<"SliceGetForestData IN:"<<Utf8ToString(it->first.c_str())<<Utf8ToString(it->second.c_str())<<endl;
	}

	bool bret = false;
	bret = ::GetForestData(n_flist, n_inwhat, n_estr);
	//n_flist = ::Slice_GetForestData(n_inwhat, n_estr, bret);
	//bret = ::GetForestData((*n_flist), n_inwhat, n_estr);
	
	cout<<"SliceGetForestData:"<<n_flist.size()<<endl;
	//cout<<"SliceGetForestData:"<<n_flist->size()<<endl;
	ForestList::const_iterator pForest = n_flist.begin();
	//ForestList::const_iterator pForest = n_flist->begin();
	
	//char strTemp[1024] = {0};
	while(pForest != n_flist.end())
	//while(pForest != n_flist->end())
	{
		NodeData::const_iterator  pNode	= pForest->begin();
		::SliceScasvapi::SNodeData nodedata;
		while(pNode != pForest->end())
		{
			//sprintf(strTemp , "%s=%s" , pNode->first.c_str() , pNode->second.c_str());			
			//nodedata[StringToUtf8(pNode->first.c_str())] = StringToUtf8(pNode->second.c_str());
			nodedata.insert(std::make_pair(StringToUtf8(pNode->first.c_str()), StringToUtf8(pNode->second.c_str())));
			//WriteLog(strTemp);
			pNode++;
		}
		s_flist.push_back(nodedata);
		//nodedata.clear();
		pForest++;
	}

	s_estr = StringToUtf8(n_estr);
	n_estr = "";
	
	
	n_inwhat.clear();
	::SliceClearForestList(n_flist);
//	n_flist.clear();
//	delete n_flist; 
//	n_flist	->clear();
	//outputForestList(n_flist);
//	n_flist.remove()
	/*for(int i = 0; i < n_flist.size(); i++)
	{
		n_flist.pop_front();
	}*/

	//clearForestList(n_flist);	
	//n_flist.erase(n_flist.begin(), n_flist.end());
	//n_flist.clear();		

	return bret;
}

void ScasvapiMethodI::shutdown(const ::Ice::Current& c)
{
	c.adapter->getCommunicator()->shutdown();
}
