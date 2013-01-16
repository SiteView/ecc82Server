/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <iostream>

#include "WObject"
#include "WStatelessSlot"
#include "WSignal"

namespace Wt {

int WObject::nextObjId_ = 0;

WObject::WObject(WObject* parent)
  : statelessSlots_(0),
    id_(nextObjId_++),
    children_(0),
    parent_(parent)
{
  if (parent) {
    if (!parent->children_)
      parent->children_ = new std::vector<WObject *>;
    parent->children_->push_back(this);
  }
}

WObject::~WObject()
{
  for (unsigned i = 0; i < statelessSlots_.size(); ++i)
    delete statelessSlots_[i];

  if (parent_) {
    std::vector<WObject *>::iterator i = std::find(parent_->children_->begin(),
						   parent_->children_->end(),
						   this);

    assert(i != parent_->children_->end());
    parent_->children_->erase(i);
  }

  if (children_) {
    for (unsigned i = 0; i < children_->size(); ++i) {
      (*children_)[i]->parent_ = 0;
      delete (*children_)[i];
    }
    delete children_;
  }
}

const std::string WObject::formName() const
{
  char buf[11];
  sprintf(buf, "o%x", id_);

  return std::string(buf);
}

void WObject::setFormData(CgiEntry *entry)
{
  std::cerr << "Internal error: WObject::setFormData(value) called"
	    << std::endl;
}

void WObject::formDataSet()
{ }

void WObject::setNoFormData()
{
  std::cerr << "Internal error: WObject::setNoFormData(value) called"
	    << std::endl;
}

void WObject::requestTooLarge(int size)
{ }

void WObject::signalConnectionsChanged()
{ 
  std::cerr << "WObject::signalConnectionsChanged()" << std::endl;
}

void WObject::resetLearnedSlots()
{
  for (unsigned i = 0; i < statelessSlots_.size(); i++)
    statelessSlots_[i]->setNotLearned();
}

void WObject::resetLearnedSlot(void (WObject::*method)())
{
  for (unsigned i = 0; i < statelessSlots_.size(); i++) {
    if (statelessSlots_[i]->implementsMethod(method)) {
      statelessSlots_[i]->setNotLearned();
      return;
    }
  }
  std::cerr << "Error: WObject::resetLearnedSlot called for a slot that has"
    " no stateless implementation." << std::endl;
  assert(false);
}

WStatelessSlot* WObject::isStateless(void (WObject::*method)())
{
  for (unsigned i = 0; i < statelessSlots_.size(); i++) {
    if (statelessSlots_[i]->implementsMethod(method))
      return statelessSlots_[i];
  }

  return 0;
}

void WObject::implementAutolearn(void (WObject::*method)())
{
  statelessSlots_.push_back(new WStatelessSlot(this, method));
}

void WObject::implementPrelearn(void (WObject::*method)(),
				void (WObject::*undoMethod)())
{        
  statelessSlots_.push_back(new WStatelessSlot(this, method, undoMethod));
}

WObject *WObject::sender()
{
  return SignalBase::currentSender();
}

/*! \mainpage %Wt documentation.
 *
 * %Wt is a C++ library for developing web applications.
 *
 * This is a reference manual which has been generated from
 * the source code. For learning web application programming with %Wt,
 * we recommend reading the <a href="../../tutorial/wt-sdj.xhtml">
 * introduction to %Wt</a> which includes a complete tutorial.
 *
 * In addition, there is also:
 * - \ref overview - Brief overview of basic %Wt concepts.
 * - \ref example - A line-by-line explanation.
 * - \ref Installation - Get started today!
 * - \ref Releasenotes - Release notes.
 */

/*! \page overview Library overview
 *
 * \section wwidget_sec 1. Wt::WWidget
 *
 * One of the key classes in %Wt is Wt::WWidget. A %WWidget provides
 * abstraction of a visual entity. The entire user-interface is
 * specified by creating a hierarchical structure of WWidgets,
 * starting at the Wt::WApplication::root(), and letting these widgets
 * respond to events.
 *
 * When a widget is inserted in the tree, ownership is transferred to the
 * tree. Whenever a widget is deleted, all its children are deleted
 * as well. When the WApplication object is deleted, the root of the
 * tree is deleted, and in this way all resources associated with any
 * widget are freed.
 *
 * Any descendent class of WWidget is a self-contained class that
 * effectively encapsulates all of its internal complexity.
 *
 * \subsection layout 1.1 Layout.
 *
 * The layout of the widgets follows (with a few exceptions) this
 * hierarchical structure. Unlike many other GUI widget frameworks,
 * %Wt does not contain layout classes. While useful, they do not map
 * very well to the capabilities of HTML/CSS. Instead, %Wt exposes the
 * layout mechanisms built-in HTML/CSS: every %WWidget has some
 * control over its own layout in reference to its immediate parent or
 * siblings.
 *
 * CSS layout considers two important categories of layout. Text-like
 * layout (\link Wt::WWidget::setInline(bool) inline\endlink) flow
 * with sibling \link Wt::WWidget::setInline(bool) inline \endlink
 * widgets in lines, wrapping at the right edge of the parent
 * container. In contrast, \link Wt::WWidget::setInline(bool) stacked
 * \endlink widgets stack vertically with respect to sibling widgets.
 *
 * \subsection style 1.2 Style.
 *
 * For visual markup of widgets, the recommended way is to use CSS style
 * sheets. These allow the visual look to be defined seperately from the
 * the rest of the application. The location of the stylesheet may be
 * configured using Wt::WApplication::useStyleSheet().
 *
 * CSS Style selectors may be used in conjunction with widget style
 * classes that may be set for any widget using
 * Wt::WWidget::setStyleClass(). The recommended way for visual
 * response to events is by changing the style class for the widget.
 *
 * In addition to configuration using style sheets, %Wt also supports
 * inline specification of style, using
 * Wt::WWidget::decorationStyle().
 *
 * \section application_sec 2. Wt::WApplication
 *
 * For every new session (which corresponds to a new user surfing to
 * your web application), a new Wt::WApplication is created. The
 * WApplication may be used to inspect startup arguments and settings
 * (using Wt::WApplication::environment()), to set or change the
 * application title (Wt::WApplication::setTitle()), to specify a
 * character encoding (Wt::WApplication::setCharacterEncoding()) and
 * locale (Wt::WApplication::setLocale()) for rendering, and other
 * application-wide settings.
 *
 * An application is exited when the user browses away from the
 * application, or when WApplication::quit() is called. In any case,
 * the entire widget tree is properly deleted, where all resources
 * held by the application may be released.
 *
 * During the lifetime of a session, the session's WApplication
 * instance is accessible using Wt::WApplication::instance().
 *
 * As of version 2.0.0, the library offers two different mechanisms to
 * map sessions onto processes: <b>dedicated processes</b> and
 * <b>shared processes</b>. The first mechanisms forks a dedicated
 * process for every distinct session. This provides the kernel-level
 * isolation of different sessions, which may be useful for highly
 * security sensitive applications. The second mechanism spawns a
 * number of processes and allocates new sessions randomly to one of
 * these processes. This reduces the danger for DoS attacks, but
 * requires more careful programming as memory corruption affects all
 * sessions in a single process, and sessions are not isolated by any
 * other mechanism but correct programming.
 *
 * \section container_sec 3. Widget containers
 *
 * With a few exceptions, all widgets are child of (and contained in)
 * a container widget such as Wt::WContainerWidget or
 * Wt::WTableCell. A widget is inserted into a WContainerWidget by
 * adding the widget to the container using
 * Wt::WContainerWidget::addWidget(WWidget *widget), or by passing the
 * parent container as an argument to the constructor.
 *
 * \section signal_slot 4. Signal/Slot mechanism
 *
 * To respond to user-interactivity events, or in general to communicate
 * events from one widget to any other, %Wt uses a signal/slot system.
 *
 * A slot is any method of any descendant of Wt::WObject. To connect a
 * signal with a slot, the only requirement is that the method
 * signature of the slot must be compatible with the signal
 * definition. While in this way every method may be used as a slot,
 * we encourage to explicitly indicate a particular method to be a
 * slot, in the same way as is done in Qt, by putting them in a
 * special section (which improves for example documentation generated
 * using doxygen):
 *
 * \code

class MyWidgetry : public Wt::WCompositeWidget
{
// ...

public slots:
  void doThis();

private slots:
  void doThat(int count);

  // ...
};

 * \endcode
 *
 * A signal may be created for an object by instantiating a Wt::Signal<..>
 * class.
 *
 * While the library defines signals and slots on the various widgets,
 * it is easy and convenient to add signals and slots to widget classes
 * to communicate events and trigger callbacks.
 *
 * Event signals are special signals that may be triggered internally
 * by the library to respond to user interactivity events. Together, the
 * abstract classes Wt::WInteractWidget and Wt::WFormWidget, which are
 * baseclasses of many widgets, provide most of these event signals. To react
 * to one of these events, the programmer simply connects a self-defined
 * or already existing slot to the signal.
 *
 * \section eventhandling 5. Server side versus client side event handling.
 *
 * By default, %Wt performs all event processing server-side. Every connected
 * event signal will cause the web browser to communicate with the web server
 * in order to perform the call-back code, and any visual changes will be
 * updated in the web page.
 *
 * However, %Wt may also be told to perform some of the visual processing
 * on the client-side. This may be used for a highly interactive user-
 * interface, because the typical communication delay is avoided. The only
 * way for portable client-side processing is JavaScript. For various
 * reasons, %Wt does not expose JavaScript, since it is an error-prone,
 * ill-defined interpreted language that poses various security problems
 * such as cross-site scripting.
 *
 * Instead, %Wt may be told to cache the result of a visual update,
 * as a result of slot call-back code, on the client-side for subsequent
 * calls, or to pre-learn the result of a visual update in advance and
 * store it (as JavaScript) on the client-side. The restriction is that
 * this is only possible for stateless call-back code -- i.e. when the
 * visual update does not depend on state that may change in the course
 * of the application.
 * See the documentation of Wt::WObject::implementStateless for details,
 * or the \ref example for the use of stateless implementations to create
 * a treelist widget that does all node expansion / collapsing client-side,
 * at least if JavaScript is available.
 *
 * The stateless slot learning allows applications to be developed entirely
 * in C++, with only one specification of the desired behaviour, and decide
 * at run-time to optimize certain event handling in client-side JavaScript
 * if possible, and fall-back to server-side event handling otherwise.
 
  \section debugging 5. Debugging

  \subsection gdb 5.1 gdb (Gnu debugger)

  When using the FastCGI connector, the %Wt application is spawned by the web
  server, and one may attach gdb to a running application to perform
  interactive debugging.

  To connect to a running instance, use the following command:
  \verbatim $ gdb /path/to/your/wt/application <pid> \endverbatim
  where \<pid\> is the process of the application, as reported by the
  web server. In this way you may debug your application after the
  event loop has started (i.e. just before the first event is handled).

  If the application fails before entering the event loop, for example
  during construction of the first widget tree, you can add a
  \verbatim sleep(30); \endverbatim
  statement to your wmain() function, to be able to break right at the
  start of your program.

  \subsection valgrind 5.2 Valgrind

  We heartly recommend using <a href="http://valgrind.org">Valgrind</a> to
  debug %Wt applications for memory-related problems or problems caused
  by reading from unitisialised values. %Wt itself has been debugged
  extensively using this tool. To enable debugging in a %Wt
  application, enable both valgrind and debug support in the %Wt configuration
  file and append a ?debug at the end of your webapp's url.
  For example, myapp.fcg?debug. The stack trace and other debugging logs
  will be available in your web server's error_log file. Note that debugging
  a %Wt app will slow it down dramatically, and therefore should be disabled
  entirely in production installations.

  \section configuration_sec 6. Configuration

  %Wt uses an XML configuration file (which by default is located in
  /etc/wt/wt_config.xml) in which several aspects of application deployment
  may be configured, on a site-wide or application-specific basis.

  <dl>
    <dt><strong>dedicated-process</strong></dt>
    <dd>Every session is mapped a dedicated process, allowing maximal
      session isolation, but at an increased session cost.</dd>
    <dt><strong>shared-process</strong></dt>
    <dd>Sessions share a fixed number of processes, yielding a lower
      session cost.</dd>
    <dt><strong>tracking</strong></dt>
    <dd>How session tracking is implemented: automatically (using cookies
      when available, otherwise using URL rewriting) or strictly using URL
      rewriting (which is allows multiple concurrent sessions from one user)
      </dd>
    <dt><strong>reload-is-new-session</strong></dt>
    <dd>Should a brower reload spawn a new session (convenient for debuggin)
      or simply refresh (using Wt::WApplication::refresh()) the current
      session ?</dd>
    <dt><strong>timeout</strong></dt>
    <dd>The timeout (in seconds) for detecting an idle session. A %Wt
      application uses a keep-alive messages to keep the session alive as
      long as the user is visiting the page. Increasing this number will
      result in a longer time between keep-alive message, resulting in a
      lower server load, but at the same time will detect a dead session
      with a longer delay.</dd>
    <dt><strong>max-request-size</strong></dt>
    <dd>The maximum HTTP request size (Kb) that is accepted. An oversized
      request will result in a Wt::WApplication::requestTooLarge signal.
      </dd>
    <dt><strong>enable-debug</strong></dt>
    <dd>When debugging is enabled, appending a debug to the initial query
      will enable debug information.</dd>
    <dt><strong>valgrind-path</strong></dt>
    <dd>Set the path to valgrind for debugging using valgrind.</dd>
    <dt><strong>run-directory</strong></dt>
    <dd>The path that is used by the library for managing sessions.</dd>
    <dt><strong>session-id-length</strong></dt>
    <dd>The length (in number of characters) for the unique session ID.</dd>
  </dl>

 *
 * You can now proceed to the \ref example
 */

/*! \page example Treelist example
 *
 * In this example we will step through the code of the <a
 * href="http://jose.med.kuleuven.be/wt/examples/treelist/demotreelist.fcg">
 * Tree List example</a>. The source code of the entire example is
 * available as leafs of the tree. Note that %Wt offers a Tree List
 * widget as part of the library (see Wt::WTreeNode), of which this
 * example is a down-stripped version.
 *
 * The example in particular demonstrates the use of stateless slot
 * learning to simultaneously implement client-side and server-side
 * event handling in C++.
 *
 * The tree constructed as hierarchy of tree nodes. A single tree node
 * is implemented in the class TreeNode. TreeNode uses the helper
 * class IconPair for rendering icons that have a state (such as the
 * expand/collapse icons). We start with a walk-over of this class.
 *
 * \section stateicon_sec IconPair: a pair of icons that reflects state.
 *
 * For the implementation of the tree list expand/collapse icons, as
 * well as the label icons (such as the folder icon), we use class
 * IconPair. It takes a pair of icons and shows only one at a
 * time. Passing clickIsSwitch = true to the constructor will make the
 * icon react to click events to switch the current icon.
 *
 * This is the class definition of IconPair:
 *
 * \dontinclude examples/treelist/IconPair.h
 * \skip IconPair
 * \until };
 *
 * IconPair is a composite widget, implemented as a Wt::WContainerWidget
 * which contains two Wt::WImage objects. The class defines two slots:
 * IconPair::showIcon1() and IconPair::showIcon2(), which show the
 * respective icon, while hiding the other icon.

 * Although %Wt is a C++ (server-side) library, it can also generate
 * client-side JavaScript code for instant visual response. This
 * example will use this capability to implement all of the tree
 * navigation at the client-side for those clients that support
 * JavaScript -- as if it were implemented as a JavaScript
 * library. But since everything is still plain C++ code, it works
 * whatever technology is available or lacking at the client
 * side. Think of a stateless slot implementation as creating a forked
 * implementation, with JavaScript in the client for visual response
 * -- when JavaScript is available, and C++ at the server. When no
 * JavaScript is available, everything happens at the server.
 *
 * The key concept behind %Wt's capability to implement things at the
 * client-side is stateless slot implementations. A stateless slot is,
 * besides a normal C++ function that may be connected to a signal, a
 * C++ function that promises to always have the same behaviour (until
 * it is reset, as we will see later).
 *
 * This applies to the two functions showIcon1() and showIcon2(), as
 * they simply set the corresponding icon, irrespective of any
 * application state. The library offers two methods for stateless
 * slot implementations: AutoLearned and PreLearned. An AutoLearned
 * stateless slot will only "become client-side" after the first
 * invocation. Applied to our tree widget, this would mean that the
 * first click on any icon would require a round-trip to the server
 * the first time only. An AutoLearned stateless slot simply requires
 * an indication that the particular slot confirms to the contract of
 * being stateless. A PreLearned stateless slot, on the other hand, is
 * "client-side" from the first invocation. To implement a PreLearned
 * stateless however, we need to do some extra work by providing
 * methods that exactly undo the effect of the slot. We provide here
 * two such undo methods: undoShowIcon1() and undoShowIcon2().
 *
 * Enough talk! Let's look at the implementation, starting with
 * the constructor.
 *
 * \dontinclude examples/treelist/IconPair.C
 * \skip IconPair::
 * \until {
 *
 * IconPair inherits from Wt::WCompositeWidget. A composite widget is
 * a widget which is composed from other widgets, in a way not exposed
 * in its API. In this way, you may later change the implementation
 * without any problem.
 *
 * Notice how we constructed three widgets that are used in the
 * implementation: two images (icon1_ and icon2_), and a container
 * (impl_) to hold them. The images are added to the container by
 * passing the container as the last argument in their constructor.
 *
 * Wt::WCompositeWidget requires to set the implementation widget,
 * which is in our case a Wt::WContainerWidget:
 *
 * \line setImplementation
 *
 * We declare the slots showIcon1() and showIcon2() as stateless
 * slots, allowing for client-side optimisation, and offer an undo
 * function which facilitates a PreLearned client-side implementation.
 *
 * The calls to Wt::WObject::implementStateless() state that the slots
 * showIcon1() and showIcon2() are stateless slots, and their visual
 * effect may be learned in advance. The effect of these statements is
 * merely an optimization. Any non-visual effects of these slots are
 * still propagated and executed, as expected.
 *
 * \until undoShowIcon2
 *
 * Next, we declare the widget to be an inline widget. An inline
 * widget will be layed out following the natural flow of text (left
 * to right).  This does not really matter for our example, since
 * TreeNode will do the layout with a Wt::WTable, but we do so to
 * provide consistency with a Wt::WImage which is also inline by
 * default.
 *
 * \line setInline 
 *
 * The initial state is to show the first icon:
 *
 * \line hide
 *
 * To react to click events, we connect signals with slots:
 *
 * \until ; //
 *
 * We change the cursor to a pointer to hint that clicking these icons
 * may do something useful.
 *
 * \until } //
 *
 * We also change the cursor to a pointer to
 * hint that clicking these icons will in fact perform an action.
 *
 * The rest of the class definition is:
 *
 * \until } //
 *
 * Note the implementations of undoShowIcon1() and undoShowIcon2():
 * they simply, but accurately, reset the state to what it was before
 * the respective showIcon1() and showIcon2() calls.
 *
 * \section treenode_sec TreeNode: an expandable tree node.
 *
 * TreeNode contains the implementation of the tree, as a hierarchy of
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
 * The TreeNode manages a list of child nodes in a WContainerWidget
 * which will be hidden and shown when the node is expanded or
 * collapsed, and children are collapsed when the node is expanded.
 *
 * This is the TreeNode class definition:
 *
 * \dontinclude examples/treelist/TreeNode.h
 * \skip class TreeNode
 * \until }; //
 *
 * The public interface of the TreeNode provides methods to manage
 * its children, and two public slots to expand or collapse the node.
 * Remember, a slot is nothing more than a method (and the public slots:
 * does not actually mean anything, except providing a hint to the user
 * of this class that these methods are made to be connected to signals).
 *
 * We start with the implementation of the constructor:
 *
 * \dontinclude examples/treelist/TreeNode.C
 * \skip TreeNode::TreeNode
 * \until {
 *
 * We start with declaring stateless implementations for the slots.
 * It is good practice to do this first, since it must be done before
 * any connections are made to the slots.
 *
 * \until undoCollapse
 *
 * We will implement the treenode as 2 by 2 table.
 *
 * \skipline setImplementation
 *
 * We create all icons. Since currently the node is empty, we only
 * show the no-expand version (which is simply a horizontal line).
 *
 * \until noExpandIcon
 *
 * The expanded content is a WContainerWidget.
 *
 * \until hide
 *
 * We create the label and child count text widgets:
 *
 * \until treenodechildcount
 *
 * Now we add all widgets in the proper table cell, and set the correct
 * alignment.
 *
 * \until setContentAlignment(AlignMiddle)
 *
 * Finally, we connect the click events of the expandIcon to the expand
 * and collapse slots.
 *
 * \until } //
 *
 * WTable::elementAt(int row, int column) is used repeatedly to add or
 * modify contents of the table cells, expanding the table geometry as
 * needed. Finally, we make connections from the expand and collapse
 * icons to the slots we define in the TreeNode class.
 *
 * Again, we optimize the visual effect of expand() and collaps() in
 * client-side JavaScript, which is possible since they both have an
 * effect independent of application state. Typically, one will start
 * with a default dynamic slot implementation, and indicate stateless
 * implementations where desired and possible, using one of the two
 * mechanisms of stateless slot learning.
 *
 * The "business logic" of the TreeNode is simply to manage its children.
 * Whenever a child is added or removed, adjustments to its look are
 * updated by calling childNodesChanged().
 *
 * \until } //
 *
 * The expand icon of the last child is rendered differently, as it
 * needs to terminate the vertical guide line. To keep the
 * implementation simple, we simply let every child reset its proper
 * look by calling adjustExpandIcon().
 *
 * \until adjustExpandIcon
 *
 * When getting a first child, or losing the last child, the expand icon
 * changes too.
 *
 * \until adjustExpandIcon
 *
 * We also update the childCount label.
 *
 * \until setText(L"")
 *
 * Finally, we call WObject::resetLearnedSlots(). Because the expand()
 * slot depends on the number of children, because it needs to
 * collapse all children -- this slot is not entirely stateless,
 * breaking the contract for a stateless slot. However, we can get
 * away with still implementing as a stateless slot, by indicating
 * when the state has changed.
 *
 * \until } //
 *
 * The implementation of the collapse slot is as follows:
 *
 * \until {
 *
 * First we record the current state, so the undo method can exactly undo
 * what happened.
 *
 * \until isHidden()
 *
 * Next, we implement the actual collapse logic:
 *
 * \until } //
 *
 * Similarly, the implementation of the expand slot. However, in this
 * case we need to collapse all children as well.
 *
 * \until } //
 *
 * Since we implement these slots as prelearned stateless slots, we
 * also need to define the undo functions. Note that Because expand()
 * also collapses all child nodes, the undo function of expand() is
 * not simply collapse() and vice-versa.
 *
 * \until } //
 *
 * Finally, the adjustExpandIcon() function sets the correct images,
 * which depends on how the node relates to its siblings. The last
 * node looks a bit different.
 *
 * \until {
 *
 * \line 
 *
 * We set the expand icon images:
 *
 * \until (imageLine
 *
 * Then, we set the vertical guide line if not the last child, and nothing
 * if the last child:
 *
 * \until } //
 *
 * Finally, we select the correct icon, depending on whether the node
 * has children:
 *
 * \until } //
 *
 * And that's it. By using the TreeNode class in a hierarchy, we can
 * create a tree widget. The tree widget will be implemented entirely
 * in JavaScript, if available, and otherwise as plain HTML. In any case,
 * client-side and server-side state are completely synchronized, and
 * identical by definition since they are derived from the same C++ code.
 *
 * You can now proceed to \ref Installation
 */

/*! \page Installation
 *
 * \htmlinclude INSTALL.html
 *
 */

/*! \page Releasenotes
 *
 * \htmlinclude ReleaseNotes.html
 *
 */

}
