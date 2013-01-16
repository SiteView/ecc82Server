/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <boost/lexical_cast.hpp>

#include <WBreak>
#include <WImage>
#include <WLabel>
#include <WGroupBox>
#include <WLineEdit>
#include <WPushButton>
#include <WText>
#include <WValidationStatus>

#include "TreeListExample.h"
#include "TreeNode.h"
#include "IconPair.h"

TreeListExample::TreeListExample(WContainerWidget *parent)
  : WContainerWidget(parent),
    testCount_(0)
{
  tree_ = makeTreeMap(L"TreeListExample", 0);
  addWidget(tree_);
  tree_->expand();

  TreeNode *treelist = makeTreeMap(L"Tree List", tree_);
  TreeNode *wstateicon = makeTreeMap(L"class IconPair", treelist);
  makeTreeFile(L"IconPair.h", wstateicon);
  makeTreeFile(L"IconPair.C", wstateicon);
  TreeNode *wtreenode = makeTreeMap(L"class TreeNode", treelist);
  makeTreeFile(L"TreeNode.h", wtreenode);
  makeTreeFile(L"TreeNode.C", wtreenode);
  TreeNode *wtreeexample = makeTreeMap(L"class TreeListExample", treelist);
  makeTreeFile(L"TreeListExample.h", wtreeexample);
  makeTreeFile(L"TreeListExample.C", wtreeexample);

  testMap_ = makeTreeMap(L"Test map", tree_);

  /*
   * Buttons to demonstrate dynamically changing the tree contents
   * implies no magic at all.
   */
  addWidget
    (new WText(L"<p>Use the following buttons to change the "
	       "contents of the Test map:</p>"));

  WGroupBox *addBox = new WGroupBox(L"Add map", this);

  WLabel *mapNameLabel = new WLabel(L"Map name:", addBox);
  mapNameLabel->setMargin(WLength(1, WLength::FontEx), Right);
  mapNameEdit_ = new WLineEdit(L"Map", addBox);
  mapNameLabel->setBuddy(mapNameEdit_);

  /*
   * Example of validation: make the map name mandatory, and give
   * feedback when invalid.
   */
  mapNameEdit_->setValidator(new WValidator(true));
  WImage *invalid = new WImage("icons/invalid.png");
  WImage *invalidEmpty = new WImage("icons/invalid.png");

  WValidationStatus *statusIcon
    = new WValidationStatus(mapNameEdit_, 0, invalid, invalidEmpty, addBox);
  statusIcon->setMargin(WLength(5), Left | Right);

  addMapButton_ = new WPushButton(L"Add map", addBox);
  addMapButton_->clicked.connect(SLOT(this, TreeListExample::addMap));

  statusIcon->validated.connect(SLOT(addMapButton_, WFormWidget::setEnabled));

  new WBreak(this);

  WGroupBox *removeBox = new WGroupBox(L"Remove map", this);

  removeMapButton_
    = new WPushButton(L"Remove map", removeBox);
  removeMapButton_->clicked.connect(SLOT(this, TreeListExample::removeMap));
  removeMapButton_->disable();
}

void TreeListExample::addMap()
{
  TreeNode *node
    = makeTreeMap(mapNameEdit_->text() + L" "
		  + boost::lexical_cast<std::wstring>(++testCount_),
		  testMap_);
  makeTreeFile(L"File " + boost::lexical_cast<std::wstring>(testCount_),
	       node);

  removeMapButton_->enable();
}

void TreeListExample::removeMap()
{
  int numMaps = testMap_->childNodes().size();

  if (numMaps > 0) {
    int c = rand() % numMaps;

    TreeNode *child = testMap_->childNodes()[c];
    testMap_->removeChildNode(child);
    delete child;

    if (numMaps == 1)
      removeMapButton_->disable();
  }
}

TreeNode *TreeListExample::makeTreeMap(const std::wstring name,
					TreeNode *parent)
{
  IconPair *labelIcon
    = new IconPair("icons/yellow-folder-closed.png",
		   "icons/yellow-folder-open.png",
		   false);

  TreeNode *node = new TreeNode(name, WText::PlainFormatting, labelIcon, 0);
  if (parent)
    parent->addChildNode(node);

  return node;
}

TreeNode *TreeListExample::makeTreeFile(const std::wstring name,
					TreeNode *parent)
{
  IconPair *labelIcon
    = new IconPair("icons/document.png", "icons/yellow-folder-open.png",
		   false);

  TreeNode *node = new TreeNode(L"<a href=\"" + name + L"\" TARGET=\"_blank\">"
				+ name + L"</a>", WText::XHMTLFormatting,
				labelIcon, 0);
  if (parent)
    parent->addChildNode(node);

  return node;
}
