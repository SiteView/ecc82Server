/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <boost/lexical_cast.hpp>

#include "WSelectionBox"
#include "DomElement.h"

namespace Wt {

WSelectionBox::WSelectionBox(WContainerWidget *parent)
  : WComboBox(parent),
    verticalSize_(5),
    verticalSizeChanged_(false)
{ }

void WSelectionBox::setVerticalSize(int items)
{
  verticalSize_ = items;
  verticalSizeChanged_ = true;
  repaint();
}

void WSelectionBox::updateDom(DomElement& element, bool all)
{
  if (verticalSizeChanged_ || all) {
    element.setAttribute("size",
			 boost::lexical_cast<std::string>(verticalSize_));
    verticalSizeChanged_ = false;
  }

  WComboBox::updateDom(element, all);

  renderOk();
}

}
