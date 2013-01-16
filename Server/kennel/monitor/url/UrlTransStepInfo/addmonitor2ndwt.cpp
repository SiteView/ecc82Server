#include <iostream>
#include "addmonitor2nd.h"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ CEccAddMonitor2nd::slots_[] = { WSlot_("Forward()", false), WSlot_("SaveMonitor()", false), WSlot_("Cancel()", false), WSlot_("BatchAdd()", false), WSlot_("EnumDynData()", false), WSlot_("SetAlertCondition()", false), WSlot_("ContinueAdd()", false), WSlot_("UrlStepSetup()", false), WSlot_() };
WSignal_ CEccAddMonitor2nd::signals_[] = { WSignal_() };

void CEccAddMonitor2nd::buildSlotMap(WSlotMap& result)
{
  CEccBaseTable::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)  {
#ifdef WIN32 
	WSlotInstance_ *pw=new WSlotInstance_(this, slots_ + i);	
		std::string strkey=slots_[i].name();	
		WObject::InsertMap(result,strkey,pw);	
#else  
	result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
#endif  
	}
}
void CEccAddMonitor2nd::buildSignalMap(WSignalMap& result)
{
  CEccBaseTable::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool CEccAddMonitor2nd::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  if (slot == slots_ + 0) {
    sender_ = sender;    Forward();
    return true;
  }
  if (slot == slots_ + 1) {
    sender_ = sender;    SaveMonitor();
    return true;
  }
  if (slot == slots_ + 2) {
    sender_ = sender;    Cancel();
    return true;
  }
  if (slot == slots_ + 3) {
    sender_ = sender;    BatchAdd();
    return true;
  }
  if (slot == slots_ + 4) {
    sender_ = sender;    EnumDynData();
    return true;
  }
  if (slot == slots_ + 5) {
    sender_ = sender;    SetAlertCondition();
    return true;
  }
  if (slot == slots_ + 6) {
    sender_ = sender;    ContinueAdd();
    return true;
  }
  if (slot == slots_ + 7) {
    sender_ = sender;    UrlStepSetup();
    return true;
  }
  return CEccBaseTable::triggerSlot(sender, slot, args);
}
bool CEccAddMonitor2nd::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return CEccBaseTable::undoTriggerSlot(slot, args);
}
