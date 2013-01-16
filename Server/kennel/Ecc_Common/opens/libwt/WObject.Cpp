/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use. map
 */
#include <iostream>

#include "WObject"
#include "WSlot_"
#include "WSignal_"
#include "WSignalInstance_"
#include "WSlotInstance_"

#include <iostream>
//#include "WObject"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WObject::slots_[] = { WSlot_() };
WSignal_ WObject::signals_[] = { WSignal_("destroyed(WObject *)", false), WSignal_() };

void WObject::buildSlotMap(WSlotMap& result)
{
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WObject::buildSignalMap(WSignalMap& result)
{
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

void WObject::destroyed(WObject * arg0)
{
  void *args[1] = {(void *)(&arg0)};  triggerSignal(signals_ + 0, args);
}

bool WObject::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  std::cerr << "WObject internal error: slot not implemented ? " << slot->name() << std::endl;
  return false;
}
bool WObject::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  std::cerr << "WObject internal error: slot not implemented ? " << slot->name() << std::endl;
  return false;
}


int WObject::nextObjId_ = 0;

WObject::WObject()
  : id_(nextObjId_++),
    signalMap_(0),
    slotMap_(0)
{
	
}

WObject::~WObject()
{
  emit destroyed(this);

  /*
   * clean up connections by deleting signal and slot instances
   */
  if (signalMap_) {
    for (WSignalMap::const_iterator i = signalMap_->begin();
	 i != signalMap_->end(); ++i)
      delete i->second;

    delete signalMap_;
    signalMap_ = 0;
  }

  if (slotMap_) {
    for (WSlotMap::const_iterator i = slotMap_->begin();
	 i != slotMap_->end(); ++i)
      delete i->second;

    delete slotMap_;
    slotMap_ = 0;
  }
}


bool WObject::connect(const WObject *sender, const char *signal,
		      const char *method, ConnectionType type) const
{
  WSignalInstance_ *si = sender->getSignal(signal);


  if (!si) {
    std::cerr << "Object::connect could not find signal \"" << signal
	      << "\"" << std::endl;
    return false;
  }
	if((type ==JAVASCRIPT))
	{
		//si->nJavaSignal= type;
		si->SetJsVal(method,type);
		return true;	
	}else{
	  WSlotInstance_ *sl = getSlot(method);
	  if (!sl) {
		std::cerr << "Object::connect could not find slot \"" << method
			  << "\"" << std::endl;
		return false;
	  }

	 
	return si->connect(sl, type);
	}
}

bool WObject::connect(const WObject *sender, const char *signal,const char* jsfun,
		      const char *method, ConnectionType type) const
{
	WSignalInstance_ *si = sender->getSignal(signal);
	if (!si) {
    std::cerr << "Object::connect could not find signal \"" << signal
	      << "\"" << std::endl;
    return false;
  }
	//si->SetJsVal(jsfun,WObject::ConnectionType::JAVASCRIPTUPDATE);
	si->SetJsVal(jsfun,type);

	WSlotInstance_ *sl = getSlot(method);
	if (!sl) {
	std::cerr << "Object::connect could not find slot \"" << method
			<< "\"" << std::endl;
	return false;
	}
	if((type==ConnectionType::JAVASCRIPTDYNAMIC)||(JAVASCRIPTUPDATE==type))
		type =ConnectionType::DYNAMIC;

	return si->connect(sl, type);


}
bool WObject::connect(const WObject *sender, const char *signal, const char* jsfun,
		      const WObject *receiver, const char * method,
		      ConnectionType type)
{
  return receiver->connect(sender, signal, jsfun, method, type);
}
bool WObject::connect(const WObject *sender, const char *signal,
		      const WObject *receiver, const char * method,
		      ConnectionType type)
{
  return receiver->connect(sender, signal, method, type);
}

void WObject::triggerSignal(const WSignal_ *signal, void **args)
{
  // XXX: do more efficient with linear search on pointers
  WSignalInstance_ *instance = getSignal("x" + signal->name());
  instance->trigger(STATIC, args);
  instance->trigger(DYNAMIC, args);
}

bool WObject::isConnected(const WSignalInstance_ *signal) const
{
	if(signal)
		return signal->isConnected();
	else return false;
}

WSignalInstance_ *WObject::getSignal(const std::string name) const
{
  if (!signalMap_) 
  {
    signalMap_ = new WSignalMap;
    (const_cast<WObject *>(this))->buildSignalMap(*signalMap_);
  }

  WSignalMap::iterator j = signalMap_->find(name.substr(1));
 
  if (j == signalMap_->end()) 
  {
    std::cerr << "Warning could not find signal: " << name << std::endl;
    return 0;
  }
  else
    return j->second;
}

WSlotInstance_ *WObject::getSlot(const std::string name) const
{
  if (!slotMap_)
  {
    slotMap_ = new WSlotMap;
//	slotMap_ -> _Tr->_Head->_Value = NULL;
//	slotMap_ -> head = NULL;
    (const_cast<WObject *>(this))->buildSlotMap(*slotMap_);
  }

  WSlotMap::iterator i = slotMap_->find(name.substr(1));
  if (i == slotMap_->end())
    return 0;
  else
    return i->second;
}

const std::string WObject::formName() const
{
  char buf[11];
  sprintf(buf, "o%x", id_);

  return std::string(buf);
}

void WObject::setFormData(const char *data, int length,
			  const char *filename)
{
  std::cerr << "Internal error: WObject::setFormData(value) called"
	    << std::endl;
}

void WObject::setNoFormData()
{
  std::cerr << "Internal error: WObject::setNoFormData(value) called"
	    << std::endl;
}

void WObject::staticSlotChanged(const std::string name)
{
  WSlotInstance_ *slot = getSlot(name);

  if (slot)
    slot->invalidateStaticCode();
}

WSlotInstance_ *WObject::findConnectedNeedTriggerStaticSlot()
{
  if (signalMap_) 
  {
    for (WSignalMap::const_iterator i = signalMap_->begin();i != signalMap_->end(); ++i) 
	 {
      WSlotInstance_ *s = i->second->findConnectedNeedTriggerStaticSlot();
      if (s)
	    return s;
    }
  }

  return 0;
}

void WObject::signalConnectionsChanged()
{ }

void WObject::InsertMap(WSlotMap& result,std::string key,WSlotInstance_*pw)
{
	result[key]=pw;
	return;
}

/*void WObject::buildSlotMap(WSlotMap& result)
{
}
  virtual void buildSignalMap(WSignalMap& result)
  virtual bool triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
  virtual bool undoTriggerSlot(const WSlot_ *slot, void **args); 
bool WObject::undoTriggerSlot(const WSlot_ *slot, void **args)
{
	return true;
}
*/
/*! \mainpage %Wt documentation.
 *
 * %Wt is a C++ library for developing web applications.
 *
 * This manual is divided in the following sections:
 * - \ref overview
 * - \ref tutorial
 * - \ref installation
 */

/*! \page overview Library overview
 *
 * \section wwidget_sec WWidget
 *
 * The main class in %Wt is WWidget. A %WWidget provides abstraction
 * of a visual entity. A User-interface is specified by creating a
 * hierarchical structure of WWidgets, starting at the
 * WApplication::root().
 *
 * The layout of the widgets follows (with a few exceptions) this
 * hierarchical structure. %Wt does not contain layout classes, like
 * other GUI frameworks. The reason for this is that this would not
 * map very well to capabilities of HTML/CSS. Instead, we follow the
 * layout mechanisms built-in CSS: every %WWidget has some control
 * over its own layout in reference to its immediate parent or
 * siblings. An important property of a widget is whether it is \link
 * WWidget::setInline(bool) \endlink inline. Inline widgets flow with
 * sibling inline widgets in lines, wrapping at the right edge of the
 * parent container. In contrast, non-inline widgets stack vertically
 * with respect to sibling widgets.
 *
 * \section container_sec WContainerWidget
 *
 * With a few exceptions, all widgets are child of (and contained in)
 * some form of WContainerWidget. The parent of widget may be specified
 * by passing as an argument to the constructor, or by adding the widget
 * to the container using WContainerWidget::addWidget(WWidget *widget).
 *
 * \section signal_slot Signal/Slot mechanism
 *
 * To respond to user-interactivity or custom events, %Wt uses a
 * signal/slot system. The abstract class WInteractWidget and
 * WFormWidget, which are base class of many widgets, provide unified
 * signals that indicate user interactivity events. To act to one of
 * these events, the programmer must \link WObject::connect() connect
 * \endlink a signal to a slot. These may be slots that are available
 * in WWidget, such as WWidget::show(), or a programmer may provide
 * new slots on its own classes that perform some call-back code.
 *
 * In principle, every connected signal will cause the web browser to
 * communicate with the web server in order to perform the call-back code,
 * and any visual changes will be updated in the web page.
 *
 * Because the delay incurred by this browser-server communication is
 * not always desired, %Wt offers the possibility to create \link
 * WObject::STATIC STATIC \endlink connections, rather than the
 * default \link WObject::DYNAMIC DYNAMIC \endlink connections. STATIC
 * connections will result in a client-side response, in addition to
 * the server-side response (for more information see WObject). While
 * immediate, static connections are limited by the fact that their
 * response must be state-less, i.e. not dependent on the current
 * application state, or signal parameters.
 *
 * You can now proceed to the \ref tutorial
 */

/*! \page tutorial Tutorial
 *
 * In this (rough) tutorial we will step through the code of the
 * <a href="http://jose.med.kuleuven.be/wt/examples/treelist/DemoTreeList.fcg">
 * Tree List example</a>. The source code of the entire example is available
 * as leafs of the tree !
 *
 * \section stateicon_sec WStateIcon: a two-state icon
 *
 * For the implementation of the expand/collapse icons, as well as the
 * open/closed folder icon, we use a class WStateIcon. It takes a pair
 * of icons and shows only one at a time. Passing clickIsSwitch = true to
 * the constructor will make the icon react to click events to switch icon.
 *
 * This is the class definition from TwoStateIcon.h :
 *
 * \code

class WStateIcon : public WContainerWidget
{
  //MOC: W_OBJECT WStateIcon:WContainerWidget
  W_OBJECT;

public:
  WStateIcon(const std::string icon1URI, const std::string icon2URI,
	     bool clickIsSwitch = true, WContainerWidget *parent = 0);
 
  WImage *icon1() const { return icon1_; }
  WImage *icon2() const { return icon2_; }

  void setIcon(int num);

public static_slots:
  //MOC: STATIC SLOT WStateIcon::showIcon1()
  void showIcon1();
  //MOC: STATIC SLOT WStateIcon::showIcon2()
  void showIcon2();

private:
  WImage *icon1_;
  WImage *icon2_;

  int previousState_;
  void undoShowIcon1();
  void undoShowIcon2();
};
 * \endcode
 *
 * The WStateIcon will be a WContainerWidget which contains two WImage objects.
 * The class defines two (static) slots: showIcon1() and showIcon2(),
 * which show the respective icon. By declaring them as
 * \link WObject static slots \endlink, we make sure their
 * behaviour can be optimised into client-side code. They are indeed
 * stateless, since the behaviour of showIcon1() is independent of the
 * current state. Because we declare as static slots, we also need to
 * provide two undo functions: undoShowIcon1() and undoShowIcon2().
 *
 * The constructor implementation is as follows:
 *
 * \code
WStateIcon::WStateIcon(const std::string icon1URI, const std::string icon2URI,
		       bool clickIsSwitch, WContainerWidget *parent)
  : WContainerWidget(parent)
{
  setInline(true);

  icon1_ = new WImage(icon1URI, this);
  icon2_ = new WImage(icon2URI, this);

  icon2_->hide();

  if (clickIsSwitch) {
    connect(icon1_, SIGNAL(clicked()),
	    icon1_, SLOT(hide()), STATIC);
    connect(icon1_, SIGNAL(clicked()),
	    icon2_, SLOT(show()), STATIC);

    connect(icon2_, SIGNAL(clicked()),
	    icon2_, SLOT(hide()), STATIC);
    connect(icon2_, SIGNAL(clicked()),
	    icon1_, SLOT(show()), STATIC);

    decorationStyle().setCursor(WCssDecorationStyle::Pointer);
  }
}
 * \endcode
 *
 * Note the static connections from the \link WInteractWidget::clicked()
 * clicked() \endlink signals to the \link WWidget::hide() \endlink and
 * \link WWidget::show() show() \endlink slots. Clicking each of the icons
 * makes that icon hidden, and the other icon shown. We also change the
 * cursor to a pointer to hint that clicking these icons will in fact
 * perform an action.
 *
 * Finally, this is the implementation of the other methods, which can
 * be used to change the WStateIcon state programmatically.
 *
 * \code
void WStateIcon::setIcon(int num)
{
  if (num == 0) {
    icon1_->show();
    icon2_->hide();
  } else {
    icon1_->hide();
    icon2_->show();
  }
}

void WStateIcon::showIcon1()
{
  previousState_ = (icon1_->isHidden() ? 1 : 0);
  setIcon(0);
}

void WStateIcon::showIcon2()
{
  previousState_ = (icon1_->isHidden() ? 1 : 0);
  setIcon(1);
}

void WStateIcon::undoShowIcon1()
{
  setIcon(previousState_);
}

void WStateIcon::undoShowIcon2()
{
  setIcon(previousState_);
}
 * \endcode
 *
 * Note the implementations of undoShowIcon1() and undoShowIcon2(): they
 * reset the state to what it was before the respective showIcon1() and
 * showIcon2() calls. This is all that is necessary to implement a static
 * slot.
 *
 * \section treenode_sec WTreeNode: an expandable tree node.
 *
 * WTreeNode contains the implementation of the tree, as a hierarchy of
 * tree nodes. The layout of a single node is done using a 2x2 WTable:
 *
 * \verbatim
   |-----------------------|
   | +/- | label           |
   |------------------------
   |     | child1          |
   |     | child2          |
   |     | child3          |
   |     |       ...       |
   |-----------------------| \endverbatim
 *
 * The WTreeNode manages a list of child nodes in a WContainerWidget which
 * will be hidden and shown when the node is expanded or collapsed.
 * Furthermore, we make sure that children are collapsed when the node is
 * expanded.
 *
 * This is the WTreeNode class definition:
 *
 *  \code
class WTreeNode : public WTable
{
  //MOC: W_OBJECT WTreeNode:WTable
  W_OBJECT;

public:
  WTreeNode(const std::string labelText,
	    WStateIcon *labelIcon, WWidget *userContent,
	    WContainerWidget *parent = 0);

  void addChildNode(WTreeNode *node);
  void removeChildNode(WTreeNode *node);

  const std::vector<WTreeNode *>& childNodes() const { return childNodes_; }

public static_slots:
  //MOC: STATIC SLOT WTreeNode::collapse()
  void collapse();

  //MOC: STATIC SLOT WTreeNode::expand()
  void expand();

private:
  std::vector<WTreeNode *> childNodes_;
  WTreeNode		   *parentNode_;

  WStateIcon               *expandIcon_;
  WImage		   *noExpandIcon_;

  WStateIcon		   *labelIcon_;
  WText			   *labelText_;
  WText			   *childCountLabel_;

  WWidget                  *userContent_;
  WContainerWidget         *expandedContent_;

  void adjustExpandIcon();
  bool isLastChildNode() const;
  void childNodesChanged();

  bool wasCollapsed_;
  void undoCollapse();
  void undoExpand();

  enum ImageIndex { Middle = 0, Last = 1 };
  static std::string imageLine_[];
  static std::string imagePlus_[];
  static std::string imageMin_[];
};
 * \endcode
 *
 * We start with the implementation of the constructor:
 *
 * \code
WTreeNode::WTreeNode(const std::string labelText,
		     WStateIcon *labelIcon,
		     WWidget *userContent, WContainerWidget *parent)
  : WTable(parent),
    parentNode_(0),
    labelIcon_(labelIcon),
    userContent_(userContent)
{
  expandIcon_ = new WStateIcon(imagePlus_[Last], imageMin_[Last]);
  expandIcon_->hide();
  noExpandIcon_ = new WImage(imageLine_[Last]);

  expandedContent_ = new WContainerWidget();
  expandedContent_->hide();
  if (userContent)
    expandedContent_->addWidget(userContent);

  labelText_ = new WText(labelText);
  childCountLabel_ = new WText();
  childCountLabel_->decorationStyle().setForegroundColor(Wt::blue);

  elementAt(0, 0)->addWidget(expandIcon_);
  elementAt(0, 0)->addWidget(noExpandIcon_);

  if (labelIcon_) {
    elementAt(0, 1)->addWidget(labelIcon_);
    labelIcon_->setVerticalAlignment(AlignMiddle);
  }
  elementAt(0, 1)->addWidget(labelText_);
  elementAt(0, 1)->addWidget(childCountLabel_);

  elementAt(1, 1)->addWidget(expandedContent_);

  elementAt(0, 0)->setContentAlignment(AlignTop);
  elementAt(0, 1)->setContentAlignment(AlignMiddle);

  connect(expandIcon_->icon1(), SIGNAL(clicked()),
	  this, SLOT(expand()), STATIC);
  connect(expandIcon_->icon2(), SIGNAL(clicked()),
	  this, SLOT(collapse()), STATIC);
}
 * \endcode
 *
 * WTable::elementAt(int row, int column) is used repeatedly to add or modify
 * contents of the cells. Finally, we make connections from the expand
 * and collapse icons to static slots we defined in the WTreeNode class.
 *
 * The implementation of the expand and collapse slots is given by :
 *
 * \code
void WTreeNode::collapse()
{
  wasCollapsed_ = expandedContent_->isHidden();

  expandIcon_->setIcon(0);
  expandedContent_->hide();
  if (labelIcon_)
    labelIcon_->setIcon(0);
}

void WTreeNode::expand()
{
  wasCollapsed_ = expandedContent_->isHidden();

  expandIcon_->setIcon(1);
  expandedContent_->show();
  if (labelIcon_)
    labelIcon_->setIcon(1);

  // collapse all children
  for (unsigned i = 0; i < childNodes_.size(); ++i)
    childNodes_[i]->collapse();
}
  * \endcode
  *
  * Since they were declared as static slots, we also need to define
  * the undo functions. Because expand() also collapses all child nodes,
  * the undo function of expand() is not simply collapse() and vice-versa.
  *
  * \code
void WTreeNode::undoCollapse()
{
  if (!wasCollapsed_) {
    // re-expand
    expandIcon_->setIcon(1);
    expandedContent_->show();
    if (labelIcon_)
      labelIcon_->setIcon(1);    
  }
}

void WTreeNode::undoExpand()
{
  if (wasCollapsed_) {
    // re-collapse
    expandIcon_->setIcon(0);
    expandedContent_->hide();
    if (labelIcon_)
      labelIcon_->setIcon(0);
  }

  // undo collapse of children
  for (unsigned i = 0; i < childNodes_.size(); ++i)
    childNodes_[i]->undoCollapse();  
}
 * \endcode
 *
 * Finally, the following code implements the functionality to
 * add or remove children:
 *
 * \code
void WTreeNode::addChildNode(WTreeNode *node)
{
  childNodes_.push_back(node);
  node->parentNode_ = this;

  expandedContent_->addWidget(node);

  childNodesChanged();
}

void WTreeNode::removeChildNode(WTreeNode *node)
{
  childNodes_.erase(find(childNodes_.begin(), childNodes_.end(), node));

  node->parentNode_ = 0;

  expandedContent_->removeWidget(node);

  childNodesChanged();
}

void WTreeNode::childNodesChanged()
{
  for (unsigned i = 0; i < childNodes_.size(); ++i)
    childNodes_[i]->adjustExpandIcon();

  childCountLabel_
    ->setText("(" + boost::lexical_cast<std::string>(childNodes_.size())
	      + ")");

  adjustExpandIcon();

  staticSlotChanged(SLOT(expand()));
  staticSlotChanged(SLOT(collapse()));
}
 * \endcode
 *
 *
 */

/*! \page installation Getting started
 *
 * \section Installation
 *
 * See the INSTALL file.
 *
 */
