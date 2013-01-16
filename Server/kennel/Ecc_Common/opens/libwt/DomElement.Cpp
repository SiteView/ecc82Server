/*
* Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
*
* See the LICENSE file for terms of use.
*/
#include <utility>
#include <iostream>

#include "DomElement.h"
#include "WebSession.h"
#include "JavaScript.h"

#include "WSignalInstance_"
#include "WObject"

using namespace Wt;

//typedef struct pair<char*,char*> PAIR_IF;


static std::string elementNames_[] =
{ "a", "br", "button", "col", "div", "fieldset", "h1", "h2", "h3", "h4",
"h5", "h6", "iframe", "img", "input", "label", "legend", "li", "ol",
"option", "ul", "select", "span", "table", "tbody", "td", "textarea",
"tr", "p" };

static bool defaultInline_[] =
{ true, true, true, false, false, false, false, false, false, false,
false, false, true, true, true, true, true, false, false,
true, false, true, true, false, false, false, true,
false, false };

int DomElement::m_nObjCount = 0;

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
deleted_(false),
removeAllChildren_(false),
replaced_(0),
insertBefore_(0),
type_(type),
numManipulations_(0)
{ 
    m_nObjCount ++;
    //	handtext = false;
    redict_ = NULL;
    contextmenu_ = NULL;
}

DomElement::~DomElement()
{
    if(replaced_)
        delete replaced_;
    if(insertBefore_)
        delete insertBefore_;
    m_nObjCount--;
  for (unsigned i = 0; i < childrenToAdd_.size(); ++i)
    delete childrenToAdd_[i].second;
}

void DomElement::addChild(DomElement *child)
{
  ++numManipulations_;
  std::string Temp = "";
  childrenToAdd_.push_back(std::make_pair(Temp, child));
  //childrenToAdd_.push_back(std::make_pair("a", "a"));
}

void DomElement::setAttribute(const std::string attribute, const std::string value)
{
  ++numManipulations_;
  attributes_[attribute] = value;
}

void DomElement::removeEventSignal(const std::string eventName)
{
  ++numManipulations_;
  std::string Temp = "";
  eventHandlers_[eventName] = std::make_pair(Temp, Temp);
}

void DomElement::setEventSignal(const std::string eventName,
				const WSignalInstance_& signal,
				const char *argName,
				const char *condition)
{
	std::string Temp = "";
  eventHandlers_[eventName] = std::make_pair(Temp, Temp);
  addEventSignal(eventName, signal, argName, condition);
}

void DomElement::setEvent(const std::string eventName,
			  const std::string jsCode)
{
	std::string Temp = "";
  eventHandlers_[eventName] = std::make_pair(jsCode, Temp);
}

void DomElement::addEvent(const std::string eventName,
			  const std::string jsCode)
{
  eventHandlers_[eventName].first += jsCode;
}

void DomElement::addEventSignal(const std::string eventName,
				const WSignalInstance_& signal,
				const char *argName,
				const char *condition)
{
   

		++numManipulations_;
		std::string JScode;
		
		
		signal.getStaticCode(JScode);
		//OutputDebugString(JScode.c_str());
		//OutputDebugString("---addEventSignal   \n");
		std::string signalName = signal.encodeCmd();
		if (argName != 0) {
			signalName = "'" + signalName + "&arg=' + event." + argName;
		} else
			signalName = "'" + signalName + "'";
		
		
		switch(signal.nJavaSignal) {
		case WObject::ConnectionType::JAVASCRIPT:
				JScode  += signal.strJsFun;
			break;

		case WObject::ConnectionType::JAVASCRIPTUPDATE:
			JScode += signal.strJsFun;
			JScode +=	signalName;
			JScode +=	");";
			
			break;
		case WObject::ConnectionType::JAVASCRIPTDYNAMIC:
			
			JScode += "if(update(" + signalName + ")==0)";
			JScode  += signal.strJsFun;
			
			break;
		default:
			JScode += "update(" + signalName + ");";
			break;
			
			


		}

		
		
		if( redict_ != NULL )
		{
			JScode += redict_;
		}
		
		if (condition)
			JScode = std::string("if (") + condition + ") {" + JScode + "}";
		
		eventHandlers_[eventName].first = JScode;
		eventHandlers_[eventName].second = signal.encodeCmd();

		//OutputDebugString(JScode.c_str());
		//OutputDebugString(" --event ---  \n");


  
}

void DomElement::setProperty(Property property, std::string value)
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
//  childrenToAdd_.push_back(std::make_pair(beforeId, child));
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

static std::string replace(std::string s, char c, std::string r)
{
  unsigned p = 0;

  while ((p = s.find(c, p)) != std::string::npos) {
    s.replace(p, 1, r);
    p += r.length();
  }

  return s;
}

static std::string escape(std::string s, char c)
{
  return replace(s, c, std::string("\\") + c);
}

std::string DomElement::asHTML() const
{
	
  if (mode_ != ModeCreate) {
    std::cerr << "ERROR: DomElement::asHTML() called with ModeUpdate"
	      << std::endl;
    exit(1);
  }


  bool needButtonWrap
    = (!(WebSession::instance_->haveJavaScript())
       && (eventHandlers_.find("click") != eventHandlers_.end())
       && (eventHandlers_.find("click")->second.first.length() > 0));

  std::string result;

  ElementType renderedType
    = (needButtonWrap && (type_ == BUTTON)) ? SPAN : type_;

  result = "<" + elementNames_[renderedType];

  if (id_.length() > 0)
    result += " id='" + id_ + "'";

  if(contextmenu_ != NULL)
  {
	  result +=" ";
	  result += contextmenu_;
	  result += " ";
  }

  std::string  strTextAreaText;
  std::string  strTemp("");
  for (AttributeMap::const_iterator i = attributes_.begin();
       i != attributes_.end(); ++i)
	   {
           strTemp = replace(i->second, '\\', "&#92;");
           //strTemp = replace(i->second, '\\', "\\\\");
		   if(renderedType==TEXTAREA)
				strTextAreaText= replace(strTemp, '\'', "&#39;");
           //result += " " + i->first + "='" + strTemp + "'";
           result += " " + i->first + "='" + replace(strTemp, '\'', "&#39;") + "'";
	   }
    

  if (WebSession::instance_->haveJavaScript()) {
    for (EventHandlerMap::const_iterator i = eventHandlers_.begin();
	 i != eventHandlers_.end(); ++i) {
      if (i->second.first.length() != 0)
		  if(strcmp(i->first.c_str(), "redict") == 0)
		  {
			  result += " onClick='"
				  + replace(i->second.first, '\'', "\"") + "'";

			  
		  }
		  else
		  {
			result += " on" + i->first + "='"
				+ replace(i->second.first, '\'', "\"") + "'";
			
			//OutputDebugString(result.c_str());
			//OutputDebugString("asHTML-------------  \n");
		  }
    }
  }

  std::string innerHTML = "";
  std::string style = "";
  for (PropertyMap::const_iterator i1 = properties_.begin();
       i1 != properties_.end(); ++i1) {
    switch (i1->first) {
    case PropertyInnerHTML:
      innerHTML += i1->second; break;
    case PropertyDisabled:
      if (i1->second == "true")
	result += " disabled='disabled'";
      break;
    case PropertyChecked:
      if (i1->second == "true")
	result += " checked='checked'";
      break;
    case PropertySelected:
      if (i1->second == "true")
	result += " selected='selected'";
      break;
    case PropertySrc:
      result += " src='" + i1->second + "'";
      break;
    default:
      if ((i1->first >= PropertyStylePosition)
	  && (i1->first <= PropertyStyleDisplay)) {
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
	    "border", "color", "overflow", "overflow",
	    "font-family", "font-style", "font-variant",
	    "font-weight", "font-size",
	    "background-color", "background-image", "background-repeat",
	    "table-layout", "border-spacing",
	    "display"};

	if ((i1->first == PropertyStyleCursor) && (i1->second == "pointer")) {
	    style += "cursor:pointer;cursor:hand;";	    
	} else {
	  style += cssNames[i1->first - PropertyStylePosition]
	    + ":" + i1->second + ";";
	}
      }
      break;
    }
  }

  if (style.length() > 0)
    result += " style='" + style + '\'';

  for (unsigned i2 = 0; i2 < childrenToAdd_.size(); ++i2) {
    innerHTML += childrenToAdd_[i2].second->asHTML();
  }


  
  if(renderedType==TEXTAREA)
  {
	  innerHTML+=strTextAreaText;
	  

  }

  if (renderedType != BR)
    result += ">" + innerHTML + "</" + elementNames_[renderedType] + ">";
  else
    result += " />";

    //result = replace(result, '\\', "\\\\");
  /*
   * Wrap inside a submit button if needed.
   */
  if (!(WebSession::instance_->haveJavaScript())
      && (eventHandlers_.find("click") != eventHandlers_.end())
      && (eventHandlers_.find("click")->second.first.length() > 0))
    result
      = std::string("<button type='submit' name='signal' value='")
      + eventHandlers_.find("click")->second.second + "'"
      + (type_ != BUTTON
	 ? " style='border:0px;background-color:white;margin:0px;padding:0px;font-size:medium;pointer:hand;cursor:pointer;cursor:hand;'"
	 : "") + ">"
      + result + "</button>";
	
  //char aa[20];
  //sprintf(aa,"%s\n",id_.c_str());

  //OutputDebugString(aa);
  ///Sleep(500);


  return result;
}

std::string DomElement::createReference() const
{
  if (mode_ == ModeCreate)
    return "document.createElement('" + elementNames_[type_] + "')";
  else
  {

	 return "getElement('" + id_ + "')";
  }
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
  return (type_ != TBODY
	  && type_ != TABLE
	  && type_ != TR);
}

JSVar DomElement::createJSStatements(JavaScript& js) const
{
  /*
   * short-cut for frequent short manipulations
   */
	  JSVar tmpe = js.declareVar(createReference());

	  //OutputDebugString(  createReference().c_str());
	
	  //this;
  if (mode_ == ModeUpdate && numManipulations_ == 1) {
	  
    if (properties_.find(PropertyStyleDisplay) != properties_.end()) {
      std::string style = properties_.find(PropertyStyleDisplay)->second;
      if (style == "none") {
		js.addStatement("hide('" + id_ + "');");
		//char ab[200]={0};
		//sprintf(ab,"hide %s \n",id_.c_str());
	
		//OutputDebugString(ab);
		return JSVar();
      } else if (style == "inline") {
		js.addStatement("inline('" + id_ + "');");
		return JSVar();
      } else if (style == "block") {
		js.addStatement("block('" + id_ + "');");
		return JSVar();
      }

    }

  }
  

  JSVar e = js.declareVar(createReference());
  
  if (deleted_) {
    js.addStatement(e.name() + ".parentNode.removeChild(" + e.name() + ");");
    return e;
  } else if (replaced_) {    
    JSVar c = replaced_->createJSStatements(js);
    js.addStatement(e.name() + ".parentNode.replaceChild(" + c.name()
		    + "," + e.name() + ");");
    return c;
  } else if (insertBefore_) {
    JSVar c = insertBefore_->createJSStatements(js);
    js.addStatement(e.name() + ".parentNode.insertBefore(" + c.name() + ","
		    + e.name() + ");");
    return c;
  }
  if (removeAllChildren_) {
    js.addStatement("while (" + e.name() + ".firstChild) {");
    js.addStatement(" " + e.name()
		    + ".removeChild(" + e.name() + ".firstChild);");
    js.addStatement("}");
  }

  AttributeMap attributesToSet = attributes_;
  if (mode_ == ModeCreate)
    if (id_.length() != 0)
      attributesToSet["id"] = id_;



  
    ElementType renderedType   = type_;


//  sprintf(sztmp,"%d\n",renderedType);
 // OutputDebugString(sztmp);
  for (AttributeMap::const_iterator i = attributesToSet.begin();
       i != attributesToSet.end(); ++i) {

		   if((renderedType== TEXTAREA) &&(i->first=="value"))
		   {
			   std::string strTxtAVal;
			   strTxtAVal =i->second;
			   strTxtAVal=replace(strTxtAVal,'\r',"");
			   strTxtAVal=replace(strTxtAVal,'\n'," ");
			   //strTxtAVal=replace(strTxtAVal,'\'',"&#39;");
			   //strTxtAVal=replace(strTxtAVal, '\\', "&#92;");
			   strTxtAVal=replace(strTxtAVal, '\\', "\\\\");
			   js.addStatement(e.name() + ".setAttribute(\"" + i->first + "\", \""
				+ strTxtAVal + "\");");


		   }
		   else{
               //std::string strTemp = replace(i->second,'\\', "&#92;");
            std::string strTemp = replace(i->second,'\\', "\\\\");
   			js.addStatement(e.name() + ".setAttribute(\"" + i->first + "\", \""
					+ strTemp  + "\");");
					//+ replace(strTemp,'\'',"&#39;") + "');");
	    
			if (i->first == "class")
					js.addStatement(e.name() + ".setAttribute('className', '"
						+ i->second + "');");
		   }
  }
  
  for (EventHandlerMap::const_iterator i1 = eventHandlers_.begin();
       i1 != eventHandlers_.end(); ++i1) {
    if ((mode_ == ModeUpdate) || (i1->second.first.length() > 0)) {

		
		std::string fName = js.getTempFunctionName();
		js.addStatement("function " + fName + "() {" + i1->second.first + "}");

		js.addStatement(e.name() + ".on" + i1->first + "=" + fName + ";");

      /*
      js.addStatement("if (" + e.name() + ".addEventListener) {");
      js.addStatement(e.name() + ".addEventListener('" + i->first + "', "
		      + fName + ", false);");
      js.addStatement("} else if (" + e.name() + ".attachEvent) {");
      js.addStatement(e.name() + ".attachEvent('on" + i->first + "', "
		      + fName + ");");
      js.addStatement("}");
      */
    }
  }

  if (mode_ == ModeCreate && canWriteInnerHTML()) {
    if (!childrenToAdd_.empty()) {
      std::string innerHTML;
      for (unsigned i2 = 0; i2 < childrenToAdd_.size(); ++i2)
	  {
		innerHTML += childrenToAdd_[i2].second->asHTML();
		//OutputDebugString( childrenToAdd_[i2].second->asHTML().c_str());
		//OutputDebugString("  --------- childrenToAdd \n");
	  }
    
	  //if(createReference()=="getElement('o4b5')")
	  //{
		 // OutputDebugString(innerHTML.c_str());
		 // OutputDebugString("---innerHTML  \n");
	  //}
      innerHTML = escape(innerHTML, '"');
	  innerHTML = replace(innerHTML, '\r', "");
      innerHTML = replace(innerHTML, '\n', " ");
      js.addStatement(e.name() + ".innerHTML=\""
		      + innerHTML + "\";");

	
    }

  } else {
    for (unsigned i2 = 0; i2 < childrenToAdd_.size(); ++i2) {
      JSVar c = childrenToAdd_[i2].second->createJSStatements(js);
      if (childrenToAdd_[i2].first.length() > 0) {
	DomElement *beforeElement = getForUpdate(childrenToAdd_[i2].first, DIV);
	JSVar before = js.declareVar(beforeElement->createReference());
	js.addStatement(e.name()
			+ ".insertBefore(" + c.name() + "," + before.name() + ");");
	delete beforeElement;
      } else {
	js.addStatement(e.name() + ".appendChild(" + c.name() + ");");
      }
    }
	
	
  }
  
  for (PropertyMap::const_iterator i3 = properties_.begin();
       i3 != properties_.end(); ++i3) {
    std::string p;
    std::string value;

	std::string strValue1;

	std::string strRef;
	strRef =createReference();

    switch(i3->first) {
    case PropertyInnerHTML:
      p = "innerHTML";
	  strValue1 =replace(i3->second, '\n', " ");
	  //strValue1 =replace(i3->second, '\n', " ");
	  strValue1 =replace(strValue1, '\r', "");
	   	
      //value = "'" + replace(i3->second, '\n', " ") + "'";
	  //strValue1 = replace(strValue1, '\\', "&#92;");
	  strValue1 = replace(strValue1, '\\', "\\\\");
	  value = "\"" + strValue1 + "\"";
	  //value = "'" + replace(strValue1, '\'', "&#39;")+"'";
	  //strValue1.erase(strValue1.begin(),strValue1.begin() + strValue1.find_first_not_of(' '));
	  //strValue1.erase(strValue1.begin() + strValue1.find_last_not_of(' ') + 1,strValue1.end());

	  //char *newtxt;
	  //newtxt= (char *)malloc(strValue1.length()+10);
	  //memset(newtxt,0,strValue1.length()+10);
	  //sprintf(newtxt,"'%s'",strValue1.c_str());
	  //value = "\'" + strValue1;
	  //value =value+ "\'";
	  //value = newtxt;
	  /*
	  if(strRef=="getElement('o4b5')")
	  {
		 // value = "\'\'";
		  //sprintf(newtxt,"\'%s\'",strValue1.c_str());
		  memset(aaa,0,2000);	
		  sprintf(aaa,"\n  ap is  --%s=%s--\n","aaaa",newtxt);
		  OutputDebugString(aaa);
	  }*/
	  //free(newtxt);
      break;
    case PropertyValue:
      p = "value";

	  value= replace(i3->second,'\r', "");
	  value=replace(value,'\n', "\\n");
	  //value =replace(value, '\'', "&#39;");
	  //value =replace(value, '\\', "&#92;");
	  value =replace(value, '\\', "\\\\");
	  value = "\"" + value + "\"";

      //value = "'" + i3->second + "'";
      break;
    case PropertyDisabled:
      p = "disabled";
      value = i3->second;
      break;
    case PropertyChecked:
      p = "checked";
      value = i3->second;
      break;
    case PropertySelected:
      p = "selected";
      value = i3->second;
      break;
    case PropertySelectedIndex:
      p = "selectedIndex";
      value = i3->second;
      break;
    case PropertySrc:
      p = "src";
      value = '\'' + i3->second + '\'';
      break;
	  /*
	case PropertyCellSpacing:
	//	OutputDebugString("PropertyCellSpacing\n");
	//	result += " cellSpacing='" + i1->second + "'";
		//result += " cellSpacing=" + i1->second + "";
      continue;
	case PropertyCellPadding:
		//OutputDebugString("PropertyCellPadding\n");
	//	result += " cellPadding='" + i1->second + "'";
      //continue;
*/

    default:
		
      if ((i3->first >= PropertyStylePosition)
	  && (i3->first <= PropertyStyleDisplay)) {
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
	    "cursor", "border", "color", "overflow", "overflow",
	    "fontFamily", "fontStyle", "fontVariant",
	    "fontWeight", "fontSize",
	    "backgroundColor", "backgroundImage", "backgroundRepeat",
	    "tableLayout", "borderSpacin", "display" };
	p = "style." + cssNames[i3->first - PropertyStylePosition];
	value = '\'' + i3->second + '\'';
      }
    }

    js.addStatement(e.name() + "." + p + "=" + value + ";");
  }
  
  return e;
}

bool DomElement::isDefaultInline() const
{
  return defaultInline_[type_];
}
