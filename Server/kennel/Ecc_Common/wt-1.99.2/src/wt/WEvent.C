/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <boost/lexical_cast.hpp>

#include "CgiParser.h"

#include "WApplication"
#include "WEvent"

namespace {
  using namespace Wt;

  int parseIntEntry(const CgiParser& parser, std::string name, int ifMissing) {
    CgiEntry *e;

    if ((e = parser.getEntry(name))) {
      try {
	return boost::lexical_cast<int>(e->value());
      } catch (boost::bad_lexical_cast) {
	std::cerr << "Could not cast to int: \"" << name 
		  << "\": " << e->value()
		  << std::endl;
	return ifMissing;
      }
    } else
      return ifMissing;
  }

  std::string getStringEntry(const CgiParser& parser, std::string name) {
    CgiEntry *e;

    if ((e = parser.getEntry(name))) {
      return e->value();
    } else
      return std::string();
  }
}

namespace Wt {

JavascriptEvent::JavascriptEvent()
{ }

void JavascriptEvent::get(const CgiParser& parser)
{
  clientX = parseIntEntry(parser, "clientX", 0);
  clientY = parseIntEntry(parser, "clientY", 0);
  documentX = parseIntEntry(parser, "documentX", 0);
  documentY = parseIntEntry(parser, "documentY", 0);
  screenX = parseIntEntry(parser, "screenX", 0);
  screenY = parseIntEntry(parser, "screenY", 0);
  widgetX = parseIntEntry(parser, "widgetX", 0);
  widgetY = parseIntEntry(parser, "widgetY", 0);
  dragDX = parseIntEntry(parser, "dragdX", 0);
  dragDY = parseIntEntry(parser, "dragdY", 0);

  alt = parser.getEntry("altKey") != 0;
  ctrl = parser.getEntry("ctrlKey") != 0;
  shift = parser.getEntry("shiftKey") != 0;
  meta = parser.getEntry("metaKey") != 0;

  code = parseIntEntry(parser, "keyCode", 0);
  CgiEntry *e;
  charCode = (e = parser.getEntry("char")) ? e->value() : ""; 
  right = (e = parser.getEntry("right")) ? (e->value() == "true") : false;

  std::string dsid = getStringEntry(parser, "dsid");
  if (dsid.empty())
    dropSource = WApplication::instance()->decodeObject(dsid);
  else
    dropSource = 0;

  dropMimeType = getStringEntry(parser, "dmt");
}

WMouseEvent::WMouseEvent()
{ }

WMouseEvent::WMouseEvent(const JavascriptEvent& jsEvent)
  : jsEvent_(jsEvent)
{ }

WKeyEvent::WKeyEvent(const JavascriptEvent& jsEvent)
  : jsEvent_(jsEvent)
{ }

WDropEvent::WDropEvent(const JavascriptEvent& jsEvent)
  : WMouseEvent(jsEvent)
{ }

}
