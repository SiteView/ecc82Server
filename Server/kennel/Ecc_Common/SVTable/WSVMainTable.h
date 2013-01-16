#pragma once

#include "WTable"

//class WSVFlexTable;
class WText;
class WImage;
class WSVMainTable : public WTable
{
public:
	WSVMainTable(WContainerWidget * parent, const std::string strTitle, const bool bHelp_ = false);
	virtual ~WSVMainTable(void);
private:
	WTable *pMainTable;
	bool bHelp;
public :
	//WTable *pContentTable;
	//WSVFlexTable * GetContentTable(std::string strFlexTitle);
	WTable *GetContentTable();
	//WSVFlexTable *pThreeTable;
	WImage *pHelpImg;
	WText * pTitleTxt;
	bool HaveHelp();
};