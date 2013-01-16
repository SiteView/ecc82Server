/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WCssDecorationStyle"
#include "DomElement.h"
#include "WWidget"
#include "WWebWidget"

using namespace Wt;

WCssDecorationStyle::WCssDecorationStyle()
  : widget_(0),
    cursor_(Auto),
    backgroundImageRepeat_(RepeatXY),
    backgroundImageLocation_(0),
    textDecoration_(0),
    borderPosition_(0),
    cursorChanged_(false),
    borderChanged_(false),
    foregroundColorChanged_(false),
    backgroundColorChanged_(false),
    backgroundImageChanged_(false),
    fontChanged_(false),
    textDecorationChanged_(false)
{ }

WCssDecorationStyle::WCssDecorationStyle(WWebWidget *widget)
  : widget_(widget),
    cursor_(Auto),
    backgroundImageRepeat_(RepeatXY),
    font_(widget),
    textDecoration_(0),
    borderPosition_(0),
    cursorChanged_(false),
    borderChanged_(false),
    foregroundColorChanged_(false),
    backgroundColorChanged_(false),
    backgroundImageChanged_(false),
    fontChanged_(false),
    textDecorationChanged_(false)
{ }

void WCssDecorationStyle::setCursor(Cursor c)
{
  cursor_ = c;
  cursorChanged_ = true;
  if (widget_) widget_->repaint();
}

void WCssDecorationStyle::setFont(const WFont& font)
{
  font_ = font;
  fontChanged_ = true;
  if (widget_) widget_->repaint();
}

void WCssDecorationStyle::setBackgroundImage(const std::string image,
					     Repeat repeat,
					     int sides)
{
  backgroundImage_ = image;
  backgroundImageRepeat_ = repeat;
  backgroundImageLocation_ = sides;
  backgroundImageChanged_ = true;
  if (widget_) widget_->repaint();
}

void WCssDecorationStyle::setBackgroundColor(WColor color)
{
  backgroundColor_ = color;
  backgroundColorChanged_ = true;
  if (widget_) widget_->repaint();
}

void WCssDecorationStyle::setForegroundColor(WColor color)
{
  foregroundColor_ = color;
  foregroundColorChanged_ = true;
  if (widget_) widget_->repaint();
}

void WCssDecorationStyle::setBorder(WBorder border, int sides)
{
  border_ = border;
  borderPosition_ = sides;
  borderChanged_ = true;
  if (widget_) widget_->repaint();
}

void WCssDecorationStyle::setTextDecoration(int options)
{
  textDecoration_ = options;
  textDecorationChanged_ = true;
  if (widget_) widget_->repaint();
}

void WCssDecorationStyle::updateDomElement(DomElement& element, bool all)
{
  /*
   * set cursor
   */
  if (cursorChanged_ || all) {
    switch (cursor_) {
    case Auto:
      if (cursorChanged_)
	element.setProperty(PropertyStyleCursor, "auto");
      break;
    case Default:
      element.setProperty(PropertyStyleCursor, "default"); break;
    case CrossHair:
      element.setProperty(PropertyStyleCursor, "crosshair"); break;
    case Pointer:
      element.setProperty(PropertyStyleCursor, "pointer"); break;
    case Move:
      element.setProperty(PropertyStyleCursor, "move"); break;
    case Wait:
      element.setProperty(PropertyStyleCursor, "wait"); break;
    case Text:
      element.setProperty(PropertyStyleCursor, "text"); break;
    case Help:
      element.setProperty(PropertyStyleCursor, "help"); break;
    }

    cursorChanged_ = false;
  }

  /*
   * set font
   */
  font_.updateDomElement(element, fontChanged_, all);
  fontChanged_ = false;

  /*
   * set border
   */
  if (borderChanged_ || all) {
    if (borderChanged_ || (element.type() == DomElement::IFRAME)
	|| (border_.style() != WBorder::None)) {
	
      if (borderPosition_ & WWidget::Top)
	  element.setProperty(PropertyStyleBorderTop, border_.cssText());
      if (borderPosition_ & WWidget::Left)
	  element.setProperty(PropertyStyleBorderLeft, border_.cssText());
      if (borderPosition_ & WWidget::Right)
	  element.setProperty(PropertyStyleBorderRight, border_.cssText());
      if (borderPosition_ & WWidget::Bottom)
	  element.setProperty(PropertyStyleBorderBottom, border_.cssText());
    }
    borderChanged_ = false;
  }

  /*
   * set colors
   */
  if (foregroundColorChanged_ || all) {
    if ((all && !foregroundColor_.isDefault())
	|| foregroundColorChanged_)
      element.setProperty(PropertyStyleColor, foregroundColor_.cssText());
    foregroundColorChanged_ = false;
  }

  if (backgroundColorChanged_ || all) {
    if ((all && !backgroundColor_.isDefault())
	|| backgroundColorChanged_)
      element.setProperty(PropertyStyleBackgroundColor,
			  backgroundColor_.cssText());
    backgroundColorChanged_ = false;
  }

  if (backgroundImageChanged_ || all) {
    if ((backgroundImage_.length() != 0) || backgroundImageChanged_) {
      element.setProperty(PropertyStyleBackgroundImage,
			  backgroundImage_.length() > 0
			  ? "url(" + backgroundImage_ + ")" 
			  : "none");
      switch (backgroundImageRepeat_) {
      case RepeatXY:
	element.setProperty(PropertyStyleBackgroundRepeat, "repeat"); break;
      case RepeatX:
	element.setProperty(PropertyStyleBackgroundRepeat, "repeat-x"); break;
      case RepeatY:
	element.setProperty(PropertyStyleBackgroundRepeat, "repeat-y"); break;
      case NoRepeat:
	element.setProperty(PropertyStyleBackgroundRepeat, "no-repeat");break;
      }

      std::string location;
      if (backgroundImageLocation_ & WWidget::Top)
	location += " top";
      if (backgroundImageLocation_ & WWidget::Left)
	location += " left";
      if (backgroundImageLocation_ & WWidget::Right)
	location += " right";
      if (backgroundImageLocation_ & WWidget::Bottom)
	location += " bottom";

      if (!location.empty())
	element.setProperty(PropertyStyleBackgroundPosition, location);
    }

    backgroundImageChanged_ = false;
  }

  if (textDecorationChanged_ ||  all) {
    std::string options;

    if (textDecoration_ & Underline)
      options += " underline";
    if (textDecoration_ & Overline)
      options += " overline";
    if (textDecoration_ & LineThrough)
      options += " line-through";
    if (textDecoration_ & Blink)
      options += " blink";

    if (!options.empty() || textDecorationChanged_)
      element.setProperty(PropertyStyleTextDecoration, options);

    textDecorationChanged_ = false;
  }
}

std::string WCssDecorationStyle::cssText()
{
  DomElement e(DomElement::ModeCreate, DomElement::A);
  updateDomElement(e, true);

  return e.cssStyle();
}
