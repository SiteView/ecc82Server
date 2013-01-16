/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WLength"
#include <boost/lexical_cast.hpp>

namespace Wt {

WLength::WLength()
  : auto_(true),
    value_(-1)
{ }

WLength::WLength(double value, Unit unit)
  : auto_(false),
    unit_(unit),
    value_(value)
{ }

bool WLength::operator== (const WLength& other) const
{
  return
    (auto_ == other.auto_)
    && (unit_ == other.unit_)
    && (value_ == other.value_);
}

const std::string WLength::cssText() const
{
  static const char *unitText[]
    = { "em", "ex", "px", "in", "cm", "mm", "pt", "pc", "%" };

  if (auto_)
    return "auto";
  else
    return boost::lexical_cast<std::string>(value_) + unitText[unit_];
}

}
