/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <boost/lexical_cast.hpp>
#include "WColor"

namespace Wt {

WColor::WColor()
  : default_(true)
{ }

WColor::WColor(int r, int g, int b)
  : default_(false),
    red_(r),
    green_(g),
    blue_(b)
{ }

WColor::WColor(std::string name)
  : default_(false),
    name_(name)
{ }

bool WColor::operator==(const WColor& other) const
{
  return ((default_ == other.default_)
	  && (red_ == other.red_)
	  && (green_ == other.green_)
	  && (blue_ == other.blue_)
	  && (name_ == other.name_));
}

bool WColor::operator!=(const WColor& other) const
{
  return !(*this == other);
}

const std::string WColor::cssText() const
{
  if (default_)
    return std::string();
  else {
    if (name_.length() > 0)
      return name_;
    else {
      return "rgb(" + boost::lexical_cast<std::string>(red_)
	+ "," + boost::lexical_cast<std::string>(green_)
	+ "," + boost::lexical_cast<std::string>(blue_) + ")";
    }
  }
}

const WColor white("white");
const WColor black("black");
const WColor red("red");
const WColor darkRed("maroon");
const WColor green("lime");
const WColor darkGreen("green");
const WColor blue("blue");
const WColor darkBlue("navy");
const WColor cyan("aqua");
const WColor darkCyan("teal");
const WColor magenta("fuchsia");
const WColor darkMagenta("purple");
const WColor yellow("yellow");
const WColor darkYellow("olive");
const WColor gray(0xa0,0xa0,0xa4);
const WColor darkGray("gray");
const WColor lightGray("silver");
const WColor transparent("transparent");

}
