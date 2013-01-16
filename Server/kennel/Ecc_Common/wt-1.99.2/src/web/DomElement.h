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

namespace Wt {

class EscapeOStream;
class EscapeOStreamScope;
class EventSignalBase;
class WObject;

enum Property { PropertyInnerHTML, PropertyValue, PropertyDisabled,
		PropertyChecked, PropertySelected, PropertySelectedIndex,
		PropertySrc, PropertyText,
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
		PropertyStyleCursor, 
		PropertyStyleBorderTop, PropertyStyleBorderRight,
		PropertyStyleBorderBottom, PropertyStyleBorderLeft,
		PropertyStyleColor,
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
		PropertyStyleBackgroundPosition,
		PropertyStyleTextDecoration,
		PropertyStyleTableLayout, PropertyStyleBorderSpacing,
		PropertyStyleDisplay };

class DomElement
{
public:
  enum ElementType { A, BR, BUTTON, COl, DIV, FIELDSET, FORM, H1, H2, H3, H4,
		     H5, H6, IFRAME, IMG, INPUT, LABEL, LEGEND, LI, OL,
		     OPTION, UL, SCRIPT, SELECT, SPAN, TABLE, TBODY, TD, TEXTAREA,
		     TR, P };

  ~DomElement();

  static DomElement *createNew(ElementType type);
  static DomElement *getForUpdate(const std::string id, ElementType type);
  static DomElement *getForUpdate(const WObject *object, ElementType type);

  /*
   * General methods (for both createnew and update modes)
   */
  void setWasEmpty(bool how); // allows optimisation of addChild()
  void addChild(DomElement *child);
  void setAttribute(const std::string attribute, const std::string value);

  void setEventSignal(const std::string eventName,
		      const EventSignalBase& signal);

  void setEvent(const std::string eventName,
		std::string jsCode,
		const std::string signalName,
		bool isExposed = false);

  struct EventAction
  {
    std::string jsCondition;
    std::string jsCode;
    std::string updateCmd;
    bool        exposed;

    EventAction(const std::string jsCondition, const std::string jsCode,
		const std::string updateCmd, bool exposed);
  };

  void setEvent(const std::string eventName,
		const std::vector<EventAction>& actions);

  void setProperty(Wt::Property property, std::string value);
  void setId(const std::string id, bool andName = false);
  void setId(const WObject *object, bool andName = false);
  void setTimeout(int msec);

  /*
   * only for ModeUpdate
   */
  void insertChild(DomElement *child, const std::string beforeId);
  void removeAllChildren();
  void removeFromParent();
  void replaceWith(DomElement *newElement);
  void insertBefore(DomElement *sibling);
  void callMethod(const std::string method);
  void callJavaScript(std::string javascript);

  enum Priority { Delete, Create, Update };
  typedef std::vector<std::pair<int, std::string> > TimeoutList;

  std::string asJavaScript(EscapeOStream& out, Priority priority) const;
  void asHTML(EscapeOStream& out, TimeoutList& timeouts) const;
  static void createTimeoutJs(EscapeOStream& out, const TimeoutList& timeouts);

  ElementType type() const { return type_; }

  bool isDefaultInline() const;
  std::string declare() const;
  void declare(EscapeOStream& out) const;
  std::string createReference() const;

  std::string id() const { return id_; }

  std::string cssStyle() const;

  static EscapeOStreamScope jsStringLiteral(EscapeOStream& out,
					    char delimiter);
  static void jsStringLiteral(EscapeOStream& out, const std::string& s,
			      char delimiter);
  static void jsStringLiteral(std::ostream& out, const std::string& s,
			      char delimiter);
  static void htmlAttributeValue(EscapeOStream& out, const std::string& s);

private:
  enum Mode { ModeCreate, ModeUpdate };

  DomElement(Mode mode, ElementType type);
  bool canWriteInnerHTML() const;
  bool containsElement(ElementType type) const;
  void processEvents() const;

  Mode         mode_;
  bool         wasEmpty_;
  bool         deleted_;         // when removeFromParent() is called
  bool	       removeAllChildren_;
  DomElement  *replaced_;        // when replaceWith() is called
  DomElement  *insertBefore_;
  ElementType  type_;
  std::string  id_;
  int          numManipulations_;
  std::vector<std::string> methodCalls_;
  int          timeOut_;
  std::string  javaScript_;
  mutable std::string  var_;

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
  std::string childrenHtml_;
  TimeoutList timeouts_;

  static int nextId_;

  friend class WCssDecorationStyle;
};

}

#endif // DOMELEMENT_H_
