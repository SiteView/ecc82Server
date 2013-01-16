#include "WObject"
#include "WStatelessSlot"
#include "WSignal"

#include <string>

namespace Wt {

WStatelessSlot::WStatelessSlot(WObject* obj, WObjectMethod method)
  : target_(obj),
    method_(method),
    undoMethod_(0),
    learned_(false)
{ }

WStatelessSlot::WStatelessSlot(WObject* obj, WObjectMethod method, 
			       WObjectMethod undomethod)
  : target_(obj),
    method_(method),
    undoMethod_(undomethod),
    learned_(false)
{ }

WStatelessSlot::WStatelessSlot(WObject* obj, const std::string& javaScript)
  : target_(obj),
    method_(0),
    undoMethod_(0),
    learned_(true),
    jscript_(javaScript)
{ }

WStatelessSlot::~WStatelessSlot()
{
  for (unsigned i = 0; i < connectingSignals_.size(); ++i)
    connectingSignals_[i]->removeSlot(this);
}

bool WStatelessSlot::implementsMethod(WObjectMethod method) const
{
  return method_ == method;
}

WStatelessSlot::SlotType WStatelessSlot::type() const
{
  if (method_ == 0)
    return JavaScriptSpecified;
  else
    if (undoMethod_ == 0)
      return AutoLearnStateless;
    else
      return PreLearnStateless;
} 
 
bool WStatelessSlot::learned() const
{
  return learned_;
}

void WStatelessSlot::setJavaScript(const std::string javaScript)
{
  jscript_ = javaScript;
  learned_ = true;

  if (method_ == 0) {
    for(size_t i = 0; i < connectingSignals_.size(); i++)
      connectingSignals_[i]->senderRepaint();
  }
}

void WStatelessSlot::setNotLearned()
{
  if (learned_) {
    jscript_.clear();
    learned_ = false;

    for(size_t i = 0; i < connectingSignals_.size(); i++)
      connectingSignals_[i]->senderRepaint();    
  }
}

std::string WStatelessSlot::javaScript() const
{
  return jscript_;
}

void WStatelessSlot::trigger()
{
  (target_->*(method_))();
}

void WStatelessSlot::undoTrigger()
{
  if (undoMethod_)
    (target_->*(undoMethod_))();
}

void WStatelessSlot::addConnection(EventSignalBase* s)
{
  std::vector<EventSignalBase *>::iterator f
    = std::find(connectingSignals_.begin(), connectingSignals_.end(), s);

  if (f == connectingSignals_.end())
    connectingSignals_.push_back(s);
}	

void WStatelessSlot::removeConnection(EventSignalBase* s)	
{
  std::vector<EventSignalBase *>::iterator f
    = std::find(connectingSignals_.begin(), connectingSignals_.end(), s);

  if(f != connectingSignals_.end())
    connectingSignals_.erase(f);
}

}
