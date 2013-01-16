
#ifndef	SITEVIEW_SOMEFUNC_H
#define	SITEVIEW_SOMEFUNC_H

#include "svbase.h"



//�� StrMap �в���ֵ
string GetValueInStrMap(const StrMap & inwhat,string key);

//���� StrMap ,�Ḳ�����е� key == value
void PutValueInStrMap(StrMap & inwhat,string key,string value);

//�� MapStrMap �в���ֵ
string GetValueInMapStrMap(const MapStrMap & fmap, string section, string key);

//���÷��ص� MapStrMap, �Ḳ�����е� key == value
void  PutReturnMapStrMap(MapStrMap & fmap, string section, string key, string value);

//���� MapStrMap �е� StrMap
StrMap & GetStrMap(MapStrMap & fmap, string section);



#endif



