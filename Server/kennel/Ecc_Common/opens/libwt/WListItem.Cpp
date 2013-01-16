/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WListItem"

WListItem::WListItem(const std::string text)
  : text_(text),
    data_(0),
    w_(0)
{ }

WListItem::~WListItem()
{
}

void WListItem::setData(void *data)
{
  data_ = data;
}

void WListItem::setWidget(WWidget *w)
{
  w_ = w;
}
