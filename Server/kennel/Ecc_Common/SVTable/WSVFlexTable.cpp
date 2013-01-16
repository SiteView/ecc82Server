#include ".\wsvflextable.h"

#include "WText"
#include "WImage"
#include "WTableCell"
#include "WScrollArea"
#include "windows.h"
//#include "webase.h"

//
WSVFlexTable::WSVFlexTable(WContainerWidget * parent, int TableType, const std::string strTitle, const bool showImg)
:WTable(parent)
{
	bShowHelp = true;
	nType = TableType;
	bShowImg = showImg;

	pTopViewTable =NULL;
	FirstTable = NULL; 
	pAactionTable=NULL;
	
	dataTitleTable=NULL;
	dataGridTable=NULL;

	this->elementAt(0,0)->setStyleClass("table_shadow_l");
	this->elementAt(0,2)->setStyleClass("table_shadow_r");

	pTopViewTable = new WTable((WContainerWidget *)this->elementAt(0,1));
	pTopViewTable->setStyleClass("table_title");

	pTopViewTable->elementAt(0,0)->setStyleClass("table_title_text");
	pTitleTxt= new WText(strTitle,(WContainerWidget *)pTopViewTable->elementAt(0,0));	

	if (bShowImg)
	{
		pShowImg = new WImage("/Images/table_unwrap.png","",(WContainerWidget *)pTopViewTable->elementAt(0,1));
		pHideImg = new WImage("/Images/table_pucker.png","",(WContainerWidget *)pTopViewTable->elementAt(0,1));

		pShowImg->hide();
		pTopViewTable->elementAt(0,1)->setStyleClass("hand");
		connect(pTopViewTable->elementAt(0,1),SIGNAL(clicked()),this,SLOT(ShowHide()));
	}

	ShowBottom();
}

//
WTable * WSVFlexTable::GetContentTable()
{	
	if(FirstTable==NULL)
	{
		FirstTable = new WTable((WContainerWidget *)this->elementAt(1,1));

		if(FirstTable)
		{
			this->elementAt(1,0)->setStyleClass("table_shadow_l");
			
			//
			if(nType == List || nType == ReportList)
				this->elementAt(1,1)->setStyleClass("table_data_grid");
			else
				this->elementAt(1,1)->setStyleClass("table_data_input");

			this->elementAt(1,2)->setStyleClass("table_shadow_r");

			if(nType == List)
			{
				// 构造列表头 Table
				FirstTable->elementAt(0, 0)->setStyleClass("table_data_grid_header");
				dataTitleTable = new WTable((WContainerWidget *)FirstTable->elementAt(0, 0));	

				// 构造列表数据项 Table
				string strTmp = "<div id='";
				strTmp += strDivId;
				strTmp += "' class='panel_item'>";
				new WText(strTmp, (WContainerWidget *)FirstTable->elementAt(1, 0));
				dataGridTable = new WTable((WContainerWidget *)FirstTable->elementAt(1, 0));
				new WText("</div>", (WContainerWidget *)FirstTable->elementAt(1, 0));
			}

			if(nType == Query)
			{
				dataGridTable = new WTable((WContainerWidget *)FirstTable->elementAt(1, 0));
			}
		}
		else 
			return NULL;
	}

	return FirstTable ;

/*
	if(pContentTable==NULL)
	{
		FirstTable = new WTable((WContainerWidget *)this->elementAt(1,1));
		FirstTable->resize(WLength(100,WLength::Percentage),WLength());

		if(FirstTable)
		{
			this->elementAt(1,0)->setStyleClass("table_shadow_l");
			FirstTable->setStyleClass("table_view_bg");
			this->elementAt(1,2)->setStyleClass("table_shadow_r");
			SecondTable = new WTable((WContainerWidget *)FirstTable->elementAt(0,0));
			
			if(SecondTable){
				SecondTable->setStyleClass("widthauto");
				pContentTable = new WTable ((WContainerWidget *)SecondTable->elementAt(0,0));
				if(pContentTable){
					pContentTable->setStyleClass("widthauto");
					return pContentTable;
				}else return NULL;
			}else  return NULL;
		}else return NULL;
	}
	return pContentTable ;*/
}

//
WTable * WSVFlexTable::GetActionTable()
{
	if(pAactionTable==NULL)
	{
		GetContentTable();
		
		if(FirstTable)
		{			
			
			int iLast = GetContentTable()->numRows();

			//如果是List风格 统一都有列表数据项是否为空此提示信息 并提供隐藏及显示的函数接口			
			if(nType == List)
			{							
				iLast++;
				pNullTipTable = new WTable((WContainerWidget *)FirstTable->elementAt(iLast, 0));
				if(pNullTipTable)
				{
					//pNullTipTable->setStyleClass("widthauto");
					pNullTipTable->hide();
				}
			}
			
			iLast++;
			pAactionTable=new WTable((WContainerWidget *)FirstTable->elementAt(iLast, 0));
			if(pAactionTable)
			{
				pAactionTable->setStyleClass("widthauto");
				return pAactionTable;
			}
			else
				return NULL;
		}
		else
			return NULL;

	}
	else 
		return pAactionTable;

/*
	if(pAactionTable==NULL)
	{
		GetContentTable();
		if(SecondTable){
			pAactionTable=new WTable ((WContainerWidget *)SecondTable->elementAt(1,0));
			if(pAactionTable){
				pAactionTable->setStyleClass("widthauto");
				return pAactionTable;
			}else return NULL;
		}else return NULL;

	}else return pAactionTable;
*/
}

//
void WSVFlexTable::ShowBottom()
{	
	new WImage("/Images/table_shadow_left_bom.png","",this->elementAt(2,0));
	this->elementAt(2,1)->setStyleClass("table_shadow_bom");
	new WImage("/Images/table_shadow_right_bom.png","",this->elementAt(2,2));	
}

//
void WSVFlexTable::ShowHide()
{
	if (!bShowImg)
	{
		return;
	}
	if(FirstTable)
	{
		if(pShowImg->isHidden()) //当前是显示状态
		{
			FirstTable->hide();
			pShowImg->show();
			pHideImg->hide();
		}
		else
		{
			FirstTable->show();
			pShowImg->hide();
			pHideImg->show();
		}
	}
}

//
void WSVFlexTable::refresh()
{
	if (!bShowImg)
	{
		return;
	}
	if(!pShowImg->isHidden()) //当前是显示状态
	{
			FirstTable->show();
			pShowImg->hide();
			pHideImg->show();
	}
}



/////////////////////////////////////
//TreeView.html的函数(List)
/////////////////////////////////////	

void WSVFlexTable::SetNullTipInfo(string strInfo)
{
	if(pNullTipTable)
	{	
		pNullTipTable->elementAt(0 , 0)->setContentAlignment(AlignTop | AlignCenter);
		WText * textInfo = new WText(strInfo, (WContainerWidget *)pNullTipTable->elementAt(0, 0));
		textInfo->decorationStyle().setForegroundColor(Wt::red); 
	}
}

void WSVFlexTable::ShowNullTip()
{
	if(pNullTipTable)
	{
		pNullTipTable->show();
	}

}

void WSVFlexTable::HideNullTip()
{
	if(pNullTipTable)
	{
		pNullTipTable->hide();
	}
}

//
WTable * WSVFlexTable::GeDataTable()
{
	return dataGridTable;
}


//
void WSVFlexTable::AppendColumn(std::string strBigColName,std::string strSmallColName, WLength width_)
{

	for (int i=0 ; i< 3; i++)
	{
		if(GetContentTable()!=NULL)
		{
			int iLast = dataTitleTable->numColumns();		
			if(iLast!=0)
			{
				new WImage("/Images/table_head_space.png","",(WContainerWidget *)dataTitleTable->elementAt(0,iLast));
				WImage * pTmpImage = new WImage("/Images/space.gif","",(WContainerWidget *)dataGridTable->elementAt(0,iLast));
				pTmpImage->resize(WLength(4), WLength());
				dataGridTable->elementAt(1,iLast)->resize(WLength(4), WLength());
				iLast = iLast+1;
			}

			//m_pColumnWidth[iLast] = width_;
			m_pColumnWidth.push_back(width_);
			dataTitleTable->elementAt(1,iLast)->setStyleClass("table_data_grid_header_text");
			dataTitleTable->elementAt(1,iLast)->resize(width_,WLength());
			dataTitleTable->elementAt(1,iLast)->setContentAlignment(AlignCenter);
			strcpy(dataTitleTable->elementAt(1,iLast)->contextmenu_,"nowrap");


			dataTitleTable->resize(WLength(100,WLength::Percentage), WLength(100,WLength::Percentage));

			new WText(strSmallColName,(WContainerWidget *)dataTitleTable->elementAt(1,iLast));

			dataGridTable->elementAt(0,iLast)->setStyleClass("table_data_grid_header_text");
			dataGridTable->elementAt(0,iLast)->resize(width_,WLength());
			dataGridTable->elementAt(0,iLast)->setContentAlignment(AlignCenter);
			strcpy(dataGridTable->elementAt(0,iLast)->contextmenu_,"nowrap");

			dataGridTable->resize(WLength(100,WLength::Percentage), WLength(100,WLength::Percentage));

			//return new WText("",(WContainerWidget *)dataGridTable->elementAt(0,iLast));
		}
	}
	
	//return NULL;
}

//
WText * WSVFlexTable::AppendColumn(std::string strColName, WLength width_)
{
	
	if(GetContentTable()!=NULL)
	{
		int iLast = dataTitleTable->numColumns();		
		if(iLast!=0)
		{
			new WImage("/Images/table_head_space.png","",(WContainerWidget *)dataTitleTable->elementAt(0,iLast));
			WImage * pTmpImage = new WImage("/Images/space.gif","",(WContainerWidget *)dataGridTable->elementAt(0,iLast));
			pTmpImage->resize(WLength(4), WLength());
			dataGridTable->elementAt(0,iLast)->resize(WLength(4), WLength());
			iLast = iLast+1;
		}

		//m_pColumnWidth[iLast] = width_;
		m_pColumnWidth.push_back(width_);
		dataTitleTable->elementAt(0,iLast)->setStyleClass("table_data_grid_header_text");
		dataTitleTable->elementAt(0,iLast)->resize(width_,WLength());
		dataTitleTable->elementAt(0,iLast)->setContentAlignment(AlignCenter);
		strcpy(dataTitleTable->elementAt(0,iLast)->contextmenu_,"nowrap");

		dataTitleTable->resize(WLength(100,WLength::Percentage), WLength(100,WLength::Percentage));
		
		new WText(strColName,(WContainerWidget *)dataTitleTable->elementAt(0,iLast));

		dataGridTable->elementAt(0,iLast)->setStyleClass("table_data_grid_header_text");
		dataGridTable->elementAt(0,iLast)->resize(width_,WLength());
		dataGridTable->elementAt(0,iLast)->setContentAlignment(AlignCenter);
		strcpy(dataGridTable->elementAt(0,iLast)->contextmenu_,"nowrap");

		dataGridTable->resize(WLength(100,WLength::Percentage), WLength(100,WLength::Percentage));

		return new WText("",(WContainerWidget *)dataGridTable->elementAt(0,iLast));

	}

	return NULL;
}

void  WSVFlexTable::SetDataRowStyle(string strCss)
{
	m_pRowDataCss.push_back(strCss);
}

void  WSVFlexTable::SetDivId(string strIn)
{
	strDivId = strIn;
}

string  WSVFlexTable::GetDivId()
{
	return strDivId;
}

//
void WSVFlexTable::InitRow(int nRow)
{
	for(int i = 1,j = 0; i < dataTitleTable->numColumns() + 1; i+=2, j++)
	{
		dataGridTable->elementAt(nRow, i)->resize(WLength(4), WLength());
		WImage * pTmpImage = new WImage("/Images/space.gif", "", (WContainerWidget *)dataGridTable->elementAt(nRow, i));
		pTmpImage->resize(WLength(4), WLength());
		dataGridTable->elementAt(nRow, i - 1)->resize(m_pColumnWidth[j], WLength());
		dataGridTable->elementAt(nRow, i - 1)->setStyleClass(m_pRowDataCss[j]);
	}
	
	if((nRow%2) == 0)
		dataGridTable->GetRow(nRow)->setStyleClass("table_data_grid_item_bg");
}

//
void WSVFlexTable::AddStandardSelLink(std::string strSelall,std::string strSelNone,std::string strSelReverse )
{
	GetActionTable();
	if(	pAactionTable!=NULL)
	{
		WTable *pTbl;
		pTbl = new WTable( (WContainerWidget *)pAactionTable->elementAt(0,0));
		pSelAll= new WText(strSelall, (WContainerWidget *)pTbl->elementAt(0,0));
		pSelAll->setStyleClass("linktext");
		pTbl->elementAt(0,0)->setStyleClass("button_link_font");
		pSelNone=new WText(strSelNone, (WContainerWidget *)pTbl->elementAt(0,1));
		pSelNone->setStyleClass("linktext");
		pTbl->elementAt(0,1)->setStyleClass("button_link_font");
		pSelReverse= new WText(strSelReverse, (WContainerWidget *)pTbl->elementAt(0,2));
		pSelReverse->setStyleClass("linktext");
		pTbl->elementAt(0,2)->setStyleClass("button_link_font");
	}
}

/////////////////////////////////////
//AddGroup.html的函数
/////////////////////////////////////

//添加栏目
bool WSVFlexTable::AppendRows(std::string strColName)
{
	if(GetContentTable()!=NULL)
	{
		int iLast = GetContentTable()->numRows();
		
		//栏
		iLast++;
		WTable * pTmpTable = new WTable((WContainerWidget *)GetContentTable()->elementAt(iLast, 0));

		//栏名称		
		pTmpTable->elementAt(0, 0)->setStyleClass("padding_top");
		WTable * pTmpTitleTable = new WTable((WContainerWidget *)pTmpTable->elementAt(0, 0));
		pTmpTitleTable->setStyleClass("table_data_input_rows");
		pTmpTitleTable->elementAt(0, 0)->setStyleClass("table_data_input_text");
		new WText(strColName, (WContainerWidget *)pTmpTitleTable->elementAt(0, 0));
		
		//栏内容
		pTmpTable->elementAt(1, 0)->setStyleClass("padding_4");	
		m_pListRowTable.push_back(new WTable((WContainerWidget *)pTmpTable->elementAt(1, 0)));
		
		//栏分隔线
		iLast++;
		WTable * pSeparatorTable = new WTable((WContainerWidget *)GetContentTable()->elementAt(iLast, 0));
		pSeparatorTable->setStyleClass("table_data_input_rows");
		pSeparatorTable->elementAt(0, 0)->setStyleClass("table_data_input_space");
		new WImage("/Images/space.gif","",pSeparatorTable->elementAt(0, 0));
	}

	return true;
}

//添加栏目内容-->标题、输入控件、帮助、错误信息 （多项怎么办？输入控件怎么定位？帮助、错误信息必须放到统一的数组中？）--->由此函数解决
WContainerWidget * WSVFlexTable::AppendRowsContent(int nRow, string strLabel, string strDes, string strError, bool noBlank)
{
	//取出指定栏目的Table
	WTable * pRowTable = NULL;
	WContainerWidget * pParentCell = NULL;

	//
	if(nRow > m_pListRowTable.size())
	{
		for (unsigned i = nRow; i > m_pListRowTable.size() ; --i)
		{
			AppendRows("");

			//此情况应该无效或不会出现， 暂时先这样处理
		}
	}
	else
	{
		//
	}

	pRowTable = m_pListRowTable[nRow];
	int iLast = pRowTable->numRows();	
	
	//标题 WText
	iLast++;
	if (noBlank)
	{
		strLabel += "<font color=red>*</font>";
	}
	new WText(strLabel, pRowTable->elementAt(iLast, 0));
	strcpy(pRowTable->elementAt(iLast, 0)->contextmenu_,"nowrap");
	pRowTable->elementAt(iLast, 0)->setStyleClass("width120");

	//输入控件
	pParentCell = pRowTable->elementAt(iLast, 1);
	strcpy(pRowTable->elementAt(iLast, 1)->contextmenu_,"nowrap");
	//pRowTable->elementAt(iLast, 1)->setStyleClass("table_gruop_item");
	
	//帮助 WText
	if(strDes != "")
	{
		iLast++;
		
		m_pListHelpText.push_back(new WText(strDes, pRowTable->elementAt(iLast, 1)));
		
		//strcpy(pRowTable->elementAt(iLast, 1)->contextmenu_,"nowrap");
		pRowTable->elementAt(iLast, 1)->setStyleClass("table_data_input_des");		
	}
	
	//错误信息 WText
	if(strError != "")
	{
		iLast++;
		
		m_pListErrorText.push_back(new WText(strError, pRowTable->elementAt(iLast, 1)));
		
		strcpy(pRowTable->elementAt(iLast, 1)->contextmenu_,"nowrap");
		pRowTable->elementAt(iLast, 1)->setStyleClass("table_data_input_error");
	}

	return pParentCell;
}

//显示或隐藏帮助信息
void WSVFlexTable::ShowOrHideHelp()
{
	if(bShowHelp)
	{
		bShowHelp = false;
		for (unsigned i = 0; i < m_pListHelpText.size(); ++i)
		{
			m_pListHelpText[i]->hide();
		}
	}
	else
	{
		bShowHelp = true;
		for (unsigned i = 0; i < m_pListHelpText.size(); ++i)
		{
			m_pListHelpText[i]->show();
		}
	}
}

//显示指定错误信息
void WSVFlexTable::ShowErrorMsg(std::list<string> msgList)
{
	for (unsigned j = 0; j < m_pListErrorText.size(); j++)
	{
		m_pListErrorText[j]->hide();
	}			

	std::list<string>::iterator msgitem;
	for(msgitem = msgList.begin(); msgitem != msgList.end(); msgitem ++)	
	{
		for (unsigned j = 0; j < m_pListErrorText.size(); j++)
		{
			if(m_pListErrorText[j]->text() == *msgitem)
			{
				m_pListErrorText[j]->show();
				continue;
			}
		}			
	}
}

//隐藏所有错误信息
void WSVFlexTable::HideAllErrorMsg()
{
	for (unsigned j = 0; j < m_pListErrorText.size(); j++)
	{
		m_pListErrorText[j]->hide();
	}			
}

//////////////////////////
//AlertLog.htm的函数
//////////////////////////

void WSVFlexTable::InitTable()
{
	if(GetContentTable()!=NULL)
	{
		int iLast = GetContentTable()->numRows();
		
		//栏内容
		iLast++;
		//GetContentTable()->elementAt(iLast, 0)->setStyleClass("padd_lr");	
		//pRowTable = new 
		//GetContentTable()->elementAt(iLast, 0)->setStyleClass("table_data_input");
		
		dataGridTable = new WTable((WContainerWidget *)GetContentTable()->elementAt(iLast, 0));
		
		if(nType == Query)
		{
			//栏分隔线
			iLast++;
			//GetContentTable()->elementAt(iLast, 0)->setStyleClass("table_gruop");
			//GetContentTable()->elementAt(iLast, 0)->setStyleClass("table_data_input_rows");	
			WTable * pSeparatorTable = new WTable((WContainerWidget *)GetContentTable()->elementAt(iLast, 0));
			//pSeparatorTable->resize(WLength(100,WLength::Percentage), WLength());
			//pSeparatorTable->elementAt(0, 0)->setStyleClass("table_space");
			pSeparatorTable->elementAt(0, 0)->setStyleClass("table_data_input_space");		
			new WImage("/Images/space.gif","",pSeparatorTable->elementAt(0, 0));
		}
	}
}

void WSVFlexTable::AppendRows()
{
	int iLast = dataGridTable->numRows();
	
	//标题 WText
	iLast++;
	dataGridTable->elementAt(iLast, 0)->setStyleClass("padding_top");
	m_pListRowTable.push_back(new WTable((WContainerWidget *)dataGridTable->elementAt(iLast, 0)));
}

//
WContainerWidget * WSVFlexTable::AppendRowsContent(int nRow, int nColumn, int nColumnCount, string strLabel, string strDes, string strError,bool noBlank)
{
	//取出指定栏目的Table
	WTable * pRowTable = NULL;
	WContainerWidget * pParentCell = NULL;

	//
	if(nRow > m_pListRowTable.size())
	{
		for (unsigned i = nRow; i > m_pListRowTable.size() ; --i)
		{
			AppendRows("");

			//此情况应该无效或不会出现， 暂时先这样处理
		}
	}
	else
	{
		//
	}

	//(nColumn * 2)

	//nColumnCount == 2 则为 50%

	pRowTable = m_pListRowTable[nRow];
	pRowTable->setStyleClass("table_data_input_rows");	
	
	//标题 WText
	if(noBlank)
		strLabel += "<font color=red>*</font>";
	new WText(strLabel, pRowTable->elementAt(nRow, (nColumn * 2)));
	strcpy(pRowTable->elementAt(nRow, (nColumn * 2))->contextmenu_,"nowrap");
	pRowTable->elementAt(nRow, (nColumn * 2))->setStyleClass("table_data_input_text");

	//输入控件
	pParentCell = pRowTable->elementAt(nRow, (nColumn * 2) + 1);
	//pRowTable->elementAt(nRow, (nColumn * 2) + 1)->setStyleClass("table_data_input_text");
	strcpy(pRowTable->elementAt(nRow, (nColumn * 2) + 1)->contextmenu_,"nowrap");

	//帮助 WText
	if(strDes != "")
	{
	
		m_pListHelpText.push_back(new WText(strDes, pRowTable->elementAt(nRow + 1, (nColumn * 2) + 1)));
		
		if(nColumnCount == 2)
			pRowTable->elementAt(nRow + 1, (nColumn * 2) + 1)->resize(WLength(50,WLength::Percentage), WLength());	

		//strcpy(pRowTable->elementAt(nRow + 1, (nColumn * 2) + 1)->contextmenu_,"nowrap");
		pRowTable->elementAt(nRow + 1, (nColumn * 2) + 1)->setStyleClass("table_data_input_des");
	}
	
	//错误信息 WText
	if(strError != "")
	{	
		m_pListErrorText.push_back(new WText(strError, pRowTable->elementAt(nRow + 2, (nColumn * 2) + 1)));
		
		if(nColumnCount == 2)
			pRowTable->elementAt(nRow + 2, (nColumn * 2) + 1)->resize(WLength(50,WLength::Percentage), WLength());

		//strcpy(pRowTable->elementAt(nRow + 2, (nColumn * 2) + 1)->contextmenu_,"nowrap");
		pRowTable->elementAt(nRow + 2, (nColumn * 2) + 1)->setStyleClass("table_data_input_error");
	}

	return pParentCell;
}

//

//////////////////////////
//EntityList.htm的函数
//////////////////////////

//添加设备行
void WSVFlexTable::AppendEntityRow(string strName, string strHref, string strDes)
{
	int iLast = GetContentTable()->numRows();	
	
	//名称 描述
	iLast++;
	GetContentTable()->elementAt(iLast, 0)->setStyleClass("entitylist_name");	
	strcpy(GetContentTable()->elementAt(iLast, 0)->contextmenu_,"nowrap");
	GetContentTable()->elementAt(iLast, 1)->setStyleClass("entitylist_des");
	strcpy(GetContentTable()->elementAt(iLast, 1)->contextmenu_,"nowrap");
		
	string strTmp = "<a style='color:#18497A' href='";
	strTmp += strHref;
	strTmp += "'>";
	strTmp += strName;
	strTmp += "</a>";

	new WText(strTmp, GetContentTable()->elementAt(iLast, 0));
	new WText(strDes, GetContentTable()->elementAt(iLast, 1));
}

//////////////////////////
//EntityDes.htm的函数
//////////////////////////

void WSVFlexTable::AppendEntityDesRow(string strLabel, string strDes)
{
	int iLast = GetContentTable()->numRows();	
	
	//名称 描述
	iLast++;
	GetContentTable()->elementAt(iLast, 0)->setStyleClass("padding_top");
	WTable * pTable = new WTable(GetContentTable()->elementAt(iLast, 0));
	pTable->setStyleClass("table_data_input_rows");

	pTable->elementAt(0, 0)->setStyleClass("table_data_input_text");	
	strcpy(pTable->elementAt(0, 0)->contextmenu_,"nowrap");
	
	strcpy(pTable->elementAt(0, 1)->contextmenu_,"nowrap");
		
	new WText(strLabel, pTable->elementAt(0, 0));
	new WText(strDes, pTable->elementAt(0, 1));
}