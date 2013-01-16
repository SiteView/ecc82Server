/*
* Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
*
* See the LICENSE file for terms of use.
*/
#include <iostream>

#include "WApplication"
#include "WSignalInstance_"
#include "WebSession.h"
#include "WContainerWidget"

#include <iostream>
//#include "WApplication"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WApplication::slots_[] = { WSlot_("quit()", false), WSlot_("scheduleUploadFiles()", false), WSlot_() };
WSignal_ WApplication::signals_[] = { WSignal_() };

void WApplication::buildSlotMap(WSlotMap& result)
{
    WObject::buildSlotMap(result);
    for (int i = 0; slots_[i].good(); ++i)
        result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WApplication::buildSignalMap(WSignalMap& result)
{
    WObject::buildSignalMap(result);
    for (int i = 0; signals_[i].good(); ++i)
        result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool WApplication::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
    if (slot == slots_ + 0) {
        sender_ = sender;    quit();
        return true;
    }
    if (slot == slots_ + 1) {
        sender_ = sender;    scheduleUploadFiles();
        return true;
    }
    return WObject::triggerSlot(sender, slot, args);
}
bool WApplication::undoTriggerSlot(const WSlot_ *slot, void **args)
{
    return WObject::undoTriggerSlot(slot, args);
}


WApplication *WApplication::instance_ = 0;
WApplication *wApp = 0;

WApplication::WApplication(int argc, char* argv[])
: loading_(false),
quited_(false),
needSubmit_(false),
submit_(false),
oaplDownloadPath_("http://witty.sourceforge.net/")
{
    root_ = new WContainerWidget();

    //  styleSheet_.addRule("table", "border-collapse: collapse; border: 10px");
    // styleSheet_.addRule("TABLE",   "display:table;width:auto;height:80px");
    ///// styleSheet_.addRule("table,tr,td,input, select, textarea", "font-size:9pt;font-family: Arial, Helvetica, sans-serif");

    // styleSheet_.addRule("input, select, textarea", "font-size:9pt;font-family: Arial, Helvetica, sans-serif");;width: 100%

    //  styleSheet_.addRule("h2",  "font-family: Arial; font-size: 40pt; Color:blue; "); line-height:48pt; 

    // styleSheet_.addRule("DIV, TD, IMG", "margin: 0px; padding: 0px;");
    //styleSheet_.addRule("DIV", "line-height: 1.2; font-size: 10pt;");


    //styleSheet_.addRule(".bg_CCCC99 ", "border-color: #C1CDD8 black black black; border-style: solid; border-top-width: 1px; border-right-width: 0px; border-bottom-width: 0px; border-left-width: 0px;width: 100%");

    //styleSheet_.addRule(".input_border ", "  background-color: #ECF4F9; border: #006699; border-style: solid; border-top-width: 1px; border-right-width: 1px; border-bottom-width: 1px; border-left-width: 1px ");

    //styleSheet_.addRule(".back_cell", "background-image:url(../icons/t2title.gif); border-bottom:solid 1px #A3A9B1;width: 100%");

    //styleSheet_.addRule(".back_Title", "background-image:url(../icons/t1title.gif); border:solid 1px #A3A9B1;width: 100%");

  //styleSheet_.addRule(".Table_Width", "width: 100%");
  
  //styleSheet_.addRule(".Cell_10", "width: 10%");

  //styleSheet_.addRule(".Cell_98", "width: 98%;");

  //styleSheet_.addRule(".Help_Font", "border:solid 1px;width: 98%;background-color:#D4D9E2;font-size:9pt;font-family:Arial;");

  //styleSheet_.addRule(".bg_Border", "border:solid 1px #999999;background-color:#FFFFFF;OVERFLOW: auto;width: 100%;"); 
  
  //styleSheet_.addRule(".commonbutton", "border:solid 1px #999999;background-color:#FFFFFF;cursor:pointer;"); 

  //styleSheet_.addRule(".bg_white", "background-color:#FFFFFF"); 

  //styleSheet_.addRule(".bg_row", "background-color:#F3F3F3"); 
/*
  WCssDecorationStyle style;
  
  style.setBackgroundColor(Wt::yellow);
  styleSheet_.addRule("*.suggestion:hover", style);

  style = WCssDecorationStyle();
  style.setBackgroundColor(Wt::yellow);
  style.setBorder(WBorder(WBorder::Outset, WBorder::Medium)); 
  style.setCursor(WCssDecorationStyle::Pointer);
  styleSheet_.addRule("*.tabbutton", style);

  style = WCssDecorationStyle();
  style.setBackgroundColor(Wt::yellow);
  style.setBorder(WBorder(WBorder::Solid, WBorder::Medium, Wt::darkYellow)); 
  styleSheet_.addRule("*.tabbuttoncurrent", style);
*/
  instance_ = this;
  wApp = this;

  nrefreshtime =0;
}

WApplication::~WApplication()
{
	delete(root_);
	instance_ = 0;
	wApp = 0;
}

int WApplication::exec()
{
//	WebSession session("ses");
//	return session.WApplicationExecCallback(this);
	return WebSession::WApplicationExecCallback(this);
}


WApplication *WApplication::instance()
{
  return instance_;
}

void WApplication::setTitle(const std::string title)
{
  title_ = title;
}

std::string WApplication::url() const
{
  return WebSession::instance_->url_;
}

std::string WApplication::submiturl() const
{
	return WebSession::instance_->submitUrl_;
}

void WApplication::quit()
{
  quited_ = true;
}

void WApplication::scheduleUploadFiles()
{
  needSubmit_ = true;
}

void WApplication::setOAPLDownloadPath(const std::string path)
{
  oaplDownloadPath_ = path;
}

void WApplication::addExposedSignal(const WSignalInstance_ *signal)
{
  exposedSignals_.insert(signal);
}

void WApplication::removeExposedSignal(const WSignalInstance_ *signal)
{
  std::set<const WSignalInstance_ *>::iterator i
    = exposedSignals_.find(signal);

  if (i == exposedSignals_.end()) 
  {
    std::cerr << "Error: WApplication::removeExposedSignal internal "
      "consistency problem." << std::endl;
  }
  else
  {
    exposedSignals_.erase(signal);
  }
}

bool WApplication::isExposedSignal(const WSignalInstance_ *signal) const
{
  return
    exposedSignals_.find(signal) != exposedSignals_.end();
}

void WApplication::addExposedResource(const WResource *resource)
{
  std::cerr << "Exposing a resource. " << std::endl;
  exposedResources_.insert(resource);
}

void WApplication::removeExposedResource(const WResource *resource)
{
  std::cerr << "Removing exposed resource. " << std::endl;
  std::set<const WResource *>::iterator i
    = exposedResources_.find(resource);

  if (i == exposedResources_.end()) {
    std::cerr << "Error: WApplication::removeExposedResource internal "
      "consistency problem." << std::endl;
  } else {
    exposedResources_.erase(resource);
  }
}

bool WApplication::isExposedResource(const WResource *resource) const
{
  return exposedResources_.find(resource) != exposedResources_.end();
}

bool WApplication::haveJavaScript() const
{
  return WebSession::instance_->haveJavaScript();
}

void WApplication::refresh()
{
  messageResourceBundle_.refresh();
  root_->refresh();
}

void WApplication::setBodyAttribute(const std::string strBody )
{
	strBodyString =strBody;
}
void WApplication::setRefreshTime(int nUpdateTime,std::string  strFuncName)
{

	nrefreshtime= nUpdateTime;
	strrefreshfunc= strFuncName;

	//char aaa[100];
	//sprintf(aaa,"\n--wa:   %d--%s\n",getRefreshTime(),getRefreshFunc().c_str());
  //OutputDebugString(aaa);
}