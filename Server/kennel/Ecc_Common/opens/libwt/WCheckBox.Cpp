/*
* Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
*
* See the LICENSE file for terms of use.
*/
#include "WCheckBox"
#include "DomElement.h"

#include <iostream>
//#include "WCheckBox"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>
#include <WLabel>

#include <WSlotInstance_>

WSlot_ WCheckBox::slots_[] = { WSlot_() };
WSignal_ WCheckBox::signals_[] = { WSignal_() };

void WCheckBox::buildSlotMap(WSlotMap& result)
{
    WAbstractToggleButton::buildSlotMap(result);
    for (int i = 0; slots_[i].good(); ++i)
        result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WCheckBox::buildSignalMap(WSignalMap& result)
{
    WAbstractToggleButton::buildSignalMap(result);
    for (int i = 0; signals_[i].good(); ++i)
        result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool WCheckBox::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
    return WAbstractToggleButton::triggerSlot(sender, slot, args);
}
bool WCheckBox::undoTriggerSlot(const WSlot_ *slot, void **args)
{
    return WAbstractToggleButton::undoTriggerSlot(slot, args);
}


WCheckBox::WCheckBox(WContainerWidget *parent)
: WAbstractToggleButton(parent),
_CheckChanged(false),
nCheck(false)
{
    setFormObject(true);
}

WCheckBox::WCheckBox(const std::string text, WContainerWidget *parent)
: WAbstractToggleButton(text, parent),
_CheckChanged(false),
nCheck(false),
_SetNameChanged(false),
_SetIdChanged(false)
{
    setFormObject(true);
    memset(nSetName, 0, 64);
    memset(id_, 0, 64);
}

void WCheckBox::setParent(WWidget *parent)
{
    
    if (parent_)
        parent_->removeChild(this);
    if (parent)
        parent->addChild(this);
    
    label_->setParent(parent);
}

void WCheckBox::SetCheck(bool bChecked)
{
	if(bChecked)
	{
		nCheck=true;
		_CheckChanged=true;
	}
	repaint();
}

/*void WCheckBox::SetName(char * nName)
{
	_SetNameChanged = true;

	strcpy(nSetName, nName);



	repaint();
}
*/
void WCheckBox::SetId(char * nId)
{
	_SetIdChanged = true;
//	strcpy(id_, nId);
	repaint();
}

void WCheckBox::updateDom(DomElement& element, bool all)
{
/*	if(_CheckChanged||all)
	{
		if (nCheck) {
			element.setAttribute("value", "1");
			element.setProperty(Wt::PropertyChecked, "true");
		}
		else  
		{
			element.setAttribute("value", "0");
			element.setProperty(Wt::PropertyChecked, "false");
		}
		_CheckChanged=false;
	}

	if(_SetNameChanged||all)
	{
		
		//element.setAttribute("name", nSetName);
	//	element.setAttribute("AutoPostBack", "true");
	//	memset(nSetName, 0, 64);
		_SetNameChanged = false;
	}
*/

	if(_SetIdChanged||all)
	{
		//element.setAttribute("id", id_);

		//element.setId(id_, true);
		//std::string temp = element.id();
		//FILE * pf = fopen("c:\\getid.txt", "a+");
		//fputs(temp.c_str(), pf);
		//fclose(pf);
		//strcpy(id_, temp.c_str());
		_SetIdChanged = false;
	}

  WAbstractToggleButton::updateDom(element, all);
}


DomElement *WCheckBox::createDomElement()
{
  DomElement *checkBox = DomElement::createNew(DomElement::INPUT);
  checkBox->setAttribute("type", "checkbox");
  checkBox->setId(this, true);
  
  std::string id = checkBox -> id();

  strcpy(id_, id.c_str());
 
  updateDom(*checkBox, true);

  return checkBox;
}

void WCheckBox::getDomChanges(std::vector<DomElement *>& result)
{
  if (renderState() == RenderUpdate) 
  {
    DomElement *e = DomElement::getForUpdate(this, DomElement::INPUT);
    updateDom(*e, false);
    result.push_back(e);
  }
}
