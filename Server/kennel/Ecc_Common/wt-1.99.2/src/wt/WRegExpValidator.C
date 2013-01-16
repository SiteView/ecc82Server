/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include "WRegExpValidator"

#include <boost/regex.hpp>

namespace Wt {

struct WRegExpValidatorImpl
{
  boost::wregex rx;

  WRegExpValidatorImpl(const boost::wregex& arx)
    : rx(arx) { }
};

#ifndef WIN32
WRegExpValidator::WRegExpValidator(const boost::wregex& rx)
  : impl_(new WRegExpValidatorImpl(rx))
{ }
#endif // WIN32

WRegExpValidator::WRegExpValidator(const std::wstring s)
  : impl_(new WRegExpValidatorImpl(boost::wregex(s)))
{ }

WRegExpValidator::~WRegExpValidator()
{
  delete impl_;
}

#ifndef WIN32
const boost::wregex& WRegExpValidator::regExp() const
{
  return impl_->rx;
}
#endif // WIN32

WValidator::State WRegExpValidator::validate(std::wstring& input, int& pos)
  const
{
  if (isMandatory()) {
    if (input.empty())
      return InvalidEmpty;
  } else {
    if (input.empty())
      return Valid;
  }

  if (boost::regex_match(input, impl_->rx))
    return Valid;
  else
    return Invalid;
}

}
