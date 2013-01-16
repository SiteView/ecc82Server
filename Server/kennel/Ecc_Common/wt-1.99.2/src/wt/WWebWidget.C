/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/framework/XMLFormatter.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>

#include "WApplication"
#include "WWebWidget"
#include "WStringUtil"

#include "DomElement.h"
#include "WebRenderer.h"
#include "WebSession.h"
#include "WContainerWidget"
#include "WebSession.h"

using namespace Wt;
using std::find;

WWebWidget::LayoutImpl::LayoutImpl()
  : positionScheme_(Static),
    floatSide_(None),
    clearSides_(None),
    popup_(false),
    verticalAlignment_(AlignBaseline),
    marginsChanged_(false)
{ 
  for (unsigned i = 0; i < 4; ++i)
    margin_[i] = WLength(0);
}

WWebWidget::LookImpl::LookImpl()
  : decorationStyle_(0),
    toolTip_(0),
    styleClassChanged_(false),
    toolTipChanged_(false)
{ }

WWebWidget::LookImpl::~LookImpl()
{
  if (decorationStyle_)
    delete decorationStyle_;
  if (toolTip_)
    delete toolTip_;
}

WWebWidget::OtherImpl::OtherImpl()
  : attributes_(0),
    attributesSet_(0),
    scriptFunctions_(0),
    scriptAdded_(0),
    dropEventSignal_(0),
    acceptedDropMimeTypes_(0)
{ }

WWebWidget::OtherImpl::~OtherImpl()
{
  if (attributes_)
    delete attributes_;
  if (attributesSet_)
    delete attributesSet_;
  if (scriptFunctions_)
    delete scriptFunctions_;
  if (scriptAdded_)
    delete scriptAdded_;
  if (dropEventSignal_)
    delete dropEventSignal_;
  if (acceptedDropMimeTypes_)
    delete acceptedDropMimeTypes_;
}

WWebWidget::WWebWidget(WContainerWidget *parent)
  : WWidget(parent),
    width_(0),
    height_(0),
    children_(0),
    childRemoveChanges_(0),
    newSiblings_(0),
    renderState_(RenderUpdate),
    layoutImpl_(0),
    lookImpl_(0),
    otherImpl_(0)
{
  flags_.set(BIT_INLINE);

  if (parent)
    parent->addWidget(this);
}

void WWebWidget::setFormObject(bool how)
{
  flags_.set(BIT_FORM_OBJECT);
}

void WWebWidget::repaint()
{
  if (renderState_ != RenderUpdate) {
    renderState_ = RenderUpdate;
    WApplication::instance()->session()->renderer().needUpdate(this);
  }
}

void WWebWidget::renderOk()
{
  if (renderState_ != RenderOk) {
    renderState_ = RenderOk;
    WApplication::instance()->session()->renderer().doneUpdate(this);
  }
}

void WWebWidget::signalConnectionsChanged()
{
  repaint();
}

WWebWidget::~WWebWidget()
{
  setParent(0);

  if (width_)
    delete width_;
  if (height_)
    delete height_;

  if (children_) {
    while (children_->size())
      delete (*children_)[0];
    delete children_;
  }

  if (childRemoveChanges_) {
    for (unsigned i = 0; i < childRemoveChanges_->size(); ++i)
      delete (*childRemoveChanges_)[i];
    delete childRemoveChanges_;
  }

  if (newSiblings_)
    delete newSiblings_;

  if (layoutImpl_)
    delete layoutImpl_;

  if (lookImpl_)
    delete lookImpl_;

  if (otherImpl_)
    delete otherImpl_;

  WApplication::instance()->session()->renderer().doneUpdate(this);
}

WCssDecorationStyle& WWebWidget::decorationStyle()
{
  if (!lookImpl_)
    lookImpl_ = new LookImpl();

  if (!lookImpl_->decorationStyle_)
    lookImpl_->decorationStyle_ = new WCssDecorationStyle(this);

  return *lookImpl_->decorationStyle_;
}

void WWebWidget::removeChild(WWidget *w)
{
  if (!children_) {
    std::cerr << "child disappeared in widget ?" << std::endl;
    return;
  }

  std::vector<WWidget *>::iterator i
    = find(children_->begin(), children_->end(), w);

  if (i != children_->end()) {
    if (!flags_.test(BIT_IGNORE_CHILD_REMOVES)) {
      DomElement *e = DomElement::getForUpdate(w->webWidget(),
					       DomElement::DIV);
      e->removeFromParent();

      if (!childRemoveChanges_)
	childRemoveChanges_ = new std::vector<DomElement *>;
      childRemoveChanges_->push_back(e);

      repaint();
    }

    w->parent_ = 0;
    children_->erase(i);
  } else {
    std::cerr << "child disappeared in widget ?" << std::endl;
  }
}

void WWebWidget::setPositionScheme(PositionScheme scheme)
{
  if (!layoutImpl_)
    layoutImpl_ = new LayoutImpl();

  layoutImpl_->positionScheme_ = scheme;

  if ((scheme == Absolute) || (scheme == Fixed))
    flags_.reset(BIT_INLINE);

  flags_.set(BIT_GEOMETRY_CHANGED);
  repaint();
}

WWidget::PositionScheme WWebWidget::positionScheme() const
{
  return layoutImpl_ ? layoutImpl_->positionScheme_ : Static;
}

WLength WWebWidget::width() const
{
  return width_ ? *width_ : WLength();
}

WLength WWebWidget::height() const
{
  return height_ ? *height_ : WLength();
}

void WWebWidget::resize(WLength width, WLength height)
{
  if (!width_ && !width.isAuto())
    width_ = new WLength();

  if (width_)
    *width_ = width;

  if (!height_ && !height.isAuto())
    height_ = new WLength();

  if (height_)
    *height_ = height;

  flags_.set(BIT_GEOMETRY_CHANGED);

  repaint();
}

void WWebWidget::setMinimumSize(WLength width, WLength height)
{
  if (!layoutImpl_)
    layoutImpl_ = new LayoutImpl();

  layoutImpl_->minimumWidth_ = width;
  layoutImpl_->minimumHeight_ = height;

  flags_.set(BIT_GEOMETRY_CHANGED);

  repaint();
}

void WWebWidget::setMaximumSize(WLength width, WLength height)
{
  if (!layoutImpl_)
    layoutImpl_ = new LayoutImpl();

  layoutImpl_->maximumWidth_ = width;
  layoutImpl_->maximumHeight_ = height;

  flags_.set(BIT_GEOMETRY_CHANGED);

  repaint();
}

void WWebWidget::setFloatSide(Side s)
{
  if (!layoutImpl_)
    layoutImpl_ = new LayoutImpl();

  layoutImpl_->floatSide_ = s;

  flags_.set(BIT_GEOMETRY_CHANGED);

  repaint();
}

WWidget::Side WWebWidget::floatSide() const
{
  if (layoutImpl_)
    return layoutImpl_->floatSide_;
  else
    return None;
}

void WWebWidget::setClearSides(int sides)
{
  if (!layoutImpl_)
    layoutImpl_ = new LayoutImpl();

  layoutImpl_->clearSides_ = sides;

  flags_.set(BIT_GEOMETRY_CHANGED);

  repaint();
}

int WWebWidget::clearSides() const
{
  if (layoutImpl_)
    return layoutImpl_->clearSides_;
  else
    return None;
}

void WWebWidget::setVerticalAlignment(VerticalAlignment va,
				      WLength length)
{
  if (!layoutImpl_)
    layoutImpl_ = new LayoutImpl();

  layoutImpl_->verticalAlignment_ = va;
  layoutImpl_->verticalAlignmentLength_ = length;

  flags_.set(BIT_GEOMETRY_CHANGED);

  repaint();
}

WWidget::VerticalAlignment WWebWidget::verticalAlignment() const
{
  return layoutImpl_ ? layoutImpl_->verticalAlignment_ : AlignBaseline;
}

WLength WWebWidget::verticalAlignmentLength() const
{
  return layoutImpl_ ? layoutImpl_->verticalAlignmentLength_ : WLength();
}

void WWebWidget::setOffset(int s, WLength l)
{
  if (!layoutImpl_)
    layoutImpl_ = new LayoutImpl();

  /*
   * FIXME: should be an int argument then ?
   */
  if (s & Top)
    layoutImpl_->offsets_[0] = l;
  if (s & Right)
    layoutImpl_->offsets_[1] = l;
  if (s & Bottom)
    layoutImpl_->offsets_[2] = l;
  if (s & Left)
    layoutImpl_->offsets_[3] = l;

  flags_.set(BIT_GEOMETRY_CHANGED);

  repaint();
}

WLength WWebWidget::offset(Side s) const
{
  if (!layoutImpl_)
    return WLength();

  switch (s) {
  case Top:
    return layoutImpl_->offsets_[0];
  case Right:
    return layoutImpl_->offsets_[1];
  case Bottom:
    return layoutImpl_->offsets_[2];
  case Left:
    return layoutImpl_->offsets_[3];
  default:
    std::cerr << "WWebWidget::offset() with weird offset called?"
	      << std::endl;
    return layoutImpl_->offsets_[0];
  }
}

int WWebWidget::zIndex() const
{
  if (layoutImpl_)
    return layoutImpl_->popup_ ? 1 : 0;
  else
    return 0;
}

void WWebWidget::setInline(bool inl)
{
  if (inl)
    flags_.set(BIT_INLINE);
  else
    flags_.reset(BIT_INLINE);

  flags_.set(BIT_GEOMETRY_CHANGED);

  repaint();
}

bool WWebWidget::isInline() const
{
  return flags_.test(BIT_INLINE);
}

void WWebWidget::setPopup(bool popup)
{
  if (!layoutImpl_)
    layoutImpl_ = new LayoutImpl();

  layoutImpl_->popup_ = popup;

  flags_.set(BIT_GEOMETRY_CHANGED);

  repaint();
}

bool WWebWidget::isPopup() const
{
  return layoutImpl_ ? layoutImpl_->popup_ : false;
}


void WWebWidget::setMargin(WLength length, int sides)
{
  if (!layoutImpl_)
    layoutImpl_ = new LayoutImpl();  

  if (sides & Top)
    layoutImpl_->margin_[0] = length;
  if (sides & Right)
    layoutImpl_->margin_[1] = length;
  if (sides & Bottom)
    layoutImpl_->margin_[2] = length;
  if (sides & Left)
    layoutImpl_->margin_[3] = length;

  layoutImpl_->marginsChanged_ = true;

  repaint();
}

WLength WWebWidget::margin(Side side) const
{
  if (!layoutImpl_)
    return WLength();

  switch (side) {
  case Top:
    return layoutImpl_->margin_[0];
  case Right:
    return layoutImpl_->margin_[1];
  case Bottom:
    return layoutImpl_->margin_[2];
  case Left:
    return layoutImpl_->margin_[3];
  default:
    std::cerr << "Error: WWebWidget::margin(Side) with illegal side."
	      << std::endl;
    return WLength();
  }
}

void WWebWidget::setStyleClass(const std::wstring styleClass)
{
  if (!lookImpl_)
    lookImpl_ = new LookImpl();

  lookImpl_->styleClass_ = styleClass;
  lookImpl_->styleClassChanged_ = true;

  repaint();
}

void WWebWidget::setStyleClass(const char *styleClass)
{
  setStyleClass(widen(styleClass));
}

const std::wstring WWebWidget::styleClass() const
{
  return lookImpl_ ? lookImpl_->styleClass_ : std::wstring();
}

void WWebWidget::setAttributeValue(const std::string name,
				   const std::wstring value)
{
  if (!otherImpl_)
    otherImpl_ = new OtherImpl();

  if (!otherImpl_->attributes_)
    otherImpl_->attributes_ = new std::map<std::string, std::wstring>;
  (*otherImpl_->attributes_)[name] = value;

  if (!otherImpl_->attributesSet_)
    otherImpl_->attributesSet_ = new std::vector<std::string>;

  otherImpl_->attributesSet_->push_back(name);

  repaint();
}

void WWebWidget::setScript(const std::string signature,
			   const std::string body)
{
  if (!otherImpl_)
    otherImpl_ = new OtherImpl();

  if (!otherImpl_->scriptFunctions_)
    otherImpl_->scriptFunctions_ = new std::map<std::string, std::string>;

  (*otherImpl_->scriptFunctions_)[signature] = body;

  if (!otherImpl_->scriptAdded_)
    otherImpl_->scriptAdded_ = new std::string;

  (*otherImpl_->scriptAdded_) += signature + " = " + body + ";";

  repaint();
}

void WWebWidget::setToolTip(const WMessage& message)
{
  if (!lookImpl_)
    lookImpl_ = new LookImpl();

  if (lookImpl_->toolTip_)
    lookImpl_->toolTip_ = new WMessage();

  *lookImpl_->toolTip_ = message;

  lookImpl_->toolTipChanged_ = true;

  repaint();
}

WMessage WWebWidget::toolTip() const
{
  return lookImpl_ ? (lookImpl_->toolTip_ ? *lookImpl_->toolTip_ : WMessage())
    : WMessage();
}

void WWebWidget::setHidden(bool hidden)
{
  if (hidden)
    flags_.set(BIT_HIDDEN);
  else
    flags_.reset(BIT_HIDDEN);

  flags_.set(BIT_HIDDEN_CHANGED);

  repaint();
}

bool WWebWidget::isHidden() const
{
  return flags_.test(BIT_HIDDEN);
}

void WWebWidget::addChild(WWidget *child)
{
  if (child->parent() == 0) {
    if (!children_)
      children_ = new std::vector<WWidget *>;

    if (std::find(children_->begin(), children_->end(), child)
	== children_->end())
      children_->push_back(child);

    child->parent_ = this;

    if (flags_.test(BIT_LOADED))
      child->load();
  } else {
    std::cerr << "WWebWidget::addChild of child with parent!" << std::endl;
  }
}

void WWebWidget::addNewSibling(WWidget *sibling)
{
  if (!newSiblings_)
    newSiblings_ = new std::vector<WWidget *>;

  newSiblings_->push_back(sibling);

  repaint();
}

void WWebWidget::updateDom(DomElement& element, bool all)
{
  /*
   * determine display
   */
  if (flags_.test(BIT_HIDDEN_CHANGED)
      || flags_.test(BIT_GEOMETRY_CHANGED)
      || all) {
    if (!flags_.test(BIT_HIDDEN)) {
      if (element.isDefaultInline() != flags_.test(BIT_INLINE)) {
	if (flags_.test(BIT_INLINE)) {
	  if (element.type() == DomElement::TABLE)
	    element.setProperty(PropertyStyleDisplay, "inline-table");
	  else if (element.type() != DomElement::TD)
	    element.setProperty(PropertyStyleDisplay, "inline-block");
	} else {
	  element.setProperty(PropertyStyleDisplay, "block");
	}
      } else if (flags_.test(BIT_HIDDEN_CHANGED)) {
	element.setProperty(PropertyStyleDisplay,
			    flags_.test(BIT_INLINE) ? "inline" : "block");
      }
    } else
      element.setProperty(PropertyStyleDisplay, "none");

    flags_.reset(BIT_HIDDEN_CHANGED);
  }

  if (flags_.test(BIT_GEOMETRY_CHANGED) || all) {

    if (layoutImpl_) {
      /*
       * set position
       */
      switch (layoutImpl_->positionScheme_) {
      case Static:
	break;
      case Relative:
	element.setProperty(PropertyStylePosition, "relative"); break;
      case Absolute:
	element.setProperty(PropertyStylePosition, "absolute"); break;
      case Fixed:
	element.setProperty(PropertyStylePosition, "fixed"); break;
      }

      /*
       * set z-index
       */
      if (layoutImpl_->popup_)
	element.setProperty(PropertyStyleZIndex,
			    boost::lexical_cast<std::string>(zIndex()));

      /*
       * set float
       */
      switch (layoutImpl_->floatSide_) {
      case None:
	break;
      case Left:
	element.setProperty(PropertyStyleFloat, "left");
	break;
      case Right:
	element.setProperty(PropertyStyleFloat, "right");
	break;
      default:
	/* illegal values */
	;
      }

      /*
       * set clear: FIXME: multiple values
       */
      switch (layoutImpl_->clearSides_) {
      case None:
	break;
      case Left:
	element.setProperty(PropertyStyleClear, "left");
	break;
      case Right:
	element.setProperty(PropertyStyleClear, "right");
	break;
      case Verticals:
	element.setProperty(PropertyStyleClear, "both");
	break;
      default:
	/* illegal values */
	;
      }

      if (!layoutImpl_->minimumWidth_.isAuto()
	  && (layoutImpl_->minimumWidth_.value() != 0))
	element.setProperty(PropertyStyleMinWidth,
			    layoutImpl_->minimumWidth_.cssText());
      if (!layoutImpl_->minimumHeight_.isAuto()
	  && (layoutImpl_->minimumHeight_.value() != 0))
	element.setProperty(PropertyStyleMinHeight,
			    layoutImpl_->minimumHeight_.cssText());
      if (!layoutImpl_->maximumWidth_.isAuto()) // == none
	element.setProperty(PropertyStyleMaxWidth,
			    layoutImpl_->maximumWidth_.cssText());
      if (!layoutImpl_->maximumHeight_.isAuto()) // == none
	element.setProperty(PropertyStyleMaxHeight,
			    layoutImpl_->maximumHeight_.cssText());

      /*
       * set offsets
       */
      if (layoutImpl_->positionScheme_ != Static) {
	static const Property properties[] = { PropertyStyleTop,
					       PropertyStyleRight,
					       PropertyStyleBottom,
					       PropertyStyleLeft };

	for (unsigned i = 0; i < 4; ++i) {
	  if (!layoutImpl_->offsets_[i].isAuto())
	    element.setProperty(properties[i], layoutImpl_->offsets_[i].cssText());
	}
      }

      /*
       * set vertical alignment
       */
      switch (layoutImpl_->verticalAlignment_) {
      case AlignBaseline:
	break;
      case AlignSub:
	element.setProperty(PropertyStyleVerticalAlign, "sub"); break;
      case AlignSuper:
	element.setProperty(PropertyStyleVerticalAlign, "super"); break;
      case AlignTop:
	element.setProperty(PropertyStyleVerticalAlign, "top"); break;
      case AlignTextTop:
	element.setProperty(PropertyStyleVerticalAlign, "text-top"); break;
      case AlignMiddle:
	element.setProperty(PropertyStyleVerticalAlign, "middle"); break;
      case AlignBottom:
	element.setProperty(PropertyStyleVerticalAlign, "bottom"); break;
      case AlignTextBottom:
	element.setProperty(PropertyStyleVerticalAlign, "text-bottom"); break;
      case AlignLength:
	element.setProperty(PropertyStyleVerticalAlign,
			    layoutImpl_->verticalAlignmentLength_.cssText());
	break;
      }
    }

    /*
     * set width & height
     */
    if (width_ && !width_->isAuto())
      element.setProperty(PropertyStyleWidth, width_->cssText());
    if (height_ && !height_->isAuto())
      element.setProperty(PropertyStyleHeight, height_->cssText());

    flags_.reset(BIT_GEOMETRY_CHANGED);
  }

  if (layoutImpl_) {
    if (layoutImpl_->marginsChanged_ || all) {
      if (layoutImpl_->marginsChanged_
	  || (layoutImpl_->margin_[0].value() != 0))
	element.setProperty(PropertyStyleMarginTop,
			    layoutImpl_->margin_[0].cssText());
      if (layoutImpl_->marginsChanged_
	  || (layoutImpl_->margin_[1].value() != 0))
	element.setProperty(PropertyStyleMarginRight,
			    layoutImpl_->margin_[1].cssText());
      if (layoutImpl_->marginsChanged_
	  || (layoutImpl_->margin_[2].value() != 0))
	element.setProperty(PropertyStyleMarginBottom,
			    layoutImpl_->margin_[2].cssText());
      if (layoutImpl_->marginsChanged_
	  || (layoutImpl_->margin_[3].value() != 0))
	element.setProperty(PropertyStyleMarginLeft,
			    layoutImpl_->margin_[3].cssText());

      layoutImpl_->marginsChanged_ = false;
    }
  }

  if (lookImpl_) {
    if (lookImpl_->toolTip_
	&& (lookImpl_->toolTipChanged_ || all)) {
      if ((lookImpl_->toolTip_->value().length() > 0)
	  || lookImpl_->toolTipChanged_)
	element.setAttribute("title", toUTF8(lookImpl_->toolTip_->value()));

      lookImpl_->toolTipChanged_ = false;
    }

    if (lookImpl_->decorationStyle_)
      lookImpl_->decorationStyle_->updateDomElement(element, all);

    if (lookImpl_->styleClassChanged_
	|| (all && (lookImpl_->styleClass_.length() != 0))) {
      element.setAttribute("class", toUTF8(lookImpl_->styleClass_));
      lookImpl_->styleClassChanged_ = false;
    }
  }

  if (otherImpl_ && otherImpl_->attributes_) {
    if (all) {
      for (std::map<std::string, std::wstring>::const_iterator i
	     = otherImpl_->attributes_->begin();
	   i != otherImpl_->attributes_->end(); ++i)
	element.setAttribute(i->first, toUTF8(i->second));
    } else if (otherImpl_->attributesSet_) {
      for (unsigned i = 0; i < otherImpl_->attributesSet_->size(); ++i) {
	std::string attr = (*otherImpl_->attributesSet_)[i];
	element.setAttribute(attr, toUTF8((*otherImpl_->attributes_)[attr]));
      }

      delete otherImpl_->attributesSet_;
      otherImpl_->attributesSet_ = 0;
    }
  }

  if (otherImpl_) {
    if (all && otherImpl_->scriptFunctions_) {
      DomElement *e = DomElement::createNew(DomElement::SCRIPT);
      e->setAttribute("defer", "true");

      std::string script;
      for (std::map<std::string, std::string>::const_iterator i
	     = otherImpl_->scriptFunctions_->begin();
	   i != otherImpl_->scriptFunctions_->end(); ++i) 
	script += i->first + " = " + i->second + ";";

      e->setProperty(PropertyText, script);
      element.addChild(e);
    } else if (otherImpl_->scriptAdded_) {
      DomElement *e = DomElement::createNew(DomElement::SCRIPT);

      e->setProperty(PropertyText, *otherImpl_->scriptAdded_);
      element.addChild(e);

      delete otherImpl_->scriptAdded_;
      otherImpl_->scriptAdded_ = 0;
    }
  }

  renderOk();

  if (childRemoveChanges_) {
    for (unsigned i = 0; i < childRemoveChanges_->size(); ++i)
      delete (*childRemoveChanges_)[i];

    delete childRemoveChanges_;
    childRemoveChanges_ = 0;
  }
}

bool WWebWidget::isStubbed() const
{
  if (flags_.test(BIT_STUBBED))
    return true;
  else
    if (parent_)
      return parent_->isStubbed();
    else
      return false;
}

bool WWebWidget::isVisible() const
{
  if (flags_.test(BIT_STUBBED) || flags_.test(BIT_HIDDEN))
    return false;
  else
    if (parent_)
      return parent_->isVisible();
    else
      return true;
}

void WWebWidget::getSFormObjects(std::vector<WObject *>& result)
{
  if (isVisible())
    getFormObjects(result);
}

void WWebWidget::getFormObjects(std::vector<WObject *>& formObjects)
{
  if (flags_.test(BIT_FORM_OBJECT))
    formObjects.push_back(this);

  if (children_)
    for (unsigned i = 0; i < children_->size(); ++i)
      (*children_)[i]->webWidget()->getSFormObjects(formObjects);
}

void WWebWidget::getSDomChanges(std::vector<DomElement *>& result)
{
  if (flags_.test(BIT_STUBBED)) {
    if (WApplication::instance()->session()->renderer().visibleOnly()) {
      /* this is during collection of stateless slots */
      getDomChanges(result);
      //propagateRenderOk();
      repaint();
    } else {
      //std::cerr << "Expanding: " << formName() << std::endl;
      flags_.reset(BIT_STUBBED);

      DomElement *stub = DomElement::getForUpdate(this, DomElement::SPAN);
      DomElement *realElement = createDomElement();
      stub->replaceWith(realElement);
      result.push_back(stub);
    }
  } else {
    if (childRemoveChanges_) {
      for (unsigned i = 0; i < childRemoveChanges_->size(); ++i)
	result.push_back((*childRemoveChanges_)[i]);

      delete childRemoveChanges_;
      childRemoveChanges_ = 0;
    }

    if (newSiblings_) {
      for (unsigned i = 0; i < newSiblings_->size(); ++i) {
	DomElement *e = DomElement::getForUpdate(this, DomElement::SPAN);
	DomElement *s = (*newSiblings_)[i]->webWidget()->createSDomElement();
	e->insertBefore(s);
	result.push_back(e);
      }

      delete newSiblings_;
      newSiblings_ = 0;
    }

    getDomChanges(result);
  }
}

void WWebWidget::propagateRenderOk()
{
  if (flags_.test(BIT_STUBBED))
    return;

  if (renderState_ != RenderOk) {
    DomElement *v = DomElement::createNew(DomElement::SPAN);
    updateDom(*v, false);
    delete v;
  }

  if (children_)
    for (unsigned i = 0; i < children_->size(); ++i)
      (*children_)[i]->webWidget()->propagateRenderOk();

  if (childRemoveChanges_) {
    for (unsigned i = 0; i < childRemoveChanges_->size(); ++i)
      delete (*childRemoveChanges_)[i];
    delete childRemoveChanges_;
    childRemoveChanges_ = 0;
  }

  if (newSiblings_) {
    delete newSiblings_;
    newSiblings_ = 0;
  }
}

DomElement *WWebWidget::createSDomElement()
{
  if (flags_.test(BIT_HIDDEN)
      && WApplication::instance()->session()->renderer().visibleOnly()) {
    propagateRenderOk();

    //std::cerr << "Stubbing: " << formName() << std::endl;
    flags_.set(BIT_STUBBED);

    DomElement *stub = DomElement::createNew(DomElement::SPAN);
    stub->setProperty(Wt::PropertyStyleDisplay, "none");
    if (wApp->environment().javaScript())
      stub->setProperty(Wt::PropertyInnerHTML, "...");
    stub->setId(this);

    repaint();

    return stub;
  } else {
    flags_.reset(BIT_STUBBED);

    return createDomElement();
  }
}

void WWebWidget::setNoFormData()
{
  WObject::setNoFormData();
}

void WWebWidget::refresh()
{
  if (lookImpl_ && lookImpl_->toolTip_)
    if (lookImpl_->toolTip_->refresh()) {
      lookImpl_->toolTipChanged_ = true;
      repaint();
    }

  if (children_)
    for (unsigned i = 0; i < children_->size(); ++i)
      (*children_)[i]->refresh();
}

void WWebWidget::setIgnoreChildRemoves(bool how)
{
  if (how)
    flags_.set(BIT_IGNORE_CHILD_REMOVES);
  else
    flags_.reset(BIT_IGNORE_CHILD_REMOVES);
}

/*
 * XML parsing for removing illegal and dangerous tags to
 * prevent XSS.
 */

XERCES_CPP_NAMESPACE_USE

namespace {

class MyHandler : public DefaultHandler, private XMLFormatTarget
{
public:
  MyHandler(std::string& result);

  virtual void writeChars(const XMLByte* const toWrite);
  virtual void writeChars(const XMLByte* const toWrite,
			  const unsigned int count,
			  XMLFormatter* const formatter);

  virtual void characters(const XMLCh *const chars, const unsigned int length);
  virtual void startElement(const XMLCh *const uri,
			    const XMLCh *const localname,
			    const XMLCh *const qname,
			    const Attributes &attrs);
  virtual void endElement(const XMLCh *const uri,
			  const XMLCh *const localname,
			  const XMLCh *const qname);

  virtual void warning(const SAXParseException& exc);
  virtual void error(const SAXParseException& exc);
  virtual void fatalError(const SAXParseException& exc);

private:
  std::string& result_;
  XMLFormatter formatter_;

  bool isBadTag(const std::string name);
  bool isBadAttribute(const std::string name);
  bool isBadAttributeValue(const std::string value);

  int discard_;

  void report(const SAXParseException& exc, const char *kind);
};

MyHandler::MyHandler(std::string& result)
  : result_(result),
    formatter_("UTF8", 0, this),
    discard_(0)
{ }

void MyHandler::writeChars(const XMLByte* const /* toWrite */)
{ }

void MyHandler::writeChars(const XMLByte* const toWrite,
			   const unsigned int count,
			   XMLFormatter* const /* formatter */)
{
  result_.append((char *)toWrite, count);
}

void MyHandler::characters(const XMLCh *const chars,
			   const unsigned int length)
{
  if (discard_ == 0)
    formatter_.formatBuf(chars, length, XMLFormatter::CharEscapes);
}

bool MyHandler::isBadTag(const std::string name)
{
  return (boost::iequals(name, "script")
	  || boost::iequals(name, "applet")
	  || boost::iequals(name, "object")
	  || boost::iequals(name, "iframe")
	  || boost::iequals(name, "frame")
	  || boost::iequals(name, "layer")
	  || boost::iequals(name, "ilayer")
	  || boost::iequals(name, "frameset")
	  || boost::iequals(name, "link")
	  || boost::iequals(name, "meta")
	  || boost::iequals(name, "title")
	  || boost::iequals(name, "base")
	  || boost::iequals(name, "basefont"));
}

bool MyHandler::isBadAttribute(const std::string name)
{
  return (boost::istarts_with(name, "on")
	  || boost::istarts_with(name, "data"));
}

bool MyHandler::isBadAttributeValue(const std::string value)
{
  return (boost::istarts_with(value, "javascript:")
	  || boost::istarts_with(value, "vbscript:"));
}

void MyHandler::startElement(const XMLCh *const uri,
			     const XMLCh *const localname,
			     const XMLCh *const qname,
			     const Attributes& attributes)
{
  const char *name = XMLString::transcode(qname);
  if (isBadTag(name)) {
    std::cerr << "Warning: discarding invalid tag: "
	      << name << std::endl;
    ++discard_;
  }
  XMLString::release((char **) &name);

  if (discard_ == 0) {
    // The name has to be representable without any escapes
    formatter_ << XMLFormatter::NoEscapes << chOpenAngle;
    formatter_ << qname;

    unsigned int len = attributes.getLength();
    for (unsigned int index = 0; index < len; index++) {
      const char *aname = XMLString::transcode(attributes.getQName(index));
      const char *v = XMLString::transcode(attributes.getValue(index));

      if (!isBadAttribute(aname) && !isBadAttributeValue(v)) {
	//
	//  Again the name has to be completely representable. But the
	//  attribute can have refs and requires the attribute style
	//  escaping.
	//
	formatter_ << XMLFormatter::NoEscapes << chSpace;
	formatter_ << attributes.getQName(index);
	formatter_ << chEqual << chDoubleQuote
		   << XMLFormatter::AttrEscapes
		   << attributes.getValue(index)
		   << XMLFormatter::NoEscapes
		   << chDoubleQuote;
      } else
	std::cerr << "Warning: discarding invalid attribute: "
		  << aname << std::endl;

      XMLString::release((char **) &aname);
      XMLString::release((char **) &v);
    }

    formatter_ << chCloseAngle;
  }
}

void MyHandler::endElement(const XMLCh *const uri,
			   const XMLCh *const localname,
			   const XMLCh *const qname)
{
  static const XMLCh  gEndElement[] = { chOpenAngle, chForwardSlash, chNull };
  if (discard_ == 0) {
    // No escapes are legal here
    formatter_ << XMLFormatter::NoEscapes << gEndElement
	       << qname << chCloseAngle;
  }

  const char *name = XMLString::transcode(qname);
  if (isBadTag(name))
    --discard_;
  XMLString::release((char **) &name);
}

void MyHandler::warning(const SAXParseException& e)
{ 
  report(e, "Warning");
}

void MyHandler::error(const SAXParseException& e)
{
  report(e, "Error");
}

void MyHandler::fatalError(const SAXParseException& e)
{
  report(e, "Fatal error");
}

void MyHandler::report(const SAXParseException& e, const char *kind)
{
  const char *message = XMLString::transcode(e.getMessage());

  std::cerr << kind << "XHTML parse error: ";

  std::cerr << " line " << e.getLineNumber()
	    << ", char " << e.getColumnNumber()
	    << ": " << message << std::endl;

  XMLString::release((char **) &message);
}

}

std::string WWebWidget::removeScript(const std::string text)
{
  std::string result;
  result.reserve(text.size());

  SAX2XMLReader* reader = XMLReaderFactory::createXMLReader();
  MyHandler handler(result);
  reader->setContentHandler(&handler);
  reader->setErrorHandler(&handler);

  MemBufInputSource source((const XMLByte *)text.c_str(), text.length(),
			   "text");
  reader->parse(source);
  delete reader;

  return result;
}

static void replace(std::wstring& s, wchar_t c, std::wstring r)
{
  std::wstring::size_type p = 0;

  while ((p = s.find(c, p)) != std::wstring::npos) {
    s.replace(p, 1, r);
    p += r.length();
  }
}

std::wstring WWebWidget::escapeText(const std::wstring text,
				    bool newlinestoo)
{
  std::wstring result = text;

  replace(result, '&', L"&amp;");
  replace(result, '<', L"&lt;");
  replace(result, '>', L"&gt;");
  // replace(result, '"', "&quot;");
  // replace(result, '\'', "&apos;");
  if (newlinestoo)
    replace(result, '\n', L"<br />");

  return result;
}

void WWebWidget::load()
{
  flags_.set(BIT_LOADED);

  if (children_)
    for (unsigned i = 0; i < children_->size(); ++i)
      (*children_)[i]->load();
}

bool WWebWidget::loaded() const
{
  return flags_.test(BIT_LOADED);
}

WWebWidget::OtherImpl
::DropMimeType::DropMimeType(const std::wstring aHoverStyleClass)
  : hoverStyleClass(aHoverStyleClass)
{ }

WWebWidget::OtherImpl::DropMimeType::DropMimeType()
{ }

bool WWebWidget::setAcceptDropsImpl(const std::string mimeType, bool accept,
				    std::wstring hoverStyleClass)
{
  bool result  = false; // whether the signal needs to be connected.
  bool changed = false;

  if (!otherImpl_)
    otherImpl_ = new OtherImpl();
  if (!otherImpl_->acceptedDropMimeTypes_)
    otherImpl_->acceptedDropMimeTypes_ = new OtherImpl::MimeTypesMap;

  OtherImpl::MimeTypesMap::iterator i
    = otherImpl_->acceptedDropMimeTypes_->find(mimeType);

  if (i == otherImpl_->acceptedDropMimeTypes_->end()) {
    if (accept) {
      result = otherImpl_->acceptedDropMimeTypes_->empty();
      (*otherImpl_->acceptedDropMimeTypes_)[mimeType]
	= OtherImpl::DropMimeType(hoverStyleClass);
      changed = true;
    }
  } else {
    if (!accept) {
      otherImpl_->acceptedDropMimeTypes_->erase(i);
      changed = true;
    }
  }

  if (changed) {
    std::wstring mimeTypes = L"";

    for (OtherImpl::MimeTypesMap::const_iterator i
	   = otherImpl_->acceptedDropMimeTypes_->begin();
	 i != otherImpl_->acceptedDropMimeTypes_->end(); ++i) {
      mimeTypes
	+= L"{" + widen(i->first) + L":" + i->second.hoverStyleClass + L"}";
    }

    setAttributeValue("amts", mimeTypes);
  }

  if (result && !otherImpl_->dropEventSignal_)
    otherImpl_->dropEventSignal_ = new EventSignal<WDropEvent>(this);

  return result;
}

void WWebWidget::updateSignalConnection(DomElement& element,
					EventSignalBase &signal,
					const std::string eventName,
					bool all)
{
  if (all || signal.needUpdate()) {

    if (signal.isConnected())
      element.setEventSignal(eventName, signal);
    else
      element.setEvent(eventName, "", "");

    signal.updateOk();
  }
}
