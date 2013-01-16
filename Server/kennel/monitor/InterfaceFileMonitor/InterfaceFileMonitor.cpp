

//该工程由 SiteViewTOOL 在 创建新监测器 时自动生成


//****************************************************************************
//	Monitor Export Function
//  本工程必须设置为 MD 编译 （项目属性\ C/C++ \代码生成\运行时库：）
//****************************************************************************
#include <afx.h>
#include <string>
#include <list>
#include <iostream>
#include <fstream>
using std::string;
using std::list;
using namespace std;
#pragma warning(disable: 4311)

bool MakeStringListByChar(CStringList& pList, const char * pInput );	//将输入参数拆入一个 list<string> 中
bool MakeCharByString(char *pOut,int &nOutSize,CString strInput );		//按要求输出返回值，将 $ 替换为 \0,  以实现 \0 分割 和  \0\0 为结束标志 的要求


#define	__InterfaceFileName__	"_InterfaceFileName="

int WriteLog(char* str)
{
//#ifndef _DEBUG
//	return 0;
//#endif
	
	char timeBuf[128], dateBuf[128], wyt[4096];
	
	_tzset();
	
	_strtime( timeBuf );
	_strdate( dateBuf );
	
    sprintf(wyt, "%s %s\t", dateBuf, timeBuf );
	
	FILE* log;
		
	log = fopen("InterfaceFileMonitor.log", "a+");
	
	if(log != NULL)
	{
		strcat(wyt, str);
		fprintf(log, "%s\n", wyt);
		fclose(log);
	}
	
	return 0;
}


//strParas 为输入参数， 以 \0 分割若干个参数， 以 \0\0 为结束标志 
//szReturn 为返回值， 以 \0 分割若干个返回值， 以 \0\0 为结束标志  例如 key1=value1\0key2=value2\0\0
//nSize 为返回值的 size


extern "C" __declspec(dllexport)
bool InterfaceFileMonitor(const char * strParas, char * szReturn, int& nSize)   
{	
//////////////////////////////////////////////////////////////////////
	
	CString	GetFileName = _T("");//接收要打开的文件名
	
//////////////////////////////////////////////////////////////////////
	ifstream read_from_file;    //从用户文件里面读内容
//////////////////////////////////////////////////////////////////////
	int symbol;
	long IntValue1,IntValue2;
	//char * IntValue1,*IntValue2;
	char * StringValue1,* StringValue2; //用户文件里面的各变量的参数名；
//////////////////////////////////////////////////////////////////////
	
	CStringList paramList;
	MakeStringListByChar(paramList,strParas); //将输入参数拆入一个 list<string> 中

///////////////////////////////////////////////////
	POSITION pos=paramList.GetHeadPosition();
	while(pos)
	{
		CString strTmp=_T("");
		strTmp = paramList.GetNext(pos);
		if(0 == strTmp.Find(__InterfaceFileName__))
		{
			GetFileName = strTmp.Right(strTmp.GetLength() - (int)strlen(__InterfaceFileName__));
		}
	}
	char * FileName = new char[100];
	FileName=GetFileName.GetBuffer();
//	WriteLog(FileName);
	GetFileName.ReleaseBuffer();
	char * FileBuf = new char[300];
	read_from_file.open(FileName);
	if(!read_from_file.is_open())
	{
//		WriteLog("打开文件失败！");
		sprintf(szReturn,"IntValue1=%d%s\0",1000,"打开文件失败！");
		return false;
	}
//	WriteLog("打开文件成功！");
	read_from_file.getline(FileBuf,100);	
	read_from_file.close();	
///////////////////////////////////////////////////////////////////
	char * ch=",";
	char * ptr[10];
	char * temp = strtok(FileBuf,ch);
	symbol=_ttoi(temp);
	int i=0;
	while(temp != NULL)
	{
		temp = strtok(NULL,ch);
		ptr[i]=temp;
		i++;
	}
//////////////////////////////////////////////////////////////////
	IntValue1=_ttoi(ptr[0]);
	IntValue2=_ttoi(ptr[1]);
	StringValue1=ptr[2];
	StringValue2=ptr[3];
//	WriteLog("获取值成功！");
	CString Input;
    sprintf(szReturn,"Symbol=%d$IntValue1=%d$IntValue2=%d$StringValue1=%s$StringValue2=%s$",symbol,IntValue1,IntValue2,StringValue1,StringValue2);
	Input = szReturn;
	char * sizePtr = szReturn;
	while(*sizePtr)
	{
		nSize+=(int)strlen(sizePtr)+1;
		sizePtr+=strlen(sizePtr)+1;
	}
	nSize+=(int)strlen(sizePtr)+1;

	MakeCharByString(szReturn,nSize,Input);	
	delete FileName;
	delete FileBuf;
/*	char * temPtr=szReturn;
	while(*temPtr)
	{
		WriteLog(temPtr);
		temPtr+=strlen(temPtr)+1;
	}
*/
	return true;
}
//将输入参数拆入一个 list<string> 中
bool MakeStringListByChar(CStringList& pList, const char * pInput )
{
	const char * p;
	int nSize;
	p=pInput;
	while(*p!='\0')
	{
		nSize = static_cast<int>(strlen(p));
		if( nSize>0 )
		{	
			pList.AddHead(p);
		//	printf(p);
		}
		p=p+nSize+1;
	}

	return true;
}// end of MakeStringListByChar



//按要求输出返回值，将 $ 替换为 \0,  以实现 \0 分割 和  \0\0 为结束标志 的要求
bool MakeCharByString(char *pOut,int &nOutSize,CString strInput )
{
	 char *p;
	
	int nSize=strInput.GetLength();
	if(nSize+2 <nOutSize)
	{
		strcpy(pOut,strInput.GetBuffer(strInput.GetLength()));
	}
	else 
		return false;
	p=pOut;
	for(int i=0;i<nSize;i++)
	{
		if(*p=='$') 	
			*p='\0';
		p++;
	}
	nOutSize=nSize+1;
	return true;
	
}//end of MakeCharByString
