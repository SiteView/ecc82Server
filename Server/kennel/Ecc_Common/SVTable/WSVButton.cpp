#include "wsvbutton.h"

#include "WText"
#include "WImage"
#include "WTableCell"

WSVButton::WSVButton(WContainerWidget * parent, const std::string strTitle, const std::string strImg,
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
		new WImage("/Images/space.gif","",(WContainerWidget *) elementAt(0,0));
		elementAt(0,0)->setStyleClass("but_img_bg_l_blue");
		strcpy(elementAt(0,0)->contextmenu_,"nowrap");

		new WImage(strFullPathImg,"",(WContainerWidget *)elementAt(0,1));

		pBtnTxt =new WText(strTitle,(WContainerWidget *)elementAt(0,2));
		elementAt(0,2)->setStyleClass("but_img_bg_m_blue");
		strcpy(elementAt(0,2)->contextmenu_,"nowrap");

		if(!strTip.empty())
			pBtnTxt->setToolTip(  strTip);

		new WImage("/Images/space.gif","",(WContainerWidget *)elementAt(0,3));
		elementAt(0,3)->setStyleClass("but_img_bg_r_blue");
		strcpy(elementAt(0,3)->contextmenu_,"nowrap");
	}
	else
	{
		new WImage("/Images/space.gif","",(WContainerWidget *) elementAt(0,0));
		elementAt(0,0)->setStyleClass("but_img_bg_l_black");
		strcpy(elementAt(0,0)->contextmenu_,"nowrap");

		
		new WImage(strFullPathImg,"",(WContainerWidget *)elementAt(0,1));

		pBtnTxt =new WText(strTitle,(WContainerWidget *)elementAt(0,2));
		elementAt(0,2)->setStyleClass("but_img_bg_m_black");
		strcpy(elementAt(0,2)->contextmenu_,"nowrap");
		if(!strTip.empty())
			pBtnTxt->setToolTip(  strTip);

		new WImage("/Images/space.gif","",(WContainerWidget *)elementAt(0,3));
		elementAt(0,3)->setStyleClass("but_img_bg_r_black");
		strcpy(elementAt(0,3)->contextmenu_,"nowrap");
	}
}

WSVButton::~WSVButton(void)
{
}
