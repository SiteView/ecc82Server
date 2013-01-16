#include "config.h"
#include <iostream>

string CConfig::GetValue(const string& nodeName) const
{
	if(!mRootElem)
	{
		cout<<"读取根结点出错"<<endl;
		return "";
	}

	TiXmlElement* pElem=mRootElem->FirstChildElement(nodeName.c_str());
	if(!pElem)
	{
		cout<<"读取"<<nodeName<<"结点出错"<<endl;
		return "";
	}

	return pElem->GetText();

}

bool CConfig::SetValue(const string& nodeName, const string & newValue, bool saveFile)
{
	if(!mRootElem)
	{
		cout<<"读取根结点出错"<<endl;
		return false;
	}

	TiXmlHandle rootHandle(mRootElem);
	TiXmlText * pText = rootHandle.FirstChildElement(nodeName).FirstChild().ToText();

	if(!pText)
	{
		cout<<"读取"<<nodeName<<"结点出错"<<endl;
		return false;
	}

	pText->SetValue(newValue);
	if (saveFile)
	{
		mXmlConfigFile.SaveFile();
	}

	return true;
}
