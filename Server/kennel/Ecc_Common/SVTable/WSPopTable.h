#pragma once

#include "WTable"

#include <list>

using namespace std;
class WImage;
class WText;
class WTable;

class WSPopTable : public WTable
{
public:
	//MOC: W_OBJECT WSPopTable:WTable
    W_OBJECT;

public:
	WSPopTable(WContainerWidget * parent, const bool bHelp_ = false);
	//virtual ~WSPopTable(void);
public :

	WTable * GeRowActionTable(int i);
	WTable * GeRowContentTable(int i);
	void AppendRows(std::string strRowName);

	virtual void refresh(); 

private slots:
private:
	void ShowTop(WTable * pTable);
	void ShowBottom(WTable * pTable);
public: 
		std::vector<WTable *> m_pListActionTable;
		std::vector<WTable *> m_pListContentTable;

		WTable * pFirstTable; 
		WTable * pContentTable;

		WText  * pTitleText;
		WImage * pHelpImg;
public : //standard button  

private slots:
	//MOC: SLOT WSPopTable::BtnProc()
	void BtnProc();
	bool bHelp;
	

};
