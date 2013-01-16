/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <boost/lexical_cast.hpp>

#include "WComboBox"
#include "DomElement.h"

#include <iostream>
//#include "WComboBox"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WComboBox::slots_[] = { WSlot_("clear()", false), WSlot_("propagateChange()", false), WSlot_() };
WSignal_ WComboBox::signals_[] = { WSignal_("activated(int)", false), WSignal_("activated(const std::string)", false), WSignal_() };

void WComboBox::buildSlotMap(WSlotMap& result)
{
  WFormWidget::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WComboBox::buildSignalMap(WSignalMap& result)
{
  WFormWidget::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

void WComboBox::activated(int arg0)
{
  void *args[1] = {(void *)(&arg0)};  triggerSignal(signals_ + 0, args);
}

void WComboBox::activated(const std::string arg0)
{
  void *args[1] = {(void *)(&arg0)};  triggerSignal(signals_ + 1, args);
}

bool WComboBox::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  if (slot == slots_ + 0) {
    sender_ = sender;    clear();
    return true;
  }
  if (slot == slots_ + 1) {
    sender_ = sender;    propagateChange();
    return true;
  }
  return WFormWidget::triggerSlot(sender, slot, args);
}
bool WComboBox::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WFormWidget::undoTriggerSlot(slot, args);
}


WComboBox::WComboBox(WContainerWidget *parent)
  : WFormWidget(parent),
    currentIndex_(-1),
    itemsChanged_(false),
    selectionChanged_(false),
    currentlyConnected_(false)
{ 
  setInline(true);
  setFormObject(true);
}

void WComboBox::addItem(const std::string text)
{
  insertItem(items_.size(), text);
}

int WComboBox::count() const
{
  return items_.size();
}

int WComboBox::currentIndex() const
{
  return currentIndex_;
}

const std::string WComboBox::currentText() const
{
  if (currentIndex_ != -1)
    return items_[currentIndex_];
  else
    return "";
}

void WComboBox::insertItem(int index, const std::string text)
{
  items_.insert(items_.begin() + index, text);
  itemsChanged_ = true;
  repaint();
}

const std::string WComboBox::itemText(int index) const
{
  return items_[index];
}

void WComboBox::removeItem(int index)
{
  items_.erase(items_.begin() + index);
  itemsChanged_ = true;
  currentIndex_ = std::min(currentIndex_, (int)items_.size() - 1);
  repaint();
}

void WComboBox::setCurrentIndex(int index)
{
  if (currentIndex_ != index) {
	

    currentIndex_ = std::min(index, (int)items_.size() - 1);
    selectionChanged_ = true;
    repaint();
  }
}
void WComboBox::setCurrentIndexByStr(std::string strSelText)
{
	 int curIndex = 0;
	
	for(int i=0;i<count();i++)
	{
		if(itemText(i)==strSelText)
         {
	         curIndex = i;
              break;
         }
	}
    setCurrentIndex(curIndex);   

}

void WComboBox::setItemText(int index, const std::string text)
{
  items_[index] = text;
  itemsChanged_ = true;
  repaint();
}

void WComboBox::clear()
{
  items_.clear();
  itemsChanged_ = true;
  repaint();
}

void WComboBox::propagateChange()
{
  emit activated(currentIndex_);
  emit activated(items_[currentIndex_]);
}

void WComboBox::updateDom(DomElement& element, bool all)
{
  if (itemsChanged_ || all) {
    if (!all)
      element.removeAllChildren();

    for (unsigned i = 0; i < items_.size(); ++i) {
      DomElement *item = DomElement::createNew(DomElement::OPTION);
      item->setAttribute("value", boost::lexical_cast<std::string>(i));
      item->setProperty(Wt::PropertyInnerHTML, items_[i]);
      if ((int)i == currentIndex_)
	item->setProperty(Wt::PropertySelected, "true");

      element.addChild(item);
    }

    itemsChanged_ = false;
  }

  if (selectionChanged_) {
    element.setProperty(Wt::PropertySelectedIndex,
			boost::lexical_cast<std::string>(currentIndex_));
    selectionChanged_ = false;
  }

  const WSignalInstance_ *s1 = getSignal(SIGNAL(activated(int)));
  const WSignalInstance_ *s2 = getSignal(SIGNAL(activated(const std::string)));
  if (isConnected(s1) || isConnected(s2))
    connect(this, SIGNAL(changed()), this, SLOT(propagateChanged()));

  WFormWidget::updateDom(element, all);

  renderOk();
}

DomElement *WComboBox::createDomElement()
{
  DomElement *result = DomElement::createNew(DomElement::SELECT);
  result->setId(this, true);

  updateDom(*result, true);

  return result;
}

void WComboBox::getDomChanges(std::vector<DomElement *>& result)
{
  if (renderState() == RenderUpdate) {
    DomElement *e = DomElement::getForUpdate(this, DomElement::SELECT);
    updateDom(*e, false);
    result.push_back(e);
  }
}

void WComboBox::setFormData(const char *value, int length,
			    const char *filename)
{
  std::string valueS = value;

  if (valueS != "")
    currentIndex_ = boost::lexical_cast<int>(valueS);
  else
    currentIndex_ = -1;
}
