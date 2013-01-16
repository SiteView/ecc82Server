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
    cursorChanged_(false),
    borderChanged_(false),
    foregroundColorChanged_(false),
    backgroundColorChanged_(false),
    backgroundImageChanged_(false),
    fontChanged_(false)
{ }

WCssDecorationStyle::WCssDecorationStyle(WWebWidget *widget)
  : widget_(widget),
    cursor_(Auto),
    font_(widget),
    cursorChanged_(false),
    borderChanged_(false),
    foregroundColorChanged_(false),
    backgroundColorChanged_(false),
    backgroundImageChanged_(false),
    fontChanged_(false)
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
					     Repeat repeat)
{
  backgroundImage_ = image;
  backgroundImageRepeat_ = repeat;
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

void WCssDecorationStyle::setBorder(WBorder border)
{
  border_ = border;
  borderChanged_ = true;
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
    if (borderChanged_ || (border_.style() != WBorder::None))
      element.setProperty(PropertyStyleBorder, border_.cssText());
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
			  ? "url(" + backgroundImage_ + ")" : "none");
      switch (PropertyStyleBackgroundRepeat) {
      case RepeatXY:
	element.setProperty(PropertyStyleBackgroundRepeat, "repeat"); break;
      case RepeatX:
	element.setProperty(PropertyStyleBackgroundRepeat, "repeat-x"); break;
      case RepeatY:
	element.setProperty(PropertyStyleBackgroundRepeat, "repeat-y"); break;
      case NoRepeat:
	element.setProperty(PropertyStyleBackgroundRepeat, "no-repeat"); break;
      }
    }

    backgroundImageChanged_ = false;
  }
}

std::string WCssDecorationStyle::cssText() const
{
  std::string style;
  /*
   * set cursor
   */
  switch (cursor_) {
  case Auto:
    break;
  case Default:
    style += "cursor: default;"; break;
  case CrossHair:
    style += "cursor: crosshair;"; break;
  case Pointer:
    style += "cursor: pointer;"; break;
  case Move:
    style += "cursor: move;"; break;
  case Wait:
    style += "cursor: wait;"; break;
  case Text:
    style += "cursor: text;"; break;
  case Help:
    style += "cursor: help;"; break;
  }

  //XXX font !

  /*
   * set border
   */
  style += "border: " + border_.cssText() + ";";

  /*
   * set colors
   */
  if (!foregroundColor_.isDefault())
    style += "color: " + foregroundColor_.cssText() + ";";

  if (!backgroundColor_.isDefault())
    style += "background-color: " + backgroundColor_.cssText() + ";";

  return style;
}
