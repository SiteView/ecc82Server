#pragma once
#include  "WTable"
#include <string>

class WText;
class WSPopButton :public WTable
{
public:
	WSPopButton(WContainerWidget * parent, const std::string strTitle, const std::string strImg, const std::string strTip="",  const bool bVip = false);
	virtual ~WSPopButton(void);
private:
	WText *pBtnTxt;
public :
	void setEnabled(bool disable);
	void setText(const std::string text);
};
