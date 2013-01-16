/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <WContainerWidget>

#include "Label.h"

Label::Label(const WMessage& text, WContainerWidget *parent)
  : WText(text, parent)
{ 
  setStyleClass(L"label");
  parent->setContentAlignment(AlignRight);
}
