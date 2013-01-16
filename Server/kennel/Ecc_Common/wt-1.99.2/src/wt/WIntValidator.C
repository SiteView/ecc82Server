/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <boost/lexical_cast.hpp>

#include "WIntValidator"

namespace Wt {

WIntValidator::WIntValidator(int bottom, int top)
  : bottom_(bottom),
    top_(top)
{ }

void WIntValidator::setBottom(int bottom)
{
  bottom_ = bottom;
}

void WIntValidator::setTop(int top)
{
  top_ = top;
}

void WIntValidator::setRange(int bottom, int top)
{
  bottom_ = bottom;
  top_ = top;
}

WValidator::State WIntValidator::validate(std::wstring& input, int& pos) const
{
  if (isMandatory()) {
    if (input.empty())
      return InvalidEmpty;
  } else {
    if (input.empty())
      return Valid;
  }

  try {
    int i = boost::lexical_cast<int>(input);

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
