
#include "somefunc.h"



//在 StrMap 中查找值
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

//设置 StrMap ,会覆盖已有的 key == value
void PutValueInStrMap(StrMap & inwhat,string key,string value)
{
	StrMap::iterator nit= inwhat.find(key);	
	if( nit==inwhat.end() )
		inwhat.insert(std::make_pair(key,value));
	else
		nit->second= value;		
}

//在 MapStrMap 中查找值
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


//设置返回的 MapStrMap, 会覆盖已有的 key == value
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

//返回 MapStrMap 中的 StrMap
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




