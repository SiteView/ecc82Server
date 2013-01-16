/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <iostream>

#include "WApplication"
#include "WWidget"
#include "WWebWidget"
#include "WSlotInstance_"
#include "JavaScript.h"
#include "DomElement.h"

#include <iostream>
//#include "WWidget"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

#include <windows.h>

WSlot_ WWidget::slots_[] = { WSlot_("hide()", true), WSlot_("show()", true), WSlot_() };
WSignal_ WWidget::signals_[] = { WSignal_() };

void WWidget::buildSlotMap(WSlotMap& result)
{
  WResource::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WWidget::buildSignalMap(WSignalMap& result)
{
  WResource::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool WWidget::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  if (slot == slots_ + 0) {
    sender_ = sender;    hide();
    return true;
  }
  if (slot == slots_ + 1) {
    sender_ = sender;    show();
    return true;
  }
  return WResource::triggerSlot(sender, slot, args);
}
bool WWidget::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  if (slot == slots_ + 0) {
    undoHide();
    return true;
  }
  if (slot == slots_ + 1) {
    undoShow();
    return true;
  }
  return WResource::undoTriggerSlot(slot, args);
}


WWidget::WWidget()
{ }

WWidget::~WWidget()
{ }

void WWidget::updateStaticSlotCode(WWebWidget *mainWidget, JavaScript& js)
{
  static void *args[20];

  /*
   * find a connection to a static slot that needs update of
   * its static code
   */
  WSlotInstance_ *s = 0;
  do
  {
	   s = findConnectedNeedTriggerStaticSlot();
	   if (s) 
	   {

			/* collect the static code */
			s->trigger(0, args);

			std::vector<DomElement *> changes;
			mainWidget->getSDomChanges(changes);
			JavaScript changeJS;
			for (unsigned i = 0; i < changes.size(); ++i) 
			{
				changes[i]->createJSStatements(changeJS);
				delete changes[i];
			}
			changes.clear();

			/* undo the effects of the static code collections */
			s->undoTrigger(args);

			mainWidget->getSDomChanges(changes);
			for (unsigned ii = 0; ii < changes.size(); ++ii) 
			{
				delete changes[ii];
			}
			changes.clear();

			/* collect the changes that are consequence of this
			* static code collection */

			//OutputDebugString(changeJS.statements().c_str());
			//OutputDebugString("--- changeJS.statements() \n ");

			s->setStaticCode(changeJS.statements());


			mainWidget->getSDomChanges(changes);
			int a =changes.size();
			for (unsigned i1 = 0; i1 < changes.size(); ++i1) 
			{	
				changes[i1]->createJSStatements(js);
				delete changes[i1];
	
			}
			
		}
  } while (s);
  
  int size = children().size();


//  printf("children().size(): %d\n", size);
  
  for (unsigned i = 0; i < size; ++i) 
  {
    children()[i]->webWidget()->updateStaticSlotCode(mainWidget, js);
  }
}

std::string WWidget::resourceMimeType() const
{
  return "text/html";
}

void WWidget::streamResourceData(FCGI_FILE *stream)
{
  DomElement *element = webWidget()->createSDomElement();

  FCGI_fprintf(stream,
	       "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\""
	       "\"http://www.w3.org/TR/REC-html40/strict.dtd\">"
	       "<html lang=\"en\" dir=\"ltr\">\n"
	       "<head>\n"
	       "<meta http-equiv='Pragma' content='no-cache'>\n"
	       "<style type=\"text/css\">\n");

  FCGI_fprintf(stream, "%s", wApp ? wApp->styleSheet().cssText().c_str() : "");

  FCGI_fprintf(stream, "\n</style>\n"
	       "<script>\n"
	       "function getElement(arg) { return window.parent.getElement(arg); }\n"
	       "function update(arg) { window.parent.update(arg); }\n"
	       "function hide(o) { "
	       "document.getElementById(o).style.display = 'none'; }\n"
	       "function inline(o) { "
	       "document.getElementById(o).style.display = 'inline'; }\n"
	       "function block(o) { "
	       "document.getElementById(o).style.display = 'block'; }\n"
	       "</script></head><body>%s</body></html>",
	       element->asHTML().c_str());

  delete element;
}
