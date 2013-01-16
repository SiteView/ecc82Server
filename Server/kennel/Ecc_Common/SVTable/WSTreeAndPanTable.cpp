#include ".\WSTreeAndPanTable.h"

#include "WText"
#include "WImage"
#include "WTableCell"
#include "WScrollArea"
#include "windows.h"
//#include "webase.h"

//
WSTreeAndPanTable::WSTreeAndPanTable(WContainerWidget * parent)
:WTable(parent)
{
	this->setStyleClass("panel_view");
	
	this->elementAt(0, 0)->setStyleClass("tree_bg");
	
	//TreeTable 可以在引用时再添加
	//new WText("<div id='tree_panel' name='tree_panel' class='panel_tree'>", this->elementAt(0, 0));
	//new WText("</div>", this->elementAt(0, 0));

	//DragTable Cell
	this->elementAt(0, 1)->setStyleClass("resize");
	strcpy(this->elementAt(0, 1)->contextmenu_, "onMouseDown='this.setCapture()' onMouseUp='this.releaseCapture();'");
	WImage * pTmpImage = new WImage("/Images/space.gif", this->elementAt(0, 1));
	pTmpImage->resize(WLength(5), WLength());

	//PanTable Cell 可以在引用时再添加
	//new WText("<div id='view_panel' class='panel_view'>", this->elementAt(0, 2));
	//new WText("</div>", this->elementAt(0, 2));
}


//
void WSTreeAndPanTable::refresh()
{

}