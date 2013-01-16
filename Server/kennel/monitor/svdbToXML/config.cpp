#include "config.h"
#include <iostream>

string CConfig::GetValue(const string& nodeName) const
{
	if(!mRootElem)
	{
		cout<<"��ȡ��������"<<endl;
		return "";
	}

	TiXmlElement* pElem=mRootElem->FirstChildElement(nodeName.c_str());
	if(!pElem)
	{
		cout<<"��ȡ"<<nodeName<<"������"<<endl;
		return "";
	}

	return pElem->GetText();

}

bool CConfig::SetValue(const string& nodeName, const string & newValue, bool saveFile)
{
	if(!mRootElem)
	{
		cout<<"��ȡ��������"<<endl;
		return false;
	}

	TiXmlHandle rootHandle(mRootElem);
	TiXmlText * pText = rootHandle.FirstChildElement(nodeName).FirstChild().ToText();

	if(!pText)
	{
		cout<<"��ȡ"<<nodeName<<"������"<<endl;
		return false;
	}

	pText->SetValue(newValue);
	if (saveFile)
	{
		mXmlConfigFile.SaveFile();
	}

	return true;
}
