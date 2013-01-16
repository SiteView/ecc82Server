/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WCssStyleSheet"
#include "WCssDecorationStyle"
#include "WStringUtil"

namespace Wt {

WCssRule::WCssRule(std::string selector)
  : selector_(selector)
{ }
WCssRule::~WCssRule()
{ }

WCssDecorationRule::WCssDecorationRule(std::string selector,
				       const WCssDecorationStyle& style)
  : WCssRule(selector),
    style_(style)
{ }

const std::string WCssDecorationRule::declarations()

{
  return style_.cssText();
}

WCssOtherRule::WCssOtherRule(std::string selector,
			     const std::wstring declarations)
  : WCssRule(selector),
    declarations_(declarations)
{ }

const std::string WCssOtherRule::declarations()
{
  return toUTF8(declarations_); 
}

WCssStyleSheet::WCssStyleSheet()
{ }

WCssStyleSheet::~WCssStyleSheet()
{
  for (std::map<std::string, WCssRule *>::const_iterator i = rules_.begin();
       i != rules_.end(); ++i)
    delete i->second;
}

void WCssStyleSheet::addRule(const std::string selector,
			     const WCssDecorationStyle& style)
{
  rules_[selector] = new WCssDecorationRule(selector, style);
}

void WCssStyleSheet::addRule(const std::string selector,
			     const std::wstring declarations)
{
  rules_[selector] = new WCssOtherRule(selector, declarations);
}

std::string WCssStyleSheet::cssText() const
{
  std::string result;
  for (std::map<std::string, WCssRule *>::const_iterator i = rules_.begin();
       i != rules_.end(); ++i) {
    result += i->first
      + " { " + i->second->declarations() + " }\n";
  }

  return result;
}

} // namespace Wt
