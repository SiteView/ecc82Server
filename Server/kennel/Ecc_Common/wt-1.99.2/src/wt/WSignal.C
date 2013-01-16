#include <boost/lexical_cast.hpp>

#include "WSignal"
#include "WApplication"
#include "WStatelessSlot"
#include "WJavascriptSlot"
#include "WebSession.h"
#include "WtException.h"

namespace Wt {

SignalBase::SignalBase(WObject *sender)
  : sender_(sender)
{ }

SignalBase::~SignalBase()
{ }

WObject *SignalBase::sender() const
{
  return sender_;
}

void SignalBase::pushSender(WObject *sender)
{
  WebSession *sess = WebSession::instance();

  sess->pushEmitStack(sender);
}

void SignalBase::popSender()
{
  WebSession *sess = WebSession::instance();

  sess->popEmitStack();
}

WObject *SignalBase::currentSender()
{
  WebSession *sess = WebSession::instance();

  return sess->emitStackTop();
}

Signal<void>::Signal(WObject *sender)
  : Signal<>(sender)
{ }

EventSignalBase
::StatelessConnection::StatelessConnection(const boost::signals::connection& c,
                                           WObject *t,
                                           WStatelessSlot *s)
  : connection(c),
    target(t),
    slot(s)
{ }

bool EventSignalBase::StatelessConnection::ok() const
{
  return target == 0 || connection.connected();
}

EventSignalBase::Impl::Impl()
  : needUpdate_(false),
    id_(-1)
{ }

int EventSignalBase::Impl::nextId_ = 0;

EventSignalBase::EventSignalBase(WObject *sender)
  : SignalBase(sender),
    impl_(0)
{ }

bool EventSignalBase::needUpdate() const
{
  return impl_ && impl_->needUpdate_;
}

void EventSignalBase::updateOk()
{
  if (impl_)
    impl_->needUpdate_ = false;
}

void EventSignalBase::removeSlot(WStatelessSlot *s)
{
  if (impl_) {
    for (unsigned i = 0; i < impl_->connections_.size(); ++i) {
      if (impl_->connections_[i].slot == s) {
	impl_->connections_.erase(impl_->connections_.begin() + i);
	senderRepaint();
	return;
      }
    }
  }

  assert(false);
}

const std::string EventSignalBase::encodeCmd() const
{
  if (!impl_)
    impl_ = new Impl();

  if (impl_->id_ == -1)
    impl_->id_ = Impl::nextId_++;

  return "s" + boost::lexical_cast<std::string>(impl_->id_);
}

const std::string EventSignalBase::javaScript() const
{
  std::string result = "";

  if (impl_) {
    for (unsigned i = 0; i < impl_->connections_.size(); ++i) {
      if (impl_->connections_[i].ok()) {
	if (impl_->connections_[i].slot->learned())
	  result += impl_->connections_[i].slot->javaScript();
      }
    }
  }

  return result;
}

bool EventSignalBase::isExposedSignal() const
{
  WApplication *app = WApplication::instance();

  return app->isExposedSignal(this);
}

EventSignalBase::~EventSignalBase()
{
  WApplication *app = WApplication::instance();

  if (app->isExposedSignal(this))
    app->removeExposedSignal(this);

  if (impl_) {
    for (unsigned i = 0; i < impl_->connections_.size(); ++i) {
      if (impl_->connections_[i].ok()) {
	impl_->connections_[i].slot->removeConnection(this);
      }
    }

    delete impl_;
  }
}

boost::signals::connection EventSignalBase::connect(WObjectMethod method,
                                                    WObject *target,
                                                    WStatelessSlot *slot)
{
  if (!impl_)
    impl_ = new Impl();

  boost::signals::connection c = impl_->dummy_.connect(boost::bind(method, target));

  slot->addConnection(this);

  impl_->connections_.push_back(StatelessConnection(c, target, slot));

  senderRepaint();

  return c;
}

void EventSignalBase::connect(WJavascriptSlot& slot)
{
  WStatelessSlot *s = slot.slotimp();
  s->addConnection(this);

  boost::signals::connection c;

  if (!impl_)
    impl_ = new Impl();

  impl_->connections_.push_back(StatelessConnection(c, 0, s));
}

bool EventSignalBase::isConnected() const
{
  if (!impl_)
    return false;

  bool result = impl_->dummy_.num_slots() > 0;

  if (!result) {
    for (unsigned i = 0; i < impl_->connections_.size(); ++i) {
      if (impl_->connections_[i].target == 0)
	return true;
    }
  }

  return result;
}

void EventSignalBase::exposeSignal()
{
  WApplication *app = WApplication::instance();

  if (!app->isExposedSignal(this))
    app->addExposedSignal(this);
}

void EventSignalBase::senderRepaint()
{
  assert(impl_);

  impl_->needUpdate_ = true;
  sender()->signalConnectionsChanged();
}

void EventSignalBase::processNonLearnedStateless()
{
  if (impl_) {
    std::vector<StatelessConnection> copy = impl_->connections_;

    for (unsigned i = 0; i < copy.size(); ++i) {
      StatelessConnection& c = copy[i];

      if (c.ok() && !c.slot->learned())
	c.slot->trigger();
    }
  }
}

void EventSignalBase::processLearnedStateless()
{
  if (impl_) {
    std::vector<StatelessConnection> copy = impl_->connections_;

    for (unsigned i = 0; i < copy.size(); ++i) {
      StatelessConnection& c = copy[i];

      if (c.ok() && c.slot->learned())
	c.slot->trigger();
    }
  }
}

void EventSignalBase::processPreLearnStateless(SlotLearnerInterface *learner)
{
  if (impl_) {
    bool changed = false;

    std::vector<StatelessConnection> copy = impl_->connections_;

    for (unsigned i = 0; i < copy.size(); ++i) {
      StatelessConnection& c = copy[i];

      if (c.ok()
	  && !c.slot->learned()
	  && c.slot->type() == WStatelessSlot::PreLearnStateless) {
	learner->learn(c.slot);
	changed = true;
      }
    }

    if (changed)
      senderRepaint();
  }
}

void EventSignalBase::processAutoLearnStateless(SlotLearnerInterface *learner)
{
  if (impl_) {
    bool changed = false;

    std::vector<StatelessConnection> copy = impl_->connections_;

    for (unsigned i = 0; i < copy.size(); ++i) {
      StatelessConnection& c = copy[i];

      if (c.ok()
	  && !c.slot->learned()
	  && c.slot->type() == WStatelessSlot::AutoLearnStateless) {
	learner->learn(c.slot);
	changed = true;
      }
    }

    if (changed)
      senderRepaint();
  }
}

EventSignal<void>::EventSignal(WObject *sender)
  : EventSignalBase(sender),
    dynamic_(0),
    relay_(0)
{ }

EventSignal<void>::EventSignal(EventSignalBase *relay)
  : EventSignalBase(0),
    dynamic_(0),
    relay_(relay)
{ }

EventSignal<void>::~EventSignal()
{
  if (dynamic_)
    delete dynamic_;
}

bool EventSignal<void>::isConnected() const
{
  if (relay_)
    return relay_->isConnected();
  else
    return EventSignalBase::isConnected()
      || (dynamic_ && dynamic_->num_slots() > 0);
}

void EventSignal<void>::connect(WJavascriptSlot& slot)
{
  if (relay_)
    relay_->connect(slot);
  else
    EventSignalBase::connect(slot);
}

boost::signals::connection EventSignal<void>
::connectBase(WObject *target, void (WObject::*method)())
{
  if (relay_)
    return relay_->connectBase(target, method);
  else
    return connect(target, method);
}

void EventSignal<void>::emit()
{
  if (relay_)
    throw WtException("Cannot emit a relayed signal");

  if (dynamic_) {
    pushSender(sender());
    (*dynamic_)();
    popSender();
  }
}

void EventSignal<void>::operator()()
{
  emit();
}

void EventSignal<void>::processDynamic(const JavascriptEvent& jse)
{
  if (relay_)
    throw WtException("Internal error: processDynamic on a relayed signal?");
  
  emit();
  processNonLearnedStateless();
}

}
