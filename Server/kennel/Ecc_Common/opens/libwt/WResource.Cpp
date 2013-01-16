/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <string>
#include <iostream>
#include <boost/lexical_cast.hpp>

#include "WResource"
#include "WApplication"

#include <iostream>
//#include "WResource"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WResource::slots_[] = { WSlot_() };
WSignal_ WResource::signals_[] = { WSignal_("dataChanged()", false), WSignal_() };

void WResource::buildSlotMap(WSlotMap& result)
{
  WObject::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WResource::buildSignalMap(WSignalMap& result)
{
  WObject::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

void WResource::dataChanged()
{
  void **args = 0;
  triggerSignal(signals_ + 0, args);
}

bool WResource::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  return WObject::triggerSlot(sender, slot, args);
}
bool WResource::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WObject::undoTriggerSlot(slot, args);
}


WResource::WResource()
{ }

WResource::~WResource()
{ 
  if (wApp && wApp->isExposedResource(this))
    wApp->removeExposedResource(this);
}

const std::string WResource::generateUrl() const
{
  if (!wApp->isExposedResource(this))
    wApp->addExposedResource(this);

  char buf[11];
  sprintf(buf, "o%08x", reinterpret_cast<unsigned>(this));

  //return wApp->url() + "&resource=" + std::string(buf)
    //+ "&rand=" + boost::lexical_cast<std::string>((int)(drand48() * 100000));
   return wApp->url() + "&resource=" + std::string(buf)
    + "&rand=" + boost::lexical_cast<std::string>((int)(rand() * 100000));
}

WResource *WResource::decodeResource(const std::string id)
{
  unsigned resourceInt;

  sscanf(id.c_str(), "o%x", &resourceInt);
  WObject *object = reinterpret_cast<WObject *>(resourceInt);

  WResource *result = dynamic_cast<WResource *>(object);

  if (wApp->isExposedResource(result))
    return result;
  else {
    std::cerr << "Error: decodeResource did not yield an exposed resource."
	      << std::endl;
    exit(1);
  }
}
