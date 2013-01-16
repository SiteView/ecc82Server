/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <boost/lexical_cast.hpp>

#include "WWidget"
#include "WWebWidget"
#include "WFont"
#include "DomElement.h"

WFont::WFont()
  : widget_(0),
    genericFamily_(Default),
    style_(NormalStyle),
    variant_(NormalVariant),
    weight_(NormalWeight),
    weightValue_(400),
    size_(Medium),
    familyChanged_(false),
    styleChanged_(false),
    variantChanged_(false),
    weightChanged_(false),
    sizeChanged_(false)
{ }

WFont::WFont(WWebWidget *widget)
  : widget_(widget),
    genericFamily_(Default),
    style_(NormalStyle),
    variant_(NormalVariant),
    weight_(NormalWeight),
    weightValue_(400),
    size_(Medium),
    familyChanged_(false),
    styleChanged_(false),
    variantChanged_(false),
    weightChanged_(false),
    sizeChanged_(false)
{ }

void WFont::setFamily(GenericFamily genericFamily,
		      const std::string specificFamilies)
{
  genericFamily_ = genericFamily;
  specificFamilies_ = specificFamilies;
  familyChanged_ = true;
  if (widget_) widget_->repaint();
}

void WFont::setStyle(Style style)
{
  style_ = style;
  styleChanged_ = true;
  if (widget_) widget_->repaint();
}

void WFont::setVariant(Variant variant)
{
  variant_ = variant;
  variantChanged_ = true;
  if (widget_) widget_->repaint();
}

void WFont::setWeight(Weight weight, int value)
{
  weight_ = weight;
  weightValue_ = weightValue_;
  weightChanged_ = true;
  if (widget_) widget_->repaint();
}

void WFont::setSize(Size size, WLength fixedSize)
{
  size_ = size;
  fixedSize_ = fixedSize;
  sizeChanged_ = true;
  if (widget_) widget_->repaint();
}

void WFont::updateDomElement(DomElement& element, bool fontall, bool all)
{
  using namespace Wt;

  if (familyChanged_ || fontall || all) {
    std::string family = specificFamilies_;
    if (family.length() > 0)
      family += ',';

    switch (genericFamily_) {
    case Default:
      if (familyChanged_ || fontall)
	family = "inherit"; // discard specific families
      break;
    case Serif:
      family += "serif"; break;
    case SansSerif:
      family += "sans-serif"; break;
    case Cursive:
      family += "cursive"; break;
    case Fantasy:
      family += "fantasay"; break;
    case Monospace:
      family += "monospace"; break;
    }

    if (family.length() > 0)
      element.setProperty(PropertyStyleFontFamily, family);

    familyChanged_ = false;
  }

  if (styleChanged_ || fontall || all) {
    switch (style_) {
    case NormalStyle:
      if (styleChanged_ || fontall)
	element.setProperty(PropertyStyleFontStyle, "normal");
      break;
    case Italic:
      element.setProperty(PropertyStyleFontStyle, "italic"); break;
    case Oblique:
      element.setProperty(PropertyStyleFontStyle, "oblique"); break;
    }

    styleChanged_ = false;
  }

  if (variantChanged_ || fontall || all) {
    switch (variant_) {
    case NormalVariant:
      if (variantChanged_ || fontall)
	element.setProperty(PropertyStyleFontVariant, "normal");
      break;
    case SmallCaps:
      element.setProperty(PropertyStyleFontVariant, "small-caps");
      break;
    }

    variantChanged_ = false;
  }

  if (weightChanged_ || fontall || all) {
    switch (weight_) {
    case NormalWeight:
      if (weightChanged_ || fontall)
	element.setProperty(PropertyStyleFontWeight, "normal");
      break;
    case Bold:
      element.setProperty(PropertyStyleFontWeight, "bold"); break;
    case Bolder:
      element.setProperty(PropertyStyleFontWeight, "bolder"); break;
    case Lighter:
      element.setProperty(PropertyStyleFontWeight, "lighter"); break;
    case Value: {
      int v = std::min(900, std::max(100, ((weightValue_ / 100))*100));
      element.setProperty(PropertyStyleFontWeight,
			  boost::lexical_cast<std::string>(v));
      break;
    }
    }

    weightChanged_ = false;
  }

  if (sizeChanged_ || fontall || all) {
    switch (size_) {
    case Medium:
      if (sizeChanged_ || fontall)
	element.setProperty(PropertyStyleFontSize, "medium");
      break;
    case XXSmall:
      element.setProperty(PropertyStyleFontSize, "xx-small"); break;
    case XSmall:
      element.setProperty(PropertyStyleFontSize, "x-small"); break;
    case Small:
      element.setProperty(PropertyStyleFontSize, "small"); break;
    case Large:
      element.setProperty(PropertyStyleFontSize, "large"); break;
    case XLarge:
      element.setProperty(PropertyStyleFontSize, "x-large"); break;
    case XXLarge:
      element.setProperty(PropertyStyleFontSize, "xx-large"); break;
    case Smaller:
      element.setProperty(PropertyStyleFontSize, "smaller"); break;
    case Larger:
      element.setProperty(PropertyStyleFontSize, "larger"); break;
    case FixedSize:
      element.setProperty(PropertyStyleFontSize, fixedSize_.cssText());
      break;
    }

    sizeChanged_ = false;
  }  
}
