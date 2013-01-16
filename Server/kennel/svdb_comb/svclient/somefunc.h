
#ifndef	SITEVIEW_SOMEFUNC_H
#define	SITEVIEW_SOMEFUNC_H

#include "svbase.h"



//在 StrMap 中查找值
string GetValueInStrMap(const StrMap & inwhat,string key);

//设置 StrMap ,会覆盖已有的 key == value
void PutValueInStrMap(StrMap & inwhat,string key,string value);

//在 MapStrMap 中查找值
string GetValueInMapStrMap(const MapStrMap & fmap, string section, string key);

//设置返回的 MapStrMap, 会覆盖已有的 key == value
void  PutReturnMapStrMap(MapStrMap & fmap, string section, string key, string value);

//返回 MapStrMap 中的 StrMap
StrMap & GetStrMap(MapStrMap & fmap, string section);



#endif



