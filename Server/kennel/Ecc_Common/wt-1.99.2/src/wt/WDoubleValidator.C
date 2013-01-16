/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <boost/lexical_cast.hpp>

#include "WDoubleValidator"

namespace Wt {

WDoubleValidator::WDoubleValidator(double bottom, double top)
  : bottom_(bottom),
    top_(top)
{ }

void WDoubleValidator::setBottom(double bottom)
{
  bottom_ = bottom;
}

void WDoubleValidator::setTop(double top)
{
  top_ = top;
}

void WDoubleValidator::setRange(double bottom, double top)
{
  bottom_ = bottom;
  top_ = top;
}

WValidator::State WDoubleValidator::validate(std::wstring& input,
					     int& pos) const
{
  if (isMandatory()) {
    if (input.empty())
      return InvalidEmpty;
  } else {
    if (input.empty())
      return Valid;
  }

  try {
    double i = boost::lexical_cast<double>(input);

    if ((i >= bottom_) && (i <= top_))
      return Valid;
    else
      return Invalid;
  } catch (boost::bad_lexical_cast& e) {
    return Invalid;
  }

  return Valid;
}

}
