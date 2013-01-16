/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <iostream>

#include "WApplication"
#include "WWidget"
#include "WWebWidget"
#include "WContainerWidget"
#include "WStringUtil"

#include "DomElement.h"
#include "EscapeOStream.h"

namespace Wt {

WWidget::WWidget(WContainerWidget* parent)
  : WResource(parent),
    parent_(0),
    resourceTriggerUpdate_(false)
{
  implementStateless(&WWidget::hide, &WWidget::undoHide);
  implementStateless(&WWidget::show, &WWidget::undoShow);
}

WWidget::~WWidget()
{ }

void WWidget::setParent(WWidget *p)
{
  if (parent_)
    parent_->removeChild(this);
  if (p)
    p->addChild(this);
}

void WWidget::hide()
{
  wasHidden_ = isHidden();
  setHidden(true);
}

void WWidget::show()
{ 
  wasHidden_ = isHidden();
  setHidden(false);
}

void WWidget::undoHide()
{
  undoShow();
}

void WWidget::undoShow()
{
  setHidden(wasHidden_);
}

std::string WWidget::jsRef() const
{
  return "getElement('" + formName() + "')";
}

const std::string WWidget::resourceMimeType() const
{
  return "text/html; charset=utf-8";
}

void WWidget::resourceHTML(std::ostream& out)
{
  DomElement *element = webWidget()->createSDomElement();
  DomElement::TimeoutList timeouts;
  EscapeOStream sout(out);
  element->asHTML(sout, timeouts);
  delete element;
}

void WWidget::streamResourceData(std::ostream& stream)
{
  std::string js
    = resourceTriggerUpdate_ ? "update(null, 'res', null, true)" : "";
  resourceTriggerUpdate_ = false;

  stream 
    << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\""
       "\"http://www.w3.org/TR/REC-html40/strict.dtd\">"
       "<html lang=\"en\" dir=\"ltr\">\n"
       "<head>\n"
       "<meta HTTP-EQUIV=\"EXPIRES\" CONTENT=\"Sun, 14 Jun 2076 00:00:00 GMT\">"
       "<script>\n"
       "function load() { window.parent.evalJavaScript(";
  DomElement::jsStringLiteral(stream, js, '\"');
  stream << ")\n;}\n"
       "</script></head>"
       "<body"
    << (!js.empty() ? " onload='load()'" : "")
    << " style='margin:0;padding:0;'>";

  resourceHTML(stream);

  stream << "</body></html>"; 
}

WWidget *WWidget::adam()
{
  if (parent_)
    return parent_->adam();
  else
    return this;
}

WMessage WWidget::tr(const char *key)
{
  return WMessage::tr(key);
}

void WWidget::acceptDrops(const std::string mimeType,
			  const std::wstring hoverStyleClass)
{
  WWebWidget *thisWebWidget = webWidget();

  if (thisWebWidget->setAcceptDropsImpl(mimeType, true, hoverStyleClass)) {
    thisWebWidget->otherImpl_->dropEventSignal_
      ->connect(SLOT(this, WWidget::dropEvent));
    thisWebWidget
      ->setAttributeValue("ds",
			  widen(thisWebWidget->otherImpl_
				->dropEventSignal_->encodeCmd()));
  }
}

void WWidget::stopAcceptDrops(const std::string mimeType)
{
  WWebWidget *thisWebWidget = webWidget();

  thisWebWidget->setAcceptDropsImpl(mimeType, false, L"");
}

void WWidget::dropEvent(WDropEvent event)
{ }

}
