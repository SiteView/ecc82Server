/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include "WMessageResourceBundle"
#include "WMessageResources"
#include "WMessage"
#include "WStringUtil"

#include <iostream>

namespace Wt {

WMessageResourceBundle::WMessageResourceBundle()
{ }

WMessageResourceBundle::~WMessageResourceBundle()
{
  for (unsigned i = 0; i < messageResources_.size(); ++i)
    delete messageResources_[i];
}

void WMessageResourceBundle::use(const std::string path, bool loadInMemory)
{
  for (unsigned i = 0; i < messageResources_.size(); ++i)
    if (messageResources_[i]->path() == path)
      return;

  messageResources_.push_back(new WMessageResources(path, loadInMemory));
}

std::wstring WMessageResourceBundle::getValue(const WMessage& message)
{
  std::wstring result;

  for (unsigned i = 0; i < messageResources_.size(); ++i) {
    if (messageResources_[i]->getValue(message, result))
      return result;
  }

  return L"??" + widen(message.key()) + L"??";
}

void WMessageResourceBundle::refresh()
{
  for (unsigned i = 0; i < messageResources_.size(); ++i) {
    messageResources_[i]->refresh();
  }
}

void WMessageResourceBundle::hibernate()
{
  for (unsigned i = 0; i < messageResources_.size(); ++i)
    messageResources_[i]->hibernate();
}

}
