#pragma once

#include "WTable"
#include <list>

using namespace std;
class WImage;
class WText;
class WTable;

class WSTreeAndPanTable :public WTable
{
public:
	//MOC: W_OBJECT WSTreeAndPanTable:WTable
    W_OBJECT;

public:
	WSTreeAndPanTable(WContainerWidget * parent);
	//virtual ~WSTreeAndPanTable(void);

public :	
	virtual void refresh();  
};
