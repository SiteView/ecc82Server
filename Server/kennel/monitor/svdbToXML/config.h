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
		//加载配置文件
		mXmlConfigFile.LoadFile();    

		//得到配置文件的根结点
		mRootElem=mXmlConfigFile.RootElement();
	}

public:
	//得到nodeName结点的值
	string GetValue(const string& nodeName) const;
	//修改nodeName结点的值
	bool SetValue(const string& nodeName, const string & newValue, bool saveFile = false);

private:
	//禁止默认构造函数被调用
	CConfig();

private:
	TiXmlDocument    mXmlConfigFile;
	TiXmlElement*    mRootElem;

};

#endif