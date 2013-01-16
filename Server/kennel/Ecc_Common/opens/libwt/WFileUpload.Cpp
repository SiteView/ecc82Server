/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <boost/lexical_cast.hpp>

#include "WFileUpload"
#include "DomElement.h"

#include <iostream>
//#include "WFileUpload"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WFileUpload::slots_[] = { WSlot_() };
WSignal_ WFileUpload::signals_[] = { WSignal_("newFileUploaded()", false), WSignal_() };

void WFileUpload::buildSlotMap(WSlotMap& result)
{
  WFormWidget::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WFileUpload::buildSignalMap(WSignalMap& result)
{
  WFormWidget::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

void WFileUpload::newFileUploaded()
{
  void **args = 0;
  triggerSignal(signals_ + 0, args);
}

bool WFileUpload::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  return WFormWidget::triggerSlot(sender, slot, args);
}
bool WFileUpload::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WFormWidget::undoTriggerSlot(slot, args);
}


WFileUpload::WFileUpload(WContainerWidget *parent)
  : WFormWidget(parent),
    content_(0)
{ 
  setInline(true);
  setFormObject(true);
}

WFileUpload::~WFileUpload()
{
  delete[] content_;
}

void WFileUpload::updateDom(DomElement& element, bool all)
{
  WFormWidget::updateDom(element, all);
}

DomElement *WFileUpload::createDomElement()
{
  DomElement *result = DomElement::createNew(DomElement::INPUT);
  result->setAttribute("type", "file");
  result->setId(this, true);

  updateDom(*result, true);

  return result;
}

void WFileUpload::getDomChanges(std::vector<DomElement *>& result)
{
  if (renderState() == RenderUpdate) {
    DomElement *e = DomElement::getForUpdate(this, DomElement::INPUT);
    updateDom(*e, false);
    result.push_back(e);
  }
}

void WFileUpload::setFormData(const char *value, int length,
			      const char *filename)
{
  delete[] content_;
  content_ = new char[length];
  memcpy(content_, value, length);
  contentLength_ = length;
  if (filename)
    fileName_ = filename;
  else
    fileName_ = "";

  emit newFileUploaded();
}

void WFileUpload::setNoFormData()
{
}
