/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WAbstractToggleButton"
#include "WLabel"
#include "DomElement.h"
#include "CgiParser.h"

namespace Wt {

WAbstractToggleButton::WAbstractToggleButton(WContainerWidget *parent)
  : WFormWidget(parent),
    checked(this),
    unChecked(this),
    checked_(false),
    checkedChanged_(false)
{
  WLabel *label = new WLabel(parent);
  label->setBuddy(this);
}

WAbstractToggleButton::WAbstractToggleButton(const WMessage& text,
					     WContainerWidget *parent)
  : WFormWidget(parent),
    checked(this),
    unChecked(this),
    checked_(false),
    checkedChanged_(false)
{ 
  WLabel *label = new WLabel(text, parent);
  label->setBuddy(this);
}

void WAbstractToggleButton::setText(const WMessage& text)
{
  label()->setText(text);
}

void WAbstractToggleButton::setChecked(bool how)
{
  checked_ = how;
  checkedChanged_ = true;

  if (how)
    checked.emit();
  else
    unChecked.emit();

  repaint();
}

void WAbstractToggleButton::setChecked()
{
  wasChecked_ = checked_;
  setChecked(true);
}

void WAbstractToggleButton::setUnChecked()
{
  wasChecked_ = checked_;
  setChecked(false);
}

void WAbstractToggleButton::undoSetChecked()
{
  setChecked(checked_);
}

void WAbstractToggleButton::undoSetUnChecked()
{
  undoSetChecked();
}

void WAbstractToggleButton::updateDom(DomElement& element, bool all)
{
  if (checkedChanged_ || all) {
    element.setProperty(Wt::PropertyChecked, checked_ ? "true" : "false");
    checkedChanged_ = false;
  }

  bool needUpdateChangeSignal =
    (changed.needUpdate()
     || checked.needUpdate()
     || unChecked.needUpdate());

  WFormWidget::updateDom(element, all);

  if (needUpdateChangeSignal || all) {
    DomElement *e = DomElement::getForUpdate(this, DomElement::INPUT);

    std::vector<DomElement::EventAction> actions;

    if (checked.isConnected())
      actions.push_back
	(DomElement::EventAction(e->createReference() + ".checked == true",
				 checked.javaScript(),
				 checked.encodeCmd(),
				 checked.isExposedSignal()));
    if (unChecked.isConnected())
      actions.push_back
	(DomElement::EventAction(e->createReference() + ".checked == false",
				 unChecked.javaScript(),
				 unChecked.encodeCmd(),
				 unChecked.isExposedSignal()));
    if (changed.isConnected())
      actions.push_back
	(DomElement::EventAction(std::string(),
				 changed.javaScript(),
				 changed.encodeCmd(),
				 changed.isExposedSignal()));

    element.setEvent("change", actions);

    checked.updateOk();
    unChecked.updateOk();
    changed.updateOk();

    delete e;
  }
}

void WAbstractToggleButton::setFormData(CgiEntry *entry)
{
  checked_ = (entry->value() != "0");
}

void WAbstractToggleButton::setNoFormData()
{
  if (isEnabled() && isVisible())
    checked_ = false;
}

}
