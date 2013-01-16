//#include "WSignalMapper"

#include <iostream>
#include "WSignalMapper"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WSignalMapper::slots_[] = { WSlot_("map()", false), WSlot_("map(WObject *)", false), WSlot_("senderDestroyed(WObject *)", false), WSlot_() };
WSignal_ WSignalMapper::signals_[] = { WSignal_("mapped(int)", false), WSignal_("mapped(const std::string)", false), WSignal_("mapped(WWidget *)", false), WSignal_() };

void WSignalMapper::buildSlotMap(WSlotMap& result)
{
  WObject::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WSignalMapper::buildSignalMap(WSignalMap& result)
{
  WObject::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

void WSignalMapper::mapped(int arg0)
{
  void *args[1] = {(void *)(&arg0)};  triggerSignal(signals_ + 0, args);
}

void WSignalMapper::mapped(const std::string arg0)
{
  void *args[1] = {(void *)(&arg0)};  triggerSignal(signals_ + 1, args);
}

void WSignalMapper::mapped(WWidget * arg0)
{
  void *args[1] = {(void *)(&arg0)};  triggerSignal(signals_ + 2, args);
}

bool WSignalMapper::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  if (slot == slots_ + 0) {
    sender_ = sender;    map();
    return true;
  }
  if (slot == slots_ + 1) {
    sender_ = sender;    map(*((WObject * *)args[0]));
    return true;
  }
  if (slot == slots_ + 2) {
    sender_ = sender;    senderDestroyed(*((WObject * *)args[0]));
    return true;
  }
  return WObject::triggerSlot(sender, slot, args);
}
bool WSignalMapper::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WObject::undoTriggerSlot(slot, args);
}


WSignalMapper::WSignalMapper()
{ }

WSignalMapper::~WSignalMapper()
{ }

WObject* WSignalMapper::mapping(WWidget *widget) const
{
    for(std::map<WObject *, WWidget *>::const_iterator i = widgetMappings_.begin(); i != widgetMappings_.end(); i ++)
    {
        if(i->second == widget)
            return i->first;
    }
    return NULL;
}

WObject* WSignalMapper::mapping(const std::string &id) const
{
    for(std::map<WObject *, std::string>::const_iterator i = stringMappings_.begin(); i != stringMappings_.end(); i ++)
    {
        if(i->second == id)
            return i->first;
    }
    return NULL;
}

WObject* WSignalMapper::mapping(int id) const
{
    for(std::map<WObject *, int>::const_iterator i = intMappings_.begin(); i != intMappings_.end(); i ++)
    {
        if(i->second == id)
            return i->first;
    }
    return NULL;
}

void WSignalMapper::removeMappings(WObject *sender)
{
  {
    std::map<WObject *, int>::iterator i = intMappings_.find(sender);
    if (i != intMappings_.end())
      intMappings_.erase(i);
  }
  {
    std::map<WObject *, std::string>::iterator i
      = stringMappings_.find(sender);
    if (i != stringMappings_.end())
      stringMappings_.erase(i);
  }
  {
    std::map<WObject *, WWidget *>::iterator i = widgetMappings_.find(sender);
    if (i != widgetMappings_.end())
      widgetMappings_.erase(i);
  }
}

void WSignalMapper::setMapping(WObject *sender, int id)
{
  addSender(sender);
  intMappings_[sender] = id;
}

void WSignalMapper::addSender(WObject *sender)
{
  connect(sender, SIGNAL(destroyed(WObject *)),
	  this, SLOT(senderDestroyed(WObject *)));
}

void WSignalMapper::setMapping(WObject *sender, const std::string text)
{
  addSender(sender);
  stringMappings_[sender] = text;
}

void WSignalMapper::setMapping(WObject *sender, WWidget *widget)
{
  addSender(sender);
  widgetMappings_[sender] = widget;
}

void WSignalMapper::senderDestroyed(WObject *sender)
{
  removeMappings(sender);
}

void WSignalMapper::map()
{
  map(sender());
}

void WSignalMapper::map(WObject *sender)
{
  {
    std::map<WObject *, int>::iterator i = intMappings_.find(sender);
    if (i != intMappings_.end())
      emit mapped(i->second);
  }
  {
    std::map<WObject *, std::string>::iterator i
      = stringMappings_.find(sender);
    if (i != stringMappings_.end())
      emit mapped(i->second);
  }
  {
    std::map<WObject *, WWidget *>::iterator i = widgetMappings_.find(sender);
    if (i != widgetMappings_.end())
      emit mapped(i->second);
  }  
}
