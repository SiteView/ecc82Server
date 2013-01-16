/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <boost/lexical_cast.hpp>
#include <iostream>

#include "WTable"
#include "WTableCell"
#include "WImage"
#include "WText"
#include "WCssDecorationStyle"
#include "WTreeNode"
#include "WIconPair"

using std::find;

namespace Wt {

std::string WTreeNode::imageLine_[] = { "line-middle.gif",
					"line-last.gif" };
std::string WTreeNode::imagePlus_[] = { "nav-plus-line-middle.gif",
					"nav-plus-line-last.gif" };
std::string WTreeNode::imageMin_[] = { "nav-minus-line-middle.gif",
				       "nav-minus-line-last.gif" };

WTreeNode::WTreeNode(const WMessage& labelText,
		     WIconPair *labelIcon, WTreeNode *parent)
  : collapsed_(true),
    parentNode_(0),
    labelIcon_(labelIcon),
    labelText_(new WText(labelText)),
    childrenLoaded_(false),
    populated_(false)
{
  create();

  if (parent)
    parent->addChildNode(this);
}

WTreeNode::WTreeNode(WTreeNode *parent)
  : collapsed_(true),
    parentNode_(0),
    labelIcon_(0),
    labelText_(0),
    childrenLoaded_(false)
{
  create();

  if (parent)
    parent->addChildNode(this);
}

WTableCell *WTreeNode::labelArea()
{
  return layout_->elementAt(0, 1);
}

WTreeNode::~WTreeNode()
{
  for (unsigned i = 0; i < childNodes_.size(); ++i) {
    delete childNodes_[i];
  }

  // also delete these two as only one of them is inserted in the
  // widget hierarchy at any time
  delete noExpandIcon_;
  delete expandIcon_;
}

void WTreeNode::populate()
{ 
  populated_ = true;
}

void WTreeNode::setImagePack(const std::string url)
{
  imagePackUrl_ = url;
  updateChildren(true);
}

std::string WTreeNode::imagePack() const
{
  if (!imagePackUrl_.empty())
    return imagePackUrl_;
  else
    if (parentNode_)
      return parentNode_->imagePack();
    else
      return "";
}

void WTreeNode::create()
{
  setImplementation(layout_ = new WTable());

  implementStateless(&WTreeNode::doExpand, &WTreeNode::undoDoExpand);
  implementStateless(&WTreeNode::doCollapse, &WTreeNode::undoDoCollapse);

  expandIcon_ = new WIconPair(imagePlus_[Last], imageMin_[Last]);
  noExpandIcon_ = new WImage(imageLine_[Last]);

  expandedContent_ = new WContainerWidget();
  expandedContent_->hide();

  if (labelText_) {
    labelText_->setStyleClass(L"treenodelabel");
    childCountLabel_ = new WText();
    childCountLabel_->setMargin(WLength(7), Left);
    childCountLabel_->setStyleClass(L"treenodechildcount");
  } else
    childCountLabel_ = 0;

  layout_->elementAt(0, 0)->addWidget(noExpandIcon_);

  if (labelIcon_) {
    layout_->elementAt(0, 1)->addWidget(labelIcon_);
    labelIcon_->setVerticalAlignment(AlignMiddle);
  }

  if (labelText_)
    layout_->elementAt(0, 1)->addWidget(labelText_);

  if (childCountLabel_)
    layout_->elementAt(0, 1)->addWidget(childCountLabel_);

  layout_->elementAt(1, 1)->addWidget(expandedContent_);

  layout_->elementAt(0, 0)->setContentAlignment(AlignTop);
  layout_->elementAt(0, 1)->setContentAlignment(AlignMiddle);

  childrenLoaded_ = false;

  setLoadPolicy(LazyLoading);
}

void WTreeNode::setLoadPolicy(LoadPolicy loadPolicy)
{
  loadPolicy_ = loadPolicy;

  switch (loadPolicy) {
  case PreLoading:
    loadChildren();

    break;
  case NextLevelLoading:
    if (expanded()) {
      loadChildren();
      loadGrandChildren();
    } else {
      if (parentNode_ && parentNode_->expanded())
	loadChildren();
      expandIcon_
	->icon1Clicked.connect(SLOT(this, WTreeNode::loadGrandChildren));
    }
    break;
  case LazyLoading:
    if (expanded())
      loadChildren();
    else {
      if (parentNode_ && parentNode_->expanded()) {
	if (!populated_) {
	  populate();
	  populated_= true;
	}
      }

      expandIcon_->icon1Clicked.connect(SLOT(this, WTreeNode::expand));
    }
  }

  if (loadPolicy_ != LazyLoading) {
    for (unsigned i = 0; i < childNodes_.size(); ++i)
      childNodes_[i]->setLoadPolicy(loadPolicy_);
  }
}

void WTreeNode::loadChildren()
{
  if (!childrenLoaded_) {
    if (!populated_) {
      populate();
      populated_ = true;
    }

    for (unsigned i = 0; i < childNodes_.size(); ++i) {
      expandedContent_->addWidget(childNodes_[i]);
      if (!childNodes_[i]->populated_) {
	childNodes_[i]->populate();
	childNodes_[i]->populated_ = true;
      }
    }

    expandIcon_->icon1Clicked.connect(SLOT(this, WTreeNode::doExpand));
    expandIcon_->icon2Clicked.connect(SLOT(this, WTreeNode::doCollapse));

    resetLearnedSlots();

    childrenLoaded_ = true;
  }
}

void WTreeNode::loadGrandChildren()
{
  for (unsigned i = 0; i < childNodes_.size(); ++i)
    childNodes_[i]->loadChildren();
}

bool WTreeNode::isLastChildNode() const
{
  if (parentNode_) {
    return parentNode_->childNodes_.back() == this;
  } else
    return true;
}

void WTreeNode::addChildNode(WTreeNode *node)
{
  childNodes_.push_back(node);
  node->parentNode_ = this;
  if (loadPolicy_ != node->loadPolicy_)
    node->setLoadPolicy(loadPolicy_);

  if (childrenLoaded_) {
    expandedContent_->addWidget(node);
    if (!node->populated_) {
      node->populate();
      node->populated_ = true;
    }
  }

  updateChildren();
}

void WTreeNode::removeChildNode(WTreeNode *node)
{
  childNodes_.erase(find(childNodes_.begin(), childNodes_.end(), node));

  node->parentNode_ = 0;

  if (childrenLoaded_)
    expandedContent_->removeWidget(node);

  updateChildren();
}

void WTreeNode::updateChildren(bool recursive)
{
  for (unsigned i = 0; i < childNodes_.size(); ++i)
    if (recursive)
      childNodes_[i]->updateChildren(recursive);
    else
      childNodes_[i]->update();

  update();

  if (childCountLabel_) {
    if (childNodes_.size())
      childCountLabel_
	->setText(L"("
		  + boost::lexical_cast<std::wstring>(childNodes_.size())
		  + L")");
    else
      childCountLabel_->setText(L"");
  }

  resetLearnedSlots();
}

bool WTreeNode::expanded() const
{
  return !collapsed_;
}

void WTreeNode::expand()
{
  if (!expanded()) {
    if (!childrenLoaded_) {
      loadChildren();
    }

    if (loadPolicy_ == NextLevelLoading)
      loadGrandChildren();

    doExpand();
  }
}

void WTreeNode::collapse()
{
  if (expanded())
    doCollapse();
}

void WTreeNode::doExpand()
{
  wasCollapsed_ = !expanded();
  collapsed_ = false;

  if (childNodes_.size() != 0) {
    expandIcon_->setState(1);
    expandedContent_->show();

    if (labelIcon_)
      labelIcon_->setState(1);
  }

  /*
   * collapse all children
   */
  for (unsigned i = 0; i < childNodes_.size(); ++i)
    childNodes_[i]->doCollapse();
}

void WTreeNode::doCollapse()
{
  wasCollapsed_ = !expanded();
  collapsed_ = true;

  // there is nothing to do if it cannot be expanded in the first place
  if (childNodes_.size() != 0) {
    expandIcon_->setState(0);
    expandedContent_->hide();

    if (labelIcon_)
      labelIcon_->setState(0);
  }
}

void WTreeNode::undoDoCollapse()
{
  if (!wasCollapsed_) {
    // re-expand
    expandIcon_->setState(1);
    expandedContent_->show();
    if (labelIcon_)
      labelIcon_->setState(1);
    collapsed_ = false;
  }
}

void WTreeNode::undoDoExpand()
{
  if (wasCollapsed_) {
    // re-collapse
    expandIcon_->setState(0);
    expandedContent_->hide();
    if (labelIcon_)
      labelIcon_->setState(0);

    collapsed_ = true;
  }

  /*
   * undo collapse of children
   */
  for (unsigned i = 0; i < childNodes_.size(); ++i)
    childNodes_[i]->undoDoCollapse();  
}

void WTreeNode::setLabelIcon(WIconPair *labelIcon)
{
  if (labelIcon_)
    delete labelIcon_;
  labelIcon_ = labelIcon;

  if (labelIcon_) {
    if (labelText_)
      layout_->elementAt(0, 1)->insertWidget(labelIcon_, labelText_);
    else
      layout_->elementAt(0, 1)->addWidget(labelIcon_);

    labelIcon_->setState(expanded() ? 1 : 0);
  }
}

void WTreeNode::update()
{
  ImageIndex index = isLastChildNode() ? Last : Middle;
  
  if (expandIcon_->state() != (expanded() ? 1 : 0))
    expandIcon_->setState(expanded() ? 1 : 0);
  if (expandedContent_->isHidden() != !expanded())
    expandedContent_->setHidden(!expanded());
  if (labelIcon_ && (labelIcon_->state() != (expanded() ? 1 : 0)))
    labelIcon_->setState(expanded() ? 1 : 0);

  if (expandIcon_->icon1()->imageRef() != imagePack() + imagePlus_[index])
    expandIcon_->icon1()->setImageRef(imagePack() + imagePlus_[index]);
  if (expandIcon_->icon2()->imageRef() != imagePack() + imageMin_[index])
    expandIcon_->icon2()->setImageRef(imagePack() + imageMin_[index]);
  if (noExpandIcon_->imageRef() != imagePack() + imageLine_[index])
    noExpandIcon_->setImageRef(imagePack() + imageLine_[index]);

  if (index == Last) {
    layout_->elementAt(0, 0)
      ->decorationStyle().setBackgroundImage("");
    layout_->elementAt(1, 0)
      ->decorationStyle().setBackgroundImage("");
  } else {
    layout_->elementAt(0, 0)
      ->decorationStyle().setBackgroundImage(imagePack() + "line-trunk.gif",
					     WCssDecorationStyle::RepeatY);
    layout_->elementAt(1, 0)
      ->decorationStyle().setBackgroundImage(imagePack() + "line-trunk.gif",
					     WCssDecorationStyle::RepeatY);
  }

  if (childNodes_.empty()) {
    if (noExpandIcon_->parent() == 0) {
      layout_->elementAt(0, 0)->addWidget(noExpandIcon_);
      layout_->elementAt(0, 0)->removeWidget(expandIcon_);
    }
  } else {
    if (expandIcon_->parent() == 0) {
      layout_->elementAt(0, 0)->addWidget(expandIcon_);
      layout_->elementAt(0, 0)->removeWidget(noExpandIcon_);
    }
  }
}

}
