// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#ifndef DOMELEMENT_H_
#define DOMELEMENT_H_

#include <map>
#include <vector>
#include <string>

class WObject;
class WSignalInstance_;
class JavaScript;
class JSVar;

namespace Wt {
  enum Property { PropertyInnerHTML, PropertyValue, PropertyDisabled,
                  PropertyChecked, PropertySelected, PropertySelectedIndex,
		  PropertySrc,
//		  PropertyCellSpacing, PropertyCellPadding, //wangpeng
                  PropertyStylePosition,
		  PropertyStyleZIndex, PropertyStyleFloat, PropertyStyleClear,
		  PropertyStyleWidth, PropertyStyleHeight,
		  PropertyStyleMinWidth, PropertyStyleMinHeight,
		  PropertyStyleMaxWidth, PropertyStyleMaxHeight,
		  PropertyStyleLeft, PropertyStyleRight,
		  PropertyStyleTop, PropertyStyleBottom,
		  PropertyStyleVerticalAlign, PropertyStyleTextAlign,
		  PropertyStylePadding,
		  PropertyStyleMarginTop, PropertyStyleMarginRight,
		  PropertyStyleMarginBottom, PropertyStyleMarginLeft,
		  PropertyStyleCursor, PropertyStyleBorder, PropertyStyleColor,
		  PropertyStyleOverflowX,
		  PropertyStyleOverflowY,
		  PropertyStyleFontFamily,
		  PropertyStyleFontStyle,
		  PropertyStyleFontVariant,
		  PropertyStyleFontWeight,
		  PropertyStyleFontSize,
		  PropertyStyleBackgroundColor,
		  PropertyStyleBackgroundImage,
		  PropertyStyleBackgroundRepeat,
		  PropertyStyleTableLayout, PropertyStyleBorderSpacing,
		  
		  PropertyStyleDisplay };
};

class __declspec(dllexport) DomElement
{
public:
  enum ElementType { A, BR, BUTTON, COl, DIV, FIELDSET, H1, H2, H3, H4,
		     H5, H6, IFRAME, IMG, INPUT, LABEL, LEGEND, LI, OL,
		     OPTION, UL, SELECT, SPAN, TABLE, TBODY, TD, TEXTAREA,
		     TR, P };

  ~DomElement();

  static DomElement *createNew(ElementType type);
  static DomElement *getForUpdate(const std::string id, ElementType type);
  static DomElement *getForUpdate(const WObject *object, ElementType type);

  static int m_nObjCount;
  /*
   * General methods (for both createnew and update modes)
   */
  void addChild(DomElement *child);
  void setAttribute(const std::string attribute, const std::string value);
  void addEventSignal(const std::string eventName,
		      const WSignalInstance_& signal,
		      const char *argName,
		      const char *condition);
  void setEventSignal(const std::string eventName,
		      const WSignalInstance_& signal,
		      const char *argName,
		      const char *condition);
  void setEvent(const std::string eventName,
		const std::string jsCode);
  void addEvent(const std::string eventName,
		const std::string jsCode);
  void removeEventSignal(const std::string eventName);

  void setProperty(Wt::Property property, std::string value);
  void setId(const std::string id, bool andName = false);
  void setId(const WObject *object, bool andName = false);

  /*
   * only for ModeUpdate
   */
  void insertChild(DomElement *child, const std::string beforeId);
  void removeAllChildren();
  void removeFromParent();
  void replaceWith(DomElement *newElement);
  void insertBefore(DomElement *sibling);

  /*
   * generate the statements to create the child and return the
   * variable which refers to the element.
   */
  JSVar createJSStatements(JavaScript& js) const;
  std::string asHTML() const;

  ElementType type() const { return type_; }

  bool isDefaultInline() const;
  std::string createReference() const;

  std::string id() const { return id_; }

  char * redict_;

  //char * bkcolor1_;
  //char * bkcolor2_;
  char * contextmenu_;


  //bool handtext;

private:
  enum Mode { ModeCreate, ModeUpdate };

  DomElement(Mode mode, ElementType type);
  bool canWriteInnerHTML() const;

  Mode        mode_;
  bool        deleted_;         // when removeFromParent() is called
  bool	      removeAllChildren_;
  DomElement *replaced_;        // when replaceWith() is called
  DomElement *insertBefore_;
  ElementType type_;
  std::string id_;
  int         numManipulations_;

  typedef std::map<std::string, std::string> AttributeMap;
  typedef std::map<Wt::Property, std::string> PropertyMap;
  typedef std::map<std::string, std::pair<std::string, std::string> >
    EventHandlerMap; // jsCode, signalNape

  AttributeMap    attributes_;
  PropertyMap     properties_;
  EventHandlerMap eventHandlers_;

  /*
   * The string is the location of the child, if it has to be inserted
   * somewhere (ModeUpdate)
   */
  std::vector<std::pair<std::string, DomElement *> > childrenToAdd_;
};

#endif // DOMELEMENT_H_
