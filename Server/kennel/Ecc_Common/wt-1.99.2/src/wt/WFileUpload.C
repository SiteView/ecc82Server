/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <boost/lexical_cast.hpp>

#include "WText"
#include "WFileUpload"
#include "WApplication"
#include "WEnvironment"
#include "WStringUtil"

#include "DomElement.h"
#include "CgiParser.h"

namespace Wt {

WFileUpload::WFileUpload(WContainerWidget *parent)
  : WWebWidget(parent),
    uploaded(this),
    fileTooLarge(this),
    changed(this),
    textSize_(20),
    textSizeChanged_(false),
    isStolen_(false),
    doUpload_(false),
    iframe_(0)
{
  methodIframe_ = WApplication::instance()->environment().javaScript();

  setInline(true);

  if (!methodIframe_)
    setFormObject(true);
}

WFileUpload::~WFileUpload()
{
  if (!isStolen_)
    unlink(spoolFileName_.c_str());
}

void WFileUpload::setFileTextSize(int chars)
{
  textSize_ = chars;
  textSizeChanged_ = true;

  repaint();
}

void WFileUpload::stealSpooledFile()
{
  isStolen_ = true;
}

void WFileUpload::updateDom(DomElement& element, bool all)
{
  WWebWidget::updateDom(element, all);
}

DomElement *WFileUpload::createDomElement()
{
  if (methodIframe_) {
    DomElement *form = DomElement::createNew(DomElement::FORM);
    form->setId(this, true);
    form->setAttribute("method", "post");
    form->setAttribute("action", generateUrl());
    form->setAttribute("enctype", "multipart/form-data");
    form->setAttribute("style", "margin:0;padding:0;display:inline");

    DomElement *input = DomElement::createNew(DomElement::INPUT);
    input->setAttribute("type", "file");
    input->setAttribute("name", "data");
    input->setAttribute("size", boost::lexical_cast<std::string>(textSize_));
    input->setId("in" + formName());

    updateSignalConnection(*input, changed, "change", true);

    form->addChild(input);

    updateDom(*form, true);

    return form;
  } else {
    DomElement *result = DomElement::createNew(DomElement::INPUT);
    result->setAttribute("type", "file");
    result->setAttribute("size", boost::lexical_cast<std::string>(textSize_));
    result->setId(this, true);
    updateSignalConnection(*result, changed, "change", true);

    updateDom(*result, true);

    return result;
  }
}

void WFileUpload::getDomChanges(std::vector<DomElement *>& result)
{
  if (renderState() == RenderUpdate) {
    if (!methodIframe_) {
      DomElement *e = DomElement::getForUpdate(this, DomElement::INPUT);

      updateDom(*e, false);

      result.push_back(e);
    } else {
      DomElement *e = DomElement::getForUpdate(this, DomElement::FORM);

      updateDom(*e, false);

      if (doUpload_) {
	iframe_ = wApp->useIframe();
	e->setAttribute("target", iframe_->formName());
	e->callMethod("submit()");
	doUpload_ = false;
      }

      result.push_back(e);
    }
  }
}

void WFileUpload::setFormData(CgiEntry *entry)
{
  if (!entry->clientFilename().empty()) {
    spoolFileName_ = entry->value();
    clientFileName_ = fromUTF8(entry->clientFilename());
    contentDescription_ = fromUTF8(entry->contentType());
    entry->stealFile();
    isStolen_ = false;
  }

  resourceTriggerUpdate_ = true;
}

void WFileUpload::formDataSet()
{
  uploaded.emit();
}

void WFileUpload::requestTooLarge(int size)
{
  fileTooLarge.emit(size);
}

void WFileUpload::upload()
{
  if (methodIframe_) {
    doUpload_ = true;
    repaint();
  }
}

void WFileUpload::setNoFormData()
{
}

void WFileUpload::resourceHTML(std::ostream&)
{
  if (iframe_) {
    wApp->releaseIframe(iframe_);
    iframe_ = 0;
  }
}

}
