#include "WSPopButton.h"

#include "WText"
#include "WImage"
#include "WTableCell"

WSPopButton::WSPopButton(WContainerWidget * parent, const std::string strTitle, const std::string strImg,
					 const std::string strTip ,const bool bVip )
:WTable(parent)
{
	if(parent)
	{
		parent->setStyleClass("padding_3");
	}
	
	setStyleClass("button_img_font");
	std::string  strFullPathImg;
	strFullPathImg ="/Images/"+strImg;
	
	if(!bVip)
	{
		WImage * pTmpLimage = new WImage("/Images/space.gif","",(WContainerWidget *) elementAt(0,0));
		pTmpLimage->resize(WLength(20), WLength());

		elementAt(0,0)->setStyleClass("pop_but_l");
		//strcpy(elementAt(0,0)->contextmenu_,"nowrap");

		

		pBtnTxt =new WText(strTitle,(WContainerWidget *)elementAt(0,1));
		elementAt(0,1)->setStyleClass("pop_but_m");
		strcpy(elementAt(0,1)->contextmenu_,"nowrap");
		if(!strTip.empty())
			pBtnTxt->setToolTip(  strTip);

		WImage * pTmpRimage = new WImage("/Images/space.gif","",(WContainerWidget *)elementAt(0,2));
		pTmpRimage->resize(WLength(20), WLength());

		elementAt(0,2)->setStyleClass("pop_but_r");
		//strcpy(elementAt(0,2)->contextmenu_,"nowrap");
	}
	else
	{
		WImage * pTmpLimage = new WImage("/Images/space.gif","",(WContainerWidget *) elementAt(0,0));
		pTmpLimage->resize(WLength(20), WLength());

		elementAt(0,0)->setStyleClass("pop_but_l");
		//strcpy(elementAt(0,0)->contextmenu_,"nowrap");

		

		pBtnTxt =new WText(strTitle,(WContainerWidget *)elementAt(0,1));
		elementAt(0,1)->setStyleClass("pop_but_m");
		strcpy(elementAt(0,1)->contextmenu_,"nowrap");
		if(!strTip.empty())
			pBtnTxt->setToolTip(  strTip);

		WImage * pTmpRimage = new WImage("/Images/space.gif","",(WContainerWidget *)elementAt(0,2));
		pTmpRimage->resize(WLength(20), WLength());

		elementAt(0,2)->setStyleClass("pop_but_r");
		//strcpy(elementAt(0,2)->contextmenu_,"nowrap");
	}
}

void WSPopButton::setEnabled(bool disable)
{
	if(disable)
		strcpy( this->contextmenu_, "");
	else
		strcpy( this->contextmenu_, "disabled");
}

void WSPopButton::setText(const std::string text)
{
	pBtnTxt->setText(text);
}

WSPopButton::~WSPopButton(void)
{
}
