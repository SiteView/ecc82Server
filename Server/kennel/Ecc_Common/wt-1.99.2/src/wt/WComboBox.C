/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <boost/lexical_cast.hpp>

#include "WComboBox"
#include "WStringUtil"

#include "DomElement.h"
#include "CgiParser.h"

namespace Wt {

WComboBox::WComboBox(WContainerWidget *parent)
  : WFormWidget(parent),
    activated(this),
    sactivated(this),
    currentIndex_(-1),
    itemsChanged_(false),
    selectionChanged_(false),
    currentlyConnected_(false)
{ 
  setInline(true);
  setFormObject(true);
}

void WComboBox::addItem(const WMessage& text)
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

const WMessage& WComboBox::currentText() const
{
  if (currentIndex_ != -1)
    return items_[currentIndex_];
  else
    assert(false);
}

void WComboBox::insertItem(int index, const WMessage& text)
{
  items_.insert(items_.begin() + index, text);

  if (currentIndex_ == -1) {
    currentIndex_ = 0;
    selectionChanged_ = true;
  }

  itemsChanged_ = true;
  repaint();
}

const WMessage& WComboBox::itemText(int index) const
{
  return items_[index];
}

void WComboBox::removeItem(int index)
{
  items_.erase(items_.begin() + index);
  itemsChanged_ = true;
  repaint();

  setCurrentIndex(currentIndex_);
}

void WComboBox::setCurrentIndex(int index)
{
  if (currentIndex_ != index) {
    currentIndex_ = std::min(index, (int)items_.size() - 1);

    selectionChanged_ = true;
    repaint();

    changed.emit();
  }
}

void WComboBox::setItemText(int index, const WMessage& text)
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

  setCurrentIndex(currentIndex_);
}

void WComboBox::propagateChange()
{
  activated.emit(currentIndex_);
  sactivated.emit(items_[currentIndex_].value());
}

void WComboBox::updateDom(DomElement& element, bool all)
{
  if (itemsChanged_ || all) {
    if (!all)
      element.removeAllChildren();

    for (unsigned i = 0; i < items_.size(); ++i) {
      DomElement *item = DomElement::createNew(DomElement::OPTION);
      item->setAttribute("value", boost::lexical_cast<std::string>(i));
      item->setProperty(Wt::PropertyInnerHTML,
			toUTF8(escapeText(items_[i].value())));
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

  if (activated.isConnected() || sactivated.isConnected())
      changed.connect(SLOT(this, WComboBox::propagateChange));

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

void WComboBox::setFormData(CgiEntry *entry)
{
  if (!entry->value().empty())
    currentIndex_ = boost::lexical_cast<int>(entry->value());
  else
    currentIndex_ = -1;
}

void WComboBox::refresh()
{
  bool changed = false;

  for (unsigned i = 0; i < items_.size(); ++i) {
    if (items_[i].refresh())
      changed = true;
  }

  if (changed) {
    itemsChanged_ = true;
    repaint();
  }

  WFormWidget::refresh();
}

WValidator::State WComboBox::validate()
{
  if (validator()) {
    int pos;
    std::wstring text = currentText().value();

    return validator()->validate(text, pos);
  } else
    return WValidator::Valid;
}

}
