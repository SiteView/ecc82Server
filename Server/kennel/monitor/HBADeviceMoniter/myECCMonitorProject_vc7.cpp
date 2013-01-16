

//�ù����� SiteViewTOOL �� �����¼���� ʱ�Զ�����


//****************************************************************************
//	Monitor Export Function
//  �����̱�������Ϊ MD ���� ����Ŀ����\ C/C++ \��������\����ʱ�⣺��
//****************************************************************************

#include <string>
#include <list>
using std::string;
using std::list;

bool MakeStringListByChar(list<string> & pList, const char * pInput );	//�������������һ�� list<string> ��
bool MakeCharByString(char *pOut,int &nOutSize,string strInput );		//��Ҫ���������ֵ���� $ �滻Ϊ \0,  ��ʵ�� \0 �ָ� ��  \0\0 Ϊ������־ ��Ҫ��



//strParas Ϊ��������� �� \0 �ָ����ɸ������� �� \0\0 Ϊ������־ 
//szReturn Ϊ����ֵ�� �� \0 �ָ����ɸ�����ֵ�� �� \0\0 Ϊ������־  ���� key1=value1\0key2=value2\0\0
//nSize Ϊ����ֵ�� size
extern "C" __declspec(dllexport) bool getHBAPortState(const char * strParas, char * szReturn, int& nSize)   
{	
// �ڴ�������ļ��������
	
	list<string> paramList;
	MakeStringListByChar(paramList,strParas); //�������������һ�� list<string> ��
	char buf[512];
	string ret;
	for(int i=0; i<=3; ++i )  
	{
	 // ...
	     ret +=buf;  //����д�뷵��ֵ
	     ret +="$";
	}
	ret+="$";
	int len=4000; char outtemp[4096]={0};
	MakeCharByString(outtemp,len, ret) ; //��Ҫ���������ֵ���� $ �滻Ϊ \0,  ��ʵ�� \0 �ָ� ��  \0\0 Ϊ������־ ��Ҫ��
	memcpy(szReturn,outtemp,len);
	nSize= len;

	return true;
}


//�������������һ�� list<string> ��
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



//��Ҫ���������ֵ���� $ �滻Ϊ \0,  ��ʵ�� \0 �ָ� ��  \0\0 Ϊ������־ ��Ҫ��
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
