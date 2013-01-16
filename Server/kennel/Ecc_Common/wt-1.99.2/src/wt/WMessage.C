/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <boost/lexical_cast.hpp>

#include "WMessage"
#include "WApplication"
#include "WStringUtil"

namespace Wt {

WMessage::WMessage(const std::wstring value)
  : value_(value)
{ }

WMessage::WMessage(const wchar_t *value)
  : value_(value ? value : L"")
{ }

WMessage::WMessage(const char *value)
  : value_(value ? widen(value) : L"")
{ }

WMessage::WMessage(const std::string value)
  : value_(widen(value))
{ }

WMessage::WMessage(const char *key, bool)
  : key_(key)
{
}

WMessage WMessage::tr(const char *key)
{
  return WMessage(key, false);
}

const std::wstring WMessage::value() const
{
  if (!literal())
    return wApp->messageResourceBundle().getValue(*this);
  else
    return value_;
}

WMessage& WMessage::arg(const std::wstring value)
{
  arguments_.push_back(value);

  return *this;
}

WMessage& WMessage::arg(int value)
{
  arguments_.push_back(boost::lexical_cast<std::wstring>(value));

  return *this;
}

bool WMessage::refresh()
{
  if (literal())
    return false;
  else
    return true;
}

}
