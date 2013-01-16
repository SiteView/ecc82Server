/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WCssStyleSheet"
#include "WCssDecorationStyle"

WCssRule::WCssRule(std::string selector)
  : selector_(selector)
{ }
WCssRule::~WCssRule()
{ }

WCssDecorationRule::WCssDecorationRule(std::string selector, const WCssDecorationStyle& style)
  : WCssRule(selector)
    
{
	style_ =style;
}

const std::string WCssDecorationRule::declarations() const
{
  return style_.cssText();
}

WCssOtherRule::WCssOtherRule(std::string selector,
			     const std::string declarations)
  : WCssRule(selector),
    declarations_(declarations)
{ }

WCssStyleSheet::WCssStyleSheet()
{ }
WCssStyleSheet::~WCssStyleSheet()
{
	WCssRule *tmpCss;
	for(std::map<std::string, WCssRule *>::iterator It =rules_.begin();
	It!=rules_.end();It++ )
	{
		tmpCss =It->second;
		It->second =NULL;
		delete(tmpCss);
	}
	rules_.clear();
}
void WCssStyleSheet::addRule(const std::string selector, const WCssDecorationStyle& style)
{
  rules_[selector] = new WCssDecorationRule(selector, style);
}

void WCssStyleSheet::addRule(const std::string selector, const std::string declarations)
{
  rules_[selector] = new WCssOtherRule(selector, declarations);
}

std::string WCssStyleSheet::cssText() const
{
  std::string result;
  for (std::map<std::string, WCssRule *>::const_iterator i = rules_.begin();
       i != rules_.end(); ++i) {
    result += i->first + " { " + i->second->declarations() + " }\n";
  }

  return result;
}
