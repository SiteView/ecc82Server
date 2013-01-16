

//该工程由 SiteViewTOOL 在 创建新监测器 时自动生成


//****************************************************************************
//	Monitor Export Function
//  本工程必须设置为 MD 编译 （项目属性\ C/C++ \代码生成\运行时库：）
//****************************************************************************

#include <string>
#include <list>
using std::string;
using std::list;

bool MakeStringListByChar(list<string> & pList, const char * pInput );	//将输入参数拆入一个 list<string> 中
bool MakeCharByString(char *pOut,int &nOutSize,string strInput );		//按要求输出返回值，将 $ 替换为 \0,  以实现 \0 分割 和  \0\0 为结束标志 的要求



//strParas 为输入参数， 以 \0 分割若干个参数， 以 \0\0 为结束标志 
//szReturn 为返回值， 以 \0 分割若干个返回值， 以 \0\0 为结束标志  例如 key1=value1\0key2=value2\0\0
//nSize 为返回值的 size
extern "C" __declspec(dllexport) bool getHBAPortState(const char * strParas, char * szReturn, int& nSize)   
{	
// 在此添加您的监测器代码
	
	list<string> paramList;
	MakeStringListByChar(paramList,strParas); //将输入参数拆入一个 list<string> 中
	char buf[512];
	string ret;
	for(int i=0; i<=3; ++i )  
	{
	 // ...
	     ret +=buf;  //依次写入返回值
	     ret +="$";
	}
	ret+="$";
	int len=4000; char outtemp[4096]={0};
	MakeCharByString(outtemp,len, ret) ; //按要求输出返回值，将 $ 替换为 \0,  以实现 \0 分割 和  \0\0 为结束标志 的要求
	memcpy(szReturn,outtemp,len);
	nSize= len;

	return true;
}


//将输入参数拆入一个 list<string> 中
bool MakeStringListByChar(list<string> & pList, const char * pInput )
{
	const char * p;
	int nSize;
	p=pInput;
	while(*p!='\0')
	{
		nSize =strlen(p);
		if( nSize>0 )
			pList.push_back(p);
		p=p+nSize+1;
	}

	return true;
}// end of MakeStringListByChar



//按要求输出返回值，将 $ 替换为 \0,  以实现 \0 分割 和  \0\0 为结束标志 的要求
bool MakeCharByString(char *pOut,int &nOutSize,string strInput )
{
	char *p;

	int nSize=strInput.size();
	if(nSize+2 <nOutSize)
		strcpy(pOut,strInput.c_str());
	else 
		return false;
	p=pOut;
	//printf("%d\n",nSize);
	for(int i=0;i<nSize;i++)
	{
		if(*p=='$') *p='\0';
		p++;
	}
	nOutSize=nSize+1;
	return true;
	
}//end of MakeCharByString
