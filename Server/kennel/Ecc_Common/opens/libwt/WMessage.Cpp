/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include "WMessage"
#include "WApplication"

WMessage::WMessage()
{ }

WMessage::WMessage(const std::string key)
  : key_(key)
{ }

const std::string WMessage::value() const
{
  return wApp->messageResourceBundle().getValue(*this);
}
