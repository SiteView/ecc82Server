/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <boost/lexical_cast.hpp>

#include "WTextArea"
#include "DomElement.h"
//#include "CgiParser.h"

WTextArea::WTextArea(WContainerWidget *parent)
  : WFormWidget(parent),
    cols_(20),
    rows_(5),
    contentChanged_(false),
    colsRowsChanged_(false)
{ 
  setInline(true);
  setFormObject(true);
}

WTextArea::WTextArea(const std::string text, WContainerWidget *parent)
  : WFormWidget(parent),
    content_(text),
    cols_(20),
    rows_(5),
    contentChanged_(false),
    colsRowsChanged_(false)
{ 
  setInline(true);
  setFormObject(true);
}

void WTextArea::setText(const std::string text)
{
  content_ = text;
  contentChanged_ = true;
  repaint();
}

void WTextArea::setColumns(int columns)
{
  cols_ = columns;
  colsRowsChanged_ = true;
  repaint();
}

void WTextArea::setRows(int rows)
{
  rows_ = rows;
  colsRowsChanged_ = true;
  repaint();
}

void WTextArea::updateDom(DomElement& element, bool all)
{
  if (contentChanged_ || all) {
    element.setAttribute("value", content_);
    element.setProperty(Wt::PropertyValue, content_);
    contentChanged_ = false;
  }

  if (colsRowsChanged_ || all) {
    element.setAttribute("cols",
			 boost::lexical_cast<std::string>(cols_));
    element.setAttribute("rows",
			 boost::lexical_cast<std::string>(rows_));
    colsRowsChanged_ = false;
  }

  WFormWidget::updateDom(element, all);
}

DomElement *WTextArea::createDomElement()
{
  DomElement *result = DomElement::createNew(DomElement::TEXTAREA);

//	DomElement *result = DomElement::createNew(DomElement::INPUT);
//	result->setAttribute("type", "textarea");

	
  result->setId(this, true);

  updateDom(*result, true);

  return result;
}

void WTextArea::getDomChanges(std::vector<DomElement *>& result)
{
  if (renderState() == RenderUpdate) {
    DomElement *e = DomElement::getForUpdate(this, DomElement::TEXTAREA);
    updateDom(*e, false);
    result.push_back(e);
  }
}

void WTextArea::setFormData(const char *value, int length,
							const char *filename)
							
{
  content_ = value;
}

/*
WValidator::State WTextArea::validate()
{
  if (validator()) {
    int pos;

    return validator()->validate(content_, pos);
  } else
    return WValidator::Valid;
}
*/