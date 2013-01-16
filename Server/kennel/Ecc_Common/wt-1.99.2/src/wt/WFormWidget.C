/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <iostream>

#include "WFormWidget"
#include "DomElement.h"
#include "WLabel"

namespace Wt {

WFormWidget::WFormWidget(WContainerWidget *parent)
  : WInteractWidget(parent),
    changed(this),
    selected(this),
    blurred(this),
    focussed(this),
    label_(0),
    enabled_(true),
    validator_(0),
    enabledChanged_(false)
{ 
  implementStateless(&WFormWidget::disable, &WFormWidget::undoDisable);
  implementStateless(&WFormWidget::enable, &WFormWidget::undoEnable);
}

WFormWidget::~WFormWidget()
{
  if (label_)
    label_->setBuddy(0);

  setValidator(0);
}

void WFormWidget::enable()
{
  wasEnabled_ = enabled_;
  setEnabled(true);
}

void WFormWidget::disable()
{
  wasEnabled_ = enabled_;
  setEnabled(false);
}

void WFormWidget::undoEnable()
{
  setEnabled(wasEnabled_);
}

void WFormWidget::undoDisable()
{
  undoEnable();
}

void WFormWidget::setEnabled(bool enabled)
{
  enabled_ = enabled;
  enabledChanged_ = true;
  repaint();
}

void WFormWidget::updateDom(DomElement& element, bool all)
{
  updateSignalConnection(element, changed, "change", all);
  updateSignalConnection(element, selected, "select", all);
  updateSignalConnection(element, blurred, "blur", all);
  updateSignalConnection(element, focussed, "focus", all);

  if (enabledChanged_ || all) {
    element.setProperty(Wt::PropertyDisabled, enabled_ ? "false" : "true");
    enabledChanged_ = false;
  }

  WInteractWidget::updateDom(element, all);
}

void WFormWidget::setLabel(WLabel *label)
{
  label_ = label;
}

void WFormWidget::setHidden(bool hidden)
{
  if (label_)
    label_->setHidden(hidden);

  WInteractWidget::setHidden(hidden);
}

void WFormWidget::setValidator(WValidator *validator)
{
  if (validator_)
    delete validator_;

  validator_ = validator;
}

WValidator::State WFormWidget::validate()
{
  std::cerr << "WFormWidget::validate() called: trying to validate a button?"
	    << std::endl;

  return WValidator::Valid;
}

}
