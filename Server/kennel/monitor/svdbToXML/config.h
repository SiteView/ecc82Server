#ifndef _CONFIG
#define _CONFIG

#include <string>
#include "tinyxml.h"

using namespace std;

class CConfig
{
public:
	explicit CConfig(const char* xmlFileName)
		:mXmlConfigFile(xmlFileName),mRootElem(0)
	{
		//���������ļ�
		mXmlConfigFile.LoadFile();    

		//�õ������ļ��ĸ����
		mRootElem=mXmlConfigFile.RootElement();
	}

public:
	//�õ�nodeName����ֵ
	string GetValue(const string& nodeName) const;
	//�޸�nodeName����ֵ
	bool SetValue(const string& nodeName, const string & newValue, bool saveFile = false);

private:
	//��ֹĬ�Ϲ��캯��������
	CConfig();

private:
	TiXmlDocument    mXmlConfigFile;
	TiXmlElement*    mRootElem;

};

#endif