/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <iostream>
#include <boost/lexical_cast.hpp>

#include "DomElement.h"
#include "EscapeOStream.h"

#include "WObject"
#include "WApplication"
#include "WEnvironment"

using std::exit;

namespace {

const char *elementNames_[] =
  { "a", "br", "button", "col", "div", "fieldset", "form", "h1", "h2", "h3", "h4",
    "h5", "h6", "iframe", "img", "input", "label", "legend", "li", "ol",
    "option", "ul", "script", "select", "span", "table", "tbody", "td", "textarea",
    "tr", "p" };

bool defaultInline_[] =
  { true, true, true, false, false, false, false, true, false, false, false,
    false, false, true, true, true, true, true, false, false,
    true, false, false, true, true, false, false, false, true,
    false, false };

}

namespace Wt {

int DomElement::nextId_ = 0;

DomElement *DomElement::createNew(ElementType type)
{
  DomElement *e = new DomElement(ModeCreate, type);
  return e;
}

DomElement *DomElement::getForUpdate(const std::string id,
				     ElementType type)
{
  DomElement *e = new DomElement(ModeUpdate, type);
  e->id_ = id;

  return e;
}

DomElement *DomElement::getForUpdate(const WObject *object,
				     ElementType type)
{
  return getForUpdate(object->formName(), type);
}

DomElement::DomElement(Mode mode, ElementType type)
  : mode_(mode),
    wasEmpty_(false),
    deleted_(false),
    removeAllChildren_(false),
    replaced_(0),
    insertBefore_(0),
    type_(type),
    numManipulations_(0),
    timeOut_(-1)
{ 
}

DomElement::~DomElement()
{
  for (unsigned i = 0; i < childrenToAdd_.size(); ++i)
    delete childrenToAdd_[i].second;

  if (replaced_)
    delete replaced_;

  if (insertBefore_)
    delete insertBefore_;  
}

void DomElement::setWasEmpty(bool how)
{
  wasEmpty_ = how;
}

void DomElement::addChild(DomElement *child)
{
  ++numManipulations_;

  if ((mode_ == ModeCreate || wasEmpty_) && canWriteInnerHTML()) {
    std::stringstream out;
    {
      EscapeOStream sout(out);

      child->asHTML(sout, timeouts_);
    }

    childrenHtml_ += out.str();
    delete child;
  } else
    childrenToAdd_.push_back(std::make_pair("", child));
}

void DomElement::setAttribute(const std::string attribute,
			      const std::string value)
{
  ++numManipulations_;
  attributes_[attribute] = value;
}

void DomElement::setEventSignal(const std::string eventName,
				const EventSignalBase& signal)
{
  setEvent(eventName, signal.javaScript(),
	   signal.encodeCmd(), signal.isExposedSignal());
}

void DomElement::setEvent(const std::string eventName,
			  std::string jsCode,
			  const std::string signalName,
			  bool isExposed)
{
  if (isExposed)
    jsCode += "update(this, '" + signalName + "', event, true);";

  ++numManipulations_;
  eventHandlers_[eventName] = std::make_pair(jsCode, signalName);
}

DomElement::EventAction::EventAction(const std::string aJsCondition,
				     const std::string aJsCode,
				     const std::string anUpdateCmd,
				     bool anExposed)
  : jsCondition(aJsCondition),
    jsCode(aJsCode),
    updateCmd(anUpdateCmd),
    exposed(anExposed)
{ }

void DomElement::setEvent(const std::string eventName,
			  const std::vector<EventAction>& actions)
{
  std::string code = "var signals='';";

  for (unsigned i = 0; i < actions.size(); ++i) {
    if (!actions[i].jsCondition.empty())
      code += "if(" + actions[i].jsCondition + "){";
    code += actions[i].jsCode;
    if (actions[i].exposed) {
      code += "if(signals.length != 0){signals += ',';} signals += '"
	+ actions[i].updateCmd + "';";
    }
    if (!actions[i].jsCondition.empty())
      code += "}";
  }

  code += "if(signals.length!=0){update(this, signals, event, true);}";

  setEvent(eventName, code, "");
}

void DomElement::processEvents() const
{
  /*
   * when we have a mouseUp event, we also need a mouseDown event
   * to be able to compute dragDX/Y.
   */
  EventHandlerMap::const_iterator mouseup = eventHandlers_.find("mouseup");

  if (mouseup != eventHandlers_.end()) {
    if (!mouseup->second.first.empty()) {
      DomElement *self = const_cast<DomElement *>(this);
      self->eventHandlers_["mousedown"].first += "saveDownPos(event);";
    }
  }
}

void DomElement::setTimeout(int msec)
{
  ++numManipulations_;
  timeOut_ = msec;
}

void DomElement::callJavaScript(const std::string jsCode)
{
  ++numManipulations_;
  javaScript_ += jsCode;
}

void DomElement::setProperty(Wt::Property property, std::string value)
{
  ++numManipulations_;
  properties_[property] = value;
}

void DomElement::setId(const std::string id, bool andName)
{
  ++numManipulations_;
  id_ = id;
  if (andName)
    setAttribute("name", id);
}

void DomElement::setId(const WObject *object, bool andName)
{
  setId(object->formName(), andName);
}

void DomElement::insertChild(DomElement *child, const std::string beforeId)
{
  ++numManipulations_;
  childrenToAdd_.push_back(std::make_pair(beforeId, child));
}

void DomElement::insertBefore(DomElement *sibling)
{
  ++numManipulations_;
  insertBefore_ = sibling;
}

void DomElement::removeFromParent()
{
  ++numManipulations_;
  deleted_ = true;
}

void DomElement::removeAllChildren()
{
  ++numManipulations_;
  removeAllChildren_ = true;
}

void DomElement::replaceWith(DomElement *newElement)
{
  ++numManipulations_;
  replaced_ = newElement;
}

void DomElement::callMethod(const std::string method)
{
  ++numManipulations_;
  methodCalls_.push_back(method);
}

/*
 * In-place version
 */
static void replace(std::string& s, char c, std::string r)
{
  std::string::size_type p = 0;

  while ((p = s.find(c, p)) != std::string::npos) {
    s.replace(p, 1, r);
    p += r.length();
  }
}

EscapeOStreamScope DomElement::jsStringLiteral(EscapeOStream& out,
					       char delimiter)
{
  EscapeOStreamScope result = out.scope();

  out.replace('\n', "\\n");
  out.replace('\r', "\\r");
  out.replace('\t', "\\t");
  out.replace(delimiter, std::string("\\") + '\'');
  out.replace('\\', "\\\\");

  return result;
}

void DomElement::jsStringLiteral(std::ostream& out, const std::string& s,
				 char delimiter)
{
  EscapeOStream sout(out);
  jsStringLiteral(sout, s, delimiter);
}

void DomElement::jsStringLiteral(EscapeOStream& out, const std::string& s,
				 char delimiter)
{
  /*
   * How to convert a random piece of text in a well-behaved javascript
   * string ?
   * 1. Replace \ -> \\
   * 2. Remove newlines -> \n
   * 3. Remove carriage returns -> \r
   * 4. Remove tab characters -> \t
   * 5. Replace delimiter -> \delimiter
   */

  out << delimiter;

  EscapeOStreamScope scope = jsStringLiteral(out, delimiter);
  out << s;
  scope.pop();

  out << delimiter;
}

void DomElement::htmlAttributeValue(EscapeOStream& out, const std::string& s)
{
  out << '"';

  EscapeOStreamScope scope = out.scope();
  out.replace('\"', "&#34");
  out << s;
  scope.pop();

  out << '"';
}

std::string DomElement::cssStyle() const
{
  std::string style = "";
  for (PropertyMap::const_iterator i = properties_.begin();
       i != properties_.end(); ++i) {
    if ((i->first >= Wt::PropertyStylePosition)
	&& (i->first <= Wt::PropertyStyleDisplay)) {
      static std::string cssNames[] =
	{ "position",
	  "z-index", "float", "clear",
	  "width", "height",
	  "min-width", "min-height",
	  "max-width", "max-height",
	  "left", "right", "top", "bottom",
	  "vertical-align", "text-align",
	  "padding",
	  "margin-top", "margin-right",
	  "margin-bottom", "margin-left", "cursor",
	  "border-top", "border-right",
	  "border-bottom", "border-left",
	  "color", "overflow", "overflow",
	  "font-family", "font-style", "font-variant",
	  "font-weight", "font-size",
	  "background-color", "background-image", "background-repeat",
	  "background-position",
	  "text-decoration", "table-layout", "border-spacing",
	  "display"};

      if ((i->first == Wt::PropertyStyleCursor) && (i->second == "pointer")) {
	style += "cursor:pointer;cursor:hand;";	    
      } else {
	style += cssNames[i->first - Wt::PropertyStylePosition]
	  + ":" + i->second + ";";
      }
    }
  }

  return style;
}

void DomElement
::createTimeoutJs(EscapeOStream& out,
		  const std::vector<std::pair<int, std::string> >& timeouts)
{
  for (unsigned i = 0; i < timeouts.size(); ++i)
    out << "addTimerEvent('"<< timeouts[i].second << "', " 
	<<boost::lexical_cast<std::string>(timeouts[i].first)
	<< ");";
}

void DomElement::asHTML(EscapeOStream& out,
			std::vector<std::pair<int, std::string> >& timeouts)
  const
{
  if (mode_ != ModeCreate) {
    std::cerr << "ERROR: DomElement::asHTML() called with ModeUpdate"
	      << std::endl;
    exit(1);
  }

  processEvents();

  bool needButtonWrap
    = (!(WApplication::instance()->environment().javaScript())
       && (eventHandlers_.find("click") != eventHandlers_.end())
       && (eventHandlers_.find("click")->second.first.length() > 0));

  ElementType renderedType
    = (needButtonWrap && (type_ == BUTTON)) ? SPAN : type_;

  if (needButtonWrap) {
    out << "<button type=\"submit\" name=\"signal\" value=";
    htmlAttributeValue(out, eventHandlers_.find("click")->second.second);
    out << (type_ != BUTTON ? " class=\"wrap\"" : "");

    PropertyMap::const_iterator i = properties_.find(Wt::PropertyDisabled);
    if ((i != properties_.end()) && (i->second=="true"))
      out << " disabled";

    out << ">";
  }

  out << "<" << elementNames_[renderedType];

  if (id_.length() > 0) {
    out << " id=";
    htmlAttributeValue(out, id_);
  }

  for (AttributeMap::const_iterator i = attributes_.begin();
       i != attributes_.end(); ++i) {
    out << " " << i->first << "=";
    htmlAttributeValue(out, i->second);
  }

  if (WApplication::instance()->environment().javaScript()) {
    for (EventHandlerMap::const_iterator i = eventHandlers_.begin();
	 i != eventHandlers_.end(); ++i) {
      if (i->second.first.length() != 0) {
	out << " on" << i->first << "=";
	htmlAttributeValue(out, i->second.first);
      }
    }
  }

  std::string innerHTML = "";

  for (PropertyMap::const_iterator i = properties_.begin();
       i != properties_.end(); ++i) {
    switch (i->first) {
    case Wt::PropertyText:
    case Wt::PropertyInnerHTML:
      innerHTML += i->second; break;
    case Wt::PropertyDisabled:
      if (i->second == "true")
	out << " disabled";
      break;
    case Wt::PropertyChecked:
      if (i->second == "true")
	out << " checked";
      break;
    case Wt::PropertySelected:
      if (i->second == "true")
	out << " selected";
      break;
    case Wt::PropertyValue:
      out << " value=";
      htmlAttributeValue(out, i->second);
      break;
    case Wt::PropertySrc:
      out << " src=";
      htmlAttributeValue(out, i->second);
      break;
    default:
      break;
    }
  }

  std::string style = cssStyle();

  if (style.length() > 0) {
    out << " style=";
    htmlAttributeValue(out, style);
  }

  if (renderedType != BR) {
    out << ">" << innerHTML << childrenHtml_;

    for (unsigned i = 0; i < childrenToAdd_.size(); ++i)
      childrenToAdd_[i].second->asHTML(out, timeouts);

    out << "</" << elementNames_[renderedType] << ">";
  } else
    out << " />";

  if (needButtonWrap)
    out << "</button>";

  if (timeOut_ != -1)
    timeouts.push_back(std::make_pair(timeOut_, id_));

  timeouts.insert(timeouts.end(), timeouts_.begin(), timeouts_.end());
}

std::string DomElement::createReference() const
{
  if (mode_ == ModeCreate) 
    return "document.createElement('"
      + std::string(elementNames_[type_]) + "')";
  else
    return "getElement('" + id_ + "')";
}

std::string DomElement::declare() const
{
  var_ = "j" + boost::lexical_cast<std::string>(nextId_++);

  return var_ + "=" + createReference() + ';';
}

bool DomElement::canWriteInnerHTML() const
{
  /*
   * http://lists.apple.com/archives/web-dev/2004/Apr/msg00122.html
   * "The problem is not that innerHTML doesn't work (it works fine),
   *  but that Safari can't handle writing the innerHTML of a <tbody> tag.
   *  If you write the entire table (including <table> and <tbody>) in the
   *  innerHTML string it works fine.
   */
  /* http://msdn.microsoft.com/workshop/author/tables/buildtables.asp
   * Note When using Dynamic HTML (DHTML) to create a document, you can 
   * create objects and set the innerText or innerHTML property of the object.
   * However, because of the specific structure required by tables,
   * the innerText and innerHTML properties of the table and tr objects are
   * read-only.
   */
  if (type_ == TBODY
      || type_ == TABLE
      || type_ == TR)
    return false;

  return true; //!containsElement(SCRIPT);
}

bool DomElement::containsElement(ElementType type) const
{
  for (unsigned i = 0; i < childrenToAdd_.size(); ++i) {
    if (childrenToAdd_[i].second->type_ == type)
      return true;
    if (childrenToAdd_[i].second->containsElement(type))
      return true;
  }

  return false;
}

void DomElement::declare(EscapeOStream& out) const
{
  if (var_.empty())
    out << declare();
}

std::string DomElement::asJavaScript(EscapeOStream& out,
				     Priority priority) const
{
  switch(priority) {
  case Delete:
    if (deleted_ || removeAllChildren_) {
      out << declare();

      if (deleted_) {
	out << var_ << ".parentNode.removeChild("
	    << var_ << ");";
      } else  if (removeAllChildren_) {
	out << "while (" << var_ << ".firstChild) {"
	    << var_ << ".removeChild(" << var_ << ".firstChild);"
	    << "}";
      }
    }

    return var_;

    break;
  case Create:
    if (mode_ == ModeCreate) {
      out << declare();

      if (id_.length() != 0)
	out << var_ << ".setAttribute('id', '" << id_ << "');";
    }

    return var_;

    break;
  case Update:
  {
    /*
     * short-cut for frequent short manipulations
     */
    if (mode_ == ModeUpdate && numManipulations_ == 1) {
      if (properties_.find(Wt::PropertyStyleDisplay) != properties_.end()) {
	std::string style = properties_.find(Wt::PropertyStyleDisplay)->second;
	if (style == "none") {
	  out << "hide('" << id_ << "');";
	  return var_;
	} else if (style == "inline") {
	  out << "inline('" + id_ + "');";
	  return var_;
	} else if (style == "block") {
	  out << "block('" + id_ + "');";
	  return var_;
	}
      }
    }

    processEvents();

    if (deleted_)
      break;

    if (replaced_) {
      declare(out);

      std::string varr = replaced_->asJavaScript(out, Create);
      replaced_->asJavaScript(out, Update);

      out << varr << ".style.display = " << var_ << ".style.display;";
      out << var_ << ".parentNode.replaceChild("
	  << varr << "," << var_ << ");";

      return var_;
    } else if (insertBefore_) {
      declare(out);

      std::string varr = insertBefore_->asJavaScript(out, Create);
      insertBefore_->asJavaScript(out, Update);

      out << var_ << ".parentNode.insertBefore(" << varr << ","
	  << var_ + ");";

      return var_;
    }

    const AttributeMap& attributesToSet = attributes_;

    for (AttributeMap::const_iterator i = attributesToSet.begin();
	 i != attributesToSet.end(); ++i) {
      declare(out);

      if (i->first == "class") {
	out << var_ << ".className = ";
	jsStringLiteral(out, i->second, '\'');
	out << ";";
      } else {
	out << var_ << ".setAttribute('" << i->first << "',";
	jsStringLiteral(out, i->second, '\'');
	out << ");";
      }
    }


    for (EventHandlerMap::const_iterator i = eventHandlers_.begin();
	 i != eventHandlers_.end(); ++i) {
      if ((mode_ == ModeUpdate) || (i->second.first.length() > 0)) {
	declare(out);

	std::string fName = "f" + boost::lexical_cast<std::string>(nextId_++);

	out << "function " << fName
	    << "(event) { if (!event) event = window.event; "
	    << i->second.first << "}";

	out << var_ << ".on" << i->first << "=" << fName << ";";
      }
    }

    if ((mode_ == ModeCreate || wasEmpty_) && canWriteInnerHTML()) {
      if (!childrenToAdd_.empty() || !childrenHtml_.empty()) {
	declare(out);

	out << var_ << ".innerHTML=\'";

	EscapeOStreamScope scope = jsStringLiteral(out, '\'');

	out << childrenHtml_;

	TimeoutList timeouts;
	for (unsigned i = 0; i < childrenToAdd_.size(); ++i)
	  childrenToAdd_[i].second->asHTML(out, timeouts);

	scope.pop();

	out << "\';";

	createTimeoutJs(out, timeouts);
      }
    } else {
      for (unsigned i = 0; i < childrenToAdd_.size(); ++i) {
	declare(out);

	DomElement *child = childrenToAdd_[i].second;

	std::string cvar = child->asJavaScript(out, Create);
	child->asJavaScript(out, Update);
	
	if (childrenToAdd_[i].first.length() > 0) {
	  DomElement *beforeElement
	    = getForUpdate(childrenToAdd_[i].first, DIV);

	  out << var_ << ".insertBefore("
	      << cvar << "," << beforeElement->createReference() << ");";

	  delete beforeElement;
	} else {
	  out << var_ << ".appendChild(" << cvar << ");";
	}
      }
    }

    for (PropertyMap::const_iterator i = properties_.begin();
	 i != properties_.end(); ++i) {
      declare(out);

      std::string p;
      std::string value;

      switch(i->first) {
      case Wt::PropertyInnerHTML:
	out << var_ << ".innerHTML=";
	jsStringLiteral(out, i->second, '\'');
	out << ';';
	break;
      case Wt::PropertyValue:
	out << var_ << ".value=";
	jsStringLiteral(out, i->second, '\'');
	out << ';';
	break;
      case Wt::PropertyDisabled:
	out << var_ << ".disabled=" << i->second << ';';
	break;
      case Wt::PropertyChecked:
	out << var_ << ".checked=" << i->second << ';';
	break;
      case Wt::PropertySelected:
	out << var_ << ".selected=" << i->second << ';';
	break;
      case Wt::PropertySelectedIndex:
	out << var_ << ".selectedIndex=" << i->second << ';';
	break;
      case Wt::PropertySrc:
	out << var_ << ".src='" << i->second << "\';";
	break;
      case Wt::PropertyText:
	out << var_ << ".text=";
	jsStringLiteral(out, i->second, '\'');
	out << ';';
	break;
      default:
	if ((i->first >= Wt::PropertyStylePosition)
	    && (i->first <= Wt::PropertyStyleDisplay)) {
	  static std::string cssNames[] =
	    { "position",
	      "zIndex", "float", "clear",
	      "width", "height",
	      "minWidth", "minHeight",
	      "maxWidth", "maxHeight",
	      "left", "right", "top", "bottom",
	      "verticalAlign", "textAlign",
	      "padding",
	      "marginTop", "marginRight",
	      "marginBottom", "marginLeft",
	      "cursor", 	    
	      "borderTop", "borderRight",
	      "borderBottom", "borderLeft",
	      "color", "overflow", "overflow",
	      "fontFamily", "fontStyle", "fontVariant",
	      "fontWeight", "fontSize",
	      "backgroundColor", "backgroundImage", "backgroundRepeat",
	      "backgroundPosition",
	      "textDecoration", "tableLayout", "borderSpacing", "display" };
	  out << var_ << ".style."
	      << cssNames[i->first - Wt::PropertyStylePosition]
	      << "=\'" << i->second << "\';";
	}
      }
    }

    for (unsigned i = 0; i < methodCalls_.size(); ++i) {
      declare(out);
      out << var_ << "." << methodCalls_[i] << ';';
    }

    if (timeOut_ != -1)
      out << "addTimerEvent('" << id_ << "', " 
	  << boost::lexical_cast<std::string>(timeOut_) + ");";


    out << javaScript_;

    return var_;
  }
  }

  return var_;
}

bool DomElement::isDefaultInline() const
{
  return defaultInline_[type_];
}

}
