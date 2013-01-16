/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <boost/lexical_cast.hpp>

#include "WApplication"
#include "WEnvironment"
#include "WTimerWidget"
#include "WTimer"
#include "DomElement.h"

namespace Wt {

WTimerWidget::WTimerWidget(WTimer *timer)
  : timer_(timer),
    timerStarted_(false)
{ }

WTimerWidget::~WTimerWidget()
{
  timer_->timerWidget_ = 0;
}

void WTimerWidget::timerStart()
{
  timerStarted_ = true;

  repaint();
}

bool WTimerWidget::timerExpired()
{
  return timer_->getRemainingInterval() == 0;
}

void WTimerWidget::updateDom(DomElement& element, bool all)
{
  if (timerStarted_ 
      || ((!WApplication::instance()->environment().javaScript() || all)
	  && timer_->isActive())) {
    element.setTimeout(timer_->getRemainingInterval());

    timerStarted_ = false;
  }

  WInteractWidget::updateDom(element, all);
}

DomElement *WTimerWidget::createDomElement()
{
  DomElement *result = DomElement::createNew(DomElement::SPAN);
  result->setId(this);

  updateDom(*result, true);

  return result;
}

void WTimerWidget::getDomChanges(std::vector<DomElement *>& result)
{
  bool forceUpdate
    = (!WApplication::instance()->environment().javaScript() 
       && timer_->isActive());

  if (forceUpdate || (renderState() == RenderUpdate)) {
    DomElement *e = DomElement::getForUpdate(this, DomElement::SPAN);
    updateDom(*e, false);
    result.push_back(e);
  }
}

}
