//
//

#include "stdafx.h"
/*
bool isNumeric(const char* str)
{
	for(;!(*str==0);str++)
	{
//		if(!isdigit(*str) && *str != '.')
		if(!isdigit(*str))
			return false;
	}
	return true;
}

//trimָʾ�Ƿ����մ���Ĭ��Ϊ������
vector<string> tokenize(const string& src, string tok, bool trim, string null_subst)
{
	vector<string> v;
	if( src.empty() || tok.empty() )
	{
		return v;
//		throw "tokenize: empty string\0";
	}
	S_T pre_index = 0, index = 0, len = 0;
	while( (index = src.find(tok, pre_index)) != npos )
//	while( (index = src.find_first_of(tok, pre_index)) != npos )
	{
		if( (len = index-pre_index)!=0 )
		v.push_back(src.substr(pre_index, len));
		else if(trim==false)
		v.push_back(null_subst);
		pre_index = index+ tok.length();
//		pre_index = index+1;
	}
	string endstr = src.substr(pre_index);
	if( trim==false ) v.push_back( endstr.empty()? null_subst:endstr );
	else if( !endstr.empty() ) v.push_back(endstr);
	return v;
}*/

