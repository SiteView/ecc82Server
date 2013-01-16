#include <iostream>
#include "UrlTransStepInfo.h"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ CUrlTransStepInfo::slots_[] = { WSlot_("ShowHelp()", false), WSlot_("AddStep()", false), WSlot_("SaveStep()", false), WSlot_("CancelStep()", false), WSlot_("EditStep(const std::string)", false), WSlot_("SelAll()", false), WSlot_("SelNone()", false), WSlot_("SelInvert()", false), WSlot_("BeforeDelStep()", false), WSlot_("DelStep()", false), WSlot_("TestMonitor()", false), WSlot_("SelFormsChanged()", false), WSlot_("ShowHtml()", false), WSlot_() };
WSignal_ CUrlTransStepInfo::signals_[] = { WSignal_() };

void CUrlTransStepInfo::buildSlotMap(WSlotMap& result)
{
  WContainerWidget::buildSlotMap(result);
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
void CUrlTransStepInfo::buildSignalMap(WSignalMap& result)
{
  WContainerWidget::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool CUrlTransStepInfo::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  if (slot == slots_ + 0) {
    sender_ = sender;    ShowHelp();
    return true;
  }
  if (slot == slots_ + 1) {
    sender_ = sender;    AddStep();
    return true;
  }
  if (slot == slots_ + 2) {
    sender_ = sender;    SaveStep();
    return true;
  }
  if (slot == slots_ + 3) {
    sender_ = sender;    CancelStep();
    return true;
  }
  if (slot == slots_ + 4) {
    sender_ = sender;    EditStep(*((const std::string *)args[0]));
    return true;
  }
  if (slot == slots_ + 5) {
    sender_ = sender;    SelAll();
    return true;
  }
  if (slot == slots_ + 6) {
    sender_ = sender;    SelNone();
    return true;
  }
  if (slot == slots_ + 7) {
    sender_ = sender;    SelInvert();
    return true;
  }
  if (slot == slots_ + 8) {
    sender_ = sender;    BeforeDelStep();
    return true;
  }
  if (slot == slots_ + 9) {
    sender_ = sender;    DelStep();
    return true;
  }
  if (slot == slots_ + 10) {
    sender_ = sender;    TestMonitor();
    return true;
  }
  if (slot == slots_ + 11) {
    sender_ = sender;    SelFormsChanged();
    return true;
  }
  if (slot == slots_ + 12) {
    sender_ = sender;    ShowHtml();
    return true;
  }
  return WContainerWidget::triggerSlot(sender, slot, args);
}
bool CUrlTransStepInfo::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WContainerWidget::undoTriggerSlot(slot, args);
}
