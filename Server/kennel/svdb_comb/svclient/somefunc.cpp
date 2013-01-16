
#include "somefunc.h"



//�� StrMap �в���ֵ
string GetValueInStrMap(const StrMap & inwhat,string key)
{
	try{
		StrMap::const_iterator nit= inwhat.find(key);	
		if(nit!=inwhat.end())
			return nit->second;
	}
	catch(...)
	{
		return "";
	}
	return "";
}

//���� StrMap ,�Ḳ�����е� key == value
void PutValueInStrMap(StrMap & inwhat,string key,string value)
{
	StrMap::iterator nit= inwhat.find(key);	
	if( nit==inwhat.end() )
		inwhat.insert(std::make_pair(key,value));
	else
		nit->second= value;		
}

//�� MapStrMap �в���ֵ
string GetValueInMapStrMap(const MapStrMap & fmap, string section, string key)
{
	MapStrMap::const_iterator mit= fmap.find(section);
	if(mit != fmap.end())
	{
		StrMap::const_iterator nit= mit->second.find(key);	
		if(nit!=mit->second.end())
			return nit->second;
	}
	return "";
}


//���÷��ص� MapStrMap, �Ḳ�����е� key == value
void  PutReturnMapStrMap(MapStrMap & fmap, string section, string key, string value)
{
	MapStrMap::iterator mit= fmap.find(section);
	if(mit==fmap.end())
	{
		StrMap ndata;
		fmap.insert(std::make_pair(section, ndata));
		mit= fmap.find(section);
	}
	StrMap::iterator nit= mit->second.find(key);	
	if( nit==mit->second.end() )
		mit->second.insert(std::make_pair(key,value));
	else
		nit->second= value;
}

//���� MapStrMap �е� StrMap
StrMap & GetStrMap(MapStrMap & fmap, string section)
{
	MapStrMap::iterator mit= fmap.find(section);
	if(mit==fmap.end())
	{
		StrMap ndata;
		fmap.insert(std::make_pair(section, ndata));
		mit= fmap.find(section);
	}
	return mit->second;
}




