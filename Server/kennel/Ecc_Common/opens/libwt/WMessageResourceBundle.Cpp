/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include "WMessageResourceBundle"
#include "WMessageResources"
#include "WMessage"

WMessageResourceBundle::WMessageResourceBundle()
{ }

void WMessageResourceBundle::use(const std::string path)
{
  for (unsigned i = 0; i < messageResources_.size(); ++i)
    if (messageResources_[i]->path() == path)
      return;

  messageResources_.push_back(new WMessageResources(path));
}

std::string WMessageResourceBundle::getValue(const WMessage& message) const
{
  std::string result;

  for (unsigned i = 0; i < messageResources_.size(); ++i) {
    if (messageResources_[i]->getValue(message, result))
      return result;
  }

  return "??" + message.key() + "??";
}

void WMessageResourceBundle::refresh()
{
  for (unsigned i = 0; i < messageResources_.size(); ++i) {
    messageResources_[i]->refresh();
  }
}
