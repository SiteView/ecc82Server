#pragma once
#include  "WTable"

class WText;
class WSVButton :public WTable
{
public:
	WSVButton(WContainerWidget * parent, const std::string strTitle, const std::string strImg, const std::string strTip="",  const bool bVip = false);
	virtual ~WSVButton(void);
public :
	WText *pBtnTxt;
};
